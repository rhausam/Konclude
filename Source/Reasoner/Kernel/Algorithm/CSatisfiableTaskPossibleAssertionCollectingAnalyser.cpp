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

#include "CSatisfiableTaskPossibleAssertionCollectingAnalyser.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {


				CSatisfiableTaskPossibleAssertionCollectingAnalyser::CSatisfiableTaskPossibleAssertionCollectingAnalyser() {
				}



				bool CSatisfiableTaskPossibleAssertionCollectingAnalyser::readCalculationConfig(CCalculationConfigurationExtension *config) {					
					if (config) {
						return true;
					}
					return false;
				}


				CIndividualProcessNode* CSatisfiableTaskPossibleAssertionCollectingAnalyser::getCorrectedIndividualNode(cint64 baseIndiID, CIndividualProcessNodeVector* indiNodeVec) {
					CIndividualProcessNode* indi = indiNodeVec->getData(baseIndiID);
					while (indi->hasMergedIntoIndividualNodeID()) {
						indi = indiNodeVec->getData(indi->getMergedIntoIndividualNodeID());
					}
					return indi;
				}


				bool CSatisfiableTaskPossibleAssertionCollectingAnalyser::analyseSatisfiableTask(CSatisfiableCalculationTask* statCalcTask, CCalculationAlgorithmContext* calcAlgContext) {
					CSatisfiableTaskRealizationPossibleAssertionCollectingAdapter* possAssCollAdapter = statCalcTask->getPossibleAssertionCollectionAdapter();
					if (possAssCollAdapter) {
						CPossibleAssertionsCollectionSet* possAssCollSet = possAssCollAdapter->getPossibleAssertionsCollectionSet();
						CConcreteOntology* ontology = possAssCollAdapter->getTestingOntology();

						if (possAssCollSet) {

							CSatisfiableCalculationTask* cachedDetTask = getCachedDeterministicCompletionGraphTask(calcAlgContext);
							CIndividualProcessNodeVector* detCachedIndiNodeVec = nullptr;
							if (cachedDetTask) {
								detCachedIndiNodeVec = cachedDetTask->getProcessingDataBox()->getIndividualProcessNodeVector();
							}

							CBOXSET<CConcept*>* eqNonCanSet = ontology->getTBox()->getEquivalentConceptNonCandidateSet(false);

							QSet< QPair<CIndividual*,CConcept*> > possConAssSet;
							cint64 maxDetBranchTag = statCalcTask->getProcessingDataBox()->getMaximumDeterministicBranchTag();

							CProcessingDataBox* procDataBox = calcAlgContext->getProcessingDataBox();
							CIndividualProcessNodeVector* indiNodeVec = procDataBox->getIndividualProcessNodeVector();
							CIndividualVector* indiVec = procDataBox->getOntology()->getABox()->getIndividualVector();
							cint64 indiMinIdx = indiNodeVec->getItemMinIndex();
							for (cint64 i = indiMinIdx; i <= 0; ++i) {
								CIndividualProcessNode* indiNode = indiNodeVec->getData(i);
								CIndividualProcessNode* cachedDetIndiNode = nullptr;
								if (detCachedIndiNodeVec) {
									cachedDetIndiNode = detCachedIndiNodeVec->getData(i);
								}
								if (indiNode && indiNode != cachedDetIndiNode && indiNode->getNominalIndividual()) {
									if (!cachedDetIndiNode || !indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHED) || indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALID | CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED)) {
										CReapplyConceptLabelSet* conLabelSet = indiNode->getReapplyConceptLabelSet(false);
										if (conLabelSet) {

											CIndividual* individual = indiNode->getNominalIndividual();
											for (CConceptDescriptor* conDesIt = conLabelSet->getAddingSortedConceptDescriptionLinker(); conDesIt; conDesIt = conDesIt->getNext()) {
												CDependencyTrackPoint* depTrackPoint = conDesIt->getDependencyTrackPoint();
												CConcept* concept = conDesIt->getConcept();
												bool conNegation = conDesIt->isNegated();
												if (!depTrackPoint || depTrackPoint->getBranchingTag() > maxDetBranchTag) {
													if (!conNegation && concept && concept->hasClassName()) {
														possConAssSet.insert( QPair<CIndividual*,CConcept*>(individual,concept) );
													}
												}
												if (concept->getOperatorCode() == CCEQCAND) {
													CConcept* eqConcept = concept->getOperandList()->getData();
													bool negation = false;
													if (!conLabelSet->containsConcept(eqConcept,negation)) {
														possConAssSet.insert( QPair<CIndividual*,CConcept*>(individual,eqConcept) );
													}
												}
											}

											if (eqNonCanSet) {
												for (CBOXSET<CConcept*>::const_iterator it = eqNonCanSet->constBegin(), itEnd = eqNonCanSet->constEnd(); it != itEnd; ++it) {
													CConcept* eqConcept(*it);
													bool negation = false;
													if (!conLabelSet->containsConcept(eqConcept,negation)) {
														possConAssSet.insert( QPair<CIndividual*,CConcept*>(individual,eqConcept) );
													}
												}
											}

											CIndividualMergingHash* indiMergingHash = indiNode->getIndividualMergingHash(false);
											if (indiMergingHash) {

												for (CIndividualMergingHash::const_iterator indiIt = indiMergingHash->constBegin(), indiItEnd = indiMergingHash->constEnd(); indiIt != indiItEnd; ++indiIt) {

													if (indiIt.value().isMergedWithIndividual()) {
														cint64 indiIDIt = indiIt.key();
														CIndividual* mergedIndividual = indiVec->getData(indiIDIt);
														CDependencyTrackPoint* mergeDepTrackPoint = indiIt.value().getDependencyTrackPoint();
														bool nonDetMerged = true;
														if (mergeDepTrackPoint && mergeDepTrackPoint->getBranchingTag() <= maxDetBranchTag) {
															nonDetMerged = false;
														}
														for (CConceptDescriptor* conDesIt = conLabelSet->getAddingSortedConceptDescriptionLinker(); conDesIt; conDesIt = conDesIt->getNext()) {
															CDependencyTrackPoint* depTrackPoint = conDesIt->getDependencyTrackPoint();
															CConcept* concept = conDesIt->getConcept();
															bool conNegation = conDesIt->isNegated();
															if (nonDetMerged || !depTrackPoint || depTrackPoint->getBranchingTag() > maxDetBranchTag) {
																if (!conNegation && concept && concept->hasClassName()) {
																	possConAssSet.insert(QPair<CIndividual*, CConcept*>(mergedIndividual, concept));
																}
															}
															if (concept->getOperatorCode() == CCEQCAND) {
																CConcept* eqConcept = concept->getOperandList()->getData();
																bool negation = false;
																if (!conLabelSet->containsConcept(eqConcept, negation)) {
																	possConAssSet.insert(QPair<CIndividual*, CConcept*>(mergedIndividual, eqConcept));
																}
															}
														}

														if (eqNonCanSet) {
															for (CBOXSET<CConcept*>::const_iterator it = eqNonCanSet->constBegin(), itEnd = eqNonCanSet->constEnd(); it != itEnd; ++it) {
																CConcept* eqConcept(*it);
																bool negation = false;
																if (!conLabelSet->containsConcept(eqConcept, negation)) {
																	possConAssSet.insert(QPair<CIndividual*, CConcept*>(mergedIndividual, eqConcept));
																}
															}
														}
													}

												}
											}

										}
									}
								}
							}

							if (!possConAssSet.isEmpty()) {	
								possAssCollSet->acquireSynchronisationLock();
								for (QSet< QPair<CIndividual*,CConcept*> >::const_iterator it = possConAssSet.constBegin(), itEnd = possConAssSet.constEnd(); it != itEnd; ++it) {
									const QPair<CIndividual*,CConcept*>& indiConPair(*it);
									possAssCollSet->addPossibleAssertion(indiConPair.first,indiConPair.second);
								}
								possAssCollSet->releaseSynchronisationLock();
							}
						}
					}
					return false;
				}



				CSatisfiableCalculationTask* CSatisfiableTaskPossibleAssertionCollectingAnalyser::getCachedDeterministicCompletionGraphTask(CCalculationAlgorithmContext* calcAlgContext) {
					CConcreteOntology* consOntology = calcAlgContext->getProcessingDataBox()->getOntology();
					if (consOntology) {
						CConsistence* consistence = consOntology->getConsistence();
						if (consistence) {
							CConsistenceData* consData = consistence->getConsistenceModelData();
							if (consData) {
								CConsistenceTaskData* consTaskData = dynamic_cast<CConsistenceTaskData*>(consData);
								if (consTaskData) {
									CSatisfiableCalculationTask* compGraphCalcTask = consTaskData->getDeterministicSatisfiableTask();				
									return compGraphCalcTask;
								}
							}
						}
					}
					return nullptr;
				}



			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
