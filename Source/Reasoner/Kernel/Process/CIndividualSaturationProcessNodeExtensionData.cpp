/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CIndividualSaturationProcessNodeExtensionData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualSaturationProcessNodeExtensionData::CIndividualSaturationProcessNodeExtensionData(CProcessContext* processContext) : mProcessContext(processContext) {
					mMemAllocMan = mProcessContext->getMemoryAllocationManager();
				}

				CIndividualSaturationProcessNodeExtensionData* CIndividualSaturationProcessNodeExtensionData::initIndividualExtensionData(CIndividualSaturationProcessNode* indiNode) {
					mIndiNode = indiNode;
					mDisComConExtData = nullptr;
					mLinkedRoleSuccHash = nullptr;
					mCriticalConceptTypeQueues = nullptr;
					mSuccessorExtensionData = nullptr;
					mNominalHandlingData = nullptr;
					return this;
				}

				CSaturationDisjunctCommonConceptExtractionData* CIndividualSaturationProcessNodeExtensionData::getDisjunctCommonConceptExtractionData(bool create) {
					if (!mDisComConExtData && create) {
						mDisComConExtData = CObjectParameterizingAllocator< CSaturationDisjunctCommonConceptExtractionData,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mDisComConExtData->initExtractionData(mIndiNode);
					}
					return mDisComConExtData;
				}


				CLinkedRoleSaturationSuccessorHash* CIndividualSaturationProcessNodeExtensionData::getLinkedRoleSuccessorHash(bool create) {
					if (create && !mLinkedRoleSuccHash) {
						mLinkedRoleSuccHash = CObjectParameterizingAllocator< CLinkedRoleSaturationSuccessorHash,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mLinkedRoleSuccHash->initRoleSuccessorHash();
					}
					return mLinkedRoleSuccHash;
				}

				CCriticalSaturationConceptTypeQueues* CIndividualSaturationProcessNodeExtensionData::getCriticalConceptTypeQueues(bool create) {
					if (!mCriticalConceptTypeQueues && create) {
						mCriticalConceptTypeQueues = CObjectParameterizingAllocator< CCriticalSaturationConceptTypeQueues,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mCriticalConceptTypeQueues->initCriticalSaturationConceptQueues(mIndiNode);
					}
					return mCriticalConceptTypeQueues;
				}


				CSaturationIndividualNodeSuccessorExtensionData* CIndividualSaturationProcessNodeExtensionData::getSuccessorExtensionData(bool create) {
					if (!mSuccessorExtensionData && create) {
						mSuccessorExtensionData = CObjectParameterizingAllocator< CSaturationIndividualNodeSuccessorExtensionData,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mSuccessorExtensionData->initExtensionData(mIndiNode);
					}
					return mSuccessorExtensionData;
				}


				CSaturationIndividualNodeNominalHandlingData* CIndividualSaturationProcessNodeExtensionData::getNominalHandlingData(bool create) {
					if (!mNominalHandlingData && create) {
						mNominalHandlingData = CObjectParameterizingAllocator< CSaturationIndividualNodeNominalHandlingData,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mNominalHandlingData->initNominalHandlingData(mIndiNode);
					}
					return mNominalHandlingData;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude