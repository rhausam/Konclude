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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CDEPENDENCYTRACKER_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CDEPENDENCYTRACKER_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"


// Other includes
#include "Reasoner/Kernel/Process/Dependency/CDependencyTrackPoint.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				using namespace Dependency;

				/*! 
				 *
				 *		\class		CDependencyTracker
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CDependencyTracker {
					// public methods
					public:
						//! Constructor
						CDependencyTracker(CDependencyTrackPoint* dependencyTrackPoint = nullptr);

						CDependencyTrackPoint* getDependencyTrackPoint() const;
						CDependencyTracker* setDependencyTrackPoint(CDependencyTrackPoint* dependencyTrackPoint);
						CDependencyTracker* initDependencyTracker(CDependencyTrackPoint* dependencyTrackPoint);


					// protected methods
					protected:

					// protected variables
					protected:
						CDependencyTrackPoint* mDependencyTrackPoint;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CDEPENDENCYTRACKER_H
