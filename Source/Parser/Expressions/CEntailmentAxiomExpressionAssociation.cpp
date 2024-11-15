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

#include "CEntailmentAxiomExpressionAssociation.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CEntailmentAxiomExpressionAssociation::CEntailmentAxiomExpressionAssociation(CAxiomExpression* axiomExpression, CEntailmentAxiomExpressionAssociation::ENTAILMENTAXIOMEXPRESSIONCONSTRUCTIONTYPE constructionType) {
				mAxiomExpression = axiomExpression;
				mConstructionType = constructionType;
			}


			CClassTermExpression* CEntailmentAxiomExpressionAssociation::takeNextConstructedTestClass() {
				if (!mConstructedClassExpList.isEmpty()) {
					return mConstructedClassExpList.takeFirst();
				}
				return nullptr;
			}

			bool CEntailmentAxiomExpressionAssociation::hasNextConstructedTestClass() {
				return !mConstructedClassExpList.isEmpty();
			}


			CObjectPropertyTermExpression* CEntailmentAxiomExpressionAssociation::takeNextConstructedTestObjectProperty() {
				if (!mConstructedObjPropExpList.isEmpty()) {
					return mConstructedObjPropExpList.takeFirst();
				}
				return nullptr;
			}

			bool CEntailmentAxiomExpressionAssociation::hasNextConstructedTestObjectProperty() {
				return !mConstructedObjPropExpList.isEmpty();
			}


			CEXPRESSIONLIST<CClassTermExpression*>* CEntailmentAxiomExpressionAssociation::getConstructedTestClasses() {
				return &mConstructedClassExpList;
			}


			CEntailmentAxiomExpressionAssociation *CEntailmentAxiomExpressionAssociation::addConstructedTestClass(CClassTermExpression *conBuildExp) {
				mConstructedClassExpList.append(conBuildExp);
				return this;
			}

			CEXPRESSIONLIST<CObjectPropertyTermExpression*>* CEntailmentAxiomExpressionAssociation::getConstructedTestObjectProperties() {
				return &mConstructedObjPropExpList;
			}


			CEntailmentAxiomExpressionAssociation *CEntailmentAxiomExpressionAssociation::addConstructedTestObjectProperties(CObjectPropertyTermExpression *conBuildExp) {
				mConstructedObjPropExpList.append(conBuildExp);
				return this;
			}

			CAxiomExpression* CEntailmentAxiomExpressionAssociation::getAxiomExpression() {
				return mAxiomExpression;
			}


			CEntailmentAxiomExpressionAssociation::ENTAILMENTAXIOMEXPRESSIONCONSTRUCTIONTYPE CEntailmentAxiomExpressionAssociation::getConstructionType() {
				return mConstructionType;
			}

			CEntailmentAxiomExpressionAssociation* CEntailmentAxiomExpressionAssociation::setConstructionType(CEntailmentAxiomExpressionAssociation::ENTAILMENTAXIOMEXPRESSIONCONSTRUCTIONTYPE constructionType) {
				mConstructionType = constructionType;
				return this;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
