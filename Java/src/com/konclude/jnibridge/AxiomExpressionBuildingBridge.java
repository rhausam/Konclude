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

package com.konclude.jnibridge;

/**
 * Builds axioms and expressions in the native reasoner, corresponds to
 * Source/Control/Interface/JNI/com_konclude_jnibridge_AxiomExpressionBuildingBridge.cpp.
 *
 * All 66 entry points that the native side declares are declared here, in the order in which
 * the OWL 2 structural specification introduces the constructs.
 *
 * The returned long values are the addresses of the built expressions, they are passed back
 * to the bridge to compose larger expressions and axioms. Axioms are told to the ontology
 * while they are built, so the return value of an axiom building method is usually not
 * needed. A returned 0 means that nothing was built, which happens when the bridge has not
 * been initialised.
 *
 * The object that is handed over together with an entity is remembered by the native side
 * and returned by the queries, which is how an entity of a client, an OWL API OWLClass for
 * instance, finds its way back into the query results. Only the first object that is handed
 * over for an entity is kept, later ones are ignored.
 *
 *
 * ARGUMENT LISTS
 *
 * The n-ary constructs take their arguments from an argument list instead of from their
 * parameters:
 *
 *   initArumentList(bridge);
 *   for (long argument : arguments) { addArgument(bridge, argument); }
 *   long expression = buildOWLObjectIntersectionFromArgumentList(bridge);
 *
 * where the building method pops the list. The native side keeps the lists on a stack, so
 * nested lists are allowed, as long as every list is popped by exactly one building method.
 *
 * ATTENTION: the native side does not check the stack. addArgument and every
 * ...FromArgumentList method crash the virtual machine if no list has been pushed, so a
 * caller has to pair them itself. closeArumentList is a no-op that the native side keeps for
 * symmetry, calling it is not required. Note the misspelling of both list methods, it is the
 * name that the native side exports.
 */
public class AxiomExpressionBuildingBridge {

	/** written by the native side, holds the CJNIAxiomExpressionVisitingLoader of this builder */
	private long mNativeDataPointer = 0;

	/** the native side instantiates this class, so the parameterless constructor is required */
	public AxiomExpressionBuildingBridge() {
	}


	// ---------------------------------------------------------------- entities

	public native long buildOWLClass(KoncludeReasonerBridge bridge, String iri, Object classObject);

	public native long buildOWLObjectProperty(KoncludeReasonerBridge bridge, String iri, Object propertyObject);

	public native long buildOWLDataProperty(KoncludeReasonerBridge bridge, String iri, Object propertyObject);

	public native long buildOWLNamedIndividual(KoncludeReasonerBridge bridge, String iri, Object individualObject);

	/** the identifier is the node identifier of the individual, without a namespace */
	public native long buildOWLAnonymousIndividual(KoncludeReasonerBridge bridge, String identifier, Object individualObject);


	// ------------------------------------------------------------ argument list

	/** pushes a new argument list, note the misspelling of the native name */
	public native void initArumentList(KoncludeReasonerBridge bridge);

	/** a no-op on the native side, note the misspelling of the native name */
	public native void closeArumentList(KoncludeReasonerBridge bridge);

	/** appends to the argument list on top of the stack, crashes if there is none */
	public native void addArgument(KoncludeReasonerBridge bridge, long argument);


	// ------------------------------------------------------- class expressions

	public native long buildOWLObjectIntersectionFromArgumentList(KoncludeReasonerBridge bridge);

	public native long buildOWLObjectUnionFromArgumentList(KoncludeReasonerBridge bridge);

	public native long buildOWLObjectComplement(KoncludeReasonerBridge bridge, long classExpression);

	public native long buildOWLObjectOneOfFromArgumentList(KoncludeReasonerBridge bridge);

	public native long buildOWLObjectSomeValuesFrom(KoncludeReasonerBridge bridge, long property, long filler);

	public native long buildOWLObjectAllValuesFrom(KoncludeReasonerBridge bridge, long property, long filler);

	/** the filler is the individual of the value restriction */
	public native long buildOWLObjectHasValue(KoncludeReasonerBridge bridge, long property, long individual);

	public native long buildOWLObjectHasSelf(KoncludeReasonerBridge bridge, long property);

	public native long buildOWLMinCardinality(KoncludeReasonerBridge bridge, long property, long filler, long cardinality);

	public native long buildOWLMaxCardinality(KoncludeReasonerBridge bridge, long property, long filler, long cardinality);

	public native long buildOWLExactCardinality(KoncludeReasonerBridge bridge, long property, long filler, long cardinality);


	// ---------------------------------------------------- property expressions

	public native long buildOWLObjectInverseOf(KoncludeReasonerBridge bridge, long property);


	// ---------------------------------------- data ranges, literals and facets

	public native long buildOWLDatatype(KoncludeReasonerBridge bridge, String iri, Object datatypeObject);

	/**
	 * Builds a literal of the given lexical form and datatype. The native side associates the
	 * handed over object with the lexical value, not with the literal, so two literals of the
	 * same lexical form and different datatypes share their object.
	 */
	public native long buildOWLLiteral(KoncludeReasonerBridge bridge, long datatype, String lexicalForm, Object literalObject);

	public native long buildOWLDataOneOfFromArgumentList(KoncludeReasonerBridge bridge);

	public native long buildOWLDataComplementOf(KoncludeReasonerBridge bridge, long dataRange);

	public native long buildOWLDataIntersectionOfFromArgumentList(KoncludeReasonerBridge bridge);

