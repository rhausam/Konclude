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

#ifndef KONCLUDE_REASONER_ANSWERER_CANSWERINGPROPAGATIONSTEERINGABSORPTIONEXTENSIONITEMENTAILMENTCONTROLLER_H
#define KONCLUDE_REASONER_ANSWERER_CANSWERINGPROPAGATIONSTEERINGABSORPTIONEXTENSIONITEMENTAILMENTCONTROLLER_H

// Libraries includes
#include <QLinkedList>

// Namespace includes
#include "AnswererSettings.h"
#include "CAnsweringMessageData.h"
#include "CAnsweringHandler.h"
#include "CAnsweringPropagationSteeringController.h"
#include "COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem.h"
#include "COptimizedComplexBuildingIndividualVariableCompositionsItem.h"


// Other includes
#include "Reasoner/Query/CComplexAnsweringQuery.h"

#include "Utilities/Memory/CMemoryPool.h"


// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"





namespace Konclude {

	using namespace Utilities::Memory;

	namespace Reasoner {

		using namespace Query;

		namespace Answerer {



			/*! 
			 *
			 *		\class		CAnsweringPropagationSteeringAbsorptionExtensionItemEntailmentController
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CAnsweringPropagationSteeringAbsorptionExtensionItemEntailmentController : public CAnsweringPropagationSteeringController {
				// public methods
				public:
					//! Constructor
					CAnsweringPropagationSteeringAbsorptionExtensionItemEntailmentController(COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationItem);

					//! Destructor
					virtual ~CAnsweringPropagationSteeringAbsorptionExtensionItemEntailmentController();


					virtual bool finalizeWithClashing();
					virtual bool finalizeWithBindingExtraction();

					virtual bool isPreparationBindingNominalIndividual(CVariable* variable, CIndividual* indi);
					virtual bool isPreparationBindingNominalIndividual(CVariable* variable, cint64 indiId);
					virtual bool isPreparationBindingAllIndividuals(CVariable* variable);


				// protected methods
				protected:

				// protected variables
				protected:
					COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* mAbsorptionPropagationItem;
					COptimizedComplexBuildingIndividualVariableCompositionsItem* mVarBuildingItem;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CANSWERINGPROPAGATIONSTEERINGABSORPTIONEXTENSIONITEMENTAILMENTCONTROLLER_H
