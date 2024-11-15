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

#include "CClassificationProgress.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			CClassificationProgress::CClassificationProgress() {
				resetValues();
			}


			qint64 CClassificationProgress::getClassificationCount() {
				return mClassificationCount;
			}

			qint64 CClassificationProgress::getTestedSatisfiable() {
				return mTestedClasses;
			}

			qint64 CClassificationProgress::getTotalSatisfiable() {
				return mTotalClasses;
			}

			qint64 CClassificationProgress::getTestedSubsumptions() {
				return mTestedSubsum;
			}

			qint64 CClassificationProgress::getTotalSubsumptions() {
				return mTotalSubsum;
			}


			double CClassificationProgress::getRemainingMilliSeconds() {
				return mRemainingMilliSeconds;
			}

			double CClassificationProgress::getProgessPercent() {
				return mProgessPercent;
			}

			CClassificationProgress *CClassificationProgress::setClassificationCount(qint64 classificationCount) {
				mClassificationCount = classificationCount;
				return this;
			}

			CClassificationProgress *CClassificationProgress::setTestedSatisfiable(qint64 testedClasses) {
				mTestedClasses = testedClasses;
				return this;
			}

			CClassificationProgress *CClassificationProgress::setTotalSatisfiable(qint64 totalClasses) {
				mTotalClasses = totalClasses;
				return this;
			}

			CClassificationProgress *CClassificationProgress::setTestedSubsumptions(qint64 testedSubsumptions) {
				mTestedSubsum = testedSubsumptions;
				return this;
			}

			CClassificationProgress *CClassificationProgress::setTotalSubsumptions(qint64 totalSubsumptions) {
				mTotalSubsum = totalSubsumptions;
				return this;
			}

			CClassificationProgress *CClassificationProgress::setRemainingMilliSeconds(double remainingMilliSeconds) {
				mRemainingMilliSeconds = remainingMilliSeconds;
				return this;
			}

			CClassificationProgress *CClassificationProgress::setProgessPercent(double progessPercent) {
				mProgessPercent = progessPercent;
				return this;
			}

			CClassificationProgress *CClassificationProgress::resetValues() {
				mClassificationCount = 0;
				mTestedClasses = 0;
				mTotalClasses = 0;
				mRemainingMilliSeconds = 0.;
				mProgessPercent = 0.;
				mTotalSubsum = 0;
				mTestedSubsum = 0;
				return this;
			}



		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
