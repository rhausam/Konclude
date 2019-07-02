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

#include "CUnspecifiedMessageErrorRecord.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Records {

				CUnspecifiedMessageErrorRecord::CUnspecifiedMessageErrorRecord(CCommand *command, const QString &errorMessage, const QString &recordDomain, double recordErrorLevel, CCommandRecordData *recordData) 
						: CProcessErrorRecord(command,recordDomain,recordErrorLevel,recordData) {

					message = errorMessage;
				}


				CUnspecifiedMessageErrorRecord::~CUnspecifiedMessageErrorRecord() {
				}


				QString CUnspecifiedMessageErrorRecord::getRecordMessage() {
					return message;
				}


				void CUnspecifiedMessageErrorRecord::makeRecord(CCommandRecorder *recorder, const QString &message, const QString &domain, CCommand *command) {
					CUnspecifiedMessageErrorRecord *record = new CUnspecifiedMessageErrorRecord(command,message,domain,70,command->getRecordData());
					if (recorder) {
						recorder->recordData(record);
					} else {
						CCommandRecorder::recordToCommand(command,record);
					}
				}


				void CUnspecifiedMessageErrorRecord::makeRecord(const QString &message, CLogIdentifier *logIdentifier, CCommand *command) {
					if (logIdentifier && command && command->getRecorder()) {
						makeRecord(command->getRecorder(),message,logIdentifier->getLogDomain(),command);
					}
				}


				void CUnspecifiedMessageErrorRecord::makeRecord(const QString &message, CCommandRecordRouter *commandRecordRouter) {
					if (commandRecordRouter) {
						makeRecord(commandRecordRouter->getCommandRecorder(),message,commandRecordRouter->getLogDomain(),commandRecordRouter->getCommand());
					}
				}


			}; // end namespace Records

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude
