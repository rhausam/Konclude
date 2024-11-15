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

#include "COptimizedRepresentativeKPSetCacheMultipleTypeLabelSetItemIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedRepresentativeKPSetCacheMultipleTypeLabelSetItemIterator::COptimizedRepresentativeKPSetCacheMultipleTypeLabelSetItemIterator(const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemKnownInstancesList, COptimizedKPSetIndividualInstantiatedItemMultiHash* individualInstantiatedItemHash, CIndividualVector* individualVector)
				: COptimizedRepresentativeKPSetCacheLabelItemIterator(labelCacheItemKnownInstancesList, QList<CBackendRepresentativeMemoryLabelCacheItem*>(), individualInstantiatedItemHash, individualVector, CRealizationIndividualSorting(CRealizationIndividualSorting::SORT_TYPE_INDIVIDUAL_ID, CRealizationIndividualSorting::SORT_ORDER_ASCENDING, CRealizationIndividualSorting::SORT_GROUP_SAME_UNIQUE), CRealizationIndividualInstanceItemReference()) {


				init();
				mPreventSameIndisMultipleOccurences = true;
			}


			CRealizationIndividualInstanceItemReferenceIterator* COptimizedRepresentativeKPSetCacheMultipleTypeLabelSetItemIterator::getCopy() {
				COptimizedRepresentativeKPSetCacheMultipleTypeLabelSetItemIterator* iteratorCopy = new COptimizedRepresentativeKPSetCacheMultipleTypeLabelSetItemIterator(QList<CBackendRepresentativeMemoryLabelCacheItem*>(), mIndividualInstantiatedItemHash, mIndividualVector);
				this->initCopy(iteratorCopy);
				return iteratorCopy;
			}

			CRealizationIndividualInstanceItemReferenceIterator* COptimizedRepresentativeKPSetCacheMultipleTypeLabelSetItemIterator::initCopy(CRealizationIndividualInstanceItemReferenceIterator* iterator) {
				COptimizedRepresentativeKPSetCacheMultipleTypeLabelSetItemIterator* cacheLabelItemIterator = (COptimizedRepresentativeKPSetCacheMultipleTypeLabelSetItemIterator*)iterator;
				COptimizedRepresentativeKPSetCacheLabelItemIterator::initCopy(cacheLabelItemIterator);
				return cacheLabelItemIterator;
			}


		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
