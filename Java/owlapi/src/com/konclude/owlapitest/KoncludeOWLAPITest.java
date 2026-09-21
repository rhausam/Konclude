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
		try {
			call.run();
			report(label + " did NOT throw, expected an UnsupportedOperationException");
			++sFailures;
		} catch (UnsupportedOperationException exception) {
			report(label + " -> UnsupportedOperationException");
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

	private static OWLReasoner reasonerFor(OWLOntology ontology) {
		return new KoncludeReasonerFactory().createReasoner(ontology);
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
		} finally {
			reasoner.dispose();
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
			checkThrowsUnsupported("isEntailed                    ", new Runnable() {
				public void run() { reasoner.isEntailed(DF.getOWLSubClassOfAxiom(cls("Man"), cls("Person"))); }
			});
			// a query about an anonymous class expression cannot be asked either, the bridge
			// identifies the asked class by its IRI
			checkThrowsUnsupported("getSubClasses(anonymous)      ", new Runnable() {
				public void run() {
					reasoner.getSubClasses(DF.getOWLObjectSomeValuesFrom(
							objectProperty("hasChild"), cls("Person")), false);
				}
			});

			check("isEntailmentCheckingSupported(SUBCLASS_OF)",
					Boolean.valueOf(reasoner.isEntailmentCheckingSupported(AxiomType.SUBCLASS_OF)),
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

		OWLOntology ontology = familyOntology();
		try {
			OWLReasoner reasoner = new KoncludeReasonerFactory()
					.createReasoner(ontology, new SimpleConfiguration(1L));
			// the installation is guarded as well, so the time out may already be reported here
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
