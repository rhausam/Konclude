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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CNONDETERMINISTICDEPENDENCYTRACKPOINT_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CNONDETERMINISTICDEPENDENCYTRACKPOINT_H

// Libraries includes


// Namespace includes
#include "DependencySettings.h"
#include "CDependencyTrackPoint.h"


// Other includes
#include "Reasoner/Kernel/Process/CClashedDependencyDescriptor.h"

#include "Utilities/Container/CLinker.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				namespace Dependency {

					/*! 
					 *
					 *		\class		CNonDeterministicDependencyTrackPoint
					 *		\author		Andreas Steigmiller
					 *		\version	0.1
					 *		\brief		TODO
					 *
					 */
					class CNonDeterministicDependencyTrackPoint : public CDependencyTrackPoint, public CLinkerBase<CNonDeterministicDependencyTrackPoint*,CNonDeterministicDependencyTrackPoint> {
						// public methods
						public:
							//! Constructor
							CNonDeterministicDependencyTrackPoint(CDependencyNode* depNode = nullptr);

							CNonDeterministicDependencyTrackPoint* initBranch(CBranchTreeNode* branchNode = nullptr);

							CClashedDependencyDescriptor* getClashes();
							CNonDeterministicDependencyTrackPoint* addClashes(CClashedDependencyDescriptor* clashes, bool setClashed = true);
							CNonDeterministicDependencyTrackPoint* setClashes(CClashedDependencyDescriptor* clashes, bool setClashed = true);
							CNonDeterministicDependencyTrackPoint* setClashedOrIrelevantBranch(bool clashedOrIrelevant = true);

							CBranchTreeNode* getBranchNode();

							bool isClashedOrIrelevantBranch();

							CXLinker<cint64>* getInvolvedIndividualIdsLinker();
							CNonDeterministicDependencyTrackPoint* setInvolvedIndividualIdsLinker(CXLinker<cint64>* linker);

						// protected methods
						protected:

						// protected variables
						protected:
							CClashedDependencyDescriptor* mClashes;
							CBranchTreeNode* mBranchNode;
							bool mClashedIrelevant;
							CXLinker<cint64>* mInvolvedIndiLinker;

						// private methods
						private:

						// private variables
						private:


					};

				}; // end namespace Dependency

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CNONDETERMINISTICDEPENDENCYTRACKPOINT_H
