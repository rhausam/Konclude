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

#ifndef KONCLUDE_REASONER_ANSWERER_CCOMPLEXCONCEPTSTEPINSTANCECOMPUTATIONPROCESS_H
#define KONCLUDE_REASONER_ANSWERER_CCOMPLEXCONCEPTSTEPINSTANCECOMPUTATIONPROCESS_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "CComplexConceptStepComputationProcess.h"
#include "CComputedItemDataNotificationLinker.h"



// Other includes
#include "Utilities/UtilitiesSettings.h"



// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Answerer {



			/*! 
			 *
			 *		\class		CComplexConceptStepInstanceComputationProcess
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CComplexConceptStepInstanceComputationProcess : public CComplexConceptStepComputationProcess {
				// public methods
				public:
					//! Constructor
					CComplexConceptStepInstanceComputationProcess();

					~CComplexConceptStepInstanceComputationProcess();

					bool requiresAdditionalInstanceComputation();

					bool requiresAllInstanceComputation();
					CComplexConceptStepInstanceComputationProcess* setAllInstanceComputationRequired(bool allInstancesRequired);


					bool hasAllInstancesComputed();
					CComplexConceptStepInstanceComputationProcess* setAllInstancesComputed(bool allInstancesComputed);


					double getExpectedInstancesCount();
					CComplexConceptStepInstanceComputationProcess* setExpectedInstancesCount(double expectedCount);

					cint64 getComputedInstancesCount();
					cint64 getRequiredInstancesCount();
					cint64 getAdditionalRequiredComputedInstancesCount();

					CComplexConceptStepInstanceComputationProcess* updateComputedInstancesCount(cint64 instancesCount, function<void(CComputedItemDataNotificationLinker* notificationLinker)> waitingHandlerFunction);
					CComplexConceptStepInstanceComputationProcess* updateAllInstancesComputed(function<void(CComputedItemDataNotificationLinker* notificationLinker)> waitingHandlerFunction);


					CComplexConceptStepInstanceComputationProcess* addComputedInstancesCountRequirement(cint64 count, CComputedItemDataNotificationLinker* notificationLinker);

					CComplexConceptStepInstanceComputationProcess* clearComputedInstancesCount();


				// protected methods
				protected:

				// protected variables
				protected:
					QMap<cint64, CRequirementWaitingDependencyData> mCountDataMap;

					bool mRequiredAllInstances;


					bool mAllInstancesComputed;

					double mExpectedInstancesCount;


					cint64 mComputedInstancesCount;
					cint64 mMaximalRequiredInstancesCount;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CCOMPLEXCONCEPTSTEPINSTANCECOMPUTATIONPROCESS_H