	public native long buildOWLDataUnionOfFromArgumentList(KoncludeReasonerBridge bridge);

	/** the object is associated with the facet, not with the restriction */
	public native long buildOWLFacetRestriction(KoncludeReasonerBridge bridge, String facetIRI, Object facetObject, long literal);

	/** the argument list holds the facet restrictions */
	public native long buildOWLDatatypeRestrictionFromArgumentList(KoncludeReasonerBridge bridge, long datatype);


	// -------------------------------------------- data property restrictions

	public native long buildOWLDataSomeValuesFrom(KoncludeReasonerBridge bridge, long property, long filler);

	public native long buildOWLDataAllValuesFrom(KoncludeReasonerBridge bridge, long property, long filler);

	/** the filler is the literal of the value restriction */
	public native long buildOWLDataHasValue(KoncludeReasonerBridge bridge, long property, long literal);

	public native long buildOWLDataMinCardinality(KoncludeReasonerBridge bridge, long property, long filler, long cardinality);

	public native long buildOWLDataMaxCardinality(KoncludeReasonerBridge bridge, long property, long filler, long cardinality);

	public native long buildOWLDataExactCardinality(KoncludeReasonerBridge bridge, long property, long filler, long cardinality);


	// ------------------------------------------------------------ class axioms

	public native long buildOWLSubClassOfAxiom(KoncludeReasonerBridge bridge, long subClass, long superClass);

	public native long buildOWLEquivalentClassesFromArgumentList(KoncludeReasonerBridge bridge);

	public native long buildOWLDisjointClassesAxiomFromArgumentList(KoncludeReasonerBridge bridge);

	/** the argument list holds the classes of the union */
	public native long buildOWLDisjointUnionAxiomFromArgumentList(KoncludeReasonerBridge bridge, long classExpression);


	// -------------------------------------------------- object property axioms

	public native long buildOWLSubObjectPropertyOfAxiom(KoncludeReasonerBridge bridge, long subProperty, long superProperty);

	/** the argument list holds the properties of the chain */
	public native long buildOWLSubPropertyChainOfAxiomFromArgumentList(KoncludeReasonerBridge bridge, long superProperty);

	public native long buildOWLEquivalentObjectPropertiesFromArgumentList(KoncludeReasonerBridge bridge);

	public native long buildOWLDisjointObjectPropertiesAxiomFromArgumentList(KoncludeReasonerBridge bridge);

	public native long buildOWLInverseObjectPropertiesAxiom(KoncludeReasonerBridge bridge, long property, long inverseProperty);

	public native long buildOWLObjectPropertyDomainAxiom(KoncludeReasonerBridge bridge, long property, long domain);

	public native long buildOWLObjectPropertyRangeAxiom(KoncludeReasonerBridge bridge, long property, long range);

	public native long buildOWLFunctionalObjectPropertyAxiom(KoncludeReasonerBridge bridge, long property);

	public native long buildOWLInverseFunctionalObjectPropertyAxiom(KoncludeReasonerBridge bridge, long property);

	public native long buildOWLReflexiveObjectPropertyAxiom(KoncludeReasonerBridge bridge, long property);

	public native long buildOWLIrreflexiveObjectPropertyAxiom(KoncludeReasonerBridge bridge, long property);

	public native long buildOWLSymmetricObjectPropertyAxiom(KoncludeReasonerBridge bridge, long property);

	public native long buildOWLAsymmetricObjectPropertyAxiom(KoncludeReasonerBridge bridge, long property);

	public native long buildOWLTransitiveObjectPropertyAxiom(KoncludeReasonerBridge bridge, long property);


	// ---------------------------------------------------- data property axioms

	public native long buildOWLSubDataPropertyOfAxiom(KoncludeReasonerBridge bridge, long subProperty, long superProperty);

	public native long buildOWLEquivalentDataPropertiesAxiomFromArgumentList(KoncludeReasonerBridge bridge);

	public native long buildOWLDisjointDataPropertiesAxiomFromArgumentList(KoncludeReasonerBridge bridge);

	public native long buildOWLDataPropertyDomainAxiom(KoncludeReasonerBridge bridge, long property, long domain);

	public native long buildOWLDataPropertyRangeAxiom(KoncludeReasonerBridge bridge, long property, long range);

	public native long buildOWLFunctionalDataPropertyAxiom(KoncludeReasonerBridge bridge, long property);


	// ------------------------------------------------------------- assertions

	public native long buildOWLDeclarationAxiom(KoncludeReasonerBridge bridge, long entity);

	public native long buildOWLClassAssertionAxiom(KoncludeReasonerBridge bridge, long classExpression, long individual);

	public native long buildOWLObjectPropertyAssertionAxiom(KoncludeReasonerBridge bridge, long property, long subject, long object);

	public native long buildOWLNegativeObjectPropertyAssertionAxiom(KoncludeReasonerBridge bridge, long property, long subject, long object);

	public native long buildOWLDataPropertyAssertionAxiom(KoncludeReasonerBridge bridge, long property, long subject, long literal);

	public native long buildOWLNegativeDataPropertyAssertionAxiom(KoncludeReasonerBridge bridge, long property, long subject, long literal);

	public native long buildOWLSameIndividualAxiomFromArgumentList(KoncludeReasonerBridge bridge);

	public native long buildOWLDifferentIndividualsAxiomFromArgumentList(KoncludeReasonerBridge bridge);


	/** only of interest for the smoke test, to show that the native side filled the field */
	public long getNativeDataPointer() {
		return mNativeDataPointer;
	}
}
