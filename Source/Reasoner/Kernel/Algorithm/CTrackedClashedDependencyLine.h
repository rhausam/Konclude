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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CTRACKEDCLASHEDDEPENDENCYLINE_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CTRACKEDCLASHEDDEPENDENCYLINE_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CTrackedClashedDescriptor.h"
#include "CTrackedClashedDescriptorHasher.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CTrackedClashedDependencyLine
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CTrackedClashedDependencyLine {
					// public methods
					public:
						//! Constructor
						CTrackedClashedDependencyLine(CPROCESSINGSET<CTrackedClashedDescriptorHasher>* clashedSet);


						CTrackedClashedDependencyLine* initTrackedClashedDependencyLine(bool exactIndiNodeTracking, cint64 individualNodeTrackLevel, cint64 branchingLevel);

						CTrackedClashedDependencyLine* sortInTrackedClashedDescriptors(CTrackedClashedDescriptor* clashedDes, bool forceInsertion = false);
						CTrackedClashedDependencyLine* moveToNextIndividualNodeLevel(cint64 newLevel);

						CTrackedClashedDependencyLine* analyseInvolvedIndividuals(CTrackedClashedDescriptor* clashedDes);
						CTrackedClashedDependencyLine* addInvolvedIndividual(CIndividualProcessNode* indiNode);
						CTrackedClashedDependencyLine* addInvolvedIndividual(cint64 indiId);


						bool hasLevelTrackedClashedDescriptors();
						CTrackedClashedDescriptor* takeNextLevelTrackedClashedDescriptor();
						CTrackedClashedDescriptor* getLevelTrackedClashedDescriptors();

						bool hasLevelTrackedBranchingClashedDescriptors();
						CTrackedClashedDescriptor* takeNextLevelTrackedBranchingClashedDescriptor();
						CTrackedClashedDescriptor* getLevelTrackedBranchingClashedDescriptors();

						bool hasPerviousLevelTrackedClashedDescriptors();
						CTrackedClashedDescriptor* takeNextPerviousLevelTrackedClashedDescriptor();
						CTrackedClashedDescriptor* getPerviousLevelTrackedClashedDescriptors();


						bool hasPerviousLevelTrackedNonDeterministicClashedDescriptors();
						CTrackedClashedDescriptor* takeNextPerviousLevelTrackedNonDeterministicClashedDescriptor();
						CTrackedClashedDescriptor* getPerviousLevelTrackedNonDeterministicClashedDescriptors();

						bool hasPerviousLevelTrackedNonDeterministicBranchingClashedDescriptors();
						CTrackedClashedDescriptor* takeNextPerviousLevelTrackedNonDeterministicBranchingClashedDescriptor();
						CTrackedClashedDescriptor* getPerviousLevelTrackedNonDeterministicBranchingClashedDescriptors();

						bool hasIndependentTrackedClashedDescriptors();
						CTrackedClashedDescriptor* takeNextIndependentTrackedClashedDescriptor();
						CTrackedClashedDescriptor* getIndependentTrackedClashedDescriptors();


						CTrackedClashedDescriptor* takeNextTrackedClashedList();
						bool hasMoreTrackedClashedList();


						CTrackedClashedDescriptor* takeNextFreeTrackedClashedDescriptor();
						CTrackedClashedDependencyLine* addFreeTrackedClashedDescriptor(CTrackedClashedDescriptor* clashDes);



						CPROCESSINGSET<CTrackedClashedDescriptorHasher>* getTrackedClashedDescriptorSet();

						cint64 getBranchingLevel();
						cint64 getIndividualNodeLevel();

						bool hasOnlyIndependentTrackedClashedDescriptorsRemaining();
						bool hasOnlyCurrentIndividualNodeLevelClashesDescriptors();


						CTrackedClashedDependencyLine* setInvolvedIndividualTrackingSet(CPROCESSINGSET<cint64>* indiTrackingSet);
						CPROCESSINGSET<cint64>* getInvolvedIndividualTrackingSet();


					// protected methods
					protected:

					// protected variables
					protected:
						bool mExactIndividualTracking;
						cint64 mIndividualTrackLevel;
						cint64 mBranchingLevel;
						CTrackedClashedDescriptor* mIndependentTrackedClashes;
						CTrackedClashedDescriptor* mLevelTrackedClashes;
						CTrackedClashedDescriptor* mLevelTrackedBranchingClashes;
						CTrackedClashedDescriptor* mPrevLevelsTrackedClashes;
						CTrackedClashedDescriptor* mPrevLevelsTrackedNonDetClashes;
						CTrackedClashedDescriptor* mPrevLevelsTrackedNonDetBranchingClashes;

						CTrackedClashedDescriptor* mFreeTrackedClashedDescriptors;

						CPROCESSINGSET<CTrackedClashedDescriptorHasher>* mClashedSet;

						CPROCESSINGSET<cint64>* mInvolvedIndividualSet;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CTRACKEDCLASHEDDEPENDENCYLINE_H
