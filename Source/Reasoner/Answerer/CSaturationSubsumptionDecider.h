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

#ifndef KONCLUDE_REASONER_ANSWERER_CSATURATIONSUBSUMPTIONDECIDER_H
#define KONCLUDE_REASONER_ANSWERER_CSATURATIONSUBSUMPTIONDECIDER_H

// Libraries includes
#include <QHash>
#include <QList>
#include <QSet>
#include <QPair>
#include <QVarLengthArray>

// Namespace includes
#include "AnswererSettings.h"

// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CRole.h"
#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptSaturationReferenceLinkingData.h"
#include "Reasoner/Kernel/Task/CSaturationTaskData.h"
#include "Reasoner/Kernel/Task/CSatisfiableCalculationTask.h"
#include "Reasoner/Kernel/Process/CIndividualSaturationProcessNode.h"
#include "Reasoner/Kernel/Process/CReapplyConceptSaturationLabelSet.h"
#include "Reasoner/Kernel/Process/CLinkedRoleSaturationSuccessorHash.h"
#include "Reasoner/Consistiser/CSaturationConceptDataItem.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;
		using namespace Kernel::Task;
		using namespace Kernel::Process;
		using namespace Consistiser;

		namespace Answerer {

			/*! 
			 *
			 *		\class		CSaturationSubsumptionDecider
			 *		\author		Konclude Developer Team
			 *		\version	0.7.0
			 *		\brief		Decides from the precomputed saturation alone whether a named class is subsumed
			 *					by a query concept, so that the sub class search of the complex expression
			 *					answering handler does not have to create a tableau job for every node it
			 *					visits. The saturation of a named class is complete for the deterministic
			 *					part of the ontology and is what the classifier reads its subsumers from;
			 *					here the structure of the query concept is matched against it. Whenever the
			 *					saturation of a node is marked insufficient, critical or unprocessed, or the
			 *					concept leaves the fragment that can be matched, the verdict is UNDECIDED and
			 *					the caller creates the job as before, so a verdict is never a guess.
			 *
			 */
			class CSaturationSubsumptionDecider {
				// public methods
				public:
					enum Verdict {
						SUBSUMED,
						NOT_SUBSUMED,
						UNDECIDED
					};

					//! Constructor, for the ontology whose precomputation holds the saturation
					CSaturationSubsumptionDecider(CConcreteOntology* ontology);
					~CSaturationSubsumptionDecider();

					//! whether the ontology has a saturation to decide from at all
					bool hasSaturation();

					/*!
					 *	Whether 'subClassConcept', a named class of the ontology, is subsumed by the query
					 *	concept, negated if 'queryNegation' is set. SUBSUMED and NOT_SUBSUMED are only
					 *	answered when the saturation warrants them.
					 */
					Verdict decideSubClass(CConcept* subClassConcept, CConcept* queryConcept, bool queryNegation);

					cint64 getSubsumedCount();
					cint64 getNotSubsumedCount();
					cint64 getUndecidedCount();
					//! the operator codes of the label concepts that kept a merge from being decided, with their counts
					QHash<cint64,cint64>* getUndecidedMergeConceptCodeCounts();
					cint64 getFiredImplicationCount();
					cint64 getFastDecisionCount();
					cint64 getClosureDecisionCount();

				// protected methods
				protected:
					/*!
					 *	The saturation node that represents the concept, null if there is none. 'unsatisfiable'
					 *	reports a clashed saturation, 'reliable' whether the saturation is complete enough
					 *	to draw a negative conclusion from, both read as the classifier reads them.
					 */
					CIndividualSaturationProcessNode* getSaturationNode(CConcept* concept, bool* unsatisfiable, bool* reliable);
					//! the node at the end of the substitute chain, which holds the label and the successors
					CIndividualSaturationProcessNode* getRepresentativeNode(CIndividualSaturationProcessNode* baseNode);
					bool isReliable(CIndividualSaturationProcessNode* baseNode);
					//! whether the concept is in the label of the node, following the substitute chain as the classifier does
					bool hasLabelConcept(CIndividualSaturationProcessNode* baseNode, CConcept* concept, bool negated);

					//! whether every instance of the node's concept satisfies the concept, negated if 'negated'
					Verdict decideEntailed(CIndividualSaturationProcessNode* baseNode, bool reliable, CConcept* concept, bool negated, cint64 depth);
					//! for a negated query: whether the node's concept and the named conjuncts have a common instance
					Verdict decideConjunctionSatisfiable(CIndividualSaturationProcessNode* baseNode, bool reliable, const QList<CConcept*>& conjuncts);
					//! the named classes of a conjunction, false if it contains something else
					bool collectNamedConjuncts(CConcept* concept, bool negated, QList<CConcept*>& conjuncts, cint64 depth);
					enum LabelConceptKind {
						LCK_SAFE,
						LCK_CONJUNCTION,
						LCK_IMPLICATION,
						LCK_UNIVERSAL,
						LCK_UNSAFE
					};
					//! what a concept of a saturated label can do when the label is merged with another one
					LabelConceptKind getLabelConceptKind(CConcept* concept, bool negated);
					//! a universal restriction of the merged label, with the part it came from, null if derived
					class CMergeUniversal {
						public:
							CMergeUniversal(CConcept* concept = nullptr, bool negated = false, CIndividualSaturationProcessNode* sourceNode = nullptr) : mConcept(concept), mNegated(negated), mSourceNode(sourceNode) {}
							CConcept* mConcept;
							bool mNegated;
							CIndividualSaturationProcessNode* mSourceNode;
					};
					/*!
					 *	The merged label of a conjunction while it is closed under the rules of the saturation.
					 *	The side of the query, which is the same for every node the search visits, is built
					 *	once and shared as the base of the states of the nodes, which hold what the node adds.
					 */
					class CMergeState {
						public:
							CMergeState(const CMergeState* base = nullptr) : mBase(base), mClashed(false), mCurrentPartNode(nullptr) {}
							bool hasPositive(CConcept* concept) const { return mPositiveSet.contains(concept) || (mBase && mBase->hasPositive(concept)); }
							bool hasNegative(CConcept* concept) const { return mNegativeSet.contains(concept) || (mBase && mBase->hasNegative(concept)); }
							bool hasPartNode(CIndividualSaturationProcessNode* node) const { return mPartNodeSet.contains(node) || (mBase && mBase->hasPartNode(node)); }
							bool hasSuccessorRole(CRole* role) const { return mSuccessorRoleSet.contains(role) || (mBase && mBase->hasSuccessorRole(role)); }
							const CMergeState* mBase;
							QSet<CConcept*> mPositiveSet;
							QSet<CConcept*> mNegativeSet;
							QList<CConcept*> mImplicationList;
							QList<CMergeUniversal> mUniversalList;
							QSet<CConcept*> mSelfConceptSet;
							QSet<CRole*> mSuccessorRoleSet;
							QSet<CRole*> mDerivedSuccessorRoleSet;
							//! for a base: the implications that wait for a trigger the base lacks, by that trigger
							QHash<CConcept*,QList<CConcept*> > mTriggerImplicationHash;
							//! for a base: the tags of the conjuncts it was built for
							QList<cint64> mConjunctTagList;
							QList<CIndividualSaturationProcessNode*> mPartNodeList;
							QSet<CIndividualSaturationProcessNode*> mPartNodeSet;
							bool mClashed;
							CIndividualSaturationProcessNode* mCurrentPartNode;
					};
					//! closes the state under the implications, false if the merge cannot be decided
					bool closeMergeState(CMergeState& state);
					//! the shared side of a conjunction of named classes, null if it cannot be decided
					const CMergeState* getConjunctionMergeBase(const QList<CConcept*>& conjuncts);
					//! decides the merge of the node with the base by scanning the node's label only, FALLBACK if an implication would fire
					enum FastVerdict { FAST_SUBSUMED, FAST_NOT_SUBSUMED, FAST_UNDECIDED, FAST_FALLBACK };
					FastVerdict decideConjunctionSatisfiableFast(CIndividualSaturationProcessNode* baseNode, const CMergeState* base);
					//! decides the merge by closing the union of the labels under the rules of the saturation
					Verdict decideConjunctionSatisfiableClosure(CIndividualSaturationProcessNode* baseNode, const CMergeState* base);
					bool addMergeLabelConcept(CMergeState& state, CConcept* concept, bool negated, bool derived);
					bool addMergeNamedClass(CMergeState& state, CConcept* concept);
					//! whether a self restriction makes a node its own successor under the role
					bool isSelfLoopOnRole(CConcept* selfConcept, CRole* role);
					//! whether the node has an active successor under the role whose label lacks the concept
					bool hasSuccessorWithoutConcept(CIndividualSaturationProcessNode* repNode, CRole* role, CConcept* concept, bool negated);
					bool isSuccessorCreatingRole(CRole* role);

				// protected variables
				protected:
					CConcreteOntology* mOntology;
					CSatisfiableCalculationTask* mSatCalcTask;

					cint64 mSubsumedCount;
					cint64 mNotSubsumedCount;
					cint64 mUndecidedCount;
					QHash<cint64,cint64> mUndecidedMergeConceptCodeCounts;
					cint64 mFiredImplicationCount;
					//! the shared sides of the conjunctions asked about, keyed by the tags of their classes
					QHash<quint64,CMergeState*> mConjunctionMergeBaseHash;
					cint64 mFastDecisionCount;
					cint64 mClosureDecisionCount;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CSATURATIONSUBSUMPTIONDECIDER_H
