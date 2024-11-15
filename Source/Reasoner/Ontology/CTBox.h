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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CTBOX_H
#define KONCLUDE_REASONER_ONTOLOGY_CTBOX_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "CConcept.h"
#include "CBoxContext.h"
#include "CConcreteOntologyContextBase.h"
#include "CConceptVector.h"
#include "CDataBox.h"
#include "CIRIName.h"
#include "CIndividual.h"
#include "CDatatypeVector.h"
#include "CConceptOfInterestActivationTriggeringData.h"


// Other includes
#include "Utilities/CSortedNegLinker.hpp"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CTBox
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CTBox : public CDataBox {
				// public methods
				public:
					//! Constructor
					CTBox(CBoxContext* boxContext = nullptr);

					//! Destructor
					virtual ~CTBox();


					CTBox *setConceptVector(CConceptVector* takeConceptVec);
					CConceptVector* getConceptVector(bool create = true);

					qint64 getConceptCount();


					CTBox *referenceTBox(CTBox *tBox);
					CTBox *getTBoxReferenceCopy(CBoxContext* boxContext = nullptr);

					virtual CBoxContext *getBoxContext(bool create = true);

					CConcept* getTopDataRangeConcept();

					CConcept* getTopConcept();
					CConcept* getBottomConcept();
					CConcept* getIndividualTriggerConcept();

					CTBox* setTopConcept(CConcept* topConcept);
					CTBox* setBottomConcept(CConcept* bottomConcept);
					CTBox* setIndividualTriggerConcept(CConcept* indTrigConcept);
					CTBox* setTopDataRangeConcept(CConcept* topDataRangeConcept);

					QString getTBoxString();

					QString getConceptOperandString(CSortedNegLinker<CConcept *> *ops);

					CBOXSET<CConcept*> *getGCIConceptSet();
					CTBox *setGCIConceptSet(CBOXSET<CConcept*>* takeContainsSet);

					CBOXSET<CConcept*> *getActiveClassConceptSet(bool create = true);
					CTBox *setActiveClassConceptSet(CBOXSET<CConcept*>* takeActiveConceptSet);

					CBOXHASH<CConcept*,CConcept*>* getTriggerImplicationHash(bool create = true);
					CTBox* setTriggerImplicationHash(CBOXHASH<CConcept*,CConcept*>* takeTriggerImplHash);

					CBOXHASH<CConcept*,CConcept*>* getEquivalentConceptCandidateHash(bool create = true);
					CBOXHASH<CConcept*,CConcept*>* getCandidateEquivalentConceptHash(bool create = true);
					CBOXSET<CConcept*>* getEquivalentConceptNonCandidateSet(bool create = true);

					CBOXHASH<CRole*,CConcept*>* getRoleDomainTriggerConceptHash(bool create = true);

					CBOXHASH<cint64,CConcept*>* getIndividualTriggerConceptHash(bool create = true);


					CBOXSET<TConceptNegPair>* getConceptOfInterestSet(bool create = true);
					CBOXSET<TConceptNegPair>* getTriggeredConceptOfInterestSet(bool create = true);
					CBOXLIST<TConceptNegPair>* getConceptOfInterestList(bool create = true);
					CBOXHASH<TConceptNegPair, CConceptOfInterestActivationTriggeringData>* getConceptOfInterestActivationTriggerDataHash(bool create = true);
					CBOXHASH<CConcept*, TConceptNegPair>* getCandidateConceptOfInterestHash(bool create = true);
					CBOXHASH<CConcept*, TConceptNegPair>* getTriggerConceptOfInterestHash(bool create = true);


					cint64 getTopConceptIndex();
					cint64 getBottomConceptIndex();
					cint64 getIndividualTriggerConceptIndex();
					cint64 getTopDataRangeConceptIndex();

					CConcept* getUniversalConnectionNominalValueConcept();
					CTBox* setUniversalConnectionNominalValueConcept(CConcept* valueCon);



					CDatatypeVector* getDatatypeVector(bool create = true);
					CTBox* setDatatypeVector(CDatatypeVector* datatypeVector);

					CTBox* setMinimalNextConceptID(cint64 nextId);
					cint64 getMinimalNextConceptID();
					cint64 getNextConceptID();

				// protected methods
				protected:

				// protected variables
				protected:
					CConceptVector* concepts;
					CConcept* mUnivConnNomValueCon;

					CBoxContext *mBoxContext;
					CBoxContext *mDeleteBoxContext;
					CMemoryAllocationManager* mMemMan;

					CBOXSET<CConcept*> *gciConceptSet;
					CBOXSET<CConcept*> *activeConceptSet;
					CBOXHASH<CConcept*,CConcept*>* mTriggerImpHash;

					CDatatypeVector* mDatatypeVector;

					CBOXHASH<CConcept*,CConcept*>* mEquivConCandidateHash;
					CBOXHASH<CConcept*,CConcept*>* mCandidateEquivConHash;
					CBOXSET<CConcept*>* mEquivConNonCandidateSet;

					CBOXHASH<CRole*,CConcept*>* mRoleDomainTriggerConceptHash;
					CBOXHASH<cint64,CConcept*>* mIndividualTriggerConceptHash;


					CBOXLIST<TConceptNegPair>* mConceptOfInterestList;
					CBOXSET<TConceptNegPair>* mConceptOfInterestSet;
					CBOXSET<TConceptNegPair>* mTriggeredConceptOfInterestSet;
					CBOXHASH<TConceptNegPair, CConceptOfInterestActivationTriggeringData>* mConOfIntActivationTriggerHash;
					CBOXHASH<CConcept*, TConceptNegPair>* mCandidateConOfIntHash;
					CBOXHASH<CConcept*, TConceptNegPair>* mTriggerConOfIntHash;


					cint64 mTopConceptIndex;
					cint64 mBottomConceptIndex;
					cint64 mIndividualTriggerConceptIndex;
					cint64 mTopDataRangeConceptIndex;


					cint64 mMinimalNextConceptID;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CTBOX_H
