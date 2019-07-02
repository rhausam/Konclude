/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CAnsweringManagerThread.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			
			CAnsweringManagerThread::CAnsweringManagerThread(CReasonerManager* reasonerManager, CConfiguration* config) : CThread("Answerer-Manager-Thread"), mHandlerProvider(new CConfigurationAnsweringHandlerFactory(config)) {
				mConfig = config;
				mConfMaxThreadCount = 1;
				mCurrentThreadCount = 0;
				mReasonerManager = reasonerManager;

				if (mQueuedThreadList.isEmpty() && mCurrentThreadCount < mConfMaxThreadCount) {
					CAnswererThread* answererThread = new CAnswererThread(mReasonerManager);
					answererThread->initializeAnswerer(mConfig);
					mQueuedThreadList.append(answererThread);
					mAnswererThreadContainer.append(answererThread);
					++mCurrentThreadCount;
				}
				startThread(QThread::HighPriority);
			}


			CAnsweringManagerThread::~CAnsweringManagerThread() {
				stopThread();
			}


			CAnsweringManager* CAnsweringManagerThread::answerComplexQuery(CComplexAnsweringQuery* complexAnsweringQuery, CCallbackData* callback) {
				postEvent(new CAnswerComplexQueryEvent(complexAnsweringQuery, callback));
				return this;
			}


			CAnsweringProgress* CAnsweringManagerThread::getAnsweringProgress() {
				CAnsweringProgress newAnsweringProg;
				double percentAvg = 0;
				cint64 percentCount = 0;
				mReadWriteLock.lockForRead();

				for (CAnswererThread* answererThread : mAnswererThreadContainer) {
					CAnsweringProgress* answeringProgress = answererThread->getAnsweringProgress();
					if (answeringProgress) {
						newAnsweringProg.setTotalInstances(newAnsweringProg.getTotalInstances() + answeringProgress->getTotalInstances());
						newAnsweringProg.setTestedInstances(newAnsweringProg.getTestedInstances() + answeringProgress->getTestedInstances());
						newAnsweringProg.setRemainingMilliSeconds(qMax(newAnsweringProg.getRemainingMilliSeconds(), answeringProgress->getRemainingMilliSeconds()));
						percentAvg += answeringProgress->getProgessPercent();
						++percentCount;
					}
				}
				mReadWriteLock.unlock();
				if (percentCount != 0) {
					percentAvg /= (double)percentCount;
				}
				newAnsweringProg.setProgessPercent(percentAvg);
				mAnsweringProgress = newAnsweringProg;
				return &mAnsweringProgress;

			}



			CAnsweringStatistics* CAnsweringManagerThread::getAnsweringStatistics(CConcreteOntology* ontology) {
				CAnsweringStatisticsCollectionStrings* stats = new CAnsweringStatisticsCollectionStrings();
				mHandlerProvider.addAnsweringStatistics(ontology, stats);
				return stats;
			}



			CAnsweringManager* CAnsweringManagerThread::finishComplexQueryAnswering(CAnsweringHandler* answeringHandler, CAnsweringManagerQueryData* queryData) {
				mHandlerProvider.releaseAnsweringHandler(answeringHandler);
				if (queryData) {
					mProcessingQuerySet.remove(queryData);
					CAnswererThread* thread = queryData->getThread();
					mProcessingThreadSet.remove(thread);
					mQueuedThreadList.append(thread);
					CCallbackData* callback = queryData->getCallback();
					if (callback) {
						callback->doCallback();
					}
					delete queryData;
				}
				return this;
			}

			

			CAnsweringManager* CAnsweringManagerThread::queueComplexQueryAnswering(CComplexAnsweringQuery* complexAnsweringQuery, CCallbackData* callback) {
				CAnsweringManagerQueryData* queryData = new CAnsweringManagerQueryData(complexAnsweringQuery, callback);
				mQueuedQueryList.append(queryData);
				return this;
			}

			CAnsweringManager* CAnsweringManagerThread::manageComplexQueryAnswering() {
				if (!mQueuedQueryList.isEmpty()) {
					if (mQueuedThreadList.isEmpty() && mCurrentThreadCount < mConfMaxThreadCount) {
						CAnswererThread* answererThread = new CAnswererThread(mReasonerManager);
						answererThread->initializeAnswerer(mConfig);
						mQueuedThreadList.append(answererThread);
						mReadWriteLock.lockForWrite();
						mAnswererThreadContainer.append(answererThread);
						mReadWriteLock.unlock();
						++mCurrentThreadCount;
					}
					if (!mQueuedThreadList.isEmpty()) {
						CAnsweringManagerQueryData* queryData = mQueuedQueryList.takeFirst();
						CComplexAnsweringQuery* complexAnsweringQuery = queryData->getQuery();
						CAnsweringHandler* handler = mHandlerProvider.getAnsweringHandler(complexAnsweringQuery);
						CAnswererThread* thread = mQueuedThreadList.takeFirst();
						queryData->setThread(thread);
						mProcessingThreadSet.insert(thread);
						mProcessingQuerySet.insert(queryData);
						CAnsweringComplexQueryCompletedEvent* completedEventCallback = new CAnsweringComplexQueryCompletedEvent(this, handler, queryData);
						thread->handleAnswering(handler, complexAnsweringQuery, completedEventCallback);
					}
				}
				return this;
			}


			CAnsweringManager* CAnsweringManagerThread::prepareAnswering(CConcreteOntology* ontology, CCallbackData* callback) {
				postEvent(new CAnsweringOntologyPreparationEvent(ontology, callback));
				return this;
			}


			bool CAnsweringManagerThread::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
				if (CThread::processCustomsEvents(type, event)) {
					return true;
				} else if (type == CAnsweringOntologyPreparationEvent::EVENTTYPE) {
					CAnsweringOntologyPreparationEvent* ape = (CAnsweringOntologyPreparationEvent *)event;
					if (mQueuedThreadList.isEmpty() && mCurrentThreadCount < mConfMaxThreadCount) {
						CAnswererThread* answererThread = new CAnswererThread(mReasonerManager);
						answererThread->initializeAnswerer(mConfig);
						mQueuedThreadList.append(answererThread);
						mReadWriteLock.lockForWrite();
						mAnswererThreadContainer.append(answererThread);
						mReadWriteLock.unlock();
						++mCurrentThreadCount;
					}
					if (!mQueuedThreadList.isEmpty()) {
						CConcreteOntology* ontology = ape->getOntology();
						CAnsweringHandler* handler = mHandlerProvider.getAnsweringHandler(ontology);
						CAnswererThread* thread = mQueuedThreadList.takeFirst();
						CAnsweringPreparedEvent* preparedEventCallback = new CAnsweringPreparedEvent(thread, handler, this, ape->getCallbackData());
						thread->prepareAnswering(handler, preparedEventCallback);
						mProcessingThreadSet.insert(thread);
					} else {
						ape->getCallbackData()->doCallback();
					}
					return true;
				} else if (type == CAnswerComplexQueryEvent::EVENTTYPE) {
					CAnswerComplexQueryEvent* acqe = (CAnswerComplexQueryEvent *)event;
					queueComplexQueryAnswering(acqe->getComplexQuery(), acqe->getCallbackData());
					manageComplexQueryAnswering();
					return true;
				} else if (type == CAnsweringComplexQueryCompletedEvent::EVENTTYPE) {
					CAnsweringComplexQueryCompletedEvent* acqce = (CAnsweringComplexQueryCompletedEvent *)event;
					finishComplexQueryAnswering(acqce->getAnsweringHandler(), acqce->getQueryData());
					manageComplexQueryAnswering();
					return true;
				} else if (type == CAnsweringPreparedEvent::EVENTTYPE) {
					CAnsweringPreparedEvent* ape = (CAnsweringPreparedEvent *)event;
					CAnswererThread* thread = ape->getAnswererThread();
					mProcessingThreadSet.remove(thread);
					mQueuedThreadList.append(thread);
					mHandlerProvider.releaseAnsweringHandler(ape->getAnsweringHandler());
					ape->getPreparedCallback()->doCallback();
					manageComplexQueryAnswering();
					return true;
				}
				return false;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude