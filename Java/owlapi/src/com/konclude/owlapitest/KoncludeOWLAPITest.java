/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

package com.konclude.owlapitest;

import com.konclude.owlapi.KoncludeOWLAPITranslator;
import com.konclude.owlapi.KoncludeReasoner;
import com.konclude.owlapi.KoncludeReasonerFactory;

import org.semanticweb.owlapi.apibinding.OWLManager;
import org.semanticweb.owlapi.model.*;
import org.semanticweb.owlapi.reasoner.*;
import org.semanticweb.owlapi.vocab.OWL2Datatype;
import org.semanticweb.owlapi.vocab.OWLFacet;

import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;
import java.util.TreeSet;

/**
 * Drives the OWL API wrapper of Konclude against the shared library, the counterpart of
 * com.konclude.smoketest.KoncludeJNISmokeTest one layer higher.
 *
 * Every scenario is selected by its name and is supposed to be started in its own virtual
 * machine, see Java/run-owlapi-test.sh. The exit code is 0 if the scenario succeeded.
 *
 *   hierarchy      the class hierarchy, direct and indirect, and the top and bottom node
 *   expressions    the questions about an anonymous class expression, as the DL query tab
 *                  of Protege asks them, and the handling of a fresh entity in one
 *   individuals    the types, the instances, the same individuals and the property values
 *   properties     the object property hierarchy and the same individuals
 *   datatypes      the sub data properties and the datatype restrictions
 *   inconsistency  an inconsistent ontology and an unsatisfiable class
 *   unsupported    every question that the bridge cannot answer is reported as unsupported
 *   lifecycle      several reasoners in one virtual machine, and flush after a change
 *   merges         an ABox that forces two individuals to be merged, which Konclude used to
 *                  stay in its precomputation for
 *   timeout        a call that overruns the configured time out is reported as a
 *                  TimeOutException instead of blocking the caller for ever
 */
public class KoncludeOWLAPITest {

	private static final String NS = "http://konclude.com/test/owlapi#";

	private static final OWLDataFactory DF = OWLManager.getOWLDataFactory();

	private static int sFailures = 0;

	private static OWLClass cls(String name) {
		return DF.getOWLClass(IRI.create(NS + name));
	}

	private static OWLObjectProperty objectProperty(String name) {
		return DF.getOWLObjectProperty(IRI.create(NS + name));
	}

	private static OWLDataProperty dataProperty(String name) {
		return DF.getOWLDataProperty(IRI.create(NS + name));
	}

	private static OWLNamedIndividual individual(String name) {
		return DF.getOWLNamedIndividual(IRI.create(NS + name));
	}


	// ------------------------------------------------------------- the checking

	private static void report(String message) {
		System.out.println("  " + message);
	}

	private static Set<String> expected(String... names) {
		return new TreeSet<String>(Arrays.asList(names));
	}

	/** the local names of the entities of a node set, without the grouping into nodes */
	private static Set<String> names(NodeSet<? extends OWLObject> nodeSet) {
		return localNames(nodeSet.getFlattened());
	}

	private static Set<String> names(Node<? extends OWLObject> node) {
		return localNames(node.getEntities());
	}

	/**
	 * A property node holds OWLObjectPropertyExpressions, which are not OWLEntities, so an
	 * anonymous expression is reported as it prints itself.
	 */
	private static Set<String> localNames(Set<? extends OWLObject> objects) {
		Set<String> names = new TreeSet<String>();
		for (OWLObject object : objects) {
			if (object instanceof OWLEntity) {
				IRI iri = ((OWLEntity) object).getIRI();
				String fragment = iri.getFragment();
				names.add(fragment != null && fragment.length() > 0 ? fragment : iri.toString());
			} else {
				names.add(object.toString());
			}
		}
		return names;
	}

	private static void check(String label, Object actual, Object expectedValue) {
		boolean ok = actual == null ? expectedValue == null : actual.equals(expectedValue);
		report(label + " = " + actual + (ok ? "" : ", EXPECTED " + expectedValue));
		if (!ok) {
			++sFailures;
		}
	}

	/**
	 * For an answer that the reasoning engine is known to get wrong, see Java/Readme.md. It is
	 * reported but does not fail the scenario, and a correct answer is reported as fixed, so
	 * that the scenario shows when the engine changes.
	 */
	private static void checkEngineDefect(String label, Object actual, Object expectedValue) {
		boolean ok = actual == null ? expectedValue == null : actual.equals(expectedValue);
		report(label + " = " + actual + (ok ? ", engine defect FIXED" : ", known engine defect, correct is "
				+ expectedValue));
	}

	/** for the answers that only have to contain something, the top node for instance */
	private static void checkContains(String label, Set<String> actual, String... required) {
		Set<String> missing = new TreeSet<String>(Arrays.asList(required));
		missing.removeAll(actual);
		report(label + " = " + actual + (missing.isEmpty() ? "" : ", MISSING " + missing));
		if (!missing.isEmpty()) {
			++sFailures;
		}
	}

	private static void checkThrowsUnsupported(String label, Runnable call) {
		checkThrows(label, UnsupportedOperationException.class, call);
	}

	private static void checkThrows(String label, Class<? extends Throwable> expected, Runnable call) {
		try {
			call.run();
			report(label + " did NOT throw, expected " + expected.getSimpleName());
			++sFailures;
		} catch (RuntimeException exception) {
			if (expected.isInstance(exception)) {
				report(label + " -> " + expected.getSimpleName());
			} else {
				report(label + " threw " + exception + ", expected " + expected.getSimpleName());
				++sFailures;
			}
		}
	}


	// ------------------------------------------------------------ the ontologies

