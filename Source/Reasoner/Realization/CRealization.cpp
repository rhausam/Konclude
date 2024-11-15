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

#include "CRealization.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realization {


			CRealization::CRealization() {
				mRealized = false;
				mConceptRealization = nullptr;
				mSameRealization = nullptr;
				mRoleRealization = nullptr;
			}

			bool CRealization::isRealized() {
				return mRealized;
			}


			CRealization* CRealization::setRealized(bool realized) {
				mRealized = realized;
				return this;
			}

			CConceptRealization* CRealization::getConceptRealization() {
				return mConceptRealization;
			}

			bool CRealization::hasConceptRealization() {
				return mConceptRealization != nullptr;
			}

			CRealization* CRealization::setConceptRealization(CConceptRealization* conRealization) {
				mConceptRealization = conRealization;
				return this;
			}



			CSameRealization* CRealization::getSameRealization() {
				return mSameRealization;
			}

			bool CRealization::hasSameRealization() {
				return mSameRealization != nullptr;
			}

			CRealization* CRealization::setSameRealization(CSameRealization* sameRealization) {
				mSameRealization = sameRealization;
				return this;
			}

			bool CRealization::hasRoleRealization() {
				return mRoleRealization != nullptr;
			}

			CRoleRealization* CRealization::getRoleRealization() {
				return mRoleRealization;
			}

			CRealization* CRealization::setRoleRealization(CRoleRealization* roleRealization) {
				mRoleRealization = roleRealization;
				return this;
			}


		}; // end namespace Realization

	}; // end namespace Reasoner

}; // end namespace Konclude
