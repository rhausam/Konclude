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

#ifndef KONCLUDE_PARSER_EXPRESSION_CEXPRESSIONHASHMIXER_H
#define KONCLUDE_PARSER_EXPRESSION_CEXPRESSIONHASHMIXER_H


// Libraries includes


// Namespace includes
#include "ExpressionSettings.h"
#include "CBuildExpression.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Parser {

		namespace Expression {

			/*! 
			 *	\class		CExpressionHashMixer
			 *	\author		Andreas Steigmiller
			 *	\version	0.1
			 *	\brief		TODO
			 */
			class CExpressionHashMixer {
				// public methods
				public:
					//! Constructor
					CExpressionHashMixer();

					//! Destructor
					virtual ~CExpressionHashMixer();

					static cint64 getHashValueMixed(cint64 hashValue1);
					static cint64 getHashValueMixed(cint64 hashValue1, cint64 hashValue2);
					static cint64 getHashValueMixed(cint64 hashValue1, cint64 hashValue2, cint64 hashValue3);
					static cint64 getHashValueMixed(cint64 hashValue1, cint64 hashValue2, cint64 hashValue3, cint64 hashValue4);

				// protected methods
				protected:

				// private methods
				private:
			};

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_EXPRESSION_CEXPRESSIONHASHMIXER_H
