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

#include "CSaturationSuccessorFUNCTIONALConceptExtensionData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSaturationSuccessorFUNCTIONALConceptExtensionData::CSaturationSuccessorFUNCTIONALConceptExtensionData(CProcessContext* processContext) : CLinkerBase<bool,CSaturationSuccessorFUNCTIONALConceptExtensionData>(false) {
					mProcessContext = processContext;
				}

				CSaturationSuccessorFUNCTIONALConceptExtensionData* CSaturationSuccessorFUNCTIONALConceptExtensionData::initSuccessorConceptExtensionData(CRole* role) {
					setData(false);
					setNext(nullptr);
					mRole = role;
					mLastResolvedIndiProcSatNode = nullptr;
					mLastExaminedLinkedSucc = nullptr;
					return this;
				}

				bool CSaturationSuccessorFUNCTIONALConceptExtensionData::isExtensionProcessingQueued() {
					return getData();
				}

				CSaturationSuccessorFUNCTIONALConceptExtensionData* CSaturationSuccessorFUNCTIONALConceptExtensionData::setExtensionProcessingQueued(bool queued) {
					setData(queued);
					return this;
				}


				CRole* CSaturationSuccessorFUNCTIONALConceptExtensionData::getRole() {
					return mRole;
				}


				CIndividualSaturationProcessNode* CSaturationSuccessorFUNCTIONALConceptExtensionData::getLastResolvedIndividualNode() {
					return mLastResolvedIndiProcSatNode;
				}

				CSaturationSuccessorFUNCTIONALConceptExtensionData* CSaturationSuccessorFUNCTIONALConceptExtensionData::setLastResolvedIndividualNode(CIndividualSaturationProcessNode* indiNode) {
					mLastResolvedIndiProcSatNode = indiNode;
					return this;
				}

				CSaturationSuccessorData* CSaturationSuccessorFUNCTIONALConceptExtensionData::getLastExaminedLinkedSuccessorData() {
					return mLastExaminedLinkedSucc;
				}

				CSaturationSuccessorFUNCTIONALConceptExtensionData* CSaturationSuccessorFUNCTIONALConceptExtensionData::setLastExaminedLinkedSuccessorData(CSaturationSuccessorData* lastExaminedLinkedSucc) {
					mLastExaminedLinkedSucc = lastExaminedLinkedSucc;
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
