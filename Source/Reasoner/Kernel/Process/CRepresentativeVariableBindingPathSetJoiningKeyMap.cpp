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

#include "CRepresentativeVariableBindingPathSetJoiningKeyMap.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CRepresentativeVariableBindingPathSetJoiningKeyMap::CRepresentativeVariableBindingPathSetJoiningKeyMap(CProcessContext* processContext) : mProcessContext(processContext),CPROCESSMAP<cint64,CRepresentativeVariableBindingPathSetJoiningKeyMapData>(processContext) {
				}

				CRepresentativeVariableBindingPathSetJoiningKeyMap* CRepresentativeVariableBindingPathSetJoiningKeyMap::initRepresentativeVariableBindingPathSetJoiningKeyMap(CRepresentativeVariableBindingPathSetJoiningKeyMap* prevMap) {
					if (prevMap) {
						*this = *prevMap;
					} else {
						CPROCESSMAP<cint64,CRepresentativeVariableBindingPathSetJoiningKeyMapData>::clear();
					}
					return this;
				}


				CRepresentativeVariableBindingPathSetJoiningKeyDataMap* CRepresentativeVariableBindingPathSetJoiningKeyMap::getJoiningKeyDataMap(cint64 joiningKey, bool create) {
					CRepresentativeVariableBindingPathSetJoiningKeyDataMap* dataMap = nullptr;
					if (create) {
						CRepresentativeVariableBindingPathSetJoiningKeyMapData& data = CPROCESSMAP<cint64,CRepresentativeVariableBindingPathSetJoiningKeyMapData>::operator[](joiningKey);
						dataMap = data.getRepresentativeVariableBindingPathSetJoiningKeyDataMap();
						if (!dataMap) {
							dataMap = CObjectParameterizingAllocator< CRepresentativeVariableBindingPathSetJoiningKeyDataMap,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
							dataMap->initVariableBindingPathSetJoiningKeyDataMap(nullptr);
							data.setRepresentativeVariableBindingPathSetJoiningKeyDataMap(dataMap);
						}
					} else {
						const CRepresentativeVariableBindingPathSetJoiningKeyMapData& data = CPROCESSMAP<cint64,CRepresentativeVariableBindingPathSetJoiningKeyMapData>::value(joiningKey);
						dataMap = data.getRepresentativeVariableBindingPathSetJoiningKeyDataMap();
					}
					return dataMap;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
