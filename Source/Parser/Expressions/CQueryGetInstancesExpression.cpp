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

#include "CQueryGetInstancesExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CQueryGetInstancesExpression::CQueryGetInstancesExpression(const QString& name, bool direct, CClassTermExpression* classTermExp) 
					: CNameAssociator(name),CClassTermExpressionAssociator(classTermExp),mDirect(direct) {
			}


			CQueryGetInstancesExpression::~CQueryGetInstancesExpression() {
			}


			CBuildExpression::ExpressionType CQueryGetInstancesExpression::getType() {
				return BETGETINDIVIDUALS;
			}


			cint64 CQueryGetInstancesExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CClassTermExpressionAssociator::getStructuralHashValue(),(cint64)mDirect);
				return hashValue;
			}

			bool CQueryGetInstancesExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CQueryGetInstancesExpression* compExpCast = dynamic_cast<CQueryGetInstancesExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				if (compExpCast->mDirect != mDirect) {
					return false;
				}
				return CClassTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}

			bool CQueryGetInstancesExpression::isDirect() {
				return mDirect;
			}

			bool CQueryGetInstancesExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CClassTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
