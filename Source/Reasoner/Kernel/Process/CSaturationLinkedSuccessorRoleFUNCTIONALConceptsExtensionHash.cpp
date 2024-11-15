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

#include "CSaturationLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSaturationLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash::CSaturationLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash(CProcessContext* processContext) : mProcessContext(processContext),mLinkedSuccRoleFUNCTIONALConceptExtHash(processContext) {
				}

				CSaturationLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash* CSaturationLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash::initLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash() {
					mLinkedSuccRoleFUNCTIONALConceptExtHash.clear();
					return this;
				}

				CPROCESSHASH<CRole*,CSaturationSuccessorFUNCTIONALConceptExtensionData*>* CSaturationLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash::getLinkedSuccessorIndividualFUNCTIONALConceptsExtensionHash() {
					return &mLinkedSuccRoleFUNCTIONALConceptExtHash;
				}

				CSaturationSuccessorFUNCTIONALConceptExtensionData* CSaturationLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash::getSuccessorFunctionalConceptsExtensionData(CRole* role, bool create) {
					if (create) {
						CSaturationSuccessorFUNCTIONALConceptExtensionData*& data = mLinkedSuccRoleFUNCTIONALConceptExtHash[role];
						if (!data) {
							data = CObjectParameterizingAllocator< CSaturationSuccessorFUNCTIONALConceptExtensionData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
							data->initSuccessorConceptExtensionData(role);
						}
						return data;
					} else {
						return mLinkedSuccRoleFUNCTIONALConceptExtHash.value(role,nullptr);
					}
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
