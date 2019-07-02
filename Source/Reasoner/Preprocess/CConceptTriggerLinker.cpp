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

#include "CConceptTriggerLinker.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CConceptTriggerLinker::CConceptTriggerLinker() : CSortedLinkerBase<CConceptTriggerLinker*,CConceptTriggerLinker>(this) {
			}

			CConceptTriggerLinker* CConceptTriggerLinker::initConceptTriggerLinker(CConcept* triggerConcept, cint64 triggerComplexity) {
				mTriggerConcept = triggerConcept;
				mTriggerComplexity = triggerComplexity;
				return this;
			}

			CConcept* CConceptTriggerLinker::getTriggerConcept() {
				return mTriggerConcept;
			}

			cint64 CConceptTriggerLinker::getTriggerComplexity() {
				return mTriggerComplexity;
			}

			CConceptTriggerLinker* CConceptTriggerLinker::setTriggerComplexity(cint64 complexity) {
				mTriggerComplexity = complexity;
				return this;
			}

			bool CConceptTriggerLinker::operator<=(const CConceptTriggerLinker& conceptTriggerLinker) {
				if (mTriggerComplexity > conceptTriggerLinker.mTriggerComplexity) {
					return true;
				} else if (mTriggerComplexity < conceptTriggerLinker.mTriggerComplexity) {
					return false;
				} 
				return mTriggerConcept >= conceptTriggerLinker.mTriggerConcept;
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude