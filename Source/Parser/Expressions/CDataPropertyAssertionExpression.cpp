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

#include "CDataPropertyAssertionExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CDataPropertyAssertionExpression::CDataPropertyAssertionExpression(CDataPropertyTermExpression *propertyExpression1, CIndividualTermExpression *sourceExpression2, CDataLiteralExpression *targetExpression3) 
					: CDataPropertyTermExpressionAssociator(propertyExpression1), CIndividualTermExpressionAssociator(sourceExpression2), CDataLiteralExpressionAssociator(targetExpression3) {
			}


			CDataPropertyAssertionExpression::~CDataPropertyAssertionExpression() {
			}


			CBuildExpression::ExpressionType CDataPropertyAssertionExpression::getType() {
				return BETDATAPROPERTYASSERTION;
			}

			cint64 CDataPropertyAssertionExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CDataPropertyTermExpressionAssociator::getStructuralHashValue(),CIndividualTermExpressionAssociator::getStructuralHashValue(),CDataLiteralExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CDataPropertyAssertionExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CDataPropertyAssertionExpression* compExpCast = dynamic_cast<CDataPropertyAssertionExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CDataPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast) && CIndividualTermExpressionAssociator::compareStructuralEquivalence(*compExpCast) && CDataLiteralExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CDataPropertyAssertionExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CDataPropertyTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					CIndividualTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					CDataLiteralExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude