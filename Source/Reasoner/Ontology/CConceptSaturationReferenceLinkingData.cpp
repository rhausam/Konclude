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

#include "CConceptSaturationReferenceLinkingData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			CConceptSaturationReferenceLinkingData::CConceptSaturationReferenceLinkingData() {
				mPositiveSatConRefLinking = nullptr;
				mNegativeSatConRefLinking = nullptr;
				mExistentialSuccessorSatConRefLinking = nullptr;
			}

			CSaturationConceptReferenceLinking* CConceptSaturationReferenceLinkingData::getPositiveConceptSaturationReferenceLinkingData() {
				return mPositiveSatConRefLinking;
			}

			CConceptSaturationReferenceLinkingData* CConceptSaturationReferenceLinkingData::setPositiveSaturationReferenceLinkingData(CSaturationConceptReferenceLinking* satConRefLinking) {
				mPositiveSatConRefLinking = satConRefLinking;
				return this;
			}

			CSaturationConceptReferenceLinking* CConceptSaturationReferenceLinkingData::getNegativeConceptSaturationReferenceLinkingData() {
				return mNegativeSatConRefLinking;
			}

			CConceptSaturationReferenceLinkingData* CConceptSaturationReferenceLinkingData::setNegativeSaturationReferenceLinkingData(CSaturationConceptReferenceLinking* satConRefLinking) {
				mNegativeSatConRefLinking = satConRefLinking;
				return this;
			}

			CConceptSaturationReferenceLinkingData* CConceptSaturationReferenceLinkingData::setSaturationReferenceLinkingData(CSaturationConceptReferenceLinking* satConRefLinking, bool negated) {
				if (!negated) {
					mPositiveSatConRefLinking = satConRefLinking;
				} else {
					mNegativeSatConRefLinking = satConRefLinking;
				}
				return this;
			}

			CSaturationConceptReferenceLinking* CConceptSaturationReferenceLinkingData::getConceptSaturationReferenceLinkingData(bool negated) {
				if (!negated) {
					return mPositiveSatConRefLinking;
				} else {
					return mNegativeSatConRefLinking;
				}
			}

			CSaturationConceptReferenceLinking* CConceptSaturationReferenceLinkingData::getExistentialSuccessorConceptSaturationReferenceLinkingData() {
				return mExistentialSuccessorSatConRefLinking;
			}

			CConceptSaturationReferenceLinkingData* CConceptSaturationReferenceLinkingData::setExistentialSuccessorConceptSaturationReferenceLinkingData(CSaturationConceptReferenceLinking* satConRefLinking) {
				mExistentialSuccessorSatConRefLinking = satConRefLinking;
				return this;
				}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
