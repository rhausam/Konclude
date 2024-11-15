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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCEUNSATISFIABLECACHEENTRIESHASH_H
#define KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCEUNSATISFIABLECACHEENTRIESHASH_H

// Libraries includes
#include <QHash>
#include <QMutex>
#include <QSemaphore>
#include <QVector>
#include <QLinkedList>

// Namespace includes
#include "CacheSettings.h"
#include "COccurrenceUnsatisfiableCacheEntry.h"
#include "CCacheValue.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		COccurrenceUnsatisfiableCacheEntriesHash
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class COccurrenceUnsatisfiableCacheEntriesHash : public QHash<CCacheValue,COccurrenceUnsatisfiableCacheEntry *> {
					// public methods
					public:
						//! Constructor
						COccurrenceUnsatisfiableCacheEntriesHash();
						COccurrenceUnsatisfiableCacheEntriesHash(const COccurrenceUnsatisfiableCacheEntriesHash &copyHash);

						//! Destructor
						virtual ~COccurrenceUnsatisfiableCacheEntriesHash();


						qint64 incReferenceCount();
						bool decReferenceCountReturnHasToBeDeleted();



					// protected methods
					protected:

					// protected variables
					protected:
						qint64 referenceCount;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCEUNSATISFIABLECACHEENTRIESHASH_H
