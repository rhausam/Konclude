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

#ifndef KONCLUDE_CONCURRENT_CSYNCEDEVENTCHANNEL_H
#define KONCLUDE_CONCURRENT_CSYNCEDEVENTCHANNEL_H

// Library includes


// Namespace includes
#include "ConcurrentSettings.h"
#include "CEventLinkerChannel.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Concurrent {


		/*! 
		 *
		 *		\class		CSyncedEventChannel
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CSyncedEventChannel : public CEventLinkerChannel {
			// public methods
			public:
				//! Constructor
				CSyncedEventChannel(CThreadActivator* threadActivator, double channelPriority = 0.);

				//! Destructor
				virtual ~CSyncedEventChannel();

				virtual CEventChannel* postEvent(CEvent* processingEvent);

			// protected methods
			protected:

			// protected variables
			protected:
				QMutex syncPostEventMutex;

			// private methods
			private:

			// private variables
			private:
		};


	}; // end namespace Concurrent

}; // end namespace Konclude

#endif // KONCLUDE_CONCURRENT_CSYNCEDEVENTCHANNEL_H