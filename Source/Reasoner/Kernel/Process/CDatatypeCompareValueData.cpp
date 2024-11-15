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

#include "CDatatypeCompareValueData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeCompareValueData::CDatatypeCompareValueData(CProcessContext* processContext) {
					mProcessContext = processContext;

					mValue = nullptr;

				}



				CDatatypeCompareValueData* CDatatypeCompareValueData::initCompareValueData(CDatatypeCompareValueData* valueData, CDataLiteralCompareValue* value) {
					mValue = value;
					if (valueData) {
						mValueExclusionData.initDataExclusion(&valueData->mValueExclusionData);
						mLeftExclusionData.initDataExclusion(&valueData->mLeftExclusionData);
						mRightExclusionData.initDataExclusion(&valueData->mRightExclusionData);
					} else {
						mValueExclusionData.initDataExclusion(nullptr);
						mLeftExclusionData.initDataExclusion(nullptr);
						mRightExclusionData.initDataExclusion(nullptr);
					}
					return this;
				}



				CDatatypeCompareValueData* CDatatypeCompareValueData::initCompareValueData(CDataLiteralCompareValue* value) {
					mValue = value;
					return this;
				}

				CDataLiteralCompareValue* CDatatypeCompareValueData::getValue() {
					return mValue;
				}


				CDatatypeCompareValueData* CDatatypeCompareValueData::setValue(CDataLiteralCompareValue* value) {
					mValue = value;
					return this;
				}


				CDatatypeCompareValueDataExclusion* CDatatypeCompareValueData::getValueExclusionData() {
					return &mValueExclusionData;
				}

				CDatatypeCompareValueDataExclusion* CDatatypeCompareValueData::getLeftExclusionData() {
					return &mLeftExclusionData;
				}

				CDatatypeCompareValueDataExclusion* CDatatypeCompareValueData::getRightExclusionData() {
					return &mRightExclusionData;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
