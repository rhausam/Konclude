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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CDatatypeValueSpaceStringTriggers_H
#define KONCLUDE_REASONER_ONTOLOGY_CDatatypeValueSpaceStringTriggers_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "CDatatypeValueSpaceCompareTriggers.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			/*! 
			 *
			 *		\class		CDatatypeValueSpaceStringTriggers
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDatatypeValueSpaceStringTriggers : public CDatatypeValueSpaceCompareTriggers {
				// public methods
				public:
					//! Constructor
					CDatatypeValueSpaceStringTriggers(CContext* boxContext);

					CDatatypeValueSpaceStringTriggers* initValueSpaceStringTriggers(CDatatypeValueSpaceStringTriggers* data);


					CDatatypeValueSpaceConceptTriggeringData* getStringConceptTriggeringData();

					CDatatypeValueSpaceConceptTriggeringData* getNonLanguageTagConceptTriggeringData();

					CDatatypeValueSpaceStringTriggers* addStringConceptTrigger(CDatatypeValueSpaceConceptTriggerLinker* conceptTrigger);
					CDatatypeValueSpaceStringTriggers* addNonLanguageTagConceptTrigger(CDatatypeValueSpaceConceptTriggerLinker* conceptTrigger);

				// protected methods
				protected:

				// protected variables
				protected:
					CDatatypeValueSpaceConceptTriggeringData mValueSpateNonLanguageTagTriggerData;


				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CDatatypeValueSpaceStringTriggers_H
