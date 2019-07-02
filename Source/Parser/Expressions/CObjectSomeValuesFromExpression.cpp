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

#include "CObjectSomeValuesFromExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CObjectSomeValuesFromExpression::CObjectSomeValuesFromExpression(CObjectPropertyTermExpression *expression1, CClassTermExpression *expression2) 
				: CObjectPropertyTermExpressionAssociator(expression1), CClassTermExpressionAssociator(expression2) {
			}


			CObjectSomeValuesFromExpression::~CObjectSomeValuesFromExpression() {
			}



			CBuildExpression::ExpressionType CObjectSomeValuesFromExpression::getType() {
				return BETOBJECTSOMEVALUEFROM;
			}

			cint64 CObjectSomeValuesFromExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CObjectPropertyTermExpressionAssociator::getStructuralHashValue(),CClassTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CObjectSomeValuesFromExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CObjectSomeValuesFromExpression* compExpCast = dynamic_cast<CObjectSomeValuesFromExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CObjectPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast) && CClassTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CObjectSomeValuesFromExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CObjectPropertyTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					CClassTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude