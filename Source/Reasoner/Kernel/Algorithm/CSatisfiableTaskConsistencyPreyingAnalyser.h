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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATISFIABLETASKCONSISTENCYPREYINGANALYSER_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATISFIABLETASKCONSISTENCYPREYINGANALYSER_H

// Libraries includes


// Namespace includes
#include "CCalculationAlgorithmContext.h"
#include "CSatisfiableConsistencyPreyingAnalyser.h"

// Other includes
#include "Reasoner/Kernel/Task/CSatisfiableCalculationTask.h"
#include "Reasoner/Kernel/Task/CConsistenceTaskData.h"

// Logger includes
#include "Logger/CLogger.h"




namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			using namespace Task;

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CSatisfiableTaskConsistencyPreyingAnalyser
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSatisfiableTaskConsistencyPreyingAnalyser : public CSatisfiableConsistencyPreyingAnalyser {
					// public methods
					public:
						//! Constructor
						CSatisfiableTaskConsistencyPreyingAnalyser();

						//! Destructor
						virtual ~CSatisfiableTaskConsistencyPreyingAnalyser();

						bool analyseSatisfiableTask(CSatisfiableCalculationTask* statCalcTask, CCalculationAlgorithmContext* calcAlgContext);



					// protected methods
					protected:

					// protected variables
					protected:

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATISFIABLETASKCONSISTENCYPREYINGANALYSER_H
