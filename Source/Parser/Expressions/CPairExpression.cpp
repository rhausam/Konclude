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

#include "CPairExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CPairExpression::CPairExpression(CBuildExpression *expression1, CBuildExpression *expression2) 
					: CListExpression() {
				push_front(expression1);
				push_back(expression2);
			}


			CPairExpression::~CPairExpression() {
			}


			void CPairExpression::setFirstExpression(CBuildExpression *expression1) {
				if (count() > 0) {
					removeFirst();
				}
				push_front(expression1);
			}

			CBuildExpression *CPairExpression::getFirstExpression() {
				if (count() <= 0) {
					return 0;
				} else {
					return first();
				}
			}

			void CPairExpression::setSecondExpression(CBuildExpression *expression2) {
				if (count() > 0) {
					removeLast();
				}
				push_back(expression2);
			}

			CBuildExpression *CPairExpression::getSecondExpression() {
				if (count() <= 0) {
					return 0;
				} else {
					return last();
				}
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
