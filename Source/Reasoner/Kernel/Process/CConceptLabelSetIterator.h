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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CCONCEPTLABELSETITERATOR_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CCONCEPTLABELSETITERATOR_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CConceptDescriptorDependencyPair.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CConceptLabelSetIterator
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CConceptLabelSetIterator {
					// public methods
					public:
						//! Constructor
						CConceptLabelSetIterator(cint64 conceptCount, CConceptDescriptor* conceptDesLinker, const CPROCESSMAP<cint64,CConceptDescriptorDependencyPair>::iterator& beginIt, const CPROCESSMAP<cint64,CConceptDescriptorDependencyPair>::iterator& endIt);

						cint64 getRemainingConceptCount();

						CConceptDescriptor* getConceptDescriptor();
						CDependencyTrackPoint* getDependencyTrackPoint();						

						CConceptLabelSetIterator& operator++();
						CConceptDescriptor* operator*();
						CConceptLabelSetIterator* moveNext();
						bool hasValue();
						bool hasNext();
						CConceptDescriptor* next(bool moveToNext = true);

						bool operator!=(CConceptLabelSetIterator& iterator);
						bool operator==(CConceptLabelSetIterator& iterator);


					// protected methods
					protected:

					// protected variables
					protected:
						CPROCESSMAP<cint64,CConceptDescriptorDependencyPair>::iterator mConDesDepBeginIt;
						CPROCESSMAP<cint64,CConceptDescriptorDependencyPair>::iterator mConDesDepEndIt;
						CConceptDescriptor* mConceptDesLinkerIt;
						cint64 mConceptCount;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CCONCEPTLABELSETITERATOR_H
