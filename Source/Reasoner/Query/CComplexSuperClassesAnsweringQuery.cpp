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

#include "CComplexSuperClassesAnsweringQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {

			CComplexSuperClassesAnsweringQuery::CComplexSuperClassesAnsweringQuery(CConcreteOntology* baseOntology, CConcreteOntology* expressionOntology, CBuildExpression* conceptExpression, CConfigurationBase* configuration, const QString& queryName) : CComplexConceptAnsweringQuery(baseOntology,expressionOntology,configuration,queryName) {
				mSuperClassNodesRequired = true;
				mExpressionOfInterest = conceptExpression;
			}

			CComplexSuperClassesAnsweringQuery::~CComplexSuperClassesAnsweringQuery() {
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
