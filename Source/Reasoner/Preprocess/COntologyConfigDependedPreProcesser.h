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

#ifndef KONCLUDE_REASONER_PREPROCESS_CONTOLOGYCONFIGDEPENDEDPREPROCESSER_H
#define KONCLUDE_REASONER_PREPROCESS_CONTOLOGYCONFIGDEPENDEDPREPROCESSER_H

// Libraries includes


// Namespace includes
#include "COntologyPreProcesser.h"
#include "CLexicalNormalisationPreProcess.h"
#include "CNegationTransformationPreProcess.h"
#include "CSubroleTransformationPreProcess.h"
#include "COntologyAssignmentTransformationPreProcess.h"
#include "COntologyStructureInspectionPreProcess.h"
#include "CRoleChainAutomataTransformationPreProcess.h"
#include "CMappingUpdaterPreProcess.h"
#include "CBranchTriggerPreProcess.h"
#include "COntologyPreProcess.h"
#include "CDataExtenderPreProcess.h"
#include "CConsistenceCheckerPreProcess.h"
#include "CTriggeredImplicationGCIAbsorberPreProcess.h"
#include "CTriggeredImplicationBinaryAbsorberPreProcess.h"
#include "CCommonDisjunctConceptExtractionPreProcess.h"
#include "CCoreOntologyCyclesExtractorPreProcess.h"
#include "CCoreBlockingConceptMarkerPreProcess.h"
#include "CFullNominalSchemaGroundingPreProcess.h"
#include "CNominalSchemaTemplateExtractionPreProcess.h"
#include "CPrecomputationPreProcess.h"
#include "CPreProcessContextBase.h"
#include "CExtractPropagationIntoCreationDirectionPreProcess.h"
#include "CExtractRelevantConceptRolePreProcess.h"
#include "CDisjunctSortingPreProcess.h"

// Other includes
#include "Config/CConfigDataReader.h"

#include "Reasoner/Ontology/COntologyTextFormater.h"

#include "Control/Command/CCommandRecordRouter.h"
#include "Control/Command/Records/CUnspecifiedMessageInformationRecord.h"

#include "Logger/CLogIdentifier.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Control::Command;
	using namespace Control::Command::Records;
	using namespace Logger;

	namespace Reasoner {

		using namespace Ontology;

		namespace Preprocess {

			/*! 
			 *
			 *		\class		COntologyConfigDependedPreProcesser
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyConfigDependedPreProcesser : public COntologyPreProcesser, public CLogIdentifier {
				// public methods
				public:
					//! Constructor
					COntologyConfigDependedPreProcesser(CCommandRecordRouter &commandRecRouter);
					COntologyConfigDependedPreProcesser();

					//! Destructor
					virtual ~COntologyConfigDependedPreProcesser();

					virtual CConcreteOntology *preprocess(CConcreteOntology *ontology, CConfiguration *config);

				// protected methods
				protected:
					virtual COntologyConfigDependedPreProcesser* reportLog(const QString& string);

				// protected variables
				private:
					CCommandRecordRouter* commandRecordRouter;

				// private methods
				private:

				// private variables
				private:


			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CONTOLOGYCONFIGDEPENDEDPREPROCESSER_H
