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

#include "CProcessTag.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CProcessTag::CProcessTag(cint64 processTag) : mProcessTag(processTag) {
				}


				cint64 CProcessTag::getProcessTag() {
					return mProcessTag;
				}

				CProcessTag* CProcessTag::initProcessTag(cint64 processTag) {
					mProcessTag = processTag;
					return this;
				}

				CProcessTag* CProcessTag::setProcessTag(cint64 processTag) {
					mProcessTag = processTag;
					return this;
				}

				bool CProcessTag::isProcessTagUpdated(cint64 processTag) {
					return processTag > mProcessTag;
				}

				bool CProcessTag::isProcessTagUpToDate(cint64 processTag) {
					return mProcessTag >= processTag;
				}

				bool CProcessTag::updateProcessTag(cint64 processTag) {
					bool updated = mProcessTag != processTag;
					mProcessTag = processTag;
					return updated;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
