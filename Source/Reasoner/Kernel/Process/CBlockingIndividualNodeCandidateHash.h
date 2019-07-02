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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CBLOCKINGINDIVIDUALNODECANDIDATEHASH_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CBLOCKINGINDIVIDUALNODECANDIDATEHASH_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CIndividualProcessNode.h"
#include "CBlockingIndividualNodeCandidateData.h"


// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CBlockingIndividualNodeCandidateHash
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBlockingIndividualNodeCandidateHash {
					// public methods
					public:
						//! Constructor
						CBlockingIndividualNodeCandidateHash(CProcessContext* context = nullptr);

						//! Destructor
						virtual ~CBlockingIndividualNodeCandidateHash();

						CBlockingIndividualNodeCandidateHash* initBlockingIndividualNodeCandidateHash(CBlockingIndividualNodeCandidateHash* prevBlockCandHash);

						CBlockingIndividualNodeCandidateData* getBlockingIndividualCandidateData(CConcept* initializationConcept, bool conceptNegation, bool create = true);
						CBlockingIndividualNodeCandidateData* getBlockingIndividualCandidateData(CConceptDescriptor* initializationConDes, bool create = true);

						
					// protected methods
					protected:

					// protected variables
					protected:
						class CBlockingCandidateHashData {
							public:
								inline CBlockingCandidateHashData() {
									mCandidateIndiData = nullptr;
									mPrevCandidateIndiData = nullptr;
								}
								inline CBlockingCandidateHashData(const CBlockingCandidateHashData& data) {
									mCandidateIndiData = nullptr;
									mPrevCandidateIndiData = data.mPrevCandidateIndiData;
								}
								CBlockingIndividualNodeCandidateData* mCandidateIndiData;
								CBlockingIndividualNodeCandidateData* mPrevCandidateIndiData;
						};
						CProcessContext* mContext;
						CMemoryAllocationManager* mMemMan;

						CPROCESSHASH< QPair<CConcept*,bool> ,CBlockingCandidateHashData>* mBlockCandidateHash;
						CPROCESSHASH< QPair<CConcept*,bool> ,CBlockingCandidateHashData>* mUseBlockCandidateHash;
						CPROCESSHASH< QPair<CConcept*,bool> ,CBlockingCandidateHashData>* mPrevBlockCandidateHash;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CBLOCKINGINDIVIDUALNODECANDIDATEHASH_H
