/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CIndividualAncestorDepthMaximumConceptProcessingPriorityStrategy.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Strategy {


				CIndividualAncestorDepthMaximumConceptProcessingPriorityStrategy::CIndividualAncestorDepthMaximumConceptProcessingPriorityStrategy() {
					mStrictIndiNodeProcessing = false;
					mAddIDIndiPriorization = false;
				}


				CIndividualAncestorDepthMaximumConceptProcessingPriorityStrategy::~CIndividualAncestorDepthMaximumConceptProcessingPriorityStrategy() {
				}


				void CIndividualAncestorDepthMaximumConceptProcessingPriorityStrategy::configureStrategy(bool strictIndiNodeProcessing, bool additionalIDIndiPriorization) {
					mStrictIndiNodeProcessing = strictIndiNodeProcessing;
					mAddIDIndiPriorization = additionalIDIndiPriorization;
				}


				CIndividualProcessNodePriority CIndividualAncestorDepthMaximumConceptProcessingPriorityStrategy::getPriorityForIndividual(CIndividualProcessNode* individual) {
					double conPriority = 0.;
					CConceptProcessingQueue* conProQueue = individual->getConceptProcessingQueue(false);
					if (conProQueue) {
						CConceptProcessPriority conProPriority;
						if (conProQueue->getNextConceptProcessPriority(&conProPriority)) {
							conPriority = conProPriority.getPriority();
						}
					}
					double indiPriority = individual->getIndividualAncestorDepth();
					if (mAddIDIndiPriorization) {
						indiPriority += - 1. / (10+individual->getIndividualID()) + 0.1;
					}
					return CIndividualProcessNodePriority(conPriority,indiPriority,mStrictIndiNodeProcessing);
				}


			}; // end namespace Strategy

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
