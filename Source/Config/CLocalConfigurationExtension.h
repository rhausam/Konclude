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

#ifndef KONCLUDE_CONFIG_CLOCALCONFIGURATIONEXTENSION_H
#define KONCLUDE_CONFIG_CLOCALCONFIGURATIONEXTENSION_H

// Libraries includes


// Namespace includes
#include "CConfigurationProvider.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Config {

		/*! 
		 *
		 *		\class		CLocalConfigurationExtension
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CLocalConfigurationExtension : public CConfigurationBase {
			// public methods
			public:
				//! Constructor
				CLocalConfigurationExtension(CConfigurationBase *previousConfiguration, qint64 configID);

				//! Destructor
				virtual ~CLocalConfigurationExtension();


				virtual CLocalConfigurationExtension *addConfigurationProvider(CConfigurationProvider *configurationProvider);

				virtual CConfigData *getRelatedConfigChange(qint64 configIndex, bool defaultCompensation = true);

				virtual qint64 getConfigurationLevel();

				virtual CConfigurationBase *appendConfigurationsToList(QLinkedList<CConfigurationBase *> *configurationList);


			// protected methods
			protected:

			// protected variables
			protected:
				QLinkedList<CConfigurationProvider *> remConfigList;
				
			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Config

}; // end namespace Konclude

#endif // KONCLUDE_CONFIG_CLOCALCONFIGURATIONEXTENSION_H
