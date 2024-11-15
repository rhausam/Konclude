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

#include "CRepresentativeJoiningAllDataExtension.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CRepresentativeJoiningAllDataExtension::CRepresentativeJoiningAllDataExtension(CProcessContext* processContext) : mProcessContext(processContext) {
					mRepVarBindPathSetData = nullptr;
					mRightResolveMap = nullptr;
					mLeftResolveMap = nullptr;
				}

				CRepresentativeVariableBindingPathSetData* CRepresentativeJoiningAllDataExtension::getRepresentativeVariableBindingPathSetData() {
					return mRepVarBindPathSetData;
				}


				CRepresentativeJoiningAllDataExtension* CRepresentativeJoiningAllDataExtension::setRepresentativeVariableBindingPathSetData(CRepresentativeVariableBindingPathSetData* repVarBindPathSetData) {
					mRepVarBindPathSetData = repVarBindPathSetData;
					return this;
				}

				CRepresentativeVariableBindingPathMap* CRepresentativeJoiningAllDataExtension::getResolveVariableBindingPathMap(bool leftMap, bool create) {
					if (leftMap) {
						return getLeftResolveVariableBindingPathMap(create);
					} else {
						return getRightResolveVariableBindingPathMap(create);
					}
				}


				CRepresentativeVariableBindingPathMap* CRepresentativeJoiningAllDataExtension::getLeftResolveVariableBindingPathMap(bool create) {
					if (create && !mLeftResolveMap) {
						mLeftResolveMap = CObjectParameterizingAllocator< CRepresentativeVariableBindingPathMap,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mLeftResolveMap->initVariableBindingPathMap(nullptr);
					}
					return mLeftResolveMap;
				}

				CRepresentativeVariableBindingPathMap* CRepresentativeJoiningAllDataExtension::getRightResolveVariableBindingPathMap(bool create) {
					if (create && !mRightResolveMap) {
						mRightResolveMap = CObjectParameterizingAllocator< CRepresentativeVariableBindingPathMap,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mRightResolveMap->initVariableBindingPathMap(nullptr);
					}
					return mRightResolveMap;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
