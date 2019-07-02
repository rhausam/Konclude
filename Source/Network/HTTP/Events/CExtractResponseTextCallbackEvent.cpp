/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CExtractResponseTextCallbackEvent.h"


namespace Konclude {

	namespace Network {

		namespace HTTP {

			namespace Events {


				CExtractResponseTextCallbackEvent::CExtractResponseTextCallbackEvent(CThread* thread, CQtHttpResponse* response, QString* text, CCallbackData* callbacks) 
						: CCustomEvent(EVENTTYPE) {
					mResponse = response;
					mText = text;
					mCallbacks = callbacks;
					mThread = thread;
				}

				CExtractResponseTextCallbackEvent::~CExtractResponseTextCallbackEvent() {
				}


				CQtHttpResponse* CExtractResponseTextCallbackEvent::getResponse() {
					return mResponse;
				}

				QString* CExtractResponseTextCallbackEvent::getText() {
					return mText;
				}

				CCallbackData* CExtractResponseTextCallbackEvent::getCallbacks() {
					return mCallbacks;
				}


				void CExtractResponseTextCallbackEvent::doCallback() {
					mThread->postEvent(this); 
				}


			}; // end namespace Events

		}; // end namespace HTTP

	}; // end namespace Network

}; // end namespace Konclude
