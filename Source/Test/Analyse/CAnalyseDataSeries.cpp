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

#include "CAnalyseDataSeries.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {


			CAnalyseDataSeries::CAnalyseDataSeries() {
				dataLinker = 0;
			}


			CAnalyseDataSeries::~CAnalyseDataSeries() {
			}


			CAnalyseDataLinker *CAnalyseDataSeries::getAnalyseDataLinker() {
				return dataLinker;
			}


			CAnalyseDataSeries *CAnalyseDataSeries::addAnalyseData(CAnalyseDataLinker *analyseDataLinker) {
				dataLinker = (CAnalyseDataLinker *)analyseDataLinker->getLastListLink()->insertNext(dataLinker);
				return this;
			}

			CAnalyseDataSeries *CAnalyseDataSeries::setAnalyseDataDescription(CAnalyseDataDescription *dataDescription) {
				dataDes = dataDescription;
				return this;
			}

			CAnalyseDataDescription *CAnalyseDataSeries::getAnalyseDataDescription() {
				return dataDes;
			}



		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
