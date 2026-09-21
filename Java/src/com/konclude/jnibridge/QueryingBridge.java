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
 * Asks the native reasoner, corresponds to
 * Source/Control/Interface/JNI/com_konclude_jnibridge_QueryingBridge.cpp.
 *
 * All 12 entry points that the native side declares are declared here.
 *
 * An entity is identified by its IRI, the object beside it is only handed over so that the
 * native side can remember it if the entity has not been built before. The results are not
 * returned, they are reported to the callback listener, which receives the objects that were
 * handed over while the entities were built.
 *
 * The hierarchy queries report a set per set of equivalent entities and therefore take a
 * SetOfObjectSetCallbackListener, the queries that answer with a flat set take an
 * ObjectSetCallbackListener. A reported set can be empty, for instance for the bottom node
 * if no object has been associated with owl:Nothing while the ontology was built.
 *
 *
 * WHAT THE NATIVE SIDE DOES NOT ANSWER
 *
 * The 12 queries do not cover the OWL API OWLReasoner interface. Missing are the hierarchy
 * of the data properties, the disjoint classes and properties, the domains and the ranges of
 * properties, the inverse properties, the different individuals, the data property values
 * and the entailment check, so the native side has to be extended for them.
 */
public class QueryingBridge {

	/** written by the native side, holds the CJNIQueryProcessor of this bridge */
	private long mNativeDataPointer = 0;

	/** the native side instantiates this class, so the parameterless constructor is required */
	public QueryingBridge() {
	}


	// ----------------------------------------------------------- consistency

	public native boolean checkIsOWLOntologyConsistent(KoncludeReasonerBridge bridge);


	// ------------------------------------------------------- class hierarchy

	public native void queryOWLSubClasses(KoncludeReasonerBridge bridge, String iri, Object classObject,
			SetOfObjectSetCallbackListener callback, boolean direct);

	public native void queryOWLSuperClasses(KoncludeReasonerBridge bridge, String iri, Object classObject,
			SetOfObjectSetCallbackListener callback, boolean direct);

	public native void queryOWLEquivalentClasses(KoncludeReasonerBridge bridge, String iri, Object classObject,
			ObjectSetCallbackListener callback);


	// --------------------------------------------- object property hierarchy

	/**
	 * ATTENTION: the native side accepts the inverses flag but ignores it, it calls
	 * queryOntologySubObjectProperties with the IRI, the direct flag and the callback only.
	 */
	public native void queryOWLSubObjectProperties(KoncludeReasonerBridge bridge, String iri, Object propertyObject,
			SetOfObjectSetCallbackListener callback, boolean direct, boolean inverses);

	/** ATTENTION: the native side ignores the inverses flag, see queryOWLSubObjectProperties */
	public native void queryOWLSuperObjectProperties(KoncludeReasonerBridge bridge, String iri, Object propertyObject,
			SetOfObjectSetCallbackListener callback, boolean direct, boolean inverses);

	public native void queryOWLEquivalentObjectProperties(KoncludeReasonerBridge bridge, String iri, Object propertyObject,
			ObjectSetCallbackListener callback);


	// --------------------------------------------------------------- ABox

	public native void queryOWLInstances(KoncludeReasonerBridge bridge, String iri, Object classObject,
			SetOfObjectSetCallbackListener callback, boolean direct);

	public native void queryOWLTypes(KoncludeReasonerBridge bridge, String iri, Object individualObject,
			SetOfObjectSetCallbackListener callback, boolean direct);

	/**
	 * The individuals that are the same as the given one, a flat set, so this takes an
	 * ObjectSetCallbackListener like the other queries that answer with one set. The generated
	 * header declared a SetOfObjectSetCallbackListener here while the implementation has always
	 * looked up visitObject on ObjectSetCallbackListener, which terminated the virtual machine
	 * as soon as the query reported anything.
	 */
	public native void queryOWLSameIndividuals(KoncludeReasonerBridge bridge, String iri, Object individualObject,
			ObjectSetCallbackListener callback);

	/** the individuals that are related to the given one by the given property */
	public native void queryOWLObjectPropertySources(KoncludeReasonerBridge bridge,
			String individualIRI, Object individualObject, String propertyIRI, Object propertyObject,
			SetOfObjectSetCallbackListener callback);

	/** the individuals that the given one is related to by the given property */
	public native void queryOWLObjectPropertyTargets(KoncludeReasonerBridge bridge,
			String individualIRI, Object individualObject, String propertyIRI, Object propertyObject,
			SetOfObjectSetCallbackListener callback);


	/** only of interest for the smoke test, to show that the native side filled the field */
	public long getNativeDataPointer() {
		return mNativeDataPointer;
	}
}
