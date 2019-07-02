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

#ifndef KONCLUDE_REASONER_QUERY_CREALIZATIONPREMISINGQUERY_H
#define KONCLUDE_REASONER_QUERY_CREALIZATIONPREMISINGQUERY_H

// Libraries includes


// Namespace includes
#include "CQuery.h"


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Realization/CRealization.h"

#include "Config/CConfigurationBase.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Realization;

		namespace Query {

			/*! 
			 *
			 *		\class		CRealizationPremisingQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRealizationPremisingQuery : public CQuery {
				// public methods
				public:
					//! Constructor
					CRealizationPremisingQuery(CConcreteOntology* ontology, CConfigurationBase* configuration);

					//! Destructor
					virtual ~CRealizationPremisingQuery();

					virtual CConcreteOntology* getOntology();
					virtual CQueryResult* constructResult(CRealization* realization) = 0;
					virtual CConfigurationBase* getConfiguration();

					CRealizationPremisingQuery* setQueryStatistics(CQueryStatistics* stats);
					virtual CQueryStatistics* getQueryStatistics();

					bool isConceptRealisationRequired();
					bool isRoleRealisationRequired();
					bool isSameIndividualRealisationRequired();

				// protected methods
				protected:

				// protected variables
				protected:
					CConcreteOntology* mOntology;
					CConfigurationBase* mConfig;
					CQueryStatistics* mQueryStats;

					bool mRequiresConceptRealisation;
					bool mRequiresRoleRealisation;
					bool mRequiresSameIndividualRealisation;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CREALIZATIONPREMISINGQUERY_H
