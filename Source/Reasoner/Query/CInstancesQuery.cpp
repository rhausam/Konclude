/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CInstancesQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CInstancesQuery::CInstancesQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, CConcept* concept, bool direct, const QString& conceptName, const QString& flatternedTypeQueryName)
					: CRealizationPremisingQuery(ontology,configuration) {
				mConceptName = conceptName;
				mConcept = concept;
				queryName = flatternedTypeQueryName;
				if (mConceptName.isEmpty()) {
					queryString = QString("Get Instances");
				} else {
					queryString = QString("Get Instances for Type '%1'").arg(mConceptName);
				}
				mResult = nullptr;

				mUseAbbreviatedIRIs = CConfigDataReader::readConfigBoolean(configuration,"Konclude.OWLlink.AbbreviatedIRIs",false);

				mRealizationCalcError = false;
				mQueryConstructError = false;
				mDirect = direct;

			}

			CInstancesQuery::~CInstancesQuery() {
				if (mResult) {
					delete mResult;
				}
			}


			CQueryResult* CInstancesQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CInstancesQuery::constructResult(CRealization* realization) {
				return constructIndividualSynsetsResult(realization);
			}

			CIndividualSynsetsResult *CInstancesQuery::constructIndividualSynsetsResult(CRealization* realization) {
				if (mResult) {
					delete mResult;
					mResult = 0;
				}
				if (realization) {
					CConceptRealization* conRealization = realization->getConceptRealization();
					if (conRealization) {
						mResult = new CIndividualSynsetsResult();
						CIndividualSynsetsResultVisitorGenerator resultGenerator(mResult,mUseAbbreviatedIRIs);
						conRealization->visitInstances(mConcept,mDirect,&resultGenerator);
					}
				}
				return mResult;
			}


			QString CInstancesQuery::getQueryName() {
				return queryName;
			}

			QString CInstancesQuery::getQueryString() {
				return queryString;
			}

			bool CInstancesQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CInstancesQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Individuals");
				} else {
					return mResult->getQueryResultString();
				}
			}

			bool CInstancesQuery::hasError() {
				return mRealizationCalcError || mQueryConstructError || CQuery::hasError();
			}

			bool CInstancesQuery::isDirect() {
				return mDirect;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
