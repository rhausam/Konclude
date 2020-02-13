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

#include "CParseInstallKnowledgeBaseExternalTriplesDSNConnectionCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CParseInstallKnowledgeBaseExternalTriplesDSNConnectionCommand::CParseInstallKnowledgeBaseExternalTriplesDSNConnectionCommand(const QString &knowledgeBaseNameString, const QString& triplesDBName, const QString& configDSNString, CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand, CCommand *parentSuperCommand)
						: CLoadKnowledgeBaseCommand(knowledgeBaseNameString,parentSuperCommand) {
					mTriplesDBName = triplesDBName;
					mConfigDSNString = configDSNString;
					mOntologyRevisionProviderCommand = ontologyRevisionProviderCommand;
					addCommandPrecondition(new CCommandProcessedPrecondition(mOntologyRevisionProviderCommand->getCommand()));
				}


				CParseInstallKnowledgeBaseExternalTriplesDSNConnectionCommand::~CParseInstallKnowledgeBaseExternalTriplesDSNConnectionCommand() {
				}


				qint64 CParseInstallKnowledgeBaseExternalTriplesDSNConnectionCommand::getCommandTag() {
					return PARSEINSTALLKNOWLEDGEBASEEXTERNALTRIPLESDSNCONNECTIONCOMMAND;
				}


				COntologyRevision* CParseInstallKnowledgeBaseExternalTriplesDSNConnectionCommand::getOntologyRevision() {
					return mOntologyRevisionProviderCommand->getOntologyRevision();
				}


				QString CParseInstallKnowledgeBaseExternalTriplesDSNConnectionCommand::getBriefCommandDescription() {
					return QString("Parse-Install-'%1'-Knowledge-Base-External-Triples-DSN-Connection Command").arg(kbString);
				}




				CCommand* CParseInstallKnowledgeBaseExternalTriplesDSNConnectionCommand::getCommand() {
					return this;
				}


				QString CParseInstallKnowledgeBaseExternalTriplesDSNConnectionCommand::getTriplesDBName() {
					return mTriplesDBName;
				}

				QString CParseInstallKnowledgeBaseExternalTriplesDSNConnectionCommand::getConfigDSNString() {
					return mConfigDSNString;
				}

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude