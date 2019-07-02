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

#ifndef KONCLUDE_TEST_ANALYSE_READER_CCLASSIFICATIONPROGRESSUPDATEREADER_H
#define KONCLUDE_TEST_ANALYSE_READER_CCLASSIFICATIONPROGRESSUPDATEREADER_H

// Libraries includes
#include <QString>


// Namespace includes


// Other includes
#include "Test/Analyse/AnalyseSettings.h"
#include "Test/Analyse/CAnalyseValue.h"
#include "Test/Analyse/CAnalyseValueUpdateReader.h"
#include "Test/Analyse/CAnalyseValueDescription.h"
#include "Test/Analyse/CIntegerAnalyseValue.h"
#include "Test/Analyse/CConvertStringAnalyseValue.h"
#include "Test/Analyse/CDoubleAnalyseValue.h"

#include "Reasoner/Classifier/CClassificationManager.h"
#include "Reasoner/Classifier/CClassifierStatistics.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;
	using namespace Reasoner::Classifier;

	namespace Test {

		namespace Analyse {

			namespace Reader {

				/*! 
				 *
				 *		\class		CClassificationProgressUpdateReader
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CClassificationProgressUpdateReader : public CAnalyseValueUpdateReader {
					// public methods
					public:
						//! Constructor
						CClassificationProgressUpdateReader(CClassificationManager *classificationManager);

						//! Destructor
						virtual ~CClassificationProgressUpdateReader();

						virtual CAnalyseValue *getAnalyseValueUpdate(CAnalyseValueDescription *valueDes);

						virtual CAnalyseValueDescription *getTotalSubsumptionTestCountValueDescription();
						virtual CAnalyseValueDescription *getTestedSubsumptionCountValueDescription();
						virtual CAnalyseValueDescription *getTotalSatisfiableTestCountValueDescription();
						virtual CAnalyseValueDescription *getTestedSatisfiableCountValueDescription();
						virtual CAnalyseValueDescription *getActiveClassifierCountValueDescription();

					// protected methods
					protected:

					// protected variables
					protected:
						CClassificationManager *classiMan;

						CAnalyseValueDescription *totalSubsumValueDescription;
						CAnalyseValueDescription *totalSatisValueDescription;
						CAnalyseValueDescription *testedSubsumValueDescription;
						CAnalyseValueDescription *testedSatisValueDescription;
						CAnalyseValueDescription *activeClassifierCountValueDescription;

						CClassifierStatistics *statistics;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Reader

		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_ANALYSE_READER_CCLASSIFICATIONPROGRESSUPDATEREADER_H
