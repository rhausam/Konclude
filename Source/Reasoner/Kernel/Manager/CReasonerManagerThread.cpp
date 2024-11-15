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

#include "CReasonerManagerThread.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Manager {


				CReasonerManagerThread::CReasonerManagerThread(CWatchDog *threadWatchDog) : CIntervalThread("Reasoner-Manager",threadWatchDog) {
					mWatchDog = threadWatchDog;
					mCalculationManager = 0;
					mPrecomputationManager = 0;
					mPreprocessingManager = 0;
					mRealizationManager = 0;
					mAnswererManager = 0;
				}


				CReasonerManager *CReasonerManagerThread::initializeManager(CConfigurationProvider *configurationProvider) {
					//stopThread();

					configProvider = configurationProvider;

					classificationMan = CConfigManagerReader::readClassificationManagerConfig(configProvider);

					bool configErrorFlag = false;
					mWorkControllerCount = 1;
					QString workerCountString = CConfigDataReader::readConfigString(configProvider,"Konclude.Calculation.WorkerCount",QString(),&configErrorFlag);
					if (!configErrorFlag) {
						if (workerCountString == "AUTO") {
							mWorkControllerCount = CThread::idealThreadCount();
						} else {
							qint64 convertedWorkerCount = workerCountString.toInt(&configErrorFlag);
							if (configErrorFlag) {
								mWorkControllerCount = convertedWorkerCount;
							}
						}
					}
					configErrorFlag = false;
					cint64 processorCount = 0;
					QString processorCountString = CConfigDataReader::readConfigString(configProvider,"Konclude.Calculation.ProcessorCount",QString(),&configErrorFlag);
					if (!configErrorFlag) {
						if (processorCountString == "AUTO") {
							processorCount = CThread::idealThreadCount();
						} else {
							qint64 convertedWorkerCount = processorCountString.toInt(&configErrorFlag);
							if (configErrorFlag) {
								processorCount = convertedWorkerCount;
							}
						}
					}
					mWorkControllerCount = qMax(processorCount,mWorkControllerCount);

					mConfgAdaptThreadPoolToWorkerCount = CConfigDataReader::readConfigBoolean(configProvider, "Konclude.Calculation.AdaptThreadPoolSizeProcessorCount", true);
					mBlockThreadPoolThreadCount = CConfigDataReader::readConfigBoolean(configProvider, "Konclude.Calculation.BlockingThreadPoolThreadsCount", 1);


					//if (!isRunning()) {
					mWatchDog = 0;
					unsatCache = 0;
					mSatExpCache = 0;
					mReuseCompGraphCache = 0;
					mSatNodeExpCache = 0;
					//}
					mPrecomputationManager = new CPrecomputationManager(this);
					mPreprocessingManager = new CPreprocessingManager(this);
					mRealizationManager = new CRealizationManager(this);
					mRequirementExpander = new COntologyProcessingRequirementExpander();

					startThread();

					return this;
				}



				CReasonerManagerThread::~CReasonerManagerThread() {
					stopThread();
				}

				CReasonerManager* CReasonerManagerThread::reasoningSatisfiableCalcualtionJob(CSatisfiableCalculationJob* satCalcJob, CCallbackData* callback) {
					postEvent(new CReasoningSatisfiableCalculationJobEvent(satCalcJob,callback));
					return this;
				}



				CJobSatisfiableResult* CReasonerManagerThread::reasoningSatisfiableCalcualtionJob(CSatisfiableCalculationJob* satCalcJob) {
					CJobSatisfiableCallbackContextData* satCalcData = nullptr;
					CBlockingCallbackData callback;
					postEvent(new CReasoningSatisfiableCalculationJobEvent(satCalcJob,&callback));
					callback.waitForCallback();
					satCalcData = (CJobSatisfiableCallbackContextData*)callback.takeCallbackDataContext();
					return satCalcData;
				}


				CReasonerManager *CReasonerManagerThread::reasoningQuery(CQuery *query, CCallbackData *callback) {
					postEvent(new CCalcQueryEvent(query,callback));
					return this;
				}


				CReasonerManager *CReasonerManagerThread::reasoningQuery(CQuery *query) {
					CBlockingCallbackData callback;
					postEvent(new CCalcQueryEvent(query,&callback));
					callback.waitForCallback();
					return this;
				}


				CReasonerManager *CReasonerManagerThread::prepareOntology(CConcreteOntology* ontology, const QList<COntologyProcessingRequirement*>& reqList, CCallbackData *callback) {
					postEvent(new CPrepareOntologyEvent(ontology,reqList,callback));
					return this;
				}


				CReasonerManager *CReasonerManagerThread::prepareOntology(CConcreteOntology* ontology, const QList<COntologyProcessingRequirement*>& reqList) {
					CBlockingCallbackData callback;
					postEvent(new CPrepareOntologyEvent(ontology,reqList,&callback));
					callback.waitForCallback();
					return this;
				}




				CUnsatisfiableCache *CReasonerManagerThread::getUnsatisfiableCache() {
					return unsatCache;
				}

				CCompletionGraphCache *CReasonerManagerThread::getCompletionGraphCache() {
					return mReuseCompGraphCache;
				}


				CSaturationCache* CReasonerManagerThread::getSaturationAssociationExpansionCache() {
					return mSatNodeExpCache;
				}


				CBackendCache* CReasonerManagerThread::getBackendAssociationCache() {
					return mBackendAssCache;
				}


				COccurrenceStatisticsCache* CReasonerManagerThread::getOccurrenceStatisticsCache() {
					return mOccStatsCache;
				}


				CCalculationManager *CReasonerManagerThread::getCalculationManager() {
					return mCalculationManager;
				}

				CPrecomputationManager* CReasonerManagerThread::getPrecomputationManager() {
					return mPrecomputationManager;
				}

				CPreprocessingManager* CReasonerManagerThread::getPreprocessingManager() {
					return mPreprocessingManager;
				}

				CRealizationManager* CReasonerManagerThread::getRealizationManager() {
					return mRealizationManager;
				}

				void CReasonerManagerThread::readConfig(CConfigurationBase *config) {
					
				}



				CTaskHandleAlgorithm *CReasonerManagerThread::createTaskHandleAlgorithm() {
					CUnsatisfiableCacheHandler* unsatCacheHandler = nullptr;
					CSatisfiableExpanderCacheHandler* satExpCacheHandler = nullptr;
					CReuseCompletionGraphCacheHandler* reuseCompGraphCacheHandler = nullptr;
					CSaturationNodeExpansionCacheHandler* satNodeCacheHandler = nullptr;
					CComputedConsequencesCacheHandler* compConsCachHandler = nullptr;
					CSaturationNodeBackendAssociationCacheHandler* backendAssCacheHandler = nullptr;
					CIndividualNodeBackendCacheHandler* backendCacheHandler = nullptr;
					COccurrenceStatisticsCacheHandler* occStatsCacheHandler = nullptr;
					CSatisfiableTaskSaturationOccurrenceStatisticsCollector* satTaskOccStatCollector = nullptr;
					if (unsatCache) {
						unsatCacheHandler = new CUnsatisfiableCacheHandler(unsatCache->getCacheReader(),unsatCache->getCacheWriter());
					}
					if (mSatExpCache) {
						satExpCacheHandler = new CSatisfiableExpanderCacheHandler(mSatExpCache->createCacheReader(),mSatExpCache->createCacheWriter());
					}
					if (mReuseCompGraphCache) {
						reuseCompGraphCacheHandler = new CReuseCompletionGraphCacheHandler(mReuseCompGraphCache->createCacheReader(),mReuseCompGraphCache->createCacheWriter());
					}
					if (mSatNodeExpCache) {
						satNodeCacheHandler = new CSaturationNodeExpansionCacheHandler(mSatNodeExpCache->createCacheReader(),mSatNodeExpCache->createCacheWriter());
					}
					if (mCompConsCache) {
						compConsCachHandler = new CComputedConsequencesCacheHandler(mCompConsCache->createCacheReader(),mCompConsCache->createCacheWriter());
					}
					COccurrenceStatisticsCacheWriter* occStatsCacheWriter = nullptr;
					if (mOccStatsCache) {
						occStatsCacheWriter = mOccStatsCache->createCacheWriter();
						satTaskOccStatCollector = new CSatisfiableTaskSaturationOccurrenceStatisticsCollector(occStatsCacheWriter);
						occStatsCacheHandler = new COccurrenceStatisticsCacheHandler(occStatsCacheWriter);
					}
					if (mBackendAssCache) {
						CBackendRepresentativeMemoryCacheWriter* backAssCacheWriter = mBackendAssCache->createCacheWriter();
						CBackendRepresentativeMemoryCacheReader* backAssCacheReader = mBackendAssCache->createCacheReader();
						backendAssCacheHandler = new CSaturationNodeBackendAssociationCacheHandler(backAssCacheReader, backAssCacheWriter, occStatsCacheWriter);
						backendCacheHandler = new CIndividualNodeBackendCacheHandler(backAssCacheReader, backAssCacheWriter);
					}
					CCalculationTableauApproximationSaturationTaskHandleAlgorithm* approxSatHandleAlg = new CCalculationTableauApproximationSaturationTaskHandleAlgorithm(backendAssCacheHandler, satTaskOccStatCollector);
					CCalculationTableauCompletionTaskHandleAlgorithm* compTaskHandleAlg = new CCalculationTableauCompletionTaskHandleAlgorithm(unsatCacheHandler,satExpCacheHandler,reuseCompGraphCacheHandler,satNodeCacheHandler,compConsCachHandler,backendCacheHandler, occStatsCacheHandler);
					return new CCalculationChooseTaskHandleAlgorithm(compTaskHandleAlg,approxSatHandleAlg);
				}



				void CReasonerManagerThread::threadStarted() {
					CIntervalThread::threadStarted();
					// generate worker framework
					LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Initializing reasoner. Creating calculation context."),this);

					if (mConfgAdaptThreadPoolToWorkerCount) {
						QThreadPool::globalInstance()->setMaxThreadCount(mWorkControllerCount);
					}
					if (mBlockThreadPoolThreadCount > 0) {
						for (cint64 i = 0; i < mBlockThreadPoolThreadCount; ++i) {
							QtConcurrent::run(QThreadPool::globalInstance(), [&]() {
								mBlockThreadPoolThreadsBlockingSemaphore.acquire(1);
								mBlockThreadPoolThreadsReleasingSemaphore.release(1);
							});
						}
					}

					unsatCache = new COccurrenceUnsatisfiableCache(mWorkControllerCount+2);
					mSatExpCache = new CSignatureSatisfiableExpanderCache(configProvider->getCurrentConfiguration());
					mReuseCompGraphCache = new CReuseCompletionGraphCache();
					mSatNodeExpCache = new CSaturationNodeAssociatedExpansionCache(configProvider->getCurrentConfiguration());
					mCompConsCache = new CComputedConsequencesCache(configProvider->getCurrentConfiguration());
					mBackendAssCache = new CBackendRepresentativeMemoryCache(configProvider->getCurrentConfiguration());
					mOccStatsCache = new COccurrenceStatisticsCache(configProvider->getCurrentConfiguration());

					CConfigDependedCalculationFactory* calcFactory = new CConfigDependedCalculationFactory(this);
					CCalculationManager* calculationManager = calcFactory->createCalculationManager(configProvider);
					calcFactory->initializeManager(calculationManager,configProvider);
					mCalculationManager = calculationManager;

					LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Reasoner initialized with %1 processing unit(s).").arg(mWorkControllerCount),this);

					startTimerWithInterval(PROGRESSQUERYTIMER,10000);
				}

				void CReasonerManagerThread::threadStopped() {
					CIntervalThread::threadStopped();

					LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Closing Reasoner."),this);

					//QThread::msleep(50);

					delete mCalculationManager;
					delete mPrecomputationManager;
					delete mPreprocessingManager;
					delete mRealizationManager;

					if (unsatCache) {
						unsatCache->stopThread(true);
						delete unsatCache;
					}
					if (mSatExpCache) {
						mSatExpCache->stopThread(true);
						delete mSatExpCache;
					}
					if (mReuseCompGraphCache) {
						mReuseCompGraphCache->stopThread(true);
						delete mReuseCompGraphCache;
					}
					if (mSatNodeExpCache) {
						mSatNodeExpCache->stopThread(true);
						delete mSatNodeExpCache;
					}

					if (mBlockThreadPoolThreadCount > 0) {
						mBlockThreadPoolThreadsBlockingSemaphore.release(mBlockThreadPoolThreadCount);
						mBlockThreadPoolThreadsReleasingSemaphore.acquire(mBlockThreadPoolThreadCount);
					}



					LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Reasoner closed."),this);

				}


				void CReasonerManagerThread::logQueryProgressUpdates() {
				}



				bool CReasonerManagerThread::processTimer(qint64 timerID) {
					if (timerID == PROGRESSQUERYTIMER) {
						logQueryProgressUpdates();
						return true;
					}
					return false;
				}



				const QList<COntologyRequirementPair> CReasonerManagerThread::getRequirementsForQuery(CQuery* query) {
					QList<COntologyRequirementPair> ontReqList;
					CSatisfiableCalculationJobsQuery* jobQuery = dynamic_cast<CSatisfiableCalculationJobsQuery*>(query);
					if (jobQuery) {
						QList<COntologyProcessingRequirement*> reqList;
						reqList.append(mRequirementExpander->getRequiredIndividualPrecomputationOntologyProcessingStepRequirement());
						CQUERYLIST<CCalculationJob*> jobList(jobQuery->getJobList());
						foreach (CCalculationJob* calcJob, jobList) {
							CConcreteOntology* ontology = calcJob->getOntology();

							QList<COntologyProcessingRequirement*> ontSpecReqList = mRequirementExpander->getUnsatisfiedRequirementsExpanded(reqList,ontology);
							foreach (COntologyProcessingRequirement* ontReq, ontSpecReqList) {
								ontReqList.append(COntologyRequirementPair(ontology,ontReq));
							}

							CSatisfiableCalculationJob* satCalcJob = dynamic_cast<CSatisfiableCalculationJob*>(calcJob);
							if (satCalcJob) {
								for (CSatisfiableCalculationConstruct* satCalcConstIt = satCalcJob->getSatisfiableCalculationConstructs(); satCalcConstIt; satCalcConstIt = satCalcConstIt->getNextConstruct()) {
									CSatisfiableCalculationConceptConstruct* satCalcConConstruct = dynamic_cast<CSatisfiableCalculationConceptConstruct*>(satCalcConstIt);
									if (satCalcConConstruct) {
										for (CXSortedNegLinker<CConcept*>* conConstIt = satCalcConConstruct->getConstructConceptLinker(); conConstIt; conConstIt = conConstIt->getNext()) {
											CConcept* concept = conConstIt->getData();
											bool conceptNegation = conConstIt->isNegated();
											COntologyProcessingConceptSaturationRequirement* conSatReq = new COntologyProcessingConceptSaturationRequirement(concept,conceptNegation);
											ontReqList.append(COntologyRequirementPair(ontology,conSatReq));
										}
									}
								}
							}

						}
					}
					CConsistencePremisingQuery* consQuery = dynamic_cast<CConsistencePremisingQuery*>(query);
					if (consQuery) {
						CConcreteOntology* ontology = consQuery->getOntology();
						QList<COntologyProcessingRequirement*> reqList;
						reqList.append(mRequirementExpander->getCompletedDefaultOntologyProcessingStepRequirement(COntologyProcessingStep::OPSCONSISTENCY));
						reqList = mRequirementExpander->getUnsatisfiedRequirementsExpanded(reqList,ontology);
						foreach (COntologyProcessingRequirement* ontReq, reqList) {
							ontReqList.append(COntologyRequirementPair(ontology,ontReq));
						}
					}
					CTaxonomyPremisingQuery* taxQuery = dynamic_cast<CTaxonomyPremisingQuery*>(query);
					if (taxQuery) {
						CConcreteOntology* ontology = taxQuery->getOntology();
						QList<COntologyProcessingRequirement*> reqList;
						reqList.append(mRequirementExpander->getCompletedDefaultOntologyProcessingStepRequirement(COntologyProcessingStep::OPSCLASSCLASSIFY));
						reqList = mRequirementExpander->getUnsatisfiedRequirementsExpanded(reqList,ontology);
						foreach (COntologyProcessingRequirement* ontReq, reqList) {
							ontReqList.append(COntologyRequirementPair(ontology,ontReq));
						}
					}
					CClassificationPremisingQuery* classQuery = dynamic_cast<CClassificationPremisingQuery*>(query);
					if (classQuery) {
						CConcreteOntology* ontology = classQuery->getOntology();
						QList<COntologyProcessingRequirement*> reqList;
						if (classQuery->isConceptClassificationRequired()) {
							reqList.append(mRequirementExpander->getCompletedDefaultOntologyProcessingStepRequirement(COntologyProcessingStep::OPSCLASSCLASSIFY));
						}
						if (classQuery->isObjectRoleClassificationRequired()) {
							reqList.append(mRequirementExpander->getCompletedDefaultOntologyProcessingStepRequirement(COntologyProcessingStep::OPSOBJECTROPERTYCLASSIFY));
						}
						if (classQuery->isDataRoleClassificationRequired()) {
							reqList.append(mRequirementExpander->getCompletedDefaultOntologyProcessingStepRequirement(COntologyProcessingStep::OPSDATAROPERTYCLASSIFY));
						}
						reqList = mRequirementExpander->getUnsatisfiedRequirementsExpanded(reqList,ontology);
						foreach (COntologyProcessingRequirement* ontReq, reqList) {
							ontReqList.append(COntologyRequirementPair(ontology,ontReq));
						}
					}
					CRealizationPremisingQuery* realQuery = dynamic_cast<CRealizationPremisingQuery*>(query);
					if (realQuery) {
						CConcreteOntology* ontology = realQuery->getOntology();
						QList<COntologyProcessingRequirement*> reqList;
						if (realQuery->isDynamicRealisationRequired()) {
							reqList.append(mRequirementExpander->getCompletedDefaultOntologyProcessingStepRequirement(COntologyProcessingStep::OPSCLASSCLASSIFY));
							reqList.append(mRequirementExpander->getCompletedDefaultOntologyProcessingStepRequirement(COntologyProcessingStep::OPSOBJECTROPERTYCLASSIFY));
							reqList.append(realQuery->getDynamicRealizationRequirement());
						} else {
							if (realQuery->isSameIndividualRealisationRequired()) {
								reqList.append(mRequirementExpander->getCompletedDefaultOntologyProcessingStepRequirement(COntologyProcessingStep::OPSSAMEINDIVIDUALSREALIZE));
							}
							if (realQuery->isConceptRealisationRequired()) {
								reqList.append(mRequirementExpander->getCompletedDefaultOntologyProcessingStepRequirement(COntologyProcessingStep::OPSCONCEPTREALIZE));
							}
							if (realQuery->isRoleRealisationRequired()) {
								reqList.append(mRequirementExpander->getCompletedDefaultOntologyProcessingStepRequirement(COntologyProcessingStep::OPSROLEREALIZE));
							}
						}
						reqList = mRequirementExpander->getUnsatisfiedRequirementsExpanded(reqList,ontology);
						foreach (COntologyProcessingRequirement* ontReq, reqList) {
							ontReqList.append(COntologyRequirementPair(ontology,ontReq));
						}
					}
					CComplexAnsweringQuery* complexAnsweringQuery = dynamic_cast<CComplexAnsweringQuery*>(query);
					if (complexAnsweringQuery) {
						CConcreteOntology* ontology = complexAnsweringQuery->getBaseOntology();
						QList<COntologyProcessingRequirement*> reqList;
						reqList.append(mRequirementExpander->getCompletedDefaultOntologyProcessingStepRequirement(COntologyProcessingStep::OPSANSWERCOMPLEXQUERY));

						CComplexConceptAnsweringQuery* complexConceptAnsweringQuery = dynamic_cast<CComplexConceptAnsweringQuery*>(complexAnsweringQuery);
						if (complexConceptAnsweringQuery) {
							if (complexConceptAnsweringQuery->isSuperClassNodesComputationRequired() || complexConceptAnsweringQuery->isSubClassNodesComputationRequired() || complexConceptAnsweringQuery->isEquivalentClassNodesComputationRequired() || complexConceptAnsweringQuery->isInstancesComputationRequired()) {
								reqList.append(mRequirementExpander->getCompletedDefaultOntologyProcessingStepRequirement(COntologyProcessingStep::OPSCLASSCLASSIFY));
							}
							if (complexConceptAnsweringQuery->isInstancesComputationRequired()) {
								reqList.append(mRequirementExpander->getCompletedDefaultOntologyProcessingStepRequirement(COntologyProcessingStep::OPSOBJECTROPERTYCLASSIFY));
								//reqList.append(mRequirementExpander->getCompletedDefaultOntologyProcessingStepRequirement(COntologyProcessingStep::OPSSAMEINDIVIDUALSREALIZE));
							}
						}
						CComplexVariablesAnsweringQuery* compVarAnsweringQuery = dynamic_cast<CComplexVariablesAnsweringQuery*>(complexAnsweringQuery);
						if (compVarAnsweringQuery) {
							reqList.append(mRequirementExpander->getCompletedDefaultOntologyProcessingStepRequirement(COntologyProcessingStep::OPSOBJECTROPERTYCLASSIFY));
							reqList.append(mRequirementExpander->getCompletedDefaultOntologyProcessingStepRequirement(COntologyProcessingStep::OPSCLASSCLASSIFY));
						}
						CComplexAxiomsDataPropertyVariablesAnsweringQuery* compDataPropVarAnsweringQuery = dynamic_cast<CComplexAxiomsDataPropertyVariablesAnsweringQuery*>(complexAnsweringQuery);
						if (compDataPropVarAnsweringQuery) {
							reqList.append(mRequirementExpander->getCompletedDefaultOntologyProcessingStepRequirement(COntologyProcessingStep::OPSDATAROPERTYCLASSIFY));
						}

						//reqList.append(mRequirementExpander->getCompletedDefaultOntologyProcessingStepRequirement(COntologyProcessingStep::OPSCLASSCLASSIFY));
						//reqList.append(mRequirementExpander->getCompletedDefaultOntologyProcessingStepRequirement(COntologyProcessingStep::OPSOBJECTROPERTYCLASSIFY));
						reqList = mRequirementExpander->getUnsatisfiedRequirementsExpanded(reqList, ontology);
						foreach(COntologyProcessingRequirement* ontReq, reqList) {
							ontReqList.append(COntologyRequirementPair(ontology, ontReq));
						}
					}
					return ontReqList;
				}



				void CReasonerManagerThread::continueRequirementProcessing(CRequirementPreparingData* reqData, CConcreteOntology* ontology) {
					COntologyRequirementPreparingData* ontReqPrepData = reqData->getOntologyRequirementPreparingData(ontology);
					if (!ontReqPrepData->mCheckingReqList.isEmpty()) {

						cint64 checkingTime = ontReqPrepData->mCheckingTimer.elapsed();
						if (ontReqPrepData->mCheckingProcessorType == COntologyProcessingStep::OPPREPROCESSOR) {
							if (!mProcessingEndMessageSet.contains(QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE>(ontology,COntologyProcessingStep::OPPREPROCESSOR))) {
								mProcessingEndMessageSet.insert(QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE>(ontology,COntologyProcessingStep::OPPREPROCESSOR));
								LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Finished preprocessing in %2 ms for ontology '%1'.").arg(ontology->getOntologyName()).arg(checkingTime),this);
							}
						} else if (ontReqPrepData->mCheckingProcessorType == COntologyProcessingStep::OPPRECOMPUTER) {
							if (!mProcessingEndMessageSet.contains(QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE>(ontology,COntologyProcessingStep::OPPRECOMPUTER))) {
								mProcessingEndMessageSet.insert(QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE>(ontology,COntologyProcessingStep::OPPRECOMPUTER));
								LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Finished precomputing in %2 ms for ontology '%1'.").arg(ontology->getOntologyName()).arg(checkingTime),this);
							}
						} else if (ontReqPrepData->mCheckingProcessorType == COntologyProcessingStep::OPCLASSCLASSIFIER) {
							if (!mProcessingEndMessageSet.contains(QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE>(ontology,COntologyProcessingStep::OPCLASSCLASSIFIER))) {
								mProcessingEndMessageSet.insert(QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE>(ontology,COntologyProcessingStep::OPCLASSCLASSIFIER));
								LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Finished class classification in %2 ms for ontology '%1'.").arg(ontology->getOntologyName()).arg(checkingTime),this);
							}
						} else if (ontReqPrepData->mCheckingProcessorType == COntologyProcessingStep::OPOBJECTPROPERTYCLASSIFIER) {
							if (!mProcessingEndMessageSet.contains(QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE>(ontology,COntologyProcessingStep::OPOBJECTPROPERTYCLASSIFIER))) {
								mProcessingEndMessageSet.insert(QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE>(ontology,COntologyProcessingStep::OPOBJECTPROPERTYCLASSIFIER));
								LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Finished object property classification in %2 ms for ontology '%1'.").arg(ontology->getOntologyName()).arg(checkingTime),this);
							}
						} else if (ontReqPrepData->mCheckingProcessorType == COntologyProcessingStep::OPDATAPROPERTYCLASSIFIER) {
							if (!mProcessingEndMessageSet.contains(QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE>(ontology,COntologyProcessingStep::OPDATAPROPERTYCLASSIFIER))) {
								mProcessingEndMessageSet.insert(QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE>(ontology,COntologyProcessingStep::OPDATAPROPERTYCLASSIFIER));
								LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Finished data property classification in %2 ms for ontology '%1'.").arg(ontology->getOntologyName()).arg(checkingTime),this);
							}
						} else if (ontReqPrepData->mCheckingProcessorType == COntologyProcessingStep::OPREALIZER) {
							if (!mProcessingEndMessageSet.contains(QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE>(ontology,COntologyProcessingStep::OPREALIZER))) {
								mProcessingEndMessageSet.insert(QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE>(ontology,COntologyProcessingStep::OPREALIZER));
								LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Finished (lazy) realization in %2 ms for ontology '%1'.").arg(ontology->getOntologyName()).arg(checkingTime),this);
							}
						}


						for (QList<COntologyProcessingRequirement*>::const_iterator it = ontReqPrepData->mCheckingReqList.constBegin(), itEnd = ontReqPrepData->mCheckingReqList.constEnd(); it != itEnd; ++it) {
							COntologyProcessingRequirement* ontProReq(*it);
							if (!ontProReq->isRequirementSatisfied(ontology)) {
								ontReqPrepData->mFailedReqList.append(ontProReq);
							} else {
								reqData->mSatisfiedReqList.append(COntologyRequirementPair(ontology,ontProReq));
							}
						}
						ontReqPrepData->mCheckingReqList.clear();
					}
					if (!ontReqPrepData->mFailedReqList.isEmpty()) {
						for (QList<COntologyProcessingRequirement*>::const_iterator it = ontReqPrepData->mFailedReqList.constBegin(), itEnd = ontReqPrepData->mFailedReqList.constEnd(); it != itEnd; ++it) {
							COntologyProcessingRequirement* failedReqData(*it);			
							const QString& failureString = failedReqData->getRequirementFailureString(ontology);
							if (!failureString.isEmpty()) {
								LOG(ERROR,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("%1").arg(failureString),this);
							}
							reqData->mFailedReqList.append(COntologyRequirementPair(ontology,failedReqData));
						}
						ontReqPrepData->mFailedReqList.clear();
						ontReqPrepData->mRequirementsProcessed = true;
						reqData->mDepCount--;
						reqData->mRequirementFailed = true;
						LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Requirements for ontology '%1' not satisfied.").arg(ontology->getOntologyName()),this);
					}

					if (!reqData->mRequirementFailed) {
						if (!ontReqPrepData->mPreprocessorReqList.isEmpty()) {
							if (!mProcessingStartMessageSet.contains(QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE>(ontology,COntologyProcessingStep::OPPREPROCESSOR))) {
								mProcessingStartMessageSet.insert(QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE>(ontology,COntologyProcessingStep::OPPREPROCESSOR));
								LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Preprocessing ontology '%1'.").arg(ontology->getOntologyName()),this);
							}
							CConfigurationBase* config = ontology->getConfiguration();
							CPreprocessor* preprocessor = mPreprocessingManager->getPreprocessor(ontology,config);
							CRequirementProcessedCallbackEvent* reqProcCallbackEvent = new CRequirementProcessedCallbackEvent(this,ontology,reqData, COntologyProcessingStep::OPPREPROCESSOR);
							preprocessor->preprocess(ontology,config,ontReqPrepData->mPreprocessorReqList,reqProcCallbackEvent);
							ontReqPrepData->mCheckingReqList = ontReqPrepData->mPreprocessorReqList;
							ontReqPrepData->mPreprocessorReqList.clear();
							ontReqPrepData->mCheckingProcessorType = COntologyProcessingStep::OPPREPROCESSOR;
							ontReqPrepData->mCheckingTimer.start();
						} else if (!ontReqPrepData->mPrecomputorReqList.isEmpty()) {
							QString exprString = QString(", expressiveness '%2'").arg(ontology->getStructureSummary()->getExpressivenessString());
							if (!mProcessingStartMessageSet.contains(QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE>(ontology,COntologyProcessingStep::OPPRECOMPUTER))) {
								mProcessingStartMessageSet.insert(QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE>(ontology,COntologyProcessingStep::OPPRECOMPUTER));
								LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Precomputing ontology '%1'%2.").arg(ontology->getOntologyName()).arg(exprString),this);
							}
							CConfigurationBase* config = ontology->getConfiguration();
							CPrecomputator* precomputer = mPrecomputationManager->getPrecomputator(ontology,config);
							CRequirementProcessedCallbackEvent* reqProcCallbackEvent = new CRequirementProcessedCallbackEvent(this,ontology,reqData, COntologyProcessingStep::OPPRECOMPUTER);
							precomputer->precompute(ontology,config,ontReqPrepData->mPrecomputorReqList,reqProcCallbackEvent);
							ontReqPrepData->mCheckingReqList = ontReqPrepData->mPrecomputorReqList;
							ontReqPrepData->mCheckingProcessorType = COntologyProcessingStep::OPPRECOMPUTER;
							ontReqPrepData->mCheckingTimer.start();
							ontReqPrepData->mPrecomputorReqList.clear();
						} else if (!ontReqPrepData->mClassClassifierReqList.isEmpty()) {
							QString exprString = QString(", expressiveness '%2'").arg(ontology->getStructureSummary()->getExpressivenessString());
							if (!mProcessingStartMessageSet.contains(QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE>(ontology,COntologyProcessingStep::OPCLASSCLASSIFIER))) {
								mProcessingStartMessageSet.insert(QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE>(ontology,COntologyProcessingStep::OPCLASSCLASSIFIER));
								LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Classifying ontology '%1'%2.").arg(ontology->getOntologyName()).arg(exprString),this);
							}
							CConfigurationBase* config = ontology->getConfiguration();
							CSubsumptionClassifier* classClassifier = classificationMan->getClassClassifier(ontology,config);
							CRequirementProcessedCallbackEvent* reqProcCallbackEvent = new CRequirementProcessedCallbackEvent(this,ontology,reqData, COntologyProcessingStep::OPCLASSCLASSIFIER);
							classClassifier->classify(ontology,config,ontReqPrepData->mClassClassifierReqList,reqProcCallbackEvent);
							ontReqPrepData->mCheckingReqList = ontReqPrepData->mClassClassifierReqList;
							ontReqPrepData->mCheckingProcessorType = COntologyProcessingStep::OPCLASSCLASSIFIER;
							ontReqPrepData->mCheckingTimer.start();
							ontReqPrepData->mClassClassifierReqList.clear();
						} else if (!ontReqPrepData->mObjectPropertyClassifierReqList.isEmpty()) {
							QString exprString = QString(", expressiveness '%2'").arg(ontology->getStructureSummary()->getExpressivenessString());
							if (!mProcessingStartMessageSet.contains(QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE>(ontology,COntologyProcessingStep::OPOBJECTPROPERTYCLASSIFIER))) {
								mProcessingStartMessageSet.insert(QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE>(ontology,COntologyProcessingStep::OPOBJECTPROPERTYCLASSIFIER));
								LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Classifying object properties for ontology '%1'%2.").arg(ontology->getOntologyName()).arg(exprString),this);
							}
							CConfigurationBase* config = ontology->getConfiguration();
							CSubsumptionClassifier* objectPropertyClassifier = classificationMan->getObjectPropertyClassifier(ontology,config);
							CRequirementProcessedCallbackEvent* reqProcCallbackEvent = new CRequirementProcessedCallbackEvent(this,ontology,reqData, COntologyProcessingStep::OPOBJECTPROPERTYCLASSIFIER);
							objectPropertyClassifier->classify(ontology,config,ontReqPrepData->mObjectPropertyClassifierReqList,reqProcCallbackEvent);
							ontReqPrepData->mCheckingReqList = ontReqPrepData->mObjectPropertyClassifierReqList;
							ontReqPrepData->mCheckingProcessorType = COntologyProcessingStep::OPOBJECTPROPERTYCLASSIFIER;
							ontReqPrepData->mCheckingTimer.start();
							ontReqPrepData->mObjectPropertyClassifierReqList.clear();
						} else if (!ontReqPrepData->mDataPropertyClassifierReqList.isEmpty()) {
							QString exprString = QString(", expressiveness '%2'").arg(ontology->getStructureSummary()->getExpressivenessString());
							if (!mProcessingStartMessageSet.contains(QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE>(ontology,COntologyProcessingStep::OPDATAPROPERTYCLASSIFIER))) {
								mProcessingStartMessageSet.insert(QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE>(ontology,COntologyProcessingStep::OPDATAPROPERTYCLASSIFIER));
								LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Classifying data properties for ontology '%1'%2.").arg(ontology->getOntologyName()).arg(exprString),this);
							}
							CConfigurationBase* config = ontology->getConfiguration();
							CSubsumptionClassifier* dataPropertyClassifier = classificationMan->getDataPropertyClassifier(ontology,config);
							CRequirementProcessedCallbackEvent* reqProcCallbackEvent = new CRequirementProcessedCallbackEvent(this,ontology,reqData, COntologyProcessingStep::OPDATAPROPERTYCLASSIFIER);
							dataPropertyClassifier->classify(ontology,config,ontReqPrepData->mDataPropertyClassifierReqList,reqProcCallbackEvent);
							ontReqPrepData->mCheckingReqList = ontReqPrepData->mDataPropertyClassifierReqList;
							ontReqPrepData->mCheckingProcessorType = COntologyProcessingStep::OPDATAPROPERTYCLASSIFIER;
							ontReqPrepData->mCheckingTimer.start();
							ontReqPrepData->mDataPropertyClassifierReqList.clear();
						} else if (!ontReqPrepData->mRealizerReqList.isEmpty()) {
							QString exprString = QString(", expressiveness '%2'").arg(ontology->getStructureSummary()->getExpressivenessString());
							if (!mProcessingStartMessageSet.contains(QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE>(ontology,COntologyProcessingStep::OPREALIZER))) {
								mProcessingStartMessageSet.insert(QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE>(ontology,COntologyProcessingStep::OPREALIZER));
								LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Realizing ontology '%1'%2.").arg(ontology->getOntologyName()).arg(exprString),this);
							}
							CConfigurationBase* config = ontology->getConfiguration();
							CRealizer* realizer = mRealizationManager->getRealizer(ontology,config);
							CRequirementProcessedCallbackEvent* reqProcCallbackEvent = new CRequirementProcessedCallbackEvent(this,ontology,reqData, COntologyProcessingStep::OPREALIZER);
							realizer->realize(ontology,config,ontReqPrepData->mRealizerReqList,reqProcCallbackEvent);
							ontReqPrepData->mCheckingReqList = ontReqPrepData->mRealizerReqList;
							ontReqPrepData->mCheckingProcessorType = COntologyProcessingStep::OPREALIZER;
							ontReqPrepData->mCheckingTimer.start();
							ontReqPrepData->mRealizerReqList.clear();
						} else if (!ontReqPrepData->mAnswererReqList.isEmpty()) {
							QString exprString = QString(", expressiveness '%2'").arg(ontology->getStructureSummary()->getExpressivenessString());
							if (!mProcessingStartMessageSet.contains(QPair<CConcreteOntology*, COntologyProcessingStep::PROCESSORTYPE>(ontology, COntologyProcessingStep::OPANSWERER))) {
								mProcessingStartMessageSet.insert(QPair<CConcreteOntology*, COntologyProcessingStep::PROCESSORTYPE>(ontology, COntologyProcessingStep::OPANSWERER));
								LOG(INFO, "::Konclude::Reasoner::Kernel::ReasonerManager", logTr("Initializing complex query answering for ontology '%1'%2.").arg(ontology->getOntologyName()).arg(exprString), this);
							}
							CConfigurationBase* config = ontology->getConfiguration();
							bool created = false;
							if (!mAnswererManager) {
								mAnswererManager = new CAnsweringManagerThread(this, config);
								created = true;
							}
							CRequirementProcessedCallbackEvent* reqProcCallbackEvent = new CRequirementProcessedCallbackEvent(this, ontology, reqData, COntologyProcessingStep::OPANSWERER);
							ontology->getProcessingSteps()->getOntologyProcessingStepDataVector()->getProcessingStepData(COntologyProcessingStep::OPSANSWERCOMPLEXQUERY)->getProcessingStatus()->setProcessingFlags(COntologyProcessingStatus::PSCOMPLETELYYPROCESSED);
							ontology->getProcessingSteps()->getOntologyProcessingStepDataVector()->getProcessingStepData(COntologyProcessingStep::OPSANSWERCOMPLEXQUERY)->getProcessingStatus()->setErrorFlags(COntologyProcessingStatus::PSSUCESSFULL);
							ontReqPrepData->mCheckingReqList = ontReqPrepData->mAnswererReqList;
							ontReqPrepData->mCheckingProcessorType = COntologyProcessingStep::OPANSWERER;
							ontReqPrepData->mCheckingTimer.start();
							ontReqPrepData->mAnswererReqList.clear();
							if (created) {
								mAnswererManager->prepareAnswering(ontology, reqProcCallbackEvent);
							} else if (!mAnswererPrepared) {
								mAnswererPreparedCallbackQueue.append(reqProcCallbackEvent);
							} else {
								reqProcCallbackEvent->doCallback();
							}
						} else {
							reqData->mDepCount--;
						}
					}

					if (reqData->mDepCount <= 0) {
						if (reqData->mQuery) {
							initiateQueryReasoning(reqData->mQuery,reqData->mCallback,reqData->mReasoningData,reqData->mFailedReqList);
						} else {
							CCallbackData* callback = reqData->mCallback;
							if (callback) {
								callback->doCallback();
							}
							delete reqData;
						}
					}
				}


				void CReasonerManagerThread::prepareOntologyReasoning(CPrepareOntologyEvent* poe) {
					CConcreteOntology* ontology = poe->getOntology();
					CCallbackData* callbackData = poe->getCallbackData();
					if (ontology) {
						QList<COntologyProcessingRequirement*> reqList(*poe->getOntologyProcessingRequirementList());
						reqList = mRequirementExpander->getUnsatisfiedRequirementsExpanded(reqList,ontology);
						if (!reqList.isEmpty()) {
							// satisfy query requirements first
							CRequirementPreparingData* reqPrepData = new CRequirementPreparingData(callbackData,nullptr,nullptr);
							for (QList<COntologyProcessingRequirement*>::const_iterator it = reqList.constBegin(), itEnd = reqList.constEnd(); it != itEnd; ++it) {
								COntologyProcessingRequirement* requirement = *it;
								COntologyRequirementPreparingData* ontReqPrepData = reqPrepData->getOntologyRequirementPreparingData(ontology);
								ontReqPrepData->addOntologyRequirement(requirement);
							}
							reqPrepData->mDepCount = reqPrepData->mOntoReqDataHash.count();
							mProcessingRequirementsSet.insert(reqPrepData);

							continueRequirementProcessing(reqPrepData,ontology);
						} else {
							if (callbackData) {
								callbackData->doCallback();
							}
						}
					} else {
						LOG(ERROR,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Ontology not specified for processing requirements."),this);
						if (callbackData) {
							callbackData->doCallback();
						}
					}
				}


				void CReasonerManagerThread::prepareQueryReasoning(CCalcQueryEvent *cqe) {
					CQuery* query = cqe->getQuery();
					CCallbackData* callbackData = cqe->getCallbackData();

					CReasoningTaskData* reasoningData = new CReasoningTaskData();

					QList<COntologyRequirementPair> reqList(getRequirementsForQuery(query));
					if (reqList.isEmpty()) {
						initiateQueryReasoning(query,callbackData,reasoningData,QList<COntologyRequirementPair>());
					} else {
						// satisfy query requirements first
						CRequirementPreparingData* reqPrepData = new CRequirementPreparingData(callbackData,query,reasoningData);
						reasoningData->mReqPrepData = reqPrepData;
						for (QList<COntologyRequirementPair>::const_iterator it = reqList.constBegin(), itEnd = reqList.constEnd(); it != itEnd; ++it) {
							COntologyRequirementPair ontReqPair(*it);
							CConcreteOntology* ontology = ontReqPair.mOntology;
							COntologyProcessingRequirement* requirement = ontReqPair.mRequirement;
							COntologyRequirementPreparingData* ontReqPrepData = reqPrepData->getOntologyRequirementPreparingData(ontology);
							ontReqPrepData->addOntologyRequirement(requirement);
						}
						reqPrepData->mDepCount = reqPrepData->mOntoReqDataHash.count();
						mProcessingRequirementsSet.insert(reqPrepData);

						if (reqPrepData->mDepCount <= 0) {
							initiateQueryReasoning(query,callbackData,reasoningData,reqPrepData->mFailedReqList);
						} else {
							for (QHash<CConcreteOntology*,COntologyRequirementPreparingData*>::const_iterator it = reqPrepData->mOntoReqDataHash.constBegin(), itEnd = reqPrepData->mOntoReqDataHash.constEnd(); it != itEnd; ++it) {
								CConcreteOntology* ontology = it.key();
								COntologyRequirementPreparingData* ontReqRepData = it.value();
								continueRequirementProcessing(reqPrepData,ontology);
							}
						}

					}

				}


				void CReasonerManagerThread::initiateQueryReasoning(CQuery* query, CCallbackData* callbackData, CReasoningTaskData* reasoningData, const QList<COntologyRequirementPair>& failedRequirementList) {
					reasoningData->mStartTime.start();
					updateBeginingCalculationStatistics(reasoningData);

					if (!failedRequirementList.isEmpty()) {
						CConcreteOntology* ontology = nullptr;
						for (QList<COntologyRequirementPair>::const_iterator it = failedRequirementList.constBegin(), itEnd = failedRequirementList.constEnd(); it != itEnd; ++it) {
							ontology = it->mOntology;
							COntologyProcessingRequirement* requirement(it->mRequirement);
							query->addErrorString(QString("Processing requirements for Ontology '%1' for Query '%2' failed.").arg(ontology->getOntologyName()).arg(query->getQueryName()));

							if (ontology) {
								CConsistence* cons = ontology->getConsistence();
								if (cons) {
									if (!cons->isOntologyConsistent()) {
										LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Ontology '%1' for Query '%2' is inconsistent.").arg(ontology->getOntologyName()).arg(query->getQueryName()),this);

										query->addQueryError(new CQueryInconsitentOntologyError(ontology));
									}
								}
							}
						}
						if (ontology) {
							updateFinishingCalculationStatistics(reasoningData,query->getQueryStatistics(),ontology->getConfiguration());
						} else {
							updateFinishingCalculationStatistics(reasoningData,query->getQueryStatistics());
						}

						CCallbackData* callback = callbackData;
						if (callback) {
							callback->doCallback();
						}
						delete reasoningData;

					} else {


						CSatisfiableCalculationJobsQuery* jobQuery = dynamic_cast<CSatisfiableCalculationJobsQuery*>(query);
						if (jobQuery) {


							CQUERYLIST<CCalculationJob*> jobList(jobQuery->getJobList());
							bool jobCalculated = false;
							cint64 depCount = 0;
							CConcreteOntology* ontology = nullptr;
							FOREACHIT (CCalculationJob* calcJob, jobList) {
								CSatisfiableCalculationJob* satCalcJob = dynamic_cast<CSatisfiableCalculationJob*>(calcJob);
								if (satCalcJob) {
									depCount++;
									CJobSatisfiableCallbackContextData* satCallbackData = new CJobSatisfiableCallbackContextData(satCalcJob);
									CQueryJobCalculatedSatisfiableCallbackEvent* callback = new CQueryJobCalculatedSatisfiableCallbackEvent(this,jobQuery,satCallbackData);
									mCalculationManager->calculateJob(satCalcJob,callback);
									mCalcJobSet.insert(satCalcJob);
									jobCalculated = true;

									ontology = satCalcJob->getOntology();
								}
							}

							reasoningData->mCallback = callbackData;
							reasoningData->mDepCount = depCount;
							mReasoningTaskDataHash.insert((cint64)query,reasoningData);
							mCalculatingQuerySet.insert(query);

							reasoningData->mFinQueryCallback = new CCalcedQueryEvent(this,jobQuery);
							LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Processing Query '%1'.").arg(query->getQueryName()),this);



						} 



						CIsTriviallyConsistentQuery* trivConsQuery = dynamic_cast<CIsTriviallyConsistentQuery *>(query);
						if (trivConsQuery) {
							reasoningData->mCallback = callbackData;
							CConcreteOntology* ontology = trivConsQuery->getOntology();
							trivConsQuery->constructResult(ontology->getBuildData());

							qint64 mSecs = reasoningData->mStartTime.elapsed();
							LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Query '%1' processed in '%2' ms.").arg(trivConsQuery->getQueryName()).arg(mSecs),this);

							CCallbackData* callback = reasoningData->mCallback;
							if (callback) {
								callback->doCallback();
							}
							delete reasoningData;
						}

						CConsistencePremisingQuery* consQuery = dynamic_cast<CConsistencePremisingQuery *>(query);
						if (consQuery) {
							reasoningData->mCallback = callbackData;
							CQueryStatistics* queryStats = consQuery->getQueryStatistics();
							CConcreteOntology* ontology = consQuery->getOntology();
							if (queryStats) {
								CConsistenceStatistics* consStats = ontology->getConsistence()->getConsistenceStatistics();
								if (consStats) {
									foreach(QString statName, consStats->getStatisticsNameStringList()) {
										cint64 statValue = consStats->getStatisticIntegerValue(statName);
										queryStats->addProcessingStatistics(statName, statValue);
									}
								}
							}
							updateFinishingCalculationStatistics(reasoningData,queryStats,ontology->getConfiguration());

							consQuery->constructResult(ontology->getConsistence());

							qint64 mSecs = reasoningData->mStartTime.elapsed();
							LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Query '%1' processed in '%2' ms.").arg(consQuery->getQueryName()).arg(mSecs),this);

							CCallbackData* callback = reasoningData->mCallback;
							if (callback) {
								callback->doCallback();
							}
							delete reasoningData;
						}


						CTaxonomyPremisingQuery *taxQuery = dynamic_cast<CTaxonomyPremisingQuery *>(query);
						if (taxQuery) {
							reasoningData->mCallback = callbackData;

							CQueryStatistics* queryStats = taxQuery->getQueryStatistics();
							CConcreteOntology* ontology = taxQuery->getOntology();
							if (queryStats) {
								CClassConceptClassification* classConClassif = ontology->getClassification()->getClassConceptClassification();
								if (classConClassif) {
									CClassificationStatistics* classifStats = classConClassif->getClassificationStatistics();
									if (classifStats) {
										foreach(QString statName, classifStats->getStatisticsNameStringList()) {
											cint64 statValue = classifStats->getStatisticIntegerValue(statName);
											queryStats->addProcessingStatistics(statName, statValue);
										}
									}
								}
							}
							updateFinishingCalculationStatistics(reasoningData,queryStats,ontology->getConfiguration());

							taxQuery->constructResult(ontology->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy());

							qint64 mSecs = reasoningData->mStartTime.elapsed();
							LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Query '%1' processed in '%2' ms.").arg(taxQuery->getQueryName()).arg(mSecs),this);


							CCallbackData* callback = reasoningData->mCallback;
							if (callback) {
								callback->doCallback();
							}
							delete reasoningData;
						}


						CClassificationPremisingQuery* classQuery = dynamic_cast<CClassificationPremisingQuery*>(query);
						if (classQuery) {
							reasoningData->mCallback = callbackData;

							CQueryStatistics* queryStats = classQuery->getQueryStatistics();
							CConcreteOntology* ontology = classQuery->getOntology();

							updateFinishingCalculationStatistics(reasoningData,queryStats,ontology->getConfiguration());

							classQuery->constructResult(ontology->getClassification());

							qint64 mSecs = reasoningData->mStartTime.elapsed();
							LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Query '%1' processed in '%2' ms.").arg(classQuery->getQueryName()).arg(mSecs),this);

							CCallbackData* callback = reasoningData->mCallback;
							if (callback) {
								callback->doCallback();
							}
							delete reasoningData;
						}


						CRealizationPremisingQuery* realQuery = dynamic_cast<CRealizationPremisingQuery *>(query);
						if (realQuery) {
							reasoningData->mCallback = callbackData;

							CQueryStatistics* queryStats = realQuery->getQueryStatistics();
							CConcreteOntology* ontology = realQuery->getOntology();

							updateFinishingCalculationStatistics(reasoningData,queryStats,ontology->getConfiguration());

							realQuery->constructResult(ontology->getRealization());

							qint64 mSecs = reasoningData->mStartTime.elapsed();
							LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Query '%1' processed in '%2' ms.").arg(realQuery->getQueryName()).arg(mSecs),this);


							CCallbackData* callback = reasoningData->mCallback;
							if (callback) {
								callback->doCallback();
							}
							delete reasoningData;
						}


						CComplexAnsweringQuery* compAnsQuery = dynamic_cast<CComplexAnsweringQuery *>(query);
						if (compAnsQuery) {
							reasoningData->mCallback = callbackData;

							CConcreteOntology* ontology = compAnsQuery->getBaseOntology();							

							CCallbackData* callback = reasoningData->mCallback;

							bool confCollectProcessStatistics = CConfigDataReader::readConfigBoolean(ontology->getConfiguration(), "Konclude.Calculation.Answering.CollectProcessStatistics", false);
							if (confCollectProcessStatistics) {
								reasoningData->mFinQueryCallback = new CCalcedQueryEvent(this, compAnsQuery);
								mReasoningTaskDataHash.insert((cint64)query, reasoningData);
								callback = reasoningData->mFinQueryCallback;
							}

							mAnswererManager->answerComplexQuery(compAnsQuery, callback);
							if (!confCollectProcessStatistics) {
								delete reasoningData;
							}
						}
					}
				}




				void CReasonerManagerThread::prepareSatisfiableBoxReasoning(CReasoningSatisfiableCalculationJobEvent *cbse) {

					CSatisfiableCalculationJob* calcJob = cbse->getSatisfiableCalculationJob();
					CCallbackData* callbackData = cbse->getCallbackData();


					CJobSatisfiableCallbackContextData* satCallbackData = (CJobSatisfiableCallbackContextData*)callbackData->getCallbackDataContext();
					if (!satCallbackData) {
						satCallbackData = new CJobSatisfiableCallbackContextData(calcJob);
					}
					CJobCalculatedSatisfiableCallbackEvent* callback = new CJobCalculatedSatisfiableCallbackEvent(this,satCallbackData);
					mCalculationManager->calculateJob(calcJob,callback);
					mCalcJobSet.insert(calcJob);

					CReasoningTaskData* reasoningData = new CReasoningTaskData();
					reasoningData->mCallback = callbackData;
					mReasoningTaskDataHash.insert((cint64)calcJob,reasoningData);
				}



				bool CReasonerManagerThread::updateBeginingCalculationStatistics(CReasoningTaskData* reaTaskData, CConfiguration* config) {
					if (!config) {
						config = configProvider->getCurrentConfiguration();
					}
					if (CConfigDataReader::readConfigBoolean(config,"Konclude.Query.Statistics.CollectCalculationStatistics",false)) {
						QHash<QString,cint64>* mBeginStatHash = mCalculationManager->getCalculationStatistics();
						reaTaskData->mCalcStat = mBeginStatHash;
						return true;
					}
					return false;
				}



				bool CReasonerManagerThread::updateFinishingCalculationStatistics(CReasoningTaskData* reaTaskData, CQueryStatistics* queryStat, CConfiguration* config) {
					if (queryStat) {		
						if (!config) {
							config = configProvider->getCurrentConfiguration();
						}
						if (CConfigDataReader::readConfigBoolean(config,"Konclude.Query.Statistics.CollectCalculationStatistics",false)) {
							QHash<QString,cint64>* mBeginStatHash = reaTaskData->mCalcStat;
							if (mBeginStatHash) {
								QHash<QString,cint64>* mFinishStatHash = mCalculationManager->getCalculationStatistics();
								for (QHash<QString,cint64>::const_iterator itF = mFinishStatHash->constBegin(), itFEnd = mFinishStatHash->constEnd(); itF != itFEnd; ++itF) {
									QString statName = itF.key();
									cint64 finVal = itF.value();
									cint64 begVal = 0;//mBeginStatHash->value(statName);
									queryStat->addProcessingStatistics(statName,finVal-begVal);
								}
								delete mFinishStatHash;
							}
							queryStat->addProcessingStatistics("calculation-reasoning-time",reaTaskData->mStartTime.elapsed());
						}
						if (CConfigDataReader::readConfigBoolean(config,"Konclude.Query.Statistics.CollectProcessingStepsStatistics",false) && reaTaskData->mReqPrepData) {
							QSet<COntologyProcessingStatistics*> addedStatSet;
							for (QList<COntologyRequirementPair>::const_iterator it = reaTaskData->mReqPrepData->mSatisfiedReqList.constBegin(), itEnd = reaTaskData->mReqPrepData->mSatisfiedReqList.constEnd(); it != itEnd; ++it) {
								COntologyRequirementPair ontReqPair(*it);
								CConcreteOntology* ontology = ontReqPair.mOntology;
								COntologyProcessingRequirement* requirement = ontReqPair.mRequirement;
								COntologyProcessingStepRequirement* ontProcStepRequirement = dynamic_cast<COntologyProcessingStepRequirement*>(requirement);
								if (ontProcStepRequirement) {								
									COntologyProcessingStepData* procStepData = ontology->getProcessingSteps()->getOntologyProcessingStepDataVector()->getProcessingStepData(ontProcStepRequirement->getRequiredProcessingStep()->getOntologyProcessingType());
									if (procStepData) {
										COntologyProcessingStatistics* procStepStatistics = procStepData->getProcessingStatistics(false);
										if (procStepStatistics && !addedStatSet.contains(procStepStatistics)) {
											addedStatSet.insert(procStepStatistics);
											queryStat->addProcessingStatistics(procStepStatistics);
										}
									}
								}
							}
						}
						return true;
					}
					return false;
				}







				void CReasonerManagerThread::finishSatisfiableBoxReasoning(CJobCalculatedSatisfiableCallbackEvent *cbsce) {
					CJobSatisfiableCallbackContextData *contextData = (CJobSatisfiableCallbackContextData *)cbsce->getCallbackDataContext();

					CSatisfiableCalculationJob* satCalcJob = contextData->getSatisfiableCalculationJob();
					bool satisfiable = contextData->isSatisfiable();
					bool calcError = contextData->hasCalculationError();

					if (calcError) {
						LOG(ERROR,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Error while calculating job."),this);
						QString errorString = mCalculationManager->getCalculationErrorString(contextData->getCalculationErrorCode());
						if (!errorString.isEmpty()) {
							LOG(ERROR,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Calculation Error: '%1'.").arg(errorString),this);
						}
					}

					CReasoningTaskData* reasoningData = mReasoningTaskDataHash.value((cint64)satCalcJob,nullptr);
					reasoningData->mDepCount--;
					CCallbackData* callback = reasoningData->mCallback;
					if (callback) {
						callback->setCallbackDataContext(cbsce->takeCallbackDataContext());
						callback->doCallback();
					}
					mReasoningTaskDataHash.remove((cint64)satCalcJob);
					delete reasoningData;
					mCalcJobSet.remove(satCalcJob);
				}


				QString CReasonerManagerThread::getLoggingCalculationStatisticsMemorySizeString(cint64 memSize) {
					QString memString;
					if (memSize > 10*1024*1024) {
						memString = QString("%1 MByte").arg(memSize/(1024*1024));
					} else if (memSize > 10*1024) {
						memString = QString("%1 KByte").arg(memSize/(1024));
					} else {
						memString = QString("%1 Byte").arg(memSize);
					}
					return memString;
				}

				QString CReasonerManagerThread::getLoggingCalculationStatisticsTimeSizeString(double milliseconds) {
					QString compTimeString;
					if (milliseconds > 2*1000*60*60*24*356) {
						compTimeString = QString("%1 y").arg(milliseconds/(1000*60*60*24*356));
					} else if (milliseconds > 2*1000*60*60*24) {
						compTimeString = QString("%1 d").arg(milliseconds/(1000*60*60*24));
					} else if (milliseconds > 10*1000*60*60) {
						compTimeString = QString("%1 h").arg(milliseconds/(1000*60*60));
					} else if (milliseconds > 10*1000*60) {
						compTimeString = QString("%1 m").arg(milliseconds/(1000*60));
					} else if (milliseconds > 10*1000) {
						compTimeString = QString("%1 s").arg(milliseconds/(1000));
					} else {
						compTimeString = QString("%1 ms").arg(milliseconds);
					}
					return compTimeString;
				}

				void CReasonerManagerThread::loggingCalculationStatistics() {
					QHash<QString,cint64>* mCalcStatHash = mCalculationManager->getCalculationStatistics();
					if (mCalcStatHash) {
						QString compTime = getLoggingCalculationStatisticsTimeSizeString(mCalcStatHash->value("calculation-computing-time"));
						QString memCons = getLoggingCalculationStatisticsMemorySizeString(mCalcStatHash->value("calculation-memory-consumption"));
						QString memRes = getLoggingCalculationStatisticsMemorySizeString(mCalcStatHash->value("calculation-memory-reservation"));
						QString statString = QString("Up to now calculation computing time: %1, calculation memory consumption: %2.")
								.arg(compTime)
								.arg(memCons);
						LOG(INFO,"::Konclude::Reasoner::Kernel::ReasonerManager",statString,this);
						delete mCalcStatHash;
					}
				}


				void CReasonerManagerThread::finishQuerySatisfiableBoxReasoning(CQueryJobCalculatedSatisfiableCallbackEvent *cbsce) {
					CJobSatisfiableCallbackContextData *contextData = (CJobSatisfiableCallbackContextData *)cbsce->getCallbackDataContext();
					CSatisfiableCalculationJobsQuery* jobQuery = cbsce->getSatisfiableCalclulationJobsQuery();
					CSatisfiableCalculationJob* satCalcJob = contextData->getSatisfiableCalculationJob();
					bool satisfiable = contextData->isSatisfiable();
					bool calcError = contextData->hasCalculationError();

					if (calcError) {
						LOG(ERROR,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Error while calculating job for query '%1'.").arg(jobQuery->getQueryName()),this);
						QString errorString = mCalculationManager->getCalculationErrorString(contextData->getCalculationErrorCode());
						if (!errorString.isEmpty()) {
							LOG(ERROR,"::Konclude::Reasoner::Kernel::ReasonerManager",logTr("Calculation Error: '%1'.").arg(errorString),this);
							jobQuery->addErrorString("Calculation error: "+errorString);
						}
					}


					CReasoningTaskData* reasoningQueryData = mReasoningTaskDataHash.value((cint64)jobQuery,nullptr);
					updateFinishingCalculationStatistics(reasoningQueryData,jobQuery->getQueryStatistics());
					QString newAnswerString;

					bool queryFinished = false;
					if (!reasoningQueryData->mAnswered) {
						queryFinished = jobQuery->interpreteJobSatisfiableCalculation(satCalcJob,satisfiable,calcError,&newAnswerString);
					}

					CReasoningTaskData* reasoningJobData = mReasoningTaskDataHash.value((cint64)satCalcJob,nullptr);
					if (reasoningJobData) {
						reasoningJobData->mDepCount--;
						CCallbackData* callback = reasoningJobData->mCallback;
						if (callback) {
							callback->setCallbackDataContext(cbsce->takeCallbackDataContext());
							callback->doCallback();
						}
						mReasoningTaskDataHash.remove((cint64)satCalcJob);
						delete reasoningJobData;
					}
					mCalcJobSet.remove(satCalcJob);

					if (--(reasoningQueryData->mDepCount) <= 0) {
						reasoningQueryData->mAnswered |= queryFinished;
						reasoningQueryData->mFinQueryCallback->sendEventWithAnswer(newAnswerString);
					}
				}

				void CReasonerManagerThread::finishQueryReasoning(CCalcedQueryEvent *cqe) {
					CQuery *query = cqe->getQuery();
					CReasoningTaskData* reasoningData = mReasoningTaskDataHash.value((cint64)query,nullptr);

					CComplexAnsweringQuery* compAnsQuery = dynamic_cast<CComplexAnsweringQuery *>(query);
					if (compAnsQuery) {
						CQueryStatistics* queryStats = query->getQueryStatistics();
						if (queryStats) {
							CAnsweringStatistics* stats = mAnswererManager->getAnsweringStatistics(compAnsQuery->getBaseOntology());
							if (stats) {
								for (const QString& statName : stats->getStatisticsNameStringList()) {
									cint64 statValue = stats->getStatisticIntegerValue(statName);
									queryStats->addProcessingStatistics(statName, statValue);
								}
								delete stats;
							}
							updateFinishingCalculationStatistics(reasoningData, queryStats, compAnsQuery->getBaseOntology()->getConfiguration());
						}
					} else {
						LOG(INFO, "::Konclude::Reasoner::Kernel::ReasonerManager", logTr("Query '%1' processed. Question '%2' answered with '%3'.\r\n").arg(cqe->getQueryName()).arg(cqe->getQueryString()).arg(cqe->getAnswerString()), this);

						qint64 mSecs = reasoningData->mStartTime.elapsed();
						LOG(INFO, "::Konclude::Reasoner::Kernel::ReasonerManager", logTr("Query '%1' processed in '%2' ms.").arg(cqe->getQueryName()).arg(mSecs), this);
					}
					
					// print calculation statistics
					loggingCalculationStatistics();


					CCallbackData* callback = reasoningData->mCallback;
					if (callback) {
						callback->doCallback();
					}
					mCalculatingQuerySet.remove(query);
					mReasoningTaskDataHash.remove((cint64)query);
					delete reasoningData;

				}


				bool CReasonerManagerThread::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
					if (CIntervalThread::processCustomsEvents(type,event)) {
						return true;
					} else if (type == CReasoningSatisfiableCalculationJobEvent::EVENTTYPE) {
						CReasoningSatisfiableCalculationJobEvent *cbse = static_cast<CReasoningSatisfiableCalculationJobEvent *>(event);					
						prepareSatisfiableBoxReasoning(cbse);
						return true;

					} else if (type == CJobCalculatedSatisfiableCallbackEvent::EVENTTYPE) {
						CJobCalculatedSatisfiableCallbackEvent *cbsce = static_cast<CJobCalculatedSatisfiableCallbackEvent *>(event);
						finishSatisfiableBoxReasoning(cbsce);
						return true;

					} else if (type == CQueryJobCalculatedSatisfiableCallbackEvent::EVENTTYPE) {
						CQueryJobCalculatedSatisfiableCallbackEvent *cbsce = static_cast<CQueryJobCalculatedSatisfiableCallbackEvent *>(event);
						finishQuerySatisfiableBoxReasoning(cbsce);
						return true;

					} else if (type == CCalcQueryEvent::EVENTTYPE) {
 						CCalcQueryEvent *cqe = static_cast<CCalcQueryEvent *>(event);

						prepareQueryReasoning(cqe);	

						return true;

					} else if (type == CPrepareOntologyEvent::EVENTTYPE) {
						CPrepareOntologyEvent* poe = static_cast<CPrepareOntologyEvent *>(event);

						prepareOntologyReasoning(poe);	

						return true;

					} else if (type == CCalcedQueryEvent::EVENTTYPE) {
						CCalcedQueryEvent *cqe = static_cast<CCalcedQueryEvent *>(event);

						finishQueryReasoning(cqe);

						return true;

					} else if (type == CRequirementProcessedCallbackEvent::EVENTTYPE) {
						CRequirementProcessedCallbackEvent* rpce = static_cast<CRequirementProcessedCallbackEvent *>(event);
						CConcreteOntology* ontology = rpce->getOntology();
						CRequirementPreparingData* reqPrepData = rpce->getRequirementPreparingData();

						continueRequirementProcessing(reqPrepData, ontology);


						if (rpce->mProcType == COntologyProcessingStep::OPANSWERER && !mAnswererPrepared) {
							mAnswererPrepared = true;
							for (CRequirementProcessedCallbackEvent* prepReqData : mAnswererPreparedCallbackQueue) {
								prepReqData->doCallback();
							}
							mAnswererPreparedCallbackQueue.clear();
						}


						return true;
					}

					return false;
				}


			}; // end namespace Manager

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
