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

#ifndef KONCLUDE_Control_COMMAND_CCLASSIFICATIONMANAGERCONFIGTYPE_H
#define KONCLUDE_Control_COMMAND_CCLASSIFICATIONMANAGERCONFIGTYPE_H

// Libraries includes
#include <QString>
#include <QVariant>

// Namespace includes


// Other includes
#include "Reasoner/Classifier/CClassificationManager.h"

#include "Config/CObjectConfigType.h"

#include "Instructions/CInitializeConfigurationCommand.h"
#include "Instructions/CInitializeReasonerCommand.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Config;
	using namespace Reasoner::Classifier;

	namespace Control {

		namespace Command {

			using namespace Instructions;

			/*! 
			 *
			 *		\class		CClassificationManagerConfigType
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CClassificationManagerConfigType :  public CObjectConfigType {
				// public methods
				public:
					//! Constructor
					CClassificationManagerConfigType(CClassificationManager *classificationManager = 0);

					//! Destructor
					virtual ~CClassificationManagerConfigType();

					
					virtual CClassificationManagerConfigType *getTypeCopy();

					virtual CClassificationManager *getClassificationManager();
					virtual CClassificationManagerConfigType *setClassificationManager(CClassificationManager *classificationManager);


					virtual bool equals(CConfigType *dataType);

				// protected methods
				protected:

				// protected variables
				protected:
					CClassificationManager *cManager;
					
				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_COMMAND_CCLASSIFICATIONMANAGERCONFIGTYPE_H
