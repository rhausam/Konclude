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

#include "CClassifyOntologyEvent.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {

			namespace Events {


				CClassifyOntologyEvent::CClassifyOntologyEvent(CConcreteOntology* ontology, CConfigurationBase* configuration, const QList<COntologyProcessingRequirement*>& requirementList, CCallbackData* callbackData) 
						: CCustomEvent(EVENTTYPE) {
					onto = ontology;
					mReqList = requirementList;
					callback = callbackData;
					config = configuration;
				}



				CClassifyOntologyEvent::~CClassifyOntologyEvent() {
				}


				CConcreteOntology *CClassifyOntologyEvent::getOntology() {
					return onto;
				}

				QList<COntologyProcessingRequirement*>* CClassifyOntologyEvent::getClassificationRequirementList() {
					return &mReqList;
				}

				CCallbackData *CClassifyOntologyEvent::getCallbackData() {
					return callback;
				}

				CConfigurationBase *CClassifyOntologyEvent::getConfiguration() {
					return config;
				}

			}; // end namespace Events

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
