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

#ifndef KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDCLASSEXTRACTEDSATURATIONONTOLOGYCLASSIFICATIONITEM_H
#define KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDCLASSEXTRACTEDSATURATIONONTOLOGYCLASSIFICATIONITEM_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "COntologyClassClassificationItem.h"
#include "COptimizedClassExtractedSaturationSatisfiableTestingItem.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			/*! 
			 *
			 *		\class		COptimizedClassExtractedSaturationOntologyClassificationItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedClassExtractedSaturationOntologyClassificationItem : public COntologyClassClassificationItem {
				// public methods
				public:
					//! Constructor
					COptimizedClassExtractedSaturationOntologyClassificationItem(CConfigurationBase *configuration, CClassifierStatistics *nextClassificationStatistics = 0);


					QHash<CConcept*,COptimizedClassExtractedSaturationSatisfiableTestingItem*>* getClassSatisfiableTestItemHash();
					QList<COptimizedClassExtractedSaturationSatisfiableTestingItem*>* getClassSatisfiableTestItemList();

					COptimizedClassExtractedSaturationSatisfiableTestingItem* getTopConceptSatisfiableTestItem();
					COptimizedClassExtractedSaturationSatisfiableTestingItem* getBottomConceptSatisfiableTestItem();


					COptimizedClassExtractedSaturationOntologyClassificationItem* initTopBottomSatisfiableTestingItems(COptimizedClassExtractedSaturationSatisfiableTestingItem* topItem, COptimizedClassExtractedSaturationSatisfiableTestingItem* bottomItem);


					COptimizedClassExtractedSaturationSatisfiableTestingItem* getClassSatisfiableTestItem(CConcept* satTestConcept, bool create = false);
				

				// protected methods
				protected:

				// protected variables
				protected:
					QHash<CConcept*,COptimizedClassExtractedSaturationSatisfiableTestingItem*> mClassSatItemHash;
					QList<COptimizedClassExtractedSaturationSatisfiableTestingItem*> mClassSatTestItemContainer;
					COptimizedClassExtractedSaturationSatisfiableTestingItem* mTopSatTestItem;
					COptimizedClassExtractedSaturationSatisfiableTestingItem* mBottomSatTestItem;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDCLASSEXTRACTEDSATURATIONONTOLOGYCLASSIFICATIONITEM_H
