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

#include "CBranchingORProcessingRestrictionSpecification.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CBranchingORProcessingRestrictionSpecification::CBranchingORProcessingRestrictionSpecification() {
					mContainedOperand = nullptr;
					mConRoleBranchTrigg = nullptr;
					mFirstNotPosAndNegContainedOperand = nullptr;
					mSecondNotPosAndNegContainedOperand = nullptr;
					mFirstNotPosAndNegContainedOperandBranchStats = nullptr;
					mSecondNotPosAndNegContainedOperandBranchStats = nullptr;
					mClashes = nullptr;
				}


				CBranchingORProcessingRestrictionSpecification* CBranchingORProcessingRestrictionSpecification::initBranchingORProcessingRestriction(CBranchingORProcessingRestrictionSpecification* prevRest) {
					CProcessingRestrictionSpecification::initProcessingRestriction(prevRest);
					if (prevRest) {
						mContainedOperand = prevRest->mContainedOperand;
						mConRoleBranchTrigg = prevRest->mConRoleBranchTrigg;
						mFirstNotPosAndNegContainedOperand = prevRest->mFirstNotPosAndNegContainedOperand;
						mSecondNotPosAndNegContainedOperand = prevRest->mSecondNotPosAndNegContainedOperand;
						mClashes = prevRest->mClashes;
						mFirstNotPosAndNegContainedOperandBranchStats = prevRest->mFirstNotPosAndNegContainedOperandBranchStats;
						mSecondNotPosAndNegContainedOperandBranchStats = prevRest->mSecondNotPosAndNegContainedOperandBranchStats;
					}
					return this;
				}

				CBranchingORProcessingRestrictionSpecification* CBranchingORProcessingRestrictionSpecification::setConceptRoleBranchingTrigger(CConceptRoleBranchingTrigger* conRoleBranchTrigger) {
					mConRoleBranchTrigg = conRoleBranchTrigger;
					return this;
				}

				CConceptRoleBranchingTrigger* CBranchingORProcessingRestrictionSpecification::getConceptRoleBranchingTrigger() {
					return mConRoleBranchTrigg;
				}

				CSortedNegLinker<CConcept*>* CBranchingORProcessingRestrictionSpecification::getFirstNotPosAndNegContainedOperand() {
					return mFirstNotPosAndNegContainedOperand;
				}

				CSortedNegLinker<CConcept*>* CBranchingORProcessingRestrictionSpecification::getSecondNotPosAndNegContainedOperand() {
					return mSecondNotPosAndNegContainedOperand;
				}

				CSortedNegLinker<CConcept*>* CBranchingORProcessingRestrictionSpecification::getContainedOperand() {
					return mContainedOperand;
				}


				CBranchingORProcessingRestrictionSpecification* CBranchingORProcessingRestrictionSpecification::setFirstNotPosAndNegContainedOperand(CSortedNegLinker<CConcept*>* firstOp) {
					mFirstNotPosAndNegContainedOperand = firstOp;
					return this;
				}

				CBranchingORProcessingRestrictionSpecification* CBranchingORProcessingRestrictionSpecification::setSecondNotPosAndNegContainedOperand(CSortedNegLinker<CConcept*>* secondOp) {
					mSecondNotPosAndNegContainedOperand = secondOp;
					return this;
				}




				CDisjunctBranchingStatistics* CBranchingORProcessingRestrictionSpecification::getFirstNotPosAndNegContainedOperandBranchingStatistics() {
					return mFirstNotPosAndNegContainedOperandBranchStats;
				}

				CDisjunctBranchingStatistics* CBranchingORProcessingRestrictionSpecification::getSecondNotPosAndNegContainedOperandBranchingStatistics() {
					return mSecondNotPosAndNegContainedOperandBranchStats;
				}

				CBranchingORProcessingRestrictionSpecification* CBranchingORProcessingRestrictionSpecification::setFirstNotPosAndNegContainedOperandBranchingStatistics(CDisjunctBranchingStatistics* branchStats) {
					mFirstNotPosAndNegContainedOperandBranchStats = branchStats;
					return this;
				}

				CBranchingORProcessingRestrictionSpecification* CBranchingORProcessingRestrictionSpecification::setSecondNotPosAndNegContainedOperandBranchingStatistics(CDisjunctBranchingStatistics* branchStats) {
					mSecondNotPosAndNegContainedOperandBranchStats = branchStats;
					return this;
				}




				CBranchingORProcessingRestrictionSpecification* CBranchingORProcessingRestrictionSpecification::setContainedOperand(CSortedNegLinker<CConcept*>* containedOp) {
					mContainedOperand = containedOp;
					return this;
				}



				CBranchingORProcessingRestrictionSpecification* CBranchingORProcessingRestrictionSpecification::addClashedConceptDescriptors(CClashedDependencyDescriptor* clashes) {
					if (clashes) {
						mClashes = clashes->append(mClashes);
					}
					return this;
				}

				CClashedDependencyDescriptor* CBranchingORProcessingRestrictionSpecification::getClashedConceptDescriptors() {
					return mClashes;
				}


			}; // end namespace Kernel

		}; // end namespace Process

	}; // end namespace Reasoner

}; // end namespace Konclude
