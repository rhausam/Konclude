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

#include "CClassExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CClassExpression::CClassExpression(const QString& className) 
					: CExpressionEntity(className) {
			}


			CClassExpression::~CClassExpression() {
			}


			CBuildExpression::ExpressionType CClassExpression::getType() {
				return BETCLASS;
			}


			cint64 CClassExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType());
				return hashValue;
			}

			bool CClassExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CClassExpression* compExpCast = dynamic_cast<CClassExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return true;
			}

			bool CClassExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					subExpressionVisitor->visitEntity(this,this,subExpressionVisitor);
					return true;
				}
				return false;		
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude