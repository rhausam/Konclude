/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem::CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem(CBackendRepresentativeMemoryCacheContext* context) : mContext(context) {
					mSignature = 0;
					mCardinalityItemCount = 0;
					mCardinalityCachetemLinker = nullptr;
				}


				CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem* CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem::appendCardinalityCacheItem(CBackendRepresentativeMemoryCardinalityCacheItem* linker) {
					mCardinalityItemCount += linker->getCount();
					mCardinalityCachetemLinker = linker->append(mCardinalityCachetemLinker);
					return this;
				}

				CBackendRepresentativeMemoryCardinalityCacheItem* CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem::getCardinalityCacheItems() {
					return mCardinalityCachetemLinker;
				}

				cint64 CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem::getCardinalityCacheItemCount() {
					return mCardinalityItemCount;
				}

				cint64 CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem::getSignature() {
					return mSignature;
				}

				CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem* CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem::setSignature(cint64 signature) {
					mSignature = signature;
					return this;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
