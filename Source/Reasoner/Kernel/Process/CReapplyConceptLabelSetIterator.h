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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYCONCEPTLABELSETITERATOR_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYCONCEPTLABELSETITERATOR_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CConceptDescriptorDependencyReapplyData.h"
#include "CReapplyConceptLabelHash.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CReapplyConceptLabelSetIterator
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CReapplyConceptLabelSetIterator {
					// public methods
					public:
						//! Constructor; without hashes it walks the concept descriptor linker (newest first), with the hashes it
						//! walks the union of their tags in ascending order, the own hash's entry taking precedence over the
						//! additional hash's one, skipping entries without concept descriptor unless skipEmptyConceptDescriptors is false.
						CReapplyConceptLabelSetIterator(cint64 conceptCount, CConceptDescriptor* conceptDesLinker, const CReapplyConceptLabelHash* conDesDepHash, const CReapplyConceptLabelHash* additionalConDesDepHash, bool skipEmptyConceptDescriptors = true);

						cint64 getRemainingConceptCount();

						CConceptDescriptor* getConceptDescriptor();
						cint64 getDataTag();
						CDependencyTrackPoint* getDependencyTrackPoint();		
						const CCondensedReapplyQueue *getPosNegReapplyQueue();


						CReapplyConceptLabelSetIterator& operator++();
						CConceptDescriptor* operator*();
						CReapplyConceptLabelSetIterator* moveNext();
						bool hasValue();
						bool hasNext();
						CConceptDescriptor* next(bool moveToNext = true);

						bool operator!=(CReapplyConceptLabelSetIterator& iterator);
						bool operator==(CReapplyConceptLabelSetIterator& iterator);


					// protected methods
					protected:
						//! Positions on the smallest remaining tag with an acceptable entry (skips the empty ones if required).
						void resolveCurrent();
						//! Advances past the current tag in the tag arrays.
						void advance();

					// protected variables
					protected:
						const cint64* mKeyIt;
						const cint64* mKeyItEnd;
						const cint64* mAddKeyIt;
						const cint64* mAddKeyItEnd;
						const CReapplyConceptLabelHash* mConDesDepHash;
						const CReapplyConceptLabelHash* mAdditionalConDesDepHash;
						cint64 mCurrentKey;
						CConceptDescriptorDependencyReapplyData* mCurrentData;
						CConceptDescriptor* mConceptDesLinkerIt;
						cint64 mConceptCount;
						bool mSkipEmptyConceptDescriptors;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYCONCEPTLABELSETITERATOR_H
