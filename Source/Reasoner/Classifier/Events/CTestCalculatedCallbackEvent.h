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

#ifndef KONCLUDE_REASONER_CLASSIFIER_EVENTS_CTESTCALCULATEDUPDATEEVENT_H
#define KONCLUDE_REASONER_CLASSIFIER_EVENTS_CTESTCALCULATEDUPDATEEVENT_H



// Libraries includes


// Namespace includes
#include "../ClassifierSettings.h"
#include "../CClassificationComputationItem.h"


// Other includes
#include "Concurrent/Events/CCustomEvent.h"

#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Reasoner/Taxonomy/CTaxonomy.h"

#include "Concurrent/Callback/CCallbackData.h"

#include "Reasoner/Query/CSatisfiableCalculationJob.h"
#include "Reasoner/Query/CJobSatisfiableCallbackContextData.h"

#include "Reasoner/Classification/CClassificationCalculationStatisticsCollection.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent::Events;
	using namespace Concurrent::Callback;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Taxonomy;
		using namespace Classification;
		using namespace Kernel::Task;

		namespace Classifier {

			namespace Events {



				/*! 
				 *	\class		CTestCalculatedCallbackEvent
				 *	\author		Andreas Steigmiller
				 *	\version	0.1
				 *	\brief		
				 */
				class CTestCalculatedCallbackEvent : public CCustomEvent, public CJobSatisfiableCallbackContextData, public CCallbackData {
					// public methods
					public:
						//! Constructor
						CTestCalculatedCallbackEvent(CThread *receiverThread, CSatisfiableCalculationJob *satCalcJob, CClassificationComputationItem *classBoxWorkItem, CClassificationCalculationStatisticsCollection* statisticCollection);

						//! Destructor
						virtual ~CTestCalculatedCallbackEvent();

						static const QEvent::Type EVENTTYPE = EVENTTESTCALCULATEDCALLBACK;

						virtual void doCallback();

						virtual bool getTestResultSatisfiable();
						virtual CSatisfiableCalculationJob *getSatisfiableCalculationJob();

						virtual CClassificationComputationItem *getClassificationWorkItem();

						CClassificationCalculationStatisticsCollection* getUsedStatisticsCollection();

					// protected methods
					protected:

					// protected variables
					protected:
						CThread *recThread;
						CSatisfiableCalculationJob* mSatCalcJob;
						CClassificationComputationItem *workItem;
						CClassificationCalculationStatisticsCollection* mUsedStatisticsCollection;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Events

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_EVENTS_CTESTCALCULATEDUPDATEEVENT_H
