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

#include "CDefaultLoaderFactory.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CDefaultLoaderFactory::CDefaultLoaderFactory() {
			}


			CDefaultLoaderFactory::~CDefaultLoaderFactory() {
			}

			CLoader *CDefaultLoaderFactory::createLoaderFromName(const QString &loaderName, CConfiguration *config) {
				CLoader *loader = 0;
				if (loaderName == "CommandLineLoader") {
					loader = new CCommandLineLoader();
				} else if (loaderName == "CoutLogObserverLoader") {
					loader = new CCoutLogObserverLoader();
				} else if (loaderName == "DefaultReasonerLoader") {
					loader = new CDefaultReasonerLoader();
				} else if (loaderName == "OWLlinkBatchFileLoader") {
					loader = new COWLlinkBatchFileLoader();
				} else if (loaderName == "AnalyseReasonerLoader") {
					loader = new CAnalyseReasonerLoader();
				} else if (loaderName == "ExperimentalReasonerLoader") {
					loader = new CExperimentalReasonerLoader();
				} else if (loaderName == "CommandLineTranlatorHelpLoader") {
					loader = new CCommandLineTranlatorHelpLoader();
				} else if (loaderName == "ConfigurableCoutLogObserverLoader") {
					loader = new CConfigurableCoutLogObserverLoader();
				} else if (loaderName == "TestHashModificationLoader") {
					loader = new CTestHashModificationLoader();
				} else if (loaderName == "TestTaskThroughputLoader") {
					loader = new CTestTaskThroughputLoader();
				} else if (loaderName == "TestQHashVsQMapLoader") {
					loader = new CTestQHashVsQMapLoader();
				} else if (loaderName == "PigenholePrincipleGeneratorLoader") {
					loader = new CPigenholePrincipleGeneratorLoader();
				} else if (loaderName == "TestOWLlinkAPIBatchFileLoader") {
					loader = new CTestOWLlinkAPIBatchFileLoader();
				} else if (loaderName == "TestMemoryAccessLoader") {
					loader = new CTestMemoryAccessLoader();
				} else if (loaderName == "OWLFilesIncompatibleAxiomChangerLoader") {
					loader = new COWLFilesIncompatibleAxiomChangerLoader();
				} else if (loaderName == "ReasonerEvaluationLoader") {
					loader = new CReasonerEvaluationLoader();
				} else if (loaderName == "OWLFilesOWLlinkTestcaseGeneratorLoader") {
					loader = new COWLFilesOWLlinkTestcaseGeneratorLoader();
				} else if (loaderName == "ReasonerEvaluationClassificationTimesComparerLoader") {
					loader = new CReasonerEvaluationClassificationTimesComparerLoader();
				} else if (loaderName == "OWLlinkHttpServerLoader") {
					loader = new COWLlinkHttpServerLoader();
				} else if (loaderName == "ReasonerEvaluationLoadingTimesComparerLoader") {
					loader = new CReasonerEvaluationLoadingTimesComparerLoader();
				} else if (loaderName == "RecursionTestDataGeneratorLoader") {
					loader = new CRecursionTestDataGeneratorLoader();
				} else if (loaderName == "LargeJoinTestDataGeneratorLoader") {
					loader = new CLargeJoinTestDataGeneratorLoader();
				} else if (loaderName == "TransitiveConnectedELClassesTestDataGeneratorLoader") {
					loader = new CTransitiveConnectedELClassesTestDataGeneratorLoader();
				} else if (loaderName == "ReasonerEvaluationProgramLoader") {
					loader = new CReasonerEvaluationProgramLoader();
				} else if (loaderName == "OREBatchProcessingLoader") {
					loader = new COREBatchProcessingLoader();
				} else if (loaderName == "ORECoutLogObserverLoader") {
					loader = new CORECoutLogObserverLoader();
				} else if (loaderName == "ReasonerEvaluationTestcaseStatisticCollectionGeneratorLoader") {
					loader = new CReasonerEvaluationTestcaseStatisticCollectionGeneratorLoader();
				} else if (loaderName == "ReasonerEvaluationOntologyExtractionLoader") {
					loader = new CReasonerEvaluationOntologyExtractionLoader();
				} else if (loaderName == "CLIClassClassificationBatchProcessingLoader") {
					loader = new CCLIClassClassificationBatchProcessingLoader();
				} else if (loaderName == "CLIObjectPropertyClassificationBatchProcessingLoader") {
					loader = new CCLIObjectPropertyClassificationBatchProcessingLoader();
				} else if (loaderName == "CLIDataPropertyClassificationBatchProcessingLoader") {
					loader = new CCLIDataPropertyClassificationBatchProcessingLoader();
				} else if (loaderName == "CLIConsistencyBatchProcessingLoader") {
					loader = new CCLIConsistencyBatchProcessingLoader();
				} else if (loaderName == "CLISatisfiabilityBatchProcessingLoader") {
					loader = new CCLISatisfiabilityBatchProcessingLoader();
				} else if (loaderName == "CLIRealizationBatchProcessingLoader") {
					loader = new CCLIRealizationBatchProcessingLoader();
				} else if (loaderName == "LoggerConfigurationLoader") {
					loader = new CLoggerConfigurationLoader();
				} else if (loaderName == "JNICommandProcessorLoader") {
					loader = new CJNICommandProcessorLoader();
				} else if (loaderName == "SPARQLBatchFileLoader") {
					loader = new CSPARQLBatchFileLoader();
				} else if (loaderName == "CLILoadingBatchProcessingLoader") {
					loader = new CCLILoadingBatchProcessingLoader();
				} else if (loaderName == "SPARQLHttpServerLoader") {
					loader = new CSPARQLHttpServerLoader();
				} else if (loaderName == "SPARQLResultsComparisonLoader") {
					loader = new CSPARQLResultsComparisonLoader();
				}
				return loader;
			}

			bool CDefaultLoaderFactory::canCreateLoaderFromName(const QString &loaderName, CConfiguration *config) {
				bool canLoad = false;
				if (loaderName == "CommandLineLoader") {
					canLoad = true;
				} else if (loaderName == "CoutLogObserverLoader") {
					canLoad = true;
				} else if (loaderName == "DefaultReasonerLoader") {
					canLoad = true;
				} else if (loaderName == "OWLlinkBatchFileLoader") {
					canLoad = true;
				} else if (loaderName == "AnalyseReasonerLoader") {
					canLoad = true;
				} else if (loaderName == "ExperimentalReasonerLoader") {
					canLoad = true;
				} else if (loaderName == "CommandLineTranlatorHelpLoader") {
					canLoad = true;
				} else if (loaderName == "ConfigurableCoutLogObserverLoader") {
					canLoad = true;
				} else if (loaderName == "TestHashModificationLoader") {
					canLoad = true;
				} else if (loaderName == "TestTaskThroughputLoader") {
					canLoad = true;
				} else if (loaderName == "TestQHashVsQMapLoader") {
					canLoad = true;
				} else if (loaderName == "PigenholePrincipleGeneratorLoader") {
					canLoad = true;
				} else if (loaderName == "TestOWLlinkAPIBatchFileLoader") {
					canLoad = true;
				} else if (loaderName == "TestMemoryAccessLoader") {
					canLoad = true;
				} else if (loaderName == "OWLFilesIncompatibleAxiomChangerLoader") {
					canLoad = true;
				} else if (loaderName == "ReasonerEvaluationLoader") {
					canLoad = true;
				} else if (loaderName == "OWLFilesOWLlinkTestcaseGeneratorLoader") {
					canLoad = true;
				} else if (loaderName == "ReasonerEvaluationClassificationTimesComparerLoader") {
					canLoad = true;
				} else if (loaderName == "OWLlinkHttpServerLoader") {
					canLoad = true;
				} else if (loaderName == "ReasonerEvaluationLoadingTimesComparerLoader") {
					canLoad = true;
				} else if (loaderName == "RecursionTestDataGeneratorLoader") {
					canLoad = true;
				} else if (loaderName == "LargeJoinTestDataGeneratorLoader") {
					canLoad = true;
				} else if (loaderName == "TransitiveConnectedELClassesTestDataGeneratorLoader") {
					canLoad = true;
				} else if (loaderName == "ReasonerEvaluationProgramLoader") {
					canLoad = true;
				} else if (loaderName == "OREBatchProcessingLoader") {
					canLoad = true;
				} else if (loaderName == "ORECoutLogObserverLoader") {
					canLoad = true;
				} else if (loaderName == "ReasonerEvaluationTestcaseStatisticCollectionGeneratorLoader") {
					canLoad = true;
				} else if (loaderName == "ReasonerEvaluationOntologyExtractionLoader") {
					canLoad = true;
				} else if (loaderName == "CLIClassClassificationBatchProcessingLoader") {
					canLoad = true;
				} else if (loaderName == "CLIObjectPropertyClassificationBatchProcessingLoader") {
					canLoad = true;
				} else if (loaderName == "CLIDataPropertyClassificationBatchProcessingLoader") {
					canLoad = true;
				} else if (loaderName == "CLIConsistencyBatchProcessingLoader") {
					canLoad = true;
				} else if (loaderName == "CLISatisfiabilityBatchProcessingLoader") {
					canLoad = true;
				} else if (loaderName == "CLIRealizationBatchProcessingLoader") {
					canLoad = true;
				} else if (loaderName == "LoggerConfigurationLoader") {
					canLoad = true;
				} else if (loaderName == "JNICommandProcessorLoader") {
					canLoad = true;
				} else if (loaderName == "SPARQLBatchFileLoader") {
					canLoad = true;
				} else if (loaderName == "CLILoadingBatchProcessingLoader") {
					canLoad = true;
				} else if (loaderName == "SPARQLHttpServerLoader") {
					canLoad = true;
				} else if (loaderName == "SPARQLResultsComparisonLoader") {
					canLoad = true;
				}
				return canLoad;
			}



		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
