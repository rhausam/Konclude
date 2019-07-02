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

#ifndef KONCLUDE_CONTROL_LOADER_CREASONEREVALUATIONLOADER_H
#define KONCLUDE_CONTROL_LOADER_CREASONEREVALUATIONLOADER_H

// Libraries includes
#include <QString>
#include <QRegExp>

// Namespace includes
#include "LoaderSettings.h"
#include "CLoader.h"
#include "CLoaderFactory.h"

// Other includes
#include "Test/Evaluation/CReasonerEvaluationExecutorThread.h"
#include "Test/Evaluation/CReasonerEvaluationTestsuite.h"

#include "Control/Command/CCommanderManagerThread.h"
#include "Control/Command/CCommanderManagerConfigType.h"

#include "Control/Command/Instructions/CInitializeConfigurationCommand.h"

#include "Config/CConfigDataReader.h"


// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


#include <stdio.h>
#include <iostream>


namespace Konclude {

	using namespace Logger;
	using namespace Config;
	using namespace Test::Evaluation;

	namespace Control {

		using namespace Command;
		using namespace Instructions;

		namespace Loader {

			/*! 
			 *
			 *		\class		CReasonerEvaluationLoader
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationLoader : public CLoader {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationLoader();

					//! Destructor
					virtual ~CReasonerEvaluationLoader();


					virtual CLoader *init(CLoaderFactory *loaderFactory = 0, CConfiguration *config = 0);
					virtual CLoader *load();
					virtual CLoader *exit();



				// protected methods
				protected:

					
				// protected variables
				protected:
					CConfiguration* mLoaderConfig;
					CCommanderManagerThread* mReasonerCommander;
					CCommanderManagerThread* mDeleteReasonerCommander;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_LOADER_CREASONEREVALUATIONLOADER_H
