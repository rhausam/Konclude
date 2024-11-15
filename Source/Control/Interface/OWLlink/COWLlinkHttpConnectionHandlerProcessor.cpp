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

#include "COWLlinkHttpConnectionHandlerProcessor.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace OWLlink {


				COWLlinkHttpConnectionHandlerProcessor::COWLlinkHttpConnectionHandlerProcessor(CConfiguration* loaderConfig, CQtHttpConnectionHandlerReleaser* releaser) : COWLlinkProcessor(false) {
					mReleaser = releaser;
					mLoaderConfig = loaderConfig;
					reasonerCommander = CConfigManagerReader::readCommanderManagerConfig(mLoaderConfig);
					mSocket = nullptr;
					mQueued = false;
					mForceDisconnect = CConfigDataReader::readConfigBoolean(mLoaderConfig,"Konclude.OWLlink.Server.ForceDisconnectAfterProcessing",false);
					mProcessingCommand = nullptr;
					startThread();
					mThreadStartedSemaphore.acquire(1);
				}


				COWLlinkHttpConnectionHandlerProcessor::~COWLlinkHttpConnectionHandlerProcessor() {
				}

				void COWLlinkHttpConnectionHandlerProcessor::threadStarted() {
					COWLlinkProcessor::threadStarted();

					mParser = new CQtHttpRequestBodyOnlyParser();
					mSocket = new QTcpSocket();
					mSocket->moveToThread(this);

					mProcessingRequest = false;
					mProcessMoreRead = false;

					connect(mSocket,SIGNAL(readyRead()),this,SLOT(connectionRead()));
					connect(mSocket,SIGNAL(disconnected()),this,SLOT(connectionDisconnect()));

					mThreadStartedSemaphore.release(1);
				}

				void COWLlinkHttpConnectionHandlerProcessor::threadStopped() {
					COWLlinkProcessor::threadStopped();

					mSocket->close();
					delete mSocket;
					delete mParser;
				}


				bool COWLlinkHttpConnectionHandlerProcessor::handleIncomingConnection(int socketDescriptor) {
					postEvent(new CHandleIncomingHttpConnectionEvent(socketDescriptor));
					return true;
				}


				bool COWLlinkHttpConnectionHandlerProcessor::isHandlerQueued() {
					return mQueued;
				}

				COWLlinkHttpConnectionHandlerProcessor* COWLlinkHttpConnectionHandlerProcessor::setHandlerQueued(bool queued) {
					mQueued = queued;
					return this;
				}



				bool COWLlinkHttpConnectionHandlerProcessor::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
					if (COWLlinkProcessor::processCustomsEvents(type,event)) {
						return true;
					} else {
						if (type == CConnectionReadyReadEvent::EVENTTYPE) {
							CConnectionReadyReadEvent* crre = (CConnectionReadyReadEvent*)event;

							if (!mProcessingRequest) {

								if (!mParser->readFromDevice(mSocket)) {
									// error
									QString errorString = QString("HTTP/1.1 400 error parsing request\r\nConnection: close\r\n\r\n400 Parsing error: %1\r\n").arg(mParser->getHttpRequestParsingErrorString());
									LOG(INFO,"::Konclude::Control::Interface::OWLlink::OWLlinkHTTPConnectionProcessor",logTr("Error parsing HTTP request, '%1'.").arg(mParser->getHttpRequestParsingErrorString()),this);
                                    mSocket->write(errorString.toLocal8Bit());
									mSocket->disconnectFromHost();
								}

								if (mParser->hasHttpRequestParsingCompleted()) {
									mProcessingRequest = true;
									mProcessingByteArray = mParser->takeBodyData();
									if (mProcessingByteArray != 0) {
										LOG(INFO,"::Konclude::Control::Interface::OWLlink::OWLlinkHTTPConnectionProcessor",logTr("HTTP request successfully parsed, %1 Bytes received.").arg(mProcessingByteArray->count()),this);


										//QFile commFile("Debugging/Communication/http-owllink-requests.txt");
										//if (commFile.open(QIODevice::Append)) {
										//	commFile.write(*mProcessingByteArray);
										//	commFile.close();
										//}


										COWLlinkQtXMLCommandParser *owllinkCommandParser = new COWLlinkQtXMLCommandParser(mLoaderConfig);
										mOwllinkInterpreter = new COWLLinkRecordInterpreter(preSynchronizer,mLoaderConfig);
										defaultCommandDelegater = mOwllinkInterpreter;

										CParseOWLlinkCommandsCommand *parseCommand = new CParseOWLlinkCommandsCommand(mProcessingByteArray,mSocket->peerAddress().toString());
										parseCommand->setRecorder(mOwllinkInterpreter);
										parseCommand->setReportErrorFromSubCommands(false);

										CCommandProcessedCallbackEvent *proComm = new CCommandProcessedCallbackEvent(this,parseCommand);
										parseCommand->addProcessedCallback(proComm);


										owllinkCommandParser->realizeCommand(parseCommand,mOwllinkInterpreter);
										mProcessingCommand = parseCommand;

										delete owllinkCommandParser;
										delete mProcessingByteArray;
									} else {
										// invalid request

										QString koncludeString = QString("%1 %2").arg(CKoncludeInfo::getKoncludeName()).arg(CKoncludeInfo::getKoncludeVersionString());
										sendData(QString("<h2>%1 OWLlink Sever</h2><p>This response is generated from Konclude's OWLlink server for invalid OWLlink requests. Please make sure that the OWLlink request is appended in OWL 2 XML serialisation as content (with correct Content-Length) of your HTTP request. See <a href=\"http://konclude.com\">konclude.com</a> for help and further information.</p>").arg(koncludeString).toLocal8Bit());

										mProcessingRequest = false;
										if (mForceDisconnect || mParser->hasRequestedCloseConnection()) {
											mSocket->disconnectFromHost();
										} else if (mProcessMoreRead) {
											mProcessMoreRead = false;
											connectionRead();
										}

										mParser->reset();

									}


								}
							}

							return true;
						} else if (type == CConnectionDisconnectedEvent::EVENTTYPE) {
							CConnectionDisconnectedEvent* cde = (CConnectionDisconnectedEvent*)event;
							mParser->reset();
							mProcessMoreRead = false;
							if (!mProcessingRequest) {
								mHandlerBusy = false;
							}
							mReleaser->releaseConnectionHandler(this);
							return true;


						} else if (type == CHandleIncomingHttpConnectionEvent::EVENTTYPE) {
							CHandleIncomingHttpConnectionEvent* hihc = (CHandleIncomingHttpConnectionEvent*)event;
							cint64 socketDescriptor = hihc->getSocketDescriptor();
							LOG(INFO,"::Konclude::Control::Interface::OWLlink::OWLlinkHTTPConnectionProcessor",logTr("Start processing new incoming connection."),this);
							mSocket->setSocketDescriptor(socketDescriptor);
							mHandlerBusy = true;
							return true;
						}

					}
					return false;
				}

				COWLlinkProcessor* COWLlinkHttpConnectionHandlerProcessor::initializeOWLlinkContent() {
					return this;
				}



				COWLlinkProcessor* COWLlinkHttpConnectionHandlerProcessor::sendData(const QByteArray& dataArray) {
					if (mSocket->state() == QTcpSocket::ConnectedState) {

						QString connectionString = QString("Connection: ");
						if (mParser->hasRequestedCloseConnection()) {
							connectionString += QString("close");
						} else {
							connectionString += QString("keep-alive");
						}
						QString serverString = QString("Server: %1 %2").arg(CKoncludeInfo::getKoncludeName()).arg(CKoncludeInfo::getKoncludeVersionString());
						QString responseHeadString = QString("HTTP/1.1 200 OK\r\n%1\r\n%2\r\nContent-Length: %3\r\n\r\n").arg(serverString).arg(connectionString).arg(dataArray.length());

						mSocket->write(responseHeadString.toLocal8Bit());

						//QFile commFile("Debugging/Communication/http-owllink-responses.txt");
						//if (commFile.open(QIODevice::Append)) {
						//	commFile.write(dataArray);
						//	commFile.close();
						//}

						mSocket->write(dataArray);


						LOG(INFO,"::Konclude::Control::Interface::OWLlink::OWLlinkHTTPConnectionProcessor",logTr("HTTP request successfully processed, %1 Bytes sent.").arg(dataArray.count()),this);
					} else {
						LOG(INFO,"::Konclude::Control::Interface::OWLlink::OWLlinkHTTPConnectionProcessor",logTr("HTTP request successfully processed, but connection already closed."),this);
					}
					return this;
				}

				COWLlinkProcessor* COWLlinkHttpConnectionHandlerProcessor::concludeOWLlinkContent() {
					sendData(mOwllinkInterpreter->getByteArray());
					delete mOwllinkInterpreter;
					delete mProcessingCommand;
					mProcessingCommand = nullptr;
					mOwllinkInterpreter = nullptr;
					mProcessingRequest = false;


					if (mForceDisconnect || mParser->hasRequestedCloseConnection()) {
						mSocket->disconnectFromHost();
					} else if (mProcessMoreRead) {
						mProcessMoreRead = false;
						connectionRead();
					}

					mParser->reset();
					return this;
				}

				void COWLlinkHttpConnectionHandlerProcessor::connectionRead() {
					postEvent(new CConnectionReadyReadEvent());
				}

				void COWLlinkHttpConnectionHandlerProcessor::connectionDisconnect() {
					postEvent(new CConnectionDisconnectedEvent());
				}

				CConfiguration* COWLlinkHttpConnectionHandlerProcessor::getConfiguration() {
					if (!reasonerCommander) {
						return mLoaderConfig;
					} else {
						CGetDescriptionCommand *getDesComm = new CGetDescriptionCommand();
						reasonerCommander->delegateCommand(getDesComm);
						CCommandExecutedBlocker commExeBlocker;
						commExeBlocker.waitExecutedCommand(getDesComm);
						CConfiguration *config = getDesComm->getConfiguration();
						return config;
					}
				}


			}; // end namespace OWLlink 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
