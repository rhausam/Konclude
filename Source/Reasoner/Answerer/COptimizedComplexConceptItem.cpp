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

#include "COptimizedComplexConceptItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexConceptItem::COptimizedComplexConceptItem() {
				mConceptItemId = 0;
				mConcept = nullptr;
				mConceptNegation = false;
				mCandidateConcept = nullptr;
				mMarkerConcept = nullptr;
				mPropagationConcept = nullptr;
				mQueuedProcessStep = nullptr;
				mKnownEquivalentClassNode = nullptr;
				mDirectSuperClassNodeSet = nullptr;
				mKnownSuperClassNodeSet = nullptr;
				mPossibleSuperClassNodeSet = nullptr;

				mDirectSubClassNodeSet = nullptr;
				mPossibleSubClassTestingNodeSet = nullptr;
				mPossibleSuperClassNodeTestingList = nullptr;
				mSatisfiable = false;
				mSuperClassComputationInitializing = false;

				mKnownSuperConceptSet = nullptr;
				mPossibleSuperConceptSet = nullptr;

				mDirectSuperClassNodeSet = nullptr;
				mKnownSuperClassNodeSet = nullptr;
				mPossibleSuperClassNodeSet = nullptr;
				mPossibleSubClassNodeTestingList = nullptr;

				mClassTermExpression = nullptr;
				mKnownInstanceSet = nullptr;
				mPossibleInstanceSet = nullptr;

				mPossibleInstanceItemSet = nullptr;
				mPossibleInstanceCandidatePropagationItemSet = nullptr;
				mPossibleInstanceItemSetCandidatePropagationIteratorInitialized = false;
				mPossibleInstanceCandidatePropagationBeginningKnownInstancesCount = 0;

				mMaximumSubClassNodeSet = nullptr;

				mLazyRealizationInitialized = false;
				mLazyRealizationInitializationRequested = false;
				mLazyNeighbourRealizationPruningRequired = false;
				mLazyNeighbourRealizationRequested = false;
				mLazyRealizationInstancesRetrieved = false;
				mCompletelyHandledChecked = false;
				mLazyRealizationProcessingCount = 0;

				mConceptAbsorbed = false;
				mCandidatePropagationProcessingCount = 0;
				mCandidatePropagated = false;
				mCandidateReceived = false;

				mVarConBaseItem = nullptr;
				mVarConSuperClassesItem = nullptr;
				mVarConSubClassesItem = nullptr;

				mRealizationRetrievingExpectedInstanceCandidateCount = 0;

				mCompletelyHandledReducedInstanceTestingConceptNegation = false;
				mCompletelyHandledReducedInstanceTestingConcept = nullptr;
				mAllAtomicConceptsCompletelyHandled = false;

				mTopObjectPropertyUsed = false;

				mRealizationIterator = nullptr;

				mPossibleTestedNonInstanceCount = 0;
				mRealizationRetrievedInstanceCandidateCount = 0;

				mLastRetrievingCertainInstanceItemCount = 0;

				mMinimalRetrievingInstanceItemSize = 0.1;

				mKnownInstanceItems = nullptr;

				mRealizationIteratorSamplingExpectedCount = -1;

				mLastConceptInstancesCacheReportedCount = -1;
				mLastConceptInstancesCacheReportedSize = -1;


			}


			COptimizedComplexConceptItem::~COptimizedComplexConceptItem() {
			}



			COptimizedComplexConceptItem* COptimizedComplexConceptItem::initComplexConcept(CConcept* concept, bool negated) {
				mConcept = concept;
				mConceptNegation = negated;
				return this;
			}

			bool COptimizedComplexConceptItem::getConceptNegation() {
				return mConceptNegation;
			}

			CConcept* COptimizedComplexConceptItem::getConcept() {
				return mConcept;
			}


			TConceptNegPair COptimizedComplexConceptItem::getConceptNegationPair() {
				return TConceptNegPair(mConcept, mConceptNegation);
			}


			bool COptimizedComplexConceptItem::representsConceptClass() {
				return !mConceptNegation && mConcept->hasClassName();
			}


			CConcept* COptimizedComplexConceptItem::getCandidateConcept() {
				return mCandidateConcept;
			}

			CConcept* COptimizedComplexConceptItem::getMarkerConcept() {
				return mMarkerConcept;
			}

			CConcept* COptimizedComplexConceptItem::getPropagationConcept() {
				return mPropagationConcept;
			}


			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setCompletelyHandledReducedInstanceTestingConcept(CConcept* concept) {
				mCompletelyHandledReducedInstanceTestingConcept = concept;
				return this;
			}

			CConcept* COptimizedComplexConceptItem::getCompletelyHandledReducedInstanceTestingConcept() {
				return mCompletelyHandledReducedInstanceTestingConcept;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setCompletelyHandledReducedInstanceTestingConceptNegation(bool negation) {
				mCompletelyHandledReducedInstanceTestingConceptNegation = negation;
				return this;
			}

			bool COptimizedComplexConceptItem::getCompletelyHandledReducedInstanceTestingConceptNegation() {
				return mCompletelyHandledReducedInstanceTestingConceptNegation;
			}


			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setAllAtomicConceptsCompletelyHandled(bool completelyHandled) {
				mAllAtomicConceptsCompletelyHandled = completelyHandled;
				return this;
			}

			bool COptimizedComplexConceptItem::getAllAtomicConceptsCompletelyHandled() {
				return mAllAtomicConceptsCompletelyHandled;
			}


			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setCandidateConcept(CConcept* concept) {
				mCandidateConcept = concept;
				return this;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setMarkerConcept(CConcept* concept) {
				mMarkerConcept = concept;
				return this;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setPropagationConcept(CConcept* concept) {
				mPropagationConcept = concept;
				return this;
			}

			CComplexConceptItemComputationProcess* COptimizedComplexConceptItem::getComputationProcess() {
				return &mComputationProcess;
			}

			bool COptimizedComplexConceptItem::isSatisfiable() {
				return mSatisfiable;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setSatisfiability(bool satisfiable) {
				mSatisfiable = satisfiable;
				return this;
			}

			bool COptimizedComplexConceptItem::isSuperClassComputationInitializing() {
				return mSuperClassComputationInitializing;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setSuperClassComputationInitializing(bool initialized) {
				mSuperClassComputationInitializing = initialized;
				return this;
			}


			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setKnownSuperClassNodeSet(QSet<CHierarchyNode*>* directClassNodeSet) {
				mKnownSuperClassNodeSet = directClassNodeSet;
				return this; 
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setPossibleSuperClassNodeSet(QSet<CHierarchyNode*>* directClassNodeSet) {
				mPossibleSuperClassNodeSet = directClassNodeSet;
				return this;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setDirectSuperClassNodeSet(QSet<CHierarchyNode*>* directClassNodeSet) {
				mDirectSuperClassNodeSet = directClassNodeSet;
				return this;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setPossibleSuperClassNodeTestingList(QList<CHierarchyNode*>* classNodeList) {
				mPossibleSuperClassNodeTestingList = classNodeList;
				return this;
			}

			QList<CHierarchyNode*>* COptimizedComplexConceptItem::getPossibleSuperClassNodeTestingList() {
				return mPossibleSuperClassNodeTestingList;
			}



			QSet<CHierarchyNode*>* COptimizedComplexConceptItem::getKnownSuperClassNodeSet() {
				return mKnownSuperClassNodeSet;
			}

			QSet<CHierarchyNode*>* COptimizedComplexConceptItem::getPossibleSuperClassNodeSet() {
				return mPossibleSuperClassNodeSet;
			}

			QSet<CHierarchyNode*>* COptimizedComplexConceptItem::getDirectSuperClassNodeSet() {
				return mDirectSuperClassNodeSet;
			}

			CComplexConceptStepComputationProcess* COptimizedComplexConceptItem::setQueuedProcessStep() {
				return mQueuedProcessStep;
			}

			bool COptimizedComplexConceptItem::hasQueuedProcessStep() {
				return mQueuedProcessStep;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setQueuedProcessStep(CComplexConceptStepComputationProcess* queuedProcessStep) {
				mQueuedProcessStep = queuedProcessStep;
				return this;
			}

			QSet<CConcept*>* COptimizedComplexConceptItem::getKnownSuperConceptSet() {
				return mKnownSuperConceptSet;
			}

			QSet<CConcept*>* COptimizedComplexConceptItem::getPossibleSuperConceptSet() {
				return mPossibleSuperConceptSet;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setKnownSuperConceptSet(QSet<CConcept*>* conceptSet) {
				mKnownSuperConceptSet = conceptSet;
				return this;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setPossibleSuperConceptSet(QSet<CConcept*>* conceptSet) {
				mPossibleSuperConceptSet = conceptSet;
				return this;
			}







			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setPossibleSubClassTestingNodeSet(QSet<CHierarchyNode*>* directClassNodeSet) {
				mPossibleSubClassTestingNodeSet = directClassNodeSet;
				return this;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setDirectSubClassNodeSet(QSet<CHierarchyNode*>* directClassNodeSet) {
				mDirectSubClassNodeSet = directClassNodeSet;
				return this;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setPossibleSubClassNodeTestingList(QList<CHierarchyNode*>* classNodeList) {
				mPossibleSubClassNodeTestingList = classNodeList;
				return this;
			}

			QList<CHierarchyNode*>* COptimizedComplexConceptItem::getPossibleSubClassNodeTestingList() {
				return mPossibleSubClassNodeTestingList;
			}




			QSet<CHierarchyNode*>* COptimizedComplexConceptItem::getPossibleSubClassTestingNodeSet() {
				return mPossibleSubClassTestingNodeSet;
			}


			QSet<CHierarchyNode*>* COptimizedComplexConceptItem::getDirectSubClassNodeSet() {
				return mDirectSubClassNodeSet;
			}



			QSet<COptimizedComplexConceptItem*>* COptimizedComplexConceptItem::getSuperComplexConceptItemSet() {
				return &mSuperComplexConceptItemSet;
			}


			bool COptimizedComplexConceptItem::hasSuperComplexConceptItem(COptimizedComplexConceptItem* item) {
				return mSuperComplexConceptItemSet.contains(item);
			}

			bool COptimizedComplexConceptItem::hasSubComplexConceptItem(COptimizedComplexConceptItem* item) {
				return mSubComplexConceptItemSet.contains(item);
			}


			QSet<COptimizedComplexConceptItem*>* COptimizedComplexConceptItem::getSubComplexConceptItemSet() {
				return &mSubComplexConceptItemSet;
			}


			QSet<CIndividualReference>* COptimizedComplexConceptItem::getInstancesItemSet() {
				return mKnownInstanceSet;
			}

			QSet<TConceptNegPair>* COptimizedComplexConceptItem::getAtomicConcpetSet() {
				return &mAtomicClassConceptSet;
			}

			CClassTermExpression* COptimizedComplexConceptItem::getClassTermExpression() {
				return mClassTermExpression;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setClassTermExpression(CClassTermExpression* classExpression) {
				mClassTermExpression = classExpression;
				return this;
			}







			QSet<CIndividualReference>* COptimizedComplexConceptItem::getPossibleInstanceIndividualSet() {
				return mPossibleInstanceSet;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setPossibleInstanceIndividualSet(QSet<CIndividualReference>* instanceSet) {
				mPossibleInstanceSet = instanceSet;
				return this;
			}


			QSet<CIndividualReference>* COptimizedComplexConceptItem::getKnownInstanceIndividualSet() {
				return mKnownInstanceSet;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setKnownInstanceIndividualSet(QSet<CIndividualReference>* instanceSet) {
				mKnownInstanceSet = instanceSet;
				return this;
			}

			QSet<CRealizationIndividualInstanceItemReference>* COptimizedComplexConceptItem::getPossibleInstanceItemSet() {
				return mPossibleInstanceItemSet;
			}


			QSet<CRealizationIndividualInstanceItemReference>::const_iterator COptimizedComplexConceptItem::getPossibleInstanceItemSetCandidatePropagationIterator() {
				return mPossibleInstanceItemSetCandidatePropagationIterator;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setPossibleInstanceItemSetCandidatePropagationIterator(QSet<CRealizationIndividualInstanceItemReference>::const_iterator it) {
				mPossibleInstanceItemSetCandidatePropagationIterator = it;
				return this;
			}


			bool COptimizedComplexConceptItem::hasPossibleInstanceItemSetCandidatePropagationIteratorInitialized() {
				return mPossibleInstanceItemSetCandidatePropagationIteratorInitialized;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setPossibleInstanceItemSetCandidatePropagationIteratorInitialized(bool initialized) {
				mPossibleInstanceItemSetCandidatePropagationIteratorInitialized = initialized;
				return this;
			}


			cint64 COptimizedComplexConceptItem::getPossibleInstanceCandidatePropagationBeginningKnownInstancesCount() {
				return mPossibleInstanceCandidatePropagationBeginningKnownInstancesCount;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setPossibleInstanceCandidatePropagationBeginningKnownInstancesCount(cint64 count) {
				mPossibleInstanceCandidatePropagationBeginningKnownInstancesCount = count;
				return this;
			}



			COptimizedComplexConceptInstanziatedIndividualItemHash* COptimizedComplexConceptItem::getKnownInstanceItems() {
				return mKnownInstanceItems;
			}


			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setKnownInstanceItems(COptimizedComplexConceptInstanziatedIndividualItemHash* instanceItems) {
				mKnownInstanceItems = instanceItems;
				return this;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setPossibleInstanceItemSet(QSet<CRealizationIndividualInstanceItemReference>* instanceSet) {
				mPossibleInstanceItemSet = instanceSet;
				return this;
			}


			QSet<CRealizationIndividualInstanceItemReference>* COptimizedComplexConceptItem::getPossibleInstanceCandidatePropagationItemSet() {
				return mPossibleInstanceCandidatePropagationItemSet;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setPossibleInstanceCandidatePropagationItemSet(QSet<CRealizationIndividualInstanceItemReference>* instanceSet) {
				mPossibleInstanceCandidatePropagationItemSet = instanceSet;
				return this;
			}


			cint64 COptimizedComplexConceptItem::getPossibleTestedNonInstanceCount() {
				return mPossibleTestedNonInstanceCount;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::incPossibleTestedNonInstanceCount(cint64 incCount) {
				mPossibleTestedNonInstanceCount += incCount;
				return this;
			}


			cint64 COptimizedComplexConceptItem::getRealizationRetrievedInstanceCandidateCount() {
				return mRealizationRetrievedInstanceCandidateCount;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::inRealizationRetrievedInstanceCandidateCount(cint64 incCount) {
				mRealizationRetrievedInstanceCandidateCount += incCount;
				return this;
			}


			cint64 COptimizedComplexConceptItem::getRealizationRetrievingExpectedInstanceCandidateCount() {
				return mRealizationRetrievingExpectedInstanceCandidateCount;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setRealizationRetrievingExpectedInstanceCandidateCount(cint64 count) {
				mRealizationRetrievingExpectedInstanceCandidateCount = count;
				return this;
			}




			double COptimizedComplexConceptItem::getRealizationIteratorSamplingExpectedCount() {
				return mRealizationIteratorSamplingExpectedCount;
			}


			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setRealizationIteratorSamplingExpectedCount(double expectedCount) {
				mRealizationIteratorSamplingExpectedCount = expectedCount;
				return this;
			}




			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setMaximumSubClassNodeSet(QSet<CHierarchyNode*>* maxSubClassNodeSet) {
				mMaximumSubClassNodeSet = maxSubClassNodeSet;
				return this;
			}


			QSet<CHierarchyNode*>* COptimizedComplexConceptItem::getMaximumSubClassNodeSet() {
				return mMaximumSubClassNodeSet;
			}


			bool COptimizedComplexConceptItem::isLazyRealizationInitializationRequested() {
				return mLazyRealizationInitializationRequested;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setLazyRealizationInitializationRequested(bool requested) {
				mLazyRealizationInitializationRequested = requested;
				return this;
			}


			bool COptimizedComplexConceptItem::isLazyRealizationInitialized() {
				return mLazyRealizationInitialized;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setLazyRealizationInitialized(bool requested) {
				mLazyRealizationInitialized = requested;
				return this;
			}

			bool COptimizedComplexConceptItem::isLazyNeighbourRealizationPruningRequired() {
				return mLazyNeighbourRealizationPruningRequired;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setLazyNeighbourRealizationPruningRequired(bool requested) {
				mLazyNeighbourRealizationPruningRequired = requested;
				return this;
			}

			bool COptimizedComplexConceptItem::isLazyNeighbourRealizationRequested() {
				return mLazyNeighbourRealizationRequested;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setLazyNeighbourRealizationRequested(bool requested) {
				mLazyNeighbourRealizationRequested = requested;
				return this;
			}

			bool COptimizedComplexConceptItem::isLazyRealizationInstancesRetrieved() {
				return mLazyRealizationInstancesRetrieved;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setLazyRealizationInstancesRetrieved(bool retrieved) {
				mLazyRealizationInstancesRetrieved = retrieved;
				return this;
			}




			bool COptimizedComplexConceptItem::isCompletelyHandledChecked() {
				return mCompletelyHandledChecked;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setCompletelyHandledChecked(bool checked) {
				mCompletelyHandledChecked = checked;
				return this;
			}


			bool COptimizedComplexConceptItem::isLazyRealizationProcessing() {
				return mLazyRealizationProcessingCount != 0;
			}

			cint64 COptimizedComplexConceptItem::getLazyRealizationProcessingCount() {
				return mLazyRealizationProcessingCount;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::incLazyRealizationProcessingCount(cint64 incCount) {
				mLazyRealizationProcessingCount += incCount;
				return this;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::decLazyRealizationProcessingCount(cint64 decCount) {
				mLazyRealizationProcessingCount -= decCount;
				return this;
			}


			COptimizedComplexVariableConceptBaseItem* COptimizedComplexConceptItem::getVariableConceptBaseItem() {
				return mVarConBaseItem;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setVariableConceptBaseItem(COptimizedComplexVariableConceptBaseItem* baseItem) {
				mVarConBaseItem = baseItem;
				return this;
			}


			COptimizedComplexVariableConceptSubSuperItem* COptimizedComplexConceptItem::getVariableConceptSuperClassesItem() {
				return mVarConSuperClassesItem;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setVariableConceptSuperClassesItem(COptimizedComplexVariableConceptSubSuperItem* superItem) {
				mVarConSuperClassesItem = superItem;
				return this;
			}



			COptimizedComplexVariableConceptSubSuperItem* COptimizedComplexConceptItem::getVariableConceptSubClassesItem() {
				return mVarConSubClassesItem;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setVariableConceptSubClassesItem(COptimizedComplexVariableConceptSubSuperItem* superItem) {
				mVarConSubClassesItem = superItem;
				return this;
			}



			COptimizedComplexVariableConceptSubSuperItem* COptimizedComplexConceptItem::getVariableConceptSuperSubClassesItem(bool superClasses) {
				if (superClasses) {
					return mVarConSuperClassesItem;
				} else {
					return mVarConSubClassesItem;
				}
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setVariableConceptSuperSubClassesItem(COptimizedComplexVariableConceptSubSuperItem* item, bool superClasses) {
				if (superClasses) {
					mVarConSuperClassesItem = item;
				} else {
					mVarConSubClassesItem = item;
				}
				return this;
			}




			bool COptimizedComplexConceptItem::isConceptAbsorbed() {
				return mConceptAbsorbed;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setConceptAbsorbed(bool absorbed) {
				mConceptAbsorbed = absorbed;
				return this;
			}



			bool COptimizedComplexConceptItem::isCandidatePropagated() {
				return mCandidatePropagated;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setCandidatePropagated(bool hasCandidate) {
				mCandidatePropagated = hasCandidate;
				return this;
			}



			bool COptimizedComplexConceptItem::isCandidateReceived() {
				return mCandidateReceived;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setCandidateReceived(bool hasCandidate) {
				mCandidateReceived = hasCandidate;
				return this;
			}


			bool COptimizedComplexConceptItem::isCandidatePropagationProcessing() {
				return mCandidatePropagationProcessingCount != 0;
			}

			cint64 COptimizedComplexConceptItem::getCandidatePropagationProcessingCount() {
				return mCandidatePropagationProcessingCount;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::incCandidatePropagationProcessingCount(cint64 incCount) {
				mCandidatePropagationProcessingCount += incCount;
				return this;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::decCandidatePropagationProcessingCount(cint64 decCount) {
				mCandidatePropagationProcessingCount -= decCount;
				return this;
			}


			bool COptimizedComplexConceptItem::isTopObjectPropertyUsed() {
				return mTopObjectPropertyUsed;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setTopObjectPropertyUsage(bool topObjectPropertyUsed) {
				mTopObjectPropertyUsed = topObjectPropertyUsed;
				return this;
			}

			CRealizationIndividualInstanceItemReferenceIterator* COptimizedComplexConceptItem::getRealizationIterator() {
				return mRealizationIterator;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setRealizationIterator(CRealizationIndividualInstanceItemReferenceIterator* realizationIterator) {
				mRealizationIterator = realizationIterator;
				return this;
			}

			CRequirementWaitingDependencyData* COptimizedComplexConceptItem::getInstancesComputationRequirements() {
				return &mInstancesComputationRequirments;
			}

			cint64 COptimizedComplexConceptItem::getConceptItemId() {
				return mConceptItemId;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setConceptItemId(cint64 id) {
				mConceptItemId = id;
				return this;
			}

			cint64 COptimizedComplexConceptItem::getLastRetrievedCertainInstanceItemCount() {
				return mLastRetrievingCertainInstanceItemCount;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setLastRetrievedCertainInstanceItemCount(cint64 certainCount) {
				mLastRetrievingCertainInstanceItemCount = certainCount;
				return this;
			}


			double COptimizedComplexConceptItem::getMinimalRetrievingInstanceItemSize() {
				return mMinimalRetrievingInstanceItemSize;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setMinimalRetrievingInstanceItemSize(double newSize) {
				mMinimalRetrievingInstanceItemSize = newSize;
				return this;
			}



			cint64 COptimizedComplexConceptItem::getLastConceptInstancesCacheReportedCount() {
				return mLastConceptInstancesCacheReportedCount;
			}

			cint64 COptimizedComplexConceptItem::getLastConceptInstancesCacheReportedSize() {
				return mLastConceptInstancesCacheReportedSize;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setLastConceptInstancesCacheReportedCount(cint64 count) {
				mLastConceptInstancesCacheReportedCount = count;
				return this;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptItem::setLastConceptInstancesCacheReportedSize(cint64 size) {
				mLastConceptInstancesCacheReportedSize = size;
				return this;
			}




		
			cint64 COptimizedComplexConceptItem::getMemoryConsumption() {
				cint64 memoryConsumption = 0;
				if (mKnownInstanceItems) {
					memoryConsumption += mKnownInstanceItems->getMemoryConsumption();
				}
				if (mPossibleInstanceCandidatePropagationItemSet) {
					cint64 bucketSize = sizeof(void*) + sizeof(uint);
					cint64 itemSize = sizeof(CRealizationIndividualInstanceItemReference);
					memoryConsumption += bucketSize * mPossibleInstanceCandidatePropagationItemSet->capacity();
					memoryConsumption += itemSize * mPossibleInstanceCandidatePropagationItemSet->size();
				}
				if (mPossibleInstanceSet) {
					cint64 bucketSize = sizeof(void*) + sizeof(uint);
					cint64 itemSize = sizeof(CIndividualReference);
					memoryConsumption += bucketSize * mPossibleInstanceSet->capacity();
					memoryConsumption += itemSize * mPossibleInstanceSet->size();
				}
				if (mKnownInstanceSet) {
					cint64 bucketSize = sizeof(void*) + sizeof(uint);
					cint64 itemSize = sizeof(CIndividualReference);
					memoryConsumption += bucketSize * mKnownInstanceSet->capacity();
					memoryConsumption += itemSize * mKnownInstanceSet->size();
				}
				return memoryConsumption;
			}




			bool COptimizedComplexConceptItem::clearComputation() {
				CCacheAnswersWeightedUsageCostItem::clearComputation();


				CComplexConceptStepInstanceComputationProcess* instanceCompStep = getComputationProcess()->getInstancesComputationProcess(true);
				instanceCompStep->clearComputedInstancesCount();


				if (mKnownInstanceSet) {
					delete mKnownInstanceSet;
					mKnownInstanceSet = nullptr;
				}
				if (mPossibleInstanceSet) {
					delete mPossibleInstanceSet;
					mPossibleInstanceSet = nullptr;
				}
				if (mPossibleInstanceItemSet) {
					delete mPossibleInstanceItemSet;
					mPossibleInstanceItemSet = nullptr;
				}
				if (mPossibleInstanceCandidatePropagationItemSet) {
					delete mPossibleInstanceCandidatePropagationItemSet;
					mPossibleInstanceCandidatePropagationItemSet = nullptr;
				}
				mPossibleInstanceItemSetCandidatePropagationIteratorInitialized = false;
				mPossibleInstanceCandidatePropagationBeginningKnownInstancesCount = 0;
				mPossibleTestedNonInstanceCount = 0;
				mRealizationRetrievedInstanceCandidateCount = 0;
				mRealizationRetrievingExpectedInstanceCandidateCount = 0;
				mRealizationIteratorSamplingExpectedCount = 0;


				mLazyRealizationInitialized = false;
				mLazyRealizationInitializationRequested = false;
				mLazyNeighbourRealizationPruningRequired = false;
				mLazyNeighbourRealizationRequested = false;
				mLazyRealizationInstancesRetrieved = false;
				mCompletelyHandledChecked = false;
				mLazyRealizationProcessingCount = 0;

				mCandidatePropagationProcessingCount = 0;
				mCandidatePropagated = false;
				mCandidateReceived = false;

				if (mRealizationIterator) {
					delete mRealizationIterator;
					mRealizationIterator = nullptr;
				}

				mLastRetrievingCertainInstanceItemCount = 0;
				mMinimalRetrievingInstanceItemSize = 0.1;

				if (mKnownInstanceItems) {
					delete mKnownInstanceItems;
					mKnownInstanceItems = nullptr;
				}


				return true;
			}

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
