/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_KERNEL_MANAGER_EVENTS_CCALCEDQUERYEVENT_H
#define KONCLUDE_REASONER_KERNEL_MANAGER_EVENTS_CCALCEDQUERYEVENT_H



// Libraries includes
#include <QString>


// Namespace includes
#include "../ManagerSettings.h"


// Other includes
#include "Reasoner/Query/CQuery.h"

#include "Concurrent/Events/CCustomEvent.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent::Events;

	namespace Reasoner {

		using namespace Query;

		namespace Kernel {

			namespace Manager {

				namespace Events {



					/*! 
					 *	\class		CCalcedQueryEvent
					 *	\author		Andreas Steigmiller
					 *	\version	0.1
					 *	\brief		
					 */
					class CCalcedQueryEvent : public CCustomEvent, public CCallbackData {
						// public methods
						public:
							//! Constructor
							CCalcedQueryEvent(CThread *receiverThread, CQuery *query);

							//! Destructor
							virtual ~CCalcedQueryEvent();

							virtual void sendEventWithAnswer(const QString& newAnswerString);

							virtual void doCallback();

							virtual QString getAnswerString();
							virtual QString getQueryString();
							virtual QString getQueryName();

							virtual CQuery *getQuery();

							static const QEvent::Type EVENTTYPE = EVENTCALCEDQUERY;

						// protected methods
						protected:

						// protected variables
						protected:
							CQuery *query;
							CThread *recThread;
							QString answerString;
							QString queryString;

							QString queryName;


						// private methods
						private:

						// private variables
						private:

					};

				}; // end namespace Events

			}; // end namespace Manager

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_MANAGER_EVENTS_CCALCEDQUERYEVENT_H
