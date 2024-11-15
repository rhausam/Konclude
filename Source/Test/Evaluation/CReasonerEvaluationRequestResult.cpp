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

#include "CReasonerEvaluationRequestResult.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationRequestResult::CReasonerEvaluationRequestResult() {
				mResponsedInTime = false;
				mResponseTime = 0;
				mSucessfullCommunication = false;
			}

			CReasonerEvaluationRequestResult::~CReasonerEvaluationRequestResult() {
				qDeleteAll(mResponseList);
				qDeleteAll(mInitResponseList);
			}

			CReasonerEvaluationRequestResult* CReasonerEvaluationRequestResult::addResponse(CReasonerEvaluationRequestResponse* response, bool initRequestResponse) {
				if (initRequestResponse) {
					mInitResponseList.append(response);
				} else {
					mResponseList.append(response);
				}
				return this;
			}

			QList<CReasonerEvaluationRequestResponse*>* CReasonerEvaluationRequestResult::getResponseList() {
				return &mResponseList;
			}

			CReasonerEvaluationRequestResult* CReasonerEvaluationRequestResult::setResponseTime(cint64 responseTime) {
				mResponseTime = responseTime;
				return this;
			}

			CReasonerEvaluationRequestResult* CReasonerEvaluationRequestResult::setResponsedInTime(bool responsedInTime) {
				mResponsedInTime = responsedInTime;
				return this;
			}

			bool CReasonerEvaluationRequestResult::hasSucessfullReasonerCommunication() {
				return mSucessfullCommunication;
			}

			CReasonerEvaluationRequestResult* CReasonerEvaluationRequestResult::setSucessfullReasonerCommunication(bool sucessfull) {
				mSucessfullCommunication = sucessfull;
				return this;
			}



			bool CReasonerEvaluationRequestResult::getResponsedInTime() {
				return mResponsedInTime;
			}

			cint64 CReasonerEvaluationRequestResult::getResponseTime() {
				return mResponseTime;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
