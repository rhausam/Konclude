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

#include "COptimizedComplexExpressionAnsweringHandler.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			



			COptimizedComplexExpressionAnsweringHandler::COptimizedComplexExpressionAnsweringHandler(COptimizedComplexExpressionOntologyAnsweringItem* ontoAnsweringItem) : CAnsweringHandler(), CLogDomain("::Konclude::Reasoner::Kernel::ComplexExpressionQueryAnswerer") {
				mOntoAnsweringItem = ontoAnsweringItem;
				mConfDebugTestingOntologyWriting = false;
				mConfInterpretNonAnswerIndividualVariablesAsAnonymousVariables = false;
				mConfDirectPossibleRealizationRequesting = false;
				mConfFailOnUnknownEntity = false;
				mConfAnonymousToDistinguishedVariablesConversion = true;
				mConfAnonymousToDistinguishedVariablesOnlyFullConversion = true;
				mConfUnsatisfiableNonInstanceStatisticsChecking = false;
				mConfOccurrenceStatisticsCollection = false;
				mConfRedundantlyRestrictedVariablesElimination = true;
				mConfAllowBindingReduction = true;
				mConfAllowJoiningBindingReduction = true;
				mConfMappingSizeBindingReduction = 5000;
				mConfDebugWriteVariableCompositionItems = false;
				mConfSamplingBasedJoinMappingSize = 1000;
				mConfBindingReducingForNonDistinctAnswerQueries = true;
				mConfDetailedMissingEntityReporting = true;

				mConfDistinctUnecessaryVariablesReduction = true;
				mConfAlwaysUnecessaryVariablesReduction = false;
				mConfConceptItemExpectedCountSamplingSize = 30;
				mConfConceptItemExpectedCountSamplingRetrievedPercentageLimit = 75.;


				cint64 computationThreadCount = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.ConcurrentComputationThreadPoolSize", 0);
				if (computationThreadCount != 0) {
					QThreadPool::globalInstance()->setMaxThreadCount(computationThreadCount);
				}


				mConfInterpretQueriesAsDistinct = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.InterpretQueriesAsDistinct", false);


				mConfInterpretNonAnswerIndividualVariablesAsAnonymousVariables = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.InterpretNonAnswerIndividualVariablesAsAnonymousVariables", false);
				mConfNonAnswerIndividualVariablesAsAnonymousVariablesInterpretingPrefixString = CConfigDataReader::readConfigString(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.NonAnswerIndividualVariablesAsAnonymousVariablesInterpretingPrefixString", "");


				mConfDebugWriteVariableCompositionItems = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Debugging.WriteAnsweringVariableCompositionItems", false);

				mConfSamplingBasedJoinMappingSize = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.SamplingBasedJoinMappingSize", 1000);

				mConfFailOnUnknownEntity = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.FailOnUnknownEntity", false);
				mConfExtendedLogging = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.ExtendedLogging", false);
				mConfVariablePreAbsorption = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.QueryAbsorption.Preabsorption", true);
				mConfRedundantTermReduction = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.RedundantTermElimination", true);

				mConfAllowBindingReduction = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.AllowBindingReduction", true);
				mConfAllowJoiningBindingReduction = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.ImplicitJoiningBindingReduction", true);
				mConfBindingReducingForNonDistinctAnswerQueries = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.BindingReducingForNonDistinctAnswerQueries", true);
				mConfMappingSizeBindingReduction = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.BindingReductionMappingSize", 5000);
				mConfPropagationJoining = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.PropagationJoining", true);
				mConfPropagationReplacement = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.PropagationReplacement", false);

				mConfAnonymousToDistinguishedVariablesConversion = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.AnonymousToDistinguishedVariableConversion", true);
				mConfAnonymousToDistinguishedVariablesOnlyFullConversion = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.AnonymousToDistinguishedVariableOnlyFullConversion", true);
				mConfOccurrenceStatisticsCollection = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Calculation.Optimization.OccurrenceStatisticsCollecting", true);
				if (mConfOccurrenceStatisticsCollection) {
					mConfUnsatisfiableNonInstanceStatisticsChecking = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.UnsatisfiableNonInstanceStatisticsChecking", true);
				}
				mConfRedundantlyRestrictedVariablesElimination = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.RedundantlyRestrictedVariablesElimination", true);


				mConfMaximumBatchMappingsComputationSize = 100000000;
				mConfBatchMappingsComputationSizeIncreasingFactor = 10.;
				mConfFirstBatchMappingsComputationSize = 10;
				mConfMinimalMappingsComputationSize = 1;
				mConfMappingsRepeatedlyInsufficientDependencyComputationIncreasingFactor = 1.3;
				mConfMappingsComputationUnlimitedInterpretationSize = Q_UINT64_C(100000000000000);


				mConfMaximumBatchMappingsComputationSize = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.MaximumBatchMappingsComputationSize", 100000000);
				mConfBatchMappingsComputationSizeIncreasingFactor = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.BatchMappingsComputationSizeIncreasingFactorPercent", 1000) / 100.;
				mConfFirstBatchMappingsComputationSize = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.FirstBatchMappingsComputationSize", 10);
				mConfMinimalMappingsComputationSize = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.MinimalMappingsComputationSize", 1);
				mConfMappingsRepeatedlyInsufficientDependencyComputationIncreasingFactor = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.MappingsRepeatedlyInsufficientDependencyComputationIncreasingFactorPercent", 130) / 100.;
				mConfMappingsComputationUnlimitedInterpretationSize = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.MappingsComputationUnlimitedInterpretationSize", Q_UINT64_C(100000000000000));
				mConfContinueMappingsCompuationWhenResultsNotWriteable = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.ContinueMappingsCompuationWhenResultsNotWriteable", false);


				mConfLazyExistentialPartAbsorption = true;
				mConfBooleanQueryExistentialPartOrdinaryEvaluation = true;

				mConfLazyExistentialPartAbsorption = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.LazyExistentialQueryPartAbsorption", true);
				mConfBooleanQueryExistentialPartOrdinaryEvaluation = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.BooleanQueryExistentialPartOrdinaryEvaluation", true);


				mConfConceptItemInstanceCandiateRetrievingSizeIncreaseFactor = 1.3;
				mConfConceptItemInstanceCandiateRetrievingMaxSize = 10000;


				mConfFullQueryMaterialization = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.FullQueryMaterialization", true);

				mConfConceptItemExpectedCountSamplingSize = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.ConceptItemExpectedCountSamplingSize", 30);
				mConfConceptItemExpectedCountSamplingRetrievedPercentageLimit = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.ConceptItemExpectedCountSamplingRetrievedPercentageLimit", 75) / 100.;

				mConfDistinctUnecessaryVariablesReduction = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.DistinctUnecessaryVariablesReductionBeforeAnswerGeneration", true);
				mConfAlwaysUnecessaryVariablesReduction = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.AlwaysUnecessaryVariablesReductionBeforeAnswerGeneration", false);


				mConfMaxVariableBindingPropagationsInitializationIndividualCount = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.MaxVariableMappingsPropagationsInitializationIndividualCount", 50000);
				mConfMaxCandidateInstancePropagationsInitializationIndividualCount = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.MaxCandidateInstancePropagationsInitializationIndividualCount", 10000);
				mConfSplitVariableMappingPropagationTestMaxIntegrationLimit = 10000;
				mConfSplitVariableMappingPropagationTestIntegrationLimitIncreaseFactor = 2.;
				mConfSplitVariableMappingPropagationTestInitialVariableIntegrationLimit = 2000;
				mConfSplitVariableMappingPropagationTestInitialTotalIntegrationLimit = 3000;
				//mConfSplitVariableMappingPropagationTestInitialVariableIntegrationLimit = 1500000;
				//mConfSplitVariableMappingPropagationTestInitialTotalIntegrationLimit = 20000000;

				mConfAbsorbedIndividualActivationInstanceCandidatePropagationSplitting = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Calculation.Answering.AbsorbedIndividualActivationCandidateInstancePropagationSplitting", true);


				mConfQueryEntailmentTestRestrictedTopPropertyPropagationSplitting = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.QueryEntailmentTestRestrictedTopPropertyPropagationSplitting", true);


				mConfAllowLastStepSplitComputation = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.AllowLastStepSplitComputation", true);
				mConfLastStepSplitComputationMappingsRequirement = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.LastStepSplitComputationMappingsRequirement", 5000000);

				mConfSkipConceptItemsIntegrationForPropagationOverVariables = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.SkipConceptItemsIntegrationForPropagationOverVariables", true);

				mAnsweringStartTime.start();
				mTestingOntologyPreprocessor = new COntologyQueryExtendedConfigDependedPreProcesser();
				mTestingOntologyBuilder = new CConcreteOntologyRebuildingPreparationBuilder(ontoAnsweringItem->getTestingOntology(true));

				mDefaultRealizationSorting = CRealizationIndividualSorting(CRealizationIndividualSorting::SORT_TYPE_INDIVIDUAL_ID, CRealizationIndividualSorting::SORT_ORDER_ASCENDING, CRealizationIndividualSorting::SORT_GROUP_SAME_UNIQUE);
				mRoleFillerRealizationSorting = CRealizationIndividualSorting(CRealizationIndividualSorting::SORT_TYPE_NOTHING, CRealizationIndividualSorting::SORT_ORDER_NOTHING, CRealizationIndividualSorting::SORT_GROUP_SAME_UNIQUE);


				mConfNonDeterministicSatisfiableCalculationContinuation = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Calculation.Answering.NonDeterministicCachedCompletionGraphContinuationPropagationTests", true);

				mConfConcurrentJoining = true;

				mConfAutomaticConcurrentComputationActivation = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.AutomaticConcurrentComputationActivation", true);
				if (mConfAutomaticConcurrentComputationActivation) {
					mConfAutomaticConcurrentComputationActivation = false;

					mConfAutomaticConcurrentComputationActivation = getWorkerCountConcurrentActivation(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Calculation.WorkerCount") || getWorkerCountConcurrentActivation(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Calculation.ProcessorCount");
				}


				mConcurrentJoinComputationTaskCount = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.ConcurrentJoinComputationTaskCount", 50);
				mConfConcurrentJoining = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.ConcurrentJoinComputation", true) || mConfAutomaticConcurrentComputationActivation;

				if (mConfConcurrentJoining) {
					mJoinComputer = new CQtConcurrentVariableMappingsCompositionJoinComputator();
				} else {
					mJoinComputer = new CSequentialVariableMappingsCompositionJoinComputator();
				}






				mConfConcurrentPropagation = false;

				mConcurrentPropagationComputationTaskCount = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.ConcurrentPropagationComputationTaskCount", 50);
				mConfConcurrentPropagation = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.ConcurrentPropagationComputation", true) || mConfAutomaticConcurrentComputationActivation;

				if (mConfConcurrentPropagation) {
					mPropagationComputer = new CQtConcurrentVariableMappingsCompositionPropagationComputator();
				} else {
					mPropagationComputer = new CSequentialVariableMappingsCompositionPropagationComputator();
				}





				mConfConcurrentAnswerGeneration = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.ConcurrentAnswerGeneration", true) || mConfAutomaticConcurrentComputationActivation;







				mConfNextVariablePropagationHandlingReducibleFactorIntegration = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.NextVariablePropagationHandlingChoise.ReducibleFactorIntegration", true);
				mConfNextVariablePropagationHandlingPropertyScoresIntegration = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.NextVariablePropagationHandlingChoise.PropertyScoresIntegration", true);
				mConfNextVariablePropagationHandlingPropertyPriorization = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.NextVariablePropagationHandlingChoise.PropertyPriorization", true);
				mConfVariableCompositionItemRoleSampling = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.NextVariablePropagationHandlingChoise.PropertyScore.VariableCompositionItemRoleSuccessorCountSampling", true);
				mConfForceAlwaysVariableCompositionItemRoleSampling = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.NextVariablePropagationHandlingChoise.PropertyScore.ForceAlwaysVariableCompositionItemRoleSuccessorCountSampling", false);

				mConfNextVariablePropagationHandlingReducibleFactor = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.NextVariablePropagationHandlingChoise.ReducibleFactorPercent", 80) / 100.;
				mConfNextVariablePropagationHandlingPropertyTotalScoreIntegrationPercent = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.NextVariablePropagationHandlingChoise.PropertyTotalScoreIntegrationPercent", 5);
				mConfNextVariablePropagationHandlingPropertyInOutFactorIntegrationPercent = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.NextVariablePropagationHandlingChoise.PropertySuccessorStatisticsSamplingFactorIntegrationPercent", 50);
				mConfVariableCompositionItemRoleSamplingCount = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.NextVariablePropagationHandlingChoise.VariableCompositionItemRoleSuccessorSamplingCount", 20);
				mConfVariableCompositionItemRoleSamplingMappingsIncreaseSize = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.NextVariablePropagationHandlingChoise.VariableCompositionItemRoleSuccessorSamplingExpectedMappingIncreaseSize", 10000);



				mConfAnswersCacheMaximumSizeBytes = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.AnswersCacheMaximumSizeBytes", -1);
				mConfAnswersCacheMaximumCount = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.AnswersCacheMaximumCount", -1);
				mConfAnswersCacheUsageWeightIncrease = CConfigDataReader::readConfigDouble(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.AnswersCacheUsageWeightIncreaseFactor", 1.03);


				readConfig();
			}




			bool COptimizedComplexExpressionAnsweringHandler::getWorkerCountConcurrentActivation(CCalculationConfigurationExtension* calcConfig, const QString& configName) {
				bool configErrorFlag = false;
				QString workerCountString = CConfigDataReader::readConfigString(calcConfig, configName, QString(), &configErrorFlag);
				if (!configErrorFlag) {
					if (workerCountString == "AUTO" && CThread::idealThreadCount() > 1) {
						return true;
					} else {
						qint64 convertedWorkerCount = workerCountString.toInt(&configErrorFlag);
						if (configErrorFlag && convertedWorkerCount > 1) {
							return true;
						}
					}
				}
				return false;
			}


			COptimizedComplexExpressionAnsweringHandler::~COptimizedComplexExpressionAnsweringHandler() {
				delete mOntoAnsweringItem;
				delete mTestingOntologyPreprocessor;
				delete mTestingOntologyBuilder;
			}














			bool COptimizedComplexExpressionAnsweringHandler::canAnsweringComplexQuery(bool composedQuery) {
				return !composedQuery;
			}

			bool COptimizedComplexExpressionAnsweringHandler::canAnsweringComplexQuery(CComplexAnsweringQuery* complexAnsweringQuery) {
				if (dynamic_cast<CComplexAnsweringExpressionQuery*>(complexAnsweringQuery)) {
					return true;
				}
				return false;
			}

			bool COptimizedComplexExpressionAnsweringHandler::addAnsweringComplexQuery(CComplexAnsweringQuery* complexAnsweringQuery, CCallbackData* callback) {
				mPendingQueryProcessingList.append(new CComplexQueryExpressionProcessingData((CComplexAnsweringExpressionQuery*)complexAnsweringQuery, callback));
				return true;
			}



			bool COptimizedComplexExpressionAnsweringHandler::initializeProcessing(CAnswererContext* answererContext) {
				if (!mOntoAnsweringItem->isOntologyBuildingAndPreprocessingPrepared()) {
					prepareProcessing(answererContext);
				}
				while (!mPendingQueryProcessingList.isEmpty()) {
					CComplexQueryExpressionProcessingData* queryData = mPendingQueryProcessingList.takeFirst();

					CAnsweringHandlingStatistics* stats = mOntoAnsweringItem->getAnsweringHandlingStatistics();
					if (stats) {
						CAnsweringHandlingStatistics* initStats = new CAnsweringHandlingStatistics();
						*initStats = *stats;
						queryData->setQueryProcessingInitializedAnsweringStatistics(initStats);
					}

					initializeQueryProcessing(queryData,answererContext);
				}
				return true;
			}



			bool COptimizedComplexExpressionAnsweringHandler::prepareProcessing(CAnswererContext* answererContext) {
				if (!mOntoAnsweringItem->isOntologyBuildingAndPreprocessingPrepared()) {
					LOG(INFO, getDomain(), logTr("Preparing query answering handler."), this);
					mTestingOntologyBuilder->prepareRebuildingUpdates();
					CConcreteOntology* testingOnto = mOntoAnsweringItem->getTestingOntology(true);
					mTestingOntologyPreprocessor->preprocess(testingOnto, testingOnto->getConfiguration());
					LOG(INFO, getDomain(), logTr("Query answering hanlder prepared."), this);
					mOntoAnsweringItem->setOntologyBuildingAndPreprocessingPrepared(true);
				}
				return true;
			}


			bool nodeSortLessPredecessorsThan(const CHierarchyNode* item1, const CHierarchyNode* item2) {
				return item1->getPredecessorNodeCount() < item2->getPredecessorNodeCount();
			}

			bool nodeSortMorePredecessorsThan(const CHierarchyNode* item1, const CHierarchyNode* item2) {
				return item1->getPredecessorNodeCount() > item2->getPredecessorNodeCount();
			}





			bool COptimizedComplexExpressionAnsweringHandler::initializeComplexConceptQueryProcessing(CComplexQueryExpressionProcessingData* queryProcessingData, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CClassTermExpression* classTermExpOfInt, bool satisfiableComputationRequired, bool superClassNodesComputationRequired, bool subClassNodesComputationRequired, bool equivalentClassNodesComputationRequired, bool subClassRealizationRequired, cint64 instanceComputationRequiredCount, COptimizedComplexConceptItem** assocConceptItem) {
				CConcept* testingConcept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value(classTermExpOfInt);
				bool testingNegation = testingConcept->hasMappingNegation();
				return initializeComplexConceptQueryProcessing(queryProcessingData, buildingVarItem, testingConcept, testingNegation, satisfiableComputationRequired, superClassNodesComputationRequired, subClassNodesComputationRequired, equivalentClassNodesComputationRequired, subClassRealizationRequired, instanceComputationRequiredCount, assocConceptItem);
			}




			bool COptimizedComplexExpressionAnsweringHandler::initializeComplexConceptQueryProcessing(CComplexQueryExpressionProcessingData* queryProcessingData, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CConcept* testingConcept, bool testingNegation, bool satisfiableComputationRequired, bool superClassNodesComputationRequired, bool subClassNodesComputationRequired, bool equivalentClassNodesComputationRequired, bool subClassRealizationRequired, cint64 instanceComputationRequiredCount, COptimizedComplexConceptItem** assocConceptItem) {
				bool created = false;
				COptimizedComplexConceptItem* conceptItem = mOntoAnsweringItem->getComplexConceptItem(testingConcept, testingNegation, true, &created);
				if (assocConceptItem) {
					*assocConceptItem = conceptItem;
				}
				if (buildingVarItem) {
					buildingVarItem->addUsedComplexConceptItem(conceptItem);
				}
				conceptItem->incCacheWeightedUsageFactor(mOntoAnsweringItem->getNextUsageWeight());
				if (created) {

					mOntoAnsweringItem->getAnsweringHandlingStatistics()->incCreatedComplexConceptItemsCount();

					if (!mOntoAnsweringItem->getTestingOntology()->getABox()->hasUniversalConnectionIndividual()) {
						bool usesTopRolePropagation = checkHasNewTopRolePropagation(testingConcept, testingNegation);
						if (usesTopRolePropagation) {
							conceptItem->setTopObjectPropertyUsage(usesTopRolePropagation);
						}
					}
					extractAtomicSubsumerExpressions(conceptItem);
					searchTrivialSuperSubConceptItems(conceptItem);
				}
				if (queryProcessingData) {
					conceptItem->incQueryUsageCount();
					queryProcessingData->setConceptItem(conceptItem);
				}
				CComplexConceptItemComputationProcess* computationProcess = conceptItem->getComputationProcess();

				bool instancesDirectResolveable = true;
				if (instanceComputationRequiredCount != 0) {
					instancesDirectResolveable = false;
					if (checkConceptInstancesCompletelyResolveable(conceptItem, nullptr)) {
						instancesDirectResolveable = true;
					}
				}


				bool requiresProcessing = false;
				subClassNodesComputationRequired |= equivalentClassNodesComputationRequired || subClassRealizationRequired;
				superClassNodesComputationRequired |= subClassNodesComputationRequired || !instancesDirectResolveable;
				satisfiableComputationRequired |= superClassNodesComputationRequired;
				if (satisfiableComputationRequired) {
					requiresProcessing |= initializeQueryProcessingStep(computationProcess->getSatisfiableClassNodesComputationProcess(true), conceptItem, queryProcessingData, buildingVarItem);
				}
				if (superClassNodesComputationRequired) {
					requiresProcessing |= initializeQueryProcessingStep(computationProcess->getSuperClassNodesComputationProcess(true), conceptItem, queryProcessingData, buildingVarItem);
				}
				if (subClassNodesComputationRequired) {
					requiresProcessing |= initializeQueryProcessingStep(computationProcess->getSubClassNodesComputationProcess(true), conceptItem, queryProcessingData, buildingVarItem);
				}
				if (equivalentClassNodesComputationRequired) {
					requiresProcessing |= initializeQueryProcessingStep(computationProcess->getEquivalentClassNodesComputationProcess(true), conceptItem, queryProcessingData, buildingVarItem);
				}
				if (subClassRealizationRequired) {
					requiresProcessing |= initializeQueryProcessingStep(computationProcess->getSubClassRealizationProcess(true), conceptItem, queryProcessingData, buildingVarItem);
				}
				if (instanceComputationRequiredCount == -1) {
					CComplexConceptStepInstanceComputationProcess* instanceCompStep = computationProcess->getInstancesComputationProcess(true);
					instanceCompStep->setAllInstanceComputationRequired(true);
					requiresProcessing |= initializeQueryProcessingStep(computationProcess->getInstancesComputationProcess(true), conceptItem, queryProcessingData, buildingVarItem);
				} else if (instanceComputationRequiredCount > 0) {
					CComplexConceptStepInstanceComputationProcess* instanceCompStep = computationProcess->getInstancesComputationProcess(true);
					if (instanceComputationRequiredCount > instanceCompStep->getComputedInstancesCount()) {

						CComputedItemDataNotificationLinker* notLinker = nullptr;
						if (queryProcessingData) {
							if (!notLinker) {
								notLinker = mOntoAnsweringItem->createComputedItemDataNotificationLinker();
							}
							notLinker->setQueryProcessingData(queryProcessingData);
							queryProcessingData->setComputationStepWaiting(instanceCompStep);
						}
						if (buildingVarItem) {
							if (!notLinker) {
								notLinker = mOntoAnsweringItem->createComputedItemDataNotificationLinker();
							}
							notLinker->setBuildingVariableItemProcessingItem(buildingVarItem);
							buildingVarItem->setComputationStepWaiting(instanceCompStep);
						}
						if (notLinker) {
							instanceCompStep->addComputedInstancesCountRequirement(instanceComputationRequiredCount, notLinker);
						}
						queueConceptItemProcessing(conceptItem, instanceCompStep);
						requiresProcessing = true;
					}
				}
				return requiresProcessing;

			}




			bool COptimizedComplexExpressionAnsweringHandler::initializeQueryProcessingStep(CComplexConceptStepComputationProcess* compStep, COptimizedComplexConceptItem* conceptItem, CComplexQueryExpressionProcessingData* queryProcessingData, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem) {
				if (compStep->isComputationProcessFinished()) {
					return false;
				} else {
					compStep->setComputationProcessRequired(true);
					CComputedItemDataNotificationLinker* notLinker = nullptr;
					if (queryProcessingData) {
						if (!notLinker) {
							notLinker = mOntoAnsweringItem->createComputedItemDataNotificationLinker();
						}
						notLinker->setQueryProcessingData(queryProcessingData);
						queryProcessingData->setComputationStepWaiting(compStep);
					}
					if (buildingVarItem) {
						if (!notLinker) {
							notLinker = mOntoAnsweringItem->createComputedItemDataNotificationLinker();
						}
						notLinker->setBuildingVariableItemProcessingItem(buildingVarItem);
						buildingVarItem->setComputationStepWaiting(compStep);
					}
					if (notLinker) {
						compStep->addNotificationLinker(notLinker);
					}

					queueConceptItemProcessing(conceptItem, compStep);
					return true;
				}
			}



			bool COptimizedComplexExpressionAnsweringHandler::checkHasNewTopRolePropagation(CConcept* concept, bool conceptNegation) {
				CRole* topRole = mOntoAnsweringItem->getTestingOntology()->getRBox()->getTopObjectRole();
				QList<CConcept*> testingList;
				QSet<CConcept*> testedSet;
				testingList.append(concept);
				testedSet.insert(concept);
				while (!testingList.isEmpty()) {
					CConcept* testingConcept = testingList.takeFirst();
					cint64 opCode = testingConcept->getOperatorCode();

					CConceptOperator* conceptOperator = testingConcept->getConceptOperator();
					if (conceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ALL_TYPE | CConceptOperator::CCFS_SOME_TYPE)) {
						CRole* role = testingConcept->getRole();
						if (role == topRole) {
							return true;
						}
					}

					if (opCode != CCEQ && opCode != CCSUB) {
						for (CSortedNegLinker<CConcept*>* opConLinkerIt = testingConcept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
							CConcept* opConcept = opConLinkerIt->getData();
							if (!testedSet.contains(opConcept)) {
								testedSet.insert(opConcept);
								testingList.append(opConcept);
							}
						}
					}

				}
				return false;
			}


			COptimizedComplexExpressionAnsweringHandler* COptimizedComplexExpressionAnsweringHandler::readConfig() {
				CCalculationConfigurationExtension* config = mOntoAnsweringItem->getCalculationConfiguration();
				mConfDebugTestingOntologyWriting = config->isDebuggingWriteDataActivated();


				mJoinComputer->configureComputator(mOntoAnsweringItem, this, mAnswererContext);
				mPropagationComputer->configureComputator(mOntoAnsweringItem, this, mAnswererContext);
				return this;
			}



			bool COptimizedComplexExpressionAnsweringHandler::checkUsesQueryOnlyKnownEntities(CConcreteOntology* expressionOntology) {
				COntologyBuildData* expressionOntologyBuildData = expressionOntology->getBuildData();
				COntologyBuildData* baseOntologyBuildData = mOntoAnsweringItem->getOntology()->getBuildData();
				if (baseOntologyBuildData->getClassEntityBuildHash()->size() != expressionOntologyBuildData->getClassEntityBuildHash()->size()) {
					if (mConfDetailedMissingEntityReporting) {
						for (CBUILDHASH<CStringRefStringHasher, CClassExpression*>::const_iterator it = expressionOntologyBuildData->getClassEntityBuildHash()->constBegin(), itEnd = expressionOntologyBuildData->getClassEntityBuildHash()->constEnd(); it != itEnd; ++it) {
							CClassExpression* classEx = it.value();
							if (!baseOntologyBuildData->getClassEntityBuildHash()->contains(classEx->getName())) {
								LOG(WARN, getDomain(), logTr("Class %1 not used in queried ontology.").arg(classEx->getName()), this);
								return false;
							}
						}
					}
					return false;
				}
				if (baseOntologyBuildData->getDataPropertyEntityBuildHash()->size() != expressionOntologyBuildData->getDataPropertyEntityBuildHash()->size()) {
					return false;
				}
				if (baseOntologyBuildData->getObjectPropertyEntityBuildHash()->size() != expressionOntologyBuildData->getObjectPropertyEntityBuildHash()->size()) {
					if (mConfDetailedMissingEntityReporting) {
						for (CBUILDHASH<CStringRefStringHasher, CObjectPropertyExpression*>::const_iterator it = expressionOntologyBuildData->getObjectPropertyEntityBuildHash()->constBegin(), itEnd = expressionOntologyBuildData->getObjectPropertyEntityBuildHash()->constEnd(); it != itEnd; ++it) {
							CObjectPropertyExpression* objPropEx = it.value();
							if (!baseOntologyBuildData->getObjectPropertyEntityBuildHash()->contains(objPropEx->getName())) {
								LOG(WARN, getDomain(), logTr("Object property %1 not used in queried ontology.").arg(objPropEx->getName()), this);
								return false;
							}
						}
					}
					return false;
				}
				if (baseOntologyBuildData->getIndividualEntityBuildHash()->size() != expressionOntologyBuildData->getIndividualEntityBuildHash()->size()) {
					COntologyTriplesAssertionsAccessor* indexedTriplesAssertionAccessor = mOntoAnsweringItem->getOntology()->getOntologyTriplesData()->getTripleAssertionAccessor();
					if (indexedTriplesAssertionAccessor) {
						CBUILDHASH<CStringRefStringHasher, CNamedIndividualExpression*>* baseIndiEntHash = baseOntologyBuildData->getIndividualEntityBuildHash();
						CBUILDHASH<CStringRefStringHasher, CNamedIndividualExpression*>* expIndiEntHash = expressionOntologyBuildData->getIndividualEntityBuildHash();

						for (CBUILDHASH<CStringRefStringHasher, CNamedIndividualExpression*>::const_iterator it = expIndiEntHash->constBegin(), itEnd = expIndiEntHash->constEnd(); it != itEnd; ++it) {
							if (!baseIndiEntHash->contains(it.key())) {
								// check whether the individual is indexed in the triple stored data
								CNamedIndividualExpression* namedIndiEntExp = it.value();

								if (!indexedTriplesAssertionAccessor->hasIndividualId(namedIndiEntExp->getName())) {
									return false;
								}

							}
						}						
					} else {
						CBUILDHASH<CStringRefStringHasher, CNamedIndividualExpression*>* baseIndiEntHash = baseOntologyBuildData->getIndividualEntityBuildHash();
						CBUILDHASH<CStringRefStringHasher, CNamedIndividualExpression*>* expIndiEntHash = expressionOntologyBuildData->getIndividualEntityBuildHash();
						COntologyStringMapping* stringMapping = mOntoAnsweringItem->getOntology()->getStringMapping();

						for (CBUILDHASH<CStringRefStringHasher, CNamedIndividualExpression*>::const_iterator it = expIndiEntHash->constBegin(), itEnd = expIndiEntHash->constEnd(); it != itEnd; ++it) {
							if (!baseIndiEntHash->contains(it.key())) {
								// check whether the individual is indexed in the triple stored data
								CNamedIndividualExpression* namedIndiEntExp = it.value();

								if (!stringMapping->getIndividualFromName(namedIndiEntExp->getName())) {
									return false;
								}

							}
						}
					}


				}
				if (baseOntologyBuildData->getDatatypeIRIBuildHash()->size() != expressionOntologyBuildData->getDatatypeIRIBuildHash()->size()) {
					return false;
				}
				if (baseOntologyBuildData->getAnonymousIndividualBuildHash()->size() != expressionOntologyBuildData->getAnonymousIndividualBuildHash()->size()) {
					return false;
				}
				return true;
			}




			static bool itemSortMoreHierachyDepthThan(const TDepthTermPair& item1, const TDepthTermPair& item2) {
				return item1.first > item2.first;
			}





			bool COptimizedComplexExpressionAnsweringHandler::tryEmbedVariableRestrictions(CExpressionVariable* varTesting, CExpressionVariable* varEmbedding, QHash<CExpressionVariable*, CExpressionVariable*>& varExpEmbeddingHash, 
					const QHash<CExpressionVariable*, CAxiomExpression*>& varExpAxiomExpHash, const QSet<CExpressionVariable*>& anonymousIndiVariableSet, const QSet<CExpressionVariable*>& answerIndiVariableSet, CAnswererContext* answererContext) {

				if (varExpEmbeddingHash.value(varTesting) == varEmbedding) {
					return true;
				} else if (varExpEmbeddingHash.contains(varTesting)) {
					return false;
				}
				if (varExpEmbeddingHash.contains(varEmbedding)) {
					return false;
				}

				if (answerIndiVariableSet.contains(varTesting)) {
					return false;
				}

				if (!anonymousIndiVariableSet.contains(varTesting)) {
					if (anonymousIndiVariableSet.contains(varEmbedding)) {
						return false;
					}
				}

				QSet<QPair<CObjectPropertyTermExpression*, bool>> testingObjPropExpSet;
				QSet<CClassTermExpression*> testingClassExpSet;
				QList<CObjectPropertyAssertionExpression*> remainingEmbedObjPropList;
				QList<CObjectPropertyAssertionExpression*> availEmbedObjPropList;

				CTaxonomy* classHierarchy = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy();
				CRolePropertiesHierarchy* rolePropertyHierarchy = mOntoAnsweringItem->getOntology()->getClassification()->getObjectPropertyRoleClassification()->getRolePropertiesHierarchy();


				for (CAxiomExpression* axExp : varExpAxiomExpHash.values(varTesting)) {
					CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(axExp);
					if (classAssExp) {
						CClassTermExpression* classExp = classAssExp->getClassTermExpression();
						testingClassExpSet.insert(classExp);
					} else {
						CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(axExp);
						if (propAssExp) {
							CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
							if (varTesting == dynamic_cast<CExpressionVariable*>(propAssExp->getFirstIndividualTermExpression())) {
								testingObjPropExpSet.insert(QPair<CObjectPropertyTermExpression*, bool>(propExp, false));
							} else {
								testingObjPropExpSet.insert(QPair<CObjectPropertyTermExpression*, bool>(propExp, true));
							}
							remainingEmbedObjPropList.append(propAssExp);
						} else {
							// TODO: extend to data values/assertions
							return false;
						}
					}
				}

				if (!testingObjPropExpSet.isEmpty() || !testingClassExpSet.isEmpty()) {
					QSet<QPair<CObjectPropertyTermExpression*, bool>> embeddingObjPropExpSet;
					QSet<CClassTermExpression*> embeddingClassExpSet;

					for (CAxiomExpression* axExp : varExpAxiomExpHash.values(varEmbedding)) {
						CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(axExp);
						if (classAssExp) {
							if (!testingClassExpSet.isEmpty()) {
								CClassTermExpression* classExp = classAssExp->getClassTermExpression();
								embeddingClassExpSet.insert(classExp);
							}
						} else {
							CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(axExp);
							if (propAssExp) {
								if (!testingObjPropExpSet.isEmpty()) {
									CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
									if (varEmbedding == dynamic_cast<CExpressionVariable*>(propAssExp->getFirstIndividualTermExpression())) {
										embeddingObjPropExpSet.insert(QPair<CObjectPropertyTermExpression*, bool>(propExp, false));
									} else {
										embeddingObjPropExpSet.insert(QPair<CObjectPropertyTermExpression*, bool>(propExp, true));
									}
									availEmbedObjPropList.append(propAssExp);
								}
							} else {
								// TODO: extend to data values/assertions
							}
						}
					}


					for (QSet<CClassTermExpression*>::iterator it = testingClassExpSet.begin(), itEnd = testingClassExpSet.end(); it != itEnd; ) {
						CClassTermExpression* testingClassExp = *it;
						if (embeddingClassExpSet.contains(testingClassExp)) {
							it = testingClassExpSet.erase(it);
						} else {
							++it;
						}
					}

					for (QSet<QPair<CObjectPropertyTermExpression*, bool>>::iterator it = testingObjPropExpSet.begin(), itEnd = testingObjPropExpSet.end(); it != itEnd; ) {
						QPair<CObjectPropertyTermExpression*, bool> propInvPair = *it;
						CObjectPropertyTermExpression* testingObjPropExp = propInvPair.first;
						bool inversed = propInvPair.second;
						if (embeddingObjPropExpSet.contains(propInvPair)) {
							it = testingObjPropExpSet.erase(it);
						} else {
							++it;
						}
					}

					if (!testingClassExpSet.isEmpty()) {

						QSet<CHierarchyNode*> embeddingHierNodeSet;
						for (QSet<CClassTermExpression*>::iterator it = embeddingClassExpSet.begin(), itEnd = embeddingClassExpSet.end(); it != itEnd; ++it) {
							CClassTermExpression* classExp = *it;
							if (classHierarchy && dynamic_cast<CClassExpression*>(classExp)) {
								CConcept* concept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value(classExp);
								if (concept) {
									CHierarchyNode* hierNode = classHierarchy->getHierarchyNode(concept, false);
									if (hierNode) {
										embeddingHierNodeSet.insert(hierNode);
										for (CHierarchyNode* predHierNode : *hierNode->getPredecessorNodeSet()) {
											embeddingHierNodeSet.insert(predHierNode);
										}
									}
								}
							}
						}


						for (QSet<CClassTermExpression*>::iterator it = testingClassExpSet.begin(), itEnd = testingClassExpSet.end(); classHierarchy && it != itEnd; ++it) {
							CClassTermExpression* classExp = *it;
							if (!dynamic_cast<CClassExpression*>(classExp)) {
								return false;
							} else {
								CConcept* concept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value(classExp);
								if (!concept) {
									return false;
								} else {
									CHierarchyNode* hierNode = classHierarchy->getHierarchyNode(concept, false);
									if (!hierNode) {
										return false;
									} else if (!embeddingHierNodeSet.contains(hierNode)) {
										return false;
									}
								}
							}
						}
					}





					if (!remainingEmbedObjPropList.isEmpty()) {


						QHash<CRolePropertiesHierarchyNode*, CObjectPropertyAssertionExpression*> embeddingHierNodeObjectPropAssHash;
						QHash<CRolePropertiesHierarchyNode*, CObjectPropertyAssertionExpression*> embeddingHierNodeInvObjectPropAssHash;
						for (CObjectPropertyAssertionExpression* propAssExp : availEmbedObjPropList) {
							CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
							bool inversed = true;
							if (varEmbedding == dynamic_cast<CExpressionVariable*>(propAssExp->getFirstIndividualTermExpression())) {
								inversed = false;
							}

							if (rolePropertyHierarchy) {

								CRole* role = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(propExp);
								if (inversed && role) {
									role = role->getInverseRole();
								}
								if (role) {
									CRolePropertiesHierarchyNode* hierNode = rolePropertyHierarchy->getHierarchyNode(role);
									if (hierNode) {
										embeddingHierNodeObjectPropAssHash.insertMulti(hierNode, propAssExp);
										for (CRolePropertiesHierarchyNode* predHierNode : *hierNode->getPredecessorNodeSet()) {
											embeddingHierNodeObjectPropAssHash.insertMulti(predHierNode, propAssExp);
										}
									}
									CRolePropertiesHierarchyNode* invHierNode = rolePropertyHierarchy->getHierarchyNode(role->getInverseRole());
									if (invHierNode) {
										embeddingHierNodeInvObjectPropAssHash.insertMulti(invHierNode, propAssExp);
										for (CRolePropertiesHierarchyNode* predHierNode : *invHierNode->getPredecessorNodeSet()) {
											embeddingHierNodeInvObjectPropAssHash.insertMulti(predHierNode, propAssExp);
										}
									}
								}
							}
						}


						if (!testingObjPropExpSet.isEmpty()) {
							for (QSet<QPair<CObjectPropertyTermExpression*, bool>>::iterator it = testingObjPropExpSet.begin(), itEnd = testingObjPropExpSet.end(); it != itEnd; ++it) {
								QPair<CObjectPropertyTermExpression*, bool> propInvPair = *it;
								CObjectPropertyTermExpression* testingObjPropExp = propInvPair.first;
								bool inversed = propInvPair.second;
								if (!dynamic_cast<CObjectPropertyExpression*>(testingObjPropExp)) {
									return false;
								} else {
									CRole* role = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(testingObjPropExp);
									if (inversed && role) {
										role = role->getInverseRole();
									}
									if (!role) {
										return false;
									} else {
										CRolePropertiesHierarchyNode* hierNode = rolePropertyHierarchy->getHierarchyNode(role);
										if (hierNode) {
											if (!embeddingHierNodeObjectPropAssHash.contains(hierNode)) {
												return false;
											}
										} else {
											CRolePropertiesHierarchyNode* invHierNode = rolePropertyHierarchy->getHierarchyNode(role->getInverseRole());
											if (invHierNode) {
												if (!embeddingHierNodeInvObjectPropAssHash.contains(invHierNode)) {
													return false;
												}
											}
										}
									}
								}
							}
						}

						// assume embedding 
						QHash<CExpressionVariable*, CExpressionVariable*> newVarExpEmbeddingHash(varExpEmbeddingHash);
						newVarExpEmbeddingHash.insert(varTesting, varEmbedding);
						if (!tryEmbedVariablesObjectPropertyAssertionList(varTesting, varEmbedding, remainingEmbedObjPropList, embeddingHierNodeObjectPropAssHash, embeddingHierNodeInvObjectPropAssHash, newVarExpEmbeddingHash, varExpAxiomExpHash, anonymousIndiVariableSet, answerIndiVariableSet, answererContext)) {
							return false;
						} 
						varExpEmbeddingHash = newVarExpEmbeddingHash;
					}
				}

				return true;
			}


			bool COptimizedComplexExpressionAnsweringHandler::tryEmbedVariablesObjectPropertyAssertionList(CExpressionVariable* varTesting, CExpressionVariable* varEmbedding, const QList<CObjectPropertyAssertionExpression*>& remainingEmbedObjPropList, const QHash<CRolePropertiesHierarchyNode*, CObjectPropertyAssertionExpression*>& embeddingHierNodeObjectPropAssHash, const QHash<CRolePropertiesHierarchyNode*, CObjectPropertyAssertionExpression*>& embeddingHierNodeInvObjectPropAssHash,
					QHash<CExpressionVariable*, CExpressionVariable*>& varExpEmbeddingHash, const QHash<CExpressionVariable*, CAxiomExpression*>& varExpAxiomExpHash, const QSet<CExpressionVariable*>& anonymousIndiVariableSet, const QSet<CExpressionVariable*>& answerIndiVariableSet, CAnswererContext* answererContext) {

				if (!remainingEmbedObjPropList.isEmpty()) {
					QList<CObjectPropertyAssertionExpression*> embedObjPropList(remainingEmbedObjPropList);
					CRolePropertiesHierarchy* rolePropertyHierarchy = mOntoAnsweringItem->getOntology()->getClassification()->getObjectPropertyRoleClassification()->getRolePropertiesHierarchy();

					CObjectPropertyAssertionExpression* propAssExp = embedObjPropList.takeFirst();
					CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
					CIndividualTermExpression* otherIndiExp = propAssExp->getFirstIndividualTermExpression();
					bool inversed = true;
					if (varTesting == dynamic_cast<CExpressionVariable*>(propAssExp->getFirstIndividualTermExpression())) {
						inversed = false;
						otherIndiExp = propAssExp->getSecondIndividualTermExpression();
					}
					CExpressionVariable* otherVarExp = dynamic_cast<CExpressionVariable*>(otherIndiExp);
					CRole* role = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(propExp);
					if (inversed && role) {
						role = role->getInverseRole();
					}
					CRolePropertiesHierarchyNode* hierNode = rolePropertyHierarchy->getHierarchyNode(role);
					QList<CObjectPropertyAssertionExpression*> embeddingPropAssExpList;
					if (hierNode) {
						embeddingPropAssExpList = embeddingHierNodeObjectPropAssHash.values(hierNode);
					} else {
						CRolePropertiesHierarchyNode* invHierNode = rolePropertyHierarchy->getHierarchyNode(role->getInverseRole());
						if (invHierNode) {
							embeddingPropAssExpList = embeddingHierNodeInvObjectPropAssHash.values(invHierNode);
						}
					}



					for (CObjectPropertyAssertionExpression* embeddingPropAssExp : embeddingPropAssExpList) {
						CObjectPropertyTermExpression* embeddingObjPropExp = embeddingPropAssExp->getObjectPropertyTermExpression();
						bool embeddingInversed = true;
						CIndividualTermExpression* embeddingOtherIndiExp = embeddingPropAssExp->getFirstIndividualTermExpression();
						if (varEmbedding == dynamic_cast<CExpressionVariable*>(embeddingPropAssExp->getFirstIndividualTermExpression())) {
							embeddingInversed = false;
							embeddingOtherIndiExp = embeddingPropAssExp->getSecondIndividualTermExpression();
						}
						CExpressionVariable* embeddingOtherVarExp = dynamic_cast<CExpressionVariable*>(embeddingOtherIndiExp);

						if (embeddingOtherVarExp && otherVarExp) {
							if (embeddingOtherVarExp == otherVarExp && (!varExpEmbeddingHash.contains(embeddingOtherVarExp) || varExpEmbeddingHash.value(embeddingOtherVarExp) == otherVarExp)) {
								QHash<CExpressionVariable*, CExpressionVariable*> newVarExpEmbeddingHash(varExpEmbeddingHash);
								newVarExpEmbeddingHash.insert(embeddingOtherVarExp, otherVarExp);
								if (tryEmbedVariablesObjectPropertyAssertionList(varTesting, varEmbedding, embedObjPropList, embeddingHierNodeObjectPropAssHash, embeddingHierNodeInvObjectPropAssHash, newVarExpEmbeddingHash, varExpAxiomExpHash, anonymousIndiVariableSet, answerIndiVariableSet, answererContext)) {
									varExpEmbeddingHash = newVarExpEmbeddingHash;
									return true;
								}
							} else {
								QHash<CExpressionVariable*, CExpressionVariable*> newVarExpEmbeddingHash(varExpEmbeddingHash);
								if (tryEmbedVariableRestrictions(otherVarExp, embeddingOtherVarExp, newVarExpEmbeddingHash, varExpAxiomExpHash, anonymousIndiVariableSet, answerIndiVariableSet, answererContext)) {
									if (tryEmbedVariablesObjectPropertyAssertionList(varTesting, varEmbedding, embedObjPropList, embeddingHierNodeObjectPropAssHash, embeddingHierNodeInvObjectPropAssHash, newVarExpEmbeddingHash, varExpAxiomExpHash, anonymousIndiVariableSet, answerIndiVariableSet, answererContext)) {
										varExpEmbeddingHash = newVarExpEmbeddingHash;
										return true;
									}
								}
							}
						} else if (otherIndiExp == embeddingOtherIndiExp) {
							if (tryEmbedVariablesObjectPropertyAssertionList(varTesting, varEmbedding, embedObjPropList, embeddingHierNodeObjectPropAssHash, embeddingHierNodeInvObjectPropAssHash, varExpEmbeddingHash, varExpAxiomExpHash, anonymousIndiVariableSet, answerIndiVariableSet, answererContext)) {
								return true;
							}
						}
						
					}

				} else {
					return true;
				}

				return false;

			}




			QList<CExpressionVariable*> COptimizedComplexExpressionAnsweringHandler::getRedundantlyEmbeddedVariables(QHash<CExpressionVariable*, CAxiomExpression*>& varExpAxiomExpHash, const QSet<CExpressionVariable*>& anonymousIndiVariableSet, const QSet<CExpressionVariable*>& answerIndiVariableSet, CComplexQueryExpressionProcessingData* queryProcessingData, CAnswererContext* answererContext) {

				CRolePropertiesHierarchy* rolePropertyHierarchy = mOntoAnsweringItem->getOntology()->getClassification()->getObjectPropertyRoleClassification()->getRolePropertiesHierarchy();
				QHash<CExpressionVariable*, CExpressionVariable*> varExpEmbeddingHash;

				QSet<CExpressionVariable*> redundantVariableSet;
				CExpressionVariable* testingVarExp = nullptr;
				QList<TDepthTermPair> propAssExpList;
				for (QHash<CExpressionVariable*, CAxiomExpression*>::iterator it = varExpAxiomExpHash.begin(), itEnd = varExpAxiomExpHash.end(); it != itEnd; ++it) {
					CExpressionVariable* varExp = it.key();
					if (varExp != testingVarExp) {
						if (testingVarExp && propAssExpList.size() >= 2) {

							qSort(propAssExpList.begin(), propAssExpList.end(), itemSortMoreHierachyDepthThan);
							QList<TDepthTermPair>::const_iterator itProp1 = propAssExpList.constBegin(), itPropEnd = propAssExpList.constEnd();
							while (itProp1 != itPropEnd) {
								TDepthTermPair item1 = *itProp1;
								++itProp1;
								CObjectPropertyAssertionExpression* propAssExp1 = (CObjectPropertyAssertionExpression*)item1.second;
								CObjectPropertyTermExpression* propExp1 = propAssExp1->getObjectPropertyTermExpression();
								CRole* role1 = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(propExp1);
								CExpressionVariable* otherVarExp1 = dynamic_cast<CExpressionVariable*>(propAssExp1->getSecondIndividualTermExpression());
								bool inversed1 = false;
								if (testingVarExp == otherVarExp1) {
									inversed1 = true;
									otherVarExp1 = dynamic_cast<CExpressionVariable*>(propAssExp1->getFirstIndividualTermExpression());
									role1 = role1->getInverseRole();
								}

								if (!varExpEmbeddingHash.contains(otherVarExp1)) {
									CRolePropertiesHierarchyNode* hierNode1 = rolePropertyHierarchy->getHierarchyNode(role1);
									CRolePropertiesHierarchyNode* invHierNode1 = rolePropertyHierarchy->getHierarchyNode(role1->getInverseRole());


									QList<TDepthTermPair>::const_iterator itProp2 = propAssExpList.constBegin();
									while (itProp2 != itPropEnd) {

										TDepthTermPair item2 = *itProp2;
										++itProp2;
										CObjectPropertyAssertionExpression* propAssExp2 = (CObjectPropertyAssertionExpression*)item2.second;
										if (propAssExp2 != propAssExp1) {
											CObjectPropertyTermExpression* propExp2 = propAssExp2->getObjectPropertyTermExpression();
											CRole* role2 = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(propExp2);
											CExpressionVariable* otherVarExp2 = dynamic_cast<CExpressionVariable*>(propAssExp2->getSecondIndividualTermExpression());
											bool inversed2 = false;
											if (testingVarExp == otherVarExp2) {
												inversed2 = true;
												otherVarExp2 = dynamic_cast<CExpressionVariable*>(propAssExp2->getFirstIndividualTermExpression());
												role2 = role2->getInverseRole();
											}
											CRolePropertiesHierarchyNode* hierNode2 = rolePropertyHierarchy->getHierarchyNode(role2);
											CRolePropertiesHierarchyNode* invHierNode2 = rolePropertyHierarchy->getHierarchyNode(role2->getInverseRole());


											if (!varExpEmbeddingHash.contains(otherVarExp2)) {
												if (otherVarExp1 != otherVarExp2 && (hierNode1 && hierNode2 && hierNode2->hasPredecessorNode(hierNode1) || invHierNode1 && invHierNode2 && invHierNode2->hasPredecessorNode(invHierNode1))) {
													if (tryEmbedVariableRestrictions(otherVarExp1, otherVarExp2, varExpEmbeddingHash, varExpAxiomExpHash, anonymousIndiVariableSet, answerIndiVariableSet, answererContext)) {
														break;
													}
												}
											}
										}


									}
								}

							}


						}
						propAssExpList.clear();
						testingVarExp = varExp;
					}
					CAxiomExpression* axExp = it.value();
					CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(axExp);
					if (propAssExp) {
						CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
						CExpressionVariable* otherVarExp1 = dynamic_cast<CExpressionVariable*>(propAssExp->getSecondIndividualTermExpression());
						CExpressionVariable* otherVarExp2 = dynamic_cast<CExpressionVariable*>(propAssExp->getFirstIndividualTermExpression());
						if (otherVarExp1 && otherVarExp2 && otherVarExp1 != otherVarExp2) {
							CRole* role = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(propExp);
							if (role && rolePropertyHierarchy) {
								CRolePropertiesHierarchyNode* hierNode = rolePropertyHierarchy->getHierarchyNode(role);
								if (hierNode) {
									propAssExpList.append(TDepthTermPair(hierNode->getPredecessorNodeSet()->count(), propAssExp));
								}
							}
						}
					}
				}


				QList<CExpressionVariable*> redEmbVarList;
				for (QHash<CExpressionVariable*, CExpressionVariable*>::const_iterator it = varExpEmbeddingHash.constBegin(), itEnd = varExpEmbeddingHash.constEnd(); it != itEnd; ++it) {
					CExpressionVariable* varExp = it.key();
					CExpressionVariable* embeddedIntoVarExp = it.value();
					if (varExp != embeddedIntoVarExp) {
						redEmbVarList.append(varExp);
					}
				}

				return redEmbVarList;
			}





			QList<CAxiomExpression*> COptimizedComplexExpressionAnsweringHandler::eliminateRedundantQueryTerms(const QList<CAxiomExpression*>& assExps, CComplexQueryExpressionProcessingData* queryProcessingData, CAnswererContext* answererContext) {
				QList<CAxiomExpression*> redElimAssExpList;
				typedef QPair<CHierarchyNode*, CIndividualTermExpression*> TConceptIndiTermPair;
				typedef QPair<CRole*, QPair<CIndividualTermExpression*, CIndividualTermExpression* > > TRoleIndiTermsPair;
				QSet< TConceptIndiTermPair > redundantClassAssertionSet;
				QSet< TRoleIndiTermsPair > redundantRoleAssertionSet;



				QList<TDepthTermPair> depthTermAxiomPairList;

				CTaxonomy* classHierarchy = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy();
				CRolePropertiesHierarchy* rolePropertyHierarchy = mOntoAnsweringItem->getOntology()->getClassification()->getObjectPropertyRoleClassification()->getRolePropertiesHierarchy();

				for (CAxiomExpression* assExp : assExps) {
					CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(assExp);
					if (classAssExp) {
						CClassTermExpression* classExp = classAssExp->getClassTermExpression();
						bool foundHierNode = false;;
						if (classHierarchy && dynamic_cast<CClassExpression*>(classExp)) {
							CConcept* concept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value(classExp);
							if (concept) {
								CHierarchyNode* hierNode = classHierarchy->getHierarchyNode(concept, false);
								if (hierNode) {
									depthTermAxiomPairList.append(TDepthTermPair(hierNode->getPredecessorNodeCount(), classAssExp));
									foundHierNode = true;
								}
							}
						} 
						if (!foundHierNode) {
							redElimAssExpList.append(classAssExp);
						}

					} else {
						CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
						if (propAssExp) {
							CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
							CRole* role = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(propExp);
							bool foundHierNode = false;
							if (role && rolePropertyHierarchy) {
								CRolePropertiesHierarchyNode* hierNode = rolePropertyHierarchy->getHierarchyNode(role);
								if (hierNode) {
									depthTermAxiomPairList.append(TDepthTermPair(hierNode->getPredecessorNodeSet()->count(), propAssExp));
									foundHierNode = true;
								}
							}
							if (!foundHierNode) {
								redElimAssExpList.append(classAssExp);
							}

						} else {
							redElimAssExpList.append(assExp);
						}
					}
				}

				qSort(depthTermAxiomPairList.begin(), depthTermAxiomPairList.end(), itemSortMoreHierachyDepthThan);

				for (TDepthTermPair depthTermPair : depthTermAxiomPairList) {
					CAxiomExpression* assExp = depthTermPair.second;
					CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(assExp);
					if (classAssExp) {
						CClassTermExpression* classExp = classAssExp->getClassTermExpression();
						CConcept* concept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value(classExp);
						CHierarchyNode* hierNode = classHierarchy->getHierarchyNode(concept, false);
						if (!redundantClassAssertionSet.contains(TConceptIndiTermPair(hierNode, classAssExp->getIndividualTermExpression()))) {
							redundantClassAssertionSet.insert(TConceptIndiTermPair(hierNode, classAssExp->getIndividualTermExpression()));
							redElimAssExpList.append(assExp);
							for (CHierarchyNode* predHierNode : *hierNode->getPredecessorNodeSet()) {
								redundantClassAssertionSet.insert(TConceptIndiTermPair(predHierNode, classAssExp->getIndividualTermExpression()));
							}
						}
					} else {
						CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
						if (propAssExp) {
							CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
							CRole* role = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(propExp);
							CRolePropertiesHierarchyNode* hierNode = rolePropertyHierarchy->getHierarchyNode(role);

							TRoleIndiTermsPair roleIndisPair(hierNode->getOneEquivalentElement(), QPair<CIndividualTermExpression*, CIndividualTermExpression* >(propAssExp->getFirstIndividualTermExpression(), propAssExp->getSecondIndividualTermExpression()));
							if (!redundantRoleAssertionSet.contains(roleIndisPair)) {
								redundantRoleAssertionSet.insert(roleIndisPair);
								CRole* invRole = hierNode->getOneEquivalentElement()->getInverseRole();
								if (invRole) {
									redundantRoleAssertionSet.insert(TRoleIndiTermsPair(invRole, QPair<CIndividualTermExpression*, CIndividualTermExpression* >(propAssExp->getSecondIndividualTermExpression(), propAssExp->getFirstIndividualTermExpression())));
								}

								redElimAssExpList.append(assExp);
								for (CRolePropertiesHierarchyNode* predHierNode : *hierNode->getPredecessorNodeSet()) {
									CRole* invPredRole = predHierNode->getOneEquivalentElement()->getInverseRole();
									redundantRoleAssertionSet.insert(TRoleIndiTermsPair(predHierNode->getOneEquivalentElement(), QPair<CIndividualTermExpression*, CIndividualTermExpression* >(propAssExp->getFirstIndividualTermExpression(), propAssExp->getSecondIndividualTermExpression())));
									if (invPredRole) {
										redundantRoleAssertionSet.insert(TRoleIndiTermsPair(invPredRole, QPair<CIndividualTermExpression*, CIndividualTermExpression* >(propAssExp->getSecondIndividualTermExpression(), propAssExp->getFirstIndividualTermExpression())));
									}
								}
							}
						}
					}
				}


				return redElimAssExpList;
			}




			COptimizedComplexExpressionAnsweringHandler* COptimizedComplexExpressionAnsweringHandler::initializeQueryProcessing(CComplexQueryExpressionProcessingData* queryProcessingData, CAnswererContext* answererContext) {
				CComplexAnsweringExpressionQuery* query = queryProcessingData->getQuery();
				CConcreteOntology* expOnto = query->getExrpessionOntology();
				CConcreteOntology* baseOnto = query->getBaseOntology();
				
				mAnswererContext = answererContext;

				mOntoAnsweringItem->incCurrentlyAnsweringQueryCount();

				bool queryProcessing = false;
				bool queryCompleted = false;
				if (!queryProcessingData->isProcessingStarted()) {
					queryProcessingData->setProcessingStarted();
				}

				CComplexAnsweringExpressionQuery* complAnsQuery = dynamic_cast<CComplexAnsweringExpressionQuery*>(query);
				if (!checkUsesQueryOnlyKnownEntities(complAnsQuery->getExrpessionOntology())) {
					if (mConfFailOnUnknownEntity) {
						failQueryProcessing(queryProcessingData, new CQueryUnknownEntityReferenceError(baseOnto));
						queryCompleted = true;
					} else {
						finishQueryProcessingAsUnsatisfaible(queryProcessingData);
						queryCompleted = true;
					}
				} else {
					increaseUsageWeight();

					CConcreteOntology* testingOnto = mOntoAnsweringItem->getTestingOntology();
					if (testingOnto) {
						CComplexConceptAnsweringQuery* compConQuery = dynamic_cast<CComplexConceptAnsweringQuery*>(query);
						if (compConQuery) {
							CBuildExpression* expOfInt = compConQuery->getExpressionOfInterest();
							mTestingOntologyBuilder->initializeBuilding();
							CBuildExpression* testingExpOfInt = mTestingOntologyBuilder->rebuildExpression(expOfInt);
							mTestingOntologyBuilder->completeBuilding();
							mTestingOntologyPreprocessor->preprocess(testingOnto, testingOnto->getConfiguration());
							CClassTermExpression* classTermExpOfInt = dynamic_cast<CClassTermExpression*>(testingExpOfInt);
							if (classTermExpOfInt) {
								bool instanceComputationRequired = compConQuery->isInstancesComputationRequired();
								bool equivalentClassNodesComputationRequired = compConQuery->isEquivalentClassNodesComputationRequired();
								bool subClassNodesComputationRequired = compConQuery->isSubClassNodesComputationRequired();
								bool superClassNodesComputationRequired = compConQuery->isSuperClassNodesComputationRequired();
								bool satisfiableComputationRequired = compConQuery->isSatisfiableComputationRequired();
								bool direct = compConQuery->isDirect();

								queryProcessing |= initializeComplexConceptQueryProcessing(queryProcessingData, nullptr, classTermExpOfInt, satisfiableComputationRequired, superClassNodesComputationRequired, subClassNodesComputationRequired, equivalentClassNodesComputationRequired, direct, instanceComputationRequired? -1 : 0, nullptr);
							}
						}
						CComplexAxiomsClassVariablesAnsweringQuery* compAxClassVarQuery = dynamic_cast<CComplexAxiomsClassVariablesAnsweringQuery*>(query);
						if (compAxClassVarQuery) {
							initComplexAxiomsClassVariablesQuery(compAxClassVarQuery, answererContext, queryProcessingData, queryCompleted, testingOnto, queryProcessing);
						}
						CComplexAxiomsObjectPropertyVariablesAnsweringQuery* compAxPropVarQuery = dynamic_cast<CComplexAxiomsObjectPropertyVariablesAnsweringQuery*>(query);
						if (compAxPropVarQuery) {
							initComplexAxiomsObjectPropertyVariablesQuery(compAxPropVarQuery, answererContext, queryProcessingData, queryCompleted, testingOnto, queryProcessing);
						}
						CComplexAxiomsDataPropertyVariablesAnsweringQuery* compAxDataVarQuery = dynamic_cast<CComplexAxiomsDataPropertyVariablesAnsweringQuery*>(query);
						if (compAxDataVarQuery) {
							initComplexAxiomsDataPropertyVariablesQuery(compAxDataVarQuery, answererContext, queryProcessingData, queryCompleted, testingOnto, queryProcessing);
						}
						CComplexAssertionsIndividualVariablesAnsweringQuery* compAssIndVarQuery = dynamic_cast<CComplexAssertionsIndividualVariablesAnsweringQuery*>(query);
						if (compAssIndVarQuery) {
							initComplexAssertionsIndividualVariablesQuery(compAssIndVarQuery, answererContext, queryProcessingData, queryCompleted, testingOnto, queryProcessing);
						}
						CComplexAxiomsIndividualMixedVariablesAnsweringQuery* compAxIndMixedVarQuery = dynamic_cast<CComplexAxiomsIndividualMixedVariablesAnsweringQuery*>(query);
						if (compAxIndMixedVarQuery) {
							initComplexAxiomsIndividualMixedVariablesQuery(compAxIndMixedVarQuery, answererContext, queryProcessingData, queryCompleted, testingOnto, queryProcessing);
						}
					}
				}

				if (!queryCompleted && !queryProcessing) {
					finishQueryProcessing(queryProcessingData);
				}

				return this;
			}

			CRole* COptimizedComplexExpressionAnsweringHandler::getResolvedDataPropertyRole(CDataPropertyTermExpression* exp) {
				CRole* role = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getDataPropertyTermRoleMappingHash()->value(exp);
				return role;
			}

			CRole* COptimizedComplexExpressionAnsweringHandler::getInverseResolvedObjectPropertyRole(CObjectPropertyTermExpression* exp) {
				bool inversed = false;
				exp = getInverseResolvedObjectPropertyTermExpression(exp, inversed);
				CRole* role = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(exp);
				if (inversed) {
					role = role->getInverseRole();
				}
				return role;
			}

			CObjectPropertyTermExpression* COptimizedComplexExpressionAnsweringHandler::getInverseResolvedObjectPropertyTermExpression(CObjectPropertyTermExpression* exp, bool& inversed) {
				inversed = false;
				while (dynamic_cast<CInverseObjectPropertyOfExpression*>(exp)) {
					CInverseObjectPropertyOfExpression* invObjPropEx = dynamic_cast<CInverseObjectPropertyOfExpression*>(exp);
					exp = invObjPropEx->getInverseOfExpression();
					inversed = !inversed;
				}
				return exp;
			}





			void COptimizedComplexExpressionAnsweringHandler::initComplexAxiomsDataPropertyVariablesQuery(CComplexAxiomsDataPropertyVariablesAnsweringQuery* compAxPropVarQuery, CAnswererContext* answererContext, CComplexQueryExpressionProcessingData* queryProcessingData, bool &queryCompleted, CConcreteOntology* testingOnto, bool &queryProcessing) {
				QList<CAxiomExpression*> assExps = *compAxPropVarQuery->getAxiomExpressions();

				QList<CSubDataPropertyOfExpression*> dataPropSubAxiomExpsEntList;
				CExpressionVariable* lastVarExpression = nullptr;
				QSet<CExpressionVariable*> varExpSet;
				bool unsupportedWarning = false;


				QHash<CExpressionVariable*, CDataPropertyTermExpression*> varSuperDataPropRestrictionExpHash;
				QHash<CExpressionVariable*, CDataPropertyTermExpression*> varSubDataPropRestrictionExpHash;
				QHash<CExpressionVariable*, CExpressionVariable*> varSubVarSuperRestHash;


				for (CAxiomExpression* assExp : assExps) {
					bool supported = false;
					CSubDataPropertyOfExpression* subDataPropOfAssExp = dynamic_cast<CSubDataPropertyOfExpression*>(assExp);
					if (subDataPropOfAssExp) {
						supported = true;
						bool hasVariable = false;
						CDataPropertyVariableExpression* subDataPropVarExp = dynamic_cast<CDataPropertyVariableExpression*>(subDataPropOfAssExp->getSubDataPropertyTermExpression());
						if (subDataPropVarExp) {
							hasVariable = true;
							lastVarExpression = subDataPropVarExp;
							varExpSet.insert(subDataPropVarExp);
						}
						CObjectPropertyVariableExpression* superDataPropVarExp = dynamic_cast<CObjectPropertyVariableExpression*>(subDataPropOfAssExp->getSuperDataPropertyTermExpression());
						if (superDataPropVarExp) {
							hasVariable = true;
							lastVarExpression = superDataPropVarExp;
							varExpSet.insert(superDataPropVarExp);
						}

						if (subDataPropVarExp && !superDataPropVarExp) {
							varSuperDataPropRestrictionExpHash.insertMulti(subDataPropVarExp, subDataPropOfAssExp->getSuperDataPropertyTermExpression());
						} else if (!subDataPropVarExp && superDataPropVarExp) {
							varSubDataPropRestrictionExpHash.insertMulti(superDataPropVarExp, subDataPropOfAssExp->getSubDataPropertyTermExpression());
						} else if (subDataPropVarExp && superDataPropVarExp) {
							varSubVarSuperRestHash.insertMulti(subDataPropVarExp, superDataPropVarExp);
						}

						if (!hasVariable) {
							dataPropSubAxiomExpsEntList.append(subDataPropOfAssExp);
						}
					}

					if (!supported) {
						if (!unsupportedWarning) {
							LOG(WARN, getDomain(), logTr("Query uses unsupported axiom types, which will be ignored."), this);
						}
						unsupportedWarning = true;
					}
				}

				if (!dataPropSubAxiomExpsEntList.isEmpty()) {
					for (CSubDataPropertyOfExpression* subDataPropOfAssExp : dataPropSubAxiomExpsEntList) {
						CRole* subRole = getResolvedDataPropertyRole(subDataPropOfAssExp->getSubDataPropertyTermExpression());
						CRole* superRole = getResolvedDataPropertyRole(subDataPropOfAssExp->getSuperDataPropertyTermExpression());
						queryProcessing |= checkEntailmentRoleSubDataPropertyAxiom(subRole, superRole, queryProcessingData, answererContext);
					}
				}




				COptimizedComplexBuildingObjectPropertyVariableCompositionsItem* varBuildItem = new COptimizedComplexBuildingObjectPropertyVariableCompositionsItem(queryProcessingData);


				for (CExpressionVariable* varExp : varExpSet) {
					QList<CDataPropertyTermExpression*> superDataPropRestrictionExps = varSuperDataPropRestrictionExpHash.values(varExp);
					QList<CDataPropertyTermExpression*> subDataPropRestrictionExps = varSubDataPropRestrictionExpHash.values(varExp);


					for (CDataPropertyTermExpression* superDataPropRestrictionExp : superDataPropRestrictionExps) {
						CRole* superRole = getResolvedDataPropertyRole(superDataPropRestrictionExp);
						varBuildItem->addVariableComplexSuperRole((CPropertyVariableExpression*)varExp, superRole);
					}

					for (CDataPropertyTermExpression* subDataPropRestrictionExp : subDataPropRestrictionExps) {
						CRole* subrRole = getResolvedDataPropertyRole(subDataPropRestrictionExp);
						varBuildItem->addVariableComplexSubRole((CPropertyVariableExpression*)varExp, subrRole);
					}

					QList<CExpressionVariable*> superVarList = varSubVarSuperRestHash.values(varExp);
					for (CExpressionVariable* superVarExp : superVarList) {
						varBuildItem->addVariableSubSuperPropagation(varExp, superVarExp);
					}

				}

				queryProcessingData->setVariableBuildingItem(varBuildItem);
				queryProcessingData->incBuildingVariableItem();

				if (!queryProcessing) {
					varBuildItem->setProcessingQueued(true);
					mOntoAnsweringItem->addProcessingVariableBuildingItem(varBuildItem);
					queryProcessing = true;
				}

			}





			COptimizedComplexBuildingObjectPropertyVariableCompositionsItem* COptimizedComplexExpressionAnsweringHandler::initComplexAxiomsObjectPropertyVariablesQuery(CComplexAxiomsObjectPropertyVariablesAnsweringQuery* compAxPropVarQuery, CAnswererContext* answererContext, CComplexQueryExpressionProcessingData* queryProcessingData, bool &queryCompleted, CConcreteOntology* testingOnto, bool &queryProcessing) {
				QList<CAxiomExpression*> assExps = *compAxPropVarQuery->getAxiomExpressions();

				QList<CSubObjectPropertyOfExpression*> objectPropSubAxiomExpsEntList;
				CExpressionVariable* lastVarExpression = nullptr;
				QSet<CExpressionVariable*> varExpSet;
				bool unsupportedWarning = false;


				QHash<CExpressionVariable*, CObjectPropertyTermExpression*> varSuperObjPropRestrictionExpHash;
				QHash<CExpressionVariable*, CObjectPropertyTermExpression*> varSubObjPropRestrictionExpHash;
				QHash<CExpressionVariable*, CExpressionVariable*> varSubVarSuperRestHash;


				for (CAxiomExpression* assExp : assExps) {
					bool supported = false;
					CSubObjectPropertyOfExpression* subObjPropOfAssExp = dynamic_cast<CSubObjectPropertyOfExpression*>(assExp);
					if (subObjPropOfAssExp) {
						bool hasVariable = false;
						CEXPRESSIONLIST<CObjectPropertyTermExpression*>* subObjPropList = subObjPropOfAssExp->getSubObjectPropertyTermExpressionList();
						if (subObjPropList->size() == 1) {
							supported = true;
							CObjectPropertyVariableExpression* subObjPropVarExp = dynamic_cast<CObjectPropertyVariableExpression*>(subObjPropList->first());
							if (subObjPropVarExp) {
								hasVariable = true;
								lastVarExpression = subObjPropVarExp;
								varExpSet.insert(subObjPropVarExp);
							}
							CObjectPropertyVariableExpression* superObjPropVarExp = dynamic_cast<CObjectPropertyVariableExpression*>(subObjPropOfAssExp->getSuperObjectPropertyTermExpression());
							if (superObjPropVarExp) {
								hasVariable = true;
								lastVarExpression = superObjPropVarExp;
								varExpSet.insert(superObjPropVarExp);
							}

							if (subObjPropVarExp && !superObjPropVarExp) {
								varSuperObjPropRestrictionExpHash.insertMulti(subObjPropVarExp, subObjPropOfAssExp->getSuperObjectPropertyTermExpression());
							} else if (!subObjPropVarExp && superObjPropVarExp) {
								varSubObjPropRestrictionExpHash.insertMulti(superObjPropVarExp, subObjPropOfAssExp->getSubObjectPropertyTermExpressionList()->first());
							} else if (subObjPropVarExp && superObjPropVarExp) {
								varSubVarSuperRestHash.insertMulti(subObjPropVarExp, superObjPropVarExp);
							}

							if (!hasVariable) {
								objectPropSubAxiomExpsEntList.append(subObjPropOfAssExp);
							}
						}
					} 
					
					if (!supported) {
						if (!unsupportedWarning) {
							LOG(WARN, getDomain(), logTr("Query uses unsupported axiom types, which will be ignored."), this);
						}
						unsupportedWarning = true;
					}
				}

				if (!objectPropSubAxiomExpsEntList.isEmpty()) {
					for (CSubObjectPropertyOfExpression* subObjPropOfAssExp : objectPropSubAxiomExpsEntList) {
						CRole* subRole = getInverseResolvedObjectPropertyRole(subObjPropOfAssExp->getSubObjectPropertyTermExpressionList()->first());
						CRole* superRole = getInverseResolvedObjectPropertyRole(subObjPropOfAssExp->getSuperObjectPropertyTermExpression());
						queryProcessing |= checkEntailmentRoleSubObjectPropertyAxiom(subRole, superRole, queryProcessingData, answererContext);
					}
				}




				COptimizedComplexBuildingObjectPropertyVariableCompositionsItem* varBuildItem = new COptimizedComplexBuildingObjectPropertyVariableCompositionsItem(queryProcessingData);


				for (CExpressionVariable* varExp : varExpSet) {
					QList<CObjectPropertyTermExpression*> superObjPropRestrictionExps = varSuperObjPropRestrictionExpHash.values(varExp);
					QList<CObjectPropertyTermExpression*> subObjPropRestrictionExps = varSubObjPropRestrictionExpHash.values(varExp);


					for (CObjectPropertyTermExpression* superObjPropRestrictionExp : superObjPropRestrictionExps) {
						CRole* superRole = getInverseResolvedObjectPropertyRole(superObjPropRestrictionExp);
						varBuildItem->addVariableComplexSuperRole((CPropertyVariableExpression*)varExp, superRole);
					}

					for (CObjectPropertyTermExpression* subObjPropRestrictionExp : subObjPropRestrictionExps) {
						CRole* subrRole = getInverseResolvedObjectPropertyRole(subObjPropRestrictionExp);
						varBuildItem->addVariableComplexSubRole((CPropertyVariableExpression*)varExp, subrRole);
					}

					QList<CExpressionVariable*> superVarList = varSubVarSuperRestHash.values(varExp);
					for (CExpressionVariable* superVarExp : superVarList) {
						varBuildItem->addVariableSubSuperPropagation(varExp, superVarExp);
					}

				}

				queryProcessingData->setVariableBuildingItem(varBuildItem);
				queryProcessingData->incBuildingVariableItem();

				if (!queryProcessing) {
					varBuildItem->setProcessingQueued(true);
					mOntoAnsweringItem->addProcessingVariableBuildingItem(varBuildItem);
					queryProcessing = true;
				}
				return varBuildItem;

			}



			COptimizedComplexBuildingClassVariableCompositionsItem* COptimizedComplexExpressionAnsweringHandler::initComplexAxiomsClassVariablesQuery(CComplexAxiomsClassVariablesAnsweringQuery* compAxClassVarQuery, CAnswererContext* answererContext, CComplexQueryExpressionProcessingData* queryProcessingData, bool &queryCompleted, CConcreteOntology* testingOnto, bool &queryProcessing) {
				QList<CAxiomExpression*> assExps = *compAxClassVarQuery->getAxiomExpressions();

				QList<CAxiomExpression*> assExpsVarList;
				QList<CSubClassOfExpression*> classSubClassExpsEntList;
				CExpressionVariable* lastVarExpression = nullptr;
				QSet<CExpressionVariable*> varExpSet;
				bool unsupportedWarning = false;

				for (CAxiomExpression* assExp : assExps) {
					CSubClassOfExpression* subClasOfAssExp = dynamic_cast<CSubClassOfExpression*>(assExp);
					if (subClasOfAssExp) {
						bool hasVariable = false;
						CClassVariableExpression* subClassVarExp = dynamic_cast<CClassVariableExpression*>(subClasOfAssExp->getFirstClassTermExpression());
						if (subClassVarExp) {
							hasVariable = true;
							lastVarExpression = subClassVarExp;
							varExpSet.insert(subClassVarExp);
						}
						CClassVariableExpression* superClassVarExp = dynamic_cast<CClassVariableExpression*>(subClasOfAssExp->getSecondClassTermExpression());
						if (superClassVarExp) {
							hasVariable = true;
							lastVarExpression = superClassVarExp;
							varExpSet.insert(superClassVarExp);
						}
						if (!hasVariable) {
							classSubClassExpsEntList.append(subClasOfAssExp);
						} else {
							assExpsVarList.append(subClasOfAssExp);
						}
					} else {
						if (!unsupportedWarning) {
							LOG(WARN, getDomain(), logTr("Query uses unsupported axiom types, which will be ignored."), this);
						}
						unsupportedWarning = true;
					}
				}

				if (!classSubClassExpsEntList.isEmpty()) {
					QList<QPair<CClassTermExpression*, CClassTermExpression*>> subSuperClassTestExpList;
					if (!classSubClassExpsEntList.isEmpty()) {
						mTestingOntologyBuilder->initializeBuilding();
						for (CSubClassOfExpression* subClasOfAssExp : classSubClassExpsEntList) {
							CClassTermExpression* subClassTermExp = subClasOfAssExp->getFirstClassTermExpression();
							CClassTermExpression* superClassTermExp = subClasOfAssExp->getSecondClassTermExpression();
							subSuperClassTestExpList.append(QPair<CClassTermExpression*, CClassTermExpression*>((CClassTermExpression*)mTestingOntologyBuilder->rebuildExpression(subClassTermExp), (CClassTermExpression*)mTestingOntologyBuilder->rebuildExpression(superClassTermExp)));
						}					

						mTestingOntologyBuilder->completeBuilding();
						mTestingOntologyPreprocessor->preprocess(testingOnto, testingOnto->getConfiguration());
					}
					
					for (QPair<CClassTermExpression*, CClassTermExpression*> classIndiPair : subSuperClassTestExpList) {
						CClassTermExpression* subClassTermExp = classIndiPair.first;
						CClassTermExpression* superClassTermExp = classIndiPair.second;
						CConcept* subConcept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value(subClassTermExp);
						CConcept* superConcept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value(superClassTermExp);
						bool subConNegation = subConcept->hasMappingNegation();
						bool superConNegation = superConcept->hasMappingNegation();
						queryProcessing |= checkEntailmentConceptSubClassAxiom(subConcept, subConNegation, superConcept, superConNegation, queryProcessingData, answererContext);
					}

				}



				QHash<CExpressionVariable*, CBuildExpression*> varSuperClassRestrictionExpHash;
				QHash<CExpressionVariable*, CBuildExpression*> varSubClassRestrictionExpHash;
				QHash<CExpressionVariable*, CExpressionVariable*> varSubVarSuperRestHash;
				for (CExpressionVariable* varExp : varExpSet) {
					for (CAxiomExpression* assExp : assExpsVarList) {
						CSubClassOfExpression* subClasOfAssExp = dynamic_cast<CSubClassOfExpression*>(assExp);
						if (subClasOfAssExp) {
							CClassVariableExpression* subClassVarExp = dynamic_cast<CClassVariableExpression*>(subClasOfAssExp->getFirstClassTermExpression());
							CClassVariableExpression* superClassVarExp = dynamic_cast<CClassVariableExpression*>(subClasOfAssExp->getSecondClassTermExpression());
							if (subClassVarExp && !superClassVarExp) {
								varSuperClassRestrictionExpHash.insertMulti(subClassVarExp, subClasOfAssExp->getSecondClassTermExpression());
							}
							if (!subClassVarExp && superClassVarExp) {
								varSubClassRestrictionExpHash.insertMulti(superClassVarExp, subClasOfAssExp->getFirstClassTermExpression());
							}
							if (varExp == subClassVarExp && subClassVarExp && superClassVarExp) {
								varSubVarSuperRestHash.insertMulti(subClassVarExp, superClassVarExp);
							}
						}
					}
				}





				mTestingOntologyBuilder->initializeBuilding();
				for (CExpressionVariable* varExp : varExpSet) {
					QList<CBuildExpression*> superClassRestrictionExps = varSuperClassRestrictionExpHash.values(varExp);
					QList<CBuildExpression*> subClassRestrictionExps = varSubClassRestrictionExpHash.values(varExp);

					if (!superClassRestrictionExps.isEmpty()) {
						QList<CBuildExpression*> instTestingExpList;

						CBuildExpression* instTestingExp = nullptr;
						for (CBuildExpression* superClassExp : superClassRestrictionExps) {
							instTestingExpList.append(mTestingOntologyBuilder->rebuildExpression(superClassExp));
						}

						if (instTestingExpList.size() > 1) {
							instTestingExp = mTestingOntologyBuilder->getObjectIntersectionOf(instTestingExpList);
							varSuperClassRestrictionExpHash.insert(varExp, instTestingExp);
						}
					}

					if (!subClassRestrictionExps.isEmpty()) {
						QList<CBuildExpression*> instTestingExpList;

						CBuildExpression* instTestingExp = nullptr;
						for (CBuildExpression* subClassExp : subClassRestrictionExps) {
							instTestingExpList.append(mTestingOntologyBuilder->rebuildExpression(subClassExp));
						}

						if (instTestingExpList.size() > 1) {
							instTestingExp = mTestingOntologyBuilder->getObjectIntersectionOf(instTestingExpList);
							varSubClassRestrictionExpHash.insert(varExp, instTestingExp);
						}
					}
				}

				mTestingOntologyBuilder->completeBuilding();
				mTestingOntologyPreprocessor->preprocess(testingOnto, testingOnto->getConfiguration());

				COptimizedComplexBuildingClassVariableCompositionsItem* varBuildItem = new COptimizedComplexBuildingClassVariableCompositionsItem(queryProcessingData);


				for (CExpressionVariable* varExp : varExpSet) {
					CBuildExpression* superClassRestrictionExp = varSuperClassRestrictionExpHash.value(varExp);
					CBuildExpression* subClassRestrictionExp = varSubClassRestrictionExpHash.value(varExp);

					if (superClassRestrictionExp) {
						CConcept* superResConcept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value((CClassTermExpression*)superClassRestrictionExp);
						bool superResConceptNegation = superResConcept->hasMappingNegation();

						COptimizedComplexConceptItem* assocConceptItem = nullptr;
						queryProcessing |= initializeComplexConceptQueryProcessing(nullptr, varBuildItem, superResConcept, superResConceptNegation, false, false, true, false, false, 0, &assocConceptItem);

						varBuildItem->addVariableComplexSuperConceptItem((CClassVariableExpression*)varExp, assocConceptItem);
					}

					if (subClassRestrictionExp) {
						CConcept* subResConcept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value((CClassTermExpression*)subClassRestrictionExp);
						bool subResConceptNegation = subResConcept->hasMappingNegation();

						COptimizedComplexConceptItem* assocConceptItem = nullptr;
						queryProcessing |= initializeComplexConceptQueryProcessing(nullptr, varBuildItem, subResConcept, subResConceptNegation, false, true, false, false, false, 0, &assocConceptItem);

						varBuildItem->addVariableComplexSubConceptItem((CClassVariableExpression*)varExp, assocConceptItem);
					}

					QList<CExpressionVariable*> superVarList = varSubVarSuperRestHash.values(varExp);
					for (CExpressionVariable* superVarExp : superVarList) {
						varBuildItem->addVariableSubSuperPropagation(varExp, superVarExp);
					}

				}

				queryProcessingData->setVariableBuildingItem(varBuildItem);
				queryProcessingData->incBuildingVariableItem();

				if (!queryProcessing) {
					varBuildItem->setProcessingQueued(true);
					mOntoAnsweringItem->addProcessingVariableBuildingItem(varBuildItem);
					queryProcessing = true;
				}

				return varBuildItem;

			}





			COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* COptimizedComplexExpressionAnsweringHandler::initComplexAxiomsIndividualMixedVariablesQuery(CComplexAxiomsIndividualMixedVariablesAnsweringQuery* compAxIndMixedVarQuery, CAnswererContext* answererContext, CComplexQueryExpressionProcessingData* queryProcessingData, bool &queryCompleted, CConcreteOntology* testingOnto, bool &queryProcessing) {
				QList<CAxiomExpression*> axExps = *compAxIndMixedVarQuery->getAxiomExpressions();
				QList<CExpressionVariable*>* disVarList = compAxIndMixedVarQuery->getDistinguishedVariableExpressions();
				QSet<CExpressionVariable*> disVarSet = disVarList->toSet();

				COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* varBuildItem = new COptimizedComplexBuildingIndividualMixedVariableCompositionsItem(queryProcessingData);

				bool unsupportedWarning = false;

				QList<CAxiomExpression*> propExtSubQueryPropAssList;
				QList<CAxiomExpression*> subQueryIndiAssExps;
				QList<CAxiomExpression*> subQueryClassAxExps;
				QList<CAxiomExpression*> subQueryDisjClassAxExps;
				QSet<CExpressionVariable*> extensionIndiVarExpSet;
				QSet<CExpressionVariable*> extensionClassVarExpSet;
				QSet<CExpressionVariable*> extensionPropertyVarExpSet;
				QSet<CExpressionVariable*> subQueryIndiVarExpSet;
				QHash<CExpressionVariable*,cint64> varExpOccurenceCountHash;
				QList<CObjectPropertyAssertionExpression*> propExtSingleOccCheckingPropAssList;
				for (CAxiomExpression* assExp : axExps) {
					bool supported = false;
					bool assertionAxiom = false;

					CSubClassOfExpression* subClassAxExp = dynamic_cast<CSubClassOfExpression*>(assExp);
					if (subClassAxExp) {
						supported = true;
						subQueryClassAxExps.append(subClassAxExp);
					}
					//CEquivalentClassesExpression* eqClassAxExp = dynamic_cast<CEquivalentClassesExpression*>(assExp);
					//if (eqClassAxExp) {
					//	supported = true;
					//	subQueryClassAxExps.append(eqClassAxExp);
					//}


					CSubObjectPropertyOfExpression* subPropAxExp = dynamic_cast<CSubObjectPropertyOfExpression*>(assExp);
					if (subPropAxExp) {

						CEXPRESSIONLIST<CObjectPropertyTermExpression*>* subObjPropList = subPropAxExp->getSubObjectPropertyTermExpressionList();
						if (subObjPropList->size() == 1) {
							supported = true;
							propExtSubQueryPropAssList.append(subPropAxExp);
						}
					}


					CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(assExp);
					if (classAssExp) {
						CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(classAssExp->getIndividualTermExpression());
						CClassVariableExpression* classVarExp = dynamic_cast<CClassVariableExpression*>(classAssExp->getClassTermExpression());
						if (indiVarExp) {
							varExpOccurenceCountHash[indiVarExp]++;
						}
						if (classVarExp) {
							varExpOccurenceCountHash[classVarExp]++;
						}
						if (indiVarExp && classVarExp) {
							extensionIndiVarExpSet.insert(indiVarExp);
							extensionClassVarExpSet.insert(classVarExp);
							varBuildItem->addIndividualVariableTypeVariableExtension(indiVarExp, classVarExp);
							supported = true;
						} else if (!classVarExp) {
							assertionAxiom = true;
							supported = true;
							subQueryIndiAssExps.append(classAssExp);
							if (indiVarExp) {
								subQueryIndiVarExpSet.insert(indiVarExp);
							}
						}
					}
					CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
					if (propAssExp) {
						bool hasVariable = false;
						CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getFirstIndividualTermExpression());
						CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getSecondIndividualTermExpression());
						CPropertyVariableExpression* propVarExp = dynamic_cast<CPropertyVariableExpression*>(propAssExp->getObjectPropertyTermExpression());
						if (firstVarExp) {
							varExpOccurenceCountHash[firstVarExp]++;
						}
						if (secondVarExp) {
							varExpOccurenceCountHash[secondVarExp]++;
						}
						if (propVarExp) {
							varExpOccurenceCountHash[propVarExp]++;
						}
						if (!propVarExp) {
							assertionAxiom = true;
							supported = true;
							subQueryIndiAssExps.append(propAssExp);
							if (firstVarExp) {
								subQueryIndiVarExpSet.insert(firstVarExp);
							}
							if (secondVarExp) {
								subQueryIndiVarExpSet.insert(secondVarExp);
							}
						} else if (propVarExp && firstVarExp && secondVarExp) {
							if (compAxIndMixedVarQuery->isDistinctRequired() && (!disVarSet.contains(secondVarExp) || !disVarSet.contains(firstVarExp))) {
								propExtSingleOccCheckingPropAssList.append(propAssExp);
								supported = true;
							}
						}
					}
					CDataPropertyAssertionExpression* dataPropAssExp = dynamic_cast<CDataPropertyAssertionExpression*>(assExp);
					if (dataPropAssExp) {
						assertionAxiom = true;
						bool hasVariable = false;
						CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(dataPropAssExp->getIndividualTermExpression());
						CDataLiteralVariableExpression* dataVarExp = dynamic_cast<CDataLiteralVariableExpression*>(dataPropAssExp->getDataLiteralTermExpression());
						CPropertyVariableExpression* propVarExp = dynamic_cast<CPropertyVariableExpression*>(dataPropAssExp->getDataPropertyTermExpression());
						if (indiVarExp) {
							varExpOccurenceCountHash[indiVarExp]++;
						}
						if (dataVarExp) {
							varExpOccurenceCountHash[dataVarExp]++;
						}
						if (propVarExp) {
							varExpOccurenceCountHash[propVarExp]++;
						}
						if (!propVarExp) {
							assertionAxiom = true;
							supported = true;
							subQueryIndiAssExps.append(dataPropAssExp);
							if (indiVarExp) {
								subQueryIndiVarExpSet.insert(indiVarExp);
							}
							if (dataVarExp) {
								subQueryIndiVarExpSet.insert(dataVarExp);
							}
						}
					}
					if (!supported) {
						if (!unsupportedWarning) {
							LOG(WARN, getDomain(), logTr("Query uses unsupported axiom types, which will be ignored."), this);
						}
						unsupportedWarning = true;
					}
				}


				for (CObjectPropertyAssertionExpression* propAssExp : propExtSingleOccCheckingPropAssList) {
					bool supported = true;
					CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getFirstIndividualTermExpression());
					CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getSecondIndividualTermExpression());
					CPropertyVariableExpression* propVarExp = dynamic_cast<CPropertyVariableExpression*>(propAssExp->getObjectPropertyTermExpression());
					CIndividualVariableExpression* indiVarExp = nullptr;
					if (varExpOccurenceCountHash[firstVarExp] > 1) {
						indiVarExp = firstVarExp;
					} else {
						indiVarExp = secondVarExp;
					}
					if (varExpOccurenceCountHash[firstVarExp] > 1 && varExpOccurenceCountHash[secondVarExp] > 1) {
						supported = false;
					} else {
						extensionIndiVarExpSet.insert(indiVarExp);
						extensionPropertyVarExpSet.insert(propVarExp);
						varBuildItem->addIndividualVariablePropertyVariableExtension(indiVarExp, propVarExp);
					}
					if (!supported) {
						if (!unsupportedWarning) {
							LOG(WARN, getDomain(), logTr("Query uses unsupported axiom types, which will be ignored."), this);
						}
						unsupportedWarning = true;
					}
				}


				if (subQueryIndiAssExps.isEmpty() && !extensionIndiVarExpSet.isEmpty()) {
					mTestingOntologyBuilder->initializeBuilding();
					CClassTermExpression* topClassExp = mOntoAnsweringItem->getTestingOntology(false)->getBuildData()->getTopClassExpression();
					CIndividualVariableExpression* indiVarExp = (CIndividualVariableExpression*)*extensionIndiVarExpSet.begin();
					CAxiomExpression* axiom = mTestingOntologyBuilder->getClassAssertion(indiVarExp, topClassExp);
					mTestingOntologyBuilder->completeBuilding();
					subQueryIndiAssExps.append(axiom);
				}


				if (!subQueryIndiAssExps.isEmpty()) {
					QList<CExpressionVariable*> subQueryDisIndiVarList;
					for (CExpressionVariable* varExp : subQueryIndiVarExpSet) {
						if (disVarSet.contains(varExp) || extensionIndiVarExpSet.contains(varExp)) {
							subQueryDisIndiVarList.append(varExp);
						}
					}
					CComplexAssertionsIndividualVariablesAnsweringQuery* subIndiVarQuery = new CComplexAssertionsIndividualVariablesAnsweringQuery(mOntoAnsweringItem->getOntology(), testingOnto, subQueryIndiAssExps, subQueryDisIndiVarList, mOntoAnsweringItem->getCalculationConfiguration(), compAxIndMixedVarQuery->getQueryName() + "-Assertion-Individual-Sub-Variables-Query");
					bool subQueryCompleted = false;
					bool subQueryProcessing = false;
					COptimizedComplexBuildingIndividualVariableCompositionsItem* subQueryIndiBuildItem = initComplexAssertionsIndividualVariablesQuery(subIndiVarQuery, answererContext, queryProcessingData, subQueryCompleted, testingOnto, subQueryProcessing, true);
					varBuildItem->addIndividualVariableSubQueryItem(subIndiVarQuery, subQueryIndiBuildItem);
					if (!subQueryCompleted) {
						CComputedItemDataNotificationLinker* notLinker = mOntoAnsweringItem->createComputedItemDataNotificationLinker();
						notLinker->setBuildingVariableItemProcessingItem(varBuildItem);
						subQueryIndiBuildItem->addNotificationLinker(notLinker);
						varBuildItem->incWaitingSubVariableBuildingItemCount();						
					} 
					if (subQueryProcessing) {
						queryProcessing = true;
					}


					cint64 classSubQueryId = 0;
					function<void(QSet<CAxiomExpression*> nextSubQueryClassAxExps, QList<CExpressionVariable*> subQueryDisClassVarList, QSet<CExpressionVariable*> handledVarExpSet)> classSubQueryHandlerFunc = [&](QSet<CAxiomExpression*> nextSubQueryClassAxExps, QList<CExpressionVariable*> subQueryDisClassVarList, QSet<CExpressionVariable*> handledVarExpSet) {
						CComplexAxiomsClassVariablesAnsweringQuery* subClassVarQuery = new CComplexAxiomsClassVariablesAnsweringQuery(mOntoAnsweringItem->getOntology(), testingOnto, nextSubQueryClassAxExps.toList(), subQueryDisClassVarList, mOntoAnsweringItem->getCalculationConfiguration(), QString("%1-Assertion-Class-Sub-Variables-Query-%1").arg(compAxIndMixedVarQuery->getQueryName()).arg(classSubQueryId++));
						bool classSubQueryCompleted = false;
						bool classSubQueryProcessing = false;
						COptimizedComplexBuildingClassVariableCompositionsItem* subQueryClassBuildItem = initComplexAxiomsClassVariablesQuery(subClassVarQuery, answererContext, queryProcessingData, classSubQueryCompleted, testingOnto, classSubQueryProcessing);
						varBuildItem->addClassVariableSubQueryItem(subClassVarQuery, subQueryClassBuildItem);
						for (CExpressionVariable* varExp : handledVarExpSet) {
							varBuildItem->setExtensionClassVariableSubQueryBuildingItem((CClassVariableExpression*)varExp, subQueryClassBuildItem);
						}
						if (!classSubQueryCompleted) {
							CComputedItemDataNotificationLinker* notLinker = mOntoAnsweringItem->createComputedItemDataNotificationLinker();
							notLinker->setBuildingVariableItemProcessingItem(varBuildItem);
							subQueryClassBuildItem->addNotificationLinker(notLinker);
							varBuildItem->incWaitingSubVariableBuildingItemCount();
						}
						if (classSubQueryProcessing) {
							queryProcessing = true;
						}
					};

					createIndividualMixedVariableConnectedSplitSubQueries(extensionClassVarExpSet, subQueryClassAxExps, disVarSet, extensionIndiVarExpSet, classSubQueryHandlerFunc);




					cint64 propertySubQueryId = 0;
					function<void(QSet<CAxiomExpression*> nextSubQueryPropertyAxExps, QList<CExpressionVariable*> subQueryDisClassVarList, QSet<CExpressionVariable*> handledVarExpSet)> propertySubQueryHandlerFunc = [&](QSet<CAxiomExpression*> nextSubQueryPropertyAxExps, QList<CExpressionVariable*> subQueryDisClassVarList, QSet<CExpressionVariable*> handledVarExpSet) {
						CComplexAxiomsObjectPropertyVariablesAnsweringQuery* subPropertyVarQuery = new CComplexAxiomsObjectPropertyVariablesAnsweringQuery(mOntoAnsweringItem->getOntology(), testingOnto, nextSubQueryPropertyAxExps.toList(), subQueryDisClassVarList, mOntoAnsweringItem->getCalculationConfiguration(), QString("%1-Assertion-Property-Sub-Variables-Query-%1").arg(compAxIndMixedVarQuery->getQueryName()).arg(classSubQueryId++));
						bool classSubQueryCompleted = false;
						bool classSubQueryProcessing = false;
						COptimizedComplexBuildingPropertyVariableCompositionsItem* subQueryPropertyBuildItem = initComplexAxiomsObjectPropertyVariablesQuery(subPropertyVarQuery, answererContext, queryProcessingData, classSubQueryCompleted, testingOnto, classSubQueryProcessing);
						varBuildItem->addPropertyVariableSubQueryItem(subPropertyVarQuery, subQueryPropertyBuildItem);
						for (CExpressionVariable* varExp : handledVarExpSet) {
							varBuildItem->setExtensionPropertyVariableSubQueryBuildingItem((CPropertyVariableExpression*)varExp, subQueryPropertyBuildItem);
						}
						if (!classSubQueryCompleted) {
							CComputedItemDataNotificationLinker* notLinker = mOntoAnsweringItem->createComputedItemDataNotificationLinker();
							notLinker->setBuildingVariableItemProcessingItem(varBuildItem);
							subQueryPropertyBuildItem->addNotificationLinker(notLinker);
							varBuildItem->incWaitingSubVariableBuildingItemCount();
						}
						if (classSubQueryProcessing) {
							queryProcessing = true;
						}
					};

					createIndividualMixedVariableConnectedSplitSubQueries(extensionPropertyVarExpSet, propExtSubQueryPropAssList, disVarSet, extensionIndiVarExpSet, propertySubQueryHandlerFunc);



					queryProcessingData->incBuildingVariableItem();
					queryProcessingData->setVariableBuildingItem(varBuildItem);

					if (!queryProcessing) {
						mOntoAnsweringItem->addProcessingVariableBuildingItem(varBuildItem);
						varBuildItem->setProcessingQueued(true);
					}

				}
				return varBuildItem;
			}
				 



			void COptimizedComplexExpressionAnsweringHandler::createIndividualMixedVariableConnectedSplitSubQueries(QSet<CExpressionVariable *> extensionVarExpSet, QList<CAxiomExpression *> subQueryAxExps, QSet<CExpressionVariable *> &disVarSet, QSet<CExpressionVariable *> &extensionIndiVarExpSet, function<void(QSet<CAxiomExpression *> nextSubQueryClassAxExps, QList<CExpressionVariable *> subQueryDisClassVarList, QSet<CExpressionVariable *> handledVarExpSet)> subQueryHandlerFunc) {
				QSet<CExpressionVariable*> handledExtensionVarExpSet;
				for (CExpressionVariable* extVarExp : extensionVarExpSet) {
					if (!handledExtensionVarExpSet.contains(extVarExp)) {

						QSet<CExpressionVariable*> handledVarExpSet;
						QList<CExpressionVariable*> handlingVarExpList;
						handlingVarExpList.append(extVarExp);
						handledVarExpSet.insert(extVarExp);

						QSet<CAxiomExpression*> nextSubQueryAxExps;

						while (!handlingVarExpList.isEmpty()) {
							CExpressionVariable* handleIndiVar = handlingVarExpList.takeFirst();

							for (CAxiomExpression* subQuAx : subQueryAxExps) {

								if (!nextSubQueryAxExps.contains(subQuAx)) {
									CSubClassOfExpression* subClassAxExp = dynamic_cast<CSubClassOfExpression*>(subQuAx);
									if (subClassAxExp) {
										CClassVariableExpression* firstClassVarExp = dynamic_cast<CClassVariableExpression*>(subClassAxExp->getFirstClassTermExpression());
										CClassVariableExpression* secondClassVarExp = dynamic_cast<CClassVariableExpression*>(subClassAxExp->getSecondClassTermExpression());
										if (handledVarExpSet.contains(firstClassVarExp) || handledVarExpSet.contains(secondClassVarExp)) {
											nextSubQueryAxExps.insert(subClassAxExp);

											if (firstClassVarExp && !handledVarExpSet.contains(firstClassVarExp)) {
												handledVarExpSet.insert(firstClassVarExp);
												handlingVarExpList.append(firstClassVarExp);
											}
											if (secondClassVarExp && !handledVarExpSet.contains(secondClassVarExp)) {
												handledVarExpSet.insert(secondClassVarExp);
												handlingVarExpList.append(secondClassVarExp);
											}
										}
									}

									CSubObjectPropertyOfExpression* subPropAxExp = dynamic_cast<CSubObjectPropertyOfExpression*>(subQuAx);
									if (subPropAxExp) {

										CEXPRESSIONLIST<CObjectPropertyTermExpression*>* subObjPropList = subPropAxExp->getSubObjectPropertyTermExpressionList();
										if (subObjPropList->size() == 1) {
											CObjectPropertyVariableExpression* subObjPropVarExp = dynamic_cast<CObjectPropertyVariableExpression*>(subObjPropList->first());
											CObjectPropertyVariableExpression* superObjPropVarExp = dynamic_cast<CObjectPropertyVariableExpression*>(subPropAxExp->getSuperObjectPropertyTermExpression());
											if (handledVarExpSet.contains(subObjPropVarExp) || handledVarExpSet.contains(superObjPropVarExp)) {
												nextSubQueryAxExps.insert(subPropAxExp);
												if (subObjPropVarExp && !handledVarExpSet.contains(subObjPropVarExp)) {
													handledVarExpSet.insert(subObjPropVarExp);
													handlingVarExpList.append(subObjPropVarExp);
												}
												if (superObjPropVarExp && !handledVarExpSet.contains(superObjPropVarExp)) {
													handledVarExpSet.insert(superObjPropVarExp);
													handlingVarExpList.append(superObjPropVarExp);
												}
											}
										}

									}
								}

							}
						}


						if (!nextSubQueryAxExps.isEmpty()) {
							QList<CExpressionVariable*> subQueryDisVarList;
							for (CExpressionVariable* varExp : extensionVarExpSet) {
								if (handledVarExpSet.contains(varExp) && (disVarSet.contains(varExp) || extensionIndiVarExpSet.contains(varExp))) {
									subQueryDisVarList.append(varExp);
								}
							}

							subQueryHandlerFunc(nextSubQueryAxExps, subQueryDisVarList, handledVarExpSet);
						}

					}
				}
			}



			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexExpressionAnsweringHandler::initComplexAssertionsIndividualVariablesQuery(CComplexAssertionsIndividualVariablesAnsweringQuery* compAssIndVarQuery, CAnswererContext* answererContext, CComplexQueryExpressionProcessingData* queryProcessingData, bool &queryCompleted, CConcreteOntology* testingOnto, bool &queryProcessing, bool forceBuildingItem) {
				QList<CAxiomExpression*> assExps = *compAssIndVarQuery->getAxiomExpressions();
				QList<CExpressionVariable*>* disVarList = compAssIndVarQuery->getDistinguishedVariableExpressions();
				QSet<CExpressionVariable*> varExpSet;
				QList<CClassAssertionExpression*> classAssExpsEntList;
				QList<CClassAssertionExpression*> compClassAssExpsEntList;
				QList<CObjectPropertyAssertionExpression*> roleAssExpsEntList;
				QList<CDataPropertyAssertionExpression*> dataRoleAssExpsEntList;
				QList<CAxiomExpression*> assExpsVarList;
				CExpressionVariable* lastVarExpression = nullptr;

				QSet<CExpressionVariable*> answerIndiVariableSet(disVarList->toSet());



				bool nonInstantiatedTerm = false;
				if (mConfUnsatisfiableNonInstanceStatisticsChecking) {
					nonInstantiatedTerm = identifyNonInstantiation(assExps, answerIndiVariableSet, answererContext);
				}

				if (nonInstantiatedTerm) {
					finishQueryProcessingAsUnsatisfaible(queryProcessingData);
					queryCompleted = true;
				} else {


					if (mConfInterpretQueriesAsDistinct) {
						compAssIndVarQuery->setDistinctRequired(true);
					}



					if (mConfRedundantTermReduction) {
						cint64 prevCount = assExps.count();
						assExps = eliminateRedundantQueryTerms(assExps, queryProcessingData, answererContext);
						cint64 newCount = assExps.count();
						if (prevCount != newCount && mConfExtendedLogging) {
							LOG(INFO, getDomain(), logTr("Eliminated %1 redundant query terms.").arg(prevCount - newCount), this);
						}
					}



					for (CAxiomExpression* assExp : assExps) {
						CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(assExp);
						if (classAssExp) {
							CIndividualVariableExpression* varExp = dynamic_cast<CIndividualVariableExpression*>(classAssExp->getIndividualTermExpression());
							if (!varExp) {
								if (classAssExp->getClassTermExpression()->getType() == CBuildExpression::BETCLASS) {
									classAssExpsEntList.append(classAssExp);
								} else {
									compClassAssExpsEntList.append(classAssExp);
								}
							} else {
								lastVarExpression = varExp;
								assExpsVarList.append(classAssExp);
							}
						}
						CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
						if (propAssExp) {
							bool hasVariable = false;
							CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getFirstIndividualTermExpression());
							if (firstVarExp) {
								hasVariable = true;
								lastVarExpression = firstVarExp;
							}
							CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getSecondIndividualTermExpression());
							if (secondVarExp) {
								hasVariable = true;
								lastVarExpression = secondVarExp;
							}
							if (!hasVariable) {
								roleAssExpsEntList.append(propAssExp);
							} else {
								assExpsVarList.append(propAssExp);
							}
						}
						CDataPropertyAssertionExpression* dataPropAssExp = dynamic_cast<CDataPropertyAssertionExpression*>(assExp);
						if (dataPropAssExp) {
							bool hasVariable = false;
							CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(dataPropAssExp->getIndividualTermExpression());
							if (indiVarExp) {
								hasVariable = true;
								lastVarExpression = indiVarExp;
							}
							CDataLiteralVariableExpression* dataVarExp = dynamic_cast<CDataLiteralVariableExpression*>(dataPropAssExp->getDataLiteralTermExpression());
							if (dataVarExp) {
								hasVariable = true;
								lastVarExpression = dataVarExp;
							}
							if (!hasVariable) {
								dataRoleAssExpsEntList.append(dataPropAssExp);
							} else {
								assExpsVarList.append(dataPropAssExp);
							}

						}
					}


					QList<CVariableBindingFilteringLiteralComparison*> dataLitCompFilterList;
					for (CVariableBindingFiltering* filteringLinker = compAssIndVarQuery->getResultFilteringLinker(); filteringLinker; filteringLinker = filteringLinker->getNext()) {
						CVariableBindingFilteringLiteralComparison* dataLiteralCompFilter = dynamic_cast<CVariableBindingFilteringLiteralComparison*>(filteringLinker);
						if (dataLiteralCompFilter && !dataLiteralCompFilter->getDataLiteral()) {
							dataLitCompFilterList.append(dataLiteralCompFilter);
						}
					}


					if (!classAssExpsEntList.isEmpty() || !compClassAssExpsEntList.isEmpty() || !roleAssExpsEntList.isEmpty() || !dataRoleAssExpsEntList.isEmpty() || !dataLitCompFilterList.isEmpty()) {
						QList<QPair<CClassTermExpression*, CIndividualTermExpression*>> indClassInstTestExpList;
						QList<QPair<CVariableBindingFilteringLiteralComparison*, CDataLiteralExpression*>> dataFilterDataLiteralExpList;
						if (!compClassAssExpsEntList.isEmpty() || !dataRoleAssExpsEntList.isEmpty() || !dataLitCompFilterList.isEmpty()) {
							mTestingOntologyBuilder->initializeBuilding();
							for (CClassAssertionExpression* assExp : compClassAssExpsEntList) {
								CClassTermExpression* classTermExp = assExp->getClassTermExpression();
								indClassInstTestExpList.append(QPair<CClassTermExpression*, CIndividualTermExpression*>((CClassTermExpression*)mTestingOntologyBuilder->rebuildExpression(classTermExp), assExp->getIndividualTermExpression()));
							}
							for (CDataPropertyAssertionExpression* dataRoleAssExp : dataRoleAssExpsEntList) {
								CDataPropertyTermExpression* dataPropTermExp = dataRoleAssExp->getDataPropertyTermExpression();
								CIndividualTermExpression* indiTermExp = dataRoleAssExp->getIndividualTermExpression();
								CDataLiteralExpression* dataLiteralExp = (CDataLiteralExpression*)dataRoleAssExp->getDataLiteralTermExpression();
								dataLiteralExp = (CDataLiteralExpression*)mTestingOntologyBuilder->rebuildExpression(dataLiteralExp);
								indClassInstTestExpList.append(QPair<CClassTermExpression*, CIndividualTermExpression*>(mTestingOntologyBuilder->getDataHasValue(dataPropTermExp, dataLiteralExp), indiTermExp));
							}
							for (CVariableBindingFilteringLiteralComparison* dataLiteralCompFilter : dataLitCompFilterList) {
								CDataLiteralExpression* dataLitExp = dataLiteralCompFilter->getDataLiteralExpression();
								dataLitExp = (CDataLiteralExpression*)mTestingOntologyBuilder->rebuildExpression(dataLitExp);
								dataFilterDataLiteralExpList.append(QPair<CVariableBindingFilteringLiteralComparison*, CDataLiteralExpression*>(dataLiteralCompFilter, dataLitExp));
							}

							mTestingOntologyBuilder->completeBuilding();
							mTestingOntologyPreprocessor->preprocess(testingOnto, testingOnto->getConfiguration());
						}
						if (!dataFilterDataLiteralExpList.isEmpty()) {
							CPreProcessContextBase preprocessingContext(mOntoAnsweringItem->getTestingOntology(), mOntoAnsweringItem->getTestingOntology()->getConfiguration());
							CDataLiteralNormalizerPreProcess dataLitNormalizer;
							for (QPair<CVariableBindingFilteringLiteralComparison*, CDataLiteralExpression*> dataFilterDataLiteralExpPair : dataFilterDataLiteralExpList) {
								CVariableBindingFilteringLiteralComparison* dataLiteralCompFilter = dataFilterDataLiteralExpPair.first;
								CDataLiteralExpression* dataLitExp = dataFilterDataLiteralExpPair.second;
								CConcept* dataConcept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getDataRangeTermConceptMappingHash()->value(dataLitExp);
								CDataLiteral* dataLiteral = dataConcept->getDataLiteral();
								if (dataLiteral) {
									CDataLiteralValue* dataLiteralValue = dataLiteral->getDataLiteralValue();
									if (!dataLiteralValue) {
										dataLitNormalizer.preprocessDataConcept(dataConcept, mOntoAnsweringItem->getTestingOntology(), &preprocessingContext);
									}
									dataLiteralCompFilter->setDataLiteral(dataLiteral);
								}
							}
						}
						if (!classAssExpsEntList.isEmpty()) {
							for (CClassAssertionExpression* assExp : classAssExpsEntList) {
								indClassInstTestExpList.append(QPair<CClassTermExpression*, CIndividualTermExpression*>(assExp->getClassTermExpression(), assExp->getIndividualTermExpression()));
							}
						}
						for (QPair<CClassTermExpression*, CIndividualTermExpression*> classIndiPair : indClassInstTestExpList) {
							CClassTermExpression* classTermExp = classIndiPair.first;
							CIndividualTermExpression* indiTermExp = classIndiPair.second;
							CConcept* testingConcept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value(classTermExp);
							CIndividual* testingIndividual = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getIndividulTermIndiMappingHash()->value(indiTermExp);
							bool testingNegation = testingConcept->hasMappingNegation();
							queryProcessing |= checkEntailmentConceptAssertion(testingConcept, testingNegation, testingIndividual, queryProcessingData, answererContext);
						}
						for (CObjectPropertyAssertionExpression* roleAssExp : roleAssExpsEntList) {
							CObjectPropertyTermExpression* objPropTermExp = roleAssExp->getObjectPropertyTermExpression();
							CIndividualTermExpression* indiSourceTermExp = roleAssExp->getFirstIndividualTermExpression();
							CIndividualTermExpression* indiDestTermExp = roleAssExp->getSecondIndividualTermExpression();
							bool inversed = false;
							while (objPropTermExp->getType() == CBuildExpression::BETINVERSEOBJECTPROPERTYOF) {
								CInverseObjectPropertyOfExpression* invPropExp = (CInverseObjectPropertyOfExpression*)objPropTermExp;
								objPropTermExp = invPropExp->getInverseOfExpression();
								inversed = !inversed;
							}
							if (inversed) {
								CIndividualTermExpression* indiTermExp = indiSourceTermExp;
								indiSourceTermExp = indiDestTermExp;
								indiDestTermExp = indiTermExp;
							}
							CRole* testingRole = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(objPropTermExp);
							CIndividual* testingIndividualSource = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getIndividulTermIndiMappingHash()->value(indiSourceTermExp);
							CIndividual* testingIndividualDestination = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getIndividulTermIndiMappingHash()->value(indiDestTermExp);
							queryProcessing |= checkEntailmentRoleAssertion(testingRole, testingIndividualSource, testingIndividualDestination, queryProcessingData, answererContext);
						}

					}


					for (CAxiomExpression* assExp : assExpsVarList) {
						CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(assExp);
						if (classAssExp) {
							CIndividualVariableExpression* varExp = dynamic_cast<CIndividualVariableExpression*>(classAssExp->getIndividualTermExpression());
							if (varExp) {
								varExpSet.insert(varExp);
							}
						}
						CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
						if (propAssExp) {
							CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getFirstIndividualTermExpression());
							if (firstVarExp) {
								varExpSet.insert(firstVarExp);
							}
							CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getSecondIndividualTermExpression());
							if (secondVarExp) {
								varExpSet.insert(secondVarExp);
							}
						}
						CDataPropertyAssertionExpression* dataPropAssExp = dynamic_cast<CDataPropertyAssertionExpression*>(assExp);
						if (dataPropAssExp) {
							CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(dataPropAssExp->getIndividualTermExpression());
							if (indiVarExp) {
								varExpSet.insert(indiVarExp);
							}
							CDataLiteralVariableExpression* dataVarExp = dynamic_cast<CDataLiteralVariableExpression*>(dataPropAssExp->getDataLiteralTermExpression());
							if (dataVarExp) {
								varExpSet.insert(dataVarExp);
							}
						}

					}



					QSet<CExpressionVariable*> initialAnonymousIndiVariableSet;
					for (CExpressionVariable* varExp : varExpSet) {
						CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(varExp);
						if (indiVarExp && indiVarExp->isAnonymousVariable()) {
							initialAnonymousIndiVariableSet.insert(varExp);
						}
						if (indiVarExp && mConfInterpretNonAnswerIndividualVariablesAsAnonymousVariables && !answerIndiVariableSet.contains(varExp)) {
							if (varExp->getName().startsWith(mConfNonAnswerIndividualVariablesAsAnonymousVariablesInterpretingPrefixString)) {
								initialAnonymousIndiVariableSet.insert(varExp);
							}
						}
					}
					bool distinct = compAssIndVarQuery->isDistinctRequired();
					QSet<CExpressionVariable*> anonymousIndiVariableSet(initialAnonymousIndiVariableSet);
					QSet<CExpressionVariable*> nonEliminatableIndiVariableSet(answerIndiVariableSet);
					if (!distinct) {
						for (CExpressionVariable* varExp : varExpSet) {
							if (!anonymousIndiVariableSet.contains(varExp) && !answerIndiVariableSet.contains(varExp)) {
								nonEliminatableIndiVariableSet.insert(varExp);
							}
						}
					}





					QList<CObjectPropertyAssertionExpression*> bothVarPropAssPropagationList;
					QList<CDataPropertyAssertionExpression*> bothVarDataPropAssPropagationList;
					QList<CDataPropertyAssertionExpression*> onlyDataVarDataPropAssList;
					QHash<CExpressionVariable*, CAxiomExpression*> varExpAxiomExpHash;
					bool unsupportedWarning = false;
					for (CAxiomExpression* assExp : assExpsVarList) {
						CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(assExp);
						if (classAssExp) {
							CIndividualVariableExpression* varExp = dynamic_cast<CIndividualVariableExpression*>(classAssExp->getIndividualTermExpression());
							if (varExp) {
								varExpAxiomExpHash.insertMulti(varExp, assExp);
							}
						}
						CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
						if (propAssExp) {
							CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getFirstIndividualTermExpression());
							if (firstVarExp) {
								varExpAxiomExpHash.insertMulti(firstVarExp, assExp);
							}
							CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getSecondIndividualTermExpression());
							if (secondVarExp && secondVarExp != firstVarExp) {
								varExpAxiomExpHash.insertMulti(secondVarExp, assExp);
							}
							if (firstVarExp && secondVarExp) {
								if (!anonymousIndiVariableSet.contains(firstVarExp) && !anonymousIndiVariableSet.contains(secondVarExp)) {
									bothVarPropAssPropagationList.append(propAssExp);
								}
							}
						}
						CDataPropertyAssertionExpression* dataPropAssExp = dynamic_cast<CDataPropertyAssertionExpression*>(assExp);
						if (dataPropAssExp) {
							CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(dataPropAssExp->getIndividualTermExpression());
							if (firstVarExp) {
								varExpAxiomExpHash.insertMulti(firstVarExp, assExp);
							}
							CDataLiteralVariableExpression* dataLitVarExp = dynamic_cast<CDataLiteralVariableExpression*>(dataPropAssExp->getDataLiteralTermExpression());
							if (firstVarExp && dataLitVarExp) {
								if (!anonymousIndiVariableSet.contains(firstVarExp) && !anonymousIndiVariableSet.contains(dataLitVarExp)) {
									bothVarDataPropAssPropagationList.append(dataPropAssExp);
								}
							}
							if (!firstVarExp && dataLitVarExp) {
								onlyDataVarDataPropAssList.append(dataPropAssExp);
							}
						}
						if (!classAssExp && !propAssExp && !dataPropAssExp) {
							if (!unsupportedWarning) {
								LOG(WARN, getDomain(), logTr("Query uses unsupported axiom types, which will be ignored."), this);
							}
							unsupportedWarning = true;
						}
					}



					// TODO: check whether all role and concept terms of an instance query are indeed instantiated via the occurrence statistics cache

					bool allAnonymousVariables = varExpSet.size() == anonymousIndiVariableSet.size();
					QSet<CExpressionVariable*> convertedVariableSet;
					bool updateIndiVarAssPropLists = tryAnonymousToIndividualVariablesConversion(anonymousIndiVariableSet, varExpSet, distinct, answererContext, varExpAxiomExpHash, allAnonymousVariables, convertedVariableSet);


					// if individual variables are eliminated, then the remaining variables (that allow the eliminations) may have to be counted several times, which is managed by the following hash
					QHash<CExpressionVariable*, cint64> variableMultipleAnswerCountingHash;
					QList<CExpressionVariable*> indiIgnoreVarSet;
					if (mConfRedundantlyRestrictedVariablesElimination) {
						QSet<CExpressionVariable*> eliminationAllowedVarSet;
						eliminationAllowedVarSet += anonymousIndiVariableSet;
						QList<CExpressionVariable*> redEmbVarSet = getRedundantlyEmbeddedVariables(varExpAxiomExpHash, anonymousIndiVariableSet, nonEliminatableIndiVariableSet, queryProcessingData, answererContext);
						if (!redEmbVarSet.isEmpty()) {
							updateIndiVarAssPropLists = true;
							indiIgnoreVarSet = redEmbVarSet;

							if (mConfExtendedLogging) {
								QStringList varList;
								for (CExpressionVariable* var : redEmbVarSet) {
									varList.append(var->getName());
								}
								LOG(INFO, getDomain(), logTr("Eliminated %1 redundantly restricted variables: %2.").arg(varList.size()).arg(varList.join(", ")), this);
							}

							for (CExpressionVariable* var : redEmbVarSet) {
								varExpSet.remove(var);
							}

							for (QSet<CExpressionVariable*>::iterator it = anonymousIndiVariableSet.begin(), itEnd = anonymousIndiVariableSet.end(); it != itEnd; ) {
								CExpressionVariable* varExp = *it;
								if (redEmbVarSet.contains(varExp)) {
									it = anonymousIndiVariableSet.erase(it);
								} else {
									++it;
								}
							}
						}
					}


					if (updateIndiVarAssPropLists) {
						bothVarPropAssPropagationList.clear();
						bothVarDataPropAssPropagationList.clear();
						onlyDataVarDataPropAssList.clear();
						varExpAxiomExpHash.clear();
						for (CAxiomExpression* assExp : assExpsVarList) {
							CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(assExp);
							if (classAssExp) {
								CIndividualVariableExpression* varExp = dynamic_cast<CIndividualVariableExpression*>(classAssExp->getIndividualTermExpression());
								if (varExp && !indiIgnoreVarSet.contains(varExp)) {
									varExpAxiomExpHash.insertMulti(varExp, assExp);
								}
							}
							CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
							if (propAssExp) {
								CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getFirstIndividualTermExpression());
								CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getSecondIndividualTermExpression());
								if (firstVarExp && !indiIgnoreVarSet.contains(firstVarExp) && (!secondVarExp || !indiIgnoreVarSet.contains(secondVarExp))) {
									varExpAxiomExpHash.insertMulti(firstVarExp, assExp);
								}
								if (secondVarExp && secondVarExp != firstVarExp && !indiIgnoreVarSet.contains(secondVarExp) && (!firstVarExp || !indiIgnoreVarSet.contains(firstVarExp))) {
									varExpAxiomExpHash.insertMulti(secondVarExp, assExp);
								}
								if (firstVarExp && secondVarExp && !indiIgnoreVarSet.contains(firstVarExp) && !indiIgnoreVarSet.contains(secondVarExp)) {
									if (!anonymousIndiVariableSet.contains(firstVarExp) && !anonymousIndiVariableSet.contains(secondVarExp)) {
										bothVarPropAssPropagationList.append(propAssExp);
									}
								}
							}
							CDataPropertyAssertionExpression* dataPropAssExp = dynamic_cast<CDataPropertyAssertionExpression*>(assExp);
							if (dataPropAssExp) {
								CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(dataPropAssExp->getIndividualTermExpression());
								if (firstVarExp && !indiIgnoreVarSet.contains(firstVarExp)) {
									varExpAxiomExpHash.insertMulti(firstVarExp, assExp);
								}
								CDataLiteralVariableExpression* dataLitVarExp = dynamic_cast<CDataLiteralVariableExpression*>(dataPropAssExp->getDataLiteralTermExpression());
								if (firstVarExp && !indiIgnoreVarSet.contains(firstVarExp) && dataLitVarExp) {
									if (!anonymousIndiVariableSet.contains(firstVarExp) && !anonymousIndiVariableSet.contains(dataLitVarExp)) {
										bothVarDataPropAssPropagationList.append(dataPropAssExp);
									}
								}
								if (!firstVarExp && dataLitVarExp) {
									onlyDataVarDataPropAssList.append(dataPropAssExp);
								}
							}
						}
					}





					if (!forceBuildingItem && varExpSet.size() == 1 && dynamic_cast<CIndividualVariableExpression*>(lastVarExpression) && !anonymousIndiVariableSet.contains(lastVarExpression)) {
						// reduce to instance test
						//TODO: make sure expression ontology contains inverses for all roles
						LOG(INFO, getDomain(), logTr("Reducing query to single (complex) instance retrieval task."), this);
						CBuildExpression* instTestingExp = nullptr;
						QList<CBuildExpression*> instTestingExpList;
						mTestingOntologyBuilder->initializeBuilding();
						bool onlyAtomicConcept = true;
						for (CAxiomExpression* assExp : assExpsVarList) {
							CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(assExp);
							if (classAssExp) {
								instTestingExpList.append(mTestingOntologyBuilder->rebuildExpression(classAssExp->getClassTermExpression()));
								if (!dynamic_cast<CClassExpression*>(classAssExp->getClassTermExpression())) {
									onlyAtomicConcept = false;
								}
							}
							CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
							if (propAssExp) {
								onlyAtomicConcept = false;
								CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
								CIndividualTermExpression* firstIndiExp = propAssExp->getFirstIndividualTermExpression();
								CIndividualTermExpression* secondIndiExp = propAssExp->getSecondIndividualTermExpression();
								CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(firstIndiExp);
								CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(secondIndiExp);
								if (firstVarExp && secondVarExp) {
									instTestingExpList.append(mTestingOntologyBuilder->getObjectHasSelf(propExp));
								} else if (firstVarExp) {
									instTestingExpList.append(mTestingOntologyBuilder->getObjectHasValue(propExp, secondIndiExp));
								} else {
									instTestingExpList.append(mTestingOntologyBuilder->getObjectHasValue(mTestingOntologyBuilder->getInverseObjectPropertyOf(propExp), firstIndiExp));
								}
							}
							CDataPropertyAssertionExpression* dataPropAssExp = dynamic_cast<CDataPropertyAssertionExpression*>(assExp);
							if (dataPropAssExp) {
								onlyAtomicConcept = false;
								CDataPropertyTermExpression* dataPropTermExp = dataPropAssExp->getDataPropertyTermExpression();
								CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(dataPropAssExp->getIndividualTermExpression());
								CDataLiteralVariableExpression* dataVarExp = dynamic_cast<CDataLiteralVariableExpression*>(dataPropAssExp->getDataLiteralTermExpression());
								if (indiVarExp && !dataVarExp) {
									CDataLiteralExpression* dataLiteralExp = (CDataLiteralExpression*)dataPropAssExp->getDataLiteralTermExpression();
									dataLiteralExp = (CDataLiteralExpression*)mTestingOntologyBuilder->rebuildExpression(dataLiteralExp);
									instTestingExpList.append(mTestingOntologyBuilder->getDataHasValue(dataPropTermExp, dataLiteralExp));
								}
							}

							//TODO: handle data assertions and data variables
						}
						instTestingExp = mTestingOntologyBuilder->getObjectIntersectionOf(instTestingExpList);
						mTestingOntologyBuilder->completeBuilding();
						mTestingOntologyPreprocessor->preprocess(testingOnto, testingOnto->getConfiguration());

						CClassTermExpression* classTermExpOfInt = dynamic_cast<CClassTermExpression*>(instTestingExp);
						if (classTermExpOfInt) {
							COptimizedComplexConceptItem* assocConceptItem = nullptr;
							cint64 initialRequestingCount = 1;
							if (mConfMinimalMappingsComputationSize < 0 && compAssIndVarQuery->getResultLimit() < 0) {
								initialRequestingCount = -1;
							}
							queryProcessing |= initializeComplexConceptQueryProcessing(queryProcessingData, nullptr, classTermExpOfInt, false, false, false, false, false, initialRequestingCount, &assocConceptItem);

							if (assocConceptItem && mConfExtendedLogging) {
								LOG(INFO, getDomain(), logTr("Using complex concept item %2 for computing bounds for variable %1.").arg(lastVarExpression->getName()).arg(assocConceptItem->getConceptItemId()), this);
							}
						}




					} else {


						COptimizedComplexBuildingIndividualVariableCompositionsItem* varBuildItem = new COptimizedComplexBuildingIndividualVariableCompositionsItem(queryProcessingData);
						varBuildItem->setAllVariableSet(varExpSet);
						varBuildItem->setDistinguishedVariableSet(disVarList->toSet());
						varBuildItem->setReductionDeniedVariableSet(disVarList->toSet());
						for (CVariableBindingOrdering* resultOrderingLinkerIt = compAssIndVarQuery->getResultOrderingLinker(); resultOrderingLinkerIt; resultOrderingLinkerIt = resultOrderingLinkerIt->getNext()) {
							varBuildItem->getReductionDeniedVariableSet()->insert(resultOrderingLinkerIt->getOrderingVariableExpression());
						}
						if (allAnonymousVariables || distinct || mConfBindingReducingForNonDistinctAnswerQueries) {
							varBuildItem->setBindingsReducible(true);
						}
						*varBuildItem->getVariableSingleCardinalitySet() = convertedVariableSet;
						varBuildItem->getVariableSingleCardinalitySet()->unite(compAssIndVarQuery->getIgnoreCardinalityVariableExpressions().toSet());


						CComplexAssertionsIndividualVariablesAnsweringExtendibleQuery* extCompAssIndVarQuery = dynamic_cast<CComplexAssertionsIndividualVariablesAnsweringExtendibleQuery*>(compAssIndVarQuery);
						if (extCompAssIndVarQuery) {
							CComplexQueryExtendibleData* extendibleData = extCompAssIndVarQuery->getExtendibleData();
							queryProcessingData->setQueryExtendibleData(extendibleData);

							if (!extendibleData->getExtendingQueryList()->isEmpty()) {
								QList<CExpressionVariable*>* extendibleQueryTransferringExtractionVariableList = varBuildItem->getExtendibleQueryTransferringExtractionVariableList();
								for (CExpressionVariable* expVar : *extCompAssIndVarQuery->getDistinguishedVariableExpressions()) {
									extendibleQueryTransferringExtractionVariableList->append(expVar);
								}
							}
						}





						mTestingOntologyBuilder->initializeBuilding();



						// do rolling up of assertions with anonymous variables
						QHash<CExpressionVariable*, CBuildExpression*> varRolledUpClassExpHash;
						QSet<CExpressionVariable*> rolledVarExpSet;
						expressionsRollingUp(anonymousIndiVariableSet, rolledVarExpSet, varExpAxiomExpHash, testingOnto, varRolledUpClassExpHash);




						QHash<CExpressionVariable*, CBuildExpression*> rebuiltVarClassTermExp = rebuildVariablesClassTermExpressions(varExpSet, rolledVarExpSet, varExpAxiomExpHash, varRolledUpClassExpHash);
						varBuildItem->setVariableClassTermExpressionHash(rebuiltVarClassTermExp);

						queryProcessingData->incBuildingVariableItem();
						queryProcessingData->setVariableBuildingItem(varBuildItem);


						if (mConfFullQueryMaterialization) {

							mTestingOntologyBuilder->completeBuilding();
							mTestingOntologyPreprocessor->preprocess(testingOnto, testingOnto->getConfiguration());

							createQueryMaterializationData(varExpSet, rolledVarExpSet, anonymousIndiVariableSet, rebuiltVarClassTermExp, varExpAxiomExpHash, allAnonymousVariables, initialAnonymousIndiVariableSet, bothVarDataPropAssPropagationList, bothVarPropAssPropagationList, onlyDataVarDataPropAssList, varBuildItem, answererContext);


							queryProcessing = createQueryMaterializationTest(varBuildItem, answererContext);
						} else {
							queryProcessing = finishConceptAndAbsorptionItemsGeneration(varExpSet, rolledVarExpSet, anonymousIndiVariableSet, rebuiltVarClassTermExp, varExpAxiomExpHash, allAnonymousVariables, initialAnonymousIndiVariableSet, bothVarDataPropAssPropagationList, bothVarPropAssPropagationList, onlyDataVarDataPropAssList, varBuildItem, answererContext);
						}



						return varBuildItem;

					}

				}
				return nullptr;
			}






			bool COptimizedComplexExpressionAnsweringHandler::areConceptItemConceptsImpliedByPropertyDomains(CIndividualVariableExpression* varExp, COptimizedComplexConceptItem* conItem, const QList<CObjectPropertyAssertionExpression*>& propAssList, COptimizedComplexBuildingIndividualVariableCompositionsItem* varBuildItem, CAnswererContext* answererContext) {
				QSet<TConceptNegPair> testingConSet = *conItem->getAtomicConcpetSet();
				for (CObjectPropertyAssertionExpression* propAssExp : propAssList) {
					CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
					CIndividualTermExpression* firstIndiExp = propAssExp->getFirstIndividualTermExpression();
					CIndividualTermExpression* secondIndiExp = propAssExp->getSecondIndividualTermExpression();
					CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(firstIndiExp);
					CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(secondIndiExp);

					CRole* role = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(propExp);
					bool domain = true;
					if (secondVarExp == varExp) {
						domain = false;
					}

					for (QSet<TConceptNegPair>::iterator it = testingConSet.begin(), itEnd = testingConSet.end(); it != itEnd; ) {
						TConceptNegPair conNegPair = *it;
						CConcept* concept = conNegPair.first;
						bool conNeg = conNegPair.second;
						if (isConceptRoleDomainRangeImplied(concept, conNeg, role, domain, answererContext)) {
							it = testingConSet.erase(it);
						} else {
							++it;
						}
					}

				}


				return testingConSet.isEmpty();
			}



			bool COptimizedComplexExpressionAnsweringHandler::isConceptRoleDomainRangeImplied(CConcept* concept, bool negation, CRole* role, bool domain, CAnswererContext* answererContext) {
				CConcept* indiTriggerConcept = mOntoAnsweringItem->getOntology()->getTBox()->getIndividualTriggerConcept();
				CConcept* topConcept = mOntoAnsweringItem->getOntology()->getTBox()->getTopConcept();
				if ((concept->getOperatorCode() == CCSOME && !negation && !concept->getOperandList()->isNegated()) || (concept->getOperatorCode() == CCALL && negation && concept->getOperandList()->isNegated())) {
					if (concept->getRole() == role && concept->getOperandCount() == 1 && concept->getOperandList()->getData()  && (concept->getOperandList()->getData() == indiTriggerConcept || concept->getOperandList()->getData() == topConcept)) {
						return true;
					}
				}
				QSet<TConceptNegPair>* conSet = getRoleDomainRangeImpliedConceptSet(role, domain, answererContext);
				if (conSet) {
					return conSet->contains(TConceptNegPair(concept, negation));
				}
				return false;
			}


			QSet<TConceptNegPair>* COptimizedComplexExpressionAnsweringHandler::getRoleDomainRangeImpliedConceptSet(CRole* role, bool domain, CAnswererContext* answererContext) {
				QSet<TConceptNegPair>* conSet = nullptr;

				CRole* domainCheckingRole = role;
				if (!domain) {
					domainCheckingRole = domainCheckingRole->getInverseRole();
				}

				QHash<CRole*, QSet<TConceptNegPair>*>* roleDomainImpliedConceptSetHash = mOntoAnsweringItem->getRoleDomainImpliedConceptSetHash();
				if (roleDomainImpliedConceptSetHash) {
					QSet<TConceptNegPair>*& domConSet = (*roleDomainImpliedConceptSetHash)[domainCheckingRole];
					if (!domConSet) {
						domConSet = new QSet<TConceptNegPair>();

						for (CSortedNegLinker<CRole*>* superRoleIt = domainCheckingRole->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
							CRole* superRole = superRoleIt->getData();
							bool superRoleInversed = superRoleIt->isNegated();
							for (CSortedNegLinker<CConcept*>* domainConceptLinkerIt = superRole->getDomainRangeConceptList(superRoleInversed); domainConceptLinkerIt; domainConceptLinkerIt = domainConceptLinkerIt->getNext()) {
								CConcept* domCon = domainConceptLinkerIt->getData();
								bool domConNeg = domainConceptLinkerIt->isNegated();

								if (!domConSet->contains(TConceptNegPair(domCon, domConNeg))) {

									QList<TConceptNegPair> processingList;
									QSet<TConceptNegPair> processingSet;

									processingList.append(TConceptNegPair(domCon, domConNeg));
									domConSet->insert(TConceptNegPair(domCon, domConNeg));

									while (!processingList.isEmpty()) {

										TConceptNegPair proConNegPair = processingList.takeFirst();
										CConcept* testCon = proConNegPair.first;
										bool testConNeg = proConNegPair.second;

										if (!testConNeg && testCon->hasClassName()) {
											CHierarchyNode* conHierNode = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy()->getHierarchyNode(testCon, false);
											if (conHierNode) {
												for (CHierarchyNode* superHierNode : *conHierNode->getParentNodeSet()) {
													if (!domConSet->contains(TConceptNegPair(superHierNode->getOneEquivalentConcept(), false))) {
														domConSet->insert(TConceptNegPair(superHierNode->getOneEquivalentConcept(), false));
														processingList.append(TConceptNegPair(superHierNode->getOneEquivalentConcept(), false));
													}
												}
												for (CConcept* eqCon : *conHierNode->getEquivalentConceptList()) {
													if (!domConSet->contains(TConceptNegPair(eqCon, false))) {
														domConSet->insert(TConceptNegPair(eqCon, false));
														processingList.append(TConceptNegPair(eqCon, false));
													}
												}
											}
										}
										CConceptOperator* testConOp = testCon->getConceptOperator();
										if (!testConNeg && testConOp->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_TRIG_AND_AQAND_TYPE)) {
											for (CSortedNegLinker<CConcept*>* opConLinkerIt = testCon->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
												CConcept* opCon = opConLinkerIt->getData();
												bool opConNeg = opConLinkerIt->isNegated();
												if (!domConSet->contains(TConceptNegPair(opCon, opConNeg))) {
													domConSet->insert(TConceptNegPair(opCon, opConNeg));
													processingList.append(TConceptNegPair(opCon, opConNeg));
												}
											}
										}
									}
								}
							}
						}
					}
					conSet = domConSet;
				}

				return conSet;
			}


			bool COptimizedComplexExpressionAnsweringHandler::checkExistentiallyOccuringRoles(CAnswererContext* answererContext) {
				QString statExistOccurRoleNamesString;
				QString existOccurRoleNamesString;
				QString oneExistTypeOccurRoleNamesString;
				CRoleVector* roleVec = mOntoAnsweringItem->getOntology()->getRBox()->getRoleVector(false);
				cint64 ontologyId = mOntoAnsweringItem->getOntology()->getOntologyID();
				COccurrenceStatisticsCacheReader* occStatsCacheReader = answererContext->getAnsweringCalculationHandler()->getOccurrenceStatisticsCacheReader();
				CRoleRealization* roleReal = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();
				for (CRole* role : *mOntoAnsweringItem->getOntology()->getRBox()->getActivePropertyRoleSet(false)) {
					if (!role->isDataRole()) {
						COccurrenceStatisticsRoleData roleOccStatsData = occStatsCacheReader->getAccummulatedRoleDataOccurrenceStatistics(ontologyId, role->getRoleTag());
						bool oneExist = false;
						if (roleOccStatsData.getExistentialInstanceOccurrencesCount() > 0) {
							if (role->getNameLinker()) {
								QString roleName = CIRIName::getRecentIRIName(role->getNameLinker());
								statExistOccurRoleNamesString += QString("%1\r\n").arg(roleName);
								oneExist = true;
							}
						}
						if (roleReal && roleReal->hasExistentiallyLinkedRoleInstances(role, false)) {
							if (role->getNameLinker()) {
								QString roleName = CIRIName::getRecentIRIName(role->getNameLinker());
								existOccurRoleNamesString += QString("%1\r\n").arg(roleName);
								oneExist = true;
							}
						}
						if (oneExist) {
							QString roleName = CIRIName::getRecentIRIName(role->getNameLinker());
							oneExistTypeOccurRoleNamesString += QString("%1\r\n").arg(roleName);
						}
					}
				}
				mDebugOneExistTypeOccurRoleNamesString = oneExistTypeOccurRoleNamesString;
				bool debug = true;
				return debug;
			}



			bool COptimizedComplexExpressionAnsweringHandler::tryAnonymousToIndividualVariablesConversion(QSet<CExpressionVariable *> &anonymousIndiVariableSet, QSet<CExpressionVariable *> &varExpSet, bool distinct, CAnswererContext* answererContext, QHash<CExpressionVariable *, CAxiomExpression *> &varExpAxiomExpHash, bool& allAnonymousVariables, QSet<CExpressionVariable*>& convertedVariableSet) {
#ifndef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED
				checkExistentiallyOccuringRoles(answererContext);
#endif
				bool updateIndiVarAssPropLists = false;
				QSet<CExpressionVariable*> backupAnonymousIndiVariableSet(anonymousIndiVariableSet);
				cint64 originalAnonymousVariableCount = anonymousIndiVariableSet.size();
				if (mConfAnonymousToDistinguishedVariablesConversion && mConfOccurrenceStatisticsCollection && (allAnonymousVariables || distinct)) {
					// identify those anonymous variables for which the related concept and role terms only occur on individual nodes
					COccurrenceStatisticsCacheReader* occStatsCacheReader = answererContext->getAnsweringCalculationHandler()->getOccurrenceStatisticsCacheReader();
					cint64 ontologyId = mOntoAnsweringItem->getOntology()->getOntologyID();
					for (QSet<CExpressionVariable*>::iterator itAnonIndiVar = anonymousIndiVariableSet.begin(); itAnonIndiVar != anonymousIndiVariableSet.end(); ) {

						CExpressionVariable* annonyomousIndiVar(*itAnonIndiVar);
						QList<CAxiomExpression*> expressions = varExpAxiomExpHash.values(annonyomousIndiVar);
						bool convertAnonymousVarToIndiVar = false;
						for (CAxiomExpression* assExp : expressions) {
							CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
							if (propAssExp) {
								CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
								if (dynamic_cast<CObjectPropertyExpression*>(propExp)) {
									CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getFirstIndividualTermExpression());
									CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getSecondIndividualTermExpression());
									// TODO: correctly resolve inverses
									CRole* role = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value((CObjectPropertyTermExpression*)propExp);
									if (!role->isComplexRole()) {
										bool inversed = true;
										CIndividualVariableExpression* otherVarExp = firstVarExp;
										if (firstVarExp == annonyomousIndiVar) {
											otherVarExp = secondVarExp;
											inversed = false;
										}
										if (inversed) {
											role = role->getInverseRole();
										}
										COccurrenceStatisticsRoleData roleOccStatsData = occStatsCacheReader->getAccummulatedRoleDataOccurrenceStatistics(ontologyId, role->getRoleTag());
										if (roleOccStatsData.getExistentialInstanceOccurrencesCount() <= 0) {
											convertAnonymousVarToIndiVar = true;
											break;
										}
									}
								}
							}

							CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(assExp);
							if (classAssExp) {
								CClassTermExpression* classExp = classAssExp->getClassTermExpression();
								if (dynamic_cast<CClassExpression*>(classExp)) {
									CConcept* concept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value((CClassTermExpression*)classExp);
									if (concept->getOperatorCode() == CCEQ) {
										concept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getTBox()->getEquivalentConceptCandidateHash(false)->value(concept);
									}
									COccurrenceStatisticsConceptData conceptOccStatsData = occStatsCacheReader->getAccummulatedConceptDataOccurrenceStatistics(ontologyId, concept->getConceptTag());
									if (conceptOccStatsData.getExistentialInstanceOccurrencesCount() <= 0) {
										convertAnonymousVarToIndiVar = true;
										break;
									}
								}
							}

						}

						if (convertAnonymousVarToIndiVar) {
							convertedVariableSet.insert(annonyomousIndiVar);
							itAnonIndiVar = anonymousIndiVariableSet.erase(itAnonIndiVar);
						} else {
							++itAnonIndiVar;
						}

					}
				}


				CRoleRealization* roleReal = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();
				if (mConfAnonymousToDistinguishedVariablesConversion && roleReal && (allAnonymousVariables || distinct)) {
					for (QSet<CExpressionVariable*>::iterator itAnonIndiVar = anonymousIndiVariableSet.begin(); itAnonIndiVar != anonymousIndiVariableSet.end(); ) {
						bool convertToDistinguishedVariable = false;

						CExpressionVariable* annonyomousIndiVar(*itAnonIndiVar);
						QList<CAxiomExpression*> expressions = varExpAxiomExpHash.values(annonyomousIndiVar);
						for (CAxiomExpression* assExp : expressions) {
							CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
							if (propAssExp) {
								CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getFirstIndividualTermExpression());
								CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getSecondIndividualTermExpression());
								if (firstVarExp && secondVarExp) {
									bool inversed = false;
									CIndividualVariableExpression* otherVarExp = firstVarExp;
									if (firstVarExp == annonyomousIndiVar) {
										otherVarExp = secondVarExp;
										inversed = true;
									}
									if (!anonymousIndiVariableSet.contains(otherVarExp)) {
										// check whether role is linking to existential individuals
										CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
										if (dynamic_cast<CObjectPropertyExpression*>(propExp)) {
											CRole* role = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value((CObjectPropertyTermExpression*)propExp);
											if (!roleReal->hasExistentiallyLinkedRoleInstances(role, inversed)) {
												convertToDistinguishedVariable = true;
												break;
											}
										}
									}
								}
							}
						}

						if (convertToDistinguishedVariable) {
							convertedVariableSet.insert(annonyomousIndiVar);
							anonymousIndiVariableSet.erase(itAnonIndiVar);
							itAnonIndiVar = anonymousIndiVariableSet.begin();
						} else {
							++itAnonIndiVar;
						}
					}
				}

				cint64 reducedAnonymousVariableCount = anonymousIndiVariableSet.size();
				if (originalAnonymousVariableCount != reducedAnonymousVariableCount) {
					updateIndiVarAssPropLists = true;
					if (allAnonymousVariables && mConfAnonymousToDistinguishedVariablesOnlyFullConversion && reducedAnonymousVariableCount > 0) {
						anonymousIndiVariableSet = backupAnonymousIndiVariableSet;
						updateIndiVarAssPropLists = false;
						if (mConfExtendedLogging) {
							LOG(INFO, getDomain(), logTr("No conversion of anonymous variables to distinguished (non-answer) variables since it is only possible for %1.").arg(originalAnonymousVariableCount - reducedAnonymousVariableCount), this);
						}
					} else if (mConfExtendedLogging) {
						LOG(INFO, getDomain(), logTr("Interpreting %1 anonymous variables as distinguished (non-answer) variables.").arg(originalAnonymousVariableCount - reducedAnonymousVariableCount), this);
					}
				}								
				return updateIndiVarAssPropLists;
			}

			bool COptimizedComplexExpressionAnsweringHandler::finishConceptAndAbsorptionItemsGeneration(const QSet<CExpressionVariable *>& varExpSet, const QSet<CExpressionVariable *>& rolledVarExpSet, const QSet<CExpressionVariable *>& anonymousIndiVariableSet, const QHash<CExpressionVariable *, CBuildExpression *>& rebuiltVarClassTermExp, const QHash<CExpressionVariable *, CAxiomExpression *>& varExpAxiomExpHash,
					bool allAnonymousVariables, const QSet<CExpressionVariable *>& initialAnonymousIndiVariableSet, const QList<CDataPropertyAssertionExpression *>& bothVarDataPropAssPropagationList, const QList<CObjectPropertyAssertionExpression *>& bothVarPropAssPropagationList, const QList<CDataPropertyAssertionExpression *>& onlyDataVarDataPropAssList, COptimizedComplexBuildingIndividualVariableCompositionsItem* varBuildItem, CAnswererContext* answererContext) {

				bool queryProcessing = false;
				CConcreteOntology* testingOnto = mOntoAnsweringItem->getTestingOntology(true);
				CComplexQueryExpressionProcessingData* queryProcessingData = varBuildItem->getQueryProcessingData();
				CComplexAssertionsIndividualVariablesAnsweringQuery* compAssIndVarQuery = (CComplexAssertionsIndividualVariablesAnsweringQuery*)queryProcessingData->getQuery();

				// building complex concept expressions for concept items
				QSet<CExpressionVariable*> prepareIndiVarSet;
				QHash<CExpressionVariable*, CBuildExpression*> individualTriggerUpdatableHash;
				QList<QPair<CIndividualVariableExpression*, CClassTermExpression*>> varClassInstTestExpList;
				varClassInstTestExpList = generateConceptItemExpressions(varExpSet, rolledVarExpSet, anonymousIndiVariableSet, rebuiltVarClassTermExp, varExpAxiomExpHash, individualTriggerUpdatableHash, prepareIndiVarSet, queryProcessingData);

				


				// identify sub queries with non-distinguished variables
				QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*> absorptionBasedHandlingQueryPartDatas;
				absorptionBasedHandlingQueryPartDatas = generateAbsorptionBasedQueryParts(anonymousIndiVariableSet, allAnonymousVariables, *varBuildItem->getReductionDeniedVariableSet(), rolledVarExpSet, initialAnonymousIndiVariableSet, varExpSet, prepareIndiVarSet, rebuiltVarClassTermExp, varExpAxiomExpHash);



				mTestingOntologyBuilder->completeBuilding();



				QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*> entailmentCheckingAbsorptionBasedHandlingQueryPartDatas;
				for (COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* data : absorptionBasedHandlingQueryPartDatas) {

					if (!mConfLazyExistentialPartAbsorption) {
						absorbExistentialQueryPart(data, answererContext);

						if (data->hasPreparationVariables()) {
							varBuildItem->addAbsorbedBasedQueryPartItemExtensionHandling(data);
						} else {
							varBuildItem->addAbsorbedBasedQueryPartEntailmentCheckingHandling(data);
							entailmentCheckingAbsorptionBasedHandlingQueryPartDatas.append(data);
						}
					} else {
						varBuildItem->addAbsorbingQueryPart(data);
					}
				}



				QList<QPair<CIndividualVariableExpression*, TConceptNegPair>> varConNegInstTestExpList;
				varConNegInstTestExpList = upgradeExistTopToIndividualTriggers(varClassInstTestExpList, individualTriggerUpdatableHash, answererContext);


				if (mConfDebugTestingOntologyWriting) {
					COntologyTextFormater::writeOntologyToFile(mOntoAnsweringItem->getTestingOntology(), "Debugging/Answering/Absorbed-Query-Expressions.txt");
				}

				mTestingOntologyPreprocessor->preprocess(testingOnto, testingOnto->getConfiguration());

				queryProcessing |= schedulingConceptItemsProcessingForBuildItem(varConNegInstTestExpList, compAssIndVarQuery, varBuildItem);
				varBuildItem->addVariableDataPropertyAssertions(bothVarDataPropAssPropagationList);
				varBuildItem->addVariablePropertyAssertions(bothVarPropAssPropagationList);
				varBuildItem->addBasicDataPropertyAssertions(onlyDataVarDataPropAssList);


				if (mConfDebugTestingOntologyWriting) {
					COntologyTextFormater::writeOntologyToFile(mOntoAnsweringItem->getTestingOntology(), "Debugging/Answering/Absorbed-Preprocessed-Query-Expressions.txt");
				}

				for (COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* data : entailmentCheckingAbsorptionBasedHandlingQueryPartDatas) {
					createAbsorbedQueryPartEntailmentTest(data, queryProcessingData, answererContext);
				}

				if (!queryProcessing) {
					varBuildItem->setProcessingQueued(true);
					mOntoAnsweringItem->addProcessingVariableBuildingItem(varBuildItem);
					queryProcessing = true;
				}									
				return queryProcessing;
			}




			bool COptimizedComplexExpressionAnsweringHandler::schedulingConceptItemsProcessingForBuildItem(const QList<QPair<CIndividualVariableExpression *, TConceptNegPair>> &varConNegInstTestExpList, CComplexAssertionsIndividualVariablesAnsweringQuery* compAssIndVarQuery, COptimizedComplexBuildingIndividualVariableCompositionsItem* varBuildItem) {
				bool queryProcessing = false;
				for (QPair<CIndividualVariableExpression*, TConceptNegPair> varConNegPair : varConNegInstTestExpList) {
					TConceptNegPair conNegPair = varConNegPair.second;
					CIndividualVariableExpression* varTermExp = varConNegPair.first;

					CConcept* testingConcept = conNegPair.first;
					bool testingNegation = conNegPair.second;

					cint64 requiredInstancesCount = 1;
					if (varConNegInstTestExpList.size() == 1 && compAssIndVarQuery && compAssIndVarQuery->getResultLimitIncludingOffset() == -1) {
						// request all if the conjunctive query can be reduced to a complex concept instances query
						requiredInstancesCount = -1;
					}

					COptimizedComplexConceptItem* assocConceptItem = nullptr;
					queryProcessing |= initializeComplexConceptQueryProcessing(nullptr, varBuildItem, testingConcept, testingNegation, false, false, false, false, false, requiredInstancesCount, &assocConceptItem);

					if (assocConceptItem && mConfExtendedLogging) {
#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
						assocConceptItem->debugVariableNameUseList.append(varTermExp->getName());
#endif
						LOG(INFO, getDomain(), logTr("Using complex concept item %2 for computing bounds of variable %1.").arg(varTermExp->getName()).arg(assocConceptItem->getConceptItemId()), this);
					}

					varBuildItem->addVariableComplexConceptItem(varTermExp, assocConceptItem);
				}
				return queryProcessing;
			}

			QList<QPair<CIndividualVariableExpression*, TConceptNegPair>> COptimizedComplexExpressionAnsweringHandler::upgradeExistTopToIndividualTriggers(QList<QPair<CIndividualVariableExpression *, CClassTermExpression *>> varClassInstTestExpList, QHash<CExpressionVariable *, CBuildExpression *> individualTriggerUpdatableHash, CAnswererContext* answererContext) {
				QList<QPair<CIndividualVariableExpression*, TConceptNegPair>> varConNegInstTestExpList;
				// upgrade \exits r.\top concepts to use individual trigger if possible
				for (QPair<CIndividualVariableExpression*, CClassTermExpression*> varClassPair : varClassInstTestExpList) {
					CClassTermExpression* classTermExp = varClassPair.second;
					CIndividualVariableExpression* varTermExp = varClassPair.first;

					TConceptNegPair upgradedConNegPair = upgradeExistTopToIndividualTrigger(classTermExp, varTermExp, &individualTriggerUpdatableHash, answererContext);
					varConNegInstTestExpList.append(QPair<CIndividualVariableExpression*, TConceptNegPair>(varTermExp, upgradedConNegPair));

				}
				return varConNegInstTestExpList;
			}

			bool COptimizedComplexExpressionAnsweringHandler::identifyNonInstantiation(const QList<CAxiomExpression *>& assExps, const QSet<CExpressionVariable *> &answerIndiVariableSet, CAnswererContext* answererContext) {
				// identify those anonymous variables for which the related concept and role terms only occur on individual nodes
				bool nonInstantiatedTerm = false;
				COccurrenceStatisticsCacheReader* occStatsCacheReader = answererContext->getAnsweringCalculationHandler()->getOccurrenceStatisticsCacheReader();
				cint64 ontologyId = mOntoAnsweringItem->getOntology()->getOntologyID();
				for (CAxiomExpression* assExp : assExps) {
					CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
					if (propAssExp) {
						CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
						if (dynamic_cast<CObjectPropertyExpression*>(propExp)) {
							CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getFirstIndividualTermExpression());
							CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getSecondIndividualTermExpression());
							// TODO: correctly resolve inverses
							CRole* role = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value((CObjectPropertyTermExpression*)propExp);
							if (role && !role->isComplexRole()) {
								CRole* inverseRole = role->getInverseRole();
								COccurrenceStatisticsRoleData roleOccStatsData = occStatsCacheReader->getAccummulatedRoleDataOccurrenceStatistics(ontologyId, role->getRoleTag());
								if (roleOccStatsData.getExistentialInstanceOccurrencesCount() <= 0 && roleOccStatsData.getIndividualInstanceOccurrencesCount() <= 0) {
									nonInstantiatedTerm = true;
									break;
								}
								if (roleOccStatsData.getIndividualInstanceOccurrencesCount() <= 0 && firstVarExp && answerIndiVariableSet.contains(firstVarExp)) {
									nonInstantiatedTerm = true;
									break;
								}
								if (secondVarExp && answerIndiVariableSet.contains(secondVarExp)) {
									COccurrenceStatisticsRoleData roleOccStatsData = occStatsCacheReader->getAccummulatedRoleDataOccurrenceStatistics(ontologyId, inverseRole->getRoleTag());
									if (roleOccStatsData.getIndividualInstanceOccurrencesCount() <= 0) {
										nonInstantiatedTerm = true;
										break;
									}
								}
							}
						}
					}

					CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(assExp);
					if (classAssExp) {
						CClassTermExpression* classExp = classAssExp->getClassTermExpression();
						if (dynamic_cast<CClassExpression*>(classExp)) {
							CConcept* concept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value((CClassTermExpression*)classExp);
							if (concept->getOperatorCode() == CCEQ) {
								concept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getTBox()->getEquivalentConceptCandidateHash(false)->value(concept);
							}
							if (concept) {
								COccurrenceStatisticsConceptData conceptOccStatsData = occStatsCacheReader->getAccummulatedConceptDataOccurrenceStatistics(ontologyId, concept->getConceptTag());
								if (conceptOccStatsData.getExistentialInstanceOccurrencesCount() <= 0 && conceptOccStatsData.getIndividualInstanceOccurrencesCount() <= 0) {
									nonInstantiatedTerm = true;
									break;
								}
								CIndividualVariableExpression* varExp = dynamic_cast<CIndividualVariableExpression*>(classAssExp->getIndividualTermExpression());
								if (conceptOccStatsData.getIndividualInstanceOccurrencesCount() <= 0 && varExp && answerIndiVariableSet.contains(varExp)) {
									nonInstantiatedTerm = true;
									break;
								}
							}
						}
					}

				}
				return nonInstantiatedTerm;
			}


			bool COptimizedComplexExpressionAnsweringHandler::checkNonTrivialAbsorptionPropagationExpression(CBuildExpression* expression) {
				if (!expression) {
					return false;
				}
				CClassTermExpression* classTermExp = dynamic_cast<CClassTermExpression*>(expression);
				if (!classTermExp) {
					return true;
				}
				CClassExpression* classExp = dynamic_cast<CClassExpression*>(expression);
				if (classExp) {
					CConcept* concept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value((CClassTermExpression*)classExp);
					if (concept->getOperatorCode() == CCEQ) {
						return true;
					}
					return false;
				}
				CObjectIntersectionOfExpression* intOfExp = dynamic_cast<CObjectIntersectionOfExpression*>(expression);
				if (intOfExp) {
					for (CClassTermExpression* intClassTermExp : *intOfExp->getClassTermExpressionList()) {
						if (checkNonTrivialAbsorptionPropagationExpression(intClassTermExp)) {
							return true;
						}
					}
					return false;
				}
				CObjectSomeValuesFromExpression* someValueOfExp = dynamic_cast<CObjectSomeValuesFromExpression*>(expression);
				if (someValueOfExp) {
					return checkNonTrivialAbsorptionPropagationExpression(someValueOfExp->getClassTermExpression());
				}
				return true;
			}


			QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData *> COptimizedComplexExpressionAnsweringHandler::generateAbsorptionBasedQueryParts(const QSet<CExpressionVariable *> &anonymousIndiVariableSet, bool allAnonymousVariables, QSet<CExpressionVariable *> &reductionForbiddenVarSet, const QSet<CExpressionVariable *> &rolledVarExpSet,
					const QSet<CExpressionVariable *>& initialAnonymousIndiVariableSet, const QSet<CExpressionVariable *>& varExpSet, const QSet<CExpressionVariable *>& prepareIndiVarSet, const QHash<CExpressionVariable *, CBuildExpression *> &rebuiltVarClassTermExp, const QHash<CExpressionVariable *, CAxiomExpression *> &varExpAxiomExpHash) {

				QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData *> absorptionBasedHandlingQueryPartDatas;
				QSet<CExpressionVariable*> anonymVarExpSubQueryIncludedSet;
				QSet<CExpressionVariable*> absorptionAnonymousInterpretedIndiVariableSet(anonymousIndiVariableSet);
				// restricted variable set to only those anonymous variables that actually have to be absorbed
				QSet<CExpressionVariable*> restrictedAbsorptionAnonymousInterpretedIndiVariableSet(anonymousIndiVariableSet);
				if (mConfVariableAbsorptionOverFullQuery && allAnonymousVariables && !anonymousIndiVariableSet.isEmpty() && anonymousIndiVariableSet.size() != rolledVarExpSet.size()) {
					absorptionAnonymousInterpretedIndiVariableSet = initialAnonymousIndiVariableSet;
				}
				// identify sub queries with non-distinguished variables
				for (CExpressionVariable* varExp : varExpSet) {

					CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(varExp);

					if (indiVarExp && !rolledVarExpSet.contains(indiVarExp) && !anonymVarExpSubQueryIncludedSet.contains(indiVarExp) && absorptionAnonymousInterpretedIndiVariableSet.contains(varExp)) {

						QSet<CExpressionVariable*> anonymVarExpSubQueryProcessingSet;
						QList<CExpressionVariable*> anonymVarExpSubQueryProcessingList;
						anonymVarExpSubQueryIncludedSet.insert(varExp);
						anonymVarExpSubQueryProcessingSet.insert(varExp);
						anonymVarExpSubQueryProcessingList.append(varExp);

						COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionBasedQueryPartData = new COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData(absorptionAnonymousInterpretedIndiVariableSet, restrictedAbsorptionAnonymousInterpretedIndiVariableSet);

						while (!anonymVarExpSubQueryProcessingList.isEmpty()) {

							CExpressionVariable* processingVarExp = anonymVarExpSubQueryProcessingList.takeFirst();
							QList<CBuildExpression*> varClassExpList(rebuiltVarClassTermExp.values(processingVarExp));


							for (CBuildExpression* classExp: varClassExpList) {
								if (checkNonTrivialAbsorptionPropagationExpression(classExp)) {
									absorptionBasedQueryPartData->setNonTrivialAbsorptionPropagation(true);
									break;
								}
							}

							reductionForbiddenVarSet.insert(processingVarExp);
							if (prepareIndiVarSet.contains(processingVarExp)) {
								absorptionBasedQueryPartData->getPrepareVariableSet()->insert(processingVarExp);
							}

							if (absorptionAnonymousInterpretedIndiVariableSet.contains(processingVarExp)) {

								QList<CAxiomExpression*> axiomList = varExpAxiomExpHash.values(processingVarExp);
								for (CAxiomExpression* assExp : axiomList) {

									CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
									if (propAssExp) {
										CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
										CIndividualTermExpression* firstIndiExp = propAssExp->getFirstIndividualTermExpression();
										CIndividualTermExpression* secondIndiExp = propAssExp->getSecondIndividualTermExpression();
										CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(firstIndiExp);
										CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(secondIndiExp);
										if (firstVarExp == processingVarExp && secondVarExp == processingVarExp) {
											CRole* role = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value((CObjectPropertyTermExpression*)propExp);
											if (!role->isComplexRole()) {
												varClassExpList.append(mTestingOntologyBuilder->getObjectHasSelf(propExp));
											} else {
												absorptionBasedQueryPartData->addVariableSelfPropertyAssertion(propAssExp);
											}
										} else if (firstVarExp == processingVarExp) {
											if (secondVarExp == nullptr) {
												varClassExpList.append(mTestingOntologyBuilder->getObjectHasValue(propExp, secondIndiExp));
											} else if (!rolledVarExpSet.contains(secondVarExp)) {
												absorptionBasedQueryPartData->addVariableNeighbouringPropertyAssertion(propAssExp);
												if (!anonymVarExpSubQueryProcessingSet.contains(secondVarExp)) {
													anonymVarExpSubQueryProcessingSet.insert(secondVarExp);
													anonymVarExpSubQueryIncludedSet.insert(secondVarExp);
													anonymVarExpSubQueryProcessingList.append(secondVarExp);
												}
											}
										} else if (secondVarExp == processingVarExp) {
											if (firstVarExp == nullptr) {
												varClassExpList.append(mTestingOntologyBuilder->getObjectHasValue(mTestingOntologyBuilder->getInverseObjectPropertyOf(propExp), firstIndiExp));
											} else if (!rolledVarExpSet.contains(firstVarExp)) {
												absorptionBasedQueryPartData->addVariableNeighbouringPropertyAssertion(propAssExp);
												if (!anonymVarExpSubQueryProcessingSet.contains(firstVarExp)) {
													anonymVarExpSubQueryProcessingSet.insert(firstVarExp);
													anonymVarExpSubQueryIncludedSet.insert(firstVarExp);
													anonymVarExpSubQueryProcessingList.append(firstVarExp);
												}
											}
										}
									}

								}
							}
							absorptionBasedQueryPartData->addVariableClassExpressions(processingVarExp, varClassExpList);


						}

						absorptionBasedHandlingQueryPartDatas.append(absorptionBasedQueryPartData);
					}
				}
				return absorptionBasedHandlingQueryPartDatas;
			}








			QHash<CExpressionVariable*, CBuildExpression*> COptimizedComplexExpressionAnsweringHandler::rebuildVariablesClassTermExpressions(const QSet<CExpressionVariable *>& varExpSet, const QSet<CExpressionVariable *> &rolledVarExpSet, QHash<CExpressionVariable *, CAxiomExpression *> &varExpAxiomExpHash, QHash<CExpressionVariable *, CBuildExpression *> &varRolledUpClassExpHash) {

				QHash<CExpressionVariable*, CBuildExpression*> rebuildVarExpClassTermExpHash;
				for (CExpressionVariable* varExp : varExpSet) {
					if (!rolledVarExpSet.contains(varExp)) {
						QList<CBuildExpression*> instTestingExpList(varRolledUpClassExpHash.values(varExp));
						QList<CAxiomExpression*> assExplist = varExpAxiomExpHash.values(varExp);
						for (CAxiomExpression* assExp : assExplist) {
							CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(assExp);
							if (classAssExp) {
								instTestingExpList.append(mTestingOntologyBuilder->rebuildExpression(classAssExp->getClassTermExpression()));
							}

							CDataPropertyAssertionExpression* dataPropAssExp = dynamic_cast<CDataPropertyAssertionExpression*>(assExp);
							if (dataPropAssExp) {
								CDataPropertyTermExpression* dataPropTermExp = dataPropAssExp->getDataPropertyTermExpression();
								CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(dataPropAssExp->getIndividualTermExpression());
								CDataLiteralVariableExpression* dataVarExp = dynamic_cast<CDataLiteralVariableExpression*>(dataPropAssExp->getDataLiteralTermExpression());
								if (indiVarExp && !dataVarExp) {
									CDataLiteralExpression* dataLiteralExp = (CDataLiteralExpression*)dataPropAssExp->getDataLiteralTermExpression();
									dataLiteralExp = (CDataLiteralExpression*)mTestingOntologyBuilder->rebuildExpression(dataLiteralExp);
									instTestingExpList.append(mTestingOntologyBuilder->getDataHasValue(dataPropTermExp, dataLiteralExp));
								}
							}

						}

						for (CBuildExpression* exp : instTestingExpList) {
							rebuildVarExpClassTermExpHash.insertMulti(varExp, exp);
						}

					}
				}


				return rebuildVarExpClassTermExpHash;
			}






			QList<QPair<CIndividualVariableExpression *, CClassTermExpression *>> COptimizedComplexExpressionAnsweringHandler::generateConceptItemExpressions(const QSet<CExpressionVariable *>& varExpSet, const QSet<CExpressionVariable *> &rolledVarExpSet, const QSet<CExpressionVariable *> &anonymousIndiVariableSet, const QHash<CExpressionVariable *, CBuildExpression *> &varRolledUpClassExpHash,
					const QHash<CExpressionVariable *, CAxiomExpression *> &varExpAxiomExpHash, QHash<CExpressionVariable *, CBuildExpression *> &individualTriggerUpdatableHash, QSet<CExpressionVariable *> &prepareIndiVarSet, CComplexQueryExpressionProcessingData* queryProcessingData) {

				CConcreteOntology* testingOnto = mOntoAnsweringItem->getTestingOntology();
				CClassTermExpression* topClassTermExp = testingOnto->getBuildData()->getTopClassExpression();
				CObjectPropertyTermExpression* topObjPropExp = testingOnto->getBuildData()->getTopObjectPropertyExpression();
				CComplexAssertionsIndividualVariablesAnsweringQuery* compAssIndVarQuery = (CComplexAssertionsIndividualVariablesAnsweringQuery*)queryProcessingData->getQuery();


				QList<QPair<CIndividualVariableExpression *, CClassTermExpression *>> varClassInstTestExpList;
				QSet<CExpressionVariable*> conceptTermOmittedIndiVarSet;

				QSet<CExpressionVariable*> conGenVarExpSet;
				QList<CExpressionVariable*> conGenVarExpList;
				for (CExpressionVariable* varExp : *compAssIndVarQuery->getDistinguishedVariableExpressions()) {
					conGenVarExpList.append(varExp);
					conGenVarExpSet.insert(varExp);
				}
				for (CExpressionVariable* varExp : varExpSet) {
					if (!conGenVarExpSet.contains(varExp)) {
						conGenVarExpSet.insert(varExp);
						conGenVarExpList.prepend(varExp);
					}
				}

				for (CExpressionVariable* varExp : conGenVarExpList) {

					CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(varExp);
					if (indiVarExp && !rolledVarExpSet.contains(varExp) && !anonymousIndiVariableSet.contains(varExp)) {
						CClassTermExpression* instTestingExp = nullptr;
						QList<CBuildExpression*> instTestingExpList(varRolledUpClassExpHash.values(indiVarExp));


						bool conceptTermRequired = true;
						QList<CAxiomExpression*> assExplist = varExpAxiomExpHash.values(varExp);
						if (assExplist.size() == 1) {
							CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExplist.first());
							if (propAssExp) {
								CIndividualVariableExpression* otherVarExp = nullptr;
								CIndividualTermExpression* firstIndiExp = propAssExp->getFirstIndividualTermExpression();
								CIndividualTermExpression* secondIndiExp = propAssExp->getSecondIndividualTermExpression();
								CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(firstIndiExp);
								CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(secondIndiExp);
								if (firstVarExp && secondVarExp) {
									otherVarExp = firstVarExp;
									if (firstVarExp == indiVarExp) {
										otherVarExp = secondVarExp;
									}

									QList<CAxiomExpression*> otherAssExplist = varExpAxiomExpHash.values(otherVarExp);

									// currently only works if propagated correctly
									if (otherAssExplist.size() == 1 && !conceptTermOmittedIndiVarSet.contains(otherVarExp)) {
										conceptTermOmittedIndiVarSet.insert(indiVarExp);
										conceptTermRequired = false;
									}

								}

							}
						}

						if (conceptTermRequired) {
							for (CAxiomExpression* assExp : assExplist) {
								CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
								if (propAssExp) {
									CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
									if (propExp != topObjPropExp) {
										CIndividualTermExpression* firstIndiExp = propAssExp->getFirstIndividualTermExpression();
										CIndividualTermExpression* secondIndiExp = propAssExp->getSecondIndividualTermExpression();
										CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(firstIndiExp);
										CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(secondIndiExp);
										if (firstVarExp == varExp && secondVarExp == varExp) {
											instTestingExpList.append(mTestingOntologyBuilder->getObjectHasSelf(propExp));
										} else if (firstVarExp == varExp) {
											if (secondVarExp == nullptr) {
												instTestingExpList.append(mTestingOntologyBuilder->getObjectHasValue(propExp, secondIndiExp));
											} else {
												if (!rolledVarExpSet.contains(secondVarExp)) {
													CObjectSomeValuesFromExpression* someValuesFromExp = mTestingOntologyBuilder->getObjectSomeValuesFrom(propExp, topClassTermExp);
													if (!anonymousIndiVariableSet.contains(secondVarExp)) {
														individualTriggerUpdatableHash.insertMulti(varExp, someValuesFromExp);
													}
													instTestingExpList.append(someValuesFromExp);
												}
											}
										} else if (secondVarExp == varExp) {
											CObjectPropertyTermExpression* invPropExp = nullptr;
											CRole* role = testingOnto->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(propExp);
											if (!role || !role->isSymmetric()) {
												invPropExp = mTestingOntologyBuilder->getInverseObjectPropertyOf(propExp);
											} else {
												invPropExp = propExp;
											}
											if (firstVarExp == nullptr) {
												instTestingExpList.append(mTestingOntologyBuilder->getObjectHasValue(invPropExp, firstIndiExp));
											} else {
												if (!rolledVarExpSet.contains(firstVarExp)) {
													CObjectSomeValuesFromExpression* someValuesFromExp = mTestingOntologyBuilder->getObjectSomeValuesFrom(invPropExp, topClassTermExp);
													if (!anonymousIndiVariableSet.contains(firstVarExp)) {
														individualTriggerUpdatableHash.insertMulti(varExp, someValuesFromExp);
													}
													instTestingExpList.append(someValuesFromExp);
												}
											}
										}
									}
								}

								CDataPropertyAssertionExpression* dataPropAssExp = dynamic_cast<CDataPropertyAssertionExpression*>(assExp);
								if (dataPropAssExp) {
									CDataPropertyTermExpression* dataPropTermExp = dataPropAssExp->getDataPropertyTermExpression();
									CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(dataPropAssExp->getIndividualTermExpression());
									CDataLiteralVariableExpression* dataVarExp = dynamic_cast<CDataLiteralVariableExpression*>(dataPropAssExp->getDataLiteralTermExpression());
									if (indiVarExp && dataVarExp) {
										instTestingExpList.append(mTestingOntologyBuilder->getDataSomeValuesFrom(dataPropTermExp, mTestingOntologyBuilder->getTopDataRange()));
									}

								}
							}


							instTestingExp = (CClassTermExpression*)mTestingOntologyBuilder->getObjectIntersectionOf(instTestingExpList);
							if (instTestingExpList.size() > 1) {
								instTestingExp = (CClassTermExpression*)mTestingOntologyBuilder->getObjectIntersectionOf(instTestingExpList);
							} else if (instTestingExpList.isEmpty()) {
								instTestingExp = mOntoAnsweringItem->getTestingOntology()->getBuildData()->getTopClassExpression();
							} else {
								instTestingExp = (CClassTermExpression*)instTestingExpList.first();
							}
							prepareIndiVarSet.insert(indiVarExp);
							varClassInstTestExpList.append(QPair<CIndividualVariableExpression*, CClassTermExpression*>(indiVarExp, instTestingExp));
						}

					}

				}
				if (!varClassInstTestExpList.isEmpty()) {
					LOG(INFO, getDomain(), logTr("Building %1 class term expressions for computation of upper/lower bounds based on complex concepts.").arg(varClassInstTestExpList.size()), this);
				}
				return varClassInstTestExpList;
			}

			void COptimizedComplexExpressionAnsweringHandler::expressionsRollingUp(QSet<CExpressionVariable *> &anonymousIndiVariableSet, QSet<CExpressionVariable *> &rolledVarExpSet, QHash<CExpressionVariable *, CAxiomExpression *> &varExpAxiomExpHash, CConcreteOntology* testingOnto, QHash<CExpressionVariable *, CBuildExpression *> &varRolledUpClassExpHash) {
				QList<CExpressionVariable*> anonymousIndiVariableProcessingList = anonymousIndiVariableSet.toList();
				QSet<CExpressionVariable*> anonymousIndiVariableProcessingSet;
				CClassTermExpression* topClassTermExp = testingOnto->getBuildData()->getTopClassExpression();
				while (!anonymousIndiVariableProcessingList.isEmpty()) {

					CExpressionVariable* anonyIndiVarExp = anonymousIndiVariableProcessingList.takeFirst();
					anonymousIndiVariableProcessingSet.remove(anonyIndiVarExp);

					if (!rolledVarExpSet.contains(anonyIndiVarExp)) {
						cint64 propAssPropagationCount = 0;
						bool rollingUpPossible = true;
						QList<CAxiomExpression*> axiomList = varExpAxiomExpHash.values(anonyIndiVarExp);
						for (CAxiomExpression* axiomExp : axiomList) {
							CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(axiomExp);
							if (propAssExp) {
								CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getFirstIndividualTermExpression());
								CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getSecondIndividualTermExpression());
								CIndividualVariableExpression* otherVarExp = firstVarExp;
								if (anonyIndiVarExp == firstVarExp) {
									otherVarExp = secondVarExp;
								}
								if (firstVarExp && secondVarExp && firstVarExp != secondVarExp && !rolledVarExpSet.contains(otherVarExp)) {
									propAssPropagationCount++;
								}
								if (firstVarExp == secondVarExp) {
									CRole* role = testingOnto->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(propAssExp->getObjectPropertyTermExpression());
									if (role && role->isComplexRole()) {
										rollingUpPossible = false;
									}
								}
							}
						}
						if (propAssPropagationCount == 1 && rollingUpPossible) {

							rolledVarExpSet.insert(anonyIndiVarExp);
							CClassTermExpression* rollingUpExp = nullptr;
							QList<CBuildExpression*> rollingUpExpList(varRolledUpClassExpHash.values(anonyIndiVarExp));

							bool inversedRollingUp = false;
							CObjectPropertyTermExpression* rollingUpPropExpression = nullptr;
							CExpressionVariable* rollingUpVarExpression = nullptr;

							for (CAxiomExpression* assExp : axiomList) {
								CClassAssertionExpression* classAssExp = dynamic_cast<CClassAssertionExpression*>(assExp);
								if (classAssExp) {
									rollingUpExpList.append(mTestingOntologyBuilder->rebuildExpression(classAssExp->getClassTermExpression()));
								}
								CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
								if (propAssExp) {
									CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
									CIndividualTermExpression* firstIndiExp = propAssExp->getFirstIndividualTermExpression();
									CIndividualTermExpression* secondIndiExp = propAssExp->getSecondIndividualTermExpression();
									CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(firstIndiExp);
									CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(secondIndiExp);
									if (firstVarExp == anonyIndiVarExp && secondVarExp == anonyIndiVarExp) {
										rollingUpExpList.append(mTestingOntologyBuilder->getObjectHasSelf(propExp));
									} else if (firstVarExp == anonyIndiVarExp) {
										if (secondVarExp == nullptr) {
											rollingUpExpList.append(mTestingOntologyBuilder->getObjectHasValue(propExp, secondIndiExp));
										} else if (!rolledVarExpSet.contains(secondVarExp)) {
											rollingUpPropExpression = propExp;
											inversedRollingUp = true;
											rollingUpVarExpression = secondVarExp;
										}
									} else if (secondVarExp == anonyIndiVarExp) {
										if (firstVarExp == nullptr) {
											rollingUpExpList.append(mTestingOntologyBuilder->getObjectHasValue(mTestingOntologyBuilder->getInverseObjectPropertyOf(propExp), firstIndiExp));
										} else if (!rolledVarExpSet.contains(firstVarExp)) {
											rollingUpPropExpression = propExp;
											inversedRollingUp = false;
											rollingUpVarExpression = firstVarExp;
										}
									}
								}

								CDataPropertyAssertionExpression* dataPropAssExp = dynamic_cast<CDataPropertyAssertionExpression*>(assExp);
								if (dataPropAssExp) {
									CDataPropertyTermExpression* dataPropTermExp = dataPropAssExp->getDataPropertyTermExpression();
									CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(dataPropAssExp->getIndividualTermExpression());
									CDataLiteralVariableExpression* dataVarExp = dynamic_cast<CDataLiteralVariableExpression*>(dataPropAssExp->getDataLiteralTermExpression());
									if (indiVarExp && !dataVarExp) {
										CDataLiteralExpression* dataLiteralExp = (CDataLiteralExpression*)dataPropAssExp->getDataLiteralTermExpression();
										dataLiteralExp = (CDataLiteralExpression*)mTestingOntologyBuilder->rebuildExpression(dataLiteralExp);
										rollingUpExpList.append(mTestingOntologyBuilder->getDataHasValue(dataPropTermExp, dataLiteralExp));
									}

								}
							}
							if (rollingUpExpList.size() > 1) {
								rollingUpExp = (CClassTermExpression*)mTestingOntologyBuilder->getObjectIntersectionOf(rollingUpExpList);
							} else if (rollingUpExpList.size() > 0) {
								rollingUpExp = (CClassTermExpression*)rollingUpExpList.first();
							} else {
								rollingUpExp = topClassTermExp;
							}

							CClassTermExpression* rolledUpExp = nullptr;
							if (inversedRollingUp) {
								rolledUpExp = mTestingOntologyBuilder->getObjectSomeValuesFrom(mTestingOntologyBuilder->getInverseObjectPropertyOf(rollingUpPropExpression), rollingUpExp);
							} else {
								rolledUpExp = mTestingOntologyBuilder->getObjectSomeValuesFrom(rollingUpPropExpression, rollingUpExp);
							}
							varRolledUpClassExpHash.insertMulti(rollingUpVarExpression, rolledUpExp);
							if (anonymousIndiVariableSet.contains(rollingUpVarExpression) && !anonymousIndiVariableProcessingSet.remove(anonyIndiVarExp)) {
								anonymousIndiVariableProcessingSet.insert(anonyIndiVarExp);
								anonymousIndiVariableProcessingList.append(rollingUpVarExpression);
							}
						}
					}

				}
				if (mConfExtendedLogging && !rolledVarExpSet.isEmpty()) {
					QStringList varList;
					for (CExpressionVariable* var : rolledVarExpSet) {
						varList.append(var->getName());
					}
					LOG(INFO, getDomain(), logTr("Rolled-up %1 variables: %2.").arg(rolledVarExpSet.size()).arg(varList.join(", ")), this);
				}
			}

			TConceptNegPair COptimizedComplexExpressionAnsweringHandler::upgradeExistTopToIndividualTrigger(CClassTermExpression* classTermExp, CExpressionVariable* varTermExp, QHash<CExpressionVariable*, CBuildExpression*>* individualTriggerUpdatableHash, CAnswererContext* answererContext) {


				QSet<TConceptNegPair> upgradableConNegSet;
				for (CBuildExpression* upgradableExp : individualTriggerUpdatableHash->values(varTermExp)) {
					CConcept* testingConcept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value((CClassTermExpression*)upgradableExp);
					bool testingNegation = testingConcept->hasMappingNegation();
					upgradableConNegSet.insert(TConceptNegPair(testingConcept, testingNegation));
				}

				CConcept* indiTriggerConcept = mOntoAnsweringItem->getOntology()->getTBox()->getIndividualTriggerConcept();
				CConcept* topConcept = mOntoAnsweringItem->getOntology()->getTBox()->getTopConcept();

				CConcept* concept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value((CClassTermExpression*)classTermExp);
				bool conceptNegation = concept->hasMappingNegation();

				QList<TConceptNegPair> upgradeOpConList;
				QList<TConceptNegPair> remainingOpConList;
				QSet<TConceptNegPair> allUpgradedOpConSet;



				QList<TConceptNegPair> testingConList;
				testingConList.append(TConceptNegPair(concept, conceptNegation));
				while (!testingConList.isEmpty()) {
					TConceptNegPair testingConNegPair = testingConList.takeFirst();
					CConcept* testingConcept = testingConNegPair.first;
					bool testingNegation = testingConNegPair.second;
					cint64 testingConOpCode = testingConcept->getOperatorCode();
					bool upgradable = false;
					bool remaining = true;

					if (!testingNegation && testingConOpCode == CCAND || testingNegation && testingConOpCode == CCOR) {
						for (CSortedNegLinker<CConcept*>* opConLinkerIt = testingConcept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opConNegation = opConLinkerIt->isNegated() ^ testingNegation;
							testingConList.append(TConceptNegPair(opConcept, opConNegation));
						}
						remaining = false;
					} else if (!testingNegation && testingConOpCode == CCSOME) {
						if (testingConcept->getOperandCount() == 1) {
							if (testingConcept->getOperandList()->getData() == topConcept && !testingConcept->getOperandList()->isNegated()) {
								if (upgradableConNegSet.contains(TConceptNegPair(testingConcept, testingNegation))) {
									upgradable = true;
								}
							}
						}
					} else if (testingNegation && testingConOpCode == CCALL) {
						if (testingConcept->getOperandCount() == 1) {
							if (testingConcept->getOperandList()->getData() == topConcept && testingConcept->getOperandList()->isNegated()) {
								if (upgradableConNegSet.contains(TConceptNegPair(testingConcept, testingNegation))) {
									upgradable = true;
								}
							}
						}
					}
					if (upgradable) {
						upgradeOpConList.append(TConceptNegPair(testingConcept, testingNegation));
					} else if (remaining) {
						remainingOpConList.append(TConceptNegPair(testingConcept, testingNegation));
						bool originalNegation = testingNegation ^ testingConcept->hasMappingNegation();
						allUpgradedOpConSet.insert(TConceptNegPair(testingConcept, originalNegation));
					}

				}


				QHash<TConceptNegPair, TConceptNegPair>* individualTriggerSingleConceptUpgradeHash = mOntoAnsweringItem->getIndividualTriggerSingleConceptUpgradeHash();
				QHash< QSet<TConceptNegPair>, TConceptNegPair>* individualTriggerMultiConceptUpgradeHash = mOntoAnsweringItem->getIndividualTriggerMultiConceptUpgradeHash();


				if (upgradeOpConList.isEmpty()) {
					return TConceptNegPair(concept, conceptNegation);
				} else {

					CMemoryAllocationManager* memMan = mOntoAnsweringItem->getTestingOntology(false)->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();
					CTBox* tBox = mOntoAnsweringItem->getTestingOntology(false)->getDataBoxes()->getTBox();
					CConceptVector* conceptVec = tBox->getConceptVector();

					QList<TConceptNegPair> upgradedOpConList;
					bool allExistingUpgrade = true;
					for (TConceptNegPair conNegPar : upgradeOpConList) {
						CConcept* opConcept = conNegPar.first;
						bool opConceptNegation = conNegPar.second;
						bool originalOpConNegation = opConceptNegation ^ opConcept->hasMappingNegation();
						TConceptNegPair opConOriginalNegPair(opConcept, originalOpConNegation);

						if (!individualTriggerSingleConceptUpgradeHash->contains(opConOriginalNegPair)) {
							CConcept* upgradedOpConcept = CObjectAllocator< CConcept >::allocateAndConstruct(memMan);
							upgradedOpConcept->initConcept();
							cint64 nextConTag = tBox->getNextConceptID();
							upgradedOpConcept->initTag(nextConTag);
							upgradedOpConcept->setOperatorCode(opConcept->getOperatorCode());
							upgradedOpConcept->setRole(opConcept->getRole());
							conceptVec->setLocalData(nextConTag, upgradedOpConcept);

							CSortedNegLinker<CConcept*>* newIndiTrigOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(memMan);
							newIndiTrigOpCon->init(indiTriggerConcept, opConcept->getOperandList()->isNegated(), upgradedOpConcept->getOperandList());
							upgradedOpConcept->setOperandList(newIndiTrigOpCon);
							upgradedOpConcept->incOperandCount();
							TConceptNegPair upgradedOpConNegPair(upgradedOpConcept, opConceptNegation);
							individualTriggerSingleConceptUpgradeHash->insert(opConOriginalNegPair, upgradedOpConNegPair);
							upgradedOpConList.append(upgradedOpConNegPair);
							allUpgradedOpConSet.insert(opConOriginalNegPair);
							allExistingUpgrade = false;
						} else {
							TConceptNegPair upgradedConNegPar = individualTriggerSingleConceptUpgradeHash->value(opConOriginalNegPair);

							CConcept* upgradedOpConcept = upgradedConNegPar.first;
							bool correctedConceptNegation = upgradedConNegPar.second ^ upgradedOpConcept->hasMappingNegation();

							allUpgradedOpConSet.insert(opConOriginalNegPair);
							upgradedOpConList.append(TConceptNegPair(upgradedOpConcept, correctedConceptNegation));
						}
					}

					if (allExistingUpgrade && individualTriggerMultiConceptUpgradeHash->contains(allUpgradedOpConSet)) {
						return individualTriggerMultiConceptUpgradeHash->value(allUpgradedOpConSet);
					} else {

						if (allUpgradedOpConSet.size() == 1) {
							TConceptNegPair allUpgradedConNegPair(*upgradedOpConList.begin());
							individualTriggerMultiConceptUpgradeHash->insert(allUpgradedOpConSet, allUpgradedConNegPair);
							return allUpgradedConNegPair;
						} else {

							CConcept* upgradedAndConcept = CObjectAllocator< CConcept >::allocateAndConstruct(memMan);
							upgradedAndConcept->initConcept();
							cint64 nextConTag = tBox->getNextConceptID();
							upgradedAndConcept->initTag(nextConTag);
							upgradedAndConcept->setOperatorCode(concept->getOperatorCode());
							upgradedAndConcept->setMappingNegation(conceptNegation);
							conceptVec->setLocalData(nextConTag, upgradedAndConcept);

							for (TConceptNegPair conNegPar : remainingOpConList) {
								CConcept* opConcept = conNegPar.first;
								bool opConceptNegation = conNegPar.second;

								CSortedNegLinker<CConcept*>* newBaseOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(memMan);
								newBaseOpCon->init(opConcept, opConceptNegation, upgradedAndConcept->getOperandList());
								upgradedAndConcept->setOperandList(newBaseOpCon);
								upgradedAndConcept->incOperandCount();
							}


							for (TConceptNegPair upgradedAndConNegPar : upgradedOpConList) {
								CConcept* upgradedOpConcept = upgradedAndConNegPar.first;
								bool opConceptNegation = upgradedAndConNegPar.second;

								CSortedNegLinker<CConcept*>* newBaseOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(memMan);
								newBaseOpCon->init(upgradedOpConcept, opConceptNegation, upgradedAndConcept->getOperandList());
								upgradedAndConcept->setOperandList(newBaseOpCon);
								upgradedAndConcept->incOperandCount();
							}
							TConceptNegPair allUpgradedConNegPair(upgradedAndConcept, conceptNegation);
							individualTriggerMultiConceptUpgradeHash->insert(allUpgradedOpConSet, allUpgradedConNegPair);
							return allUpgradedConNegPair;
						}
					}


				}
			}








			bool COptimizedComplexExpressionAnsweringHandler::ensureRoleTargetsRealization(CRole* role, bool inversed, const QSet<CIndividualReference>& individualSourceSet, COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext) {
				QList<COntologyProcessingRequirement*> reqList;
				QList<CIndividualReference> reqIndiList;
				bool targetsRealizationMissing = false;
				for (CIndividualReference individualSource : individualSourceSet) {
					CRoleTargetsRealizationHasher enHasher(role, individualSource, inversed);
					CEntailmentVariableBuildingItemData*& varBuildData = mRoleTargetsProcDataHash[enHasher];
					if (!varBuildData) {
						varBuildData = new CEntailmentVariableBuildingItemData();
					}
					if (!varBuildData->mHasResult) {
						targetsRealizationMissing = true;
						if (!varBuildData->mHasRequested) {
							COntologyProcessingRoleRealizationRequirement* roleRealReq = new COntologyProcessingRoleRealizationRequirement(role, inversed, individualSource);
							reqList.append(roleRealReq);
							reqIndiList.append(individualSource);
						}
						CXLinker<COptimizedComplexBuildingIndividualVariableCompositionsItem*>* procDataLinker = mOntoAnsweringItem->createBuildingVariableItemProcessingLinker();
						procDataLinker->setData(buildingVarItem);
						varBuildData->mProcDataLinker = procDataLinker->append(varBuildData->mProcDataLinker);
						buildingVarItem->incWaitingRoleTargetsRealizationCount();
					}
				}
				if (!reqList.isEmpty()) {
					CAnsweringMessageDataRequirementCompletedRoleTargetsRealization* reqCompMess = new CAnsweringMessageDataRequirementCompletedRoleTargetsRealization(role, inversed, reqIndiList);
					processRequirements(answererContext, mOntoAnsweringItem->getOntology(), reqList, reqCompMess);
				}
				return targetsRealizationMissing;
			}





			bool COptimizedComplexExpressionAnsweringHandler::checkEntailmentRoleAssertion(CRole* role, CIndividual* individualSource, CIndividual* individualDest, CComplexQueryExpressionProcessingData* procData, CAnswererContext* answererContext) {
				CEntailmentRoleAssertionHasher enHasher(role, individualSource, individualDest);
				CEntailmentQueryProcessingData*& queryProcData = mRoleAssEntProcDataHash[enHasher];
				if (!queryProcData) {
					queryProcData = new CEntailmentQueryProcessingData();
				}
				if (queryProcData->mHasResult) {
					if (!queryProcData->mEntailed) {
						procData->setUnsatisfiable(true);
						return false;
					} else {
						return false;
					}
				} else {
					procData->incEntailmentComputation();
					if (!queryProcData->mHasRequested) {
						CAnsweringMessageDataRequirementCompletedRoleAssertionEntailment* reqCompMess = new CAnsweringMessageDataRequirementCompletedRoleAssertionEntailment(queryProcData, role,individualSource,individualDest);
						QList<COntologyProcessingRequirement*> reqList;
						COntologyProcessingRoleRealizationRequirement* roleRealReq = new COntologyProcessingRoleRealizationRequirement(role,false,individualSource,individualDest);
						reqList.append(roleRealReq);
						processRequirements(answererContext, mOntoAnsweringItem->getOntology(), reqList, reqCompMess);
					}
					CXLinker<CComplexQueryExpressionProcessingData*>* procDataLinker = mOntoAnsweringItem->createQueryProcessingLinker();
					procDataLinker->setData(procData);
					queryProcData->mProcDataLinker = procDataLinker->append(queryProcData->mProcDataLinker);
					return true;
				}
			}




			bool COptimizedComplexExpressionAnsweringHandler::checkEntailmentRoleSubObjectPropertyAxiom(CRole* subRole, CRole* superRole, CComplexQueryExpressionProcessingData* procData, CAnswererContext* answererContext) {

				if (subRole->hasPropertyName() && superRole->hasPropertyName()) {
					CRolePropertiesHierarchyNode* subConHierNode = mOntoAnsweringItem->getOntology()->getClassification()->getObjectPropertyRoleClassification()->getRolePropertiesHierarchy()->getHierarchyNode(subRole, false);
					CRolePropertiesHierarchyNode* superConHierNode = mOntoAnsweringItem->getOntology()->getClassification()->getObjectPropertyRoleClassification()->getRolePropertiesHierarchy()->getHierarchyNode(superRole, false);

					if (!subConHierNode->hasPredecessorNode(superConHierNode)) {
						procData->setUnsatisfiable(true);
					}
				}
				return false;
			}




			bool COptimizedComplexExpressionAnsweringHandler::checkEntailmentRoleSubDataPropertyAxiom(CRole* subRole, CRole* superRole, CComplexQueryExpressionProcessingData* procData, CAnswererContext* answererContext) {

				if (subRole->hasPropertyName() && superRole->hasPropertyName()) {
					CRolePropertiesHierarchyNode* subConHierNode = mOntoAnsweringItem->getOntology()->getClassification()->getDataPropertyRoleClassification()->getRolePropertiesHierarchy()->getHierarchyNode(subRole, false);
					CRolePropertiesHierarchyNode* superConHierNode = mOntoAnsweringItem->getOntology()->getClassification()->getDataPropertyRoleClassification()->getRolePropertiesHierarchy()->getHierarchyNode(superRole, false);

					if (!subConHierNode->hasPredecessorNode(superConHierNode)) {
						procData->setUnsatisfiable(true);
					}
				}
				return false;
			}



			bool COptimizedComplexExpressionAnsweringHandler::checkEntailmentConceptSubClassAxiom(CConcept* subConcept, bool subConNegation, CConcept* superConcept, bool superConNegation, CComplexQueryExpressionProcessingData* procData, CAnswererContext* answererContext) {

				if (subConcept->hasClassName() && !subConNegation && superConcept->hasClassName() && !superConNegation) {
					CHierarchyNode* subConHierNode = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy()->getHierarchyNode(subConcept, false);
					CHierarchyNode* superConHierNode = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy()->getHierarchyNode(superConcept, false);

					if (!subConHierNode->hasPredecessorNode(superConHierNode)) {
						procData->setUnsatisfiable(true);
					}
					return false;
				}

				CEntailmentSubConceptAxiomHasher enHasher(subConcept, subConNegation, superConcept, superConNegation);
				CEntailmentQueryProcessingData*& queryProcData = mSubConEntProcDataHash[enHasher];
				if (!queryProcData) {
					queryProcData = new CEntailmentQueryProcessingData();
				}
				if (queryProcData->mHasResult) {
					if (!queryProcData->mEntailed) {
						procData->setUnsatisfiable(true);
						return false;
					} else {
						return false;
					}
				} else {
					procData->incEntailmentComputation();

					CSatisfiableCalculationJob* satCalcJob = nullptr;
					CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());
					bool topObjectRoleUsed = false;
					if (!mOntoAnsweringItem->getTestingOntology()->getABox()->hasUniversalConnectionIndividual()) {
						topObjectRoleUsed |= checkHasNewTopRolePropagation(subConcept, !subConNegation);
						topObjectRoleUsed |= checkHasNewTopRolePropagation(superConcept, superConNegation);
					}
					satCalcJob = extendProcessingByTopPropagation(satCalcJobGen, satCalcJob, topObjectRoleUsed, answererContext);
					satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(subConcept, !subConNegation, superConcept, superConNegation, nullptr, satCalcJob);
					CAnsweringMessageDataCalculationCompletedClassSubsumption* completedMessage = new CAnsweringMessageDataCalculationCompletedClassSubsumption(satCalcJob, queryProcData, subConcept, subConNegation, superConcept, superConNegation);
					mOntoAnsweringItem->getAnsweringHandlingStatistics()->incComplexConceptSubClassAxiomEntailmentTestingCount();
					processCalculationJob(answererContext, satCalcJob, completedMessage);
					return true;
				}
			}


			bool COptimizedComplexExpressionAnsweringHandler::checkEntailmentConceptAssertion(CConcept* concept, bool negation, CIndividual* individual, CComplexQueryExpressionProcessingData* procData, CAnswererContext* answererContext) {
				CEntailmentConceptAssertionHasher enHasher(concept, negation, individual);
				CEntailmentQueryProcessingData*& queryProcData = mConAssEntProcDataHash[enHasher];
				if (!queryProcData) {
					queryProcData = new CEntailmentQueryProcessingData();
				}
				if (queryProcData->mHasResult) {
					if (!queryProcData->mEntailed) {
						procData->setUnsatisfiable(true);
						return false;
					} else {
						return false;
					}
				} else {
					procData->incEntailmentComputation();
					if (concept->hasClassName() && !negation) {
						if (!queryProcData->mHasRequested) {
							CAnsweringMessageDataRequirementCompletedConceptAssertionEntailment* reqCompMess = new CAnsweringMessageDataRequirementCompletedConceptAssertionEntailment(queryProcData, concept, individual);
							QList<COntologyProcessingRequirement*> reqList;
							COntologyProcessingConceptRealizationRequirement* conRealReq = new COntologyProcessingConceptRealizationRequirement(individual, concept);
							reqList.append(conRealReq);
							processRequirements(answererContext, mOntoAnsweringItem->getOntology(), reqList, reqCompMess);
						} 
						CXLinker<CComplexQueryExpressionProcessingData*>* procDataLinker = mOntoAnsweringItem->createQueryProcessingLinker();
						procDataLinker->setData(procData);
						queryProcData->mProcDataLinker = procDataLinker->append(queryProcData->mProcDataLinker);
						return true;
					} else {
						CSatisfiableCalculationJob* satCalcJob = nullptr;
						CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());
						bool topObjectRoleUsed = false;
						if (!mOntoAnsweringItem->getTestingOntology()->getABox()->hasUniversalConnectionIndividual()) {
							topObjectRoleUsed = checkHasNewTopRolePropagation(concept, !negation);
						}
						satCalcJob = extendProcessingByTopPropagation(satCalcJobGen, satCalcJob, topObjectRoleUsed, answererContext);
						satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(concept, !negation, individual, satCalcJob);
						CAnsweringMessageDataCalculationCompletedInstanceEntailment* completedMessage = new CAnsweringMessageDataCalculationCompletedInstanceEntailment(satCalcJob, queryProcData, individual, concept, negation);
						mOntoAnsweringItem->getAnsweringHandlingStatistics()->incComplexConceptAssertionEntailmentTestingCount();
						processCalculationJob(answererContext, satCalcJob, completedMessage);
						return true;
					}
				}
			}





			bool COptimizedComplexExpressionAnsweringHandler::queueConceptItemProcessing(COptimizedComplexConceptItem* conceptItem, CComplexConceptStepComputationProcess* compStep) {
				if (!conceptItem->hasQueuedProcessStep() && !compStep->isComputationProcessQueued() && !compStep->isComputationProcessProcessing()) {
					conceptItem->setQueuedProcessStep(compStep);
					compStep->setComputationProcessQueued(true);
					COptimizedComplexConceptStepAnsweringItem* processingStep = mOntoAnsweringItem->getConceptProcessingStepItem(compStep->getComputationType());
					processingStep->addQueuedConceptItem(conceptItem);
					return true;
				}
				return false;
			}


			bool COptimizedComplexExpressionAnsweringHandler::requeueConceptItemNextProcessing(COptimizedComplexConceptItem* conceptItem, CComplexConceptStepComputationProcess* compStep) {
				conceptItem->setQueuedProcessStep(nullptr);
				CComplexConceptStepComputationProcess* nextCompStep = conceptItem->getComputationProcess()->getNextConceptStepComputationProcess(compStep, true);
				if (nextCompStep && !conceptItem->hasQueuedProcessStep() && !nextCompStep->isComputationProcessQueued() && !nextCompStep->isComputationProcessProcessing()) {
					conceptItem->setQueuedProcessStep(nextCompStep);
					nextCompStep->setComputationProcessQueued(true);
					COptimizedComplexConceptStepAnsweringItem* processingStep = mOntoAnsweringItem->getConceptProcessingStepItem(nextCompStep->getComputationType());
					processingStep->addQueuedConceptItem(conceptItem);
					return true;
				}
				return false;
			}

















			bool COptimizedComplexExpressionAnsweringHandler::failQueryProcessing(CComplexQueryExpressionProcessingData* queryProcessingData, CQueryError* queryError) {
				CQuery* query = queryProcessingData->getQuery();
				QString errorString = queryError->getErrorString();
				query->addQueryError(queryError);

				queryProcessingData->setProcessingFinished();
				cint64 processingTime = queryProcessingData->getTotalProcessingTime();
				LOG(WARN, getDomain(), logTr("Complex query processing failed after %1 ms due to error '%2'.").arg(processingTime).arg(errorString), this);

				CCallbackData* callback = queryProcessingData->getCallback();
				if (callback) {
					callback->doCallback();
				}
				delete queryProcessingData;
				mOntoAnsweringItem->decCurrentlyAnsweringQueryCount();
				return true;
			}

			


			bool COptimizedComplexExpressionAnsweringHandler::finishQueryProcessingAsUnsatisfaible(CComplexQueryExpressionProcessingData* queryProcessingData) {
				CComplexAnsweringExpressionQuery* query = queryProcessingData->getQuery();
				COptimizedComplexBuildingVariableCompositionsItem* varBuildItem = queryProcessingData->getVariableBuildingItem();

				queryProcessingData->setProcessingFinished();
				cint64 processingTime = queryProcessingData->getTotalProcessingTime();
				LOG(INFO, getDomain(), logTr("Finished unsatisfiable query processing in %1 ms.").arg(processingTime), this);

				CComplexAssertionsIndividualVariablesAnsweringQuery* compAssIndVarQuery = dynamic_cast<CComplexAssertionsIndividualVariablesAnsweringQuery*>(query);
				if (compAssIndVarQuery) {
					if (compAssIndVarQuery->isBooleanEntailmentResultRequired()) {
						CBooleanQueryResult* entailedResult = new CBooleanQueryResult(false);
						query->setQueryResult(entailedResult);
					} else {
						QStringList varList;
						QHash<CExpressionVariable*, cint64> disVarIdHash;
						cint64 nextId = 0;
						for (CExpressionVariable* varExp : *compAssIndVarQuery->getDistinguishedVariableExpressions()) {
							disVarIdHash.insert(varExp, nextId++);
							varList.append(varExp->getName());
						}
						CVariableBindingsAnswersResult* bindsAnswersResult = nullptr;
						CVariableBindingsAnswersStreamingResult* existBindsAnswersStreamingResult = dynamic_cast<CVariableBindingsAnswersStreamingResult*>(query->getQueryResult());
						if (existBindsAnswersStreamingResult) {
							existBindsAnswersStreamingResult->initResult(varList);
							bindsAnswersResult = existBindsAnswersStreamingResult;
						} else {
							CVariableBindingsAnswersResult* bindsAnswersResult = new CVariableBindingsAnswersListResult(varList);
							query->setQueryResult(bindsAnswersResult);
						}
					}
				}


				CComplexConceptAnsweringQuery* compConQuery = dynamic_cast<CComplexConceptAnsweringQuery*>(query);
				if (compConQuery) {
					bool flattened = compConQuery->isFlattened();
					if (compConQuery->isSatisfiableComputationRequired()) {
						CBooleanQueryResult* booleanQueryResult = new CBooleanQueryResult(false);
						query->setQueryResult(booleanQueryResult);
					}
					if (compConQuery->isSuperClassNodesComputationRequired() || compConQuery->isSubClassNodesComputationRequired()) {
						CClassSynsetsResult* classSynsetsResult = new CClassSynsetsResult();
						if (compConQuery->isSuperClassNodesComputationRequired()) {
							CHierarchyNode* nextNode = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy()->getTopHierarchyNode();
							CClassSynsetResult* nextClassSynset = new CClassSynsetResult(nextNode->getEquivalentConceptStringList(false));
							classSynsetsResult->addClassSynset(nextClassSynset);
						}
						query->setQueryResult(classSynsetsResult);
					}
					if (compConQuery->isEquivalentClassNodesComputationRequired()) {
						CClassSetResult* classesResult = new CClassSetResult();
						query->setQueryResult(classesResult);
					}
					if (compConQuery->isInstancesComputationRequired()) {
						if (flattened) {
							CIndividualsResult* individualResult = new CIndividualsResult();
							query->setQueryResult(individualResult);
						} else {
							CIndividualSynsetsResult* indiSynsResult = new CIndividualSynsetsResult();
							query->setQueryResult(indiSynsResult);
						}
					}
				}

				if (varBuildItem) {
					delete varBuildItem;
				}


				CCallbackData* callback = queryProcessingData->getCallback();
				if (callback) {
					callback->doCallback();
				}
				delete queryProcessingData;
				mOntoAnsweringItem->decCurrentlyAnsweringQueryCount();
				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incTotalAnsweredQueriesCount();

				cleanUpAnswersCache(true);
				return true;
			}



			

			bool COptimizedComplexExpressionAnsweringHandler::finishQueryProcessing(CComplexQueryExpressionProcessingData* queryProcessingData) {
				CComplexAnsweringExpressionQuery* query = queryProcessingData->getQuery();
				COptimizedComplexConceptItem* conceptItem = queryProcessingData->getConceptItem();
				COptimizedComplexBuildingVariableCompositionsItem* varBuildItem = queryProcessingData->getVariableBuildingItem();

				if (!queryProcessingData->isProcessingFinished()) {
					queryProcessingData->setProcessingFinished();
					cint64 firstProcessingTime = queryProcessingData->getTotalProcessingTime();
					LOG(INFO, getDomain(), logTr("Computed first complex query result in %1 ms.").arg(firstProcessingTime), this);
				}



				CComplexVariablesAnsweringQuery* compVarQuery = dynamic_cast<CComplexVariablesAnsweringQuery*>(query);
				if (compVarQuery) {

					if (compVarQuery->isBooleanEntailmentResultRequired()) {

						CBooleanQueryResult* entailedResult = new CBooleanQueryResult(false);
						if (!queryProcessingData->isUnsatisfiable()) {
							if (varBuildItem && varBuildItem->isSatisfiable()) {

								CExpressionVariable* lastVarExp = varBuildItem->getLastHandledVariableExpression();
								if (!lastVarExp) {
									entailedResult->setResult(true);
								} else {
									COptimizedComplexVariableCompositionItem* varItem = varBuildItem->getVariableLastCompositionItem(lastVarExp);
									if (varItem->getVariableMapping()->getBindingCount() > 0) {
										entailedResult->setResult(true);
									}
								}

							}
						}
						query->setQueryResult(entailedResult);

					} else {



						if ((mConfDistinctUnecessaryVariablesReduction || mConfAlwaysUnecessaryVariablesReduction) && !queryProcessingData->isUnsatisfiable()) {
							CComplexAssertionsIndividualVariablesAnsweringQuery* compAssIndVarQuery = dynamic_cast<CComplexAssertionsIndividualVariablesAnsweringQuery*>(compVarQuery);
							if (compAssIndVarQuery && varBuildItem && varBuildItem->isSatisfiable()) {
								CExpressionVariable* lastVarExp = varBuildItem->getLastHandledVariableExpression();
								if (lastVarExp) {
									COptimizedComplexVariableCompositionItem* lastVarItem = varBuildItem->getVariableLastCompositionItem(lastVarExp);
									COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping = varBuildItem->getVariableItemIndexMapping(lastVarExp).value(lastVarItem);
									QHash<CExpressionVariable*, cint64> varIdxHash;

									bool distinct = compAssIndVarQuery->isDistinctRequired();
									if ((distinct || mConfAlwaysUnecessaryVariablesReduction) && !compAssIndVarQuery->getResultOrderingLinker() && !compAssIndVarQuery->getResultFilteringLinker()) {
										QList<cint64> mNonAnswerVariableMappingIndexList;
										QSet<CExpressionVariable*> distinguishedAnswerVariableSet(compAssIndVarQuery->getDistinguishedVariableExpressions()->toSet());
										QHash<CExpressionVariable*, cint64> reductionVarExpIndHash;
										for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemIndexMapping->constBegin(), itEnd = itemIndexMapping->constEnd(); it != itEnd; ++it) {
											cint64 varIdx = it.key();
											CExpressionVariable* varExp = it.value();
											if (distinguishedAnswerVariableSet.contains(varExp)) {
												varIdxHash.insert(varExp, varIdx);
											} else {
												mNonAnswerVariableMappingIndexList.append(varIdx);
												reductionVarExpIndHash.insert(varExp, varIdx);
											}
										}

										if (!mNonAnswerVariableMappingIndexList.isEmpty()) {
											bool processing = false;
											buildVariableReductionItem(varBuildItem, lastVarExp, true, lastVarItem, itemIndexMapping, reductionVarExpIndHash, &processing);
											if (processing) {
												return false;
											}
										}
									}

								}
							}
						}




						


						CAbstractComplexQueryFinishingHandler* finishingHandler = queryProcessingData->getFinishingHandler();

						if (!finishingHandler) {
							if (mConfConcurrentAnswerGeneration) {
								finishingHandler = new CQtConcurrentStreamingComplexQueryFinishingHandler(queryProcessingData, mOntoAnsweringItem, mConfExtendedLogging, getDomain(), mConfMaximumBatchMappingsComputationSize, mConfFirstBatchMappingsComputationSize, mConfBatchMappingsComputationSizeIncreasingFactor, mConfContinueMappingsCompuationWhenResultsNotWriteable, mConfMappingsComputationUnlimitedInterpretationSize);
							} else {
								finishingHandler = new CSequentialStreamingComplexQueryFinishingHandler(queryProcessingData, mOntoAnsweringItem, mConfExtendedLogging, getDomain(), mConfMaximumBatchMappingsComputationSize, mConfFirstBatchMappingsComputationSize, mConfBatchMappingsComputationSizeIncreasingFactor, mConfContinueMappingsCompuationWhenResultsNotWriteable, mConfMappingsComputationUnlimitedInterpretationSize);
							}
							queryProcessingData->setFinishingHandler(finishingHandler);
							finishingHandler->init();
						}

						if (finishingHandler->processQueryResults()) {

#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
							if (mConfDebugWriteVariableCompositionItems) {
								CAbstractStreamingComplexQueryFinishingHandler* incFinishingHandler = dynamic_cast<CAbstractStreamingComplexQueryFinishingHandler*>(finishingHandler);
								if (incFinishingHandler && incFinishingHandler->getResultUsingLastVariableCompositionItem()) {
									debugWriteVariableMappingToFileWithHeader(varBuildItem, incFinishingHandler->getResultUsingLastVariableCompositionItem(), "last");

									//debugCheckVariableMappingFromFile(incFinishingHandler->mVarBuildItem, incFinishingHandler->mLastVarItem);

									//debugWriteVariableMappingToFile(incFinishingHandler->mVarBuildItem, incFinishingHandler->mLastVarItem);
								}
							}
#endif


						 	finishingHandler->finishQueryProcessing();
							delete finishingHandler;
						} else {
							return false;
						}

					



						//QSet<QString> indi1Set;
						//QSet<QString> indi2Set;
						//QFile file1("./Tests/sparql-response.xml");
						//cint64 firstMissingIndiID1 = -1;
						//cint64 firstMissingIndiID2 = -1;
						//if (file1.open(QIODevice::ReadOnly)) {
						//	while (!file1.atEnd()) {
						//		QString line = file1.readLine().trimmed();
						//		if (line.startsWith("<uri>")) {
						//			QString indiName = line.mid(5, line.length() - 11);
						//			indi1Set.insert(indiName);
						//		}
						//	}

						//	auto it = bindsAnswersResult->getVariableBindingsAnswersIterator();
						//	while (it->hasNext()) {
						//		auto binding = it->getNext();
						//		auto bindIt = binding->getVariableBindingsIterator();
						//		auto bindingData = bindIt->getNext();
						//		QString bindingString = bindingData->getBindingString();
						//		indi2Set.insert(bindingString);
						//		if (!indi1Set.contains(bindingString) && firstMissingIndiID2 < 0) {
						//			CVariableBindingStringDataResult* dataResult = dynamic_cast<CVariableBindingStringDataResult*>(bindingData);
						//			firstMissingIndiID2 = (cint64)dataResult->getData();
						//		}
						//	}


						//	QStringList additionalIndis2;
						//	QStringList additionalIndis1;
						//	for (QString indi2 : indi2Set) {
						//		if (!indi1Set.contains(indi2)) {
						//			additionalIndis2.append(indi2);
						//			bool debug = true;
						//		}
						//	}

						//	for (QString indi1 : indi1Set) {
						//		if (!indi2Set.contains(indi1)) {
						//			if (firstMissingIndiID1 < 0) {
						//				firstMissingIndiID1 = mOntoAnsweringItem->getOntology()->getStringMapping()->getIndividualFromName(indi1)->getIndividualID();
						//			}
						//			additionalIndis1.append(indi1);
						//			bool debug = true;
						//		}
						//	}

						//	if (!additionalIndis1.isEmpty() || !additionalIndis2.isEmpty()) {
						//		bool debug = true;

						//		QHash< TConceptNegPair, COptimizedComplexConceptItem* >* conItemHash = mOntoAnsweringItem->getComplexConceptNegationItemHash();
						//		for (COptimizedComplexConceptItem* conItem : *conItemHash) {
						//			if (conItem->getKnownInstanceItemSet() && conItem->getKnownInstanceItemSet()->size() >= 1400 && conItem->getKnownInstanceItemSet()->size() <= 2000) {

						//				bool abc = true;
						//				while (abc) {
						//					initializeRealizationInstancesIterators(conItem, 1, mAnswererContext);
						//					auto realIt = conItem->getRealizationIterator();
						//					realIt->begin();
						//					cint64 currentId = realIt->currentIndividualInstanceItemReference().getIndividualID();

						//					while (currentId < firstMissingIndiID1 && !realIt->atEnd()) {
						//						auto copyIt = realIt;
						//						currentId = realIt->currentIndividualInstanceItemReference().getIndividualID();
						//						if (currentId <= firstMissingIndiID1) {
						//							copyIt = realIt->getCopy();
						//						}
						//						realIt->moveNext();
						//						cint64 nextId = realIt->currentIndividualInstanceItemReference().getIndividualID();
						//						if (nextId > firstMissingIndiID1) {
						//							copyIt->moveNext();
						//						}
						//					}

						//				}

						//			}
						//		}
						//	}

						//}


					}
				}
				if (varBuildItem) {
					delete varBuildItem;
				}


				CComplexConceptAnsweringQuery* compConQuery = dynamic_cast<CComplexConceptAnsweringQuery*>(query);
				if (conceptItem && compConQuery) {
					bool direct = compConQuery->isDirect();
					bool flattened = compConQuery->isFlattened();
					if (compConQuery->isSatisfiableComputationRequired()) {
						bool satisfiable = conceptItem->isSatisfiable();
						CBooleanQueryResult* booleanQueryResult = new CBooleanQueryResult(satisfiable);
						query->setQueryResult(booleanQueryResult);
					}
					if (compConQuery->isSuperClassNodesComputationRequired() || compConQuery->isSubClassNodesComputationRequired()) {
						CClassSynsetsResult* classSynsetsResult = new CClassSynsetsResult();
						bool subClasses = compConQuery->isSubClassNodesComputationRequired();
						bool superClasses = compConQuery->isSuperClassNodesComputationRequired();
						bool abbreviatedIRIs = false;


						QSet<CHierarchyNode*>* visitedHierNodeSet = nullptr;
						if (!direct) {
							visitedHierNodeSet = new QSet<CHierarchyNode*>();
						}
						QList<CHierarchyNode*> visitHierNodetList;
						if (subClasses) {
							QSet<CHierarchyNode*>* childNodeSet = conceptItem->getDirectSubClassNodeSet();
							for (QSet<CHierarchyNode*>::const_iterator it = childNodeSet->constBegin(), itEnd = childNodeSet->constEnd(); it != itEnd; ++it) {
								CHierarchyNode* childNode(*it);
								visitHierNodetList.append(childNode);
							}
						}
						if (superClasses) {
							QSet<CHierarchyNode*>* parentNodeSet = conceptItem->getDirectSuperClassNodeSet();
							for (QSet<CHierarchyNode*>::const_iterator it = parentNodeSet->constBegin(), itEnd = parentNodeSet->constEnd(); it != itEnd; ++it) {
								CHierarchyNode* parentNode(*it);
								visitHierNodetList.append(parentNode);
							}
						}

						while (!visitHierNodetList.isEmpty()) {
							CHierarchyNode* nextNode = visitHierNodetList.takeFirst();

							CClassSynsetResult* nextClassSynset = new CClassSynsetResult(nextNode->getEquivalentConceptStringList(abbreviatedIRIs));
							classSynsetsResult->addClassSynset(nextClassSynset);


							if (!direct) {
								if (subClasses) {
									QSet<CHierarchyNode*>* tmpChildNodeSet = nextNode->getChildNodeSet();
									for (QSet<CHierarchyNode*>::const_iterator it = tmpChildNodeSet->constBegin(), itEnd = tmpChildNodeSet->constEnd(); it != itEnd; ++it) {
										CHierarchyNode* nextChildNode(*it);
										if (!visitedHierNodeSet->contains(nextChildNode)) {
											visitedHierNodeSet->insert(nextChildNode);
											visitHierNodetList.append(nextChildNode);
										}
									}
								}
								if (superClasses) {
									QSet<CHierarchyNode*>* tmpParentNodeSet = nextNode->getParentNodeSet();
									for (QSet<CHierarchyNode*>::const_iterator it = tmpParentNodeSet->constBegin(), itEnd = tmpParentNodeSet->constEnd(); it != itEnd; ++it) {
										CHierarchyNode* nextParentNode(*it);
										if (!visitedHierNodeSet->contains(nextParentNode)) {
											visitedHierNodeSet->insert(nextParentNode);
											visitHierNodetList.append(nextParentNode);
										}
									}
								}
							}
						}
						delete visitedHierNodeSet;
						query->setQueryResult(classSynsetsResult);
					}
					if (compConQuery->isEquivalentClassNodesComputationRequired()) {
						CClassSetResult* classesResult = new CClassSetResult();
						QSet<CHierarchyNode*>* childNodeSet = conceptItem->getDirectSubClassNodeSet();
						QSet<CHierarchyNode*>* parentNodeSet = conceptItem->getDirectSuperClassNodeSet();
						bool abbreviatedIRIs = false;
						if (parentNodeSet && childNodeSet && parentNodeSet->size() == 1 && childNodeSet->size() == 1) {
							CHierarchyNode* node = (*childNodeSet->constBegin());
							if (parentNodeSet->contains(node)) {
								QStringList eqClassNameList(node->getEquivalentConceptStringList(abbreviatedIRIs));
								for (QStringList::const_iterator it = eqClassNameList.constBegin(), itEnd = eqClassNameList.constEnd(); it != itEnd; ++it) {
									classesResult->addClass(*it);
								}
							}
						}
						query->setQueryResult(classesResult);
					}
					if (compConQuery->isInstancesComputationRequired()) {
						CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();
						CConceptRealization* conceptRealization = mOntoAnsweringItem->getOntology()->getRealization()->getConceptRealization();

						COptimizedComplexConceptInstanziatedIndividualItemHash* instanceItems = conceptItem->getKnownInstanceItems();
						bool abbreviatedIRIs = false;
						COptimizedComplexConceptInstanziatedIndividualItemLinker* instanceItemsLinker = nullptr;
						if (instanceItems) {
							instanceItemsLinker = instanceItems->getAddedRealizationIndividualInstanceItemReferenceLinker();
						}


						if (direct) {
							// TODO: make more efficient
							COptimizedComplexConceptInstanziatedIndividualItemHash* directInstanceItems = new COptimizedComplexConceptInstanziatedIndividualItemHash(*instanceItems);
							for (auto node : *conceptItem->getDirectSubClassNodeSet()) {
								CRealizationIndividualInstanceItemReferenceIterator* instanceIt = conceptRealization->getConceptInstancesIterator(node->getOneEquivalentConcept(), false, mDefaultRealizationSorting);
								instanceIt->begin();
								while (!instanceIt->atEnd()) {
									CRealizationIndividualInstanceItemReference itemRef = instanceIt->currentIndividualInstanceItemReference();
									COptimizedComplexConceptInstanziatedIndividualItemLinker itemRefLinker(itemRef);
									directInstanceItems->remove(&itemRefLinker);
									instanceIt->moveNext();
								}
								delete instanceIt;
							}

							instanceItemsLinker = nullptr;
							CQtManagedRestrictedModificationHash<COptimizedComplexConceptInstanziatedIndividualItemHasher, COptimizedComplexConceptInstanziatedIndividualItemLinker*>* directIndiRefLinkerHash = directInstanceItems->getIndividualInstanceItemReferenceLinkerHash();
							for (CQtManagedRestrictedModificationHash<COptimizedComplexConceptInstanziatedIndividualItemHasher, COptimizedComplexConceptInstanziatedIndividualItemLinker*>::const_iterator it = directIndiRefLinkerHash->constBegin(), itEnd = directIndiRefLinkerHash->constEnd(); it != itEnd; ++it) {
								COptimizedComplexConceptInstanziatedIndividualItemLinker* itemLinker = it.value();
								COptimizedComplexConceptInstanziatedIndividualItemLinker* newLinker = new COptimizedComplexConceptInstanziatedIndividualItemLinker(itemLinker->getRealizationIndividualInstanceItemReference());
								instanceItemsLinker = newLinker->append(instanceItemsLinker);
							}
						}


						if (flattened) {
							CIndividualsResult* individualResult = new CIndividualsResult();
							CIndividualsResultVisitorGenerator resultGenerator(individualResult, abbreviatedIRIs, true, mOntoAnsweringItem->getOntology()->getIndividualNameResolver());
							if (instanceItemsLinker) {
								for (auto itemLinker = instanceItemsLinker; itemLinker; itemLinker = itemLinker->getNext()) {
									sameRealization->visitSameIndividuals(itemLinker->getRealizationIndividualInstanceItemReference(), &resultGenerator);
								}
							}
							query->setQueryResult(individualResult);
						} else {
							CIndividualSynsetsResult* indiSynsResult = new CIndividualSynsetsResult();
							if (instanceItemsLinker) {
								for (auto itemLinker = instanceItemsLinker; itemLinker; itemLinker = itemLinker->getNext()) {
									CIndividualSynsetResult* individualSynsetResult = new CIndividualSynsetResult();
									CIndividualSynsetResultVisitorGenerator resultGenerator(individualSynsetResult, abbreviatedIRIs, true, mOntoAnsweringItem->getOntology()->getIndividualNameResolver());
									sameRealization->visitSameIndividuals(itemLinker->getRealizationIndividualInstanceItemReference(), &resultGenerator);
									indiSynsResult->addIndividualSynset(individualSynsetResult);
								}
							}
							query->setQueryResult(indiSynsResult);
						}

						if (direct) {
							while (instanceItemsLinker) {
								COptimizedComplexConceptInstanziatedIndividualItemLinker* tmpInstanceItemsLinker = instanceItemsLinker;
								instanceItemsLinker = instanceItemsLinker->getNext();
								delete tmpInstanceItemsLinker;
							}
						}

					}
				}
				if (conceptItem) {
					conceptItem->decQueryUsageCount();
				}

				queryProcessingData->setProcessingFinished();
				cint64 processingTime = queryProcessingData->getTotalProcessingTime();

				QString realReqString;
				CAnsweringHandlingStatistics* stats = mOntoAnsweringItem->getAnsweringHandlingStatistics();
				if (stats) {
					CAnsweringHandlingStatistics* initialStats = queryProcessingData->getQueryProcessingInitializedAnsweringStatistics();
					if (initialStats) {
						cint64 reqRealReqCount = stats->getRequestedRealizationRequirementCount() - initialStats->getRequestedRealizationRequirementCount();
						if (reqRealReqCount > 0) {
							realReqString += QString(", using %1 realization requests").arg(reqRealReqCount);
						}
					}
				}


				LOG(INFO, getDomain(), logTr("Finished processing of complex query '%3' in %1 ms%2.").arg(processingTime).arg(realReqString).arg(query->getQueryName()), this);

				CCallbackData* callback = queryProcessingData->getCallback();
				if (callback) {
					callback->doCallback();
				}
				CComplexQueryExtendibleData* extendibleData = queryProcessingData->getQueryExtendibleData();
				if (extendibleData) {
					installQueryExtendible(extendibleData, queryProcessingData);
				} else {
					delete queryProcessingData;
				}
				mOntoAnsweringItem->decCurrentlyAnsweringQueryCount();
				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incTotalAnsweredQueriesCount();
				cleanUpAnswersCache(true);
				return true;
			}























			bool COptimizedComplexExpressionAnsweringHandler::installQueryExtendible(CComplexQueryExtendibleData* extendibleData, CComplexQueryExpressionProcessingData* queryProcessingData) {
				QHash<cint64, CComplexQueryExpressionProcessingData*>* queryExtensionProcessingDataHash = mOntoAnsweringItem->getQueryExtensionProcessingDataHash();
				cint64 nextQueryExtensionId = mOntoAnsweringItem->getNextQueryExtensionId(true);
				extendibleData->setExtendibleId(nextQueryExtensionId);
				queryExtensionProcessingDataHash->insert(nextQueryExtensionId, queryProcessingData);
				return true;
			}
























































































			bool COptimizedComplexExpressionAnsweringHandler::continueCalculationCreation(CAnswererContext* answererContext) {
				bool processing = false;
				COptimizedComplexConceptStepAnsweringItem* conSatProcStepItem = mOntoAnsweringItem->getConceptSatisfiabilityProcessingStepItem();
				COptimizedComplexConceptStepAnsweringItem* conSuperProcStepItem = mOntoAnsweringItem->getConceptSuperClassesProcessingStepItem();
				COptimizedComplexConceptStepAnsweringItem* conSubProcStepItem = mOntoAnsweringItem->getConceptSubClassesProcessingStepItem();
				COptimizedComplexConceptStepAnsweringItem* conSubRealStepItem = mOntoAnsweringItem->getConceptSubClassesRealizationProcessingStepItem();
				COptimizedComplexConceptStepAnsweringItem* conEqProcStepItem = mOntoAnsweringItem->getConceptEquivalentClassesProcessingStepItem();
				COptimizedComplexConceptStepAnsweringItem* instancesProcStepItem = mOntoAnsweringItem->getConceptInstancesProcessingStepItem();
				QList<COptimizedComplexBuildingVariableCompositionsItem*>* buildingVarItemList = mOntoAnsweringItem->getVariableBuildingItemProcessingList();
				while (!processing && (conSatProcStepItem->hasConceptItemsQueued() || conSuperProcStepItem->hasConceptItemsQueued() || conSubProcStepItem->hasConceptItemsQueued() || conEqProcStepItem->hasConceptItemsQueued() || instancesProcStepItem->hasConceptItemsQueued() || !buildingVarItemList->isEmpty())) {
					
					
					processing = createComplexConceptItemCalculation(processing, conSatProcStepItem, answererContext, conSuperProcStepItem, conSubProcStepItem, conSubRealStepItem, conEqProcStepItem, instancesProcStepItem);





					// variable items
					if (!processing && !buildingVarItemList->isEmpty()) {
						bool continueProcessingBuildingVarItem = false;
						COptimizedComplexBuildingVariableCompositionsItem* buildVarItem = buildingVarItemList->first();

						COptimizedComplexBuildingClassVariableCompositionsItem* buildingClassVarItem = dynamic_cast<COptimizedComplexBuildingClassVariableCompositionsItem*>(buildVarItem);
						COptimizedComplexBuildingPropertyVariableCompositionsItem* buildingPropVarItem = dynamic_cast<COptimizedComplexBuildingPropertyVariableCompositionsItem*>(buildVarItem);
						COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingIndiVarItem = dynamic_cast<COptimizedComplexBuildingIndividualVariableCompositionsItem*>(buildVarItem);
						COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* buildingIndiMixedVarItem = dynamic_cast<COptimizedComplexBuildingIndividualMixedVariableCompositionsItem*>(buildVarItem);
						if (buildingIndiVarItem) {

							QList<COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*>* absorptionHanldingQueryExtensionPropagationList = buildingIndiVarItem->getAbsorptionBasedHandlingExtensionItemPropagationList();
							if (!processing && absorptionHanldingQueryExtensionPropagationList && !absorptionHanldingQueryExtensionPropagationList->isEmpty() && buildingIndiVarItem->isSatisfiable()) {
								COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationItem = absorptionHanldingQueryExtensionPropagationList->takeFirst();
								COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionHanldingQueryPart = absorptionPropagationItem->getAbsorptionBasedHandlingData();

								absorptionPropagationItem->setSplitPropagationItemProcessingQueued(false);
								if (absorptionHanldingQueryPart->hasIndividualVariables()) {
									processing = createVariableBindingPropagationTest(absorptionPropagationItem, buildingIndiVarItem, answererContext, continueProcessingBuildingVarItem);
								} else {
									processing = createAbsorbedQueryPartEntailmentTest(absorptionPropagationItem, buildingIndiVarItem, answererContext, continueProcessingBuildingVarItem);
								}

							}
						}


						bool subBuildItemProcessing = false;
						QList<COptimizedComplexVariableCompositionItem*>* compositionVarItemList = buildVarItem->getComputeVariableMappingItemList();
						// variable composition items
						while (!processing && !compositionVarItemList->isEmpty()) {
							COptimizedComplexVariableCompositionItem* compVarItem = compositionVarItemList->takeFirst();
							startMeasuringVariableCompositionItemComputationCost(compVarItem);
							compVarItem->setVariableMappingsComputationStarted(true);

							if (compVarItem->getCompositionType() == COptimizedComplexVariableCompositionItem::CONCEPT_BASE) {
								computeVariableCompositionItemFromConceptItemBase(compVarItem, buildVarItem, processing);

							} else if (compVarItem->getCompositionType() == COptimizedComplexVariableCompositionItem::CONCEPT_SUB_SUPER_CLASSES) {
								computeVariableCompositionItemFromConceptItemSubSuperCLasses(compVarItem, buildVarItem);

							} else if (compVarItem->getCompositionType() == COptimizedComplexVariableCompositionItem::SUB_SUPER_CLASSES_PROPAGATION) {
								computeVariableCompositionItemSubSuperClassesPropagation(compVarItem, buildVarItem);

							} else if (compVarItem->getCompositionType() == COptimizedComplexVariableCompositionItem::ROLE_SUB_SUPER_PROPERTIES) {
								computeVariableCompositionItemFromRoleSubSuperProperties(compVarItem, buildVarItem);


							} else if (compVarItem->getCompositionType() == COptimizedComplexVariableCompositionItem::SUB_SUPER_PROPERTIES_PROPAGATION) {
								computeVariableCompositionItemSubSuperPropertiesPropagation(compVarItem, buildVarItem);



							} else if (compVarItem->getCompositionType() == COptimizedComplexVariableCompositionItem::INDIVIDUAL_TYPE_EXTENSION) {
								computeVariableCompositionItemIndividualTypeExtension(compVarItem, buildingIndiMixedVarItem, processing, subBuildItemProcessing);

							} else if (compVarItem->getCompositionType() == COptimizedComplexVariableCompositionItem::INDIVIDUAL_PROPERTY_EXTENSION) {
								computeVariableCompositionItemIndividualPropertyExtension(compVarItem, buildingIndiMixedVarItem, processing, subBuildItemProcessing);



							} else if (compVarItem->getCompositionType() == COptimizedComplexVariableCompositionItem::BINDING_REDUCTION) {
								computeVariableCompositionItemReduction(compVarItem, buildVarItem);


							} else if (compVarItem->getCompositionType() == COptimizedComplexVariableCompositionItem::BINDING_EXTRACTION) {
								computeVariableCompositionItemExtraction(compVarItem, buildVarItem);


							} else if (compVarItem->getCompositionType() == COptimizedComplexVariableCompositionItem::JOINING) {
								computeVariableCompositionItemJoin(compVarItem, buildVarItem, processing);


							} else if (compVarItem->getCompositionType() == COptimizedComplexVariableCompositionItem::ROLE_PROPAGATION || compVarItem->getCompositionType() == COptimizedComplexVariableRolePropagationJoiningItem::ROLE_PROPAGATION_JOIN || compVarItem->getCompositionType() == COptimizedComplexVariableRolePropagationReplacementItem::ROLE_PROPAGATION_REPLACEMENT) {
								computeVariableCompositionItemPropagation(compVarItem, buildVarItem, answererContext, processing);



							} else if (compVarItem->getCompositionType() == COptimizedComplexVariableCompositionItem::DATA_LITERAL_BASE) {
								computeVariableCompositionItemFromDataLiteralBase(compVarItem, answererContext, buildVarItem);


							} else if (compVarItem->getCompositionType() == COptimizedComplexVariableCompositionItem::ABSOROPTION_BASED_EXTENSION) {
								computeVariableCompositionItemAbsorptionBasedExtension(compVarItem, processing, buildingIndiVarItem, answererContext, continueProcessingBuildingVarItem);



							} else if (compVarItem->getCompositionType() == COptimizedComplexVariableCompositionItem::DATA_LITERAL_EXTENSION) {
								computeVariableCompositionItemDataLiteralExtension(compVarItem, answererContext, buildVarItem);

							}

							compVarItem->setComputationQueued(false);
							if (!processing) {
								endMeasuringVariableCompositionItemComputationCost(compVarItem);
							}

						}











						
							
						// building variable items
						if (!subBuildItemProcessing && !processing && compositionVarItemList->isEmpty() && buildingIndiVarItem) {
							processing = createIndividualVariableCompositionItemsComputation(buildingIndiVarItem, processing, continueProcessingBuildingVarItem, compositionVarItemList, answererContext);
						}


						// building classes items
						if (!subBuildItemProcessing && !processing && compositionVarItemList->isEmpty() && buildingClassVarItem) {
							processing = createClassVariableCompositionItemsComputation(buildingClassVarItem, processing, continueProcessingBuildingVarItem, compositionVarItemList, answererContext);
						}

						// building property items
						if (!subBuildItemProcessing && !processing && compositionVarItemList->isEmpty() && buildingPropVarItem) {
							processing = createPropertyVariableCompositionItemsComputation(buildingPropVarItem, processing, continueProcessingBuildingVarItem, compositionVarItemList, answererContext);
						}

						// building indi mixed items
						if (!subBuildItemProcessing && !processing && compositionVarItemList->isEmpty() && buildingIndiMixedVarItem) {
							processing = createIndividualMixedVariableCompositionItemsComputation(buildingIndiMixedVarItem, processing, continueProcessingBuildingVarItem, compositionVarItemList, answererContext);
						}

						if (subBuildItemProcessing || processing && buildVarItem->isWaitingComputation() && !continueProcessingBuildingVarItem) {
							buildingVarItemList->removeFirst();
							buildVarItem->setProcessingQueued(false);
						}


						if (!processing && !subBuildItemProcessing) {
							if (!buildVarItem->isWaitingComputation() && (!buildVarItem->isSatisfiable() || buildVarItem->getComputeVariableMappingItemList()->isEmpty())) {
								CComplexQueryExpressionProcessingData* queryProcData = buildVarItem->getQueryProcessingData();
								queryProcData->decBuildingVariableItem();

								buildingVarItemList->removeFirst();
								buildVarItem->setProcessingQueued(false);

								CComputedItemDataNotificationLinker* notLinker = buildVarItem->takeNotificationLinker();
								if (notLinker) {
									COptimizedComplexBuildingVariableCompositionsItem* depBuildVarItem = notLinker->getBuildingVariableItemProcessingItem();
									if (depBuildVarItem) {
										depBuildVarItem->decWaitingSubVariableBuildingItemCount();
										if (!depBuildVarItem->isWaitingComputation() && !depBuildVarItem->isProcessingQueued()) {
											depBuildVarItem->setProcessingQueued(true);
											mOntoAnsweringItem->addProcessingVariableBuildingItem(depBuildVarItem);
										}
									}
								}


								if (!queryProcData->hasComputation()) {
									if (queryProcData->isComputationError()) {
										failQueryProcessing(queryProcData, new CQueryUnspecifiedStringError("Error in computation"));
									} else if (!finishQueryProcessing(queryProcData)) {
										queryProcData->incBuildingVariableItem();
										if (!buildVarItem->isProcessingQueued()) {
											buildVarItem->setProcessingQueued(true);
											mOntoAnsweringItem->addProcessingVariableBuildingItem(buildVarItem);
										}
									}
								}
							}
						}




					}

				}

				return processing;
			}




			bool COptimizedComplexExpressionAnsweringHandler::createComplexConceptItemCalculation(bool processing, COptimizedComplexConceptStepAnsweringItem* conSatProcStepItem, CAnswererContext* answererContext, COptimizedComplexConceptStepAnsweringItem* conSuperProcStepItem, COptimizedComplexConceptStepAnsweringItem* conSubProcStepItem, COptimizedComplexConceptStepAnsweringItem* conSubRealStepItem, COptimizedComplexConceptStepAnsweringItem* conEqProcStepItem, COptimizedComplexConceptStepAnsweringItem* instancesProcStepItem) {
				// satisfiability
				if (!processing && conSatProcStepItem->hasConceptItemsQueued()) {
					COptimizedComplexConceptItem* conceptItem = conSatProcStepItem->takeNextQueuedConceptItem();
					CComplexConceptStepComputationProcess* compStep = conceptItem->getComputationProcess()->getSatisfiableClassNodesComputationProcess(true);
					compStep->setComputationProcessQueued(false);
					if (!compStep->isComputationProcessFinished()) {
						if (searchSuperSubSatisfiabilityResult(conceptItem, answererContext)) {
						} else if (createSatisfiabilityTest(conceptItem, answererContext)) {
							processing = true;
							compStep->setComputationProcessStarted(true);
							compStep->setComputationProcessProcessing(true);
							compStep->incCurrentlyRunningComputationCount();
						}
					}
					if (!processing) {
						finishCalculationStepProcessing(conceptItem, compStep, answererContext);
					}
				}
				// super classes
				if (!processing && conSuperProcStepItem->hasConceptItemsQueued()) {
					COptimizedComplexConceptItem* conceptItem = conSuperProcStepItem->getNextQueuedConceptItem();
					CComplexConceptStepComputationProcess* compStep = conceptItem->getComputationProcess()->getSuperClassNodesComputationProcess(true);
					processing = false;
					if (!compStep->isComputationProcessFinished()) {
						QList<CHierarchyNode*>* possibleClassNodeTestingList = conceptItem->getPossibleSuperClassNodeTestingList();
						if (!possibleClassNodeTestingList && !compStep->isComputationProcessStarted()) {
							compStep->setComputationProcessStarted(true);

							searchSuperSubSuperClassesResult(conceptItem, answererContext);

							QSet<CConcept*>* knowSuperConceptSet = conceptItem->getKnownSuperConceptSet();
							QSet<CConcept*>* possibleSuperConceptSet = conceptItem->getPossibleSuperConceptSet();
							QSet<CHierarchyNode*>* knownSuperClassesSet = conceptItem->getKnownSuperClassNodeSet();
							QSet<CHierarchyNode*>* possibleSuperClassesSet = conceptItem->getPossibleSuperClassNodeSet();


							if (knowSuperConceptSet && possibleSuperConceptSet || knownSuperClassesSet && possibleSuperClassesSet) {
								conceptItem->setSuperClassComputationInitializing(false);
							} else {
								conceptItem->setSuperClassComputationInitializing(true);
								if (createSatisfiabilityTest(conceptItem, answererContext)) {
									processing = true;
									compStep->setComputationProcessProcessing(true);
									compStep->incCurrentlyRunningComputationCount();
								}
							}
						}

						if (!possibleClassNodeTestingList && !conceptItem->isSuperClassComputationInitializing()) {

							QSet<CConcept*>* knowSuperConceptSet = conceptItem->getKnownSuperConceptSet();
							if (knowSuperConceptSet) {
								CClassConceptClassification* classClassification = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification();
								CTaxonomy* taxonomy = classClassification->getClassConceptTaxonomy();
								QSet<CHierarchyNode*>* knownSuperClassesSet = conceptItem->getKnownSuperClassNodeSet();
								if (!knownSuperClassesSet) {
									knownSuperClassesSet = new QSet<CHierarchyNode*>();
									conceptItem->setKnownSuperClassNodeSet(knownSuperClassesSet);
								}
								for (QSet<CConcept*>::const_iterator it = knowSuperConceptSet->constBegin(), itEnd = knowSuperConceptSet->constEnd(); it != itEnd; ++it) {
									CConcept* knownSuperClass(*it);
									CHierarchyNode* hierNode = taxonomy->getHierarchyNode(knownSuperClass, false);
									knownSuperClassesSet->insert(hierNode);
								}
								delete knowSuperConceptSet;
								conceptItem->setKnownSuperConceptSet(nullptr);
							}

							QSet<CConcept*>* possibleSuperConceptSet = conceptItem->getPossibleSuperConceptSet();
							if (possibleSuperConceptSet) {
								CClassConceptClassification* classClassification = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification();
								CTaxonomy* taxonomy = classClassification->getClassConceptTaxonomy();
								QSet<CHierarchyNode*>* knownSuperClassesSet = conceptItem->getKnownSuperClassNodeSet();
								QSet<CHierarchyNode*>* possibleSuperClassesSet = new QSet<CHierarchyNode*>();
								for (QSet<CConcept*>::const_iterator it = possibleSuperConceptSet->constBegin(), itEnd = possibleSuperConceptSet->constEnd(); it != itEnd; ++it) {
									CConcept* possibleSuperClass(*it);
									CHierarchyNode* hierNode = taxonomy->getHierarchyNode(possibleSuperClass, false);
									if (!knownSuperClassesSet->contains(hierNode)) {
										possibleSuperClassesSet->insert(hierNode);
									}
								}
								if (conceptItem->getPossibleSuperClassNodeSet()) {
									conceptItem->getPossibleSuperClassNodeSet()->intersect(*possibleSuperClassesSet);
									delete possibleSuperClassesSet;
								} else {
									conceptItem->setPossibleSuperClassNodeSet(possibleSuperClassesSet);
								}
								delete possibleSuperConceptSet;
								conceptItem->setPossibleSuperConceptSet(nullptr);
							}


							QSet<CHierarchyNode*>* possibleSuperClassNodeSet = conceptItem->getPossibleSuperClassNodeSet();
							if (!possibleSuperClassNodeSet || possibleSuperClassNodeSet->isEmpty()) {
								QSet<CHierarchyNode*>* knownSuperClassSet = conceptItem->getKnownSuperClassNodeSet();
								if (knownSuperClassSet) {
									QSet<CHierarchyNode*>* directSuperClassSet = getDirectReducedSuperNodeSet(*knownSuperClassSet);
									conceptItem->setKnownSuperClassNodeSet(nullptr);
									delete knownSuperClassSet;
									conceptItem->setDirectSuperClassNodeSet(directSuperClassSet);
									delete possibleSuperClassNodeSet;
									conceptItem->setPossibleSuperClassNodeSet(nullptr);
								}
								finishCalculationStepProcessing(conceptItem, compStep, answererContext);
							} else {
								possibleClassNodeTestingList = new QList<CHierarchyNode *>();
								for (QSet<CHierarchyNode*>::const_iterator it = possibleSuperClassNodeSet->constBegin(), itEnd = possibleSuperClassNodeSet->constEnd(); it != itEnd; ++it) {
									CHierarchyNode* hierNode(*it);
									possibleClassNodeTestingList->append(hierNode);
								}
								qSort(possibleClassNodeTestingList->begin(), possibleClassNodeTestingList->end(), nodeSortMorePredecessorsThan);
								conceptItem->setPossibleSuperClassNodeTestingList(possibleClassNodeTestingList);
								delete possibleSuperClassNodeSet;
								conceptItem->setPossibleSuperClassNodeSet(nullptr);
							}
						}
						if (possibleClassNodeTestingList && !possibleClassNodeTestingList->isEmpty()) {
							CHierarchyNode* testingNode = possibleClassNodeTestingList->takeFirst();
							if (createSuperClassSubsumptionTest(conceptItem, testingNode, answererContext)) {
								processing = true;
								compStep->setComputationProcessProcessing(true);
								compStep->incCurrentlyRunningComputationCount();
							}
						}
						if (!possibleClassNodeTestingList || possibleClassNodeTestingList->isEmpty()) {
							compStep->setComputationProcessQueued(false);
							conSuperProcStepItem->takeNextQueuedConceptItem();
						}
					}
				}

				// sub classes
				if (!processing && conSubProcStepItem->hasConceptItemsQueued()) {
					COptimizedComplexConceptItem* conceptItem = conSubProcStepItem->getNextQueuedConceptItem();
					CComplexConceptStepComputationProcess* compStep = conceptItem->getComputationProcess()->getSubClassNodesComputationProcess(true);
					processing = false;
					if (!compStep->isComputationProcessFinished()) {
						if (!compStep->isComputationProcessStarted()) {
							compStep->setComputationProcessStarted(true);

							searchSuperSubSubClassesResult(conceptItem, answererContext);
						}

						
						QList<CHierarchyNode*>* possibleClassNodeTestingList = conceptItem->getPossibleSubClassNodeTestingList();
						if (possibleClassNodeTestingList && !possibleClassNodeTestingList->isEmpty()) {
							CHierarchyNode* testingNode = possibleClassNodeTestingList->takeFirst();
							QSet<CHierarchyNode*>* maxSubNodeSet = conceptItem->getMaximumSubClassNodeSet();
							if (maxSubNodeSet && maxSubNodeSet->contains(testingNode)) {
								conceptItem->getDirectSubClassNodeSet()->insert(testingNode);
							} else if (createSubClassSubsumptionTest(conceptItem, testingNode, answererContext)) {
								processing = true;
								compStep->setComputationProcessProcessing(true);
								compStep->incCurrentlyRunningComputationCount();
							}
						}
						if (!possibleClassNodeTestingList || possibleClassNodeTestingList->isEmpty()) {
							compStep->setComputationProcessQueued(false);
							conSubProcStepItem->takeNextQueuedConceptItem();
							if (compStep->getCurrentlyRunningComputationCount() == 0) {
								if (!conceptItem->getPossibleSubClassNodeTestingList() || conceptItem->getPossibleSubClassNodeTestingList()->isEmpty()) {
									finishSubClassCalculationStepProcessing(conceptItem, compStep, answererContext);
								}
							}
						}
					}
				}


				if (!processing && conSubRealStepItem->hasConceptItemsQueued()) {
					COptimizedComplexConceptItem* conceptItem = conSubRealStepItem->getNextQueuedConceptItem();
					CComplexConceptStepComputationProcess* compStep = conceptItem->getComputationProcess()->getSubClassRealizationProcess(true);
					CHierarchyNode* bottomHierNode = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy()->getBottomHierarchyNode();

					if (!compStep->isComputationProcessFinished()) {
						if (!compStep->isComputationProcessStarted()) {
							compStep->setComputationProcessStarted(true);

							QList<COntologyProcessingRequirement*> reqList;
							for (CHierarchyNode* node : *conceptItem->getDirectSubClassNodeSet()) {
								if (node != bottomHierNode) {
									COntologyProcessingConceptRealizationRequirement* conRealReq = new COntologyProcessingConceptRealizationRequirement(CIndividualReference(), node->getOneEquivalentConcept());
									reqList.append(conRealReq);
								}
							}

							if (!reqList.isEmpty()) {
								CAnsweringMessageDataRequirementCompletedRealization* reqCompMess = new CAnsweringMessageDataRequirementCompletedRealization(conceptItem);
								processRequirements(answererContext, mOntoAnsweringItem->getOntology(), reqList, reqCompMess);
								processing = true;
							} else {
								finishCalculationStepProcessing(conceptItem, compStep, answererContext);
							}
						}
					}

				}

				// equivalent classes
				if (!processing && conEqProcStepItem->hasConceptItemsQueued()) {
					COptimizedComplexConceptItem* conceptItem = conEqProcStepItem->takeNextQueuedConceptItem();
					CComplexConceptStepComputationProcess* compStep = conceptItem->getComputationProcess()->getEquivalentClassNodesComputationProcess(true);
					compStep->setComputationProcessQueued(false);
					if (!compStep->isComputationProcessStarted()) {
						compStep->setComputationProcessStarted(true);
					}
					if (!processing) {
						finishCalculationStepProcessing(conceptItem, compStep, answererContext);
					}
				}


				// instances
				if (!processing && instancesProcStepItem->hasConceptItemsQueued()) {
					COptimizedComplexConceptItem* conceptItem = instancesProcStepItem->getNextQueuedConceptItem();
					CComplexConceptStepInstanceComputationProcess* compStep = conceptItem->getComputationProcess()->getInstancesComputationProcess(true);
					processing = false;
					if (!compStep->isComputationProcessFinished() || compStep->requiresAdditionalInstanceComputation()) {

						startMeasuringComplexConceptItemComputationCost(conceptItem);


						if (!conceptItem->isLazyRealizationInitialized()) {
							conceptItem->setLazyRealizationInitialized(true);
							if (initializeRealization(conceptItem, answererContext)) {
								processing = true;
								compStep->setComputationProcessQueued(false);
								instancesProcStepItem->takeNextQueuedConceptItem();
							}
						}

						cint64 newRequiredCandidateCount = 0;
						if (compStep->requiresAllInstanceComputation()) {
							newRequiredCandidateCount = -1;
						} else {
							// subtract the eliminated candidates, i.e., those instance candidates that are confirmed non-instances
							cint64 possibleTestedNonInstanceCount = conceptItem->getPossibleTestedNonInstanceCount();
							cint64 realizationRetrievedInstanceCandidateCount = conceptItem->getRealizationRetrievedInstanceCandidateCount();
							cint64 requiredInstancesCount = compStep->getRequiredInstancesCount();
							newRequiredCandidateCount = requiredInstancesCount - realizationRetrievedInstanceCandidateCount + possibleTestedNonInstanceCount;
							if (newRequiredCandidateCount < 0) {
								newRequiredCandidateCount = 0;
							}
						}


						if (conceptItem->isLazyRealizationInitialized() && !conceptItem->isLazyRealizationProcessing() && !compStep->isComputationProcessStarted()) {
							compStep->setComputationProcessStarted(true);

							if (mConfExtendedLogging) {
								LOG(INFO, getDomain(), logTr("Starting instance computation for complex concept item %1.").arg(conceptItem->getConceptItemId()), this);
							}



							if (initializeRealizationInstancesIterators(conceptItem, newRequiredCandidateCount, answererContext)) {
								processing = true;
								compStep->setComputationProcessQueued(false);
								instancesProcStepItem->takeNextQueuedConceptItem();
							}
						}



						if (!processing && !conceptItem->isLazyRealizationProcessing() && !conceptItem->isCompletelyHandledChecked()) {
							conceptItem->setCompletelyHandledChecked(true);
							checkPossibleInstancesCompletelyHandled(conceptItem, answererContext);
						}


						if (!processing && compStep->isComputationProcessStarted() && !conceptItem->isLazyRealizationProcessing() && !conceptItem->isLazyRealizationInstancesRetrieved()) {

							QSet<CRealizationIndividualInstanceItemReference>* possibleInstanceItemSet = conceptItem->getPossibleInstanceItemSet();

							if ((!possibleInstanceItemSet || possibleInstanceItemSet->isEmpty() || conceptItem->isLazyRealizationInitializationRequested()) && newRequiredCandidateCount != 0) {
								conceptItem->setLazyRealizationInitializationRequested(false);

								cint64 retrievingCount = newRequiredCandidateCount;
								double minRetrievingSize = conceptItem->getMinimalRetrievingInstanceItemSize();
								if (retrievingCount != -1) {
									minRetrievingSize = qMin(minRetrievingSize, mConfConceptItemInstanceCandiateRetrievingMaxSize);
									retrievingCount = qMax(retrievingCount, (cint64)minRetrievingSize);
								}

								if (initializeKnownPossibleInstances(conceptItem, retrievingCount, answererContext)) {
									conceptItem->setLazyRealizationInitializationRequested(true);
									processing = true;
									compStep->setComputationProcessQueued(false);
									instancesProcStepItem->takeNextQueuedConceptItem();
								} else {

									if (retrievingCount != -1 && minRetrievingSize < mConfConceptItemInstanceCandiateRetrievingMaxSize) {
										minRetrievingSize *= mConfConceptItemInstanceCandiateRetrievingSizeIncreaseFactor;
										conceptItem->setMinimalRetrievingInstanceItemSize(minRetrievingSize);
									}

									if (mConfExtendedLogging) {

										cint64 newCertainInstances = conceptItem->getLastRetrievedCertainInstanceItemCount();
										cint64 possibleInstances = 0;
										QString requestedCountString = QString::number(retrievingCount);
										if (newRequiredCandidateCount == -1) {
											requestedCountString = QString("all");
										}
										if (conceptItem->getPossibleInstanceItemSet()) {
											possibleInstances = conceptItem->getPossibleInstanceItemSet()->size();
										}
										mTotalPossibleInstancesCount += possibleInstances;
										LOG(INFO, getDomain(), logTr("Retrieved %2 certain and %3 possible of %4 requested instances for complex concept item %1.").arg(conceptItem->getConceptItemId()).arg(newCertainInstances).arg(possibleInstances).arg(requestedCountString), this);
									}

									conceptItem->setCompletelyHandledChecked(false);
									conceptItem->setCandidatePropagated(false);
								}
							}
						}



						//if (!processing && !conceptItem->isLazyRealizationProcessing() && !compStep->isComputationProcessStarted()) {
						//	compStep->setComputationProcessStarted(true);

						//	searchKnownPossibleInstances(conceptItem, answererContext);
						//}


						//if (!processing && !conceptItem->isLazyRealizationProcessing() && !conceptItem->isLazyNeighbourRealizationRequested()) {
						//	conceptItem->setLazyNeighbourRealizationRequested(true);
						//	if (schedulePossibleInstancesNeighourRealization(conceptItem, answererContext)) {
						//		processing = true;
						//		compStep->setComputationProcessQueued(false);
						//		instancesProcStepItem->takeNextQueuedConceptItem();
						//	}
						//}


						//if (!processing && !conceptItem->isLazyRealizationProcessing() && conceptItem->isLazyNeighbourRealizationPruningRequired()) {
						//	conceptItem->setLazyNeighbourRealizationPruningRequired(false);
						//	neighboursPrunePossibleInstances(conceptItem, answererContext);
						//}





						if (!processing && !conceptItem->isLazyRealizationProcessing() && !conceptItem->isConceptAbsorbed()) {
							QSet<CRealizationIndividualInstanceItemReference>* possibleInstanceItemSet = conceptItem->getPossibleInstanceItemSet();
							if (possibleInstanceItemSet && !possibleInstanceItemSet->isEmpty()) {
								CConcreteOntology* testingOnto = mOntoAnsweringItem->getTestingOntology();

								CConcept* reducedConcept = conceptItem->getCompletelyHandledReducedInstanceTestingConcept();
								bool reducedNegation = conceptItem->getCompletelyHandledReducedInstanceTestingConceptNegation();
								CBOXLIST<TConceptNegPair>* conceptOfInterestList = testingOnto->getTBox()->getConceptOfInterestList(true);
								conceptOfInterestList->append(TConceptNegPair(reducedConcept, reducedNegation));
								mTestingOntologyPreprocessor->preprocess(testingOnto, testingOnto->getConfiguration());
								conceptItem->setConceptAbsorbed(true);
							}
						}


						if (!processing && !conceptItem->isLazyRealizationProcessing() && conceptItem->isConceptAbsorbed() && !conceptItem->isCandidatePropagated()) {
							QSet<CRealizationIndividualInstanceItemReference>* possibleInstanceItemSet = conceptItem->getPossibleInstanceItemSet();
							QSet<CRealizationIndividualInstanceItemReference>* possibleInstanceCandPropItemSet = conceptItem->getPossibleInstanceCandidatePropagationItemSet();
							if (possibleInstanceItemSet && !possibleInstanceItemSet->isEmpty() || possibleInstanceCandPropItemSet && !possibleInstanceCandPropItemSet->isEmpty()) {


								CConcreteOntology* testingOnto = mOntoAnsweringItem->getTestingOntology();
								CBOXHASH<TConceptNegPair, CConceptOfInterestActivationTriggeringData>* activationTriggeringDataHash = testingOnto->getTBox()->getConceptOfInterestActivationTriggerDataHash(false);
								if (activationTriggeringDataHash) {
									CConcept* reducedConcept = conceptItem->getCompletelyHandledReducedInstanceTestingConcept();
									bool reducedNegation = conceptItem->getCompletelyHandledReducedInstanceTestingConceptNegation();

									CConceptOfInterestActivationTriggeringData activationTriggeringData = activationTriggeringDataHash->value(TConceptNegPair(reducedConcept, reducedNegation));
									CConcept* candidateConcept = activationTriggeringData.getCandidateConcept();
									CConcept* triggeringConcept = activationTriggeringData.getTriggeringConcept();

									if (triggeringConcept && !conceptItem->isLazyRealizationInstancesRetrieved()) {

										bool createOnePassPropagationTest = true;
										for (CConceptOfInterestActivationLinker* activationLinkerIt = activationTriggeringData.getActivationLinker(); activationLinkerIt; activationLinkerIt = activationLinkerIt->getNext()) {
											if (!activationLinkerIt->getActivationIndividual()) {
												createOnePassPropagationTest = false;
											}
										}


										if (createOnePassPropagationTest) {
											compStep->setAllInstanceComputationRequired(true);
											if (initializeKnownPossibleInstances(conceptItem, -1, answererContext)) {
												processing = true;
												compStep->setComputationProcessQueued(false);
												instancesProcStepItem->takeNextQueuedConceptItem();
											}
										}
									}


									if (!processing && (candidateConcept || triggeringConcept)) {


										if (createCandidatePropagationInstanceTest(conceptItem, activationTriggeringData, answererContext)) {
											processing = true;
											compStep->setComputationProcessProcessing(true);
											compStep->incCurrentlyRunningComputationCount();
											conceptItem->incCandidatePropagationProcessingCount();
										}
									}
								}


								QSet<CRealizationIndividualInstanceItemReference>::const_iterator candIt = conceptItem->getPossibleInstanceItemSetCandidatePropagationIterator();
								possibleInstanceCandPropItemSet = conceptItem->getPossibleInstanceCandidatePropagationItemSet();
								if (possibleInstanceCandPropItemSet && candIt == possibleInstanceCandPropItemSet->constEnd()) {
									conceptItem->setPossibleInstanceItemSetCandidatePropagationIteratorInitialized(false);
									conceptItem->setCandidatePropagated(true);
									instancesProcStepItem->takeNextQueuedConceptItem();
								}
							}
						}



						if (!processing && !conceptItem->isLazyRealizationProcessing() && !conceptItem->isCandidatePropagationProcessing()) {


							QSet<CRealizationIndividualInstanceItemReference>* possibleInstanceItemSet = conceptItem->getPossibleInstanceItemSet();
							if (possibleInstanceItemSet && !possibleInstanceItemSet->isEmpty()) {
								QSet<CRealizationIndividualInstanceItemReference>::iterator it = possibleInstanceItemSet->begin();
								CRealizationIndividualInstanceItemReference testingItem(*it);
								it = possibleInstanceItemSet->erase(it);
								if (createInvidiualItemInstanceTest(conceptItem, testingItem, answererContext)) {
									processing = true;
									compStep->setComputationProcessProcessing(true);
									compStep->incCurrentlyRunningComputationCount();
								}
							}
							if (!possibleInstanceItemSet || possibleInstanceItemSet->isEmpty()) {

								compStep->setComputationProcessQueued(false);
								instancesProcStepItem->takeNextQueuedConceptItem();
								if (compStep->getCurrentlyRunningComputationCount() == 0) {


									if (mConfExtendedLogging) {
										cint64 knownInstancesCount = 0;
										if (conceptItem->getKnownInstanceItems()) {
											knownInstancesCount = conceptItem->getKnownInstanceItems()->size();
										}
										LOG(INFO, getDomain(), logTr("Determined %2 known instances for complex concept item %1.").arg(conceptItem->getConceptItemId()).arg(knownInstancesCount), this);
									}


									conceptItem->setLastRetrievedCertainInstanceItemCount(conceptItem->getKnownInstanceItems()->size());
									updateComputedInstancesCount(conceptItem, compStep, answererContext);
									endMeasuringComplexConceptItemComputationCost(conceptItem);

									if (conceptItem->isLazyRealizationInstancesRetrieved()) {
										delete conceptItem->getPossibleInstanceItemSet();
										conceptItem->setPossibleInstanceItemSet(nullptr);
										finishCalculationStepProcessing(conceptItem, compStep, answererContext);
									} else {
										compStep->setComputationProcessQueued(false);
										conceptItem->setQueuedProcessStep(nullptr);
									}
								}

							}
						}
					}
				}


				return processing;
			}



			bool COptimizedComplexExpressionAnsweringHandler::createClassVariableCompositionItemsComputation(COptimizedComplexBuildingClassVariableCompositionsItem* buildingVarItem, bool processing, bool& continueProcessingBuildingVarItem, QList<COptimizedComplexVariableCompositionItem *>* compositionVarItemList, CAnswererContext* answererContext) {
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();

				if (!buildingVarItem->isInitializedBaseConceptVariableItems()) {
					QSet<CClassVariableExpression*>* remVarExpSet = buildingVarItem->getRemainingVariableExpressionSet();

					for (CClassVariableExpression* varExp : *remVarExpSet) {
						QList<COptimizedComplexConceptItem*> superConItemList = buildingVarItem->getVariableSuperConceptItems(varExp);
						QList<COptimizedComplexConceptItem*> subConItemList = buildingVarItem->getVariableSubConceptItems(varExp);

						auto hanlder = [&](COptimizedComplexConceptItem* conItem, bool superClasses) {

							COptimizedComplexVariableConceptSubSuperItem* varConBaseItem = conItem->getVariableConceptSuperSubClassesItem(superClasses);
							if (!varConBaseItem) {
								varConBaseItem = new COptimizedComplexVariableConceptSubSuperItem(conItem, superClasses);
								conItem->setVariableConceptSuperSubClassesItem(varConBaseItem, superClasses);
								varConBaseItem->setVariableMappingsComputationRequirement(mConfMinimalMappingsComputationSize);
								varConBaseItem->setComputationStepId(mOntoAnsweringItem->getNextComputationStepId());
							}
							addUsedVariableCompositionItem(buildingVarItem, varConBaseItem);
							if (!varConBaseItem->isVariableMappingsInitialized()) {
								varConBaseItem->setVariableMappingsInitialized(true);

								COptimizedComplexVariableIndividualMappings* varMapping = new COptimizedComplexVariableIndividualMappingsHash(1);
								varMapping->setBindingMapping(0, COptimizedComplexVariableIndividualMappings::CLASS_VARIABLE);
								varConBaseItem->setVariableMapping(varMapping);

								queueVariableItemComputation(buildingVarItem, varConBaseItem);
								processing = true;
							}

							COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(varExp);
							COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping = new COptimizedComplexVariableCompositionItemVariableIndexMapping();
							varItemIndexMapping.insert(varConBaseItem, itemIndexMapping);
							itemIndexMapping->insert(0, varExp);

#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
							QStringList debugVariableNameStringList = debugGetItemVariableNames(buildingVarItem, varExp, varConBaseItem);
							varConBaseItem->debugVariableNameStringList = debugVariableNameStringList;
							varConBaseItem->debugCreationString = QString("Computation step %1").arg(varConBaseItem->getComputationStepId()) + " with [" + debugVariableNameStringList.join(", ") + "] initialized";
							debugCreationStringList += varConBaseItem->debugCreationString;
							debugCreationString = debugCreationStringList.join("\r\n");
#endif

							QSet<COptimizedComplexVariableCompositionItem*>* varConBaseItemSet = varExpVarComItemHash->value(varExp);
							if (!varConBaseItemSet) {
								varConBaseItemSet = new QSet<COptimizedComplexVariableCompositionItem*>();
								varExpVarComItemHash->insert(varExp, varConBaseItemSet);
							}
							varConBaseItemSet->insert(varConBaseItem);


							buildingVarItem->setLastHandledVariableExpression(varExp);
							buildingVarItem->updateLastHandledVariableItemAssociation(varExp, varConBaseItem);
						};

						for (COptimizedComplexConceptItem* conItem : superConItemList) {
							hanlder(conItem, false);
						}
						for (COptimizedComplexConceptItem* conItem : subConItemList) {
							hanlder(conItem, true);
						}

					}
					

					buildingVarItem->setInitializedBaseConceptVariableItems(true);
				}



				QList<CClassVariableExpression*> remainVarExpList = buildingVarItem->getRemainingVariableExpressionSet()->toList();

				while (!processing && !remainVarExpList.isEmpty()) {

					CClassVariableExpression* varExp = remainVarExpList.takeFirst();
					COptimizedComplexVariableCompositionItem* varCompItem = nullptr;
					// join items
					QSet<COptimizedComplexVariableCompositionItem*>* joiningVarItemSet = varExpVarComItemHash->value(varExp);
					if (joiningVarItemSet->size() == 1) {
						varCompItem = *joiningVarItemSet->constBegin();
					} else {
						bool lastComputation = false;
						varCompItem = buildJoinedVariableJoiningItems(buildingVarItem, varExp, &processing, lastComputation).first;
					}

					cint64 propagatedCount = 0;
					QSet<QPair<CExpressionVariable*, CExpressionVariable*>> remSubSuperPropSet = buildingVarItem->getVariableRemainingSubSuperPropagationSet(varExp);
					if (!processing) {

						for (QPair<CExpressionVariable*, CExpressionVariable* > remSubSuperPropPair : remSubSuperPropSet) {
							CExpressionVariable* subVar = remSubSuperPropPair.first;
							CExpressionVariable* superVar = remSubSuperPropPair.second;
							CExpressionVariable* otherVar = superVar;
							bool superPropagation = true;
							if (varExp == superVar) {
								otherVar = subVar;
								superPropagation = false;
							}
							if (remSubSuperPropSet.size() - propagatedCount == 1 || !buildingVarItem->isRemovingPropagationDisconnecting(varExp, subVar, superVar)) {

								buildVariableSubSuperPropagationItem(buildingVarItem, varExp, varCompItem, nullptr, (CClassVariableExpression*)otherVar, superPropagation, &processing, answererContext);
								propagatedCount++;
								buildingVarItem->removeVariableSubSuperPropagation(subVar, superVar);
								buildingVarItem->setVariableExpressionUnhandled((CClassVariableExpression*)otherVar);

							}
						}

						if (propagatedCount == remSubSuperPropSet.size()) {
							buildingVarItem->setVariableExpressionHandled(varExp);
							buildingVarItem->setLastHandledVariableExpression(varExp);
							if (!processing) {
								remainVarExpList = buildingVarItem->getRemainingVariableExpressionSet()->toList();
							}
						}

					}

				}


				return processing;
			}










			bool COptimizedComplexExpressionAnsweringHandler::createIndividualMixedVariableCompositionItemsComputation(COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* buildingVarItem, bool processing, bool& continueProcessingBuildingVarItem, QList<COptimizedComplexVariableCompositionItem *>* compositionVarItemList, CAnswererContext* answererContext) {
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
				COptimizedComplexBuildingIndividualVariableCompositionsItem* indiSubBuildingItem = buildingVarItem->getIndividualVariableSubQueryBuildingItem();
				CExpressionVariable* lastIndiVarExp = indiSubBuildingItem->getLastHandledVariableExpression();

				if (!buildingVarItem->isInitializedVariableExtensions()) {

					COptimizedComplexVariableCompositionItem* lastIndiVarCompItem = indiSubBuildingItem->getVariableLastCompositionItem(lastIndiVarExp);
					COptimizedComplexVariableCompositionItemVariableIndexMapping* lastIndiItemIndexMapping = indiSubBuildingItem->getVariableItemIndexMapping(lastIndiVarExp).value(lastIndiVarCompItem);
					buildingVarItem->setLastIndividualVariableCompositionItem(lastIndiVarCompItem);
					buildingVarItem->setLastIndividualVariableCompositionItemIndexMapping(lastIndiItemIndexMapping);
					QSet<COptimizedComplexVariableCompositionItem*>* compItemSet = new QSet<COptimizedComplexVariableCompositionItem *>();
					varExpVarComItemHash->insert(lastIndiVarExp, compItemSet);
					if (lastIndiVarCompItem) {
						compItemSet->insert(lastIndiVarCompItem);
						buildingVarItem->getVariableItemIndexMapping(lastIndiVarExp).insert(lastIndiVarCompItem, lastIndiItemIndexMapping);
					}

					QList<COptimizedComplexBuildingClassVariableCompositionsItem*>* subClassItemList = buildingVarItem->getSubQueryClassVariableBuildingItemList();
					for (COptimizedComplexBuildingClassVariableCompositionsItem* subClassBuildItem : *subClassItemList) {

						CExpressionVariable* lastClassVarExp = subClassBuildItem->getLastHandledVariableExpression();
						COptimizedComplexVariableCompositionItem* lastClassVarCompItem = subClassBuildItem->getVariableLastCompositionItem(lastClassVarExp);
						COptimizedComplexVariableCompositionItemVariableIndexMapping* lastIndiItemIndexMapping = subClassBuildItem->getVariableItemIndexMapping(lastClassVarExp).value(lastClassVarCompItem);
						//QSet<COptimizedComplexVariableCompositionItem*>* compItemSet = new QSet<COptimizedComplexVariableCompositionItem *>();
						compItemSet->insert(lastClassVarCompItem);
						//varExpVarComItemHash->insert(lastIndiVarExp, compItemSet);
						buildingVarItem->getVariableItemIndexMapping(lastIndiVarExp).insert(lastClassVarCompItem, lastIndiItemIndexMapping);

					}
					buildingVarItem->setInitializedVariableExtensions(true);
				}

				if (!buildingVarItem->isIndividualVariableTypeExtensionsProcessed()) {

					COptimizedComplexVariableCompositionItem* lastIndiVarCompItem = buildingVarItem->getLastIndividualVariableCompositionItem();
					COptimizedComplexVariableCompositionItemVariableIndexMapping* lastIndiVarCompItemIndexMapping = buildingVarItem->getLastIndividualVariableCompositionItemIndexMapping();
					

					QSet<CIndividualVariableExpression*>* remVarExpSet = buildingVarItem->getRemainingVariableExpressionSet();


					for (CIndividualVariableExpression* varExp : *remVarExpSet) {

						if (!processing) {
							QSet<QPair<CIndividualVariableExpression*, CClassVariableExpression*>>* mHandledIndividualVariableTypeExtensionVariablePairSet = buildingVarItem->getIndividualVariableTypeExtensionVariableHandledPairSet();
							QList<CClassVariableExpression*> typeExtVarList = buildingVarItem->getIndividualVariableTypeExtensionVariables(varExp);
							for (CClassVariableExpression* typeExtVar : typeExtVarList) {
								if (!processing) {
									if (!mHandledIndividualVariableTypeExtensionVariablePairSet->contains(QPair<CIndividualVariableExpression*, CClassVariableExpression*>(varExp, typeExtVar))) {
										mHandledIndividualVariableTypeExtensionVariablePairSet->insert(QPair<CIndividualVariableExpression*, CClassVariableExpression*>(varExp, typeExtVar));
										processing = createIndividualTypeExtension(buildingVarItem, typeExtVar, lastIndiVarCompItemIndexMapping, varExp, lastIndiVarCompItem, processing, lastIndiVarExp, varExpVarComItemHash);
									}
								}
							}
						}

						if (!processing) {
							QSet<QPair<CIndividualVariableExpression*, CPropertyVariableExpression*>>* mHandledIndividualVariablePropertyExtensionVariablePairSet = buildingVarItem->getIndividualVariablePropertyExtensionVariableHandledPairSet();
							QList<CPropertyVariableExpression*> propExtVarList = buildingVarItem->getIndividualVariablePropertyExtensionVariables(varExp);
							for (CPropertyVariableExpression* propExtVar : propExtVarList) {
								if (!processing) {
									if (!mHandledIndividualVariablePropertyExtensionVariablePairSet->contains(QPair<CIndividualVariableExpression*, CPropertyVariableExpression*>(varExp, propExtVar))) {
										mHandledIndividualVariablePropertyExtensionVariablePairSet->insert(QPair<CIndividualVariableExpression*, CPropertyVariableExpression*>(varExp, propExtVar));
										processing = createIndividualPropertyExtension(buildingVarItem, propExtVar, lastIndiVarCompItemIndexMapping, varExp, lastIndiVarCompItem, processing, lastIndiVarExp, varExpVarComItemHash);
									}
								}
							}
						}

					}

					if (!processing) {
						buildingVarItem->setIndividualVariableTypeExtensionsProcessed(true);
					}
				}



				if (!processing && buildingVarItem->isIndividualVariableTypeExtensionsProcessed()) {
					COptimizedComplexVariableCompositionItem* varCompItem = nullptr;
					COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(lastIndiVarExp);
					QSet<COptimizedComplexVariableCompositionItem*>* joiningVarCompItemSet = buildingVarItem->getJoiningVariableCompositionItemSet();
					if (joiningVarCompItemSet->size() > 1) {

						QSet<COptimizedComplexVariableCompositionItem*>* varConBaseItemSet = varExpVarComItemHash->value(lastIndiVarExp);
						for (QSet<COptimizedComplexVariableCompositionItem*>::iterator it = varConBaseItemSet->begin(); it != varConBaseItemSet->end(); ) {
							if (!joiningVarCompItemSet->contains(*it)) {
								varItemIndexMapping.remove(*it);
								it = varConBaseItemSet->erase(it);
							} else {
								++it;
							}
						}

						bool lastComputation = false;
						varCompItem = buildJoinedVariableJoiningItems(buildingVarItem, lastIndiVarExp, &processing, lastComputation).first;
						joiningVarCompItemSet->clear();
						joiningVarCompItemSet->insert(varCompItem);
					}
				}				

				return processing;
			}







			bool COptimizedComplexExpressionAnsweringHandler::createIndividualTypeExtension(COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* buildingVarItem, CClassVariableExpression* typeExtVar, COptimizedComplexVariableCompositionItemVariableIndexMapping* lastIndiVarCompItemIndexMapping, CIndividualVariableExpression* varExp, COptimizedComplexVariableCompositionItem* lastIndiVarCompItem, bool processing, CExpressionVariable* lastIndiVarExp, QHash<CExpressionVariable *, QSet<COptimizedComplexVariableCompositionItem *> *>* varExpVarComItemHash) {
				COptimizedComplexBuildingClassVariableCompositionsItem* classVarCompBuildingItem = buildingVarItem->getExtensionClassVariableSubQueryBuildingItem(typeExtVar);

				COptimizedComplexVariableCompositionItem* classVarItem = nullptr;
				CExpressionVariable* lastClassVarExp = nullptr;
				COptimizedComplexVariableCompositionItemVariableIndexMapping* classItemIndexMapping = nullptr;
				if (classVarCompBuildingItem) {
					classVarItem = classVarCompBuildingItem->getVariableLastCompositionItem(typeExtVar);
					lastClassVarExp = classVarCompBuildingItem->getLastHandledVariableExpression();
					classItemIndexMapping = classVarCompBuildingItem->getVariableItemIndexMapping(lastClassVarExp).value(classVarItem);
				}

				cint64 indiVarPos = lastIndiVarCompItemIndexMapping->getSearchedVariablePosition(varExp);
				cint64 classVarPos = 0;
				cint64 classVarCount = 1;
				if (classItemIndexMapping) {
					classVarPos = classItemIndexMapping->getSearchedVariablePosition(typeExtVar);
					classVarCount = classItemIndexMapping->count();
				}

				cint64 indiVarCount = lastIndiVarCompItemIndexMapping->count();

				cint64 newVarCount = indiVarCount + classVarCount;

				COptimizedComplexVariableIndividualTypeExtensionItem*& varIndiTypeExtItem = lastIndiVarCompItem->getIndividualTypeExtensionItem(indiVarPos, classVarItem, classVarPos);
				if (!varIndiTypeExtItem) {
					varIndiTypeExtItem = new COptimizedComplexVariableIndividualTypeExtensionItem(lastIndiVarCompItem, classVarItem, indiVarPos, classVarPos);
					varIndiTypeExtItem->setVariableMappingsComputationRequirement(mConfMinimalMappingsComputationSize);
					varIndiTypeExtItem->setComputationStepId(mOntoAnsweringItem->getNextComputationStepId());
				}
				addUsedVariableCompositionItem(buildingVarItem, varIndiTypeExtItem);

				if (!varIndiTypeExtItem->isVariableMappingsInitialized()) {
					varIndiTypeExtItem->setVariableMappingsInitialized(true);

					COptimizedComplexVariableIndividualMappings* varMapping = new COptimizedComplexVariableIndividualMappingsHash(newVarCount);
					for (cint64 i = 0; i < indiVarCount; ++i) {
						varMapping->setBindingMapping(i, lastIndiVarCompItem->getVariableMapping()->getBindingMapping(i));
					}
					if (classVarItem) {
						for (cint64 i = 0; i < classVarCount; ++i) {
							varMapping->setBindingMapping(i + indiVarCount, classVarItem->getVariableMapping()->getBindingMapping(i));
						}
					} else {
						varMapping->setBindingMapping(indiVarCount, COptimizedComplexVariableIndividualMappings::CLASS_VARIABLE);
					}
					varIndiTypeExtItem->setVariableMapping(varMapping);

					queueVariableItemComputation(buildingVarItem, varIndiTypeExtItem);
					processing = true;
				}

				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(varExp);
				COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping = new COptimizedComplexVariableCompositionItemVariableIndexMapping();
				varItemIndexMapping.insert(varIndiTypeExtItem, itemIndexMapping);

				for (auto it = lastIndiVarCompItemIndexMapping->constBegin(), itEnd = lastIndiVarCompItemIndexMapping->constEnd(); it != itEnd; ++it) {
					itemIndexMapping->insert(it.key(), it.value());
				}
				if (classItemIndexMapping) {
					for (auto it = classItemIndexMapping->constBegin(), itEnd = classItemIndexMapping->constEnd(); it != itEnd; ++it) {
						itemIndexMapping->insert(it.key() + indiVarCount, it.value());
					}
				} else {
					itemIndexMapping->insert(indiVarCount, typeExtVar);
				}

#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				QStringList debugIndiVariableNameStringList = debugGetItemVariableNames(buildingVarItem, lastIndiVarExp, lastIndiVarCompItem);
				QStringList debugClassVariableNameStringList;
				if (classVarItem) {
					debugClassVariableNameStringList = debugGetItemVariableNames(buildingVarItem, lastIndiVarExp, classVarItem);
				} else {
					debugClassVariableNameStringList.append(typeExtVar->getName());
				}
				QStringList debugVariableNameStringList = debugGetItemVariableNames(buildingVarItem, varExp, varIndiTypeExtItem);
				varIndiTypeExtItem->debugVariableNameStringList = debugVariableNameStringList;
				varIndiTypeExtItem->debugCreationString = QString("Computation step %1").arg(varIndiTypeExtItem->getComputationStepId()) + " with [" + debugVariableNameStringList.join(", ") + "] obtained by extending [" + debugIndiVariableNameStringList.join(", ") + "] with types of [" + debugClassVariableNameStringList.join(", ") + "]";
				debugCreationStringList += varIndiTypeExtItem->debugCreationString;
				debugCreationString = debugCreationStringList.join("\r\n");
#endif

				QSet<COptimizedComplexVariableCompositionItem*>* varConBaseItemSet = varExpVarComItemHash->value(lastIndiVarExp);
				if (!varConBaseItemSet) {
					varConBaseItemSet = new QSet<COptimizedComplexVariableCompositionItem*>();
					varExpVarComItemHash->insert(lastIndiVarExp, varConBaseItemSet);
				}
				varConBaseItemSet->insert(varIndiTypeExtItem);


				buildingVarItem->setLastHandledVariableExpression(lastIndiVarExp);
				buildingVarItem->updateLastHandledVariableItemAssociation(lastIndiVarExp, varIndiTypeExtItem);

				buildingVarItem->getJoiningVariableCompositionItemSet()->insert(varIndiTypeExtItem);
				return processing;
			}









			bool COptimizedComplexExpressionAnsweringHandler::createIndividualPropertyExtension(COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* buildingVarItem, CPropertyVariableExpression* typeExtVar, COptimizedComplexVariableCompositionItemVariableIndexMapping* lastIndiVarCompItemIndexMapping, CIndividualVariableExpression* varExp, COptimizedComplexVariableCompositionItem* lastIndiVarCompItem, bool processing, CExpressionVariable* lastIndiVarExp, QHash<CExpressionVariable *, QSet<COptimizedComplexVariableCompositionItem *> *>* varExpVarComItemHash) {
				COptimizedComplexBuildingPropertyVariableCompositionsItem* propertyVarCompBuildingItem = buildingVarItem->getExtensionPropertyVariableSubQueryBuildingItem(typeExtVar);

				COptimizedComplexVariableCompositionItem* propVarItem = nullptr;
				CExpressionVariable* lastClassVarExp = nullptr;
				COptimizedComplexVariableCompositionItemVariableIndexMapping* propItemIndexMapping = nullptr;
				if (propertyVarCompBuildingItem) {
					propVarItem = propertyVarCompBuildingItem->getVariableLastCompositionItem(typeExtVar);
					lastClassVarExp = propertyVarCompBuildingItem->getLastHandledVariableExpression();
					propItemIndexMapping = propertyVarCompBuildingItem->getVariableItemIndexMapping(lastClassVarExp).value(propVarItem);
				}

				cint64 indiVarPos = lastIndiVarCompItemIndexMapping->getSearchedVariablePosition(varExp);
				cint64 propVarPos = 0;
				cint64 propVarCount = 1;
				if (propItemIndexMapping) {
					propVarPos = propItemIndexMapping->getSearchedVariablePosition(typeExtVar);
					propVarCount = propItemIndexMapping->count();
				}

				cint64 indiVarCount = lastIndiVarCompItemIndexMapping->count();

				cint64 newVarCount = indiVarCount + propVarCount;

				COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem*& varIndiPropExtItem = lastIndiVarCompItem->getIndividualNeighboringPropertyExtensionItem(indiVarPos, propVarItem, propVarPos);
				if (!varIndiPropExtItem) {
					varIndiPropExtItem = new COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem(lastIndiVarCompItem, propVarItem, indiVarPos, propVarPos);
					varIndiPropExtItem->setVariableMappingsComputationRequirement(mConfMinimalMappingsComputationSize);
					varIndiPropExtItem->setComputationStepId(mOntoAnsweringItem->getNextComputationStepId());
				}
				addUsedVariableCompositionItem(buildingVarItem, varIndiPropExtItem);

				if (!varIndiPropExtItem->isVariableMappingsInitialized()) {
					varIndiPropExtItem->setVariableMappingsInitialized(true);

					COptimizedComplexVariableIndividualMappings* varMapping = new COptimizedComplexVariableIndividualMappingsHash(newVarCount);
					for (cint64 i = 0; i < indiVarCount; ++i) {
						varMapping->setBindingMapping(i, lastIndiVarCompItem->getVariableMapping()->getBindingMapping(i));
					}
					if (propVarItem) {
						for (cint64 i = 0; i < propVarCount; ++i) {
							varMapping->setBindingMapping(i + indiVarCount, propVarItem->getVariableMapping()->getBindingMapping(i));
						}
					} else {
						varMapping->setBindingMapping(indiVarCount, COptimizedComplexVariableIndividualMappings::PROPERTY_VARIABLE);
					}
					varIndiPropExtItem->setVariableMapping(varMapping);

					queueVariableItemComputation(buildingVarItem, varIndiPropExtItem);
					processing = true;
				}

				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(varExp);
				COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping = new COptimizedComplexVariableCompositionItemVariableIndexMapping();
				varItemIndexMapping.insert(varIndiPropExtItem, itemIndexMapping);

				for (auto it = lastIndiVarCompItemIndexMapping->constBegin(), itEnd = lastIndiVarCompItemIndexMapping->constEnd(); it != itEnd; ++it) {
					itemIndexMapping->insert(it.key(), it.value());
				}
				if (propItemIndexMapping) {
					for (auto it = propItemIndexMapping->constBegin(), itEnd = propItemIndexMapping->constEnd(); it != itEnd; ++it) {
						itemIndexMapping->insert(it.key() + indiVarCount, it.value());
					}
				} else {
					itemIndexMapping->insert(indiVarCount, typeExtVar);
				}

#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				QStringList debugIndiVariableNameStringList = debugGetItemVariableNames(buildingVarItem, lastIndiVarExp, lastIndiVarCompItem);
				QStringList debugPropVariableNameStringList;
				if (propVarItem) {
					debugPropVariableNameStringList = debugGetItemVariableNames(buildingVarItem, lastIndiVarExp, propVarItem);
				} else {
					debugPropVariableNameStringList.append(typeExtVar->getName());
				}
				QStringList debugVariableNameStringList = debugGetItemVariableNames(buildingVarItem, varExp, varIndiPropExtItem);
				varIndiPropExtItem->debugVariableNameStringList = debugVariableNameStringList;
				varIndiPropExtItem->debugCreationString = QString("Computation step %1").arg(varIndiPropExtItem->getComputationStepId()) + " with [" + debugVariableNameStringList.join(", ") + "] obtained by extending [" + debugIndiVariableNameStringList.join(", ") + "] with neighboring properties of [" + debugPropVariableNameStringList.join(", ") + "]";
				debugCreationStringList += varIndiPropExtItem->debugCreationString;
				debugCreationString = debugCreationStringList.join("\r\n");
#endif

				QSet<COptimizedComplexVariableCompositionItem*>* varConBaseItemSet = varExpVarComItemHash->value(lastIndiVarExp);
				if (!varConBaseItemSet) {
					varConBaseItemSet = new QSet<COptimizedComplexVariableCompositionItem*>();
					varExpVarComItemHash->insert(lastIndiVarExp, varConBaseItemSet);
				}
				varConBaseItemSet->insert(varIndiPropExtItem);


				buildingVarItem->setLastHandledVariableExpression(lastIndiVarExp);
				buildingVarItem->updateLastHandledVariableItemAssociation(lastIndiVarExp, varIndiPropExtItem);

				buildingVarItem->getJoiningVariableCompositionItemSet()->insert(varIndiPropExtItem);
				return processing;
			}










			bool COptimizedComplexExpressionAnsweringHandler::createPropertyVariableCompositionItemsComputation(COptimizedComplexBuildingPropertyVariableCompositionsItem* buildingVarItem, bool processing, bool& continueProcessingBuildingVarItem, QList<COptimizedComplexVariableCompositionItem *>* compositionVarItemList, CAnswererContext* answererContext) {
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();

				if (!buildingVarItem->isInitializedBaseRoleVariableItems()) {
					QSet<CPropertyVariableExpression*>* remVarExpSet = buildingVarItem->getRemainingVariableExpressionSet();

					for (CPropertyVariableExpression* varExp : *remVarExpSet) {
						QList<CRole*> superRoleList = buildingVarItem->getVariableSuperRoles(varExp);
						QList<CRole*> subRoleList = buildingVarItem->getVariableSubRoles(varExp);

						auto hanlder = [&](CRole* role, bool superProperties) {

							COptimizedComplexVariableRoleSubSuperItem* varPropBaseItem = mOntoAnsweringItem->getRoleSubSuperItem(role, superProperties);
							if (!varPropBaseItem) {
								varPropBaseItem = new COptimizedComplexVariableRoleSubSuperItem(role, superProperties);
								mOntoAnsweringItem->setRoleSubSuperItem(role, superProperties, varPropBaseItem);
								varPropBaseItem->setVariableMappingsComputationRequirement(mConfMinimalMappingsComputationSize);
								varPropBaseItem->setComputationStepId(mOntoAnsweringItem->getNextComputationStepId());
							}
							addUsedVariableCompositionItem(buildingVarItem, varPropBaseItem);
							if (!varPropBaseItem->isVariableMappingsInitialized()) {
								varPropBaseItem->setVariableMappingsInitialized(true);

								COptimizedComplexVariableIndividualMappings* varMapping = new COptimizedComplexVariableIndividualMappingsHash(1);
								varMapping->setBindingMapping(0, COptimizedComplexVariableIndividualMappings::PROPERTY_VARIABLE);
								varPropBaseItem->setVariableMapping(varMapping);

								queueVariableItemComputation(buildingVarItem, varPropBaseItem);
								processing = true;
							}

							COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(varExp);
							COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping = new COptimizedComplexVariableCompositionItemVariableIndexMapping();
							varItemIndexMapping.insert(varPropBaseItem, itemIndexMapping);
							itemIndexMapping->insert(0, varExp);

#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
							QStringList debugVariableNameStringList = debugGetItemVariableNames(buildingVarItem, varExp, varPropBaseItem);
							varPropBaseItem->debugVariableNameStringList = debugVariableNameStringList;
							varPropBaseItem->debugCreationString = QString("Computation step %1").arg(varPropBaseItem->getComputationStepId()) + " with [" + debugVariableNameStringList.join(", ") + "] initialized";
							debugCreationStringList += varPropBaseItem->debugCreationString;
							debugCreationString = debugCreationStringList.join("\r\n");
#endif

							QSet<COptimizedComplexVariableCompositionItem*>* varConBaseItemSet = varExpVarComItemHash->value(varExp);
							if (!varConBaseItemSet) {
								varConBaseItemSet = new QSet<COptimizedComplexVariableCompositionItem*>();
								varExpVarComItemHash->insert(varExp, varConBaseItemSet);
							}
							varConBaseItemSet->insert(varPropBaseItem);


							buildingVarItem->setLastHandledVariableExpression(varExp);
							buildingVarItem->updateLastHandledVariableItemAssociation(varExp, varPropBaseItem);
						};

						for (CRole* conItem : superRoleList) {
							hanlder(conItem, false);
						}
						for (CRole* conItem : subRoleList) {
							hanlder(conItem, true);
						}

					}


					buildingVarItem->setInitializedBaseRoleVariableItems(true);
				}



				QList<CPropertyVariableExpression*> remainVarExpList = buildingVarItem->getRemainingVariableExpressionSet()->toList();

				while (!processing && !remainVarExpList.isEmpty()) {

					CPropertyVariableExpression* varExp = remainVarExpList.takeFirst();
					COptimizedComplexVariableCompositionItem* varCompItem = nullptr;
					// join items
					QSet<COptimizedComplexVariableCompositionItem*>* joiningVarItemSet = varExpVarComItemHash->value(varExp);
					if (joiningVarItemSet->size() == 1) {
						varCompItem = *joiningVarItemSet->constBegin();
					} else {
						bool lastComputation = false;
						varCompItem = buildJoinedVariableJoiningItems(buildingVarItem, varExp, &processing, lastComputation).first;
					}

					cint64 propagatedCount = 0;
					QSet<QPair<CExpressionVariable*, CExpressionVariable*>> remSubSuperPropSet = buildingVarItem->getVariableRemainingSubSuperPropagationSet(varExp);
					if (!processing) {

						for (QPair<CExpressionVariable*, CExpressionVariable* > remSubSuperPropPair : remSubSuperPropSet) {
							CExpressionVariable* subVar = remSubSuperPropPair.first;
							CExpressionVariable* superVar = remSubSuperPropPair.second;
							CExpressionVariable* otherVar = superVar;
							bool superPropagation = true;
							if (varExp == superVar) {
								otherVar = subVar;
								superPropagation = false;
							}
							if (remSubSuperPropSet.size() - propagatedCount == 1 || !buildingVarItem->isRemovingPropagationDisconnecting(varExp, subVar, superVar)) {

								buildVariableSubSuperPropagationItem(buildingVarItem, varExp, varCompItem, nullptr, (CPropertyVariableExpression*)otherVar, superPropagation, &processing, answererContext);
								propagatedCount++;
								buildingVarItem->removeVariableSubSuperPropagation(subVar, superVar);
								buildingVarItem->setVariableExpressionUnhandled((CPropertyVariableExpression*)otherVar);

							}
						}

						if (propagatedCount == remSubSuperPropSet.size()) {
							buildingVarItem->setVariableExpressionHandled(varExp);
							buildingVarItem->setLastHandledVariableExpression(varExp);
							if (!processing) {
								remainVarExpList = buildingVarItem->getRemainingVariableExpressionSet()->toList();
							}
						}

					}

				}


				return processing;
			}











			bool COptimizedComplexExpressionAnsweringHandler::createIndividualVariableCompositionItemsComputation(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, bool processing, bool& continueProcessingBuildingVarItem, QList<COptimizedComplexVariableCompositionItem *>* compositionVarItemList, CAnswererContext* answererContext) {
				QHash<CExpressionVariable*, COptimizedComplexConceptItem*>* varConItemHash = buildingVarItem->getVariableConceptItemHash();
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
				QSet<CIndividualVariableExpression*>* reuseVarExpComputationsCheckSet = buildingVarItem->getReuseVariableExpressionComputationsCheckSet();

				if (!buildingVarItem->isInitializedBaseConceptVariableItems()) {

					for (QHash<CExpressionVariable*, COptimizedComplexConceptItem*>::const_iterator it = varConItemHash->constBegin(), itEnd = varConItemHash->constEnd(); it != itEnd && buildingVarItem->isSatisfiable(); ++it) {
						CIndividualVariableExpression* varExp = (CIndividualVariableExpression*)it.key();
						COptimizedComplexConceptItem* conItem = it.value();
						if (!conItem->getKnownInstanceItems() || conItem->getKnownInstanceItems()->isEmpty()) {
							buildingVarItem->setSatisfiability(false);
							break;
						}
					}

					if (buildingVarItem->isSatisfiable()) {

						QSet<CIndividualVariableExpression*> propagatableOverVarSet;
						bool allPropagatableOverVar = true;
						for (QHash<CExpressionVariable*, COptimizedComplexConceptItem*>::const_iterator it = varConItemHash->constBegin(), itEnd = varConItemHash->constEnd(); it != itEnd && mConfSkipConceptItemsIntegrationForPropagationOverVariables; ++it) {
							CIndividualVariableExpression* varExp = (CIndividualVariableExpression*)it.key();
							COptimizedComplexConceptItem* conItem = it.value();

							QList<CObjectPropertyAssertionExpression*> propAssList = buildingVarItem->getUnhanledPropertyAssertionsExpressions(varExp);
							QList<CDataPropertyAssertionExpression*> dataPropAssList = buildingVarItem->getUnhanledDataPropertyAssertionsExpressions(varExp);

							if (dataPropAssList.isEmpty() && propAssList.size() == 2) {

								bool selfRestriction = false;
								for (CObjectPropertyAssertionExpression* propAss : propAssList) {
									if (buildingVarItem->getOtherVariableExpression(varExp, propAss) == varExp) {
										selfRestriction = true;
										break;
									}
								}

								if (!selfRestriction) {
									if (areConceptItemConceptsImpliedByPropertyDomains(varExp, conItem, propAssList, buildingVarItem, answererContext)) {
										propagatableOverVarSet.insert(varExp);
									} else {
										allPropagatableOverVar = false;
									}
								} else {
									allPropagatableOverVar = false;
								}

							} else {
								allPropagatableOverVar = false;
							}
						}


						if (allPropagatableOverVar) {
							QList<CIndividualVariableExpression*> remainVarExpList = getSortedRemainingVariableExpressionList(buildingVarItem, &propagatableOverVarSet);
							if (!remainVarExpList.isEmpty()) {
								propagatableOverVarSet.remove(remainVarExpList.first());
							}

							if (propagatableOverVarSet.size() == varConItemHash->size()) {
								propagatableOverVarSet.clear();
							}
						}



						for (QHash<CExpressionVariable*, COptimizedComplexConceptItem*>::const_iterator it = varConItemHash->constBegin(), itEnd = varConItemHash->constEnd(); it != itEnd; ++it) {
							CIndividualVariableExpression* varExp = (CIndividualVariableExpression*)it.key();
							COptimizedComplexConceptItem* conItem = it.value();


							//if (!conItem->isLazyRealizationInstancesRetrieved() && mConfMinimalMappingsComputationSize < 0) {
							//	CComplexConceptStepInstanceComputationProcess* instComStep = conItem->getComputationProcess()->getInstancesComputationProcess(true);
							//	instComStep->setAllInstanceComputationRequired(true);
							//	processing |= initializeQueryProcessingStep(instComStep, conItem, nullptr, buildingVarItem);
							//}
							if (!propagatableOverVarSet.contains(varExp)) {

								COptimizedComplexVariableConceptBaseItem* varConBaseItem = conItem->getVariableConceptBaseItem();
								if (!varConBaseItem) {
									varConBaseItem = new COptimizedComplexVariableConceptBaseItem(conItem);
									conItem->setVariableConceptBaseItem(varConBaseItem);
									reuseVarExpComputationsCheckSet->remove(varExp);
									varConBaseItem->setVariableMappingsComputationRequirement(mConfMinimalMappingsComputationSize);
									varConBaseItem->setComputationStepId(mOntoAnsweringItem->getNextComputationStepId());
								}
								addUsedVariableCompositionItem(buildingVarItem, varConBaseItem);
								if (!varConBaseItem->isVariableMappingsInitialized()) {
									varConBaseItem->setVariableMappingsInitialized(true);

									COptimizedComplexVariableIndividualMappings* varMapping = new COptimizedComplexVariableIndividualMappingsHash(1);
									varMapping->setBindingMapping(0, COptimizedComplexVariableIndividualMappings::INDIVIDUAL_VARIABLE);
									varConBaseItem->setVariableMapping(varMapping);

									queueVariableItemComputation(buildingVarItem, varConBaseItem);
									processing = true;
								}

								COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(varExp);
								COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping = new COptimizedComplexVariableCompositionItemVariableIndexMapping();
								varItemIndexMapping.insert(varConBaseItem, itemIndexMapping);
								itemIndexMapping->insert(0, varExp);

#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
								QStringList debugVariableNameStringList = debugGetItemVariableNames(buildingVarItem, varExp, varConBaseItem);
								varConBaseItem->debugVariableNameStringList = debugVariableNameStringList;
								varConBaseItem->debugCreationString = QString("Computation step %1").arg(varConBaseItem->getComputationStepId()) + " with [" + debugVariableNameStringList.join(", ") + "] initialized";
								debugCreationStringList += varConBaseItem->debugCreationString;
								debugCreationString = debugCreationStringList.join("\r\n");
#endif

								QSet<COptimizedComplexVariableCompositionItem*>* varConBaseItemSet = new QSet<COptimizedComplexVariableCompositionItem*>();
								varConBaseItemSet->insert(varConBaseItem);
								varExpVarComItemHash->insert(varExp, varConBaseItemSet);


								buildingVarItem->setLastHandledVariableExpression(varExp);
								buildingVarItem->updateLastHandledVariableItemAssociation(varExp, varConBaseItem);
							}
						}










						while (!reuseVarExpComputationsCheckSet->isEmpty()) {

							QSet<CIndividualVariableExpression*>::iterator it = reuseVarExpComputationsCheckSet->begin();
							CIndividualVariableExpression* varExp = *it;
							reuseVarExpComputationsCheckSet->erase(it);

							COptimizedComplexVariableCompositionItem* varCompItem = nullptr;

							// join items
							QSet<COptimizedComplexVariableCompositionItem*>* joiningVarItemSet = varExpVarComItemHash->value(varExp);
							if (joiningVarItemSet) {
								if (joiningVarItemSet->size() == 1) {
									varCompItem = *joiningVarItemSet->constBegin();
								} else {
									QList<QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>> joinedVarCompItems = reuseJoinedVariableCompositionItems(buildingVarItem, varExp);
									if (joinedVarCompItems.size() == 1) {
										varCompItem = joinedVarCompItems.first().first;
									}
								}

								if (varCompItem) {
									QList<CObjectPropertyAssertionExpression*> propAssList = buildingVarItem->getUnhanledPropertyAssertionsExpressions(varExp);
									cint64 reusedCount = 0;
									for (CObjectPropertyAssertionExpression* propAss : propAssList) {

										if (tryReuseVariableRolePropagationItem(buildingVarItem, varExp, varCompItem, nullptr, propAss)) {
											if (propAssList.size() - reusedCount == 1 || !buildingVarItem->isRemovingPropertyAssertionDisconnecting(varExp, propAss)) {
												reusedCount++;
												buildingVarItem->setPropertyAssertionHandled(propAss);
												reuseVarExpComputationsCheckSet->insert(buildingVarItem->getOtherVariableExpression(varExp, propAss));
												buildingVarItem->setVariableExpressionUnhandled(buildingVarItem->getOtherVariableExpression(varExp, propAss));
											}
										}
									}
									if (reusedCount == propAssList.size()) {
										buildingVarItem->setVariableExpressionHandled(varExp);
										buildingVarItem->setLastHandledVariableExpression(varExp);
									}
								}
							}

						}


						buildingVarItem->setInitializedBaseConceptVariableItems(true);
					}
				}






				// initialize items extracted from extendible query
				if (!processing && compositionVarItemList->isEmpty()) {
					QHash<cint64, CComplexQueryExpressionProcessingData*>* queryExtensionProcessingDataHash = mOntoAnsweringItem->getQueryExtensionProcessingDataHash();
					QList<CExpressionVariable*>* extendibleQueryTransferringExtractionVariableList = buildingVarItem->getExtendibleQueryTransferringExtractionVariableList();
					while (!extendibleQueryTransferringExtractionVariableList->isEmpty()) {
						CExpressionVariable* expVar = extendibleQueryTransferringExtractionVariableList->takeFirst();
						CComplexQueryExpressionProcessingData* queryProcessingData = buildingVarItem->getQueryProcessingData();
						CComplexQueryExtendibleData* queryExtendibleData = queryProcessingData->getQueryExtendibleData();
						QList<CQuery*>* dependingQueries = queryExtendibleData->getExtendingQueryList();
						for (CQuery* dependingQuery : *dependingQueries) {
							CComplexAssertionsIndividualVariablesAnsweringExtendibleQuery* depExtCompAssIndVarQuery = dynamic_cast<CComplexAssertionsIndividualVariablesAnsweringExtendibleQuery*>(dependingQuery);
							if (depExtCompAssIndVarQuery) {
								CComplexQueryExtendibleData* depQueryExtendibleData = depExtCompAssIndVarQuery->getExtendibleData();
								cint64 depExtId = depQueryExtendibleData->getExtendibleId();
								CComplexQueryExpressionProcessingData* depQueryProcessingData = queryExtensionProcessingDataHash->value(depExtId);
								if (depExtId > 0 && depQueryProcessingData) {
									QList<CExpressionVariable*>* depQueryVarList = depExtCompAssIndVarQuery->getDistinguishedVariableExpressions();
									cint64 varInd = 0;
									CExpressionVariable* depFoundVariable = nullptr;
									for (CExpressionVariable* depQueryVar : *depQueryVarList) {
										if (depQueryVar->getName() == expVar->getName() || depQueryVar == expVar) {
											depFoundVariable = depQueryVar;
											break;
										}
									}
									if (depFoundVariable) {
										COptimizedComplexBuildingVariableCompositionsItem* depVarBuildItem = depQueryProcessingData->getVariableBuildingItem();


										CExpressionVariable* depLastVarExp = depVarBuildItem->getLastHandledVariableExpression();
										if (depLastVarExp) {
											COptimizedComplexVariableCompositionItem* depLastVarItem = depVarBuildItem->getVariableLastCompositionItem(depLastVarExp);
											COptimizedComplexVariableCompositionItemVariableIndexMapping* depItemIndexMapping = depVarBuildItem->getVariableItemIndexMapping(depLastVarExp).value(depLastVarItem);


											//COptimizedComplexVariableCompositionItem* depLastVarItem = depVarBuildItem->getVariableLastCompositionItem(depFoundVariable);
											//COptimizedComplexVariableCompositionItemVariableIndexMapping* depItemIndexMapping = depVarBuildItem->getVariableItemIndexMapping(depFoundVariable).value(depLastVarItem);

											//cint64 depVarIdx = -1;
											//for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = depItemIndexMapping->constBegin(), itEnd = depItemIndexMapping->constEnd(); it != itEnd; ++it) {
											//	cint64 varIdx = it.key();
											//	CExpressionVariable* varExp = it.value();
											//	if (depFoundVariable == varExp) {
											//		depVarIdx = varIdx;
											//		break;
											//	}
											//}

											buildQueryTransferringVariableExtractionItem(depVarBuildItem, depFoundVariable, depLastVarItem, depItemIndexMapping, buildingVarItem, expVar, &processing);
											break;

										}
									}
								}
							}
						}
					}
				}




				// initialize basic data literals items
				if (!processing && compositionVarItemList->isEmpty()) {


					QList<CDataPropertyAssertionExpression*>* basicDataPropAssList = buildingVarItem->getBasicDataPropertyAssertionList();

					QHash<CDataLiteralVariableExpression*, CIndividualVariableExpression*>* dataVarIndiVarHash = buildingVarItem->getDataLiteralVariableIndidualVariableMappingHash();
					QSet<CIndividualVariableExpression*>* remainingDataVarExpSet = buildingVarItem->getRemainingExtendingDataVariableSet();

					if (!buildingVarItem->isInitializedBasicDataVariableItems() && buildingVarItem->isSatisfiable()) {

						for (CDataPropertyAssertionExpression* dataPropAssExp : *basicDataPropAssList) {
							CDataLiteralVariableExpression* dataVarExp = (CDataLiteralVariableExpression*)dataPropAssExp->getDataLiteralTermExpression();
							CIndividualTermExpression* indiTermExp = dataPropAssExp->getIndividualTermExpression();
							CDataPropertyTermExpression* dataPropertyExp = dataPropAssExp->getDataPropertyTermExpression();


							CRole* dataRole = mOntoAnsweringItem->getOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getDataPropertyTermRoleMappingHash()->value(dataPropertyExp);
							CIndividual* indi = mOntoAnsweringItem->getOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getIndividulTermIndiMappingHash()->value(indiTermExp);


							COptimizedComplexVariableDataLiteralBaseItem* dataLiteralBaseItem = new COptimizedComplexVariableDataLiteralBaseItem(indi, dataRole);
							COptimizedComplexVariableIndividualMappings* varMapping = new COptimizedComplexVariableIndividualMappingsHash(1);
							varMapping->setBindingMapping(0, COptimizedComplexVariableIndividualMappings::DATA_LITERAL_VARIABLE);
							dataLiteralBaseItem->setVariableMapping(varMapping);
							dataLiteralBaseItem->setVariableMappingsComputationRequirement(-1);
							dataLiteralBaseItem->setComputationStepId(mOntoAnsweringItem->getNextComputationStepId());
							addUsedVariableCompositionItem(buildingVarItem, dataLiteralBaseItem);
							queueVariableItemComputation(buildingVarItem, dataLiteralBaseItem);
							processing = true;


							COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(dataVarExp);
							COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping = new COptimizedComplexVariableCompositionItemVariableIndexMapping();
							varItemIndexMapping.insert(dataLiteralBaseItem, itemIndexMapping);
							itemIndexMapping->insert(0, dataVarExp);

#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
							QStringList debugVariableNameStringList = debugGetItemVariableNames(buildingVarItem, dataVarExp, dataLiteralBaseItem);
							dataLiteralBaseItem->debugVariableNameStringList = debugVariableNameStringList;
							dataLiteralBaseItem->debugCreationString = QString("Computation step %1").arg(dataLiteralBaseItem->getComputationStepId()) + " with [" + debugVariableNameStringList.join(", ") + "] initialized";
							debugCreationStringList += dataLiteralBaseItem->debugCreationString;
							debugCreationString = debugCreationStringList.join("\r\n");
#endif
							QSet<COptimizedComplexVariableCompositionItem*>* varConBaseItemSet = new QSet<COptimizedComplexVariableCompositionItem*>();
							varConBaseItemSet->insert(dataLiteralBaseItem);
							varExpVarComItemHash->insert(dataVarExp, varConBaseItemSet);

							buildingVarItem->setLastHandledVariableExpression(dataVarExp);
							buildingVarItem->updateLastHandledVariableItemAssociation(dataVarExp, dataLiteralBaseItem);

						}

						buildingVarItem->setInitializedBasicDataVariableItems(true);
					}

					basicDataPropAssList->clear();
				}






				// data literals extension of variable items 
				if (!processing && compositionVarItemList->isEmpty()) {


					QHash<CDataLiteralVariableExpression*, CIndividualVariableExpression*>* dataVarIndiVarHash = buildingVarItem->getDataLiteralVariableIndidualVariableMappingHash();
					QSet<CIndividualVariableExpression*>* remainingDataVarExpSet = buildingVarItem->getRemainingExtendingDataVariableSet();

					if (!buildingVarItem->isInitializedConceptDataVariableExtensionItems() && buildingVarItem->isSatisfiable()) {

						for (CIndividualVariableExpression* indiVarExp : *remainingDataVarExpSet) {
							QList<CDataPropertyAssertionExpression*> unDataPropAssList = buildingVarItem->getUnhanledDataPropertyAssertionsExpressions(indiVarExp);


							QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
							COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(indiVarExp);
							COptimizedComplexVariableCompositionItem* varCompItem = *varExpVarComItemHash->value(indiVarExp)->begin();
							// TODO: check if correct for multiple item index mappings (as long as data literals are processed with priority, then there is probably no problem)
							COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping = varItemIndexMapping.value(varCompItem);


							for (CDataPropertyAssertionExpression* dataPropAss : unDataPropAssList) {
								CDataLiteralVariableExpression* dataVarExp = (CDataLiteralVariableExpression*)dataPropAss->getDataLiteralTermExpression();
								CDataPropertyTermExpression* dataPropExp = dataPropAss->getDataPropertyTermExpression();

								buildVariableRolePropagationItem(buildingVarItem, indiVarExp, varCompItem, itemIndexMapping, dataVarExp, dataPropExp, &processing, answererContext);

								if (dataVarIndiVarHash->count(dataVarExp) <= 1) {
									buildingVarItem->setDataPropertyAssertionHandled(dataPropAss);
								}
							}
						}

						buildingVarItem->setInitializedConceptDataVariableExtensionItems(true);
					}



				}




				// joining basic data literals items and combining them with data extension items
				if (!processing && compositionVarItemList->isEmpty()) {


					QSet<CDataLiteralVariableExpression*>* basicDataVarJoiningSet = buildingVarItem->getBasicDataVariableJoiningSet();
					if (!buildingVarItem->isJoinedBasicDataVariableItems() && buildingVarItem->isSatisfiable()) {

						for (CDataLiteralVariableExpression* dataVarExp : *basicDataVarJoiningSet) {
							QSet<COptimizedComplexVariableCompositionItem*>* joiningVarItemSet = varExpVarComItemHash->value(dataVarExp);
							COptimizedComplexVariableCompositionItem* dataVarCompItem = *joiningVarItemSet->constBegin();
							if (joiningVarItemSet->size() > 1) {
								dataVarCompItem = buildJoinedVariableJoiningItems(buildingVarItem, dataVarExp, &processing).first;
							}

							QList<CDataPropertyAssertionExpression*> dataPropAssList = buildingVarItem->getUnhanledDataPropertyAssertionsExpressions(dataVarExp);

							for (QList<CDataPropertyAssertionExpression*>::iterator it = dataPropAssList.begin(), itEnd = dataPropAssList.end(); it != itEnd; ++it) {
								CDataPropertyAssertionExpression* dataPropAssExp(*it);

								CIndividualVariableExpression* indiVar = (CIndividualVariableExpression*)dataPropAssExp->getIndividualTermExpression();
								COptimizedComplexVariableCompositionItem* indiVarCompItem = *varExpVarComItemHash->value(indiVar)->begin();
								buildVariableJoiningItem(buildingVarItem, indiVar, false, indiVar, indiVarCompItem, dataVarExp, dataVarCompItem, nullptr, nullptr, &processing);

							}
						}
						buildingVarItem->setJoinedBasicDataVariableItems(true);
					}
				}







				QSet<CIndividualVariableExpression*>* remainVarExpSet = buildingVarItem->getRemainingVariableExpressionSet();
				if (!processing && !remainVarExpSet->isEmpty() && buildingVarItem->isSatisfiable() && (!buildingVarItem->isWaitingComputation() || continueProcessingBuildingVarItem)) {

					//TODO: sort remainVarExpList by number of instances
					QList<CIndividualVariableExpression*> remainVarExpList = getSortedRemainingVariableExpressionList(buildingVarItem, remainVarExpSet);

					while (!remainVarExpList.isEmpty() && !processing) {
						CIndividualVariableExpression* varExp = remainVarExpList.takeFirst();


						QList<CObjectPropertyAssertionExpression*> propAssList = buildingVarItem->getUnhanledPropertyAssertionsExpressions(varExp);
						QList<CDataPropertyAssertionExpression*> dataPropAssList = buildingVarItem->getUnhanledDataPropertyAssertionsExpressions(varExp);

						COptimizedComplexVariableCompositionItem* varCompItem = nullptr;
						// join items
						QSet<COptimizedComplexVariableCompositionItem*>* joiningVarItemSet = varExpVarComItemHash->value(varExp);
						COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(varExp);
						if (joiningVarItemSet->size() == 1 && varItemIndexMapping.size() == 1) {
							varCompItem = *joiningVarItemSet->constBegin();
						} else {
							bool lastComputation = remainVarExpList.isEmpty() && propAssList.isEmpty() && dataPropAssList.isEmpty() && (!buildingVarItem->getAbsorbingQueryPartsList() || buildingVarItem->getAbsorbingQueryPartsList()->isEmpty()) && mConfMinimalMappingsComputationSize < 0;
							if (lastComputation) {
								lastComputation = false;
								CComplexQueryExpressionProcessingData* queryProcData = buildingVarItem->getQueryProcessingData();
								CComplexAssertionsIndividualVariablesAnsweringQuery* compAssIndVarQuery = (CComplexAssertionsIndividualVariablesAnsweringQuery*)queryProcData->getQuery();
								if (compAssIndVarQuery && dynamic_cast<CVariableBindingsAnswersStreamingResult*>(compAssIndVarQuery->getQueryResult()) && !compAssIndVarQuery->getResultOrderingLinker()) {
									lastComputation = true;
								}
							}
							varCompItem = buildJoinedVariableJoiningItems(buildingVarItem, varExp, &processing, lastComputation).first;
						}
						cint64 propagatedCount = 0;
						if (!processing) {

							if (!dataPropAssList.isEmpty() && propAssList.isEmpty()) {
								for (QList<CDataPropertyAssertionExpression*>::iterator it = dataPropAssList.begin(), itEnd = dataPropAssList.end(); it != itEnd; ++it) {
									CDataPropertyAssertionExpression* dataPropAssExp(*it);
									QList<CIndividualVariableExpression*> indiBackDataPropAssList = buildingVarItem->getUnhandledDataExtensionIndividualTermExpressions(dataPropAssExp, varExp);
									if (indiBackDataPropAssList.size() <= 0) {
										buildingVarItem->setDataPropertyAssertionHandled(dataPropAssExp);
									} else if (indiBackDataPropAssList.size() == 1 || !buildingVarItem->isRemovingDataPropertyAssertionDisconnecting(varExp, dataPropAssExp)) {

										CIndividualVariableExpression* connIndiVarExp = *indiBackDataPropAssList.begin();
										COptimizedComplexVariableCompositionItem* connVarCompItem = *varExpVarComItemHash->value(connIndiVarExp)->begin();

										buildVariableJoiningItem(buildingVarItem, connIndiVarExp, false, varExp, varCompItem, connIndiVarExp, connVarCompItem, nullptr, nullptr, &processing);

										propagatedCount++;
										buildingVarItem->setDataPropertyAssertionHandled(dataPropAssExp);
										buildingVarItem->setVariableExpressionUnhandled(connIndiVarExp);
									}
								}

							} else {

								QList<CObjectPropertyAssertionExpression*> sortedPropAssList = getVariableRemainingSortedObjectPropertyList(buildingVarItem, varExp, varCompItem, propAssList);

								for (CObjectPropertyAssertionExpression* propAss : sortedPropAssList) {
									if (sortedPropAssList.size() - propagatedCount == 1 || !buildingVarItem->isRemovingPropertyAssertionDisconnecting(varExp, propAss)) {

										QHash<CExpressionVariable*, cint64> reductionVarExpIndHash = getPropagationReducableVariables(buildingVarItem, varExp, varCompItem, nullptr, propAss);
										varCompItem = buildVariableReductionItem(buildingVarItem, varExp, false, varCompItem, nullptr, reductionVarExpIndHash, &processing).first;

										buildVariableRolePropagationItem(buildingVarItem, varExp, varCompItem, nullptr, reductionVarExpIndHash, propAss, &processing, answererContext);
										propagatedCount++;
										buildingVarItem->setPropertyAssertionHandled(propAss);
										buildingVarItem->setVariableExpressionUnhandled(buildingVarItem->getOtherVariableExpression(varExp, propAss));
									}
								}
							}

							if (propagatedCount == propAssList.size() + dataPropAssList.size()) {
								buildingVarItem->setVariableExpressionHandled(varExp);
								buildingVarItem->setLastHandledVariableExpression(varExp);
								if (!processing) {
									remainVarExpList = getSortedRemainingVariableExpressionList(buildingVarItem, remainVarExpSet);
								}
							}

						}

					}
				}







				QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* absorbingQueryPartsList = buildingVarItem->getAbsorbingQueryPartsList();
				if (!processing && absorbingQueryPartsList && !absorbingQueryPartsList->isEmpty() && buildingVarItem->isSatisfiable() && (!buildingVarItem->isWaitingComputation() || continueProcessingBuildingVarItem)) {

					// - try to use ordinary sparql evaluation with limit 1 if it is an ask query
					// -- check whether all properties and classes are instantiated by individual nodes
					// -- build corresponding concept items and add property and class assertion to variable building item
					// -- reschedule absorption handling part such that absorption-based item is created if no answer is found

					CComplexQueryExpressionProcessingData* queryProcessingData = buildingVarItem->getQueryProcessingData();
					if (mConfBooleanQueryExistentialPartOrdinaryEvaluation && !buildingVarItem->hasAbsorptionBasedQueryPartsOrdinaryEvaluated()) {
						buildingVarItem->setAbsorptionBasedQueryPartsOrdinaryEvaluated(true);

						processing = generateOrdinaryVariableBuiltItemEvaluationFromAbsorptionBasedPart(queryProcessingData, answererContext, absorbingQueryPartsList, buildingVarItem, processing);
					}

					if (!processing) {

						bool ordinaryEvaluationSucceeded = false;
						COptimizedComplexBuildingIndividualVariableCompositionsItem* subItemOrdinaryEval = buildingVarItem->getAbsorptionBasedQueryPartsOrdinaryEvaluationSubVariableBuiltItem();
						if (buildingVarItem->hasAbsorptionBasedQueryPartsOrdinaryEvaluated() && subItemOrdinaryEval && subItemOrdinaryEval->isSatisfiable()) {
							LOG(INFO, getDomain(), logTr("Found answer with ordinary evaluation for %1 existential parts of Boolean query, skipping absorption-based handling.").arg(absorbingQueryPartsList->size()), this);
							ordinaryEvaluationSucceeded = true;
						} else if (buildingVarItem->hasAbsorptionBasedQueryPartsOrdinaryEvaluated() && subItemOrdinaryEval) {
							LOG(INFO, getDomain(), logTr("No answer found with ordinary evaluation for %1 existential parts of Boolean query, continuing with absorption-based handling.").arg(absorbingQueryPartsList->size()), this);
						}

						if (!ordinaryEvaluationSucceeded) {
							QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*> entailmentCheckingAbsorptionBasedHandlingQueryPartDatas;
							while (!absorbingQueryPartsList->isEmpty()) {
								COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorpingHanldingQueryPart = absorbingQueryPartsList->takeFirst();
								absorbExistentialQueryPart(absorpingHanldingQueryPart, answererContext);

								if (absorpingHanldingQueryPart->hasPreparationVariables()) {
									buildingVarItem->addAbsorbedBasedQueryPartItemExtensionHandling(absorpingHanldingQueryPart);
								} else {
									buildingVarItem->addAbsorbedBasedQueryPartEntailmentCheckingHandling(absorpingHanldingQueryPart);
									entailmentCheckingAbsorptionBasedHandlingQueryPartDatas.append(absorpingHanldingQueryPart);
								}

							}

							if (mConfDebugTestingOntologyWriting) {
								COntologyTextFormater::writeOntologyToFile(mOntoAnsweringItem->getTestingOntology(), "Debugging/Answering/Absorbed-Query-Expressions.txt");
							}

							mTestingOntologyPreprocessor->preprocess(mOntoAnsweringItem->getTestingOntology(), mOntoAnsweringItem->getTestingOntology()->getConfiguration());

							if (mConfDebugTestingOntologyWriting) {
								COntologyTextFormater::writeOntologyToFile(mOntoAnsweringItem->getTestingOntology(), "Debugging/Answering/Absorbed-Preprocessed-Query-Expressions.txt");
							}

							for (COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* data : entailmentCheckingAbsorptionBasedHandlingQueryPartDatas) {
								createAbsorbedQueryPartEntailmentTest(data, queryProcessingData, answererContext);
							}
						} else {
							absorbingQueryPartsList->clear();
						}


					}

				}







				QList<COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*>* absorptionHanldingQueryExtensionJoiningList = buildingVarItem->getAbsorptionBasedHandlingExtensionItemJoiningList();
				if (!processing && absorptionHanldingQueryExtensionJoiningList && !absorptionHanldingQueryExtensionJoiningList->isEmpty() && buildingVarItem->isSatisfiable() && (!buildingVarItem->isWaitingComputation() || continueProcessingBuildingVarItem)) {

					COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationItem = absorptionHanldingQueryExtensionJoiningList->takeFirst();
					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionHandlingData = absorptionPropagationItem->getAbsorptionBasedHandlingData();
					CExpressionVariable* initializerVariableExp = absorptionHandlingData->getInitializerVariableExpression();


					QList< QPair<COptimizedComplexVariableCompositionItem*, CExpressionVariable*> >* variableExtendingItemList = absorptionPropagationItem->getExtendingCompositionItemAssociatedVariablePairList();
					QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();

					// TODO: check, may has to be extended/adapted for cases where same item is used for several variables


					QSet<CExpressionVariable*>* indiVarSet = absorptionHandlingData->getIndividualVariableSet();
					QSet<COptimizedComplexVariableCompositionItem*> integratedItemSet;

					for (CExpressionVariable* indiVarExp : *indiVarSet) {
						COptimizedComplexVariableCompositionItem* varCompItem = buildingVarItem->getVariableLastCompositionItem(indiVarExp);
						CExpressionVariable* associatedVariableExpression = buildingVarItem->getVariableLastCompositionItemAssociatedVariableExpression(indiVarExp);

						if (!integratedItemSet.contains(varCompItem)) {
							integratedItemSet.insert(varCompItem);
							if (varCompItem->getVariableMapping()->getBindingSize() > 1) {
								variableExtendingItemList->append(QPair<COptimizedComplexVariableCompositionItem*, CExpressionVariable*>(varCompItem, associatedVariableExpression));
							}
						}


					}



					COptimizedComplexVariableCompositionItem* lastCompItem = absorptionPropagationItem;
					CExpressionVariable* lastVariableExp = initializerVariableExp;
					for (QPair<COptimizedComplexVariableCompositionItem*, CExpressionVariable*> extendingItemAssociatedVariablePair : *variableExtendingItemList) {
						COptimizedComplexVariableCompositionItem* extendingItem = extendingItemAssociatedVariablePair.first;
						CExpressionVariable* associatedVariableExpression = extendingItemAssociatedVariablePair.second;
						lastCompItem = buildVariableJoiningItem(buildingVarItem, initializerVariableExp, false, lastVariableExp, lastCompItem, associatedVariableExpression, extendingItem, nullptr, nullptr, &processing).first;
					}


					for (CExpressionVariable* indiVarExp : *indiVarSet) {
						QSet<COptimizedComplexVariableCompositionItem*>* compItems = varExpVarComItemHash->value(indiVarExp);
						if (compItems->size() > 0) {
							compItems->clear();
							compItems->insert(absorptionPropagationItem);
						}
					}

					for (CExpressionVariable* indiVarExp : *indiVarSet) {
						buildingVarItem->updateLastHandledVariableItemAssociation(indiVarExp, lastCompItem);
					}
					buildingVarItem->updateLastHandledVariableItemAssociation(initializerVariableExp, lastCompItem);
					buildingVarItem->setLastHandledVariableExpression(initializerVariableExp);


				}


				QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* absorbedQueryPartsItemExtensionList = buildingVarItem->getAbsorbedQueryPartItemExtensionHandlingList();
				if (!processing && absorbedQueryPartsItemExtensionList && !absorbedQueryPartsItemExtensionList->isEmpty() && buildingVarItem->isSatisfiable() && (!buildingVarItem->isWaitingComputation() || continueProcessingBuildingVarItem)) {

					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionHanldingQueryPart = absorbedQueryPartsItemExtensionList->takeFirst();

					buildAbsorbedPartHandlingItem(buildingVarItem, absorptionHanldingQueryPart, varExpVarComItemHash, &processing);

				}

				return processing;
			}

			bool COptimizedComplexExpressionAnsweringHandler::generateOrdinaryVariableBuiltItemEvaluationFromAbsorptionBasedPart(CComplexQueryExpressionProcessingData* queryProcessingData, CAnswererContext* answererContext, QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData *>* absorbingQueryPartsList, COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, bool processing) {
				CComplexAssertionsIndividualVariablesAnsweringQuery* compAssIndVarQuery = dynamic_cast<CComplexAssertionsIndividualVariablesAnsweringQuery*>(queryProcessingData->getQuery());
				if (compAssIndVarQuery->isBooleanEntailmentResultRequired()) {

					bool ordinaryEvaluationUseful = true;

					cint64 statsTerms = 0;
					cint64 statsTermsNoDeterministicInstances = 0;
					cint64 statsTermsNoIndividualInstances = 0;
					COccurrenceStatisticsCacheReader* occStatsCacheReader = answererContext->getAnsweringCalculationHandler()->getOccurrenceStatisticsCacheReader();
					cint64 ontologyId = mOntoAnsweringItem->getOntology()->getOntologyID();

					for (COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorpingHanldingQueryPart : *absorbingQueryPartsList) {

						QSet<CExpressionVariable*>* restExpVarSet = absorpingHanldingQueryPart->getRestrictedAbsorptionVariableSet();

						QSet<CObjectPropertyAssertionExpression*>* propAssSet = absorpingHanldingQueryPart->getPropertyAssertionSet();
						for (CObjectPropertyAssertionExpression* propAssExp : *propAssSet) {
							CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
							if (dynamic_cast<CObjectPropertyExpression*>(propExp)) {
								CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getFirstIndividualTermExpression());
								CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getSecondIndividualTermExpression());
								// TODO: correctly resolve inverses

								if (restExpVarSet->contains(firstVarExp) || restExpVarSet->contains(secondVarExp)) {

									CRole* role = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value((CObjectPropertyTermExpression*)propExp);
									CRole* inverseRole = role->getInverseRole();
									COccurrenceStatisticsRoleData roleOccStatsData = occStatsCacheReader->getAccummulatedRoleDataOccurrenceStatistics(ontologyId, role->getRoleTag());
									++statsTerms;
									if (roleOccStatsData.getDeterministicInstanceOccurrencesCount() <= 0) {
										statsTermsNoDeterministicInstances++;
									}
									if (roleOccStatsData.getIndividualInstanceOccurrencesCount() <= 0) {
										statsTermsNoIndividualInstances++;
										break;
									}
									if (inverseRole) {
										++statsTerms;
										COccurrenceStatisticsRoleData invRoleOccStatsData = occStatsCacheReader->getAccummulatedRoleDataOccurrenceStatistics(ontologyId, inverseRole->getRoleTag());
										if (invRoleOccStatsData.getDeterministicInstanceOccurrencesCount() <= 0) {
											statsTermsNoDeterministicInstances++;
										}
										if (invRoleOccStatsData.getIndividualInstanceOccurrencesCount() <= 0) {
											statsTermsNoIndividualInstances++;
											break;
										}
									}
								}
							}
						}

						QHash<CExpressionVariable*, CBuildExpression*>* varBuiltExpHash = absorpingHanldingQueryPart->getVariableBuiltExpressionsHash();
						for (QHash<CExpressionVariable*, CBuildExpression*>::const_iterator it = varBuiltExpHash->constBegin(), itEnd = varBuiltExpHash->constEnd(); it != itEnd && statsTermsNoIndividualInstances <= 0; ++it) {

							CIndividualVariableExpression* varExp = dynamic_cast<CIndividualVariableExpression*>(it.key());
							CClassTermExpression* classExp = (CClassTermExpression*)it.value();

							if (restExpVarSet->contains(varExp) && dynamic_cast<CClassExpression*>(classExp)) {
								CConcept* concept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value((CClassTermExpression*)classExp);
								if (concept->getOperatorCode() == CCEQ) {
									concept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getTBox()->getEquivalentConceptCandidateHash(false)->value(concept);
								}
								COccurrenceStatisticsConceptData conceptOccStatsData = occStatsCacheReader->getAccummulatedConceptDataOccurrenceStatistics(ontologyId, concept->getConceptTag());
								++statsTerms;
								if (conceptOccStatsData.getDeterministicInstanceOccurrencesCount() <= 0) {
									statsTermsNoDeterministicInstances++;
								}
								if (conceptOccStatsData.getIndividualInstanceOccurrencesCount() <= 0) {
									statsTermsNoIndividualInstances++;
									break;
								}
							}


						}
					}


					ordinaryEvaluationUseful = statsTermsNoIndividualInstances <= 0 && (double)statsTermsNoDeterministicInstances / (double)statsTerms < 0.5;



					if (ordinaryEvaluationUseful) {

						CConcreteOntology* testingOnto = mOntoAnsweringItem->getTestingOntology();
						QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarCompItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();


						cint64 queryyClassTerms = 0;

						QList<CObjectPropertyAssertionExpression*> bothVarPropAssPropagationList;
						QSet<CExpressionVariable*> conceptVarExpSet;
						QHash<CExpressionVariable*, CAxiomExpression*> varExpAxiomExpHash;
						QHash<CExpressionVariable*, CBuildExpression*> varRolledUpClassExpHash;
						for (COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorpingHanldingQueryPart : *absorbingQueryPartsList) {

							QSet<CObjectPropertyAssertionExpression*>* propAssSet = absorpingHanldingQueryPart->getPropertyAssertionSet();
							for (CObjectPropertyAssertionExpression* propAssExp : *propAssSet) {
								CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
								CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getFirstIndividualTermExpression());
								CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getSecondIndividualTermExpression());
								bool restrictedPropAss = false;
								if (!varExpVarCompItemHash->contains(firstVarExp)) {
									restrictedPropAss = true;
								}
								if (!varExpVarCompItemHash->contains(secondVarExp)) {
									restrictedPropAss = true;
								}
								if (restrictedPropAss) {
									bothVarPropAssPropagationList.append(propAssExp);
									if (firstVarExp) {
										if (!varExpVarCompItemHash->contains(firstVarExp)) {
											conceptVarExpSet.insert(firstVarExp);
										}
										varExpAxiomExpHash.insertMulti(firstVarExp, propAssExp);
									}
									if (secondVarExp && secondVarExp != firstVarExp) {
										if (!varExpVarCompItemHash->contains(secondVarExp)) {
											conceptVarExpSet.insert(secondVarExp);
										}
										varExpAxiomExpHash.insertMulti(secondVarExp, propAssExp);
									}
								}
							}

							QHash<CExpressionVariable*, CBuildExpression*>* varBuiltExpHash = absorpingHanldingQueryPart->getVariableBuiltExpressionsHash();
							for (QHash<CExpressionVariable*, CBuildExpression*>::const_iterator it = varBuiltExpHash->constBegin(), itEnd = varBuiltExpHash->constEnd(); it != itEnd && statsTermsNoIndividualInstances <= 0; ++it) {
								CIndividualVariableExpression* varExp = dynamic_cast<CIndividualVariableExpression*>(it.key());
								CClassTermExpression* classExp = (CClassTermExpression*)it.value();
								if (!varExpVarCompItemHash->contains(varExp)) {
									conceptVarExpSet.insert(varExp);
									varRolledUpClassExpHash.insert(varExp, classExp);
									++queryyClassTerms;
								}
							}
						}


						LOG(INFO, getDomain(), logTr("Trying ordinary evaluation with %2 terms for %1 existential parts of Boolean query.").arg(absorbingQueryPartsList->size()).arg(bothVarPropAssPropagationList.size() + queryyClassTerms), this);

						mTestingOntologyBuilder->initializeBuilding();

						// building complex concept expressions for concept items
						QSet<CExpressionVariable*> prepareIndiVarSet;
						QSet<CExpressionVariable*> rolledVarExpSet;
						QSet<CExpressionVariable*> anonymousIndiVariableSet;
						QHash<CExpressionVariable*, CBuildExpression*> individualTriggerUpdatableHash;
						QList<QPair<CIndividualVariableExpression*, CClassTermExpression*>> varClassInstTestExpList;
						varClassInstTestExpList = generateConceptItemExpressions(conceptVarExpSet, rolledVarExpSet, anonymousIndiVariableSet, varRolledUpClassExpHash, varExpAxiomExpHash, individualTriggerUpdatableHash, prepareIndiVarSet, queryProcessingData);


						mTestingOntologyBuilder->completeBuilding();


						QList<QPair<CIndividualVariableExpression*, TConceptNegPair>> varConNegInstTestExpList;
						varConNegInstTestExpList = upgradeExistTopToIndividualTriggers(varClassInstTestExpList, individualTriggerUpdatableHash, answererContext);



						if (mConfDebugTestingOntologyWriting) {
							COntologyTextFormater::writeOntologyToFile(testingOnto, "Debugging/Answering/Absorbed-Query-Expressions.txt");
						}

						mTestingOntologyPreprocessor->preprocess(testingOnto, testingOnto->getConfiguration());


						COptimizedComplexBuildingIndividualVariableCompositionsItem* limitedSubBuildingVarItem = buildingVarItem->createExtendingBuildingVariableCompositionsItem();

						
						bool conceptItemsProcessing = schedulingConceptItemsProcessingForBuildItem(varConNegInstTestExpList, nullptr, limitedSubBuildingVarItem);

						limitedSubBuildingVarItem->addVariablePropertyAssertions(bothVarPropAssPropagationList);
						buildingVarItem->setAbsorptionBasedQueryPartsOrdinaryEvaluationSubVariableBuiltItem(limitedSubBuildingVarItem);




						// todo

						CComputedItemDataNotificationLinker* notLinker = mOntoAnsweringItem->createComputedItemDataNotificationLinker();
						notLinker->setBuildingVariableItemProcessingItem(buildingVarItem);
						limitedSubBuildingVarItem->addNotificationLinker(notLinker);
						buildingVarItem->incWaitingSubVariableBuildingItemCount();

						queryProcessingData->incBuildingVariableItem();
						if (!conceptItemsProcessing) {
							mOntoAnsweringItem->addProcessingVariableBuildingItem(limitedSubBuildingVarItem);
							limitedSubBuildingVarItem->setProcessingQueued(true);
						}
						processing = true;
					}
				}
				return processing;
			}




			static bool itemSortLessVariableMappingsThan(const TRemainingVariableMappingCountSortingItem& item1, const TRemainingVariableMappingCountSortingItem& item2) {
				if (item1.second < item2.second) {
					return true;
				} else if (item1.second == item2.second) {
					return item1.first->getName() < item2.first->getName();
				} else {
					return false;
				}
			}




			static bool itemSortLessObjectPropertyScoresThan(const TRemainingVariableCompositionItemObjectPropertySortingItem& item1, const TRemainingVariableCompositionItemObjectPropertySortingItem& item2) {
				if (item1.second < item2.second) {
					return true;
				} else if (item1.second == item2.second) {
					return item1.first->getObjectPropertyTermExpression() < item2.first->getObjectPropertyTermExpression();
				} else {
					return false;
				}
			}




			QList<CObjectPropertyAssertionExpression*> COptimizedComplexExpressionAnsweringHandler::getVariableRemainingSortedObjectPropertyList(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CIndividualVariableExpression* varExp, COptimizedComplexVariableCompositionItem* varCompItem, const QList<CObjectPropertyAssertionExpression*>& propAssList) {
				QList<CObjectPropertyAssertionExpression*> sortedPropAssList;

				if (mConfNextVariablePropagationHandlingPropertyPriorization) {
					double expectedBindingSize = varCompItem->getVariableMappingsExpectedCount();

					QList<TRemainingVariableCompositionItemObjectPropertySortingItem> sortingItemList;
					for (CObjectPropertyAssertionExpression* propAssExp : propAssList) {

						double propInOutModificationFactor = 1;
						double propTotalScore = 0;
						getVariableCompositionItemPropertyAssertionHandlingScores(buildingVarItem, varExp, varCompItem, propAssExp, expectedBindingSize, propTotalScore, propInOutModificationFactor);


						double propScoredExpectedBindingsSize = (propInOutModificationFactor * mConfNextVariablePropagationHandlingPropertyInOutFactorIntegrationPercent + expectedBindingSize * (100. - mConfNextVariablePropagationHandlingPropertyInOutFactorIntegrationPercent)) / 100.;
						double propTotalScoredExpectedBindingsSize = propScoredExpectedBindingsSize;
						if (propTotalScore != 0.) {
							propTotalScoredExpectedBindingsSize = (propScoredExpectedBindingsSize * (100. - mConfNextVariablePropagationHandlingPropertyTotalScoreIntegrationPercent) + propTotalScore * mConfNextVariablePropagationHandlingPropertyTotalScoreIntegrationPercent) / 100.;
						}

						sortingItemList.append(TRemainingVariableCompositionItemObjectPropertySortingItem(propAssExp, propTotalScoredExpectedBindingsSize));
					}
					qSort(sortingItemList.begin(), sortingItemList.end(), itemSortLessObjectPropertyScoresThan);
					for (TRemainingVariableCompositionItemObjectPropertySortingItem remVarSortItem : sortingItemList) {
						sortedPropAssList.append(remVarSortItem.first);
					}
				} else {
					sortedPropAssList = propAssList;
				}
				return sortedPropAssList;
			}



			QList<CIndividualVariableExpression*> COptimizedComplexExpressionAnsweringHandler::getSortedRemainingVariableExpressionList(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, QSet<CIndividualVariableExpression*>* remainVarExpSet) {
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
				QList<TRemainingVariableMappingCountSortingItem> remainVarExpSortList;


				auto remainVarOtherVarsAssociatedFunc = [&](CExpressionVariable* remainVarExp, COptimizedComplexVariableCompositionItem* joiningVarItem)->QSet<CExpressionVariable*> {
					QSet<CExpressionVariable*> varsAssociatedSet;
					varsAssociatedSet.insert(remainVarExp);
					COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(remainVarExp);
					COptimizedComplexVariableCompositionItemVariableIndexMapping* itemMapping = varItemIndexMapping.value(joiningVarItem);
					if (itemMapping) {
						for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemMapping->constBegin(), itEnd = itemMapping->constEnd(); it != itEnd; ++it) {
							CExpressionVariable* otherAssocExpVar = it.value();
							varsAssociatedSet.insert(remainVarExp);
						}
					}
					return varsAssociatedSet;
				};


				auto remainVarOtherVarsAssociatedItemSetFunc = [&](CExpressionVariable* remainVarExp, QSet<COptimizedComplexVariableCompositionItem*>* joiningVarItemSet)->QSet<CExpressionVariable*> {
					QSet<CExpressionVariable*> remainVarOtherVarsAssociatedSet;
					remainVarOtherVarsAssociatedSet.insert(remainVarExp);
					for (COptimizedComplexVariableCompositionItem* joiningVarItem : *joiningVarItemSet) {
						COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(remainVarExp);
						COptimizedComplexVariableCompositionItemVariableIndexMapping* itemMapping = varItemIndexMapping.value(joiningVarItem);
						if (itemMapping) {
							for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemMapping->constBegin(), itEnd = itemMapping->constEnd(); it != itEnd; ++it) {
								CExpressionVariable* otherAssocExpVar = it.value();
								remainVarOtherVarsAssociatedSet.insert(remainVarExp);
							}
						}
					}
					return remainVarOtherVarsAssociatedSet;
				};

				QHash<CExpressionVariable*, cint64> varsAssociatedCountHash;
				for (CIndividualVariableExpression* remainVarExp : *remainVarExpSet) {
					QSet<COptimizedComplexVariableCompositionItem*>* joiningVarItemSet = varExpVarComItemHash->value(remainVarExp);

					if (joiningVarItemSet) {
						QSet<CExpressionVariable*> remainVarOtherVarsAssociatedSet = remainVarOtherVarsAssociatedItemSetFunc(remainVarExp, joiningVarItemSet);
						for (CExpressionVariable* assocVar : remainVarOtherVarsAssociatedSet) {
							varsAssociatedCountHash[assocVar]++;
						}
					}
				}


				
				for (CIndividualVariableExpression* remainVarExp : *remainVarExpSet) {
					QSet<COptimizedComplexVariableCompositionItem*>* joiningVarItemSet = varExpVarComItemHash->value(remainVarExp);

					if (joiningVarItemSet) {
						QSet<CExpressionVariable*> remainVarOtherVarsAssociatedSet = remainVarOtherVarsAssociatedItemSetFunc(remainVarExp, joiningVarItemSet);
						
						double minExpectedBindingSize = DOUBLE_MAX;
						COptimizedComplexVariableCompositionItem* minJoiningVarItem = nullptr;
						for (COptimizedComplexVariableCompositionItem* joiningVarItem : *joiningVarItemSet) {
							if (joiningVarItem->getVariableMappingsExpectedCount() < minExpectedBindingSize) {
								minExpectedBindingSize = joiningVarItem->getVariableMappingsExpectedCount();
								minJoiningVarItem = joiningVarItem;
							}
						}

						// TODO: maybe integrate other items in the estimation (but on the ohter hand, they are joined anyway first and then the estimation is recomputed)
						//if (joiningVarItemSet->size() > 1 && minJoiningVarItem) {
						//	QSet<CExpressionVariable*> minVarsAssociatedSet = remainVarOtherVarsAssociatedFunc(remainVarExp, minJoiningVarItem);
						//	for (COptimizedComplexVariableCompositionItem* joiningVarItem : *joiningVarItemSet) {
						//		if (joiningVarItem != minJoiningVarItem) {
						//			cint64 minVarCount = minVarsAssociatedSet.size();
						//			QSet<CExpressionVariable*> varsAssociatedSet = remainVarOtherVarsAssociatedFunc(remainVarExp, joiningVarItem);
						//			cint64 newVarCount = 0;
						//			cint64 missVarCount = 0;
						//			for (CExpressionVariable* assocVar : varsAssociatedSet) {
						//				if (!minVarsAssociatedSet.contains(assocVar)) {
						//					++newVarCount;
						//				}
						//			}
						//			for (CExpressionVariable* assocVar : minVarsAssociatedSet) {
						//				if (!varsAssociatedSet.contains(assocVar)) {
						//					++missVarCount;
						//				}
						//			}
						//			cint64 changeVarCount = newVarCount - missVarCount;
						//		}
						//	}
						//}



						double expectedBindingsModificationFactor = 1.;
						QList<CObjectPropertyAssertionExpression*> propAssList;
						if (mConfNextVariablePropagationHandlingReducibleFactorIntegration || mConfNextVariablePropagationHandlingPropertyScoresIntegration) {
							propAssList = buildingVarItem->getUnhanledPropertyAssertionsExpressions(remainVarExp);
						}

						if (mConfNextVariablePropagationHandlingReducibleFactorIntegration) {
							// determine whether only one object property remains
							if (propAssList.size() == 1) {
								// determine the reducible variables after the propagation
								cint64 reducibleVarCount = 0;

								for (CExpressionVariable* assocVar : remainVarOtherVarsAssociatedSet) {
									cint64 varOccCount = varsAssociatedCountHash.value(assocVar);
									if (varOccCount <= 1 && !buildingVarItem->getReductionDeniedVariableSet()->contains(assocVar)) {
										reducibleVarCount++;
									}
								}


								if (reducibleVarCount >= 1) {
									expectedBindingsModificationFactor *= pow(mConfNextVariablePropagationHandlingReducibleFactor, (double)reducibleVarCount);
								}
							}
						}


						double propInOutModificationFactor = 1.;
						double propTotalScore = 0.;
						if (mConfNextVariablePropagationHandlingPropertyScoresIntegration) {
							for (CObjectPropertyAssertionExpression* propAssExp : propAssList) {

								getVariableCompositionItemPropertyAssertionHandlingScores(buildingVarItem, remainVarExp, minJoiningVarItem, propAssExp, minExpectedBindingSize, propInOutModificationFactor, propTotalScore);

							}
						}

						double propScoredExpectedBindingsSize = (propInOutModificationFactor * mConfNextVariablePropagationHandlingPropertyInOutFactorIntegrationPercent + minExpectedBindingSize * (100. - mConfNextVariablePropagationHandlingPropertyInOutFactorIntegrationPercent)) / 100.;
						double propTotalScoredExpectedBindingsSize = propScoredExpectedBindingsSize;
						if (propTotalScore != 0.) {
							propTotalScoredExpectedBindingsSize = (propScoredExpectedBindingsSize * (100. - mConfNextVariablePropagationHandlingPropertyTotalScoreIntegrationPercent) + propTotalScore * mConfNextVariablePropagationHandlingPropertyTotalScoreIntegrationPercent) / 100.;
						}
						double expectedBindingsSize = propTotalScoredExpectedBindingsSize * expectedBindingsModificationFactor;


						TRemainingVariableMappingCountSortingItem remVarSortItem(remainVarExp, expectedBindingsSize);
						remainVarExpSortList.append(remVarSortItem);
					}
				}





				qSort(remainVarExpSortList.begin(), remainVarExpSortList.end(), itemSortLessVariableMappingsThan);

				QStringList remainingVarStringList;

				QList<CIndividualVariableExpression*> remainVarExpList;
				for (TRemainingVariableMappingCountSortingItem remVarSortItem : remainVarExpSortList) {
					remainVarExpList.append(remVarSortItem.first);

					if (mConfExtendedLogging) {
						QString expectedCoundString;
						expectedCoundString.setNum(remVarSortItem.second, 'f', 1);
						remainingVarStringList.append(QString("%1 with %2 expected mappings").arg(remVarSortItem.first->getName()).arg(expectedCoundString));
					}
				}

				if (mConfExtendedLogging) {
					LOG(INFO, getDomain(), logTr("Continue propagation for remaining variables: %1.").arg(remainingVarStringList.join(", ")), this);
				}

				return remainVarExpList;
			}



			void COptimizedComplexExpressionAnsweringHandler::getVariableCompositionItemPropertyAssertionHandlingScores(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CIndividualVariableExpression* remainVarExp, COptimizedComplexVariableCompositionItem* minJoiningVarItem, CObjectPropertyAssertionExpression* propAssExp, double minExpectedBindingSize, double &propInOutModificationFactor, double &propTotalScore) {

				COccurrenceStatisticsCacheReader* occStatsCacheReader = mAnswererContext->getAnsweringCalculationHandler()->getOccurrenceStatisticsCacheReader();
				CConcreteOntology* ontology = mOntoAnsweringItem->getOntology();
				cint64 ontologyId = ontology->getOntologyID();
				CRole* role = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value((CObjectPropertyTermExpression*)propAssExp->getObjectPropertyTermExpression());
				CRole* outRole = role;
				CRole* inRole = role->getInverseRole();
				CIndividualVariableExpression* otherVarExp = (CIndividualVariableExpression*)propAssExp->getSecondIndividualTermExpression();
				if (propAssExp->getFirstIndividualTermExpression() != remainVarExp) {
					outRole = inRole;
					inRole = role;
					otherVarExp = (CIndividualVariableExpression*)propAssExp->getFirstIndividualTermExpression();
				}
				if (role) {
					//if (CIRIName::getRecentIRIName(role->getPropertyNameLinker()) == "http://semantics.crl.ibm.com/univ-bench-dl.owl#like") {
					//	hasLikeRole = true;
					//}

					double complexFactor = 1.;
					if (role->isComplexRole()) {
						complexFactor = 2.5;
					}

					COccurrenceStatisticsRoleData roleOccStatsData = occStatsCacheReader->getAccummulatedRoleDataOccurrenceStatistics(ontologyId, role->getRoleTag());
					double roleOccFactor = complexFactor * (roleOccStatsData.getDeterministicInstanceOccurrencesCount() + roleOccStatsData.getNonDeterministicInstanceOccurrencesCount() * 2 + roleOccStatsData.getExistentialInstanceOccurrencesCount() * 1.5 + roleOccStatsData.getIndividualInstanceOccurrencesCount());
					propTotalScore += roleOccFactor;
				}

				double inOutRatio = 1;
				if (outRole && inRole) {
					COccurrenceStatisticsRoleData outRoleOccStatsData = occStatsCacheReader->getAccummulatedRoleDataOccurrenceStatistics(ontologyId, outRole->getRoleTag());
					COccurrenceStatisticsRoleData inRoleOccStatsData = occStatsCacheReader->getAccummulatedRoleDataOccurrenceStatistics(ontologyId, inRole->getRoleTag());
					cint64 inRoleInstCount = inRoleOccStatsData.getOutgoingNodeInstanceOccurrencesCount();
					cint64 outRoleInstCount = outRoleOccStatsData.getOutgoingNodeInstanceOccurrencesCount();

					if (!mConfForceAlwaysVariableCompositionItemRoleSampling && outRoleInstCount != inRoleInstCount && inRoleInstCount != 0) {
						inOutRatio = (double)inRoleInstCount / (double)outRoleInstCount;
					} else if (mConfForceAlwaysVariableCompositionItemRoleSampling || mConfVariableCompositionItemRoleSampling) {
						inOutRatio = getVariableCompositionItemRoleSampledInOutRatio(buildingVarItem, remainVarExp, otherVarExp, minJoiningVarItem, outRole, inRole, mAnswererContext);
					}

					propInOutModificationFactor += minExpectedBindingSize * inOutRatio;
				}
			}



			cint64 COptimizedComplexExpressionAnsweringHandler::getVariableCompositionItemVariablePosition(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp, COptimizedComplexVariableCompositionItem* varItem) {
				cint64 propVarPos = -1;
				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(varExp);
				COptimizedComplexVariableCompositionItemVariableIndexMapping* itemMapping = varItemIndexMapping.value(varItem);
				if (itemMapping) {
					for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemMapping->constBegin(), itEnd = itemMapping->constEnd(); it != itEnd; ++it) {
						CExpressionVariable* otherAssocExpVar = it.value();
						if (otherAssocExpVar == varExp) {
							propVarPos = it.key();
							break;
						}
					}
				}
				return propVarPos;
			}



			double COptimizedComplexExpressionAnsweringHandler::getVariableCompositionItemRoleSampledInOutRatio(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp, CExpressionVariable* otherVarExp, COptimizedComplexVariableCompositionItem* varItem, CRole* outRole, CRole* inRole, CAnswererContext* answererContext) {
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
				QSet<COptimizedComplexVariableCompositionItem*>* otherVarItemSet = varExpVarComItemHash->value(otherVarExp);
				if (otherVarItemSet) {
					double minExpectedBindingSize = DOUBLE_MAX;
					COptimizedComplexVariableCompositionItem* minOtherVarItem = nullptr;
					for (COptimizedComplexVariableCompositionItem* otherVarItem : *otherVarItemSet) {
						if (otherVarItem->getVariableMappingsExpectedCount() < minExpectedBindingSize) {
							minExpectedBindingSize = otherVarItem->getVariableMappingsExpectedCount();
							minOtherVarItem = otherVarItem;
						}
					}
					if (minOtherVarItem) {
						double sampledInCountEstimation = getVariableCompositionItemRoleAverageSampledOutCount(buildingVarItem, varExp, varItem, outRole, answererContext);
						return sampledInCountEstimation;
						//double sampledOutCountEstimation = getVariableCompositionItemRoleAverageSampledOutCount(buildingVarItem, otherVarExp, minOtherVarItem, inRole, answererContext);
						//double inOutRatio = (double)sampledInCountEstimation / (double)sampledOutCountEstimation;
						//return inOutRatio;
					}
				}
				return 1.;

			}



			double COptimizedComplexExpressionAnsweringHandler::getVariableCompositionItemRoleAverageSampledOutCount(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp, COptimizedComplexVariableCompositionItem* varItem, CRole* role, CAnswererContext* answererContext) {
				CRoleRealization* roleRealization = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();
				CRoleInstantiatedItem* roleInstancesItem = roleRealization->getRoleInstantiatedItem(role);


				cint64 variableCompositionItemRoleSamplingCount = mConfVariableCompositionItemRoleSamplingCount;
				cint64 expectedCount = varItem->getVariableMappingsExpectedCount();
				if (expectedCount > mConfVariableCompositionItemRoleSamplingMappingsIncreaseSize) {
					variableCompositionItemRoleSamplingCount *= expectedCount / mConfVariableCompositionItemRoleSamplingMappingsIncreaseSize;
				}
				COptimizedComplexVariableCompositionItemRoleSamplingData& roleSamplingData = varItem->getRoleSamplingData(role);
				if (roleSamplingData.getSampleCount() < variableCompositionItemRoleSamplingCount && varItem->getVariableMappingsCurrentCount() > roleSamplingData.getSampleCount()) {

					cint64 propVarPos = getVariableCompositionItemVariablePosition(buildingVarItem, varExp, varItem);


					cint64 averageCount = 0;
					double averageEstimatedCount = 0;

					auto averageBindingsEstimate = [&](COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindCardLinker) {
						CRealizationIndividualInstanceItemReference propInstItemRef = bindCardLinker->getBindings()->getBinding(propVarPos).reference;
						CRealizationIndividualInstanceItemReferenceIterator* iterator = roleRealization->getRoleFillerInstancesIterator(propInstItemRef, roleInstancesItem, false, mRoleFillerRealizationSorting);
						iterator->begin();
						double estimatedCount = iterator->remainingInstancesEstimation().getEstimatedCount();
						averageEstimatedCount += estimatedCount;
						averageCount++;
						delete iterator;
					};

					COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* bindCardBatchLinker = varItem->getVariableMapping()->getFirstAddedBindingsCardinalityBatchLinker();
					COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindCardLinker = varItem->getVariableMapping()->getLastAddedBindingsCardinalityLinker();
					if (bindCardLinker && bindCardBatchLinker) {
						bindCardLinker = bindCardLinker->getNext();
					}
					while (averageCount < variableCompositionItemRoleSamplingCount && (bindCardLinker || bindCardBatchLinker)) {
						if (bindCardBatchLinker) {
							COptimizedComplexVariableIndividualBindingsCardinalityLinker* batchBindCardLinker = bindCardBatchLinker->getStartBindingsCardinalityLinker();
							if (batchBindCardLinker) {
								averageBindingsEstimate(batchBindCardLinker);
							}
							bindCardBatchLinker = bindCardBatchLinker->getNext();
						} else if (bindCardLinker) {
							averageBindingsEstimate(bindCardLinker);
							bindCardLinker = bindCardLinker->getNext();
						}
					}
					if (averageCount != 0) {
						averageEstimatedCount /= averageCount;
					}
					roleSamplingData.updateSamplingData(averageCount, averageEstimatedCount);
				}
				return roleSamplingData.getSampleEstimatedRoleSuccessorCount();
			}





			bool COptimizedComplexExpressionAnsweringHandler::processExtractedInstanceCandidatePossiblePropagations(CAnsweringMessageDataInstancePossiblePropagationsData* instanceCandidatePropagationMessage, CAnswererContext* answererContext) {
				CConcept* concept = instanceCandidatePropagationMessage->getTestingConcept();
				bool negation = instanceCandidatePropagationMessage->getTestingConceptNegation();
				COptimizedComplexConceptItem* conceptItem = mOntoAnsweringItem->getComplexConceptItem(concept, negation);
				CCLASSSUBSUMPTIONMESSAGELIST<CIndividualReference>* indiList = instanceCandidatePropagationMessage->getIndividualReferenceList();

				QSet<CRealizationIndividualInstanceItemReference>* possibleInstanceItemSet = conceptItem->getPossibleInstanceItemSet();
				QSet<CRealizationIndividualInstanceItemReference>* possibleInstanceCandPropItemSet = conceptItem->getPossibleInstanceCandidatePropagationItemSet();
				//mTestedPossibleInstancesCount += possibleInstanceItemSet->size();
				//delete possibleInstanceItemSet;
				//conceptItem->setPossibleInstanceItemSet(nullptr);
				//conceptItem->incPossibleTestedNonInstanceCount(possibleInstanceItemSet->size());
				if (indiList && !indiList->isEmpty()) {
					CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();
					for (CCLASSSUBSUMPTIONMESSAGELIST<CIndividualReference>::const_iterator it = indiList->constBegin(), itEnd = indiList->constEnd(); it != itEnd; ++it) {
						CRealizationIndividualInstanceItemReference indiItemRef = sameRealization->getSameInstanceItemReference(*it);
						if (possibleInstanceCandPropItemSet->contains(indiItemRef)) {
							possibleInstanceItemSet->insert(indiItemRef);
						}
						//mTestedPossibleInstancesCount++;
					}
					//conceptItem->incPossibleTestedNonInstanceCount(newPossibleInstanceItemSet->size() - possibleInstanceItemSet->size());
				}
				conceptItem->setCandidateReceived(true);
				return true;
			}



			bool COptimizedComplexExpressionAnsweringHandler::processExtractedInstanceCertainPropagations(CAnsweringMessageDataInstanceCertainPropagationsData* instanceCandidatePropagationMessage, CAnswererContext* answererContext) {

				CConcreteOntology* testingOnto = mOntoAnsweringItem->getTestingOntology(false);
				CConcept* concept = instanceCandidatePropagationMessage->getTestingConcept();
				bool negation = instanceCandidatePropagationMessage->getTestingConceptNegation();
				CConcept* propagatedConcept = instanceCandidatePropagationMessage->getPropagatedConcept();
				COptimizedComplexConceptItem* conceptItem = mOntoAnsweringItem->getComplexConceptItem(concept, negation);
				CCLASSSUBSUMPTIONMESSAGELIST<CIndividualReference>* indiList = instanceCandidatePropagationMessage->getIndividualReferenceList();

				QSet<CRealizationIndividualInstanceItemReference>* possibleInstanceItemSet = conceptItem->getPossibleInstanceItemSet();
				QSet<CRealizationIndividualInstanceItemReference>* possibleInstanceCandPropItemSet = conceptItem->getPossibleInstanceCandidatePropagationItemSet();
				COptimizedComplexConceptInstanziatedIndividualItemHash* directInstanceItems = conceptItem->getKnownInstanceItems();

				if (indiList && !indiList->isEmpty()) {
					CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();
					for (CCLASSSUBSUMPTIONMESSAGELIST<CIndividualReference>::const_iterator it = indiList->constBegin(), itEnd = indiList->constEnd(); it != itEnd; ++it) {
						CRealizationIndividualInstanceItemReference indiItemRef = sameRealization->getSameInstanceItemReference(*it);
						//mTestedPossibleInstancesCount++;
						if (possibleInstanceCandPropItemSet->contains(indiItemRef)) {
							directInstanceItems->addRealizationIndividualInstanceItemReference(indiItemRef);
						}
					}
				}
				return true;
			}






			bool COptimizedComplexExpressionAnsweringHandler::visitAllIndividuals(function<bool(const CIndividualReference& indiRef)> visitFunc) {

				CConcreteOntology* ontology = mOntoAnsweringItem->getOntology();

				CBOXSET<CIndividual*>* activeIndiSet = ontology->getABox()->getActiveIndividualSet();
				CIndividualVector* indiVec = ontology->getABox()->getIndividualVector(false);
				cint64 indiCount = 0;
				if (indiVec) {
					indiCount = indiVec->getItemCount();
				}

				bool visited = false;
				bool continueVisiting = true;
				cint64 maxTriplesIndexedIndiId = 0;
				COntologyTriplesAssertionsAccessor* triplesAccessor = ontology->getOntologyTriplesData()->getTripleAssertionAccessor();
				if (triplesAccessor) {
					maxTriplesIndexedIndiId = ontology->getOntologyTriplesData()->getTripleAssertionAccessor()->getMaxIndexedIndividualId();
				}
				if (indiVec) {
					cint64 indiCount = indiVec->getItemCount();
					for (cint64 idx = 0; idx < indiCount && continueVisiting; ++idx) {
						CIndividual* indi = indiVec->getData(idx);

						CIndividualReference indiRef;
						if (indi && activeIndiSet->contains(indi)) {
							indiRef = CIndividualReference(indi);
							visited = true;
						} else if (idx <= maxTriplesIndexedIndiId) {
							visited = true;
							indiRef = CIndividualReference(idx);
						}
						continueVisiting = visitFunc(indiRef);						

					}
				}
				return visited;
			}





			bool COptimizedComplexExpressionAnsweringHandler::createAbsorbedQueryPartEntailmentTest(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionHanldingQueryPart, CComplexQueryExpressionProcessingData* procData, CAnswererContext* answererContext) {
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CExpressionVariable* initializerVariable = absorptionHanldingQueryPart->getInitializerVariableExpression();
				CVariable* variable = absorptionHanldingQueryPart->getVariableExpressionVariableHash()->value(initializerVariable);
				CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());

				CConcept* restrictedTopPropConcept = nullptr;
				bool splitting = false;
				if (!splitting && mConfQueryEntailmentTestRestrictedTopPropertyPropagationSplitting && mConfMaxVariableBindingPropagationsInitializationIndividualCount > 0 && absorptionHanldingQueryPart->isTopObjectPropertyInitializationPropagation()) {
					restrictedTopPropConcept = getAbsorbExistentialQueryPartRestrictedTopPropagationConcept(absorptionHanldingQueryPart, answererContext);
					splitting = true;
				}

				if (!splitting) {
					splitting = !absorptionHanldingQueryPart->isTopObjectPropertyUsed() && mConfMaxVariableBindingPropagationsInitializationIndividualCount > 0;
				}

				if (!splitting) {
					satCalcJob = extendProcessingByTopPropagation(satCalcJobGen, satCalcJob, absorptionHanldingQueryPart->isTopObjectPropertyUsed(), answererContext);

					procData->incEntailmentComputation();
					visitAllIndividuals([&](const CIndividualReference& indiRef)->bool {
						satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(absorptionHanldingQueryPart->getInitializerConcept(), false, indiRef, satCalcJob);
						return true;
					});

					CAnsweringPropagationSteeringAbsorptionEntailmentController* propagationSteeringController = new CAnsweringPropagationSteeringAbsorptionEntailmentController(restrictedTopPropConcept);
					CAnsweringMessageDataCalculationCompletedVariableBindingEntailment* completedMessage = new CAnsweringMessageDataCalculationCompletedVariableBindingEntailment(satCalcJob, nullptr, absorptionHanldingQueryPart, procData, nullptr, nullptr);
					satCalcJob->setSatisfiableAnswererBindingPropagationAdapter(new CSatisfiableTaskAnswererBindingPropagationAdapter(absorptionHanldingQueryPart->getInitializerConcept(), absorptionHanldingQueryPart->getPropagationFinalizationConcept(), this, mOntoAnsweringItem->getTestingOntology(false), answererContext->getAnsweringCalculationHandler(), propagationSteeringController));
					mOntoAnsweringItem->getAnsweringHandlingStatistics()->incExistentialQueryPartEntailmentTestingCount();
					processCalculationJob(answererContext, satCalcJob, completedMessage);
				} else {

					// TODO: make sure that the absorption does not create a propagation over top object property/role such that all individuals have to be expanded
					CAnsweringSplitEntailmentTestingCancellationAdapter* cancelAdapter = new CAnsweringSplitEntailmentTestingCancellationAdapter();
					cint64 currentCount = 0;
					cint64 totalCount = 0;
					visitAllIndividuals([&](const CIndividualReference& indiRef)->bool {
						satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(absorptionHanldingQueryPart->getInitializerConcept(), false, indiRef, satCalcJob);
						totalCount++;
						if (++currentCount >= mConfMaxVariableBindingPropagationsInitializationIndividualCount) {
							currentCount = 0;
							procData->incEntailmentComputation();
							CAnsweringPropagationSteeringAbsorptionEntailmentController* propagationSteeringController = new CAnsweringPropagationSteeringAbsorptionEntailmentController(restrictedTopPropConcept);
							CAnsweringMessageDataCalculationCompletedVariableBindingEntailment* completedMessage = new CAnsweringMessageDataCalculationCompletedVariableBindingEntailment(satCalcJob, nullptr, absorptionHanldingQueryPart, procData, nullptr, cancelAdapter);
							satCalcJob->setSatisfiableAnswererBindingPropagationAdapter(new CSatisfiableTaskAnswererBindingPropagationAdapter(absorptionHanldingQueryPart->getInitializerConcept(), absorptionHanldingQueryPart->getPropagationFinalizationConcept(), this, mOntoAnsweringItem->getTestingOntology(false), answererContext->getAnsweringCalculationHandler(), propagationSteeringController));
							satCalcJob->setCancellationAdapter(cancelAdapter->incCurrentEntailmentTestingCount());
							mOntoAnsweringItem->getAnsweringHandlingStatistics()->incExistentialQueryPartEntailmentTestingCount();
							processCalculationJob(answererContext, satCalcJob, completedMessage);

						}
						return true;
					});

					if (satCalcJob) {
						procData->incEntailmentComputation();
						CAnsweringPropagationSteeringAbsorptionEntailmentController* propagationSteeringController = new CAnsweringPropagationSteeringAbsorptionEntailmentController(restrictedTopPropConcept);
						CAnsweringMessageDataCalculationCompletedVariableBindingEntailment* completedMessage = new CAnsweringMessageDataCalculationCompletedVariableBindingEntailment(satCalcJob, nullptr, absorptionHanldingQueryPart, procData, nullptr, cancelAdapter);
						satCalcJob->setCancellationAdapter(cancelAdapter->incCurrentEntailmentTestingCount());
						satCalcJob->setSatisfiableAnswererBindingPropagationAdapter(new CSatisfiableTaskAnswererBindingPropagationAdapter(absorptionHanldingQueryPart->getInitializerConcept(), absorptionHanldingQueryPart->getPropagationFinalizationConcept(), this, mOntoAnsweringItem->getTestingOntology(false), answererContext->getAnsweringCalculationHandler(), propagationSteeringController));
						mOntoAnsweringItem->getAnsweringHandlingStatistics()->incExistentialQueryPartEntailmentTestingCount();
						processCalculationJob(answererContext, satCalcJob, completedMessage);
					}

					if (mConfExtendedLogging) {
						LOG(INFO, getDomain(), logTr("Scheduled %1 entailment tests for absorbed query part with %2 initialization individuals in average.").arg(cancelAdapter->getCurrentEntailmentTestingCount()).arg(totalCount / cancelAdapter->getCurrentEntailmentTestingCount()), this);
					}


				}
				return true;
			}







			bool COptimizedComplexExpressionAnsweringHandler::createVariableBindingConfirmationTest(COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationExtension, COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableIndividualBindingsCardinalityLinker* testingVarIndiBindingCardLinker, CSameRealization* sameRealization, CAnswererContext* answererContext) {
				COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionHanldingQueryPart = absorptionPropagationExtension->getAbsorptionBasedHandlingData();
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				buildingVarItem->incVariableBindingsConfirmationCount();
				CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());

				CVariable* initializingVariable = absorptionHanldingQueryPart->getVariableExpressionVariableHash()->value(absorptionHanldingQueryPart->getInitializerVariableExpression());
				CRealizationIndividualInstanceItemReference instanceItemRef = testingVarIndiBindingCardLinker->getBindings()->getBinding(absorptionPropagationExtension->getVariableIndexHash()->value(initializingVariable)).reference;
				QList<CIndividualReference> sameIndiList;
				CInstanceBindingIndividualCollectionVisitor visitor(&sameIndiList);
				sameRealization->visitSameIndividuals(instanceItemRef, &visitor);
				CIndividualReference individualRef = sameIndiList.first();

				satCalcJob = extendProcessingByTopPropagation(satCalcJobGen, satCalcJob, absorptionHanldingQueryPart->isTopObjectPropertyUsed(), answererContext);
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(absorptionHanldingQueryPart->getInitializerConcept(), false, individualRef, satCalcJob);
				CAnsweringMessageDataCalculationCompletedVariableBindingConfirmation* completedMessage = new CAnsweringMessageDataCalculationCompletedVariableBindingConfirmation(satCalcJob, absorptionPropagationExtension, buildingVarItem, testingVarIndiBindingCardLinker);
				CAnsweringPropagationSteeringAbsorptionExtensionItemBindingController* propagationSteeringController = new CAnsweringPropagationSteeringAbsorptionExtensionItemBindingController(absorptionPropagationExtension, *testingVarIndiBindingCardLinker->getBindings(), sameRealization);
				satCalcJob->setSatisfiableAnswererBindingPropagationAdapter(new CSatisfiableTaskAnswererBindingPropagationAdapter(absorptionHanldingQueryPart->getInitializerConcept(), absorptionHanldingQueryPart->getPropagationFinalizationConcept(), this, mOntoAnsweringItem->getTestingOntology(false), answererContext->getAnsweringCalculationHandler(), propagationSteeringController));
				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incExistentialQueryPartVariableBindingConfirmationCount();
				processCalculationJob(answererContext, satCalcJob, completedMessage);
				return true;
			}







			bool COptimizedComplexExpressionAnsweringHandler::continueCalculationJobFromNondeterministicCachedGraph(CSatisfiableCalculationJob* satCalcJob, CAnswererContext* answererContext) {
				if (mConfNonDeterministicSatisfiableCalculationContinuation) {
					satCalcJob->setAllowNonDeterministicSatisfiableCalculationContinuation(true);
					return true;
				}
				return false;
			}



			bool COptimizedComplexExpressionAnsweringHandler::createAbsorbedQueryPartEntailmentTest(COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationExtension, COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext, bool &continueProcessingBuildingVarItem) {

				CComplexQueryExpressionProcessingData* queryProcData = buildingVarItem->getQueryProcessingData();
				if (absorptionPropagationExtension->isEntailmentFound()) {
					queryProcData->decBuildingVariableItem();
					absorptionPropagationExtension->setPropagationInitializationIndividualsSplittedTestsRemaining(false);
					if (!queryProcData->hasComputation() && !absorptionPropagationExtension->hasRemainingPropagationInitializationIndividualsSplittedTests()) {
						finishQueryProcessing(queryProcData);
					}
					return false;
				}


				buildingVarItem->getQueryProcessingData()->incEntailmentComputation();
				COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionHanldingQueryPart = absorptionPropagationExtension->getAbsorptionBasedHandlingData();
				CSatisfiableCalculationJob* satCalcJob = nullptr;



				buildingVarItem->incVariableBindingsPropagationCount();
				CExpressionVariable* initializerVariable = absorptionHanldingQueryPart->getInitializerVariableExpression();
				if (!absorptionHanldingQueryPart->getPrepareVariableSet()->contains(initializerVariable)) {
					initializerVariable = *absorptionHanldingQueryPart->getPrepareVariableSet()->begin();
				}
				CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());
				CVariable* variable = absorptionHanldingQueryPart->getVariableExpressionVariableHash()->value(initializerVariable);
				satCalcJob = extendProcessingByTopPropagation(satCalcJobGen, satCalcJob, absorptionHanldingQueryPart->isTopObjectPropertyUsed(), answererContext);
				QSet<CIndividualReference>* indiSet = absorptionPropagationExtension->getVariableSteeringIndividualBindingSet(variable);

				QSet<CIndividualReference>* splitIndiSet = nullptr;
				if (!absorptionHanldingQueryPart->isTopObjectPropertyUsed() && (absorptionPropagationExtension->isPropagationInitializationIndividualsSplitted() || indiSet->size() > mConfMaxVariableBindingPropagationsInitializationIndividualCount && mConfMaxVariableBindingPropagationsInitializationIndividualCount > 0)) {
					splitIndiSet = new QSet<CIndividualReference>();
					absorptionPropagationExtension->setPropagationInitializationIndividualsSplitted(true);
				}
				for (QSet<CIndividualReference>::iterator it = indiSet->begin(); it != indiSet->end() && (mConfMaxVariableBindingPropagationsInitializationIndividualCount < 0 || !splitIndiSet || splitIndiSet->size() < mConfMaxVariableBindingPropagationsInitializationIndividualCount); ) {
					CIndividualReference individual(*it);
					satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(absorptionHanldingQueryPart->getInitializerConcept(), false, individual, satCalcJob);
					if (splitIndiSet) {
						splitIndiSet->insert(individual);
						it = indiSet->erase(it);
					} else {
						++it;
					}
				}
				if (splitIndiSet) {
					if (mConfExtendedLogging) {
						LOG(INFO, getDomain(), logTr("%1. entailment test for absorbed query part based on %2 bindings scheduled.").arg(absorptionPropagationExtension->getNextPropagationInitializationIndividualsSplittedTestId()).arg(splitIndiSet->size()), this);
					}
					if (!indiSet->isEmpty()) {
						// put into queue such that remaining propagation tests are scheduled at some point
						if (!absorptionPropagationExtension->isSplitPropagationItemProcessingQueued()) {
							buildingVarItem->addAbsorptionBasedHandlingExtensionPropagationItem(absorptionPropagationExtension);
							absorptionPropagationExtension->setSplitPropagationItemProcessingQueued(true);
						}
						absorptionPropagationExtension->setPropagationInitializationIndividualsSplittedTestsRemaining(true);
						continueProcessingBuildingVarItem = true;
					} else {
						queryProcData->decBuildingVariableItem();
						absorptionPropagationExtension->setPropagationInitializationIndividualsSplittedTestsRemaining(false);
					}
				}



				CAnsweringPropagationSteeringAbsorptionExtensionItemController* propagationSteeringController = new CAnsweringPropagationSteeringAbsorptionExtensionItemController(absorptionPropagationExtension, variable, splitIndiSet);
				CAnsweringSplitEntailmentTestingCancellationAdapter* cancelAdapter = nullptr;
				if (splitIndiSet) {
					cancelAdapter = absorptionPropagationExtension->getEntailmentTestingCancellationAdapter();
					cancelAdapter->incCurrentEntailmentTestingCount();
					satCalcJob->setCancellationAdapter(cancelAdapter);
				}
				absorptionPropagationExtension->addPropagationSteeringController(propagationSteeringController);
				CAnsweringMessageDataCalculationCompletedVariableBindingEntailment* completedMessage = new CAnsweringMessageDataCalculationCompletedVariableBindingEntailment(satCalcJob, propagationSteeringController, absorptionHanldingQueryPart, buildingVarItem->getQueryProcessingData(), absorptionPropagationExtension, cancelAdapter);
				satCalcJob->setSatisfiableAnswererBindingPropagationAdapter(new CSatisfiableTaskAnswererBindingPropagationAdapter(absorptionHanldingQueryPart->getInitializerConcept(), absorptionHanldingQueryPart->getPropagationFinalizationConcept(), this, mOntoAnsweringItem->getTestingOntology(false), answererContext->getAnsweringCalculationHandler(), propagationSteeringController));
				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incExistentialQueryPartEntailmentTestingCount();
				if (!absorptionHanldingQueryPart->hasNonTrivialAbsorptionPropagation()) {
					continueCalculationJobFromNondeterministicCachedGraph(satCalcJob, answererContext);
				}
				processCalculationJob(answererContext, satCalcJob, completedMessage);
				return true;
			}




			bool COptimizedComplexExpressionAnsweringHandler::processVariableBindingsEntailmentCalculationCompleted(CAnsweringMessageDataCalculationCompletedVariableBindingEntailment* message, CAnswererContext* answererContext) {
				COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorbedQueryPartData = message->getAbsorbedQueryPartData();
				CComplexQueryExpressionProcessingData* queryProcData = message->getQueryProcessingData();
				COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationExtension = message->getAbsorptionPropagationExtension();
				CAnsweringPropagationSteeringAbsorptionExtensionItemController* propagationSteeringController = message->getPropagationSteeringController();
				CAnsweringSplitEntailmentTestingCancellationAdapter* cancelAdapter = message->getCancellationAdapter();

				if (cancelAdapter) {
					cancelAdapter->decCurrentEntailmentTestingCount();
				}
				if (cancelAdapter && !message->isSatisfiable()) {
					cancelAdapter->setEntailmentFound();
				}

				if (!absorptionPropagationExtension && message->isSatisfiable()) {
					queryProcData->setUnsatisfiable(true);
				}
				if (absorptionPropagationExtension && !message->isSatisfiable()) {
					absorptionPropagationExtension->setEntailmentFound(true);
				}
				if (absorptionPropagationExtension) {
					absorptionPropagationExtension->removePropagationSteeringController(propagationSteeringController);
				}
				if (propagationSteeringController) {
					delete propagationSteeringController;
				}

				queryProcData->decEntailmentComputation();
				if (!absorptionPropagationExtension && !cancelAdapter || absorptionPropagationExtension && !absorptionPropagationExtension->hasRemainingPropagationInitializationIndividualsSplittedTests() || cancelAdapter->getCurrentEntailmentTestingCount() == 0) {
					if (!queryProcData->hasComputation()) {
						if (absorptionPropagationExtension && !absorptionPropagationExtension->isEntailmentFound()) {
							queryProcData->setUnsatisfiable(true);
						}
						if (cancelAdapter && !cancelAdapter->hasEntailmentFound()) {
							queryProcData->setUnsatisfiable(true);
						}
						finishQueryProcessing(queryProcData);
					}
				}
				return true;
			}










			bool COptimizedComplexExpressionAnsweringHandler::createVariableBindingPropagationTest(COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationExtension, COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext, bool &continueProcessingBuildingVarItem) {
				COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionHanldingQueryPart = absorptionPropagationExtension->getAbsorptionBasedHandlingData();
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				buildingVarItem->incVariableBindingsPropagationCount();
				CExpressionVariable* initializerVariable = absorptionHanldingQueryPart->getInitializerVariableExpression();
				CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());
				CVariable* variable = absorptionHanldingQueryPart->getVariableExpressionVariableHash()->value(initializerVariable);
				satCalcJob = extendProcessingByTopPropagation(satCalcJobGen, satCalcJob, absorptionHanldingQueryPart->isTopObjectPropertyUsed(), answererContext);


				QSet<CIndividualReference>* splitIndiSet = nullptr;
				CAnsweringSplitPropagationTestControllingItem* repSlitTestContItem = nullptr;
				QList<CAnsweringSplitPropagationTestControllingItem*>* remainingRepeatedSplitTestingItemList = absorptionPropagationExtension->getRemainingRepeatedSplitTestingItemList();
				bool remainingTests = false;
				if (!remainingRepeatedSplitTestingItemList->isEmpty()) {
					repSlitTestContItem = remainingRepeatedSplitTestingItemList->takeFirst();
					repSlitTestContItem->getBaseVariableData()->visitIntegrateableVariableBindings([&](const CIndividualReference& indiRef)->bool {
						satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(absorptionHanldingQueryPart->getInitializerConcept(), false, indiRef, satCalcJob);
						return true;
					});
					if (!remainingRepeatedSplitTestingItemList->isEmpty()) {
						remainingTests = true;
					}

					if (absorptionPropagationExtension->hasPreparedSplitPropagationTestControllingItems()) {
						if (absorptionPropagationExtension->hasRemainingPreparedSplitPropagationTestControllingItems()) {
							remainingTests = true;
						}
					} else {
						QSet<CIndividualReference>* indiSet = absorptionPropagationExtension->getVariableSteeringIndividualBindingSet(variable);
						if (!indiSet->isEmpty()) {
							remainingTests = true;
						}
					}
				} else {
					bool splitting = false;
					QSet<CIndividualReference>* indiSet = absorptionPropagationExtension->getVariableSteeringIndividualBindingSet(variable);
					if (!absorptionHanldingQueryPart->isTopObjectPropertyUsed() && absorptionPropagationExtension->isPropagationInitializationIndividualsSplitted() || indiSet->size() > mConfMaxVariableBindingPropagationsInitializationIndividualCount && mConfMaxVariableBindingPropagationsInitializationIndividualCount > 0) {
						splitting = true;
					}

					if (splitting && absorptionPropagationExtension->hasPreparedSplitPropagationTestControllingItems()) {
						CAnsweringSplitPropagationTestControllingItem* nextPrepSplitItem = absorptionPropagationExtension->getNextPreparedSplitPropagationTestControllingItem();

						if (nextPrepSplitItem) {
							CVariable* baseVar = nextPrepSplitItem->getBaseVariableData()->getVariable();
							cint64 newTotalLimit = nextPrepSplitItem->getIntendedIntegrationLimit();
							for (CExpressionVariable* preparedVarExp : *absorptionHanldingQueryPart->getPrepareVariableSet()) {
								CVariable* prepVar = absorptionHanldingQueryPart->getVariableExpressionVariableHash()->value(preparedVarExp);
								if (prepVar && prepVar != baseVar) {
									CAnsweringSplitPropagationTestingVariableData* perpVarData = nextPrepSplitItem->getVariableData(prepVar);
									if (!perpVarData || perpVarData->getIntegrateableVariableBindingCount() <= 0) {

										QSet<CIndividualReference>* bindIndiSet = absorptionPropagationExtension->getVariableSteeringIndividualBindingSet(prepVar);
										if (!perpVarData) {
											perpVarData = new CAnsweringSplitPropagationTestingVariableData(nextPrepSplitItem->getNextVariableDataId(), prepVar, preparedVarExp, bindIndiSet);
											nextPrepSplitItem->addVariableData(perpVarData, false);
											perpVarData->setVariableBindingsIntegrationLimit(mConfSplitVariableMappingPropagationTestInitialVariableIntegrationLimit);
										} else {
											perpVarData->addIntegrateableVariableBindings(bindIndiSet);
										}
										newTotalLimit += mConfSplitVariableMappingPropagationTestInitialVariableIntegrationLimit;
									}
								}
							}
							nextPrepSplitItem->setIntegrationLimit(newTotalLimit);

							splitIndiSet = nextPrepSplitItem->getBaseVariableData()->getVariableBindingIndividualSet();
							repSlitTestContItem = nextPrepSplitItem;

							nextPrepSplitItem->getBaseVariableData()->visitIntegrateableVariableBindings([&](const CIndividualReference& indiRef)->bool {
								satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(absorptionHanldingQueryPart->getInitializerConcept(), false, indiRef, satCalcJob);
								return true;
							});
						}

						if (absorptionPropagationExtension->hasRemainingPreparedSplitPropagationTestControllingItems()) {
							remainingTests = true;
						}

					} else {
						if (splitting) {
							splitIndiSet = new QSet<CIndividualReference>();
							absorptionPropagationExtension->setPropagationInitializationIndividualsSplitted(true);
						}
						for (QSet<CIndividualReference>::iterator it = indiSet->begin(); it != indiSet->end() && (mConfMaxVariableBindingPropagationsInitializationIndividualCount < 0 || !splitIndiSet || splitIndiSet->size() < mConfMaxVariableBindingPropagationsInitializationIndividualCount); ) {
							CIndividualReference indiRef(*it);
							satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(absorptionHanldingQueryPart->getInitializerConcept(), false, indiRef, satCalcJob);
							if (splitIndiSet) {
								splitIndiSet->insert(indiRef);
								it = indiSet->erase(it);
							} else {
								++it;
							}
						}
						if (splitIndiSet) {
							if (!indiSet->isEmpty()) {
								remainingTests = true;
							}
						}
					}
				}
				if (remainingTests) {
					// put into queue such that remaining propagation tests are scheduled at some point
					if (!absorptionPropagationExtension->isSplitPropagationItemProcessingQueued()) {
						buildingVarItem->addAbsorptionBasedHandlingExtensionPropagationItem(absorptionPropagationExtension);
						absorptionPropagationExtension->setSplitPropagationItemProcessingQueued(true);
					}
					absorptionPropagationExtension->setPropagationInitializationIndividualsSplittedTestsRemaining(true);
					continueProcessingBuildingVarItem = true;
				} else {
					absorptionPropagationExtension->setPropagationInitializationIndividualsSplittedTestsRemaining(false);
				}


				CAnsweringPropagationSteeringController* propagationSteeringController = nullptr;
				if (splitIndiSet || repSlitTestContItem) {
					if (repSlitTestContItem) {
						propagationSteeringController = repSlitTestContItem;
					} else {
						repSlitTestContItem = createSplitPropagationTestControllingItem(absorptionPropagationExtension, absorptionHanldingQueryPart, splitIndiSet);
						propagationSteeringController = repSlitTestContItem;
					}
					if (mConfExtendedLogging) {
						LOG(INFO, getDomain(), logTr("%2. marker propagation test for split %1 of absorbed query part based on %3 bindings scheduled.").arg(repSlitTestContItem->getTestingItemId()).arg(repSlitTestContItem->getNextTestingId() + 1).arg(repSlitTestContItem->getBaseVariableData()->getIntegrateableVariableBindingCount()), this);
					}
				} else {
					propagationSteeringController = new CAnsweringPropagationSteeringAbsorptionExtensionItemController(absorptionPropagationExtension, variable);
				}
				absorptionPropagationExtension->addPropagationSteeringController(propagationSteeringController);
				CAnsweringMessageDataCalculationCompletedVariableBindingPropagations* completedMessage = new CAnsweringMessageDataCalculationCompletedVariableBindingPropagations(satCalcJob, propagationSteeringController, absorptionPropagationExtension, buildingVarItem);
				satCalcJob->setSatisfiableAnswererBindingPropagationAdapter(new CSatisfiableTaskAnswererBindingPropagationAdapter(absorptionHanldingQueryPart->getInitializerConcept(), absorptionHanldingQueryPart->getPropagationFinalizationConcept(), this, mOntoAnsweringItem->getTestingOntology(false), answererContext->getAnsweringCalculationHandler(), propagationSteeringController));
				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incExistentialQueryPartVariableBindingPropagationCount();
				if (!absorptionHanldingQueryPart->hasNonTrivialAbsorptionPropagation()) {
					continueCalculationJobFromNondeterministicCachedGraph(satCalcJob, answererContext);
				}
				processCalculationJob(answererContext, satCalcJob, completedMessage);
				return true;
			}









			CAnsweringSplitPropagationTestControllingItem* COptimizedComplexExpressionAnsweringHandler::createSplitPropagationTestControllingItem(COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationExtension, COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionHanldingQueryPart, QSet<CIndividualReference>* splitIndiSet, bool initializeBindingSets) {
				CAnsweringSplitPropagationTestControllingItem* splitTestingItem = absorptionPropagationExtension->getNextSplitPropagationTestControllingItem();
				cint64 varDataId = 0;
				CExpressionVariable* baseVarExp = absorptionHanldingQueryPart->getInitializerVariableExpression();
				CVariable* baseVar = absorptionHanldingQueryPart->getVariableExpressionVariableHash()->value(baseVarExp);
				CAnsweringSplitPropagationTestingVariableData* baseVarData = new CAnsweringSplitPropagationTestingVariableData(varDataId, baseVar, baseVarExp, splitIndiSet);
				splitTestingItem->addVariableData(baseVarData, true);
				cint64 totalLimit = mConfSplitVariableMappingPropagationTestInitialTotalIntegrationLimit;
				for (CExpressionVariable* preparedVarExp : *absorptionHanldingQueryPart->getPrepareVariableSet()) {
					CVariable* prepVar = absorptionHanldingQueryPart->getVariableExpressionVariableHash()->value(preparedVarExp);
					if (prepVar && prepVar != baseVar) {
						QSet<CIndividualReference>* bindIndiSet = nullptr;
						if (initializeBindingSets) {
							bindIndiSet = absorptionPropagationExtension->getVariableSteeringIndividualBindingSet(prepVar);
						}
						CAnsweringSplitPropagationTestingVariableData* perpVarData = new CAnsweringSplitPropagationTestingVariableData(varDataId, prepVar, preparedVarExp, bindIndiSet);
						splitTestingItem->addVariableData(perpVarData, false);
						perpVarData->setVariableBindingsIntegrationLimit(mConfSplitVariableMappingPropagationTestInitialVariableIntegrationLimit);
						totalLimit += mConfSplitVariableMappingPropagationTestInitialVariableIntegrationLimit;
					}
				}
				splitTestingItem->setIntegrationLimit(totalLimit);						
				return splitTestingItem;
			}



			bool COptimizedComplexExpressionAnsweringHandler::processVariableBindingsPropagationItemCalculationCompleted(CAnsweringMessageDataCalculationCompletedVariableBindingPropagations* message, bool computationError, CAnswererContext* answererContext) {
				COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationItem = message->getAbsorptionPropagationItem();
				COptimizedComplexBuildingIndividualVariableCompositionsItem* buildVarItem = message->getVariableBuildingItem();
				CAnsweringPropagationSteeringController* propagationSteeringController = message->getPropagationSteeringController();
				absorptionPropagationItem->removePropagationSteeringController(propagationSteeringController);
				if (propagationSteeringController) {
					CAnsweringSplitPropagationTestControllingItem* splitPropTestContItem = dynamic_cast<CAnsweringSplitPropagationTestControllingItem*>(propagationSteeringController);
					if (splitPropTestContItem) {
						cint64 intLimit = splitPropTestContItem->getIntendedIntegrationLimit();
						cint64 newIntLimit = qMax(intLimit * mConfSplitVariableMappingPropagationTestIntegrationLimitIncreaseFactor, mConfSplitVariableMappingPropagationTestMaxIntegrationLimit);
						splitPropTestContItem->reconfigureVariableIntegrationLimits(newIntLimit);
						splitPropTestContItem->disableCheckedVariableIntegrations();
						if (!splitPropTestContItem->hasCheckedAllVariableIntegrations()) {
							absorptionPropagationItem->addRemainingRepeatedSplitTestingItem(splitPropTestContItem);

							if (!absorptionPropagationItem->isSplitPropagationItemProcessingQueued()) {
								buildVarItem->addAbsorptionBasedHandlingExtensionPropagationItem(absorptionPropagationItem);
								absorptionPropagationItem->setSplitPropagationItemProcessingQueued(true);
								if (!buildVarItem->isProcessingQueued()) {
									buildVarItem->setProcessingQueued(true);
									mOntoAnsweringItem->addProcessingVariableBuildingItem(buildVarItem);
								}
							}
						}
					} else {
						delete propagationSteeringController;
					}
				}

				if (computationError) {
					buildVarItem->getQueryProcessingData()->setComputationError(computationError);
				}

				buildVarItem->decVariableBindingsPropagationCount();

				if (!absorptionPropagationItem->hasRemainingPropagationInitializationIndividualsSplittedTests()) {
					if (!absorptionPropagationItem->isVariableMappingsComputed()) {
						// schedule item processing
						queueVariableItemComputation(buildVarItem, absorptionPropagationItem);
					}
					buildVarItem->setProcessingQueued(true);
					mOntoAnsweringItem->addProcessingVariableBuildingItem(buildVarItem);
				}

				return true;
			}






			bool COptimizedComplexExpressionAnsweringHandler::processVariableBindingsConfirmationCalculationCompleted(CAnsweringMessageDataCalculationCompletedVariableBindingConfirmation* message, CAnswererContext* answererContext) {
				COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationItem = message->getAbsorptionPropagationItem();
				COptimizedComplexBuildingIndividualVariableCompositionsItem* buildVarItem = message->getVariableBuildingItem();

				COptimizedComplexVariableIndividualBindingsCardinalityLinker* variableBindingCardLinker = message->getPropagatedBindingCardinalityLinker();

				QHash<COptimizedComplexVariableIndividualBindingsHasher, COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* testingVariableMapping = absorptionPropagationItem->getTestingVariableMapping();
				testingVariableMapping->remove(variableBindingCardLinker);

				bool variableBindingEntailed = !message->isSatisfiable();
				if (variableBindingEntailed) {
					absorptionPropagationItem->getVariableMapping()->addInsertingBindingsCardinalityLinker(variableBindingCardLinker, true);
				}

				buildVarItem->decVariableBindingsConfirmationCount();

				COptimizedComplexVariableIndividualMappings* possibleVarIndiBindings = absorptionPropagationItem->getPossibleVariableMapping();
				if (!buildVarItem->isProcessingQueued() && possibleVarIndiBindings->getBindingCount() <= 0 && !buildVarItem->isWaitingVariableBindingsConfirmation()) {
					buildVarItem->setProcessingQueued(true);
					mOntoAnsweringItem->addProcessingVariableBuildingItem(buildVarItem);
				}

				return true;
			}








			bool COptimizedComplexExpressionAnsweringHandler::processExtractedVariableBindingPropagations(CAnsweringMessageDataVariableBindingPropagations* variableBindingsPropagationMessage, CAnswererContext* answererContext) {


				CVARIABLEBINDINGMESSAGELIST<CAnsweringMessageDataVariableBindingPropagationsData*>* individualBindingList = variableBindingsPropagationMessage->getIndividualBindingList();
				CVARIABLEBINDINGMESSAGELIST<CVariable*>* variablesList = variableBindingsPropagationMessage->getVariablesList();

				CConcept* propagationFinalizationConcept = variableBindingsPropagationMessage->getPropagationFinalizationConcept();
				QHash<CConcept*, COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* propagationFinalizationConceptAbsorptionDataHash = mOntoAnsweringItem->getPropagationFinalizationConceptAbsorptionDataHash();
				QHash<CConcept*, COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*>* propagationFinalizationConceptAbsorptionItemHash = mOntoAnsweringItem->getPropagationFinalizationConceptAbsorptionItemHash();


				COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionItem = propagationFinalizationConceptAbsorptionItemHash->value(propagationFinalizationConcept);
				COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionData = propagationFinalizationConceptAbsorptionDataHash->value(propagationFinalizationConcept);

				CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();
				QHash<CVariable*, cint64>* variableIndexHash = absorptionItem->getVariableIndexHash();
				COptimizedComplexVariableIndividualMappings* variableMapping = absorptionItem->getVariableMapping();



				QHash<CExpressionVariable*, cint64> variablePosHash;
				cint64 varPosCount = 0;
				QList<QList<cint64>> variableIndexPosList;
				for (CVariable* variable : *variablesList) {
					QList<cint64> indexs = variableIndexHash->values(variable);
					variableIndexPosList.append(indexs);
					varPosCount += indexs.size();
				}

				COptimizedComplexVariableIndividualMappings* possibleVariableMapping = absorptionItem->getPossibleVariableMapping();

				COptimizedComplexVariableIndividualBindings* propagatedBinding = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinality* propagatedCardinalites = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* propagatedLinker = nullptr;

				COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem = nullptr;

				cint64 count = 0;
				// first verify that variable binding is valid
				for (CAnsweringMessageDataVariableBindingPropagationsData* individualBindingData : *individualBindingList) {
					bool possiblePropagation = individualBindingData->isPossibleBinding();
					count++;

					if (!propagatedBinding) {
						propagatedBinding = createBindingsForVariableCompositionItems(varPosCount, absorptionItem, buildingVarItem);
					}

					cint64 pos = 0;
					for (QList<cint64> variableIndexPositions : variableIndexPosList) {
						CIndividualReference indiRef = individualBindingData->getBindingIndividualReference(pos++);
						for (cint64 variableIndexPos : variableIndexPositions) {
							if (variableIndexPos >= 0) {
								CRealizationIndividualInstanceItemReference instanceItemRef = sameRealization->getSameInstanceItemReference(indiRef);
								propagatedBinding->setBinding(variableIndexPos, instanceItemRef);
							}
						}
					}

					if (!propagatedLinker) {
						propagatedLinker = createBindingsLinkerForVariableCompositionItems(propagatedBinding, nullptr, absorptionItem, buildingVarItem);
					} else {
						propagatedLinker->updateCardinality(propagatedCardinalites);
					}


					if (!possiblePropagation) {
						if (variableMapping->addInsertingBindingsCardinalityLinker(propagatedLinker, true)) {
							propagatedBinding = nullptr;
							propagatedLinker = nullptr;
							propagatedCardinalites = nullptr;
						} else {
							propagatedCardinalites = propagatedLinker->getCurrentCardinalities();
						}

					} else {
						if (!possibleVariableMapping) {
							possibleVariableMapping = new COptimizedComplexVariableIndividualMappingsHash(variablesList->size());
							absorptionItem->setPossibleVariableMapping(possibleVariableMapping);
						}
						if (possibleVariableMapping->addInsertingBindingsCardinalityLinker(propagatedLinker, true)) {
							propagatedBinding = nullptr;
							propagatedLinker = nullptr;
							propagatedCardinalites = nullptr;
						} else {
							propagatedCardinalites = propagatedLinker->getCurrentCardinalities();
						}
					}
				}

				if (mConfExtendedLogging) {
					cint64 possMappingCount = 0;
					if (possibleVariableMapping) {
						possMappingCount = possibleVariableMapping->getBindingCount();
					}
					cint64 certMappingCount = variableMapping->getBindingCount();
					LOG(INFO, getDomain(), logTr("Obtained %1 certain and %2 possible mappings from propagated marker for absorbed query part.").arg(certMappingCount).arg(possMappingCount), this);
				}


				return true;
			}







			bool COptimizedComplexExpressionAnsweringHandler::canRetrieveInstantiatingDataValueIndividuals(CRole* dataRole, CDataLiteral* dataLiteral) {
				COntologyTriplesAssertionsAccessor* triplesAssertionAccessor = mOntoAnsweringItem->getOntology()->getOntologyTriplesData()->getTripleAssertionAccessor();
				if (triplesAssertionAccessor && dataRole->hasPropertyName()) {
					CDatatype* datatype = dataLiteral->getDatatype();
					if (datatype) {
						if (datatype->getDatatypeType() == CDatatype::DT_STRING) {
							return true;
						}
					}
				}
				return false;
			}


			QMap<cint64, IndividualIdCandidateData>* COptimizedComplexExpressionAnsweringHandler::getInstantiatingDataValueIndividualIdMap(CRole* dataRole, CDataLiteral* dataLiteral, QMap<cint64, IndividualIdCandidateData>* instantiatingDataValueIndividualIdMap, CAnswererContext* answererContext) {
				if (!instantiatingDataValueIndividualIdMap) {
					instantiatingDataValueIndividualIdMap = new QMap<cint64, IndividualIdCandidateData>();
				}
				COntologyTriplesAssertionsAccessor* triplesAssertionAccessor = mOntoAnsweringItem->getOntology()->getOntologyTriplesData()->getTripleAssertionAccessor();
				triplesAssertionAccessor->visitDataValueIndividuals(dataRole, dataLiteral, [&](cint64 indiId)->bool {
					instantiatingDataValueIndividualIdMap->insert(indiId, IndividualIdCandidateData());
					return true;
				});
				return instantiatingDataValueIndividualIdMap;
			}






			QSet<CDataLiteral*>* COptimizedComplexExpressionAnsweringHandler::getInstantiatedDataLiterals(const CIndividualReference& indiRef, CRole* dataRole, QSet<CDataLiteral*>* dataLiteralSet, CAnswererContext* answererContext) {

				if (!dataLiteralSet) {
					dataLiteralSet = new QSet<CDataLiteral*>();
				}
				QHash<QPair<QString, CDatatype*>, CDataLiteral*>* dataValueDatatypeSingleLiteralHash = nullptr;
				COntologyTriplesAssertionsAccessor* triplesAssertionAccessor = mOntoAnsweringItem->getOntology()->getOntologyTriplesData()->getTripleAssertionAccessor();
				if (triplesAssertionAccessor) {
					dataValueDatatypeSingleLiteralHash = mOntoAnsweringItem->getDataValueDatatypeSingleLiteralHash();
				}
				CDataLiteralNormalizerPreProcess dataLitNormalizer;
				CPreProcessContextBase preprocessingContext(mOntoAnsweringItem->getTestingOntology(), mOntoAnsweringItem->getTestingOntology()->getConfiguration());
				CContext* dataValueMemoryManagementContext = mOntoAnsweringItem->getDataValueMemoryManagementContext();

				CIndividualProcessNodeVector* indiProcVector = nullptr;
				CConsistence* consistence = mOntoAnsweringItem->getOntology()->getConsistence();
				if (consistence) {
					CConsistenceData* consData = consistence->getConsistenceModelData();
					if (consData) {
						CConsistenceTaskData* consTaskData = dynamic_cast<CConsistenceTaskData*>(consData);
						CSatisfiableCalculationTask* satConsTask = consTaskData->getCompletionGraphCachedSatisfiableTask();
						if (!satConsTask) {
							satConsTask = consTaskData->getDeterministicSatisfiableTask();
						}
						if (satConsTask) {
							CProcessingDataBox* procDataBox = satConsTask->getProcessingDataBox();
							if (procDataBox) {
								indiProcVector = procDataBox->getIndividualProcessNodeVector();
							}
						}
					}
				}


				if (indiProcVector) {
					bool nondeterministicNodeFlag = true;
					CIndividualProcessNode* indiProcNode = getNominalIndividualProcessNode(indiRef, indiProcVector, &nondeterministicNodeFlag, answererContext);
					if (indiProcNode) {
						CRoleSuccessorLinkIterator succRoleIt = indiProcNode->getRoleSuccessorLinkIterator(dataRole);
						while (succRoleIt.hasNext()) {
							bool nonDeterministicEdgeFlag = true;
							CIndividualLinkEdge* linkEdge = succRoleIt.next(true);
							CDependencyTrackPoint* linkEdgeDepPoint = linkEdge->getDependencyTrackPoint();
							if (linkEdgeDepPoint) {
								nonDeterministicEdgeFlag = linkEdgeDepPoint->getBranchingTag() > 0;
							}
							cint64 succIndiId = linkEdge->getOppositeIndividualID(indiProcNode);
							CIndividualProcessNode* succNode = indiProcVector->getData(succIndiId);

							for (CConceptDescriptor* conDesIt = succNode->getReapplyConceptLabelSet(false)->getAddingSortedConceptDescriptionLinker(); conDesIt; conDesIt = conDesIt->getNextConceptDesciptor()) {
								CConcept* concept = conDesIt->getConcept();
								bool negated = conDesIt->isNegated();
								CDependencyTrackPoint* conDepTrackPoint = conDesIt->getDependencyTrackPoint();
								bool nondeterministicConcept = true;
								if (!negated && concept->getOperatorCode() == CCDATALITERAL) {

									if (conDepTrackPoint) {
										nondeterministicConcept = conDepTrackPoint->getBranchingTag() > 0;
									}

									CDataLiteral* dataLiteral = concept->getDataLiteral();
									if (!nondeterministicNodeFlag && !nonDeterministicEdgeFlag && !nondeterministicConcept) {
										if (dataValueDatatypeSingleLiteralHash) {
											CDataLiteral*& hashedValue = (*dataValueDatatypeSingleLiteralHash)[QPair<QString, CDatatype*>(dataLiteral->getLexicalDataLiteralValueString(), dataLiteral->getDatatype())];
											if (!hashedValue) {
												hashedValue = dataLiteral;
											}
											dataLiteralSet->insert(hashedValue);
										} else {
											dataLiteralSet->insert(dataLiteral);
										}
									}
								}
							}
						}
					}
				}


				if (triplesAssertionAccessor) {
					class COntologyTriplesIndividualDataAssertionsVisitor : public COntologyTriplesIndividualAssertionsVisitor {
					public:
						function<bool(CDataLiteral* dataLiteral, CRole* dataRole)> mDataLiteralHandleFunc;

						COntologyTriplesIndividualDataAssertionsVisitor(function<bool(CDataLiteral* dataLiteral, CRole* dataRole)> dataLiteralHandleFunc) {
							mDataLiteralHandleFunc = dataLiteralHandleFunc;
						}

						bool visitIndividualName(const QString& indiName, COntologyTriplesAssertionsAccessor* accessor) {
							// for debugging, it is visiting the individual name
							return true;
						}

						bool visitRoleAssertion(CRole* role, cint64 otherIndividualId, COntologyTriplesAssertionsAccessor* accessor) {
							return false;
						}

						bool visitReverseRoleAssertion(CRole* role, cint64 otherIndividualId, COntologyTriplesAssertionsAccessor* accessor) {
							return false;
						}

						bool visitDataAssertion(CRole* role, CDataLiteral* dataLiteral, COntologyTriplesAssertionsAccessor* accessor) {
							return mDataLiteralHandleFunc(dataLiteral, role);
						}

						bool visitConceptAssertion(CConcept* concept, COntologyTriplesAssertionsAccessor* accessor) {
							return false;
						}
					} visitor([&](CDataLiteral* dataLiteral, CRole* dataLiteralDataRole)->bool {
						if (dataRole == dataLiteralDataRole) {

							if (dataValueDatatypeSingleLiteralHash) {
								CDataLiteral*& hashedValue = (*dataValueDatatypeSingleLiteralHash)[QPair<QString, CDatatype*>(dataLiteral->getLexicalDataLiteralValueString(), dataLiteral->getDatatype())];
								if (!hashedValue) {
									CDataLiteral* copiedDataLiteral = CObjectParameterizingAllocator<CDataLiteral, CContext*>::allocateAndConstructAndParameterize(nullptr, nullptr);
									copiedDataLiteral->initDataLiteral(dataLiteral->getLexicalDataLiteralValueString(), dataLiteral->getDatatype());

									CDataLiteralValue* dataLiteralValue = copiedDataLiteral->getDataLiteralValue();
									if (!dataLiteralValue) {
										dataLitNormalizer.createPreprocessedDataLiteralValue(copiedDataLiteral, &preprocessingContext, dataValueMemoryManagementContext);
									}

									hashedValue = copiedDataLiteral;
								}
								dataLiteralSet->insert(hashedValue);
							} else {
								CDataLiteral* copiedDataLiteral = CObjectParameterizingAllocator<CDataLiteral, CContext*>::allocateAndConstructAndParameterize(nullptr, nullptr);
								copiedDataLiteral->initDataLiteral(dataLiteral->getLexicalDataLiteralValueString(), dataLiteral->getDatatype());

								CDataLiteralValue* dataLiteralValue = copiedDataLiteral->getDataLiteralValue();
								if (!dataLiteralValue) {
									dataLitNormalizer.createPreprocessedDataLiteralValue(copiedDataLiteral, &preprocessingContext, dataValueMemoryManagementContext);
								}

								dataLiteralSet->insert(copiedDataLiteral);
							}


						}
						return true;
					});



					triplesAssertionAccessor->visitIndividualAssertions(indiRef.getIndividualID(), &visitor, false, false, false, true);
				} else {

					CIndividual* indi = indiRef.getIndividual();
					if (indi) {
						for (CDataAssertionLinker* dataAssertionLinkerIt = indi->getAssertionDataLinker(); dataAssertionLinkerIt; dataAssertionLinkerIt = dataAssertionLinkerIt->getNext()) {
							if (dataRole == dataAssertionLinkerIt->getRole()) {

								CDataLiteral* dataLiteral = dataAssertionLinkerIt->getDataLiteral();

								if (dataValueDatatypeSingleLiteralHash) {
									CDataLiteral*& hashedValue = (*dataValueDatatypeSingleLiteralHash)[QPair<QString, CDatatype*>(dataLiteral->getLexicalDataLiteralValueString(), dataLiteral->getDatatype())];
									if (!hashedValue) {
										hashedValue = dataLiteral;
									}
									dataLiteralSet->insert(hashedValue);
								} else {
									dataLiteralSet->insert(dataLiteral);
								}

							}
						}
					}
				}

				return dataLiteralSet;
			}







			CIndividualProcessNode* COptimizedComplexExpressionAnsweringHandler::getMergeCorrectedIndividualProcessNode(CIndividualProcessNode* indiProcNode, CIndividualProcessNodeVector* indiProcVector, bool* nonDeterministicallyMergedFlag, CAnswererContext* answererContext) {
				bool nonDeterministicMerged = false;
				while (indiProcNode && indiProcNode->getMergedIntoIndividualNodeID() != indiProcNode->getIndividualNodeID()) {
					cint64 mergeID = indiProcNode->getMergedIntoIndividualNodeID();
					CDependencyTrackPoint* mergedDepTrackPoint = indiProcNode->getMergedDependencyTrackPoint();
					indiProcNode = indiProcVector->getData(mergeID);
					if (!mergedDepTrackPoint || mergedDepTrackPoint->getBranchingTag() > 0) {
						nonDeterministicMerged = true;
					}
				}
				if (nonDeterministicallyMergedFlag) {
					*nonDeterministicallyMergedFlag = nonDeterministicMerged;
				}
				return indiProcNode;
			}




			CIndividualProcessNode* COptimizedComplexExpressionAnsweringHandler::getNominalIndividualProcessNode(const CIndividualReference& indiRef, CIndividualProcessNodeVector* indiProcVector, bool* nondeterministicNodeFlag, CAnswererContext* answererContext) {

				cint64 indiID = -indiRef.getIndividualID();

				if (indiProcVector) {
					CIndividualProcessNode* baseIndiProcNode = indiProcVector->getData(indiID);
					CIndividualProcessNode* indiProcNode = baseIndiProcNode;
					bool nondeterministicallyMergedFlag = false;
					if (indiProcNode) {
						indiProcNode = getMergeCorrectedIndividualProcessNode(indiProcNode, indiProcVector, &nondeterministicallyMergedFlag, answererContext);
					}
					if (nondeterministicNodeFlag) {
						*nondeterministicNodeFlag = nondeterministicallyMergedFlag;
					}
					return indiProcNode;
				}
				return nullptr;
			}






			bool COptimizedComplexExpressionAnsweringHandler::queueVariableItemComputation(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem) {
				if (!varCompItem->isComputationQueued()) {
					varCompItem->setComputationQueued(true);
					buildingVarItem->addComputeVariableMappingItem(varCompItem);
					return true;
				}
				return false;
			}




			bool COptimizedComplexExpressionAnsweringHandler::debugCheckVariableMappingContainsSolution(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem) {
				QHash<CExpressionVariable*, COptimizedComplexConceptItem*>* varConItemHash = buildingVarItem->getVariableConceptItemHash();
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();

				QHash<CExpressionVariable*, COptimizedComplexVariableCompositionItemVariableExpressionMapping>* varItemIndexMappingHash = buildingVarItem->getVariableItemIndexMappingHash();

				bool allContainingSolution = true;
				for (QHash<CExpressionVariable*, COptimizedComplexVariableCompositionItemVariableExpressionMapping>::const_iterator it = varItemIndexMappingHash->constBegin(), itEnd = varItemIndexMappingHash->constEnd(); it != itEnd; ++it) {
					CExpressionVariable* varExp = it.key();
					const COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = it.value();
					if (varItemIndexMapping.contains(varCompItem)) {
						allContainingSolution &= debugCheckVariableMappingContainsSolution(buildingVarItem, varExp, varCompItem);
					}
				}

				return allContainingSolution;

			}



			QString COptimizedComplexExpressionAnsweringHandler::debugGetVariableMappingsHeaderString(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem, cint64** sortArray) {
				QHash<CExpressionVariable*, COptimizedComplexConceptItem*>* varConItemHash = buildingVarItem->getVariableConceptItemHash();
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();

				QString varHeaderString;
				QList<CExpressionVariable*> varList;				
				for (auto it = varExpVarComItemHash->constBegin(), itEnd = varExpVarComItemHash->constEnd(); it != itEnd; ++it) {
					CExpressionVariable* var = it.key();
					QSet<COptimizedComplexVariableCompositionItem*>* compItemSet = it.value();
					if (compItemSet && compItemSet->contains(varCompItem)) {
						varList.append(var);
					}
				}
#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				if (varList.isEmpty() && varCompItem->debugVarItemIndexMapping) {
					varList.append(nullptr);
				}
#endif

				for (CExpressionVariable* var : varList) {
					QStringList varNameStrings = debugGetItemVariableNames(buildingVarItem, var, varCompItem);
					if (sortArray) {
						if (!*sortArray) {
							*sortArray = new cint64[varNameStrings.length()];
						}
						QHash<QString, cint64> varNameIndHash;
						QStringList sortedVarNameStrings = varNameStrings;
						qSort(sortedVarNameStrings.begin(), sortedVarNameStrings.end());
						cint64 nextVarId = 0;
						for (const QString& varName : sortedVarNameStrings) {
							varNameIndHash.insert(varName, nextVarId++);
						}
						cint64 varOriginalInd = 0;
						for (const QString& varName : varNameStrings) {
							cint64 varNewInd = varNameIndHash.value(varName);
							(*sortArray)[varOriginalInd++] = varNewInd;
						}
						varNameStrings = sortedVarNameStrings;
					}
					varHeaderString += varNameStrings.join("\t") + "\r\n";
				}

				return varHeaderString;
			}



			QSet<QString> COptimizedComplexExpressionAnsweringHandler::debugGetVariableMappingStringSet(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem, cint64** sortArray) {
				QSet<QString> bindingStringSet;
				COptimizedComplexVariableIndividualMappings* varMapping = varCompItem->getVariableMapping();

				CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();

				for (COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = varMapping->getLastAddedBindingsCardinalityLinker(); linker; linker = linker->getNext()) {
					COptimizedComplexVariableIndividualBindings* binding = linker->getBindings();

					QStringList bindingStringList;

					for (cint64 i = 0; i < binding->getBindingSize(); ++i) {

						COptimizedComplexVariableIndividualMappings::VARIABLE_TYPE bindingType = varMapping->getBindingMapping(i);
						if (bindingType == COptimizedComplexVariableIndividualMappings::INDIVIDUAL_VARIABLE) {

							CRealizationIndividualInstanceItemReference instItemRef = binding->getBinding(i).reference;
							COptimizedKPSetIndividualItem* indiItem = (COptimizedKPSetIndividualItem*)instItemRef.getRealizationInstanceItem();
							if (indiItem && indiItem->getIndividualId() != instItemRef.getIndividualID()) {
								bool debug = true;
							}
							CIndividual* individual = instItemRef.getIndividual();
							QString individualString = QString::number(instItemRef.getIndividualID());
							if (individual) {
								QList<CIndividualReference> sameIndiList;
								QList<QString> upSameIndiList;
								CInstanceBindingIndividualCollectionVisitor visitor(&sameIndiList);
								sameRealization->visitSameIndividuals(instItemRef, &visitor);
								for (const CIndividualReference& indiRef : sameIndiList) {
									CIndividual* indi = mOntoAnsweringItem->getOntology()->getABox()->getIndividualVector()->getData(indiRef.getIndividualID());
									upSameIndiList.append(CIRIName::getRecentIRIName(indi->getIndividualNameLinker()));
								}
								qSort(upSameIndiList.begin(), upSameIndiList.end());
								individualString = upSameIndiList.first();
							} else {
								individualString = mOntoAnsweringItem->getOntology()->getIndividualNameResolver()->getIndividualName(instItemRef);
							}
							bindingStringList.append(individualString);

						}
					}

					if (sortArray && *sortArray) {
						QStringList sortedBindingStringList(bindingStringList);
						cint64 bindingPos = 0;
						for (const QString& bindingString : bindingStringList) {
							cint64 newPos = (*sortArray)[bindingPos++];
							sortedBindingStringList[newPos] = bindingString;
						}
						bindingStringList = sortedBindingStringList;
					}
					QString bindingString = bindingStringList.join("\t");

					cint64 card = 0;
					if (linker->getCurrentCardinalities()) {
						card = linker->getCurrentCardinalities()->getSameIndividualsSeparatlyConsideredCardinality();
					}
					bindingString.append(QString("\t%1").arg(card));
					bindingStringSet.insert(bindingString);
				}
				return bindingStringSet;
			}


			bool COptimizedComplexExpressionAnsweringHandler::debugWriteVariableMappingToFileWithHeader(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem, const QString& suffix) {
				COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingIndiVarItem = dynamic_cast<COptimizedComplexBuildingIndividualVariableCompositionsItem*>(buildingVarItem);
				if (buildingIndiVarItem) {
					COptimizedComplexVariableIndividualMappings* varMapping = varCompItem->getVariableMapping();
					QString nextFileName = QString("Debugging/Answering/item-bindings-%1-%2.tsv").arg(varCompItem->getComputationStepId()).arg(nextBindingsFileId++);
					if (!suffix.isEmpty()) {
						nextFileName = QString("Debugging/Answering/item-bindings-%1.tsv").arg(suffix);
					}
					QFile bindingsFile(nextFileName);
					if (bindingsFile.open(QIODevice::WriteOnly)) {

						cint64* sortArray = nullptr;

						QString varHeaderString = debugGetVariableMappingsHeaderString(buildingIndiVarItem, varCompItem, &sortArray);
						bindingsFile.write(varHeaderString.toLocal8Bit());

#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
						cint64 debugId = 0;
						for (auto debugString : debugCreationStringList) {
							if (debugString == varCompItem->debugCreationString) {
								bindingsFile.write(QString("#-%1 [current] >> %2\r\n").arg(debugId++).arg(debugString).toLocal8Bit());
							} else {
								bindingsFile.write(QString("#-%1 --------- >> %2\r\n").arg(debugId++).arg(debugString).toLocal8Bit());
							}
						}

#endif

						QSet<QString> bindingsStringSet = debugGetVariableMappingStringSet(buildingIndiVarItem, varCompItem, &sortArray);
						for (QString bindingString : bindingsStringSet) {
							bindingsFile.write(bindingString.toLocal8Bit());
							bindingsFile.write("\r\n");
						}
						bindingsFile.close();
						if (sortArray) {
							delete[] sortArray;
						}
					}
					return true;
				} else {
					return false;
				}
			}




			bool COptimizedComplexExpressionAnsweringHandler::debugWriteVariableMappingToFile(COptimizedComplexBuildingVariableCompositionsItem* buildVarItem, COptimizedComplexVariableCompositionItem* varCompItem) {
				COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem = dynamic_cast<COptimizedComplexBuildingIndividualVariableCompositionsItem*>(buildVarItem);
				if (buildingVarItem) {
					COptimizedComplexVariableIndividualMappings* varMapping = varCompItem->getVariableMapping();
					cint64 compStep = varCompItem->getComputationStepId();
					QFile bindingsFile(QString("Debugging/Answering/item-bindings-step-%1.tsv").arg(compStep));
					QString bindingsString;

					cint64* sortArray = nullptr;
					QString varHeaderString = debugGetVariableMappingsHeaderString(buildingVarItem, varCompItem, &sortArray);

					QSet<QString> bindingsStringSet = debugGetVariableMappingStringSet(buildingVarItem, varCompItem, &sortArray);
					for (QString bindingString : bindingsStringSet) {
						if (!bindingsString.isEmpty()) {
							bindingsString.append("\n");
						}
						bindingsString.append(bindingString);
					}
					if (bindingsFile.open(QIODevice::WriteOnly)) {
						bindingsFile.write(bindingsString.toLocal8Bit());
						bindingsFile.close();
					}

					if (sortArray) {
						delete[] sortArray;
					}
				}

				return true;
			}


			bool COptimizedComplexExpressionAnsweringHandler::debugCheckVariableMappingFromFile(COptimizedComplexBuildingVariableCompositionsItem* buildVarItem, COptimizedComplexVariableCompositionItem* varCompItem) {
				COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem = dynamic_cast<COptimizedComplexBuildingIndividualVariableCompositionsItem*>(buildVarItem);
				if (buildingVarItem) {
					COptimizedComplexVariableIndividualMappings* varMapping = varCompItem->getVariableMapping();
					cint64 compStep = varCompItem->getComputationStepId();
					QFile bindingsFile(QString("Debugging/Answering/item-bindings-step.tsv"));

					cint64* sortArray = nullptr;
					QString varHeaderString = debugGetVariableMappingsHeaderString(buildingVarItem, varCompItem, &sortArray);

					QSet<QString> itemBindingsStringSet = debugGetVariableMappingStringSet(buildingVarItem, varCompItem, &sortArray);
					QSet<QString> fileBindingsStringSet;

					if (bindingsFile.open(QIODevice::ReadOnly)) {
						while (!bindingsFile.atEnd()) {
							fileBindingsStringSet.insert(bindingsFile.readLine().trimmed());
						}
						bindingsFile.close();
					}

					QStringList itemAdditionalBindings;
					for (QString itemBinding : itemBindingsStringSet) {
						if (!fileBindingsStringSet.contains(itemBinding.trimmed())) {
							itemAdditionalBindings.append(itemBinding.trimmed());
						}
					}

					QStringList fileAdditionalBindings;
					for (QString fileBinding : fileBindingsStringSet) {
						if (!itemBindingsStringSet.contains(fileBinding.trimmed())) {
							fileAdditionalBindings.append(fileBinding.trimmed());
						}
					}

					if (fileAdditionalBindings.size() > 0 || itemAdditionalBindings.size() > 0) {
						bool debug = true;

						COptimizedComplexVariableConceptBaseItem* conceptBaseItem = (COptimizedComplexVariableConceptBaseItem*)varCompItem;
						COptimizedComplexConceptItem* conceptItem = conceptBaseItem->getConceptItem();

						QMap<cint64, CIndividual*> missingIndiIdMap;
						CIndividualVector* indiVec = mOntoAnsweringItem->getOntology()->getABox()->getIndividualVector();
						for (cint64 i = 0; i <= indiVec->getItemMaxIndex(); ++i) {
							CIndividual* indi = indiVec->getData(i);
							if (indi) {
								QString indiName = CIRIName::getRecentIRIName(indi->getIndividualNameLinker());
								if (fileAdditionalBindings.contains(indiName + "\t0")) {
									missingIndiIdMap.insert(indi->getIndividualID(), indi);
								}
							}
						}


						QMap<cint64, CIndividual*> additionalIndiIdMap;
						for (cint64 i = 0; i <= indiVec->getItemMaxIndex(); ++i) {
							CIndividual* indi = indiVec->getData(i);
							if (indi) {
								QString indiName = CIRIName::getRecentIRIName(indi->getIndividualNameLinker());
								if (itemAdditionalBindings.contains(indiName + "\t0")) {
									additionalIndiIdMap.insert(indi->getIndividualID(), indi);
								}
							}
						}



						initializeRealizationInstancesIterators(conceptItem, -1, mAnswererContext);


						CRealizationIndividualInstanceItemReferenceIterator* iterator1 = conceptItem->getRealizationIterator();
						if (iterator1) {
							iterator1->begin();
							COptimizedComplexConceptInstanziatedIndividualItemHash* directInstanceItems = new COptimizedComplexConceptInstanziatedIndividualItemHash();
							QSet<CRealizationIndividualInstanceItemReference>* possiblenstanceItemSet = conceptItem->getPossibleInstanceItemSet();
							cint64 visitedIndis = 0;
							while (!iterator1->atEnd()) {

								CRealizationIndividualInstanceItemReference indiItemRef = iterator1->currentIndividualInstanceItemReference();
								QString indiName = CIRIName::getRecentIRIName(indiItemRef.getIndividual()->getIndividualNameLinker());


								cint64 nextAdditionalIndiId = -1;
								QMap<cint64, CIndividual*>::iterator it = additionalIndiIdMap.upperBound(indiItemRef.getIndividualID());
								if (it != additionalIndiIdMap.end()) {
									nextAdditionalIndiId = it.key();
								}

								CRealizationIndividualInstanceItemReferenceIterator* iteratorCopy = iterator1->getCopy();
								iterator1->moveNext();
								if (!iterator1->atEnd()) {
									CRealizationIndividualInstanceItemReference nextIndiItemRef = iterator1->currentIndividualInstanceItemReference();
									bool debug = false;

									if (nextIndiItemRef.getIndividualID() > nextAdditionalIndiId && nextAdditionalIndiId >= 0) {
										debug = true;
									}

									QString nextIndiName = CIRIName::getRecentIRIName(nextIndiItemRef.getIndividual()->getIndividualNameLinker());
									if (fileAdditionalBindings.contains(nextIndiName + "\t0")) {
										debug = true;
									}

									if (!itemBindingsStringSet.contains(nextIndiName + "\t0")) {
										debug = true;
									}


									if (debug) {
										bool debug = true;
										initializeRealizationInstancesIterators(conceptItem, -1, mAnswererContext);
										CRealizationIndividualInstanceItemReferenceIterator* iteratorCopy2 = iteratorCopy->getCopy();
										iteratorCopy->moveNext();
										iteratorCopy2->moveNext();
									}
								}

							}
						}



						initializeRealizationInstancesIterators(conceptItem, -1, mAnswererContext);


						CRealizationIndividualInstanceItemReferenceIterator* iterator = conceptItem->getRealizationIterator();
						if (iterator) {
							iterator->begin();
							COptimizedComplexConceptInstanziatedIndividualItemHash* directInstanceItems = new COptimizedComplexConceptInstanziatedIndividualItemHash();
							QSet<CRealizationIndividualInstanceItemReference>* possiblenstanceItemSet = conceptItem->getPossibleInstanceItemSet();
							cint64 visitedIndis = 0;
							while (!iterator->atEnd()) {

								CRealizationIndividualInstanceItemReference indiItemRef = iterator->currentIndividualInstanceItemReference();
								QString indiName = CIRIName::getRecentIRIName(indiItemRef.getIndividual()->getIndividualNameLinker());


								cint64 nextMissingIndiId = -1;
								QMap<cint64, CIndividual*>::iterator it = missingIndiIdMap.upperBound(indiItemRef.getIndividualID());
								if (it != missingIndiIdMap.end()) {
									nextMissingIndiId = it.key();
								}

								CRealizationIndividualInstanceItemReferenceIterator* iteratorCopy = iterator->getCopy();
								iterator->moveNext();
								if (!iterator->atEnd()) {
									CRealizationIndividualInstanceItemReference nextIndiItemRef = iterator->currentIndividualInstanceItemReference();
									bool debug = false;
									if (nextIndiItemRef.getIndividualID() > nextMissingIndiId && nextMissingIndiId >= 0) {
										debug = true;
									}

									QString nextIndiName = CIRIName::getRecentIRIName(nextIndiItemRef.getIndividual()->getIndividualNameLinker());
									if (itemAdditionalBindings.contains(nextIndiName + "\t0")) {
										debug = true;
									}

									if (debug) {
										bool debug = true;
										initializeRealizationInstancesIterators(conceptItem, -1, mAnswererContext);
										CRealizationIndividualInstanceItemReferenceIterator* iteratorCopy2 = iteratorCopy->getCopy();
										iteratorCopy->moveNext();
										iteratorCopy2->moveNext();
									}
								}

							}
						}


					}

					if (sortArray) {
						delete[] sortArray;
					}
				}

				return true;
			}



			bool COptimizedComplexExpressionAnsweringHandler::debugCheckVariableMappingContainsSolution(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp, COptimizedComplexVariableCompositionItem* varCompItem) {
				QHash<CExpressionVariable*, COptimizedComplexConceptItem*>* varConItemHash = buildingVarItem->getVariableConceptItemHash();
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(varExp);


				QHash<QString, QString> varSolutionHash;
				varSolutionHash.insert("X", "http://www.Department0.University0.edu/GraduateStudent112");
				varSolutionHash.insert("Y", "http://www.Department0.University0.edu/AssociateProfessor9");
				varSolutionHash.insert("Z", "http://www.Department0.University0.edu/GraduateCourse31");

				COptimizedComplexVariableCompositionItemVariableIndexMapping* itemMapping = varItemIndexMapping.value(varCompItem);
				QVector<QString> nameStringVec(itemMapping->count());

				for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemMapping->constBegin(), itEnd = itemMapping->constEnd(); it != itEnd; ++it) {
					cint64 idx = it.key();
					CExpressionVariable* expression = it.value();
					QString varName = expression->getName();
					nameStringVec[idx] = varName;
				}

				bool contained = false;
				COptimizedComplexVariableIndividualMappings* varMapping = varCompItem->getVariableMapping();
				for (COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = varMapping->getLastAddedBindingsCardinalityLinker(); linker; linker = linker->getNext()) {
					COptimizedComplexVariableIndividualBindings* binding = linker->getBindings();

					bool allVariableBindingsMatchSolution = true;
					for (cint64 i = 0; allVariableBindingsMatchSolution && i < itemMapping->count(); ++i) {



						QString varString = nameStringVec[i];
						if (varSolutionHash.contains(varString)) {
							QString solution = varSolutionHash[varString];

							COptimizedComplexVariableIndividualMappings::VARIABLE_TYPE bindingType = varMapping->getBindingMapping(i);
							if (bindingType == COptimizedComplexVariableIndividualMappings::INDIVIDUAL_VARIABLE) {

								CRealizationIndividualInstanceItemReference instItemRef = binding->getBinding(i).reference;
								COptimizedKPSetIndividualItem* indiItem = (COptimizedKPSetIndividualItem*)instItemRef.getRealizationInstanceItem();
								CIndividual* individual = indiItem->getIndividual();
								QString individualString = QString::number(indiItem->getIndividualId());
								if (individual) {
									individualString = CIRIName::getRecentIRIName(individual->getIndividualNameLinker());
								} else {
									individualString = mOntoAnsweringItem->getOntology()->getIndividualNameResolver()->getIndividualName(indiItem->getIndividualReference());
								}

								if (individualString != solution) {
									allVariableBindingsMatchSolution = false;
								}

							} else if (bindingType == COptimizedComplexVariableIndividualMappings::DATA_LITERAL_VARIABLE) {

								CDataLiteral* dataLiteral = (CDataLiteral*)binding->getBinding(i).pointer;
								QString literalString = "\"" + dataLiteral->getDataLiteralValue()->getValueString() + "\"^^" + dataLiteral->getDatatype()->getDatatypeIRI();
								if (literalString != solution) {
									allVariableBindingsMatchSolution = false;
								}

							}


						}
					}

					if (allVariableBindingsMatchSolution) {
						contained = true;
						break;
					}

				}
				if (!contained) {
					bool debug = false;
					if (debug) {
						debugCheckVariableMappingContainsSolution(buildingVarItem, varExp, varCompItem);
					}
				}
				return contained;

			}




			QStringList COptimizedComplexExpressionAnsweringHandler::debugGetItemVariableNames(COptimizedComplexBuildingVariableCompositionsItem* buildVarItem, CExpressionVariable* varExp, COptimizedComplexVariableCompositionItem* varCompItem) {
				QStringList varNameList;
				QHash<CExpressionVariable*, COptimizedComplexConceptItem*>* varConItemHash = buildVarItem->getVariableConceptItemHash();
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildVarItem->getVariableExpressionVariableCompositionItemHash();
				COptimizedComplexVariableCompositionItemVariableIndexMapping* itemMapping = nullptr;
				if (varExp && varCompItem) {
					COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildVarItem->getVariableItemIndexMapping(varExp);
					itemMapping = varItemIndexMapping.value(varCompItem);
				}
#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				else {
					itemMapping = varCompItem->debugVarItemIndexMapping;
				}
#endif


				if (itemMapping) {
					QVector<QString> nameStringVec(itemMapping->count());

					for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemMapping->constBegin(), itEnd = itemMapping->constEnd(); it != itEnd; ++it) {
						cint64 idx = it.key();
						CExpressionVariable* expression = it.value();
						QString varName = expression->getName();
						nameStringVec[idx] = varName;
					}
					for (cint64 i = 0; i < itemMapping->count(); ++i) {
						varNameList.append(nameStringVec[i]);
					}
				} else {
					bool debug = true;
				}
				return varNameList;
			}




			bool COptimizedComplexExpressionAnsweringHandler::tryReuseVariableRolePropagationItem(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CIndividualVariableExpression* varExp, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping, CObjectPropertyAssertionExpression* propAss) {

				QHash<CExpressionVariable*, COptimizedComplexConceptItem*>* varConItemHash = buildingVarItem->getVariableConceptItemHash();
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(varExp);

				CRole* role = mOntoAnsweringItem->getOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(propAss->getObjectPropertyTermExpression());
				bool inversed = false;
				if (propAss->getFirstIndividualTermExpression() != varExp) {
					inversed = true;
				}
				CIndividualVariableExpression* otherExp = buildingVarItem->getOtherVariableExpression(varExp, propAss);
				if (!itemIndexMapping) {
					itemIndexMapping = varItemIndexMapping.value(varCompItem);
				}
				bool propVarExpAlreadyPresent = false;
				cint64 varIdx = 0;
				cint64 propVarIdx = itemIndexMapping->size();
				for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemIndexMapping->constBegin(), itEnd = itemIndexMapping->constEnd(); it != itEnd; ++it) {
					cint64 idx = it.key();
					CExpressionVariable* testVarExp = it.value();
					if (testVarExp == otherExp) {
						propVarExpAlreadyPresent = true;
						propVarIdx = idx;
					}
					else if (varExp == testVarExp) {
						varIdx = idx;
					}
				}

				COptimizedComplexVariableIndividualMappings* varMapping = varCompItem->getVariableMapping();

				if (varCompItem->hasRoleInversePropagationItem(role, inversed, varIdx)) {
					COptimizedComplexVariableRolePropagationItem* varRolePropItem = varCompItem->getRoleInversePropagationItem(role, inversed, varIdx);



					COptimizedComplexVariableCompositionItemVariableExpressionMapping& propVarItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(otherExp);
					COptimizedComplexVariableCompositionItemVariableIndexMapping* propItemIndexMapping = new COptimizedComplexVariableCompositionItemVariableIndexMapping();
					propVarItemIndexMapping.insertMulti(varRolePropItem, propItemIndexMapping);

					addUsedVariableCompositionItem(buildingVarItem, varRolePropItem);

					cint64 newIdx = itemIndexMapping->size();
					for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemIndexMapping->constBegin(), itEnd = itemIndexMapping->constEnd(); it != itEnd; ++it) {
						cint64 idx = it.key();
						CExpressionVariable* propVarExp = it.value();
						propItemIndexMapping->insert(idx, propVarExp);
					}
					propItemIndexMapping->insert(newIdx, otherExp);

					varExpVarComItemHash->value(otherExp)->insert(varRolePropItem);
					buildingVarItem->updateLastHandledVariableItemAssociation(otherExp, varRolePropItem);


#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
					QStringList debugVariableNamePrevItemStringList = debugGetItemVariableNames(buildingVarItem, varExp, varCompItem);
					QStringList debugVariableNamePropItemStringList = debugGetItemVariableNames(buildingVarItem, otherExp, varRolePropItem);
					varRolePropItem->debugVariableNameStringList = debugVariableNamePropItemStringList;
					QString roleName = CIRIName::getRecentIRIName(role->getPropertyNameLinker());
					if (inversed) {
						roleName = CIRIName::getRecentIRIName(role->getInverseRole()->getPropertyNameLinker()) + "^-";
					}
					varRolePropItem->debugCreationString = "Computation step " + QString::number(varRolePropItem->getComputationStepId()) + " with [" + debugVariableNamePropItemStringList.join(", ") + "] obtained by propagating " + varExp->getName() + " from [" + debugVariableNamePrevItemStringList.join(", ") + "] of step " + QString::number(varCompItem->getComputationStepId()) + " over role " + roleName;
					debugCreationStringList += varRolePropItem->debugCreationString;
					debugCreationString = debugCreationStringList.join("\r\n");
#endif


					return true;
				} else {
					return false;
				}
			}












			QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> COptimizedComplexExpressionAnsweringHandler::buildVariableSubSuperPropagationItem(COptimizedComplexBuildingPropertyVariableCompositionsItem* buildingVarItem, CPropertyVariableExpression* varExp, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping, CPropertyVariableExpression* otherVariable, bool superPropagation, bool* processing, CAnswererContext* answererContext) {
				QHash<CExpressionVariable*, COptimizedComplexConceptItem*>* varConItemHash = buildingVarItem->getVariableConceptItemHash();
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(varExp);

				if (!itemIndexMapping) {
					itemIndexMapping = varItemIndexMapping.value(varCompItem);
				}
				bool propVarExpAlreadyPresent = false;
				cint64 varIdx = 0;
				cint64 propVarIdx = itemIndexMapping->size();
				for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemIndexMapping->constBegin(), itEnd = itemIndexMapping->constEnd(); it != itEnd; ++it) {
					cint64 idx = it.key();
					CExpressionVariable* testVarExp = it.value();
					if (testVarExp == otherVariable) {
						propVarExpAlreadyPresent = true;
						propVarIdx = idx;
					} else if (varExp == testVarExp) {
						varIdx = idx;
					}
				}

				COptimizedComplexVariableIndividualMappings* varMapping = varCompItem->getVariableMapping();

				COptimizedComplexVariableSubSuperPropertiesPropagationItem*& varSubSuperPropertiesPropItem = varCompItem->getSubSuperPropertiesPropagationItem(superPropagation, varIdx);
				bool created = false;
				if (!varSubSuperPropertiesPropItem) {
					varSubSuperPropertiesPropItem = new COptimizedComplexVariableSubSuperPropertiesPropagationItem(varCompItem, superPropagation, varIdx);
					created = true;
					varSubSuperPropertiesPropItem->setVariableMappingsComputationRequirement(mConfMinimalMappingsComputationSize);
					varSubSuperPropertiesPropItem->setComputationStepId(mOntoAnsweringItem->getNextComputationStepId());
				}
				addUsedVariableCompositionItem(buildingVarItem, varSubSuperPropertiesPropItem);
				if (!varSubSuperPropertiesPropItem->isVariableMappingsInitialized()) {
					varSubSuperPropertiesPropItem->setVariableMappingsInitialized(true);
					// schedule item processing
					queueVariableItemComputation(buildingVarItem, varSubSuperPropertiesPropItem);
					if (processing) {
						*processing = true;
					}

				}



				COptimizedComplexVariableCompositionItemVariableExpressionMapping& propVarItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(otherVariable);
				COptimizedComplexVariableCompositionItemVariableIndexMapping* extendItemIndexMapping = new COptimizedComplexVariableCompositionItemVariableIndexMapping();
				propVarItemIndexMapping.insertMulti(varSubSuperPropertiesPropItem, extendItemIndexMapping);

				cint64 newIdx = itemIndexMapping->size();
				COptimizedComplexVariableIndividualMappings* propVarMapping = nullptr;
				if (created) {
					propVarMapping = new COptimizedComplexVariableIndividualMappingsHash(newIdx + 1);
				}
				for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemIndexMapping->constBegin(), itEnd = itemIndexMapping->constEnd(); it != itEnd; ++it) {
					cint64 idx = it.key();
					CExpressionVariable* propVarExp = it.value();
					extendItemIndexMapping->insert(idx, propVarExp);
					if (created) {
						propVarMapping->setBindingMapping(idx, varMapping->getBindingMapping(idx));
					}
				}
				extendItemIndexMapping->insert(newIdx, otherVariable);
				if (created) {
					propVarMapping->setBindingMapping(newIdx, COptimizedComplexVariableIndividualMappings::PROPERTY_VARIABLE);
					varSubSuperPropertiesPropItem->setVariableMapping(propVarMapping);
				}



#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				QStringList debugVariableNamePrevItemStringList = debugGetItemVariableNames(buildingVarItem, varExp, varCompItem);
#endif

				varExpVarComItemHash->value(otherVariable)->insert(varSubSuperPropertiesPropItem);
				buildingVarItem->updateLastHandledVariableItemAssociation(otherVariable, varSubSuperPropertiesPropItem);


#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				QStringList debugVariableNameNewItemStringList = debugGetItemVariableNames(buildingVarItem, varExp, varSubSuperPropertiesPropItem);
				varSubSuperPropertiesPropItem->debugVariableNameStringList = debugVariableNameNewItemStringList;
				QString propName = "super";
				if (!superPropagation) {
					propName = "sub";
				}
				varSubSuperPropertiesPropItem->debugCreationString = "Computation step " + QString::number(varSubSuperPropertiesPropItem->getComputationStepId()) + " with [" + debugVariableNameNewItemStringList.join(", ") + "] obtained by propagating " + varExp->getName() + " from [" + debugVariableNamePrevItemStringList.join(", ") + "] of step " + QString::number(varCompItem->getComputationStepId()) + " with " + propName + " properties propagation";
				debugCreationStringList += varSubSuperPropertiesPropItem->debugCreationString;
				debugCreationString = debugCreationStringList.join("\r\n");
#endif


				return QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>(varSubSuperPropertiesPropItem, extendItemIndexMapping);
			}








			QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> COptimizedComplexExpressionAnsweringHandler::buildVariableSubSuperPropagationItem(COptimizedComplexBuildingClassVariableCompositionsItem* buildingVarItem, CClassVariableExpression* varExp, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping, CClassVariableExpression* otherVariable, bool superPropagation, bool* processing, CAnswererContext* answererContext) {
				QHash<CExpressionVariable*, COptimizedComplexConceptItem*>* varConItemHash = buildingVarItem->getVariableConceptItemHash();
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(varExp);

				if (!itemIndexMapping) {
					itemIndexMapping = varItemIndexMapping.value(varCompItem);
				}
				bool propVarExpAlreadyPresent = false;
				cint64 varIdx = 0;
				cint64 propVarIdx = itemIndexMapping->size();
				for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemIndexMapping->constBegin(), itEnd = itemIndexMapping->constEnd(); it != itEnd; ++it) {
					cint64 idx = it.key();
					CExpressionVariable* testVarExp = it.value();
					if (testVarExp == otherVariable) {
						propVarExpAlreadyPresent = true;
						propVarIdx = idx;
					} else if (varExp == testVarExp) {
						varIdx = idx;
					}
				}

				COptimizedComplexVariableIndividualMappings* varMapping = varCompItem->getVariableMapping();

				COptimizedComplexVariableSubSuperClassesPropagationItem*& varSubSuperClassesPropItem = varCompItem->getSubSuperClassesPropagationItem(superPropagation, varIdx);
				bool created = false;
				if (!varSubSuperClassesPropItem) {
					varSubSuperClassesPropItem = new COptimizedComplexVariableSubSuperClassesPropagationItem(varCompItem, superPropagation, varIdx);
					created = true;
					varSubSuperClassesPropItem->setVariableMappingsComputationRequirement(mConfMinimalMappingsComputationSize);
					varSubSuperClassesPropItem->setComputationStepId(mOntoAnsweringItem->getNextComputationStepId());
				}
				addUsedVariableCompositionItem(buildingVarItem, varSubSuperClassesPropItem);
				if (!varSubSuperClassesPropItem->isVariableMappingsInitialized()) {
					varSubSuperClassesPropItem->setVariableMappingsInitialized(true);
					// schedule item processing
					queueVariableItemComputation(buildingVarItem, varSubSuperClassesPropItem);
					if (processing) {
						*processing = true;
					}

				}



				COptimizedComplexVariableCompositionItemVariableExpressionMapping& propVarItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(otherVariable);
				COptimizedComplexVariableCompositionItemVariableIndexMapping* extendItemIndexMapping = new COptimizedComplexVariableCompositionItemVariableIndexMapping();
				propVarItemIndexMapping.insertMulti(varSubSuperClassesPropItem, extendItemIndexMapping);

				cint64 newIdx = itemIndexMapping->size();
				COptimizedComplexVariableIndividualMappings* propVarMapping = nullptr;
				if (created) {
					propVarMapping = new COptimizedComplexVariableIndividualMappingsHash(newIdx + 1);
				}
				for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemIndexMapping->constBegin(), itEnd = itemIndexMapping->constEnd(); it != itEnd; ++it) {
					cint64 idx = it.key();
					CExpressionVariable* propVarExp = it.value();
					extendItemIndexMapping->insert(idx, propVarExp);
					if (created) {
						propVarMapping->setBindingMapping(idx, varMapping->getBindingMapping(idx));
					}
				}
				extendItemIndexMapping->insert(newIdx, otherVariable);
				if (created) {
					propVarMapping->setBindingMapping(newIdx, COptimizedComplexVariableIndividualMappings::CLASS_VARIABLE);
					varSubSuperClassesPropItem->setVariableMapping(propVarMapping);
				}



#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				QStringList debugVariableNamePrevItemStringList = debugGetItemVariableNames(buildingVarItem, varExp, varCompItem);
#endif

				varExpVarComItemHash->value(otherVariable)->insert(varSubSuperClassesPropItem);
				buildingVarItem->updateLastHandledVariableItemAssociation(otherVariable, varSubSuperClassesPropItem);


#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				QStringList debugVariableNameNewItemStringList = debugGetItemVariableNames(buildingVarItem, varExp, varSubSuperClassesPropItem);
				varSubSuperClassesPropItem->debugVariableNameStringList = debugVariableNameNewItemStringList;
				QString propName = "super";
				if (!superPropagation) {
					propName = "sub";
				}
				varSubSuperClassesPropItem->debugCreationString = "Computation step " + QString::number(varSubSuperClassesPropItem->getComputationStepId()) + " with [" + debugVariableNameNewItemStringList.join(", ") + "] obtained by propagating " + varExp->getName() + " from [" + debugVariableNamePrevItemStringList.join(", ") + "] of step " + QString::number(varCompItem->getComputationStepId()) + " to " + propName + " classes propagation";
				debugCreationStringList += varSubSuperClassesPropItem->debugCreationString;
				debugCreationString = debugCreationStringList.join("\r\n");
#endif


				return QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>(varSubSuperClassesPropItem, extendItemIndexMapping);
			}










			QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> COptimizedComplexExpressionAnsweringHandler::buildVariableRolePropagationItem(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CIndividualVariableExpression* varExp, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping, CDataLiteralVariableExpression* dataVariable, CDataPropertyTermExpression* dataPropTerm, bool* processing, CAnswererContext* answererContext) {
				QHash<CExpressionVariable*, COptimizedComplexConceptItem*>* varConItemHash = buildingVarItem->getVariableConceptItemHash();
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(varExp);

				CRole* dataRole = mOntoAnsweringItem->getOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getDataPropertyTermRoleMappingHash()->value(dataPropTerm);
				if (!itemIndexMapping) {
					itemIndexMapping = varItemIndexMapping.value(varCompItem);
				}
				bool propVarExpAlreadyPresent = false;
				cint64 varIdx = 0;
				cint64 propVarIdx = itemIndexMapping->size();
				for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemIndexMapping->constBegin(), itEnd = itemIndexMapping->constEnd(); it != itEnd; ++it) {
					cint64 idx = it.key();
					CExpressionVariable* testVarExp = it.value();
					if (testVarExp == dataVariable) {
						propVarExpAlreadyPresent = true;
						propVarIdx = idx;
					}
					else if (varExp == testVarExp) {
						varIdx = idx;
					}
				}

				COptimizedComplexVariableIndividualMappings* varMapping = varCompItem->getVariableMapping();

				COptimizedComplexVariableDataLiteralExtensionItem*& varDataRoleExtItem = varCompItem->getDataLiteralExtensionItem(dataRole, varIdx);
				bool created = false;
				if (!varDataRoleExtItem) {
					varDataRoleExtItem = new COptimizedComplexVariableDataLiteralExtensionItem(varCompItem, dataRole, varIdx);
					created = true;
					varDataRoleExtItem->setVariableMappingsComputationRequirement(mConfMinimalMappingsComputationSize);
					varDataRoleExtItem->setComputationStepId(mOntoAnsweringItem->getNextComputationStepId());
				}
				addUsedVariableCompositionItem(buildingVarItem, varDataRoleExtItem);
				if (!varDataRoleExtItem->isVariableMappingsInitialized()) {
					varDataRoleExtItem->setVariableMappingsInitialized(true);
					// schedule item processing
					queueVariableItemComputation(buildingVarItem, varDataRoleExtItem);
					if (processing) {
						*processing = true;
					}

				}



				COptimizedComplexVariableCompositionItemVariableIndexMapping* extendItemIndexMapping = new COptimizedComplexVariableCompositionItemVariableIndexMapping();

				cint64 newIdx = itemIndexMapping->size();
				COptimizedComplexVariableIndividualMappings* propVarMapping = nullptr;
				if (created) {
					propVarMapping = new COptimizedComplexVariableIndividualMappingsHash(newIdx + 1);
				}
				for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemIndexMapping->constBegin(), itEnd = itemIndexMapping->constEnd(); it != itEnd; ++it) {
					cint64 idx = it.key();
					CExpressionVariable* propVarExp = it.value();
					extendItemIndexMapping->insert(idx, propVarExp);
					if (created) {
						propVarMapping->setBindingMapping(idx, varMapping->getBindingMapping(idx));
					}
				}
				extendItemIndexMapping->insert(newIdx, dataVariable);
				if (created) {
					propVarMapping->setBindingMapping(newIdx, COptimizedComplexVariableIndividualMappings::DATA_LITERAL_VARIABLE);
					varDataRoleExtItem->setVariableMapping(propVarMapping);
				}



#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				QStringList debugVariableNamePrevItemStringList = debugGetItemVariableNames(buildingVarItem, varExp, varCompItem);
#endif

				varItemIndexMapping.insert(varDataRoleExtItem, extendItemIndexMapping);
				varExpVarComItemHash->value(varExp)->insert(varDataRoleExtItem);


#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				QStringList debugVariableNameNewItemStringList = debugGetItemVariableNames(buildingVarItem, varExp, varDataRoleExtItem);
				varDataRoleExtItem->debugVariableNameStringList = debugVariableNameNewItemStringList;
				QString roleName = CIRIName::getRecentIRIName(dataRole->getPropertyNameLinker());
				varDataRoleExtItem->debugCreationString = "Computation step " + QString::number(varDataRoleExtItem->getComputationStepId()) + " with [" + debugVariableNameNewItemStringList.join(", ") + "] obtained by extending " + varExp->getName() + " from [" + debugVariableNamePrevItemStringList.join(", ") + "] of step " + QString::number(varCompItem->getComputationStepId()) + " with data values for data role " + roleName;
				debugCreationStringList += varDataRoleExtItem->debugCreationString;
				debugCreationString = debugCreationStringList.join("\r\n");
#endif

				buildingVarItem->updateLastHandledVariableItemAssociation(varExp, varDataRoleExtItem);

				return QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>(varDataRoleExtItem, extendItemIndexMapping);
			}









			QHash<CExpressionVariable*, cint64> COptimizedComplexExpressionAnsweringHandler::getPropagationReducableVariables(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CExpressionVariable* propVarExp, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping, CObjectPropertyAssertionExpression* propAss) {
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(propVarExp);

				if (!itemIndexMapping) {
					itemIndexMapping = varItemIndexMapping.value(varCompItem);
				}
				QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* absorbingQueryPartsList = buildingVarItem->getAbsorbingQueryPartsList();
				QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* absorbedQueryPartsItemExtensionList = buildingVarItem->getAbsorbedQueryPartItemExtensionHandlingList();

				QHash<CExpressionVariable*, cint64> reductionVarIdxSet;
				if (mConfAllowBindingReduction && buildingVarItem->isBindingsReducible() && varCompItem->getVariableMappingsExpectedCount() > mConfMappingSizeBindingReduction && absorbedQueryPartsItemExtensionList->isEmpty() && absorbingQueryPartsList->isEmpty()) {
					// test whether there is no absorption propagation
					// test whether there exists a variable that is not distinguished, has no remaining properties, and is not used by a remaining item for any variable
					QSet<CExpressionVariable*>* allVarExpSet = buildingVarItem->getAllVariableSet();
					QSet<CExpressionVariable*>* reductionDeniedVarExpSet = buildingVarItem->getReductionDeniedVariableSet();

					for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemIndexMapping->constBegin(), itEnd = itemIndexMapping->constEnd(); it != itEnd; ++it) {
						cint64 idx = it.key();
						CExpressionVariable* varExp = it.value();
						CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(varExp);

						if (!reductionDeniedVarExpSet->contains(varExp) && indiVarExp) {

							QList<CObjectPropertyAssertionExpression*> propAssList = buildingVarItem->getUnhanledPropertyAssertionsExpressions(indiVarExp);
							QList<CDataPropertyAssertionExpression*> dataPropAssList = buildingVarItem->getUnhanledDataPropertyAssertionsExpressions(indiVarExp);
							if (dataPropAssList.isEmpty() && (propVarExp != varExp && propAssList.isEmpty() || propAssList.size() == 1 && propAssList.first() == propAss)) {

								bool varExpStillRequired = false;
								for (CExpressionVariable* otherVarExp : *allVarExpSet) {



									CIndividualVariableExpression* otherIndiVarExp = dynamic_cast<CIndividualVariableExpression*>(otherVarExp);
									if (otherIndiVarExp) {
										QList<CObjectPropertyAssertionExpression*> propAssList = buildingVarItem->getUnhanledPropertyAssertionsExpressions(otherIndiVarExp);
										QList<CDataPropertyAssertionExpression*> dataPropAssList = buildingVarItem->getUnhanledDataPropertyAssertionsExpressions(otherIndiVarExp);
										QSet<COptimizedComplexVariableCompositionItem*>* remainingItemSet = varExpVarComItemHash->value(otherVarExp);

										if (remainingItemSet) {
											bool investigateVarItemMappings = false;
											if (!dataPropAssList.isEmpty() || !propAssList.isEmpty() || remainingItemSet->size() > 1) {
												investigateVarItemMappings = true;
											}
											if (!investigateVarItemMappings && remainingItemSet->size() == 1) {
												QList<COptimizedComplexVariableCompositionItemVariableIndexMapping*> firstOtherItemIndexMappings = buildingVarItem->getVariableItemIndexMapping(otherVarExp).values(*remainingItemSet->begin());
												if (firstOtherItemIndexMappings.size() > 1) {
													investigateVarItemMappings = true;
												}
											}

											if (investigateVarItemMappings) {
												for (COptimizedComplexVariableCompositionItem* otherVarCompItem : *remainingItemSet) {
													if (otherVarCompItem) {
														QList<COptimizedComplexVariableCompositionItemVariableIndexMapping*> otherItemIndexMappings = buildingVarItem->getVariableItemIndexMapping(otherVarExp).values(otherVarCompItem);
														for (COptimizedComplexVariableCompositionItemVariableIndexMapping* otherItemIndexMapping : otherItemIndexMappings) {
															if (otherItemIndexMapping && (otherVarCompItem != varCompItem || otherItemIndexMapping != itemIndexMapping)) {
																for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator otherIt = otherItemIndexMapping->constBegin(), otherItEnd = otherItemIndexMapping->constEnd(); otherIt != otherItEnd; ++otherIt) {
																	CExpressionVariable* testVarExp = otherIt.value();
																	if (testVarExp == varExp) {
																		varExpStillRequired = true;
																		break;
																	}
																}
															}
															if (varExpStillRequired) {
																break;
															}
														}
													}
													if (varExpStillRequired) {
														break;
													}
												}
											}
										}
									}
									if (varExpStillRequired) {
										break;
									}

									//QSet<COptimizedComplexVariableCompositionItem*>* remainingItemSet = varExpVarComItemHash->value(otherVarExp);
									//for (COptimizedComplexVariableCompositionItem* remainingItem : *remainingItemSet) {
									//}
									//CExpressionVariable* associatedVariableExpression = buildingVarItem->getVariableLastCompositionItemAssociatedVariableExpression(otherVarExp);

									//COptimizedComplexVariableCompositionItem* otherVarCompItem = buildingVarItem->getVariableLastCompositionItem(otherVarExp);
									//if (otherVarCompItem != varCompItem && otherVarCompItem) {
									//	COptimizedComplexVariableCompositionItemVariableIndexMapping* otherItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(otherVarExp).value(otherVarCompItem);
									//	if (otherItemIndexMapping) {
									//		for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator otherIt = otherItemIndexMapping->constBegin(), otherItEnd = otherItemIndexMapping->constEnd(); otherIt != otherItEnd && !varExpStillRequired; ++otherIt) {
									//			CExpressionVariable* testVarExp = otherIt.value();
									//			if (testVarExp == varExp) {
									//				varExpStillRequired = true;
									//			}
									//		}
									//	}
									//}
									//if (varExpStillRequired) {
									//	break;
									//}
								}

								if (!varExpStillRequired) {
									reductionVarIdxSet.insertMulti(varExp, idx);
								}

							}

						}
					}
				}
				return reductionVarIdxSet;
			}




			QHash<CExpressionVariable*, cint64> COptimizedComplexExpressionAnsweringHandler::getReducableVariables(COptimizedComplexBuildingVariableCompositionsItem* buildVarItem, CExpressionVariable* propVarExp, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping) {
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildVarItem->getVariableExpressionVariableCompositionItemHash();
				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildVarItem->getVariableItemIndexMapping(propVarExp);

				QHash<CExpressionVariable*, cint64> reductionVarIdxSet;
				if (!itemIndexMapping) {
					itemIndexMapping = varItemIndexMapping.value(varCompItem);
				}

				COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingIndiVarItem = dynamic_cast<COptimizedComplexBuildingIndividualVariableCompositionsItem*>(buildVarItem);
				if (buildingIndiVarItem) {
					QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* absorbedQueryPartsItemExtensionList = buildingIndiVarItem->getAbsorbedQueryPartItemExtensionHandlingList();



					if (mConfAllowBindingReduction && buildingIndiVarItem->isBindingsReducible() && varCompItem->getVariableMappingsExpectedCount() >= mConfMappingSizeBindingReduction && absorbedQueryPartsItemExtensionList->isEmpty()) {
						// test whether there is no absorption propagation
						// test whether there exists a variable that is not distinguished, has no remaining properties, and is not used by a remaining item for any variable
						QSet<CExpressionVariable*>* allVarExpSet = buildingIndiVarItem->getAllVariableSet();
						QSet<CExpressionVariable*>* reductionDeniedVarExpSet = buildingIndiVarItem->getReductionDeniedVariableSet();

						QHash<CExpressionVariable*, cint64> varUseCountHash;

						for (CExpressionVariable* varExp : *allVarExpSet) {

							CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(varExp);
							if (indiVarExp) {
								QList<CObjectPropertyAssertionExpression*> propAssList = buildingIndiVarItem->getUnhanledPropertyAssertionsExpressions(indiVarExp);
								QList<CDataPropertyAssertionExpression*> dataPropAssList = buildingIndiVarItem->getUnhanledDataPropertyAssertionsExpressions(indiVarExp);
								QSet<COptimizedComplexVariableCompositionItem*>* remainingItemSet = varExpVarComItemHash->value(varExp);
								if (remainingItemSet) {
									bool investigateVarItemMappings = false;
									if (!dataPropAssList.isEmpty() || !propAssList.isEmpty()) {
										varUseCountHash[varExp] += 2;
										investigateVarItemMappings = true;
									}
									if (remainingItemSet->size() > 1) {
										investigateVarItemMappings = true;
									}
									if (!investigateVarItemMappings && remainingItemSet->size() == 1) {
										QList<COptimizedComplexVariableCompositionItemVariableIndexMapping*> firstOtherItemIndexMappings = buildingIndiVarItem->getVariableItemIndexMapping(varExp).values(*remainingItemSet->begin());
										if (firstOtherItemIndexMappings.size() > 1) {
											investigateVarItemMappings = true;
										}
									}

									if (investigateVarItemMappings) {
										for (COptimizedComplexVariableCompositionItem* remainingItem : *remainingItemSet) {
											if (remainingItem) {
												QList<COptimizedComplexVariableCompositionItemVariableIndexMapping*> otherItemIndexMappings = buildingIndiVarItem->getVariableItemIndexMapping(varExp).values(remainingItem);
												for (COptimizedComplexVariableCompositionItemVariableIndexMapping* otherItemIndexMapping : otherItemIndexMappings) {
													for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator otherIt = otherItemIndexMapping->constBegin(), otherItEnd = otherItemIndexMapping->constEnd(); otherIt != otherItEnd; ++otherIt) {
														CExpressionVariable* otherVarExp = otherIt.value();
														varUseCountHash[otherVarExp]++;
													}
												}
											}
										}
									}
								}

							}
						}


						for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemIndexMapping->constBegin(), itEnd = itemIndexMapping->constEnd(); it != itEnd; ++it) {
							cint64 idx = it.key();
							CExpressionVariable* varExp = it.value();
							CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(varExp);

							if (!reductionDeniedVarExpSet->contains(varExp) && indiVarExp) {

								cint64 useCount = varUseCountHash.value(varExp);
								if (useCount <= 1) {
									reductionVarIdxSet.insertMulti(varExp, idx);
								}
							}
						}
					}
				}

				COptimizedComplexBuildingClassVariableCompositionsItem* buildingClassVarItem = dynamic_cast<COptimizedComplexBuildingClassVariableCompositionsItem*>(buildVarItem);
				if (buildingClassVarItem) {

					if (mConfAllowBindingReduction && varCompItem->getVariableMappingsExpectedCount() >= mConfMappingSizeBindingReduction) {
						// test whether there is no absorption propagation
						// test whether there exists a variable that is not distinguished, has no remaining properties, and is not used by a remaining item for any variable
						QSet<CClassVariableExpression*>* allVarExpSet = buildingClassVarItem->getAllVariableSet();


						QHash<CExpressionVariable*, cint64> varUseCountHash;

						for (CClassVariableExpression* varExp : *allVarExpSet) {

							QSet<QPair<CExpressionVariable*, CExpressionVariable*>> remSubSuperPropSet = buildingClassVarItem->getVariableRemainingSubSuperPropagationSet(varExp);
							QSet<COptimizedComplexVariableCompositionItem*>* remainingItemSet = varExpVarComItemHash->value(varExp);

							if (remainingItemSet) {
								bool investigateVarItemMappings = false;
								if (!remSubSuperPropSet.isEmpty()) {
									varUseCountHash[varExp] += 2;
									investigateVarItemMappings = true;
								}
								if (remainingItemSet->size() > 1) {
									investigateVarItemMappings = true;
								}
								if (!investigateVarItemMappings && remainingItemSet->size() == 1) {
									QList<COptimizedComplexVariableCompositionItemVariableIndexMapping*> firstOtherItemIndexMappings = buildingIndiVarItem->getVariableItemIndexMapping(varExp).values(*remainingItemSet->begin());
									if (firstOtherItemIndexMappings.size() > 1) {
										investigateVarItemMappings = true;
									}
								}

								if (investigateVarItemMappings) {
									for (COptimizedComplexVariableCompositionItem* remainingItem : *remainingItemSet) {
										if (remainingItem) {
											QList<COptimizedComplexVariableCompositionItemVariableIndexMapping*> otherItemIndexMappings = buildingIndiVarItem->getVariableItemIndexMapping(varExp).values(remainingItem);
											for (COptimizedComplexVariableCompositionItemVariableIndexMapping* otherItemIndexMapping : otherItemIndexMappings) {
												for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator otherIt = otherItemIndexMapping->constBegin(), otherItEnd = otherItemIndexMapping->constEnd(); otherIt != otherItEnd; ++otherIt) {
													CExpressionVariable* otherVarExp = otherIt.value();
													varUseCountHash[otherVarExp]++;
												}
											}
										}
									}
								}
							}

						}


						for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemIndexMapping->constBegin(), itEnd = itemIndexMapping->constEnd(); it != itEnd; ++it) {
							cint64 idx = it.key();
							CExpressionVariable* varExp = it.value();
							CClassVariableExpression* classVarExp = dynamic_cast<CClassVariableExpression*>(varExp);
							if (classVarExp) {
								cint64 useCount = varUseCountHash.value(varExp);
								if (useCount <= 1) {
									reductionVarIdxSet.insertMulti(varExp, idx);
								}
							}
						}
					}
				}

				return reductionVarIdxSet;
			}


			CConcept* COptimizedComplexExpressionAnsweringHandler::getAbsorbExistentialQueryPartRestrictedTopPropagationConcept(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* data, CAnswererContext* answererContext) {
				CConcept* resTopPropCon = data->getRestrictedTopPropagationConcept();
				if (!resTopPropCon && !data->hasRestrictedTopPropagationConceptChecked()) {
					CConcept* initCon = data->getInitializerConcept();
					if (initCon) {
						CRole* topRole = mOntoAnsweringItem->getOntology()->getDataBoxes()->getRBox()->getTopObjectRole();
						QList<TConceptNegPair> invConNegList;
						invConNegList.append(TConceptNegPair(initCon, false));
						while (!invConNegList.isEmpty() && !resTopPropCon) {
							TConceptNegPair conNegPair = invConNegList.takeFirst();
							CConcept* concept = conNegPair.first;
							bool conNeg = conNegPair.second;
							CConceptOperator* conOp = concept->getConceptOperator();
							cint64 opCode = concept->getOperatorCode();
							if (!conNeg && conOp->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ALL_AQALL_TYPE)) {
								CRole* role = concept->getRole();
								if (role == topRole) {
									resTopPropCon = concept;
								}
							} else if (!conNeg && conOp->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_TRIG_AND_AQAND_TYPE)) {
								for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
									CConcept* opCon = opConLinkerIt->getData();
									bool opConNeg = opConLinkerIt->isNegated();
									invConNegList.append(TConceptNegPair(opCon, opConNeg));
								}
							}

						}
					}
					data->setRestrictedTopPropagationConcept(resTopPropCon);
					data->setRestrictedTopPropagationConceptChecked(true);
				}
				return resTopPropCon;
			}





			void COptimizedComplexExpressionAnsweringHandler::absorbExistentialQueryPart(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* data, CAnswererContext* answererContext) {
				QHash<CConcept*, COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* propagationFinalizationConceptAbsorptionDataHash = mOntoAnsweringItem->getPropagationFinalizationConceptAbsorptionDataHash();
				QStringList varList;
				if (mConfExtendedLogging) {
					for (CExpressionVariable* var : *data->getAbsorptionVariableSet()) {
						varList.append(var->getName());
					}
				}
				COccurrenceStatisticsCacheReader* occStatsCacheReader = answererContext->getAnsweringCalculationHandler()->getOccurrenceStatisticsCacheReader();
				if (mConfVariablePreAbsorption) {
					COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler preAbsorptionHandler(data, mOntoAnsweringItem, occStatsCacheReader);
					preAbsorptionHandler.readConfig(mOntoAnsweringItem->getCalculationConfiguration());
					cint64 prevConCount = mOntoAnsweringItem->getTestingOntology()->getTBox()->getNextConceptID();
					preAbsorptionHandler.absorbQueryPart();
					cint64 newConCount = mOntoAnsweringItem->getTestingOntology()->getTBox()->getNextConceptID();
					if (mConfExtendedLogging) {
						LOG(INFO, getDomain(), logTr("Preabsorbed existential query part consiting of variables %1 with %2 concepts.").arg(varList.join(", ")).arg(newConCount - prevConCount), this);
					}

				}

				COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler absorptionHandler(data, mOntoAnsweringItem, occStatsCacheReader);
				absorptionHandler.readConfig(mOntoAnsweringItem->getCalculationConfiguration());
				cint64 prevConCount = mOntoAnsweringItem->getTestingOntology()->getTBox()->getNextConceptID();
				absorptionHandler.absorbQueryPart();
				cint64 newConCount = mOntoAnsweringItem->getTestingOntology()->getTBox()->getNextConceptID();
				if (mConfExtendedLogging) {
					LOG(INFO, getDomain(), logTr("Absorbed existential query part consisting of variables %1 with %2 concepts (%3 is initialization and %4 is first binding variable).").arg(varList.join(", ")).arg(newConCount - prevConCount).arg(data->getInitializerVariableExpression()->getName()).arg(data->getBindingPropagationStartVariableExpression()->getName()), this);
				}

				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incExistentialQueryPartAbsorptionCount();


				CConcept* propFinalConcept = data->getPropagationFinalizationConcept();
				propagationFinalizationConceptAbsorptionDataHash->insert(propFinalConcept, data);
			}



			QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> COptimizedComplexExpressionAnsweringHandler::buildAbsorbedPartHandlingItem(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionHanldingQueryPart, QHash<CExpressionVariable *, QSet<COptimizedComplexVariableCompositionItem *> *>* varExpVarComItemHash, bool* processing) {
				COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationItem = new COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem(absorptionHanldingQueryPart);
				absorptionPropagationItem->setComputationStepId(mOntoAnsweringItem->getNextComputationStepId());
				QList<COptimizedComplexVariableCompositionItem*> computationDependentItemList;


				QSet<CExpressionVariable*>* preparationVarSet = absorptionHanldingQueryPart->getPrepareVariableSet();
				QSet<CExpressionVariable*>* indiVarSet = absorptionHanldingQueryPart->getIndividualBindingsVariableSet();

				QSet<CExpressionVariable*> useVarSet(*preparationVarSet);
				useVarSet = useVarSet.unite(*indiVarSet);

				QList<CExpressionVariable*> indiIntVarList = indiVarSet->toList();
				CExpressionVariable* initializationVar = absorptionHanldingQueryPart->getInitializerVariableExpression();
				if (!useVarSet.contains(initializationVar)) {
					indiIntVarList.append(initializationVar);
				}

				QHash<CExpressionVariable*, CVariable*>* variableExpressionVariableHash = absorptionHanldingQueryPart->getVariableExpressionVariableHash();




				QHash<CConcept*, COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*>* propagationFinalizationConceptAbsorptionItemHash = mOntoAnsweringItem->getPropagationFinalizationConceptAbsorptionItemHash();
				COptimizedComplexVariableCompositionItemVariableIndexMapping* extendItemIndexMapping = new COptimizedComplexVariableCompositionItemVariableIndexMapping();
				cint64 nextVariableIdx = 0;
				COptimizedComplexVariableIndividualMappings* newPropVarMapping = new COptimizedComplexVariableIndividualMappingsHash(indiVarSet->size());

				QHash<CVariable*, cint64>* variableIndexHash = absorptionPropagationItem->getVariableIndexHash();

				QSet<COptimizedComplexVariableCompositionItem*>* integratedItemSet = absorptionPropagationItem->getIntegratedVariableCompositionItemSet();

				CExpressionVariable* initializingVarExp = absorptionHanldingQueryPart->getInitializerVariableExpression();

#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				QStringList prevItemsStringList;
#endif

				cint64 omputationItemDepth = 0;

				for (CExpressionVariable* indiVarExp : useVarSet) {
					COptimizedComplexVariableCompositionItem* varCompItem = buildingVarItem->getVariableLastCompositionItem(indiVarExp);
					computationDependentItemList.append(varCompItem);
					varCompItem->addComputationSuccessorItem(absorptionPropagationItem);
					omputationItemDepth = qMax(omputationItemDepth, varCompItem->getComputationItemDepth() + 1);

					CExpressionVariable* associatedVariableExpression = buildingVarItem->getVariableLastCompositionItemAssociatedVariableExpression(indiVarExp);

					if (!integratedItemSet->contains(varCompItem) && indiVarSet->contains(indiVarExp)) {
						integratedItemSet->insert(varCompItem);
					}

					CVariable* variable = variableExpressionVariableHash->value(indiVarExp);
					absorptionPropagationItem->setVariableCompositionItem(variable, varCompItem);
					absorptionPropagationItem->setVariableExpressionCompositionItem(indiVarExp, varCompItem);

					propagationFinalizationConceptAbsorptionItemHash->insert(absorptionHanldingQueryPart->getPropagationFinalizationConcept(), absorptionPropagationItem);

					// in principle there can be several item index mappings, but at this stage they should already be joined, hence only one remaining for that item
					COptimizedComplexVariableCompositionItemVariableIndexMapping* varIndexMapping = buildingVarItem->getVariableItemIndexMapping(associatedVariableExpression).value(varCompItem);

					cint64 varPos = -1;
					if (varIndexMapping) {
						varPos = varIndexMapping->getSearchedVariablePosition(indiVarExp);
					}

					COptimizedComplexVariableIndividualMappings* varMapping = varCompItem->getVariableMapping();
					if (varMapping->getBindingCount() > 0) {
						if (indiVarSet->contains(indiVarExp)) {
							variableIndexHash->insertMulti(variable, nextVariableIdx);
							newPropVarMapping->setBindingMapping(nextVariableIdx, varMapping->getBindingMapping(varPos));
							extendItemIndexMapping->insert(nextVariableIdx++, indiVarExp);
						}
					} else {
						buildingVarItem->setSatisfiability(false);
					}


#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
					QStringList debugVariableNamePrevItemStringList = debugGetItemVariableNames(buildingVarItem, associatedVariableExpression, varCompItem);
					prevItemsStringList += "[" + debugVariableNamePrevItemStringList.join(", ") + "] of step " + QString::number(varCompItem->getComputationStepId());
#endif

				}
				absorptionPropagationItem->setVariableMapping(newPropVarMapping);
				absorptionPropagationItem->setComputationDependentItemList(computationDependentItemList);
				absorptionPropagationItem->setComputationItemDepth(omputationItemDepth);
				addUsedVariableCompositionItem(buildingVarItem, absorptionPropagationItem);

				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(initializingVarExp);
				varItemIndexMapping.insert(absorptionPropagationItem, extendItemIndexMapping);
				varExpVarComItemHash->value(initializingVarExp)->insert(absorptionPropagationItem);


#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				QStringList debugVariableNameNewItemStringList = debugGetItemVariableNames(buildingVarItem, initializingVarExp, absorptionPropagationItem);
				absorptionPropagationItem->debugVariableNameStringList = debugVariableNameNewItemStringList;
				absorptionPropagationItem->debugCreationString = QString("Computation step %1").arg(absorptionPropagationItem->getComputationStepId()) + " with [" + debugVariableNameNewItemStringList.join(", ") + "] absorption propagation based on { " + prevItemsStringList.join(" }, { ") + " } ";
				debugCreationStringList += absorptionPropagationItem->debugCreationString;
				debugCreationString = debugCreationStringList.join("\r\n");
#endif



				if (buildingVarItem->isSatisfiable()) {

					if (!absorptionPropagationItem->isVariableMappingsInitialized()) {
						absorptionPropagationItem->setVariableMappingsInitialized(true);
						// schedule item processing
						queueVariableItemComputation(buildingVarItem, absorptionPropagationItem);
						
						if (processing) {
							*processing = true;
						}
					}
				}
				return QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>(absorptionPropagationItem, extendItemIndexMapping);
			}




			QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> COptimizedComplexExpressionAnsweringHandler::buildQueryTransferringVariableExtractionItem(COptimizedComplexBuildingVariableCompositionsItem* extractionBuildingVarItem, CExpressionVariable* extractionVarExp, COptimizedComplexVariableCompositionItem* extractionVarCompItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* extractionItemIndexMapping, COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp, bool* processing) {
				COptimizedComplexVariableCompositionItemVariableExpressionMapping& extractionVarItemIndexMapping = extractionBuildingVarItem->getVariableItemIndexMapping(extractionVarExp);

				if (!extractionItemIndexMapping) {
					extractionItemIndexMapping = extractionVarItemIndexMapping.value(extractionVarCompItem);
				}

				cint64 extractionVarId = 0;
				for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = extractionItemIndexMapping->constBegin(), itEnd = extractionItemIndexMapping->constEnd(); it != itEnd; ++it) {
					cint64 varInd = it.key();
					CExpressionVariable* varExp = it.value();
					if (varExp == extractionVarExp) {
						extractionVarId = varInd;
						break;
					}
				}


				COptimizedComplexVariableExtractionItem*& varRedItem = extractionVarCompItem->getExtractionItem(extractionVarId);
				bool created = false;
				if (!varRedItem) {
					varRedItem = new COptimizedComplexVariableExtractionItem(extractionVarCompItem, extractionVarId);
					created = true;
					varRedItem->setVariableMappingsComputationRequirement(mConfMinimalMappingsComputationSize);
					varRedItem->setComputationStepId(mOntoAnsweringItem->getNextComputationStepId());
				}
				addUsedVariableCompositionItem(buildingVarItem, varRedItem);

				COptimizedComplexVariableCompositionItemVariableIndexMapping* redItemIndexMapping = new COptimizedComplexVariableCompositionItemVariableIndexMapping();
				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(varExp);
				redItemIndexMapping->insert(0, varExp);
				varItemIndexMapping.insertMulti(varRedItem, redItemIndexMapping);
#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				varRedItem->debugVarItemIndexMapping = redItemIndexMapping;
#endif

				COptimizedComplexVariableIndividualMappings* redVarMapping = nullptr;
				if (created) {
					redVarMapping = new COptimizedComplexVariableIndividualMappingsHash(1);
					COptimizedComplexVariableIndividualMappings* varMapping = extractionVarCompItem->getVariableMapping();
					redVarMapping->setBindingMapping(0, varMapping->getBindingMapping(extractionVarId));
				}



				if (!varRedItem->isVariableMappingsInitialized()) {
					varRedItem->setVariableMappingsInitialized(true);
					// schedule item processing
					queueVariableItemComputation(buildingVarItem, varRedItem);
					if (processing) {
						*processing = true;
					}
				}

				if (created) {
					varRedItem->setVariableMapping(redVarMapping);
				}

				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
				varExpVarComItemHash->value(varExp)->insert(varRedItem);


#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				QStringList debugVariableNamePrevItemStringList = debugGetItemVariableNames(extractionBuildingVarItem, extractionVarExp, extractionVarCompItem);
				QStringList debugVariableNameRedItemStringList = debugGetItemVariableNames(buildingVarItem, varExp, varRedItem);
				varRedItem->debugVariableNameStringList = debugVariableNameRedItemStringList;
				varRedItem->debugCreationString = "Computation step " + QString::number(varRedItem->getComputationStepId()) + " with [" + debugVariableNameRedItemStringList.join(", ") + "] obtained by extracting " + extractionVarExp->getName() + " from [" + debugVariableNamePrevItemStringList.join(", ") + "] of step " + QString::number(extractionVarCompItem->getComputationStepId());
				debugCreationStringList += varRedItem->debugCreationString;
				debugCreationString = debugCreationStringList.join("\r\n");
#endif


				buildingVarItem->updateLastHandledVariableItemAssociation(varExp, varRedItem);

				return QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>(varRedItem, redItemIndexMapping);
			}





			QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> COptimizedComplexExpressionAnsweringHandler::buildVariableReductionItem(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* propVarExp, bool allopReductionForVar, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping, QHash<CExpressionVariable*, cint64>& reductionVarExpIndHash, bool* processing) {
				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(propVarExp);

				if (!itemIndexMapping) {
					itemIndexMapping = varItemIndexMapping.value(varCompItem);
				}

				if (!reductionVarExpIndHash.isEmpty()) {

					QMap<cint64,cint64> reductionVarIdxMap;
					cint64 nextRedNumber = 0;
					CExpressionVariable* redExpVar = nullptr;
					for (QHash<CExpressionVariable*, cint64>::iterator it = reductionVarExpIndHash.begin(); it != reductionVarExpIndHash.end(); ) {
						CExpressionVariable* expVar = it.key();
						cint64 varIdx = it.value();
						if (allopReductionForVar || expVar != propVarExp) {
							reductionVarIdxMap.insert(varIdx, nextRedNumber++);
							it = reductionVarExpIndHash.erase(it);
							redExpVar = expVar;
						} else {
							++it;
						}
					}



					cint64 redNumber = 0;
					while (!reductionVarIdxMap.isEmpty()) {


						QMap<cint64, cint64>::iterator redIt = reductionVarIdxMap.begin();
						cint64 nextRedIdx = redIt.key();
						cint64 correctedRedIdx = nextRedIdx - redNumber++;
						redIt = reductionVarIdxMap.erase(redIt);


						COptimizedComplexVariableReductionItem*& varRedItem = varCompItem->getReductionItem(correctedRedIdx);
						bool created = false;
						if (!varRedItem) {
							varRedItem = new COptimizedComplexVariableReductionItem(varCompItem, correctedRedIdx);
							created = true;
							varRedItem->setVariableMappingsComputationRequirement(mConfMinimalMappingsComputationSize);
							varRedItem->setComputationStepId(mOntoAnsweringItem->getNextComputationStepId());
						}
						addUsedVariableCompositionItem(buildingVarItem, varRedItem);

						COptimizedComplexVariableCompositionItemVariableIndexMapping* redItemIndexMapping = new COptimizedComplexVariableCompositionItemVariableIndexMapping();
						varItemIndexMapping.insertMulti(varRedItem, redItemIndexMapping);
#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
						varRedItem->debugVarItemIndexMapping = redItemIndexMapping;
#endif

						cint64 newIdx = itemIndexMapping->size();
						CExpressionVariable* redVarExp = nullptr;
						COptimizedComplexVariableIndividualMappings* redVarMapping = nullptr;
						if (created) {
							redVarMapping = new COptimizedComplexVariableIndividualMappingsHash(newIdx - 1);
						}

						cint64 nextIdx = 0;
						COptimizedComplexVariableIndividualMappings* varMapping = varCompItem->getVariableMapping();
						for (cint64 i = 0; i < itemIndexMapping->size(); ++i) {
							CExpressionVariable* indVarExp = itemIndexMapping->value(i);
							if (i != correctedRedIdx) {
								redItemIndexMapping->insert(nextIdx, indVarExp);
								if (created) {
									redVarMapping->setBindingMapping(nextIdx++, varMapping->getBindingMapping(i));
								} else {
									nextIdx++;
								}
							} else {
								redVarExp = indVarExp;
							}
						}

						if (!varRedItem->isVariableMappingsInitialized()) {
							varRedItem->setVariableMappingsInitialized(true);
							// schedule item processing
							queueVariableItemComputation(buildingVarItem, varRedItem);
							if (processing) {
								*processing = true;
							}
						}

						if (created) {
							varRedItem->setVariableMapping(redVarMapping);
						}

						varExpVarComItemHash->value(propVarExp)->remove(varCompItem);
						varExpVarComItemHash->value(propVarExp)->insert(varRedItem);


#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
						QStringList debugVariableNamePrevItemStringList = debugGetItemVariableNames(buildingVarItem, propVarExp, varCompItem);
						QStringList debugVariableNameRedItemStringList = debugGetItemVariableNames(buildingVarItem, propVarExp, varRedItem);
						varRedItem->debugVariableNameStringList = debugVariableNameRedItemStringList;
						varRedItem->debugCreationString = "Computation step " + QString::number(varRedItem->getComputationStepId()) + " with [" + debugVariableNameRedItemStringList.join(", ") + "] obtained by reducing " + redVarExp->getName() + " from [" + debugVariableNamePrevItemStringList.join(", ") + "] of step " + QString::number(varCompItem->getComputationStepId());
						debugCreationStringList += varRedItem->debugCreationString;
						debugCreationString = debugCreationStringList.join("\r\n");
#endif


						buildingVarItem->updateLastHandledVariableItemAssociation(propVarExp, varRedItem);
						if (!allopReductionForVar) {
							buildingVarItem->clearLastHandledVariableItemAssociation(redExpVar);
						}

						varItemIndexMapping.removeItemVariableExpressionMapping(varCompItem, itemIndexMapping);

						varCompItem = varRedItem;
						itemIndexMapping = redItemIndexMapping;

					}
				}

				return QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>(varCompItem, itemIndexMapping);
			}




			QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> COptimizedComplexExpressionAnsweringHandler::buildVariableRolePropagationItem(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CIndividualVariableExpression* varExp, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping, QHash<CExpressionVariable*, cint64>& reductionVarExpIndHash, CObjectPropertyAssertionExpression* propAss, bool* processing, CAnswererContext* answererContext) {

				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(varExp);

				CRole* role = mOntoAnsweringItem->getOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(propAss->getObjectPropertyTermExpression());
				bool inversed = false;
				if (propAss->getFirstIndividualTermExpression() != varExp) {
					inversed = true;
				}


				if (mConfExtendedLogging) {
					QString roleName = "?";
					if (role->getPropertyNameLinker()) {
						roleName = CIRIName::getRecentIRIName(role->getPropertyNameLinker());
						if (inversed) {
							roleName = roleName + " inversed";
						}
					} else if (role->getInverseRole() && role->getInverseRole()->getPropertyNameLinker()) {
						roleName = CIRIName::getRecentIRIName(role->getInverseRole()->getPropertyNameLinker());
						if (!inversed) {
							roleName = roleName + " inversed";
						}
					}
					LOG(INFO, getDomain(), logTr("Scheduling propagation of mappings associated with %1 over role %2.").arg(varExp->getName()).arg(roleName), this);
				}


				CIndividualVariableExpression* otherExp = buildingVarItem->getOtherVariableExpression(varExp, propAss);
				if (!itemIndexMapping) {
					itemIndexMapping = varItemIndexMapping.value(varCompItem);
				}
				bool propToVarExpAlreadyPresent = false;
				cint64 varIdx = 0;
				cint64 propToVarIdx = itemIndexMapping->size();
				for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemIndexMapping->constBegin(), itEnd = itemIndexMapping->constEnd(); it != itEnd; ++it) {
					cint64 idx = it.key();
					CExpressionVariable* testVarExp = it.value();
					if (testVarExp == otherExp) {
						propToVarExpAlreadyPresent = true;
						propToVarIdx = idx;
					} 
					if (varExp == testVarExp) {
						varIdx = idx;
					}
				}

				bool replacement = false;
				if (!propToVarExpAlreadyPresent && mConfPropagationReplacement && reductionVarExpIndHash.value(varExp, -1) == varIdx) {
					replacement = true;
				}
				if (propToVarExpAlreadyPresent && !mConfPropagationJoining) {
					propToVarExpAlreadyPresent = false;
				}

				cint64 newVarIdx = varIdx;
				cint64 newVarCount = itemIndexMapping->size();

				COptimizedComplexVariableIndividualMappings* varMapping = varCompItem->getVariableMapping();

#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				QString withJoinReplacing;
#endif


				bool created = false;
				COptimizedComplexVariableRolePropagationAbstractItem* varRolePropItem = nullptr;
				if (!propToVarExpAlreadyPresent) {
					if (replacement) {
						COptimizedComplexVariableRolePropagationReplacementItem*& replacementVarRolePropItem = varCompItem->getRoleInversePropagationReplacementItem(role, inversed, varIdx, varIdx);
						if (!replacementVarRolePropItem) {
							replacementVarRolePropItem = new COptimizedComplexVariableRolePropagationReplacementItem(varCompItem, role, inversed, varIdx, varIdx);
							created = true;
						}
#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
						withJoinReplacing = " with replacement ";
#endif

						varRolePropItem = replacementVarRolePropItem;
					} else {
						newVarIdx = itemIndexMapping->size();
						newVarCount = itemIndexMapping->size() + 1;
						COptimizedComplexVariableRolePropagationItem*& newVarRolePropItem = varCompItem->getRoleInversePropagationItem(role, inversed, varIdx);
						if (!newVarRolePropItem) {
							newVarRolePropItem = new COptimizedComplexVariableRolePropagationItem(varCompItem, role, inversed, varIdx);
							created = true;
						}
						varRolePropItem = newVarRolePropItem;
					}
				} else {
					COptimizedComplexVariableRolePropagationJoiningItem*& joinVarRolePropItem = varCompItem->getRoleInversePropagationJoiningItem(role, inversed, varIdx, propToVarIdx);
					newVarIdx = propToVarIdx;
					if (!joinVarRolePropItem) {
						joinVarRolePropItem = new COptimizedComplexVariableRolePropagationJoiningItem(varCompItem, role, inversed, varIdx, propToVarIdx);
						created = true;
					}
#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
					withJoinReplacing = " with joining ";
#endif
					varRolePropItem = joinVarRolePropItem;
				}
				addUsedVariableCompositionItem(buildingVarItem, varRolePropItem);
				if (created) {
					varRolePropItem->setComputationStepId(mOntoAnsweringItem->getNextComputationStepId());
					varRolePropItem->setVariableMappingsComputationRequirement(mConfMinimalMappingsComputationSize);
				}
				if (!varRolePropItem->isVariableMappingsInitialized()) {
					varRolePropItem->setVariableMappingsInitialized(true);
					// schedule item processing
					queueVariableItemComputation(buildingVarItem, varRolePropItem);
					if (processing) {
						*processing = true;
					}

					//// schedule realization requirement processing
					//CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();
					//QSet<CIndividualReference> individualSet;
					//class CRoleTargetsRealizationRequirementInstanceItemVisitor : public CSameRealizationIndividualVisitor {
					//public:
					//	QSet<CIndividualReference>* mInstanceItemSet;

					//	CRoleTargetsRealizationRequirementInstanceItemVisitor(QSet<CIndividualReference>* instanceItemSet) {
					//		mInstanceItemSet = instanceItemSet;
					//	}

					//	bool visitIndividual(const CIndividualReference& indiRef, CSameRealization* sameRealization) {
					//		mInstanceItemSet->insert(indiRef);
					//		return false;
					//	}
					//} visitor(&individualSet);
					//for (COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = varMapping->getLastAddedBindingsCardinalityLinker(); linker; linker = linker->getNext()) {
					//	COptimizedComplexVariableIndividualBindings* binding = linker->getBindings();
					//	CRealizationIndividualInstanceItemReference instItemRef = bindings->getBinding(varIdx).reference;
					//	sameRealization->visitSameIndividuals(instItemRef, &visitor);
					//}
					//if (!individualSet.isEmpty()) {
					//	ensureRoleTargetsRealization(role, inversed, individualSet, buildingVarItem, answererContext);
					//}
				}


				COptimizedComplexVariableCompositionItemVariableExpressionMapping& propVarItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(otherExp);
				COptimizedComplexVariableCompositionItemVariableIndexMapping* propItemIndexMapping = new COptimizedComplexVariableCompositionItemVariableIndexMapping();
				propVarItemIndexMapping.insertMulti(varRolePropItem, propItemIndexMapping);

				COptimizedComplexVariableIndividualMappings* propVarMapping = nullptr;
				if (created) {
					if (mConfConcurrentPropagation) {
						propVarMapping = new COptimizedComplexVariableIndividualMappingsMultiHash(newVarCount, mConcurrentPropagationComputationTaskCount);
					} else {
						propVarMapping = new COptimizedComplexVariableIndividualMappingsHash(newVarCount);
					}
				}
				for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemIndexMapping->constBegin(), itEnd = itemIndexMapping->constEnd(); it != itEnd; ++it) {
					cint64 idx = it.key();
					CExpressionVariable* propVarExp = it.value();
					propItemIndexMapping->insert(idx, propVarExp);
					if (created) {
						propVarMapping->setBindingMapping(idx, varMapping->getBindingMapping(idx));
					}
				}
				propItemIndexMapping->insert(newVarIdx, otherExp);
				if (created) {
					propVarMapping->setBindingMapping(newVarIdx, COptimizedComplexVariableIndividualMappings::INDIVIDUAL_VARIABLE);
					varRolePropItem->setVariableMapping(propVarMapping);
				}

				QSet<COptimizedComplexVariableCompositionItem*>*& otherVarCompItemSet = (*varExpVarComItemHash)[otherExp];
				if (!otherVarCompItemSet) {
					otherVarCompItemSet = new QSet<COptimizedComplexVariableCompositionItem*>();
				}
				otherVarCompItemSet->insert(varRolePropItem);


#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				QStringList debugVariableNamePrevItemStringList = debugGetItemVariableNames(buildingVarItem, varExp, varCompItem);
				QStringList debugVariableNamePropItemStringList = debugGetItemVariableNames(buildingVarItem, otherExp, varRolePropItem);
				varRolePropItem->debugVariableNameStringList = debugVariableNamePropItemStringList;
				QString roleName = CIRIName::getRecentIRIName(role->getPropertyNameLinker());
				if (inversed) {
					if (role->getInverseRole()->getPropertyNameLinker()) {
						roleName = CIRIName::getRecentIRIName(role->getInverseRole()->getPropertyNameLinker());
					} else {
						roleName += "^-";
					}
				}
				varRolePropItem->debugCreationString = "Computation step " + QString::number(varRolePropItem->getComputationStepId()) + " with [" + debugVariableNamePropItemStringList.join(", ") + "] obtained by propagating " + varExp->getName() + withJoinReplacing + " from ["+debugVariableNamePrevItemStringList.join(", ")+"] of step " + QString::number(varCompItem->getComputationStepId()) + " over role "+ roleName;
				debugCreationStringList += varRolePropItem->debugCreationString;
				debugCreationString = debugCreationStringList.join("\r\n");
#endif

				buildingVarItem->updateLastHandledVariableItemAssociation(otherExp, varRolePropItem);

				return QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>(varRolePropItem, propItemIndexMapping);

			}





			bool itemMappingSortLessBindingsThan(QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> compItemMapping1Pair, QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> compItemMapping2Pair) {
				return compItemMapping1Pair.first->getVariableMappingsExpectedCount() < compItemMapping2Pair.first->getVariableMappingsExpectedCount();
			}




			QList<QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>> COptimizedComplexExpressionAnsweringHandler::reuseJoinedVariableCompositionItems(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp) {


				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();
				COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(varExp);


				QList<QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>> compItemMappingPairList;
				for (COptimizedComplexVariableCompositionItemVariableExpressionMapping::const_iterator it = varItemIndexMapping.constBegin(), itEnd = varItemIndexMapping.constEnd(); it != itEnd; ++it) {
					COptimizedComplexVariableCompositionItem* joiningItem = it.key();
					COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping = it.value();
					compItemMappingPairList.append(QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>(joiningItem, itemIndexMapping));
				}


				// sort list
				qSort(compItemMappingPairList.begin(), compItemMappingPairList.end(), itemMappingSortLessBindingsThan);

				QList<QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>> compItemMappingPairPreJoinedList;
				while (!compItemMappingPairList.isEmpty()) {
					QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> compItemMappingPair = compItemMappingPairList.takeFirst();
					COptimizedComplexVariableCompositionItem* joiningItem1 = compItemMappingPair.first;
					COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping1 = compItemMappingPair.second;

					bool joined = false;

					QList<QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>>::iterator it = compItemMappingPairPreJoinedList.begin();
					while (it != compItemMappingPairPreJoinedList.end() && !joined) {
						QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> testedCompItemMappingPair(*it);
						COptimizedComplexVariableCompositionItem* joiningItem2 = testedCompItemMappingPair.first;
						COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping2 = testedCompItemMappingPair.second;

						if (hasVariableJoiningItem(buildingVarItem, varExp, false, joiningItem1, joiningItem2, itemIndexMapping1, itemIndexMapping2)) {
							joined = true;


							QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> joinedItemMappingPair = buildVariableJoiningItem(buildingVarItem, varExp, false, joiningItem1, joiningItem2, itemIndexMapping1, itemIndexMapping2, nullptr);
							compItemMappingPairList.prepend(joinedItemMappingPair);
							compItemMappingPairPreJoinedList.erase(it);

						} else {
							++it;
						}
					}
					if (!joined) {
						compItemMappingPairPreJoinedList.append(compItemMappingPair);
					}
				}

				qSort(compItemMappingPairPreJoinedList.begin(), compItemMappingPairPreJoinedList.end(), itemMappingSortLessBindingsThan);
				return compItemMappingPairPreJoinedList;

			}



			QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> COptimizedComplexExpressionAnsweringHandler::buildJoinedVariableJoiningItems(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp, bool* processing, bool lastComputationItem) {

				cint64 joinCount = 0;
				cint64 reductionCount = 0;

				QList<QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>> compItemMappingPairPreJoinedList = reuseJoinedVariableCompositionItems(buildingVarItem, varExp);


				for (cint64 i = 0; i < compItemMappingPairPreJoinedList.size(); ) {

					QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> compItemMappingPair = compItemMappingPairPreJoinedList.at(i);
					COptimizedComplexVariableCompositionItem* joiningItem = compItemMappingPair.first;
					COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping = compItemMappingPair.second;
					QSet<CExpressionVariable*> varExpSet;
					bool sameVarExpJoiningRequired = false;
					for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator itMap = itemIndexMapping->constBegin(), itMapEnd = itemIndexMapping->constEnd(); !sameVarExpJoiningRequired && itMap != itMapEnd; ++itMap) {
						CExpressionVariable* usedVarExp(itMap.value());
						if (varExpSet.contains(usedVarExp)) {
							sameVarExpJoiningRequired = true;
						} else {
							varExpSet.insert(usedVarExp);
						}
					}
					if (sameVarExpJoiningRequired) {
						QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> joinedItemMappingPair = buildVariableJoiningItem(buildingVarItem, varExp, true, joiningItem, joiningItem, itemIndexMapping, itemIndexMapping, processing);
						compItemMappingPairPreJoinedList.replace(i, joinedItemMappingPair);
						joinCount++;
					} else {
						++i;
					}
				}

				QList<QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>> reducedCompItemMappingPairPreJoinedList;
				while (!compItemMappingPairPreJoinedList.isEmpty()) {
					QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> compItemMappingPair = compItemMappingPairPreJoinedList.takeFirst();
					COptimizedComplexVariableCompositionItem* joiningItem = compItemMappingPair.first;
					COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping = compItemMappingPair.second;

					QHash<CExpressionVariable*, cint64> reductionVarExpIndHash;
					if (!mConfAllowJoiningBindingReduction) {
						reductionVarExpIndHash = getReducableVariables(buildingVarItem, varExp, joiningItem, itemIndexMapping);
					}
					compItemMappingPair = buildVariableReductionItem(buildingVarItem, varExp, false, joiningItem, itemIndexMapping, reductionVarExpIndHash, processing);

					if (joiningItem != compItemMappingPair.first) {
						++reductionCount;
					}

					reducedCompItemMappingPairPreJoinedList.append(compItemMappingPair);
				}



				QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> compItemMapping1Pair = reducedCompItemMappingPairPreJoinedList.takeFirst();
				COptimizedComplexVariableCompositionItem* joiningItem1 = compItemMapping1Pair.first;
				COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping1 = compItemMapping1Pair.second;

				while (!reducedCompItemMappingPairPreJoinedList.isEmpty()) {
					QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> compItemMapping2Pair = reducedCompItemMappingPairPreJoinedList.takeFirst();
					COptimizedComplexVariableCompositionItem* joiningItem2 = compItemMapping2Pair.first;
					COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping2 = compItemMapping2Pair.second;

					bool lastJoinComputation = lastComputationItem && mConfAllowJoiningBindingReduction && reducedCompItemMappingPairPreJoinedList.isEmpty();
					QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> joinedItemMappingPair = buildVariableJoiningItem(buildingVarItem, varExp, false, joiningItem1, joiningItem2, itemIndexMapping1, itemIndexMapping2, processing, lastJoinComputation);

					QHash<CExpressionVariable*, cint64> reductionVarExpIndHash;
					if (!mConfAllowJoiningBindingReduction) {
						reductionVarExpIndHash = getReducableVariables(buildingVarItem, varExp, joinedItemMappingPair.first, joinedItemMappingPair.second);
					}
					QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> reducedJoinedItemMappingPair = buildVariableReductionItem(buildingVarItem, varExp, false, joinedItemMappingPair.first, joinedItemMappingPair.second, reductionVarExpIndHash, processing);

					if (joinedItemMappingPair.first != reducedJoinedItemMappingPair.first) {
						++reductionCount;
					}

					joiningItem1 = reducedJoinedItemMappingPair.first;
					itemIndexMapping1 = reducedJoinedItemMappingPair.second;
					joinCount++;
				}

				if (mConfExtendedLogging) {
					LOG(INFO, getDomain(), logTr("Scheduling %2 joins with %3 reductions for items associated with variable %1.").arg(varExp->getName()).arg(joinCount).arg(reductionCount), this);
				}

				return QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>(joiningItem1, itemIndexMapping1);
			}











			bool COptimizedComplexExpressionAnsweringHandler::hasVariableJoiningItem(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp, bool itemSameVarReduction, COptimizedComplexVariableCompositionItem* joiningItem1, COptimizedComplexVariableCompositionItem* joiningItem2, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping1, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping2) {

				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();



				COptimizedComplexVariableCompositionItem* minLeftItem = qMin(joiningItem1, joiningItem2);
				COptimizedComplexVariableCompositionItem* maxRightItem = qMax(joiningItem1, joiningItem2);

				COptimizedComplexVariableCompositionItemVariableIndexMapping* minLeftItemIndexMapping = itemIndexMapping1;
				COptimizedComplexVariableCompositionItemVariableIndexMapping* maxRightItemIndexMapping = itemIndexMapping2;

				if (minLeftItem == joiningItem2) {
					minLeftItemIndexMapping = itemIndexMapping2;
					maxRightItemIndexMapping = itemIndexMapping1;
				}

				COptimizedComplexVariableJoiningBindingPositionMapping joiningBindingPositionMapping(maxRightItem, minLeftItemIndexMapping->size(), maxRightItemIndexMapping->size());

				QSet<CExpressionVariable*> reductionVarExpSet = getJoinReducableVariables(buildingVarItem, varExp, minLeftItem, minLeftItemIndexMapping, maxRightItem, maxRightItemIndexMapping, itemSameVarReduction);
				cint64 nextVarIdx = fillJoiningBindingPositionMapping(minLeftItemIndexMapping, joiningBindingPositionMapping, maxRightItemIndexMapping, itemSameVarReduction, reductionVarExpSet);


				return minLeftItem->hasJoiningItem(joiningBindingPositionMapping);
			}






			QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> COptimizedComplexExpressionAnsweringHandler::buildVariableJoiningItem(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp, bool itemSameVarReduction, COptimizedComplexVariableCompositionItem* joiningItem1, COptimizedComplexVariableCompositionItem* joiningItem2, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping1, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping2, bool* processing, bool lastComputationItem) {
				return buildVariableJoiningItem(buildingVarItem, varExp, itemSameVarReduction, varExp, joiningItem1, varExp, joiningItem2, itemIndexMapping1, itemIndexMapping2, processing, lastComputationItem);
			}



			QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> COptimizedComplexExpressionAnsweringHandler::buildVariableJoiningItem(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* destVarExp, bool itemSameVarReduction, CExpressionVariable* varExp1, COptimizedComplexVariableCompositionItem* joiningItem1, CExpressionVariable* varExp2, COptimizedComplexVariableCompositionItem* joiningItem2, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping1, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping2, bool* processing, bool lastComputationItem) {


				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildingVarItem->getVariableExpressionVariableCompositionItemHash();



				COptimizedComplexVariableCompositionItem* minLeftItem = joiningItem1;
				COptimizedComplexVariableCompositionItem* maxRightItem = joiningItem2;

				CExpressionVariable* minLeftVarExp = varExp1;
				CExpressionVariable* maxRightVarExp = varExp2;

				COptimizedComplexVariableCompositionItemVariableIndexMapping* minLeftItemIndexMapping = itemIndexMapping1;
				COptimizedComplexVariableCompositionItemVariableIndexMapping* maxRightItemIndexMapping = itemIndexMapping2;


				COptimizedComplexVariableCompositionItemVariableExpressionMapping& destVarItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(destVarExp);

				if (joiningItem2 < joiningItem1) {
					minLeftItem = joiningItem2;
					maxRightItem = joiningItem1;

					minLeftVarExp = varExp2;
					maxRightVarExp = varExp1;

					minLeftItemIndexMapping = itemIndexMapping2;
					maxRightItemIndexMapping = itemIndexMapping1;

				}

				COptimizedComplexVariableCompositionItemVariableExpressionMapping& minLeftVarItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(minLeftVarExp);
				COptimizedComplexVariableCompositionItemVariableExpressionMapping& maxRightVarItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(maxRightVarExp);


				QList<COptimizedComplexVariableCompositionItemVariableIndexMapping*> minLeftItemIndexMappingList = minLeftVarItemIndexMapping.values(minLeftItem);
				QList<COptimizedComplexVariableCompositionItemVariableIndexMapping*> maxRightItemIndexMappingList = maxRightVarItemIndexMapping.values(maxRightItem);



				if (!minLeftItemIndexMapping) {
					minLeftItemIndexMapping = minLeftItemIndexMappingList.first();
				}
				if (!maxRightItemIndexMapping) {
					maxRightItemIndexMapping = maxRightItemIndexMappingList.first();
				}

				COptimizedComplexVariableJoiningBindingPositionMapping joiningBindingPositionMapping(maxRightItem, minLeftItemIndexMapping->size(), maxRightItemIndexMapping->size());


				QSet<CExpressionVariable*> reductionVarExpSet = getJoinReducableVariables(buildingVarItem, destVarExp, minLeftItem, minLeftItemIndexMapping, maxRightItem, maxRightItemIndexMapping, itemSameVarReduction);
				cint64 nextVarIdx = fillJoiningBindingPositionMapping(minLeftItemIndexMapping, joiningBindingPositionMapping, maxRightItemIndexMapping, itemSameVarReduction, reductionVarExpSet);


				COptimizedComplexVariableJoiningItem*& joinedItem = minLeftItem->getJoiningItem(joiningBindingPositionMapping);

				bool createdJoinedItem = false;
				if (!joinedItem) {
					createdJoinedItem = true;
					joinedItem = new COptimizedComplexVariableJoiningItem(minLeftItem, maxRightItem, joiningBindingPositionMapping);
					joinedItem->setComputationStepId(mOntoAnsweringItem->getNextComputationStepId());
					if (lastComputationItem && mConfAllowLastStepSplitComputation) {
						joinedItem->setVariableMappingsComputationRequirement(mConfLastStepSplitComputationMappingsRequirement);
						joinedItem->setLastComputation(true);
					} else {
						joinedItem->setVariableMappingsComputationRequirement(mConfMinimalMappingsComputationSize);
					}
				}
				addUsedVariableCompositionItem(buildingVarItem, joinedItem);

				COptimizedComplexVariableCompositionItemVariableIndexMapping* joinedItemIndexMapping = new COptimizedComplexVariableCompositionItemVariableIndexMapping();
				COptimizedComplexVariableIndividualMappings* joinedVarMapping = nullptr;
				if (createdJoinedItem) {
					if (mConfConcurrentJoining) {
						joinedVarMapping = new COptimizedComplexVariableIndividualMappingsMultiHash(nextVarIdx, mConcurrentJoinComputationTaskCount);
					} else {
						joinedVarMapping = new COptimizedComplexVariableIndividualMappingsHash(nextVarIdx);
					}
				}
#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				joinedItem->debugVarItemIndexMapping = joinedItemIndexMapping;
#endif

				COptimizedComplexVariableIndividualMappings* minLeftVarMapping = minLeftItem->getVariableMapping();
				COptimizedComplexVariableIndividualMappings* maxRightVarMapping = maxRightItem->getVariableMapping();
				for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = minLeftItemIndexMapping->constBegin(), itEnd = minLeftItemIndexMapping->constEnd(); it != itEnd; ++it) {
					cint64 idx = it.key();
					cint64 mappedVarIdx = joiningBindingPositionMapping.getMappedBindingPosition(true, idx);
					if (mappedVarIdx >= 0) {
						CExpressionVariable* joinVarExp = it.value();
						joinedItemIndexMapping->insert(mappedVarIdx, joinVarExp);
						if (createdJoinedItem) {
							joinedVarMapping->setBindingMapping(mappedVarIdx, minLeftVarMapping->getBindingMapping(idx));
						}
					}
				}
				if (!itemSameVarReduction) {
					for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = maxRightItemIndexMapping->constBegin(), itEnd = maxRightItemIndexMapping->constEnd(); it != itEnd; ++it) {
						cint64 idx = it.key();
						cint64 mappedVarIdx = joiningBindingPositionMapping.getMappedBindingPosition(false, idx);
						if (mappedVarIdx >= 0) {
							CExpressionVariable* joinVarExp = it.value();
							joinedItemIndexMapping->insert(mappedVarIdx, joinVarExp);
							if (createdJoinedItem) {
								joinedVarMapping->setBindingMapping(mappedVarIdx, maxRightVarMapping->getBindingMapping(idx));
							}
						}
					}
				}
				// insert new mapping
				if (createdJoinedItem) {
					joinedItem->setVariableMapping(joinedVarMapping);
				}
				destVarItemIndexMapping.insertMulti(joinedItem, joinedItemIndexMapping);


				varExpVarComItemHash->value(destVarExp)->insert(joinedItem);

				if (!joinedItem->isVariableMappingsInitialized()) {
					joinedItem->setVariableMappingsInitialized(true);

					if (processing) {
						*processing = true;
					}
					// schedule item processing
					queueVariableItemComputation(buildingVarItem, joinedItem);
				}

#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				QStringList debugVariableNameLeftItemStringList = debugGetItemVariableNames(buildingVarItem, minLeftVarExp, minLeftItem);
				QStringList debugVariableNameRightItemStringList = debugGetItemVariableNames(buildingVarItem, maxRightVarExp, maxRightItem);

				QStringList debugVariableNameJoinedItemStringList = debugGetItemVariableNames(buildingVarItem, destVarExp, joinedItem);
				joinedItem->debugVariableNameStringList = debugVariableNameJoinedItemStringList;
				QString reductionString;
				QStringList reductionVarStringList;
				for (CExpressionVariable* varExp : reductionVarExpSet) {
					reductionVarStringList.append(varExp->getName());
				}
				if (!reductionVarStringList.isEmpty()) {
					reductionString = QString(" with implicit reduction of variables <%1> ").arg(reductionVarStringList.join(", "));
				}
				joinedItem->debugCreationString = "Computation step " + QString::number(joinedItem->getComputationStepId()) + " with [" + debugVariableNameJoinedItemStringList.join(", ") + "] obtained by joining at " + destVarExp->getName() + reductionString + " from [" + debugVariableNameLeftItemStringList.join(", ") + "] of step " + QString::number(minLeftItem->getComputationStepId()) + " and [" + debugVariableNameRightItemStringList.join(", ") + "] of step " + QString::number(maxRightItem->getComputationStepId());
				debugCreationStringList += joinedItem->debugCreationString;
				debugCreationString = debugCreationStringList.join("\r\n");
#endif


				// re-add remaining mappings
				if (minLeftVarExp == destVarExp) {
					minLeftVarItemIndexMapping.remove(minLeftItem);
					for (COptimizedComplexVariableCompositionItemVariableIndexMapping* mapping : minLeftItemIndexMappingList) {
						minLeftVarItemIndexMapping.insertMulti(minLeftItem, mapping);
					}
					varExpVarComItemHash->value(minLeftVarExp)->remove(minLeftItem);
				}
				if (maxRightVarExp == destVarExp) {
					maxRightVarItemIndexMapping.remove(maxRightItem);
					for (COptimizedComplexVariableCompositionItemVariableIndexMapping* mapping : maxRightItemIndexMappingList) {
						maxRightVarItemIndexMapping.insertMulti(maxRightItem, mapping);
					}
					varExpVarComItemHash->value(maxRightVarExp)->remove(maxRightItem);
				}
				buildingVarItem->updateLastHandledVariableItemAssociation(destVarExp, joinedItem);


				destVarItemIndexMapping.removeItemVariableExpressionMapping(minLeftItem, minLeftItemIndexMapping);
				destVarItemIndexMapping.removeItemVariableExpressionMapping(maxRightItem, maxRightItemIndexMapping);


				return QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>(joinedItem, joinedItemIndexMapping);

			}






			QSet<CExpressionVariable*> COptimizedComplexExpressionAnsweringHandler::getJoinReducableVariables(COptimizedComplexBuildingVariableCompositionsItem* buildVarItem, CExpressionVariable* destVarExp, COptimizedComplexVariableCompositionItem* minLeftItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* minLeftItemIndexMapping, COptimizedComplexVariableCompositionItem* maxRightItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* maxRightItemIndexMapping, bool itemSameVarReduction) {

				QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* varExpVarComItemHash = buildVarItem->getVariableExpressionVariableCompositionItemHash();

				QSet<CExpressionVariable*> reductionVarIdxSet;
				COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem = dynamic_cast<COptimizedComplexBuildingIndividualVariableCompositionsItem*>(buildVarItem);
				if (buildingVarItem) {
					QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* absorbedQueryPartsItemExtensionList = buildingVarItem->getAbsorbedQueryPartItemExtensionHandlingList();
					if (mConfAllowBindingReduction && mConfAllowJoiningBindingReduction && buildingVarItem->isBindingsReducible() && absorbedQueryPartsItemExtensionList->isEmpty()) {
						// test whether there is no absorption propagation
						// test whether there exists a variable that is not distinguished, has no remaining properties, and is not used by a remaining item for any variable
						QSet<CExpressionVariable*>* allVarExpSet = buildingVarItem->getAllVariableSet();
						QSet<CExpressionVariable*>* reductionDeniedVarExpSet = buildingVarItem->getReductionDeniedVariableSet();

						QHash<CExpressionVariable*, cint64> varUseCountHash;

						for (CExpressionVariable* varExp : *allVarExpSet) {

							CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(varExp);
							if (indiVarExp) {
								QList<CObjectPropertyAssertionExpression*> propAssList = buildingVarItem->getUnhanledPropertyAssertionsExpressions(indiVarExp);
								QList<CDataPropertyAssertionExpression*> dataPropAssList = buildingVarItem->getUnhanledDataPropertyAssertionsExpressions(indiVarExp);
								QSet<COptimizedComplexVariableCompositionItem*>* remainingItemSet = varExpVarComItemHash->value(varExp);
								if (remainingItemSet) {
									bool investigateVarItemMappings = false;
									if (!dataPropAssList.isEmpty() || !propAssList.isEmpty()) {
										varUseCountHash[varExp] += 2;
										investigateVarItemMappings = true;
									}
									if (remainingItemSet->size() > 1) {
										investigateVarItemMappings = true;
									}
									if (!investigateVarItemMappings && remainingItemSet->size() == 1) {
										QList<COptimizedComplexVariableCompositionItemVariableIndexMapping*> firstOtherItemIndexMappings = buildingVarItem->getVariableItemIndexMapping(varExp).values(*remainingItemSet->begin());
										if (firstOtherItemIndexMappings.size() > 1) {
											investigateVarItemMappings = true;
										}
									}

									if (investigateVarItemMappings) {
										for (COptimizedComplexVariableCompositionItem* remainingItem : *remainingItemSet) {
											if (remainingItem) {
												QList<COptimizedComplexVariableCompositionItemVariableIndexMapping*> otherItemIndexMappings = buildingVarItem->getVariableItemIndexMapping(varExp).values(remainingItem);
												for (COptimizedComplexVariableCompositionItemVariableIndexMapping* otherItemIndexMapping : otherItemIndexMappings) {
													if ((remainingItem != minLeftItem || otherItemIndexMapping != minLeftItemIndexMapping) && (remainingItem != maxRightItem || otherItemIndexMapping != maxRightItemIndexMapping)) {
														for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator otherIt = otherItemIndexMapping->constBegin(), otherItEnd = otherItemIndexMapping->constEnd(); otherIt != otherItEnd; ++otherIt) {
															CExpressionVariable* otherVarExp = otherIt.value();
															varUseCountHash[otherVarExp]++;
														}
													}
												}
											}
										}
									}
								}

							}
						}


						if (!itemSameVarReduction) {
							QSet<CExpressionVariable*> leftVarSingleCheckSet;
							for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = minLeftItemIndexMapping->constBegin(), itEnd = minLeftItemIndexMapping->constEnd(); it != itEnd; ++it) {
								CExpressionVariable* varExp = it.value();
								leftVarSingleCheckSet.insert(varExp);
								varUseCountHash[varExp]++;
							}
							for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = maxRightItemIndexMapping->constBegin(), itEnd = maxRightItemIndexMapping->constEnd(); it != itEnd; ++it) {
								CExpressionVariable* varExp = it.value();
								if (leftVarSingleCheckSet.contains(varExp)) {
									leftVarSingleCheckSet.remove(varExp);
								} else {
									varUseCountHash[varExp]++;
								}
							}

							COptimizedComplexVariableCompositionItemVariableExpressionMapping& varItemIndexMapping = buildingVarItem->getVariableItemIndexMapping(destVarExp);
							if (varItemIndexMapping.size() > 1) {
								for (COptimizedComplexVariableCompositionItemVariableIndexMapping* otherItemIndexMapping : varItemIndexMapping) {
									if (otherItemIndexMapping != maxRightItemIndexMapping && otherItemIndexMapping != minLeftItemIndexMapping) {
										for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator otherIt = otherItemIndexMapping->constBegin(), otherItEnd = otherItemIndexMapping->constEnd(); otherIt != otherItEnd; ++otherIt) {
											CExpressionVariable* otherVarExp = otherIt.value();
											varUseCountHash[otherVarExp]++;
										}
									}
								}
							}

						} else {
							QSet<CExpressionVariable*> leftVarSingleCheckSet;
							QSet<CExpressionVariable*> leftVarSingleCheckSet2;
							for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = minLeftItemIndexMapping->constBegin(), itEnd = minLeftItemIndexMapping->constEnd(); it != itEnd; ++it) {
								CExpressionVariable* varExp = it.value();
								if (!leftVarSingleCheckSet.contains(varExp)) {
									leftVarSingleCheckSet.insert(varExp);
									varUseCountHash[varExp]++;
								} else {
									if (!leftVarSingleCheckSet2.contains(varExp)) {
										leftVarSingleCheckSet2.insert(varExp);
									} else {
										varUseCountHash[varExp]++;
									}
								}
							}
						}


						for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = minLeftItemIndexMapping->constBegin(), itEnd = minLeftItemIndexMapping->constEnd(); it != itEnd; ++it) {
							cint64 idx = it.key();
							CExpressionVariable* varExp = it.value();
							CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(varExp);
							if (!reductionDeniedVarExpSet->contains(varExp) && indiVarExp) {
								cint64 useCount = varUseCountHash.value(varExp);
								if (useCount <= 1) {
									reductionVarIdxSet.insert(varExp);
								}
							}
						}
						if (!itemSameVarReduction) {
							for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = maxRightItemIndexMapping->constBegin(), itEnd = maxRightItemIndexMapping->constEnd(); it != itEnd; ++it) {
								cint64 idx = it.key();
								CExpressionVariable* varExp = it.value();
								CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(varExp);
								if (!reductionDeniedVarExpSet->contains(varExp) && indiVarExp) {
									cint64 useCount = varUseCountHash.value(varExp);
									if (useCount <= 1) {
										reductionVarIdxSet.insert(varExp);
									}
								}
							}
						}

					}
				}
				return reductionVarIdxSet;
			}





			cint64 COptimizedComplexExpressionAnsweringHandler::fillJoiningBindingPositionMapping(COptimizedComplexVariableCompositionItemVariableIndexMapping* minLeftItemIndexMapping, COptimizedComplexVariableJoiningBindingPositionMapping &joiningBindingPositionMapping, COptimizedComplexVariableCompositionItemVariableIndexMapping* maxRightItemIndexMapping, bool itemSameVarReduction, QSet<CExpressionVariable *> &reductionVarExpSet) {
				cint64 nextVarIdx = 0;
				QHash<cint64, cint64> minLeftVarIdsCorrectionHash;
				QSet<CExpressionVariable*> sameItemVarExpMergedSet;
				QHash<CExpressionVariable*, cint64> minLeftItemVarExpIdxHash;
				QHash<CExpressionVariable*, cint64> maxRightItemVarExpIdxHash;
				QSet<CExpressionVariable*> commonVarExpSet;
				cint64 minLeftEliminationCount = 0;

				for (cint64 idx = 0; idx < minLeftItemIndexMapping->size(); ++idx) {
					joiningBindingPositionMapping.addBindingPositionMapping(true, idx, -1);
				}
				for (cint64 idx = 0; idx < maxRightItemIndexMapping->size(); ++idx) {
					joiningBindingPositionMapping.addBindingPositionMapping(false, idx, -1);
				}


				if (!itemSameVarReduction) {
					// determine the variables that occur in both items and use their positions for the hashing key linker
					for (cint64 idx = 0; idx < minLeftItemIndexMapping->size(); ++idx) {
						CExpressionVariable* refVarExp = minLeftItemIndexMapping->value(idx);
						if (!minLeftItemVarExpIdxHash.contains(refVarExp)) {
							minLeftItemVarExpIdxHash.insert(refVarExp, idx);
						}
					}
					for (cint64 idx = 0; idx < maxRightItemIndexMapping->size(); ++idx) {
						CExpressionVariable* refVarExp = maxRightItemIndexMapping->value(idx);
						if (minLeftItemVarExpIdxHash.contains(refVarExp)) {
							commonVarExpSet.insert(refVarExp);
						}
						if (!maxRightItemVarExpIdxHash.contains(refVarExp)) {
							maxRightItemVarExpIdxHash.insert(refVarExp, idx);
						}
					}


					CXLinker<cint64>* leftKeyBindingLinker = nullptr;
					CXLinker<cint64>* rightKeyBindingLinker = nullptr;

					CXLinker<cint64>* lastLeftKeyBindingLinker = nullptr;
					CXLinker<cint64>* lastRightKeyBindingLinker = nullptr;


					CXLinker<cint64>* leftRemainingBindingLinker = nullptr;
					CXLinker<cint64>* rightRemainingBindingLinker = nullptr;

					CXLinker<cint64>* lastLeftRemainingBindingLinker = nullptr;
					CXLinker<cint64>* lastRightRemainingBindingLinker = nullptr;




					for (QSet<CExpressionVariable*>::const_iterator it = commonVarExpSet.constBegin(), itEnd = commonVarExpSet.constEnd(); it != itEnd; ++it) {
						CExpressionVariable* commVarExp = *it;

						cint64 leftVarIdx = minLeftItemVarExpIdxHash.value(commVarExp);
						CXLinker<cint64>* newLeftKeyBindingLinker = new CXLinker<cint64>();
						newLeftKeyBindingLinker->initLinker(leftVarIdx);
						if (lastLeftKeyBindingLinker) {
							lastLeftKeyBindingLinker->append(newLeftKeyBindingLinker);
							lastLeftKeyBindingLinker = newLeftKeyBindingLinker;
						} else {
							lastLeftKeyBindingLinker = newLeftKeyBindingLinker;
							leftKeyBindingLinker = newLeftKeyBindingLinker;
						}

						cint64 rightVarIdx = maxRightItemVarExpIdxHash.value(commVarExp);
						CXLinker<cint64>* newRightKeyBindingLinker = new CXLinker<cint64>();
						newRightKeyBindingLinker->initLinker(rightVarIdx);
						if (lastRightKeyBindingLinker) {
							lastRightKeyBindingLinker->append(newRightKeyBindingLinker);
							lastRightKeyBindingLinker = newRightKeyBindingLinker;
						} else {
							lastRightKeyBindingLinker = newRightKeyBindingLinker;
							rightKeyBindingLinker = newRightKeyBindingLinker;
						}
					}
					joiningBindingPositionMapping.setLeftKeyBindingLinker(leftKeyBindingLinker);
					joiningBindingPositionMapping.setRightKeyBindingLinker(rightKeyBindingLinker);


					for (cint64 idx = 0; idx < minLeftItemIndexMapping->size(); ++idx) {
						CExpressionVariable* refVarExp = minLeftItemIndexMapping->value(idx);
						if (!reductionVarExpSet.contains(refVarExp)) {
							cint64 corrIdx = nextVarIdx++;
							joiningBindingPositionMapping.addBindingPositionMapping(true, idx, corrIdx);
							minLeftVarIdsCorrectionHash.insert(idx, corrIdx);

							CXLinker<cint64>* newLeftRemainingBindingLinker = new CXLinker<cint64>();
							newLeftRemainingBindingLinker->initLinker(idx);
							if (lastLeftRemainingBindingLinker) {
								lastLeftRemainingBindingLinker->append(newLeftRemainingBindingLinker);
								lastLeftRemainingBindingLinker = newLeftRemainingBindingLinker;
							} else {
								lastLeftRemainingBindingLinker = newLeftRemainingBindingLinker;
								leftRemainingBindingLinker = newLeftRemainingBindingLinker;
							}

						}
					}
					for (cint64 idx = 0; idx < maxRightItemIndexMapping->size(); ++idx) {
						CExpressionVariable* refVarExp = maxRightItemIndexMapping->value(idx);
						if (!reductionVarExpSet.contains(refVarExp)) {
							if (!commonVarExpSet.contains(refVarExp) || maxRightItemVarExpIdxHash.value(refVarExp) != idx) {
								joiningBindingPositionMapping.addBindingPositionMapping(false, idx, nextVarIdx++);
							}

							CXLinker<cint64>* newRightRemainingBindingLinker = new CXLinker<cint64>();
							newRightRemainingBindingLinker->initLinker(idx);
							if (lastRightRemainingBindingLinker) {
								lastRightRemainingBindingLinker->append(newRightRemainingBindingLinker);
								lastRightRemainingBindingLinker = newRightRemainingBindingLinker;
							} else {
								lastRightRemainingBindingLinker = newRightRemainingBindingLinker;
								rightRemainingBindingLinker = newRightRemainingBindingLinker;
							}


						}
					}
					joiningBindingPositionMapping.setLeftRemainingBindingLinker(leftRemainingBindingLinker);
					joiningBindingPositionMapping.setRightRemainingBindingLinker(rightRemainingBindingLinker);


					for (CExpressionVariable* redVarExp : reductionVarExpSet) {
						if (minLeftItemVarExpIdxHash.contains(redVarExp)) {
							cint64 idx = minLeftItemVarExpIdxHash.value(redVarExp);
							joiningBindingPositionMapping.addBindingPositionMapping(true, idx, -2);
							joiningBindingPositionMapping.setVariableReduction(true);
						} else if (maxRightItemVarExpIdxHash.contains(redVarExp)) {
							cint64 idx = maxRightItemVarExpIdxHash.value(redVarExp);
							joiningBindingPositionMapping.addBindingPositionMapping(false, idx, -2);
							joiningBindingPositionMapping.setVariableReduction(true);
						}
					}


				} else {
					// determine the variables that occur twice and use their positions (together with the remaining variables' positions) for the hashing key linker

					for (cint64 idx = 0; idx < minLeftItemIndexMapping->size(); ++idx) {
						CExpressionVariable* refVarExp = minLeftItemIndexMapping->value(idx);
						if (!minLeftItemVarExpIdxHash.contains(refVarExp)) {
							minLeftItemVarExpIdxHash.insert(refVarExp, idx);
						} else {
							commonVarExpSet.insert(refVarExp);
							if (!maxRightItemVarExpIdxHash.contains(refVarExp)) {
								maxRightItemVarExpIdxHash.insert(refVarExp, idx);
							}
						}
					}


					CXLinker<cint64>* leftKeyBindingLinker = nullptr;
					CXLinker<cint64>* rightKeyBindingLinker = nullptr;

					CXLinker<cint64>* lastLeftKeyBindingLinker = nullptr;
					CXLinker<cint64>* lastRightKeyBindingLinker = nullptr;




					for (cint64 idx = 0; idx < minLeftItemIndexMapping->size(); ++idx) {
						CExpressionVariable* refVarExp = minLeftItemIndexMapping->value(idx);

						cint64 leftVarIdx = idx;
						CXLinker<cint64>* newLeftKeyBindingLinker = new CXLinker<cint64>();
						newLeftKeyBindingLinker->initLinker(leftVarIdx);
						if (lastLeftKeyBindingLinker) {
							lastLeftKeyBindingLinker->append(newLeftKeyBindingLinker);
							lastLeftKeyBindingLinker = newLeftKeyBindingLinker;
						} else {
							lastLeftKeyBindingLinker = newLeftKeyBindingLinker;
							leftKeyBindingLinker = newLeftKeyBindingLinker;
						}

						cint64 rightVarIdx = idx;
						if (commonVarExpSet.contains(refVarExp)) {
							if (!sameItemVarExpMergedSet.contains(refVarExp)) {
								sameItemVarExpMergedSet.insert(refVarExp);
								rightVarIdx = maxRightItemVarExpIdxHash.value(refVarExp);
							}
						}
						CXLinker<cint64>* newRightKeyBindingLinker = new CXLinker<cint64>();
						newRightKeyBindingLinker->initLinker(rightVarIdx);
						if (lastRightKeyBindingLinker) {
							lastRightKeyBindingLinker->append(newRightKeyBindingLinker);
							lastRightKeyBindingLinker = newRightKeyBindingLinker;
						} else {
							lastRightKeyBindingLinker = newRightKeyBindingLinker;
							rightKeyBindingLinker = newRightKeyBindingLinker;
						}
					}
					joiningBindingPositionMapping.setLeftKeyBindingLinker(leftKeyBindingLinker);
					joiningBindingPositionMapping.setRightKeyBindingLinker(rightKeyBindingLinker);


					for (cint64 idx = 0; idx < minLeftItemIndexMapping->size(); ++idx) {
						CExpressionVariable* refVarExp = minLeftItemIndexMapping->value(idx);
						if (!reductionVarExpSet.contains(refVarExp)) {

							bool addVarIdx = false;
							if (!commonVarExpSet.contains(refVarExp)) {
								addVarIdx = true;
							} else {
								if (sameItemVarExpMergedSet.contains(refVarExp)) {
									sameItemVarExpMergedSet.remove(refVarExp);
									addVarIdx = false;
								} else {
									addVarIdx = true;
								}
							}

							if (addVarIdx) {
								cint64 corrIdx = nextVarIdx++;
								joiningBindingPositionMapping.addBindingPositionMapping(true, idx, corrIdx);
								minLeftVarIdsCorrectionHash.insert(idx, corrIdx);
							}
						}
					}

					for (CExpressionVariable* redVarExp : reductionVarExpSet) {
						cint64 idx = minLeftItemVarExpIdxHash.value(redVarExp);
						joiningBindingPositionMapping.addBindingPositionMapping(true, idx, -2);
						joiningBindingPositionMapping.setVariableReduction(true);
					}

				}
				joiningBindingPositionMapping.setSelfSameVariableReduction(itemSameVarReduction);
				return nextVarIdx;
			}

			bool COptimizedComplexExpressionAnsweringHandler::finishSubClassCalculationStepProcessing(COptimizedComplexConceptItem* conceptItem, CComplexConceptStepComputationProcess* compStep, CAnswererContext* answererContext) {
				delete conceptItem->getPossibleSubClassNodeTestingList();
				conceptItem->setPossibleSubClassNodeTestingList(nullptr);
				delete conceptItem->getPossibleSubClassTestingNodeSet();
				conceptItem->setPossibleSubClassTestingNodeSet(nullptr);
				delete conceptItem->getMaximumSubClassNodeSet();
				conceptItem->setMaximumSubClassNodeSet(nullptr);
				return finishCalculationStepProcessing(conceptItem, compStep, answererContext);
			}





			bool COptimizedComplexExpressionAnsweringHandler::initializeRealization(COptimizedComplexConceptItem* conceptItem, CAnswererContext* answererContext) {
				CConceptRealization* conceptRealization = mOntoAnsweringItem->getOntology()->getRealization()->getConceptRealization();
				CRoleRealization* roleRealization = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();
				if (!conceptRealization || !roleRealization) {
					QList<COntologyProcessingRequirement*> reqList;
					COntologyProcessingInitializedRealizationRequirement* initRealReq = new COntologyProcessingInitializedRealizationRequirement();
					reqList.append(initRealReq);
					CAnsweringMessageDataRequirementCompletedRealization* reqCompMess = new CAnsweringMessageDataRequirementCompletedRealization(conceptItem);
					processRequirements(answererContext, mOntoAnsweringItem->getOntology(), reqList, reqCompMess);
					conceptItem->incLazyRealizationProcessingCount();
					return true;
				}
				return false;
			}



			bool COptimizedComplexExpressionAnsweringHandler::initializeRealizationInstancesIterators(COptimizedComplexConceptItem* conceptItem, cint64 requiredInstancesCount, CAnswererContext* answererContext) {
				QSet<CHierarchyNode*>* superHierNodeSet = conceptItem->getDirectSuperClassNodeSet();
				QSet<CHierarchyNode*>* subHierNodeSet = conceptItem->getDirectSubClassNodeSet();
				CHierarchyNode* topHierNode = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy()->getTopHierarchyNode();
				CHierarchyNode* bottomHierNode = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy()->getBottomHierarchyNode();

				CConceptRealization* conceptRealization = mOntoAnsweringItem->getOntology()->getRealization()->getConceptRealization();
				CRoleRealization* roleRealization = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();

				QList<CRealizationIndividualInstanceItemReferenceIterator*> realizationIteratorList;
				bool unsatisfiable = false;

				QList<COntologyProcessingRequirement*> reqList;
				bool subInsteadOfTopHierNodeRealization = false;
				if (superHierNodeSet) {
					for (CHierarchyNode* node : *superHierNodeSet) {
						if (node != topHierNode) {
							if (mConfDirectPossibleRealizationRequesting) {
								COntologyProcessingConceptRealizationRequirement* conRealReq = new COntologyProcessingConceptRealizationRequirement(CIndividualReference(), node->getOneEquivalentConcept());
								reqList.append(conRealReq);
							}
							CRealizationIndividualInstanceItemReferenceIterator* iterator = conceptRealization->getConceptInstancesIterator(node->getOneEquivalentConcept(), false, mDefaultRealizationSorting);
							realizationIteratorList.append(iterator);
							if (iterator->atEnd() && !iterator->requiresInitialization()) {
								unsatisfiable = true;
								break;
							}
						} else {
							subInsteadOfTopHierNodeRealization = true;
						}
					}
				} else {
					for (TConceptNegPair conNegPair : *conceptItem->getAtomicConcpetSet()) {
						CConcept* concept = conNegPair.first;
						bool negation = conNegPair.second;
						if (!negation && (concept->getOperatorCode() == CCATOM || concept->getOperatorCode() == CCSUB || concept->getOperatorCode() == CCEQ)) {
							if (mConfDirectPossibleRealizationRequesting) {
								COntologyProcessingConceptRealizationRequirement* conRealReq = new COntologyProcessingConceptRealizationRequirement(CIndividualReference(), concept);
								reqList.append(conRealReq);
							}
							CRealizationIndividualInstanceItemReferenceIterator* iterator = conceptRealization->getConceptInstancesIterator(concept, false, mDefaultRealizationSorting);
							realizationIteratorList.append(iterator);
							if (iterator->atEnd() && !iterator->requiresInitialization()) {
								unsatisfiable = true;
								break;
							}

						}
					}
				}
				if (subInsteadOfTopHierNodeRealization && subHierNodeSet && !unsatisfiable) {
					for (CHierarchyNode* node : *subHierNodeSet) {
						if (node != bottomHierNode) {
							if (mConfDirectPossibleRealizationRequesting) {
								COntologyProcessingConceptRealizationRequirement* conRealReq = new COntologyProcessingConceptRealizationRequirement(CIndividualReference(), node->getOneEquivalentConcept());
								reqList.append(conRealReq);
							}
						}
					}
				}

				typedef QPair<CRole*, CDataLiteral*> TRoleDataLiteralPair;

				CConcept* indiTriggerConcept = mOntoAnsweringItem->getOntology()->getTBox()->getIndividualTriggerConcept();
				CConcept* topDataRangeConcept = mOntoAnsweringItem->getOntology()->getTBox()->getTopDataRangeConcept();

				QSet<CRole*> roleRequiredNeighboursSet;
				QSet<CRole*> roleRequiredExistentialSet;
				QSet<TRoleIndividualPair> roleRequiredSuccIndiSet;
				QSet<TRoleDataLiteralPair> roleDataLiteralRequiredInstantiatingSet;
				QSet<CRole*> roleRequiredDataSet;

				if (!unsatisfiable) {
					for (TConceptNegPair conNegPair : *conceptItem->getAtomicConcpetSet()) {
						CConcept* concept = conNegPair.first;
						bool negation = conNegPair.second;
						if (!negation && concept->getOperatorCode() == CCVALUE) {
							CIndividual* nomIndividual = concept->getNominalIndividual();
							CRole* role = concept->getRole();
							roleRequiredSuccIndiSet.insert(TRoleIndividualPair(role, nomIndividual));

						} else if (!negation && concept->getOperatorCode() == CCSOME) {
							CRole* role = concept->getRole();
							if (!role->isDataRole() && concept->getOperandCount() == 1 && concept->getOperandList()->getData()) {
								if (concept->getOperandList()->getData() == indiTriggerConcept && !concept->getOperandList()->isNegated()) {
									roleRequiredNeighboursSet.insert(role);
								} else if (concept->getOperandList()->getData()->getOperatorCode() == CCNOMINAL && !concept->getOperandList()->isNegated()) {
									CIndividual* nomIndi = concept->getOperandList()->getData()->getNominalIndividual();
									roleRequiredSuccIndiSet.insert(TRoleIndividualPair(role, nomIndi));
								} else {
									roleRequiredExistentialSet.insert(role);
								}
							}

							if (role->isDataRole() && concept->getOperandCount() == 1 && concept->getOperandList()->getData()) {
								CConcept* opConcept = concept->getOperandList()->getData();
								bool opConNegation = concept->getOperandList()->isNegated();

								if (opConcept == topDataRangeConcept && !opConNegation) {
									roleRequiredDataSet.insert(role);
								} else if (opConcept->getOperatorCode() == CCDATALITERAL && !opConNegation) {
									if (canRetrieveInstantiatingDataValueIndividuals(role, opConcept->getDataLiteral())) {
										roleDataLiteralRequiredInstantiatingSet.insert(TRoleDataLiteralPair(role, opConcept->getDataLiteral()));
									} else {
										roleRequiredDataSet.insert(role);
									}
								}
							}

						} else if (negation && concept->getOperatorCode() == CCALL) {
							CRole* role = concept->getRole();
							if (!role->isDataRole() && concept->getOperandCount() == 1 && concept->getOperandList()->getData()) {
								if (concept->getOperandList()->getData() == indiTriggerConcept && concept->getOperandList()->isNegated()) {
									roleRequiredNeighboursSet.insert(role);
								} else if (concept->getOperandList()->getData()->getOperatorCode() == CCNOMINAL && concept->getOperandList()->isNegated()) {
									CIndividual* nomIndi = concept->getOperandList()->getData()->getNominalIndividual();
									roleRequiredSuccIndiSet.insert(TRoleIndividualPair(role, nomIndi));
								} else {
									roleRequiredExistentialSet.insert(role);
								}
							}


							if (role->isDataRole() && concept->getOperandCount() == 1 && concept->getOperandList()->getData()) {
								CConcept* opConcept = concept->getOperandList()->getData();
								bool opConNegation = concept->getOperandList()->isNegated();

								if (opConcept == topDataRangeConcept && opConNegation) {
									roleRequiredDataSet.insert(role);
								} else if (opConcept->getOperatorCode() == CCDATALITERAL && opConNegation) {
									if (canRetrieveInstantiatingDataValueIndividuals(role, opConcept->getDataLiteral())) {
										roleDataLiteralRequiredInstantiatingSet.insert(TRoleDataLiteralPair(role, opConcept->getDataLiteral()));									
									} else {
										roleRequiredDataSet.insert(role);
									}
								}
							}

						} else if (!negation && concept->getOperatorCode() == CCAQCHOOCE) {
							CRole* role = nullptr;
							if (concept->getOperandCount() >= 1) {
								CConcept* aqChOpConcept = concept->getOperandList()->getData();
								if (aqChOpConcept && aqChOpConcept->getOperatorCode() == CCAQSOME && !concept->getOperandList()->isNegated()) {
									role = aqChOpConcept->getRole();

									if (aqChOpConcept->getOperandCount() == 1 && aqChOpConcept->getOperandList()->getData()) {
										if (aqChOpConcept->getOperandList()->getData() == indiTriggerConcept && !aqChOpConcept->getOperandList()->isNegated()) {
											roleRequiredNeighboursSet.insert(role);
										} else if (aqChOpConcept->getOperandList()->getData()->getOperatorCode() == CCNOMINAL && !aqChOpConcept->getOperandList()->isNegated()) {
											CIndividual* nomIndi = aqChOpConcept->getOperandList()->getData()->getNominalIndividual();
											roleRequiredSuccIndiSet.insert(TRoleIndividualPair(role, nomIndi));
										} else {
											roleRequiredExistentialSet.insert(role);
										}
									}

								}
							}
						}
					}
				}
				if (!roleDataLiteralRequiredInstantiatingSet.isEmpty() && !unsatisfiable) {
					for (const TRoleDataLiteralPair& roleDataLiteralPair : roleDataLiteralRequiredInstantiatingSet) {
						CRole* role = roleDataLiteralPair.first;
						CDataLiteral* dataLiteral = roleDataLiteralPair.second;
						QMap<cint64, IndividualIdCandidateData>* instantiatingDataValueIndividualIdMap = getInstantiatingDataValueIndividualIdMap(role, dataLiteral, nullptr, answererContext);
						if (!instantiatingDataValueIndividualIdMap->isEmpty()) {
							CRealizationIndividualInstanceItemReferenceIterator* iterator = conceptRealization->getIterationCandidateIndividualIdIterator(*instantiatingDataValueIndividualIdMap, mDefaultRealizationSorting);
							realizationIteratorList.append(iterator);
							if (iterator->atEnd() && !iterator->requiresInitialization()) {
								unsatisfiable = true;
								break;
							}
						} else {
							unsatisfiable = true;
							break;
						}
						delete instantiatingDataValueIndividualIdMap;
					}
				}
				if (!roleRequiredDataSet.isEmpty() && !unsatisfiable) {
					for (CRole* role : roleRequiredDataSet) {
						CRealizationIndividualInstanceItemReferenceIterator* iterator = roleRealization->getRoleDataInstancesIterator(role, mDefaultRealizationSorting);
						realizationIteratorList.append(iterator);
						if (iterator->atEnd() && !iterator->requiresInitialization()) {
							unsatisfiable = true;
						}
					}
				}
				if (!roleRequiredNeighboursSet.isEmpty() || !roleRequiredSuccIndiSet.isEmpty() || !roleRequiredExistentialSet.isEmpty()) {
					if (!unsatisfiable) {
						for (CRole* role : roleRequiredNeighboursSet) {
							if (mConfDirectPossibleRealizationRequesting) {
								COntologyProcessingRoleRealizationRequirement* roleRealReq = new COntologyProcessingRoleRealizationRequirement(role, false);
								reqList.append(roleRealReq);
							}
							CRealizationIndividualInstanceItemReferenceIterator* iterator = roleRealization->getRoleNeighbouringInstancesIterator(role, false, mDefaultRealizationSorting);
							realizationIteratorList.append(iterator);
							if (iterator->atEnd() && !iterator->requiresInitialization()) {
								unsatisfiable = true;
								break;
							}
						}
					}
					if (!unsatisfiable) {
						for (const TRoleIndividualPair& roleIndiPair : roleRequiredSuccIndiSet) {
							CRole* role = roleIndiPair.first;
							CIndividual* nomIndividual = roleIndiPair.second;
							if (mConfDirectPossibleRealizationRequesting) {
								COntologyProcessingRoleRealizationRequirement* roleRealReq = new COntologyProcessingRoleRealizationRequirement(role, true, nomIndividual);
								reqList.append(roleRealReq);
							}
							CRealizationIndividualInstanceItemReferenceIterator* iterator = roleRealization->getRoleFillerInstancesIterator(roleRealization->getRoleInstanceItemReference(nomIndividual), role, true, mDefaultRealizationSorting);
							realizationIteratorList.append(iterator);
							if (iterator->atEnd() && !iterator->requiresInitialization()) {
								unsatisfiable = true;
								break;
							}
						}
					}
					if (!unsatisfiable && !roleRequiredExistentialSet.isEmpty()) {
						for (CRole* role : roleRequiredExistentialSet) {
							if (!roleRequiredNeighboursSet.contains(role)) {
								CRealizationIndividualInstanceItemReferenceIterator* iterator = roleRealization->getRoleExistentialInstancesIterator(role, false, mDefaultRealizationSorting);
								realizationIteratorList.append(iterator);
								if (iterator->atEnd() && !iterator->requiresInitialization()) {
									unsatisfiable = true;
									break;
								}
							}
						}
					}
				}


				if (!unsatisfiable && realizationIteratorList.isEmpty()) {
					CRealizationIndividualInstanceItemReferenceIterator* iterator = conceptRealization->getConceptInstancesIterator(topHierNode->getOneEquivalentConcept(), false, mDefaultRealizationSorting);
					realizationIteratorList.append(iterator);
					if (iterator->atEnd() && !iterator->requiresInitialization()) {
						unsatisfiable = true;
					}
				}


				if (!realizationIteratorList.isEmpty()) {
					if (!unsatisfiable) {
						CRealizationIndividualInstanceItemReferenceIterator* iterator = conceptRealization->getIterationIntersectionIterator(realizationIteratorList);
						if (iterator->requiresInitialization()) {
							COntologyProcessingIteratorRealizationRequirement* roleRealReq = new COntologyProcessingIteratorRealizationRequirement(iterator, requiredInstancesCount);
							reqList.append(roleRealReq);
						}
						conceptItem->setRealizationIterator(iterator);
					} else {
						qDeleteAll(realizationIteratorList);
					}
				}


				if (!reqList.isEmpty()) {
					if (!unsatisfiable) {
						CAnsweringMessageDataRequirementCompletedRealization* reqCompMess = new CAnsweringMessageDataRequirementCompletedRealization(conceptItem);
						processRequirements(answererContext, mOntoAnsweringItem->getOntology(), reqList, reqCompMess);
						conceptItem->incLazyRealizationProcessingCount();
						return true;
					} else {
						qDeleteAll(reqList);
					}
				}
				return false;
			}



			bool COptimizedComplexExpressionAnsweringHandler::initializeKnownPossibleInstances(COptimizedComplexConceptItem* conceptItem, cint64 requiredInstancesCount, CAnswererContext* answererContext) {

				COptimizedComplexConceptInstanziatedIndividualItemHash* directInstanceItems = conceptItem->getKnownInstanceItems();
				if (!directInstanceItems) {
					directInstanceItems = new COptimizedComplexConceptInstanziatedIndividualItemHash();
					conceptItem->setKnownInstanceItems(directInstanceItems);
				}
				

				bool completelyHandled = conceptItem->getAllAtomicConceptsCompletelyHandled();
				cint64 certainInstances = directInstanceItems->size();


				CRealizationIndividualInstanceItemReferenceIterator* iterator = conceptItem->getRealizationIterator();

				//CRealizationIndividualInstanceItemReferenceIterator* iteratorCopy = iterator->getCopy();

				if (iterator) {
					iterator->begin();

					QSet<CRealizationIndividualInstanceItemReference>* possiblenstanceItemSet = conceptItem->getPossibleInstanceItemSet();
					if (!possiblenstanceItemSet) {
						possiblenstanceItemSet = new QSet<CRealizationIndividualInstanceItemReference>();
						conceptItem->setPossibleInstanceItemSet(possiblenstanceItemSet);
					}
					cint64 visitedIndis = 0;
					while (!iterator->atEnd() && !iterator->requiresInitialization() && (requiredInstancesCount < 0 || visitedIndis < requiredInstancesCount)) {
						CRealizationIndividualInstanceItemReference indiItemRef = iterator->currentIndividualInstanceItemReference();
						if (completelyHandled && iterator->isCurrentInstanceCertain()) {							
							directInstanceItems->addRealizationIndividualInstanceItemReference(indiItemRef);
						} else {
							possiblenstanceItemSet->insert(indiItemRef);
						}
						iterator->moveNext();
						conceptItem->inRealizationRetrievedInstanceCandidateCount();
						++visitedIndis;
					}

					//if (visitedIndis <= 0 && conceptItem->debugVariableNameUseList.contains("v0__HClFC__n550")) {
					//	iteratorCopy->begin();
					//	while (!iteratorCopy->atEnd()) {
					//		iteratorCopy->moveNext();
					//	}
					//	initializeRealizationInstancesIterators(conceptItem, 1, answererContext);
					//	CRealizationIndividualInstanceItemReferenceIterator* iterator = conceptItem->getRealizationIterator();
					//	iterator->begin();
					//}


					cint64 newCertainInstances = directInstanceItems->size() - certainInstances;
					conceptItem->setLastRetrievedCertainInstanceItemCount(newCertainInstances);

					cint64 estimatedInstances = iterator->remainingInstancesEstimation().getEstimatedCount();
					conceptItem->setRealizationRetrievingExpectedInstanceCandidateCount(conceptItem->getRealizationRetrievedInstanceCandidateCount() + estimatedInstances);


					if (iterator->requiresInitialization()) {
						QList<COntologyProcessingRequirement*> reqList;
						COntologyProcessingIteratorRealizationRequirement* roleRealReq = new COntologyProcessingIteratorRealizationRequirement(iterator, requiredInstancesCount);
						reqList.append(roleRealReq);

						CAnsweringMessageDataRequirementCompletedRealization* reqCompMess = new CAnsweringMessageDataRequirementCompletedRealization(conceptItem);
						processRequirements(answererContext, mOntoAnsweringItem->getOntology(), reqList, reqCompMess);
						conceptItem->incLazyRealizationProcessingCount();
						return true;
					} else if (conceptItem->getRealizationIterator()->atEnd()) {
						conceptItem->setLazyRealizationInstancesRetrieved(true);
					}
				} else {
					conceptItem->setLazyRealizationInstancesRetrieved(true);
				}

				return false;
			}






			bool COptimizedComplexExpressionAnsweringHandler::schedulePossibleInstancesNeighourRealization(COptimizedComplexConceptItem* conceptItem, CAnswererContext* answererContext) {
				QSet<CRealizationIndividualInstanceItemReference>* possibleDirectInstanceItemSet = conceptItem->getPossibleInstanceItemSet();
				CConcept* indiTriggerConcept = mOntoAnsweringItem->getOntology()->getTBox()->getIndividualTriggerConcept();

				QSet<CRole*> roleRequiredNeighboursSet;
				QSet<TRoleIndividualPair> roleRequiredSuccIndiSet;
				for (TConceptNegPair conNegPair : *conceptItem->getAtomicConcpetSet()) {
					CConcept* concept = conNegPair.first;
					bool negation = conNegPair.second;
					if (!negation && concept->getOperatorCode() == CCSOME) {
						CRole* role = concept->getRole();
						if (concept->getOperandCount() == 1 && concept->getOperandList()->getData()) {
							if (concept->getOperandList()->getData() == indiTriggerConcept && !concept->getOperandList()->isNegated()) {
								roleRequiredNeighboursSet.insert(role);
							} else if (concept->getOperandList()->getData()->getOperatorCode() == CCNOMINAL && !concept->getOperandList()->isNegated()) {
								CIndividual* nomIndi = concept->getOperandList()->getData()->getNominalIndividual();
								roleRequiredSuccIndiSet.insert(TRoleIndividualPair(role, nomIndi));
							}
						}

					} else if (negation && concept->getOperatorCode() == CCALL) {
						CRole* role = concept->getRole();
						if (concept->getOperandCount() == 1 && concept->getOperandList()->getData()) {
							if (concept->getOperandList()->getData() == indiTriggerConcept && concept->getOperandList()->isNegated()) {
								roleRequiredNeighboursSet.insert(role);
							} else if (concept->getOperandList()->getData()->getOperatorCode() == CCNOMINAL && concept->getOperandList()->isNegated()) {
								CIndividual* nomIndi = concept->getOperandList()->getData()->getNominalIndividual();
								roleRequiredSuccIndiSet.insert(TRoleIndividualPair(role, nomIndi));
							}
						}

					} else if (!negation && concept->getOperatorCode() == CCAQCHOOCE) {
						CRole* role = nullptr;
						if (concept->getOperandCount() >= 1) {
							CConcept* aqChOpConcept = concept->getOperandList()->getData();
							if (aqChOpConcept && aqChOpConcept->getOperatorCode() == CCAQSOME && !concept->getOperandList()->isNegated()) {
								role = aqChOpConcept->getRole();

								if (aqChOpConcept->getOperandCount() == 1 && aqChOpConcept->getOperandList()->getData()) {
									if (aqChOpConcept->getOperandList()->getData() == indiTriggerConcept && !concept->getOperandList()->isNegated()) {
										roleRequiredNeighboursSet.insert(role);
									} else if (aqChOpConcept->getOperandList()->getData()->getOperatorCode() == CCNOMINAL && !aqChOpConcept->getOperandList()->isNegated()) {
										CIndividual* nomIndi = aqChOpConcept->getOperandList()->getData()->getNominalIndividual();
										roleRequiredSuccIndiSet.insert(TRoleIndividualPair(role, nomIndi));
									}
								}

							}
						}

					}

				}
				if (!roleRequiredNeighboursSet.isEmpty() || !roleRequiredSuccIndiSet.isEmpty()) {
					conceptItem->setLazyNeighbourRealizationPruningRequired(true);
					CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();

					class CIndividualTestingVisitor : public CSameRealizationIndividualVisitor {
					public:
						CIndividualReference testingIndiRef;

						bool visitIndividual(const CIndividualReference& indiRef, CSameRealization* sameRealization) {
							testingIndiRef = indiRef;
							return false;
						}
					} visitor;

					QList<COntologyProcessingRequirement*> reqList;
					for (CRole* role : roleRequiredNeighboursSet) {
						for (CRealizationIndividualInstanceItemReference possInstItem : *possibleDirectInstanceItemSet) {
							sameRealization->visitSameIndividuals(possInstItem, &visitor);
							COntologyProcessingRoleRealizationRequirement* roleRealReq = new COntologyProcessingRoleRealizationRequirement(role, false, visitor.testingIndiRef);
							reqList.append(roleRealReq);
						}
					}
					for (const TRoleIndividualPair& roleIndiPair : roleRequiredSuccIndiSet) {
						CRole* role = roleIndiPair.first;
						CIndividual* succIndi = roleIndiPair.second;
						for (CRealizationIndividualInstanceItemReference possInstItem : *possibleDirectInstanceItemSet) {
							sameRealization->visitSameIndividuals(possInstItem, &visitor);
							COntologyProcessingRoleRealizationRequirement* roleRealReq = new COntologyProcessingRoleRealizationRequirement(role, false, visitor.testingIndiRef, succIndi);
							reqList.append(roleRealReq);
						}
					}
					CAnsweringMessageDataRequirementCompletedRealization* reqCompMess = new CAnsweringMessageDataRequirementCompletedRealization(conceptItem);
					processRequirements(answererContext, mOntoAnsweringItem->getOntology(), reqList, reqCompMess);
					conceptItem->incLazyRealizationProcessingCount();
					return true;
				}
				return false;
			}









			bool COptimizedComplexExpressionAnsweringHandler::checkConceptInstancesCompletelyResolveable(COptimizedComplexConceptItem* conceptItem, QSet<TConceptNegPair>* remainingConNegPair) {


				CRoleRealization* roleRealization = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();

				CConcept* indiTriggerConcept = mOntoAnsweringItem->getOntology()->getTBox()->getIndividualTriggerConcept();
				CConcept* topDataRangeConcept = mOntoAnsweringItem->getOntology()->getTBox()->getTopDataRangeConcept();
				CConcept* topConcept = mOntoAnsweringItem->getOntology()->getTBox()->getTopConcept();

				bool allConceptCompletelyHandled = true;


				for (TConceptNegPair conNegPair : *conceptItem->getAtomicConcpetSet()) {

					bool conceptCompletelyHandled = false;

					CConcept* concept = conNegPair.first;
					bool negation = conNegPair.second;
					if (!negation && concept->getOperatorCode() == CCSOME) {
						// TODO: we should check for the special concept O here instead of top
						CRole* role = concept->getRole();
						if (!role->isDataRole() && concept->getOperandCount() == 1 && concept->getOperandList()->getData()) {
							if (concept->getOperandList()->getData() == indiTriggerConcept && !concept->getOperandList()->isNegated()) {
								conceptCompletelyHandled = true;
							} else if (concept->getOperandList()->getData()->getOperatorCode() == CCNOMINAL && !concept->getOperandList()->isNegated()) {
								conceptCompletelyHandled = true;
							} else if (concept->getOperandList()->getData() == topConcept && !concept->getOperandList()->isNegated()) {
								conceptCompletelyHandled = true;
							}
						}

						if (role->isDataRole() && !conceptCompletelyHandled && concept->getOperandCount() == 1 && concept->getOperandList()->getData()) {

							CConcept* opConcept = concept->getOperandList()->getData();
							bool opConNegation = concept->getOperandList()->isNegated();

							if (opConcept == topDataRangeConcept && !opConNegation) {
								conceptCompletelyHandled = true;
							} else if (opConcept->getOperatorCode() == CCDATALITERAL && !opConNegation) {
								if (canRetrieveInstantiatingDataValueIndividuals(role, opConcept->getDataLiteral())) {
									conceptCompletelyHandled = true;
								}
							}
						}

					} else if (negation && concept->getOperatorCode() == CCALL) {
						// TODO: we should check for the special concept O here instead of top
						CRole* role = concept->getRole();
						if (!role->isDataRole() && concept->getOperandCount() == 1 && concept->getOperandList()->getData()) {
							if (concept->getOperandList()->getData() == indiTriggerConcept && concept->getOperandList()->isNegated()) {
								conceptCompletelyHandled = true;
							} else if (concept->getOperandList()->getData()->getOperatorCode() == CCNOMINAL && concept->getOperandList()->isNegated()) {
								conceptCompletelyHandled = true;
							} else if (concept->getOperandList()->getData() == topConcept && concept->getOperandList()->isNegated()) {
								conceptCompletelyHandled = true;
							}
						}

						if (role->isDataRole() && !conceptCompletelyHandled && concept->getOperandCount() == 1 && concept->getOperandList()->getData()) {

							CConcept* opConcept = concept->getOperandList()->getData();
							bool opConNegation = concept->getOperandList()->isNegated();

							if (opConcept == topDataRangeConcept && opConNegation) {
								conceptCompletelyHandled = true;
							} else if (opConcept->getOperatorCode() == CCDATALITERAL && opConNegation) {
								if (canRetrieveInstantiatingDataValueIndividuals(role, opConcept->getDataLiteral())) {
									conceptCompletelyHandled = true;
								}
							}
						}
					} else if (!negation && concept->getOperatorCode() == CCVALUE) {
						conceptCompletelyHandled = true;
					} else if (!negation && concept->hasClassName()) {
						conceptCompletelyHandled = true;
					} else if (!negation && concept->getOperatorCode() == CCAQCHOOCE) {
						bool indiTriggerConceptRestriction = false;
						if (concept->getOperandCount() >= 1) {
							CConcept* aqChOpConcept = concept->getOperandList()->getData();
							if (aqChOpConcept && aqChOpConcept->getOperatorCode() == CCAQSOME && !concept->getOperandList()->isNegated()) {

								if (aqChOpConcept->getOperandCount() == 1) {
									if (aqChOpConcept->getOperandList()->getData() == indiTriggerConcept && !aqChOpConcept->getOperandList()->isNegated()) {
										indiTriggerConceptRestriction = true;
									} else if (aqChOpConcept->getOperandList()->getData()->getOperatorCode() == CCNOMINAL && !aqChOpConcept->getOperandList()->isNegated()) {
										indiTriggerConceptRestriction = true;
									} else if (aqChOpConcept->getOperandList()->getData() == topConcept && !aqChOpConcept->getOperandList()->isNegated()) {
										conceptCompletelyHandled = true;
									}
								}

							}
						}
						if (indiTriggerConceptRestriction) {
							conceptCompletelyHandled = true;
						}
					}

					if (!conceptCompletelyHandled) {
						allConceptCompletelyHandled = false;
						if (remainingConNegPair) {
							remainingConNegPair->insert(conNegPair);
						}
					}
				}

				return allConceptCompletelyHandled;
			}




			bool COptimizedComplexExpressionAnsweringHandler::checkPossibleInstancesCompletelyHandled(COptimizedComplexConceptItem* conceptItem, CAnswererContext* answererContext) {

				if (!conceptItem->getCompletelyHandledReducedInstanceTestingConcept()) {

					QSet<TConceptNegPair> remainingConNegPair;
					bool allConceptCompletelyHandled = checkConceptInstancesCompletelyResolveable(conceptItem, &remainingConNegPair);
					conceptItem->setCompletelyHandledReducedInstanceTestingConcept(conceptItem->getConcept());
					conceptItem->setCompletelyHandledReducedInstanceTestingConceptNegation(conceptItem->getConceptNegation());

					conceptItem->setAllAtomicConceptsCompletelyHandled(allConceptCompletelyHandled);
					if (allConceptCompletelyHandled) {
						QSet<CRealizationIndividualInstanceItemReference>* possibleDirectInstanceItemSet = conceptItem->getPossibleInstanceItemSet();
						COptimizedComplexConceptInstanziatedIndividualItemHash* knownDirectInstanceItems = conceptItem->getKnownInstanceItems();

						if (possibleDirectInstanceItemSet) {
							mTestedPossibleInstancesCount += possibleDirectInstanceItemSet->size();
							for (const CRealizationIndividualInstanceItemReference& item : *possibleDirectInstanceItemSet) {
								knownDirectInstanceItems->addRealizationIndividualInstanceItemReference(item);
							}
							possibleDirectInstanceItemSet->clear();
						}
						return true;
					} else {
						// create reduced testing concept
						CMemoryAllocationManager* memMan = mOntoAnsweringItem->getTestingOntology(false)->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();
						CTBox* tBox = mOntoAnsweringItem->getTestingOntology(false)->getDataBoxes()->getTBox();
						CConceptVector* conceptVec = tBox->getConceptVector();

						CConcept* reducedConcept = CObjectAllocator< CConcept >::allocateAndConstruct(memMan);
						reducedConcept->initConcept();
						cint64 nextConTag = tBox->getNextConceptID();
						reducedConcept->initTag(nextConTag);
						reducedConcept->setOperatorCode(CCAND);
						conceptVec->setLocalData(nextConTag, reducedConcept);

						for (TConceptNegPair conNegPar : remainingConNegPair) {
							CConcept* opConcept = conNegPar.first;
							bool opConceptNegation = conNegPar.second;

							CSortedNegLinker<CConcept*>* newIndiTrigOpCon = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(memMan);
							newIndiTrigOpCon->init(opConcept, opConceptNegation, reducedConcept->getOperandList());
							reducedConcept->setOperandList(newIndiTrigOpCon);
							reducedConcept->incOperandCount();
						}
						conceptItem->setCompletelyHandledReducedInstanceTestingConcept(reducedConcept);
						conceptItem->setCompletelyHandledReducedInstanceTestingConceptNegation(false);

					}
				} else if (conceptItem->getAllAtomicConceptsCompletelyHandled()) {
					QSet<CRealizationIndividualInstanceItemReference>* possibleDirectInstanceItemSet = conceptItem->getPossibleInstanceItemSet();
					COptimizedComplexConceptInstanziatedIndividualItemHash* knownDirectInstanceItems = conceptItem->getKnownInstanceItems();
					if (possibleDirectInstanceItemSet) {
						mTestedPossibleInstancesCount += possibleDirectInstanceItemSet->size();
						for (const CRealizationIndividualInstanceItemReference& item : *possibleDirectInstanceItemSet) {
							knownDirectInstanceItems->addRealizationIndividualInstanceItemReference(item);
						}
						possibleDirectInstanceItemSet->clear();
					}
					return true;
				}
				return false;
			}







			bool COptimizedComplexExpressionAnsweringHandler::neighboursPrunePossibleInstances(COptimizedComplexConceptItem* conceptItem, CAnswererContext* answererContext) {
				CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();
				CRoleRealization* roleRealization = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();
				QSet<CRealizationIndividualInstanceItemReference>* possibleDirectInstanceItemSet = conceptItem->getPossibleInstanceItemSet();
				CConcept* indiTriggerConcept = mOntoAnsweringItem->getOntology()->getTBox()->getIndividualTriggerConcept();

				QSet<CRole*> roleRequiredNeighboursSet;
				QSet<TRoleIndividualPair> roleRequiredSuccIndiSet;
				for (TConceptNegPair conNegPair : *conceptItem->getAtomicConcpetSet()) {
					CConcept* concept = conNegPair.first;
					bool negation = conNegPair.second;
					if (!negation && concept->getOperatorCode() == CCSOME) {
						CRole* role = concept->getRole();
						if (concept->getOperandCount() == 1 && concept->getOperandList()->getData()) {
							if (concept->getOperandList()->getData() == indiTriggerConcept && !concept->getOperandList()->isNegated()) {
								roleRequiredNeighboursSet.insert(role);
							} else if (concept->getOperandList()->getData()->getOperatorCode() == CCNOMINAL && !concept->getOperandList()->isNegated()) {
								CIndividual* nomIndi = concept->getOperandList()->getData()->getNominalIndividual();
								roleRequiredSuccIndiSet.insert(TRoleIndividualPair(role, nomIndi));
							}
						}
					} else if (negation && concept->getOperatorCode() == CCALL) {
						CRole* role = concept->getRole();
						if (concept->getOperandCount() == 1 && concept->getOperandList()->getData()) {
							if (concept->getOperandList()->getData() == indiTriggerConcept && concept->getOperandList()->isNegated()) {
								roleRequiredNeighboursSet.insert(role);
							} else if (concept->getOperandList()->getData()->getOperatorCode() == CCNOMINAL && concept->getOperandList()->isNegated()) {
								CIndividual* nomIndi = concept->getOperandList()->getData()->getNominalIndividual();
								roleRequiredSuccIndiSet.insert(TRoleIndividualPair(role, nomIndi));
							}
						}
					} else if (!negation && concept->getOperatorCode() == CCAQCHOOCE) {
						CRole* role = nullptr;
						if (concept->getOperandCount() >= 1) {
							CConcept* aqChOpConcept = concept->getOperandList()->getData();
							if (aqChOpConcept && aqChOpConcept->getOperatorCode() == CCAQSOME && !concept->getOperandList()->isNegated()) {
								role = aqChOpConcept->getRole();

								if (aqChOpConcept->getOperandCount() == 1 && aqChOpConcept->getOperandList()->getData()) {
									if (aqChOpConcept->getOperandList()->getData() == indiTriggerConcept && !aqChOpConcept->getOperandList()->isNegated()) {
										roleRequiredNeighboursSet.insert(role);
									} else if (aqChOpConcept->getOperandList()->getData()->getOperatorCode() == CCNOMINAL && !aqChOpConcept->getOperandList()->isNegated()) {
										CIndividual* nomIndi = aqChOpConcept->getOperandList()->getData()->getNominalIndividual();
										roleRequiredSuccIndiSet.insert(TRoleIndividualPair(role, nomIndi));
									}
								}

							}
						}
					}
				}
				

				if (roleRealization && sameRealization) {
					class CPossibleInstancesRoleNeighbourPruningVisitor : public CRoleRealizationInstanceVisitor {
					public:
						bool mHasRoleNeighbourFlag = false;

						bool visitRoleInstance(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleRealization* roleRealization) {
							mHasRoleNeighbourFlag = true;
							return false;
						}
					};

					class CIndividualTestingVisitor : public CSameRealizationIndividualVisitor {
					public:
						CIndividualReference testingIndiRef;

						bool visitIndividual(const CIndividualReference& indiRef, CSameRealization* sameRealization) {
							testingIndiRef = indiRef;
							return false;
						}
					} sameInstanceVisitor;

					CPossibleInstancesRoleNeighbourPruningVisitor roleInstanceVisitor;
					for (QSet<CRealizationIndividualInstanceItemReference>::iterator it = possibleDirectInstanceItemSet->begin(); it != possibleDirectInstanceItemSet->end(); ) {
						CRealizationIndividualInstanceItemReference possInstItem(*it);
						bool hasRoleNeighbour = true;

						for (QSet<TRoleIndividualPair>::const_iterator itRoleSucc = roleRequiredSuccIndiSet.constBegin(), itRoleSuccEnd = roleRequiredSuccIndiSet.constEnd(); hasRoleNeighbour && itRoleSucc != itRoleSuccEnd; ++itRoleSucc) {
							CRole* role = itRoleSucc->first;
							CIndividual* succIndi = itRoleSucc->second;
							roleInstanceVisitor.mHasRoleNeighbourFlag = false;
							sameRealization->visitSameIndividuals(possInstItem, &sameInstanceVisitor);
							hasRoleNeighbour = roleRealization->isRoleInstance(role, sameInstanceVisitor.testingIndiRef, succIndi);
						}

						for (QSet<CRole*>::const_iterator itRole = roleRequiredNeighboursSet.constBegin(), itRoleEnd = roleRequiredNeighboursSet.constEnd(); hasRoleNeighbour && itRole != itRoleEnd; ++itRole) {
							CRole* role(*itRole);
							roleInstanceVisitor.mHasRoleNeighbourFlag = false;
							roleRealization->visitTargetIndividuals(roleRealization->getRoleInstanceItemReference(possInstItem), roleRealization->getRoleInstantiatedItem(role), &roleInstanceVisitor);
							hasRoleNeighbour = roleInstanceVisitor.mHasRoleNeighbourFlag;
						}

						if (!hasRoleNeighbour) {
							mTestedPossibleInstancesCount++;
							it = possibleDirectInstanceItemSet->erase(it);
						} else {
							++it;
						}
					}
				}
				return true;
			}


			bool COptimizedComplexExpressionAnsweringHandler::searchKnownPossibleInstances(COptimizedComplexConceptItem* conceptItem, CAnswererContext* answererContext) {
				CConceptRealization* conceptRealization = mOntoAnsweringItem->getOntology()->getRealization()->getConceptRealization();
				CRoleRealization* roleRealization = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();
				QSet<CRealizationIndividualInstanceItemReference>* possibleDirectInstanceItemSet = new QSet<CRealizationIndividualInstanceItemReference>();
				COptimizedComplexConceptInstanziatedIndividualItemHash* knownDirectInstanceItems = new COptimizedComplexConceptInstanziatedIndividualItemHash();
				bool possibleInstancesInitialized = false;

				class CPossibleInstancesOptimizedComplexConceptItemVisitor : public CConceptRealizationInstanceVisitor {
				public:
					QSet<CRealizationIndividualInstanceItemReference>* mInstanceItemSet;

					CPossibleInstancesOptimizedComplexConceptItemVisitor(QSet<CRealizationIndividualInstanceItemReference>* instanceItemSet) {
						mInstanceItemSet = instanceItemSet;
					}

					bool visitInstance(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CConceptRealization* conRealization) {
						mInstanceItemSet->insert(indiRealItemRef);
						return true;
					}
				};

				if (conceptRealization) {
					CHierarchyNode* topHierNode = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy()->getTopHierarchyNode();
					CHierarchyNode* bottomHierNode = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy()->getBottomHierarchyNode();

					QSet<CHierarchyNode*>* superClassNodeSet = conceptItem->getDirectSuperClassNodeSet();
					QSet<CHierarchyNode*>* subClassNodeSet = conceptItem->getDirectSubClassNodeSet();
					QSet<CHierarchyNode*> searchingInstanceNodeSet;
					QList<CHierarchyNode*> searchingInstanceNodeList;
					for (auto node : *superClassNodeSet) {
						searchingInstanceNodeSet.insert(node);
						if (!subClassNodeSet->contains(node)) {
							searchingInstanceNodeList.append(node);
						}
						QSet<CRealizationIndividualInstanceItemReference> tmpDirectInstanceItemSet;
						QSet<CRealizationIndividualInstanceItemReference>* fillDirectInstanceItemSet = possibleDirectInstanceItemSet;
						if (possibleInstancesInitialized) {
							fillDirectInstanceItemSet = &tmpDirectInstanceItemSet;
						}
						while (!searchingInstanceNodeList.isEmpty()) {
							auto node = searchingInstanceNodeList.takeFirst();

							if (node == topHierNode) {

								QSet<CRealizationIndividualInstanceItemReference> excludeDirectInstanceItemSet;
								for (auto subNode : *subClassNodeSet) {
									if (subNode != bottomHierNode) {
										CPossibleInstancesOptimizedComplexConceptItemVisitor possibleRealizationVisitor(&excludeDirectInstanceItemSet);
										conceptRealization->visitDirectInstances(subNode->getOneEquivalentConcept(), &possibleRealizationVisitor);
									}
								}



								visitAllIndividuals([&](const CIndividualReference& indiRef)->bool {
									CRealizationIndividualInstanceItemReference indiItemRef = conceptRealization->getInstanceItemReference(indiRef);
									if (!excludeDirectInstanceItemSet.contains(indiItemRef)) {
										fillDirectInstanceItemSet->insert(indiItemRef);
									}
									return true;
								});


							} else {

								CPossibleInstancesOptimizedComplexConceptItemVisitor possibleRealizationVisitor(fillDirectInstanceItemSet);
								conceptRealization->visitDirectInstances(node->getOneEquivalentConcept(), &possibleRealizationVisitor);

								for (auto childNode : *node->getChildNodeSet()) {
									if (!subClassNodeSet->contains(childNode)) {
										searchingInstanceNodeList.append(childNode);
										searchingInstanceNodeSet.insert(childNode);
									}
								}
							}
						}
						if (!possibleInstancesInitialized) {
							possibleInstancesInitialized = true;
						} else {
							possibleDirectInstanceItemSet->intersect(tmpDirectInstanceItemSet);
						}

					}

				}



				if (roleRealization) {
					class CPossibleInstancesOptimizedComplexRoleItemVisitor : public CRoleRealizationInstanceVisitor {
					public:
						QSet<CRealizationIndividualInstanceItemReference>* mInstanceItemSet;

						CPossibleInstancesOptimizedComplexRoleItemVisitor(QSet<CRealizationIndividualInstanceItemReference>* instanceItemSet) {
							mInstanceItemSet = instanceItemSet;
						}

						bool visitRoleInstance(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleRealization* roleRealization) {
							mInstanceItemSet->insert(indiRealItemRef);
							return true;
						}
					};

					for (TConceptNegPair conNegPair : *conceptItem->getAtomicConcpetSet()) {
						CConcept* concept = conNegPair.first;
						bool negation = conNegPair.second;
						if (!negation && concept->getOperatorCode() == CCVALUE) {

							CIndividual* nomIndividual = concept->getNominalIndividual();
							CRole* role = concept->getRole();

							if (!possibleInstancesInitialized) {
								possibleInstancesInitialized = true;
								CPossibleInstancesOptimizedComplexRoleItemVisitor possibleRealizationVisitor(possibleDirectInstanceItemSet);
								roleRealization->visitSourceIndividuals(nomIndividual, role, &possibleRealizationVisitor);
							} else {
								QSet<CRealizationIndividualInstanceItemReference> tmpDirectInstanceItemSet;
								CPossibleInstancesOptimizedComplexRoleItemVisitor possibleRealizationVisitor(&tmpDirectInstanceItemSet);
								roleRealization->visitSourceIndividuals(nomIndividual, role, &possibleRealizationVisitor);
								possibleDirectInstanceItemSet->intersect(tmpDirectInstanceItemSet);
							}
						}
					}


				}
					
				class CSuperInstancesOptimizedComplexConceptItemVisitor : public COptimizedComplexConceptItemVisitor {
				public:
					QSet<CRealizationIndividualInstanceItemReference>* mPossibleDirectInstanceItemSet;
					CConceptRealization* mConceptRealization;

					CSuperInstancesOptimizedComplexConceptItemVisitor(QSet<CRealizationIndividualInstanceItemReference>* possibleDirectInstanceItemSet, CConceptRealization* conceptRealization) {
						mPossibleDirectInstanceItemSet = possibleDirectInstanceItemSet;
						mConceptRealization = conceptRealization;
					}

					cint64 mSuperItemVisitingLimit = 50;
					cint64 mSuperItemResultVisitingLimit = 5;


					bool visitComplexSuperConceptItem(COptimizedComplexConceptItem* item, bool& continueVisitingSuperItems) {
						CComplexConceptStepComputationProcess* compStep = item->getComputationProcess()->getInstancesComputationProcess(false);
						if (compStep && compStep->isComputationProcessFinished()) {
							COptimizedComplexConceptInstanziatedIndividualItemHash* superInstanceItems = item->getKnownInstanceItems();
							if (superInstanceItems) {
								QSet<CRealizationIndividualInstanceItemReference> superIndirectAllInstanceItemSet;
								for (auto itemLinker = superInstanceItems->getAddedRealizationIndividualInstanceItemReferenceLinker(); itemLinker; itemLinker = itemLinker->getNext()) {
									superIndirectAllInstanceItemSet.insert(itemLinker->getRealizationIndividualInstanceItemReference());
								}
								// may be a little bit inefficient
								CPossibleInstancesOptimizedComplexConceptItemVisitor possibleRealizationVisitor(&superIndirectAllInstanceItemSet);
								for (auto node : *item->getDirectSubClassNodeSet()) {
									mConceptRealization->visitInstances(node->getOneEquivalentConcept(), false, &possibleRealizationVisitor);
								}
								mPossibleDirectInstanceItemSet->intersect(superIndirectAllInstanceItemSet);
								continueVisitingSuperItems = false;
								if (--mSuperItemResultVisitingLimit < 0) {
									return false;
								}
							}
						}
						if (--mSuperItemVisitingLimit < 0) {
							return false;
						}
						return true;
					}
				} superItemVisitor(possibleDirectInstanceItemSet, conceptRealization);
				visitSuperSubConceptItems(conceptItem, false, true, &superItemVisitor);




				class CSubInstancesOptimizedComplexConceptItemVisitor : public COptimizedComplexConceptItemVisitor {
				public:
					QSet<CRealizationIndividualInstanceItemReference>* mPossibleDirectInstanceItemSet;
					COptimizedComplexConceptInstanziatedIndividualItemHash* mKnownDirectInstanceItems;

					CSubInstancesOptimizedComplexConceptItemVisitor(QSet<CRealizationIndividualInstanceItemReference>* possibleDirectInstanceItemSet, COptimizedComplexConceptInstanziatedIndividualItemHash* knownDirectInstanceItems) {
						mPossibleDirectInstanceItemSet = possibleDirectInstanceItemSet;
						mKnownDirectInstanceItems = knownDirectInstanceItems;
					}

					cint64 mSubItemVisitingLimit = 50;
					cint64 mSubItemResultVisitingLimit = 5;

					bool visitComplexSuperConceptItem(COptimizedComplexConceptItem* item, bool& continueVisitingSuperItems) {
						CComplexConceptStepComputationProcess* compStep = item->getComputationProcess()->getInstancesComputationProcess(false);
						if (compStep && compStep->isComputationProcessFinished()) {
							COptimizedComplexConceptInstanziatedIndividualItemHash* subInstanceItems = item->getKnownInstanceItems();
							if (subInstanceItems) {
								for (auto itemLinker = subInstanceItems->getAddedRealizationIndividualInstanceItemReferenceLinker(); itemLinker; itemLinker = itemLinker->getNext()) {
									auto item = itemLinker->getRealizationIndividualInstanceItemReference();
									if (mPossibleDirectInstanceItemSet->contains(item)) {
										mKnownDirectInstanceItems->addRealizationIndividualInstanceItemReference(item);
										mPossibleDirectInstanceItemSet->remove(item);
									}
								}
								continueVisitingSuperItems = false;
								if (--mSubItemResultVisitingLimit < 0) {
									return false;
								}
							}
						}
						if (--mSubItemVisitingLimit < 0) {
							return false;
						}
						return true;
					}
				} subItemVisitor(possibleDirectInstanceItemSet, knownDirectInstanceItems);
				visitSuperSubConceptItems(conceptItem, true, false, &subItemVisitor);



				conceptItem->setKnownInstanceItems(knownDirectInstanceItems);
				conceptItem->setPossibleInstanceItemSet(possibleDirectInstanceItemSet);

				mTotalPossibleInstancesCount += possibleDirectInstanceItemSet->size();

				return true;
			}




			bool COptimizedComplexExpressionAnsweringHandler::createSuperClassSubsumptionTest(COptimizedComplexConceptItem* conceptItem, CHierarchyNode* testingNode, CAnswererContext* answererContext) {
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());
				CConcept* superClassConcept = testingNode->getOneEquivalentConcept();
				satCalcJob = extendProcessingByTopPropagation(satCalcJobGen, satCalcJob, conceptItem->isTopObjectPropertyUsed(), answererContext);
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(conceptItem->getConcept(), conceptItem->getConceptNegation(), superClassConcept, true, nullptr, satCalcJob);
				CAnsweringMessageDataCalculationCompletedSubsumptionSuperClass* completedMessage = new CAnsweringMessageDataCalculationCompletedSubsumptionSuperClass(satCalcJob, conceptItem, testingNode);
				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incComplexConceptItemsSuperClassSubsumptionTestingCount();
				processCalculationJob(answererContext, satCalcJob, completedMessage);
				return true;
			}



			bool COptimizedComplexExpressionAnsweringHandler::createSubClassSubsumptionTest(COptimizedComplexConceptItem* conceptItem, CHierarchyNode* testingNode, CAnswererContext* answererContext) {
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());
				CConcept* subClassConcept = testingNode->getOneEquivalentConcept();
				satCalcJob = extendProcessingByTopPropagation(satCalcJobGen, satCalcJob, conceptItem->isTopObjectPropertyUsed(), answererContext);
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(subClassConcept, false, conceptItem->getConcept(), !conceptItem->getConceptNegation(), nullptr, satCalcJob);
				CAnsweringMessageDataCalculationCompletedSubsumptionSubClass* completedMessage = new CAnsweringMessageDataCalculationCompletedSubsumptionSubClass(satCalcJob, conceptItem, testingNode);
				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incComplexConceptItemsSubClassSubsumptionTestingCount();
				processCalculationJob(answererContext, satCalcJob, completedMessage);
				return true;
			}



			bool COptimizedComplexExpressionAnsweringHandler::createInvidiualInstanceTest(COptimizedComplexConceptItem* conceptItem, const CIndividualReference& testingIndiRef, CAnswererContext* answererContext) {
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());
				satCalcJob = extendProcessingByTopPropagation(satCalcJobGen, satCalcJob, conceptItem->isTopObjectPropertyUsed(), answererContext);
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(conceptItem->getConcept(), !conceptItem->getConceptNegation(), testingIndiRef, satCalcJob);
				CAnsweringMessageDataCalculationCompletedInstanceIndividual* completedMessage = new CAnsweringMessageDataCalculationCompletedInstanceIndividual(satCalcJob, conceptItem, testingIndiRef);

				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incComplexConceptItemsPossibleInstanceVerificationCount();

				processCalculationJob(answererContext, satCalcJob, completedMessage);
				return true;
			}










			CConcreteOntology* COptimizedComplexExpressionAnsweringHandler::createQueryMaterializationData(QSet<CExpressionVariable *> varExpSet, QSet<CExpressionVariable *> rolledVarExpSet, QSet<CExpressionVariable *> anonymousIndiVariableSet, QHash<CExpressionVariable *, CBuildExpression *> rebuiltVarClassTermExp, QHash<CExpressionVariable *, CAxiomExpression *> varExpAxiomExpHash,
					bool allAnonymousVariables, QSet<CExpressionVariable *> initialAnonymousIndiVariableSet, QList<CDataPropertyAssertionExpression *> bothVarDataPropAssPropagationList, QList<CObjectPropertyAssertionExpression *> bothVarPropAssPropagationList, QList<CDataPropertyAssertionExpression *> onlyDataVarDataPropAssList, COptimizedComplexBuildingIndividualVariableCompositionsItem* varBuildItem, CAnswererContext* answererContext) {


				CComplexQueryFinishingBuildingVariableCompositionsItemData* buildFinishingData = varBuildItem->getBuildingFinishingData();
				if (!buildFinishingData) {
					buildFinishingData = new CComplexQueryFinishingBuildingVariableCompositionsItemData();
					buildFinishingData->varExpSet = varExpSet;
					buildFinishingData->rolledVarExpSet = rolledVarExpSet;
					buildFinishingData->anonymousIndiVariableSet = anonymousIndiVariableSet;
					buildFinishingData->rebuiltVarClassTermExp = rebuiltVarClassTermExp;
					buildFinishingData->varExpAxiomExpHash = varExpAxiomExpHash;
					buildFinishingData->allAnonymousVariables = allAnonymousVariables;
					buildFinishingData->initialAnonymousIndiVariableSet = initialAnonymousIndiVariableSet;
					buildFinishingData->bothVarDataPropAssPropagationList = bothVarDataPropAssPropagationList;
					buildFinishingData->bothVarPropAssPropagationList = bothVarPropAssPropagationList;
					buildFinishingData->onlyDataVarDataPropAssList = onlyDataVarDataPropAssList;
					varBuildItem->setBuildingFinishingData(buildFinishingData);
				}

				CConcreteOntology* ontology = mOntoAnsweringItem->getTestingOntology();
				CComplexQueryMaterializationData* matData = varBuildItem->getMaterializationData();
				if (!matData) {
					matData = new CComplexQueryMaterializationData();
					varBuildItem->setMaterializationData(matData);
				}
				CConcreteOntology* tmpMatOntology = matData->getTemporaryMaterializationOntology();
				if (!tmpMatOntology) {
					tmpMatOntology = new CConcreteOntology(ontology, ontology->getConfiguration());
					tmpMatOntology->setOntologyID(ontology->getOntologyID());
					tmpMatOntology->setConsistence(ontology->getConsistence());
					tmpMatOntology->getTBox()->setMinimalNextConceptID(ontology->getTBox()->getMinimalNextConceptID());
					matData->setTemporaryMaterializationOntology(tmpMatOntology);

					CABox* tempRoleRealAbox = tmpMatOntology->getABox();
					CIndividualVector* indiVec = tempRoleRealAbox->getIndividualVector(true);

					CClassTermExpression* topClassTermExp = tmpMatOntology->getBuildData()->getTopClassExpression();
					CObjectPropertyTermExpression* topObjPropExp = tmpMatOntology->getBuildData()->getTopObjectPropertyExpression();

					for (CExpressionVariable* varExp : varExpSet) {

						if (!rolledVarExpSet.contains(varExp)) {
							CIndividual* varIndi = matData->getVariableIndividual(varExp);


							QList<CBuildExpression*> varExpList = rebuiltVarClassTermExp.values(varExp);

							QList<CConcept*> conceptList;

							for (CBuildExpression* varClassTermExp : varExpList) {
								CClassTermExpression* classTermExp = dynamic_cast<CClassTermExpression*>(varClassTermExp);

								CConcept* concept = tmpMatOntology->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value((CClassTermExpression*)classTermExp);
								if (concept) {
									conceptList.append(concept);
								}

							}



							for (CConcept* concept : conceptList) {
								CConceptAssertionLinker* nextConceptAssertionLinker = new CConceptAssertionLinker();
								nextConceptAssertionLinker->initNegLinker(concept, concept->hasMappingNegation());

								varIndi->addAssertionConceptLinker(nextConceptAssertionLinker);
							}




							QList<CAxiomExpression*> assExplist = varExpAxiomExpHash.values(varExp);
							for (CAxiomExpression* assExp : assExplist) {
								CObjectPropertyAssertionExpression* propAssExp = dynamic_cast<CObjectPropertyAssertionExpression*>(assExp);
								if (propAssExp) {
									CObjectPropertyTermExpression* propExp = propAssExp->getObjectPropertyTermExpression();
									if (propExp != topObjPropExp) {
										CRole* role = tmpMatOntology->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(propExp);
										CIndividualTermExpression* firstIndiExp = propAssExp->getFirstIndividualTermExpression();
										CIndividualTermExpression* secondIndiExp = propAssExp->getSecondIndividualTermExpression();
										CIndividualVariableExpression* firstVarExp = dynamic_cast<CIndividualVariableExpression*>(firstIndiExp);
										CIndividualVariableExpression* secondVarExp = dynamic_cast<CIndividualVariableExpression*>(secondIndiExp);
										if ((!firstVarExp || !rolledVarExpSet.contains(firstVarExp)) && (!secondVarExp || !rolledVarExpSet.contains(secondVarExp))) {
											CIndividual* firstIndi = nullptr;
											CIndividual* secondIndi = nullptr;
											if (firstVarExp) {
												firstIndi = matData->getVariableIndividual(firstVarExp);
											} else {
												firstIndi = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getIndividulTermIndiMappingHash()->value(firstIndiExp);
												if (!firstIndi) {
													CNamedIndividualExpression* namedFirstIndiExp = dynamic_cast<CNamedIndividualExpression*>(firstIndiExp);
													if (namedFirstIndiExp) {
														firstIndi = mOntoAnsweringItem->getTestingOntology()->getStringMapping()->getIndividualFromName(namedFirstIndiExp->getName());
													}
												}
												if (firstIndi) {
													firstIndi = CConceptRoleIndividualLocator::getLocatedIndividual(firstIndi, tmpMatOntology);
												}
											}
											if (secondVarExp) {
												secondIndi = matData->getVariableIndividual(secondVarExp);
											} else {
												secondIndi = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getIndividulTermIndiMappingHash()->value(secondIndiExp);
												if (!secondIndi) {
													CNamedIndividualExpression* namedSecondIndiExp = dynamic_cast<CNamedIndividualExpression*>(secondIndiExp);
													if (namedSecondIndiExp) {
														secondIndi = mOntoAnsweringItem->getTestingOntology()->getStringMapping()->getIndividualFromName(namedSecondIndiExp->getName());
													}
												}
												if (secondIndi) {
													secondIndi = CConceptRoleIndividualLocator::getLocatedIndividual(secondIndi, tmpMatOntology);
												}
											}

											if (firstIndi && secondIndi) {
												CRoleAssertionLinker* nextRoleAssertionLinker = new CRoleAssertionLinker();
												nextRoleAssertionLinker->initRoleAssertionLinker(role, secondIndi);
												firstIndi->addAssertionRoleLinker(nextRoleAssertionLinker);

												CReverseRoleAssertionLinker* nextReverseRoleAssertionLinker = new CReverseRoleAssertionLinker();
												nextReverseRoleAssertionLinker->initReverseRoleAssertionLinker(nextRoleAssertionLinker, firstIndi);
												secondIndi->addReverseAssertionRoleLinker(nextReverseRoleAssertionLinker);
											}

										}
									}
								}

								//CDataPropertyAssertionExpression* dataPropAssExp = dynamic_cast<CDataPropertyAssertionExpression*>(assExp);
								//if (dataPropAssExp) {
								//	CDataPropertyTermExpression* dataPropTermExp = dataPropAssExp->getDataPropertyTermExpression();
								//	CIndividualVariableExpression* indiVarExp = dynamic_cast<CIndividualVariableExpression*>(dataPropAssExp->getIndividualTermExpression());
								//	CDataLiteralVariableExpression* dataVarExp = dynamic_cast<CDataLiteralVariableExpression*>(dataPropAssExp->getDataLiteralTermExpression());
								//	if (indiVarExp && !dataVarExp) {
								//		CRole* role = tmpMatOntology->getDataBoxes()->getExpressionDataBoxMapping()->getDataPropertyTermRoleMappingHash()->value(dataPropTermExp);
								//		CDataLiteralExpression* dataLiteralExp = (CDataLiteralExpression*)dataPropAssExp->getDataLiteralTermExpression();


								//		CDataLiteral* dataLiteral = CObjectParameterizingAllocator<CDataLiteral, CContext*>::allocateAndConstructAndParameterize(mMemMan, updateConcreteOntology->getDataBoxes()->getBoxContext());
								//		dataLiteral->initDataLiteral(QString::fromUtf8(literalValue), datatype);

								//		createDataLiteralValue(dataLiteral, updateConcreteOntology);

								//		CDataAssertionLinker* dataAssertionLinker = new CDataAssertionLinker();
								//		dataAssertionLinker->initDataAssertionLinker(role, )
								//		

								//		dataLiteralExp = (CDataLiteralExpression*)mTestingOntologyBuilder->rebuildExpression(dataLiteralExp);
								//		instTestingExpList.append(mTestingOntologyBuilder->getDataHasValue(dataPropTermExp, dataLiteralExp));
								//	}

								//}
							}
						}


					}



				}
				return tmpMatOntology;
			}



			bool COptimizedComplexExpressionAnsweringHandler::createQueryMaterializationTest(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext) {
				CSatisfiableCalculationJob* satCalcJob = nullptr;

				CComplexQueryMaterializationData* matData = buildingVarItem->getMaterializationData();

				CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());

				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(matData->getVariableIndividualList());



				CAnsweringMessageDataCalculationCompletedQueryMaterialization* completedMessage = new CAnsweringMessageDataCalculationCompletedQueryMaterialization(satCalcJob, buildingVarItem, matData);
				satCalcJob->setSatisfiableAnswererMaterializationAdapter(new CSatisfiableTaskAnswererQueryingMaterializationAdapter(matData->getVariableIndividualList(), (cint64)matData, this, mOntoAnsweringItem->getTestingOntology(false), answererContext->getAnsweringCalculationHandler()));
				

				//mOntoAnsweringItem->getAnsweringHandlingStatistics()->getComplexConceptItemsPossibleInstanceVerificationCount();

				processCalculationJob(answererContext, satCalcJob, completedMessage);
				return true;
			}



			static bool tuplesSortMoreHierachyDepthThan(const TDepthHierarchyNodeConceptClassTermExpressionTuple& item1, const TDepthHierarchyNodeConceptClassTermExpressionTuple& item2) {
				return item1.first.first == -1 || item1.first.first > item2.first.first;
			}


			bool COptimizedComplexExpressionAnsweringHandler::processQueryMaterializationCalculationCompleted(CAnsweringMessageDataCalculationCompletedQueryMaterialization* message, CAnswererContext* answererContext) {

				COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem = message->getBuildingVariableCompositionItem();
				CComplexQueryMaterializationData* matData = message->getMaterializationData();
				CComplexQueryExpressionProcessingData* queryProcessingData = buildingVarItem->getQueryProcessingData();

				if (!message->isSatisfiable()) {
					buildingVarItem->setSatisfiability(false);

					LOG(INFO, getDomain(), logTr("Query materialization revealed unsatisfiability."), this);
					finishQueryProcessingAsUnsatisfaible(queryProcessingData);

				} else {

					// sort concepts with classified class hierarchy and use only minimized/necessary concepts
					CTaxonomy* classHierarchy = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy();

					QList<CIndividual*> indiList = matData->getVariableIndividualList();
					QHash<CExpressionVariable*, CConcept*>* variableMaterializedConceptsHash = matData->getVariableMaterializedConceptsHash();
					QHash<CExpressionVariable*, CBuildExpression*>* variableClassTermExpHash = buildingVarItem->getVariableClassTermExpressionHash();

					QHash<CExpressionVariable*, CBuildExpression*> newVariableClassTermExpHash;
					for (CIndividual* indi : indiList) {
						CExpressionVariable* expVar = matData->getVariable(indi);
						QList<CConcept*> conceptList = variableMaterializedConceptsHash->values(expVar);

						QList< QPair<CConcept*, CClassTermExpression*> > conceptClassTermExpList;
						for (CConcept* concept : conceptList) {
							CClassTermExpression* classExp = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getConceptClassTermMappingHash()->value(concept);
							conceptClassTermExpList.append(QPair<CConcept*, CClassTermExpression*>(concept, classExp));
						}

						for (CBuildExpression* buildExp : variableClassTermExpHash->values(expVar)) {
							CClassTermExpression* classExp = (CClassTermExpression*)buildExp;
							CConcept* concept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value(classExp);
							conceptClassTermExpList.append(QPair<CConcept*, CClassTermExpression*>(concept, classExp));
						}

						CHierarchyNode* topHierNode = classHierarchy->getTopHierarchyNode();

						QList<TDepthHierarchyNodeConceptClassTermExpressionTuple> depthConceptHierTupleList;
						for (QPair<CConcept*, CClassTermExpression*> conceptClassTermExpPair : conceptClassTermExpList) {
							CConcept* concept = conceptClassTermExpPair.first;
							CClassTermExpression* classTermExp = conceptClassTermExpPair.second;

							CHierarchyNode* hierNode = classHierarchy->getHierarchyNode(concept, false);
							if (hierNode != topHierNode) {
								if (hierNode) {
									depthConceptHierTupleList.append(TDepthHierarchyNodeConceptClassTermExpressionTuple(QPair<cint64, CHierarchyNode*>(hierNode->getPredecessorNodeCount(), hierNode), QPair<CConcept*, CClassTermExpression*>(concept, classTermExp)));
								} else {
									depthConceptHierTupleList.append(TDepthHierarchyNodeConceptClassTermExpressionTuple(QPair<cint64, CHierarchyNode*>(-1, nullptr), QPair<CConcept*, CClassTermExpression*>(concept, classTermExp)));
								}
							}

						}
						qSort(depthConceptHierTupleList.begin(), depthConceptHierTupleList.end(), tuplesSortMoreHierachyDepthThan);


						QList< CClassTermExpression* > newClassTermExpList;
						QSet<CHierarchyNode*> includedPredecessorNodeSet;
						for (const TDepthHierarchyNodeConceptClassTermExpressionTuple& tuple : depthConceptHierTupleList) {
							CHierarchyNode* hierNode = tuple.first.second;
							bool include = false;
							if (hierNode) {
								if (!includedPredecessorNodeSet.contains(hierNode)) {
									include = true;
								}
							} else {
								include = true;
							}

							if (include) {
								newClassTermExpList.append(tuple.second.second);
								if (hierNode) {
									includedPredecessorNodeSet.insert(hierNode);
									for (CHierarchyNode* predHierNode : *hierNode->getPredecessorNodeSet()) {
										includedPredecessorNodeSet.insert(predHierNode);
									}
								}
							}
						}

						for (CClassTermExpression* classTermExp : newClassTermExpList) {
							if (classTermExp) {
								newVariableClassTermExpHash.insertMulti(expVar, classTermExp);
							}
						}

					}
					buildingVarItem->setVariableClassTermExpressionHash(newVariableClassTermExpHash);


					CComplexQueryFinishingBuildingVariableCompositionsItemData* buildFinishingData = buildingVarItem->getBuildingFinishingData();
					if (buildFinishingData) {

						mTestingOntologyBuilder->initializeBuilding();

						finishConceptAndAbsorptionItemsGeneration(buildFinishingData->varExpSet, buildFinishingData->rolledVarExpSet, buildFinishingData->anonymousIndiVariableSet, *buildingVarItem->getVariableClassTermExpressionHash(), buildFinishingData->varExpAxiomExpHash, buildFinishingData->allAnonymousVariables, buildFinishingData->initialAnonymousIndiVariableSet,
							buildFinishingData->bothVarDataPropAssPropagationList, buildFinishingData->bothVarPropAssPropagationList, buildFinishingData->onlyDataVarDataPropAssList, buildingVarItem, answererContext);

					}
				}



				return true;
			}



			bool COptimizedComplexExpressionAnsweringHandler::processExtractedMaterializationConcept(CAnsweringMessageDataCalculationMaterializedConcepts* matConceptsMessage, CAnswererContext* answererContext) {
				CComplexQueryMaterializationData* matData = (CComplexQueryMaterializationData*)matConceptsMessage->getMaterializationDataIdentifier();
				CIndividual* varIndi = matConceptsMessage->getVariableIndividual();

				QHash<CExpressionVariable*, CConcept*>* variableMaterializedConceptsHash = matData->getVariableMaterializedConceptsHash();
				CExpressionVariable* var = matData->getVariable(varIndi);

				CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>* subsumerList = matConceptsMessage->getClassSubsumerList();
				if (subsumerList) {
					for (CConcept* subsumerCon : *subsumerList) {
						variableMaterializedConceptsHash->insertMulti(var, subsumerCon);
					}
				}

				return true;
			}




			bool COptimizedComplexExpressionAnsweringHandler::canSplitActivationIndividualPropagation(COptimizedComplexConceptItem* conceptItem, CConceptOfInterestActivationTriggeringData activationTriggeringData, CAnswererContext* answererContext) {
				CConceptOfInterestActivationLinker* activationLinker = activationTriggeringData.getActivationLinker();
				if (activationLinker && !activationLinker->hasNext() && activationLinker->getActivationIndividual()) {
					CConcept* actCon = activationLinker->getActivationConcept();

					CConceptOperator* conceptOperator = actCon->getConceptOperator();
					if (conceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ALL_AQALL_TYPE)) {
						CRole* role = actCon->getRole();
						if (!role->isComplexRole()) {
							return true;
						}
					}

				}
				return false;
			}





			double COptimizedComplexExpressionAnsweringHandler::getNextRoleActivationPropagationMultiplicationEstimation(double currentExpectedCount, const QSet<CRealizationIndividualInstanceItemReference>& currentSample, CConcept* currentPropConcept, QSet<CRealizationIndividualInstanceItemReference>& nextSample, CConcept*& nextPropagationConcept, CAnswererContext* answererContext) {
				CRoleRealization* roleRealization = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();

				nextPropagationConcept = nullptr;

				QList<TConceptNegPair> testingPropConList;
				testingPropConList.append(TConceptNegPair(currentPropConcept, false));
				while (!testingPropConList.isEmpty()) {
					TConceptNegPair conNegPair = testingPropConList.takeFirst();
					CConcept* concept = conNegPair.first;
					bool conNeg = conNegPair.second;

					CConceptOperator* conceptOperator = concept->getConceptOperator();
					if (!conNeg && conceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ALL_AQALL_TYPE)) {
						if (concept->getRole() && !concept->getRole()->isComplexRole()) {
							nextPropagationConcept = concept;
							break;
						}
					}

					if (!conNeg && conceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_TRIG_AND_AQAND_TYPE)) {
						for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
							testingPropConList.append(TConceptNegPair(opConLinkerIt->getData(), opConLinkerIt->isNegated()));
						}
					} else if (!conNeg && conceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_IMPL_TYPE) && concept->getOperandList()) {
						testingPropConList.prepend(TConceptNegPair(concept->getOperandList()->getData(), concept->getOperandList()->isNegated()));
					}

				}


				CRole* role = nullptr;
				if (nextPropagationConcept) {
					role = nextPropagationConcept->getRole();

					CRoleInstantiatedItem* roleInstancesItem = roleRealization->getRoleInstantiatedItem(role);


					cint64 requiredSamplingCount = 15;
					if (requiredSamplingCount > currentSample.size()) {
						requiredSamplingCount = currentSample.size();
					}
					cint64 currentSampleCount = 0;
					if (currentSampleCount < requiredSamplingCount) {

						cint64 averageCount = 0;
						double averageEstimatedCount = 0;
						cint64 nextSampleAddingCount = 3;

						auto averageBindingsEstimate = [&](const CRealizationIndividualInstanceItemReference& propInstItemRef) {
							CRealizationIndividualInstanceItemReferenceIterator* iterator = roleRealization->getRoleFillerInstancesIterator(propInstItemRef, roleInstancesItem, false, mRoleFillerRealizationSorting);
							iterator->begin();

							double estimatedCount = iterator->remainingInstancesEstimation().getEstimatedCount();
							while (!iterator->atEnd() && nextSampleAddingCount-- > 0) {
								nextSample.insert(iterator->currentIndividualInstanceItemReference());
								iterator->moveNext();
							}

							averageEstimatedCount += estimatedCount;
							averageCount++;
							delete iterator;
						};

						QSet<CRealizationIndividualInstanceItemReference>::const_iterator sampleIt = currentSample.constBegin();
						while (currentSampleCount++ < requiredSamplingCount && sampleIt != currentSample.constEnd()) {
							CRealizationIndividualInstanceItemReference sampleInstItemRef = *sampleIt;
							averageBindingsEstimate(sampleInstItemRef);
						}
						if (averageCount != 0) {
							averageEstimatedCount = averageEstimatedCount / averageCount * currentExpectedCount;
						}

						return averageEstimatedCount;
					}
				}
				return 0;
			}







			CConcept* COptimizedComplexExpressionAnsweringHandler::getSplitActivationIndividualPropagationConcept(COptimizedComplexConceptItem* conceptItem, CConceptOfInterestActivationTriggeringData activationTriggeringData, CVariable*& propVar, double& splitCompSize, CAnswererContext* answererContext) {
				CConceptOfInterestActivationLinker* activationLinker = activationTriggeringData.getActivationLinker();
				CConcept* actCon = activationLinker->getActivationConcept();

				CConcept* splitPropConcept = actCon;

				bool splitEstimationReached = false;
				double splitEstimationSize = mConfAbsorbedIndividualActivationInstanceCandidatePropagationDirectSplittingSize;
				double firstEstimationSize = -1;


				double maxEstimationSize = 0;
				CConcept* maxPropConcept = actCon;


				CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();
				QSet<CRealizationIndividualInstanceItemReference> currentIndiSampleSet;
				double currentEstimationCount = 1;
				CConcept* currentPropConcept = actCon;
				currentIndiSampleSet.insert(sameRealization->getSameInstanceItemReference(activationTriggeringData.getActivationLinker()->getActivationIndividual()));


				while (currentEstimationCount < splitEstimationSize && currentPropConcept) {

					QSet<CRealizationIndividualInstanceItemReference> nextIndiSampleSet;
					CConcept* nextPropConcept = nullptr;
					double nextPropEstimationCount = getNextRoleActivationPropagationMultiplicationEstimation(currentEstimationCount, currentIndiSampleSet, currentPropConcept, nextIndiSampleSet, nextPropConcept, answererContext);

					if (nextPropEstimationCount > maxEstimationSize) {
						maxPropConcept = nextPropConcept;
					}
					if (firstEstimationSize < 0) {
						firstEstimationSize = nextPropEstimationCount;
					}

					if (nextPropConcept && nextPropEstimationCount >= splitEstimationSize) {
						currentEstimationCount = nextPropEstimationCount;
						splitPropConcept = nextPropConcept;
						splitEstimationReached = true;
						break;
					} else if (nextPropConcept) {
						currentPropConcept = nextPropConcept->getOperandList()->getData();
						currentEstimationCount = nextPropEstimationCount;
						currentIndiSampleSet = nextIndiSampleSet;
					} else {
						break;
					}
				}

				if (!splitEstimationReached && maxPropConcept && maxEstimationSize > mConfAbsorbedIndividualActivationInstanceCandidatePropagationMinimumSplittingSize) {
					splitEstimationReached = maxPropConcept;
					currentEstimationCount = maxEstimationSize;
				}

				if (splitEstimationReached) {
					splitCompSize = currentEstimationCount;
					propVar = splitPropConcept->getVariable();
					if (!propVar) {
						CMemoryAllocationManager* memMan = mOntoAnsweringItem->getTestingOntology(false)->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();
						propVar = CObjectAllocator< CVariable >::allocateAndConstruct(memMan);
						CSortedLinker<CVariable*>* propVarLinker = CObjectAllocator< CSortedLinker<CVariable*> >::allocateAndConstruct(memMan);
						propVarLinker->setData(propVar);
						propVar->initVariable(nullptr, 0);
						splitPropConcept->setVariableLinker(propVarLinker);
					}
					return actCon;
				}



				QHash<CConcept*, CConcept*>* activationPropagationSplitConceptHash = mOntoAnsweringItem->getActivationPropagationSplitConceptHash();
				CConcept*& concept = (*activationPropagationSplitConceptHash)[actCon];

				splitCompSize = firstEstimationSize;

				if (!concept) {
					CMemoryAllocationManager* memMan = mOntoAnsweringItem->getTestingOntology(false)->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();
					CTBox* tBox = mOntoAnsweringItem->getTestingOntology(false)->getDataBoxes()->getTBox();
					CConceptVector* conceptVec = tBox->getConceptVector();

					CConcept* propConcept = CObjectAllocator< CConcept >::allocateAndConstruct(memMan);
					cint64 nextConTag = tBox->getNextConceptID();
					propConcept->initConceptCopy(actCon);
					propConcept->setOperandList(actCon->getOperandList());
					propConcept->setTag(nextConTag);
					conceptVec->setLocalData(nextConTag, propConcept);

					propVar = CObjectAllocator< CVariable >::allocateAndConstruct(memMan);
					CSortedLinker<CVariable*>* propVarLinker = CObjectAllocator< CSortedLinker<CVariable*> >::allocateAndConstruct(memMan);
					propVarLinker->setData(propVar);
					propVar->initVariable(nullptr, 0);
					propConcept->setVariableLinker(propVarLinker);
					concept = propConcept;
				}
				if (concept) {
					propVar = concept->getVariable();
				}

				return concept;
			}


			bool COptimizedComplexExpressionAnsweringHandler::createCandidatePropagationInstanceTest(COptimizedComplexConceptItem* conceptItem, CConceptOfInterestActivationTriggeringData activationTriggeringData, CAnswererContext* answererContext) {
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());
				QSet<CRealizationIndividualInstanceItemReference>* possibleInstanceItemSet = conceptItem->getPossibleInstanceItemSet();
				QSet<CRealizationIndividualInstanceItemReference> testingSet;
				CComplexConceptStepInstanceComputationProcess* compStep = conceptItem->getComputationProcess()->getInstancesComputationProcess(true);

				if (mConfAbsorbedIndividualActivationInstanceCandidatePropagationSplitting && canSplitActivationIndividualPropagation(conceptItem, activationTriggeringData, answererContext)) {
					CVariable* propVar = nullptr;
					double splitCompSize = 0;
					CConcept* splitableIndActCon = getSplitActivationIndividualPropagationConcept(conceptItem, activationTriggeringData, propVar, splitCompSize, answererContext);
					testingSet = *possibleInstanceItemSet;

					cint64 moduloCount = 1;
					if (propVar && splitCompSize > mConfAbsorbedIndividualActivationInstanceCandidatePropagationMinimumSplittingSize) {
						double splitFac = 40 * log(splitCompSize) / log(10);
						moduloCount = qMin((cint64)(splitCompSize / splitFac), mConfAbsorbedIndividualActivationInstanceCandidatePropagationMaximumSplittingCount);
					}
					for (cint64 indiModulo = 0; indiModulo < moduloCount; ++indiModulo) {
						satCalcJob = nullptr;

						CAnsweringPropagationSteeringAbsorptionPropagationController* propCon = nullptr;
						if (moduloCount > 1) {
							propCon = new CAnsweringPropagationSteeringAbsorptionPropagationController(propVar, moduloCount, indiModulo);
						}
						satCalcJob = extendProcessingByTopPropagation(satCalcJobGen, satCalcJob, conceptItem->isTopObjectPropertyUsed(), answererContext);
						satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(splitableIndActCon, false, activationTriggeringData.getActivationLinker()->getActivationIndividual(), satCalcJob);

						CAnsweringMessageDataCalculationCompletedInstanceCandidatePropagationItem* completedMessage = new CAnsweringMessageDataCalculationCompletedInstanceCandidatePropagationItem(satCalcJob, conceptItem, testingSet);
						satCalcJob->setSatisfiableAnswererInstancePropagationMessageAdapter(new CSatisfiableTaskAnswererInstancePropagationMessageAdapter(conceptItem->getConcept(), conceptItem->getConceptNegation(), activationTriggeringData.getTriggeringConcept() == nullptr, this, mOntoAnsweringItem->getTestingOntology(false), answererContext->getAnsweringCalculationHandler(), propCon));

						mOntoAnsweringItem->getAnsweringHandlingStatistics()->incComplexConceptItemsCandidatePropagationCount();

						continueCalculationJobFromNondeterministicCachedGraph(satCalcJob, answererContext);
						processCalculationJob(answererContext, satCalcJob, completedMessage);

						if (indiModulo != 0) {
							compStep->setComputationProcessProcessing(true);
							compStep->incCurrentlyRunningComputationCount();
							conceptItem->incCandidatePropagationProcessingCount();
						}
					}
					conceptItem->setPossibleInstanceItemSetCandidatePropagationIterator(possibleInstanceItemSet->constEnd());
					conceptItem->setPossibleInstanceItemSetCandidatePropagationIteratorInitialized(true);
					conceptItem->setPossibleInstanceCandidatePropagationItemSet(possibleInstanceItemSet);
					conceptItem->setPossibleInstanceItemSet(new QSet<CRealizationIndividualInstanceItemReference>());
					conceptItem->setPossibleInstanceCandidatePropagationBeginningKnownInstancesCount(conceptItem->getKnownInstanceItems()->size());


				} else {

					satCalcJob = extendProcessingByTopPropagation(satCalcJobGen, satCalcJob, conceptItem->isTopObjectPropertyUsed(), answererContext);
					bool allOnceProp = false;
					for (CConceptOfInterestActivationLinker* activationLinkerIt = activationTriggeringData.getActivationLinker(); activationLinkerIt; activationLinkerIt = activationLinkerIt->getNext()) {
						if (activationLinkerIt->getActivationIndividual()) {
							satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(activationLinkerIt->getActivationConcept(), false, activationLinkerIt->getActivationIndividual(), satCalcJob);
							allOnceProp = true;
							testingSet = *possibleInstanceItemSet;
							conceptItem->setPossibleInstanceItemSetCandidatePropagationIteratorInitialized(true);
							conceptItem->setPossibleInstanceCandidatePropagationItemSet(possibleInstanceItemSet);
							conceptItem->setPossibleInstanceItemSetCandidatePropagationIterator(possibleInstanceItemSet->constBegin());
							conceptItem->setPossibleInstanceItemSet(new QSet<CRealizationIndividualInstanceItemReference>());
							conceptItem->setPossibleInstanceCandidatePropagationBeginningKnownInstancesCount(conceptItem->getKnownInstanceItems()->size());
						}
					}
					for (CConceptOfInterestActivationLinker* activationLinkerIt = activationTriggeringData.getActivationLinker(); activationLinkerIt; activationLinkerIt = activationLinkerIt->getNext()) {
						if (!activationLinkerIt->getActivationIndividual()) {

							if (!conceptItem->hasPossibleInstanceItemSetCandidatePropagationIteratorInitialized()) {
								conceptItem->setPossibleInstanceItemSetCandidatePropagationIteratorInitialized(true);
								conceptItem->setPossibleInstanceCandidatePropagationItemSet(possibleInstanceItemSet);
								conceptItem->setPossibleInstanceItemSet(new QSet<CRealizationIndividualInstanceItemReference>());
								conceptItem->setPossibleInstanceItemSetCandidatePropagationIterator(possibleInstanceItemSet->constBegin());
								conceptItem->setPossibleInstanceCandidatePropagationBeginningKnownInstancesCount(conceptItem->getKnownInstanceItems()->size());
							}

							cint64 count = 0;
							QSet<CRealizationIndividualInstanceItemReference>::const_iterator candIt = conceptItem->getPossibleInstanceItemSetCandidatePropagationIterator();
							for (QSet<CRealizationIndividualInstanceItemReference>::const_iterator candItEnd = conceptItem->getPossibleInstanceCandidatePropagationItemSet()->constEnd(); candIt != candItEnd && (count++ <= mConfMaxCandidateInstancePropagationsInitializationIndividualCount || allOnceProp); ++candIt) {
								CRealizationIndividualInstanceItemReference possibleInstanceItem(*candIt);
								testingSet.insert(possibleInstanceItem);

								satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(activationLinkerIt->getActivationConcept(), false, possibleInstanceItem.getIndividualIdentifier(), satCalcJob);
							}
							conceptItem->setPossibleInstanceItemSetCandidatePropagationIterator(candIt);
						}
					}
					if (allOnceProp) {
						conceptItem->setPossibleInstanceItemSetCandidatePropagationIterator(possibleInstanceItemSet->constEnd());
					}


					CAnsweringMessageDataCalculationCompletedInstanceCandidatePropagationItem* completedMessage = new CAnsweringMessageDataCalculationCompletedInstanceCandidatePropagationItem(satCalcJob, conceptItem, testingSet);
					satCalcJob->setSatisfiableAnswererInstancePropagationMessageAdapter(new CSatisfiableTaskAnswererInstancePropagationMessageAdapter(conceptItem->getConcept(), conceptItem->getConceptNegation(), activationTriggeringData.getTriggeringConcept() == nullptr, this, mOntoAnsweringItem->getTestingOntology(false), answererContext->getAnsweringCalculationHandler()));

					mOntoAnsweringItem->getAnsweringHandlingStatistics()->incComplexConceptItemsCandidatePropagationCount();

					continueCalculationJobFromNondeterministicCachedGraph(satCalcJob, answererContext);
					processCalculationJob(answererContext, satCalcJob, completedMessage);

				}
				return true;
			}




			bool COptimizedComplexExpressionAnsweringHandler::createInvidiualItemInstanceTest(COptimizedComplexConceptItem* conceptItem, const CRealizationIndividualInstanceItemReference& testingItemRef, CAnswererContext* answererContext) {
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());

				//if (visitor.testingIndi) {
				//	if (CIRIName::getRecentIRIName(visitor.testingIndi->getIndividualNameLinker()) == "http://www.Department1.University0.edu/UndergraduateStudent79") {
				//		bool debug = true;
				//	}
				//}
				//if (visitor.testingIndi) {
				//	for (auto it = visitor.testingIndi->getAssertionRoleLinker(); it; it = it->getNext()) {
				//		CIndividual* othIndi = it->getIndividual();
				//		if (CIRIName::getRecentIRIName(othIndi->getIndividualNameLinker()) == "http://www.Department0.University0.edu/GraduateCourse0" && CIRIName::getRecentIRIName(it->getRole()->getPropertyNameLinker()) == "http://swat.cse.lehigh.edu/onto/University1-fully.owl#takesCourse") {
				//			bool debug = true;
				//		}
				//	}
				//}

				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incComplexConceptItemsPossibleInstanceVerificationCount();
				
				CConcept* reducedConcept = conceptItem->getCompletelyHandledReducedInstanceTestingConcept();
				bool reducedNegation = conceptItem->getCompletelyHandledReducedInstanceTestingConceptNegation();
				satCalcJob = extendProcessingByTopPropagation(satCalcJobGen, satCalcJob, conceptItem->isTopObjectPropertyUsed(), answererContext);
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(reducedConcept, !reducedNegation, testingItemRef, satCalcJob);

				CAnsweringMessageDataCalculationCompletedInstanceItem* completedMessage = new CAnsweringMessageDataCalculationCompletedInstanceItem(satCalcJob, conceptItem, testingItemRef);
				processCalculationJob(answererContext, satCalcJob, completedMessage);
				return true;
			}


			bool COptimizedComplexExpressionAnsweringHandler::createSatisfiabilityTest(COptimizedComplexConceptItem* conceptItem, CAnswererContext* answererContext) {
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CSatisfiableCalculationJobGenerator satCalcJobGen(mOntoAnsweringItem->getTestingOntology());
				satCalcJob = extendProcessingByTopPropagation(satCalcJobGen, satCalcJob, conceptItem->isTopObjectPropertyUsed(), answererContext);
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(conceptItem->getConcept(), conceptItem->getConceptNegation(), nullptr, satCalcJob);
				CAnsweringMessageDataCalculationCompletedSatisfiable* completedMessage = new CAnsweringMessageDataCalculationCompletedSatisfiable(satCalcJob, conceptItem);
				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incComplexConceptItemsSatisfiabilityTestingCount();
				satCalcJob->setSatisfiableAnswererSubsumptionMessageAdapter(new CSatisfiableTaskAnswererSubsumptionMessageAdapter(conceptItem->getConcept(), conceptItem->getConceptNegation(), this, mOntoAnsweringItem->getTestingOntology(false), answererContext->getAnsweringCalculationHandler()));
				processCalculationJob(answererContext, satCalcJob, completedMessage);
				return true;
			}


			CSatisfiableCalculationJob* COptimizedComplexExpressionAnsweringHandler::extendProcessingByTopPropagation(CSatisfiableCalculationJobGenerator& satCalcJobGen, CSatisfiableCalculationJob* satCalcJob, bool hasTopObjectPropertyPropagation, CAnswererContext* answererContext) {
				if (hasTopObjectPropertyPropagation) {
					CConcept* topRolePropagationExistingIndividualCreationConcept = mOntoAnsweringItem->getTopRolePropagationExistingIndividualCreationConcept();
					if (!topRolePropagationExistingIndividualCreationConcept) {
						CMemoryAllocationManager* memMan = mOntoAnsweringItem->getTestingOntology(false)->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();
						CTBox* tBox = mOntoAnsweringItem->getTestingOntology(false)->getDataBoxes()->getTBox();
						CConceptVector* conceptVec = tBox->getConceptVector();

						topRolePropagationExistingIndividualCreationConcept = CObjectAllocator<CConcept>::allocateAndConstruct(memMan);

						CIndividualVector* indiVec = mOntoAnsweringItem->getTestingOntology(false)->getABox()->getIndividualVector();
						cint64 univConnIndiID = mOntoAnsweringItem->getTestingOntology(false)->getABox()->getNextIndividualId(true);
						CIndividual* universalConnIndi = CObjectAllocator< CIndividual >::allocateAndConstruct(memMan);
						universalConnIndi->initIndividual(univConnIndiID);
						indiVec->setData(univConnIndiID, universalConnIndi);

						topRolePropagationExistingIndividualCreationConcept->initConcept();
						cint64 nextConTag = tBox->getNextConceptID();
						topRolePropagationExistingIndividualCreationConcept->initTag(nextConTag);
						topRolePropagationExistingIndividualCreationConcept->setOperatorCode(CCVALUE);
						topRolePropagationExistingIndividualCreationConcept->setRole(mOntoAnsweringItem->getTestingOntology(false)->getRBox()->getTopObjectRole());
						topRolePropagationExistingIndividualCreationConcept->incOperandCount();
						topRolePropagationExistingIndividualCreationConcept->setNominalIndividual(universalConnIndi);
						conceptVec->setLocalData(nextConTag, topRolePropagationExistingIndividualCreationConcept);

						mOntoAnsweringItem->setTopRolePropagationExistingIndividual(universalConnIndi);
						mOntoAnsweringItem->setTopRolePropagationExistingIndividualCreationConcept(topRolePropagationExistingIndividualCreationConcept);
					}



					visitAllIndividuals([&](const CIndividualReference& indiRef)->bool {
						satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(topRolePropagationExistingIndividualCreationConcept, false, indiRef, satCalcJob);
						return true;
					});
					
					satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(topRolePropagationExistingIndividualCreationConcept, false, mOntoAnsweringItem->getTopRolePropagationExistingIndividual(), satCalcJob);

				}
				return satCalcJob;
			}




			bool COptimizedComplexExpressionAnsweringHandler::processRequirements(CAnswererContext* answererContext, CConcreteOntology* ontology, const QList<COntologyProcessingRequirement*>& reqList, CAnsweringMessageDataRequirementCompleted* completedMessage) {
				if (!mOntoAnsweringItem->hasRequirementProcessingSchedulingReported()) {
					LOG(INFO, getDomain(), logTr("Scheduling processing of (realization) requirements."), this);
					mOntoAnsweringItem->setRequirementProcessingSchedulingReported(true);
				}
				if (mConfExtendedLogging) {
					LOG(INFO, getDomain(), logTr("Scheduling processing of %1 requirements.").arg(reqList.size()), this);
				}
				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incRequestedRealizationRequirementCount(reqList.size());
				answererContext->getAnsweringCalculationHandler()->ensureRequirements(this, ontology, reqList, completedMessage);
				return true;
			}


			bool COptimizedComplexExpressionAnsweringHandler::processCalculationJob(CAnswererContext* answererContext, CSatisfiableCalculationJob* satCalcJob, CAnsweringMessageDataCalculationCompleted* completedMessage) {
				satCalcJob->setCalculationConfiguration(mOntoAnsweringItem->getCalculationConfiguration());

				CAnsweringCalculationStatisticsCollection* statColl = nullptr;
				if (mOntoAnsweringItem->isProcessStatisticsCollectionConfigured()) {
					statColl = mOntoAnsweringItem->getCalculationStatisticsCollection();
					completedMessage->setStatisticsCollection(statColl);
				}
				satCalcJob->setCalclulationStatisticsCollector(statColl);

				answererContext->getAnsweringCalculationHandler()->postCalculation(this, satCalcJob, completedMessage, completedMessage);
				return true;
			}








			bool COptimizedComplexExpressionAnsweringHandler::processCalculationMessage(CAnsweringMessageData* message, CAnswererContext* answererContext) {
				while (message) {
					CAnsweringMessageData::ANSWERINGMESSAGEDATA messageType = message->getAnsweringMessageDataType();
					if (messageType == CAnsweringMessageData::CALCULATIONCOMPLETED) {
						CAnsweringMessageDataCalculationCompleted* calcCompMessage = (CAnsweringMessageDataCalculationCompleted*)message;

						bool errorOcurred = false;
						if (calcCompMessage->hasCalculationError()) {
							LOG(ERROR, getDomain(), logTr("Error in computation, answer computation for ontology '%1' failed.").arg(mOntoAnsweringItem->getOntology()->getTerminologyName()), this);
							errorOcurred = true;
						}

						CAnsweringMessageDataCalculationCompleted::CALCULATIONTYPE calcType = calcCompMessage->getCalculationType();
						if (calcType == CAnsweringMessageDataCalculationCompleted::SATISFIABLECALCULATION) {
							CAnsweringMessageDataCalculationCompletedSatisfiable* satCalcCompMessage = (CAnsweringMessageDataCalculationCompletedSatisfiable*)calcCompMessage;
							processSatisfiableCalculationCompleted(satCalcCompMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationCompleted::SUPERCLASSSUBSUMPTIONCALCULATION) {
							CAnsweringMessageDataCalculationCompletedSubsumptionSuperClass* subCalcCompMessage = (CAnsweringMessageDataCalculationCompletedSubsumptionSuperClass*)calcCompMessage;
							processSuperClassSubsumptionCalculationCompleted(subCalcCompMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationCompleted::SUBCLASSSUBSUMPTIONCALCULATION) {
							CAnsweringMessageDataCalculationCompletedSubsumptionSubClass* subCalcCompMessage = (CAnsweringMessageDataCalculationCompletedSubsumptionSubClass*)calcCompMessage;
							processSubClassSubsumptionCalculationCompleted(subCalcCompMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationCompleted::INDIVIDUALINSTANCECALCULATION) {
							CAnsweringMessageDataCalculationCompletedInstanceIndividual* instCalcCompMessage = (CAnsweringMessageDataCalculationCompletedInstanceIndividual*)calcCompMessage;
							processIndividualInstanceCalculationCompleted(instCalcCompMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationCompleted::INSTANCEENTAILMENTCALCULATION) {
							CAnsweringMessageDataCalculationCompletedInstanceEntailment* instCalcCompMessage = (CAnsweringMessageDataCalculationCompletedInstanceEntailment*)calcCompMessage;
							processInstanceEntailmentCalculationCompleted(instCalcCompMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationCompleted::CLASSSUBSUMPTIONCALCULATION) {
							CAnsweringMessageDataCalculationCompletedClassSubsumption* subsumCalcCompMessage = (CAnsweringMessageDataCalculationCompletedClassSubsumption*)calcCompMessage;
							processClassSumsumptionEntailmentCalculationCompleted(subsumCalcCompMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationCompleted::ITEMINSTANCECALCULATION) {
							CAnsweringMessageDataCalculationCompletedInstanceItem* instCalcCompMessage = (CAnsweringMessageDataCalculationCompletedInstanceItem*)calcCompMessage;
							processIndividualItemCalculationCompleted(instCalcCompMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationCompleted::VARIABLEBINDINGPPROPAGATIONCALCULATION) {
							CAnsweringMessageDataCalculationCompletedVariableBindingPropagations* varBindPropCompMessage = (CAnsweringMessageDataCalculationCompletedVariableBindingPropagations*)calcCompMessage;
							processVariableBindingsPropagationItemCalculationCompleted(varBindPropCompMessage, errorOcurred, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationCompleted::VARIABLEBINDINGCONFIRMATIONCALCULATION) {
							CAnsweringMessageDataCalculationCompletedVariableBindingConfirmation* varBindConfCompMessage = (CAnsweringMessageDataCalculationCompletedVariableBindingConfirmation*)calcCompMessage;
							processVariableBindingsConfirmationCalculationCompleted(varBindConfCompMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationCompleted::VARIABLEBINDINGENTAILMENTCALCULATION) {
							CAnsweringMessageDataCalculationCompletedVariableBindingEntailment* varBindEntCompMessage = (CAnsweringMessageDataCalculationCompletedVariableBindingEntailment*)calcCompMessage;
							processVariableBindingsEntailmentCalculationCompleted(varBindEntCompMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationCompleted::ITEMINSTANCECANDIDATEPROPAGATIONCALCULATION) {
							CAnsweringMessageDataCalculationCompletedInstanceCandidatePropagationItem* instCandPropMessage = (CAnsweringMessageDataCalculationCompletedInstanceCandidatePropagationItem*)calcCompMessage;
							processIndividualCandidatePropagationCalculationCompleted(instCandPropMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationCompleted::QUERYMATERIALIZATIONCALCULATION) {
							CAnsweringMessageDataCalculationCompletedQueryMaterialization* matConCompMessage = (CAnsweringMessageDataCalculationCompletedQueryMaterialization*)calcCompMessage;
							processQueryMaterializationCalculationCompleted(matConCompMessage, answererContext);
						}

						CAnsweringCalculationStatisticsCollection* statColl = calcCompMessage->getStatisticsCollection();
						if (statColl) {
							mOntoAnsweringItem->reuseCalculationStatisticsCollection(statColl);
						}

					} else if (messageType == CAnsweringMessageData::CALCULATIONADAPTER) {
						CAnsweringMessageDataCalculationAdapter* calcAdapMessage = (CAnsweringMessageDataCalculationAdapter*)message;
						CAnsweringMessageDataCalculationAdapter::CALCULATIONADAPTERTYPE calcType = calcAdapMessage->getCalculationAdapterType();
						if (calcType == CAnsweringMessageDataCalculationAdapter::CLASSSUBSUMERS) {
							CAnsweringMessageDataCalculationClassSubsumers* classSubsumersMessage = (CAnsweringMessageDataCalculationClassSubsumers*)calcAdapMessage;
							processExtractedClassSubsumers(classSubsumersMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationAdapter::POSSIBLECLASSSUBSUMERS) {
							CAnsweringMessageDataCalculationPossibleClassSubsumers* possibleClassSubsumersMessage = (CAnsweringMessageDataCalculationPossibleClassSubsumers*)calcAdapMessage;
							processExtractedPossibleClassSubsumers(possibleClassSubsumersMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationAdapter::PROPAGATION_BINDINGS) {
							CAnsweringMessageDataVariableBindingPropagations* variableBindingsPropagationMessage = (CAnsweringMessageDataVariableBindingPropagations*)calcAdapMessage;
							processExtractedVariableBindingPropagations(variableBindingsPropagationMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationAdapter::INSTANCE_CERTAIN_PROPAGATIONS) {
							CAnsweringMessageDataInstanceCertainPropagationsData* instanceCandidatePropagationMessage = (CAnsweringMessageDataInstanceCertainPropagationsData*)calcAdapMessage;
							processExtractedInstanceCertainPropagations(instanceCandidatePropagationMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationAdapter::INSTANCE_POSSIBLE_PROPAGATIONS) {
							CAnsweringMessageDataInstancePossiblePropagationsData* instanceCandidatePossiblePropagationMessage = (CAnsweringMessageDataInstancePossiblePropagationsData*)calcAdapMessage;
							processExtractedInstanceCandidatePossiblePropagations(instanceCandidatePossiblePropagationMessage, answererContext);
						} else if (calcType == CAnsweringMessageDataCalculationAdapter::MATERIALIZED_CONCEPTS) {
							CAnsweringMessageDataCalculationMaterializedConcepts* matConceptsMessage = (CAnsweringMessageDataCalculationMaterializedConcepts*)calcAdapMessage;
							processExtractedMaterializationConcept(matConceptsMessage, answererContext);
						}
					}
					message = message->getNext();
				}
				return false;
			}



			bool COptimizedComplexExpressionAnsweringHandler::processRequirementMessage(CAnsweringMessageData* message, CAnswererContext* answererContext) {
				while (message) {
					CAnsweringMessageData::ANSWERINGMESSAGEDATA messageType = message->getAnsweringMessageDataType();
					if (messageType == CAnsweringMessageData::REQUIREMENTCOMPLETED) {
						if (mConfExtendedLogging) {
							LOG(INFO, getDomain(), logTr("Finished processing requirements, continuing query answering."), this);
						}

						CAnsweringMessageDataRequirementCompleted* reqCompMessage = (CAnsweringMessageDataRequirementCompleted*)message;
						CAnsweringMessageDataRequirementCompleted::REQUIREMENTTYPE reqType = reqCompMessage->getRequirementType();
						if (reqType == CAnsweringMessageDataRequirementCompleted::REALIZATIONREQUIREMENT) {
							CAnsweringMessageDataRequirementCompletedRealization* compRealReqMess = (CAnsweringMessageDataRequirementCompletedRealization*)reqCompMessage;
							COptimizedComplexConceptItem* conItem = compRealReqMess->getConceptItem();
							if (conItem) {
								conItem->decLazyRealizationProcessingCount();
								if (conItem->getLazyRealizationProcessingCount() <= 0) {
									CComplexConceptStepComputationProcess* compStep = conItem->getComputationProcess()->getInstancesComputationProcess(false);
									conItem->setQueuedProcessStep(compStep);
									compStep->setComputationProcessQueued(true);
									COptimizedComplexConceptStepAnsweringItem* processingStep = mOntoAnsweringItem->getConceptProcessingStepItem(compStep->getComputationType());
									processingStep->addQueuedConceptItem(conItem);
								}
							}
							COptimizedComplexVariableCompositionItem* variableCompositionItem = compRealReqMess->getVariableCompositionItem();
							COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem = compRealReqMess->getBuildingVariableCompositionItem();
							if (buildingVarItem && variableCompositionItem) {
								buildingVarItem->decVariableCompositionItemWaitingCount();
								buildingVarItem->addComputeVariableMappingItem(variableCompositionItem);
								if (buildingVarItem->getVariableCompositionItemWaitingCount() <= 0) {
									buildingVarItem->setProcessingQueued(true);
									mOntoAnsweringItem->addProcessingVariableBuildingItem(buildingVarItem);
								}
							}
						} else if (reqType == CAnsweringMessageDataRequirementCompleted::SUBCLASSREALIZATIONREQUIREMENT) {
							CAnsweringMessageDataRequirementCompletedSubClassRealization* subClassRealReqMess = (CAnsweringMessageDataRequirementCompletedSubClassRealization*)reqCompMessage;
							COptimizedComplexConceptItem* conItem = subClassRealReqMess->getConceptItem();
							CComplexConceptStepComputationProcess* compStep = conItem->getComputationProcess()->getSubClassRealizationProcess(false);
							finishCalculationStepProcessing(conItem, compStep, answererContext);

						} else if (reqType == CAnsweringMessageDataRequirementCompleted::CONCEPTASSERTIONENTAILMENTREQUIREMENT) {
							CAnsweringMessageDataRequirementCompletedConceptAssertionEntailment* entReqMes = (CAnsweringMessageDataRequirementCompletedConceptAssertionEntailment*)reqCompMessage;
							CEntailmentQueryProcessingData* procData = entReqMes->getEntailmentProcessingData();
							procData->mHasResult = true;
							CConcept* concept = entReqMes->getConcept();
							CIndividual* individual = entReqMes->getIndividual();
							CConceptRealization* conReal = mOntoAnsweringItem->getOntology()->getRealization()->getConceptRealization();
							procData->mEntailed = conReal->isConceptInstance(individual,concept);
							for (CXLinker<CComplexQueryExpressionProcessingData*>* procDataLinkerIt = procData->mProcDataLinker; procDataLinkerIt; procDataLinkerIt = procDataLinkerIt->getNext()) {
								CComplexQueryExpressionProcessingData* queryProcData = procDataLinkerIt->getData();
								queryProcData->decEntailmentComputation();
								if (!queryProcData->hasComputation()) {
									finishQueryProcessing(queryProcData);
								}
							}
							mOntoAnsweringItem->releaseQueryProcessingLinker(procData->mProcDataLinker);
							procData->mProcDataLinker = nullptr;

						} else if (reqType == CAnsweringMessageDataRequirementCompleted::ROLEASSERTIONENTAILMENTREQUIREMENT) {
							CAnsweringMessageDataRequirementCompletedRoleAssertionEntailment* entReqMes = (CAnsweringMessageDataRequirementCompletedRoleAssertionEntailment*)reqCompMessage;
							CEntailmentQueryProcessingData* procData = entReqMes->getEntailmentProcessingData();
							procData->mHasResult = true;
							CRole* role = entReqMes->getRole();
							CIndividual* individualSource = entReqMes->getIndividualSource();
							CIndividual* individualDest = entReqMes->getIndividualDestination();
							CRoleRealization* roleReal = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();
							procData->mEntailed = roleReal->isRoleInstance(role,individualSource,individualDest);
							for (CXLinker<CComplexQueryExpressionProcessingData*>* procDataLinkerIt = procData->mProcDataLinker; procDataLinkerIt; procDataLinkerIt = procDataLinkerIt->getNext()) {
								CComplexQueryExpressionProcessingData* queryProcData = procDataLinkerIt->getData();
								queryProcData->decEntailmentComputation();
								if (!queryProcData->hasComputation()) {
									finishQueryProcessing(queryProcData);
								}
							}
							mOntoAnsweringItem->releaseQueryProcessingLinker(procData->mProcDataLinker);
							procData->mProcDataLinker = nullptr;

						} else if (reqType == CAnsweringMessageDataRequirementCompletedRoleTargetsRealization::ROLETARGETSREALIZATIONREQUIREMENT) {
							CAnsweringMessageDataRequirementCompletedRoleTargetsRealization* roleTarRealMes = (CAnsweringMessageDataRequirementCompletedRoleTargetsRealization*)reqCompMessage;
							CRole* role = roleTarRealMes->getRole();
							bool inversed = roleTarRealMes->getInversed();
							QList<CIndividualReference>* individualSourceList = roleTarRealMes->getIndividualSourceList();

							for (CIndividualReference individualSource : *individualSourceList) {

								CRoleTargetsRealizationHasher enHasher(role, individualSource, inversed);
								CEntailmentVariableBuildingItemData* varBuildData = mRoleTargetsProcDataHash[enHasher];

								for (CXLinker<COptimizedComplexBuildingIndividualVariableCompositionsItem*>* procDataLinkerIt = varBuildData->mProcDataLinker; procDataLinkerIt; procDataLinkerIt = procDataLinkerIt->getNext()) {
									COptimizedComplexBuildingIndividualVariableCompositionsItem* varBuildItem = procDataLinkerIt->getData();
									varBuildItem->decWaitingRoleTargetsRealizationCount();
									if (!varBuildItem->isWaitingRoleTargetsRealization()) {
										varBuildItem->setProcessingQueued(true);
										mOntoAnsweringItem->addProcessingVariableBuildingItem(varBuildItem);
									}
								}
								mOntoAnsweringItem->releaseBuildingVariableItemProcessingLinker(varBuildData->mProcDataLinker);
								varBuildData->mProcDataLinker = nullptr;
							}

						}
					}
					message = message->getNext();
				}
				return false;				
			}






			QSet<CHierarchyNode*>* COptimizedComplexExpressionAnsweringHandler::getDirectReducedSuperNodeSet(const QSet<CHierarchyNode*>& superClassesSet) {
				QSet<CHierarchyNode*>* directSuperClassesSet = new QSet<CHierarchyNode*>();
				QList<CHierarchyNode*> predCountSortedSuperClassNodeList;
				for (QSet<CHierarchyNode*>::const_iterator it = superClassesSet.constBegin(), itEnd = superClassesSet.constEnd(); it != itEnd; ++it) {
					CHierarchyNode* superClassNode(*it);
					if (superClassNode) {
						predCountSortedSuperClassNodeList.append(superClassNode);
					}
				}
				qSort(predCountSortedSuperClassNodeList.begin(), predCountSortedSuperClassNodeList.end(), nodeSortMorePredecessorsThan);
				for (QList<CHierarchyNode*>::const_iterator it = predCountSortedSuperClassNodeList.constBegin(), itEnd = predCountSortedSuperClassNodeList.constEnd(); it != itEnd; ++it) {
					CHierarchyNode* superClassNode(*it);
					bool alreadyContained = false;
					for (QSet<CHierarchyNode*>::const_iterator itDirect = directSuperClassesSet->constBegin(), itDirectEnd = directSuperClassesSet->constEnd(); !alreadyContained && itDirect != itDirectEnd; ++itDirect) {
						CHierarchyNode* directSuperNode(*itDirect);
						if (directSuperNode->hasPredecessorNode(superClassNode)) {
							alreadyContained = true;
						}
					}
					if (!alreadyContained) {
						directSuperClassesSet->insert(superClassNode);
						if (directSuperClassesSet->count() == 1 && superClassNode->getPredecessorNodeCount()+1 == predCountSortedSuperClassNodeList.count()) {
							break;
						}
					}
				}
				return directSuperClassesSet;
			}





			bool COptimizedComplexExpressionAnsweringHandler::processExtractedClassSubsumers(CAnsweringMessageDataCalculationClassSubsumers* message, CAnswererContext* answererContext) {
				CConcept* concept = message->getSubsumedConcept();
				bool negation = message->getSubsumedConceptNegation();
				COptimizedComplexConceptItem* conceptItem = mOntoAnsweringItem->getComplexConceptItem(concept,negation);
				CClassConceptClassification* classClassification = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification();
				CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>* superClassesList = message->getClassSubsumerList();
				if (classClassification) {
					QSet<CHierarchyNode*>* superClassesSet = new QSet<CHierarchyNode*>();
					CTaxonomy* taxonomy = classClassification->getClassConceptTaxonomy();
					for (CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>::const_iterator it = superClassesList->constBegin(), itEnd = superClassesList->constEnd(); it != itEnd; ++it) {
						CConcept* superClass(*it);
						CHierarchyNode* hierNode = taxonomy->getHierarchyNode(superClass, false);
						if (hierNode) {
							superClassesSet->insert(hierNode);
						}
					}
					conceptItem->setKnownSuperClassNodeSet(superClassesSet);
				} else {
					QSet<CConcept*>* knownSuperConceptSet = new QSet<CConcept*>();
					for (CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>::const_iterator it = superClassesList->constBegin(), itEnd = superClassesList->constEnd(); it != itEnd; ++it) {
						CConcept* superClass(*it);
						knownSuperConceptSet->insert(superClass);
					}
					conceptItem->setKnownSuperConceptSet(knownSuperConceptSet);
				}
				return true;
			}




			bool COptimizedComplexExpressionAnsweringHandler::processExtractedPossibleClassSubsumers(CAnsweringMessageDataCalculationPossibleClassSubsumers* message, CAnswererContext* answererContext) {
				CConcept* concept = message->getSubsumedConcept();
				bool negation = message->getSubsumedConceptNegation();
				COptimizedComplexConceptItem* conceptItem = mOntoAnsweringItem->getComplexConceptItem(concept, negation);
				CClassConceptClassification* classClassification = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification();
				if (classClassification) {
					QSet<CHierarchyNode*>* possSuperClassesSet = new QSet<CHierarchyNode*>();
					QSet<CHierarchyNode*>* knownSuperClassesSet = conceptItem->getKnownSuperClassNodeSet();
					QSet<CConcept*>* knowSuperConceptSet = conceptItem->getKnownSuperConceptSet();
					CTaxonomy* taxonomy = classClassification->getClassConceptTaxonomy();
					CCLASSSUBSUMPTIONMESSAGELIST<CAnsweringMessageDataCalculationPossibleClassSubsumersData*>* possSuperClassesList = message->getPossibleClassSubsumerList();
					for (CCLASSSUBSUMPTIONMESSAGELIST<CAnsweringMessageDataCalculationPossibleClassSubsumersData*>::const_iterator it = possSuperClassesList->constBegin(), itEnd = possSuperClassesList->constEnd(); it != itEnd; ++it) {
						CAnsweringMessageDataCalculationPossibleClassSubsumersData* possSuperClassData(*it);
						if (possSuperClassData->isPossibleSubsumerValid()) {
							CConcept* possSuperClass = possSuperClassData->getPossibleSubsumerConcept();
							CHierarchyNode* hierNode = taxonomy->getHierarchyNode(possSuperClass, false);
							if (hierNode) {
								if (knownSuperClassesSet && !knownSuperClassesSet->contains(hierNode) || knowSuperConceptSet && !knowSuperConceptSet->contains(possSuperClass)) {
									possSuperClassesSet->insert(hierNode);
								}
							}
						}
					}
					conceptItem->setPossibleSuperClassNodeSet(possSuperClassesSet);
				} else {
					QSet<CConcept*>* knowSuperConceptSet = conceptItem->getKnownSuperConceptSet();
					QSet<CConcept*>* possibleSuperConceptSet = new QSet<CConcept*>();
					CCLASSSUBSUMPTIONMESSAGELIST<CAnsweringMessageDataCalculationPossibleClassSubsumersData*>* possSuperClassesList = message->getPossibleClassSubsumerList();
					for (CCLASSSUBSUMPTIONMESSAGELIST<CAnsweringMessageDataCalculationPossibleClassSubsumersData*>::const_iterator it = possSuperClassesList->constBegin(), itEnd = possSuperClassesList->constEnd(); it != itEnd; ++it) {
						CAnsweringMessageDataCalculationPossibleClassSubsumersData* possSuperClassData(*it);
						if (possSuperClassData->isPossibleSubsumerValid()) {
							CConcept* possSuperClass = possSuperClassData->getPossibleSubsumerConcept();
							if (knowSuperConceptSet && !knowSuperConceptSet->contains(possSuperClass)) {
								possibleSuperConceptSet->insert(possSuperClass);
							}
						}
					}
					conceptItem->setPossibleSuperConceptSet(possibleSuperConceptSet);
				}
				return true;
			}



			bool COptimizedComplexExpressionAnsweringHandler::processSatisfiableCalculationCompleted(CAnsweringMessageDataCalculationCompletedSatisfiable* message, CAnswererContext* answererContext) {
				COptimizedComplexConceptItem* conceptItem = message->getConceptItem();
				if (conceptItem->isSuperClassComputationInitializing()) {
					conceptItem->setSuperClassComputationInitializing(false);
					CComplexConceptStepComputationProcess* compStep = conceptItem->getComputationProcess()->getSuperClassNodesComputationProcess(true);
					compStep->setComputationProcessProcessing(false);
					compStep->decCurrentlyRunningComputationCount();
					COptimizedComplexConceptStepAnsweringItem* processingStep = mOntoAnsweringItem->getConceptProcessingStepItem(compStep->getComputationType());
					processingStep->addQueuedConceptItem(conceptItem);
				} else {
					bool satisfiable = message->getCalculationCallbackContextData()->isSatisfiable();
					conceptItem->setSatisfiability(satisfiable);
					CComplexConceptStepComputationProcess* compStep = conceptItem->getComputationProcess()->getSatisfiableClassNodesComputationProcess(true);
					compStep->setComputationProcessProcessing(false);
					compStep->decCurrentlyRunningComputationCount();
					compStep->incFinishedComputationCount();
					finishCalculationStepProcessing(conceptItem, compStep, answererContext);
				}
				return true;
			}


			bool COptimizedComplexExpressionAnsweringHandler::processSuperClassSubsumptionCalculationCompleted(CAnsweringMessageDataCalculationCompletedSubsumptionSuperClass* message, CAnswererContext* answererContext) {
				COptimizedComplexConceptItem* conceptItem = message->getConceptItem();
				bool satisfiable = message->getCalculationCallbackContextData()->isSatisfiable();
				CHierarchyNode* superClassNode = message->getSuperClassNode();
				CComplexConceptStepComputationProcess* compStep = conceptItem->getComputationProcess()->getSuperClassNodesComputationProcess(true);
				if (!satisfiable) {
					conceptItem->getKnownSuperClassNodeSet()->insert(superClassNode);
				}
				compStep->decCurrentlyRunningComputationCount();
				compStep->incFinishedComputationCount();
				if (compStep->getCurrentlyRunningComputationCount() == 0) {
					compStep->setComputationProcessProcessing(false);
					if (!conceptItem->getPossibleSuperClassNodeTestingList() || conceptItem->getPossibleSuperClassNodeTestingList()->isEmpty()) {
						delete conceptItem->getPossibleSuperClassNodeTestingList();
						conceptItem->setPossibleSuperClassNodeTestingList(nullptr);
						QSet<CHierarchyNode*>* knownSuperClassSet = conceptItem->getKnownSuperClassNodeSet();
						if (knownSuperClassSet) {
							QSet<CHierarchyNode*>* directSuperClassSet = getDirectReducedSuperNodeSet(*knownSuperClassSet);
							conceptItem->setKnownSuperClassNodeSet(nullptr);
							delete knownSuperClassSet;
							conceptItem->setDirectSuperClassNodeSet(directSuperClassSet);
						}
						finishCalculationStepProcessing(conceptItem, compStep, answererContext);
					}
				}
				return true;
			}


			bool COptimizedComplexExpressionAnsweringHandler::processSubClassSubsumptionCalculationCompleted(CAnsweringMessageDataCalculationCompletedSubsumptionSubClass* message, CAnswererContext* answererContext) {
				COptimizedComplexConceptItem* conceptItem = message->getConceptItem();
				bool satisfiable = message->getCalculationCallbackContextData()->isSatisfiable();
				CHierarchyNode* subClassNode = message->getSubClassNode();
				CComplexConceptStepComputationProcess* compStep = conceptItem->getComputationProcess()->getSubClassNodesComputationProcess(true);
				if (!satisfiable) {
					conceptItem->getDirectSubClassNodeSet()->insert(subClassNode);
				} else {
					for (QSet<CHierarchyNode*>::const_iterator it = subClassNode->getChildNodeSet()->constBegin(), itEnd = subClassNode->getChildNodeSet()->constEnd(); it != itEnd; ++it) {
						CHierarchyNode* childNode(*it);
						if (!conceptItem->getPossibleSubClassTestingNodeSet()->contains(childNode)) {
							conceptItem->getPossibleSubClassTestingNodeSet()->insert(childNode);
							conceptItem->getPossibleSubClassNodeTestingList()->append(childNode);
							if (conceptItem->getPossibleSubClassNodeTestingList()->size() == 1) {
								if (!compStep->isComputationProcessQueued()) {
									compStep->setComputationProcessQueued(true);
									COptimizedComplexConceptStepAnsweringItem* processingStep = mOntoAnsweringItem->getConceptProcessingStepItem(compStep->getComputationType());
									processingStep->addQueuedConceptItem(conceptItem);
								}
							}
						}
					}
				}
				compStep->incFinishedComputationCount();
				compStep->decCurrentlyRunningComputationCount();
				if (compStep->getCurrentlyRunningComputationCount() == 0) {
					compStep->setComputationProcessProcessing(false);
					if (!conceptItem->getPossibleSubClassNodeTestingList() || conceptItem->getPossibleSubClassNodeTestingList()->isEmpty()) {
						finishSubClassCalculationStepProcessing(conceptItem, compStep, answererContext);
					}
				}
				return true;
			}






			bool COptimizedComplexExpressionAnsweringHandler::processIndividualCandidatePropagationCalculationCompleted(CAnsweringMessageDataCalculationCompletedInstanceCandidatePropagationItem* message, CAnswererContext* answererContext) {
				COptimizedComplexConceptItem* conceptItem = message->getConceptItem();
				conceptItem->decCandidatePropagationProcessingCount();
				CComplexConceptStepComputationProcess* compStep = conceptItem->getComputationProcess()->getInstancesComputationProcess(true);


				QSet<CRealizationIndividualInstanceItemReference>* testingSet = message->getTestingSet();

				compStep->decCurrentlyRunningComputationCount();
				compStep->incFinishedComputationCount();
				if (compStep->getCurrentlyRunningComputationCount() == 0) {
					compStep->setComputationProcessProcessing(false);
				}


				QSet<CRealizationIndividualInstanceItemReference>* possibleInstanceCandPropItemSet = conceptItem->getPossibleInstanceCandidatePropagationItemSet();
				QSet<CRealizationIndividualInstanceItemReference>* possibleInstanceItemSet = conceptItem->getPossibleInstanceItemSet();
				if (conceptItem->getCandidatePropagationProcessingCount() <= 0 && conceptItem->isCandidatePropagated()) {
					cint64 testedCount = possibleInstanceCandPropItemSet->size() - possibleInstanceItemSet->size();
					cint64 knownInstancesCount = conceptItem->getKnownInstanceItems()->size();
					cint64 newKnownInstancesCount = knownInstancesCount - conceptItem->getPossibleInstanceCandidatePropagationBeginningKnownInstancesCount();
					conceptItem->incPossibleTestedNonInstanceCount(possibleInstanceCandPropItemSet->size() - possibleInstanceItemSet->size() - newKnownInstancesCount);
					mTestedPossibleInstancesCount += testedCount;
					delete possibleInstanceCandPropItemSet;
					conceptItem->setPossibleInstanceCandidatePropagationItemSet(nullptr);
					if (possibleInstanceItemSet->isEmpty()) {
						delete possibleInstanceItemSet;
						conceptItem->setPossibleInstanceItemSet(nullptr);
					}
					COptimizedComplexConceptStepAnsweringItem* processingStep = mOntoAnsweringItem->getConceptProcessingStepItem(compStep->getComputationType());
					processingStep->addQueuedConceptItem(conceptItem);

				}

				return true;
			}



			bool COptimizedComplexExpressionAnsweringHandler::processIndividualItemCalculationCompleted(CAnsweringMessageDataCalculationCompletedInstanceItem* message, CAnswererContext* answererContext) {
				COptimizedComplexConceptItem* conceptItem = message->getConceptItem();
				bool satisfiable = message->getCalculationCallbackContextData()->isSatisfiable();
				CRealizationIndividualInstanceItemReference testingItemRef = message->getInstanceItemReference();
				CComplexConceptStepComputationProcess* compStep = conceptItem->getComputationProcess()->getInstancesComputationProcess(true);
				if (!satisfiable) {
					conceptItem->getKnownInstanceItems()->addRealizationIndividualInstanceItemReference(testingItemRef);
				} else {
					conceptItem->incPossibleTestedNonInstanceCount(1);
				}
				compStep->decCurrentlyRunningComputationCount();
				compStep->incFinishedComputationCount();
				mTestedPossibleInstancesCount++;
				if (compStep->getCurrentlyRunningComputationCount() == 0) {
					compStep->setComputationProcessProcessing(false);
				}

				COptimizedComplexConceptStepAnsweringItem* processingStep = mOntoAnsweringItem->getConceptProcessingStepItem(compStep->getComputationType());
				processingStep->addQueuedConceptItem(conceptItem);
				return true;
			}




			bool COptimizedComplexExpressionAnsweringHandler::processIndividualInstanceCalculationCompleted(CAnsweringMessageDataCalculationCompletedInstanceIndividual* message, CAnswererContext* answererContext) {
				bool satisfiable = message->getCalculationCallbackContextData()->isSatisfiable();
				CIndividualReference testingIndiRef = message->getIndividualReference();
				COptimizedComplexConceptItem* conceptItem = message->getConceptItem();
				if (conceptItem) {
					CComplexConceptStepInstanceComputationProcess* compStep = conceptItem->getComputationProcess()->getInstancesComputationProcess(true);
					if (!satisfiable) {
						conceptItem->getKnownInstanceIndividualSet()->insert(testingIndiRef);
					} else {
						conceptItem->incPossibleTestedNonInstanceCount();
					}
					compStep->decCurrentlyRunningComputationCount();
					compStep->incFinishedComputationCount();
					if (compStep->getCurrentlyRunningComputationCount() == 0) {
						compStep->setComputationProcessProcessing(false);

						if (!conceptItem->getPossibleInstanceIndividualSet() || conceptItem->getPossibleInstanceIndividualSet()->isEmpty()) {
							updateComputedInstancesCount(conceptItem, compStep, answererContext);

							if (conceptItem->isLazyRealizationInstancesRetrieved()) {
								delete conceptItem->getPossibleInstanceIndividualSet();
								finishCalculationStepProcessing(conceptItem, compStep, answererContext);
							}
						}
					}
				}
				return true;
			}



			bool COptimizedComplexExpressionAnsweringHandler::processInstanceEntailmentCalculationCompleted(CAnsweringMessageDataCalculationCompletedInstanceEntailment* message, CAnswererContext* answererContext) {
				bool satisfiable = message->getCalculationCallbackContextData()->isSatisfiable();
				CIndividualReference testingIndiRef = message->getIndividualReference();
				CConcept* concept = message->getConcept();
				bool negation = message->getNegation();
				CEntailmentQueryProcessingData* entailmentData = message->getEntailmentData();
				entailmentData->mEntailed = !satisfiable;
				entailmentData->mHasResult = true;
				for (CXLinker<CComplexQueryExpressionProcessingData*>* procDataLinkerIt = entailmentData->mProcDataLinker; procDataLinkerIt; procDataLinkerIt = procDataLinkerIt->getNext()) {
					CComplexQueryExpressionProcessingData* queryProcData = procDataLinkerIt->getData();
					queryProcData->decEntailmentComputation();
					if (!queryProcData->hasComputation()) {
						finishQueryProcessing(queryProcData);
					}
				}
				mOntoAnsweringItem->releaseQueryProcessingLinker(entailmentData->mProcDataLinker);
				entailmentData->mProcDataLinker = nullptr;
				return true;
			}




			bool COptimizedComplexExpressionAnsweringHandler::processClassSumsumptionEntailmentCalculationCompleted(CAnsweringMessageDataCalculationCompletedClassSubsumption* message, CAnswererContext* answererContext) {
				bool satisfiable = message->getCalculationCallbackContextData()->isSatisfiable();
				CConcept* subConcept = message->getSubConcept();
				bool subConNegation = message->getSubConceptNegation();
				CEntailmentQueryProcessingData* entailmentData = message->getEntailmentData();
				entailmentData->mEntailed = !satisfiable;
				entailmentData->mHasResult = true;
				for (CXLinker<CComplexQueryExpressionProcessingData*>* procDataLinkerIt = entailmentData->mProcDataLinker; procDataLinkerIt; procDataLinkerIt = procDataLinkerIt->getNext()) {
					CComplexQueryExpressionProcessingData* queryProcData = procDataLinkerIt->getData();
					queryProcData->decEntailmentComputation();
					if (!queryProcData->hasComputation()) {
						finishQueryProcessing(queryProcData);
					}
				}
				mOntoAnsweringItem->releaseQueryProcessingLinker(entailmentData->mProcDataLinker);
				entailmentData->mProcDataLinker = nullptr;
				return true;
			}



			bool COptimizedComplexExpressionAnsweringHandler::notifyWaitingItemData(CComputedItemDataNotificationLinker* notLinker, CComplexConceptStepComputationProcess* compStep, CAnswererContext* answererContext) {
				if (notLinker) {
					for (CComputedItemDataNotificationLinker* notLinkerIt = notLinker; notLinkerIt; notLinkerIt = notLinkerIt->getNext()) {
						COptimizedComplexVariableCompositionItem* varCompItem = notLinkerIt->getVariableCompositionItem();
						COptimizedComplexBuildingVariableCompositionsItem* buildVarItem = notLinkerIt->getBuildingVariableItemProcessingItem();
						if (buildVarItem) {
							if (varCompItem) {
								queueVariableItemComputation(buildVarItem, varCompItem);
							}
							buildVarItem->setComputationStepFinished(compStep);
							if (!buildVarItem->isWaitingComputationStep() && !buildVarItem->isProcessingQueued()) {
								buildVarItem->setProcessingQueued(true);
								mOntoAnsweringItem->addProcessingVariableBuildingItem(buildVarItem);
							}
						}
						CComplexQueryExpressionProcessingData* queryProcData = notLinkerIt->getQueryProcessingData();
						if (queryProcData) {
							queryProcData->setComputationStepFinished(compStep);
							if (!queryProcData->hasComputation()) {
								finishQueryProcessing(queryProcData);
							}
						}
					}
					mOntoAnsweringItem->releaseComputedItemDataNotificationLinker(notLinker);
					return true;
				}
				return false;
			}



			bool COptimizedComplexExpressionAnsweringHandler::finishCalculationStepProcessing(COptimizedComplexConceptItem* conceptItem, CComplexConceptStepComputationProcess* compStep, CAnswererContext* answererContext) {
				compStep->setComputationProcessFinished(true);

				notifyWaitingItemData(compStep->takeNotificationLinker(), compStep, answererContext);

				requeueConceptItemNextProcessing(conceptItem, compStep);
				return true;
			}




			bool COptimizedComplexExpressionAnsweringHandler::updateComputedInstancesCount(COptimizedComplexConceptItem* conceptItem, CComplexConceptStepInstanceComputationProcess* compStep, CAnswererContext* answererContext) {
				compStep->updateComputedInstancesCount(conceptItem->getKnownInstanceItems()->size(), [&](CComputedItemDataNotificationLinker* notLinker) {
					notifyWaitingItemData(notLinker, compStep, answererContext);
				});
				if (conceptItem->isLazyRealizationInstancesRetrieved()) {
					compStep->setExpectedInstancesCount(conceptItem->getKnownInstanceItems()->size());
					compStep->updateAllInstancesComputed([&](CComputedItemDataNotificationLinker* notLinker) {
						notifyWaitingItemData(notLinker, compStep, answererContext);
					});
				} else {
					cint64 currentKnownInstances = conceptItem->getKnownInstanceItems()->size();
					cint64 currentRetrievedInstances = conceptItem->getRealizationRetrievedInstanceCandidateCount();
					cint64 exprectedRetrievedInstances = conceptItem->getRealizationRetrievingExpectedInstanceCandidateCount();
					double correctionFactor = (double)currentKnownInstances / (double)currentRetrievedInstances;
					double exprectedKnownInstances = (double)currentKnownInstances / (double)currentRetrievedInstances * (double)exprectedRetrievedInstances;


					CRealizationIndividualInstanceItemReferenceIterator* realizationIterator = conceptItem->getRealizationIterator();
					COptimizedRepresentativeKPSetIntersectionCombinationIterator* intRealizationIterator = dynamic_cast<COptimizedRepresentativeKPSetIntersectionCombinationIterator*>(conceptItem->getRealizationIterator());
					if (intRealizationIterator && (currentRetrievedInstances < mConfConceptItemExpectedCountSamplingRetrievedPercentageLimit * exprectedRetrievedInstances || conceptItem->getRealizationIteratorSamplingExpectedCount() >= 0)) {
						cint64 currentIndiId = realizationIterator->currentIndividualInstanceItemReference().getIndividualID();
						cint64 nextIndiId = mOntoAnsweringItem->getOntology()->getABox()->getNextIndividualId(false);
						if (currentRetrievedInstances < mConfConceptItemExpectedCountSamplingRetrievedPercentageLimit * exprectedRetrievedInstances && conceptItem->getRealizationIteratorSamplingExpectedCount() < 0) {

							//QStringList sampleStringList;
							//QString sampleString;

							CRealizationIndividualInstanceItemReferenceIterator* samplingIterator = realizationIterator->getCopy();
							cint64 samplingCount = 0;
							cint64 availableSampleCount = 0;
							double availableSampleIndiDiff = 0;
							double nonAvailableSampleIndiDiff = 0;
							cint64 availableSampleRequiredInitializationCount = 0;
							double samplingRange = 0;
							double samplingRangeExpectedCount = 0;
							for (cint64 samplingNr = currentKnownInstances + 1; samplingNr < mConfConceptItemExpectedCountSamplingSize; ++samplingNr) {
								double samplingIndiId = (double)nextIndiId / (double)mConfConceptItemExpectedCountSamplingSize * (double)samplingNr;
								double nextSamplingIndiId = (double)nextIndiId / (double)mConfConceptItemExpectedCountSamplingSize * (double)(samplingNr + 1);
								if (samplingIndiId > currentIndiId) {
									++samplingCount;
									samplingRange += nextSamplingIndiId - samplingIndiId;

									if (!samplingIterator->atEnd()) {
										cint64 sampleIndiId = samplingIterator->currentIndividualInstanceItemReference().getIndividualID();
										if (sampleIndiId < samplingIndiId) {
											samplingIterator->moveTo(CRealizationIndividualInstanceItemReference(samplingIndiId + 1., nullptr), false);
											if (!samplingIterator->atEnd()) {
												sampleIndiId = samplingIterator->currentIndividualInstanceItemReference().getIndividualID();
											} else {
												sampleIndiId = -1;
											}
										}
										if (sampleIndiId >= samplingIndiId && sampleIndiId < nextSamplingIndiId) {
											availableSampleCount++;

											cint64 nextSampleIndiId = -1;
											bool requiresInitialization = samplingIterator->requiresInitialization();
											samplingIterator->moveNext();
											if (!samplingIterator->atEnd()) {
												nextSampleIndiId = samplingIterator->currentIndividualInstanceItemReference().getIndividualID();
											}
											if (nextSampleIndiId == -1 || nextSampleIndiId > nextSamplingIndiId) {
												// only one individual in sampling range
												double newExpectedCount = 1.;
												if (requiresInitialization) {
													availableSampleRequiredInitializationCount++;
													newExpectedCount = 0.5;
												}
												availableSampleIndiDiff += (nextSamplingIndiId - samplingIndiId);
												samplingRangeExpectedCount += newExpectedCount;
												//sampleStringList.append(QString("Sample \t%1 between \t%2 and \t%3 is \t%4 with nothing else, adding \t%6 to expected \t(now %7)").arg(samplingNr).arg(samplingIndiId).arg(nextSamplingIndiId).arg(sampleIndiId).arg(newExpectedCount).arg(samplingRangeExpectedCount));
												//sampleString = sampleStringList.join("\r\n");
											} else {
												double indiSampleDiff = nextSampleIndiId - sampleIndiId;
												double indiStartDiff = qMax(sampleIndiId - samplingIndiId, 1.);
												double indiDiff = indiSampleDiff / 5. * 4 + indiStartDiff / 5;

												availableSampleIndiDiff += indiDiff;
												if (requiresInitialization) {
													availableSampleRequiredInitializationCount++;
													double newExpectedCount = ((nextSamplingIndiId - samplingIndiId) / indiDiff) * 0.5;
													samplingRangeExpectedCount += newExpectedCount;
												} else {
													double newExpectedCount = ((nextSamplingIndiId - samplingIndiId) / indiDiff);
													samplingRangeExpectedCount += newExpectedCount;
													//sampleStringList.append(QString("Sample \t%1 between \t%2 and \t%3 is \t%4 and %8 with diff \t%5, adding \t%6 to expected \t(now %7)").arg(samplingNr).arg(samplingIndiId).arg(nextSamplingIndiId).arg(sampleIndiId).arg(indiDiff).arg(newExpectedCount).arg(samplingRangeExpectedCount).arg(nextSampleIndiId));
													//sampleString = sampleStringList.join("\r\n");
												}

											}


										} else {
											availableSampleIndiDiff += nextSamplingIndiId - samplingIndiId;
											nonAvailableSampleIndiDiff += nextSamplingIndiId - samplingIndiId;
											//sampleStringList.append(QString("Sample \t%1 between \t%2 and \t%3 does not exist, interpreting it as diff \t%4").arg(samplingNr).arg(samplingIndiId).arg(nextSamplingIndiId).arg(nextSamplingIndiId - samplingIndiId));
											//sampleString = sampleStringList.join("\r\n");
										}
									} else {
										availableSampleIndiDiff += nextSamplingIndiId - samplingIndiId;
										nonAvailableSampleIndiDiff += nextSamplingIndiId - samplingIndiId;
										//sampleStringList.append(QString("Sample \t%1 between \t%2 and \t%3 does not exist, interpreting it as diff \t%4").arg(samplingNr).arg(samplingIndiId).arg(nextSamplingIndiId).arg(nextSamplingIndiId - samplingIndiId));
										//sampleString = sampleStringList.join("\r\n");
									}


								}
							}
							delete samplingIterator;

							if (samplingCount > 0) {
								double diffBasedSamplingRangeExpectedCount = nextIndiId / availableSampleIndiDiff * availableSampleCount;
								double samplesExpectedIndiCount = diffBasedSamplingRangeExpectedCount;
								if (availableSampleIndiDiff <= 0) {
									samplesExpectedIndiCount = 0;
								}
								conceptItem->setRealizationIteratorSamplingExpectedCount(samplesExpectedIndiCount);

								if (mConfExtendedLogging) {
									LOG(INFO, getDomain(), logTr("Took %2 samples to correct expected instance count from %3 to %4 for complex concept item %1.").arg(conceptItem->getConceptItemId()).arg(samplingCount).arg(exprectedKnownInstances).arg(samplesExpectedIndiCount), this);
								}

							}
						}


						double sampleWeightFactor = (double)(nextIndiId - currentIndiId) / (double)nextIndiId;
						double iteratorWeightFactor = (double)(currentIndiId) / (double)nextIndiId;
						double samplesExpectedIndiCount = conceptItem->getRealizationIteratorSamplingExpectedCount();
						double samplingCorrectedExpectedCount = exprectedKnownInstances * iteratorWeightFactor + samplesExpectedIndiCount * sampleWeightFactor;
						exprectedKnownInstances = samplingCorrectedExpectedCount * correctionFactor;
					}


					compStep->setExpectedInstancesCount(exprectedKnownInstances);



				}
				return true;
			}

			QSet<COptimizedComplexConceptItem*>* COptimizedComplexExpressionAnsweringHandler::collectComplexConceptItemPredecessors(COptimizedComplexConceptItem* conceptItem, QSet<COptimizedComplexConceptItem*>* predItemSet) {
				if (!predItemSet) {
					predItemSet = new QSet<COptimizedComplexConceptItem *>();
				}
				QList<COptimizedComplexConceptItem*> addingList;
				addingList.append(conceptItem);
				while (!addingList.isEmpty()) {
					COptimizedComplexConceptItem* testingItem = addingList.takeFirst();
					if (!predItemSet->contains(testingItem)) {
						predItemSet->insert(testingItem);
						for (auto superItem : *testingItem->getSuperComplexConceptItemSet()) {
							addingList.append(superItem);
						}
					}
				}
				return predItemSet;
			}



			QSet<COptimizedComplexConceptItem*>* COptimizedComplexExpressionAnsweringHandler::collectComplexConceptItemSucessors(COptimizedComplexConceptItem* conceptItem, QSet<COptimizedComplexConceptItem*>* succItemSet) {
				if (!succItemSet) {
					succItemSet = new QSet<COptimizedComplexConceptItem *>();
				}
				QList<COptimizedComplexConceptItem*> addingList;
				addingList.append(conceptItem);
				while (!addingList.isEmpty()) {
					COptimizedComplexConceptItem* testingItem = addingList.takeFirst();
					if (!succItemSet->contains(testingItem)) {
						succItemSet->insert(testingItem);
						for (auto superItem : *testingItem->getSubComplexConceptItemSet()) {
							addingList.append(superItem);
						}
					}
				}
				return succItemSet;
			}



			bool COptimizedComplexExpressionAnsweringHandler::reorderConceptItemParentAddition(COptimizedComplexConceptItem* conceptItem, COptimizedComplexConceptItem* newParentConceptItem) {
				QSet<COptimizedComplexConceptItem*>* newParentPredItemSet = collectComplexConceptItemPredecessors(newParentConceptItem);
				QSet<COptimizedComplexConceptItem*>* conceptItemSuccItemSet = collectComplexConceptItemSucessors(conceptItem);
				QList<COptimizedComplexConceptItem*> removeParentItemList;

				for (QSet<COptimizedComplexConceptItem*>::iterator it = conceptItem->getSuperComplexConceptItemSet()->begin(); it != conceptItem->getSuperComplexConceptItemSet()->end();) {
					COptimizedComplexConceptItem* parentItem(*it);
					if (newParentPredItemSet->contains(parentItem)) {
						parentItem->getSubComplexConceptItemSet()->remove(conceptItem);
						it = conceptItem->getSuperComplexConceptItemSet()->erase(it);
					} else {
						++it;
					}
				}
				for (QSet<COptimizedComplexConceptItem*>::iterator it = newParentConceptItem->getSubComplexConceptItemSet()->begin(); it != newParentConceptItem->getSubComplexConceptItemSet()->end();) {
					COptimizedComplexConceptItem* childItem(*it);
					if (conceptItemSuccItemSet->contains(childItem)) {
						it = newParentConceptItem->getSubComplexConceptItemSet()->erase(it);
						childItem->getSuperComplexConceptItemSet()->remove(newParentConceptItem);
					} else {
						++it;
					}
				}
				delete conceptItemSuccItemSet;
				delete newParentPredItemSet;
				
				conceptItem->getSuperComplexConceptItemSet()->insert(newParentConceptItem);
				newParentConceptItem->getSubComplexConceptItemSet()->insert(conceptItem);

				return true;
			}


			bool COptimizedComplexExpressionAnsweringHandler::reorderConceptClassItems() {
				QList<COptimizedComplexConceptItem*>* classItemReorderingList = mOntoAnsweringItem->getConceptClassItemReorderingList();
				CClassConceptClassification* classClassification = mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification();
				if (classClassification) {
					CTaxonomy* taxonomy = classClassification->getClassConceptTaxonomy();
					while (!classItemReorderingList->isEmpty()) {
						COptimizedComplexConceptItem* classItem = classItemReorderingList->takeFirst();
						CConcept* classConcept = classItem->getConcept();
						CHierarchyNode* classHierNode = taxonomy->getHierarchyNode(classConcept, false);
						if (classHierNode) {
							QList<CHierarchyNode*> predTestingList;
							for (auto parentHierNode : *classHierNode->getParentNodeSet()) {
								predTestingList.append(parentHierNode);
							}
							while (!predTestingList.isEmpty()) {
								auto parentHierNode = predTestingList.takeFirst();
								bool foundParent = false;
								for (auto parentConcept : *parentHierNode->getEquivalentConceptList()) {
									COptimizedComplexConceptItem* parentClassItem = mOntoAnsweringItem->getComplexConceptItem(parentConcept, false, false);
									if (parentClassItem) {
										foundParent = true;
										if (!classItem->hasSuperComplexConceptItem(parentClassItem)) {
											reorderConceptItemParentAddition(classItem, parentClassItem);
										}
									}
								}
								if (!foundParent) {
									for (auto nextParentHierNode : *parentHierNode->getParentNodeSet()) {
										predTestingList.append(nextParentHierNode);
									}
								}
							}
						}
					}
				}
				return true;
			}



			bool COptimizedComplexExpressionAnsweringHandler::extractAtomicSubsumerExpressions(COptimizedComplexConceptItem* conceptItem) {
				CConcept* concept = conceptItem->getConcept();
				bool conceptNegation = conceptItem->getConceptNegation();
				QList<TConceptNegPair> testingList;
				testingList.append(TConceptNegPair(concept,conceptNegation));
				QSet<TConceptNegPair>* atomicConceptSet = conceptItem->getAtomicConcpetSet();
				while (!testingList.isEmpty()) {
					TConceptNegPair testingConNegPair = testingList.takeFirst();
					CConcept* testingConcept = testingConNegPair.first;
					bool testingNegation = testingConNegPair.second;
					cint64 opCode = testingConcept->getOperatorCode();
					if (!testingNegation && opCode == CCAND || testingNegation && opCode == CCOR) {
						for (CSortedNegLinker<CConcept*>* opLinkerIt = testingConcept->getOperandList(); opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
							CConcept* opConcept = opLinkerIt->getData();
							bool opNegation = opLinkerIt->isNegated() ^ testingNegation;
							testingList.append(TConceptNegPair(opConcept, opNegation));
						}
					} else {
						atomicConceptSet->insert(testingConNegPair);
					}
				}
				return !atomicConceptSet->isEmpty();
			}


			bool COptimizedComplexExpressionAnsweringHandler::isTriviallySubsumedConceptItem(COptimizedComplexConceptItem* subsumedConceptItem, COptimizedComplexConceptItem* subsumerConceptItem) {
				if (subsumedConceptItem->getAtomicConcpetSet()->contains(subsumerConceptItem->getConceptNegationPair())) {
					return true;
				}
				if (subsumedConceptItem->getAtomicConcpetSet()->contains(*subsumerConceptItem->getAtomicConcpetSet())) {
					return true;
				}
				return false;
			}





			bool COptimizedComplexExpressionAnsweringHandler::searchSuperSubSuperClassesResult(COptimizedComplexConceptItem* startConceptItem, CAnswererContext* answererContext) {

				if (!startConceptItem->isSatisfiable()) {
					QSet<CHierarchyNode*> bottomClassNodeSet;
					bottomClassNodeSet.insert(mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy()->getBottomHierarchyNode());
					startConceptItem->setKnownSuperClassNodeSet(new QSet<CHierarchyNode*>(bottomClassNodeSet));
					return true;
				}
				if (startConceptItem->getConcept()->hasClassName()) {
					QSet<CHierarchyNode*> chilsClassNodeSet;
					chilsClassNodeSet.insert(mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy()->getHierarchyNode(startConceptItem->getConcept()));
					startConceptItem->setKnownSuperClassNodeSet(new QSet<CHierarchyNode*>(chilsClassNodeSet));
					return true;
				}


				class CSuperSubsumptionOptimizedComplexConceptItemVisitor : public COptimizedComplexConceptItemVisitor {
				public:
					QSet<CHierarchyNode*>* mFoundKnwonSuperClassNodeSet = nullptr;
					QSet<CHierarchyNode*>* mFoundPossibleSuperClassNodeSet = nullptr;

					cint64 mSuperItemVisitingLimit = 50;
					cint64 mSuperItemResultVisitingLimit = 10;

					cint64 mSubItemVisitingLimit = 50;
					cint64 mSubItemResultVisitingLimit = 10;

					bool visitComplexSuperConceptItem(COptimizedComplexConceptItem* item, bool& continueVisitingSuperItems) {
						CComplexConceptStepComputationProcess* compStep = item->getComputationProcess()->getSuperClassNodesComputationProcess(false);
						if (compStep && compStep->isComputationProcessFinished()) {
							QSet<CHierarchyNode*>* superClassNodeSet = item->getDirectSuperClassNodeSet();
							if (!mFoundKnwonSuperClassNodeSet) {
								mFoundKnwonSuperClassNodeSet = new QSet<CHierarchyNode*>();
							}
							mFoundKnwonSuperClassNodeSet->operator+=(*superClassNodeSet);
							continueVisitingSuperItems = false;
							if (--mSuperItemResultVisitingLimit < 0) {
								return false;
							}
						}
						if (--mSuperItemVisitingLimit < 0) {
							return false;
						}
						return true;
					}

					bool visitComplexSubConceptItem(COptimizedComplexConceptItem* item, bool& continueVisitingSubItems) {
						CComplexConceptStepComputationProcess* compStep = item->getComputationProcess()->getSuperClassNodesComputationProcess(false);
						if (compStep && compStep->isComputationProcessFinished()) {
							QSet<CHierarchyNode*>* superClassNodeSet = item->getDirectSuperClassNodeSet();
							if (!mFoundPossibleSuperClassNodeSet) {
								mFoundPossibleSuperClassNodeSet = new QSet<CHierarchyNode*>();
								if (mFoundKnwonSuperClassNodeSet && !mFoundKnwonSuperClassNodeSet->isEmpty()) {
									for (auto node : *superClassNodeSet) {
										if (!mFoundKnwonSuperClassNodeSet->contains(node)) {
											mFoundPossibleSuperClassNodeSet->insert(node);
										}
									}
								} else {
									*mFoundPossibleSuperClassNodeSet = *superClassNodeSet;
								}
							} else {
								mFoundPossibleSuperClassNodeSet->intersect(*superClassNodeSet);
							}
							if (mFoundPossibleSuperClassNodeSet->isEmpty()) {
								return false;
							}
							continueVisitingSubItems = false;
							if (--mSubItemResultVisitingLimit < 0) {
								return false;
							}
						}
						if (--mSubItemVisitingLimit < 0) {
							return false;
						}
						return true;
					}

				} visitor;
				visitSuperSubConceptItems(startConceptItem, false, true, &visitor);
				if (visitor.mFoundKnwonSuperClassNodeSet) {
					startConceptItem->setKnownSuperClassNodeSet(visitor.mFoundKnwonSuperClassNodeSet);
					visitSuperSubConceptItems(startConceptItem, true, false, &visitor);
					if (visitor.mFoundPossibleSuperClassNodeSet) {
						startConceptItem->setPossibleSuperClassNodeSet(visitor.mFoundPossibleSuperClassNodeSet);
						return true;
					}
				}
				return false;
			}


















			bool COptimizedComplexExpressionAnsweringHandler::searchSuperSubSubClassesResult(COptimizedComplexConceptItem* startConceptItem, CAnswererContext* answererContext) {

				if (!startConceptItem->isSatisfiable()) {
					QSet<CHierarchyNode*> bottomClassNodeSet;
					bottomClassNodeSet.insert(mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy()->getBottomHierarchyNode());
					startConceptItem->setDirectSubClassNodeSet(new QSet<CHierarchyNode*>(bottomClassNodeSet));
					return true;
				}
				if (startConceptItem->getConcept()->hasClassName()) {
					QSet<CHierarchyNode*> chilsClassNodeSet;
					chilsClassNodeSet.insert(mOntoAnsweringItem->getOntology()->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy()->getHierarchyNode(startConceptItem->getConcept()));
					startConceptItem->setDirectSubClassNodeSet(new QSet<CHierarchyNode*>(chilsClassNodeSet));
					return true;
				}


				class CSuperSubsumptionOptimizedComplexConceptItemVisitor : public COptimizedComplexConceptItemVisitor {
				public:
					QSet<CHierarchyNode*>* mFoundMinimumSubClassNodeSet = nullptr;
					QSet<CHierarchyNode*>* mFoundMaximumSubClassNodeSet = nullptr;

					cint64 mSuperItemVisitingLimit = 50;
					cint64 mSuperItemResultVisitingLimit = 10;

					cint64 mSubItemVisitingLimit = 50;
					cint64 mSubItemResultVisitingLimit = 10;

					bool visitComplexSuperConceptItem(COptimizedComplexConceptItem* item, bool& continueVisitingSuperItems) {
						CComplexConceptStepComputationProcess* compStep = item->getComputationProcess()->getSubClassNodesComputationProcess(false);
						if (compStep && compStep->isComputationProcessFinished()) {
							QSet<CHierarchyNode*>* superClassNodeSet = item->getDirectSubClassNodeSet();
							if (!mFoundMinimumSubClassNodeSet) {
								mFoundMinimumSubClassNodeSet = new QSet<CHierarchyNode*>();
							}
							mFoundMinimumSubClassNodeSet->operator+=(*superClassNodeSet);
							continueVisitingSuperItems = false;
							if (--mSuperItemResultVisitingLimit < 0) {
								return false;
							}
						}
						if (--mSuperItemVisitingLimit < 0) {
							return false;
						}
						return true;
					}

					bool visitComplexSubConceptItem(COptimizedComplexConceptItem* item, bool& continueVisitingSubItems) {
						CComplexConceptStepComputationProcess* compStep = item->getComputationProcess()->getSubClassNodesComputationProcess(false);
						if (compStep && compStep->isComputationProcessFinished()) {
							QSet<CHierarchyNode*>* superClassNodeSet = item->getDirectSubClassNodeSet();
							if (!mFoundMaximumSubClassNodeSet) {
								mFoundMaximumSubClassNodeSet = new QSet<CHierarchyNode*>();
							}
							mFoundMaximumSubClassNodeSet->operator+=(*superClassNodeSet);
							continueVisitingSubItems = false;
							if (--mSuperItemResultVisitingLimit < 0) {
								return false;
							}
							continueVisitingSubItems = false;
							if (--mSubItemResultVisitingLimit < 0) {
								return false;
							}
						}
						if (--mSubItemVisitingLimit < 0) {
							return false;
						}
						return true;
					}

				} visitor;
				visitSuperSubConceptItems(startConceptItem, false, true, &visitor);



				QSet<CHierarchyNode*> testingSubClassCandidateSet;
				if (visitor.mFoundMinimumSubClassNodeSet) {
					visitor.mFoundMinimumSubClassNodeSet;
					for (auto node : *visitor.mFoundMinimumSubClassNodeSet) {
						testingSubClassCandidateSet.insert(node);
					}
				}
				QSet<CHierarchyNode*>* directSuperClassNodeSet = startConceptItem->getDirectSuperClassNodeSet();
				if (directSuperClassNodeSet) {
					for (QSet<CHierarchyNode*>::const_iterator it = directSuperClassNodeSet->constBegin(), itEnd = directSuperClassNodeSet->constEnd(); it != itEnd; ++it) {
						CHierarchyNode* hierNode(*it);
						testingSubClassCandidateSet.insert(hierNode);
					}
				}
				QSet<CHierarchyNode*>* possibleClassNodeTestingSet = getDirectReducedSuperNodeSet(testingSubClassCandidateSet);
				startConceptItem->setPossibleSubClassTestingNodeSet(possibleClassNodeTestingSet);
				QList<CHierarchyNode*>* possibleClassNodeTestingList = new QList<CHierarchyNode *>();
				startConceptItem->setPossibleSubClassNodeTestingList(possibleClassNodeTestingList);


				for (auto node : *possibleClassNodeTestingSet) {
					possibleClassNodeTestingList->append(node);
				}

				QSet<CHierarchyNode*>* directSubClassNodeSet = new QSet<CHierarchyNode*>();
				startConceptItem->setDirectSubClassNodeSet(directSubClassNodeSet);


				visitSuperSubConceptItems(startConceptItem, true, false, &visitor);
				if (visitor.mFoundMaximumSubClassNodeSet) {
					startConceptItem->setMaximumSubClassNodeSet(visitor.mFoundMaximumSubClassNodeSet);
					return true;
				}
				return false;
			}















			bool COptimizedComplexExpressionAnsweringHandler::searchSuperSubSatisfiabilityResult(COptimizedComplexConceptItem* startConceptItem, CAnswererContext* answererContext) {
				class CSatisfiabilityOptimizedComplexConceptItemVisitor : public COptimizedComplexConceptItemVisitor {
				public: 
					bool mSatisfiabilityResult = false;
					bool mFoundSatisfiability = false;
					bool visitComplexConceptItem(COptimizedComplexConceptItem* item, bool superItem, bool subItem) {
						CComplexConceptStepComputationProcess* compStep = item->getComputationProcess()->getSatisfiableClassNodesComputationProcess(false);
						if (compStep && compStep->isComputationProcessFinished()) {
							if (superItem) {
								if (!item->isSatisfiable()) {
									mSatisfiabilityResult = false;
									mFoundSatisfiability = true;
									return false;
								}
							}
							if (subItem) {
								if (item->isSatisfiable()) {
									mSatisfiabilityResult = true;
									mFoundSatisfiability = true;
									return false;
								}
							}
						}
						return true;
					}

				} visitor;
				visitSuperSubConceptItems(startConceptItem, true, true, &visitor);
				if (visitor.mFoundSatisfiability) {
					startConceptItem->setSatisfiability(visitor.mSatisfiabilityResult);
					startConceptItem->getComputationProcess()->getSatisfiableClassNodesComputationProcess(true)->incDerivedComputationCount()->setComputationProcessFinished(true);
					return true;
				}
				return false;
			}



			bool COptimizedComplexExpressionAnsweringHandler::visitSuperSubConceptItems(COptimizedComplexConceptItem* startConceptItem, bool visitSubItems, bool visitSuperItems, COptimizedComplexConceptItemVisitor* visitor) {
				reorderConceptClassItems();
				if (visitSuperItems) {
					QList<COptimizedComplexConceptItem*> superVisitingList;
					QSet<COptimizedComplexConceptItem*> superVisitingSet;
					superVisitingList.append(startConceptItem);
					while (!superVisitingList.isEmpty()) {
						COptimizedComplexConceptItem* visitingItem = superVisitingList.takeFirst();
						bool continueVisiting = true;
						if (!visitor->visitComplexSuperConceptItem(visitingItem, continueVisiting)) {
							return false;
						}
						if (continueVisiting) {
							for (COptimizedComplexConceptItem* superItem : *visitingItem->getSuperComplexConceptItemSet()) {
								if (!superVisitingSet.contains(superItem)) {
									superVisitingSet.insert(superItem);
									superVisitingList.append(superItem);
								}
							}
						}
					}
				}
				if (visitSubItems) {
					QList<COptimizedComplexConceptItem*> subVisitingList;
					QSet<COptimizedComplexConceptItem*> subVisitingSet;
					subVisitingList.append(startConceptItem);
					while (!subVisitingList.isEmpty()) {
						COptimizedComplexConceptItem* visitingItem = subVisitingList.takeFirst();
						bool continueVisiting = true;
						if (!visitor->visitComplexSubConceptItem(visitingItem, continueVisiting)) {
							return false;
						}
						if (continueVisiting) {
							for (COptimizedComplexConceptItem* subItem : *visitingItem->getSubComplexConceptItemSet()) {
								if (!subVisitingSet.contains(subItem)) {
									subVisitingSet.insert(subItem);
									subVisitingList.append(subItem);
								}
							}
						}
					}
				}
				return true;
			}


			bool itemSortMorePredecessorsThan(const TConceptItemPredecessorSet& item1, const TConceptItemPredecessorSet& item2) {
				return item1.second->count() > item1.second->count();
			}


			bool COptimizedComplexExpressionAnsweringHandler::searchTrivialSuperSubConceptItems(COptimizedComplexConceptItem* conceptItem) {
				QSet<TConceptNegPair>* atomicConceptSet = conceptItem->getAtomicConcpetSet();
				COptimizedComplexConceptItem* topConceptItem = mOntoAnsweringItem->getTopConceptItem();
				QList<COptimizedComplexConceptItem*> testingConceptItemList;
				for (TConceptNegPair atomicConNegPair : *atomicConceptSet) {
					bool atomicConceptClass = false;
					if (atomicConNegPair.first->hasClassName() && !atomicConNegPair.second) {						
						atomicConceptClass = true;
					}
					bool created = false;
					COptimizedComplexConceptItem* atomicConceptItem = mOntoAnsweringItem->getComplexConceptItem(atomicConNegPair.first, atomicConNegPair.second, atomicConceptClass, &created);
					if (created) {
						if (atomicConceptItem != topConceptItem) {
							topConceptItem->getSubComplexConceptItemSet()->insert(atomicConceptItem);
							atomicConceptItem->getSuperComplexConceptItemSet()->insert(topConceptItem);
						}
					}
					if (atomicConceptItem && atomicConceptItem != conceptItem) {
						testingConceptItemList.append(atomicConceptItem);
					}
				}
				reorderConceptClassItems();

				//TODO: sorting super concept list??, remove indirect super concepts
				QSet<COptimizedComplexConceptItem*>* superConceptItemSet = conceptItem->getSuperComplexConceptItemSet();
				QSet<COptimizedComplexConceptItem*>* subConceptItemSet = conceptItem->getSubComplexConceptItemSet();

				if (testingConceptItemList.isEmpty()) {
					testingConceptItemList.append(mOntoAnsweringItem->getTopConceptItem());
				}
				
				QList<TConceptItemPredecessorSet> testingConceptItemPredSetList;
				while (!testingConceptItemList.isEmpty()) {
					COptimizedComplexConceptItem* superConceptItem = testingConceptItemList.takeFirst();
					testingConceptItemPredSetList.append(TConceptItemPredecessorSet(superConceptItem, collectComplexConceptItemPredecessors(superConceptItem)));
				}
				qSort(testingConceptItemPredSetList.begin(), testingConceptItemPredSetList.end(), itemSortMorePredecessorsThan);

				QSet<COptimizedComplexConceptItem*>* commonPredItemSet = nullptr;
				while (!testingConceptItemPredSetList.isEmpty()) {
                    TConceptItemPredecessorSet conceptItemPredecessorSet = testingConceptItemPredSetList.takeFirst();
					if (!commonPredItemSet) {
						testingConceptItemList.append(conceptItemPredecessorSet.first);
						commonPredItemSet = conceptItemPredecessorSet.second;
					} else {
						if (!commonPredItemSet->contains(conceptItemPredecessorSet.first)) {
							testingConceptItemList.append(conceptItemPredecessorSet.first);
							collectComplexConceptItemPredecessors(conceptItemPredecessorSet.first, commonPredItemSet);
							delete conceptItemPredecessorSet.second;
						}
					}
				}
				delete commonPredItemSet;

				while (!testingConceptItemList.isEmpty()) {
					COptimizedComplexConceptItem* superConceptItem = testingConceptItemList.takeFirst();
					bool foundSubsumption = false;
					if (topConceptItem != superConceptItem) {
						for (COptimizedComplexConceptItem* testingConceptItem : *superConceptItem->getSubComplexConceptItemSet()) {
							if (isTriviallySubsumedConceptItem(conceptItem, testingConceptItem)) {
								testingConceptItemList.append(testingConceptItem);
								foundSubsumption = true;
							}
						}
						if (!foundSubsumption) {
							superConceptItemSet->insert(superConceptItem);
						}
					}
				}

				for (auto superConceptItem : *superConceptItemSet) {
					//TODO: handle equivalence, merge with other items
					for (auto childSuperConceptItem : *superConceptItem->getSubComplexConceptItemSet()) {
						if (isTriviallySubsumedConceptItem(childSuperConceptItem, conceptItem)) {
							testingConceptItemList.append(superConceptItem);
						}
					}
				}

				while (!testingConceptItemList.isEmpty()) {
					COptimizedComplexConceptItem* subConceptItem = testingConceptItemList.takeFirst();
					bool foundSubsumption = false;
					for (COptimizedComplexConceptItem* testingConceptItem : *subConceptItem->getSubComplexConceptItemSet()) {
						if (isTriviallySubsumedConceptItem(testingConceptItem, conceptItem)) {
							testingConceptItemList.append(testingConceptItem);
							foundSubsumption = true;
						}
					}
					if (!foundSubsumption) {
						if (subConceptItem != conceptItem) {
							subConceptItemSet->insert(subConceptItem);
						}
					}
				}

				for (COptimizedComplexConceptItem* subConceptItem : *conceptItem->getSubComplexConceptItemSet()) {
					if (subConceptItem != conceptItem) {
						subConceptItem->getSuperComplexConceptItemSet()->insert(conceptItem);
						for (COptimizedComplexConceptItem* superConceptItem : *conceptItem->getSuperComplexConceptItemSet()) {
							if (subConceptItem->hasSuperComplexConceptItem(superConceptItem)) {
								subConceptItem->getSuperComplexConceptItemSet()->remove(superConceptItem);
								superConceptItem->getSuperComplexConceptItemSet()->remove(subConceptItem);
							}
						}
					}
				}
				for (COptimizedComplexConceptItem* superConceptItem : *conceptItem->getSuperComplexConceptItemSet()) {
					superConceptItem->getSubComplexConceptItemSet()->insert(conceptItem);
				}

				return true;
			}



			bool COptimizedComplexExpressionAnsweringHandler::isAnsweringFinished(CAnswererContext* answererContext) {
				return !mOntoAnsweringItem->hasCurrentlyAnsweringQueries();
			}





			CAnsweringProgress* COptimizedComplexExpressionAnsweringHandler::getAnsweringProgress() {


				if (mTotalPossibleInstancesCount != mTestedPossibleInstancesCount) {
					cint64 tested = mTestedPossibleInstancesCount;
					cint64 total = mTotalPossibleInstancesCount;
					cint64 elapsedTime = mAnsweringStartTime.elapsed();
					double remainingMilliSeconds = 0.;
					double progessPercent = 0.;
					if (elapsedTime <= 0 && total > 0) {
						remainingMilliSeconds = 0.;
					} else {
						remainingMilliSeconds = ((double)elapsedTime / (double)tested) * (total - tested);
					}
					if (total <= 0) {
						progessPercent = 0.;
					} else {
						progessPercent = ((double)tested / (double)total) * 100.;
					}
					mAnsweringProgress.setProgessPercent(progessPercent);
					mAnsweringProgress.setRemainingMilliSeconds(remainingMilliSeconds);
					mAnsweringProgress.setTestedInstances(tested);
					mAnsweringProgress.setTotalInstances(total);
				} else {
					mAnsweringProgress.setProgessPercent(0.);
					mAnsweringProgress.setRemainingMilliSeconds(0.);
					mAnsweringProgress.setTestedInstances(mTestedPossibleInstancesCount);
					mAnsweringProgress.setTotalInstances(mTotalPossibleInstancesCount);
				}
				return &mAnsweringProgress;
			}



			CAnsweringHandler* COptimizedComplexExpressionAnsweringHandler::storeAnsweringStatistics() {
				CAnsweringStatisticsCollectionStrings* stats = new CAnsweringStatisticsCollectionStrings();
				addAnsweringStatistics(stats, false);
				mOntoAnsweringItem->setDifferenceStoredAnsweringStatisticsCollectionStrings(stats);
				return this;
			}


			CAnsweringHandler* COptimizedComplexExpressionAnsweringHandler::addAnsweringStatistics(CAnsweringStatistics* stats, bool lastStoredDiff) {
				CAnsweringCalculationStatisticsCollectionStrings* calcStatsColl = mOntoAnsweringItem->getAnsweringStatisticsCollectionStrings();
				CAnsweringStatisticsCollectionStrings* diffStats = mOntoAnsweringItem->getDifferenceStoredAnsweringStatisticsCollectionStrings();
				if (calcStatsColl) {
					for (const QString& statName : calcStatsColl->getStatisticsNameStringList()) {
						cint64 value = calcStatsColl->getStatisticIntegerValue(statName);
						if (lastStoredDiff && diffStats) {
							value = value - diffStats->getStatisticIntegerValue(statName);
						}
						stats->addProcessingStatistics(statName, value);
					}
				}

				CAnsweringStatistics* tmpStats = stats;
				if (lastStoredDiff && diffStats) {
					tmpStats = new CAnsweringStatisticsCollectionStrings();
				}
				CAnsweringHandlingStatistics* handlingStats = mOntoAnsweringItem->getAnsweringHandlingStatistics();
				if (handlingStats) {
					stats->addProcessingStatistics("answering-processed-query-count", handlingStats->getTotalAnsweredQueriesCount());
					stats->addProcessingStatistics("answering-existential-query-part-absorption-count", handlingStats->getExistentialQueryPartAbsorptionCount());
					stats->addProcessingStatistics("answering-complex-concept-item-creation-count", handlingStats->getCreatedComplexConceptItemsCount());
					stats->addProcessingStatistics("answering-requested-realization-requirements-count", handlingStats->getRequestedRealizationRequirementCount());
					stats->addProcessingStatistics("answering-complex-concept-item-candidate-propagation-count", handlingStats->getComplexConceptItemsCandidatePropagationCount());
					stats->addProcessingStatistics("answering-complex-concept-item-sub-class-subsumption-count", handlingStats->getComplexConceptItemsSubClassSubsumptionTestingCount());
					stats->addProcessingStatistics("answering-complex-concept-item-super-class-subsumption-count", handlingStats->getComplexConceptItemsSuperClassSubsumptionTestingCount());
					stats->addProcessingStatistics("answering-existential-query-part-entailment-testing-count", handlingStats->getExistentialQueryPartEntailmentTestingCount());
					stats->addProcessingStatistics("answering-existential-query-part-variable-binding-propagation-count", handlingStats->getExistentialQueryPartVariableBindingPropagationCount());
					stats->addProcessingStatistics("answering-existential-query-part-variable-binding-confirmation-count", handlingStats->getExistentialQueryPartVariableBindingConfirmationCount());
					stats->addProcessingStatistics("answering-complex-concept-assertion-entailment-testing-count", handlingStats->getComplexConceptAssertionEntailmentTestingCount());
				}
				if (lastStoredDiff && diffStats) {
					for (const QString& statName : tmpStats->getStatisticsNameStringList()) {
						cint64 value = tmpStats->getStatisticIntegerValue(statName);
						value = value - diffStats->getStatisticIntegerValue(statName);
						stats->addProcessingStatistics(statName, value);
					}
				}

				return this;
			}



























			bool COptimizedComplexExpressionAnsweringHandler::finishVariableCompositionItemComputation(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem, bool allMappingsComputed) {
				if (allMappingsComputed) {
					varCompItem->setVariableMappingsComputed(true);
				}
				if (varCompItem->getVariableMapping()->getBindingCount() <= 0) {
					buildingVarItem->setSatisfiability(false);
				}
				if (mConfDebugWriteVariableCompositionItems) {
					debugWriteVariableMappingToFileWithHeader(buildingVarItem, varCompItem);

					//if (varCompItem->getComputationStepId() >= 5) {
					//	debugCheckVariableMappingFromFile(buildingVarItem, varCompItem);
					//}
					//debugWriteVariableMappingToFile(buildingVarItem, varCompItem);
				}
				QList<COptimizedComplexVariableCompositionItemUpdateData>* updateList = varCompItem->getComputationUpdateItemList();
				for (COptimizedComplexVariableCompositionItemUpdateData updateData : *updateList) {
					COptimizedComplexVariableCompositionItem* updateItem = updateData.getVariableCompositionItem();
					COptimizedComplexBuildingVariableCompositionsItem* rescheduleBuildingVarItem = updateData.getVariableBuildingItem();
					if (!rescheduleBuildingVarItem) {
						rescheduleBuildingVarItem = buildingVarItem;
					}
					updateItem->decDependencyUpdatingCount();
					if (updateItem->getDependencyUpdatingCount() == 0) {
						queueVariableItemComputation(rescheduleBuildingVarItem, updateItem);
						if (updateData.getVariableBuildingItem()) {
							rescheduleBuildingVarItem->decWaitingSubVariableBuildingItemCount();
							if (rescheduleBuildingVarItem->getWaitingSubVariableBuildingItemCount() == 0) {
								if (!rescheduleBuildingVarItem->isProcessingQueued()) {
									rescheduleBuildingVarItem->setProcessingQueued(true);
									mOntoAnsweringItem->addProcessingVariableBuildingItem(rescheduleBuildingVarItem);
								}
							}
						}
					}
				}
				updateList->clear();
				return true;
			}






			bool COptimizedComplexExpressionAnsweringHandler::rescheduleVariableCompositionItemComputation(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItem* depSchedulingCompItem, cint64 additionalRequiredMappingCount, COptimizedComplexBuildingVariableCompositionsItem* rescheduleBuildingVarItem) {
				if (!depSchedulingCompItem->isVariableMappingsComputed()) {
					if (additionalRequiredMappingCount != -1) {
						depSchedulingCompItem->setVariableMappingsComputationRequirement(qMax(depSchedulingCompItem->getVariableMappingsComputationRequirement(), additionalRequiredMappingCount));
					} else {
						depSchedulingCompItem->setVariableMappingsComputationRequirement(-1);
					}
					depSchedulingCompItem->addComputationUpdateItem(varCompItem, rescheduleBuildingVarItem);
					if (rescheduleBuildingVarItem && rescheduleBuildingVarItem != buildingVarItem) {
						if (!buildingVarItem->isProcessingQueued()) {
							buildingVarItem->setProcessingQueued(true);
							mOntoAnsweringItem->addProcessingVariableBuildingItem(buildingVarItem);
							if (buildingVarItem->getQueryProcessingData()) {
								buildingVarItem->getQueryProcessingData()->incBuildingVariableItem();
							}
						}
						rescheduleBuildingVarItem->incWaitingSubVariableBuildingItemCount();
					}
					varCompItem->incDependencyUpdatingCount();
					queueVariableItemComputation(buildingVarItem, depSchedulingCompItem);
					return true;
				}
				return false;
			}


			bool COptimizedComplexExpressionAnsweringHandler::expectedUpdatedReschedulingDependentItemComputation(COptimizedComplexVariableCompositionItemDependence* baseItemDep, COptimizedComplexVariableIndividualMappings* reducedVarMapping, COptimizedComplexVariableCompositionItem* reductionItem, COptimizedComplexVariableCompositionItem* baseItem, COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexBuildingVariableCompositionsItem* rescheduleBuildingVarItem) {
				bool dependentItemRescheduled = false;
				cint64 totalProcessedMappingsCount = baseItemDep->getTotalProcessedBindingsCardinalityLinkerCount();
				cint64 resultingMappingsCount = reducedVarMapping->getBindingCount();
				cint64 requiredMappingCount = reductionItem->getVariableMappingsComputationRequirement();
				cint64 remaingRequiredMappingCount = requiredMappingCount;
				double depItemToThisMappingChangeFactor = (double)resultingMappingsCount / (double)totalProcessedMappingsCount;
				double depAdditionalRequiredMappingCount = depItemToThisMappingChangeFactor * (double)remaingRequiredMappingCount;

				// determine expected count
				double expectedCount = depItemToThisMappingChangeFactor * baseItem->getVariableMappingsExpectedCount();
				compVarItem->setVariableMappingsExpectedCount(expectedCount);

				if (reductionItem->requiresMoreVariableMappingsComputation()) {
					depAdditionalRequiredMappingCount = qMax(depAdditionalRequiredMappingCount + 1., totalProcessedMappingsCount + 1.);

					depAdditionalRequiredMappingCount = depAdditionalRequiredMappingCount * compVarItem->repeatedRequirementInsufficientDependencyComputationIncreaseFactor();

					if (requiredMappingCount == -1 || mConfMappingsComputationUnlimitedInterpretationSize >= 0 && depAdditionalRequiredMappingCount >= mConfMappingsComputationUnlimitedInterpretationSize) {
						depAdditionalRequiredMappingCount = -1;
					}

					if (!baseItemDep->hasMappingsCountComputationRequested(depAdditionalRequiredMappingCount)) {
						baseItemDep->setLastRequestedMappingsComputationCount(depAdditionalRequiredMappingCount);
						dependentItemRescheduled = rescheduleVariableCompositionItemComputation(buildingVarItem, compVarItem, baseItem, depAdditionalRequiredMappingCount, rescheduleBuildingVarItem);
						if (dependentItemRescheduled && mConfExtendedLogging) {
							LOG(INFO, getDomain(), logTr("Computation step %1 (attempt %2): Requesting %3 mappings from step %4.").arg(compVarItem->getComputationStepId()).arg(compVarItem->getComputationAttempt(false)).arg(depAdditionalRequiredMappingCount).arg(baseItem->getComputationStepId()), this);
						}
					}
				}
				return dependentItemRescheduled;
			}




			void COptimizedComplexExpressionAnsweringHandler::computeVariableCompositionItemFromRoleSubSuperProperties(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem) {
				COptimizedComplexVariableRoleSubSuperItem* roleSubSuperItem = (COptimizedComplexVariableRoleSubSuperItem*)compVarItem;
				CRole* role = roleSubSuperItem->getRole();
				bool superProperties = roleSubSuperItem->isRoleSuperProperties();
				COptimizedComplexVariableIndividualMappings* variableMapping = roleSubSuperItem->getVariableMapping();

				CRolePropertiesHierarchy* roleHierarchy = nullptr;
				if (!role->isDataRole()) {
					roleHierarchy = mOntoAnsweringItem->getOntology()->getClassification()->getPropertyRoleClassification()->getRolePropertiesHierarchy();
				} else {
					roleHierarchy = mOntoAnsweringItem->getOntology()->getClassification()->getDataPropertyRoleClassification()->getRolePropertiesHierarchy();
				}

				QSet<CRolePropertiesHierarchyNode*> processedNodeSet;
				QList<CRolePropertiesHierarchyNode*> processingNodeList;
				CRolePropertiesHierarchyNode* hierNode = roleHierarchy->getHierarchyNode(role);
				if (hierNode) {
					processingNodeList.append(hierNode);
					processedNodeSet.insert(hierNode);
				}

				bool mappingProcessed = true;
				COptimizedComplexVariableIndividualBindings* binding = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = nullptr;


				while (!processingNodeList.isEmpty()) {
					CRolePropertiesHierarchyNode* hierNode = processingNodeList.takeFirst();

					if (!binding) {
						binding = createBindingsForVariableCompositionItems(1, compVarItem, buildingVarItem);
					}
					binding->setBinding(0, hierNode);
					if (!linker) {
						linker = createBindingsLinkerForVariableCompositionItems(binding, nullptr, compVarItem, buildingVarItem);
					}
					if (variableMapping->addInsertingBindingsCardinalityLinker(linker, true)) {
						binding = nullptr;
						linker = nullptr;
					}

					QSet<CRolePropertiesHierarchyNode*>* nextNodeSet = nullptr;
					if (superProperties) {
						nextNodeSet = hierNode->getParentNodeSet();
					} else {
						nextNodeSet = hierNode->getChildNodeSet();
					}
					for (CRolePropertiesHierarchyNode* nextHierNode : *nextNodeSet) {
						if (!processedNodeSet.contains(nextHierNode)) {
							processingNodeList.append(nextHierNode);
							processedNodeSet.insert(nextHierNode);
						}
					}

				}


				variableMapping->incCurrentUpdateId();
				roleSubSuperItem->setVariableMappingsExpectedCount(roleSubSuperItem->getVariableMappingsCurrentCount());

				finishVariableCompositionItemComputation(buildingVarItem, compVarItem, true);
			}




			void COptimizedComplexExpressionAnsweringHandler::computeVariableCompositionItemFromConceptItemSubSuperCLasses(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem) {
				COptimizedComplexVariableConceptSubSuperItem* conceptSubSuperItem = (COptimizedComplexVariableConceptSubSuperItem*)compVarItem;
				COptimizedComplexConceptItem* conItem = conceptSubSuperItem->getConceptItem();
				bool superClasses = conceptSubSuperItem->isConceptItemSuperClasses();
				COptimizedComplexVariableIndividualMappings* variableMapping = conceptSubSuperItem->getVariableMapping();

				bool mappingProcessed = true;
				COptimizedComplexVariableIndividualBindings* binding = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = nullptr;


				QSet<CHierarchyNode*>* hierNodeSet = nullptr;
				if (superClasses) {
					hierNodeSet = conItem->getDirectSuperClassNodeSet();
				} else {
					hierNodeSet = conItem->getDirectSubClassNodeSet();
				}

				QSet<CHierarchyNode*> processedNodeSet;
				QList<CHierarchyNode*> processingNodeList;
				for (CHierarchyNode* hierNode : *hierNodeSet) {
					processingNodeList.append(hierNode);
					processedNodeSet.insert(hierNode);
				}

				while (!processingNodeList.isEmpty()) {
					CHierarchyNode* hierNode = processingNodeList.takeFirst();

					if (!binding) {
						binding = createBindingsForVariableCompositionItems(1, compVarItem, buildingVarItem);
					}
					binding->setBinding(0, hierNode);
					if (!linker) {
						linker = createBindingsLinkerForVariableCompositionItems(binding, nullptr, compVarItem, buildingVarItem);
					}
					if (variableMapping->addInsertingBindingsCardinalityLinker(linker, true)) {
						binding = nullptr;
						linker = nullptr;
					}

					QSet<CHierarchyNode*>* nextNodeSet = nullptr;
					if (superClasses) {
						nextNodeSet = hierNode->getParentNodeSet();
					} else {
						nextNodeSet = hierNode->getChildNodeSet();
					}
					for (CHierarchyNode* nextHierNode : *nextNodeSet) {
						if (!processedNodeSet.contains(nextHierNode)) {
							processingNodeList.append(nextHierNode);
							processedNodeSet.insert(nextHierNode);
						}
					}

				}


				variableMapping->incCurrentUpdateId();
				conceptSubSuperItem->setVariableMappingsExpectedCount(conceptSubSuperItem->getVariableMappingsCurrentCount());

				finishVariableCompositionItemComputation(buildingVarItem, compVarItem, true);
			}







			void COptimizedComplexExpressionAnsweringHandler::computeVariableCompositionItemSubSuperClassesPropagation(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem) {
				COptimizedComplexVariableSubSuperClassesPropagationItem* conceptSubSuperClassesPropItem = (COptimizedComplexVariableSubSuperClassesPropagationItem*)compVarItem;
				bool superClasses = conceptSubSuperClassesPropItem->isSuperClassesPropagation();
				COptimizedComplexVariableIndividualMappings* variableMapping = conceptSubSuperClassesPropItem->getVariableMapping();


				COptimizedComplexVariableCompositionItem* baseItem = conceptSubSuperClassesPropItem->getBaseItem();
				COptimizedComplexVariableCompositionItemDependence* baseItemDep = conceptSubSuperClassesPropItem->getBaseItemDependence();
				COptimizedComplexVariableIndividualMappings* baseVarMapping = baseItem->getVariableMapping();
				COptimizedComplexVariableIndividualMappings* propVarMapping = conceptSubSuperClassesPropItem->getVariableMapping();

				cint64 baseSize = baseVarMapping->getBindingSize();
				cint64 propSize = baseSize + 1;
				cint64 newVarIdx = baseSize;
				cint64 propVarIdx = conceptSubSuperClassesPropItem->getVariableIndex();

				COptimizedComplexVariableIndividualBindings* propagatedBindings = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinality* propagatedCardinalites = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = nullptr;


				if (baseVarMapping->getBindingCount() <= 0) {
					if (rescheduleVariableCompositionItemComputation(buildingVarItem, compVarItem, baseItem, 1)) {
						return;
					}
				}

				COptimizedComplexVariableIndividualMappings::VARIABLE_TYPE redVarMapType = baseVarMapping->getBindingMapping(propVarIdx);

				function<void(COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* cardinalites)> propHandlingFunc = [&](COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* cardinalites) {
					TIndividualInstanceItemDataBinding& propBinding = bindings->getBinding(propVarIdx);
					CHierarchyNode* baseHierNode = propBinding.classHierNode;

					QSet<CHierarchyNode*>* hierNodeSet = nullptr;
					if (superClasses) {
						hierNodeSet = baseHierNode->getParentNodeSet();
					} else {
						hierNodeSet = baseHierNode->getChildNodeSet();
					}

					QSet<CHierarchyNode*> processedNodeSet;
					QList<CHierarchyNode*> processingNodeList;
					for (CHierarchyNode* hierNode : *hierNodeSet) {
						processingNodeList.append(hierNode);
						processedNodeSet.insert(hierNode);
					}
					processingNodeList.append(baseHierNode);
					processedNodeSet.insert(baseHierNode);

					while (!processingNodeList.isEmpty()) {
						CHierarchyNode* hierNode = processingNodeList.takeFirst();

						if (!propagatedBindings) {
							propagatedBindings = createBindingsForVariableCompositionItems(propSize, compVarItem, buildingVarItem);
						}
						if (!propagatedCardinalites) {
							propagatedCardinalites = createBindingsCardinalitesForVariableCompositionItems(compVarItem, buildingVarItem);
						}
						if (propagatedCardinalites) {
							if (cardinalites) {
								*propagatedCardinalites = *cardinalites;
							} else {
								propagatedCardinalites->initCardinalities(1, 1);
							}
							propagatedCardinalites->setCardinalityUpdateId(-1);
						}

						cint64 nextIdx = 0;
						for (cint64 i = 0; i < baseSize; ++i) {
							propagatedBindings->setBinding(nextIdx++, bindings->getBinding(i));
						}
						propagatedBindings->setBinding(newVarIdx, hierNode);

						if (!linker) {
							linker = createBindingsLinkerForVariableCompositionItems(propagatedBindings, nullptr, compVarItem, buildingVarItem);
						} else {
							linker->setInitialCardinality(propagatedCardinalites);
						}
						if (variableMapping->addInsertingBindingsCardinalityLinker(linker, true)) {
							propagatedBindings = nullptr;
							propagatedCardinalites = nullptr;
							linker = nullptr;
						} else {
							propagatedCardinalites = linker->getCurrentCardinalities();
						}

						QSet<CHierarchyNode*>* nextNodeSet = nullptr;
						if (superClasses) {
							nextNodeSet = hierNode->getParentNodeSet();
						} else {
							nextNodeSet = hierNode->getChildNodeSet();
						}
						for (CHierarchyNode* nextHierNode : *nextNodeSet) {
							if (!processedNodeSet.contains(nextHierNode)) {
								processingNodeList.append(nextHierNode);
								processedNodeSet.insert(nextHierNode);
							}
						}

					}

				};



				bool mappingProcessed = false;
				while (conceptSubSuperClassesPropItem->requiresMoreVariableMappingsComputation() && (!baseItemDep->isBatchProcessed() || baseItemDep->loadNextBatch())) {

					while (conceptSubSuperClassesPropItem->requiresMoreVariableMappingsComputation() && baseItemDep->getBatchCurrentBindingsCardinalityLinker(false)) {
						mappingProcessed = true;
						COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinker = baseItemDep->getBatchCurrentBindingsCardinalityLinker(true);
						COptimizedComplexVariableIndividualBindings* bindings = bindingLinker->getBindings();
						COptimizedComplexVariableIndividualBindingsCardinality* cardinalites = bindingLinker->getInitialCardinalities();

						propHandlingFunc(bindings, cardinalites);
					}


					while (conceptSubSuperClassesPropItem->requiresMoreVariableMappingsComputation() && baseItemDep->getBatchCurrentUpdatedCardinalityLinker(false)) {
						mappingProcessed = true;
						COptimizedComplexVariableIndividualUpdateCardinalityLinker* updatedCardinalityLinker = baseItemDep->getBatchCurrentUpdatedCardinalityLinker(true);
						COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites = updatedCardinalityLinker->getPreviousCardinality();
						COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinker = updatedCardinalityLinker->getUpdatedBindingsCardinalityLinker();
						COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites = updatedCardinalityLinker->getNewCardinality();
						COptimizedComplexVariableIndividualBindings* bindings = bindingLinker->getBindings();

						COptimizedComplexVariableIndividualBindingsCardinality tmpDiffCardinalites = *newCardinalites;
						tmpDiffCardinalites.substractCardinalities(prevCardinalites);

						propHandlingFunc(bindings, &tmpDiffCardinalites);
					}

				}

				propVarMapping->incCurrentUpdateId();

				bool dependentItemRescheduled = false;
				dependentItemRescheduled = expectedUpdatedReschedulingDependentItemComputation(baseItemDep, propVarMapping, conceptSubSuperClassesPropItem, baseItem, compVarItem, buildingVarItem);

				if (!dependentItemRescheduled && compVarItem->getDependencyUpdatingCount() <= 0) {
					finishVariableCompositionItemComputation(buildingVarItem, compVarItem, baseItemDep->isProcessingFinished());
					if (mConfExtendedLogging) {
						LOG(INFO, getDomain(), logTr("Computation step %3 (attempt %4): Sub/Super classes propagation computation from %1 mappings of step %5 resulted in %2 mappings.").arg(baseVarMapping->getBindingCount()).arg(compVarItem->getVariableMapping()->getBindingCount()).arg(compVarItem->getComputationStepId()).arg(compVarItem->getComputationAttempt()).arg(baseItem->getComputationStepId()), this);
					}
				}

				configureDependentComputationIncreaseFactor(dependentItemRescheduled, mappingProcessed, compVarItem);

			}










			void COptimizedComplexExpressionAnsweringHandler::computeVariableCompositionItemSubSuperPropertiesPropagation(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem) {
				COptimizedComplexVariableSubSuperPropertiesPropagationItem* conceptSubSuperPropertiesPropItem = (COptimizedComplexVariableSubSuperPropertiesPropagationItem*)compVarItem;
				bool superProperties = conceptSubSuperPropertiesPropItem->isSuperPropertiesPropagation();
				COptimizedComplexVariableIndividualMappings* variableMapping = conceptSubSuperPropertiesPropItem->getVariableMapping();


				COptimizedComplexVariableCompositionItem* baseItem = conceptSubSuperPropertiesPropItem->getBaseItem();
				COptimizedComplexVariableCompositionItemDependence* baseItemDep = conceptSubSuperPropertiesPropItem->getBaseItemDependence();
				COptimizedComplexVariableIndividualMappings* baseVarMapping = baseItem->getVariableMapping();
				COptimizedComplexVariableIndividualMappings* propVarMapping = conceptSubSuperPropertiesPropItem->getVariableMapping();

				cint64 baseSize = baseVarMapping->getBindingSize();
				cint64 propSize = baseSize + 1;
				cint64 newVarIdx = baseSize;
				cint64 propVarIdx = conceptSubSuperPropertiesPropItem->getVariableIndex();

				COptimizedComplexVariableIndividualBindings* propagatedBindings = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinality* propagatedCardinalites = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = nullptr;


				if (baseVarMapping->getBindingCount() <= 0) {
					if (rescheduleVariableCompositionItemComputation(buildingVarItem, compVarItem, baseItem, 1)) {
						return;
					}
				}

				COptimizedComplexVariableIndividualMappings::VARIABLE_TYPE redVarMapType = baseVarMapping->getBindingMapping(propVarIdx);

				function<void(COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* cardinalites)> propHandlingFunc = [&](COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* cardinalites) {
					TIndividualInstanceItemDataBinding& propBinding = bindings->getBinding(propVarIdx);
					CRolePropertiesHierarchyNode* baseHierNode = propBinding.propertyHierNode;

					QSet<CRolePropertiesHierarchyNode*>* hierNodeSet = nullptr;
					if (superProperties) {
						hierNodeSet = baseHierNode->getParentNodeSet();
					} else {
						hierNodeSet = baseHierNode->getChildNodeSet();
					}

					QSet<CRolePropertiesHierarchyNode*> processedNodeSet;
					QList<CRolePropertiesHierarchyNode*> processingNodeList;
					for (CRolePropertiesHierarchyNode* hierNode : *hierNodeSet) {
						processingNodeList.append(hierNode);
						processedNodeSet.insert(hierNode);
					}
					processingNodeList.append(baseHierNode);
					processedNodeSet.insert(baseHierNode);

					while (!processingNodeList.isEmpty()) {
						CRolePropertiesHierarchyNode* hierNode = processingNodeList.takeFirst();

						if (!propagatedBindings) {
							propagatedBindings = createBindingsForVariableCompositionItems(propSize, compVarItem, buildingVarItem);
						}
						if (!propagatedCardinalites) {
							propagatedCardinalites = createBindingsCardinalitesForVariableCompositionItems(compVarItem, buildingVarItem);
						}
						if (propagatedCardinalites) {
							if (cardinalites) {
								*propagatedCardinalites = *cardinalites;
							} else {
								propagatedCardinalites->initCardinalities(1, 1);
							}
							propagatedCardinalites->setCardinalityUpdateId(-1);
						}

						cint64 nextIdx = 0;
						for (cint64 i = 0; i < baseSize; ++i) {
							propagatedBindings->setBinding(nextIdx++, bindings->getBinding(i));
						}
						propagatedBindings->setBinding(newVarIdx, hierNode);

						if (!linker) {
							linker = createBindingsLinkerForVariableCompositionItems(propagatedBindings, nullptr, compVarItem, buildingVarItem);
						} else {
							linker->setInitialCardinality(propagatedCardinalites);
						}
						if (variableMapping->addInsertingBindingsCardinalityLinker(linker, true)) {
							propagatedBindings = nullptr;
							propagatedCardinalites = nullptr;
							linker = nullptr;
						} else {
							propagatedCardinalites = linker->getCurrentCardinalities();
						}

						QSet<CRolePropertiesHierarchyNode*>* nextNodeSet = nullptr;
						if (superProperties) {
							nextNodeSet = hierNode->getParentNodeSet();
						} else {
							nextNodeSet = hierNode->getChildNodeSet();
						}
						for (CRolePropertiesHierarchyNode* nextHierNode : *nextNodeSet) {
							if (!processedNodeSet.contains(nextHierNode)) {
								processingNodeList.append(nextHierNode);
								processedNodeSet.insert(nextHierNode);
							}
						}

					}

				};



				bool mappingProcessed = false;
				while (conceptSubSuperPropertiesPropItem->requiresMoreVariableMappingsComputation() && (!baseItemDep->isBatchProcessed() || baseItemDep->loadNextBatch())) {

					while (conceptSubSuperPropertiesPropItem->requiresMoreVariableMappingsComputation() && baseItemDep->getBatchCurrentBindingsCardinalityLinker(false)) {
						mappingProcessed = true;
						COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinker = baseItemDep->getBatchCurrentBindingsCardinalityLinker(true);
						COptimizedComplexVariableIndividualBindings* bindings = bindingLinker->getBindings();
						COptimizedComplexVariableIndividualBindingsCardinality* cardinalites = bindingLinker->getInitialCardinalities();

						propHandlingFunc(bindings, cardinalites);
					}


					while (conceptSubSuperPropertiesPropItem->requiresMoreVariableMappingsComputation() && baseItemDep->getBatchCurrentUpdatedCardinalityLinker(false)) {
						mappingProcessed = true;
						COptimizedComplexVariableIndividualUpdateCardinalityLinker* updatedCardinalityLinker = baseItemDep->getBatchCurrentUpdatedCardinalityLinker(true);
						COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites = updatedCardinalityLinker->getPreviousCardinality();
						COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinker = updatedCardinalityLinker->getUpdatedBindingsCardinalityLinker();
						COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites = updatedCardinalityLinker->getNewCardinality();
						COptimizedComplexVariableIndividualBindings* bindings = bindingLinker->getBindings();

						COptimizedComplexVariableIndividualBindingsCardinality tmpDiffCardinalites = *newCardinalites;
						tmpDiffCardinalites.substractCardinalities(prevCardinalites);

						propHandlingFunc(bindings, &tmpDiffCardinalites);
					}

				}

				propVarMapping->incCurrentUpdateId();

				bool dependentItemRescheduled = false;
				dependentItemRescheduled = expectedUpdatedReschedulingDependentItemComputation(baseItemDep, propVarMapping, conceptSubSuperPropertiesPropItem, baseItem, compVarItem, buildingVarItem);

				if (!dependentItemRescheduled && compVarItem->getDependencyUpdatingCount() <= 0) {
					finishVariableCompositionItemComputation(buildingVarItem, compVarItem, baseItemDep->isProcessingFinished());
					if (mConfExtendedLogging) {
						LOG(INFO, getDomain(), logTr("Computation step %3 (attempt %4): Sub/Super properties propagation computation from %1 mappings of step %5 resulted in %2 mappings.").arg(baseVarMapping->getBindingCount()).arg(compVarItem->getVariableMapping()->getBindingCount()).arg(compVarItem->getComputationStepId()).arg(compVarItem->getComputationAttempt()).arg(baseItem->getComputationStepId()), this);
					}
				}

				configureDependentComputationIncreaseFactor(dependentItemRescheduled, mappingProcessed, compVarItem);

			}










			void COptimizedComplexExpressionAnsweringHandler::computeVariableCompositionItemIndividualTypeExtension(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* buildingVarItem, bool& processing, bool& subBuildItemProcessing) {
				COptimizedComplexVariableIndividualTypeExtensionItem* indiTypeExtItem = (COptimizedComplexVariableIndividualTypeExtensionItem*)compVarItem;
				COptimizedComplexVariableIndividualMappings* variableMapping = indiTypeExtItem->getVariableMapping();


				CConceptRealization* conReal = mOntoAnsweringItem->getOntology()->getRealization()->getConceptRealization();

				COptimizedComplexVariableCompositionItem* baseItem = indiTypeExtItem->getBaseItem();
				COptimizedComplexVariableCompositionItemDependence* baseItemDep = indiTypeExtItem->getBaseItemDependence();
				COptimizedComplexVariableIndividualMappings* baseVarMapping = baseItem->getVariableMapping();
				COptimizedComplexVariableIndividualMappings* extVarMapping = indiTypeExtItem->getVariableMapping();

				COptimizedComplexVariableCompositionItem* classTypeExtItem = indiTypeExtItem->getClassVariableItem();

				cint64 indiVarPos = indiTypeExtItem->getExtensionIndividualVariableId();
				cint64 classVarPos = indiTypeExtItem->getExtensionClassVariableId();

				cint64 baseSize = baseVarMapping->getBindingSize();
				cint64 extSize = 0;

				COptimizedComplexVariableIndividualMappings* classTypeExtVarMapping = nullptr;
				if (classTypeExtItem) {
					classTypeExtVarMapping = classTypeExtItem->getVariableMapping();
					extSize = classTypeExtVarMapping->getBindingSize();
				} else {
					extSize = 1;
				}
				cint64 newSize = baseSize + extSize;


				COptimizedComplexVariableIndividualBindings* extendedBindings = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinality* extendedCardinalites = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = nullptr;


				if (classTypeExtItem && rescheduleVariableCompositionItemComputation(buildingVarItem, compVarItem, classTypeExtItem, -1)) {
					return;
				}

				if (baseVarMapping->getBindingCount() <= 0) {
					if (rescheduleVariableCompositionItemComputation(buildingVarItem->getIndividualVariableSubQueryBuildingItem(), compVarItem, baseItem, 1, buildingVarItem)) {
						subBuildItemProcessing = true;
						return;
					}
				}


				QList<COptimizedComplexVariableIndividualTypeExtensionProcessingRealizationData>* waitingProcessedRealizationBindingsCardinalityDataList =  indiTypeExtItem->getWaitingProcessedRealizationBindingsCardinalityDataList();

				QList<COntologyProcessingRequirement*> reqList;


				function<void(COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* cardinalites, bool realized)> extHandlingFunc = [&](COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* cardinalites, bool realized) {
					TIndividualInstanceItemDataBinding& propBinding = bindings->getBinding(indiVarPos);

					CRealizationIndividualInstanceItemReference indInstItem = propBinding.reference;

					auto bindingCreationFunc = [&](CHierarchyNode* node, COptimizedComplexVariableIndividualBindings* classExtBindings) {
						if (!extendedBindings) {
							extendedBindings = createBindingsForVariableCompositionItems(newSize, compVarItem, buildingVarItem);
						}
						if (!extendedCardinalites) {
							extendedCardinalites = createBindingsCardinalitesForVariableCompositionItems(compVarItem, buildingVarItem);
						}
						if (extendedCardinalites) {
							if (cardinalites) {
								*extendedCardinalites = *cardinalites;
							} else {
								extendedCardinalites->initCardinalities(1, 1);
							}
							extendedCardinalites->setCardinalityUpdateId(-1);
						}

						cint64 nextIdx = 0;
						for (cint64 i = 0; i < baseSize; ++i) {
							extendedBindings->setBinding(nextIdx++, bindings->getBinding(i));
						}
						if (classExtBindings) {
							for (cint64 i = 0; i < extSize; ++i) {
								extendedBindings->setBinding(nextIdx++, classExtBindings->getBinding(i));
							}
						} else {
							extendedBindings->setBinding(nextIdx++, node);
						}

						if (!linker) {
							linker = createBindingsLinkerForVariableCompositionItems(extendedBindings, nullptr, compVarItem, buildingVarItem);
						} else {
							linker->setInitialCardinality(extendedCardinalites);
						}
						if (variableMapping->addInsertingBindingsCardinalityLinker(linker, true)) {
							extendedBindings = nullptr;
							extendedCardinalites = nullptr;
							linker = nullptr;
						} else {
							extendedCardinalites = linker->getCurrentCardinalities();
						}

					};

					bool realizationRequired = false;
					if (classTypeExtItem) {
						COptimizedComplexVariableCompositionItemDependence classItemDep(classTypeExtItem);
						while (!realized && (!classItemDep.isBatchProcessed() || classItemDep.loadNextBatch())) {
							COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindCardLinker = classItemDep.getBatchCurrentBindingsCardinalityLinker(true);
							COptimizedComplexVariableIndividualBindings* classExtBindings = bindCardLinker->getBindings();
							TIndividualInstanceItemDataBinding& classExtBinding = classExtBindings->getBinding(classVarPos);
							CHierarchyNode* classHierNode = classExtBinding.classHierNode;
							CConcept* classCon = classHierNode->getOneEquivalentConcept();
							CConceptInstantiatedItem* conceptItem = conReal->getInstantiatedItem(classCon);

							if (conReal->requiresConceptInstanceRealization(indInstItem, conceptItem)) {
								COntologyProcessingConceptRealizationRequirement* roleRealReq = new COntologyProcessingConceptRealizationRequirement(indInstItem, classCon);
								reqList.append(roleRealReq);
							}
						}

						if (realizationRequired) {
							waitingProcessedRealizationBindingsCardinalityDataList->append(COptimizedComplexVariableIndividualTypeExtensionProcessingRealizationData(bindings, cardinalites));
						} else {

							classItemDep.reset();
							while (!classItemDep.isBatchProcessed() || classItemDep.loadNextBatch()) {
								COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindCardLinker = classItemDep.getBatchCurrentBindingsCardinalityLinker(true);
								COptimizedComplexVariableIndividualBindings* classExtBindings = bindCardLinker->getBindings();
								TIndividualInstanceItemDataBinding& classExtBinding = classExtBindings->getBinding(classVarPos);
								CHierarchyNode* classHierNode = classExtBinding.classHierNode;
								CConcept* classCon = classHierNode->getOneEquivalentConcept();
								CConceptInstantiatedItem* conceptItem = conReal->getInstantiatedItem(classCon);

								if (conReal->isConceptInstance(indInstItem, conceptItem)) {

									bindingCreationFunc(nullptr, classExtBindings);

								}
							}					

						}
					} else {
						if (!realized && conReal->requiresConceptInstanceRealization(indInstItem)) {
							COntologyProcessingConceptRealizationRequirement* roleRealReq = new COntologyProcessingConceptRealizationRequirement(indInstItem);
							reqList.append(roleRealReq);
						}



						class CConceptRealizationHierarchyNodeBindingsConverterVisitor : public CConceptRealizationInstantiatedToHierarchyNodeVisitor {
						public:
							function<void(CHierarchyNode*)> mFunc;
							CConceptRealizationHierarchyNodeBindingsConverterVisitor(function<void(CHierarchyNode*)> func) {
								mFunc = func;
							}

							virtual bool visitHierarchyNode(CHierarchyNode* hierNode, CConceptRealization* conRealization)  {
								mFunc(hierNode);
								return true;
							}

						} visitor([&](CHierarchyNode* node) {
							bindingCreationFunc(node, nullptr);
						});

						if (realizationRequired) {
							waitingProcessedRealizationBindingsCardinalityDataList->append(COptimizedComplexVariableIndividualTypeExtensionProcessingRealizationData(bindings, cardinalites));
						} else {
							conReal->visitAllTypes(indInstItem, &visitor);
						}
					}

				};



				while (!waitingProcessedRealizationBindingsCardinalityDataList->isEmpty()) {
					COptimizedComplexVariableIndividualTypeExtensionProcessingRealizationData data = waitingProcessedRealizationBindingsCardinalityDataList->takeFirst();
					extHandlingFunc(data.getBindings(), data.getCardinalites(), true);
				}


				bool mappingProcessed = false;
				while (indiTypeExtItem->requiresMoreVariableMappingsComputation() && (!baseItemDep->isBatchProcessed() || baseItemDep->loadNextBatch())) {

					while (indiTypeExtItem->requiresMoreVariableMappingsComputation() && baseItemDep->getBatchCurrentBindingsCardinalityLinker(false)) {
						mappingProcessed = true;
						COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinker = baseItemDep->getBatchCurrentBindingsCardinalityLinker(true);
						COptimizedComplexVariableIndividualBindings* bindings = bindingLinker->getBindings();
						COptimizedComplexVariableIndividualBindingsCardinality* cardinalites = bindingLinker->getInitialCardinalities();

						extHandlingFunc(bindings, cardinalites, false);
					}


					while (indiTypeExtItem->requiresMoreVariableMappingsComputation() && baseItemDep->getBatchCurrentUpdatedCardinalityLinker(false)) {
						mappingProcessed = true;
						COptimizedComplexVariableIndividualUpdateCardinalityLinker* updatedCardinalityLinker = baseItemDep->getBatchCurrentUpdatedCardinalityLinker(true);
						COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites = updatedCardinalityLinker->getPreviousCardinality();
						COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinker = updatedCardinalityLinker->getUpdatedBindingsCardinalityLinker();
						COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites = updatedCardinalityLinker->getNewCardinality();
						COptimizedComplexVariableIndividualBindings* bindings = bindingLinker->getBindings();

						COptimizedComplexVariableIndividualBindingsCardinality tmpDiffCardinalites = *newCardinalites;
						tmpDiffCardinalites.substractCardinalities(prevCardinalites);

						extHandlingFunc(bindings, &tmpDiffCardinalites, false);
					}

				}


				bool realizationScheduled = indiTypeExtItem->getScheduledRealizationCount() > 0;
				if (!reqList.isEmpty()) {
					realizationScheduled = true;
					indiTypeExtItem->incScheduledRealizationCount(reqList.size());
					processCompositionItemRequirements(reqList, compVarItem, buildingVarItem, mAnswererContext);
					processing = true;
				}


				extVarMapping->incCurrentUpdateId();

				bool dependentItemRescheduled = false;
				dependentItemRescheduled = expectedUpdatedReschedulingDependentItemComputation(baseItemDep, extVarMapping, indiTypeExtItem, baseItem, compVarItem, buildingVarItem->getIndividualVariableSubQueryBuildingItem(), buildingVarItem);
				if (dependentItemRescheduled) {
					subBuildItemProcessing = true;
				}

				if (!realizationScheduled && !dependentItemRescheduled && compVarItem->getDependencyUpdatingCount() <= 0) {
					finishVariableCompositionItemComputation(buildingVarItem, compVarItem, baseItemDep->isProcessingFinished());
					if (mConfExtendedLogging) {
						if (classTypeExtItem) {
							LOG(INFO, getDomain(), logTr("Computation step %3 (attempt %4): Individual type extension computation from %1 mappings of step %5 and %7 mappings of step %6 resulted in %2 mappings.")
								.arg(baseVarMapping->getBindingCount()).arg(compVarItem->getVariableMapping()->getBindingCount()).arg(compVarItem->getComputationStepId()).arg(compVarItem->getComputationAttempt()).arg(baseItem->getComputationStepId()).arg(classTypeExtItem->getComputationStepId()).arg(classTypeExtVarMapping->getBindingCount()), this);
						} else {
							LOG(INFO, getDomain(), logTr("Computation step %3 (attempt %4): Individual type extension computation from %1 mappings of step %5 resulted in %2 mappings.")
								.arg(baseVarMapping->getBindingCount()).arg(compVarItem->getVariableMapping()->getBindingCount()).arg(compVarItem->getComputationStepId()).arg(compVarItem->getComputationAttempt()).arg(baseItem->getComputationStepId()), this);
						}
					}
				}

				configureDependentComputationIncreaseFactor(dependentItemRescheduled, mappingProcessed, compVarItem);

			}








			void COptimizedComplexExpressionAnsweringHandler::computeVariableCompositionItemIndividualPropertyExtension(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* buildingVarItem, bool& processing, bool& subBuildItemProcessing) {
				COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem* indiPropExtItem = (COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem*)compVarItem;
				COptimizedComplexVariableIndividualMappings* variableMapping = indiPropExtItem->getVariableMapping();


				CRoleRealization* roleReal = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();

				COptimizedComplexVariableCompositionItem* baseItem = indiPropExtItem->getBaseItem();
				COptimizedComplexVariableCompositionItemDependence* baseItemDep = indiPropExtItem->getBaseItemDependence();
				COptimizedComplexVariableIndividualMappings* baseVarMapping = baseItem->getVariableMapping();
				COptimizedComplexVariableIndividualMappings* extVarMapping = indiPropExtItem->getVariableMapping();

				COptimizedComplexVariableCompositionItem* propTypeExtItem = indiPropExtItem->getPropertyVariableItem();

				cint64 indiVarPos = indiPropExtItem->getExtensionIndividualVariableId();
				cint64 propVarPos = indiPropExtItem->getExtensionPropertyVariableId();

				cint64 baseSize = baseVarMapping->getBindingSize();
				cint64 extSize = 0;

				COptimizedComplexVariableIndividualMappings* propTypeExtVarMapping = nullptr;
				if (propTypeExtItem) {
					propTypeExtVarMapping = propTypeExtItem->getVariableMapping();
					extSize = propTypeExtVarMapping->getBindingSize();
				} else {
					extSize = 1;
				}
				cint64 newSize = baseSize + extSize;


				COptimizedComplexVariableIndividualBindings* extendedBindings = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinality* extendedCardinalites = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = nullptr;


				if (propTypeExtItem && rescheduleVariableCompositionItemComputation(buildingVarItem, compVarItem, propTypeExtItem, -1)) {
					return;
				}

				if (baseVarMapping->getBindingCount() <= 0) {
					if (rescheduleVariableCompositionItemComputation(buildingVarItem->getIndividualVariableSubQueryBuildingItem(), compVarItem, baseItem, 1, buildingVarItem)) {
						subBuildItemProcessing = true;
						return;
					}
				}


				QList<COptimizedComplexVariableIndividualPropertyExtensionProcessingRealizationData>* waitingProcessedRealizationBindingsCardinalityDataList =  indiPropExtItem->getWaitingProcessedRealizationBindingsCardinalityDataList();

				QList<COntologyProcessingRequirement*> reqList;


				function<void(COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* cardinalites, bool realized)> extHandlingFunc = [&](COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* cardinalites, bool realized) {
					TIndividualInstanceItemDataBinding& propBinding = bindings->getBinding(indiVarPos);

					CRealizationIndividualInstanceItemReference indInstItem = propBinding.reference;

					auto bindingCreationFunc = [&](CRolePropertiesHierarchyNode* node, COptimizedComplexVariableIndividualBindings* propExtBindings) {
						if (!extendedBindings) {
							extendedBindings = createBindingsForVariableCompositionItems(newSize, compVarItem, buildingVarItem);
						}
						if (!extendedCardinalites) {
							extendedCardinalites = createBindingsCardinalitesForVariableCompositionItems(compVarItem, buildingVarItem);
						}
						if (extendedCardinalites) {
							if (cardinalites) {
								*extendedCardinalites = *cardinalites;
							} else {
								extendedCardinalites->initCardinalities(1, 1);
							}
							extendedCardinalites->setCardinalityUpdateId(-1);
						}

						cint64 nextIdx = 0;
						for (cint64 i = 0; i < baseSize; ++i) {
							extendedBindings->setBinding(nextIdx++, bindings->getBinding(i));
						}
						if (propExtBindings) {
							for (cint64 i = 0; i < extSize; ++i) {
								extendedBindings->setBinding(nextIdx++, propExtBindings->getBinding(i));
							}
						} else {
							extendedBindings->setBinding(nextIdx++, node);
						}

						if (!linker) {
							linker = createBindingsLinkerForVariableCompositionItems(extendedBindings, nullptr, compVarItem, buildingVarItem);
						} else {
							linker->setInitialCardinality(extendedCardinalites);
						}
						if (variableMapping->addInsertingBindingsCardinalityLinker(linker, true)) {
							extendedBindings = nullptr;
							extendedCardinalites = nullptr;
							linker = nullptr;
						} else {
							extendedCardinalites = linker->getCurrentCardinalities();
						}

					};

					bool realizationRequired = false;
					if (propTypeExtItem) {
						COptimizedComplexVariableCompositionItemDependence classItemDep(propTypeExtItem);
						while (!realized && (!classItemDep.isBatchProcessed() || classItemDep.loadNextBatch())) {
							COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindCardLinker = classItemDep.getBatchCurrentBindingsCardinalityLinker(true);
							COptimizedComplexVariableIndividualBindings* propExtBindings = bindCardLinker->getBindings();
							TIndividualInstanceItemDataBinding& propExtBinding = propExtBindings->getBinding(propVarPos);
							CRolePropertiesHierarchyNode* propHierNode = propExtBinding.propertyHierNode;
							CRole* propRole = propHierNode->getOneEquivalentElement();
							CRoleInstantiatedItem* roleItem = roleReal->getRoleInstantiatedItem(propRole);

							if (roleReal->requiresSourceIndividualRolesRealization(indInstItem, roleItem)) {
								COntologyProcessingRoleRealizationRequirement* roleRealReq = new COntologyProcessingRoleRealizationRequirement(propRole, false, indInstItem);
								reqList.append(roleRealReq);
							}
						}

						if (realizationRequired) {
							waitingProcessedRealizationBindingsCardinalityDataList->append(COptimizedComplexVariableIndividualPropertyExtensionProcessingRealizationData(bindings, cardinalites));
						} else {

							classItemDep.reset();
							while (!classItemDep.isBatchProcessed() || classItemDep.loadNextBatch()) {
								COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindCardLinker = classItemDep.getBatchCurrentBindingsCardinalityLinker(true);
								COptimizedComplexVariableIndividualBindings* propExtBindings = bindCardLinker->getBindings();
								TIndividualInstanceItemDataBinding& propExtBinding = propExtBindings->getBinding(propVarPos);
								CRolePropertiesHierarchyNode* propHierNode = propExtBinding.propertyHierNode;
								CRole* propRole = propHierNode->getOneEquivalentElement();
								CRoleInstantiatedItem* roleItem = roleReal->getRoleInstantiatedItem(propRole);

								if (roleReal->isSourceIndividualRole(indInstItem, roleItem)) {

									bindingCreationFunc(nullptr, propExtBindings);

								}
							}					

						}
					} else {
						if (!realized && roleReal->requiresSourceIndividualRolesRealization(indInstItem)) {
							COntologyProcessingRoleRealizationRequirement* roleRealReq = new COntologyProcessingRoleRealizationRequirement(nullptr, false, indInstItem);
							reqList.append(roleRealReq);
						}



						class CConceptRealizationHierarchyNodeBindingsConverterVisitor : public CRoleRealizationInstantiatedToHierarchyNodeVisitor {
						public:
							function<void(CRolePropertiesHierarchyNode*)> mFunc;
							CConceptRealizationHierarchyNodeBindingsConverterVisitor(function<void(CRolePropertiesHierarchyNode*)> func) {
								mFunc = func;
							}

							virtual bool visitHierarchyNode(CRolePropertiesHierarchyNode* roleHierNode, CRoleRealization* roleRealization) {
								mFunc(roleHierNode);
								return true;
							}

						} visitor([&](CRolePropertiesHierarchyNode* node) {
							bindingCreationFunc(node, nullptr);
						});

						if (realizationRequired) {
							waitingProcessedRealizationBindingsCardinalityDataList->append(COptimizedComplexVariableIndividualPropertyExtensionProcessingRealizationData(bindings, cardinalites));
						} else {
							roleReal->visitSourceIndividualRoles(indInstItem, &visitor);
						}
					}

				};



				while (!waitingProcessedRealizationBindingsCardinalityDataList->isEmpty()) {
					COptimizedComplexVariableIndividualPropertyExtensionProcessingRealizationData data = waitingProcessedRealizationBindingsCardinalityDataList->takeFirst();
					extHandlingFunc(data.getBindings(), data.getCardinalites(), true);
				}


				bool mappingProcessed = false;
				while (indiPropExtItem->requiresMoreVariableMappingsComputation() && (!baseItemDep->isBatchProcessed() || baseItemDep->loadNextBatch())) {

					while (indiPropExtItem->requiresMoreVariableMappingsComputation() && baseItemDep->getBatchCurrentBindingsCardinalityLinker(false)) {
						mappingProcessed = true;
						COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinker = baseItemDep->getBatchCurrentBindingsCardinalityLinker(true);
						COptimizedComplexVariableIndividualBindings* bindings = bindingLinker->getBindings();
						COptimizedComplexVariableIndividualBindingsCardinality* cardinalites = bindingLinker->getInitialCardinalities();

						extHandlingFunc(bindings, cardinalites, false);
					}


					while (indiPropExtItem->requiresMoreVariableMappingsComputation() && baseItemDep->getBatchCurrentUpdatedCardinalityLinker(false)) {
						mappingProcessed = true;
						COptimizedComplexVariableIndividualUpdateCardinalityLinker* updatedCardinalityLinker = baseItemDep->getBatchCurrentUpdatedCardinalityLinker(true);
						COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites = updatedCardinalityLinker->getPreviousCardinality();
						COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinker = updatedCardinalityLinker->getUpdatedBindingsCardinalityLinker();
						COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites = updatedCardinalityLinker->getNewCardinality();
						COptimizedComplexVariableIndividualBindings* bindings = bindingLinker->getBindings();

						COptimizedComplexVariableIndividualBindingsCardinality tmpDiffCardinalites = *newCardinalites;
						tmpDiffCardinalites.substractCardinalities(prevCardinalites);

						extHandlingFunc(bindings, &tmpDiffCardinalites, false);
					}

				}


				bool realizationScheduled = indiPropExtItem->getScheduledRealizationCount() > 0;
				if (!reqList.isEmpty()) {
					realizationScheduled = true;
					indiPropExtItem->incScheduledRealizationCount(reqList.size());
					processCompositionItemRequirements(reqList, compVarItem, buildingVarItem, mAnswererContext);
					processing = true;
				}


				extVarMapping->incCurrentUpdateId();

				bool dependentItemRescheduled = false;
				dependentItemRescheduled = expectedUpdatedReschedulingDependentItemComputation(baseItemDep, extVarMapping, indiPropExtItem, baseItem, compVarItem, buildingVarItem->getIndividualVariableSubQueryBuildingItem(), buildingVarItem);
				if (dependentItemRescheduled) {
					subBuildItemProcessing = true;
				}

				if (!realizationScheduled && !dependentItemRescheduled && compVarItem->getDependencyUpdatingCount() <= 0) {
					finishVariableCompositionItemComputation(buildingVarItem, compVarItem, baseItemDep->isProcessingFinished());
					if (mConfExtendedLogging) {
						if (propTypeExtItem) {
							LOG(INFO, getDomain(), logTr("Computation step %3 (attempt %4): Individual neighboring extension computation from %1 mappings of step %5 and %7 mappings of step %6 resulted in %2 mappings.")
								.arg(baseVarMapping->getBindingCount()).arg(compVarItem->getVariableMapping()->getBindingCount()).arg(compVarItem->getComputationStepId()).arg(compVarItem->getComputationAttempt()).arg(baseItem->getComputationStepId()).arg(propTypeExtItem->getComputationStepId()).arg(propTypeExtVarMapping->getBindingCount()), this);
						} else {
							LOG(INFO, getDomain(), logTr("Computation step %3 (attempt %4): Individual neighboring extension computation from %1 mappings of step %5 resulted in %2 mappings.")
								.arg(baseVarMapping->getBindingCount()).arg(compVarItem->getVariableMapping()->getBindingCount()).arg(compVarItem->getComputationStepId()).arg(compVarItem->getComputationAttempt()).arg(baseItem->getComputationStepId()), this);
						}
					}
				}

				configureDependentComputationIncreaseFactor(dependentItemRescheduled, mappingProcessed, compVarItem);
			}




			void COptimizedComplexExpressionAnsweringHandler::computeVariableCompositionItemFromConceptItemBase(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, bool& processing) {
				COptimizedComplexVariableConceptBaseItem* conceptBaseItem = (COptimizedComplexVariableConceptBaseItem*)compVarItem;
				COptimizedComplexConceptItem* conItem = conceptBaseItem->getConceptItem();
				COptimizedComplexVariableIndividualMappings* variableMapping = conceptBaseItem->getVariableMapping();

				CConceptRealization* conceptRealization = mOntoAnsweringItem->getOntology()->getRealization()->getConceptRealization();
				COptimizedComplexConceptInstanziatedIndividualItemHash* instanceItems = conItem->getKnownInstanceItems();

				COptimizedComplexConceptInstanziatedIndividualItemLinker* instanceItemProcessingLinker = conceptBaseItem->getInstanceItemProcessingLinker();
				COptimizedComplexConceptInstanziatedIndividualItemLinker* instanceItemProcessedLinker = conceptBaseItem->getInstanceItemProcessedLinker();
				COptimizedComplexConceptInstanziatedIndividualItemLinker* instanceItemRetrievedLinker = conceptBaseItem->getInstanceItemRetrievedLinker();


				bool mappingProcessed = false;
				if (instanceItems) {
					COptimizedComplexVariableIndividualBindings* binding = nullptr;
					COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = nullptr;

					while (compVarItem->requiresMoreVariableMappingsComputation() && (instanceItemProcessingLinker != instanceItemProcessedLinker || instanceItemRetrievedLinker != instanceItems->getAddedRealizationIndividualInstanceItemReferenceLinker())) {

						while (compVarItem->requiresMoreVariableMappingsComputation() && instanceItemProcessingLinker != instanceItemProcessedLinker) {
							mappingProcessed = true;
							auto item = instanceItemProcessingLinker->getRealizationIndividualInstanceItemReference();
							instanceItemProcessingLinker = instanceItemProcessingLinker->getNext();
							if (!binding) {
								binding = createBindingsForVariableCompositionItems(1, compVarItem, buildingVarItem);
							}
							binding->setBinding(0, item);
							if (!linker) {
								linker = createBindingsLinkerForVariableCompositionItems(binding, nullptr, compVarItem, buildingVarItem);
							}
							if (variableMapping->addInsertingBindingsCardinalityLinker(linker, true)) {
								binding = nullptr;
								linker = nullptr;
							}
						}

						if (instanceItemProcessingLinker == instanceItemProcessedLinker && instanceItemRetrievedLinker != instanceItems->getAddedRealizationIndividualInstanceItemReferenceLinker()) {
							instanceItemProcessedLinker = instanceItemRetrievedLinker;
							instanceItemProcessingLinker = instanceItemRetrievedLinker = instanceItems->getAddedRealizationIndividualInstanceItemReferenceLinker();
						}

					}

					conceptBaseItem->setInstanceItemProcessingLinker(instanceItemProcessingLinker);
					conceptBaseItem->setInstanceItemProcessedLinker(instanceItemProcessedLinker);
					conceptBaseItem->setInstanceItemRetrievedLinker(instanceItemRetrievedLinker);

				}
				variableMapping->incCurrentUpdateId();



				bool moreInstanceComputationScheduled = false;
				CComplexConceptStepInstanceComputationProcess* instanceCompStep = conItem->getComputationProcess()->getInstancesComputationProcess(true);
				conceptBaseItem->setVariableMappingsExpectedCount(instanceCompStep->getExpectedInstancesCount());

				if (!instanceCompStep->hasAllInstancesComputed() && (instanceCompStep->getComputedInstancesCount() < compVarItem->getVariableMappingsComputationRequirement() || compVarItem->getVariableMappingsComputationRequirement() == -1)) {

					moreInstanceComputationScheduled = true;
					CComputedItemDataNotificationLinker* notLinker = mOntoAnsweringItem->createComputedItemDataNotificationLinker();
					notLinker->setVariableCompositionItem(compVarItem);
					notLinker->setBuildingVariableItemProcessingItem(buildingVarItem);

					double mappingRequirementCount = conceptBaseItem->getVariableMappingsComputationRequirement();

					mappingRequirementCount = mappingRequirementCount * conceptBaseItem->repeatedRequirementInsufficientDependencyComputationIncreaseFactor();

					if (mappingRequirementCount == -1 || mConfMappingsComputationUnlimitedInterpretationSize >= 0 && mappingRequirementCount >= mConfMappingsComputationUnlimitedInterpretationSize) {
						mappingRequirementCount = -1;
					}

					if (mappingRequirementCount >= 0) {
						instanceCompStep->addComputedInstancesCountRequirement(mappingRequirementCount, notLinker);
					} else {
						instanceCompStep->setAllInstanceComputationRequired(true);
						instanceCompStep->addNotificationLinker(notLinker);
					}
					buildingVarItem->setComputationStepWaiting(instanceCompStep);
					queueConceptItemProcessing(conItem, instanceCompStep);
					processing = true;

					if (mConfExtendedLogging) {
						LOG(INFO, getDomain(), logTr("Computation step %1 (attempt %2): Requesting %3 mappings from concept item %4.").arg(compVarItem->getComputationStepId()).arg(compVarItem->getComputationAttempt(false)).arg(mappingRequirementCount).arg(conItem->getConceptItemId()), this);
					}

				}


				//if (instanceCompStep->hasAllInstancesComputed() && compVarItem->debugVariableNameStringList.contains("v0__genid634__n274")) {
				//	if (instanceCompStep->hasAllInstancesComputed() && instanceItemProcessingLinker == instanceItemProcessedLinker && instanceItemRetrievedLinker == instanceItems->getAddedRealizationIndividualInstanceItemReferenceLinker()) {
				//		debugCheckVariableMappingFromFile(buildingVarItem, compVarItem);

				//		//debugWriteVariableMappingToFile(buildingVarItem, compVarItem);
				//	}
				//}


				if (!moreInstanceComputationScheduled) {
					bool completed = instanceCompStep->hasAllInstancesComputed() && instanceItemProcessingLinker == instanceItemProcessedLinker && (!instanceItems || instanceItemRetrievedLinker == instanceItems->getAddedRealizationIndividualInstanceItemReferenceLinker());
					finishVariableCompositionItemComputation(buildingVarItem, compVarItem, completed);
				}

				configureDependentComputationIncreaseFactor(moreInstanceComputationScheduled, mappingProcessed, compVarItem);



				//debugCheckVariableMappingContainsSolution(buildingVarItem, compVarItem);
			}

			void COptimizedComplexExpressionAnsweringHandler::computeVariableCompositionItemReduction(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem) {
				COptimizedComplexVariableReductionItem* reductionItem = (COptimizedComplexVariableReductionItem*)compVarItem;
				cint64 redIdx = reductionItem->getReductionVariableIndex();
				COptimizedComplexVariableIndividualMappings* reducedVarMapping = reductionItem->getVariableMapping();

				CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();

				// determine how many mappings must be calculated
				// process currently processing mappings linker of dependent item
				//     count processed mappings
				// process currently processing cardinality update linker of dependent item
				// update linkers to last one from dependent item and repeat processing
				// calculate estimation of how many mappings of dependent item are required to fulfill the requirements for the processing item
				// reschedule computation of dependent item
				// or trigger computation for dependent items


				COptimizedComplexVariableCompositionItem* baseItem = reductionItem->getBaseItem();
				COptimizedComplexVariableCompositionItemDependence* baseItemDep = reductionItem->getBaseItemDependence();
				COptimizedComplexVariableIndividualMappings* baseVarMapping = baseItem->getVariableMapping();
				cint64 baseSize = baseVarMapping->getBindingSize();
				cint64 reducedSize = baseSize - 1;
				COptimizedComplexVariableIndividualBindings* reducedBindings = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinality* reducedCardinalites = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = nullptr;


				if (baseVarMapping->getBindingCount() <= 0) {
					if (rescheduleVariableCompositionItemComputation(buildingVarItem, compVarItem, baseItem, 1)) {
						return;
					}
				}

				COptimizedComplexVariableIndividualMappings::VARIABLE_TYPE redVarMapType = baseVarMapping->getBindingMapping(redIdx);

				function<void(COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* cardinalites)> redHandlingFunc = [&](COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* cardinalites) {
					if (!reducedBindings) {
						reducedBindings = createBindingsForVariableCompositionItems(reducedSize, compVarItem, buildingVarItem);
					}
					if (!reducedCardinalites) {
						reducedCardinalites = createBindingsCardinalitesForVariableCompositionItems(compVarItem, buildingVarItem);
					}
					if (reducedCardinalites) {
						if (cardinalites) {
							*reducedCardinalites = *cardinalites;
						} else {
							reducedCardinalites->initCardinalities(1, 1);
						}
						reducedCardinalites->setCardinalityUpdateId(-1);
					}

					if (redVarMapType == COptimizedComplexVariableIndividualMappings::INDIVIDUAL_VARIABLE) {
						TIndividualInstanceItemDataBinding& reducedBinding = bindings->getBinding(redIdx);
						CInstanceBindingIndividualCountingVisitor visitor;
						sameRealization->visitSameIndividuals(reducedBinding.reference, &visitor);
						if (visitor.individualCount > 1) {
							reducedCardinalites->setSameIndividualsSeparatlyConsideredCardinality(visitor.individualCount * reducedCardinalites->getSameIndividualsSeparatlyConsideredCardinality());
						}
					} else if (redVarMapType == COptimizedComplexVariableIndividualMappings::CLASS_VARIABLE) {
						TIndividualInstanceItemDataBinding& reducedBinding = bindings->getBinding(redIdx);
						CHierarchyNode* hierNode = reducedBinding.classHierNode;
						reducedCardinalites->setSameIndividualsSeparatlyConsideredCardinality(hierNode->getEquivalentConceptCount() * reducedCardinalites->getSameIndividualsSeparatlyConsideredCardinality());
					} else if (redVarMapType == COptimizedComplexVariableIndividualMappings::PROPERTY_VARIABLE) {
						TIndividualInstanceItemDataBinding& reducedBinding = bindings->getBinding(redIdx);
						CRolePropertiesHierarchyNode* hierNode = reducedBinding.propertyHierNode;
						reducedCardinalites->setSameIndividualsSeparatlyConsideredCardinality(hierNode->getEquivalentElementCount() * reducedCardinalites->getSameIndividualsSeparatlyConsideredCardinality());
					}

					cint64 nextIdx = 0;
					for (cint64 i = 0; i < baseSize; ++i) {
						if (i != redIdx) {
							reducedBindings->setBinding(nextIdx++, bindings->getBinding(i));
						}
					}
					if (!linker) {
						linker = createBindingsLinkerForVariableCompositionItems(reducedBindings, reducedCardinalites, compVarItem, buildingVarItem);
					} else {
						linker->setInitialCardinality(reducedCardinalites);
					}
					if (reducedVarMapping->addInsertingBindingsCardinalityLinker(linker, true)) {
						reducedBindings = nullptr;
						reducedCardinalites = nullptr;
						linker = nullptr;
					} else {
						reducedCardinalites = linker->getCurrentCardinalities();
					}
				};

				bool mappingProcessed = false;
				while (reductionItem->requiresMoreVariableMappingsComputation() && (!baseItemDep->isBatchProcessed() || baseItemDep->loadNextBatch())) {

					while (reductionItem->requiresMoreVariableMappingsComputation() && baseItemDep->getBatchCurrentBindingsCardinalityLinker(false)) {
						mappingProcessed = true;
						COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinker = baseItemDep->getBatchCurrentBindingsCardinalityLinker(true);
						COptimizedComplexVariableIndividualBindings* bindings = bindingLinker->getBindings();
						COptimizedComplexVariableIndividualBindingsCardinality* cardinalites = bindingLinker->getInitialCardinalities();

						redHandlingFunc(bindings, cardinalites);
					}


					while (reductionItem->requiresMoreVariableMappingsComputation() && baseItemDep->getBatchCurrentUpdatedCardinalityLinker(false)) {
						mappingProcessed = true;
						COptimizedComplexVariableIndividualUpdateCardinalityLinker* updatedCardinalityLinker = baseItemDep->getBatchCurrentUpdatedCardinalityLinker(true);
						COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites = updatedCardinalityLinker->getPreviousCardinality();
						COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinker = updatedCardinalityLinker->getUpdatedBindingsCardinalityLinker();
						COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites = updatedCardinalityLinker->getNewCardinality();
						COptimizedComplexVariableIndividualBindings* bindings = bindingLinker->getBindings();

						COptimizedComplexVariableIndividualBindingsCardinality tmpDiffCardinalites = *newCardinalites;
						tmpDiffCardinalites.substractCardinalities(prevCardinalites);

						redHandlingFunc(bindings, &tmpDiffCardinalites);
					}

				}

				reducedVarMapping->incCurrentUpdateId();

				bool dependentItemRescheduled = false;
				dependentItemRescheduled = expectedUpdatedReschedulingDependentItemComputation(baseItemDep, reducedVarMapping, reductionItem, baseItem, compVarItem, buildingVarItem);

				if (!dependentItemRescheduled && compVarItem->getDependencyUpdatingCount() <= 0) {
					finishVariableCompositionItemComputation(buildingVarItem, compVarItem, baseItemDep->isProcessingFinished());
					if (mConfExtendedLogging) {
						LOG(INFO, getDomain(), logTr("Computation step %3 (attempt %4): Reduce computation from %1 mappings of step %5 resulted in %2 mappings.").arg(baseVarMapping->getBindingCount()).arg(compVarItem->getVariableMapping()->getBindingCount()).arg(compVarItem->getComputationStepId()).arg(compVarItem->getComputationAttempt()).arg(baseItem->getComputationStepId()), this);
					}
				}

				configureDependentComputationIncreaseFactor(dependentItemRescheduled, mappingProcessed, compVarItem);



			}







			void COptimizedComplexExpressionAnsweringHandler::computeVariableCompositionItemExtraction(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem) {
				COptimizedComplexVariableExtractionItem* extractionItem = (COptimizedComplexVariableExtractionItem*)compVarItem;
				cint64 extIdx = extractionItem->getExtractionVariableIndex();
				COptimizedComplexVariableIndividualMappings* reducedVarMapping = extractionItem->getVariableMapping();


				// determine how many mappings must be calculated
				// process currently processing mappings linker of dependent item
				//     count processed mappings
				// process currently processing cardinality update linker of dependent item
				// update linkers to last one from dependent item and repeat processing
				// calculate estimation of how many mappings of dependent item are required to fulfill the requirements for the processing item
				// reschedule computation of dependent item
				// or trigger computation for dependent items


				COptimizedComplexVariableCompositionItem* baseItem = extractionItem->getBaseItem();
				COptimizedComplexVariableCompositionItemDependence* baseItemDep = extractionItem->getBaseItemDependence();
				COptimizedComplexVariableIndividualMappings* baseVarMapping = baseItem->getVariableMapping();
				COptimizedComplexVariableIndividualBindings* extractedBindings = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = nullptr;


				if (baseVarMapping->getBindingCount() <= 0) {
					if (rescheduleVariableCompositionItemComputation(buildingVarItem, compVarItem, baseItem, 1)) {
						return;
					}
				}


				function<void(COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* cardinalites)> redHandlingFunc = [&](COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* cardinalites) {
					if (!extractedBindings) {
						extractedBindings = createBindingsForVariableCompositionItems(1, compVarItem, buildingVarItem);
					}

					TIndividualInstanceItemDataBinding& reducedBinding = bindings->getBinding(extIdx);
					extractedBindings->setBinding(0, reducedBinding);

					if (!linker) {
						linker = createBindingsLinkerForVariableCompositionItems(extractedBindings, nullptr, compVarItem, buildingVarItem);
					} else {
						linker->setInitialCardinality(nullptr);
					}
					if (reducedVarMapping->addInsertingBindingsCardinalityLinker(linker, true)) {
						extractedBindings = nullptr;
						linker = nullptr;
					}
				};

				bool mappingProcessed = false;
				while (extractionItem->requiresMoreVariableMappingsComputation() && (!baseItemDep->isBatchProcessed() || baseItemDep->loadNextBatch())) {

					while (extractionItem->requiresMoreVariableMappingsComputation() && baseItemDep->getBatchCurrentBindingsCardinalityLinker(false)) {
						mappingProcessed = true;
						COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinker = baseItemDep->getBatchCurrentBindingsCardinalityLinker(true);
						COptimizedComplexVariableIndividualBindings* bindings = bindingLinker->getBindings();
						COptimizedComplexVariableIndividualBindingsCardinality* cardinalites = bindingLinker->getInitialCardinalities();

						redHandlingFunc(bindings, cardinalites);
					}


					while (extractionItem->requiresMoreVariableMappingsComputation() && baseItemDep->getBatchCurrentUpdatedCardinalityLinker(false)) {
						mappingProcessed = true;
						baseItemDep->getBatchCurrentUpdatedCardinalityLinker(true);
					}

				}

				reducedVarMapping->incCurrentUpdateId();

				bool dependentItemRescheduled = false;
				dependentItemRescheduled = expectedUpdatedReschedulingDependentItemComputation(baseItemDep, reducedVarMapping, extractionItem, baseItem, compVarItem, buildingVarItem);

				if (!dependentItemRescheduled && compVarItem->getDependencyUpdatingCount() <= 0) {
					finishVariableCompositionItemComputation(buildingVarItem, compVarItem, baseItemDep->isProcessingFinished());
					if (mConfExtendedLogging) {
						LOG(INFO, getDomain(), logTr("Computation step %3 (attempt %4): Extraction computation from %1 mappings of step %5 resulted in %2 mappings.").arg(baseVarMapping->getBindingCount()).arg(compVarItem->getVariableMapping()->getBindingCount()).arg(compVarItem->getComputationStepId()).arg(compVarItem->getComputationAttempt()).arg(baseItem->getComputationStepId()), this);
					}
				}

				configureDependentComputationIncreaseFactor(dependentItemRescheduled, mappingProcessed, compVarItem);



			}





			COptimizedComplexExpressionAnsweringHandler* COptimizedComplexExpressionAnsweringHandler::computeVariableCompositionItemJoin(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, bool& processing) {
				COptimizedComplexVariableJoiningItem* joiningItem = (COptimizedComplexVariableJoiningItem*)compVarItem;

				mJoinComputer->computeVariableMappingsComposition(joiningItem, buildingVarItem, mAnswererContext, processing);

				if (mConfDebugWriteVariableCompositionItems) {
					debugWriteVariableMappingToFileWithHeader(buildingVarItem, compVarItem);
				}

				//debugWriteVariableMappingToFileWithHeader(buildingVarItem, joiningItem);
				//if (joiningItem->getComputationStepId() >= 5) {
				//	debugCheckVariableMappingFromFile(buildingVarItem, joiningItem);
				//}
				//debugWriteVariableMappingToFile(buildingVarItem, joiningItem);

				return this;
			}





			COptimizedComplexExpressionAnsweringHandler* COptimizedComplexExpressionAnsweringHandler::computeVariableCompositionItemPropagation(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext, bool& processing) {
				COptimizedComplexVariableRolePropagationAbstractItem* rolePropItem = (COptimizedComplexVariableRolePropagationAbstractItem*)compVarItem;

				mPropagationComputer->computeVariableMappingsComposition(compVarItem, buildingVarItem, answererContext, processing);

				if (mConfDebugWriteVariableCompositionItems) {
					debugWriteVariableMappingToFileWithHeader(buildingVarItem, compVarItem);
				}

				//debugWriteVariableMappingToFileWithHeader(buildingVarItem, compVarItem);
				//if (compVarItem->getComputationStepId() >= 5) {
				//	debugCheckVariableMappingFromFile(buildingVarItem, compVarItem);
				//}
				//debugWriteVariableMappingToFile(buildingVarItem, compVarItem);

				return this;
			}







			bool COptimizedComplexExpressionAnsweringHandler::processCompositionItemRequirements(const QList<COntologyProcessingRequirement*>& reqList, COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext) {

				CAnsweringMessageDataRequirementCompletedRealization* reqCompMess = new CAnsweringMessageDataRequirementCompletedRealization(compVarItem, buildingVarItem);
				buildingVarItem->incVariableCompositionItemWaitingCount();
				processRequirements(answererContext, mOntoAnsweringItem->getOntology(), reqList, reqCompMess);
				return true;
			}





			void COptimizedComplexExpressionAnsweringHandler::computeVariableCompositionItemFromDataLiteralBase(COptimizedComplexVariableCompositionItem* compVarItem, CAnswererContext* answererContext, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem) {
				COptimizedComplexVariableDataLiteralBaseItem* dataBaseItem = (COptimizedComplexVariableDataLiteralBaseItem*)compVarItem;
				CRole* dataRole = dataBaseItem->getDataRole();
				CIndividualReference indiRef = dataBaseItem->getIndividualReference();
				COptimizedComplexVariableIndividualMappings* variableMapping = dataBaseItem->getVariableMapping();

				cint64 varIdx = 0;

				QSet<CDataLiteral*> dataLiteralSet;
				getInstantiatedDataLiterals(indiRef, dataRole, &dataLiteralSet, answererContext);

				COptimizedComplexVariableIndividualBindings* binding = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = nullptr;

				for (CDataLiteral* dataLiteral : dataLiteralSet) {
					if (!binding) {
						binding = createBindingsForVariableCompositionItems(1, compVarItem, buildingVarItem);
					}

					binding->setBinding(varIdx, dataLiteral);

					if (!linker) {
						linker = createBindingsLinkerForVariableCompositionItems(binding, nullptr, compVarItem, buildingVarItem);
					}
					if (variableMapping->addInsertingBindingsCardinalityLinker(linker, true)) {
						binding = nullptr;
						linker = nullptr;
					}
				}

				variableMapping->incCurrentUpdateId();
				finishVariableCompositionItemComputation(buildingVarItem, compVarItem, true);

				if (mConfExtendedLogging) {
					QString roleName = CIRIName::getRecentIRIName(dataRole->getPropertyNameLinker());
					LOG(INFO, getDomain(), logTr("Computation step %3 (attempt %4): Initialized %1 mappings for data role %2.").arg(variableMapping->getBindingCount()).arg(roleName).arg(compVarItem->getComputationStepId()).arg(compVarItem->getComputationAttempt()), this);
				}
			}

			void COptimizedComplexExpressionAnsweringHandler::computeVariableCompositionItemAbsorptionBasedExtension(COptimizedComplexVariableCompositionItem* compVarItem, bool &processing, COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext, bool &continueProcessingBuildingVarItem) {
				COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absBasedItem = (COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*)compVarItem;
				bool oneDependentItemRescheduled = false;


				if (!absBasedItem->isDependentMappingsComputationSchedluled()) {
					QSet<COptimizedComplexVariableCompositionItem*> scheduledItems;
					QHash<CExpressionVariable*, COptimizedComplexVariableCompositionItem*>* varCompItemHash = absBasedItem->getVariableExpressionCompositionItemHash();
					for (QHash<CExpressionVariable*, COptimizedComplexVariableCompositionItem*>::const_iterator itVarItem = varCompItemHash->constBegin(), itVarItemEnd = varCompItemHash->constEnd(); itVarItem != itVarItemEnd; ++itVarItem) {
						CExpressionVariable* indiVarExp = itVarItem.key();
						COptimizedComplexVariableCompositionItem* varCompItem = itVarItem.value();
						if (!varCompItem->isVariableMappingsComputed() && !scheduledItems.contains(varCompItem)) {
							
							scheduledItems.insert(varCompItem);
							bool dependentItemRescheduled = rescheduleVariableCompositionItemComputation(buildingVarItem, compVarItem, varCompItem, -1);

							if (dependentItemRescheduled && mConfExtendedLogging) {
								LOG(INFO, getDomain(), logTr("Computation step %1 (attempt %2): Requesting all mappings from step %3.").arg(compVarItem->getComputationStepId()).arg(compVarItem->getComputationAttempt(false)).arg(varCompItem->getComputationStepId()), this);
							}
							oneDependentItemRescheduled |= dependentItemRescheduled;


						}
					}
					absBasedItem->setDependentMappingsComputationSchedluled(true);
				}


				if (!oneDependentItemRescheduled) {
					if (!absBasedItem->isPropatationTestCreated()) {
						QHash<CExpressionVariable*, COptimizedComplexVariableCompositionItem*>* varCompItemHash = absBasedItem->getVariableExpressionCompositionItemHash();
						absBasedItem->setPropatationTestCreated(true);
						QHash<CConcept*, COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*>* propagationFinalizationConceptAbsorptionItemHash = mOntoAnsweringItem->getPropagationFinalizationConceptAbsorptionItemHash();
						COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionHanldingQueryPart = absBasedItem->getAbsorptionBasedHandlingData();
						QHash<CExpressionVariable*, CVariable*>* variableExpressionVariableHash = absorptionHanldingQueryPart->getVariableExpressionVariableHash();
						cint64 steeringVariableBindingsCount = 0;
						CExpressionVariable* initVarExp = absorptionHanldingQueryPart->getInitializerVariableExpression();
						bool supportSplitting = false;
						cint64 estimatedOverallBindings = 0;
						for (QHash<CExpressionVariable*, COptimizedComplexVariableCompositionItem*>::const_iterator itVarItem = varCompItemHash->constBegin(), itVarItemEnd = varCompItemHash->constEnd(); itVarItem != itVarItemEnd; ++itVarItem) {
							CExpressionVariable* indiVarExp = itVarItem.key();
							COptimizedComplexVariableCompositionItem* varCompItem = itVarItem.value();
							estimatedOverallBindings += varCompItem->getVariableMapping()->getBindingCount();
						}
						double initBindingBucketSize = (double)estimatedOverallBindings / (double)mConfMaxVariableBindingPropagationsInitializationIndividualCount;
						initBindingBucketSize = qMax(initBindingBucketSize, 10.);
						initBindingBucketSize = qMin(initBindingBucketSize, 100.);
						cint64 initBindingBucketCount = initBindingBucketSize;
						bool splittingInitialized = false;
						if (estimatedOverallBindings > 100 && absorptionHanldingQueryPart->hasIndividualVariables()) {
							supportSplitting = true;
						}



						for (QHash<CExpressionVariable*, COptimizedComplexVariableCompositionItem*>::const_iterator itVarItem = varCompItemHash->constBegin(), itVarItemEnd = varCompItemHash->constEnd(); itVarItem != itVarItemEnd; ++itVarItem) {
							CExpressionVariable* indiVarExp = itVarItem.key();
							COptimizedComplexVariableCompositionItem* varCompItem = itVarItem.value();

							CVariable* variable = variableExpressionVariableHash->value(indiVarExp);
							QSet<CIndividualReference>*& variableSteeringIndividualBinding = absBasedItem->getVariableSteeringIndividualBindingSet(variable);
							if (!variableSteeringIndividualBinding) {
								variableSteeringIndividualBinding = new QSet<CIndividualReference>();
							}

							CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();
							QList<CIndividualReference> indiList;

							CExpressionVariable* associatedVariableExpression = buildingVarItem->getVariableLastCompositionItemAssociatedVariableExpression(indiVarExp);

							// TODO: use latest item for variable to identify allowed bindings
							COptimizedComplexVariableCompositionItemVariableIndexMapping* varIndexMapping = buildingVarItem->getVariableItemIndexMapping(associatedVariableExpression).value(varCompItem);
							cint64 varPos = -1;
							cint64 initVarPos = -1;
							if (varIndexMapping) {
								for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = varIndexMapping->constBegin(), itEnd = varIndexMapping->constEnd(); it != itEnd; ++it) {
									if (it.value() == indiVarExp) {
										varPos = it.key();
									}
									if (it.value() == initVarExp && initVarExp != indiVarExp) {
										initVarPos = it.key();
									}
								}
							}
							if (varPos >= 0) {
								for (COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinkerIt = varCompItem->getVariableMapping()->getLastAddedBindingsCardinalityLinker(); bindingLinkerIt; bindingLinkerIt = bindingLinkerIt->getNext()) {
									COptimizedComplexVariableIndividualBindings* bindings = bindingLinkerIt->getBindings();
									TIndividualInstanceItemDataBinding& instItem = bindings->getBinding(varPos);

									variableSteeringIndividualBinding->insert(instItem.reference.getIndividualIdentifier());

									//indiList.clear();
									//CInstanceBindingIndividualCollectionVisitor visitor(&indiList);
									//sameRealization->visitSameIndividuals(instItem.reference, &visitor);

									//for (CIndividualReference indiRef : indiList) {
									//	variableSteeringIndividualBinding->insert(indiRef);
									//}

									if (supportSplitting && initVarPos >= 0) {
										if (!splittingInitialized) {
											absBasedItem->initPreparedSplitPropagationTestControllingItemSize(initBindingBucketCount);
										}

										TIndividualInstanceItemDataBinding& initVarInstItem = bindings->getBinding(initVarPos);
										cint64 initVarInstItemIndiId = initVarInstItem.reference.getIndividualID();

										cint64 initVarInstItemIndiHash = 0;
										if (sizeof(quint64) > sizeof(uint)) {
											initVarInstItemIndiHash = uint((initVarInstItemIndiId >> (8 * sizeof(uint) - 1)) ^ initVarInstItemIndiId);
										} else {
											initVarInstItemIndiHash = uint(initVarInstItemIndiId);
										}


										cint64 testingBucketId = initVarInstItemIndiHash % initBindingBucketCount;

										CAnsweringSplitPropagationTestControllingItem*& splitTestingItem = absBasedItem->getPreparedSplitPropagationTestControllingItem(testingBucketId);
										if (!splitTestingItem) {
											splitTestingItem = createSplitPropagationTestControllingItem(absBasedItem, absorptionHanldingQueryPart, nullptr, false);
											absBasedItem->addReaminingPreparedSplitPropagationTestControllingItem(splitTestingItem);
										}
										splitTestingItem->getBaseVariableData()->addIntegrateableVariableBinding(initVarInstItem.reference);
										splitTestingItem->getVariableData(variable)->addIntegrateableVariableBinding(instItem.reference);
									}


								}

								steeringVariableBindingsCount += variableSteeringIndividualBinding->count();
							}


						}



						if (absorptionHanldingQueryPart->hasIndividualVariables()) {
							if (mConfExtendedLogging) {
								LOG(INFO, getDomain(), logTr("Scheduling marker propagation for absorbed query part based on %1 bindings.").arg(steeringVariableBindingsCount), this);
							}
							processing = createVariableBindingPropagationTest(absBasedItem, buildingVarItem, answererContext, continueProcessingBuildingVarItem);
						} else {
							if (mConfExtendedLogging) {
								LOG(INFO, getDomain(), logTr("Testing entailment with prepared upper bounds for absorbed query part."), this);
							}
							processing = createAbsorbedQueryPartEntailmentTest(absBasedItem, buildingVarItem, answererContext, continueProcessingBuildingVarItem);
						}
					

					} else {

						COptimizedComplexVariableIndividualMappings* possibleVarIndiBindings = absBasedItem->getPossibleVariableMapping();
						if (possibleVarIndiBindings && possibleVarIndiBindings->getBindingCount() > 0) {
							QHash<COptimizedComplexVariableIndividualBindingsHasher, COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* testingVarIndiBindings = absBasedItem->getTestingVariableMapping();
							if (!testingVarIndiBindings) {
								testingVarIndiBindings = new QHash<COptimizedComplexVariableIndividualBindingsHasher, COptimizedComplexVariableIndividualBindingsCardinalityLinker*>();
								absBasedItem->setTestingVariableMapping(testingVarIndiBindings);
							}

							COptimizedComplexVariableIndividualBindingsCardinalityLinker* testingVarIndiBindingCardLinker = possibleVarIndiBindings->takeBindingsCardinalityLinker();
							testingVarIndiBindings->insert(testingVarIndiBindingCardLinker, testingVarIndiBindingCardLinker);

							CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();
							processing = createVariableBindingConfirmationTest(absBasedItem, buildingVarItem, testingVarIndiBindingCardLinker, sameRealization, answererContext);
							buildingVarItem->addComputeVariableMappingItem(absBasedItem);
							if (possibleVarIndiBindings->getBindingCount() > 0) {
								continueProcessingBuildingVarItem = true;
							}

						} else if (!buildingVarItem->isWaitingVariableBindingsConfirmation()) {
							finishVariableCompositionItemComputation(buildingVarItem, compVarItem, true);
							if (compVarItem->getVariableMapping()->getBindingCount() > 0) {
								QList<COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*>* absorptionHanldingQueryExtensionJoiningList = buildingVarItem->getAbsorptionBasedHandlingExtensionItemJoiningList();
								absorptionHanldingQueryExtensionJoiningList->append(absBasedItem);
							}

							if (mConfExtendedLogging) {
								LOG(INFO, getDomain(), logTr("Computation step %2 (attempt %3): Obtained %1 mappings via absorbed query entailment tests.").arg(compVarItem->getVariableMapping()->getBindingCount()).arg(compVarItem->getComputationStepId()).arg(compVarItem->getComputationAttempt()), this);
							}

						}

					}
				}
			}

			void COptimizedComplexExpressionAnsweringHandler::computeVariableCompositionItemDataLiteralExtension(COptimizedComplexVariableCompositionItem* compVarItem, CAnswererContext* answererContext, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem) {
				COptimizedComplexVariableDataLiteralExtensionItem* dataExtItem = (COptimizedComplexVariableDataLiteralExtensionItem*)compVarItem;
				COptimizedComplexVariableCompositionItem* baseVarItem = dataExtItem->getBaseItem();
				COptimizedComplexVariableCompositionItemDependence* baseItemDep = dataExtItem->getBaseItemDependence();

				CRole* dataRole = dataExtItem->getRole();
				COptimizedComplexVariableIndividualMappings* baseVariableMapping = baseVarItem->getVariableMapping();
				COptimizedComplexVariableIndividualMappings* variableMapping = dataExtItem->getVariableMapping();

				CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();

				// get variable index
				cint64 varIdx = dataExtItem->getVariableIndex();
				cint64 baseBindingSize = baseVariableMapping->getBindingSize();
				cint64 newBindingSize = baseVariableMapping->getBindingSize() + 1;

				cint64* baseBindingMapping = new cint64[newBindingSize];
				for (cint64 i = 0; i < baseBindingSize; ++i) {
					baseBindingMapping[i] = i;
				}
				cint64 newVarIdx = baseBindingSize;

				COptimizedComplexVariableIndividualBindings* extBindings = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinality* extCardinalites = nullptr;
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* extLinker = nullptr;


				function<void(COptimizedComplexVariableIndividualBindings* varBinding, COptimizedComplexVariableIndividualBindingsCardinality* varCardinalities)> dataExtHandlingFunc = [&](COptimizedComplexVariableIndividualBindings* varBinding, COptimizedComplexVariableIndividualBindingsCardinality* varCardinalities) {

					CRealizationIndividualInstanceItemReference instItem = varBinding->getBinding(varIdx).reference;

					QList<CIndividualReference> indiList;
					CInstanceBindingIndividualCollectionVisitor visitor(&indiList);
					sameRealization->visitSameIndividuals(instItem, &visitor);

					QSet<CDataLiteral*> dataLiteralSet;
					for (CIndividualReference indi : indiList) {
						getInstantiatedDataLiterals(indi, dataRole, &dataLiteralSet, answererContext);
					}

					for (CDataLiteral* dataLiteral : dataLiteralSet) {

						if (!extBindings) {
							extBindings = createBindingsForVariableCompositionItems(newBindingSize, compVarItem, buildingVarItem);
						}
						if (!extCardinalites && varCardinalities) {
							extCardinalites = createBindingsCardinalitesForVariableCompositionItems(compVarItem, buildingVarItem);
						}
						if (varCardinalities) {
							*extCardinalites = *varCardinalities;
						}
						if (extCardinalites) {
							extCardinalites->setCardinalityUpdateId(-1);
						}

						for (cint64 i = 0; i < baseBindingSize; ++i) {
							extBindings->setBinding(baseBindingMapping[i], varBinding->getBinding(i));
						}
						extBindings->setBinding(newVarIdx, dataLiteral);

						if (!extLinker) {
							extLinker = createBindingsLinkerForVariableCompositionItems(extBindings, extCardinalites, compVarItem, buildingVarItem);
						} else {
							extLinker->setInitialCardinality(extCardinalites);
						}
						if (variableMapping->addInsertingBindingsCardinalityLinker(extLinker, true)) {
							extBindings = nullptr;
							extCardinalites = nullptr;
							extLinker = nullptr;
						} else {
							extCardinalites = extLinker->getCurrentCardinalities();
						}

					}
				};


				bool mappingProcessed = false;
				while (dataExtItem->requiresMoreVariableMappingsComputation() && (!baseItemDep->isBatchProcessed() || baseItemDep->loadNextBatch())) {

					while (dataExtItem->requiresMoreVariableMappingsComputation() && baseItemDep->getBatchCurrentBindingsCardinalityLinker(false)) {
						mappingProcessed = true;
						COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinker = baseItemDep->getBatchCurrentBindingsCardinalityLinker(true);
						COptimizedComplexVariableIndividualBindings* bindings = bindingLinker->getBindings();
						COptimizedComplexVariableIndividualBindingsCardinality* cardinalites = bindingLinker->getInitialCardinalities();

						dataExtHandlingFunc(bindings, cardinalites);
					}


					while (dataExtItem->requiresMoreVariableMappingsComputation() && baseItemDep->getBatchCurrentUpdatedCardinalityLinker(false)) {
						mappingProcessed = true;
						COptimizedComplexVariableIndividualUpdateCardinalityLinker* updatedCardinalityLinker = baseItemDep->getBatchCurrentUpdatedCardinalityLinker(true);
						COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites = updatedCardinalityLinker->getPreviousCardinality();
						COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinker = updatedCardinalityLinker->getUpdatedBindingsCardinalityLinker();
						COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites = updatedCardinalityLinker->getNewCardinality();
						COptimizedComplexVariableIndividualBindings* bindings = bindingLinker->getBindings();

						COptimizedComplexVariableIndividualBindingsCardinality tmpDiffCardinalites = *newCardinalites;
						tmpDiffCardinalites.substractCardinalities(prevCardinalites);

						dataExtHandlingFunc(bindings, &tmpDiffCardinalites);
					}

				}
				variableMapping->incCurrentUpdateId();


				bool dependentItemRescheduled = false;
				dependentItemRescheduled = expectedUpdatedReschedulingDependentItemComputation(baseItemDep, variableMapping, dataExtItem, baseVarItem, compVarItem, buildingVarItem);

				if (!dependentItemRescheduled && compVarItem->getDependencyUpdatingCount() <= 0) {

					finishVariableCompositionItemComputation(buildingVarItem, compVarItem, baseItemDep->isProcessingFinished());

					if (mConfExtendedLogging) {
						QString roleName = CIRIName::getRecentIRIName(dataRole->getPropertyNameLinker());
						LOG(INFO, getDomain(), logTr("Computation step %4 (attempt %5): Extending %1 mappings via data role %2 resulted in %3 mappings.").arg(variableMapping->getBindingCount()).arg(roleName).arg(compVarItem->getVariableMapping()->getBindingCount()).arg(compVarItem->getComputationStepId()).arg(compVarItem->getComputationAttempt()), this);
					}
				}

				configureDependentComputationIncreaseFactor(dependentItemRescheduled, mappingProcessed, dataExtItem);

				//if (variableMapping->count() == 0) {
				//	bool debug = true;
				//}
				//debugCheckVariableMappingContainsSolution(buildingVarItem, compVarItem);
			}

			void COptimizedComplexExpressionAnsweringHandler::configureDependentComputationIncreaseFactor(bool dependentItemRescheduled, bool mappingProcessed, COptimizedComplexVariableCompositionItem* compVarItem) {
				if (dependentItemRescheduled && mappingProcessed && compVarItem->requiresMoreVariableMappingsComputation() && !compVarItem->isVariableMappingsComputed()) {
					compVarItem->increaseRepeatedRequirementInsufficientDependencyComputationIncreaseFactor(mConfMappingsRepeatedlyInsufficientDependencyComputationIncreasingFactor);
				} else if (!compVarItem->requiresMoreVariableMappingsComputation()) {
					compVarItem->setRepeatedRequirementInsufficientDependencyComputationIncreaseFactor(1.);
				}
			}








			COptimizedComplexVariableIndividualBindings* COptimizedComplexExpressionAnsweringHandler::createBindingsForVariableCompositionItems(cint64 bindingSize, COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem) {
				COptimizedComplexVariableIndividualMappings* variableMapping = compVarItem->getVariableMapping();
				CMemoryAllocationManager* memMan = variableMapping->getBindingsMemoryAllocationManager();

				TIndividualInstanceItemDataBinding* bindingArray = CObjectAllocator<TIndividualInstanceItemDataBinding>::allocateAndConstructArray(memMan, bindingSize);
				COptimizedComplexVariableIndividualBindings* bindings = CObjectAllocator<COptimizedComplexVariableIndividualBindings>::allocateAndConstruct(memMan);
				bindings->initIndividualBindings(bindingSize, bindingArray);
				return bindings;
			}



			COptimizedComplexVariableIndividualBindingsCardinality* COptimizedComplexExpressionAnsweringHandler::createBindingsCardinalitesForVariableCompositionItems(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem) {
				COptimizedComplexVariableIndividualMappings* variableMapping = compVarItem->getVariableMapping();
				CMemoryAllocationManager* memMan = variableMapping->getBindingsMemoryAllocationManager();
				COptimizedComplexVariableIndividualBindingsCardinality* bindingsCard = CObjectAllocator<COptimizedComplexVariableIndividualBindingsCardinality>::allocateAndConstruct(memMan);
				return bindingsCard;
			}



			COptimizedComplexVariableIndividualBindingsCardinalityLinker* COptimizedComplexExpressionAnsweringHandler::createBindingsLinkerForVariableCompositionItems(COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* cardinality, COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem) {
				COptimizedComplexVariableIndividualMappings* variableMapping = compVarItem->getVariableMapping();
				CMemoryAllocationManager* memMan = variableMapping->getBindingsMemoryAllocationManager();
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = CObjectAllocator<COptimizedComplexVariableIndividualBindingsCardinalityLinker>::allocateAndConstruct(memMan);
				linker->initBindingsCardinalityLinker(bindings, cardinality);
				return linker;
			}






			bool COptimizedComplexExpressionAnsweringHandler::queueComputedInstancesCacheMemoryConsumptionUpdate(COptimizedComplexConceptItem* conceptItem) {

				bool updated = updateAnswersCacheConsumptionForItem(conceptItem);

				if (!conceptItem->isCacheUpdateQueued()) {
					conceptItem->setCacheUpdateQueued(true);
					QList<COptimizedComplexConceptItem*>* updateList = mOntoAnsweringItem->getCacheAnswersComplexConceptItemUpdateList();
					updateList->append(conceptItem);
					updated = true;
				}
				return updated;
			}



			bool COptimizedComplexExpressionAnsweringHandler::queueAnswersCacheItemUpdate(COptimizedComplexVariableCompositionItem* varCompItem) {

				bool updated = updateAnswersCacheConsumptionForItem(varCompItem);


				if (!varCompItem->isCacheUpdateQueued()) {
					varCompItem->setCacheUpdateQueued(true);
					QMap<cint64, QList<COptimizedComplexVariableCompositionItem*>* >* cacheAnswersUpdateDepthVariableCompositionItemsMap = mOntoAnsweringItem->getCacheAnswersUpdateDepthVariableCompositionItemsMap();
					QList<COptimizedComplexVariableCompositionItem*>* queuedItemList = cacheAnswersUpdateDepthVariableCompositionItemsMap->value(-varCompItem->getComputationItemDepth());
					if (!queuedItemList) {
						queuedItemList = new QList<COptimizedComplexVariableCompositionItem *>();
						cacheAnswersUpdateDepthVariableCompositionItemsMap->insert(-varCompItem->getComputationItemDepth(), queuedItemList);
					}
					queuedItemList->append(varCompItem);
					if (varCompItem->getCompositionType() == COptimizedComplexVariableCompositionItem::CONCEPT_BASE) {
						COptimizedComplexVariableConceptBaseItem* conBaseVarCompItem = (COptimizedComplexVariableConceptBaseItem*)varCompItem;
						if (conBaseVarCompItem) {
							queueComputedInstancesCacheMemoryConsumptionUpdate(conBaseVarCompItem->getConceptItem());
						}
					}


					QList<COptimizedComplexVariableCompositionItem*> dependentUpdateItemList;
					QList<COptimizedComplexVariableCompositionItem*>* dependentItemList = varCompItem->getComputationDependentItemList();
					dependentUpdateItemList.append(*dependentItemList);

					while (!dependentUpdateItemList.isEmpty()) {
						COptimizedComplexVariableCompositionItem* updateItem = dependentUpdateItemList.takeFirst();
						if (!updateItem->isCacheUpdateQueued() && updateItem->isVariableMappingsComputationStarted()) {
							updateItem->setCacheUpdateQueued(true);
							QList<COptimizedComplexVariableCompositionItem*>* queuedItemList = cacheAnswersUpdateDepthVariableCompositionItemsMap->value(-updateItem->getComputationItemDepth());
							if (!queuedItemList) {
								queuedItemList = new QList<COptimizedComplexVariableCompositionItem *>();
								cacheAnswersUpdateDepthVariableCompositionItemsMap->insert(-updateItem->getComputationItemDepth(), queuedItemList);
							}
							queuedItemList->append(updateItem);

							if (updateItem->getCompositionType() == COptimizedComplexVariableCompositionItem::CONCEPT_BASE) {
								COptimizedComplexVariableConceptBaseItem* conBaseVarCompItem = (COptimizedComplexVariableConceptBaseItem*)updateItem;
								if (conBaseVarCompItem) {
									queueComputedInstancesCacheMemoryConsumptionUpdate(conBaseVarCompItem->getConceptItem());
								}
							}

							if (!updateItem->isVariableMappingsComputed()) {
								dependentUpdateItemList.append(*updateItem->getComputationDependentItemList());
							}
						}
					}

					updated = true;

				}
				return updated;
			}



			bool COptimizedComplexExpressionAnsweringHandler::updateQueuedAnswersCacheItems() {
				QMap<cint64, QList<COptimizedComplexVariableCompositionItem*>* >* cacheAnswersUpdateDepthVariableCompositionItemsMap = mOntoAnsweringItem->getCacheAnswersUpdateDepthVariableCompositionItemsMap();
				QMap<double, CCacheAnswersWeightedUsageCostItemData>* cacheAnswersWeightedUsageCostItemSetDataMap = mOntoAnsweringItem->getCacheAnswersWeightedUsageCostItemSetDataMap();
				while (!cacheAnswersUpdateDepthVariableCompositionItemsMap->isEmpty()) {
					QMap<cint64, QList<COptimizedComplexVariableCompositionItem*>* >::iterator it = cacheAnswersUpdateDepthVariableCompositionItemsMap->begin();
					QList<COptimizedComplexVariableCompositionItem*>* updateItemList = it.value();

					for (COptimizedComplexVariableCompositionItem* updateItem : *updateItemList) {
						updateItem->setCacheUpdateQueued(false);

						// collect costs of all incomplete computed successors
						double sumCost = 0;
						for (COptimizedComplexVariableCompositionItem* succItem : *updateItem->getComputationSuccessorItemSet()) {
							if (!succItem->isVariableMappingsComputed()) {
								double cost = succItem->getCacheDescendantsIncompleteComputationCostSum() + succItem->getComputationCost();
								sumCost += cost;
							}
						}
						updateItem->setCacheDescendantsIncompleteComputationCostSum(sumCost);


						double totalCost = updateItem->getComputationCost() + sumCost;
						double usageWeightFactor = updateItem->getCacheWeightedUsageFactor();

						double newCacheInstalledCostWeightedUsage = (mConfAnswersCacheCostBaseValue + totalCost) * usageWeightFactor;

						double lastCacheInstalledCostWeightedUsage = updateItem->getLastCacheInstalledCostWeightedUsage();
						if (newCacheInstalledCostWeightedUsage != lastCacheInstalledCostWeightedUsage) {
							if (lastCacheInstalledCostWeightedUsage != -1) {
								CCacheAnswersWeightedUsageCostItemData& itemSetData = cacheAnswersWeightedUsageCostItemSetDataMap->operator[](lastCacheInstalledCostWeightedUsage);
								QSet<COptimizedComplexVariableCompositionItem*>* cacheAnswersCountVarCompItemSet = itemSetData.getComplexVariableItemSet(false);
								if (cacheAnswersCountVarCompItemSet) {
									cacheAnswersCountVarCompItemSet->remove(updateItem);
									if (itemSetData.isEmpty()) {
										cacheAnswersWeightedUsageCostItemSetDataMap->remove(lastCacheInstalledCostWeightedUsage);
									}
								}
							}
							CCacheAnswersWeightedUsageCostItemData& itemSetData = cacheAnswersWeightedUsageCostItemSetDataMap->operator[](newCacheInstalledCostWeightedUsage);
							itemSetData.addItem(updateItem);
							updateItem->setLastCacheInstalledCostWeightedUsage(newCacheInstalledCostWeightedUsage);
						}

					}
					delete updateItemList;
					cacheAnswersUpdateDepthVariableCompositionItemsMap->erase(it);
				}



				QList<COptimizedComplexConceptItem*>* updateList = mOntoAnsweringItem->getCacheAnswersComplexConceptItemUpdateList();
				while (!updateList->isEmpty()) {
					COptimizedComplexConceptItem* compConItem = updateList->takeFirst();
					compConItem->setCacheUpdateQueued(false);

					double computationCost = compConItem->getComputationCost();
					COptimizedComplexVariableConceptBaseItem* varConBaseItem = compConItem->getVariableConceptBaseItem();
					if (varConBaseItem && !varConBaseItem->isVariableMappingsComputed()) {
						double descendantsCosts = varConBaseItem->getCacheDescendantsIncompleteComputationCostSum();
						descendantsCosts += varConBaseItem->getComputationCost();
						computationCost += descendantsCosts;
					}

					double usageWeightFactor = compConItem->getCacheWeightedUsageFactor();
					double newCacheInstalledCostWeightedUsage = (mConfAnswersCacheCostBaseValue + computationCost) * usageWeightFactor;

					double lastCacheInstalledCostWeightedUsage = compConItem->getLastCacheInstalledCostWeightedUsage();
					if (newCacheInstalledCostWeightedUsage != lastCacheInstalledCostWeightedUsage) {
						if (lastCacheInstalledCostWeightedUsage != -1) {
							CCacheAnswersWeightedUsageCostItemData& itemSetData = cacheAnswersWeightedUsageCostItemSetDataMap->operator[](lastCacheInstalledCostWeightedUsage);
							QSet<COptimizedComplexConceptItem*>* cacheAnswersCountCompConCompItemSet = itemSetData.getComplexConceptItemSet(false);
							if (cacheAnswersCountCompConCompItemSet) {
								cacheAnswersCountCompConCompItemSet->remove(compConItem);
								if (itemSetData.isEmpty()) {
									cacheAnswersWeightedUsageCostItemSetDataMap->remove(lastCacheInstalledCostWeightedUsage);
								}
							}
						}
						CCacheAnswersWeightedUsageCostItemData& itemSetData = cacheAnswersWeightedUsageCostItemSetDataMap->operator[](newCacheInstalledCostWeightedUsage);
						itemSetData.addItem(compConItem);
						compConItem->setLastCacheInstalledCostWeightedUsage(newCacheInstalledCostWeightedUsage);
					}

				}


				return true;
			}




			bool COptimizedComplexExpressionAnsweringHandler::updateAnswersCacheConsumptionForItem(COptimizedComplexConceptItem* compConItem) {
				bool updated = false;
				COptimizedComplexConceptInstanziatedIndividualItemHash* knownInstItemsHash = compConItem->getKnownInstanceItems();
				cint64 currentInstanceCount = 0;
				if (knownInstItemsHash) {
					currentInstanceCount = knownInstItemsHash->size();
				}
				cint64 lastInstanceCacheReportedCount = compConItem->getLastConceptInstancesCacheReportedCount();

				if (lastInstanceCacheReportedCount != currentInstanceCount) {
					cint64 currentVariableMappingCacheReportedCount = mOntoAnsweringItem->getCacheAnswersCount();
					if (lastInstanceCacheReportedCount != -1) {
						currentVariableMappingCacheReportedCount -= lastInstanceCacheReportedCount;
					}
					currentVariableMappingCacheReportedCount += currentInstanceCount;
					mOntoAnsweringItem->setCacheAnswersCount(currentVariableMappingCacheReportedCount);
					compConItem->setLastConceptInstancesCacheReportedCount(currentInstanceCount);
					updated = true;
				}


				cint64 currentInstanceMemorySize = compConItem->getMemoryConsumption();
				cint64 lastInstanceCacheReportedSize = compConItem->getLastConceptInstancesCacheReportedSize();
				if (lastInstanceCacheReportedSize != currentInstanceMemorySize) {
					cint64 currentVariableMappingCacheReportedSize = mOntoAnsweringItem->getCacheAnswersSizeBytes();
					if (lastInstanceCacheReportedSize != -1) {
						currentVariableMappingCacheReportedSize -= lastInstanceCacheReportedSize;
					}
					currentVariableMappingCacheReportedSize += currentInstanceMemorySize;
					mOntoAnsweringItem->setCacheAnswersSizeBytes(currentVariableMappingCacheReportedSize);
					compConItem->setLastConceptInstancesCacheReportedSize(currentInstanceMemorySize);
					updated = true;
				}

				return updated;
			}

			bool COptimizedComplexExpressionAnsweringHandler::updateAnswersCacheConsumptionForItem(COptimizedComplexVariableCompositionItem* varCompItem) {
				bool updated = false;
				COptimizedComplexVariableIndividualMappings* variableMapping = varCompItem->getVariableMapping();
				variableMapping->updateBindingsMemoryConsumption();
				cint64 currentBindingCount = variableMapping->getBindingCount();
				cint64 currentBindingMemorySize = variableMapping->getBindingsMemoryUsage();
				cint64 lastVariableMappingCacheReportedCount = varCompItem->getLastVariableMappingCacheReportedCount();
				cint64 lastVariableMappingCacheReportedSize = varCompItem->getLastVariableMappingCacheReportedSize();

				if (lastVariableMappingCacheReportedCount != currentBindingCount) {
					cint64 currentVariableMappingCacheReportedCount = mOntoAnsweringItem->getCacheAnswersCount();
					if (lastVariableMappingCacheReportedCount != -1) {
						currentVariableMappingCacheReportedCount -= lastVariableMappingCacheReportedCount;
					}
					currentVariableMappingCacheReportedCount += currentBindingCount;
					mOntoAnsweringItem->setCacheAnswersCount(currentVariableMappingCacheReportedCount);
					varCompItem->setLastVariableMappingCacheReportedCount(currentBindingCount);
					updated = true;
				}

				if (lastVariableMappingCacheReportedSize != currentBindingMemorySize) {
					cint64 currentVariableMappingCacheReportedSize = mOntoAnsweringItem->getCacheAnswersSizeBytes();
					if (lastVariableMappingCacheReportedSize != -1) {
						currentVariableMappingCacheReportedSize -= lastVariableMappingCacheReportedSize;
					}
					currentVariableMappingCacheReportedSize += currentBindingMemorySize;
					mOntoAnsweringItem->setCacheAnswersSizeBytes(currentVariableMappingCacheReportedSize);
					varCompItem->setLastVariableMappingCacheReportedSize(currentBindingMemorySize);
					updated = true;
				}

				return updated;
			}

			bool COptimizedComplexExpressionAnsweringHandler::increaseUsageWeight() {
				QMap<double, CCacheAnswersWeightedUsageCostItemData>* cacheAnswersWeightedUsageCostItemSetDataMap = mOntoAnsweringItem->getCacheAnswersWeightedUsageCostItemSetDataMap();
				double weight = mOntoAnsweringItem->getNextUsageWeight();
				weight = weight * mConfAnswersCacheUsageWeightIncrease;
				if (weight > 1000000) {
					double redFactor = weight;
					weight = 1;
					QSet<COptimizedComplexVariableCompositionItem*>* container = mOntoAnsweringItem->getVariableCompositionItemContainer();
					for (COptimizedComplexVariableCompositionItem* item : *container) {
						double itemWeightUsage = item->getCacheWeightedUsageFactor();
						itemWeightUsage = itemWeightUsage / redFactor;
						item->setCacheWeightedUsageFactor(itemWeightUsage);
						double lastCacheInstalledCostWeightedUsage = item->getLastCacheInstalledCostWeightedUsage();
						if (lastCacheInstalledCostWeightedUsage != -1) {
							double totalCost = item->getComputationCost() + item->getCacheDescendantsIncompleteComputationCostSum();
							double newCacheInstalledCostWeightedUsage = (mConfAnswersCacheCostBaseValue + totalCost) * itemWeightUsage;

							CCacheAnswersWeightedUsageCostItemData& lastItemSetData = cacheAnswersWeightedUsageCostItemSetDataMap->operator[](lastCacheInstalledCostWeightedUsage);
							QSet<COptimizedComplexVariableCompositionItem*>* cacheAnswersCountVarCompItemSet = lastItemSetData.getComplexVariableItemSet(false);
							if (cacheAnswersCountVarCompItemSet) {
								cacheAnswersCountVarCompItemSet->remove(item);
								if (lastItemSetData.isEmpty()) {
									cacheAnswersWeightedUsageCostItemSetDataMap->remove(lastCacheInstalledCostWeightedUsage);
								}
							}

							CCacheAnswersWeightedUsageCostItemData& newItemSetData = cacheAnswersWeightedUsageCostItemSetDataMap->operator[](newCacheInstalledCostWeightedUsage);
							newItemSetData.addItem(item);

						}
					}


					QSet<COptimizedComplexConceptItem*>* conItemContainer = mOntoAnsweringItem->getComplexConceptItemContainer();
					for (COptimizedComplexConceptItem* item : *conItemContainer) {
						double itemWeightUsage = item->getCacheWeightedUsageFactor();
						itemWeightUsage = itemWeightUsage / redFactor;
						item->setCacheWeightedUsageFactor(itemWeightUsage);
						double lastCacheInstalledCostWeightedUsage = item->getLastCacheInstalledCostWeightedUsage();
						if (lastCacheInstalledCostWeightedUsage != -1) {

							double computationCost = item->getComputationCost();
							COptimizedComplexVariableConceptBaseItem* varConBaseItem = item->getVariableConceptBaseItem();
							if (varConBaseItem) {
								double descendantsCosts = varConBaseItem->getCacheDescendantsIncompleteComputationCostSum();
								descendantsCosts += varConBaseItem->getComputationCost();
								computationCost += descendantsCosts;
							}

							double newCacheInstalledCostWeightedUsage = (mConfAnswersCacheCostBaseValue + computationCost) * itemWeightUsage;

							CCacheAnswersWeightedUsageCostItemData& lastItemSetData = cacheAnswersWeightedUsageCostItemSetDataMap->operator[](lastCacheInstalledCostWeightedUsage);
							QSet<COptimizedComplexConceptItem*>* cacheAnswersCountVarCompItemSet = lastItemSetData.getComplexConceptItemSet(false);
							if (cacheAnswersCountVarCompItemSet) {
								cacheAnswersCountVarCompItemSet->remove(item);
								if (lastItemSetData.isEmpty()) {
									cacheAnswersWeightedUsageCostItemSetDataMap->remove(lastCacheInstalledCostWeightedUsage);
								}
							}

							CCacheAnswersWeightedUsageCostItemData& newItemSetData = cacheAnswersWeightedUsageCostItemSetDataMap->operator[](newCacheInstalledCostWeightedUsage);
							newItemSetData.addItem(item);

						}
					}
				}
				mOntoAnsweringItem->setNextUsageWeight(weight);
				return true;
			}

			bool COptimizedComplexExpressionAnsweringHandler::cleanUpAnswersCache(bool queryProcessingFinished) {
				bool cleaned = false;
				cint64 clearedVarCompItemCount = 0;
				cint64 prevCacheSize = mOntoAnsweringItem->getCacheAnswersSizeBytes();
				if (queryProcessingFinished) {
					mOntoAnsweringItem->setDeleteMoreCacheEntriesWhileQueryProcessing(true);
				}
				if ((queryProcessingFinished || mOntoAnsweringItem->canDeleteMoreCacheEntriesWhileQueryProcessing()) && (mConfAnswersCacheMaximumSizeBytes != -1 && mOntoAnsweringItem->getCacheAnswersSizeBytes() > mConfAnswersCacheMaximumSizeBytes ||
					mConfAnswersCacheMaximumCount != -1 && mOntoAnsweringItem->getCacheAnswersCount() > mConfAnswersCacheMaximumCount)) {

					updateQueuedAnswersCacheItems();
					QMap<double, CCacheAnswersWeightedUsageCostItemData>* cacheAnswersWeightedUsageCostItemSetDataMap = mOntoAnsweringItem->getCacheAnswersWeightedUsageCostItemSetDataMap();
					QMap<double, CCacheAnswersWeightedUsageCostItemData>::iterator it = cacheAnswersWeightedUsageCostItemSetDataMap->begin();
					while ((mConfAnswersCacheMaximumSizeBytes != -1 && mOntoAnsweringItem->getCacheAnswersSizeBytes() > mConfAnswersCacheMaximumSizeBytes || 
							mConfAnswersCacheMaximumCount != -1 && mOntoAnsweringItem->getCacheAnswersCount() > mConfAnswersCacheMaximumCount) && 
							!cacheAnswersWeightedUsageCostItemSetDataMap->isEmpty() && it != cacheAnswersWeightedUsageCostItemSetDataMap->end()) {

						CCacheAnswersWeightedUsageCostItemData& itemData = it.value();
						QSet<COptimizedComplexVariableCompositionItem*>* cacheAnswersWeightedUsageCostVariableCompositionItemSet = itemData.getComplexVariableItemSet(false);
						QSet<COptimizedComplexConceptItem*>* cacheAnswersWeightedUsageCostComplexConceptItemSet = itemData.getComplexConceptItemSet(false);
						


						if (cacheAnswersWeightedUsageCostComplexConceptItemSet) {
							QSet<COptimizedComplexConceptItem*>::iterator itItem = cacheAnswersWeightedUsageCostComplexConceptItemSet->begin();
							while ((mConfAnswersCacheMaximumSizeBytes != -1 && mOntoAnsweringItem->getCacheAnswersSizeBytes() > mConfAnswersCacheMaximumSizeBytes ||
								mConfAnswersCacheMaximumCount != -1 && mOntoAnsweringItem->getCacheAnswersCount() > mConfAnswersCacheMaximumCount) &&
								!cacheAnswersWeightedUsageCostComplexConceptItemSet->isEmpty() && itItem != cacheAnswersWeightedUsageCostComplexConceptItemSet->end()) {

								COptimizedComplexConceptItem* resetComputationItem = *itItem;
								if (resetComputationItem->getQueryUsageCount() <= 0) {
									itItem = cacheAnswersWeightedUsageCostComplexConceptItemSet->erase(itItem);
									resetComputationItem->setLastCacheInstalledCostWeightedUsage(-1);

									cleaned = true;

									// reset computation for the item
									cleanUpAnswersCacheItem(resetComputationItem, clearedVarCompItemCount);
								} else {
									++itItem;
								}

							}
						}


						if (cacheAnswersWeightedUsageCostVariableCompositionItemSet) {
							QSet<COptimizedComplexVariableCompositionItem*>::iterator itItem = cacheAnswersWeightedUsageCostVariableCompositionItemSet->begin();
							while ((mConfAnswersCacheMaximumSizeBytes != -1 && mOntoAnsweringItem->getCacheAnswersSizeBytes() > mConfAnswersCacheMaximumSizeBytes ||
								mConfAnswersCacheMaximumCount != -1 && mOntoAnsweringItem->getCacheAnswersCount() > mConfAnswersCacheMaximumCount) &&
								!cacheAnswersWeightedUsageCostVariableCompositionItemSet->isEmpty() && itItem != cacheAnswersWeightedUsageCostVariableCompositionItemSet->end()) {

								COptimizedComplexVariableCompositionItem* resetComputationItem = *itItem;
								if (resetComputationItem->getQueryUsageCount() <= 0) {
									itItem = cacheAnswersWeightedUsageCostVariableCompositionItemSet->erase(itItem);
									resetComputationItem->setLastCacheInstalledCostWeightedUsage(-1);

									cleaned = true;

									// reset computation for the item and all incompletely computed successors
									cleanUpAnswersCacheItem(resetComputationItem, clearedVarCompItemCount);
								} else {
									++itItem;
								}

							}
						}


						if (!cacheAnswersWeightedUsageCostVariableCompositionItemSet || cacheAnswersWeightedUsageCostVariableCompositionItemSet->isEmpty()) {
							it = cacheAnswersWeightedUsageCostItemSetDataMap->erase(it);
						} else {
							++it;
						}
					}

					if (clearedVarCompItemCount < 0) {
						mOntoAnsweringItem->setDeleteMoreCacheEntriesWhileQueryProcessing(false);
					}
				}
				if (cleaned && mConfExtendedLogging) {
					cint64 cacheSize = mOntoAnsweringItem->getCacheAnswersSizeBytes();
					LOG(INFO, getDomain(), logTr("Deleted (intermediate) results for %1 compuation steps requiring %4, %3 answers in cache remain with %2.").arg(clearedVarCompItemCount).arg(getCacheSizeString(cacheSize)).arg(mOntoAnsweringItem->getCacheAnswersCount()).arg(getCacheSizeString(prevCacheSize - cacheSize)), this);
				}
				return cleaned;
			}

			QString COptimizedComplexExpressionAnsweringHandler::getCacheSizeString(cint64 cacheSize) {
				QString cacheSizeUnit = "B";
				if (cacheSize > 10 * 1024) {
					cacheSize /= 1024;
					cacheSizeUnit = "KB";
				}
				if (cacheSize > 10 * 1024) {
					cacheSize /= 1024;
					cacheSizeUnit = "MB";
				}
				if (cacheSize > 10 * 1024) {
					cacheSize /= 1024;
					cacheSizeUnit = "GB";
				}
				return QString("%1 %2").arg(cacheSize).arg(cacheSizeUnit);
			}



			bool COptimizedComplexExpressionAnsweringHandler::cleanUpAnswersCacheItem(COptimizedComplexConceptItem* comConItem, cint64& clearedVarCompItemCount) {
				comConItem->clearComputation();
				++clearedVarCompItemCount;

				queueComputedInstancesCacheMemoryConsumptionUpdate(comConItem);

				if (comConItem->getVariableConceptBaseItem() && !comConItem->getVariableConceptBaseItem()->isVariableMappingsComputed()) {
					cleanUpAnswersCacheItem(comConItem->getVariableConceptBaseItem(), clearedVarCompItemCount);
				}
				return true;
			}


			bool COptimizedComplexExpressionAnsweringHandler::cleanUpAnswersCacheItem(COptimizedComplexVariableCompositionItem* varCompItem, cint64& clearedVarCompItemCount) {
				QList<COptimizedComplexVariableCompositionItem*> clearComputationItemList;
				clearComputationItemList.append(varCompItem);


				while (!clearComputationItemList.isEmpty()) {
					COptimizedComplexVariableCompositionItem* clearCompItem = clearComputationItemList.takeFirst();

					clearCompItem->clearComputation();

					if (clearCompItem->isVariableMappingsComputationStarted()) {
						queueAnswersCacheItemUpdate(clearCompItem);
					}
					++clearedVarCompItemCount;

					QSet<COptimizedComplexVariableCompositionItem*>* succComputationItemSet = clearCompItem->getComputationSuccessorItemSet();
					for (COptimizedComplexVariableCompositionItem* succComputationItem : *succComputationItemSet) {
						if (!succComputationItem->isVariableMappingsComputed() && succComputationItem->isVariableMappingsComputationStarted()) {
							clearComputationItemList.append(succComputationItem);
						}
					}
				}
				return true;
			}



			bool COptimizedComplexExpressionAnsweringHandler::addUsedVariableCompositionItem(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem) {
				QSet<COptimizedComplexVariableCompositionItem*>* container = mOntoAnsweringItem->getVariableCompositionItemContainer();
				container->insert(varCompItem);

				if (buildingVarItem->addUsedComplexVariableCompositionItem(varCompItem)) {
					varCompItem->incCacheWeightedUsageFactor(mOntoAnsweringItem->getNextUsageWeight());
					return true;
				}
				return false;
			}


			bool COptimizedComplexExpressionAnsweringHandler::startMeasuringVariableCompositionItemComputationCost(COptimizedComplexVariableCompositionItem* varCompItem) {
				varCompItem->startComputationCostMearsuring();
				return true;
			}

			bool COptimizedComplexExpressionAnsweringHandler::endMeasuringVariableCompositionItemComputationCost(COptimizedComplexVariableCompositionItem* varCompItem) {
				varCompItem->stopComputationCostMearsuring();
				queueAnswersCacheItemUpdate(varCompItem);
				cleanUpAnswersCache(false);
				return true;
			}


			bool COptimizedComplexExpressionAnsweringHandler::startMeasuringComplexConceptItemComputationCost(COptimizedComplexConceptItem* compConItem) {
				compConItem->startComputationCostMearsuring();
				return true;
			}

			bool COptimizedComplexExpressionAnsweringHandler::endMeasuringComplexConceptItemComputationCost(COptimizedComplexConceptItem* compConItem) {
				compConItem->stopComputationCostMearsuring();
				queueComputedInstancesCacheMemoryConsumptionUpdate(compConItem);
				cleanUpAnswersCache(false);
				return true;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
