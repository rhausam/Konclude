/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CActiveEntityCountItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CActiveEntityCountItem::CActiveEntityCountItem() {
				mActiveCount = 0;
				mEntity = nullptr;
			}

			cint64 CActiveEntityCountItem::getActiveCount() {
				return mActiveCount;
			}

			CActiveEntityCountItem* CActiveEntityCountItem::setActiveCount(cint64 activeCount) {
				mActiveCount = activeCount;
				return this;
			}


			CActiveEntityCountItem* CActiveEntityCountItem::incActiveCount(cint64 incCount) {
				mActiveCount += incCount;
				return this;
			}

			CActiveEntityCountItem* CActiveEntityCountItem::decActiveCount(cint64 decCount) {
				mActiveCount -= decCount;
				return this;
			}


			CActiveEntityCountItem* CActiveEntityCountItem::setEntity(CExpressionEntity* entity) {
				mEntity = entity;
				return this;
			}

			CExpressionEntity* CActiveEntityCountItem::getEntity() {
				return mEntity;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude