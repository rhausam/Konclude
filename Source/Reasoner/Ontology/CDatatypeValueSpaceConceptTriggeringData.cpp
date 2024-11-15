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

#include "CDatatypeValueSpaceConceptTriggeringData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CDatatypeValueSpaceConceptTriggeringData::CDatatypeValueSpaceConceptTriggeringData() {
				mPartialTriggerLinker = nullptr;
				mCompleteTriggerLinker = nullptr;
			}


			CDatatypeValueSpaceConceptTriggeringData* CDatatypeValueSpaceConceptTriggeringData::initConceptTriggeringData(CDatatypeValueSpaceConceptTriggeringData* data) {
				if (data) {
					mPartialTriggerLinker = data->mPartialTriggerLinker;
					mCompleteTriggerLinker = data->mCompleteTriggerLinker;
				} else {
					mPartialTriggerLinker = nullptr;
					mCompleteTriggerLinker = nullptr;
				}
				return this;
			}


			bool CDatatypeValueSpaceConceptTriggeringData::hasPartialConceptTriggers() {
				return mPartialTriggerLinker != nullptr;
			}


			CDatatypeValueSpaceConceptTriggerLinker* CDatatypeValueSpaceConceptTriggeringData::getPartialConceptTriggerLinker() {
				return mPartialTriggerLinker;
			}

			CDatatypeValueSpaceConceptTriggeringData* CDatatypeValueSpaceConceptTriggeringData::setPartialConceptTriggerLinker(CDatatypeValueSpaceConceptTriggerLinker* conceptTiggerLinker) {
				mPartialTriggerLinker = conceptTiggerLinker;
				return this;
			}

			CDatatypeValueSpaceConceptTriggeringData* CDatatypeValueSpaceConceptTriggeringData::appendPartialConceptTriggers(CDatatypeValueSpaceConceptTriggerLinker* conceptTiggerLinker) {
				if (conceptTiggerLinker) {
					mPartialTriggerLinker = conceptTiggerLinker->append(mPartialTriggerLinker);
				}
				return this;
			}













			bool CDatatypeValueSpaceConceptTriggeringData::hasCompleteConceptTriggers() {
				return mCompleteTriggerLinker != nullptr;
			}


			CDatatypeValueSpaceConceptTriggerLinker* CDatatypeValueSpaceConceptTriggeringData::getCompleteConceptTriggerLinker() {
				return mCompleteTriggerLinker;
			}

			CDatatypeValueSpaceConceptTriggeringData* CDatatypeValueSpaceConceptTriggeringData::setCompleteConceptTriggerLinker(CDatatypeValueSpaceConceptTriggerLinker* conceptTiggerLinker) {
				mCompleteTriggerLinker = conceptTiggerLinker;
				return this;
			}

			CDatatypeValueSpaceConceptTriggeringData* CDatatypeValueSpaceConceptTriggeringData::appendCompleteConceptTriggers(CDatatypeValueSpaceConceptTriggerLinker* conceptTiggerLinker) {
				if (conceptTiggerLinker) {
					mCompleteTriggerLinker = conceptTiggerLinker->append(mCompleteTriggerLinker);
				}
				return this;
			}



			bool CDatatypeValueSpaceConceptTriggeringData::hasConceptTriggers() {
				return hasPartialConceptTriggers() || hasCompleteConceptTriggers();
			}

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
