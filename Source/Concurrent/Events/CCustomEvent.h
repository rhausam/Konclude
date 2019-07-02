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
#ifndef KONCLUDE_CONCURRENT_CCUSTOMEVENT_H
#define KONCLUDE_CONCURRENT_CCUSTOMEVENT_H

#include <QEvent>



namespace Konclude {

	namespace Concurrent {

		namespace Events {


			/*! 
			 *	\class		CCustomEvent
			 *	\author		Andreas Steigmiller
			 *	\version	0.1
			 *	\brief		
			 */
			class CCustomEvent : public QEvent {
				// public methods
				public:
					//! Constructor
					CCustomEvent(QEvent::Type type, void *object = 0);

					//! Destructor
					virtual ~CCustomEvent();

					CCustomEvent *setObject(void *object);
					void *getObject();


				// protected methods
				protected:


				// private members
				private:

					void *obj;
			};

		}; // end namespace Events

	}; // end namespace Concurrent

}; // end namespace Konclude

#endif // KONCLUDE_CONCURRENT_CCUSTOMEVENT_H