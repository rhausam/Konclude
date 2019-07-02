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

#include "CClassificationStatusUpdateReader.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {

			namespace Reader {

				CClassificationStatusUpdateReader::CClassificationStatusUpdateReader(CSubsumptionClassifier *subsumptionClassifier) {
					subsumClassifier = subsumptionClassifier;

					statusValueDescription = 0;

					if (subsumClassifier) {
						statusValueDescription = new CAnalyseValueDescription();
						statusValueDescription->setValueName(QString("SubsumptionClassifier::StatusText"));
						statusValueDescription->setTemplateValue(new CStringAnalyseValue(0));
					}
				}

				CClassificationStatusUpdateReader::~CClassificationStatusUpdateReader() {
					if (statusValueDescription) {
						delete statusValueDescription;
					}
				}



				CAnalyseValueDescription *CClassificationStatusUpdateReader::getSubsumptionClassifierStatusDescription() {
					return statusValueDescription;
				}

				CAnalyseValue *CClassificationStatusUpdateReader::getAnalyseValueUpdate(CAnalyseValueDescription *valueDes) {
					CAnalyseValue *val = 0;

					if (valueDes == statusValueDescription && subsumClassifier) {
						val = new CStringAnalyseValue(subsumClassifier->getStatusString());
					}
					return val;
				}



			}; // end namespace Reader

		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
