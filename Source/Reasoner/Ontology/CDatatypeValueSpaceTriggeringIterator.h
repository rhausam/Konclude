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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CDatatypeValueSpaceTriggeringIterator_H
#define KONCLUDE_REASONER_ONTOLOGY_CDatatypeValueSpaceTriggeringIterator_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "CDatatypeValueSpaceTriggeringMapArranger.h"
#include "CDatatypeValueSpaceTriggeringMapData.h"


// Other includes
#include "Utilities/Container/CLinker.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

				/*! 
				 *
				 *		\class		CDatatypeValueSpaceTriggeringIterator
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CDatatypeValueSpaceTriggeringIterator {
					// public methods
					public:
						//! Constructor
						CDatatypeValueSpaceTriggeringIterator(const CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::const_iterator beginIt, const CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::const_iterator endIt, bool upIteration = true);
						CDatatypeValueSpaceTriggeringIterator(const CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::const_iterator beginIt, const CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::const_iterator lastIt, const CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::const_iterator endIt, bool upIteration = true);

						bool hasNext();

						CDatatypeValueSpaceTriggeringData* next(bool moveNext = true);

					// protected methods
					protected:


					// protected variables
					protected:
						CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::const_iterator mBegIt;
						CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::const_iterator mCurIt;
						CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::const_iterator mEndIt;
						CBOXMAP<CDatatypeValueSpaceTriggeringMapArranger,CDatatypeValueSpaceTriggeringMapData>::const_iterator mLastIt;
						bool mUpIteration;



					// private methods
					private:

					// private variables
					private:

				};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CDatatypeValueSpaceTriggeringIterator_H
