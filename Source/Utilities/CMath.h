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

#ifndef KONCLUDE_UTILITIES_CMATH_H
#define KONCLUDE_UTILITIES_CMATH_H

// Libraries includes


// Namespace includes
#include "CMemoryReleaser.h"
#include "UtilitiesSettings.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Utilities {


		/*! 
		 *
		 *		\class		CMath
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CMath {
			// public methods
			public:
				//! Constructor
				CMath();

				//! Destructor
				virtual ~CMath();


				static qint64 log2(qint64 x);



			// protected methods
			protected:

			// protected variables
			protected:

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CMATH_H