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

#ifndef KONCLUDE_REASONER_QUERY_CQUERYSTATISTICS_H
#define KONCLUDE_REASONER_QUERY_CQUERYSTATISTICS_H

// Libraries includes
#include <QList>
#include <QString>

// Namespace includes
#include "Reasoner/Ontology/COntologyProcessingStatistics.h"

// Other includes
#include "Utilities/UtilitiesSettings.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		using namespace Ontology;

		namespace Query {

			/*! 
			 *
			 *		\class		CQueryStatistics
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CQueryStatistics {
				// public methods
				public:
					//! Constructor
					CQueryStatistics();

					//! Destructor
					virtual ~CQueryStatistics();

					virtual QList<QString> getStatisticsNameStringList() = 0;
					virtual cint64 getStatisticIntegerValue(const QString& statName) = 0;
					virtual bool addProcessingStatistics(const QString& statName, cint64 statValue) = 0;
					virtual bool incProcessingStatistics(const QString& statName, cint64 incStatValue = 1) = 0;
					virtual bool addProcessingStatistics(COntologyProcessingStatistics* ontProcStats) = 0;

				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CQUERYSTATISTICS_H
