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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CPROPAGATIONBINDING_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CPROPAGATIONBINDING_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CDependencyTracker.h"
#include "CConceptDescriptor.h"


// Other includes
#include "Reasoner/Ontology/CVariable.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CPropagationBinding
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CPropagationBinding : public CDependencyTracker {
					// public methods
					public:
						//! Constructor
						CPropagationBinding();

						CPropagationBinding* initPropagationBinding(cint64 propID, CDependencyTrackPoint* dependencyTrackPoint, CIndividualProcessNode* indi, CConceptDescriptor* conDes, CVariable* variable);

						cint64 getPropagationID();
						CPropagationBinding* setPropagationID(cint64 propID);

						CVariable* getBindedVariable();
						CPropagationBinding* setBindedVariable(CVariable* variable);

						CIndividualProcessNode* getBindedIndividual();
						CPropagationBinding* setBindedIndividual(CIndividualProcessNode* indi);

						CConceptDescriptor* getBindedConceptDescriptor();
						CPropagationBinding* setBindedConceptDescriptor(CConceptDescriptor* conDes);

					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mPropID;
						CVariable* mVariable;
						CIndividualProcessNode* mIndiNode;
						CConceptDescriptor* mConDes;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CPROPAGATIONBINDING_H
