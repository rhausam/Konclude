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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYQUEUE_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYQUEUE_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CReapplyConceptDescriptor.h"
#include "CReapplyQueueIterator.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {


//#define KONCLUDE_EMPLOY_REASONER_KERNEL_PROCESS_REAPPLY_QUEUE_COUNT

				/*! 
				 *
				 *		\class		CReapplyQueue
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CReapplyQueue {
					// public methods
					public:
						//! Constructor
						CReapplyQueue();

#ifdef KONCLUDE_EMPLOY_REASONER_KERNEL_PROCESS_REAPPLY_QUEUE_COUNT
						qint64 getReapplyCount();
#endif
						bool isEmpty();

						bool hasConceptDescriptor(CConceptDescriptor* conceptDescriptor);

						CReapplyQueue* initReapplyQueue(CReapplyQueue* prevReapplyQueue);

						CReapplyQueueIterator getIterator(bool clearDynamicReapplyQueue = true);
						CReapplyQueue* addReapplyConceptDescriptor(CReapplyConceptDescriptor *conProDes);


					// protected methods
					protected:

					// protected variables
					protected:
						CReapplyConceptDescriptor* mStaticReapplyDesLinker;
						CReapplyConceptDescriptor* mDynamicReapplyDesLinker;
#ifdef KONCLUDE_EMPLOY_REASONER_KERNEL_PROCESS_REAPPLY_QUEUE_COUNT
						cint64 mDesStaticCount;
						cint64 mDesDynamicCount;
#endif

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYQUEUE_H
