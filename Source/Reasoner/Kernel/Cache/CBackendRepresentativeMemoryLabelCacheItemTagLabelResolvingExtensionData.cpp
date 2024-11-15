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

#include "CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData::CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData(CContext* context) : CBackendRepresentativeMemoryLabelCacheItemExtensionData(context) {
					mCacheItemExtensionType = TAG_RESOLVING_HASH;
				}



				CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData* CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData::initTagLabelResolvingExtensionData() {
					mTagLabelResolvingDataLinkerHash = CObjectParameterizingAllocator< CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingDataLinker*>, CContext* >::allocateAndConstructAndParameterize(mContext->getMemoryAllocationManager(), mContext);
					return this;
				}



				CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingDataLinker* CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData::getTagLabelResolvingDataLinker(cint64 tag) {
					return mTagLabelResolvingDataLinkerHash->value(tag);
				}

				CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData* CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData::appendTagLabelResolvingDataLinker(cint64 tag, CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingDataLinker* linker) {
					CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingDataLinker*& exLinker = (*mTagLabelResolvingDataLinkerHash)[tag];
					exLinker = linker->append(exLinker);
					return this;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
