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

#include "CSequentialStreamingComplexQueryFinishingHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Conclusion {

				CSequentialStreamingComplexQueryFinishingHandler::CSequentialStreamingComplexQueryFinishingHandler(CComplexQueryExpressionProcessingData* queryProcessingData, COptimizedComplexExpressionOntologyAnsweringItem* ontoAnsweringItem, bool confExtendedLogging, const QString& logDomain, cint64 confMaximumBatchMappingsComputationSize, cint64 confFirstBatchMappingsComputationSize, double confBatchMappingsComputationSizeIncreasingFactor, bool confContinueMappingsCompuationWhenResultsNotWriteable, cint64 confMappingsComputationUnlimitedInterpretationSize)
				: CAbstractStreamingComplexQueryFinishingHandler(queryProcessingData, ontoAnsweringItem, confExtendedLogging, logDomain, confMaximumBatchMappingsComputationSize, confFirstBatchMappingsComputationSize, confBatchMappingsComputationSizeIncreasingFactor, confContinueMappingsCompuationWhenResultsNotWriteable, confMappingsComputationUnlimitedInterpretationSize) {
				}


				CSequentialStreamingComplexQueryFinishingHandler::~CSequentialStreamingComplexQueryFinishingHandler() {
				}


				


				void CSequentialStreamingComplexQueryFinishingHandler::processUpdatedBindingsCardinalityLinkers() {

					COptimizedComplexVariableCompositionItemDependence* varItemProcessingDep = mLastVarItemProcessingDep;
					if (varItemProcessingDep->getDependentItem()->hasSplitComputations()) {
						varItemProcessingDep = new COptimizedComplexVariableCompositionItemDependence(varItemProcessingDep->getDependentItem()->getCurrentSplitComputationItem());
						mSplitComputedMappingCount += varItemProcessingDep->getDependentItem()->getVariableMappingsCurrentCount();
					}


					while ((mQueryProcessingData->getOffsetSkippedMappingCount() + mBindsAnswersResult->getResultCount() < mCompVarQuery->getResultLimitIncludingOffset() || mCompVarQuery->getResultLimitIncludingOffset() == -1) && (!varItemProcessingDep->isBatchProcessed() || varItemProcessingDep->loadNextBatch())) {

						while ((mQueryProcessingData->getOffsetSkippedMappingCount() + mBindsAnswersResult->getResultCount() <= mCompVarQuery->getResultLimitIncludingOffset() || mCompVarQuery->getResultLimitIncludingOffset() == -1) && varItemProcessingDep->getBatchCurrentBindingsCardinalityLinker(false)) {
							COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinker = varItemProcessingDep->getBatchCurrentBindingsCardinalityLinker(true);
							COptimizedComplexVariableIndividualBindings* bindings = bindingLinker->getBindings();
							COptimizedComplexVariableIndividualBindingsCardinality* cardinalites = bindingLinker->getInitialCardinalities();

							handleBindingCardinalityLinker(bindings, cardinalites);
						}

						while ((mQueryProcessingData->getOffsetSkippedMappingCount() + mBindsAnswersResult->getResultCount() < mCompVarQuery->getResultLimitIncludingOffset() || mCompVarQuery->getResultLimitIncludingOffset() == -1) && varItemProcessingDep->getBatchCurrentUpdatedCardinalityLinker(false)) {
							COptimizedComplexVariableIndividualUpdateCardinalityLinker* updatedCardinalityLinker = varItemProcessingDep->getBatchCurrentUpdatedCardinalityLinker(true);
							if (!mDistinct) {
								COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites = updatedCardinalityLinker->getPreviousCardinality();
								COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinker = updatedCardinalityLinker->getUpdatedBindingsCardinalityLinker();
								COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites = updatedCardinalityLinker->getNewCardinality();
								COptimizedComplexVariableIndividualBindings* bindings = bindingLinker->getBindings();

								COptimizedComplexVariableIndividualBindingsCardinality tmpDiffCardinalites = *newCardinalites;
								tmpDiffCardinalites.substractCardinalities(prevCardinalites);

								handleBindingCardinalityLinker(bindings, &tmpDiffCardinalites);
							}
						}
					}

					if (varItemProcessingDep != mLastVarItemProcessingDep) {
						delete varItemProcessingDep;
					}

				}



			}; // end namespace Conclusion

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