	/**
	 * A small family ontology that exercises the constructs the bridge supports: a class
	 * hierarchy through equivalences and existential restrictions, disjointness, an object
	 * property hierarchy with an inverse, and an ABox with a sameAs.
	 */
	private static OWLOntology familyOntology() throws OWLOntologyCreationException {
		OWLOntologyManager manager = OWLManager.createOWLOntologyManager();
		OWLOntology ontology = manager.createOntology(IRI.create(NS));

		OWLObjectProperty hasChild = objectProperty("hasChild");
		OWLObjectProperty hasSon = objectProperty("hasSon");
		OWLObjectProperty hasParent = objectProperty("hasParent");

		manager.addAxioms(ontology, new HashSet<OWLAxiom>(Arrays.<OWLAxiom>asList(
			DF.getOWLSubClassOfAxiom(cls("Man"), cls("Person")),
			DF.getOWLSubClassOfAxiom(cls("Woman"), cls("Person")),
			DF.getOWLDisjointClassesAxiom(cls("Man"), cls("Woman")),
			DF.getOWLEquivalentClassesAxiom(cls("Parent"),
				DF.getOWLObjectIntersectionOf(cls("Person"),
					DF.getOWLObjectSomeValuesFrom(hasChild, cls("Person")))),
			DF.getOWLEquivalentClassesAxiom(cls("Father"),
				DF.getOWLObjectIntersectionOf(cls("Man"), cls("Parent"))),
			DF.getOWLEquivalentClassesAxiom(cls("Mother"),
				DF.getOWLObjectIntersectionOf(cls("Woman"), cls("Parent"))),
			DF.getOWLEquivalentClassesAxiom(cls("Grandparent"),
				DF.getOWLObjectIntersectionOf(cls("Person"),
					DF.getOWLObjectSomeValuesFrom(hasChild, cls("Parent")))),
			// unsatisfiable, so that the bottom node has something in it
			DF.getOWLEquivalentClassesAxiom(cls("Impossible"),
				DF.getOWLObjectIntersectionOf(cls("Man"), cls("Woman"))),

			DF.getOWLSubObjectPropertyOfAxiom(hasSon, hasChild),
			DF.getOWLInverseObjectPropertiesAxiom(hasParent, hasChild),
			DF.getOWLObjectPropertyDomainAxiom(hasChild, cls("Person")),
			DF.getOWLObjectPropertyRangeAxiom(hasChild, cls("Person")),

			DF.getOWLClassAssertionAxiom(cls("Man"), individual("john")),
			DF.getOWLClassAssertionAxiom(cls("Woman"), individual("mary")),
			DF.getOWLObjectPropertyAssertionAxiom(hasChild, individual("john"), individual("mary")),
			DF.getOWLSameIndividualAxiom(individual("john"), individual("johnny")),
			// dropped by the translator, it must not disturb anything
			DF.getOWLAnnotationAssertionAxiom(DF.getRDFSLabel(), cls("Person").getIRI(),
				DF.getOWLLiteral("a person"))
		)));
		return ontology;
	}

	/**
	 * The loading configuration the scenarios create their reasoners with, empty for the default.
	 * The 'expressions-tableau' scenario switches the decision of the sub classes of an expression
	 * from the saturation off, so that the answers of the tableau path stay tested as well.
	 */
	private static String sLoadingConfiguration = "";

	private static OWLReasoner reasonerFor(OWLOntology ontology) {
		return new KoncludeReasonerFactory(sLoadingConfiguration).createReasoner(ontology);
	}


	// -------------------------------------------------------------- the scenarios

	private static void runHierarchy() throws Exception {
		OWLOntology ontology = familyOntology();
		OWLReasoner reasoner = reasonerFor(ontology);
		try {
			check("consistent", Boolean.valueOf(reasoner.isConsistent()), Boolean.TRUE);

			check("subClasses(Person, direct=false)", names(reasoner.getSubClasses(cls("Person"), false)),
					expected("Father", "Grandparent", "Impossible", "Man", "Mother", "Parent", "Woman", "Nothing"));
			check("subClasses(Person, direct=true) ", names(reasoner.getSubClasses(cls("Person"), true)),
					expected("Man", "Parent", "Woman"));
			check("superClasses(Father, false)    ", names(reasoner.getSuperClasses(cls("Father"), false)),
					expected("Man", "Parent", "Person", "Thing"));
			check("superClasses(Father, true)     ", names(reasoner.getSuperClasses(cls("Father"), true)),
					expected("Man", "Parent"));
			check("equivalentClasses(Father)      ", names(reasoner.getEquivalentClasses(cls("Father"))),
					expected("Father"));

			checkContains("topClassNode                   ", names(reasoner.getTopClassNode()), "Thing");
			checkContains("bottomClassNode                ", names(reasoner.getBottomClassNode()),
					"Nothing", "Impossible");
			check("isSatisfiable(Person)          ", Boolean.valueOf(reasoner.isSatisfiable(cls("Person"))),
					Boolean.TRUE);
			check("isSatisfiable(Impossible)      ", Boolean.valueOf(reasoner.isSatisfiable(cls("Impossible"))),
					Boolean.FALSE);
			// the bottom node is not a direct sub class of owl:Thing beside the roots, and owl:Thing
			// is not a direct super class of it beside the leaves; the taxonomy used to keep the
			// link between the two that an empty taxonomy starts with, which is how unsatisfiable
			// classes came to be shown under owl:Thing in Protege
			check("subClasses(Thing, direct=true)  ", names(reasoner.getSubClasses(DF.getOWLThing(), true)),
					expected("Person"));
			check("superClasses(Nothing, direct=true)", names(reasoner.getSuperClasses(DF.getOWLNothing(), true)),
					expected("Father", "Grandparent", "Mother"));
		} finally {
			reasoner.dispose();
		}
	}

