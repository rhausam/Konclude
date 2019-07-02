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

#include "CIndividualsResultVisitorGenerator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CIndividualsResultVisitorGenerator::CIndividualsResultVisitorGenerator(CIndividualsResult* individualsResult, bool abbreviatedIRIs) {
				mIndividualsResult = individualsResult;
				mAbbreviatedIRIs = abbreviatedIRIs;
			}

			bool CIndividualsResultVisitorGenerator::visitIndividual(CIndividual* individual, CConceptRealization* conRealization) {
				QString individualString;
				if (mAbbreviatedIRIs) {
					individualString = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(individual->getIndividualNameLinker());
				} 
				if (individualString.isEmpty()) {
					individualString = CIRIName::getRecentIRIName(individual->getIndividualNameLinker());
				}
				if (!individualString.isEmpty()) {
					mIndividualsResult->addIndividual(individualString);
				}
				return true;
			}


			bool CIndividualsResultVisitorGenerator::visitIndividual(CIndividual* individual, CRoleRealization* roleRealization) {
				QString individualString;
				if (mAbbreviatedIRIs) {
					individualString = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(individual->getIndividualNameLinker());
				} 
				if (individualString.isEmpty()) {
					individualString = CIRIName::getRecentIRIName(individual->getIndividualNameLinker());
				}
				if (!individualString.isEmpty()) {
					mIndividualsResult->addIndividual(individualString);
				}
				return true;
			}


			bool CIndividualsResultVisitorGenerator::visitIndividual(CIndividual* individual, CSameRealization* sameRealization) {
				QString individualString;
				if (mAbbreviatedIRIs) {
					individualString = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(individual->getIndividualNameLinker());
				} 
				if (individualString.isEmpty()) {
					individualString = CIRIName::getRecentIRIName(individual->getIndividualNameLinker());
				}
				if (!individualString.isEmpty()) {
					mIndividualsResult->addIndividual(individualString);
				}
				return true;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
