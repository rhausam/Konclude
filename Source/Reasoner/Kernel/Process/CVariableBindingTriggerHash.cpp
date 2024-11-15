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

#include "CVariableBindingTriggerHash.h"
#include "CIndividualProcessNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CVariableBindingTriggerHash::CVariableBindingTriggerHash(CProcessContext* context) : CPROCESSHASH<TVariableIndividualPair,CVariableBindingTriggerData>(context) {
					mContext = context;
				}


				CVariableBindingTriggerHash* CVariableBindingTriggerHash::initVariableBindingTriggerHash(CVariableBindingTriggerHash* prevHash) {
					if (prevHash) {
						CPROCESSHASH<TVariableIndividualPair, CVariableBindingTriggerData>::operator=(*prevHash);
					} else {
						CPROCESSHASH<TVariableIndividualPair,CVariableBindingTriggerData>::clear();
					}
					return this;
				}



				CVariableBindingTriggerData& CVariableBindingTriggerHash::getTriggerData(CVariable* variable, CIndividualProcessNode* indiNode) {
					return CPROCESSHASH<TVariableIndividualPair,CVariableBindingTriggerData>::operator[](TVariableIndividualPair(variable,indiNode->getIndividualNodeID()));
				}


				CVariableBindingTriggerLinker* CVariableBindingTriggerHash::setTriggeredReturnTriggerLinker(CVariable* variable, CIndividualProcessNode* indiNode) {
					CVariableBindingTriggerData& triggerData = getTriggerData(variable,indiNode);
					CVariableBindingTriggerLinker* triggerLinker = triggerData.getVariableBindingTriggerLinker();
					triggerData.setTriggered(true);
					triggerData.clearVariableBindingTriggerLinker();
					return triggerLinker;
				}


				bool CVariableBindingTriggerHash::tryInsertVariableBindingTrigger(CVariable* variable, CIndividualProcessNode* indiNode, CVariableBindingPathDescriptor* varBindPathDes, CVariableBindingDescriptor* varBindDes, bool leftTriggered) {
					CVariableBindingTriggerData& triggerData = getTriggerData(variable,indiNode);
					if (triggerData.isTriggered()) {
						return false;
					}
					CMemoryAllocationManager* taskMemMan = mContext->getUsedMemoryAllocationManager();
					CVariableBindingTriggerLinker* triggerLinker = CObjectAllocator< CVariableBindingTriggerLinker >::allocateAndConstruct(taskMemMan);
					triggerLinker->initTriggerLinker(varBindPathDes,varBindDes,leftTriggered);
					triggerData.addVariableBindingTriggerLinker(triggerLinker);
					return true;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
