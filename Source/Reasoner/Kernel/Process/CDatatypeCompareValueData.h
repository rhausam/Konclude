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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPECOMPAREVALUEDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPECOMPAREVALUEDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CDatatypeCompareValueDataExclusion.h"


// Other includes
#include "Reasoner/Kernel/Process/Dependency/CDependencyTrackPoint.h"

#include "Utilities/Container/CLinker.h"

#include "Reasoner/Ontology/CDataLiteralCompareValue.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				using namespace Dependency;

				/*! 
				 *
				 *		\class		CDatatypeCompareValueData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CDatatypeCompareValueData {
					// public methods
					public:
						//! Constructor
						CDatatypeCompareValueData(CProcessContext* processContext);

						CDatatypeCompareValueData* initCompareValueData(CDatatypeCompareValueData* valueData, CDataLiteralCompareValue* value);
						CDatatypeCompareValueData* initCompareValueData(CDataLiteralCompareValue* value);


						CDatatypeCompareValueDataExclusion* getValueExclusionData();
						CDatatypeCompareValueDataExclusion* getLeftExclusionData();
						CDatatypeCompareValueDataExclusion* getRightExclusionData();

						CDataLiteralCompareValue* getValue();

						CDatatypeCompareValueData* setValue(CDataLiteralCompareValue* value);

					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mProcessContext;

						CDataLiteralCompareValue* mValue;

						CDatatypeCompareValueDataExclusion mValueExclusionData;
						CDatatypeCompareValueDataExclusion mLeftExclusionData;
						CDatatypeCompareValueDataExclusion mRightExclusionData;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CDATATYPECOMPAREVALUEDATA_H
