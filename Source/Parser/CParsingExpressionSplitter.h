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

#ifndef KONCLUDE_REASONER_GENERATOR_CPARSINGEXPRESSIONSPLITTER_H
#define KONCLUDE_REASONER_GENERATOR_CPARSINGEXPRESSIONSPLITTER_H


// Libraries includes
#include <QListIterator>
#include <QHash>
#include <QString>
#include <QObject>

// Namespace includes


// Other includes
#include "Parser/Expressions/CClassTermExpression.h"
#include "Parser/Expressions/CObjectPropertyTermExpression.h"
#include "Parser/Expressions/CIndividualTermExpression.h"
#include "Parser/Expressions/CObjectIndividualVariableExpression.h"
#include "Parser/Expressions/CDataPropertyTermExpression.h"
#include "Parser/Expressions/CDataRangeTermExpression.h"
#include "Parser/Expressions/CDataLiteralTermExpression.h"

// Logger includes
#include "Logger/CLogger.h"





namespace Konclude {

	using namespace Utilities;
	using namespace Parser::Expression;

	namespace Parser {

		/*! 
		 *
		 *		\class		CParsingExpressionSplitter
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CParsingExpressionSplitter  {
			// public methods
			public:
				//! Constructor
				CParsingExpressionSplitter(const CEXPRESSIONLIST<CBuildExpression*>& expressions);
				CParsingExpressionSplitter(CEXPRESSIONLIST<CBuildExpression*>* expressions);
				CParsingExpressionSplitter(CBuildExpression* expression1 = nullptr, CBuildExpression* expression2 = nullptr, CBuildExpression* expression3 = nullptr);
				//! Destructor
				virtual ~CParsingExpressionSplitter();

				bool proofExpressionComposition(cint64 requiredClassExpressions, cint64 requiredObjectPropertyExpressions = 0, cint64 requiredIndividualExpressions = 0, cint64 requiredVariableExpressions = 0, cint64 requiredNotVariableExpressions = 0, cint64 dataRangeExpressions = 0, cint64 dataPropertyExpressions = 0);

				CEXPRESSIONLIST<CClassTermExpression*>* getClassTermExpressionList();
				CEXPRESSIONLIST<CObjectPropertyTermExpression*>* getObjectPropertyTermExpressionList();
				CEXPRESSIONLIST<CIndividualTermExpression*>* getIndividualTermExpressionList();
				CEXPRESSIONLIST<CDataPropertyTermExpression*>* getDataPropertyTermExpressionList();
				CEXPRESSIONLIST<CDataRangeTermExpression*>* getDataRangeTermExpressionList();
				CEXPRESSIONLIST<CDataLiteralTermExpression*>* getDataLiteralTermExpressionList();

				CEXPRESSIONLIST<CObjectIndividualVariableExpression*>* getClassVariableTermExpressionList();
				CEXPRESSIONLIST<CClassTermExpression*>* getClassNotVariableTermExpressionList();

				CClassTermExpression* getFirstClassTermExpression();
				CClassTermExpression* getFirstOptionalClassTermExpression();
				CClassTermExpression* getSecondClassTermExpression();

				CObjectPropertyTermExpression* getFirstObjectPropertyTermExpression();
				CObjectPropertyTermExpression* getSecondObjectPropertyTermExpression();

				CIndividualTermExpression* getFirstIndividualTermExpression();
				CIndividualTermExpression* getSecondIndividualTermExpression();

				CObjectIndividualVariableExpression* getFirstClassVariableTermExpression();
				CObjectIndividualVariableExpression* getSecondClassVariableTermExpression();


				CDataPropertyTermExpression* getFirstDataPropertyTermExpression();
				CDataPropertyTermExpression* getSecondDataPropertyTermExpression();


				CDataRangeTermExpression* getFirstDataRangeTermExpression();
				CDataRangeTermExpression* getSecondDataRangeTermExpression();

				CDataLiteralTermExpression* getFirstDataLiteralTermExpression();
				CDataLiteralTermExpression* getSecondDataLiteralTermExpression();

			// protected methods
			protected:
				bool splitExpressions();

			// protected variables
			protected:
				CEXPRESSIONLIST<CBuildExpression*> mExpressionList;
				CEXPRESSIONLIST<CClassTermExpression*> mClassList;
				CEXPRESSIONLIST<CObjectPropertyTermExpression*> mObjectPropertyList;
				CEXPRESSIONLIST<CIndividualTermExpression*> mIndiList;
				CEXPRESSIONLIST<CDataPropertyTermExpression*> mDataPropertyList;
				CEXPRESSIONLIST<CDataRangeTermExpression*> mDataRangeList;
				CEXPRESSIONLIST<CDataLiteralTermExpression*> mDataLitList;

				CEXPRESSIONLIST<CObjectIndividualVariableExpression*> mObjIndiVarList;
				CEXPRESSIONLIST<CClassTermExpression*> mNotObjIndiVarList;
			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // end KONCLUDE_REASONER_GENERATOR_CPARSINGEXPRESSIONSPLITTER_H
