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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CPROCESSCONTEXTBASE_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CPROCESSCONTEXTBASE_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Scheduler;

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CProcessContextBase
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CProcessContextBase : public CProcessContext {
					// public methods
					public:
						//! Constructor
						CProcessContextBase(CMemoryPoolContainer* memoryPoolContainer, CTaskHandleContext* handleContext);

						//! Destructor
						virtual ~CProcessContextBase();

						CProcessContextBase* referenceProcessContext(CProcessContextBase* reference);

						virtual CProcessTagger* getProcessTagger();
						virtual CMemoryAllocationManager* getMemoryAllocationManager();
						virtual CProcessingStatisticGathering* getProcessingStatisticsGathering();

						virtual CTaskContext* updateContext(CTaskHandleContext* handleContext);

					// protected methods
					protected:

					// protected variables
					protected:
						CProcessMemoryPoolAllocationManager* mMemMan;
						CProcessTagger* mProcessTagger;
						CProcessingStatisticGathering* mProcStatGath;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CPROCESSCONTEXTBASE_H
