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

#include "CPropagationBinding.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CPropagationBinding::CPropagationBinding() {
				}

				cint64 CPropagationBinding::getPropagationID() {
					return mPropID;
				}

				CPropagationBinding* CPropagationBinding::setPropagationID(cint64 propID) {
					mPropID = propID;
					return this;
				}

				CPropagationBinding* CPropagationBinding::initPropagationBinding(cint64 propID, CDependencyTrackPoint* dependencyTrackPoint, CIndividualProcessNode* indi, CConceptDescriptor* conDes, CVariable* variable) {
					initDependencyTracker(dependencyTrackPoint);
					mVariable = variable;
					mIndiNode = indi;
					mPropID = propID;
					mConDes = conDes;
					return this;
				}

				CVariable* CPropagationBinding::getBindedVariable() {
					return mVariable;
				}

				CPropagationBinding* CPropagationBinding::setBindedVariable(CVariable* variable) {
					mVariable = variable;
					return this;
				}

				CIndividualProcessNode* CPropagationBinding::getBindedIndividual() {
					return mIndiNode;
				}

				CPropagationBinding* CPropagationBinding::setBindedIndividual(CIndividualProcessNode* indi) {
					mIndiNode = indi;
					return this;
				}

				CConceptDescriptor* CPropagationBinding::getBindedConceptDescriptor() {
					return mConDes;
				}

				CPropagationBinding* CPropagationBinding::setBindedConceptDescriptor(CConceptDescriptor* conDes) {
					mConDes = conDes;
					return this;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
