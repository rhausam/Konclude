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

#include "CIsTriviallyConsistentQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CIsTriviallyConsistentQuery::CIsTriviallyConsistentQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, const QString& consistentQueryName) {


				mConfig = configuration;
				mOntology = ontology;
				queryName = consistentQueryName;
				queryString = QString("Is ontology trivially consistent?");
				mResult = nullptr;
			}

			CIsTriviallyConsistentQuery::~CIsTriviallyConsistentQuery() {
				if (mResult) {
					delete mResult;
				}
				if (mQueryStats) {
					delete mQueryStats;
				}
			}


			CQueryResult* CIsTriviallyConsistentQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CIsTriviallyConsistentQuery::constructResult(COntologyBuildData* buildData) {
				return constructConsistenceResult(buildData);
			}


			CBooleanQueryResult* CIsTriviallyConsistentQuery::constructConsistenceResult(COntologyBuildData* buildData) {
				if (buildData) {

					COntologyBuildExpressionCounter* buildExpCounter = buildData->getBuildExpressionCounter();
					bool trivialConsistent = false;
					if (buildExpCounter) {
						trivialConsistent = !buildExpCounter->hasConsistencyProblematicExpressions();
					}

					mResult = new CBooleanQueryResult(trivialConsistent);
				}
				return mResult;
			}


			QString CIsTriviallyConsistentQuery::getQueryName() {
				return queryName;
			}

			QString CIsTriviallyConsistentQuery::getQueryString() {
				return queryString;
			}

			bool CIsTriviallyConsistentQuery::hasAnswer() {
				return mResult != 0;
			}


			CQuery* CIsTriviallyConsistentQuery::addQueryError(CQueryError* queryError) {
				if (CQueryInconsitentOntologyError::hasInconsistentOntologyError(queryError)) {
					mResult = new CBooleanQueryResult(false);
				}
				return CQuery::addQueryError(queryError);
			}


			QString CIsTriviallyConsistentQuery::getAnswerString() {
				if (!mResult) {
					return QString("Consistency Unknown");
				} else {
					return mResult->getQueryResultString();
				}
			}

			CConcreteOntology *CIsTriviallyConsistentQuery::getOntology() {
				return mOntology;
			}

			CConfigurationBase *CIsTriviallyConsistentQuery::getConfiguration() {
				return mConfig;
			}


			CIsTriviallyConsistentQuery* CIsTriviallyConsistentQuery::setQueryStatistics(CQueryStatistics* stats) {
				mQueryStats = stats;
				return this;
			}

			CQueryStatistics* CIsTriviallyConsistentQuery::getQueryStatistics() {
				return mQueryStats;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
