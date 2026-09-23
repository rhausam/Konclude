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

#include "CSaturationSubsumptionDecider.h"

#include "Reasoner/Consistiser/CSaturationConceptReferenceLinking.h"
#include "Reasoner/Kernel/Process/CIndividualSaturationProcessNodeStatusFlags.h"
#include "Reasoner/Kernel/Process/CLinkedRoleSaturationSuccessorData.h"
#include "Reasoner/Kernel/Process/CSaturationSuccessorData.h"
#include "Reasoner/Ontology/CRBox.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CSaturationSubsumptionDecider::CSaturationSubsumptionDecider(CConcreteOntology* ontology) {
				mOntology = ontology;
				mSatCalcTask = nullptr;
				mSubsumedCount = 0;
				mNotSubsumedCount = 0;
				mUndecidedCount = 0;
				mFiredImplicationCount = 0;
				mFastDecisionCount = 0;
				mClosureDecisionCount = 0;
				// the saturation of the ontology, as CPrecomputedSaturationSubsumerExtractor takes it
				CPrecomputation* precomputation = mOntology->getPrecomputation();
				if (precomputation) {
					CSaturationTaskData* saturationTaskData = (CSaturationTaskData*)precomputation->getSaturationModelData();
					if (saturationTaskData) {
						mSatCalcTask = saturationTaskData->getSaturationTask();
					}
				}
			}


			CSaturationSubsumptionDecider::~CSaturationSubsumptionDecider() {
				qDeleteAll(mConjunctionMergeBaseHash);
			}


			bool CSaturationSubsumptionDecider::hasSaturation() {
				return mSatCalcTask != nullptr;
			}


			cint64 CSaturationSubsumptionDecider::getSubsumedCount() {
				return mSubsumedCount;
			}

			cint64 CSaturationSubsumptionDecider::getNotSubsumedCount() {
				return mNotSubsumedCount;
			}

			cint64 CSaturationSubsumptionDecider::getUndecidedCount() {
				return mUndecidedCount;
			}

			QHash<cint64,cint64>* CSaturationSubsumptionDecider::getUndecidedMergeConceptCodeCounts() {
				return &mUndecidedMergeConceptCodeCounts;
			}

			cint64 CSaturationSubsumptionDecider::getFiredImplicationCount() {
				return mFiredImplicationCount;
			}

			cint64 CSaturationSubsumptionDecider::getFastDecisionCount() {
				return mFastDecisionCount;
			}

			cint64 CSaturationSubsumptionDecider::getClosureDecisionCount() {
				return mClosureDecisionCount;
			}



			CSaturationSubsumptionDecider::Verdict CSaturationSubsumptionDecider::decideSubClass(CConcept* subClassConcept, CConcept* queryConcept, bool queryNegation) {
				Verdict verdict = UNDECIDED;
				if (mSatCalcTask && subClassConcept && queryConcept) {
					bool unsatisfiable = false;
					bool reliable = false;
					CIndividualSaturationProcessNode* node = getSaturationNode(subClassConcept, &unsatisfiable, &reliable);
					if (!node) {
						// the reasons besides label concepts are counted under negative keys, printed halved
						// by the handler: -100 no saturation node, -300 insufficient, -400 unprocessed, -500
						// critical, -600 not completed, -1100 cardinality or nominal flags, -1500 data value,
						// -1600 nominal integrated, -1700 no label, -1000 functional role, -2500 and -3000
						// universal restrictions reaching successors and self loops, -3500 derived successor,
						// and for the query side -4600 no node, -4700 unreliable, -4800 flags, -4900 data
						// value or nominal, -5000 no label
						mUndecidedMergeConceptCodeCounts[-200] = mUndecidedMergeConceptCodeCounts.value(-200, 0) + 1;
					}
					if (node) {
						if (unsatisfiable) {
							// an unsatisfiable class is subsumed by everything
							verdict = SUBSUMED;
						} else {
							verdict = decideEntailed(node, reliable, queryConcept, queryNegation, 0);
						}
					}
				}
				if (verdict == SUBSUMED) {
					++mSubsumedCount;
				} else if (verdict == NOT_SUBSUMED) {
					++mNotSubsumedCount;
				} else {
					++mUndecidedCount;
				}
				return verdict;
			}



			CIndividualSaturationProcessNode* CSaturationSubsumptionDecider::getSaturationNode(CConcept* concept, bool* unsatisfiable, bool* reliable) {
				CConceptProcessData* conProData = (CConceptProcessData*)concept->getConceptData();
				if (!conProData) {
					return nullptr;
				}
				CConceptSaturationReferenceLinkingData* conSatRefLinking = (CConceptSaturationReferenceLinkingData*)conProData->getConceptReferenceLinking();
				if (!conSatRefLinking) {
					return nullptr;
				}
				CSaturationConceptReferenceLinking* satConRefLinking = (CSaturationConceptReferenceLinking*)conSatRefLinking->getPositiveConceptSaturationReferenceLinkingData();
				if (!satConRefLinking) {
					return nullptr;
				}
				CIndividualSaturationProcessNode* baseNode = (CIndividualSaturationProcessNode*)satConRefLinking->getIndividualProcessNodeForConcept();
				if (!baseNode) {
					return nullptr;
				}
				CIndividualSaturationProcessNode* repNode = getRepresentativeNode(baseNode);
				CIndividualSaturationProcessNodeStatusFlags* baseFlags = baseNode->getIndirectStatusFlags();
				CIndividualSaturationProcessNodeStatusFlags* repFlags = repNode->getIndirectStatusFlags();
				if (unsatisfiable) {
					*unsatisfiable = baseFlags->hasClashedFlag() || repFlags->hasClashedFlag();
				}
				if (reliable) {
					*reliable = isReliable(baseNode);
				}
				return baseNode;
			}


			CIndividualSaturationProcessNode* CSaturationSubsumptionDecider::getRepresentativeNode(CIndividualSaturationProcessNode* baseNode) {
				CIndividualSaturationProcessNode* node = baseNode;
				while (node->hasSubstituteIndividualNode()) {
					node = node->getSubstituteIndividualNode();
				}
				return node;
			}


			bool CSaturationSubsumptionDecider::isReliable(CIndividualSaturationProcessNode* baseNode) {
				// the flags the classifier consults before it trusts a saturation, plus the critical one
				CIndividualSaturationProcessNode* repNode = getRepresentativeNode(baseNode);
				CIndividualSaturationProcessNodeStatusFlags* baseFlags = baseNode->getIndirectStatusFlags();
				CIndividualSaturationProcessNodeStatusFlags* repFlags = repNode->getIndirectStatusFlags();
				if (baseFlags->hasInsufficientFlag() || repFlags->hasInsufficientFlag()) {
					mUndecidedMergeConceptCodeCounts[-600] = mUndecidedMergeConceptCodeCounts.value(-600, 0) + 1;
					return false;
				}
				if (baseFlags->hasUnprocessedFlag() || repFlags->hasUnprocessedFlag()) {
					mUndecidedMergeConceptCodeCounts[-800] = mUndecidedMergeConceptCodeCounts.value(-800, 0) + 1;
					return false;
				}
				if (baseFlags->hasCriticalFlag() || repFlags->hasCriticalFlag()) {
					mUndecidedMergeConceptCodeCounts[-1000] = mUndecidedMergeConceptCodeCounts.value(-1000, 0) + 1;
					return false;
				}
				if (!repNode->isCompleted()) {
					mUndecidedMergeConceptCodeCounts[-1200] = mUndecidedMergeConceptCodeCounts.value(-1200, 0) + 1;
					return false;
				}
				return true;
			}


			bool CSaturationSubsumptionDecider::hasLabelConcept(CIndividualSaturationProcessNode* baseNode, CConcept* concept, bool negated) {
				CIndividualSaturationProcessNode* node = baseNode;
				while (node->hasSubstituteIndividualNode()) {
					if (node != baseNode && !negated) {
						// a substitute stands for a concept that subsumes the base one, see the classifier's extractor
						CSaturationConceptDataItem* satConDataItem = (CSaturationConceptDataItem*)node->getSaturationConceptReferenceLinking();
						if (satConDataItem && !satConDataItem->getSaturationNegation() && !satConDataItem->getSaturationRoleRanges()) {
							if (satConDataItem->getSaturationConcept() == concept) {
								return true;
							}
						}
					}
					node = node->getSubstituteIndividualNode();
				}
				CReapplyConceptSaturationLabelSet* labelSet = node->getReapplyConceptSaturationLabelSet(false);
				if (labelSet && labelSet->hasConcept(concept, negated)) {
					return true;
				}
				return false;
			}


			bool CSaturationSubsumptionDecider::isSuccessorCreatingRole(CRole* role) {
				// a role whose successors the saturation may not have materialised, because they are
				// implied by chains or transitivity that the preprocessing compiled away
				if (role->isDataRole()) {
					return false;
				}
				if (role->isComplexRole() || role->isTransitive() || role->hasRoleChainSuperSharing() || role->hasRoleChainSubSharing()) {
					return false;
				}
				return true;
			}



			CSaturationSubsumptionDecider::Verdict CSaturationSubsumptionDecider::decideEntailed(CIndividualSaturationProcessNode* baseNode, bool reliable, CConcept* concept, bool negated, cint64 depth) {
				if (depth > 64) {
					return UNDECIDED;
				}
				cint64 opCode = concept->getOperatorCode();
				CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();

				if (opCode == CCNOT && opLinker) {
					return decideEntailed(baseNode, reliable, opLinker->getData(), !(opLinker->isNegated() ^ negated), depth + 1);
				}

				if (!negated) {
					// every instance of the node's concept has to satisfy the concept
					if (opCode == CCTOP) {
						return SUBSUMED;
					}
					if (opCode == CCBOTTOM) {
						return reliable ? NOT_SUBSUMED : UNDECIDED;
					}
					if (concept->hasClassName()) {
						if (hasLabelConcept(baseNode, concept, false)) {
							return SUBSUMED;
						}
						// a complete saturation contains every named subsumer, which is how the classifier
						// derives the class hierarchy from it
						return reliable ? NOT_SUBSUMED : UNDECIDED;
					}
					if (opCode == CCAND) {
						bool undecided = false;
						for (CSortedNegLinker<CConcept*>* opIt = opLinker; opIt; opIt = opIt->getNext()) {
							Verdict opVerdict = decideEntailed(baseNode, reliable, opIt->getData(), opIt->isNegated(), depth + 1);
							if (opVerdict == NOT_SUBSUMED) {
								return NOT_SUBSUMED;
							} else if (opVerdict == UNDECIDED) {
								undecided = true;
							}
						}
						return undecided ? UNDECIDED : SUBSUMED;
					}
					if (opCode == CCOR) {
						for (CSortedNegLinker<CConcept*>* opIt = opLinker; opIt; opIt = opIt->getNext()) {
							if (decideEntailed(baseNode, reliable, opIt->getData(), opIt->isNegated(), depth + 1) == SUBSUMED) {
								return SUBSUMED;
							}
						}
						// a disjunction can be entailed without any disjunct being entailed
						return UNDECIDED;
					}
					if (opCode == CCSOME && opLinker) {
						if (hasLabelConcept(baseNode, concept, false)) {
							return SUBSUMED;
						}
						CRole* role = concept->getRole();
						CConcept* filler = opLinker->getData();
						bool fillerNegation = opLinker->isNegated();
						if (!role || role->isDataRole()) {
							return UNDECIDED;
						}
						bool undecided = false;
						CIndividualSaturationProcessNode* repNode = getRepresentativeNode(baseNode);
						CLinkedRoleSaturationSuccessorHash* succHash = repNode->getLinkedRoleSuccessorHash(false);
						if (succHash && succHash->hasLinkedRoleSuccessorData(role)) {
							// the successors are linked under every super role, so the role itself suffices;
							// looked up without the getter, whose subscript access modifies the node's hash
							// from this thread while the tableau tasks read it
							CLinkedRoleSaturationSuccessorData* roleSuccData = succHash->getLinkedRoleSuccessorHash()->value(role);
							CPROCESSMAP<cint64,CSaturationSuccessorData*>* succMap = roleSuccData ? roleSuccData->getSuccessorNodeDataMap(false) : nullptr;
							if (succMap) {
								for (CPROCESSMAP<cint64,CSaturationSuccessorData*>::const_iterator it = succMap->constBegin(), itEnd = succMap->constEnd(); it != itEnd; ++it) {
									CSaturationSuccessorData* succData = it.value();
									if (succData->mActiveCount <= 0) {
										continue;
									}
									if (succData->mVALUENominalConnection || !succData->mSuccIndiNode) {
										undecided = true;
										continue;
									}
									CIndividualSaturationProcessNode* succNode = succData->mSuccIndiNode;
									Verdict succVerdict = decideEntailed(succNode, isReliable(succNode), filler, fillerNegation, depth + 1);
									if (succVerdict == SUBSUMED) {
										return SUBSUMED;
									} else if (succVerdict == UNDECIDED) {
										undecided = true;
									}
								}
							}
						}
						if (undecided || !reliable || !isSuccessorCreatingRole(role)) {
							return UNDECIDED;
						}
						return NOT_SUBSUMED;
					}
					// universal and cardinality restrictions, nominals, self restrictions and data ranges are
					// only recognised when the label carries the very concept
					if (hasLabelConcept(baseNode, concept, false)) {
						return SUBSUMED;
					}
					return UNDECIDED;

				} else {
					// no instance of the node's concept may satisfy the concept, i.e. their conjunction is
					// unsatisfiable
					if (opCode == CCTOP) {
						return reliable ? NOT_SUBSUMED : UNDECIDED;
					}
					if (opCode == CCBOTTOM) {
						return SUBSUMED;
					}
					if (opCode == CCOR) {
						// the negation of a disjunction is the conjunction of the negated disjuncts
						bool undecided = false;
						for (CSortedNegLinker<CConcept*>* opIt = opLinker; opIt; opIt = opIt->getNext()) {
							Verdict opVerdict = decideEntailed(baseNode, reliable, opIt->getData(), !opIt->isNegated(), depth + 1);
							if (opVerdict == NOT_SUBSUMED) {
								return NOT_SUBSUMED;
							} else if (opVerdict == UNDECIDED) {
								undecided = true;
							}
						}
						return undecided ? UNDECIDED : SUBSUMED;
					}
					if (hasLabelConcept(baseNode, concept, true)) {
						return SUBSUMED;
					}
					if (concept->hasClassName() || opCode == CCAND) {
						// subsumed as soon as one conjunct is excluded on its own
						if (opCode == CCAND) {
							for (CSortedNegLinker<CConcept*>* opIt = opLinker; opIt; opIt = opIt->getNext()) {
								if (decideEntailed(baseNode, reliable, opIt->getData(), !opIt->isNegated(), depth + 1) == SUBSUMED) {
									return SUBSUMED;
								}
							}
						}
						// otherwise the node's concept and the conjunction have to be shown compatible
						QList<CConcept*> conjuncts;
						if (collectNamedConjuncts(concept, false, conjuncts, 0)) {
							return decideConjunctionSatisfiable(baseNode, reliable, conjuncts);
						}
					}
					return UNDECIDED;
				}
			}



			bool CSaturationSubsumptionDecider::collectNamedConjuncts(CConcept* concept, bool negated, QList<CConcept*>& conjuncts, cint64 depth) {
				if (negated || depth > 64) {
					return false;
				}
				if (concept->hasClassName()) {
					conjuncts.append(concept);
					return true;
				}
				if (concept->getOperatorCode() == CCAND) {
					for (CSortedNegLinker<CConcept*>* opIt = concept->getOperandList(); opIt; opIt = opIt->getNext()) {
						if (!collectNamedConjuncts(opIt->getData(), opIt->isNegated(), conjuncts, depth + 1)) {
							return false;
						}
					}
					return true;
				}
				return false;
			}



			CSaturationSubsumptionDecider::LabelConceptKind CSaturationSubsumptionDecider::getLabelConceptKind(CConcept* concept, bool negated) {
				// what a concept of a saturated label can do when the label is merged with another
				// one, following the rules the saturation applies to each operator:
				//  - a named class, the top concept, a conjunction, an existential restriction, an at
				//    least restriction or the self loop adds nothing at the merged node beyond what its
				//    own part already derived, its successors stay separate, so it can only clash with
				//    its complement;
				//  - an implication fires when all its triggers are present, which the other part
				//    may supply;
				//  - a universal restriction, including the automaton transitions of the role chains,
				//    propagates into the successors of the other parts;
				//  - everything else, disjunctions, choices, cardinalities, nominals, data, branch
				//    triggers, is left to the tableau.
				// The operator codes of the basic constructors come in pairs whose sign the negation
				// flag flips, an existential restriction is stored as a negated universal one, so the
				// effective operator is taken for those.
				if (concept->hasClassName()) {
					return LCK_SAFE;
				}
				cint64 opCode = concept->getOperatorCode();
				cint64 absCode = opCode < 0 ? -opCode : opCode;
				bool paired = absCode == 1 || absCode == 3 || absCode == 4 || absCode == 5 || absCode == 11;
				cint64 effCode = (paired && negated) ? -opCode : opCode;
				bool effNegated = paired ? false : negated;
				if (effCode == CCTOP || effCode == CCBOTTOM) {
					return LCK_SAFE;
				}
				if (!effNegated) {
					if (effCode == CCATOM || effCode == CCSUB || effCode == CCEQ) {
						return LCK_SAFE;
					}
					if (effCode == CCAND || effCode == CCIMPLTRIG || effCode == CCAQAND || effCode == CCIMPLAQAND || effCode == CCAQCHOOCE) {
						return LCK_CONJUNCTION;
					}
					if (effCode == CCSOME || effCode == CCAQSOME || effCode == CCATLEAST || effCode == CCSELF) {
						return LCK_SAFE;
					}
					if (effCode == CCIMPL) {
						return LCK_IMPLICATION;
					}
					if (effCode == CCALL || effCode == CCAQALL || effCode == CCIMPLALL || effCode == CCIMPLAQALL) {
						return LCK_UNIVERSAL;
					}
				} else {
					if (effCode == CCATOM || effCode == CCSUB || effCode == CCEQ) {
						return LCK_SAFE;
					}
				}
				return LCK_UNSAFE;
			}


			bool CSaturationSubsumptionDecider::isSelfLoopOnRole(CConcept* selfConcept, CRole* role) {
				// whether a self restriction makes the node its own successor under the role, directly
				// or through a sub role
				CRole* selfRole = selfConcept->getRole();
				if (!selfRole) {
					return false;
				}
				if (selfRole == role || selfRole->getRoleTag() == role->getRoleTag()) {
					return true;
				}
				for (CSortedNegLinker<CRole*>* superIt = selfRole->getIndirectSuperRoleList(); superIt; superIt = superIt->getNext()) {
					if (!superIt->isNegated() && (superIt->getData() == role || superIt->getData()->getRoleTag() == role->getRoleTag())) {
						return true;
					}
				}
				return false;
			}


			bool CSaturationSubsumptionDecider::hasSuccessorWithoutConcept(CIndividualSaturationProcessNode* repNode, CRole* role, CConcept* concept, bool negated) {
				// whether the node has an active successor under the role that lacks the concept, which
				// a universal restriction of another part would then add
				CLinkedRoleSaturationSuccessorHash* succHash = repNode->getLinkedRoleSuccessorHash(false);
				if (!succHash || !succHash->hasLinkedRoleSuccessorData(role)) {
					return false;
				}
				// looked up without the getter, whose subscript access modifies the node's hash from
				// this thread while the tableau tasks read it
				CLinkedRoleSaturationSuccessorData* roleSuccData = succHash->getLinkedRoleSuccessorHash()->value(role);
				CPROCESSMAP<cint64,CSaturationSuccessorData*>* succMap = roleSuccData ? roleSuccData->getSuccessorNodeDataMap(false) : nullptr;
				if (!succMap) {
					return false;
				}
				for (CPROCESSMAP<cint64,CSaturationSuccessorData*>::const_iterator it = succMap->constBegin(), itEnd = succMap->constEnd(); it != itEnd; ++it) {
					CSaturationSuccessorData* succData = it.value();
					if (succData->mActiveCount <= 0) {
						continue;
					}
					if (succData->mVALUENominalConnection || !succData->mSuccIndiNode) {
						return true;
					}
					if (!hasLabelConcept(succData->mSuccIndiNode, concept, negated)) {
						return true;
					}
				}
				return false;
			}



			bool CSaturationSubsumptionDecider::addMergeLabelConcept(CMergeState& state, CConcept* concept, bool negated, bool derived) {
				// adds a concept to the merged label and everything the saturation would derive from it
				// at the merged node; false if the merge cannot be decided
				if (negated ? state.hasNegative(concept) : state.hasPositive(concept)) {
					return true;
				}
				if (negated) {
					state.mNegativeSet.insert(concept);
				} else {
					state.mPositiveSet.insert(concept);
				}
				if (state.hasPositive(concept) && state.hasNegative(concept)) {
					state.mClashed = true;
					return true;
				}
				if (!negated && concept->hasClassName()) {
					if (derived) {
						// a named class derived at the merged node brings its own saturation along
						return addMergeNamedClass(state, concept);
					}
					return true;
				}
				LabelConceptKind kind = getLabelConceptKind(concept, negated);
				cint64 opCode = concept->getOperatorCode();
				if (kind == LCK_UNSAFE) {
					cint64 codeKey = opCode * 2 + (negated ? 1 : 0);
					mUndecidedMergeConceptCodeCounts[codeKey] = mUndecidedMergeConceptCodeCounts.value(codeKey, 0) + 1;
					return false;
				}
				if (kind == LCK_CONJUNCTION) {
					if (derived) {
						for (CSortedNegLinker<CConcept*>* opIt = concept->getOperandList(); opIt; opIt = opIt->getNext()) {
							if (opCode == CCAQCHOOCE && opIt->isNegated()) {
								// the choose rule adds the operands of the descriptor's polarity only
								continue;
							}
							if (!addMergeLabelConcept(state, opIt->getData(), opIt->isNegated(), true)) {
								return false;
							}
						}
					}
					// the operands of a conjunction of a part are in that part's label already
					return true;
				}
				if (kind == LCK_IMPLICATION) {
					state.mImplicationList.append(concept);
					return true;
				}
				if (kind == LCK_UNIVERSAL) {
					state.mUniversalList.append(CMergeUniversal(concept, negated, derived ? nullptr : state.mCurrentPartNode));
					return true;
				}
				if (!negated && opCode == CCSELF) {
					state.mSelfConceptSet.insert(concept);
					return true;
				}
				if (derived) {
					cint64 absCode = opCode < 0 ? -opCode : opCode;
					bool existential = (absCode == 5 || absCode == 11) && ((opCode < 0) != negated);
					if (existential || (!negated && opCode == CCATLEAST)) {
						// a derived successor: everything a universal restriction or a cardinality could
						// do with it is left to the tableau, and the domain of its role joins the node
						CRole* role = concept->getRole();
						if (!role || opCode == CCATLEAST) {
							mUndecidedMergeConceptCodeCounts[-7000] = mUndecidedMergeConceptCodeCounts.value(-7000, 0) + 1;
							return false;
						}
						state.mDerivedSuccessorRoleSet.insert(role);
						for (CSortedNegLinker<CConcept*>* domIt = role->getDomainConceptList(); domIt; domIt = domIt->getNext()) {
							if (!addMergeLabelConcept(state, domIt->getData(), domIt->isNegated(), true)) {
								return false;
							}
						}
					}
				}
				return true;
			}


			bool CSaturationSubsumptionDecider::addMergeNamedClass(CMergeState& state, CConcept* concept) {
				// adds the saturation of a named class to the merged label, its successors become
				// successors of the merged node
				bool unsatisfiable = false;
				bool reliable = false;
				CIndividualSaturationProcessNode* baseNode = getSaturationNode(concept, &unsatisfiable, &reliable);
				if (!baseNode) {
					mUndecidedMergeConceptCodeCounts[-9200] = mUndecidedMergeConceptCodeCounts.value(-9200, 0) + 1;
					return false;
				}
				if (unsatisfiable) {
					state.mClashed = true;
					return true;
				}
				if (!reliable) {
					mUndecidedMergeConceptCodeCounts[-9400] = mUndecidedMergeConceptCodeCounts.value(-9400, 0) + 1;
					return false;
				}
				CIndividualSaturationProcessNode* repNode = getRepresentativeNode(baseNode);
				if (state.hasPartNode(repNode)) {
					return true;
				}
				state.mPartNodeSet.insert(repNode);
				state.mPartNodeList.append(repNode);
				CIndividualSaturationProcessNode* previousPartNode = state.mCurrentPartNode;
				state.mCurrentPartNode = repNode;

				CIndividualSaturationProcessNodeStatusFlags* flags = repNode->getIndirectStatusFlags();
				if (flags->hasCardinalityRestrictedFlag() || flags->hasCardinalityProplematicFlag() || flags->hasNominalConnectionFlag()) {
					mUndecidedMergeConceptCodeCounts[-9600] = mUndecidedMergeConceptCodeCounts.value(-9600, 0) + 1;
					return false;
				}
				if (repNode->hasDataValueApplied() || repNode->hasNominalIntegrated()) {
					mUndecidedMergeConceptCodeCounts[-9800] = mUndecidedMergeConceptCodeCounts.value(-9800, 0) + 1;
					return false;
				}
				CIndividualSaturationProcessNode* node = baseNode;
				while (node->hasSubstituteIndividualNode()) {
					if (node != baseNode) {
						CSaturationConceptDataItem* satConDataItem = (CSaturationConceptDataItem*)node->getSaturationConceptReferenceLinking();
						if (satConDataItem && !satConDataItem->getSaturationNegation() && !satConDataItem->getSaturationRoleRanges()) {
							if (!addMergeLabelConcept(state, satConDataItem->getSaturationConcept(), false, false)) {
								return false;
							}
						}
					}
					node = node->getSubstituteIndividualNode();
				}
				CReapplyConceptSaturationLabelSet* labelSet = repNode->getReapplyConceptSaturationLabelSet(false);
				if (!labelSet) {
					mUndecidedMergeConceptCodeCounts[-10000] = mUndecidedMergeConceptCodeCounts.value(-10000, 0) + 1;
					return false;
				}
				for (CConceptSaturationDescriptor* conDesIt = labelSet->getConceptSaturationDescriptionLinker(); conDesIt; conDesIt = conDesIt->getNext()) {
					if (!addMergeLabelConcept(state, conDesIt->getConcept(), conDesIt->isNegated(), false)) {
						return false;
					}
				}
				// successors under a functional role would be merged with those of the other parts
				CLinkedRoleSaturationSuccessorHash* succHash = repNode->getLinkedRoleSuccessorHash(false);
				if (succHash) {
					CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* roleSuccHash = succHash->getLinkedRoleSuccessorHash();
					for (CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>::const_iterator it = roleSuccHash->constBegin(), itEnd = roleSuccHash->constEnd(); it != itEnd; ++it) {
						CRole* role = it.key();
						CLinkedRoleSaturationSuccessorData* roleSuccData = it.value();
						if (roleSuccData && roleSuccData->getSuccessorCount() > 0) {
							if (role->isFunctional() && state.hasSuccessorRole(role)) {
								mUndecidedMergeConceptCodeCounts[-2000] = mUndecidedMergeConceptCodeCounts.value(-2000, 0) + 1;
								return false;
							}
							state.mSuccessorRoleSet.insert(role);
						}
					}
				}
				state.mCurrentPartNode = previousPartNode;
				return true;
			}



			bool CSaturationSubsumptionDecider::closeMergeState(CMergeState& state) {
				// fires the implications, the base's and the state's, until none fires any more; the
				// first operand is the implied concept, the ones after it are the triggers, which the
				// saturation waits for in order, see updateImplicationReapplyConceptSaturationDescriptor
				bool changed = !state.mClashed;
				while (changed && !state.mClashed) {
					changed = false;
					for (cint64 listIndex = 0; listIndex < 2 && !state.mClashed; ++listIndex) {
						const QList<CConcept*>* implList = listIndex == 0 ? (state.mBase ? &state.mBase->mImplicationList : nullptr) : &state.mImplicationList;
						if (!implList) {
							continue;
						}
						for (cint64 i = 0; i < implList->size() && !state.mClashed; ++i) {
							CConcept* implConcept = implList->at(i);
							CSortedNegLinker<CConcept*>* impliedLinker = implConcept->getOperandList();
							if (!impliedLinker) {
								continue;
							}
							bool triggered = true;
							for (CSortedNegLinker<CConcept*>* trigIt = impliedLinker->getNext(); trigIt; trigIt = trigIt->getNext()) {
								// a trigger counts when its concept is in the label positively, whatever the flag
								// of its operand linker says, see insertConceptReapplicationReturnTriggered
								if (!state.hasPositive(trigIt->getData())) {
									triggered = false;
									break;
								}
							}
							if (triggered) {
								bool concluded = impliedLinker->isNegated() ? state.hasNegative(impliedLinker->getData()) : state.hasPositive(impliedLinker->getData());
								if (!concluded) {
									++mFiredImplicationCount;
									if (!addMergeLabelConcept(state, impliedLinker->getData(), impliedLinker->isNegated(), true)) {
										return false;
									}
									changed = true;
								}
							}
						}
					}
				}
				return true;
			}



			const CSaturationSubsumptionDecider::CMergeState* CSaturationSubsumptionDecider::getConjunctionMergeBase(const QList<CConcept*>& conjuncts) {
				quint64 key = 1469598103934665603ULL;
				for (CConcept* conjunct : conjuncts) {
					key = (key ^ (quint64)conjunct->getConceptTag()) * 1099511628211ULL;
				}
				QHash<quint64,CMergeState*>::const_iterator found = mConjunctionMergeBaseHash.constFind(key);
				while (found != mConjunctionMergeBaseHash.constEnd() && found.key() == key) {
					CMergeState* candidate = found.value();
					bool same = candidate->mConjunctTagList.size() == conjuncts.size();
					for (cint64 i = 0; same && i < conjuncts.size(); ++i) {
						same = candidate->mConjunctTagList.at(i) == conjuncts.at(i)->getConceptTag();
					}
					if (same) {
						return candidate->mClashed || !candidate->mImplicationList.isEmpty() || !candidate->mPositiveSet.isEmpty() ? candidate : nullptr;
					}
					++found;
				}
				if (mConjunctionMergeBaseHash.size() > 4096) {
					// the sides of the queries asked so far, enough for a session of an editor
					qDeleteAll(mConjunctionMergeBaseHash);
					mConjunctionMergeBaseHash.clear();
				}
				CMergeState* base = new CMergeState();
				for (CConcept* conjunct : conjuncts) {
					base->mConjunctTagList.append(conjunct->getConceptTag());
				}
				bool decided = true;
				for (CConcept* conjunct : conjuncts) {
					if (!addMergeNamedClass(*base, conjunct)) {
						decided = false;
						break;
					}
				}
				if (decided && !base->mClashed) {
					decided = closeMergeState(*base);
				}
				if (decided && !base->mClashed) {
					// the implications that wait for something the base lacks, by the first such trigger
					for (CConcept* implConcept : base->mImplicationList) {
						CSortedNegLinker<CConcept*>* impliedLinker = implConcept->getOperandList();
						if (!impliedLinker) {
							continue;
						}
						for (CSortedNegLinker<CConcept*>* trigIt = impliedLinker->getNext(); trigIt; trigIt = trigIt->getNext()) {
							if (!base->hasPositive(trigIt->getData())) {
								base->mTriggerImplicationHash[trigIt->getData()].append(implConcept);
							}
						}
					}
				}
				if (!decided) {
					// an undecidable base is kept as an empty one, so that it is not built again
					delete base;
					base = new CMergeState();
					for (CConcept* conjunct : conjuncts) {
						base->mConjunctTagList.append(conjunct->getConceptTag());
					}
				}
				mConjunctionMergeBaseHash.insertMulti(key, base);
				return decided ? base : nullptr;
			}



			CSaturationSubsumptionDecider::FastVerdict CSaturationSubsumptionDecider::decideConjunctionSatisfiableFast(CIndividualSaturationProcessNode* baseNode, const CMergeState* base) {
				// The node's label is scanned once and nothing is copied: a concept that the base holds
				// in the other polarity is a clash, a construct the tableau has to handle leaves the
				// merge undecided, and an implication that the union would fire hands over to the
				// closure. What remains is checked for universal restrictions reaching successors of the
				// other side and for functional roles with successors on both sides.
				CIndividualSaturationProcessNode* repNode = getRepresentativeNode(baseNode);
				CIndividualSaturationProcessNodeStatusFlags* flags = repNode->getIndirectStatusFlags();
				if (flags->hasCardinalityRestrictedFlag() || flags->hasCardinalityProplematicFlag() || flags->hasNominalConnectionFlag()) {
					mUndecidedMergeConceptCodeCounts[-2200] = mUndecidedMergeConceptCodeCounts.value(-2200, 0) + 1;
					return FAST_UNDECIDED;
				}
				if (repNode->hasDataValueApplied()) {
					mUndecidedMergeConceptCodeCounts[-3000] = mUndecidedMergeConceptCodeCounts.value(-3000, 0) + 1;
					return FAST_UNDECIDED;
				}
				if (repNode->hasNominalIntegrated()) {
					mUndecidedMergeConceptCodeCounts[-3200] = mUndecidedMergeConceptCodeCounts.value(-3200, 0) + 1;
					return FAST_UNDECIDED;
				}
				CReapplyConceptSaturationLabelSet* labelSet = repNode->getReapplyConceptSaturationLabelSet(false);
				if (!labelSet) {
					mUndecidedMergeConceptCodeCounts[-3400] = mUndecidedMergeConceptCodeCounts.value(-3400, 0) + 1;
					return FAST_UNDECIDED;
				}
				QVarLengthArray<CConcept*,4> substituteArray;
				CIndividualSaturationProcessNode* node = baseNode;
				while (node->hasSubstituteIndividualNode()) {
					if (node != baseNode) {
						CSaturationConceptDataItem* satConDataItem = (CSaturationConceptDataItem*)node->getSaturationConceptReferenceLinking();
						if (satConDataItem && !satConDataItem->getSaturationNegation() && !satConDataItem->getSaturationRoleRanges()) {
							CConcept* substitute = satConDataItem->getSaturationConcept();
							if (base->hasNegative(substitute)) {
								return FAST_SUBSUMED;
							}
							if (base->mTriggerImplicationHash.contains(substitute)) {
								return FAST_FALLBACK;
							}
							substituteArray.append(substitute);
						}
					}
					node = node->getSubstituteIndividualNode();
				}
				// present in the union of the node and the base
				struct Presence {
					const CMergeState* mBase;
					CReapplyConceptSaturationLabelSet* mLabelSet;
					QVarLengthArray<CConcept*,4>* mSubstitutes;
					bool has(CConcept* concept, bool negated) const {
						if (negated ? mBase->hasNegative(concept) : mBase->hasPositive(concept)) {
							return true;
						}
						if (mLabelSet->hasConcept(concept, negated)) {
							return true;
						}
						if (!negated) {
							for (CConcept* substitute : *mSubstitutes) {
								if (substitute == concept) {
									return true;
								}
							}
						}
						return false;
					}
				} presence = { base, labelSet, &substituteArray };

				QVarLengthArray<CConcept*,16> implicationArray;
				QVarLengthArray<QPair<CConcept*,bool>,16> universalArray;
				QVarLengthArray<CConcept*,4> selfArray;
				for (CConceptSaturationDescriptor* conDesIt = labelSet->getConceptSaturationDescriptionLinker(); conDesIt; conDesIt = conDesIt->getNext()) {
					CConcept* concept = conDesIt->getConcept();
					bool negated = conDesIt->isNegated();
					if (negated ? base->hasPositive(concept) : base->hasNegative(concept)) {
						return FAST_SUBSUMED;
					}
					LabelConceptKind kind = getLabelConceptKind(concept, negated);
					if (kind == LCK_UNSAFE) {
						cint64 codeKey = concept->getOperatorCode() * 2 + (negated ? 1 : 0);
						mUndecidedMergeConceptCodeCounts[codeKey] = mUndecidedMergeConceptCodeCounts.value(codeKey, 0) + 1;
						return FAST_UNDECIDED;
					}
					if (kind == LCK_IMPLICATION) {
						implicationArray.append(concept);
					} else if (kind == LCK_UNIVERSAL) {
						universalArray.append(QPair<CConcept*,bool>(concept, negated));
					} else if (!negated && concept->getOperatorCode() == CCSELF) {
						selfArray.append(concept);
					}
					if (!negated && base->mTriggerImplicationHash.contains(concept)) {
						// the node supplies a trigger the base waited for
						for (CConcept* implConcept : base->mTriggerImplicationHash.value(concept)) {
							CSortedNegLinker<CConcept*>* impliedLinker = implConcept->getOperandList();
							bool triggered = true;
							for (CSortedNegLinker<CConcept*>* trigIt = impliedLinker->getNext(); trigIt && triggered; trigIt = trigIt->getNext()) {
								triggered = presence.has(trigIt->getData(), false);
							}
							if (triggered && !presence.has(impliedLinker->getData(), impliedLinker->isNegated())) {
								return FAST_FALLBACK;
							}
						}
					}
				}
				// an implication of the node that the base completes
				for (CConcept* implConcept : implicationArray) {
					CSortedNegLinker<CConcept*>* impliedLinker = implConcept->getOperandList();
					if (!impliedLinker) {
						continue;
					}
					bool triggered = true;
					for (CSortedNegLinker<CConcept*>* trigIt = impliedLinker->getNext(); trigIt && triggered; trigIt = trigIt->getNext()) {
						triggered = presence.has(trigIt->getData(), false);
					}
					if (triggered && !presence.has(impliedLinker->getData(), impliedLinker->isNegated())) {
						return FAST_FALLBACK;
					}
				}
				// successors under a functional role on both sides would be merged
				CLinkedRoleSaturationSuccessorHash* succHash = repNode->getLinkedRoleSuccessorHash(false);
				if (succHash) {
					CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* roleSuccHash = succHash->getLinkedRoleSuccessorHash();
					for (CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>::const_iterator it = roleSuccHash->constBegin(), itEnd = roleSuccHash->constEnd(); it != itEnd; ++it) {
						CRole* role = it.key();
						CLinkedRoleSaturationSuccessorData* roleSuccData = it.value();
						if (roleSuccData && roleSuccData->getSuccessorCount() > 0 && role->isFunctional() && base->hasSuccessorRole(role)) {
							mUndecidedMergeConceptCodeCounts[-2000] = mUndecidedMergeConceptCodeCounts.value(-2000, 0) + 1;
							return FAST_UNDECIDED;
						}
					}
				}
				// the universal restrictions of the base reach the node's successors and self loops
				for (const CMergeUniversal& universal : base->mUniversalList) {
					CRole* role = universal.mConcept->getRole();
					CSortedNegLinker<CConcept*>* opLinker = universal.mConcept->getOperandList();
					if (!role || !opLinker) {
						return FAST_UNDECIDED;
					}
					for (CSortedNegLinker<CConcept*>* opIt = opLinker; opIt; opIt = opIt->getNext()) {
						bool propagatedNegation = opIt->isNegated() ^ universal.mNegated;
						if (hasSuccessorWithoutConcept(repNode, role, opIt->getData(), propagatedNegation)) {
							mUndecidedMergeConceptCodeCounts[-5000] = mUndecidedMergeConceptCodeCounts.value(-5000, 0) + 1;
							return FAST_UNDECIDED;
						}
						for (CConcept* selfConcept : selfArray) {
							if (isSelfLoopOnRole(selfConcept, role) && !presence.has(opIt->getData(), propagatedNegation)) {
								mUndecidedMergeConceptCodeCounts[-6000] = mUndecidedMergeConceptCodeCounts.value(-6000, 0) + 1;
								return FAST_UNDECIDED;
							}
						}
					}
				}
				// and the node's universal restrictions reach the successors and self loops of the base
				for (const QPair<CConcept*,bool>& universal : universalArray) {
					CRole* role = universal.first->getRole();
					CSortedNegLinker<CConcept*>* opLinker = universal.first->getOperandList();
					if (!role || !opLinker) {
						return FAST_UNDECIDED;
					}
					for (CSortedNegLinker<CConcept*>* opIt = opLinker; opIt; opIt = opIt->getNext()) {
						bool propagatedNegation = opIt->isNegated() ^ universal.second;
						for (CIndividualSaturationProcessNode* partNode : base->mPartNodeList) {
							if (hasSuccessorWithoutConcept(partNode, role, opIt->getData(), propagatedNegation)) {
								mUndecidedMergeConceptCodeCounts[-5000] = mUndecidedMergeConceptCodeCounts.value(-5000, 0) + 1;
								return FAST_UNDECIDED;
							}
						}
						for (CConcept* selfConcept : base->mSelfConceptSet) {
							if (isSelfLoopOnRole(selfConcept, role) && !presence.has(opIt->getData(), propagatedNegation)) {
								mUndecidedMergeConceptCodeCounts[-6000] = mUndecidedMergeConceptCodeCounts.value(-6000, 0) + 1;
								return FAST_UNDECIDED;
							}
						}
					}
				}
				return FAST_NOT_SUBSUMED;
			}



			CSaturationSubsumptionDecider::Verdict CSaturationSubsumptionDecider::decideConjunctionSatisfiable(CIndividualSaturationProcessNode* baseNode, bool reliable, const QList<CConcept*>& conjuncts) {
				if (!reliable) {
					return UNDECIDED;
				}
				const CMergeState* base = getConjunctionMergeBase(conjuncts);
				if (!base) {
					return UNDECIDED;
				}
				if (base->mClashed) {
					// the conjunction has no instance on its own, so nothing has a common instance with it
					return SUBSUMED;
				}
				FastVerdict fastVerdict = decideConjunctionSatisfiableFast(baseNode, base);
				if (fastVerdict != FAST_FALLBACK) {
					++mFastDecisionCount;
					return fastVerdict == FAST_SUBSUMED ? SUBSUMED : fastVerdict == FAST_NOT_SUBSUMED ? NOT_SUBSUMED : UNDECIDED;
				}
				++mClosureDecisionCount;
				return decideConjunctionSatisfiableClosure(baseNode, base);
			}



			CSaturationSubsumptionDecider::Verdict CSaturationSubsumptionDecider::decideConjunctionSatisfiableClosure(CIndividualSaturationProcessNode* baseNode, const CMergeState* base) {
				// The merged label starts as the union of the saturated labels of the parts, and is
				// closed under the rules the saturation applies at a node: a conjunction adds its
				// operands, an implication whose triggers are all present adds its conclusion, and a
				// named class that is derived brings its saturation along. The parts' successors are
				// the successors of the merged node. The closure clashes if a concept ends up positive
				// and negated, which is how an absorbed disjointness surfaces; otherwise the closure
				// with the successors is a model unless a universal restriction reaches a successor of
				// another part that lacks its operand, or a construct that only the tableau handles is
				// involved, in which case nothing is decided.
				CIndividualSaturationProcessNode* repNode = getRepresentativeNode(baseNode);
				CSaturationConceptDataItem* satConDataItem = (CSaturationConceptDataItem*)repNode->getSaturationConceptReferenceLinking();
				CConcept* baseConcept = satConDataItem ? satConDataItem->getSaturationConcept() : nullptr;
				if (!baseConcept || !baseConcept->hasClassName()) {
					// the node of the sub class candidate is always that of a named class
					return UNDECIDED;
				}
				CMergeState state(base);
				bool decided = addMergeNamedClass(state, baseConcept);
				if (decided && !state.mClashed) {
					decided = closeMergeState(state);
				}
				if (state.mClashed) {
					// everything in the closure is entailed for the conjunction, so a clash is a proof
					return SUBSUMED;
				}
				if (!decided) {
					return UNDECIDED;
				}
				// a universal restriction reaches the successors of the other parts, and the derived ones
				QList<CIndividualSaturationProcessNode*> partNodeList = base->mPartNodeList + state.mPartNodeList;
				QSet<CConcept*> selfConceptSet = base->mSelfConceptSet + state.mSelfConceptSet;
				for (cint64 listIndex = 0; listIndex < 2; ++listIndex) {
					const QList<CMergeUniversal>& universalList = listIndex == 0 ? base->mUniversalList : state.mUniversalList;
					for (const CMergeUniversal& universal : universalList) {
						CRole* role = universal.mConcept->getRole();
						CSortedNegLinker<CConcept*>* opLinker = universal.mConcept->getOperandList();
						if (!role || !opLinker) {
							mUndecidedMergeConceptCodeCounts[-4000] = mUndecidedMergeConceptCodeCounts.value(-4000, 0) + 1;
							return UNDECIDED;
						}
						for (CRole* derivedRole : state.mDerivedSuccessorRoleSet) {
							if (derivedRole == role || derivedRole->getRoleTag() == role->getRoleTag()) {
								mUndecidedMergeConceptCodeCounts[-8000] = mUndecidedMergeConceptCodeCounts.value(-8000, 0) + 1;
								return UNDECIDED;
							}
							for (CSortedNegLinker<CRole*>* superIt = derivedRole->getIndirectSuperRoleList(); superIt; superIt = superIt->getNext()) {
								if (!superIt->isNegated() && (superIt->getData() == role || superIt->getData()->getRoleTag() == role->getRoleTag())) {
									mUndecidedMergeConceptCodeCounts[-8000] = mUndecidedMergeConceptCodeCounts.value(-8000, 0) + 1;
									return UNDECIDED;
								}
							}
						}
						for (CIndividualSaturationProcessNode* partNode : partNodeList) {
							if (partNode == universal.mSourceNode) {
								continue;
							}
							if (listIndex == 0 && base->hasPartNode(partNode)) {
								// the base is consistent with its own successors already
								continue;
							}
							for (CSortedNegLinker<CConcept*>* opIt = opLinker; opIt; opIt = opIt->getNext()) {
								// a negated existential restriction propagates the negated filler
								bool propagatedNegation = opIt->isNegated() ^ universal.mNegated;
								if (hasSuccessorWithoutConcept(partNode, role, opIt->getData(), propagatedNegation)) {
									mUndecidedMergeConceptCodeCounts[-5000] = mUndecidedMergeConceptCodeCounts.value(-5000, 0) + 1;
									return UNDECIDED;
								}
							}
						}
						// a self loop makes the merged node its own successor
						for (CConcept* selfConcept : selfConceptSet) {
							if (isSelfLoopOnRole(selfConcept, role)) {
								for (CSortedNegLinker<CConcept*>* opIt = opLinker; opIt; opIt = opIt->getNext()) {
									bool propagatedNegation = opIt->isNegated() ^ universal.mNegated;
									bool present = propagatedNegation ? state.hasNegative(opIt->getData()) : state.hasPositive(opIt->getData());
									if (!present) {
										mUndecidedMergeConceptCodeCounts[-6000] = mUndecidedMergeConceptCodeCounts.value(-6000, 0) + 1;
										return UNDECIDED;
									}
								}
							}
						}
					}
				}
				return NOT_SUBSUMED;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
