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

#include "CTellKnowledgeBaseOWL2XMLNodeCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CTellKnowledgeBaseOWL2XMLNodeCommand::CTellKnowledgeBaseOWL2XMLNodeCommand(const QString &knowledgeBaseNameString, const QDomElement &owl2XMLNode, CCommand *parentSuperCommand) 
						: CTellKnowledgeBaseCommand(knowledgeBaseNameString,parentSuperCommand) {
					owl2Node = owl2XMLNode;
				}


				CTellKnowledgeBaseOWL2XMLNodeCommand::~CTellKnowledgeBaseOWL2XMLNodeCommand() {
				}


				qint64 CTellKnowledgeBaseOWL2XMLNodeCommand::getCommandTag() {
					return TELLKNOWLEDGEBASEOWL2XMLCOMMAND;
				}




				QString CTellKnowledgeBaseOWL2XMLNodeCommand::getBriefCommandDescription() {
					return QString("Tell-'%1'-Knowledge-Base-OWL2-XML-Axioms Command").arg(kbString);
				}


				QDomElement CTellKnowledgeBaseOWL2XMLNodeCommand::getOWL2Node() {
					return owl2Node;
				}




			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude
