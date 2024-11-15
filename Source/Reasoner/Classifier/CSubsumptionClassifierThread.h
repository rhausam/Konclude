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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CSUBSUMPTIONCLASSIFIERTHREAD_H
#define KONCLUDE_REASONER_CLASSIFIER_CSUBSUMPTIONCLASSIFIERTHREAD_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "CSubsumptionClassifier.h"
#include "COntologyClassificationItem.h"
#include "CClassifyingCallbackDataContext.h"
#include "CClassSubsumptionClassifierContext.h"

// Other includes
#include "Reasoner/Classifier/Events/CClassifyOntologyEvent.h"
#include "Reasoner/Classifier/Events/CTestCalculatedCallbackEvent.h"
#include "Reasoner/Classifier/Events/CCallbackClassifiedOntologyEvent.h"
#include "Reasoner/Classifier/Events/COntologyTellClassificationMessageEvent.h"

#include "Reasoner/Ontology/CConcept.h"

#include "Reasoner/Kernel/Manager/CReasonerManager.h"


#include "Concurrent/CThread.h"

#include "Concurrent/Callback/CBlockingCallbackData.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Concurrent;
	using namespace Concurrent::Callback;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Kernel::Manager;
		using namespace Kernel::Calculation;

		namespace Classifier {

			using namespace Events;


			/*! 
			 *
			 *		\class		CSubsumptionClassifierThread
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CSubsumptionClassifierThread : public CSubsumptionClassifier, public CThread {
				// public methods
				public:
					//! Constructor
					CSubsumptionClassifierThread(CReasonerManager *reasonerManager);

					//! Destructor
					virtual ~CSubsumptionClassifierThread();

					virtual bool classify(CConcreteOntology *ontology, CConfigurationBase *config, const QList<COntologyProcessingRequirement*>& requirementList, CCallbackData* callback);
					virtual bool classify(CConcreteOntology *ontology, CConfigurationBase *config, const QList<COntologyProcessingRequirement*>& requirementList);
					virtual bool callbackClassified(CConcreteOntology *ontology, CCallbackData *callback);


					virtual CClassifierStatistics *getClassificationStatistics();

					virtual CClassificationProgress* getClassificationProgress();

					virtual CClassificationMessageDataObserver* tellClassificationMessage(CConcreteOntology *ontology, CClassificationMessageData* messageData, CMemoryPool* memoryPool);

					virtual bool isClassifierActive();

				// protected methods
				protected:
					virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);


					virtual bool isOntologyClassificationScheduled(CConcreteOntology *ontology);
					virtual CSubsumptionClassifierThread* scheduleOntologyClassification(CConcreteOntology *ontology, CClassificationCalculationSupport *classificationSupport, CConfigurationBase *config);
					virtual CSubsumptionClassifierThread* rescheduleOntologyClassification(CConcreteOntology *ontology);
					virtual CSubsumptionClassifierThread* addOntologyClassifiedCallback(CConcreteOntology *ontology, CCallbackData *callback);
					virtual CSubsumptionClassifierThread* addOntologyClassifyRequirements(CConcreteOntology *ontology, const QList<COntologyProcessingRequirement*>& requirementList);

					virtual bool createNextSubsumtionTest() = 0;

					virtual CSubsumptionClassifierThread* processCalculationJob(CSatisfiableCalculationJob* job, COntologyClassificationItem *ontClassItem, CCallbackData* callback, bool directlySubmit = true);
					virtual CSubsumptionClassifierThread* submitCalculationJobs();

					virtual bool interpreteTestResults(CTestCalculatedCallbackEvent *testResult) = 0;

					virtual bool startProcessMoreTests();
					virtual bool canProcessMoreTests();
					virtual bool doNextPendingTests();


					virtual bool processToldClassificationMessage(COntologyClassificationItem *ontClassItem, CClassificationMessageData* messageData, CMemoryPool* memoryPools);

				// protected variables
				protected:
					qint64 confMaxTestParallelCount;
					qint64 confMinTestParallelCount;

					qint64 currRunningTestParallelCount;

					QList<COntologyClassificationItem*> ontItemList;
					QList<COntologyClassificationItem*> processingOntItemList;
					QHash<CConcreteOntology*,COntologyClassificationItem*> ontItemHash;
					cint64 mClassificationCount;

					CReasonerManager* reasoner;
					CCalculationManager* calculationManager;
					QList< QPair<CCalculationJob*,CCallbackData*> > mJobCallbackList;

					CClassifierStatistics* statistics;

					cint64 mStatCalculatingJobs;

					CClassSubsumptionClassifierContext mContext;

					bool mClassifierActive;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CSUBSUMPTIONCLASSIFIERTHREAD_H
