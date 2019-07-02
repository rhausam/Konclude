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

#include "CCalculationChooseTaskHandleAlgorithm.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CCalculationChooseTaskHandleAlgorithm::CCalculationChooseTaskHandleAlgorithm(CTaskHandleAlgorithm* tableauCompTaskHandleAlg, CTaskHandleAlgorithm* tableauDefaultSaturTaskHandleAlg, CTaskHandleAlgorithm* tableauPilingSaturTaskHandleAlg, CTaskHandleAlgorithm* tableauApproxSaturTaskHandleAlg) {
					mTableauCompTaskHandleAlg = tableauCompTaskHandleAlg;
					mTableauDefaultSaturTaskHandleAlg = tableauDefaultSaturTaskHandleAlg;
					mTableauPilingSaturTaskHandleAlg = tableauPilingSaturTaskHandleAlg;
					mTableauApproxSaturTaskHandleAlg = tableauApproxSaturTaskHandleAlg;
				}

				bool CCalculationChooseTaskHandleAlgorithm::handleTask(CTaskProcessorContext *processorContext, CTask* task) {
					if (mTableauCompTaskHandleAlg && task->getTaskType() == CSatisfiableCalculationTask::CALCULATIONTABLEAUCOMPLETIONTASK) {
						return mTableauCompTaskHandleAlg->handleTask(processorContext,task);
					} else if (mTableauDefaultSaturTaskHandleAlg && task->getTaskType() == CSatisfiableCalculationTask::CALCULATIONTABLEAUAPPROXIMATEDSATURATIONTASK) {
						return mTableauApproxSaturTaskHandleAlg->handleTask(processorContext,task);
					} else if (mTableauDefaultSaturTaskHandleAlg && task->getTaskType() == CSatisfiableCalculationTask::CALCULATIONTABLEAUDEFAULTSATURATIONTASK) {
						return mTableauDefaultSaturTaskHandleAlg->handleTask(processorContext,task);
					} else if (mTableauPilingSaturTaskHandleAlg && task->getTaskType() == CSatisfiableCalculationTask::CALCULATIONTABLEAUPILINGSATURATIONTASK) {
						return mTableauPilingSaturTaskHandleAlg->handleTask(processorContext,task);
					}
					return false;
				}

			}; // end namespace Algorithm 

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude