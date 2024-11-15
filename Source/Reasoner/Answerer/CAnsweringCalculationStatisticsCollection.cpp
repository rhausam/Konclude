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

#include "CAnsweringCalculationStatisticsCollection.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CAnsweringCalculationStatisticsCollection::CAnsweringCalculationStatisticsCollection() : CLinkerBase<CAnsweringCalculationStatisticsCollection*,CAnsweringCalculationStatisticsCollection>(this) {
			}


			CAnsweringCalculationStatisticsCollection::~CAnsweringCalculationStatisticsCollection() {
			}


			bool CAnsweringCalculationStatisticsCollection::addProcessingStatistics(const QString& statName, cint64 statValue) {
				cint64 prevValue = mStatNameStringsValueHash.value(statName,0);
				mStatNameStringsValueHash.insert(statName,statValue+prevValue);
				return true;
			}


			CAnsweringCalculationStatisticsCollection* CAnsweringCalculationStatisticsCollection::appendStatisticsStringNames(QSet<QString>* statNamesSet) {
				if (statNamesSet) {
					foreach (QString statName, mStatNameStringsValueHash.keys()) {
						statNamesSet->insert(statName);
					}
				} 
				return this;
			}

			cint64 CAnsweringCalculationStatisticsCollection::getStatisticIntegerValue(const QString& statName) {
				return mStatNameStringsValueHash.value(statName,0);
			}

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
