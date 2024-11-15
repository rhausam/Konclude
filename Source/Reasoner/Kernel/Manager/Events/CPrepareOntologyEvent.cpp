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

#include "CPrepareOntologyEvent.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Manager {

				namespace Events {


					CPrepareOntologyEvent::CPrepareOntologyEvent(CConcreteOntology* ontology, const QList<COntologyProcessingRequirement*>& reqList, CCallbackData *callback) 
							: CCustomEvent(EVENTTYPE) {

						mOntology = ontology;
						mReqList = reqList;
						callbackData = callback;
					}



					CPrepareOntologyEvent::~CPrepareOntologyEvent() {
					}


					QList<COntologyProcessingRequirement*>* CPrepareOntologyEvent::getOntologyProcessingRequirementList() {
						return &mReqList;
					}

					CCallbackData *CPrepareOntologyEvent::getCallbackData() {
						return callbackData;
					}

					CConcreteOntology *CPrepareOntologyEvent::getOntology() {
						return mOntology;
					}




				}; // end namespace Events

			}; // end namespace Manager

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
