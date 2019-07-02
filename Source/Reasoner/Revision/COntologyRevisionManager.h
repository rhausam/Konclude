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

#ifndef KONCLUDE_REASONER_REVISION_CONTOLOGYREVISIONMANAGER_H
#define KONCLUDE_REASONER_REVISION_CONTOLOGYREVISIONMANAGER_H

// Libraries includes
#include <QString>

// Namespace includes
#include "COntologyRevision.h"


// Other includes
#include "Reasoner/Ontology/CTerminology.h"
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Control/Command/CCommandDelegater.h"

#include "Config/CConfigurationProvider.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Control::Command;
	using namespace Config;

	namespace Reasoner {

		using namespace Ontology;

		namespace Revision {

			/*! 
			 *
			 *		\class		COntologyRevisionManager
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyRevisionManager : public CCommandDelegater {
				// public methods
				public:
					//! Constructor
					COntologyRevisionManager();

					//! Destructor
					virtual ~COntologyRevisionManager();

					virtual COntologyRevisionManager *initializeManager(CConfigurationProvider *configurationProvider) = 0;

					virtual qint64 getOntologyCount() = 0;
					

				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Revision

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REVISION_CONTOLOGYREVISIONMANAGER_H