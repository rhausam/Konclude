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

#ifndef KONCLUDE_REASONER_ONTOLOGY_COntologyProcessingStatus_H
#define KONCLUDE_REASONER_ONTOLOGY_COntologyProcessingStatus_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		COntologyProcessingStatus
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyProcessingStatus {
				// public methods
				public:
					//! Constructor
					COntologyProcessingStatus();

					cint64 getProcessingFlags();
					COntologyProcessingStatus* setProcessingFlags(cint64 flags);

					cint64 getErrorFlags();
					COntologyProcessingStatus* setErrorFlags(cint64 flags);


					bool hasPartialProcessingFlags(cint64 flags);
					bool hasPartialErrorFlags(cint64 flags);


					const static cint64 PSUNPROCESSED				= 0x0001;
					const static cint64 PSPARTIALLYPROCESSED		= 0x0002;
					const static cint64 PSCOMPLETELYYPROCESSED		= 0x0004;

					const static cint64 PSSUCESSFULL				= 0x0001;
					const static cint64 PSFAILED					= 0x0002;
					const static cint64 PSFAILEDREQUIREMENT			= 0x0004;
					const static cint64 PSUNSUPPORTED				= 0x0008;

					const static cint64 PSDECIDABILITYVIOLATION		= 0x0010;
					const static cint64 PSINCONSITENT				= 0x0020;


				// protected methods
				protected:

				// protected variables
				protected:
					cint64 mProcessingFlags;
					cint64 mErrorFlags;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_COntologyProcessingStatus_H