	/**
	 * The questions about anonymous class expressions, which the DL query tab of Protege asks
	 * and which used to be unsupported, since the bridge identified a class by its IRI. The
	 * expressions are chosen so that the family ontology decides them: hasChild has Person as
	 * its domain and range, so 'hasChild some Person' is equivalent to Parent.
	 */
	private static void runExpressions() throws Exception {
		OWLOntology ontology = familyOntology();
		OWLObjectProperty hasChild = objectProperty("hasChild");
		OWLClassExpression hasChildPerson = DF.getOWLObjectSomeValuesFrom(hasChild, cls("Person"));
		OWLClassExpression manWithChild = DF.getOWLObjectIntersectionOf(cls("Man"), hasChildPerson);
		OWLClassExpression notMan = DF.getOWLObjectComplementOf(cls("Man"));
		OWLClassExpression manAndWoman = DF.getOWLObjectIntersectionOf(cls("Man"), cls("Woman"));
		OWLClassExpression womanOrParent = DF.getOWLObjectUnionOf(cls("Woman"), cls("Parent"));
		OWLClassExpression hasChildWoman = DF.getOWLObjectSomeValuesFrom(hasChild, cls("Woman"));
		final OWLClassExpression hasChildFresh = DF.getOWLObjectSomeValuesFrom(hasChild, cls("Fresh"));

		final OWLReasoner reasoner = reasonerFor(ontology);
		try {
			check("equivalentClasses(hasChild some Person)", names(reasoner.getEquivalentClasses(hasChildPerson)),
					expected("Parent"));
			check("subClasses(hasChild some Person, true) ", names(reasoner.getSubClasses(hasChildPerson, true)),
					expected("Father", "Grandparent", "Mother"));
			checkContains("subClasses(hasChild some Person, false)", names(reasoner.getSubClasses(hasChildPerson, false)),
					"Father", "Grandparent", "Mother", "Impossible");
			check("superClasses(hasChild some Person, true)", names(reasoner.getSuperClasses(hasChildPerson, true)),
					expected("Person"));

			// the engine loses 'Man' from this expression once 'hasChild some Person' has been
			// asked about before, as it was above, over OWLlink as well, see Java/Readme.md
			checkEngineDefect("equivalentClasses(Man and hasChild some Person)", names(reasoner.getEquivalentClasses(manWithChild)),
					expected("Father"));
			checkEngineDefect("superClasses(Man and ..., true)  ", names(reasoner.getSuperClasses(manWithChild, true)),
					expected("Man", "Parent"));
			checkEngineDefect("superClasses(Man and ..., false) ", names(reasoner.getSuperClasses(manWithChild, false)),
					expected("Man", "Parent", "Person", "Thing"));

			// the inferred disjoint classes of Protege are the sub classes of the complement
			check("subClasses(not Man, true)        ", names(reasoner.getSubClasses(notMan, true)),
					expected("Woman"));
			checkContains("subClasses(not Man, false)       ", names(reasoner.getSubClasses(notMan, false)),
					"Woman", "Mother", "Impossible");
			check("equivalentClasses(not Man)       ", names(reasoner.getEquivalentClasses(notMan)),
					expected());

			check("isSatisfiable(hasChild some Person)", Boolean.valueOf(reasoner.isSatisfiable(hasChildPerson)),
					Boolean.TRUE);
			check("isSatisfiable(Man and Woman)     ", Boolean.valueOf(reasoner.isSatisfiable(manAndWoman)),
					Boolean.FALSE);
			// the expression is equivalent to owl:Nothing, which has no strict sub class
			check("equivalentClasses(Man and Woman) ", names(reasoner.getEquivalentClasses(manAndWoman)),
					expected("Impossible", "Nothing"));
			check("subClasses(Man and Woman, false) ", names(reasoner.getSubClasses(manAndWoman, false)),
					expected());

			check("instances(hasChild some Woman)   ", names(reasoner.getInstances(hasChildWoman, false)),
					expected("john", "johnny"));
			NodeSet<OWLNamedIndividual> womenOrParents = reasoner.getInstances(womanOrParent, false);
			check("instances(Woman or Parent)       ", names(womenOrParents),
					expected("john", "johnny", "mary"));
			check("instances(Woman or Parent) nodes ", Integer.valueOf(womenOrParents.getNodes().size()),
					Integer.valueOf(2));
			// every instance of the union is an instance of Woman or of Parent, so none is direct,
			// while nothing named lies below 'hasChild some Woman'
			check("instances(Woman or Parent, direct)", names(reasoner.getInstances(womanOrParent, true)),
					expected());
			check("instances(hasChild some Woman, direct)", names(reasoner.getInstances(hasChildWoman, true)),
					expected("john", "johnny"));

			// the named case still takes the class hierarchy directly, same answers
			check("subClasses(Parent, true)         ", names(reasoner.getSubClasses(cls("Parent"), true)),
					expected("Father", "Grandparent", "Mother"));

			// a fresh entity in an expression is never asked of the native side, with the default
			// policy ALLOW the answer is the trivial one
			check("subClasses(hasChild some Fresh)  ", names(reasoner.getSubClasses(hasChildFresh, false)),
					expected());
			check("equivalentClasses(hasChild some Fresh)", names(reasoner.getEquivalentClasses(hasChildFresh)),
					expected());
			check("isSatisfiable(hasChild some Fresh)", Boolean.valueOf(reasoner.isSatisfiable(hasChildFresh)),
					Boolean.TRUE);
			// and the reasoner keeps answering afterwards
			check("equivalentClasses(hasChild some Person) again", names(reasoner.getEquivalentClasses(hasChildPerson)),
					expected("Parent"));
		} finally {
			reasoner.dispose();
		}

		final OWLReasoner strictReasoner = new KoncludeReasonerFactory().createReasoner(ontology,
				new SimpleConfiguration(FreshEntityPolicy.DISALLOW, Long.MAX_VALUE));
		try {
			checkThrows("subClasses(hasChild some Fresh) with DISALLOW", FreshEntitiesException.class, new Runnable() {
				public void run() { strictReasoner.getSubClasses(hasChildFresh, false); }
			});
		} finally {
			strictReasoner.dispose();
		}
	}

	private static void runIndividuals() throws Exception {
		OWLOntology ontology = familyOntology();
		final OWLReasoner reasoner = reasonerFor(ontology);
		try {
			// john has a child who is a person, so he is a Parent and, being a Man, a Father
			check("types(john, direct=false)      ", names(reasoner.getTypes(individual("john"), false)),
					expected("Father", "Man", "Parent", "Person", "Thing"));
			check("types(john, direct=true)       ", names(reasoner.getTypes(individual("john"), true)),
					expected("Father"));
			check("instances(Parent, false)       ", names(reasoner.getInstances(cls("Parent"), false)),
					expected("john", "johnny"));
			check("instances(Person, false)       ", names(reasoner.getInstances(cls("Person"), false)),
					expected("john", "johnny", "mary"));
			check("objectPropertyValues(john, hasChild)",
					names(reasoner.getObjectPropertyValues(individual("john"), objectProperty("hasChild"))),
					expected("mary"));

			check("sameIndividuals(john)          ", names(reasoner.getSameIndividuals(individual("john"))),
					expected("john", "johnny"));

			// the individuals of a node set are grouped by sameAs, which is what
			// IndividualNodeSetPolicy.BY_SAME_AS promises
			NodeSet<OWLNamedIndividual> parents = reasoner.getInstances(cls("Parent"), false);
			check("instances(Parent) node count   ", Integer.valueOf(parents.getNodes().size()),
					Integer.valueOf(1));
			check("instances(Parent) single node  ", names(parents.getNodes().iterator().next()),
					expected("john", "johnny"));
		} finally {
			reasoner.dispose();
		}
	}

