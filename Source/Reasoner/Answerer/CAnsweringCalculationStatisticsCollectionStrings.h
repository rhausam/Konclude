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

#ifndef KONCLUDE_REASONER_ANSWERER_CANSWERINGCALCULATIONSTATISTICSCOLLECTIONSTRINGS_H
#define KONCLUDE_REASONER_ANSWERER_CANSWERINGCALCULATIONSTATISTICSCOLLECTIONSTRINGS_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "CAnsweringStatistics.h"
#include "CAnsweringCalculationStatisticsCollection.h"

// Other includes
#include "Reasoner/Kernel/Task/CCalculationStatisticsCollector.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Kernel::Task;

		namespace Answerer {

			/*! 
			 *
			 *		\class		CAnsweringProcessStatisticsCollectionStrings
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CAnsweringCalculationStatisticsCollectionStrings : public CAnsweringStatistics {
				// public methods
				public:
					//! Constructor
					CAnsweringCalculationStatisticsCollectionStrings();

					//! Destructor
					virtual ~CAnsweringCalculationStatisticsCollectionStrings();

					virtual bool addProcessingStatistics(const QString& statName, cint64 statValue);
					CAnsweringCalculationStatisticsCollection* getCalculationStatisticsCollectorLinker();
					CAnsweringCalculationStatisticsCollection* createCalculationStatisticsCollection();

					virtual QList<QString> getStatisticsNameStringList();
					virtual cint64 getStatisticIntegerValue(const QString& statName);

				// protected methods
				protected:

				// protected variables
				protected:
					CAnsweringCalculationStatisticsCollection* mCalcStatCollLinker;
					bool mStatStringsCollected;
					QHash<QString,cint64> mStatNameStringsValueHash;
					QSet<QString> mStatNameStrings;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CANSWERINGCALCULATIONSTATISTICSCOLLECTIONSTRINGS_H
