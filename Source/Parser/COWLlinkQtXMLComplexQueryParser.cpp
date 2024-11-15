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

#include "COWLlinkQtXMLComplexQueryParser.h"


namespace Konclude {

	namespace Parser {



		COWLlinkQtXMLComplexQueryParser::COWLlinkQtXMLComplexQueryParser(CQueryBuilder *queryBuilder, COntologyQtXMLDocumentParser* ontoParser) : COWLlinkQueryParser(queryBuilder) {
			mOntoParser = ontoParser;
			LOG(INFO,"::Konclude::Parser::OWLlinkQtXMLComplexQueryParser",logTr("OWLlink Query Parser initialized."),this);
		}


		COWLlinkQtXMLComplexQueryParser::~COWLlinkQtXMLComplexQueryParser() {
		}

		QList<CBuildExpression*> COWLlinkQtXMLComplexQueryParser::parseOntologyChildNodesInverseHandling(QDomElement *node, bool& inversed) {
			return parseOntologyChildNodes(node);
		}


		QList<CBuildExpression *> COWLlinkQtXMLComplexQueryParser::parseOntologyChildNodes(QDomElement *node) {
			QList<CBuildExpression *> builds;
			QDomElement childNode = node->firstChildElement();
			while (!childNode.isNull()) {
				CBuildExpression *expression = mOntoParser->getParsedBuildNode(&childNode);
				if (expression) {
					builds.append(expression);
				}
				childNode = childNode.nextSiblingElement();
			}
			return builds;
		}


	}; // end namespace Parser

}; // end namespace Konclude
