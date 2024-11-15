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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CSIGNATURESATISFIABLEEXPANDERCACHEENTRY_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CSIGNATURESATISFIABLEEXPANDERCACHEENTRY_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CCacheValue.h"
#include "CExpanderCacheValueLinker.h"
#include "CExpanderBranchedLinker.h"
#include "CSignatureSatisfiableExpanderCacheContext.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		CSignatureSatisfiableExpanderCacheEntry
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSignatureSatisfiableExpanderCacheEntry {
					// public methods
					public:
						//! Constructor
						CSignatureSatisfiableExpanderCacheEntry(CSignatureSatisfiableExpanderCacheContext* context);


						CSignatureSatisfiableExpanderCacheEntry* appendExpanderCacheValueLinker(CExpanderCacheValueLinker* linker);
						CExpanderCacheValueLinker* getExpanderCacheValueLinker();

						CSignatureSatisfiableExpanderCacheEntry* appendExpanderBranchedLinker(CExpanderBranchedLinker* linker);
						CExpanderBranchedLinker* getExpanderBranchedLinker();

						CCACHINGHASH<cint64,CExpanderCacheValueLinker*>* getTagExpanderCacheValueHash();
						CSignatureSatisfiableExpanderCacheEntry* setTagExpanderCacheValueHash(CCACHINGHASH<cint64,CExpanderCacheValueLinker*>* hash);
						cint64 getExpanderCacheValueCount();

						bool isSatisfiable();
						CSignatureSatisfiableExpanderCacheEntry* setSatisfiable(bool satisfiable);

						bool isSatisfiableWithoutBranchedConcepts();
						CSignatureSatisfiableExpanderCacheEntry* setSatisfiableWithoutBranchedConcepts(bool sat);

						bool hasMultipleExpanded();
						CSignatureSatisfiableExpanderCacheEntry* setMultipleExpanded(bool expanded);


					// protected methods
					protected:

					// protected variables
					protected:
						CSignatureSatisfiableExpanderCacheContext* mContext;
						CCACHINGHASH<cint64,CExpanderCacheValueLinker*>* mTagExpanderCacheValueHash;
						CExpanderCacheValueLinker* mDetExpandValueLinker;
						CExpanderBranchedLinker* mExpandBranchedLinker;
						cint64 mDetExpandCount;
						bool mSatisfiable;
						bool mSatisfiableWithoutBranchedConcept;
						bool mMultipleExpanded;



					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CSIGNATURESATISFIABLEEXPANDERCACHEENTRY_H
