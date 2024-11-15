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

#include "CCallbackListExecuter.h"



namespace Konclude {

	namespace Concurrent {

		namespace Callback {



			CCallbackListExecuter::CCallbackListExecuter() {
			}


			CCallbackListExecuter::~CCallbackListExecuter() {
				callbackList.clear();
			}

			void CCallbackListExecuter::doCallback() {
				foreach (CCallbackData *callbackData, callbackList) {
					callbackData->doCallback();
				}
				callbackList.clear();
			}


			qint64 CCallbackListExecuter::getCallbackCount() {
				return callbackList.count();
			}



			CCallbackListExecuter *CCallbackListExecuter::addCallbackData(CCallbackData *callbackData) {
				callbackList.removeAll(callbackData);
				callbackList.append(callbackData);
				return this;
			}

			CCallbackListExecuter *CCallbackListExecuter::removeCallbackData(CCallbackData *callbackData) {
				callbackList.removeAll(callbackData);
				return this;
			}

		}; // end namespace Callback

	}; // end namespace Concurrent

}; // end namespace Konclude