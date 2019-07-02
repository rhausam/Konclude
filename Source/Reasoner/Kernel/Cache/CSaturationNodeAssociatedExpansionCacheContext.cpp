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

#include "CSaturationNodeAssociatedExpansionCacheContext.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CSaturationNodeAssociatedExpansionCacheContext::CSaturationNodeAssociatedExpansionCacheContext() {
					mMemoryPoolProvider = new CNewAllocationMemoryPoolProvider();
					mMemMan = new CLimitedReserveMemoryPoolAllocationManager(mMemoryPoolProvider);
					mAddRelMemory = 0;
					mConSatUpdateLinker = nullptr;
				}


				CSaturationNodeAssociatedExpansionCacheContext::~CSaturationNodeAssociatedExpansionCacheContext() {
					delete mMemMan;
					delete mMemoryPoolProvider;
				}


				CMemoryPoolAllocationManager* CSaturationNodeAssociatedExpansionCacheContext::getMemoryPoolAllocationManager() {
					return mMemMan;
				}


				CMemoryAllocationManager* CSaturationNodeAssociatedExpansionCacheContext::getMemoryAllocationManager() {
					return mMemMan;
				}

				CMemoryPoolProvider* CSaturationNodeAssociatedExpansionCacheContext::getMemoryPoolProvider() {
					return mMemoryPoolProvider;
				}

				cint64 CSaturationNodeAssociatedExpansionCacheContext::getMemoryConsumption() {
					return mAddRelMemory + mMemoryPoolProvider->getAllocatedReleaseDifferencePoolSize();
				}

				CSaturationNodeAssociatedExpansionCacheContext* CSaturationNodeAssociatedExpansionCacheContext::releaseTemporaryMemoryPools(CMemoryPool* memoryPools) {
					CMemoryPool* memoryPoolIt = memoryPools;
					while (memoryPoolIt) {
						mAddRelMemory += memoryPoolIt->getMemoryBlockSize();
						memoryPoolIt = memoryPoolIt->getNext();
					}
					mMemMan->releaseTemporaryMemoryPools(memoryPools);
					return this;
				}


				CIndividualSaturationProcessNodeStatusUpdateLinker* CSaturationNodeAssociatedExpansionCacheContext::getIndividualSaturationUpdateLinker(bool create) {
					CIndividualSaturationProcessNodeStatusUpdateLinker* conSatUpdateLinker = mConSatUpdateLinker;
					if (!conSatUpdateLinker && create) {
						conSatUpdateLinker = CObjectAllocator<CIndividualSaturationProcessNodeStatusUpdateLinker>::allocateAndConstruct(mMemMan);
					}
					conSatUpdateLinker->clearNext();
					if (mConSatUpdateLinker) {
						mConSatUpdateLinker = mConSatUpdateLinker->getNext();
					}
					return conSatUpdateLinker;
				}


				CSaturationNodeAssociatedExpansionCacheContext* CSaturationNodeAssociatedExpansionCacheContext::addIndividualSaturationUpdateLinker(CIndividualSaturationProcessNodeStatusUpdateLinker* linker) {
					if (linker) {
						linker->clearNext();
						linker = linker->append(mConSatUpdateLinker);
					}
					return this;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude