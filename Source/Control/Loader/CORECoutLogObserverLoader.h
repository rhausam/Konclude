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

#ifndef KONCLUDE_CONTROL_LOADER_CORECOUTLOGOBSERVERLOADER_H
#define KONCLUDE_CONTROL_LOADER_CORECOUTLOGOBSERVERLOADER_H

// Libraries includes
#include <QString>
#include <QRegExp>

// Namespace includes
#include "CLoader.h"

// Other includes
#include "Utilities/CSingletonProvider.hpp"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CConsolePrintConfigurableLogObserver.h"


namespace Konclude {

	using namespace Logger;
	using namespace Utilities;

	namespace Control {

		namespace Loader {

			/*! 
			 *
			 *		\class		CORECoutLogObserverLoader
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CORECoutLogObserverLoader : public CLoader {
				// public methods
				public:
					//! Constructor
					CORECoutLogObserverLoader();

					//! Destructor
					virtual ~CORECoutLogObserverLoader();


					virtual CLoader *init(CLoaderFactory *loaderFactory = 0, CConfiguration *config = 0);
					virtual CLoader *load();
					virtual CLoader *exit();


				// protected methods
				protected:

				// protected variables
				protected:
					CConsolePrintConfigurableLogObserver *consoleSingletonLogObserver;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_LOADER_CORECOUTLOGOBSERVERLOADER_H
