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

#ifndef KONCLUDE_TEST_EVALUATION_CReasonerEvaluationStatisticsNameCollectingExtractor_H
#define KONCLUDE_TEST_EVALUATION_CReasonerEvaluationStatisticsNameCollectingExtractor_H

// Libraries includes
#include <QList>
#include <QFile>
#include <QDomDocument>
#include <QSet>
#include <QString>

// Namespace includes
#include "EvaluationSettings.h"
#include "CReasonerEvaluationDataValue.h"
#include "CReasonerEvaluationExtractor.h"
#include "CReasonerEvaluationDoubleDataValue.h"
#include "CReasonerEvaluationDataValueCacher.h"
#include "CReasonerEvaluationStringValue.h"


// Other includes
#include "Config/CConfiguration.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\class		CReasonerEvaluationStatisticsNameCollectingExtractor
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationStatisticsNameCollectingExtractor : public CReasonerEvaluationExtractor {
				// public methods
				public:

					//! Constructor
					CReasonerEvaluationStatisticsNameCollectingExtractor(QSet<QString>* statisticsNamesSet, CReasonerEvaluationDataValueCacher* cacher = nullptr);

					virtual CReasonerEvaluationDataValue* extractEvaluationData(const QString& responseFileString);


				// protected methods
				protected:
					virtual bool extractEvaluationData(QDomDocument& document, const QString& responseFileString);


				// protected variables
				protected:
					CReasonerEvaluationDataValueCacher* mCacher;
					double mTimelimit;
					double mErrorPunishmentTime;

					QSet<QString>* mStatisticsNamesSet;

				// private methods
				private:

				// private variables
				private:

			};

	}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CReasonerEvaluationStatisticsNameCollectingExtractor_H
