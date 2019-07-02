/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_CLASSIFIER_EVENTS_CONTOLOGYTELLCONCEPTPOSSIBLESUBSUMPTIONSOCCURRENCEEVENT_H
#define KONCLUDE_REASONER_CLASSIFIER_EVENTS_CONTOLOGYTELLCONCEPTPOSSIBLESUBSUMPTIONSOCCURRENCEEVENT_H



// Libraries includes


// Namespace includes
#include "../ClassifierSettings.h"
#include "../CConceptNegationPair.h"


// Other includes
#include "Concurrent/Events/CCustomEvent.h"

#include "Reasoner/Ontology/CConcreteOntology.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent::Events;

	namespace Reasoner {

		using namespace Ontology;

		namespace Classifier {

			namespace Events {



				/*! 
				 *	\class		COntologyTellConceptPossibleSubsumptionsOccurrenceEvent
				 *	\author		Andreas Steigmiller
				 *	\version	0.1
				 *	\brief		
				 */
				class COntologyTellConceptPossibleSubsumptionsOccurrenceEvent : public CCustomEvent {
					// public methods
					public:
						//! Constructor
						COntologyTellConceptPossibleSubsumptionsOccurrenceEvent(CConcreteOntology *ontology, CConcept *concept, const QList<CConcept*>& possConceptSubsumList);

						//! Destructor
						virtual ~COntologyTellConceptPossibleSubsumptionsOccurrenceEvent();

						CConcreteOntology *getOntology();

						static const QEvent::Type EVENTTYPE = EVENTONTOLOGYTELLCONPOSSSUBSUM;

						CConcept *getConcept();
						QList<CConcept*> getPossibleConceptSubsumptionOccurenceList();

					// protected methods
					protected:

					// protected variables
					protected:
						CConcreteOntology* mOnto;
						QList<CConcept*> mPossConceptSubsumList;
						CConcept* mCon;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Events

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_EVENTS_CONTOLOGYTELLCONCEPTPOSSIBLESUBSUMPTIONSOCCURRENCEEVENT_H