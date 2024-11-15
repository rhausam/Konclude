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

#include "COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionData) {
				mAbsorptionData = absorptionData;
				mPossibleVariableMapping = nullptr;
				mTestingVariableMapping = nullptr;
				mPropatationTestCreated = false;
				mDependentMappingsComputationScheduled = false;
				mPropagationInitializationIndividualsSplitted = false;
				mPropagationInitializationIndividualsSplittedTestsRemaining = false;
				mNextPropagationInitializationIndividualsSplittedTestId = 1;
				mEntailmentFound = false;
				mCancellationAdapter = nullptr;
				mNextSplitPropagationTestingItemId = 0;
				mSplitPropagationItemProcessingQueued = false;
			}


			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::~COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem() {
			}


			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::COMPOSITION_TYPE COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getCompositionType() {
				return ABSOROPTION_BASED_EXTENSION;
			}


			QHash<CExpressionVariable*, COptimizedComplexVariableCompositionItem*>* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getVariableExpressionCompositionItemHash() {
				return &mVariableExpressionBaseItem;
			}


			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getVariableCompositionItem(CExpressionVariable* variableExpression) {
				return mVariableExpressionBaseItem.value(variableExpression);
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getVariableCompositionItem(CVariable* variable) {
				return mVariableBaseItem.value(variable);
			}

			COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getAbsorptionBasedHandlingData() {
				return mAbsorptionData;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::setVariableExpressionCompositionItem(CExpressionVariable* variableExpression, COptimizedComplexVariableCompositionItem* compositionItem) {
				mVariableExpressionBaseItem.insert(variableExpression, compositionItem);
				return this;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::setVariableCompositionItem(CVariable* variable, COptimizedComplexVariableCompositionItem* compositionItem) {
				mVariableBaseItem.insert(variable, compositionItem);
				return this;
			}

			QSet<CIndividualReference>*& COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getVariableSteeringIndividualBindingSet(CVariable* variable) {
				return mVariableIndividualBindingSet[variable];
			}

			QSet<COptimizedComplexVariableCompositionItem*>* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getIntegratedVariableCompositionItemSet() {
				return &mIntegratedItemSet;
			}

			QList< QPair<COptimizedComplexVariableCompositionItem*, CExpressionVariable*> >* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getExtendingCompositionItemAssociatedVariablePairList() {
				return &mVariableExtendingItemList;
			}

			QHash<CVariable*, cint64>* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getVariableIndexHash() {
				return &mVariableIndexHash;
			}

			COptimizedComplexVariableIndividualMappings* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getPossibleVariableMapping() {
				return mPossibleVariableMapping;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::setPossibleVariableMapping(COptimizedComplexVariableIndividualMappings* possibleVariableMapping) {
				mPossibleVariableMapping = possibleVariableMapping;
				return this;
			}

			QHash<COptimizedComplexVariableIndividualBindingsHasher, COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getTestingVariableMapping() {
				return mTestingVariableMapping;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::setTestingVariableMapping(QHash<COptimizedComplexVariableIndividualBindingsHasher, COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* testingVariableMapping) {
				mTestingVariableMapping = testingVariableMapping;
				return this;
			}

			bool COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::isPropatationTestCreated() {
				return mPropatationTestCreated;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::setPropatationTestCreated(bool created) {
				mPropatationTestCreated = created;
				return this;
			}


			bool COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::isDependentMappingsComputationSchedluled() {
				return mDependentMappingsComputationScheduled;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::setDependentMappingsComputationSchedluled(bool scheduled) {
				mDependentMappingsComputationScheduled = scheduled;
				return this;
			}

			bool COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::isPropagationInitializationIndividualsSplitted() {
				return mPropagationInitializationIndividualsSplitted;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::setPropagationInitializationIndividualsSplitted(bool splitted) {
				mPropagationInitializationIndividualsSplitted = splitted;
				return this;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::addPropagationSteeringController(CAnsweringPropagationSteeringController* propagationSteeringController) {
				mPropagationSteeringControllerSet.insert(propagationSteeringController);
				return this;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::removePropagationSteeringController(CAnsweringPropagationSteeringController* propagationSteeringController) {
				mPropagationSteeringControllerSet.remove(propagationSteeringController);
				return this;
			}


			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::setPropagationInitializationIndividualsSplittedTestsRemaining(bool remainingTests) {
				mPropagationInitializationIndividualsSplittedTestsRemaining = remainingTests;
				return this;
			}

			bool COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::hasRemainingPropagationInitializationIndividualsSplittedTests() {
				return mPropagationInitializationIndividualsSplittedTestsRemaining || !mPropagationSteeringControllerSet.isEmpty() || !mRemainingRepeatedSplitTestingItemList.isEmpty();
			}


			cint64 COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getNextPropagationInitializationIndividualsSplittedTestId(cint64 incCount) {
				cint64 tmp = mNextPropagationInitializationIndividualsSplittedTestId;
				mNextPropagationInitializationIndividualsSplittedTestId += incCount;
				return tmp;
			}


			bool COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::isEntailmentFound() {
				return mEntailmentFound;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::setEntailmentFound(bool entailmentFound) {
				mEntailmentFound = entailmentFound;
				return this;
			}


			CAnsweringSplitEntailmentTestingCancellationAdapter* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getEntailmentTestingCancellationAdapter() {
				if (!mCancellationAdapter) {
					mCancellationAdapter = new CAnsweringSplitEntailmentTestingCancellationAdapter();
				}
				return mCancellationAdapter;
			}


			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::initPreparedSplitPropagationTestControllingItemSize(cint64 preparedSplitSize) {
				mPreparedSplitTestingItemVec.resize(preparedSplitSize);
				return this;
			}

			CAnsweringSplitPropagationTestControllingItem*& COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getPreparedSplitPropagationTestControllingItem(cint64 preparedSplitId) {
				return mPreparedSplitTestingItemVec[preparedSplitId];
			}

			CAnsweringSplitPropagationTestControllingItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getNextPreparedSplitPropagationTestControllingItem() {
				if (!mRemainingPreparedSplitTestingItemList.isEmpty()) {
					return mRemainingPreparedSplitTestingItemList.takeFirst();
				}
				return nullptr;
			}


			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::addReaminingPreparedSplitPropagationTestControllingItem(CAnsweringSplitPropagationTestControllingItem* item) {
				mRemainingPreparedSplitTestingItemList.append(item);
				return this;
			}


			bool COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::hasPreparedSplitPropagationTestControllingItems() {
				return !mPreparedSplitTestingItemVec.isEmpty();
			}

			bool COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::hasRemainingPreparedSplitPropagationTestControllingItems() {
				return !mRemainingPreparedSplitTestingItemList.isEmpty();
			}

			CAnsweringSplitPropagationTestControllingItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getNextSplitPropagationTestControllingItem() {
				CAnsweringSplitPropagationTestControllingItem* item = new CAnsweringSplitPropagationTestControllingItem(mNextSplitPropagationTestingItemId++, mAbsorptionData);
				addSplitPropagationTestingItem(item->getTestingItemId(), item);
				return item;
			}

			CAnsweringSplitPropagationTestControllingItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getSplitPropagationTestingItem(cint64 splitItemId) {
				return mSplitPropagationTestingItemHash.value(splitItemId);
			}

			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::addSplitPropagationTestingItem(cint64 splitItemId, CAnsweringSplitPropagationTestControllingItem* item) {
				mSplitPropagationTestingItemHash.insert(splitItemId, item);
				return this;
			}


			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::addRemainingRepeatedSplitTestingItem(CAnsweringSplitPropagationTestControllingItem* item) {
				mRemainingRepeatedSplitTestingItemList.append(item);
				return this;
			}

			QList<CAnsweringSplitPropagationTestControllingItem*>* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getRemainingRepeatedSplitTestingItemList() {
				return &mRemainingRepeatedSplitTestingItemList;
			}


			bool COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::isSplitPropagationItemProcessingQueued() {
				return mSplitPropagationItemProcessingQueued;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::setSplitPropagationItemProcessingQueued(bool queued) {
				mSplitPropagationItemProcessingQueued = queued;
				return this;
			}


			bool COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::clearComputation() {
				COptimizedComplexVariableCompositionItem::clearComputation();
				CExpressionVariable* initializerVariable = mAbsorptionData->getInitializerVariableExpression();
				CVariable* variable = mAbsorptionData->getVariableExpressionVariableHash()->value(initializerVariable);


				for (QHash<cint64, CAnsweringSplitPropagationTestControllingItem*>::const_iterator it = mSplitPropagationTestingItemHash.constBegin(), itEnd = mSplitPropagationTestingItemHash.constEnd(); it != itEnd; ++it) {
					CAnsweringSplitPropagationTestControllingItem* item = it.value();
					if (item) {
						delete item;
					}
				}
				mRemainingRepeatedSplitTestingItemList.clear();
				mPreparedSplitTestingItemVec.clear();


				if (mTestingVariableMapping) {
					delete mTestingVariableMapping;
					mTestingVariableMapping = nullptr;
				}
				if (mPossibleVariableMapping) {
					delete mPossibleVariableMapping;
					mPossibleVariableMapping = nullptr;
				}

				mPropatationTestCreated = false;


				mDependentMappingsComputationScheduled = false;
				mPropagationInitializationIndividualsSplitted = false;
				mPropagationInitializationIndividualsSplittedTestsRemaining = false;
				mNextPropagationInitializationIndividualsSplittedTestId = 1;
				mEntailmentFound = false;
				if (mCancellationAdapter) {
					delete mCancellationAdapter;
					mCancellationAdapter = nullptr;
				}
				mNextSplitPropagationTestingItemId = 0;
				mSplitPropagationItemProcessingQueued = false;

				return true;
			}

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
