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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEINDIVIDUALMAPPINGS_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEINDIVIDUALMAPPINGS_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexVariableIndividualBindings.h"
#include "COptimizedComplexVariableIndividualBindingsHasher.h"
#include "COptimizedComplexVariableIndividualBindingsCardinalityLinker.h"
#include "COptimizedComplexVariableIndividualUpdateCardinalityLinker.h"
#include "COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker.h"


// Other includes



// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {



			/*! 
			 *
			 *		\class		COptimizedComplexVariableIndividualMappings
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexVariableIndividualMappings  {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableIndividualMappings(cint64 bindingSize);
					virtual ~COptimizedComplexVariableIndividualMappings();


					enum VARIABLE_TYPE {
						INDIVIDUAL_VARIABLE, DATA_LITERAL_VARIABLE, CLASS_VARIABLE, PROPERTY_VARIABLE
					};



					VARIABLE_TYPE getBindingMapping(cint64 idx);
					COptimizedComplexVariableIndividualMappings* setBindingMapping(cint64 idx, VARIABLE_TYPE varType);

					cint64 getBindingSize();
					cint64 getBindingCount();
					cint64 getMaximumCardinalitySameIndividualsJointlyConsidered();
					cint64 getMaximumCardinalitySameIndividualsSeparatelyConsidered();

					COptimizedComplexVariableIndividualBindingsCardinalityLinker* getLastAddedBindingsCardinalityLinker();
					COptimizedComplexVariableIndividualMappings* addLastAddedBindingsCardinalityLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker);


					COptimizedComplexVariableIndividualMappings* addLastUpdatedCardinalityLinker(COptimizedComplexVariableIndividualUpdateCardinalityLinker* linker);
					COptimizedComplexVariableIndividualUpdateCardinalityLinker* getLastUpdatedCardinalityLinker();

					COptimizedComplexVariableIndividualMappings* setCurrentUpdateId(cint64 currUpdateId);
					cint64 getCurrentUpdateId();

					virtual COptimizedComplexVariableIndividualMappings* incCurrentUpdateId(cint64 incUpdateCount = 1);


					virtual COptimizedComplexVariableIndividualBindingsCardinalityLinker* takeBindingsCardinalityLinker() = 0;


					virtual bool addBindingsCardinalities(COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* addedCardinalites) = 0;
					virtual bool addInsertingBindingsCardinalityLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker, bool addCardinalitiesIfAlreadyPresent) = 0;


					COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* getLastAddedBindingsCardinalityBatchLinker();
					COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* getFirstAddedBindingsCardinalityBatchLinker();

					virtual COptimizedComplexVariableIndividualMappings* clearComputedMappings();


					cint64 getBindingsMemoryUsage();
					COptimizedComplexVariableIndividualMappings* setBindingsMemoryUsage(cint64 usage);
					COptimizedComplexVariableIndividualMappings* incBindingsMemoryUsage(cint64 usageInc);


					virtual CMemoryAllocationManager* getBindingsMemoryAllocationManager() = 0;
					virtual COptimizedComplexVariableIndividualMappings* updateBindingsMemoryConsumption() = 0;


					double getComputationCost();
					COptimizedComplexVariableIndividualMappings* setComputationCost(double cost);

				// protected methods
				protected:
					COptimizedComplexVariableIndividualMappings* extendAddedBindingsCardinalityBatchLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker);

				// protected variables
				protected:
					cint64 mBindingSize;
					VARIABLE_TYPE* mBindingMapping;
					cint64 mBindingCount;
					cint64 mMaximumCardinalitySameIndividualsJointlyConsidered;
					cint64 mMaximumCardinalitySameIndividualsSeparatelyConsidered;

					COptimizedComplexVariableIndividualBindingsCardinalityLinker* mLastAddedBindingsCardinalityLinker;

					COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* mLastAddedBindingsCardinalityBatchLinker;
					COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* mFirstAddedBindingsCardinalityBatchLinker;
					cint64 mLastAddedBindingsCardinalityBatchLinkerUpdateId;

					COptimizedComplexVariableIndividualUpdateCardinalityLinker* mLastUpdateCardinalityLinker;
					cint64 mCurrentUpdateId;


					cint64 mCurrentLinkerBatchingSize;
					double mLinkerBatchingSizeIncreasingFactor;


					cint64 mBindingsMemoryUsage;

					double mComputationCost;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEINDIVIDUALMAPPINGS_H
