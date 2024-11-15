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

#ifndef KONCLUDE_Control_COMMAND_EVENTS_CCOMMANDPRECONDITIONCHANGEEVENT_H
#define KONCLUDE_Control_COMMAND_EVENTS_CCOMMANDPRECONDITIONCHANGEEVENT_H



// Libraries includes


// Namespace includes
#include "../CommandSettings.h"
#include "../CCommand.h"


// Other includes
#include "Concurrent/Events/CCustomEvent.h"
#include "Concurrent/Callback/CCallbackData.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent::Events;
	using namespace Concurrent::Callback;

	namespace Control {

		namespace Command {

			namespace Events {



				/*! 
				 *	\class		CCommandPreconditionChangeEvent
				 *	\author		Andreas Steigmiller
				 *	\version	0.1
				 *	\brief		
				 */
				class CCommandPreconditionChangeEvent : public CCustomEvent, public CCallbackData {
					// public methods
					public:
						//! Constructor
						CCommandPreconditionChangeEvent(CThread *eventReceiver, CCommand *command);

						//! Destructor
						virtual ~CCommandPreconditionChangeEvent();

						virtual CCommand *getCommand();

						virtual void doCallback();

					// protected methods
					protected:

					// protected variables
					protected:
						CCommand *comm;
						CThread *receiver;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Events

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_COMMAND_EVENTS_CCOMMANDPRECONDITIONCHANGEEVENT_H
