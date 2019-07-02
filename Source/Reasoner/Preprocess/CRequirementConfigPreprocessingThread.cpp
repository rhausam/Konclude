/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CRequirementConfigPreprocessingThread.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CRequirementConfigPreprocessingThread::CRequirementConfigPreprocessingThread(CReasonerManager *reasonerManager) : CPreprocessingThread(reasonerManager),CLogDomain("::Konclude::Reasoner::Kernel::Preprocessor") {
			}


			CRequirementConfigPreprocessingThread::~CRequirementConfigPreprocessingThread() {
			}


			COntologyPreprocessingItem* CRequirementConfigPreprocessingThread::initializeOntologyPreprocessingItem(CConcreteOntology* ontology, CConfigurationBase* config) {
				CRequirementConfigPreprocessingItem* item = new CRequirementConfigPreprocessingItem();
				item->initRequirementConfigPreprocessingItem(ontology,config);
				return item;
			}








			bool CRequirementConfigPreprocessingThread::createNextTest() {


				COntologyPreprocessingItem* loopOntPreCompItem = 0;
				bool workTestCreated = false;
				while (!workTestCreated && !mProcessingOntItemList.isEmpty()) {
					COntologyPreprocessingItem* ontPreCompItem = mProcessingOntItemList.first();

					if (ontPreCompItem == loopOntPreCompItem) {
						// don't run into infinite loop without doing something
						break;
					}

					CRequirementConfigPreprocessingItem* reqConfPreCompItem = (CRequirementConfigPreprocessingItem*)ontPreCompItem;



					bool failDebug = false;
					if (!workTestCreated && reqConfPreCompItem->isActiveCountingStepRequired()) {
						if (!reqConfPreCompItem->isActiveCountingStepFinished()) {
							if (reqConfPreCompItem->areActiveCountingStepProcessingRequirementSatisfied()) {
								LOG(INFO,getDomain(),logTr("Counting active entities."),this);
								countActiveEntites(reqConfPreCompItem);
								reqConfPreCompItem->getActiveCountingProcessingStep()->setStepFinished(true);
								reqConfPreCompItem->getActiveCountingProcessingStep()->submitRequirementsUpdate();
							} else {
								reqConfPreCompItem->getActiveCountingProcessingStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSFAILEDREQUIREMENT);
							}
						} else {
							reqConfPreCompItem->getActiveCountingProcessingStep()->submitRequirementsUpdate();
						}
					}


					if (!workTestCreated && reqConfPreCompItem->isBuildStepRequired()) {
						if (!reqConfPreCompItem->isBuildStepFinished()) {
							if (reqConfPreCompItem->areBuildStepProcessingRequirementSatisfied()) {
								LOG(INFO,getDomain(),logTr("Build intern ontology representation."),this);
								createBuild(reqConfPreCompItem);
								if (reqConfPreCompItem->failAfterBuilt()) {
									failDebug = true;
									reqConfPreCompItem->getBuildProcessingStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED);
								} else {
									reqConfPreCompItem->getBuildProcessingStep()->setStepFinished(true);
									reqConfPreCompItem->getBuildProcessingStep()->submitRequirementsUpdate();
								}
							} else {
								reqConfPreCompItem->getBuildProcessingStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSFAILEDREQUIREMENT);
							}
						} else {
							reqConfPreCompItem->getBuildProcessingStep()->submitRequirementsUpdate();
						}
					}


					if (!workTestCreated && reqConfPreCompItem->isPreprocessStepRequired()) {
						if (!reqConfPreCompItem->isPreprocessStepFinished()) {
							if (reqConfPreCompItem->arePreprocessStepProcessingRequirementSatisfied()) {
								LOG(INFO,getDomain(),logTr("Preprocess ontology."),this);
								createPreprocess(reqConfPreCompItem);
								if (reqConfPreCompItem->failAfterPreprocessing()) {
									failDebug = true;
									reqConfPreCompItem->getPreprocessProcessingStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED);
								} else {
									reqConfPreCompItem->getPreprocessProcessingStep()->setStepFinished(true);
									reqConfPreCompItem->getPreprocessProcessingStep()->submitRequirementsUpdate();
								}
							} else {
								reqConfPreCompItem->getPreprocessProcessingStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSFAILEDREQUIREMENT);
							}
						} else {
							reqConfPreCompItem->getPreprocessProcessingStep()->submitRequirementsUpdate();
						}
					}

					if (!workTestCreated) {
						if (!reqConfPreCompItem->hasRemainingProcessingRequirements()) {
							finishOntologyPreprocessing(reqConfPreCompItem);
							mProcessingOntItemList.removeFirst();
						} else {
							mProcessingOntItemList.removeFirst();
							mProcessingOntItemList.append(reqConfPreCompItem);
							loopOntPreCompItem = ontPreCompItem;
						}
					}
				}
				return workTestCreated;
			}



			bool CRequirementConfigPreprocessingThread::countActiveEntites(CRequirementConfigPreprocessingItem* reqConfPreCompItem) {

				CConcreteOntology* ontology = reqConfPreCompItem->getOntology();
				CConfiguration* ontConfig = ontology->getConfiguration();


				CONTOLOGYAXIOMLIST< QPair<CAxiomExpression*,bool> >* changeAxiomList = ontology->getBuildData()->getChangeAxiomList();
				cint64 newAxiomCount = changeAxiomList->size();

				COntologyIncrementalRevisionData* incRevData = ontology->getIncrementalRevisionData();
				cint64 lastChangeCountedAxiom = incRevData->getLastChangeCountedAxiom();
				COntologyIncrementalAxiomChangeData* incAxChangeData = incRevData->getAxiomChangeData();
				bool axiomsChanged = false;
				for (cint64 currAxiom = lastChangeCountedAxiom; currAxiom < newAxiomCount; ++currAxiom) {
					const QPair<CAxiomExpression*,bool>& axiomChangePair((*changeAxiomList)[currAxiom]);
					bool axiomAdded = axiomChangePair.second;
					CAxiomExpression* axiom = axiomChangePair.first;
					CBuildExpression::ExpressionType axiomType = axiom->getType();
					if (axiomType == CBuildExpression::BETDECLARATION) {
						CDeclarationAxiomExpression* decAxiomExp = dynamic_cast<CDeclarationAxiomExpression*>(axiom);
						if (decAxiomExp) {
							CExpressionEntity* entityExp = decAxiomExp->getEntity();
							CClassExpression* classExp = dynamic_cast<CClassExpression*>(entityExp);
							if (classExp) {
								if (axiomAdded) {
									incAxChangeData->incAddedTBoxAxiomCount();
								} else {
									incAxChangeData->incDeletedTBoxAxiomCount();
								}
							} else {
								CObjectPropertyExpression* objPropExp = dynamic_cast<CObjectPropertyExpression*>(entityExp);
								if (objPropExp) {
									if (axiomAdded) {
										incAxChangeData->incAddedRBoxAxiomCount();
									} else {
										incAxChangeData->incDeletedRBoxAxiomCount();
									}
								} else {
									CDataPropertyExpression* dataPropExp = dynamic_cast<CDataPropertyExpression*>(entityExp);
									if (dataPropExp) {
										if (axiomAdded) {
											incAxChangeData->incAddedRBoxAxiomCount();
										} else {
											incAxChangeData->incDeletedRBoxAxiomCount();
										}
									} else {
										CNamedIndividualExpression* namedIndiExp = dynamic_cast<CNamedIndividualExpression*>(entityExp);
										if (namedIndiExp) {
											if (axiomAdded) {
												incAxChangeData->incAddedABoxAxiomCount();
											} else {
												incAxChangeData->incDeletedABoxAxiomCount();
											}
										} else {
											CAnonymousIndividualExpression* anonyIndiExp = dynamic_cast<CAnonymousIndividualExpression*>(entityExp);
											if (anonyIndiExp) {
												if (axiomAdded) {
													incAxChangeData->incAddedABoxAxiomCount();
												} else {
													incAxChangeData->incDeletedABoxAxiomCount();
												}
											}
										}
									}
								}
							}
						}
					} else if (axiomType == CBuildExpression::BETEQUIVALENTCLASSES || axiomType == CBuildExpression::BETESUBCLASSOF || axiomType == CBuildExpression::BETDISJOINTUNION || axiomType == CBuildExpression::BETDISJOINTCLASSES) {
						if (axiomAdded) {
							incAxChangeData->incAddedTBoxAxiomCount();
						} else {
							incAxChangeData->incDeletedTBoxAxiomCount();
						}
					} else if (axiomType == CBuildExpression::BETCLASSASSERTION || axiomType == CBuildExpression::BETOBJECTPROPERTYASSERTION || axiomType == CBuildExpression::BETSAMEINDIVIDUAL || 
								axiomType == CBuildExpression::BETDIFFERENTINDIVIDUAL || axiomType == CBuildExpression::BETNEGATIVEOBJECTPROPERTYASSERTION || 
								axiomType == CBuildExpression::BETDATAPROPERTYASSERTION || axiomType == CBuildExpression::BETNEGATIVEDATAPROPERTYASSERTION) {
						if (axiomAdded) {
							incAxChangeData->incAddedABoxAxiomCount();
						} else {
							incAxChangeData->incDeletedABoxAxiomCount();
						}
					} else if (axiomType == CBuildExpression::BETSUBOBJECTPROPERTYOF || axiomType == CBuildExpression::BETTRANSETIVEPROPERTYOF || axiomType == CBuildExpression::BETINVERSEOBJECTPROPERTYOF || axiomType == CBuildExpression::BETFUNCTIONALPROPERTYOF || 
								axiomType == CBuildExpression::BETOBJECTPROPERTYDOMAIN || axiomType == CBuildExpression::BETOBJECTPROPERTYRANGE || axiomType == CBuildExpression::BETFUNCTIONALOBJECTPROPERTY || axiomType == CBuildExpression::BETTRANSITIVEOBJECTPROPERTY ||
								axiomType == CBuildExpression::BETINVERSEOBJECTPROPERTIES || axiomType == CBuildExpression::BETEQUIVALENTOBJECTPROPERTIES || axiomType == CBuildExpression::BETSYMMETRICPROPERTY || axiomType == CBuildExpression::BETASYMMETRICPROPERTY ||
								axiomType == CBuildExpression::BETREFLEXIVEPROPERTY || axiomType == CBuildExpression::BETIRREFLEXIVEPROPERTY || axiomType == CBuildExpression::BETOBJECTPROPERTYCHAIN || axiomType == CBuildExpression::BETDISJOINTOBJECTPROPERTIES  || axiomType == CBuildExpression::BETINVERSEFUNCTIONALPROPERTY) {
						if (axiomAdded) {
							incAxChangeData->incAddedRBoxAxiomCount();
						} else {
							incAxChangeData->incDeletedRBoxAxiomCount();
						}
					} else if (axiomType == CBuildExpression::BETDATAPROPERTYDOMAIN || axiomType == CBuildExpression::BETDATAPROPERTYRANGE || axiomType == CBuildExpression::BETSUBDATAPROPERTYOF || axiomType == CBuildExpression::BETDISJOINTDATAPROPERTIES || 
						axiomType == CBuildExpression::BETEQUIVALENTDATAPROPERTIES || axiomType == CBuildExpression::BETFUNCTIONALDATAPROPERTY || axiomType == CBuildExpression::BETDATAPROPERTYASSERTION || axiomType == CBuildExpression::BETNEGATIVEDATAPROPERTYASSERTION) {
							if (axiomAdded) {
								incAxChangeData->incAddedRBoxAxiomCount();
							} else {
								incAxChangeData->incDeletedRBoxAxiomCount();
							}
					}
					axiomsChanged = true;
				}
				incRevData->setLastChangeCountedAxiom(newAxiomCount);



				bool incRebuild = CConfigDataReader::readConfigBoolean(ontConfig,"Konclude.Ontology.Revision.IncrementalRebuild");
				if (!incRebuild && ontology->getIncrementalRevisionData()->getBasementOntology() != ontology) {
					COntologyIncrementalRevisionData* incRevData = ontology->getIncrementalRevisionData();
					ontology->getDataBoxes()->referenceDataBoxes(incRevData->getBasementOntology()->getDataBoxes());
				} else {
					incRevData->setPreviousConsistentOntology(searchPreviousConsistentOntologyVersion(incRevData,reqConfPreCompItem));
					incRevData->setPreviousClassesClassifiedOntology(searchPreviousClassesClassifiedOntologyVersion(incRevData,reqConfPreCompItem));
					incRevData->setPreviousClassTypesRealizedOntology(searchPreviousClassRealizedOntologyVersion(incRevData,reqConfPreCompItem));

				}




				bool newAxiomsCounted = false;
				CExpressionDataBoxMapping* expDataBoxMapping = ontology->getDataBoxes()->getExpressionDataBoxMapping();
				CActiveEntityCountVector* activeEntityCountVector = expDataBoxMapping->getActiveEntityVector();
				activeEntityCountVector->increaseVectorSize(ontology->getBuildData()->getNextEntityNumber(false));
				CActiveEntitySubExpressionVisitorUpdater* activeEntityCountUpdater = new CActiveEntitySubExpressionVisitorUpdater(activeEntityCountVector);

				cint64 lastCountedAxiom = expDataBoxMapping->getLastActiveCountedEntityAxiom();
				for (cint64 currAxiom = lastCountedAxiom; currAxiom < newAxiomCount; ++currAxiom) {
					const QPair<CAxiomExpression*,bool>& axiomChangePair((*changeAxiomList)[currAxiom]);
					bool axiomAdded = axiomChangePair.second;
					CAxiomExpression* axiom = axiomChangePair.first;
					activeEntityCountUpdater->updateEntitiesForAxioms(axiom,axiomAdded);
					newAxiomsCounted = true;
				}
				expDataBoxMapping->setLastActiveCountedEntityAxiom(newAxiomCount);

				delete activeEntityCountUpdater;
				return newAxiomsCounted;
			}





			CConcreteOntology* CRequirementConfigPreprocessingThread::searchPreviousClassRealizedOntologyVersion(COntologyIncrementalRevisionData* incRevData, CRequirementConfigPreprocessingItem* reqConfPreCompItem) {
				CConcreteOntology* lastRealizedOntology = nullptr;
				CConcreteOntology* prevOntology = incRevData->getPreviousOntologyVersion();
				while (prevOntology && !lastRealizedOntology) {
					CRealization* prevRealizedData = prevOntology->getRealization();
					if (prevRealizedData && prevRealizedData->hasConceptRealization()) {
						lastRealizedOntology = prevOntology;
					}
					if (!lastRealizedOntology) {
						prevOntology = prevOntology->getIncrementalRevisionData()->getPreviousOntologyVersion();
					}
				}
				return lastRealizedOntology;
			}




			CConcreteOntology* CRequirementConfigPreprocessingThread::searchPreviousClassesClassifiedOntologyVersion(COntologyIncrementalRevisionData* incRevData, CRequirementConfigPreprocessingItem* reqConfPreCompItem) {
				CConcreteOntology* lastClassifiedOntology = nullptr;
				CConcreteOntology* prevOntology = incRevData->getPreviousOntologyVersion();
				while (prevOntology && !lastClassifiedOntology) {
					CClassification* prevClassifiedData = prevOntology->getClassification();
					if (prevClassifiedData && prevClassifiedData->hasClassConceptClassification()) {
						lastClassifiedOntology = prevOntology;
					}
					if (!lastClassifiedOntology) {
						prevOntology = prevOntology->getIncrementalRevisionData()->getPreviousOntologyVersion();
					}
				}
				return lastClassifiedOntology;
			}




			CConcreteOntology* CRequirementConfigPreprocessingThread::searchPreviousConsistentOntologyVersion(COntologyIncrementalRevisionData* incRevData, CRequirementConfigPreprocessingItem* reqConfPreCompItem) {
				CConcreteOntology* lastConsOntology = nullptr;
				CConcreteOntology* prevOntology = incRevData->getPreviousOntologyVersion();
				while (prevOntology && !lastConsOntology) {
					CConsistence* prevConsistenceData = prevOntology->getConsistence();
					if (prevConsistenceData && prevConsistenceData->isOntologyConsistent()) {
						if (prevConsistenceData->areAllIndividualsRepresentativelyStored() || prevConsistenceData->hasConsistenceModelData()) {
							lastConsOntology = prevOntology;
						}
					}
					if (!lastConsOntology) {
						prevOntology = prevOntology->getIncrementalRevisionData()->getPreviousOntologyVersion();
					}
				}
				return lastConsOntology;
			}





			bool CRequirementConfigPreprocessingThread::createBuild(CRequirementConfigPreprocessingItem* reqConfPreCompItem) {
				CConcreteOntology* ontology = reqConfPreCompItem->getOntology();
				ontology->getDataBoxes()->setBuildContinuation();
				CConcreteOntologyUpdateBuilder upBuilder(ontology);
				upBuilder.initializeBuilding();
				upBuilder.completeBuilding();
				return false;
			}


			bool CRequirementConfigPreprocessingThread::createPreprocess(CRequirementConfigPreprocessingItem* reqConfPreCompItem) {
				CConcreteOntology* ontology = reqConfPreCompItem->getOntology();
				CConfiguration* config = reqConfPreCompItem->getCalculationConfiguration();

				COntologyConfigDependedPreProcesser ontConDepPreProcessor;
				ontConDepPreProcessor.preprocess(ontology,config);
				return false;
			}


			bool CRequirementConfigPreprocessingThread::preprocessingTested(COntologyPreprocessingItem* ontPreCompItem, CPreprocessingTestingItem* preTestItem, CPreprocessingCalculatedCallbackEvent* pcce) {
				return false;
			}


			bool CRequirementConfigPreprocessingThread::finishOntologyPreprocessing(CRequirementConfigPreprocessingItem* reqConfPreCompItem) {
				if (reqConfPreCompItem->areAllStepFinished()) {
					reqConfPreCompItem->setPreprocessingFinished(true);
					CConcreteOntology* ontology = reqConfPreCompItem->getOntology();

					CPreprocessing* preprocessing = ontology->getPreprocessing();
					preprocessing->setPreprocessed(true);
				}

				mActiveOntItemSet.remove(reqConfPreCompItem);
				mInactiveOntItemSet.insert(reqConfPreCompItem);

				reqConfPreCompItem->doPreprocessingFinishedCallback(true);
				return true;
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
