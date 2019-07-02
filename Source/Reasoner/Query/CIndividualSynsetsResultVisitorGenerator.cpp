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

#include "CIndividualSynsetsResultVisitorGenerator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CIndividualSynsetsResultVisitorGenerator::CIndividualSynsetsResultVisitorGenerator(CIndividualSynsetsResult* classSynsetsResult, bool abbreviatedIRIs) {
				mIndividualSynsetsResult = classSynsetsResult;
				mAbbreviatedIRIs = abbreviatedIRIs;
			}


			bool CIndividualSynsetsResultVisitorGenerator::visitInstance(CConceptInstanceItem* item, CConceptRealization* conRealization) {
				mTmpIndividualSynsetResult = new CIndividualSynsetResult();
				conRealization->visitIndividuals(item,this);
				mIndividualSynsetsResult->addIndividualSynset(mTmpIndividualSynsetResult);
				return true;
			}

			bool CIndividualSynsetsResultVisitorGenerator::visitIndividual(CIndividual* individual, CConceptRealization* conRealization) {
				QString individualString;
				if (individualString.isEmpty()) {
					individualString = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(individual->getIndividualNameLinker());
				} 
				if (individualString.isEmpty()) {
					individualString = CIRIName::getRecentIRIName(individual->getIndividualNameLinker());
				}
				if (!individualString.isEmpty()) {
					mTmpIndividualSynsetResult->addEquivalentIndividualName(individualString);
				}
				return true;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
