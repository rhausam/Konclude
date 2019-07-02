/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALSATURATIONSUCCESSORLINKDATALINKER_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALSATURATIONSUCCESSORLINKDATALINKER_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CSaturationSuccessorData.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"

#include "Utilities/Container/CLinker.h"



namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CIndividualSaturationSuccessorLinkDataLinker
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIndividualSaturationSuccessorLinkDataLinker : public CLinkerBase<CSaturationSuccessorData*,CIndividualSaturationSuccessorLinkDataLinker> {
					// public methods
					public:
						//! Constructor
						CIndividualSaturationSuccessorLinkDataLinker();

						CIndividualSaturationSuccessorLinkDataLinker* initSuccessorLinkDataLinker(CSaturationSuccessorData* succLinkData);


					// protected methods
					protected:

					// protected variables
					protected:

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALSATURATIONSUCCESSORLINKDATALINKER_H