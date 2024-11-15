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

#include "CTestExtendedCalculateQueryCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CTestExtendedCalculateQueryCommand::CTestExtendedCalculateQueryCommand(CQueryCommandProvider *queryCommandProvider, CCommand *parentSuperCommand) 
						: CCalculateQueryCommand(queryCommandProvider,parentSuperCommand) {

					timingLevel = 0;
					calcTime = 0;
					hasCalcTime = false;

					expectedResult = 0;

					creationTime.start();
				}


				CTestExtendedCalculateQueryCommand::~CTestExtendedCalculateQueryCommand() {
				}

				qint64 CTestExtendedCalculateQueryCommand::getCommandTag() {
					return TESTEXTENDEDCALCULATEQUERYCOMMAND;
				}


				QString CTestExtendedCalculateQueryCommand::getBriefCommandDescription() {
					return QString("Calculate-Test-Query Command");
				}



				qint64 CTestExtendedCalculateQueryCommand::getTimingReportLevel() {
					return timingLevel;
				}

				CTestExtendedCalculateQueryCommand *CTestExtendedCalculateQueryCommand::setTimingReportLevel(qint64 timingReportLevel) {
					timingLevel = timingReportLevel;
					return this;
				}


				qint64 CTestExtendedCalculateQueryCommand::getCalculationTime() {
					return calcTime;
				}
				
				
				CTestExtendedCalculateQueryCommand *CTestExtendedCalculateQueryCommand::setCalculationTime(qint64 calculationTime) {
					calcTime = calculationTime;
					hasCalcTime = true;
					return this;
				}

				bool CTestExtendedCalculateQueryCommand::hasCalculationTime() {
					return hasCalcTime;
				}


				qint64 CTestExtendedCalculateQueryCommand::getCreationElapsedMilliSeconds() {
					return creationTime.elapsed();
				}

				CTestExtendedCalculateQueryCommand *CTestExtendedCalculateQueryCommand::setExpectedQueryResult(CQueryResult *expectedQueryResult) {
					expectedResult = expectedQueryResult;
					return this;
				}

				CQueryResult *CTestExtendedCalculateQueryCommand::getExpectedQueryResult() {
					return expectedResult;
				}



			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude
