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

#include "CCalcedQueryEvent.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Manager {

				namespace Events {


					CCalcedQueryEvent::CCalcedQueryEvent(CThread *receiverThread, CQuery *query) 
							: CCustomEvent(EVENTTYPE) {

						this->query = query;
						recThread = receiverThread;

						queryString = query->getQueryString();
						queryName = query->getQueryName();
					}



					CCalcedQueryEvent::~CCalcedQueryEvent() {
					}

					void CCalcedQueryEvent::doCallback() {
						answerString = query->getAnswerString();
						if (recThread) {
							recThread->postEvent(this);
						}
					}

					void CCalcedQueryEvent::sendEventWithAnswer(const QString& newAnswerString) {
						answerString = newAnswerString;
						if (recThread) {
							recThread->postEvent(this);
						}
					}

					QString CCalcedQueryEvent::getAnswerString() {
						return answerString;
					}

					QString CCalcedQueryEvent::getQueryString() {
						return queryString;
					}

					QString CCalcedQueryEvent::getQueryName() {
						return queryName;
					}

					CQuery *CCalcedQueryEvent::getQuery() {
						return query;
					}

				}; // end namespace Events

			}; // end namespace Manager

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
