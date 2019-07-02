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

#include "CClassSetResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CClassSetResult::CClassSetResult() {
			}


			QString CClassSetResult::getQueryResultString() {
				return QString();
			}

			bool CClassSetResult::isResultEquivalentTo(CQueryResult *otherQueryResult) {
				CClassSetResult* classesResult = dynamic_cast<CClassSetResult*>(otherQueryResult);
				if (classesResult) {
					if (mClassSet.count() == classesResult->mClassSet.count()) {
						for (QSet<QString>::const_iterator it = mClassSet.constBegin(), itEnd = mClassSet.constEnd(); it != itEnd; ++it) {
							const QString& classString(*it);
							if (!classesResult->mClassSet.contains(classString)) {
								return false;
							}
						}
						return true;
					}
				}
				return false;
			}

			CClassSetResult* CClassSetResult::addClass(const QString &className) {
				mClassSet.insert(className);
				return this;
			}

			QSet<QString>* CClassSetResult::getClassesSet() {
				return &mClassSet;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude