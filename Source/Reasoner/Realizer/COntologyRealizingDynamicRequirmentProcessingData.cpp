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

#include "COntologyRealizingDynamicRequirmentProcessingData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COntologyRealizingDynamicRequirmentProcessingData::COntologyRealizingDynamicRequirmentProcessingData(COntologyProcessingRequirement* procReq, COntologyRealizingDynamicRequirmentCallbackData* callback) {
				mAssRealStep = nullptr;
				mCallbackData = callback;
				mProcessingItemCount = 0;
				mProcReq = procReq;
				mStatistics = nullptr;
				if (callback) {
					mStatistics = callback->getStatistics();
				}
			}


			COntologyRealizingDynamicRequirmentProcessingData* COntologyRealizingDynamicRequirmentProcessingData::incProcessingItemCount(cint64 incCount) {
				mProcessingItemCount += incCount;
				return this;
			}


			COntologyRealizingDynamicRequirmentProcessingData* COntologyRealizingDynamicRequirmentProcessingData::decProcessingItemCount(cint64 decCount) {
				mProcessingItemCount -= decCount;
				return this;
			}

			COntologyRealizingDynamicRequirmentProcessingData* COntologyRealizingDynamicRequirmentProcessingData::setCallbackData(COntologyRealizingDynamicRequirmentCallbackData* callback) {
				mCallbackData = callback;
				return this;
			}

			cint64 COntologyRealizingDynamicRequirmentProcessingData::getCurrentProcessingItemCount() {
				return mProcessingItemCount;
			}

			bool COntologyRealizingDynamicRequirmentProcessingData::hasCurrentProcessingItemCount() {
				return mProcessingItemCount > 0;
			}

			CRealizingTestingStep* COntologyRealizingDynamicRequirmentProcessingData::getAssociatedRelizationTestingStep() {
				return mAssRealStep;
			}

			COntologyRealizingDynamicRequirmentProcessingData* COntologyRealizingDynamicRequirmentProcessingData::setAssociatedRelizationTestingStep(CRealizingTestingStep* step) {
				mAssRealStep = step;
				return this;
			}

			COntologyRealizingDynamicRequirmentCallbackData* COntologyRealizingDynamicRequirmentProcessingData::getCallbackData() {
				return mCallbackData;
			}

			COntologyProcessingRequirement* COntologyRealizingDynamicRequirmentProcessingData::getProcessingRequirement() {
				return mProcReq;
			}

			COntologyRealizingDynamicRequirmentProcessingData* COntologyRealizingDynamicRequirmentProcessingData::setProcessingRequirement(COntologyProcessingRequirement* procReq) {
				mProcReq = procReq;
				return this;
			}

			COntologyRealizingDynamicRequirmentProcessingStatistics* COntologyRealizingDynamicRequirmentProcessingData::getStatistics() {
				return mStatistics;
			}


		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude