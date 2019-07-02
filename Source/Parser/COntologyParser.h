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

#ifndef KONCLUDE_PARSER_CONTOLOGYPARSER_H
#define KONCLUDE_PARSER_CONTOLOGYPARSER_H


// Libraries includes
#include <QString>
#include <QDomElement>


// Namespace includes
#include "Expressions/CBuildExpression.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;



namespace Konclude {

	namespace Parser {

		using namespace Expression;



		/*! 
		 *
		 *		\class		COntologyParser
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		Parser for OWL/XML Files
		 *
		 */
		class COntologyParser {
			// public methods
			public:
				//! Constructor
				COntologyParser();

				//! Destructor
				virtual ~COntologyParser();

				virtual bool parseOntologyFile(const QString& filename) = 0;
				virtual bool parseTellOntologyAxiomNode(QDomElement *ontologyNode) = 0;
				virtual bool parseRetractOntologyAxiomNode(QDomElement *ontologyNode) = 0;

				virtual CBuildExpression *getParsedBuildNode(QDomElement *node) = 0;

			// protected methods
			protected:

			// protected variables
			protected:

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CONTOLOGYPARSER_H
