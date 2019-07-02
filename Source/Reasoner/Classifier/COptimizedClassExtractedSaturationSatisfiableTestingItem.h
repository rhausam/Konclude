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

#ifndef KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDCLASSEXTRACTEDSATURATIONSATISFIABLETESTINGITEM_H
#define KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDCLASSEXTRACTEDSATURATIONSATISFIABLETESTINGITEM_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "CConceptSubsumerObserver.h"
#include "CClassificationConceptReferenceLinking.h"


// Other includes
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptReferenceLinking.h"
#include "Reasoner/Ontology/CConceptSatisfiableReferenceLinkingData.h"
#include "Reasoner/Taxonomy/CHierarchyNode.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;
		using namespace Taxonomy;

		namespace Classifier {


			/*! 
			 *
			 *		\class		COptimizedClassExtractedSaturationSatisfiableTestingItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedClassExtractedSaturationSatisfiableTestingItem : public CClassificationConceptReferenceLinking, public CConceptSubsumerObserver {
				// public methods
				public:
					//! Constructor
					COptimizedClassExtractedSaturationSatisfiableTestingItem();

					COptimizedClassExtractedSaturationSatisfiableTestingItem* initSatisfiableTestingItem(CConcept* satTestConcept);
					
					CConcept* getTestingConcept();

					CHierarchyNode* getSatisfiableConceptHierarchyNode();
					COptimizedClassExtractedSaturationSatisfiableTestingItem* setSatisfiableConceptHierarchyNode(CHierarchyNode* hierNode);

					QList<COptimizedClassExtractedSaturationSatisfiableTestingItem*>* getSubsumingConceptItemList();


					COptimizedClassExtractedSaturationSatisfiableTestingItem* setExtractionSubsumerCount(cint64 subsumerCount);
					cint64 getExtractionSubsumerCount() const;


					COptimizedClassExtractedSaturationSatisfiableTestingItem* setDirectSubsumer(COptimizedClassExtractedSaturationSatisfiableTestingItem* subsumingItem);
					COptimizedClassExtractedSaturationSatisfiableTestingItem* getDirectSubsumer();
					bool hasDirectSubsumer();


					COptimizedClassExtractedSaturationSatisfiableTestingItem* addSubsumingConceptItem(COptimizedClassExtractedSaturationSatisfiableTestingItem* subsumingItem);

					CConceptSubsumerObserver* tellConceptSupsumption(CConcept* subsumedConcept, CConcept* subsumerConcept);


					QList<COptimizedClassExtractedSaturationSatisfiableTestingItem*>* sortSubsumingConceptItemList();

					bool isEquivalentItem();
					COptimizedClassExtractedSaturationSatisfiableTestingItem* setEquivalentItem(bool equivalent);


					bool isPredecessorItem();
					COptimizedClassExtractedSaturationSatisfiableTestingItem* setPredecessorItem(bool predecessorItem);


				// protected methods
				protected:

				// protected variables
				protected:
					CConcept* mTestConcept;
					CHierarchyNode* mConSatHierNode;
					QList<COptimizedClassExtractedSaturationSatisfiableTestingItem*> mSubsumingConceptItemList;
					COptimizedClassExtractedSaturationSatisfiableTestingItem* mDirectSubsumerItem;
					cint64 mExtractedSubsumerCount;
					bool mEquiItem;
					bool mPredecessorItem;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDCLASSEXTRACTEDSATURATIONSATISFIABLETESTINGITEM_H