	/**
	 * The object property hierarchy, whose four native queries reported nothing until the
	 * defects described in Java/Readme.md were fixed. The inferred sameness of two individuals
	 * is checked here as well, since it rests on the same realization.
	 */
	private static void runProperties() throws Exception {
		OWLOntology ontology = familyOntology();
		OWLReasoner reasoner = reasonerFor(ontology);
		try {
			check("subObjectProperties(hasChild, false)",
					names(reasoner.getSubObjectProperties(objectProperty("hasChild"), false)),
					expected("hasSon", "bottomObjectProperty"));
			check("subObjectProperties(hasChild, true) ",
					names(reasoner.getSubObjectProperties(objectProperty("hasChild"), true)),
					expected("hasSon"));
			check("superObjectProperties(hasSon, false)",
					names(reasoner.getSuperObjectProperties(objectProperty("hasSon"), false)),
					expected("hasChild", "topObjectProperty"));
			check("superObjectProperties(hasSon, true) ",
					names(reasoner.getSuperObjectProperties(objectProperty("hasSon"), true)),
					expected("hasChild"));
			check("equivalentObjectProperties(hasChild)",
					names(reasoner.getEquivalentObjectProperties(objectProperty("hasChild"))),
					expected("hasChild"));
			checkContains("topObjectPropertyNode               ",
					names(reasoner.getTopObjectPropertyNode()), "topObjectProperty");
			// as for the classes, the bottom property is not a direct sub property of the top one
			check("subObjectProperties(top, direct=true)",
					names(reasoner.getSubObjectProperties(DF.getOWLTopObjectProperty(), true)),
					expected("hasChild", "hasParent"));
			check("superObjectProperties(bottom, true) ",
					names(reasoner.getSuperObjectProperties(DF.getOWLBottomObjectProperty(), true)),
					expected("hasParent", "hasSon"));
		} finally {
			reasoner.dispose();
		}

		// an equivalence of two properties that the family ontology does not have
		OWLOntologyManager manager = OWLManager.createOWLOntologyManager();
		OWLOntology equivalences = manager.createOntology(IRI.create(NS + "equivalences"));
		manager.addAxioms(equivalences, new HashSet<OWLAxiom>(Arrays.<OWLAxiom>asList(
			DF.getOWLEquivalentObjectPropertiesAxiom(objectProperty("p"), objectProperty("q"))
		)));
		OWLReasoner equivalenceReasoner = reasonerFor(equivalences);
		try {
			check("equivalentObjectProperties(p)       ",
					names(equivalenceReasoner.getEquivalentObjectProperties(objectProperty("p"))),
					expected("p", "q"));
		} finally {
			equivalenceReasoner.dispose();
		}

		// NOTE: a sameness that is inferred rather than asserted is asked for in the 'merges'
		// scenario, it needs a loading configuration of its own.
	}

	/**
	 * The two constructs whose native entry points were mixed up in
	 * com_konclude_jnibridge_AxiomExpressionBuildingBridge.cpp and are therefore easy to break
	 * again: a sub data property axiom and a datatype restriction.
	 */
	private static void runDatatypes() throws Exception {
		OWLOntologyManager manager = OWLManager.createOWLOntologyManager();
		OWLOntology ontology = manager.createOntology(IRI.create(NS));
		OWLDataProperty age = dataProperty("age");
		OWLDataProperty measure = dataProperty("measure");
		OWLDatatype integer = OWL2Datatype.XSD_INTEGER.getDatatype(DF);
		OWLDataRange atLeast5 = DF.getOWLDatatypeRestriction(integer,
				DF.getOWLFacetRestriction(OWLFacet.MIN_INCLUSIVE, DF.getOWLLiteral(5)));
		OWLDataRange atLeast10 = DF.getOWLDatatypeRestriction(integer,
				DF.getOWLFacetRestriction(OWLFacet.MIN_INCLUSIVE, DF.getOWLLiteral(10)));

		manager.addAxioms(ontology, new HashSet<OWLAxiom>(Arrays.<OWLAxiom>asList(
			DF.getOWLSubDataPropertyOfAxiom(age, measure),
			DF.getOWLSubClassOfAxiom(cls("Aged"), DF.getOWLDataSomeValuesFrom(age, integer)),
			DF.getOWLEquivalentClassesAxiom(cls("Measured"), DF.getOWLDataSomeValuesFrom(measure, integer)),
			DF.getOWLEquivalentClassesAxiom(cls("AtLeast5"), DF.getOWLDataSomeValuesFrom(age, atLeast5)),
			DF.getOWLEquivalentClassesAxiom(cls("AtLeast10"), DF.getOWLDataSomeValuesFrom(age, atLeast10))
		)));

		OWLReasoner reasoner = reasonerFor(ontology);
		try {
			check("unsupported axioms             ",
					Integer.valueOf(((KoncludeReasoner) reasoner).getUnsupportedAxioms().size()),
					Integer.valueOf(0));
			// Aged is subsumed by Measured only if SubDataPropertyOf reached the reasoner
			checkContains("subClasses(Measured, false)    ",
					names(reasoner.getSubClasses(cls("Measured"), false)), "Aged", "AtLeast5", "AtLeast10");
			// AtLeast10 is subsumed by AtLeast5 only if the facets are compared
			checkContains("subClasses(AtLeast5, false)    ",
					names(reasoner.getSubClasses(cls("AtLeast5"), false)), "AtLeast10");
		} finally {
			reasoner.dispose();
		}
	}

	private static void runInconsistency() throws Exception {
		OWLOntologyManager manager = OWLManager.createOWLOntologyManager();
		OWLOntology ontology = manager.createOntology(IRI.create(NS));
		manager.addAxioms(ontology, new HashSet<OWLAxiom>(Arrays.<OWLAxiom>asList(
			DF.getOWLDisjointClassesAxiom(cls("Man"), cls("Woman")),
			DF.getOWLClassAssertionAxiom(cls("Man"), individual("pat")),
			DF.getOWLClassAssertionAxiom(cls("Woman"), individual("pat"))
		)));

		OWLReasoner reasoner = reasonerFor(ontology);
		try {
			check("consistent", Boolean.valueOf(reasoner.isConsistent()), Boolean.FALSE);
		} finally {
			reasoner.dispose();
		}
	}

