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

#include "CBuildQueryCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CBuildQueryCommand::CBuildQueryCommand(CQueryCommandProvider *queryCommandProvider, CCommand *parentSuperCommand) 
						: CPreconditionCommand(parentSuperCommand) {

					queryProvider = queryCommandProvider;
					query = 0;
				}


				CBuildQueryCommand::~CBuildQueryCommand() {
				}

				qint64 CBuildQueryCommand::getCommandTag() {
					return BUILDQUERYCOMMAND;
				}


				QString CBuildQueryCommand::getBriefCommandDescription() {
					return QString("Build-Query Command");
				}


				CBuildQueryCommand *CBuildQueryCommand::setQueryCommandProvider(CQueryCommandProvider *queryCommandProvider) {
					queryProvider = queryCommandProvider;
					return this;
				}


				CCommand *CBuildQueryCommand::getCommand() {
					return this;
				}

				CQuery *CBuildQueryCommand::getQuery() {
					if (queryProvider) {
						return queryProvider->getQuery();
					}
					return query;
				}




			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude
