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

#include "CReasonerEvaluationExecutorThread.h"
#include "CReasonerEvaluationConfigurationLoader.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationExecutorThread::CReasonerEvaluationExecutorThread(CConfiguration *config, CWatchDog *watchDog)
					: CThread("Reasoner-Evaluation-Executor-Thread",watchDog), CLogIdentifier("::Konclude::Test::ReasonerEvaluationExecutor",this) {

				mLoaderConfig = config;
				mCommanderManager = CConfigManagerReader::readCommanderManagerConfig(mLoaderConfig);
				mReasonerEvalFactory = new CConfigDependendReasonerEvaluationFactory();
				mReasonerEvaluator = new CReasonerEvaluationEvaluator();
				mReasonerClient = new CReasonerEvaluationRequestClientThread();
				mReasonerProvider = nullptr;
				mNumberStringSize = 4;
				mNextTestRemainingExecutionCount = 0;
				mNextTestExecutionNumber = 0;
				mTestCount = 0;
				mErrorCommBreakCount = 2;
				mFirstTest = true;
				mCriticalProcessesTester = nullptr;
				startThread();
			}


			CReasonerEvaluationExecutorThread::~CReasonerEvaluationExecutorThread() {
				delete mReasonerEvalFactory;
				delete mReasonerEvaluator;
				delete mReasonerProvider;
				delete mReasonerClient;
				delete mCriticalProcessesTester;
			}

			bool CReasonerEvaluationExecutorThread::executeEvaluation(CReasonerEvaluationTestsuite* evaluationTestsuite, CCallbackData* callbackData) {
                QCoreApplication::postEvent(this,new CReasonerEvaluationTestsuiteEvent(evaluationTestsuite,callbackData));
				return true;
			}

			bool CReasonerEvaluationExecutorThread::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
				if (CThread::processCustomsEvents(type,event)) {
					return true;
				} else if (type == CReasonerEvaluationTestsuiteEvent::EVENTTYPE) {
					CReasonerEvaluationTestsuiteEvent* rete = (CReasonerEvaluationTestsuiteEvent*)event;


					mEvalTestsuite = rete->getEvaluationTestsuite();
					mExecutedCallback = rete->getCallbackData();
					mPlatformName = mEvalTestsuite->getPlatformString();
					mExecutedTest = false;
					

					mFiltering = mEvalTestsuite->getEvaluationFiltering();

					bool configsSuccessfullyLoaded = loadConfigurations();
					if (!configsSuccessfullyLoaded) {
						failedEvaluationTests();
					} else {
						if (mCommanderManager) {
							CGetSettingsCommand* getConfigCommand = new CGetSettingsCommand();
							getConfigCommand->addProcessedCallback(new CCommandProcessedCallbackEvent(this,getConfigCommand));
							mCommanderManager->realizeCommand(getConfigCommand);
						}
					}

					return true;
				} else if (type == CCommandProcessedCallbackEvent::EVENTTYPE) {
					CCommandProcessedCallbackEvent *commProCallbackEvent = (CCommandProcessedCallbackEvent*)event;	
					CGetSettingsCommand* getConfigCommand = (CGetSettingsCommand*)commProCallbackEvent->getCommand();
					CConfigurationBase* configurationBase = getConfigCommand->getConfiguration();
					mConfiguration = configurationBase;
					if (!mConfiguration) {
						mConfiguration = mLoaderConfig;
					}
					initEvaluationTests();

					if (prepareNextEvaluationTest()) {
						executeNextEvaluationTest(mReasonerInitFileString,mNextTestInputFile);
					} else {
						finishEvaluationTests();
					}
				} else if (type == CReasonerEvaluationNextEvent::EVENTTYPE) {
					CReasonerEvaluationNextEvent* nextEvalEvent = (CReasonerEvaluationNextEvent*)event;	

					CReasonerEvaluationRequestResult* requestResults = mReasonerClient->getReasonerEvaluationResult();
					CReasonerEvaluationTerminationResult* terminationResults = mReasonerProvider->destroyReasoner();

					QString outputFileString = getNextOutputFileString();

					if (mFirstTest) {
						mFirstTest = false;
						executeNextEvaluationTest(mReasonerInitFileString,mNextTestInputFile);

						if (mWaitingTimeBetweenTests > 0) {
							LOG(INFO,getLogDomain(),logTr("Waiting %1 ms before continuation.").arg(mWaitingTimeBetweenTests),this);
							QThread::msleep(mWaitingTimeBetweenTests);
						}

					} else {
						bool succ = evaluateExecutedTest(terminationResults,requestResults,outputFileString);

						if (mWaitingTimeBetweenTests > 0) {
							LOG(INFO,getLogDomain(),logTr("Waiting %1 ms before continuation.").arg(mWaitingTimeBetweenTests),this);
							QThread::msleep(mWaitingTimeBetweenTests);
						}

						delete terminationResults;
						delete requestResults;

						if (succ) {
							if (--mNextTestRemainingExecutionCount > 0) {
								++mNextTestExecutionNumber;
								executeNextEvaluationTest(mReasonerInitFileString,mNextTestInputFile);
							} else {
								if (prepareNextEvaluationTest()) {
									executeNextEvaluationTest(mReasonerInitFileString,mNextTestInputFile);
								} else {
									finishEvaluationTests();
								}
							}
						} else {
							if (--mErrorCommBreakCount > 0) {
								executeNextEvaluationTest(mReasonerInitFileString,mNextTestInputFile);
							} else {
								failedEvaluationTests();
							}
						}
					}
				}

				return false;
			}

			bool CReasonerEvaluationExecutorThread::loadConfigurations() {
				bool configsSuccessfullyLoaded = true;
				QString globalConfString = getCorrectedPathString(mEvalTestsuite->getGlobalConfigString());
				if (!globalConfString.isEmpty()) {
					CReasonerEvaluationConfigurationLoader* globalConfLoader = new CReasonerEvaluationConfigurationLoader();
					LOG(INFO,getLogDomain(),logTr("Loading global configuration '%1'.").arg(globalConfString),this);
					configsSuccessfullyLoaded &= globalConfLoader->loadConfiguration(globalConfString,mLoaderConfig);
					delete globalConfLoader;
				} else {
					configsSuccessfullyLoaded = false;
				}
				QString reasonerConfString = getCorrectedPathString(mEvalTestsuite->getReasonerConfigString());
				if (!reasonerConfString.isEmpty()) {
					CReasonerEvaluationConfigurationLoader* reasonerConfLoader = new CReasonerEvaluationConfigurationLoader();
					LOG(INFO,getLogDomain(),logTr("Loading reasoner configuration '%1'.").arg(reasonerConfString),this);
					configsSuccessfullyLoaded &= reasonerConfLoader->loadConfiguration(reasonerConfString,mLoaderConfig);
					delete reasonerConfLoader;
				} else {
					configsSuccessfullyLoaded = false;
				}
				return configsSuccessfullyLoaded;
			}


			bool CReasonerEvaluationExecutorThread::initEvaluationTests() {
				mInputDir = mEvalTestsuite->getTestsDirectoryString();
				mOutputDir = mEvalTestsuite->getEvaluationDirectoryString();

				if (!mInputDir.endsWith("/")) {
					mInputDir += "/";
				}
				if (!mOutputDir.endsWith("/")) {
					mOutputDir += "/";
				}

				mRemainingSubDirs.append(".");

				mReasonerInitFileString = getCorrectedPathString(CConfigDataReader::readConfigString(mConfiguration,"Konclude.Evaluation.Reasoner.InitializationFile"));
				mConfigName = CConfigDataReader::readConfigString(mConfiguration,"Konclude.Evaluation.Config.Name");
				mReasonerName = CConfigDataReader::readConfigString(mConfiguration,"Konclude.Evaluation.Reasoner.Name");
				mReasonerOutputDirectory = getCorrectedPathString(CConfigDataReader::readConfigString(mConfiguration,"Konclude.Evaluation.Reasoner.OutputDirectory"));
				mTestCount = CConfigDataReader::readConfigInteger(mConfiguration,"Konclude.Evaluation.TestingCount");

				mErrorCommBreakCount = CConfigDataReader::readConfigInteger(mConfiguration,"Konclude.Evaluation.CancelReasonerErrorCount");

				mTestTimeout = CConfigDataReader::readConfigInteger(mConfiguration,"Konclude.Evaluation.RequestTimeout");

				mWaitingTimeBetweenTests = CConfigDataReader::readConfigInteger(mConfiguration,"Konclude.Evaluation.WaitingTimeBetweenTests");
				mWaitingTimeAfterReasonerCreation = CConfigDataReader::readConfigInteger(mConfiguration,"Konclude.Evaluation.WaitingTimeAfterReasonerCreation");
				mSystemReadyRetestTimeForTests = CConfigDataReader::readConfigInteger(mConfiguration,"Konclude.Evaluation.SystemReadyRetestTimeForTests");

				mFirstTestWarmUp = CConfigDataReader::readConfigBoolean(mConfiguration,"Konclude.Evaluation.FirstTestsWarmingUp");
				mAllTestsWarmUp = CConfigDataReader::readConfigBoolean(mConfiguration,"Konclude.Evaluation.AllTestsWarmingUp");

				mCriticalProcessesTester = new CCriticalSystemProcessTester(mConfiguration);

				if (mFirstTestWarmUp) {
					mFirstTest = true;
				}

				LOG(INFO,getLogDomain(),logTr("Preparing reasoner '%1' evaluation with config file '%2' and test suite '%3'.").arg(mReasonerName).arg(mConfigName).arg(mInputDir),this);
				LOG(INFO,getLogDomain(),logTr("Using init file '%1' and output directory '%2'.").arg(mReasonerInitFileString).arg(mReasonerOutputDirectory),this);

				return true;
			}


			bool CReasonerEvaluationExecutorThread::finishEvaluationTests() {
				LOG(INFO,getLogDomain(),logTr("Finished reasoner '%1' evaluation with config file '%2' and test suite '%3'.").arg(mReasonerName).arg(mConfigName).arg(mInputDir),this);
				if (mExecutedCallback) {
					mExecutedCallback->setCallbackDataContext(new CReasonerEvaluationEvaluatedCallbackContextData(mExecutedTest,mReasonerName,mOutputDir+mConfigName+"/"+mReasonerOutputDirectory,mTestTimeout,mTestCount));
					mExecutedCallback->doCallback();
				}
				delete mEvalTestsuite;
				return true;
			}



			bool CReasonerEvaluationExecutorThread::failedEvaluationTests() {
				LOG(INFO,getLogDomain(),logTr("Failed evaluation for reasoner with config file '%2' and test suite '%3'.").arg(mEvalTestsuite->getReasonerConfigString()).arg(mEvalTestsuite->getTestsDirectoryString()),this);
				if (mExecutedCallback) {
					mExecutedCallback->setCallbackDataContext(new CReasonerEvaluationEvaluatedCallbackContextData(true));
					mExecutedCallback->doCallback();
				}
				delete mEvalTestsuite;
				return true;
			}




			bool CReasonerEvaluationExecutorThread::prepareNextEvaluationTest() {
				while (mNextTestRemainingExecutionCount <= 0) {
					QString testFile = nextEvaluationTestfile();
					if (testFile.isEmpty()) {
						return false;
					} else {
						if (mAllTestsWarmUp) {
							mFirstTest = true;
						}
						QString reasonerString = mReasonerOutputDirectory;
						QString configString = mConfigName;
						QDir genPath(mOutputDir);
						if (genPath.mkpath(configString+"/"+reasonerString+testFile)) {
							QDir outputDir(mOutputDir+configString+"/"+reasonerString+testFile);
							QStringList responseList = outputDir.entryList(QDir::Files);
							mNextTestExecutionNumber = responseList.count();
							mNextTestRemainingExecutionCount = mTestCount - mNextTestExecutionNumber;
							if (mNextTestRemainingExecutionCount > 0) {
								mNextTestInputFile = mInputDir+testFile;
								mNextTestOutputDir = mOutputDir+configString+"/"+reasonerString+testFile+"/";
								return true;
							}
						}
					}
				}
				return false;
			}



			QString CReasonerEvaluationExecutorThread::getCorrectedPathString(const QString& string) {
				QString corrString = string;
				corrString = corrString.replace("$$_PLATFORM_$$",mPlatformName);
				corrString = CReasonerEvaluationPathConverter::getCorrectedPathString(corrString);
				return corrString;
			}


			bool CReasonerEvaluationExecutorThread::testExistFile(const QString& fileString) {
				QFile file(fileString);
				if (file.exists()) {
					return true;
				}
				return false;
			}



			QString CReasonerEvaluationExecutorThread::getNextOutputFileString() {
				QString outputFileString = QString("%2%1-response.xml").arg(getNumberString(mNextTestExecutionNumber)).arg(mNextTestOutputDir);
				while (testExistFile(outputFileString)) {
					++mNextTestExecutionNumber;
					outputFileString = QString("%2%1-response.xml").arg(getNumberString(mNextTestExecutionNumber)).arg(mNextTestOutputDir);
				}
				return outputFileString;
			}

			QString CReasonerEvaluationExecutorThread::getNumberString(cint64 number) {
				QString numberString = QString::number(number);
				while (numberString.size() < mNumberStringSize) {
					numberString = QString("0%1").arg(numberString);
				}
				return numberString;
			}


			bool CReasonerEvaluationExecutorThread::evaluateExecutedTest(CReasonerEvaluationTerminationResult* terminationResults, CReasonerEvaluationRequestResult* requestResults, const QString& testcaseOutput) {
				if (requestResults->hasSucessfullReasonerCommunication()) {
					LOG(INFO,getLogDomain(),logTr("Evaluate executed test case to '%1'.").arg(testcaseOutput),this);
					mReasonerEvaluator->evaluateResults(requestResults,terminationResults,mNextTestInputFile,testcaseOutput,mConfiguration);
					mExecutedTest = true;
					return true;
				} else {
					LOG(ERROR,getLogDomain(),logTr("Communication with reasoner failed for test case '%1'.").arg(testcaseOutput),this);
				}
				return false;
			}


			bool CReasonerEvaluationExecutorThread::testSystemReadyForNextEvaluationTest() {
				if (mCriticalProcessesTester->hasCriticalSystemProcesses()) {
					LOG(WARNING,getLogDomain(),logTr("Critical processes are running on the test system."),this);
					return false;
				}
				return true;
			}

			void CReasonerEvaluationExecutorThread::delayNextEvaluationTestUntilSystemReady() {
				while (!testSystemReadyForNextEvaluationTest()) {
					LOG(WARNING,getLogDomain(),logTr("Test system not ready, retest after %1 ms.").arg(mSystemReadyRetestTimeForTests),this);
					QThread::msleep(mSystemReadyRetestTimeForTests);
				}
			}



			bool CReasonerEvaluationExecutorThread::executeNextEvaluationTest(const QString& testcaseInit, const QString& testcaseInput) {

				delayNextEvaluationTestUntilSystemReady();

				if (!mReasonerProvider) {
					mReasonerProvider = mReasonerEvalFactory->createReasonerProvider(mConfiguration);
				}
				mReasonerProvider->createReasoner(mConfiguration);

				QString addressString = mReasonerProvider->getReasonerAddressString();

				if (mFirstTest) {
					LOG(INFO,getLogDomain(),logTr("Warming up for reasoner '%1' with test case '%2'.").arg(mReasonerName).arg(testcaseInput),this);
				} else {
					LOG(INFO,getLogDomain(),logTr("Testing reasoner '%1' with test case '%2'.").arg(mReasonerName).arg(testcaseInput),this);
				}
				if (mWaitingTimeAfterReasonerCreation > 0) {
					LOG(INFO,getLogDomain(),logTr("Waiting %1 ms after created reasoner.").arg(mWaitingTimeAfterReasonerCreation),this);
					QThread::msleep(mWaitingTimeAfterReasonerCreation);
				}
				mReasonerClient->evaluateReasoner(testcaseInit,testcaseInput,addressString,mConfiguration,new CReasonerEvaluationNextEvent(this,0));
				return true;
			}




			bool CReasonerEvaluationExecutorThread::testFileFiltered(const QString& testFileNameString, const QString& testFilePathString, const QString& subDirPathString) {
				if (!mFiltering) {
					return true;
				} else {
					QStringList statisticList = mFiltering->loadStatistics(testFileNameString,subDirPathString);
					return mFiltering->testSatisfiesFilter(testFileNameString,testFilePathString,subDirPathString,statisticList);
				}
			}


			QString CReasonerEvaluationExecutorThread::nextEvaluationTestfile() {
				if (mRemainingFiles.isEmpty()) {
					while (mRemainingFiles.isEmpty() && !mRemainingSubDirs.isEmpty()) {
						QString nextDir = mRemainingSubDirs.takeFirst();
						QDir dir(mInputDir+nextDir);
						QStringList subDirList = dir.entryList(QDir::Dirs,QDir::Name|QDir::IgnoreCase);
						foreach (QString subDir, subDirList) {
							if (!subDir.startsWith("-") && subDir != "." && subDir != "..") {
								mRemainingSubDirs.prepend(QString("%1").arg(nextDir+"/"+subDir));
							}
						}
						QStringList fileList = dir.entryList(QDir::Files,QDir::Name|QDir::IgnoreCase);
						foreach (QString fileString, fileList) {
							if (!fileString.startsWith("-")) {
								if (testFileFiltered(fileString,mInputDir,nextDir+"/")) {
									mRemainingFiles.append(nextDir+"/"+fileString);
								} else {
									LOG(INFO,getLogDomain(),logTr("File '%1' in directory '%2' filtered out.").arg(fileString).arg(nextDir),this);
								}
							}
						}
					}
				}
				QString nextTestfile;
				if (!mRemainingFiles.isEmpty()) {
					nextTestfile = mRemainingFiles.takeFirst();
				}
				return nextTestfile;
			}

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude