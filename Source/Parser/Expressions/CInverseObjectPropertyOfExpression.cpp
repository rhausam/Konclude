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

#include "CInverseObjectPropertyOfExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {

			CInverseObjectPropertyOfExpression::CInverseObjectPropertyOfExpression(CObjectPropertyTermExpression *expression)
					: CObjectPropertyTermExpressionAssociator(expression) {
			}


			CInverseObjectPropertyOfExpression::~CInverseObjectPropertyOfExpression() {
			}


			CObjectPropertyTermExpression *CInverseObjectPropertyOfExpression::getInverseOfExpression() {
				return getObjectPropertyTermExpression();
			}


			CBuildExpression::ExpressionType CInverseObjectPropertyOfExpression::getType() {
				return BETINVERSEOBJECTPROPERTYOF;
			}

			cint64 CInverseObjectPropertyOfExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CObjectPropertyTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CInverseObjectPropertyOfExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CInverseObjectPropertyOfExpression* compExpCast = dynamic_cast<CInverseObjectPropertyOfExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CObjectPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CInverseObjectPropertyOfExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CObjectPropertyTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
