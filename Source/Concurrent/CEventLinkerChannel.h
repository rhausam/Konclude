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

#ifndef KONCLUDE_CONCURRENT_CEVENTLINKERCHANNEL_H
#define KONCLUDE_CONCURRENT_CEVENTLINKERCHANNEL_H

// Library includes


// Namespace includes
#include "ConcurrentSettings.h"
#include "CEventChannel.h"
#include "CEventLinker.h"
#include "CThreadActivator.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Concurrent {


		/*! 
		 *
		 *		\class		CEventLinkerChannel
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CEventLinkerChannel : public CEventChannel {
			// public methods
			public:
				//! Constructor
				CEventLinkerChannel(CThreadActivator* threadActivator, double channelPriority = 0.);

				//! Destructor
				virtual ~CEventLinkerChannel();

				virtual bool hasEvents();
				virtual CEventChannel* postEvent(CEvent* processingEvent);
				virtual bool checkPostEvent(CEvent* processingEvent);

				virtual CEventLinker* takeEvents(CEventLinker* addEventLinker);

				virtual double getChannelPriority();


			// protected methods
			protected:

			// protected variables
			protected:
				CEventLinker* mLastPostedEventLinker;
				CEventLinker* mLastTakedDuplicatedPostEventLinker;
				CEventLinker* mPostedEventLinker;
				CEventLinker* mLastTakedPostEventLinker;

				CThreadActivator* mThreadActivator;

				cint64 acquiredLinkerCount;
				cint64 fetchedLinkerCount;
				cint64 postedLinkerCount;

				double mChannelPriority;

			// private methods
			private:

			// private variables
			private:
		};


	}; // end namespace Concurrent

}; // end namespace Konclude

#endif // KONCLUDE_CONCURRENT_CEVENTLINKERCHANNEL_H
