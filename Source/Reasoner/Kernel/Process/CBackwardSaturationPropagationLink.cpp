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

#include "CBackwardSaturationPropagationLink.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CBackwardSaturationPropagationLink::CBackwardSaturationPropagationLink() : CLinkerBase<CRole*,CBackwardSaturationPropagationLink>(nullptr) {
					mSourceIndividual = nullptr;
				}


				CBackwardSaturationPropagationLink* CBackwardSaturationPropagationLink::initBackwardPropagationLink(CIndividualSaturationProcessNode* sourceIndividual, CRole* role) {
					setData(role);
					mSourceIndividual = sourceIndividual;
					return this;
				}


				CRole* CBackwardSaturationPropagationLink::getLinkRole() {
					return getData();
				}

				CBackwardSaturationPropagationLink* CBackwardSaturationPropagationLink::setLinkRole(CRole* role) {
					setData(role);
					return this;
				}

				CIndividualSaturationProcessNode* CBackwardSaturationPropagationLink::getSourceIndividual() {
					return mSourceIndividual;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
