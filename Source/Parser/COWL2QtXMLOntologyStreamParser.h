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

#ifndef KONCLUDE_PARSER_COWL2QTXMLONTOLOGYSTREAMPARSER_H
#define KONCLUDE_PARSER_COWL2QTXMLONTOLOGYSTREAMPARSER_H

// Libraries includes
#include <QXmlDefaultHandler>
#include <QXmlSimpleReader>
#include <QXmlStreamReader>

// Namespace includes
#include "ParserSettings.h"
#include "COntologyBuilder.h"
#include "CXMLOWL2StreamHandler.h"


// Other includes
#include "Context/CContextBase.h"

#include "Utilities/Memory/CMemoryPoolAllocationManager.h"
#include "Utilities/Memory/CMemoryPoolProvider.h"
#include "Utilities/Memory/CNewAllocationMemoryPoolProvider.h"

// Logger includes
#include "Logger/CLogger.h"


using namespace Konclude::Logger;



namespace Konclude {

	using namespace Utilities::Memory;
	using namespace Context;

	namespace Parser {







		/*! 
		 *
		 *		\class		COWL2QtXMLOntologyStreamParser
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class COWL2QtXMLOntologyStreamParser {
			// public methods
			public:
				//! Constructor
				COWL2QtXMLOntologyStreamParser(COntologyBuilder* ontologyBuilder);

				virtual ~COWL2QtXMLOntologyStreamParser();

				bool parseOntology(const QString& fileString);
				bool parseOntology(QIODevice* ioDevice);

			// protected methods
			protected:


			// protected variables
			protected:
				CXMLOWL2StreamHandler* mOWLXMLStreamHandler;
				COntologyBuilder* mOntologyBuilder;



			// private methods
			private:


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_COWL2QTXMLONTOLOGYSTREAMPARSER_H
