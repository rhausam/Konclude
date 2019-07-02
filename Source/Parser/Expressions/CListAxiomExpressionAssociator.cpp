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

#include "CListAxiomExpressionAssociator.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CListAxiomExpressionAssociator::CListAxiomExpressionAssociator(CAxiomExpression *expression1, CAxiomExpression *expression2, CAxiomExpression *expression3) 
					: CListExpressionAssociator(expression1,expression2,expression3) {
			}


			CListAxiomExpressionAssociator::CListAxiomExpressionAssociator(const CEXPRESSIONLIST<CAxiomExpression*>& expressionList) : CListExpressionAssociator(*(CEXPRESSIONLIST<CBuildExpression*>*)&expressionList) {
			}


			CListAxiomExpressionAssociator::~CListAxiomExpressionAssociator() {
			}

			CEXPRESSIONLIST<CAxiomExpression*>* CListAxiomExpressionAssociator::getAxiomExpressionList() {
				// bad cast to avoid lot of reimplementation 
				return (CEXPRESSIONLIST<CAxiomExpression*>*)&mExpressionList;
			}

			bool CListAxiomExpressionAssociator::compareStructuralEquivalence(const CListAxiomExpressionAssociator& listClassTermExpressionAssociator) {
				return CListExpressionAssociator::compareStructuralEquivalence(listClassTermExpressionAssociator);
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude