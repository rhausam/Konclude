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

#include "CSaturationCalculationConstruct.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CSaturationCalculationConstruct::CSaturationCalculationConstruct() : CLinkerBase<CSaturationCalculationConstruct*,CSaturationCalculationConstruct>(this) {
				mIndividual = nullptr;
				mConstructConcept = nullptr;
				mIndiID = -1;
				mConstructConceptNegation = false;
			}

			CSaturationCalculationConstruct* CSaturationCalculationConstruct::getNextConstruct() {
				return getNext();
			}

			CConcept* CSaturationCalculationConstruct::getConstructConcept() {
				return mConstructConcept;
			}

			CSaturationCalculationConstruct* CSaturationCalculationConstruct::setConstructConcept(CConcept* concept) {
				mConstructConcept = concept;
				return this;
			}

			bool CSaturationCalculationConstruct::getConstructConceptNegation() {
				return mConstructConceptNegation;
			}

			CSaturationCalculationConstruct* CSaturationCalculationConstruct::setConstructConceptNegation(bool negation) {
				mConstructConceptNegation = negation;
				return this;
			}


			CSaturationCalculationConstruct* CSaturationCalculationConstruct::setIndividual(CIndividual* individual) {
				mIndividual = individual;
				return this;
			}

			CIndividual* CSaturationCalculationConstruct::getIndividual() {
				return mIndividual;
			}

			CSaturationCalculationConstruct* CSaturationCalculationConstruct::setIndividualID(cint64 individualID) {
				mIndiID = individualID;
				return this;
			}

			cint64 CSaturationCalculationConstruct::getIndividualID() {
				return mIndiID;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
