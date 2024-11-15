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

#include "CSameIndividualsQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CSameIndividualsQuery::CSameIndividualsQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, const CIndividualReference& individual, const QString& indiName, const QString& sameIndisQueryName)
					: CRealizationPremisingQuery(ontology,configuration) {
				mIndividualName = indiName;
				mIndividual = individual;
				queryName = sameIndisQueryName;
				if (mIndividualName.isEmpty()) {
					queryString = QString("Get Same Individuals");
				} else {
					queryString = QString("Get Same Individuals for '%1'").arg(mIndividualName);
				}
				mResult = nullptr;

				mUseAbbreviatedIRIs = CConfigDataReader::readConfigBoolean(configuration,"Konclude.OWLlink.AbbreviatedIRIs",false);
				mAnonymousIndividuals = CConfigDataReader::readConfigBoolean(configuration, "Konclude.OWLlink.ResultAnonymousIndividualsIntegration", false);

				mRealizationCalcError = false;
				mQueryConstructError = false;

				mRequiresSameIndividualRealisation = true;
				mDynamicRealisation = true;

				mCalcConfig = configuration;
			}

			CSameIndividualsQuery::~CSameIndividualsQuery() {
				if (mResult) {
					delete mResult;
				}
				delete mCalcConfig;
			}


			CQueryResult* CSameIndividualsQuery::getQueryResult() {
				return mResult;
			}


			CQueryResult *CSameIndividualsQuery::constructResult(CRealization* realization) {
				return constructIndividualsResult(realization);
			}

			CIndividualSynonymsResult *CSameIndividualsQuery::constructIndividualsResult(CRealization* realization) {
				if (mResult) {
					delete mResult;
					mResult = 0;
				}
				if (realization) {
					CSameRealization* sameRealization = realization->getSameRealization();
					if (sameRealization) {
						mResult = new CIndividualSynonymsResult();
						CIndividualsResultVisitorGenerator resultGenerator(mResult,mUseAbbreviatedIRIs, mAnonymousIndividuals, mOntology->getIndividualNameResolver());
						sameRealization->visitSameIndividuals(mIndividual,&resultGenerator);
					}
				}
				return mResult;
			}


			QString CSameIndividualsQuery::getQueryName() {
				return queryName;
			}

			QString CSameIndividualsQuery::getQueryString() {
				return queryString;
			}

			bool CSameIndividualsQuery::hasAnswer() {
				return mResult != 0;
			}

			QString CSameIndividualsQuery::getAnswerString() {
				if (!mResult) {
					return QString("Unknown Individuals");
				} else {
					return mResult->getQueryResultString();
				}
			}

			bool CSameIndividualsQuery::hasError() {
				return mRealizationCalcError || mQueryConstructError || CQuery::hasError();
			}

			COntologyProcessingDynamicRealizationRequirement* CSameIndividualsQuery::getDynamicRealizationRequirement() {
				return new COntologyProcessingSameRealizationRequirement(mIndividual, CIndividualReference());
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
