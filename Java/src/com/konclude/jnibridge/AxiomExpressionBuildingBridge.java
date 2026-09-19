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
 * ATTENTION: this is only the part of the bridge that the smoke test requires. The native
 * side provides 66 entry points, the remaining ones still have to be declared here before
 * they can be used. The declarations of the missing methods can be derived from
 * com_konclude_jnibridge_AxiomExpressionBuildingBridge.h, where the first jobject after the
 * JNIEnv is this builder and the second one is the KoncludeReasonerBridge.
 *
 * The returned long values are the addresses of the built expressions, they are passed back
 * to the bridge to compose larger expressions and axioms. Axioms are told to the ontology
 * while they are built, so the return value of an axiom building method is usually not
 * needed.
 *
 * The object that is handed over together with an entity is remembered by the native side
 * and returned by the queries, which is how an entity of a client, an OWL API OWLClass for
 * instance, finds its way back into the query results.
 */
public class AxiomExpressionBuildingBridge {

	/** written by the native side, holds the CJNIAxiomExpressionVisitingLoader of this builder */
	private long mNativeDataPointer = 0;

	/** the native side instantiates this class, so the parameterless constructor is required */
	public AxiomExpressionBuildingBridge() {
	}

	public native long buildOWLClass(KoncludeReasonerBridge bridge, String iri, Object classObject);

	public native long buildOWLDeclarationAxiom(KoncludeReasonerBridge bridge, long entity);

	public native long buildOWLSubClassOfAxiom(KoncludeReasonerBridge bridge, long subClass, long superClass);

	/** only of interest for the smoke test, to show that the native side filled the field */
	public long getNativeDataPointer() {
		return mNativeDataPointer;
	}
}
