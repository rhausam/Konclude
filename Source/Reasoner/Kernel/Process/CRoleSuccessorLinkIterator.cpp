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

#include "CRoleSuccessorLinkIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CRoleSuccessorLinkIterator::CRoleSuccessorLinkIterator()
					: mLinkLinker(nullptr),mLastLink(nullptr) {
				}

				CRoleSuccessorLinkIterator::CRoleSuccessorLinkIterator(const CPROCESSHASH<cint64,CIndividualLinkEdge*>::iterator& beginIt, const CPROCESSHASH<cint64,CIndividualLinkEdge*>::iterator& endIt)
						: mLinkLinker(nullptr),mBeginIt(beginIt),mEndIt(endIt),mLastLink(nullptr) {
				}

				CRoleSuccessorLinkIterator::CRoleSuccessorLinkIterator(CIndividualLinkEdge* linkLinker, CIndividualLinkEdge* lastLink)
						: mLinkLinker(linkLinker),mLastLink(lastLink) {
				}


				bool CRoleSuccessorLinkIterator::hasNext() {
					return (mLinkLinker && mLinkLinker != mLastLink) || mBeginIt != mEndIt;
				}

				CIndividualLinkEdge* CRoleSuccessorLinkIterator::next(bool moveNext) {
					CIndividualLinkEdge* link = nullptr;
					if (mLinkLinker) {
						link = mLinkLinker;
						if (moveNext) {
							mLinkLinker = mLinkLinker->getNext();
						}
					} else {
						if (mBeginIt != mEndIt) {
							link = mBeginIt.value();
							if (moveNext) {
								++mBeginIt;
							}
						}
					}
					return link;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
