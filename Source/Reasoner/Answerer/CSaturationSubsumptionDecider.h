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
#include "Reasoner/Taxonomy/CHierarchyNode.h"
#include "Reasoner/Taxonomy/CTaxonomy.h"
#include "Reasoner/Classification/CClassification.h"
#include "Reasoner/Classification/CClassConceptClassification.h"

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
						UNDECIDED,
						//! the class named by getCompletionConcept has no reliable saturation; once its label is completed, see setCompletedLabel, the decision can be made
						NEEDS_COMPLETION
					};

					//! one concept of a completed label, see setCompletedLabel
					class CCompletedLabelEntry {
						public:
							CCompletedLabelEntry() : mConcept(nullptr), mNegated(false), mDeterministic(false) {}
							CCompletedLabelEntry(CConcept* concept, bool negated, bool deterministic) : mConcept(concept), mNegated(negated), mDeterministic(deterministic) {}
							CConcept* mConcept;
							bool mNegated;
							bool mDeterministic;
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

					//! after NEEDS_COMPLETION: the class whose label has to be completed
					CConcept* getCompletionConcept();
					//! the classes above the class in the class hierarchy, whose consequences a completion test has to include
					QList<CConcept*> getHierarchyAncestors(CConcept* concept);
					/*!
					 *	The label of the root node of a satisfiable completion graph of the class, which stands
					 *	in for its saturation from now on. An entry that was derived with a choice holds in the
					 *	model the tableau found, not necessarily in every model, so it can show a conjunction
					 *	satisfiable but not unsatisfiable.
					 */
					void setCompletedLabel(CConcept* concept, const QVector<CCompletedLabelEntry>& entries);
					//! the class was found unsatisfiable, so it is subsumed by everything
					void setCompletedLabelUnsatisfiable(CConcept* concept);
					//! no label could be obtained, the class stays undecided
					void setCompletionFailed(CConcept* concept);
					bool hasCompletedLabel(CConcept* concept);
					//! whether a universal restriction of a query that reaches the successors of a completed label is decided from the saturation of the label's existential restrictions (Konclude.Answering.SaturationBasedSubClassLabelCompletionSuccessors)
					void setCompletedSuccessorDecision(bool enabled);
					//! whether an at-most-one restriction over a functional role may be merged like any safe concept, see getLabelConceptKind
					void setFunctionalAtMostSafe(bool enabled);
					//! whether the successor two sides share over a functional role is decided from the saturations of its fillers, see decideMergedFunctionalSuccessor
					void setFunctionalSuccessorMerge(bool enabled);

					cint64 getSubsumedCount();
					cint64 getNotSubsumedCount();
					cint64 getUndecidedCount();
					cint64 getCompletionRequestCount();
					cint64 getCompletedDecisionCount();
					//! how often the completed label of a sub class candidate decided it, and how often it left it undecided,
					//! counted only for the queries for which a reliable saturation decided some candidate: for a query that
					//! the saturation cannot decide at all, such as an existential restriction over a plain EL ontology, a
					//! completed label cannot help either, and its candidates say nothing about the labels' worth (issue #33)
					cint64 getCompletedLabelDecisiveCount();
					cint64 getCompletedLabelUndecidedCount();
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
					CIndividualSaturationProcessNode* getSaturationNode(CConcept* concept, bool* unsatisfiable, bool* reliable, bool negated = false);
					//! the node at the end of the substitute chain, which holds the label and the successors
					CIndividualSaturationProcessNode* getRepresentativeNode(CIndividualSaturationProcessNode* baseNode);
					bool isReliable(CIndividualSaturationProcessNode* baseNode);
					//! whether the concept is in the label of the node, following the substitute chain as the classifier does
					bool hasLabelConcept(CIndividualSaturationProcessNode* baseNode, CConcept* concept, bool negated);

					//! whether every instance of the node's concept satisfies the concept, negated if 'negated'
					Verdict decideEntailed(CIndividualSaturationProcessNode* baseNode, bool reliable, CConcept* concept, bool negated, cint64 depth);
					bool isSubRoleOf(CRole* subRole, CRole* superRole);
					Verdict decideExistentialFromLabel(CIndividualSaturationProcessNode* baseNode, CRole* role, CConcept* filler, bool fillerNegation, cint64 depth);
					Verdict decideExistentialFromLabelUncached(CIndividualSaturationProcessNode* baseNode, CRole* role, CConcept* filler, bool fillerNegation, cint64 depth);
					Verdict decideEntailedExistential(CIndividualSaturationProcessNode* baseNode, bool reliable, CConcept* concept, bool conceptNegated, CRole* role, CConcept* filler, bool fillerNegation, cint64 depth);
					//! for a negated query: whether the node's concept and the named conjuncts have a common instance
					Verdict decideConjunctionSatisfiable(CIndividualSaturationProcessNode* baseNode, bool reliable, const QList<CConcept*>& conjuncts);
					//! the named classes of a conjunction, false if it contains something else
					bool collectNamedConjuncts(CConcept* concept, bool negated, QList<CConcept*>& conjuncts, cint64 depth);
					enum LabelConceptKind {
						LCK_SAFE,
						LCK_CONJUNCTION,
						LCK_IMPLICATION,
						LCK_UNIVERSAL,
						//! an equivalence candidate: a named class the node may turn out to belong to
						LCK_CANDIDATE,
						LCK_UNSAFE
					};
					//! what a concept of a saturated label can do when the label is merged with another one
					LabelConceptKind getLabelConceptKind(CConcept* concept, bool negated);
					//! a universal restriction of the merged label, with the part it came from, null if derived
					class CCompletedLabel;

					//! an existential restriction of a completed label merged into a state, kept for the merged successors over functional roles
					class CMergeExistential {
						public:
							CMergeExistential(CConcept* concept = nullptr, bool negated = false, bool deterministic = false) : mConcept(concept), mNegated(negated), mDeterministic(deterministic) {}
							CConcept* mConcept;
							bool mNegated;
							bool mDeterministic;
					};

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
							CMergeState(const CMergeState* base = nullptr) : mBase(base), mClashed(false), mCurrentPartNode(nullptr), mHasNondeterministic(false), mOverlayLabelSet(nullptr), mOverlayPolarityHash(nullptr), mOverlayEntries(nullptr) {}
							/*!
							 *	The overlay is the label of the node the merge is about, consulted in place instead
							 *	of being copied into the sets: the saturated label with the concepts of its substitute
							 *	chain, or the polarity hash of a completed label.
							 */
							bool hasOverlay(CConcept* concept, bool negated) const {
								if (mOverlayLabelSet && mOverlayLabelSet->hasConcept(concept, negated)) {
									return true;
								}
								if (!negated) {
									for (CConcept* substitute : mOverlaySubstituteArray) {
										if (substitute == concept) {
											return true;
										}
									}
								}
								return mOverlayPolarityHash && (mOverlayPolarityHash->value(concept, 0) & (negated ? 2 : 1)) != 0;
							}
							bool hasPositive(CConcept* concept) const { return mPositiveSet.contains(concept) || hasOverlay(concept, false) || (mBase && mBase->hasPositive(concept)); }
							bool hasNegative(CConcept* concept) const { return mNegativeSet.contains(concept) || hasOverlay(concept, true) || (mBase && mBase->hasNegative(concept)); }
							//! as hasPositive and hasNegative, without the overlay, for the clash check of the overlay's own concepts
							bool hasPositiveBesidesOverlay(CConcept* concept) const { return mPositiveSet.contains(concept) || (mBase && mBase->hasPositive(concept)); }
							bool hasNegativeBesidesOverlay(CConcept* concept) const { return mNegativeSet.contains(concept) || (mBase && mBase->hasNegative(concept)); }
							bool hasPartNode(CIndividualSaturationProcessNode* node) const { return mPartNodeSet.contains(node) || (mBase && mBase->hasPartNode(node)); }
							bool hasSuccessorRole(CRole* role) const { return mSuccessorRoleSet.contains(role) || (mBase && mBase->hasSuccessorRole(role)); }
							//! whether a part came from a completed label with an entry that rests on a choice, so that a clash is no proof
							bool hasNondeterministic() const { return mHasNondeterministic || (mBase && mBase->hasNondeterministic()); }
							//! whether a part from a completed label has a successor whose role, or a super role of it, has the tag
							bool hasCompletedSuccessorRoleTag(cint64 roleTag) const { return mCompletedSuccessorRoleTagSet.contains(roleTag) || (mBase && mBase->hasCompletedSuccessorRoleTag(roleTag)); }
							bool hasDataRoleTag(cint64 roleTag) const { return mDataRoleTagSet.contains(roleTag) || (mBase && mBase->hasDataRoleTag(roleTag)); }
							//! whether a part has a self loop on the role with the tag, or on a sub role of it
							bool hasSelfRoleTag(cint64 roleTag) const { return mSelfRoleTagSet.contains(roleTag) || (mBase && mBase->hasSelfRoleTag(roleTag)); }
							const CMergeState* mBase;
							QSet<CConcept*> mPositiveSet;
							QSet<CConcept*> mNegativeSet;
							QList<CConcept*> mImplicationList;
							QList<CMergeUniversal> mUniversalList;
							QSet<CConcept*> mSelfConceptSet;
							//! the roles of the self loops of the parts, with their super roles
							QSet<cint64> mSelfRoleTagSet;
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
							bool mHasNondeterministic;
							CReapplyConceptSaturationLabelSet* mOverlayLabelSet;
							QVarLengthArray<CConcept*,4> mOverlaySubstituteArray;
							const QHash<CConcept*,quint8>* mOverlayPolarityHash;
							const QVector<CCompletedLabelEntry>* mOverlayEntries;
							//! the named classes of the equivalence candidates of the parts, see closeMergeStateWithCandidates
							QList<CConcept*> mCandidateList;
							QSet<CConcept*> mAddedCandidateSet;
							//! the roles of the successors of the parts that came from completed labels, with their super roles
							QSet<cint64> mCompletedSuccessorRoleTagSet;
							//! the data roles with successors, of every part
							QSet<cint64> mDataRoleTagSet;
							//! the existential and at-least restrictions of the parts that came from completed labels
							QList<CMergeExistential> mExistentialList;
							//! the completed labels the parts came from
							QList<CCompletedLabel*> mCompletedLabelList;
					};

					//! the completed label of a class, see setCompletedLabel
					class CCompletedLabel {
						public:
							CCompletedLabel() : mConcept(nullptr), mHasNondeterministic(false), mUnsatisfiable(false), mFailed(false), mExtended(false), mExtensionFailed(false), mExtending(false) {}
							CConcept* mConcept;
							bool hasEntry(CConcept* concept, bool negated) const { return (mPolarityHash.value(concept, 0) & (negated ? 2 : 1)) != 0; }
							bool hasDeterministicEntry(CConcept* concept, bool negated) const { return (mPolarityHash.value(concept, 0) & (negated ? 8 : 4)) != 0; }
							QVector<CCompletedLabelEntry> mEntries;
							//! bit 1 positive, 2 negative, 4 positive without a choice, 8 negative without a choice
							QHash<CConcept*,quint8> mPolarityHash;
							bool mHasNondeterministic;
							bool mUnsatisfiable;
							bool mFailed;
							//! whether the label was closed under the rules with its equivalence candidates added, see extendCompletedLabel
							bool mExtended;
							bool mExtensionFailed;
							bool mExtending;
							//! the roles of the existential restrictions, and the tags of these roles with their super roles
							QList<CRole*> mSuccessorRoleList;
							QSet<cint64> mSuccessorRoleTagSet;
							QSet<cint64> mDataRoleTagSet;
					};
					//! closes the state under the implications, false if the merge cannot be decided
					bool closeMergeState(CMergeState& state);
					/*!
					 *	Adds the classes of the equivalence candidates of all parts and closes again, until
					 *	none is left: a merge that stays satisfiable with a class added is satisfiable without
					 *	it, so a merge is only called satisfiable once its candidates are in.
					 */
					bool closeMergeStateWithCandidates(CMergeState& state);
					//! the shared side of a conjunction of named classes, null if it cannot be decided
					const CMergeState* getConjunctionMergeBase(const QList<CConcept*>& conjuncts);
					//! builds the merge of the named conjuncts into the state, false if it cannot be decided
					bool fillConjunctionMergeBase(CMergeState* base, const QList<CConcept*>& conjuncts);
					//! the functional roles over which the label and the base both have successors (or a self loop), decided from the merged successors where possible
					Verdict decideSuccessorRoleConflicts(CCompletedLabel* label, const CMergeState* other, bool nondeterministic, QList<QPair<CConcept*,bool> >& backPropagated);
					//! the successor the label and the base share over a functional role: SUBSUMED if its merged label clashes, NOT_SUBSUMED if it is satisfiable, UNDECIDED otherwise
					Verdict decideMergedFunctionalSuccessor(CCompletedLabel* label, const CMergeState* other, CRole* functionalRole, QList<QPair<CConcept*,bool> >& backPropagated);
					//! the same for the links of one side given as (role, named filler or nullptr for top) pairs with the side's universal restrictions
					//! backPropagated receives the operands the merged successor's universal restrictions over the inverse of a link carry to the root
					Verdict decideMergedFunctionalSuccessorLinks(const QList<QPair<CRole*,CConcept*> >& links, const QList<QPair<CConcept*,bool> >& sideUniversals, const CMergeState* other, CRole* functionalRole, QList<QPair<CConcept*,bool> >& backPropagated);
					//! the links an existential restriction over the functional role or a sub role of it contributes; false if it is one the merge cannot take
					bool collectRestrictionLinks(CConcept* concept, bool negated, CRole* functionalRole, QList<QPair<CRole*,CConcept*> >& links);
					//! the links of a saturation node's successors over the functional role or a sub role of it; false if one of them is not a named class
					bool collectNodeFunctionalLinks(CIndividualSaturationProcessNode* repNode, CRole* functionalRole, QList<QPair<CRole*,CConcept*> >& links);
					//! the functional roles over which a saturation node and a state both have successors, decided from the merged successors where possible
					Verdict decideNodeSuccessorRoleConflicts(CIndividualSaturationProcessNode* repNode, const QList<CRole*>& conflictRoleList, const QList<QPair<CConcept*,bool> >& nodeUniversals, const CMergeState* other, bool nondeterministic, QList<QPair<CConcept*,bool> >& backPropagated);
					//! whether every completed label merged into the state (and its base chain, if asked) with successors under the role satisfies the universal restriction at them, see completedLabelSuccessorsSatisfyUniversal
					bool completedLabelsSatisfyUniversal(const CMergeState* holder, bool withBase, const QList<QPair<CConcept*,bool> >& allUniversals, CRole* role, CConcept* universalConcept, bool negated);
					//! the universal restrictions of a state, and of its base chain if asked
					void collectUniversals(const CMergeState* state, bool withBase, QList<QPair<CConcept*,bool> >& universals);
					//! decides the merge of the node with the base by scanning the node's label only, FALLBACK if an implication would fire
					enum FastVerdict { FAST_SUBSUMED, FAST_NOT_SUBSUMED, FAST_UNDECIDED, FAST_FALLBACK };
					FastVerdict decideConjunctionSatisfiableFast(CIndividualSaturationProcessNode* baseNode, const CMergeState* base);
					//! decides the merge by closing the union of the labels under the rules of the saturation
					Verdict decideConjunctionSatisfiableClosure(CIndividualSaturationProcessNode* baseNode, const CMergeState* base);
					bool addMergeLabelConcept(CMergeState& state, CConcept* concept, bool negated, bool derived);
					bool addMergeNamedClass(CMergeState& state, CConcept* concept);
					//! adds a completed label as a part of the merge, its successors by their roles only
					bool addCompletedLabelToMergeState(CMergeState& state, CCompletedLabel* label);
					/*!
					 *	Makes the node the part of the merge the decision is about, with its label as the
					 *	overlay of the state instead of copied into it; what the label holds beyond concepts,
					 *	implications, universal restrictions, self restrictions, candidates and successors, is
					 *	collected as addMergeNamedClass does. False if the merge cannot be decided.
					 */
					bool overlayNodeIntoMergeState(CMergeState& state, CIndividualSaturationProcessNode* baseNode);
					bool overlayCompletedLabelIntoMergeState(CMergeState& state, CCompletedLabel* label);
					/*!
					 *	Closes the label under the rules and adds its equivalence candidates with their
					 *	saturations, once, so that no query has to; what the closure with the candidates
					 *	derives rests on a choice. False if it cannot be done, with getCompletionConcept set
					 *	when a candidate needs its label completed first.
					 */
					bool extendCompletedLabel(CCompletedLabel* label);
					//! like decideEntailed, for a class whose saturation is replaced by its completed label
					Verdict decideEntailedCompleted(CCompletedLabel* label, CConcept* concept, bool negated, cint64 depth);
					//! like the fast scan, for a completed label against the query's merged label
					Verdict decideCompletedLabelSatisfiable(CCompletedLabel* label, const CMergeState* base);
					Verdict decideCompletedLabelSatisfiableClosure(CCompletedLabel* label, const CMergeState* base);
					//! the tag of the role and of every super role of it
					static void collectRoleTagsWithSuperRoles(CRole* role, QSet<cint64>& tagSet);
					/*!
					 *	Whether the sub class is below the super class in the class hierarchy, which the
					 *	classification made complete; UNDECIDED when one of them has no node there.
					 */
					Verdict decideNamedSubsumptionFromHierarchy(CConcept* subClassConcept, CConcept* superClassConcept);
					//! whether a role of the existential restrictions is functional while a part has a successor under it, or a data role that a part has too
					bool hasSuccessorRoleConflict(const QList<CRole*>& successorRoleList, const QSet<cint64>& dataRoleTagSet, const CMergeState& state);
					//! an at-most-one restriction over a functional role with the top filler, the concept a functional property axiom becomes
					bool isFunctionalAtMostOne(CConcept* concept);
					//! whether a self loop on a functional role, or on a sub role of one, meets a successor of the state over that role
					bool hasFunctionalSelfLoopConflict(CConcept* selfConcept, const CMergeState& state);
					//! records a self loop of a part, false when it conflicts with a successor over a functional role
					bool addSelfLoop(CMergeState& state, CConcept* concept);
					//! whether a self restriction makes a node its own successor under the role
					bool isSelfLoopOnRole(CConcept* selfConcept, CRole* role);
					//! whether the node has an active successor under the role whose label lacks the concept
					bool hasSuccessorWithoutConcept(CIndividualSaturationProcessNode* repNode, CRole* role, CConcept* concept, bool negated, cint64 depth = 0);
					//! whether a successor's saturated label holds the concept, or the concept would add nothing at it (a conjunction of held concepts, a disjunction with a held disjunct, a universal restriction its successors satisfy)
					bool saturationNodeSatisfies(CIndividualSaturationProcessNode* succNode, CConcept* concept, bool negated, cint64 depth);
					//! whether a universal restriction of the base that reaches the successors of a completed label is satisfied by the saturations of the label's existential restrictions
					bool completedLabelSuccessorsSatisfyUniversal(CCompletedLabel* label, const QList<QPair<CConcept*,bool> >& otherUniversals, CRole* role, CConcept* universalConcept, bool negated);
					//! a saturation node whose successors stand for those of a completion model's node: processed, completed, not insufficient, no nominal (critical and cardinality flags allowed, see the definition)
					bool isUsableSuccessorSaturation(CIndividualSaturationProcessNode* node);
					//! whether a filler propagated into a successor cannot add successors to it: negated named classes, plain universal restrictions, conjunctions and disjunctions of such
					bool isSuccessorNeutralFiller(CConcept* concept, bool negated);
					bool isSuccessorCreatingRole(CRole* role);

				// protected variables
				protected:
					CConcreteOntology* mOntology;
					//! per role with chains, whether its automaton's first roles could be determined and which they are (issue #19)
					QHash<CRole*,QPair<bool,QSet<CRole*> > > mAutomatonFirstRoleHash;
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
					QHash<CConcept*,CCompletedLabel*> mCompletedLabelHash;
					bool mCompletedSuccessorDecision;
					bool mFunctionalAtMostSafe;
					bool mFunctionalSuccessorMerge;
					CConcept* mCompletionConcept;
					//! the class and the saturation node the current decision is about, at the root of the entailment check
					CConcept* mRootSubClassConcept;
					CIndividualSaturationProcessNode* mRootBaseNode;
					cint64 mCompletionRequestCount;
					cint64 mCompletedDecisionCount;
					class CQueryCompletionStatistics {
						public:
							cint64 mReliableDecisiveCount = 0;
							cint64 mCompletedDecisiveCount = 0;
							cint64 mCompletedUndecidedCount = 0;
					};
					QHash<QPair<CConcept*,bool>, CQueryCompletionStatistics> mQueryCompletionStatisticsHash;
					typedef QPair<QPair<CIndividualSaturationProcessNode*,CConcept*>,cint64> CExistentialMemoKey;
					//! the existential restrictions decided from a node's label, by node, filler and role tag with the filler's sign (issue #19)
					QHash<CExistentialMemoKey,Verdict> mExistentialMemoHash;
					CConcept* mExistentialMemoQueryConcept;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CSATURATIONSUBSUMPTIONDECIDER_H
