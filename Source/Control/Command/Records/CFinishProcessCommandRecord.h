/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#ifndef KONCLUDE_Control_COMMAND_RECORDS_CFINISHPROCESSCOMMANDRECORD_H
#define KONCLUDE_Control_COMMAND_RECORDS_CFINISHPROCESSCOMMANDRECORD_H

// Libraries includes
#include <QString>

// Namespace includes
#include "Control/Command/CommandSettings.h"
#include "Control/Command/CCommand.h"
#include "Control/Command/CCommandRecordData.h"

// Other includes
#include "Logger/Record/CContextRecorder.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Records {

				/*! 
				 *
				 *		\class		CFinishProcessCommandRecord
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CFinishProcessCommandRecord : public CCommandRecordData {
					// public methods
					public:
						//! Constructor
						CFinishProcessCommandRecord(CCommand *command, const QString &recordDomain, CCommandRecordData *nextRecordData = 0);

						//! Destructor
						virtual ~CFinishProcessCommandRecord();

						virtual QString getRecordMessage();

						static void makeRecord(CCommandRecorder *recorder, const QString &domain, CCommand *command);
						static void makeRecord(CCommandRecordRouter *commandRecordRouter);


					// protected methods
					protected:

					// protected variables
					protected:

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Records

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_COMMAND_RECORDS_CFINISHPROCESSCOMMANDRECORD_H