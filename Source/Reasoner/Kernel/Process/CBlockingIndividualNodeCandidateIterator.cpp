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

#include "CBlockingIndividualNodeCandidateIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CBlockingIndividualNodeCandidateIterator::CBlockingIndividualNodeCandidateIterator(CPROCESSMAP<cint64,CIndividualProcessNode*>* candidateMap, cint64 candidateIndividualID)
						: mCandidateMap(candidateMap) {

					if (mCandidateMap) {
						mItBegin = mCandidateMap->upperBound(-candidateIndividualID);
						mItEnd = mCandidateMap->end();
						mItLast = mItBegin;
					}
				}



				bool CBlockingIndividualNodeCandidateIterator::hasNext() {
					return mCandidateMap && mItBegin != mItEnd;
				}


				bool CBlockingIndividualNodeCandidateIterator::hasIndividualCandidates() {
					return mCandidateMap && !mCandidateMap->isEmpty();
				}

				cint64 CBlockingIndividualNodeCandidateIterator::next(bool moveNext) {
					CIndividualProcessNode* indi = nextIndividualCandidate(moveNext);
					if (indi) {
						return indi->getIndividualNodeID();
					}
					return 0;
				}


				CIndividualProcessNode* CBlockingIndividualNodeCandidateIterator::nextIndividualCandidate(bool moveNext) {
					CIndividualProcessNode* indi = nullptr;
					if (mCandidateMap && mItBegin != mItEnd) {
						indi = mItBegin.value();
						mItLast = mItBegin;
						if (moveNext) {
							++mItBegin;
						}
					}
					return indi;
				}


				bool CBlockingIndividualNodeCandidateIterator::removeLastIndividualCandidate() {
					if (mItLast != mItEnd) {						
						mItBegin = mCandidateMap->erase(mItLast);
						mItLast = mItBegin;
						return true;
					}
					return false;
				}


				bool CBlockingIndividualNodeCandidateIterator::hasIndividualCandidate(cint64 indiID) {
					return mCandidateMap && mCandidateMap->contains(-indiID);
				}

				bool CBlockingIndividualNodeCandidateIterator::hasIndividualCandidate(CIndividualProcessNode* indi) {
					return hasIndividualCandidate(indi->getIndividualNodeID());
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
