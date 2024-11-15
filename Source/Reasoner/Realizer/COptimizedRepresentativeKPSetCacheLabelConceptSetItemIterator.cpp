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

#include "COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator::COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator(const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemKnownInstancesList, const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemPossibleInstancesList, 
					COptimizedKPSetIndividualInstantiatedItemMultiHash* individualInstantiatedItemHash, CIndividualVector* individualVector, COptimizedKPSetConceptInstancesItem* conceptInstancesItem, bool onlyMostSpecificInstances, const CRealizationIndividualSorting& sorting, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor, CBackendRepresentativeMemoryCacheReader* backendAssocCacheReader)

				: COptimizedRepresentativeKPSetCacheLabelItemIterator(labelCacheItemKnownInstancesList, labelCacheItemPossibleInstancesList, individualInstantiatedItemHash, individualVector, sorting, indiInstItemRefCursor, moveOverCursor) {

				TConceptInstancesItemMostSpecificFlagPair conItemMostSpecFlagPair(conceptInstancesItem, onlyMostSpecificInstances);
				QSet<TConceptInstancesItemMostSpecificFlagPair> conItemMostSpecFlagPairSet;
				conItemMostSpecFlagPairSet.insert(conItemMostSpecFlagPair);
				for (CBackendRepresentativeMemoryLabelCacheItem* labelItem : labelCacheItemPossibleInstancesList) {
					mPossibleLabelConceptInstancesItemOnlyMostSpecificFlagPairSetHash.insert(labelItem, conItemMostSpecFlagPairSet);
				}
				mConceptInstancesItemOnlyMostSpecificFlagPairSet.insert(conItemMostSpecFlagPair);
				mBackendAssocCacheReader = backendAssocCacheReader;

				init();
			}



			COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator::COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator(const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemKnownInstancesList, const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemPossibleInstancesList,
				COptimizedKPSetIndividualInstantiatedItemMultiHash* individualInstantiatedItemHash, CIndividualVector* individualVector, const QHash<CBackendRepresentativeMemoryLabelCacheItem*, QSet<TConceptInstancesItemMostSpecificFlagPair>>& possibleLabelConceptInstancesItemOnlyMostSpecificFlagPairSetHash, const QSet<TConceptInstancesItemMostSpecificFlagPair>& onceptInstancesItemOnlyMostSpecificFlagPairSet, const CRealizationIndividualSorting& sorting, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor, CBackendRepresentativeMemoryCacheReader* backendAssocCacheReader)

				: mPossibleLabelConceptInstancesItemOnlyMostSpecificFlagPairSetHash(possibleLabelConceptInstancesItemOnlyMostSpecificFlagPairSetHash), mConceptInstancesItemOnlyMostSpecificFlagPairSet(onceptInstancesItemOnlyMostSpecificFlagPairSet), COptimizedRepresentativeKPSetCacheLabelItemIterator(labelCacheItemKnownInstancesList, labelCacheItemPossibleInstancesList, individualInstantiatedItemHash, individualVector, sorting, indiInstItemRefCursor, moveOverCursor) {

				mBackendAssocCacheReader = backendAssocCacheReader;
				init();
			}


			QHash<CBackendRepresentativeMemoryLabelCacheItem*, QSet<TConceptInstancesItemMostSpecificFlagPair>>* COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator::getPossibleLabelConceptInstancesItemOnlyMostSpecificFlagPairSetHash() {
				return &mPossibleLabelConceptInstancesItemOnlyMostSpecificFlagPairSetHash;
			}

			QSet<TConceptInstancesItemMostSpecificFlagPair>* COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator::getConceptInstancesItemOnlyMostSpecificFlagPairSet() {
				return &mConceptInstancesItemOnlyMostSpecificFlagPairSet;
			}

			QSet<TConceptInstancesItemMostSpecificFlagPair>* COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator::currentConceptInstancesItemOnlyMostSpecificFlagPairSet() {
				if (mCurrentIteratorData) {
					CAssociationMapIteratorDataWithConceptInstancesItems* conceptInstanceItemsIteratorData = (CAssociationMapIteratorDataWithConceptInstancesItems*)mCurrentIteratorData;
					return &conceptInstanceItemsIteratorData->mConceptInstancesItemOnlyMostSpecificFlagPairSet;
				} else {
					return nullptr;
				}
			} 



			COptimizedRepresentativeKPSetCacheLabelItemIterator::CAssociationMapIteratorData* COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator::createAssociationMapIteratorData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, const CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator& iterator, bool possibleInstances) {
				CAssociationMapIteratorDataWithConceptInstancesItems* iteratorData = new CAssociationMapIteratorDataWithConceptInstancesItems(labelItem, iterator, possibleInstances);
				if (possibleInstances) {
					iteratorData->mConceptInstancesItemOnlyMostSpecificFlagPairSet = mPossibleLabelConceptInstancesItemOnlyMostSpecificFlagPairSetHash.value(labelItem);
				}
				return iteratorData;
			}


			bool COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator::isValidIteratorPosition(CAssociationMapIteratorData* iteratorData, cint64 indiId) {
				CAssociationMapIteratorDataWithConceptInstancesItems* conceptInstanceItemsIteratorData = (CAssociationMapIteratorDataWithConceptInstancesItems*)iteratorData;
				++mAssociatedIndiPossibleVisistedCount;

				if (!iteratorData->mCurrentIndiRealizationItem) {
					if (conceptInstanceItemsIteratorData->mConceptInstancesItemOnlyMostSpecificFlagPairSet.isEmpty()) {
						--mAssociatedIndiPossibleVisistedCount;
					} else {
						++mAssociatedIndiPossibleConfirmedCount;
					}
					return true;
				}
				if (mConsiderSameMergedIndis || (!iteratorData->mCurrentIndiRealizationItem->isItemSameIndividualMerged() && indiId == iteratorData->mCurrentIndiRealizationItem->getIndividualId())) {



					if (conceptInstanceItemsIteratorData->mConceptInstancesItemOnlyMostSpecificFlagPairSet.isEmpty()) {
						--mAssociatedIndiPossibleVisistedCount;
						return true;
					} else {
						bool validAsPossibleInstanceOnly = false;
						bool validAsKnownInstance = false;
						bool validInstance = true;
						for (TConceptInstancesItemMostSpecificFlagPair conceptInstancesItemMostSpecificPair : conceptInstanceItemsIteratorData->mConceptInstancesItemOnlyMostSpecificFlagPairSet) {
							COptimizedKPSetConceptInstancesItem* conceptInstancesItem = conceptInstancesItemMostSpecificPair.first;
							bool onlyMostSpecificInstances = conceptInstancesItemMostSpecificPair.second;

							COptimizedKPSetConceptInstancesData* conceptInstanceData = iteratorData->mCurrentIndiRealizationItem->getKnownPossibleInstancesHash()->getInstanceItemData(conceptInstancesItem);
							if (conceptInstanceData->mKnownInstance && (!onlyMostSpecificInstances || conceptInstanceData->mMostSpecific)) {
								validAsKnownInstance = true;
							} else if (mAllowPossibleInstances) {
								validAsPossibleInstanceOnly = true;
							} else if (conceptInstanceData->mPossibleInstance && !conceptInstanceData->mTestedInstance && !conceptInstanceData->mDerived) {
								--mAssociatedIndiPossibleVisistedCount;
								iteratorData->mInitializationRequired = true;
								validInstance = false;
								return false;
							} else {
								validInstance = false;
							}

						}


						if (validInstance) {
							if (validAsPossibleInstanceOnly) {
								++mAssociatedIndiPossibleConfirmedCount;
								iteratorData->mCurrentInstanceOnlyPossible = true;
								return true;
							} else if (validAsKnownInstance) {
								++mAssociatedIndiPossibleConfirmedCount;
								return true;
							}
						}
					}


				}
				return false;
			}


			CRealizationIndividualInstanceItemReferenceIterator* COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator::getCopy() {
				COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator* iteratorCopy = new COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator(QList<CBackendRepresentativeMemoryLabelCacheItem*>(), QList<CBackendRepresentativeMemoryLabelCacheItem*>(), mIndividualInstantiatedItemHash, mIndividualVector, mPossibleLabelConceptInstancesItemOnlyMostSpecificFlagPairSetHash, mConceptInstancesItemOnlyMostSpecificFlagPairSet, mSorting, mIndiInstItemRefCursor, mMoveOverCursor, mBackendAssocCacheReader);
				this->initCopy(iteratorCopy);
				return iteratorCopy;
			}

			CRealizationIndividualInstanceItemReferenceIterator* COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator::initCopy(CRealizationIndividualInstanceItemReferenceIterator* iterator) {
				COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator* cacheLabelItemIterator = (COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator*)iterator;
				COptimizedRepresentativeKPSetCacheLabelItemIterator::initCopy(cacheLabelItemIterator);
				return cacheLabelItemIterator;
			}


		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
