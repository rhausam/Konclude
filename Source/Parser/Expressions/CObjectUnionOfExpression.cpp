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

#include "CObjectUnionOfExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CObjectUnionOfExpression::CObjectUnionOfExpression(CClassTermExpression *expression1, CClassTermExpression *expression2) 
					: CListClassTermExpressionAssociator(expression1,expression2) {
			}

			CObjectUnionOfExpression::CObjectUnionOfExpression(const CEXPRESSIONLIST<CClassTermExpression*>& expressionList) 
					: CListClassTermExpressionAssociator(expressionList) {
			}


			CObjectUnionOfExpression::~CObjectUnionOfExpression() {
			}


			CBuildExpression::ExpressionType CObjectUnionOfExpression::getType() {
				return BETOBJECTUNION;
			}

			cint64 CObjectUnionOfExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CListClassTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CObjectUnionOfExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CObjectUnionOfExpression* compExpCast = dynamic_cast<CObjectUnionOfExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CListClassTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CObjectUnionOfExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CListClassTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
