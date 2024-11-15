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

#include "CBranchingTree.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				namespace Dependency {


					CBranchingTree::CBranchingTree(CProcessContext* processContext) {
						mPocessContext = processContext;
					}


					CBranchingTree* CBranchingTree::initBranchingTree(CBranchingTree* tree) {
						mRootNode = nullptr;
						mCurrNode = nullptr;
						mPrevCurrNode = nullptr;
						mBaseDepNode = nullptr;
						if (tree) {
							mPrevCurrNode = tree->mPrevCurrNode;
							mRootNode = tree->mRootNode;
							mBaseDepNode = tree->mBaseDepNode;
						}
						return this;
					}


					CBranchTreeNode* CBranchingTree::getBranchTreeNode(Task::CSatisfiableCalculationTask* task, bool forceBranchCreation) {
						if (mCurrNode) {
							if (mCurrNode->getSatisfiableCalculationTask() != task) {
								CBranchTreeNode* branchNode = CObjectParameterizingAllocator< CBranchTreeNode,CProcessContext* >::allocateAndConstructAndParameterize(mPocessContext->getUsedMemoryAllocationManager(),mPocessContext);
								branchNode->initBranchingCopyNode(mCurrNode,task);
								mCurrNode = branchNode;
								mPrevCurrNode = mCurrNode;
								if (branchNode->isRootNode()) {
									mRootNode = branchNode;
								}
							}
							if (forceBranchCreation) {
								CBranchTreeNode* branchNode = CObjectParameterizingAllocator< CBranchTreeNode,CProcessContext* >::allocateAndConstructAndParameterize(mPocessContext->getUsedMemoryAllocationManager(),mPocessContext);
								branchNode->initBranchingChildNode(mCurrNode,task);
								mCurrNode = branchNode;
								mPrevCurrNode = mCurrNode;
							}
						} else if (!mCurrNode) {
							if (mPrevCurrNode) {
								CBranchTreeNode* branchNode = CObjectParameterizingAllocator< CBranchTreeNode,CProcessContext* >::allocateAndConstructAndParameterize(mPocessContext->getUsedMemoryAllocationManager(),mPocessContext);
								branchNode->initBranchingChildNode(mPrevCurrNode,task);
								mCurrNode = branchNode;
								mPrevCurrNode = mCurrNode;
							} else {
								CBranchTreeNode* branchNode = CObjectParameterizingAllocator< CBranchTreeNode,CProcessContext* >::allocateAndConstructAndParameterize(mPocessContext->getUsedMemoryAllocationManager(),mPocessContext);
								branchNode->initBranchingRootNode(task);
								mCurrNode = branchNode;
								mRootNode = branchNode;
								mPrevCurrNode = mCurrNode;
							}
						}
						return mCurrNode;
					}


					CDependencyNode* CBranchingTree::getBaseDependencyNode(bool create) {
						if (!mBaseDepNode && create) {
							CIndependentBaseDependencyNode* indBaseDepNode = CObjectParameterizingAllocator< CIndependentBaseDependencyNode,CProcessContext* >::allocateAndConstructAndParameterize(mPocessContext->getUsedMemoryAllocationManager(),mPocessContext);
							indBaseDepNode->initIndependentBaseDependencyNode();
							mBaseDepNode = indBaseDepNode;
						}
						return mBaseDepNode;
					}


				}; // end namespace Dependency

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
