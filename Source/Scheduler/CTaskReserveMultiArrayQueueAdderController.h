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

#ifndef KONCLUDE_SCHEDULER_CTASKRESERVEMULTIARRAYQUEUEADDERCONTROLLER_H
#define KONCLUDE_SCHEDULER_CTASKRESERVEMULTIARRAYQUEUEADDERCONTROLLER_H

// Libraries includes


// Namespace includes
#include "SchedulerSettings.h"
#include "CTaskReserveQueueAdder.h"
#include "CTaskReserveQueueAdderController.h"
#include "CTaskReserveArrayQueue.h"
#include "CTaskReserveMultiArrayQueue.h"
#include "CTask.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Scheduler {


		/*! 
		 *
		 *		\class		CTaskReserveMultiArrayQueueAdderController
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CTaskReserveMultiArrayQueueAdderController : public CTaskReserveQueueAdderController {
			// public methods
			public:
				//! Constructor
				CTaskReserveMultiArrayQueueAdderController(CTaskReserveMultiArrayQueue* queue);

				virtual bool requiresTaskAddedNotification();

				bool hasRequiresTaskAddedNotificationFlag();
				CTaskReserveMultiArrayQueueAdderController* setRequiresTaskAddedNotificationFlag(bool flag);

				CTaskReserveQueueAdder* createTaskReserveQueueAdder();

			// protected methods
			protected:

			// protected variables
			protected:
				CTaskReserveMultiArrayQueue* mQueue;
				bool mRequireTaskAddedNotificationFlag;


			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_CTASKRESERVEMULTIARRAYQUEUEADDERCONTROLLER_H
