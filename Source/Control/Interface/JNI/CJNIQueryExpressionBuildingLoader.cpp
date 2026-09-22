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

#include "CJNIQueryExpressionBuildingLoader.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace JNI {


				CJNIQueryExpressionBuildingLoader::CJNIQueryExpressionBuildingLoader(CJNIInstanceManager* jniInstanceManager, CJNIOntologyRevisionData* ontRevData, CConcreteOntology* expressionOntology)
						: CJNIAxiomExpressionVisitingLoader(jniInstanceManager,ontRevData,expressionOntology) {
					mExpressionOntology = expressionOntology;
				}

				CJNIQueryExpressionBuildingLoader::~CJNIQueryExpressionBuildingLoader() {
					// the expressions that were built here and found nowhere else, the ones of the
					// installed ontology are shared with it and stay
					qDeleteAll(mOwnedExpressionList);
				}


				CConcreteOntology* CJNIQueryExpressionBuildingLoader::getExpressionOntology() {
					return mExpressionOntology;
				}


				CConcreteOntologyBuildDataUpdater* CJNIQueryExpressionBuildingLoader::addHashedExpression(CBuildExpression* expression) {
					mSeparateExpressionBuildHash.insert(CExpressionHasher(expression),expression);
					mOwnedExpressionList.append(expression);
					return this;
				}


				CBuildExpression* CJNIQueryExpressionBuildingLoader::getHashedExpression(CBuildExpression* expression) {
					CExpressionHasher expHasher(expression);
					CBuildExpression* hashExpression = mExpressionBuildHash->value(expHasher,nullptr);
					if (!hashExpression) {
						hashExpression = mSeparateExpressionBuildHash.value(expHasher,nullptr);
					}
					return hashExpression;
				}


			}; // end namespace JNI

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
