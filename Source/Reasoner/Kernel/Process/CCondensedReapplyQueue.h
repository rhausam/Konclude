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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CCONDENSEDREAPPLYQUEUE_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CCONDENSEDREAPPLYQUEUE_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CCondensedReapplyConceptDescriptor.h"
#include "CCondensedReapplyQueueIterator.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {


//#define KONCLUDE_EMPLOY_REASONER_KERNEL_PROCESS_CONDENSED_REAPPLY_QUEUE_COUNT

				/*! 
				 *
				 *		\class		CCondensedReapplyQueue
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CCondensedReapplyQueue {
					// public methods
					public:
						//! Constructor
						CCondensedReapplyQueue();

#ifdef KONCLUDE_EMPLOY_REASONER_KERNEL_PROCESS_CONDENSED_REAPPLY_QUEUE_COUNT
						qint64 getReapplyCount();
#endif
						bool isEmpty() const;

						bool hasConceptDescriptor(CConceptDescriptor* conceptDescriptor);


						CCondensedReapplyQueue* initReapplyQueue(CCondensedReapplyQueue* prevReapplyQueue);

						CCondensedReapplyQueueIterator getIterator(bool positiveDescriptors, bool negativeDescriptors, bool clearDynamicReapplyQueue);
						CCondensedReapplyQueueIterator getIterator(bool onlyPositiveDescriptors, bool clearDynamicReapplyQueue = true);
						CCondensedReapplyQueue* addReapplyConceptDescriptor(CCondensedReapplyConceptDescriptor *conProDes);


					// protected methods
					protected:

					// protected variables
					protected:
						CCondensedReapplyConceptDescriptor* mDynamicPosNegReapplyDesLinker;
#ifdef KONCLUDE_EMPLOY_REASONER_KERNEL_PROCESS_CONDENSED_REAPPLY_QUEUE_COUNT
						cint64 mDynamicPosNegCount;
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

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CCONDENSEDREAPPLYQUEUE_H
