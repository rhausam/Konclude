/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CTaskPreyingAdapter.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Task {

				CTaskPreyingAdapter::CTaskPreyingAdapter() {
					mConsObs = nullptr;
					mPreyListner = nullptr;
					mSaturationObs = nullptr;
				}

				CTaskPreyingAdapter::CTaskPreyingAdapter(CConsistenceObserver* consObs) {
					mConsObs = consObs;
					mPreyListner = nullptr;
					mSaturationObs = nullptr;
				}

				CTaskPreyingAdapter::CTaskPreyingAdapter(CSaturationObserver* saturationObs) {
					mSaturationObs = saturationObs;
					mConsObs = nullptr;
					mPreyListner = nullptr;
				}

				CTaskPreyingAdapter::CTaskPreyingAdapter(CTaskPreyingListner* preyedListner) {
					mConsObs = nullptr;
					mPreyListner = preyedListner;
				}

				CConsistenceObserver* CTaskPreyingAdapter::getConsitenceObserver() {
					return mConsObs;
				}

				CTaskPreyingAdapter* CTaskPreyingAdapter::setConsitenceObserver(CConsistenceObserver* consObs) {
					mConsObs = consObs;
					return this;
				}

				CTaskPreyingListner* CTaskPreyingAdapter::getTaskPreyingListner() {
					return mPreyListner;
				}

				CTaskPreyingAdapter* CTaskPreyingAdapter::setTaskPreyingListner(CTaskPreyingListner* preyedListner) {
					mPreyListner = preyedListner;
					return this;
				}

				CSaturationObserver* CTaskPreyingAdapter::getSaturationObserver() {
					return mSaturationObs;
				}

				CTaskPreyingAdapter* CTaskPreyingAdapter::setSaturationObserver(CSaturationObserver* saturationObs) {
					mSaturationObs = saturationObs;
					return this;
				}

			}; // end namespace Task

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
