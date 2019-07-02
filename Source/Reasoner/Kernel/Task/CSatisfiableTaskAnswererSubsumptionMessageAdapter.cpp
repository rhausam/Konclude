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

#include "CSatisfiableTaskAnswererSubsumptionMessageAdapter.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Task {

				CSatisfiableTaskAnswererSubsumptionMessageAdapter::CSatisfiableTaskAnswererSubsumptionMessageAdapter(CConcept* testingConcept, bool testingNegation, CAnsweringHandler* answeringHandler, CConcreteOntology* testingOntology, CAnsweringMessageDataObserver* observer) {
					mTestingConcept = testingConcept;
					mOntology = testingOntology;
					mMessageObserver = observer;
					mAnsweringHandler = answeringHandler;
					mTestingConceptNegation = testingNegation;
				}


				CConcept* CSatisfiableTaskAnswererSubsumptionMessageAdapter::getTestingConcept() {
					return mTestingConcept;
				}


				bool CSatisfiableTaskAnswererSubsumptionMessageAdapter::getTestingConceptNegation() {
					return mTestingConceptNegation;
				}

				CConcreteOntology* CSatisfiableTaskAnswererSubsumptionMessageAdapter::getTestingOntology() {
					return mOntology;
				}

				CAnsweringHandler* CSatisfiableTaskAnswererSubsumptionMessageAdapter::getAnsweringHandler() {
					return mAnsweringHandler;
				}

				CAnsweringMessageDataObserver *CSatisfiableTaskAnswererSubsumptionMessageAdapter::getAnswererMessageDataObserver() {
					return mMessageObserver;
				}


			}; // end namespace Task

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude