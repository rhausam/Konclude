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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPEXMLVALUESPACEDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPEXMLVALUESPACEDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CDatatypeXMLValueSpaceMap.h"
#include "CDatatypeCompareValueSpaceData.h"


// Other includes
#include "Utilities/Container/CLinker.h"

#include "Reasoner/Ontology/CDatatypeValueSpaceXMLType.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CDatatypeXMLValueSpaceData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CDatatypeXMLValueSpaceData : public CDatatypeCompareValueSpaceData {
					// public methods
					public:
						//! Constructor
						CDatatypeXMLValueSpaceData(CProcessContext* processContext);


						CDatatypeXMLValueSpaceData* copyXMLValueSpaceData(CDatatypeXMLValueSpaceData* spaceData);
						CDatatypeXMLValueSpaceData* initXMLValueSpaceData(CDatatypeValueSpaceXMLType* valueSpaceType);

						CDatatypeXMLValueSpaceMap* getXMLValueSpaceMap(bool create);

					// protected methods
					protected:
						virtual CDatatypeCompareValueSpaceMap* createValueSpaceMap();

					// protected variables
					protected:
						CDatatypeXMLValueSpaceMap* mXMLValueSpaceMap;
						CDatatypeValueSpaceXMLType* mValueSpaceType;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPEXMLVALUESPACEDATA_H
