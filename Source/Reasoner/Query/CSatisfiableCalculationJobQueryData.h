/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_QUERY_CSATISFIABLECALCULATIONJOBQUERYDATA_H
#define KONCLUDE_REASONER_QUERY_CSATISFIABLECALCULATIONJOBQUERYDATA_H

// Libraries includes


// Namespace includes
#include "QuerySettings.h"
#include "CCalculationJobQueryData.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CSatisfiableCalculationJobQueryData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CSatisfiableCalculationJobQueryData : public CCalculationJobQueryData {
				// public methods
				public:
					//! Constructor
					CSatisfiableCalculationJobQueryData(CQuery* query = nullptr, bool ustBeSatisfiableForPositive = true);

					//! Destructor
					virtual ~CSatisfiableCalculationJobQueryData();

					virtual CQuery* getQuery();

					bool needsJobSatisfiabilityForPositiveQueryAnswer();
					bool isJobCalculationSatisfiable();
					bool isJobCalculationError();
					bool isJobCalculated();
					CSatisfiableCalculationJobQueryData* setJobCalculated(bool jobCalculated);
					CSatisfiableCalculationJobQueryData* setJobCalculationError(bool jobCalculationError);
					CSatisfiableCalculationJobQueryData* setJobCalculationSatisfiable(bool jobCalculationSatisfiable);


				// protected methods
				protected:

				// protected variables
				protected:
					CQuery* mQuery;
					bool mMustBeSatisfiableForPositive;
					bool mJobCalculated;
					bool mJobCalculationError;
					bool mJobCalculationSatisfiable;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CSATISFIABLECALCULATIONJOBQUERYDATA_H