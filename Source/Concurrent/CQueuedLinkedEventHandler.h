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

#ifndef KONCLUDE_CONCURRENT_CQUEUEDLINKEDEVENTHANDLER_H
#define KONCLUDE_CONCURRENT_CQUEUEDLINKEDEVENTHANDLER_H

// Library includes


// Namespace includes
#include "ConcurrentSettings.h"
#include "CEventLinkerQueue.h"
#include "CEventHandler.h"
#include "CEventProcessor.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Concurrent {


		/*! 
		 *
		 *		\class		CQueuedLinkedEventHandler
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CQueuedLinkedEventHandler : public CEventLinkerQueue, virtual public CEventHandler {
			// public methods
			public:
				//! Constructor
				CQueuedLinkedEventHandler(CEventProcessor* eventProcessor, CThreadActivator* threadActivator);

				//! Destructor
				virtual ~CQueuedLinkedEventHandler();

				virtual cint64 handleEvents(CContext* handlerContext, double minEventPriority = 0., cint64 maxProcessingCount = -1);
				virtual bool needEventProcessing(double minEventPriority = 0.);


			// protected methods
			protected:

			// protected variables
			protected:
				CEventLinker* mUnprocessedEvents;
				CEventProcessor* mEventProcessor;

			// private methods
			private:

			// private variables
			private:
		};


	}; // end namespace Concurrent

}; // end namespace Konclude

#endif // KONCLUDE_CONCURRENT_CQUEUEDLINKEDEVENTHANDLER_H