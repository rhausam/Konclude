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

#include "CProcessQueryOWL2XMLNodeCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CProcessQueryOWL2XMLNodeCommand::CProcessQueryOWL2XMLNodeCommand(const QString &knowledgeBaseNameString, const QDomElement &queryNode, CCommand *parentSuperCommand) 
						: CProcessQueryCommand(knowledgeBaseNameString,parentSuperCommand) {

					node = queryNode;
				}


				CProcessQueryOWL2XMLNodeCommand::~CProcessQueryOWL2XMLNodeCommand() {
				}


				QString CProcessQueryOWL2XMLNodeCommand::getBriefCommandDescription() {
					return QString("Process-Knowledge-Base-Query Command");
				}


				QDomElement CProcessQueryOWL2XMLNodeCommand::getQueryNode() {
					return node;
				}


				qint64 CProcessQueryOWL2XMLNodeCommand::getCommandTag() {
					return PROCESSQUERYKNOWLEDGEBASEOWL2XMLCOMMAND;
				}




			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude
