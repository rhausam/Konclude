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

package com.konclude.owlapi;

import com.konclude.jnibridge.AxiomExpressionBuildingBridge;
import com.konclude.jnibridge.KoncludeReasonerBridge;

import org.semanticweb.owlapi.model.*;
import org.semanticweb.owlapi.vocab.OWLFacet;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Translates the axioms of an OWL API ontology into the axiom building bridge of Konclude.
 *
 * The translator is used between initAxiomExpressionVisitingBridge and
 * finalizeAxiomExpressionVisitingBridge:
 *
 *   bridge.initAxiomExpressionVisitingBridge(builder);
 *   new KoncludeOWLAPITranslator(bridge, builder).translateOntology(ontology);
 *   bridge.finalizeAxiomExpressionVisitingBridge(builder);
 *
 * Every entity is built once and is handed to the native side together with the OWL API
 * object that denotes it, so the queries report OWL API objects back, an OWLClass for the
 * sub classes of a class for instance. The translator is therefore tied to the builder it
 * was constructed with and must not outlive it.
 *
 *
 * WHAT IS NOT TRANSLATED
 *
 * Annotations and annotation axioms are dropped, which is what the other interfaces of
 * Konclude do as well, see Readme.md. They are counted, see getIgnoredAnnotationAxiomCount.
 *
 * Three constructs have no entry point in the bridge and are reported as unsupported:
 *
 *   HasKey                the native side has no entry point
 *   DatatypeDefinition    the native side has no entry point
 *   SWRL rules            the native side has no entry point
 *
 * By default an unsupported construct ends in a KoncludeUnsupportedConstructException, since
 * dropping an axiom silently makes the reasoner report entailments that do not hold. With
 * setIgnoreUnsupportedAxioms(true) the axiom is skipped instead and is collected in
 * getUnsupportedAxioms(), which a caller can report.
 *
 * Language tags of literals are lost, the bridge takes a lexical form and a datatype only.
 *
 *
 * NOTE ON THE VISITORS
 *
 * The class implements the visitors of the OWL API by hand instead of extending an adapter,
 * so that the compiler reports a construct that is not handled. The visitors of the OWL API
 * 4.5 do not have default methods, which is why every method is present here.
 */
public class KoncludeOWLAPITranslator implements
		OWLAxiomVisitorEx<Long>,
		OWLClassExpressionVisitorEx<Long>,
		OWLDataVisitorEx<Long>,
		OWLPropertyExpressionVisitorEx<Long>,
		OWLIndividualVisitorEx<Long> {

	private final KoncludeReasonerBridge mBridge;
	private final AxiomExpressionBuildingBridge mBuilder;

	/** the address of every entity that has been built, so that it is built only once */
	private final Map<OWLObject, Long> mEntityPointers = new HashMap<OWLObject, Long>();

	private final List<OWLAxiom> mUnsupportedAxioms = new ArrayList<OWLAxiom>();

	private boolean mIgnoreUnsupportedAxioms = false;
	private boolean mDeclareEntities = true;
	private int mTranslatedAxiomCount = 0;
	private int mIgnoredAnnotationAxiomCount = 0;

	public KoncludeOWLAPITranslator(KoncludeReasonerBridge bridge, AxiomExpressionBuildingBridge builder) {
		if (bridge == null || builder == null) {
			throw new IllegalArgumentException("the bridge and the builder are required");
		}
		mBridge = bridge;
		mBuilder = builder;
	}


	// ------------------------------------------------------------------ setup

	/**
	 * Whether an axiom that the bridge does not support is skipped and collected instead of
	 * reported by a KoncludeUnsupportedConstructException. Off by default, because the answers
	 * of the reasoner are wrong if an axiom is dropped.
	 */
	public void setIgnoreUnsupportedAxioms(boolean ignoreUnsupportedAxioms) {
		mIgnoreUnsupportedAxioms = ignoreUnsupportedAxioms;
	}

	/**
	 * Whether a declaration axiom is built for every entity that is encountered. On by
	 * default, it makes the entities of the ontology known to the reasoner even if they only
	 * occur in a declaration of the source ontology.
	 */
	public void setDeclareEntities(boolean declareEntities) {
		mDeclareEntities = declareEntities;
	}


	// -------------------------------------------------------------- reporting

	/** the axioms that were skipped because the bridge does not support them */
	public List<OWLAxiom> getUnsupportedAxioms() {
		return Collections.unmodifiableList(mUnsupportedAxioms);
	}

	public int getTranslatedAxiomCount() {
		return mTranslatedAxiomCount;
	}

	public int getIgnoredAnnotationAxiomCount() {
		return mIgnoredAnnotationAxiomCount;
	}


	// ------------------------------------------------------------ translation

	/** translates the axioms of the ontology and of everything it imports */
	public void translateOntology(OWLOntology ontology) {
		translateBuiltInEntities(ontology.getOWLOntologyManager().getOWLDataFactory());
		for (OWLOntology imported : ontology.getImportsClosure()) {
			translateAxioms(imported.getAxioms());
		}
	}

	/**
	 * Builds owl:Thing, owl:Nothing and the top and bottom properties, so that the native side
	 * knows an object for them and the queries can report the top and the bottom node. Without
	 * them a query that answers with owl:Nothing reports an empty set instead.
	 */
	public void translateBuiltInEntities(OWLDataFactory factory) {
		classExpression(factory.getOWLThing());
		classExpression(factory.getOWLNothing());
		property(factory.getOWLTopObjectProperty());
		property(factory.getOWLBottomObjectProperty());
		property(factory.getOWLTopDataProperty());
		property(factory.getOWLBottomDataProperty());
	}

	/**
	 * Whether the entity has been built, which is the case if it occurs in a translated axiom
	 * or is one of the built-in entities. The native side answers a query about an entity that
	 * it does not know with an empty result, so a caller can tell the two apart with this.
	 */
	public boolean isTranslated(OWLObject entity) {
		return mEntityPointers.containsKey(entity);
	}

	public void translateAxioms(Collection<? extends OWLAxiom> axioms) {
		for (OWLAxiom axiom : axioms) {
			translateAxiom(axiom);
		}
	}

	/**
	 * Translates one axiom and tells it to the ontology of the native side. Returns the
	 * address of the built axiom, or 0 if the axiom was an annotation axiom or was skipped
	 * because it is not supported.
	 */
	public long translateAxiom(OWLAxiom axiom) {
		try {
			long pointer = unbox(axiom.accept(this));
			if (pointer != 0) {
				++mTranslatedAxiomCount;
			}
			return pointer;
		} catch (KoncludeUnsupportedConstructException exception) {
			if (!mIgnoreUnsupportedAxioms) {
				throw exception;
			}
			mUnsupportedAxioms.add(axiom);
			return 0;
		}
	}

	/** builds a class expression and returns its address, for a caller that queries with it */
	public long translateClassExpression(OWLClassExpression classExpression) {
		return unbox(classExpression.accept(this));
	}

	/** builds an entity and returns its address */
	public long translateEntity(OWLEntity entity) {
		if (entity.isOWLClass()) {
			return unbox(entity.asOWLClass().accept((OWLClassExpressionVisitorEx<Long>) this));
		}
		if (entity.isOWLObjectProperty() || entity.isOWLDataProperty()) {
			return unbox(((OWLPropertyExpression) entity).accept((OWLPropertyExpressionVisitorEx<Long>) this));
		}
		if (entity.isOWLNamedIndividual()) {
			return unbox(entity.asOWLNamedIndividual().accept((OWLIndividualVisitorEx<Long>) this));
		}
		if (entity.isOWLDatatype()) {
			return unbox(entity.asOWLDatatype().accept((OWLDataVisitorEx<Long>) this));
		}
		throw new KoncludeUnsupportedConstructException(entity, "the bridge has no entry point for it");
	}


	// -------------------------------------------------------------- the axioms

	@Override
	public Long visit(OWLDeclarationAxiom axiom) {
		OWLEntity entity = axiom.getEntity();
		if (entity.isOWLAnnotationProperty()) {
			// annotations are dropped, so an annotation property is of no interest
			++mIgnoredAnnotationAxiomCount;
			return Long.valueOf(0);
		}
		// translateEntity already declares the entity if declarations are switched on, the
		// explicit declaration here keeps the axiom translated even if they are switched off
		long entityPointer = translateEntity(entity);
		return Long.valueOf(mBuilder.buildOWLDeclarationAxiom(mBridge, entityPointer));
	}

	@Override
	public Long visit(OWLSubClassOfAxiom axiom) {
		long subClass = classExpression(axiom.getSubClass());
		long superClass = classExpression(axiom.getSuperClass());
		return Long.valueOf(mBuilder.buildOWLSubClassOfAxiom(mBridge, subClass, superClass));
	}

	@Override
	public Long visit(OWLEquivalentClassesAxiom axiom) {
		long[] arguments = classExpressions(axiom.getClassExpressions());
		pushArguments(arguments);
		return Long.valueOf(mBuilder.buildOWLEquivalentClassesFromArgumentList(mBridge));
	}

	@Override
	public Long visit(OWLDisjointClassesAxiom axiom) {
		long[] arguments = classExpressions(axiom.getClassExpressions());
		pushArguments(arguments);
		return Long.valueOf(mBuilder.buildOWLDisjointClassesAxiomFromArgumentList(mBridge));
	}

	@Override
	public Long visit(OWLDisjointUnionAxiom axiom) {
		long namedClass = classExpression(axiom.getOWLClass());
		long[] arguments = classExpressions(axiom.getClassExpressions());
		pushArguments(arguments);
		return Long.valueOf(mBuilder.buildOWLDisjointUnionAxiomFromArgumentList(mBridge, namedClass));
	}

	@Override
	public Long visit(OWLSubObjectPropertyOfAxiom axiom) {
		long subProperty = property(axiom.getSubProperty());
		long superProperty = property(axiom.getSuperProperty());
		return Long.valueOf(mBuilder.buildOWLSubObjectPropertyOfAxiom(mBridge, subProperty, superProperty));
	}

	@Override
	public Long visit(OWLSubPropertyChainOfAxiom axiom) {
		long superProperty = property(axiom.getSuperProperty());
		long[] chain = properties(axiom.getPropertyChain());
		pushArguments(chain);
		return Long.valueOf(mBuilder.buildOWLSubPropertyChainOfAxiomFromArgumentList(mBridge, superProperty));
	}

	@Override
	public Long visit(OWLEquivalentObjectPropertiesAxiom axiom) {
		long[] arguments = properties(axiom.getProperties());
		pushArguments(arguments);
		return Long.valueOf(mBuilder.buildOWLEquivalentObjectPropertiesFromArgumentList(mBridge));
	}

	@Override
	public Long visit(OWLDisjointObjectPropertiesAxiom axiom) {
		long[] arguments = properties(axiom.getProperties());
		pushArguments(arguments);
		return Long.valueOf(mBuilder.buildOWLDisjointObjectPropertiesAxiomFromArgumentList(mBridge));
	}

	@Override
	public Long visit(OWLInverseObjectPropertiesAxiom axiom) {
		long first = property(axiom.getFirstProperty());
		long second = property(axiom.getSecondProperty());
		return Long.valueOf(mBuilder.buildOWLInverseObjectPropertiesAxiom(mBridge, first, second));
	}

	@Override
	public Long visit(OWLObjectPropertyDomainAxiom axiom) {
		long propertyPointer = property(axiom.getProperty());
		long domain = classExpression(axiom.getDomain());
		return Long.valueOf(mBuilder.buildOWLObjectPropertyDomainAxiom(mBridge, propertyPointer, domain));
	}

	@Override
	public Long visit(OWLObjectPropertyRangeAxiom axiom) {
		long propertyPointer = property(axiom.getProperty());
		long range = classExpression(axiom.getRange());
		return Long.valueOf(mBuilder.buildOWLObjectPropertyRangeAxiom(mBridge, propertyPointer, range));
	}

	@Override
	public Long visit(OWLFunctionalObjectPropertyAxiom axiom) {
		return Long.valueOf(mBuilder.buildOWLFunctionalObjectPropertyAxiom(mBridge, property(axiom.getProperty())));
	}

	@Override
	public Long visit(OWLInverseFunctionalObjectPropertyAxiom axiom) {
		return Long.valueOf(mBuilder.buildOWLInverseFunctionalObjectPropertyAxiom(mBridge, property(axiom.getProperty())));
	}

	@Override
	public Long visit(OWLReflexiveObjectPropertyAxiom axiom) {
		return Long.valueOf(mBuilder.buildOWLReflexiveObjectPropertyAxiom(mBridge, property(axiom.getProperty())));
	}

	@Override
	public Long visit(OWLIrreflexiveObjectPropertyAxiom axiom) {
		return Long.valueOf(mBuilder.buildOWLIrreflexiveObjectPropertyAxiom(mBridge, property(axiom.getProperty())));
	}

	@Override
	public Long visit(OWLSymmetricObjectPropertyAxiom axiom) {
		return Long.valueOf(mBuilder.buildOWLSymmetricObjectPropertyAxiom(mBridge, property(axiom.getProperty())));
	}

	@Override
	public Long visit(OWLAsymmetricObjectPropertyAxiom axiom) {
		return Long.valueOf(mBuilder.buildOWLAsymmetricObjectPropertyAxiom(mBridge, property(axiom.getProperty())));
	}

	@Override
	public Long visit(OWLTransitiveObjectPropertyAxiom axiom) {
		return Long.valueOf(mBuilder.buildOWLTransitiveObjectPropertyAxiom(mBridge, property(axiom.getProperty())));
	}

	@Override
	public Long visit(OWLSubDataPropertyOfAxiom axiom) {
		long subProperty = property(axiom.getSubProperty());
		long superProperty = property(axiom.getSuperProperty());
		return Long.valueOf(mBuilder.buildOWLSubDataPropertyOfAxiom(mBridge, subProperty, superProperty));
	}

	@Override
	public Long visit(OWLEquivalentDataPropertiesAxiom axiom) {
		long[] arguments = properties(axiom.getProperties());
		pushArguments(arguments);
		return Long.valueOf(mBuilder.buildOWLEquivalentDataPropertiesAxiomFromArgumentList(mBridge));
	}

	@Override
	public Long visit(OWLDisjointDataPropertiesAxiom axiom) {
		long[] arguments = properties(axiom.getProperties());
		pushArguments(arguments);
		return Long.valueOf(mBuilder.buildOWLDisjointDataPropertiesAxiomFromArgumentList(mBridge));
	}

	@Override
	public Long visit(OWLDataPropertyDomainAxiom axiom) {
		long propertyPointer = property(axiom.getProperty());
		long domain = classExpression(axiom.getDomain());
		return Long.valueOf(mBuilder.buildOWLDataPropertyDomainAxiom(mBridge, propertyPointer, domain));
	}

	@Override
	public Long visit(OWLDataPropertyRangeAxiom axiom) {
		long propertyPointer = property(axiom.getProperty());
		long range = dataRange(axiom.getRange());
		return Long.valueOf(mBuilder.buildOWLDataPropertyRangeAxiom(mBridge, propertyPointer, range));
	}

	@Override
	public Long visit(OWLFunctionalDataPropertyAxiom axiom) {
		return Long.valueOf(mBuilder.buildOWLFunctionalDataPropertyAxiom(mBridge, property(axiom.getProperty())));
	}

	@Override
	public Long visit(OWLClassAssertionAxiom axiom) {
		long classPointer = classExpression(axiom.getClassExpression());
		long individualPointer = individual(axiom.getIndividual());
		return Long.valueOf(mBuilder.buildOWLClassAssertionAxiom(mBridge, classPointer, individualPointer));
	}

	@Override
	public Long visit(OWLObjectPropertyAssertionAxiom axiom) {
		long propertyPointer = property(axiom.getProperty());
		long subject = individual(axiom.getSubject());
		long object = individual(axiom.getObject());
		return Long.valueOf(mBuilder.buildOWLObjectPropertyAssertionAxiom(mBridge, propertyPointer, subject, object));
	}

	@Override
	public Long visit(OWLNegativeObjectPropertyAssertionAxiom axiom) {
		long propertyPointer = property(axiom.getProperty());
		long subject = individual(axiom.getSubject());
		long object = individual(axiom.getObject());
		return Long.valueOf(mBuilder.buildOWLNegativeObjectPropertyAssertionAxiom(mBridge, propertyPointer, subject, object));
	}

	@Override
	public Long visit(OWLDataPropertyAssertionAxiom axiom) {
		long propertyPointer = property(axiom.getProperty());
		long subject = individual(axiom.getSubject());
		long value = literal(axiom.getObject());
		return Long.valueOf(mBuilder.buildOWLDataPropertyAssertionAxiom(mBridge, propertyPointer, subject, value));
	}

	@Override
	public Long visit(OWLNegativeDataPropertyAssertionAxiom axiom) {
		long propertyPointer = property(axiom.getProperty());
		long subject = individual(axiom.getSubject());
		long value = literal(axiom.getObject());
		return Long.valueOf(mBuilder.buildOWLNegativeDataPropertyAssertionAxiom(mBridge, propertyPointer, subject, value));
	}

	@Override
	public Long visit(OWLSameIndividualAxiom axiom) {
		long[] arguments = individuals(axiom.getIndividuals());
		pushArguments(arguments);
		return Long.valueOf(mBuilder.buildOWLSameIndividualAxiomFromArgumentList(mBridge));
	}

	@Override
	public Long visit(OWLDifferentIndividualsAxiom axiom) {
		long[] arguments = individuals(axiom.getIndividuals());
		pushArguments(arguments);
		return Long.valueOf(mBuilder.buildOWLDifferentIndividualsAxiomFromArgumentList(mBridge));
	}

	@Override
	public Long visit(OWLHasKeyAxiom axiom) {
		throw new KoncludeUnsupportedConstructException(axiom, "the bridge has no entry point for it");
	}

	@Override
	public Long visit(OWLDatatypeDefinitionAxiom axiom) {
		throw new KoncludeUnsupportedConstructException(axiom, "the bridge has no entry point for it");
	}

	@Override
	public Long visit(SWRLRule rule) {
		throw new KoncludeUnsupportedConstructException(rule, "the bridge has no entry point for it");
	}


	// --------------------------------------------- the annotation axioms, dropped

	@Override
	public Long visit(OWLAnnotationAssertionAxiom axiom) {
		return ignoredAnnotationAxiom();
	}

	@Override
	public Long visit(OWLSubAnnotationPropertyOfAxiom axiom) {
		return ignoredAnnotationAxiom();
	}

	@Override
	public Long visit(OWLAnnotationPropertyDomainAxiom axiom) {
		return ignoredAnnotationAxiom();
	}

	@Override
	public Long visit(OWLAnnotationPropertyRangeAxiom axiom) {
		return ignoredAnnotationAxiom();
	}

	private Long ignoredAnnotationAxiom() {
		++mIgnoredAnnotationAxiomCount;
		return Long.valueOf(0);
	}


	// -------------------------------------------------- the class expressions

	@Override
	public Long visit(OWLClass classExpression) {
		Long known = mEntityPointers.get(classExpression);
		if (known != null) {
			return known;
		}
		long pointer = required(mBuilder.buildOWLClass(mBridge,
				classExpression.getIRI().toString(), classExpression), classExpression);
		return rememberEntity(classExpression, pointer);
	}

	@Override
	public Long visit(OWLObjectIntersectionOf classExpression) {
		pushArguments(classExpressions(classExpression.getOperands()));
		return Long.valueOf(required(mBuilder.buildOWLObjectIntersectionFromArgumentList(mBridge), classExpression));
	}

	@Override
	public Long visit(OWLObjectUnionOf classExpression) {
		pushArguments(classExpressions(classExpression.getOperands()));
		return Long.valueOf(required(mBuilder.buildOWLObjectUnionFromArgumentList(mBridge), classExpression));
	}

	@Override
	public Long visit(OWLObjectComplementOf classExpression) {
		long operand = classExpression(classExpression.getOperand());
		return Long.valueOf(required(mBuilder.buildOWLObjectComplement(mBridge, operand), classExpression));
	}

	@Override
	public Long visit(OWLObjectOneOf classExpression) {
		pushArguments(individuals(classExpression.getIndividuals()));
		return Long.valueOf(required(mBuilder.buildOWLObjectOneOfFromArgumentList(mBridge), classExpression));
	}

	@Override
	public Long visit(OWLObjectSomeValuesFrom classExpression) {
		long propertyPointer = property(classExpression.getProperty());
		long filler = classExpression(classExpression.getFiller());
		return Long.valueOf(required(mBuilder.buildOWLObjectSomeValuesFrom(mBridge, propertyPointer, filler), classExpression));
	}

	@Override
	public Long visit(OWLObjectAllValuesFrom classExpression) {
		long propertyPointer = property(classExpression.getProperty());
		long filler = classExpression(classExpression.getFiller());
		return Long.valueOf(required(mBuilder.buildOWLObjectAllValuesFrom(mBridge, propertyPointer, filler), classExpression));
	}

	@Override
	public Long visit(OWLObjectHasValue classExpression) {
		long propertyPointer = property(classExpression.getProperty());
		long filler = individual(classExpression.getFiller());
		return Long.valueOf(required(mBuilder.buildOWLObjectHasValue(mBridge, propertyPointer, filler), classExpression));
	}

	@Override
	public Long visit(OWLObjectHasSelf classExpression) {
		long propertyPointer = property(classExpression.getProperty());
		return Long.valueOf(required(mBuilder.buildOWLObjectHasSelf(mBridge, propertyPointer), classExpression));
	}

	@Override
	public Long visit(OWLObjectMinCardinality classExpression) {
		long propertyPointer = property(classExpression.getProperty());
		long filler = classExpression(classExpression.getFiller());
		return Long.valueOf(required(mBuilder.buildOWLMinCardinality(mBridge, propertyPointer, filler,
				classExpression.getCardinality()), classExpression));
	}

	@Override
	public Long visit(OWLObjectMaxCardinality classExpression) {
		long propertyPointer = property(classExpression.getProperty());
		long filler = classExpression(classExpression.getFiller());
		return Long.valueOf(required(mBuilder.buildOWLMaxCardinality(mBridge, propertyPointer, filler,
				classExpression.getCardinality()), classExpression));
	}

	@Override
	public Long visit(OWLObjectExactCardinality classExpression) {
		long propertyPointer = property(classExpression.getProperty());
		long filler = classExpression(classExpression.getFiller());
		return Long.valueOf(required(mBuilder.buildOWLExactCardinality(mBridge, propertyPointer, filler,
				classExpression.getCardinality()), classExpression));
	}

	@Override
	public Long visit(OWLDataSomeValuesFrom classExpression) {
		long propertyPointer = property(classExpression.getProperty());
		long filler = dataRange(classExpression.getFiller());
		return Long.valueOf(required(mBuilder.buildOWLDataSomeValuesFrom(mBridge, propertyPointer, filler), classExpression));
	}

	@Override
	public Long visit(OWLDataAllValuesFrom classExpression) {
		long propertyPointer = property(classExpression.getProperty());
		long filler = dataRange(classExpression.getFiller());
		return Long.valueOf(required(mBuilder.buildOWLDataAllValuesFrom(mBridge, propertyPointer, filler), classExpression));
	}

	@Override
	public Long visit(OWLDataHasValue classExpression) {
		long propertyPointer = property(classExpression.getProperty());
		long value = literal(classExpression.getFiller());
		return Long.valueOf(required(mBuilder.buildOWLDataHasValue(mBridge, propertyPointer, value), classExpression));
	}

	@Override
	public Long visit(OWLDataMinCardinality classExpression) {
		long propertyPointer = property(classExpression.getProperty());
		long filler = dataRange(classExpression.getFiller());
		return Long.valueOf(required(mBuilder.buildOWLDataMinCardinality(mBridge, propertyPointer, filler,
				classExpression.getCardinality()), classExpression));
	}

	@Override
	public Long visit(OWLDataMaxCardinality classExpression) {
		long propertyPointer = property(classExpression.getProperty());
		long filler = dataRange(classExpression.getFiller());
		return Long.valueOf(required(mBuilder.buildOWLDataMaxCardinality(mBridge, propertyPointer, filler,
				classExpression.getCardinality()), classExpression));
	}

	@Override
	public Long visit(OWLDataExactCardinality classExpression) {
		long propertyPointer = property(classExpression.getProperty());
		long filler = dataRange(classExpression.getFiller());
		return Long.valueOf(required(mBuilder.buildOWLDataExactCardinality(mBridge, propertyPointer, filler,
				classExpression.getCardinality()), classExpression));
	}


	// ----------------------------------------------- the property expressions

	@Override
	public Long visit(OWLObjectProperty property) {
		Long known = mEntityPointers.get(property);
		if (known != null) {
			return known;
		}
		long pointer = required(mBuilder.buildOWLObjectProperty(mBridge,
				property.getIRI().toString(), property), property);
		return rememberEntity(property, pointer);
	}

	@Override
	public Long visit(OWLObjectInverseOf property) {
		long inverted = property(property.getInverse());
		return Long.valueOf(required(mBuilder.buildOWLObjectInverseOf(mBridge, inverted), property));
	}

	@Override
	public Long visit(OWLDataProperty property) {
		Long known = mEntityPointers.get(property);
		if (known != null) {
			return known;
		}
		long pointer = required(mBuilder.buildOWLDataProperty(mBridge,
				property.getIRI().toString(), property), property);
		return rememberEntity(property, pointer);
	}

	@Override
	public Long visit(OWLAnnotationProperty property) {
		// annotations are dropped, an annotation property cannot occur in a logical axiom
		throw new KoncludeUnsupportedConstructException(property, "annotations are not translated");
	}


	// ------------------------------------------------------- the individuals

	@Override
	public Long visit(OWLNamedIndividual individual) {
		Long known = mEntityPointers.get(individual);
		if (known != null) {
			return known;
		}
		long pointer = required(mBuilder.buildOWLNamedIndividual(mBridge,
				individual.getIRI().toString(), individual), individual);
		return rememberEntity(individual, pointer);
	}

	@Override
	public Long visit(OWLAnonymousIndividual individual) {
		Long known = mEntityPointers.get(individual);
		if (known != null) {
			return known;
		}
		// the native side prepends an empty namespace, so the node identifier is enough
		long pointer = required(mBuilder.buildOWLAnonymousIndividual(mBridge,
				individual.getID().getID(), individual), individual);
		// an anonymous individual is not declared, it is not an entity
		mEntityPointers.put(individual, Long.valueOf(pointer));
		return Long.valueOf(pointer);
	}


	// ------------------------------------------ the data ranges and literals

	@Override
	public Long visit(OWLDatatype datatype) {
		Long known = mEntityPointers.get(datatype);
		if (known != null) {
			return known;
		}
		long pointer = required(mBuilder.buildOWLDatatype(mBridge,
				datatype.getIRI().toString(), datatype), datatype);
		return rememberEntity(datatype, pointer);
	}

	@Override
	public Long visit(OWLLiteral literal) {
		long datatype = unbox(literal.getDatatype().accept((OWLDataVisitorEx<Long>) this));
		// the language tag is lost, the bridge takes a lexical form and a datatype only
		return Long.valueOf(required(mBuilder.buildOWLLiteral(mBridge, datatype,
				literal.getLiteral(), literal), literal));
	}

	@Override
	public Long visit(OWLDataOneOf dataRange) {
		pushArguments(literals(dataRange.getValues()));
		return Long.valueOf(required(mBuilder.buildOWLDataOneOfFromArgumentList(mBridge), dataRange));
	}

	@Override
	public Long visit(OWLDataComplementOf dataRange) {
		long operand = dataRange(dataRange.getDataRange());
		return Long.valueOf(required(mBuilder.buildOWLDataComplementOf(mBridge, operand), dataRange));
	}

	@Override
	public Long visit(OWLDataIntersectionOf dataRange) {
		pushArguments(dataRanges(dataRange.getOperands()));
		return Long.valueOf(required(mBuilder.buildOWLDataIntersectionOfFromArgumentList(mBridge), dataRange));
	}

	@Override
	public Long visit(OWLDataUnionOf dataRange) {
		pushArguments(dataRanges(dataRange.getOperands()));
		return Long.valueOf(required(mBuilder.buildOWLDataUnionOfFromArgumentList(mBridge), dataRange));
	}

	@Override
	public Long visit(OWLDatatypeRestriction dataRange) {
		long datatype = dataRange(dataRange.getDatatype());
		pushArguments(facetRestrictions(dataRange.getFacetRestrictions()));
		return Long.valueOf(required(mBuilder.buildOWLDatatypeRestrictionFromArgumentList(mBridge, datatype), dataRange));
	}

	@Override
	public Long visit(OWLFacetRestriction facetRestriction) {
		OWLFacet facet = facetRestriction.getFacet();
		long value = literal(facetRestriction.getFacetValue());
		return Long.valueOf(required(mBuilder.buildOWLFacetRestriction(mBridge,
				facet.getIRI().toString(), facet, value), facetRestriction));
	}


	// ------------------------------------------------------------- the helpers

	/**
	 * Builds the given expressions and pushes them as an argument list, which the following
	 * ...FromArgumentList call pops. Every expression is built before the list is pushed, so
	 * that a nested list cannot end up in the wrong list.
	 */
	private void pushArguments(long[] arguments) {
		mBuilder.initArumentList(mBridge);
		for (int i = 0; i < arguments.length; ++i) {
			mBuilder.addArgument(mBridge, arguments[i]);
		}
	}

	private long[] classExpressions(Collection<? extends OWLClassExpression> expressions) {
		long[] pointers = new long[expressions.size()];
		int index = 0;
		for (OWLClassExpression expression : expressions) {
			pointers[index++] = classExpression(expression);
		}
		return pointers;
	}

	private long[] properties(Collection<? extends OWLPropertyExpression> expressions) {
		long[] pointers = new long[expressions.size()];
		int index = 0;
		for (OWLPropertyExpression expression : expressions) {
			pointers[index++] = property(expression);
		}
		return pointers;
	}

	private long[] individuals(Collection<? extends OWLIndividual> expressions) {
		long[] pointers = new long[expressions.size()];
		int index = 0;
		for (OWLIndividual expression : expressions) {
			pointers[index++] = individual(expression);
		}
		return pointers;
	}

	private long[] dataRanges(Collection<? extends OWLDataRange> expressions) {
		long[] pointers = new long[expressions.size()];
		int index = 0;
		for (OWLDataRange expression : expressions) {
			pointers[index++] = dataRange(expression);
		}
		return pointers;
	}

	private long[] facetRestrictions(Collection<? extends OWLFacetRestriction> expressions) {
		long[] pointers = new long[expressions.size()];
		int index = 0;
		for (OWLFacetRestriction expression : expressions) {
			pointers[index++] = unbox(expression.accept((OWLDataVisitorEx<Long>) this));
		}
		return pointers;
	}

	private long[] literals(Collection<? extends OWLLiteral> expressions) {
		long[] pointers = new long[expressions.size()];
		int index = 0;
		for (OWLLiteral expression : expressions) {
			pointers[index++] = literal(expression);
		}
		return pointers;
	}

	private long classExpression(OWLClassExpression expression) {
		return unbox(expression.accept((OWLClassExpressionVisitorEx<Long>) this));
	}

	private long property(OWLPropertyExpression expression) {
		return unbox(expression.accept((OWLPropertyExpressionVisitorEx<Long>) this));
	}

	private long individual(OWLIndividual expression) {
		return unbox(expression.accept((OWLIndividualVisitorEx<Long>) this));
	}

	private long dataRange(OWLDataRange expression) {
		return unbox(expression.accept((OWLDataVisitorEx<Long>) this));
	}

	private long literal(OWLLiteral expression) {
		return unbox(expression.accept((OWLDataVisitorEx<Long>) this));
	}

	/** remembers the address of an entity and declares it if declarations are switched on */
	private Long rememberEntity(OWLEntity entity, long pointer) {
		Long boxed = Long.valueOf(pointer);
		mEntityPointers.put(entity, boxed);
		if (mDeclareEntities && !entity.isBuiltIn()) {
			mBuilder.buildOWLDeclarationAxiom(mBridge, pointer);
		}
		return boxed;
	}

	/**
	 * The native side answers with 0 if it could not build the expression, which happens if
	 * the bridge has not been initialised. Reporting it here keeps a 0 from being passed on
	 * as an address.
	 */
	private long required(long pointer, OWLObject source) {
		if (pointer == 0) {
			throw new IllegalStateException("the bridge did not build an expression for '" + source
					+ "', is initAxiomExpressionVisitingBridge missing?");
		}
		return pointer;
	}

	private static long unbox(Long value) {
		return value == null ? 0 : value.longValue();
	}
}
