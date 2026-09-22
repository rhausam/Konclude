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

#ifndef KONCLUDE_CONTROL_INTERFACE_JNI_CJNIQUERYPROCESSOR_H
#define KONCLUDE_CONTROL_INTERFACE_JNI_CJNIQUERYPROCESSOR_H

// Libraries includes
#include <QStack>

// Namespace includes
#include "JNISettings.h"
#include "CJNIOntologyRevisionData.h"
#include "CJNICommandProcessor.h"
#include "CJNIInstanceManager.h"
#include "CJNIQueryExpressionBuildingLoader.h"

// Other includes
#include "Reasoner/Generator/CConcreteOntologyUpdateCollectorBuilder.h"

#include "Parser/Expressions/CBuildExpression.h"
#include "Parser/Expressions/CClassTermExpression.h"

#include "Reasoner/Revision/COntologyRevision.h"

#include "Reasoner/Query/CSetOfEntityExpressionSetResultVisitingCallback.h"
#include "Reasoner/Query/CComplexConceptAnsweringQuery.h"
#include "Reasoner/Query/CComplexSatisfiabilityAnsweringQuery.h"
#include "Reasoner/Query/CComplexSubClassesAnsweringQuery.h"
#include "Reasoner/Query/CComplexSuperClassesAnsweringQuery.h"
#include "Reasoner/Query/CComplexEquivalentClassesAnsweringQuery.h"
#include "Reasoner/Query/CComplexIndividualInstancesAnsweringQuery.h"
#include "Reasoner/Query/CBooleanQueryResult.h"
#include "Reasoner/Query/CClassSynsetsResult.h"
#include "Reasoner/Query/CClassSetResult.h"
#include "Reasoner/Query/CIndividualSynsetsResult.h"
#include "Reasoner/Query/CEquivalentClassesResultVisitCallbackQuery.h"
#include "Reasoner/Query/CSubSuperClassesResultVisitCallbackQuery.h"
#include "Reasoner/Query/CEquivalentPropertiesResultVisitCallbackQuery.h"
#include "Reasoner/Query/CSubSuperPropertiesResultVisitCallbackQuery.h"

#include "Reasoner/Query/CInstancesResultVisitCallbackQuery.h"
#include "Reasoner/Query/CSameIndividualsResultVisitCallbackQuery.h"
#include "Reasoner/Query/CTypesResultVisitCallbackQuery.h"
#include "Reasoner/Query/CObjectPropertySourcesTargetsResultVisitCallbackQuery.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Reasoner::Generator;
	using namespace Reasoner::Query;
	using namespace Parser::Expression;

	namespace Control {

		namespace Interface {

			namespace JNI {

				/*! 
				 *
				 *		\class		CJNIQueryProcessor
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CJNIQueryProcessor {

					// public methods
					public:
						//! Constructor
						CJNIQueryProcessor(CJNIInstanceManager* jniInstanceManager, CJNIOntologyRevisionData* ontRevData);

						//! Destructor
						virtual ~CJNIQueryProcessor();


						bool checkIsOntologyConsistent();


						bool queryOntologySubClasses(const QString& className, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback);
						bool queryOntologySuperClasses(const QString& className, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback);
						bool queryOntologyEquivalentClasses(const QString& className, CEntityExpressionSetResultVisitingCallback* visitingCallback);


						bool queryOntologySubObjectProperties(const QString& propertyName, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback);
						bool queryOntologySuperObjectProperties(const QString& propertyName, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback);
						bool queryOntologyEquivalentObjectProperties(const QString& propertyName, CEntityExpressionSetResultVisitingCallback* visitingCallback);



						bool queryOntologyInstances(const QString& className, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback);
						bool queryOntologyTypes(const QString& indiName, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback);
						bool queryOntologySameIndividuals(const QString& indiName, CEntityExpressionSetResultVisitingCallback* visitingCallback);
						bool queryOntologyObjectPropertySources(const QString& indiName, const QString& propertyName, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback);
						bool queryOntologyObjectPropertyTargets(const QString& indiName, const QString& propertyName, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback);


						/**
						 * The builder for the class expressions that the queries below are asked
						 * about. It builds into a revision of the installed ontology that is never
						 * installed, created on first use and kept for the lifetime of this processor,
						 * so the expressions stay valid as long as the querying bridge is open.
						 * Returns null if no revision could be created.
						 */
						CJNIAxiomExpressionVisitingLoader* getClassExpressionBuilder();

						/**
						 * The queries about a class expression, the anonymous class expressions of the
						 * OWL API. They are answered by the complex answering queries that the OWLlink
						 * interface uses, whose results name the entities, so the names are resolved
						 * to the entity expressions of the installed ontology and reported through the
						 * same callbacks as the named queries above. Each returns false if the query
						 * was not answered and says why in the error message.
						 */
						bool checkIsClassExpressionSatisfiable(CBuildExpression* classExpression, bool& satisfiable, QString& errorMessage);
						bool queryClassExpressionSubClasses(CBuildExpression* classExpression, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback, QString& errorMessage);
						bool queryClassExpressionSuperClasses(CBuildExpression* classExpression, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback, QString& errorMessage);
						bool queryClassExpressionEquivalentClasses(CBuildExpression* classExpression, CEntityExpressionSetResultVisitingCallback* visitingCallback, QString& errorMessage);
						bool queryClassExpressionInstances(CBuildExpression* classExpression, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback, QString& errorMessage);


					// protected methods
					protected:
						CClassTermExpression* getQueryClassTermExpression(CBuildExpression* classExpression, QString& errorMessage);
						CQueryResult* calculateClassExpressionQuery(CComplexConceptAnsweringQuery* query, QString& errorMessage);

						bool computeClassExpressionEquivalentClassNames(CClassTermExpression* classTermExp, QStringList& classNames, QString& errorMessage);
						bool queryClassExpressionSubSuperClasses(CBuildExpression* classExpression, bool subClasses, bool superClasses, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback, QString& errorMessage);

						void visitClassName(const QString& className, CEntityExpressionSetResultVisitingCallback* visitingCallback);
						void visitIndividualName(const QString& individualName, CEntityExpressionSetResultVisitingCallback* visitingCallback);

					// protected variables
					protected:
						CJNIInstanceManager* mJNIInstanceManager;
						CJNICommandProcessor* mJNICommandProcessor;
						CJNIOntologyRevisionData* mOntRevData;

						COntologyRevision* mExpressionOntRev;
						CJNIQueryExpressionBuildingLoader* mExpressionBuilder;



					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace JNI 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_INTERFACE_JNI_CJNIQUERYPROCESSOR_H
