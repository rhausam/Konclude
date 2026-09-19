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
 * ATTENTION: this is only the part of the bridge that the smoke test requires. The native
 * side provides 12 entry points, namely
 *
 *   checkIsOWLOntologyConsistent
 *   queryOWLSubClasses, queryOWLSuperClasses, queryOWLEquivalentClasses
 *   queryOWLSubObjectProperties, queryOWLSuperObjectProperties, queryOWLEquivalentObjectProperties
 *   queryOWLInstances, queryOWLTypes, queryOWLSameIndividuals
 *   queryOWLObjectPropertySources, queryOWLObjectPropertyTargets
 *
 * so the queries of the data property hierarchy and several other questions of the OWL API
 * OWLReasoner interface are not provided by the native side yet.
 *
 * The results are not returned, they are reported to the callback listener, which receives
 * the objects that were handed over while the entities were built.
 */
public class QueryingBridge {

	/** written by the native side, holds the CJNIQueryProcessor of this bridge */
	private long mNativeDataPointer = 0;

	/** the native side instantiates this class, so the parameterless constructor is required */
	public QueryingBridge() {
	}

	public native void queryOWLSubClasses(KoncludeReasonerBridge bridge, String iri, Object classObject,
			SetOfObjectSetCallbackListener callback, boolean direct);

	/** only of interest for the smoke test, to show that the native side filled the field */
	public long getNativeDataPointer() {
		return mNativeDataPointer;
	}
}
