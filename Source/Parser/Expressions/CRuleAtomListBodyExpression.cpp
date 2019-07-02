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

#include "CRuleAtomListBodyExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CRuleAtomListBodyExpression::CRuleAtomListBodyExpression(const CEXPRESSIONLIST<CRuleAtomTermExpression*>& expressionList) : CRuleAtomListExpression(expressionList) {
			}


			CRuleAtomListBodyExpression::~CRuleAtomListBodyExpression() {
			}


			CBuildExpression::ExpressionType CRuleAtomListBodyExpression::getType() {
				return BETRULEATOMBODYLIST;
			}


			cint64 CRuleAtomListBodyExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CListRuleAtomTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CRuleAtomListBodyExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CRuleAtomListBodyExpression* compExpCast = dynamic_cast<CRuleAtomListBodyExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CListRuleAtomTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CRuleAtomListBodyExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CListRuleAtomTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
