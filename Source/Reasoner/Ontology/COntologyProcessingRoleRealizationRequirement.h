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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYPROCESSINGROLEREALIZATIONREQUIREMENT_H
#define KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYPROCESSINGROLEREALIZATIONREQUIREMENT_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "COntologyProcessingDynamicRealizationRequirement.h"
#include "CConcreteOntology.h"
#include "COntologyProcessingStatus.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		COntologyProcessingRoleRealizationRequirement
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyProcessingRoleRealizationRequirement : public COntologyProcessingDynamicRealizationRequirement {
				// public methods
				public:
					//! Constructor
					COntologyProcessingRoleRealizationRequirement(CRole* role, bool inversed = false, const CIndividualReference& indiSourceRef = CIndividualReference(), const CIndividualReference& indiDestinationRef = CIndividualReference(), cint64 requiredProcessingFlags = COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, cint64 forbiddenProcessingFlags = 0, cint64 requiredErrorFlags = COntologyProcessingStatus::PSSUCESSFULL, cint64 forbiddenErrorFlags = 0);


					CRole* getRole();
					bool isRoleInversed();
					CIndividualReference getIndividualSourceReference();
					CIndividualReference getIndividualDestinationReference();

					const QString getRequirementFailureString(COntology* ontology);

				// protected methods
				protected:

				// protected variables
				protected:
					CRole* mRole;
					bool mInversed;
					CIndividualReference mIndiSourceRef;
					CIndividualReference mIndiDestinationRef;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYPROCESSINGROLEREALIZATIONREQUIREMENT_H
