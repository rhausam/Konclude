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

#include "CCalculateQueriesCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CCalculateQueriesCommand::CCalculateQueriesCommand(CQueriesCommandProvider *queriesCommandProvider, CCommand *parentSuperCommand)
						: CPreconditionCommand(parentSuperCommand) {

					mQueriesProvider = queriesCommandProvider;
					addCommandPrecondition(new CCommandProcessedPrecondition(mQueriesProvider->getCommand()));
				}


				CCalculateQueriesCommand::CCalculateQueriesCommand(const QList<CQuery*>& queryList, CCommand *parentSuperCommand): CPreconditionCommand(parentSuperCommand) {
					mQueriesProvider = nullptr;
					mQueryList = queryList;
				}


				CCalculateQueriesCommand::~CCalculateQueriesCommand() {
				}

				qint64 CCalculateQueriesCommand::getCommandTag() {
					return CALCULATEQUERYCOMMAND;
				}


				QString CCalculateQueriesCommand::getBriefCommandDescription() {
					return QString("Calculate-Query Command");
				}


				CCommand *CCalculateQueriesCommand::getCommand() {
					return this;
				}

				QList<CQuery*> CCalculateQueriesCommand::getQueryList() {
					if (mQueriesProvider) {
						return mQueriesProvider->getQueryList();
					}
					return mQueryList;
				}



			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude
