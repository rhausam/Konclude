/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CSaturationPredecessorRoleFUNCTIONALConceptsExtensionHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSaturationPredecessorRoleFUNCTIONALConceptsExtensionHash::CSaturationPredecessorRoleFUNCTIONALConceptsExtensionHash(CProcessContext* processContext) : mProcessContext(processContext),mLinkedPredecessorRoleFUNCTIONALConceptExtHash(processContext) {
				}

				CSaturationPredecessorRoleFUNCTIONALConceptsExtensionHash* CSaturationPredecessorRoleFUNCTIONALConceptsExtensionHash::initLinkedPredecessorRoleFUNCTIONALConceptsExtensionHash() {
					mLinkedPredecessorRoleFUNCTIONALConceptExtHash.clear();
					return this;
				}

				CPROCESSHASH<CRole*,CSaturationPredecessorFUNCTIONALConceptExtensionData*>* CSaturationPredecessorRoleFUNCTIONALConceptsExtensionHash::getLinkedPredecessorIndividualFUNCTIONALConceptsExtensionHash() {
					return &mLinkedPredecessorRoleFUNCTIONALConceptExtHash;
				}

				CSaturationPredecessorFUNCTIONALConceptExtensionData* CSaturationPredecessorRoleFUNCTIONALConceptsExtensionHash::getPredecessorFunctionalConceptsExtensionData(CRole* role, bool create) {
					if (create) {
						CSaturationPredecessorFUNCTIONALConceptExtensionData*& data = mLinkedPredecessorRoleFUNCTIONALConceptExtHash[role];
						if (!data) {
							data = CObjectParameterizingAllocator< CSaturationPredecessorFUNCTIONALConceptExtensionData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
							data->initPredecessorMergedConceptExtensionData(role);
						}
						return data;
					} else {
						return mLinkedPredecessorRoleFUNCTIONALConceptExtHash.value(role,nullptr);
					}
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude