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

#include "CJNIQueryProcessor.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace JNI {


				CJNIQueryProcessor::CJNIQueryProcessor(CJNIInstanceManager* jniInstanceManager, CJNIOntologyRevisionData* ontRevData) {
					mOntRevData = ontRevData;
					mJNIInstanceManager = jniInstanceManager;
					mJNICommandProcessor = jniInstanceManager->getJNICommandProcessor();
					mExpressionOntRev = nullptr;
					mExpressionBuilder = nullptr;
					mProgressManagersLookedUp = false;
					mReasonerManager = nullptr;
					mClassificationManager = nullptr;
				}

				CJNIQueryProcessor::~CJNIQueryProcessor() {
					// the revision the expressions were built into is left to the reasoner, as the
					// OWLlink interface leaves the revisions of its complex queries, the reasoner does
					// not track a revision that is not installed and its ontology shares its data with
					// the installed one
					delete mExpressionBuilder;
				}


				bool CJNIQueryProcessor::queryReasoningProgress(QVector<double>& values) {
					values.fill(0.,PROGRESS_VALUE_COUNT);
					if (!mProgressManagersLookedUp) {
						// the commander registers both managers in its global configuration, which
						// the command processor asks it for; the lookup goes through the commander's
						// thread, which answers while a calculation runs on the reasoner's
						CConfiguration* config = mJNICommandProcessor->getConfiguration();
						mReasonerManager = CConfigManagerReader::readReasonerManagerConfig(config);
						mClassificationManager = CConfigManagerReader::readClassificationManagerConfig(config);
						mProgressManagersLookedUp = mReasonerManager != nullptr || mClassificationManager != nullptr;
					}
					if (mReasonerManager) {
						CCalculationManager* calculationManager = mReasonerManager->getCalculationManager();
						if (calculationManager) {
							CConcurrentTaskCalculationEnvironment* calcEnv = dynamic_cast<CConcurrentTaskCalculationEnvironment*>(calculationManager->getCalculationContext());
							if (calcEnv) {
								values[0] = (double)calcEnv->getCalculationStatisticTasksProcessedCount();
								values[1] = calcEnv->getCalculationApproximatedRemainingTasksCount();
							}
						}
						CReasonerManagerThread* managerThread = dynamic_cast<CReasonerManagerThread*>(mReasonerManager);
						if (managerThread) {
							CRealizationManager* realizationManager = managerThread->getRealizationManager();
							if (realizationManager) {
								CRealizationProgress* realizationProgress = realizationManager->getRealizationProgress();
								if (realizationProgress) {
									values[6] = realizationProgress->getProgessPercent();
									values[7] = (double)realizationProgress->getTestedClasses();
									values[8] = (double)realizationProgress->getTotalClasses();
									values[9] = realizationProgress->getRemainingMilliSeconds();
								}
							}
						}
					}
					if (mClassificationManager) {
						CClassificationProgress* classificationProgress = mClassificationManager->getClassificationProgress();
						if (classificationProgress) {
							values[2] = classificationProgress->getProgessPercent();
							values[3] = (double)(classificationProgress->getTestedSatisfiable()+classificationProgress->getTestedSubsumptions());
							values[4] = (double)(classificationProgress->getTotalSatisfiable()+classificationProgress->getTotalSubsumptions());
							values[5] = classificationProgress->getRemainingMilliSeconds();
						}
					}
					return mProgressManagersLookedUp;
				}


				bool CJNIQueryProcessor::checkIsOntologyConsistent() {
					bool consistent = false;
					CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
					CIsConsistentQuery* consistencyQuery = new CIsConsistentQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig);
					mJNICommandProcessor->calculateOntologyQuery(consistencyQuery);
					CQueryResult* result = consistencyQuery->getQueryResult();
					if (result) {
						CBooleanQueryResult* boolQueryResult = dynamic_cast<CBooleanQueryResult*>(result);
						consistent = boolQueryResult->getResult();
					}
					delete consistencyQuery;
					return consistent;
				}






				bool CJNIQueryProcessor::queryOntologySubClasses(const QString& className, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CConcept* concept = ont->getStringMapping()->getConceptFromName(className);
					if (concept) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CSubSuperClassesResultVisitCallbackQuery* subSuperClassQuery = new CSubSuperClassesResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,concept,className,true,false,direct);
						mJNICommandProcessor->calculateOntologyQuery(subSuperClassQuery);
						CQueryResult* result = subSuperClassQuery->getQueryResult();
						if (result) {
							subSuperClassQuery->callbackVisitingClasses(visitingCallback);
						}
						delete subSuperClassQuery;
						return result != nullptr;
					}
					return false;
				}


				bool CJNIQueryProcessor::queryOntologySuperClasses(const QString& className, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CConcept* concept = ont->getStringMapping()->getConceptFromName(className);
					if (concept) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CSubSuperClassesResultVisitCallbackQuery* subSuperClassQuery = new CSubSuperClassesResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,concept,className,false,true,direct);
						mJNICommandProcessor->calculateOntologyQuery(subSuperClassQuery);
						CQueryResult* result = subSuperClassQuery->getQueryResult();
						if (result) {
							subSuperClassQuery->callbackVisitingClasses(visitingCallback);
						}
						delete subSuperClassQuery;
						return result != nullptr;
					}
					return false;
				}


				bool CJNIQueryProcessor::queryOntologyEquivalentClasses(const QString& className, CEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CConcept* concept = ont->getStringMapping()->getConceptFromName(className);
					if (concept) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CEquivalentClassesResultVisitCallbackQuery* equivClassQuery = new CEquivalentClassesResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,concept,className);
						mJNICommandProcessor->calculateOntologyQuery(equivClassQuery);
						CQueryResult* result = equivClassQuery->getQueryResult();
						if (result) {
							equivClassQuery->callbackVisitingClasses(visitingCallback);
						}
						delete equivClassQuery;
						return result != nullptr;
					} else {
						LOG(INFO,"::Konclude::JNIQueryProcessor",logTr("Could not resolve concept for '%1'.").arg(className),0);
					}
					return false;
				}










				bool CJNIQueryProcessor::queryOntologySubObjectProperties(const QString& propertyName, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CRole* role = ont->getStringMapping()->getRoleFromName(propertyName);
					if (role) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CSubSuperPropertiesResultVisitCallbackQuery* subSuperPropertiesQuery = new CSubSuperPropertiesResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,role,propertyName,true,false,direct,false);
						mJNICommandProcessor->calculateOntologyQuery(subSuperPropertiesQuery);
						CQueryResult* result = subSuperPropertiesQuery->getQueryResult();
						if (result) {
							subSuperPropertiesQuery->callbackVisitingProperties(visitingCallback);
						}
						delete subSuperPropertiesQuery;
						return result != nullptr;
					}
					return false;
				}


				bool CJNIQueryProcessor::queryOntologySuperObjectProperties(const QString& propertyName, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CRole* role = ont->getStringMapping()->getRoleFromName(propertyName);
					if (role) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CSubSuperPropertiesResultVisitCallbackQuery* subSuperPropertiesQuery = new CSubSuperPropertiesResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,role,propertyName,false,true,direct,false);
						mJNICommandProcessor->calculateOntologyQuery(subSuperPropertiesQuery);
						CQueryResult* result = subSuperPropertiesQuery->getQueryResult();
						if (result) {
							subSuperPropertiesQuery->callbackVisitingProperties(visitingCallback);
						}
						delete subSuperPropertiesQuery;
						return result != nullptr;
					}
					return false;
				}


				bool CJNIQueryProcessor::queryOntologyEquivalentObjectProperties(const QString& propertyName, CEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CRole* role = ont->getStringMapping()->getRoleFromName(propertyName);
					if (role) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CEquivalentPropertiesResultVisitCallbackQuery* equivPropQuery = new CEquivalentPropertiesResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,role,false,propertyName);
						mJNICommandProcessor->calculateOntologyQuery(equivPropQuery);
						CQueryResult* result = equivPropQuery->getQueryResult();
						if (result) {
							equivPropQuery->callbackVisitingProperties(visitingCallback);
						}
						delete equivPropQuery;
						return result != nullptr;
					} else {
						LOG(INFO,"::Konclude::JNIQueryProcessor",logTr("Could not resolve role for '%1'.").arg(propertyName),0);
					}
					return false;
				}







				bool CJNIQueryProcessor::queryOntologyInstances(const QString& className, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CConcept* concept = ont->getStringMapping()->getConceptFromName(className);
					if (concept) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CInstancesResultVisitCallbackQuery* instancesQuery = new CInstancesResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,concept,direct,className);
						mJNICommandProcessor->calculateOntologyQuery(instancesQuery);
						CQueryResult* result = instancesQuery->getQueryResult();
						if (result) {
							instancesQuery->callbackVisitingInstances(visitingCallback);
						}
						delete instancesQuery;
						return result != nullptr;
					} else {
						LOG(INFO,"::Konclude::JNIQueryProcessor",logTr("Could not resolve concept for '%1'.").arg(className),0);
					}
					return false;
				}




				bool CJNIQueryProcessor::queryOntologyTypes(const QString& indiName, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CIndividual* indi = ont->getStringMapping()->getIndividualFromName(indiName);
					if (indi) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CTypesResultVisitCallbackQuery* typesQuery = new CTypesResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,indi,direct,indiName);
						mJNICommandProcessor->calculateOntologyQuery(typesQuery);
						CQueryResult* result = typesQuery->getQueryResult();
						if (result) {
							typesQuery->callbackVisitingTypes(visitingCallback);
						}
						delete typesQuery;
						return result != nullptr;
					} else {
						LOG(INFO,"::Konclude::JNIQueryProcessor",logTr("Could not resolve individual for '%1'.").arg(indiName),0);
					}
					return false;
				}



				bool CJNIQueryProcessor::queryOntologySameIndividuals(const QString& indiName, CEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CIndividual* indi = ont->getStringMapping()->getIndividualFromName(indiName);
					if (indi) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CSameIndividualsResultVisitCallbackQuery* sameIndiQuery = new CSameIndividualsResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,indi,indiName);
						mJNICommandProcessor->calculateOntologyQuery(sameIndiQuery);
						CQueryResult* result = sameIndiQuery->getQueryResult();
						if (result) {
							sameIndiQuery->callbackVisitingInstances(visitingCallback);
						}
						delete sameIndiQuery;
						return result != nullptr;
					}
					return false;
				}




				bool CJNIQueryProcessor::queryOntologyObjectPropertySources(const QString& indiName, const QString& propertyName, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CIndividual* indi = ont->getStringMapping()->getIndividualFromName(indiName);
					CRole* role = ont->getStringMapping()->getRoleFromName(propertyName);
					if (indi && role) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CObjectPropertySourcesTargetsResultVisitCallbackQuery* propertySouTarQuery = new CObjectPropertySourcesTargetsResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,indi,role,true,false);
						mJNICommandProcessor->calculateOntologyQuery(propertySouTarQuery);
						CQueryResult* result = propertySouTarQuery->getQueryResult();
						if (result) {
							propertySouTarQuery->callbackVisitingIndividuals(visitingCallback);
						}
						delete propertySouTarQuery;
						return result != nullptr;
					}
					return false;
				}




				bool CJNIQueryProcessor::queryOntologyObjectPropertyTargets(const QString& indiName, const QString& propertyName, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CIndividual* indi = ont->getStringMapping()->getIndividualFromName(indiName);
					CRole* role = ont->getStringMapping()->getRoleFromName(propertyName);
					if (indi && role) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CObjectPropertySourcesTargetsResultVisitCallbackQuery* propertySouTarQuery = new CObjectPropertySourcesTargetsResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,indi,role,false,true);
						mJNICommandProcessor->calculateOntologyQuery(propertySouTarQuery);
						CQueryResult* result = propertySouTarQuery->getQueryResult();
						if (result) {
							propertySouTarQuery->callbackVisitingIndividuals(visitingCallback);
						}
						delete propertySouTarQuery;
						return result != nullptr;
					}
					return false;
				}





				CJNIAxiomExpressionVisitingLoader* CJNIQueryProcessor::getClassExpressionBuilder() {
					if (!mExpressionBuilder) {
						COntologyRevision* installedOntRev = mOntRevData->getOntologyRevision();
						mExpressionOntRev = mJNICommandProcessor->getNewOntologyRevision(installedOntRev);
						if (!mExpressionOntRev) {
							LOG(ERROR,"::Konclude::JNIQueryProcessor",logTr("Could not create the ontology revision for the class expressions of the queries."),0);
							return nullptr;
						}
						// the installed revision data keeps the entity mapping, so an entity that is
						// first mentioned in a query is recorded where the results are resolved
						mExpressionBuilder = new CJNIQueryExpressionBuildingLoader(mJNIInstanceManager,mOntRevData,mExpressionOntRev->getOntology());
						mExpressionBuilder->initializeBuilding();
					}
					return mExpressionBuilder;
				}


				CClassTermExpression* CJNIQueryProcessor::getQueryClassTermExpression(CBuildExpression* classExpression, QString& errorMessage) {
					if (!classExpression) {
						errorMessage = QString("No class expression was handed over for the query.");
						return nullptr;
					}
					if (!mExpressionBuilder) {
						errorMessage = QString("The class expression builder of the querying bridge has not been initialised, so the class expression cannot belong to it.");
						return nullptr;
					}
					CClassTermExpression* classTermExp = dynamic_cast<CClassTermExpression*>(classExpression);
					if (!classTermExp) {
						errorMessage = QString("The expression handed over for the query is not a class expression.");
						return nullptr;
					}
					return classTermExp;
				}


				CQueryResult* CJNIQueryProcessor::calculateClassExpressionQuery(CComplexConceptAnsweringQuery* query, QString& errorMessage) {
					mJNICommandProcessor->calculateOntologyQuery(query);
					CQueryResult* result = query->getQueryResult();
					if (query->hasError() || !result) {
						QStringList errorStringList = query->getErrorStringList();
						if (errorStringList.isEmpty()) {
							errorMessage = QString("The query '%1' about a class expression was not answered.").arg(query->getQueryName());
						} else {
							errorMessage = QString("The query '%1' about a class expression failed: %2").arg(query->getQueryName()).arg(errorStringList.join("; "));
						}
						return nullptr;
					}
					return result;
				}


				void CJNIQueryProcessor::visitClassName(const QString& className, CEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CClassExpression* classExp = ont->getBuildData()->getClassEntityBuildHash()->value(className,nullptr);
					if (classExp) {
						visitingCallback->visitEntityExpression(classExp,ont);
					}
				}


				void CJNIQueryProcessor::visitIndividualName(const QString& individualName, CEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CNamedIndividualExpression* indiExp = ont->getBuildData()->getIndividualEntityBuildHash()->value(individualName,nullptr);
					if (indiExp) {
						visitingCallback->visitEntityExpression(indiExp,ont);
					}
				}



				bool CJNIQueryProcessor::checkIsClassExpressionSatisfiable(CBuildExpression* classExpression, bool& satisfiable, QString& errorMessage) {
					CClassTermExpression* classTermExp = getQueryClassTermExpression(classExpression,errorMessage);
					if (!classTermExp) {
						return false;
					}
					COntologyRevision* ontRev = mOntRevData->getOntologyRevision();
					CComplexSatisfiabilityAnsweringQuery* query = new CComplexSatisfiabilityAnsweringQuery(ontRev->getOntology(),mExpressionOntRev->getOntology(),classTermExp,ontRev->getOntologyConfiguration(),QString("JNI-Class-Expression-Satisfiability-Query"));
					CQueryResult* result = calculateClassExpressionQuery(query,errorMessage);
					CBooleanQueryResult* boolResult = dynamic_cast<CBooleanQueryResult*>(result);
					if (boolResult) {
						satisfiable = boolResult->getResult();
					} else if (result) {
						errorMessage = QString("The satisfiability query about a class expression was answered with an unexpected result.");
					}
					delete query;
					return boolResult != nullptr;
				}


				bool CJNIQueryProcessor::computeClassExpressionEquivalentClassNames(CClassTermExpression* classTermExp, QStringList& classNames, QString& errorMessage) {
					COntologyRevision* ontRev = mOntRevData->getOntologyRevision();
					CComplexEquivalentClassesAnsweringQuery* query = new CComplexEquivalentClassesAnsweringQuery(ontRev->getOntology(),mExpressionOntRev->getOntology(),classTermExp,ontRev->getOntologyConfiguration(),QString("JNI-Class-Expression-EquivalentClasses-Query"));
					CQueryResult* result = calculateClassExpressionQuery(query,errorMessage);
					CClassSetResult* classSetResult = dynamic_cast<CClassSetResult*>(result);
					if (classSetResult) {
						for (const QString& className : *classSetResult->getClassesSet()) {
							classNames.append(className);
						}
					} else if (result) {
						errorMessage = QString("The equivalent classes query about a class expression was answered with an unexpected result.");
					}
					delete query;
					return classSetResult != nullptr;
				}


				bool CJNIQueryProcessor::queryClassExpressionSubSuperClasses(CBuildExpression* classExpression, bool subClasses, bool superClasses, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback, QString& errorMessage) {
					CClassTermExpression* classTermExp = getQueryClassTermExpression(classExpression,errorMessage);
					if (!classTermExp) {
						return false;
					}
					COntologyRevision* ontRev = mOntRevData->getOntologyRevision();
					CConcreteOntology* ont = ontRev->getOntology();

					// the complex query reports a class that is equivalent to the expression among its sub
					// and its super classes, and for the direct ones reports nothing else, while the OWL API
					// asks for the strict ones. An expression that is equivalent to a class therefore has
					// the sub and super classes of that class, which the class hierarchy answers directly.
					QStringList equivalentClassNames;
					if (!computeClassExpressionEquivalentClassNames(classTermExp,equivalentClassNames,errorMessage)) {
						return false;
					}
					for (const QString& className : equivalentClassNames) {
						CConcept* concept = ont->getStringMapping()->getConceptFromName(className);
						if (concept) {
							CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(ontRev->getOntologyConfiguration());
							CSubSuperClassesResultVisitCallbackQuery* subSuperClassQuery = new CSubSuperClassesResultVisitCallbackQuery(ont,calcConfig,concept,className,subClasses,superClasses,direct);
							mJNICommandProcessor->calculateOntologyQuery(subSuperClassQuery);
							CQueryResult* result = subSuperClassQuery->getQueryResult();
							if (result) {
								subSuperClassQuery->callbackVisitingClasses(visitingCallback);
							} else {
								errorMessage = QString("The sub/super classes query for '%1', which is equivalent to the asked class expression, was not answered.").arg(className);
							}
							delete subSuperClassQuery;
							return result != nullptr;
						}
					}

					// the complex query is always asked for all sub or super classes, its direct ones are
					// not minimal, a class below another one of the answer is reported as direct as well,
					// so the direct ones are taken from the class hierarchy: a sub class of the answer is
					// direct if none of its parents is in the answer, a super class if none of its children
					CComplexConceptAnsweringQuery* query = nullptr;
					if (subClasses) {
						query = new CComplexSubClassesAnsweringQuery(ont,mExpressionOntRev->getOntology(),classTermExp,ontRev->getOntologyConfiguration(),QString("JNI-Class-Expression-SubClasses-Query"));
					} else {
						query = new CComplexSuperClassesAnsweringQuery(ont,mExpressionOntRev->getOntology(),classTermExp,ontRev->getOntologyConfiguration(),QString("JNI-Class-Expression-SuperClasses-Query"));
					}
					query->setDirect(false);
					CQueryResult* result = calculateClassExpressionQuery(query,errorMessage);
					CClassSynsetsResult* synsetsResult = dynamic_cast<CClassSynsetsResult*>(result);
					if (synsetsResult) {
						QList<CHierarchyNode*> nodeList;
						QSet<CHierarchyNode*> nodeSet;
						CTaxonomy* taxonomy = ont->getClassification()->getClassConceptClassification()->getClassConceptTaxonomy();
						for (CClassSynsetResult* synset : *synsetsResult->getClassSynsetList()) {
							CHierarchyNode* node = nullptr;
							for (const QString& className : synset->getEquivalentClassNameList()) {
								CConcept* concept = ont->getStringMapping()->getConceptFromName(className);
								if (concept) {
									node = taxonomy->getHierarchyNode(concept);
									if (node) {
										break;
									}
								}
							}
							if (node && !nodeSet.contains(node)) {
								nodeSet.insert(node);
								nodeList.append(node);
							}
						}
						for (CHierarchyNode* node : nodeList) {
							if (direct) {
								bool nodeDirect = true;
								QSet<CHierarchyNode*>* neighbourNodeSet = subClasses ? node->getParentNodeSet() : node->getChildNodeSet();
								for (CHierarchyNode* neighbourNode : *neighbourNodeSet) {
									if (nodeSet.contains(neighbourNode)) {
										nodeDirect = false;
										break;
									}
								}
								if (!nodeDirect) {
									continue;
								}
							}
							visitingCallback->startEntityExpressionSet(ont);
							for (CConcept* concept : *node->getEquivalentConceptList()) {
								visitingCallback->visitConceptAssociatedEntityExpression(concept,ont);
							}
							visitingCallback->endEntityExpressionSet(ont);
						}
					} else if (result) {
						errorMessage = QString("The sub/super classes query about a class expression was answered with an unexpected result.");
					}
					delete query;
					return synsetsResult != nullptr;
				}


				bool CJNIQueryProcessor::queryClassExpressionSubClasses(CBuildExpression* classExpression, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback, QString& errorMessage) {
					return queryClassExpressionSubSuperClasses(classExpression,true,false,direct,visitingCallback,errorMessage);
				}


				bool CJNIQueryProcessor::queryClassExpressionSuperClasses(CBuildExpression* classExpression, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback, QString& errorMessage) {
					return queryClassExpressionSubSuperClasses(classExpression,false,true,direct,visitingCallback,errorMessage);
				}


				bool CJNIQueryProcessor::queryClassExpressionEquivalentClasses(CBuildExpression* classExpression, CEntityExpressionSetResultVisitingCallback* visitingCallback, QString& errorMessage) {
					CClassTermExpression* classTermExp = getQueryClassTermExpression(classExpression,errorMessage);
					if (!classTermExp) {
						return false;
					}
					QStringList equivalentClassNames;
					if (!computeClassExpressionEquivalentClassNames(classTermExp,equivalentClassNames,errorMessage)) {
						return false;
					}
					for (const QString& className : equivalentClassNames) {
						visitClassName(className,visitingCallback);
					}
					return true;
				}


				bool CJNIQueryProcessor::queryClassExpressionInstances(CBuildExpression* classExpression, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback, QString& errorMessage) {
					CClassTermExpression* classTermExp = getQueryClassTermExpression(classExpression,errorMessage);
					if (!classTermExp) {
						return false;
					}
					COntologyRevision* ontRev = mOntRevData->getOntologyRevision();
					CComplexIndividualInstancesAnsweringQuery* query = new CComplexIndividualInstancesAnsweringQuery(ontRev->getOntology(),mExpressionOntRev->getOntology(),classTermExp,ontRev->getOntologyConfiguration(),QString("JNI-Class-Expression-Instances-Query"));
					query->setDirect(direct);
					query->setFlattened(false);
					CQueryResult* result = calculateClassExpressionQuery(query,errorMessage);
					CIndividualSynsetsResult* synsetsResult = dynamic_cast<CIndividualSynsetsResult*>(result);
					if (synsetsResult) {
						CConcreteOntology* ont = ontRev->getOntology();
						for (CIndividualSynsetResult* synset : *synsetsResult->getIndividualSynsetList()) {
							visitingCallback->startEntityExpressionSet(ont);
							for (const QString& individualName : synset->getEquivalentIndividualNameList()) {
								visitIndividualName(individualName,visitingCallback);
							}
							visitingCallback->endEntityExpressionSet(ont);
						}
					} else if (result) {
						errorMessage = QString("The instances query about a class expression was answered with an unexpected result.");
					}
					delete query;
					return synsetsResult != nullptr;
				}






			}; // end namespace JNI 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
