/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CDependencyTrackPoint.h"
#include "CDependencyNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				namespace Dependency {


					CDependencyTrackPoint::CDependencyTrackPoint(CDependencyNode* depNode) : CBranchingTag(0) {
						mDepNode = depNode;
						mRelevantFlag = false;
					}

					CDependencyNode* CDependencyTrackPoint::getDependencyNode() {
						return mDepNode;
					}


					cint64 CDependencyTrackPoint::getProcessingTag() {
						return mDepNode->getProcessingTag();
					}


					bool CDependencyTrackPoint::isPointingToDeterministicDependencyNode() {
						return mDepNode->isDeterministiDependencyNode();
					}

					bool CDependencyTrackPoint::isPointingToIndependentDependencyNode() {
						return mDepNode->isIndependentBaseDependencyType();
					}

					bool CDependencyTrackPoint::isDependencyRelevant() {
						return mRelevantFlag;
					}

					CDependencyTrackPoint* CDependencyTrackPoint::setDependencyRelevance(bool relevant) {
						mRelevantFlag = relevant;
						return this;
					}



				}; // end namespace Dependency

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
