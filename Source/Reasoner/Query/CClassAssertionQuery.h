/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_REASONER_QUERY_CCLASSASSERTIONQUERY_H
#define KONCLUDE_REASONER_QUERY_CCLASSASSERTIONQUERY_H

// Libraries includes
#include <QString>


// Namespace includes
#include "CQuery.h"
#include "CIndividualClassAssertionsResult.h"
#include "CConsistencePremisingQuery.h"


// Other includes
#include "Config/CConfigDataReader.h"

#include "Reasoner/Kernel/Process/CProcessingDataBox.h"
#include "Reasoner/Kernel/Process/CIndividualProcessNodeVector.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		using namespace Kernel::Process;

		namespace Query {

			/*! 
			 *
			 *		\class		CClassAssertionQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CClassAssertionQuery : public CConsistencePremisingQuery {
				// public methods
				public:
					//! Constructor
					CClassAssertionQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, const QSet<CIndividual*>& individualSet, const QString& queryName = QString("UnnamedConceptAssertionsQuery"));

					//! Destructor
					virtual ~CClassAssertionQuery();

					virtual CQueryResult *constructResult(CConsistence* consistence);
					virtual CIndividualClassAssertionsResult* constructAssertionResult(CConsistence* consistence);

					virtual QString getQueryName();
					virtual QString getQueryString();
					virtual bool hasAnswer();
					virtual QString getAnswerString();

					virtual CQueryResult* getQueryResult();

					virtual bool hasError();

				// protected methods
				protected:
					virtual bool addIndividualResultData(cint64 indiID, CIndividualProcessNodeVector* indiNodeVec);

				// protected variables
				protected:
					QString mQueryName;
					QString mQueryString;

					CIndividualClassAssertionsResult* mResult;

					bool mUseAbbreviatedIRIs;

					bool mQueryConstructError;
					bool mConsistencyCalcError;

					CConfigurationBase* mCalcConfig;

					bool mDeterministicAssertions;
					bool mNondeterministicAssertions;
					QSet<CIndividual*> mIndividualSet;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CCLASSASSERTIONQUERY_H