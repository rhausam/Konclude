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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_EVENTS_CWRITEUNSATISFIABLECACHEENTRYEVENT_H
#define KONCLUDE_REASONER_KERNEL_CACHE_EVENTS_CWRITEUNSATISFIABLECACHEENTRYEVENT_H

// Libraries includes
#include <QEvent>
#include <QList>


// Namespace includes

// Other includes
#include "../CCacheValue.h"
#include "../CacheSettings.h"

#include "Concurrent/Events/CCustomEvent.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				namespace Events {


					/*! 
					 *	\class		CWriteUnsatisfiableCacheEntryEvent
					 *	\author		Andreas Steigmiller
					 *	\version	0.1
					 *	\brief		
					 */
					class CWriteUnsatisfiableCacheEntryEvent : public Concurrent::Events::CCustomEvent {
						// public methods
						public:
							//! Constructor
							CWriteUnsatisfiableCacheEntryEvent(QList<CCacheValue> list = QList<CCacheValue>());

							//! Destructor
							virtual ~CWriteUnsatisfiableCacheEntryEvent();

							QList<CCacheValue> *getCacheEntryList();
							void setCacheEntryList(QList<CCacheValue> list);


						// protected methods
						protected:

						// private members
						private:
							QList<CCacheValue> cacheEntry;


					};

				}; // end namespace Events

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_EVENTS_CWRITEUNSATISFIABLECACHEENTRYEVENT_H
