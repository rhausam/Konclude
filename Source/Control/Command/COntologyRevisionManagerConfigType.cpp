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

#include "COntologyRevisionManagerConfigType.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			COntologyRevisionManagerConfigType::COntologyRevisionManagerConfigType(COntologyRevisionManager *ontologyRevisionManager) {
				orManager = ontologyRevisionManager;
			}


			COntologyRevisionManagerConfigType::~COntologyRevisionManagerConfigType() {
			}


			COntologyRevisionManagerConfigType *COntologyRevisionManagerConfigType::getTypeCopy() {
				return new COntologyRevisionManagerConfigType(orManager);
			}


			COntologyRevisionManager *COntologyRevisionManagerConfigType::getOntologyRevisionManager() {
				return orManager;
			}

			COntologyRevisionManagerConfigType *COntologyRevisionManagerConfigType::setOntologyRevisionManager(COntologyRevisionManager *ontologyRevisionManager) {
				orManager = ontologyRevisionManager;
				return this;
			}


			bool COntologyRevisionManagerConfigType::equals(CConfigType *dataType) {
				COntologyRevisionManagerConfigType *rCommanderType = dynamic_cast<COntologyRevisionManagerConfigType *>(dataType);
				if (rCommanderType) {
					return (orManager == rCommanderType->orManager);
				}
				return false;
			}



		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude
