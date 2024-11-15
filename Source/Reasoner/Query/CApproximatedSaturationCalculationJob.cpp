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

#include "CApproximatedSaturationCalculationJob.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CApproximatedSaturationCalculationJob::CApproximatedSaturationCalculationJob() {
				mQueryData = nullptr;
				mSatCalcConstructionConstructLinker = nullptr;
				mSatCalcProcessingConstructLinker = nullptr;
				mCalculationConfiguration = nullptr;
				mCalclulationStatisticsCollector = nullptr;
				mPreyingAdapter = nullptr;
				mContinueSaturationData = nullptr;
				mSeparateSaturation = false;
				mIndiAnalAdapter = nullptr;
				mOccurrenceStatisticsCollectingAdapter = nullptr;
			}


			CApproximatedSaturationCalculationJob::~CApproximatedSaturationCalculationJob() {
				delete mQueryData;
				CApproximatedSaturationCalculationConstructionConstruct* satCalcConstConstructLinkerIt = mSatCalcConstructionConstructLinker;
				while (satCalcConstConstructLinkerIt) {
					CApproximatedSaturationCalculationConstructionConstruct* tmpLinker = satCalcConstConstructLinkerIt;
					satCalcConstConstructLinkerIt = satCalcConstConstructLinkerIt->getNext();
					delete tmpLinker;
				}
				CApproximatedSaturationCalculationProcessingConstruct* satCalcProcConstructLinkerIt = mSatCalcProcessingConstructLinker;
				while (satCalcProcConstructLinkerIt) {
					CApproximatedSaturationCalculationProcessingConstruct* tmpLinker = satCalcProcConstructLinkerIt;
					satCalcProcConstructLinkerIt = satCalcProcConstructLinkerIt->getNext();
					delete tmpLinker;
				}
			}

			CCalculationJobQueryData* CApproximatedSaturationCalculationJob::getQueryData() {
				return mQueryData;
			}

			CApproximatedSaturationCalculationJob* CApproximatedSaturationCalculationJob::setQueryData(CCalculationJobQueryData* takeQueryData) {
				delete mQueryData;
				mQueryData = takeQueryData;
				return this;
			}

			CApproximatedSaturationCalculationJob* CApproximatedSaturationCalculationJob::addSatisfiableCalculationConstructionConstruct(CApproximatedSaturationCalculationConstructionConstruct* takeSatCalcConstruct) {
				if (takeSatCalcConstruct) {
					mSatCalcConstructionConstructLinker = takeSatCalcConstruct->append(mSatCalcConstructionConstructLinker);
				}
				return this;
			}

			CApproximatedSaturationCalculationConstructionConstruct* CApproximatedSaturationCalculationJob::getSatisfiableCalculationConstructionConstructs() {
				return mSatCalcConstructionConstructLinker;
			}



			CApproximatedSaturationCalculationJob* CApproximatedSaturationCalculationJob::addSatisfiableCalculationProcessingConstruct(CApproximatedSaturationCalculationProcessingConstruct* takeSatCalcConstruct) {
				if (takeSatCalcConstruct) {
					mSatCalcProcessingConstructLinker = takeSatCalcConstruct->append(mSatCalcProcessingConstructLinker);
				}
				return this;
			}

			CApproximatedSaturationCalculationProcessingConstruct* CApproximatedSaturationCalculationJob::getSatisfiableCalculationProcessingConstructs() {
				return mSatCalcProcessingConstructLinker;
			}

			CApproximatedSaturationCalculationJob* CApproximatedSaturationCalculationJob::setCalculationConfiguration(CCalculationConfigurationExtension *calculationConfiguration) {
				mCalculationConfiguration = calculationConfiguration;
				return this;
			}

			CCalculationConfigurationExtension* CApproximatedSaturationCalculationJob::getCalculationConfiguration() {
				return mCalculationConfiguration;
			}

			CApproximatedSaturationCalculationJob* CApproximatedSaturationCalculationJob::setCalclulationStatisticsCollector(CCalculationStatisticsCollector* calclulationStatisticsCollector) {
				mCalclulationStatisticsCollector = calclulationStatisticsCollector;
				return this;
			}

			CCalculationStatisticsCollector* CApproximatedSaturationCalculationJob::getCalclulationStatisticsCollector() {
				return mCalclulationStatisticsCollector;
			}

			CApproximatedSaturationCalculationJob* CApproximatedSaturationCalculationJob::setSaturationTaskPreyingAdapter(CTaskPreyingAdapter* preyingAdapter) {
				mPreyingAdapter = preyingAdapter;
				return this;
			}

			CTaskPreyingAdapter* CApproximatedSaturationCalculationJob::getSaturationTaskPreyingAdapter() {
				return mPreyingAdapter;
			}


			CSaturationData* CApproximatedSaturationCalculationJob::getContinueSaturationData() {
				return mContinueSaturationData;
			}

			CApproximatedSaturationCalculationJob* CApproximatedSaturationCalculationJob::setContinueSaturationData(CSaturationData* saturationData) {
				mContinueSaturationData = saturationData;
				return this;
			}

			bool CApproximatedSaturationCalculationJob::isSeparateSaturation() {
				return mSeparateSaturation;
			}

			CApproximatedSaturationCalculationJob* CApproximatedSaturationCalculationJob::setSeparateSaturation(bool separateSaturation) {
				mSeparateSaturation = separateSaturation;
				return this;
			}

			CApproximatedSaturationCalculationJob* CApproximatedSaturationCalculationJob::setSaturationIndividualsAnalysationObserver(CSaturationIndividualsAnalysingAdapter* indiAnalAdapter) {
				mIndiAnalAdapter = indiAnalAdapter;
				return this;
			}

			CSaturationIndividualsAnalysingAdapter* CApproximatedSaturationCalculationJob::getSaturationIndividualsAnalysationObserver() {
				return mIndiAnalAdapter;
			}


			CApproximatedSaturationCalculationJob* CApproximatedSaturationCalculationJob::setOccurrenceStatisticsCollectingAdapter(CSaturationOccurrenceStatisticsCollectingAdapter* collAdapter) {
				mOccurrenceStatisticsCollectingAdapter = collAdapter;
				return this;
			}

			CSaturationOccurrenceStatisticsCollectingAdapter* CApproximatedSaturationCalculationJob::getOccurrenceStatisticsCollectingAdapter() {
				return mOccurrenceStatisticsCollectingAdapter;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
