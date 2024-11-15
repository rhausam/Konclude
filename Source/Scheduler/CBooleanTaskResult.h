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

#ifndef KONCLUDE_SCHEDULER_CBOOLEANTASKRESULT_H
#define KONCLUDE_SCHEDULER_CBOOLEANTASKRESULT_H

// Libraries includes


// Namespace includes
#include "SchedulerSettings.h"
#include "CTaskResult.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Scheduler {


		/*! 
		 *
		 *		\class		CBooleanTaskResult
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CBooleanTaskResult : public CTaskResult {
			// public methods
			public:
				//! Constructor
				CBooleanTaskResult();

				//! Destructor
				virtual ~CBooleanTaskResult();

				virtual bool hasResult();

				bool getResultValue();
				bool hasResultValue(bool testValue = true);

				CBooleanTaskResult* setResultValue(bool resultValue);
				CBooleanTaskResult* setValidResult(bool validResult);

				CBooleanTaskResult* installResult(bool resultValue);


			// protected methods
			protected:

			// protected variables
			protected:
				bool mValidResult;
				bool mResultValue;


			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_CBOOLEANTASKRESULT_H
