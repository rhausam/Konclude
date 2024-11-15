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

#ifndef KONCLUDE_REASONER_QUERY_CSAMEINDIVIDUALSRESULTVISITCALLBACKQUERY_H
#define KONCLUDE_REASONER_QUERY_CSAMEINDIVIDUALSRESULTVISITCALLBACKQUERY_H

// Libraries includes
#include <QString>


// Namespace includes
#include "CQuery.h"
#include "CSucceedQueryResult.h"
#include "CRealizationPremisingQuery.h"
#include "CEntityExpressionSetResultVisitingCallback.h"
#include "CEntityExpressionSetResultVisitingCallbackGenerator.h"

#include "Config/CConfigDataReader.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CSameIndividualsResultVisitCallbackQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CSameIndividualsResultVisitCallbackQuery : public CRealizationPremisingQuery {
				// public methods
				public:
					//! Constructor
					CSameIndividualsResultVisitCallbackQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, CIndividual* individual, const QString& indiName, const QString& sameIndisQueryName = QString("UnnamedSameIndividualsQuery"));

					//! Destructor
					virtual ~CSameIndividualsResultVisitCallbackQuery();

					virtual CQueryResult* constructResult(CRealization* realization);
					virtual bool callbackVisitingInstances(CEntityExpressionSetResultVisitingCallback* visitingCallback);

					virtual QString getQueryName();
					virtual QString getQueryString();
					virtual bool hasAnswer();
					virtual QString getAnswerString();

					virtual CQueryResult* getQueryResult();

					virtual bool hasError();

				// protected methods
				protected:

				// protected variables
				protected:
					QString mIndividualName;
					CIndividual* mIndividual;
					QString queryName;
					QString queryString;

					CSucceedQueryResult* mResult;
					CConceptRealization* mSameRealization;

					bool mQueryConstructError;
					bool mRealizationCalcError;

					CConfigurationBase* mCalcConfig;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CSAMEINDIVIDUALSRESULTVISITCALLBACKQUERY_H
