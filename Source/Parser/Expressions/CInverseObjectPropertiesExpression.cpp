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

#include "CInverseObjectPropertiesExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CInverseObjectPropertiesExpression::CInverseObjectPropertiesExpression(CObjectPropertyTermExpression *expression1, CObjectPropertyTermExpression *expression2) 
					: CUnorderedPairObjectPropertyTermExpressionAssociator(expression1,expression2) {
			}


			CInverseObjectPropertiesExpression::~CInverseObjectPropertiesExpression() {
			}



			CBuildExpression::ExpressionType CInverseObjectPropertiesExpression::getType() {
				return BETINVERSEOBJECTPROPERTIES;
			}


			cint64 CInverseObjectPropertiesExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CUnorderedPairObjectPropertyTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CInverseObjectPropertiesExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CInverseObjectPropertiesExpression* compExpCast = dynamic_cast<CInverseObjectPropertiesExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CUnorderedPairObjectPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CInverseObjectPropertiesExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CUnorderedPairObjectPropertyTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude