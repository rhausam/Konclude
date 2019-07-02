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

#include "CConcreteOntologyQueryBasicBuilder.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {


			CConcreteOntologyQueryBasicBuilder::CConcreteOntologyQueryBasicBuilder(CConcreteOntology* ontology, CConfigurationBase *configuration) {
				config = configuration;
				mOntology = ontology;
			}



			CConcreteOntologyQueryBasicBuilder::~CConcreteOntologyQueryBasicBuilder() {
			}


			CQueryIsClassSatisfiableExpression* CConcreteOntologyQueryBasicBuilder::getIsClassSatisfiableQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName) {
				CQueryIsClassSatisfiableExpression* expression = nullptr;
				CExpressionSplitter expSplitter(testClassExpressions);
				if (expSplitter.testForExpressionComposition(1)) {
					expression = getIsClassSatisfiableQuery(expSplitter.getFirstClassTermExpression(),queryName);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Couldn't match parameters for 'IsClassSatisfiableQuery'-Expression."),this);
				}
				return expression;
			}

			CQueryAreClassesEquivalentExpression* CConcreteOntologyQueryBasicBuilder::getAreClassesEquivalenceQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName) {
				CQueryAreClassesEquivalentExpression* expression = nullptr;
				CExpressionSplitter expSplitter(testClassExpressions);
				if (expSplitter.testForExpressionComposition(2)) {
					expression = getAreClassesEquivalenceQuery(*expSplitter.getClassTermExpressionList(),queryName);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Couldn't match parameters for 'AreClassesEquivalentQuery'-Expression."),this);
				}
				return expression;
			}

			CQueryAreClassesDisjointExpression* CConcreteOntologyQueryBasicBuilder::getAreClassesDisjointQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName) {
				CQueryAreClassesDisjointExpression* expression = nullptr;
				CExpressionSplitter expSplitter(testClassExpressions);
				if (expSplitter.testForExpressionComposition(2)) {
					expression = getAreClassesDisjointQuery(*expSplitter.getClassTermExpressionList(),queryName);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Couldn't match parameters for 'AreClassesDisjointQuery'-Expression."),this);
				}
				return expression;
			}


			CQueryIsClassSubsumedByExpression* CConcreteOntologyQueryBasicBuilder::getIsClassSubsumedByQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName) {
				CQueryIsClassSubsumedByExpression* expression = nullptr;
				CExpressionSplitter expSplitter(testClassExpressions);
				if (expSplitter.testForExpressionComposition(2)) {
					expression = getIsClassSubsumedByQuery(expSplitter.getFirstClassTermExpression(),expSplitter.getSecondClassTermExpression(),queryName);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Couldn't match parameters for 'IsClassSubsumedByQuery'-Expression."),this);
				}
				return expression;
			}

			CQueryIsInstanceOfExpression* CConcreteOntologyQueryBasicBuilder::getIsInstanceOfQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName) {
				CQueryIsInstanceOfExpression* expression = nullptr;
				CExpressionSplitter expSplitter(testClassExpressions);
				if (expSplitter.testForExpressionComposition(1,0,1)) {
					expression = getIsInstanceOfQuery(expSplitter.getFirstIndividualTermExpression(),expSplitter.getFirstClassTermExpression(),queryName);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Couldn't match parameters for 'IsInstanceOfQuery'-Expression."),this);
				}
				return expression;
			}

			
			CQueryGetFlattenedTypesExpression* CConcreteOntologyQueryBasicBuilder::getGetFlattenedTypesQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, bool direct, const QString& queryName) {
				CQueryGetFlattenedTypesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(testClassExpressions);
				if (expSplitter.testForExpressionComposition(0,0,1)) {
					expression = getGetFlattenedTypesQuery(expSplitter.getFirstIndividualTermExpression(),direct,queryName);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Couldn't match parameters for 'GetFlattenedTypesQuery'-Expression."),this);
				}
				return expression;
			}

			CQueryGetFlattenedInstancesExpression* CConcreteOntologyQueryBasicBuilder::getGetFlattenedInstancesQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, bool direct, const QString& queryName) {
				CQueryGetFlattenedInstancesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(testClassExpressions);
				if (expSplitter.testForExpressionComposition(1,0,0)) {
					expression = getGetFlattenedInstancesQuery(expSplitter.getFirstClassTermExpression(),direct,queryName);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Couldn't match parameters for 'GetFlattenedInstancesQuery'-Expression."),this);
				}
				return expression;
			}



			CQueryGetTypesExpression* CConcreteOntologyQueryBasicBuilder::getGetTypesQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, bool direct, const QString& queryName) {
				CQueryGetTypesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(testClassExpressions);
				if (expSplitter.testForExpressionComposition(0,0,1)) {
					expression = getGetTypesQuery(expSplitter.getFirstIndividualTermExpression(),direct,queryName);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Couldn't match parameters for 'GetTypesQuery'-Expression."),this);
				}
				return expression;
			}

			CQueryGetSameIndividualsExpression* CConcreteOntologyQueryBasicBuilder::getGetSameIndividualsQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName) {
				CQueryGetSameIndividualsExpression* expression = nullptr;
				CExpressionSplitter expSplitter(testClassExpressions);
				if (expSplitter.testForExpressionComposition(0,0,1)) {
					expression = getGetSameIndividualsQuery(expSplitter.getFirstIndividualTermExpression(),queryName);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Couldn't match parameters for 'GetSameIndividualsQuery'-Expression."),this);
				}
				return expression;
			}

			CQueryGetInstancesExpression* CConcreteOntologyQueryBasicBuilder::getGetInstancesQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, bool direct, const QString& queryName) {
				CQueryGetInstancesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(testClassExpressions);
				if (expSplitter.testForExpressionComposition(1,0,0)) {
					expression = getGetInstancesQuery(expSplitter.getFirstClassTermExpression(),direct,queryName);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Couldn't match parameters for 'GetInstancesQuery'-Expression."),this);
				}
				return expression;
			}



			CQueryGetObjectPropertyTargetsExpression* CConcreteOntologyQueryBasicBuilder::getGetObjectPropertyTargetsQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName) {
				CQueryGetObjectPropertyTargetsExpression* expression = nullptr;
				CExpressionSplitter expSplitter(testClassExpressions);
				if (expSplitter.testForExpressionComposition(0,1,1)) {
					expression = getGetObjectPropertyTargetsQuery(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstIndividualTermExpression(),queryName);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Couldn't match parameters for 'GetObjectPropertyTargets'-Expression."),this);
				}
				return expression;
			}


			CQueryGetFlattenedObjectPropertyTargetsExpression* CConcreteOntologyQueryBasicBuilder::getGetFlattenedObjectPropertyTargetsQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName) {
				CQueryGetFlattenedObjectPropertyTargetsExpression* expression = nullptr;
				CExpressionSplitter expSplitter(testClassExpressions);
				if (expSplitter.testForExpressionComposition(0,1,1)) {
					expression = getGetFlattenedObjectPropertyTargetsQuery(expSplitter.getFirstObjectPropertyTermExpression(),expSplitter.getFirstIndividualTermExpression(),queryName);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Couldn't match parameters for 'GetFlattenedObjectPropertyTargets'-Expression."),this);
				}
				return expression;
			}

			CQueryGetSubClassesExpression* CConcreteOntologyQueryBasicBuilder::getGetSubClassesQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, bool direct, const QString& queryName) {
				CQueryGetSubClassesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(testClassExpressions);
				if (expSplitter.testForExpressionComposition(1,0,0)) {
					expression = getGetSubClassesQuery(expSplitter.getFirstClassTermExpression(),direct,queryName);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Couldn't match parameters for 'GetSubClassesQuery'-Expression."),this);
				}
				return expression;
			}

			CQueryGetSuperClassesExpression* CConcreteOntologyQueryBasicBuilder::getGetSuperClassesQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, bool direct, const QString& queryName) {
				CQueryGetSuperClassesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(testClassExpressions);
				if (expSplitter.testForExpressionComposition(1,0,0)) {
					expression = getGetSuperClassesQuery(expSplitter.getFirstClassTermExpression(),direct,queryName);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Couldn't match parameters for 'GetSuperClassesQuery'-Expression."),this);
				}
				return expression;
			}

			CQueryGetEquivalentClassesExpression* CConcreteOntologyQueryBasicBuilder::getGetEquivalentClassesQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName) {
				CQueryGetEquivalentClassesExpression* expression = nullptr;
				CExpressionSplitter expSplitter(testClassExpressions);
				if (expSplitter.testForExpressionComposition(1,0,0)) {
					expression = getGetEquivalentClassesQuery(expSplitter.getFirstClassTermExpression(),queryName);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Couldn't match parameters for 'GetEquivalentClassesQuery'-Expression."),this);
				}
				return expression;
			}





			CQueryGetDeterministicIndividualsExpression* CConcreteOntologyQueryBasicBuilder::getGetDeterministicIndividualsQuery(const CEXPRESSIONLIST<CBuildExpression*>& expressions, const QString& queryName) {
				CQueryGetDeterministicIndividualsExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				expression = getGetDeterministicIndividualsQuery(*expSplitter.getIndividualTermExpressionList(),queryName);
				return expression;
			}

			CQueryGetNondeterministicIndividualsExpression* CConcreteOntologyQueryBasicBuilder::getGetNondeterministicIndividualsQuery(const CEXPRESSIONLIST<CBuildExpression*>& expressions, const QString& queryName) {
				CQueryGetNondeterministicIndividualsExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				expression = getGetNondeterministicIndividualsQuery(*expSplitter.getIndividualTermExpressionList(),queryName);
				return expression;
			}

			CQueryGetDeterministicClassAssertionsExpression* CConcreteOntologyQueryBasicBuilder::getGetDeterministicClassAssertionsQuery(const CEXPRESSIONLIST<CBuildExpression*>& expressions, const QString& queryName) {
				CQueryGetDeterministicClassAssertionsExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				expression = getGetDeterministicClassAssertionsQuery(*expSplitter.getIndividualTermExpressionList(),queryName);
				return expression;
			}

			CQueryGetNondeterministicClassAssertionsExpression* CConcreteOntologyQueryBasicBuilder::getGetNondeterministicClassAssertionsQuery(const CEXPRESSIONLIST<CBuildExpression*>& expressions, const QString& queryName) {
				CQueryGetNondeterministicClassAssertionsExpression* expression = nullptr;
				CExpressionSplitter expSplitter(expressions);
				expression = getGetNondeterministicClassAssertionsQuery(*expSplitter.getIndividualTermExpressionList(),queryName);
				return expression;
			}





			CQueryIsEntailedExpression* CConcreteOntologyQueryBasicBuilder::getIsEntailedQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName) {
				CQueryIsEntailedExpression* expression = nullptr;
				CEXPRESSIONLIST<CAxiomExpression*> axiomExpressionList;
				for (CEXPRESSIONLIST<CBuildExpression*>::const_iterator it = testClassExpressions.constBegin(), itEnd = testClassExpressions.constEnd(); it != itEnd; ++it) {
					CAxiomExpression* axiomExpression = dynamic_cast<CAxiomExpression*>(*it);
					if (axiomExpression) {
						axiomExpressionList.append(axiomExpression);
					}
				}
				if (!axiomExpressionList.isEmpty()) {
					expression = getIsEntailedQuery(axiomExpressionList,queryName);
				} else {
					LOG(ERROR,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Couldn't match parameters for 'IsEntailedQuery'-Expression."),this);
				}
				return expression;
			}


			CQueryIsClassSatisfiableExpression *CConcreteOntologyQueryBasicBuilder::getIsClassSatisfiableQuery(CClassTermExpression* testClassExpressions, const QString& queryName) {
				CQueryIsClassSatisfiableExpression *classSatExp = new CQueryIsClassSatisfiableExpression(queryName,testClassExpressions);
				container.append(classSatExp);
				classSatExpList.append(classSatExp);
				return classSatExp;
			}


			CQueryIsInstanceOfExpression *CConcreteOntologyQueryBasicBuilder::getIsInstanceOfQuery(CIndividualTermExpression* individualTermExpression, CClassTermExpression* classExpression, const QString& queryName) {
				CQueryIsInstanceOfExpression *instanceQExp = 0;
				instanceQExp = new CQueryIsInstanceOfExpression(queryName,individualTermExpression,classExpression);
				instanceExpList.append(instanceQExp);
				container.append(instanceQExp);
				return instanceQExp;
			}


			CQueryGetFlattenedTypesExpression* CConcreteOntologyQueryBasicBuilder::getGetFlattenedTypesQuery(CIndividualTermExpression* individualTermExpression, bool direct, const QString& queryName) {
				CQueryGetFlattenedTypesExpression* flattenedTypes = 0;
				flattenedTypes = new CQueryGetFlattenedTypesExpression(queryName,direct,individualTermExpression);
				flattenedTypesExpList.append(flattenedTypes);
				container.append(flattenedTypes);
				return flattenedTypes;
			}


			CQueryGetFlattenedInstancesExpression* CConcreteOntologyQueryBasicBuilder::getGetFlattenedInstancesQuery(CClassTermExpression* classTermExpression, bool direct, const QString& queryName) {
				CQueryGetFlattenedInstancesExpression* flattenedInstances = 0;
				flattenedInstances = new CQueryGetFlattenedInstancesExpression(queryName,direct,classTermExpression);
				flattenedInstancesExpList.append(flattenedInstances);
				container.append(flattenedInstances);
				return flattenedInstances;
			}


			CQueryGetTypesExpression* CConcreteOntologyQueryBasicBuilder::getGetTypesQuery(CIndividualTermExpression* individualTermExpression, bool direct, const QString& queryName) {
				CQueryGetTypesExpression* types = 0;
				types = new CQueryGetTypesExpression(queryName,direct,individualTermExpression);
				typesExpList.append(types);
				container.append(types);
				return types;
			}


			CQueryGetSameIndividualsExpression* CConcreteOntologyQueryBasicBuilder::getGetSameIndividualsQuery(CIndividualTermExpression* individualTermExpression, const QString& queryName) {
				CQueryGetSameIndividualsExpression* types = 0;
				types = new CQueryGetSameIndividualsExpression(queryName,individualTermExpression);
				sameIndiExpList.append(types);
				container.append(types);
				return types;
			}

			CQueryGetInstancesExpression* CConcreteOntologyQueryBasicBuilder::getGetInstancesQuery(CClassTermExpression* classTermExpression, bool direct, const QString& queryName) {
				CQueryGetInstancesExpression* instances = 0;
				instances = new CQueryGetInstancesExpression(queryName,direct,classTermExpression);
				instancesExpList.append(instances);
				container.append(instances);
				return instances;
			}

 
			CQueryGetObjectPropertyTargetsExpression* CConcreteOntologyQueryBasicBuilder::getGetObjectPropertyTargetsQuery(CObjectPropertyTermExpression* objectPropertyExpression, CIndividualTermExpression* individualExpression, const QString& queryName) {
				CQueryGetObjectPropertyTargetsExpression* instances = 0;
				instances = new CQueryGetObjectPropertyTargetsExpression(queryName,objectPropertyExpression,individualExpression);
				mObjectPropTargetsExpList.append(instances);
				container.append(instances);
				return instances;
			}

			CQueryGetFlattenedObjectPropertyTargetsExpression* CConcreteOntologyQueryBasicBuilder::getGetFlattenedObjectPropertyTargetsQuery(CObjectPropertyTermExpression* objectPropertyExpression, CIndividualTermExpression* individualExpression, const QString& queryName) {
				CQueryGetFlattenedObjectPropertyTargetsExpression* instances = 0;
				instances = new CQueryGetFlattenedObjectPropertyTargetsExpression(queryName,objectPropertyExpression,individualExpression);
				mObjectFlattenedPropTargetsExpList.append(instances);
				container.append(instances);
				return instances;
			}

			CQueryGetSubClassesExpression* CConcreteOntologyQueryBasicBuilder::getGetSubClassesQuery(CClassTermExpression* classTermExpression, bool direct, const QString& queryName) {
				CQueryGetSubClassesExpression* instances = 0;
				instances = new CQueryGetSubClassesExpression(queryName,direct,classTermExpression);
				mSubClassesExpList.append(instances);
				container.append(instances);
				return instances;
			}


			CQueryGetSuperClassesExpression* CConcreteOntologyQueryBasicBuilder::getGetSuperClassesQuery(CClassTermExpression* classTermExpression, bool direct, const QString& queryName) {
				CQueryGetSuperClassesExpression* instances = 0;
				instances = new CQueryGetSuperClassesExpression(queryName,direct,classTermExpression);
				mSuperClassesExpList.append(instances);
				container.append(instances);
				return instances;
			}

			CQueryGetEquivalentClassesExpression* CConcreteOntologyQueryBasicBuilder::getGetEquivalentClassesQuery(CClassTermExpression* classTermExpression, const QString& queryName) {
				CQueryGetEquivalentClassesExpression* instances = 0;
				instances = new CQueryGetEquivalentClassesExpression(queryName,classTermExpression);
				mEquivClassesExpList.append(instances);
				container.append(instances);
				return instances;
			}






			CQueryGetDeterministicIndividualsExpression* CConcreteOntologyQueryBasicBuilder::getGetDeterministicIndividualsQuery(const CEXPRESSIONLIST<CIndividualTermExpression*>& indiExpressions, const QString& queryName) {
				CQueryGetDeterministicIndividualsExpression* queryExpression = nullptr;
				queryExpression = new CQueryGetDeterministicIndividualsExpression(queryName,indiExpressions);
				mDetIndisExpList.append(queryExpression);
				container.append(queryExpression);
				return queryExpression;
			}


			CQueryGetNondeterministicIndividualsExpression* CConcreteOntologyQueryBasicBuilder::getGetNondeterministicIndividualsQuery(const CEXPRESSIONLIST<CIndividualTermExpression*>& indiExpressions, const QString& queryName) {
				CQueryGetNondeterministicIndividualsExpression* queryExpression = nullptr;
				queryExpression = new CQueryGetNondeterministicIndividualsExpression(queryName,indiExpressions);
				mNdetIndisExpList.append(queryExpression);
				container.append(queryExpression);
				return queryExpression;
			}


			CQueryGetDeterministicClassAssertionsExpression* CConcreteOntologyQueryBasicBuilder::getGetDeterministicClassAssertionsQuery(const CEXPRESSIONLIST<CIndividualTermExpression*>& indiExpressions, const QString& queryName) {
				CQueryGetDeterministicClassAssertionsExpression* queryExpression = nullptr;
				queryExpression = new CQueryGetDeterministicClassAssertionsExpression(queryName,indiExpressions);
				mDetClassAssExpList.append(queryExpression);
				container.append(queryExpression);
				return queryExpression;
			}


			CQueryGetNondeterministicClassAssertionsExpression* CConcreteOntologyQueryBasicBuilder::getGetNondeterministicClassAssertionsQuery(const CEXPRESSIONLIST<CIndividualTermExpression*>& indiExpressions, const QString& queryName) {
				CQueryGetNondeterministicClassAssertionsExpression* queryExpression = nullptr;
				queryExpression = new CQueryGetNondeterministicClassAssertionsExpression(queryName,indiExpressions);
				mNdetClassAssExpList.append(queryExpression);
				container.append(queryExpression);
				return queryExpression;
			}




			QString CConcreteOntologyQueryBasicBuilder::getStringFromConcept(CConcept *concept) {
				QString className;
				if (concept) {
					if (concept->hasClassName()) {
						QString iriClassNameString = CIRIName::getRecentIRIName(concept->getClassNameLinker());
						className = iriClassNameString;
					} else {
						className = CConceptTextFormater::getConceptString(concept,false);
					}
				}
				return className;
			}


			CConcept *CConcreteOntologyQueryBasicBuilder::getConceptFromBuildExpression(CClassTermExpression *buildExp) {
				CConcept *concept = mOntology->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value(buildExp,nullptr);
				return concept;
			}


			CRole *CConcreteOntologyQueryBasicBuilder::getRoleFromBuildExpression(CObjectPropertyTermExpression *buildExp) {
				CRole* role = mOntology->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(buildExp,nullptr);
				return role;
			}

			CIndividual *CConcreteOntologyQueryBasicBuilder::getIndividualFromBuildExpression(CIndividualTermExpression *buildExp) {
				CIndividual *indi = mOntology->getDataBoxes()->getExpressionDataBoxMapping()->getIndividulTermIndiMappingHash()->value(buildExp,nullptr);
				return indi;
			}





			QList<CQuery*> CConcreteOntologyQueryBasicBuilder::generateQuerys() {
				QList<CQuery*> queryList;

				CSatisfiableCalculationJobGenerator* satCalcJobGenerator = new CSatisfiableCalculationJobGenerator(mOntology);

				bool confBuildQueryStats = CConfigDataReader::readConfigBoolean(config,"Konclude.Query.Statistics.CollectStatistics",false);

				foreach (CQueryIsClassSatisfiableExpression *classSatExp, classSatExpList) {
					QString queryName = classSatExp->getName();
					CCalculationConfigurationExtension *calcConfig = new CCalculationConfigurationExtension(config,0);
					CClassTermExpression *satBuildExp = classSatExp->getClassTermExpression();
					CConcept *concept = getConceptFromBuildExpression(satBuildExp);
					if (concept) {
						QString className = getStringFromConcept(concept);

						CIsClassSatisfiableQuery *query = new CIsClassSatisfiableQuery(className,queryName);
						CSatisfiableCalculationJob* satCalcJob = satCalcJobGenerator->getSatisfiableCalculationJob(concept);
						CQueryStatisticsCollectionStrings* queryStats = nullptr;
						if (confBuildQueryStats) {
							queryStats = new CQueryStatisticsCollectionStrings();
							satCalcJob->setCalclulationStatisticsCollector(queryStats->createCalculationStatisticsCollection());
						}
						query->setCalculationConfiguration(calcConfig);
						satCalcJob->setCalculationConfiguration(calcConfig);
						query->addTestSatisfiableCalculationJob(satCalcJob,true);
						query->setQueryStatistics(queryStats);

						queryList.append(query);
						LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Generated IsClassSatisfiable-Query '%1' with question '%2'.").arg(query->getQueryName()).arg(query->getQueryString()),this);
					}
					delete classSatExp;
				}




				foreach (CQueryIsInstanceOfExpression *instanceExp, instanceExpList) {
					QString queryName = instanceExp->getName();

					CIndividualTermExpression *indiExp = instanceExp->getIndividualTermExpression();
					CClassTermExpression *classExp = instanceExp->getClassTermExpression();

					if (indiExp && classExp) {

						CIndividual *indi = getIndividualFromBuildExpression(indiExp);
						CConcept *concept = getConceptFromBuildExpression(classExp);

						QString iriClassNameString = CIRIName::getRecentIRIName(indi->getIndividualNameLinker());
						QString indiString = iriClassNameString;
						QString conceptString = getStringFromConcept(concept);

						CIsInstanceOfQuery *query = new CIsInstanceOfQuery(indiString,conceptString,queryName);
						CCalculationConfigurationExtension *calcConfig = new CCalculationConfigurationExtension(config,0);

						CSatisfiableCalculationJob* satCalcJob = satCalcJobGenerator->getSatisfiableCalculationJob(concept,true,indi);

						CQueryStatisticsCollectionStrings* queryStats = nullptr;
						if (confBuildQueryStats) {
							queryStats = new CQueryStatisticsCollectionStrings();
							satCalcJob->setCalclulationStatisticsCollector(queryStats->createCalculationStatisticsCollection());
						}
						query->setQueryStatistics(queryStats);
						query->setCalculationConfiguration(calcConfig);


						satCalcJob->setCalculationConfiguration(calcConfig);
						query->addTestSatisfiableCalculationJob(satCalcJob,false);

						queryList.append(query);
						LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Generated IsIstanceOf-Query '%1' with question '%2'.").arg(query->getQueryName()).arg(query->getQueryString()),this);
					}
					delete instanceExp;
				}


				foreach (CQueryGetFlattenedTypesExpression* flattenedTypes, flattenedTypesExpList) {
					QString queryName = flattenedTypes->getName();
					CIndividualTermExpression *indiExp = flattenedTypes->getIndividualTermExpression();

					if (indiExp) {
						CIndividual *indi = getIndividualFromBuildExpression(indiExp);

						QString iriIndiNameString = CIRIName::getRecentIRIName(indi->getIndividualNameLinker());
						QString indiString = iriIndiNameString;

						CCalculationConfigurationExtension *calcConfig = new CCalculationConfigurationExtension(config,0);
						CFlattenedTypesQuery *query = new CFlattenedTypesQuery(mOntology,calcConfig,indi,flattenedTypes->isDirect(),indiString,queryName);

						CQueryStatisticsCollectionStrings* queryStats = nullptr;
						if (confBuildQueryStats) {
							queryStats = new CQueryStatisticsCollectionStrings();
						}
						query->setQueryStatistics(queryStats);

						queryList.append(query);
						LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Generated FlattenedTypes-Query '%1' with question '%2'.").arg(query->getQueryName()).arg(query->getQueryString()),this);
					}
					delete flattenedTypes;
				}


				foreach (CQueryGetFlattenedInstancesExpression* flattenedInstances, flattenedInstancesExpList) {
					QString queryName = flattenedInstances->getName();
					CClassTermExpression* classExp = flattenedInstances->getClassTermExpression();

					if (classExp) {
						CConcept* concept = getConceptFromBuildExpression(classExp);

						QString iriClassNameString = CIRIName::getRecentIRIName(concept->getClassNameLinker());
						QString classString = iriClassNameString;

						CCalculationConfigurationExtension *calcConfig = new CCalculationConfigurationExtension(config,0);
						CFlattenedInstancesQuery *query = new CFlattenedInstancesQuery(mOntology,calcConfig,concept,flattenedInstances->isDirect(),classString,queryName);

						CQueryStatisticsCollectionStrings* queryStats = nullptr;
						if (confBuildQueryStats) {
							queryStats = new CQueryStatisticsCollectionStrings();
						}
						query->setQueryStatistics(queryStats);

						queryList.append(query);
						LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Generated FlattenedInstances-Query '%1' with question '%2'.").arg(query->getQueryName()).arg(query->getQueryString()),this);
					}
					delete flattenedInstances;
				}



				foreach (CQueryGetTypesExpression* typesExp, typesExpList) {
					QString queryName = typesExp->getName();
					CIndividualTermExpression *indiExp = typesExp->getIndividualTermExpression();

					if (indiExp) {
						CIndividual *indi = getIndividualFromBuildExpression(indiExp);

						QString iriIndiNameString = CIRIName::getRecentIRIName(indi->getIndividualNameLinker());
						QString indiString = iriIndiNameString;

						CCalculationConfigurationExtension *calcConfig = new CCalculationConfigurationExtension(config,0);
						CTypesQuery *query = new CTypesQuery(mOntology,calcConfig,indi,typesExp->isDirect(),indiString,queryName);

						CQueryStatisticsCollectionStrings* queryStats = nullptr;
						if (confBuildQueryStats) {
							queryStats = new CQueryStatisticsCollectionStrings();
						}
						query->setQueryStatistics(queryStats);

						queryList.append(query);
						LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Generated Types-Query '%1' with question '%2'.").arg(query->getQueryName()).arg(query->getQueryString()),this);
					}
					delete typesExp;
				}



				foreach (CQueryGetSameIndividualsExpression* sameIndExp, sameIndiExpList) {
					QString queryName = sameIndExp->getName();
					CIndividualTermExpression *indiExp = sameIndExp->getIndividualTermExpression();

					if (indiExp) {
						CIndividual *indi = getIndividualFromBuildExpression(indiExp);

						QString iriIndiNameString = CIRIName::getRecentIRIName(indi->getIndividualNameLinker());
						QString indiString = iriIndiNameString;

						CCalculationConfigurationExtension *calcConfig = new CCalculationConfigurationExtension(config,0);
						CSameIndividualsQuery *query = new CSameIndividualsQuery(mOntology,calcConfig,indi,indiString,queryName);

						CQueryStatisticsCollectionStrings* queryStats = nullptr;
						if (confBuildQueryStats) {
							queryStats = new CQueryStatisticsCollectionStrings();
						}
						query->setQueryStatistics(queryStats);

						queryList.append(query);
						LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Generated Same-Individuals-Query '%1' with question '%2'.").arg(query->getQueryName()).arg(query->getQueryString()),this);
					}
					delete sameIndExp;
				}


				foreach (CQueryGetInstancesExpression* instancesExp, instancesExpList) {
					QString queryName = instancesExp->getName();
					CClassTermExpression* classExp = instancesExp->getClassTermExpression();

					if (classExp) {
						CConcept* concept = getConceptFromBuildExpression(classExp);

						QString iriClassNameString = CIRIName::getRecentIRIName(concept->getClassNameLinker());
						QString classString = iriClassNameString;

						CCalculationConfigurationExtension *calcConfig = new CCalculationConfigurationExtension(config,0);
						CInstancesQuery *query = new CInstancesQuery(mOntology,calcConfig,concept,instancesExp->isDirect(),classString,queryName);

						CQueryStatisticsCollectionStrings* queryStats = nullptr;
						if (confBuildQueryStats) {
							queryStats = new CQueryStatisticsCollectionStrings();
						}
						query->setQueryStatistics(queryStats);

						queryList.append(query);
						LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Generated Instances-Query '%1' with question '%2'.").arg(query->getQueryName()).arg(query->getQueryString()),this);
					}
				}


				foreach (CQueryGetSubClassesExpression* subClassesExp, mSubClassesExpList) {
					QString queryName = subClassesExp->getName();
					CClassTermExpression* classExp = subClassesExp->getClassTermExpression();

					if (classExp) {
						CConcept* concept = getConceptFromBuildExpression(classExp);

						QString iriClassNameString = CIRIName::getRecentIRIName(concept->getClassNameLinker());
						QString classString = iriClassNameString;

						CCalculationConfigurationExtension *calcConfig = new CCalculationConfigurationExtension(config,0);
						CSubClassesQuery *query = new CSubClassesQuery(mOntology,calcConfig,concept,subClassesExp->isDirect(),classString,queryName);

						CQueryStatisticsCollectionStrings* queryStats = nullptr;
						if (confBuildQueryStats) {
							queryStats = new CQueryStatisticsCollectionStrings();
						}
						query->setQueryStatistics(queryStats);

						queryList.append(query);
						LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Generated GetSubClasses-Query '%1' with question '%2'.").arg(query->getQueryName()).arg(query->getQueryString()),this);
					}
					delete subClassesExp;
				}



				foreach (CQueryGetSuperClassesExpression* superClassesExp, mSuperClassesExpList) {
					QString queryName = superClassesExp->getName();
					CClassTermExpression* classExp = superClassesExp->getClassTermExpression();

					if (classExp) {
						CConcept* concept = getConceptFromBuildExpression(classExp);

						QString iriClassNameString = CIRIName::getRecentIRIName(concept->getClassNameLinker());
						QString classString = iriClassNameString;

						CCalculationConfigurationExtension *calcConfig = new CCalculationConfigurationExtension(config,0);
						CSuperClassesQuery *query = new CSuperClassesQuery(mOntology,calcConfig,concept,superClassesExp->isDirect(),classString,queryName);

						CQueryStatisticsCollectionStrings* queryStats = nullptr;
						if (confBuildQueryStats) {
							queryStats = new CQueryStatisticsCollectionStrings();
						}
						query->setQueryStatistics(queryStats);

						queryList.append(query);
						LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Generated GetSuperClasses-Query '%1' with question '%2'.").arg(query->getQueryName()).arg(query->getQueryString()),this);
					}
					delete superClassesExp;
				}



				foreach (CQueryGetEquivalentClassesExpression* equivClassesExp, mEquivClassesExpList) {
					QString queryName = equivClassesExp->getName();
					CClassTermExpression* classExp = equivClassesExp->getClassTermExpression();

					if (classExp) {
						CConcept* concept = getConceptFromBuildExpression(classExp);

						QString iriClassNameString = CIRIName::getRecentIRIName(concept->getClassNameLinker());
						QString classString = iriClassNameString;

						CCalculationConfigurationExtension *calcConfig = new CCalculationConfigurationExtension(config,0);
						CEquivalentClassesQuery *query = new CEquivalentClassesQuery(mOntology,calcConfig,concept,classString,queryName);

						CQueryStatisticsCollectionStrings* queryStats = nullptr;
						if (confBuildQueryStats) {
							queryStats = new CQueryStatisticsCollectionStrings();
						}
						query->setQueryStatistics(queryStats);

						queryList.append(query);
						LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Generated GetEquivalentClasses-Query '%1' with question '%2'.").arg(query->getQueryName()).arg(query->getQueryString()),this);
					}
					delete equivClassesExp;
				}


				foreach (CQueryGetObjectPropertyTargetsExpression* objPropTargetsExp, mObjectPropTargetsExpList) {
					QString queryName = objPropTargetsExp->getName();
					CObjectPropertyTermExpression* objPropTermExp = objPropTargetsExp->getObjectPropertyTermExpression();
					CIndividualTermExpression* indiTermExp = objPropTargetsExp->getIndividualTermExpression();

					if (objPropTermExp && indiTermExp) {
						CRole* role = getRoleFromBuildExpression(objPropTermExp);
						CIndividual* individual = getIndividualFromBuildExpression(indiTermExp);


						CCalculationConfigurationExtension *calcConfig = new CCalculationConfigurationExtension(config,0);
						CObjectPropertyTargetsQuery *query = new CObjectPropertyTargetsQuery(mOntology,calcConfig,individual,role,queryName);

						CQueryStatisticsCollectionStrings* queryStats = nullptr;
						if (confBuildQueryStats) {
							queryStats = new CQueryStatisticsCollectionStrings();
						}
						query->setQueryStatistics(queryStats);

						queryList.append(query);
						LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Generated ObjectPropertyTargets-Query '%1' with question '%2'.").arg(query->getQueryName()).arg(query->getQueryString()),this);
					}
				}

				foreach (CQueryGetFlattenedObjectPropertyTargetsExpression* objPropTargetsExp, mObjectFlattenedPropTargetsExpList) {
					QString queryName = objPropTargetsExp->getName();
					CObjectPropertyTermExpression* objPropTermExp = objPropTargetsExp->getObjectPropertyTermExpression();
					CIndividualTermExpression* indiTermExp = objPropTargetsExp->getIndividualTermExpression();

					if (objPropTermExp && indiTermExp) {
						CRole* role = getRoleFromBuildExpression(objPropTermExp);
						CIndividual* individual = getIndividualFromBuildExpression(indiTermExp);


						CCalculationConfigurationExtension *calcConfig = new CCalculationConfigurationExtension(config,0);
						CFlattenedObjectPropertyTargetsQuery *query = new CFlattenedObjectPropertyTargetsQuery(mOntology,calcConfig,individual,role,queryName);

						CQueryStatisticsCollectionStrings* queryStats = nullptr;
						if (confBuildQueryStats) {
							queryStats = new CQueryStatisticsCollectionStrings();
						}
						query->setQueryStatistics(queryStats);

						queryList.append(query);
						LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Generated FlattenedObjectPropertyTargets-Query '%1' with question '%2'.").arg(query->getQueryName()).arg(query->getQueryString()),this);
					}
				}




				foreach (CQueryGetDeterministicIndividualsExpression* indiQueryExp, mDetIndisExpList) {
					QString queryName = indiQueryExp->getName();
					CEXPRESSIONLIST<CIndividualTermExpression*>* indiExpList = indiQueryExp->getIndividualTermExpressionList();

					if (indiExpList) {
						QSet<CIndividual*> individualSet;
						for (CEXPRESSIONLIST<CIndividualTermExpression*>::const_iterator it = indiExpList->constBegin(), itEnd = indiExpList->constEnd(); it != itEnd; ++it) {
							CIndividualTermExpression* indiTermExp(*it);
							CIndividual* individual = getIndividualFromBuildExpression(indiTermExp);
							individualSet.insert(individual);
						}

						CCalculationConfigurationExtension *calcConfig = new CCalculationConfigurationExtension(config,0);
						CDeterministicIndividualsQuery *query = new CDeterministicIndividualsQuery(mOntology,calcConfig,individualSet,queryName);

						CQueryStatisticsCollectionStrings* queryStats = nullptr;
						if (confBuildQueryStats) {
							queryStats = new CQueryStatisticsCollectionStrings();
						}
						query->setQueryStatistics(queryStats);

						queryList.append(query);
						LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Generated DeterministicIndividuals-Query '%1' with question '%2'.").arg(query->getQueryName()).arg(query->getQueryString()),this);
					}
				}




				foreach (CQueryGetNondeterministicIndividualsExpression* indiQueryExp, mNdetIndisExpList) {
					QString queryName = indiQueryExp->getName();
					CEXPRESSIONLIST<CIndividualTermExpression*>* indiExpList = indiQueryExp->getIndividualTermExpressionList();

					if (indiExpList) {
						QSet<CIndividual*> individualSet;
						for (CEXPRESSIONLIST<CIndividualTermExpression*>::const_iterator it = indiExpList->constBegin(), itEnd = indiExpList->constEnd(); it != itEnd; ++it) {
							CIndividualTermExpression* indiTermExp(*it);
							CIndividual* individual = getIndividualFromBuildExpression(indiTermExp);
							individualSet.insert(individual);
						}

						CCalculationConfigurationExtension *calcConfig = new CCalculationConfigurationExtension(config,0);
						CNondeterministicIndividualsQuery *query = new CNondeterministicIndividualsQuery(mOntology,calcConfig,individualSet,queryName);

						CQueryStatisticsCollectionStrings* queryStats = nullptr;
						if (confBuildQueryStats) {
							queryStats = new CQueryStatisticsCollectionStrings();
						}
						query->setQueryStatistics(queryStats);

						queryList.append(query);
						LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Generated NondeterministicIndividuals-Query '%1' with question '%2'.").arg(query->getQueryName()).arg(query->getQueryString()),this);
					}
				}




				foreach (CQueryGetDeterministicClassAssertionsExpression* indiQueryExp, mDetClassAssExpList) {
					QString queryName = indiQueryExp->getName();
					CEXPRESSIONLIST<CIndividualTermExpression*>* indiExpList = indiQueryExp->getIndividualTermExpressionList();

					if (indiExpList) {
						QSet<CIndividual*> individualSet;
						for (CEXPRESSIONLIST<CIndividualTermExpression*>::const_iterator it = indiExpList->constBegin(), itEnd = indiExpList->constEnd(); it != itEnd; ++it) {
							CIndividualTermExpression* indiTermExp(*it);
							CIndividual* individual = getIndividualFromBuildExpression(indiTermExp);
							individualSet.insert(individual);
						}

						CCalculationConfigurationExtension *calcConfig = new CCalculationConfigurationExtension(config,0);
						CDeterministicClassAssertionQuery *query = new CDeterministicClassAssertionQuery(mOntology,calcConfig,individualSet,queryName);

						CQueryStatisticsCollectionStrings* queryStats = nullptr;
						if (confBuildQueryStats) {
							queryStats = new CQueryStatisticsCollectionStrings();
						}
						query->setQueryStatistics(queryStats);

						queryList.append(query);
						LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Generated DeterministicClassAssertions-Query '%1' with question '%2'.").arg(query->getQueryName()).arg(query->getQueryString()),this);
					}
				}




				foreach (CQueryGetNondeterministicClassAssertionsExpression* indiQueryExp, mNdetClassAssExpList) {
					QString queryName = indiQueryExp->getName();
					CEXPRESSIONLIST<CIndividualTermExpression*>* indiExpList = indiQueryExp->getIndividualTermExpressionList();

					if (indiExpList) {
						QSet<CIndividual*> individualSet;
						for (CEXPRESSIONLIST<CIndividualTermExpression*>::const_iterator it = indiExpList->constBegin(), itEnd = indiExpList->constEnd(); it != itEnd; ++it) {
							CIndividualTermExpression* indiTermExp(*it);
							CIndividual* individual = getIndividualFromBuildExpression(indiTermExp);
							individualSet.insert(individual);
						}

						CCalculationConfigurationExtension *calcConfig = new CCalculationConfigurationExtension(config,0);
						CNondeterministicClassAssertionQuery *query = new CNondeterministicClassAssertionQuery(mOntology,calcConfig,individualSet,queryName);

						CQueryStatisticsCollectionStrings* queryStats = nullptr;
						if (confBuildQueryStats) {
							queryStats = new CQueryStatisticsCollectionStrings();
						}
						query->setQueryStatistics(queryStats);

						queryList.append(query);
						LOG(NOTICE,"::Konclude::Reasoner::Generator::ConcreteOntologyQueryBuilder",logTr("Generated NondeterministicClassAssertions-Query '%1' with question '%2'.").arg(query->getQueryName()).arg(query->getQueryString()),this);
					}
				}


				delete satCalcJobGenerator;


				return queryList;
			}



		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude
