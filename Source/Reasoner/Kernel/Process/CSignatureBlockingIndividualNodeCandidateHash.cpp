/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CSignatureBlockingIndividualNodeCandidateHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSignatureBlockingIndividualNodeCandidateHash::CSignatureBlockingIndividualNodeCandidateHash(CProcessContext* context) : mContext(context) {
					mMemMan = mContext->getUsedMemoryAllocationManager();
					mBlockCandidateHash = nullptr;
					mUseBlockCandidateHash = nullptr;
					mPrevBlockCandidateHash = nullptr;
				}

				CSignatureBlockingIndividualNodeCandidateHash::~CSignatureBlockingIndividualNodeCandidateHash() {
				}


				CSignatureBlockingIndividualNodeCandidateHash* CSignatureBlockingIndividualNodeCandidateHash::initBlockingIndividualNodeCandidateHash(CSignatureBlockingIndividualNodeCandidateHash* prevBlockCandHash) {
					mBlockCandidateHash = nullptr;
					if (prevBlockCandHash) {
						mPrevBlockCandidateHash = prevBlockCandHash->mUseBlockCandidateHash;
						mUseBlockCandidateHash = mPrevBlockCandidateHash;
					} else {
						mPrevBlockCandidateHash = nullptr;
						mUseBlockCandidateHash = nullptr;
					}
					return this;
				}


				CSignatureBlockingIndividualNodeCandidateData* CSignatureBlockingIndividualNodeCandidateHash::getBlockingIndividualCandidateData(cint64 signature, bool create) {
					if (!mUseBlockCandidateHash || !mBlockCandidateHash && create) {
						mBlockCandidateHash = CObjectParameterizingAllocator< CPROCESSHASH<cint64,CSignatureBlockingCandidateHashData>,CContext* >::allocateAndConstructAndParameterize(mMemMan,mContext);
						if (mPrevBlockCandidateHash) {
							*mBlockCandidateHash = *mPrevBlockCandidateHash;
						}
						mUseBlockCandidateHash = mBlockCandidateHash;
					}
					if (create) {
						CSignatureBlockingCandidateHashData& data = (*mUseBlockCandidateHash)[signature];
						if (!data.mCandidateIndiData) {
							data.mCandidateIndiData = CObjectParameterizingAllocator< CSignatureBlockingIndividualNodeCandidateData,CProcessContext* >::allocateAndConstructAndParameterize(mMemMan,mContext);
							data.mCandidateIndiData->initBlockingCandidateData(data.mPrevCandidateIndiData);
							data.mPrevCandidateIndiData = data.mCandidateIndiData;
						}
						return data.mPrevCandidateIndiData;
					} else {
						CSignatureBlockingCandidateHashData* data = nullptr;
						if (mUseBlockCandidateHash->tryGetValuePointer(signature,data)) {
							return data->mPrevCandidateIndiData;
						}
					}
					return nullptr;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
