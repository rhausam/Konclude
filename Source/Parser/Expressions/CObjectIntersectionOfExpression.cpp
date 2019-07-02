/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CObjectIntersectionOfExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CObjectIntersectionOfExpression::CObjectIntersectionOfExpression(CClassTermExpression *expression1, CClassTermExpression *expression2) 
					: CListClassTermExpressionAssociator(expression1,expression2) {
			}

			CObjectIntersectionOfExpression::CObjectIntersectionOfExpression(const CEXPRESSIONLIST<CClassTermExpression*>& expressionList) 
					: CListClassTermExpressionAssociator(expressionList) {
			}

			CObjectIntersectionOfExpression::~CObjectIntersectionOfExpression() {
			}



			CBuildExpression::ExpressionType CObjectIntersectionOfExpression::getType() {
				return BETOBJECTINTERSECTION;
			}


			cint64 CObjectIntersectionOfExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CListClassTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CObjectIntersectionOfExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CObjectIntersectionOfExpression* compExpCast = dynamic_cast<CObjectIntersectionOfExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CListClassTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}



			bool CObjectIntersectionOfExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CListClassTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