	private static void runEntailment() throws Exception {
		OWLOntology ontology = familyOntology();
		OWLObjectProperty hasChild = objectProperty("hasChild");
		OWLObjectProperty hasSon = objectProperty("hasSon");
		final OWLReasoner reasoner = reasonerFor(ontology);
		try {
			check("supported(SUBCLASS_OF)          ",
					Boolean.valueOf(reasoner.isEntailmentCheckingSupported(AxiomType.SUBCLASS_OF)), Boolean.TRUE);
			check("supported(DISJOINT_CLASSES)     ",
					Boolean.valueOf(reasoner.isEntailmentCheckingSupported(AxiomType.DISJOINT_CLASSES)), Boolean.TRUE);

			// named classes, an inferred one, a wrong one, and one that needs the complement
			check("Father subClassOf Person        ", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLSubClassOfAxiom(cls("Father"), cls("Person")))), Boolean.TRUE);
			check("Person subClassOf Father        ", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLSubClassOfAxiom(cls("Person"), cls("Father")))), Boolean.FALSE);
			check("Man subClassOf Thing            ", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLSubClassOfAxiom(cls("Man"), DF.getOWLThing()))), Boolean.TRUE);
			check("Impossible subClassOf Woman     ", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLSubClassOfAxiom(cls("Impossible"), cls("Woman")))), Boolean.TRUE);
			// anonymous expressions on both sides
			check("Man and hasChild some Person subClassOf Father", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLSubClassOfAxiom(DF.getOWLObjectIntersectionOf(cls("Man"),
							DF.getOWLObjectSomeValuesFrom(hasChild, cls("Person"))), cls("Father")))), Boolean.TRUE);
			check("Parent equivalent to Person and hasChild some Person", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLEquivalentClassesAxiom(cls("Parent"), DF.getOWLObjectIntersectionOf(cls("Person"),
							DF.getOWLObjectSomeValuesFrom(hasChild, cls("Person")))))), Boolean.TRUE);
			check("Father equivalent to Mother     ", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLEquivalentClassesAxiom(cls("Father"), cls("Mother")))), Boolean.FALSE);
			check("Father disjoint with Mother     ", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLDisjointClassesAxiom(cls("Father"), cls("Mother")))), Boolean.TRUE);
			check("Father disjoint with Parent     ", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLDisjointClassesAxiom(cls("Father"), cls("Parent")))), Boolean.FALSE);

			// properties
			check("hasChild domain Person          ", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLObjectPropertyDomainAxiom(hasChild, cls("Person")))), Boolean.TRUE);
			check("hasChild domain Man             ", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLObjectPropertyDomainAxiom(hasChild, cls("Man")))), Boolean.FALSE);
			check("hasChild range Person           ", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLObjectPropertyRangeAxiom(hasChild, cls("Person")))), Boolean.TRUE);
			check("hasChild range Woman            ", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLObjectPropertyRangeAxiom(hasChild, cls("Woman")))), Boolean.FALSE);
			check("hasSon subPropertyOf hasChild   ", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLSubObjectPropertyOfAxiom(hasSon, hasChild))), Boolean.TRUE);
			check("hasChild subPropertyOf hasSon   ", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLSubObjectPropertyOfAxiom(hasChild, hasSon))), Boolean.FALSE);

			// individuals
			check("john : Man                      ", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLClassAssertionAxiom(cls("Man"), individual("john")))), Boolean.TRUE);
			check("john : Father                   ", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLClassAssertionAxiom(cls("Father"), individual("john")))), Boolean.TRUE);
			check("john : Woman                    ", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLClassAssertionAxiom(cls("Woman"), individual("john")))), Boolean.FALSE);
			check("john : hasChild some Woman      ", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLClassAssertionAxiom(DF.getOWLObjectSomeValuesFrom(hasChild, cls("Woman")),
							individual("john")))), Boolean.TRUE);
			check("john hasChild mary              ", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLObjectPropertyAssertionAxiom(hasChild, individual("john"), individual("mary")))), Boolean.TRUE);
			check("mary hasChild john              ", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLObjectPropertyAssertionAxiom(hasChild, individual("mary"), individual("john")))), Boolean.FALSE);
			check("john same as johnny             ", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLSameIndividualAxiom(individual("john"), individual("johnny")))), Boolean.TRUE);
			check("john same as mary               ", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLSameIndividualAxiom(individual("john"), individual("mary")))), Boolean.FALSE);
			check("john different from mary        ", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLDifferentIndividualsAxiom(individual("john"), individual("mary")))), Boolean.TRUE);
			check("john different from johnny      ", Boolean.valueOf(reasoner.isEntailed(
					DF.getOWLDifferentIndividualsAxiom(individual("john"), individual("johnny")))), Boolean.FALSE);

			// a set is entailed when each of its axioms is
			check("set of two entailed axioms      ", Boolean.valueOf(reasoner.isEntailed(new HashSet<OWLAxiom>(Arrays.<OWLAxiom>asList(
					DF.getOWLSubClassOfAxiom(cls("Father"), cls("Person")),
					DF.getOWLClassAssertionAxiom(cls("Man"), individual("john")))))), Boolean.TRUE);
			check("set with one wrong axiom        ", Boolean.valueOf(reasoner.isEntailed(new HashSet<OWLAxiom>(Arrays.<OWLAxiom>asList(
					DF.getOWLSubClassOfAxiom(cls("Father"), cls("Person")),
					DF.getOWLSubClassOfAxiom(cls("Person"), cls("Father")))))), Boolean.FALSE);
		} finally {
			reasoner.dispose();
		}
	}

	/**
	 * interrupt() releases a caller that waits for the reasoner's thread. Whether the call below
	 * is still in flight when the interrupt arrives is a race on a small ontology, so both
	 * outcomes are accepted; what has to hold is that nothing else is thrown and that the
	 * reasoner answers correctly afterwards.
	 */
	private static void runInterrupt() throws Exception {
		OWLOntology ontology = familyOntology();
		final OWLReasoner reasoner = reasonerFor(ontology);
		try {
			reasoner.interrupt();
			check("answers after an idle interrupt ", names(reasoner.getSubClasses(cls("Person"), true)),
					expected("Man", "Parent", "Woman"));

		} finally {
			reasoner.dispose();
		}

		// a call caught in flight: the family ontology is classified in a millisecond, so a
		// larger one is built whose classification takes long enough for the interrupt to land
		OWLOntology large = wideOntology(4000);
		final OWLReasoner slow = reasonerFor(large);
		try {
			final Throwable[] thrown = new Throwable[1];
			final long[] released = new long[1];
			final java.util.concurrent.CountDownLatch started = new java.util.concurrent.CountDownLatch(1);
			Thread caller = new Thread(new Runnable() {
				public void run() {
					try {
						started.countDown();
						slow.precomputeInferences(InferenceType.CLASS_HIERARCHY);
					} catch (Throwable t) {
						thrown[0] = t;
					}
					released[0] = System.nanoTime();
				}
			});
			caller.start();
			started.await();
			// interrupt until the caller is released: the call is in flight from the moment it is
			// handed to the reasoner's thread, and it takes far longer than the interval here
			long interrupted = 0;
			while (caller.isAlive()) {
				interrupted = System.nanoTime();
				slow.interrupt();
				Thread.sleep(1);
			}
			caller.join();
			check("the call in flight was interrupted", Boolean.valueOf(thrown[0] != null), Boolean.TRUE);
			if (thrown[0] != null) {
				check("what an interrupted call throws ", thrown[0].getClass(), ReasonerInterruptedException.class);
				report(String.format("the caller was released %.1f ms after the last interrupt", (released[0] - interrupted) / 1e6));
			}
			// the calculation ran on and later calls queue behind it, so the answer is complete:
			// the 4000 classes, their 4000 defined counterparts and owl:Nothing
			check("answers after the interrupt     ", Integer.valueOf(slow.getSubClasses(cls("Root"), false).getFlattened().size()),
					Integer.valueOf(8001));
		} finally {
			slow.dispose();
		}
	}

	/** what a progress monitor was told, in order, for the progress scenario */
	private static class RecordingMonitor implements ReasonerProgressMonitor {
		final java.util.List<String> tasks = new java.util.ArrayList<String>();
		int stopped = 0;
		int busy = 0;
		int reports = 0;
		int badReports = 0;
		int previousValue = -1;
		int lastValue = -1;
		int lastMax = -1;
		/** the reports per task, in the order of the tasks */
		final java.util.List<Integer> reportsPerTask = new java.util.ArrayList<Integer>();
		public synchronized void reasonerTaskStarted(String name) { tasks.add(name); reportsPerTask.add(0); previousValue = -1; }
		public synchronized void reasonerTaskStopped() { ++stopped; }
		public synchronized void reasonerTaskBusy() { ++busy; }
		public synchronized void reasonerTaskProgressChanged(int value, int max) {
			++reports;
			if (!reportsPerTask.isEmpty()) {
				int last = reportsPerTask.size() - 1;
				reportsPerTask.set(last, reportsPerTask.get(last) + 1);
			}
			// within a task a value never below the previous one, never above the maximum, which may grow
			if (value < previousValue || value > max || max <= 0) {
				++badReports;
			}
			previousValue = value;
			lastValue = value;
			lastMax = max;
		}
	}

	/**
	 * An ontology of n independent classes with a disjunction each, which the saturation cannot
	 * decide, so the classifier tests every one of them and its counts climb: 20000 classes
	 * take about 1.6 seconds of tests. The wide ontology is no use here, the saturation
	 * classifies it without a test.
	 */
	private static OWLOntology disjunctiveOntology(int n) throws OWLOntologyCreationException {
		OWLOntologyManager manager = OWLManager.createOWLOntologyManager();
		OWLOntology ontology = manager.createOntology(IRI.create(NS + "disjunctive"));
		OWLObjectProperty r = objectProperty("r");
		OWLClass root = cls("Root");
		Set<OWLAxiom> axioms = new HashSet<OWLAxiom>();
		for (int i = 0; i < n; i++) {
			OWLClass a = cls("A" + i);
			OWLClass b = cls("B" + i);
			axioms.add(DF.getOWLSubClassOfAxiom(a, root));
			axioms.add(DF.getOWLSubClassOfAxiom(b, root));
			axioms.add(DF.getOWLSubClassOfAxiom(cls("C" + i), DF.getOWLObjectIntersectionOf(root,
					DF.getOWLObjectUnionOf(DF.getOWLObjectSomeValuesFrom(r, a), DF.getOWLObjectSomeValuesFrom(r, b)))));
			axioms.add(DF.getOWLEquivalentClassesAxiom(cls("D" + i), DF.getOWLObjectIntersectionOf(root,
					DF.getOWLObjectSomeValuesFrom(r, DF.getOWLObjectUnionOf(a, b)))));
		}
		manager.addAxioms(ontology, axioms);
		return ontology;
	}

	private static void runProgress() throws Exception {
		OWLOntology large = disjunctiveOntology(20000);
		RecordingMonitor monitor = new RecordingMonitor();
		OWLReasoner reasoner = new KoncludeReasonerFactory().createReasoner(large, new SimpleConfiguration(monitor));
		try {
			reasoner.precomputeInferences(InferenceType.CLASS_HIERARCHY, InferenceType.CLASS_ASSERTIONS);
			check("tasks reported, in order        ", monitor.tasks,
					Arrays.asList(KoncludeReasoner.PRECOMPUTING, ReasonerProgressMonitor.CLASSIFYING, ReasonerProgressMonitor.REALIZING));
			check("every task was stopped          ", Integer.valueOf(monitor.stopped), Integer.valueOf(monitor.tasks.size()));
			check("busy was reported               ", Boolean.valueOf(monitor.busy > 0), Boolean.TRUE);
			check("progress was reported           ", Boolean.valueOf(monitor.reports > 0), Boolean.TRUE);
			// the saturation of 80 000 axioms takes about half a second, two intervals, and the
			// saturation nodes are counted from its first node on
			check("reported while precomputing     ", Boolean.valueOf(monitor.reportsPerTask.get(0) > 0), Boolean.TRUE);
			check("reported while classifying      ", Boolean.valueOf(monitor.reportsPerTask.get(1) > 0), Boolean.TRUE);
			check("reports out of order or range   ", Integer.valueOf(monitor.badReports), Integer.valueOf(0));
			report(String.format("%d progress reports, %s per task, the last %d / %d, %d busy reports",
					monitor.reports, monitor.reportsPerTask, monitor.lastValue, monitor.lastMax, monitor.busy));
		} finally {
			reasoner.dispose();
		}

		// an ontology the saturation classifies on its own has nothing for the classifier to
		// test, so only the precomputation is reported, and the tasks still come to an end
		OWLOntology small = familyOntology();
		RecordingMonitor quiet = new RecordingMonitor();
		OWLReasoner quick = new KoncludeReasonerFactory().createReasoner(small, new SimpleConfiguration(quiet));
		try {
			quick.precomputeInferences(InferenceType.CLASS_HIERARCHY);
			check("first task of a small ontology  ", quiet.tasks.get(0), KoncludeReasoner.PRECOMPUTING);
			check("its tasks were stopped          ", Integer.valueOf(quiet.stopped), Integer.valueOf(quiet.tasks.size()));
			check("its reports out of order        ", Integer.valueOf(quiet.badReports), Integer.valueOf(0));
		} finally {
			quick.dispose();
		}
	}

	/**
	 * An ontology of n classes below Root, each with an existential restriction to the next, so
	 * that classifying it takes long enough to interrupt.
	 */
	private static OWLOntology wideOntology(int n) throws OWLOntologyCreationException {
		OWLOntologyManager manager = OWLManager.createOWLOntologyManager();
		OWLOntology ontology = manager.createOntology(IRI.create(NS + "wide"));
		OWLObjectProperty next = objectProperty("next");
		Set<OWLAxiom> axioms = new HashSet<OWLAxiom>();
		for (int i = 0; i < n; i++) {
			OWLClass c = cls("C" + i);
			axioms.add(DF.getOWLSubClassOfAxiom(c, cls("Root")));
			axioms.add(DF.getOWLSubClassOfAxiom(c, DF.getOWLObjectSomeValuesFrom(next, cls("C" + ((i + 1) % n)))));
			axioms.add(DF.getOWLEquivalentClassesAxiom(cls("D" + i),
					DF.getOWLObjectIntersectionOf(cls("Root"), DF.getOWLObjectSomeValuesFrom(next, c))));
		}
		manager.addAxioms(ontology, axioms);
		return ontology;
	}

	private static void runUnsupported() throws Exception {
		OWLOntology ontology = familyOntology();
		final OWLReasoner reasoner = reasonerFor(ontology);
		try {
			checkThrowsUnsupported("getDisjointClasses            ", new Runnable() {
				public void run() { reasoner.getDisjointClasses(cls("Man")); }
			});
			checkThrowsUnsupported("getDisjointObjectProperties   ", new Runnable() {
				public void run() { reasoner.getDisjointObjectProperties(objectProperty("hasChild")); }
			});
			checkThrowsUnsupported("getInverseObjectProperties    ", new Runnable() {
				public void run() { reasoner.getInverseObjectProperties(objectProperty("hasChild")); }
			});
			checkThrowsUnsupported("getObjectPropertyDomains      ", new Runnable() {
				public void run() { reasoner.getObjectPropertyDomains(objectProperty("hasChild"), false); }
			});
			checkThrowsUnsupported("getObjectPropertyRanges       ", new Runnable() {
				public void run() { reasoner.getObjectPropertyRanges(objectProperty("hasChild"), false); }
			});
			checkThrowsUnsupported("getSubDataProperties          ", new Runnable() {
				public void run() { reasoner.getSubDataProperties(dataProperty("age"), false); }
			});
			checkThrowsUnsupported("getDataPropertyValues         ", new Runnable() {
				public void run() { reasoner.getDataPropertyValues(individual("john"), dataProperty("age")); }
			});
			checkThrowsUnsupported("getDifferentIndividuals       ", new Runnable() {
				public void run() { reasoner.getDifferentIndividuals(individual("john")); }
			});
			checkThrows("isEntailed(HasKey)              ", UnsupportedEntailmentTypeException.class, new Runnable() {
				public void run() { reasoner.isEntailed(DF.getOWLHasKeyAxiom(cls("Person"), objectProperty("hasChild"))); }
			});
			check("isEntailmentCheckingSupported(HAS_KEY)",
					Boolean.valueOf(reasoner.isEntailmentCheckingSupported(AxiomType.HAS_KEY)),
					Boolean.FALSE);
			check("individualNodeSetPolicy       ", reasoner.getIndividualNodeSetPolicy(),
					IndividualNodeSetPolicy.BY_SAME_AS);
			check("bufferingMode                 ", reasoner.getBufferingMode(), BufferingMode.BUFFERING);
			check("reasonerName                  ", reasoner.getReasonerName(), "Konclude");
		} finally {
			reasoner.dispose();
		}
	}

	private static void runLifecycle() throws Exception {
		// several reasoners in one virtual machine, each with its own library instance
		for (int i = 1; i <= 3; ++i) {
			OWLOntology ontology = familyOntology();
			OWLReasoner reasoner = reasonerFor(ontology);
			try {
				checkContains("reasoner " + i + ": types(john, false)  ",
						names(reasoner.getTypes(individual("john"), false)), "Father", "Parent");
			} finally {
				reasoner.dispose();
			}
		}

		// a change of the ontology is picked up by flush
		OWLOntology ontology = familyOntology();
		OWLOntologyManager manager = ontology.getOWLOntologyManager();
		OWLReasoner reasoner = reasonerFor(ontology);
		try {
			check("before: subClasses(Person, true) has Student",
					Boolean.valueOf(names(reasoner.getSubClasses(cls("Person"), true)).contains("Student")),
					Boolean.FALSE);

			manager.addAxiom(ontology, DF.getOWLSubClassOfAxiom(cls("Student"), cls("Person")));
			check("pending changes                ",
					Integer.valueOf(reasoner.getPendingChanges().size()), Integer.valueOf(1));
			check("pending additions              ",
					Integer.valueOf(reasoner.getPendingAxiomAdditions().size()), Integer.valueOf(1));

			reasoner.flush();
			check("after flush: pending changes   ",
					Integer.valueOf(reasoner.getPendingChanges().size()), Integer.valueOf(0));
			checkContains("after flush: subClasses(Person, true)",
					names(reasoner.getSubClasses(cls("Person"), true)), "Student");
		} finally {
			reasoner.dispose();
		}

		// dispose is idempotent
		reasoner.dispose();
		report("dispose twice did not fail");
	}

	/**
	 * An ABox that forces two named individuals to be merged, here by a functional property
	 * with two targets. Konclude used to stay in its precomputation for such an ontology and
	 * answered nothing that needs the classification or the realisation, see Java/Readme.md.
	 * This is the check that the fix in
	 * CTotallyPrecomputationThread::createIndividualPrecomputationCheck keeps working, so it
	 * deliberately uses the default loading configuration and no merge safety.
	 */
	private static void runMerges() throws Exception {
		OWLOntologyManager manager = OWLManager.createOWLOntologyManager();
		OWLOntology merging = manager.createOntology(IRI.create(NS + "merging"));
		manager.addAxioms(merging, new HashSet<OWLAxiom>(Arrays.<OWLAxiom>asList(
			DF.getOWLFunctionalObjectPropertyAxiom(objectProperty("r")),
			DF.getOWLObjectPropertyAssertionAxiom(objectProperty("r"), individual("x"), individual("y1")),
			DF.getOWLObjectPropertyAssertionAxiom(objectProperty("r"), individual("x"), individual("y2")),
			DF.getOWLClassAssertionAxiom(cls("Person"), individual("y1"))
		)));

		KoncludeReasoner reasoner = (KoncludeReasoner) reasonerFor(merging);
		try {
			check("completion graph not forced    ", Boolean.valueOf(reasoner.getAppliedLoadingConfiguration()
					.contains("ForceFullCompletionGraphConstruction")), Boolean.FALSE);
			check("consistent", Boolean.valueOf(reasoner.isConsistent()), Boolean.TRUE);
			// y1 and y2 have to be the same, r is functional and x is related to both
			check("sameIndividuals(y1), inferred  ",
					names(reasoner.getSameIndividuals(individual("y1"))), expected("y1", "y2"));
			// and the merge carries the type of y1 over to y2
			check("instances(Person, false)       ",
					names(reasoner.getInstances(cls("Person"), false)), expected("y1", "y2"));
			check("types(y2, false) has Person    ", Boolean.valueOf(
					names(reasoner.getTypes(individual("y2"), false)).contains("Person")), Boolean.TRUE);
		} finally {
			reasoner.dispose();
		}

		// the merge is still found when it is asked for, which is what MergeSafety.DETECT rests
		// on for a shared library that predates the fix
		KoncludeReasoner detecting = (KoncludeReasoner) new KoncludeReasonerFactory(
				KoncludeReasoner.MergeSafety.DETECT).createReasoner(merging);
		try {
			check("forced merge found by DETECT   ",
					Boolean.valueOf(detecting.getForcedIndividualMerge() != null), Boolean.TRUE);
			check("DETECT builds the graph        ", Boolean.valueOf(detecting.getAppliedLoadingConfiguration()
					.contains("ForceFullCompletionGraphConstruction")), Boolean.TRUE);
			check("and answers the same           ",
					names(detecting.getSameIndividuals(individual("y1"))), expected("y1", "y2"));
		} finally {
			detecting.dispose();
		}

		// an ontology with the same functional property but only one value has no merge
		OWLOntology harmless = manager.createOntology(IRI.create(NS + "harmless"));
		manager.addAxioms(harmless, new HashSet<OWLAxiom>(Arrays.<OWLAxiom>asList(
			DF.getOWLFunctionalObjectPropertyAxiom(objectProperty("r")),
			DF.getOWLObjectPropertyAssertionAxiom(objectProperty("r"), individual("x"), individual("y1")),
			DF.getOWLSubClassOfAxiom(cls("Man"), cls("Person"))
		)));
		KoncludeReasoner harmlessReasoner = (KoncludeReasoner) new KoncludeReasonerFactory(
				KoncludeReasoner.MergeSafety.DETECT).createReasoner(harmless);
		try {
			check("no merge in the harmless one   ",
					Boolean.valueOf(harmlessReasoner.getForcedIndividualMerge() != null), Boolean.FALSE);
			checkContains("subClasses(Person, false)      ",
					names(harmlessReasoner.getSubClasses(cls("Person"), false)), "Man");
		} finally {
			harmlessReasoner.dispose();
		}
	}

	/**
	 * The time out of the OWL API, which reports a call that does not come back instead of
	 * blocking the caller for ever. A time out of one millisecond is used so that the
	 * initialisation of the library instance already overruns it, since there is no longer a
	 * question that Konclude is known not to answer.
	 *
	 * ATTENTION: the call keeps running, the bridge cannot cancel it, which is why this
	 * scenario is the last thing its virtual machine does.
	 */
	private static void runTimeout() throws Exception {
		// first without a time out, so that the watch dog is shown not to be in the way when it
		// is not asked for. This has to come first: the abandoned call of the time out below
		// keeps running and keeps its library instance, and starting another reasoner beside it
		// is not safe
		OWLReasoner plain = reasonerFor(familyOntology());
		try {
			checkContains("no time out configured         ",
					names(plain.getSubClasses(cls("Person"), false)), "Man", "Woman");
		} finally {
			plain.dispose();
		}

		// the family ontology alone was once created, translated and classified within the
		// 1 ms on a fast machine, so the timed reasoner gets an ontology whose translation alone
		// takes tens of thousands of native calls, which cannot be done in 1 ms anywhere
		OWLOntology ontology = familyOntology();
		OWLOntologyManager manager = ontology.getOWLOntologyManager();
		Set<OWLAxiom> padding = new HashSet<OWLAxiom>();
		for (int i = 0; i < 20000; ++i) {
			padding.add(DF.getOWLSubClassOfAxiom(cls("Padding" + i), cls("Person")));
		}
		manager.addAxioms(ontology, padding);
		try {
			OWLReasoner reasoner = new KoncludeReasonerFactory()
					.createReasoner(ontology, new SimpleConfiguration(1L));
			// the installation is guarded as well, so the time out is expected to be reported here
			reasoner.getSubClasses(cls("Person"), false);
			report("nothing overran the 1 ms time out, expected a TimeOutException");
			++sFailures;
			reasoner.dispose();
		} catch (TimeOutException exception) {
			report("a call overran the 1 ms time out -> TimeOutException");
		}
	}

	// -------------------------------------------------------------------- the main

	public static void main(String[] arguments) throws Exception {
		String scenario = arguments.length > 0 ? arguments[0] : "hierarchy";
		System.out.println("scenario '" + scenario + "'");

		if ("hierarchy".equals(scenario)) {
			runHierarchy();
		} else if ("expressions".equals(scenario)) {
			runExpressions();
		} else if ("expressions-tableau".equals(scenario)) {
			sLoadingConfiguration = KoncludeReasoner.DEFAULT_LOADING_CONFIGURATION
					+ "+=Konclude.Answering.SaturationBasedSubClassDecision=false ";
			runExpressions();
		} else if ("individuals".equals(scenario)) {
			runIndividuals();
		} else if ("properties".equals(scenario)) {
			runProperties();
		} else if ("datatypes".equals(scenario)) {
			runDatatypes();
		} else if ("inconsistency".equals(scenario)) {
			runInconsistency();
		} else if ("unsupported".equals(scenario)) {
			runUnsupported();
		} else if ("lifecycle".equals(scenario)) {
			runLifecycle();
		} else if ("merges".equals(scenario)) {
			runMerges();
		} else if ("timeout".equals(scenario)) {
			runTimeout();
		} else if ("entailment".equals(scenario)) {
			runEntailment();
		} else if ("interrupt".equals(scenario)) {
			runInterrupt();
		} else if ("progress".equals(scenario)) {
			runProgress();
		} else {
			System.err.println("unknown scenario '" + scenario + "'");
			System.exit(2);
		}

		if (sFailures > 0) {
			System.out.println("FAILED, " + sFailures + " check(s) did not hold");
			System.exit(1);
		}
		System.out.println("PASSED");
	}
}
