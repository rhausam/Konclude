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

#include "CConceptRepresentativePropagationSetHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CConceptRepresentativePropagationSetHash::CConceptRepresentativePropagationSetHash(CProcessContext* processContext) : mProcessContext(processContext) {
				}


				CConceptRepresentativePropagationSetHash* CConceptRepresentativePropagationSetHash::initConceptRepresentativePropagationSetHash(CConceptRepresentativePropagationSetHash* prevHash) {
					if (prevHash) {
						CPROCESSHASH<cint64, CConceptRepresentativePropagationSetHashData>::operator =(*prevHash);
					} else {
						CPROCESSHASH<cint64,CConceptRepresentativePropagationSetHashData>::clear();
					}
					return this;
				}


				CRepresentativePropagationSet* CConceptRepresentativePropagationSetHash::getRepresentativePropagationSet(CConcept* concept, bool localize) {
					CRepresentativePropagationSet* repPropSet = nullptr;
					cint64 conTag = concept->getConceptTag();
					if (localize) {
						CConceptRepresentativePropagationSetHashData& data = CPROCESSHASH<cint64,CConceptRepresentativePropagationSetHashData>::operator[](conTag);
						if (!data.mLocRepPropSet) {
							CMemoryAllocationManager* taskMemMan = mProcessContext->getUsedMemoryAllocationManager();
							repPropSet = CObjectParameterizingAllocator< CRepresentativePropagationSet,CProcessContext* >::allocateAndConstructAndParameterize(taskMemMan,mProcessContext);
							repPropSet->initRepresentativePropagationSet(data.mUseRepPropSet);
							data.mUseRepPropSet = data.mLocRepPropSet = repPropSet;
						} else {
							repPropSet = data.mUseRepPropSet;
						}
					} else {
						CConceptRepresentativePropagationSetHashData data = CPROCESSHASH<cint64,CConceptRepresentativePropagationSetHashData>::value(conTag);
						repPropSet = data.mUseRepPropSet;
					}
					return repPropSet;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
