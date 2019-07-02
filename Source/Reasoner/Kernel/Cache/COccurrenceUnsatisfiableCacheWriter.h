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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCEUNSATISFIABLECACHEWRITER_H
#define KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCEUNSATISFIABLECACHEWRITER_H

// Libraries includes
#include <QListIterator>

// Namespace includes
#include "CacheSettings.h"
#include "CCacheValue.h"
#include "COccurrenceUnsatisfiableCache.h"
#include "COccurrenceUnsatisfiableCacheEntry.h"
#include "CUnsatisfiableCacheWriter.h"


// Other includes
#include "Utilities/CMemoryManager.hpp"
#include "Utilities/CDblLinker.hpp"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		COccurrenceUnsatisfiableCacheWriter
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class COccurrenceUnsatisfiableCacheWriter : public CUnsatisfiableCacheWriter {
					// public methods
					public:
						//! Constructor
						COccurrenceUnsatisfiableCacheWriter(COccurrenceUnsatisfiableCache *unsatisfiableCache);

						//! Destructor
						virtual ~COccurrenceUnsatisfiableCacheWriter();


						void setUnsatisfiable(QList<CCacheValue> &itemList);
						virtual void setUnsatisfiable(QVector<CCacheValue> &itemVec, qint64 count, QVector<CCacheValue> &clashVec, qint64 clashCount);



					// protected methods
					protected:


					// private methods
					private:

					// private variables
					private:
						COccurrenceUnsatisfiableCache *cache;

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCEUNSATISFIABLECACHEWRITER_H
