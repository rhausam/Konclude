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

#include "CNegationTransformationPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CNegationTransformationPreProcess::CNegationTransformationPreProcess() {
				mLastConceptId = 0;
				mLastRoleId = 0;
				mLastIndiId = 0;
			}


			CNegationTransformationPreProcess::~CNegationTransformationPreProcess() {
			}


			CConcreteOntology *CNegationTransformationPreProcess::preprocess(CConcreteOntology *ontology, CPreProcessContext* context) {

				mOntology = ontology;
				mLastConceptId = 0;
				mLastRoleId = 0;
				mLastIndiId = 0;

			

				return continuePreprocessing();
			}
			

			CConcreteOntology* CNegationTransformationPreProcess::continuePreprocessing() {
				if (mOntology) {
					CTBox* tbox = mOntology->getDataBoxes()->getTBox();
					CABox *abox = mOntology->getDataBoxes()->getABox();
					CRBox *rbox = mOntology->getDataBoxes()->getRBox();

					CConceptVector *concepts = tbox->getConceptVector();
					qint64 itemConCounts = concepts->getItemCount();
					for (qint64 i = mLastConceptId; i < itemConCounts; ++i) {
						CConcept *concept = concepts->getLocalData(i);

						if (concept && concept->getOperatorCode() != CCNOT) {

							CSortedNegLinker<CConcept *> *operands = concept->getOperandList();
							while (operands) {
								while (operands->getData()->getDefinitionOperatorTag() == CCNOT) {
									CConcept *compConcept = operands->getData()->getOperandList()->getData();
									operands->setData(compConcept);
									operands->setNegated(!operands->isNegated());
								}
								operands = operands->getNext();
							}
						}
					}
					mLastConceptId = itemConCounts;


					CRoleVector *roles = rbox->getRoleVector();
					qint64 itemRolCounts = roles->getItemCount();
					for (qint64 i = mLastRoleId; i < itemRolCounts; ++i) {
						CRole *role = roles->getLocalData(i);

						if (role) {

							CSortedNegLinker<CConcept *> *operandsDomIt = role->getDomainConceptList();
							while (operandsDomIt) {
								while (operandsDomIt->getData()->getDefinitionOperatorTag() == CCNOT) {
									CConcept *compConcept = operandsDomIt->getData()->getOperandList()->getData();
									operandsDomIt->setData(compConcept);
									operandsDomIt->setNegated(!operandsDomIt->isNegated());
								}
								operandsDomIt = operandsDomIt->getNext();
							}

							CSortedNegLinker<CConcept *> *operandsRanIt = role->getRangeConceptList();
							while (operandsRanIt) {
								while (operandsRanIt->getData()->getDefinitionOperatorTag() == CCNOT) {
									CConcept *compConcept = operandsRanIt->getData()->getOperandList()->getData();
									operandsRanIt->setData(compConcept);
									operandsRanIt->setNegated(!operandsRanIt->isNegated());
								}
								operandsRanIt = operandsRanIt->getNext();
							}
						}
					}
					mLastRoleId = itemRolCounts;

					CIndividualVector *indis = abox->getIndividualVector();
					qint64 itemIndiCounts = indis->getItemCount();
					for (qint64 i = mLastIndiId; i < itemIndiCounts; ++i) {
						CIndividual *indi = indis->getLocalData(i);

						if (indi) {
							for (CConceptAssertionLinker* conAssLinkerIt = indi->getAssertionConceptLinker(); conAssLinkerIt; conAssLinkerIt = conAssLinkerIt->getNext()) {
								CConcept *compConcept = conAssLinkerIt->getData();
								if (compConcept->getDefinitionOperatorTag() == CCNOT) {
									conAssLinkerIt->setData(compConcept->getOperandList()->getData());
									conAssLinkerIt->setNegation(!conAssLinkerIt->getNegation());
								}
							}
						}
					}
					mLastIndiId = itemIndiCounts;
				}
				return mOntology;
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
