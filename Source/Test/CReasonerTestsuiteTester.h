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

#ifndef KONCLUDE_TEST_CREASONERTESTSUITETESTER_H
#define KONCLUDE_TEST_CREASONERTESTSUITETESTER_H

// Libraries includes
#include <QProcess>
#include <QDir>

// Namespace includes
#include "CReasonerTestfileTest.h"
#include "CReasonerTestfileTestResult.h"
#include "CReasonerTestfileTester.h"
#include "CReasonerTestfileEvaluator.h"
#include "CReasonerTestfileErrorEvaluator.h"


// Other includes
#include "Control/Command/CCommandRecordRouter.h"
#include "Control/Command/CCommandExecutedBlocker.h"

#include "Control/Command/Instructions/CTestTestsuiteCommand.h"
#include "Control/Command/Instructions/CTestTestfileCommand.h"

#include "Control/Command/Records/CStartProcessCommandRecord.h"
#include "Control/Command/Records/CStopProcessCommandRecord.h"
#include "Control/Command/Records/CFinishProcessCommandRecord.h"
#include "Control/Command/Records/CUnspecifiedMessageInformationRecord.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"



namespace Konclude {

	using namespace Control::Command;
	using namespace Control::Command::Records;
	using namespace Control::Command::Instructions;

	namespace Test {


		/*! 
		 *
		 *		\class		CReasonerTestsuiteTester
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CReasonerTestsuiteTester : public CLogIdentifier {

			// public methods
			public:
				//! Constructor
				CReasonerTestsuiteTester(CConfiguration *config);

				//! Destructor
				virtual ~CReasonerTestsuiteTester();

				virtual CReasonerTestsuiteTester *realizeTestCommand(CTestTestsuiteCommand *command);

			// protected methods
			protected:


			// protected variables
			protected:
				CConfiguration *configuration;
				CReasonerTestfileTester *testFileTester;
				CReasonerTestfileErrorEvaluator *testErrorEvaluator;


			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CREASONERTESTSUITETESTER_H
