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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACETRIGGERINGDATA_H
#define KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACETRIGGERINGDATA_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "CDataLiteralCompareValue.h"
#include "CDatatypeValueSpaceConceptTriggeringData.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			/*! 
			 *
			 *		\class		CDatatypeValueSpaceTriggeringData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDatatypeValueSpaceTriggeringData {
				// public methods
				public:
					//! Constructor
					CDatatypeValueSpaceTriggeringData();

					CDatatypeValueSpaceTriggeringData* initValueTriggeringData(CDatatypeValueSpaceTriggeringData* valueData, CDataLiteralCompareValue* value);


					CDataLiteralCompareValue* getValue();
					CDatatypeValueSpaceTriggeringData* setValue(CDataLiteralCompareValue* value);

					CDatatypeValueSpaceConceptTriggeringData* getTriggeringData();
					CDatatypeValueSpaceConceptTriggeringData* getTriggeringData(bool min, bool inclusive);

					CDatatypeValueSpaceConceptTriggeringData* getDirectValueTriggeringData();

					CDatatypeValueSpaceConceptTriggeringData* getMinInclusiveTriggeringData();
					CDatatypeValueSpaceConceptTriggeringData* getMinExclusiveTriggeringData();

					CDatatypeValueSpaceConceptTriggeringData* getMaxInclusiveTriggeringData();
					CDatatypeValueSpaceConceptTriggeringData* getMaxExclusiveTriggeringData();

				// protected methods
				protected:

				// protected variables
				protected:
					CDataLiteralCompareValue* mValue;

					CDatatypeValueSpaceConceptTriggeringData mValueTriggerData;

					CDatatypeValueSpaceConceptTriggeringData mMinInclusiveTriggerData;
					CDatatypeValueSpaceConceptTriggeringData mMinExclusiveTriggerData;

					CDatatypeValueSpaceConceptTriggeringData mMaxInclusiveTriggerData;
					CDatatypeValueSpaceConceptTriggeringData mMaxExclusiveTriggerData;

				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CDATATYPEVALUESPACETRIGGERINGDATA_H
