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

#include "CReapplyConceptLabelSetIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {



				CReapplyConceptLabelSetIterator::CReapplyConceptLabelSetIterator(cint64 conceptCount, CConceptDescriptor* conceptDesLinker, const CReapplyConceptLabelHash* conDesDepHash, const CReapplyConceptLabelHash* additionalConDesDepHash, bool skipEmptyConceptDescriptors) 
						: mConceptCount(conceptCount),mConceptDesLinkerIt(conceptDesLinker),mConDesDepHash(conDesDepHash),mAdditionalConDesDepHash(additionalConDesDepHash),mSkipEmptyConceptDescriptors(skipEmptyConceptDescriptors) {

					mKeyIt = mKeyItEnd = nullptr;
					mAddKeyIt = mAddKeyItEnd = nullptr;
					mCurrentKey = -1;
					mCurrentData = nullptr;
					if (mConDesDepHash && mConDesDepHash->count() > 0) {
						mKeyIt = mConDesDepHash->sortedKeys();
						mKeyItEnd = mKeyIt + mConDesDepHash->count();
					}
					if (mAdditionalConDesDepHash && mAdditionalConDesDepHash->count() > 0) {
						mAddKeyIt = mAdditionalConDesDepHash->sortedKeys();
						mAddKeyItEnd = mAddKeyIt + mAdditionalConDesDepHash->count();
					}
					if (!mConceptDesLinkerIt) {
						resolveCurrent();
					}
				}


				void CReapplyConceptLabelSetIterator::advance() {
					if (mKeyIt != mKeyItEnd && mAddKeyIt != mAddKeyItEnd) {
						if (*mKeyIt == *mAddKeyIt) {
							++mKeyIt;
							++mAddKeyIt;
						} else if (*mAddKeyIt < *mKeyIt) {
							++mAddKeyIt;
						} else {
							++mKeyIt;
						}
					} else if (mKeyIt != mKeyItEnd) {
						++mKeyIt;
					} else if (mAddKeyIt != mAddKeyItEnd) {
						++mAddKeyIt;
					}
				}


				void CReapplyConceptLabelSetIterator::resolveCurrent() {
					for (;;) {
						mCurrentData = nullptr;
						if (mKeyIt != mKeyItEnd && (mAddKeyIt == mAddKeyItEnd || *mKeyIt <= *mAddKeyIt)) {
							mCurrentKey = *mKeyIt;
							mConDesDepHash->tryGetValuePointer(mCurrentKey,mCurrentData);
							if (mSkipEmptyConceptDescriptors && mCurrentData && !mCurrentData->mConceptDescriptor && mAddKeyIt != mAddKeyItEnd && *mAddKeyIt == mCurrentKey) {
								// the own entry only carries a reapplication queue, the shared one may carry the descriptor
								CConceptDescriptorDependencyReapplyData* addData = nullptr;
								if (mAdditionalConDesDepHash->tryGetValuePointer(mCurrentKey,addData) && addData->mConceptDescriptor) {
									mCurrentData = addData;
								}
							}
						} else if (mAddKeyIt != mAddKeyItEnd) {
							mCurrentKey = *mAddKeyIt;
							mAdditionalConDesDepHash->tryGetValuePointer(mCurrentKey,mCurrentData);
						} else {
							mCurrentKey = -1;
							return;
						}
						if (mCurrentData && (!mSkipEmptyConceptDescriptors || mCurrentData->mConceptDescriptor)) {
							return;
						}
						advance();
					}
				}



				cint64 CReapplyConceptLabelSetIterator::getRemainingConceptCount() {
					return mConceptCount;
				}


				CReapplyConceptLabelSetIterator& CReapplyConceptLabelSetIterator::operator++() {
					return *moveNext();
				}

				CConceptDescriptor* CReapplyConceptLabelSetIterator::operator*() {
					return getConceptDescriptor();
				}


				CReapplyConceptLabelSetIterator* CReapplyConceptLabelSetIterator::moveNext() {
					if (mConceptDesLinkerIt) {
						mConceptDesLinkerIt = mConceptDesLinkerIt->getNext();
						--mConceptCount;
					} else if (mCurrentData) {
						advance();
						resolveCurrent();
						--mConceptCount;
					}
					return this;
				}

				cint64 CReapplyConceptLabelSetIterator::getDataTag() {
					if (mConceptDesLinkerIt) {
						return mConceptDesLinkerIt->getConceptTag();
					} else {
						return mCurrentKey;
					}
				}


				CConceptDescriptor* CReapplyConceptLabelSetIterator::getConceptDescriptor() {
					if (mConceptDesLinkerIt) {
						return mConceptDesLinkerIt;
					} else if (mCurrentData) {
						return mCurrentData->mConceptDescriptor;
					}
					return nullptr;
				}

				CDependencyTrackPoint* CReapplyConceptLabelSetIterator::getDependencyTrackPoint() {
					if (mConceptDesLinkerIt) {
						return mConceptDesLinkerIt->getDependencyTrackPoint();
					} else {
						CConceptDescriptor* conDes = getConceptDescriptor();
						if (conDes) {
							return conDes->getDependencyTrackPoint();
						} else {
							return nullptr;
						}
					}
				}

				const CCondensedReapplyQueue *CReapplyConceptLabelSetIterator::getPosNegReapplyQueue() {
					if (mConceptDesLinkerIt) {
						return nullptr;
					} else if (mCurrentData) {
						return &mCurrentData->mPosNegReapplyQueue;
					}
					return nullptr;
				}


				CConceptDescriptor* CReapplyConceptLabelSetIterator::next(bool moveToNext) {
					CConceptDescriptor* conDes = getConceptDescriptor();
					if (moveToNext) {
						moveNext();
					}
					return conDes;
				}

				bool CReapplyConceptLabelSetIterator::hasValue() {
					return mConceptDesLinkerIt || mCurrentData;
				}

				bool CReapplyConceptLabelSetIterator::hasNext() {
					return mConceptDesLinkerIt || mCurrentData;
				}

				bool CReapplyConceptLabelSetIterator::operator!=(CReapplyConceptLabelSetIterator& iterator) {
					return !(*this == iterator);
				}

				bool CReapplyConceptLabelSetIterator::operator==(CReapplyConceptLabelSetIterator& iterator) {
					return mConceptDesLinkerIt == iterator.mConceptDesLinkerIt && mKeyIt == iterator.mKeyIt && mAddKeyIt == iterator.mAddKeyIt && mCurrentData == iterator.mCurrentData;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
