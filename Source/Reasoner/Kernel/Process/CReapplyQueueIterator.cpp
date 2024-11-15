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

#include "CReapplyQueueIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CReapplyQueueIterator::CReapplyQueueIterator() {
					mStaticReapplyDesLinker = nullptr;
					mDynamicReapplyDesLinker = nullptr;
				}

				CReapplyQueueIterator::CReapplyQueueIterator(CReapplyConceptDescriptor* staticReapplyDesLinker, CReapplyConceptDescriptor* dynamicReapplyDesLinker)
						: mStaticReapplyDesLinker(staticReapplyDesLinker), mDynamicReapplyDesLinker(dynamicReapplyDesLinker) {
				}

				CReapplyConceptDescriptor* CReapplyQueueIterator::next(bool moveNext) {
					CReapplyConceptDescriptor* nextDes = nullptr;
					if (mDynamicReapplyDesLinker) {
						nextDes = mDynamicReapplyDesLinker;
						if (moveNext) {
							mDynamicReapplyDesLinker = mDynamicReapplyDesLinker->getNext();
						}
					} else if (mStaticReapplyDesLinker) {
						nextDes = mStaticReapplyDesLinker;
						if (moveNext) {
							mStaticReapplyDesLinker = mStaticReapplyDesLinker->getNext();
						}
					}
					return nextDes;
				}

				bool CReapplyQueueIterator::hasNext() {
					return mDynamicReapplyDesLinker || mStaticReapplyDesLinker;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
