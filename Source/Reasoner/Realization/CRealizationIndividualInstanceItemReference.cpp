/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CRealizationIndividualInstanceItemReference.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realization {


			CRealizationIndividualInstanceItemReference::CRealizationIndividualInstanceItemReference() : CIndividualReference(-1) {
				mRealizationInstanceItem = nullptr;
			}

			CRealizationIndividualInstanceItemReference::CRealizationIndividualInstanceItemReference(cint64 id, CSameInstanceItem* realizationInstanceItem) : CIndividualReference(id) {
				mRealizationInstanceItem = realizationInstanceItem;
			}


			CRealizationIndividualInstanceItemReference::CRealizationIndividualInstanceItemReference(const CIndividualIdentifier& identifier, CSameInstanceItem* realizationInstanceItem) : CIndividualReference(identifier) {
				mRealizationInstanceItem = realizationInstanceItem;
			}

			CRealizationIndividualInstanceItemReference::CRealizationIndividualInstanceItemReference(CIndividual* individual, CSameInstanceItem* realizationInstanceItem) : CIndividualReference(individual) {
				mRealizationInstanceItem = realizationInstanceItem;
			}

			CRealizationIndividualInstanceItemReference::CRealizationIndividualInstanceItemReference(CIndividual* individual, cint64 indiId, CSameInstanceItem* realizationInstanceItem) : CIndividualReference(individual, indiId) {
				mRealizationInstanceItem = realizationInstanceItem;
			}

			CRealizationIndividualInstanceItemReference::CRealizationIndividualInstanceItemReference(const CIndividualReference& indiRef, CSameInstanceItem* realizationInstanceItem) : CIndividualReference(indiRef) {
				mRealizationInstanceItem = realizationInstanceItem;
			}

			CSameInstanceItem* CRealizationIndividualInstanceItemReference::getRealizationInstanceItem() const {
				return mRealizationInstanceItem;
			}




		}; // end namespace Realization

	}; // end namespace Reasoner

}; // end namespace Konclude