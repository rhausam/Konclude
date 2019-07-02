/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CExpressionDataBoxMapping.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CExpressionDataBoxMapping::CExpressionDataBoxMapping(COntologyContext* ontoContext) : mConstructFlags(ontoContext) {
				mOntoContext = ontoContext;
				CMemoryAllocationManager* memAllocMan = CContext::getMemoryAllocationManager(mOntoContext);

				mActiveEntityCountVector = CObjectParameterizingAllocator< CActiveEntityCountVector,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mClassTermConceptHash = CObjectParameterizingAllocator< CBUILDHASH<CClassTermExpression*,CConcept*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mConceptClassTermHash = CObjectParameterizingAllocator< CBUILDHASH<CConcept*,CClassTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mObjPropTermRoleHash = CObjectParameterizingAllocator< CBUILDHASH<CObjectPropertyTermExpression*,CRole*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mRoleObjPropTermHash = CObjectParameterizingAllocator< CBUILDHASH<CRole*,CObjectPropertyTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mIndividulTermIndiHash = CObjectParameterizingAllocator< CBUILDHASH<CIndividualTermExpression*,CIndividual*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mIndiIndividulTermHash = CObjectParameterizingAllocator< CBUILDHASH<CIndividual*,CIndividualTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mUpdateClassAxiomHash = CObjectParameterizingAllocator< CBUILDHASH<CClassAxiomExpression*,bool>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mUpdateObjectPropertyAxiomHash = CObjectParameterizingAllocator< CBUILDHASH<CObjectPropertyAxiomExpression*,bool>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mUpdateAssertionAxiomHash = CObjectParameterizingAllocator< CBUILDHASH<CAssertionAxiomExpression*,bool>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mClassTermClassAxiomSet = CObjectParameterizingAllocator< CBUILDSET< QPair<CClassTermExpression*,CClassAxiomExpression*> >,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mClassTermClassAxiomHash = CObjectParameterizingAllocator< CBUILDHASH<CClassTermExpression*,CClassAxiomExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mObjPropTermObjPropAxiomSet = CObjectParameterizingAllocator< CBUILDSET< QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*> >,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mObjPropTermObjPropAxiomHash = CObjectParameterizingAllocator< CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mIndiTermAssertionAxiomSet = CObjectParameterizingAllocator< CBUILDSET< QPair<CIndividualTermExpression*,CAssertionAxiomExpression*> >,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mIndiTermAssertionAxiomHash = CObjectParameterizingAllocator< CBUILDHASH<CIndividualTermExpression*,CAssertionAxiomExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mTopRebuildClassTermExpressionsSet = CObjectParameterizingAllocator< CBUILDSET<CClassTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mTopRebuildObjectPropertyTermExpressionsSet = CObjectParameterizingAllocator< CBUILDSET<CObjectPropertyTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mObjPropTermRoleChainHash = CObjectParameterizingAllocator< CBUILDHASH<CObjectPropertyAxiomExpression*,CRoleChain*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mRoleChainObjPropTermHash = CObjectParameterizingAllocator< CBUILDHASH<CRoleChain*,CObjectPropertyAxiomExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mBuildedConceptSet = CObjectParameterizingAllocator< CBUILDSET<CClassTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mBuildedRoleSet = CObjectParameterizingAllocator< CBUILDSET<CObjectPropertyTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mBuildedIndividualSet = CObjectParameterizingAllocator< CBUILDSET<CIndividualTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
			
				mBuildedConceptList = CObjectParameterizingAllocator< CBUILDLIST<CClassTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mBuildedRoleList = CObjectParameterizingAllocator< CBUILDLIST<CObjectPropertyTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mBuildedIndividualList = CObjectParameterizingAllocator< CBUILDLIST<CIndividualTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mExpressionBuildHash = CObjectParameterizingAllocator< CBUILDHASH<CExpressionHasher,CBuildExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mExpressionBuildListContainer = CObjectParameterizingAllocator< CBUILDLIST<CBuildExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);
				mInverseObjectPropertyHash = CObjectParameterizingAllocator< CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*>,CContext* >::allocateAndConstructAndParameterize(memAllocMan,mOntoContext);

				mLastActiveCountedEntityAxiom = 0;

				mLastProcessedChangedAxiom = 0;
				mLastProcessedBuildIndividual = 0;
				mLastProcessedBuildConcept = 0;
				mLastProcessedBuildRole = 0;

				mLastProcessedInverseProperty = 0;
				mLastProcessedExpression = 0;

				mLastBuildedIndividual = 0;
				mLastBuildedConcept = 0;
				mLastBuildedRole = 0;
			}

			CExpressionDataBoxMapping::~CExpressionDataBoxMapping() {
				CMemoryAllocationManager* memAllocMan = CContext::getMemoryAllocationManager(mOntoContext);
				COPADestroyAndRelease(mClassTermConceptHash,memAllocMan);
				COPADestroyAndRelease(mConceptClassTermHash,memAllocMan);
				COPADestroyAndRelease(mObjPropTermRoleHash,memAllocMan);
				COPADestroyAndRelease(mRoleObjPropTermHash,memAllocMan);
				COPADestroyAndRelease(mIndividulTermIndiHash,memAllocMan);
				COPADestroyAndRelease(mIndiIndividulTermHash,memAllocMan);

				COPADestroyAndRelease(mUpdateClassAxiomHash,memAllocMan);
				COPADestroyAndRelease(mUpdateObjectPropertyAxiomHash,memAllocMan);
				COPADestroyAndRelease(mUpdateAssertionAxiomHash,memAllocMan);

				COPADestroyAndRelease(mObjPropTermObjPropAxiomSet,memAllocMan);
				COPADestroyAndRelease(mObjPropTermObjPropAxiomHash,memAllocMan);

				COPADestroyAndRelease(mIndiTermAssertionAxiomSet,memAllocMan);
				COPADestroyAndRelease(mIndiTermAssertionAxiomHash,memAllocMan);

				COPADestroyAndRelease(mTopRebuildClassTermExpressionsSet,memAllocMan);
				COPADestroyAndRelease(mTopRebuildObjectPropertyTermExpressionsSet,memAllocMan);

				COPADestroyAndRelease(mClassTermClassAxiomSet,memAllocMan);
				COPADestroyAndRelease(mClassTermClassAxiomHash,memAllocMan);

				COPADestroyAndRelease(mObjPropTermRoleChainHash,memAllocMan);
				COPADestroyAndRelease(mRoleChainObjPropTermHash,memAllocMan);

				COPADestroyAndRelease(mBuildedConceptSet,memAllocMan);
				COPADestroyAndRelease(mBuildedRoleSet,memAllocMan);
				COPADestroyAndRelease(mBuildedIndividualSet,memAllocMan);

				COPADestroyAndRelease(mBuildedIndividualList,memAllocMan);
				COPADestroyAndRelease(mBuildedRoleList,memAllocMan);
				COPADestroyAndRelease(mBuildedConceptList,memAllocMan);

				COPADestroyAndRelease(mExpressionBuildHash,memAllocMan);
				COPADestroyAndRelease(mExpressionBuildListContainer,memAllocMan);
				COPADestroyAndRelease(mInverseObjectPropertyHash,memAllocMan);
			}

			CExpressionDataBoxMapping* CExpressionDataBoxMapping::referenceDataBoxMapping(CExpressionDataBoxMapping* dataBoxMapping) {
				*mClassTermConceptHash = *dataBoxMapping->mClassTermConceptHash;
				*mConceptClassTermHash = *dataBoxMapping->mConceptClassTermHash;
				*mObjPropTermRoleHash = *dataBoxMapping->mObjPropTermRoleHash;
				*mRoleObjPropTermHash = *dataBoxMapping->mRoleObjPropTermHash;
				*mIndividulTermIndiHash = *dataBoxMapping->mIndividulTermIndiHash;
				*mIndiIndividulTermHash = *dataBoxMapping->mIndiIndividulTermHash;

				mActiveEntityCountVector->initActiveEntityCountVector(dataBoxMapping->mActiveEntityCountVector);

				mUpdateClassAxiomHash->clear();
				mUpdateObjectPropertyAxiomHash->clear();
				mUpdateAssertionAxiomHash->clear();

				//*mUpdateClassAxiomHash = *dataBoxMapping->mUpdateClassAxiomHash;
				//*mUpdateObjectPropertyAxiomHash = *dataBoxMapping->mUpdateObjectPropertyAxiomHash;

				*mClassTermClassAxiomSet = *dataBoxMapping->mClassTermClassAxiomSet;
				*mClassTermClassAxiomHash = *dataBoxMapping->mClassTermClassAxiomHash;


				*mObjPropTermObjPropAxiomSet = *dataBoxMapping->mObjPropTermObjPropAxiomSet;
				*mObjPropTermObjPropAxiomHash = *dataBoxMapping->mObjPropTermObjPropAxiomHash;

				*mIndiTermAssertionAxiomSet = *dataBoxMapping->mIndiTermAssertionAxiomSet;
				*mIndiTermAssertionAxiomHash = *dataBoxMapping->mIndiTermAssertionAxiomHash;

				*mTopRebuildClassTermExpressionsSet = *dataBoxMapping->mTopRebuildClassTermExpressionsSet;
				*mTopRebuildObjectPropertyTermExpressionsSet = *dataBoxMapping->mTopRebuildObjectPropertyTermExpressionsSet;

				*mObjPropTermRoleChainHash = *dataBoxMapping->mObjPropTermRoleChainHash;
				*mRoleChainObjPropTermHash = *dataBoxMapping->mRoleChainObjPropTermHash;

				mLastActiveCountedEntityAxiom = dataBoxMapping->mLastActiveCountedEntityAxiom;

				mLastProcessedChangedAxiom = dataBoxMapping->mLastProcessedChangedAxiom;
				mLastProcessedBuildIndividual = dataBoxMapping->mLastProcessedBuildIndividual;
				mLastProcessedBuildConcept = dataBoxMapping->mLastProcessedBuildConcept;
				mLastProcessedBuildRole = dataBoxMapping->mLastProcessedBuildRole;

				mLastProcessedInverseProperty = dataBoxMapping->mLastProcessedInverseProperty;
				mLastProcessedExpression = dataBoxMapping->mLastProcessedExpression;

				mLastBuildedIndividual = dataBoxMapping->mLastBuildedIndividual;
				mLastBuildedConcept = dataBoxMapping->mLastBuildedConcept;
				mLastBuildedRole = dataBoxMapping->mLastBuildedRole;

				*mBuildedIndividualList = *dataBoxMapping->mBuildedIndividualList;
				*mBuildedConceptList = *dataBoxMapping->mBuildedConceptList;
				*mBuildedRoleList = *dataBoxMapping->mBuildedRoleList;

				*mBuildedIndividualSet = *dataBoxMapping->mBuildedIndividualSet;
				*mBuildedRoleSet = *dataBoxMapping->mBuildedRoleSet;
				*mBuildedConceptSet = *dataBoxMapping->mBuildedConceptSet;

				*mInverseObjectPropertyHash = *dataBoxMapping->mInverseObjectPropertyHash;
				*mExpressionBuildHash = *dataBoxMapping->mExpressionBuildHash;
				*mExpressionBuildListContainer = *dataBoxMapping->mExpressionBuildListContainer;

				mConstructFlags.referenceBuildConstructFlags(&dataBoxMapping->mConstructFlags);
				return this;
			}


			CActiveEntityCountVector* CExpressionDataBoxMapping::getActiveEntityVector() {
				return mActiveEntityCountVector;
			}


			COntologyBuildConstructFlags* CExpressionDataBoxMapping::getBuildConstructFlags() {
				return &mConstructFlags;
			}
			CBUILDHASH<CClassTermExpression*,CConcept*>* CExpressionDataBoxMapping::getClassTermConceptMappingHash() {
				return mClassTermConceptHash;
			}

			CBUILDHASH<CConcept*,CClassTermExpression*>* CExpressionDataBoxMapping::getConceptClassTermMappingHash() {
				return mConceptClassTermHash;
			}

			CBUILDHASH<CObjectPropertyTermExpression*,CRole*>* CExpressionDataBoxMapping::getObjectPropertyTermRoleMappingHash() {
				return mObjPropTermRoleHash;
			}

			CBUILDHASH<CRole*,CObjectPropertyTermExpression*>* CExpressionDataBoxMapping::getRoleObjectPropertyTermMappingHash() {
				return mRoleObjPropTermHash;
			}


			CBUILDHASH<CIndividualTermExpression*,CIndividual*>* CExpressionDataBoxMapping::getIndividulTermIndiMappingHash() {
				return mIndividulTermIndiHash;
			}

			CBUILDHASH<CIndividual*,CIndividualTermExpression*>* CExpressionDataBoxMapping::getIndiIndividulTermMappingHash() {
				return mIndiIndividulTermHash;
			}


			CBUILDHASH<CClassAxiomExpression*,bool>* CExpressionDataBoxMapping::getUpdatedClassAxiomTellOrRetractHash() {
				return mUpdateClassAxiomHash;
			}


			CBUILDHASH<CObjectPropertyAxiomExpression*,bool>* CExpressionDataBoxMapping::getUpdateObjectPropertyAxiomHash() {
				return mUpdateObjectPropertyAxiomHash;
			}

			CBUILDHASH<CAssertionAxiomExpression*,bool>* CExpressionDataBoxMapping::getUpdateAssertionAxiomHash() {
				return mUpdateAssertionAxiomHash;
			}


			CBUILDSET< QPair<CClassTermExpression*,CClassAxiomExpression*> >* CExpressionDataBoxMapping::getClassTermExpressionClassAxiomExpressionSet() {
				return mClassTermClassAxiomSet;
			}

			CBUILDHASH<CClassTermExpression*,CClassAxiomExpression*>* CExpressionDataBoxMapping::getClassTermExpressionClassAxiomExpressionHash() {
				return mClassTermClassAxiomHash;
			}



			CBUILDSET< QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*> >* CExpressionDataBoxMapping::getObjectPropertyTermObjectPropertyAxiomSet() {
				return mObjPropTermObjPropAxiomSet;
			}

			CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>* CExpressionDataBoxMapping::getObjectPropertyTermObjectPropertyAxiomHash() {
				return mObjPropTermObjPropAxiomHash;
			}


			CBUILDSET< QPair<CIndividualTermExpression*,CAssertionAxiomExpression*> >* CExpressionDataBoxMapping::getIndividualTermAssertionAxiomSet() {
				return mIndiTermAssertionAxiomSet;
			}

			CBUILDHASH<CIndividualTermExpression*,CAssertionAxiomExpression*>* CExpressionDataBoxMapping::getIndividualTermAssertionAxiomHash() {
				return mIndiTermAssertionAxiomHash;
			}



			CBUILDSET<CClassTermExpression*>* CExpressionDataBoxMapping::getTopRebuildClassTermExpressionsSet() {
				return mTopRebuildClassTermExpressionsSet;
			}

			CBUILDSET<CObjectPropertyTermExpression*>* CExpressionDataBoxMapping::getTopRebuildObjectPropertyTermExpressionsSet() {
				return mTopRebuildObjectPropertyTermExpressionsSet;
			}

			CBUILDHASH<CObjectPropertyAxiomExpression*,CRoleChain*>* CExpressionDataBoxMapping::getObjectPropertyTermRoleChainHash() {
				return mObjPropTermRoleChainHash;
			}

			CBUILDHASH<CRoleChain*,CObjectPropertyAxiomExpression*>* CExpressionDataBoxMapping::getRoleChainObjectPropertyTermHash() {
				return mRoleChainObjPropTermHash;
			}



			CBUILDSET<CClassTermExpression*>* CExpressionDataBoxMapping::getBuildedConceptSet() {
				return mBuildedConceptSet;
			}

			CBUILDSET<CObjectPropertyTermExpression*>* CExpressionDataBoxMapping::getBuildedRoleSet() {
				return mBuildedRoleSet;
			}

			CBUILDSET<CIndividualTermExpression*>* CExpressionDataBoxMapping::getBuildedIndividualSet() {
				return mBuildedIndividualSet;
			}


			cint64 CExpressionDataBoxMapping::getLastActiveCountedEntityAxiom() {
				return mLastActiveCountedEntityAxiom;
			}

			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastActiveCountedEntityAxiom(cint64 lastCounted) {
				mLastActiveCountedEntityAxiom = lastCounted;
				return this;
			}



			cint64 CExpressionDataBoxMapping::getLastProcessedChangedAxiom() {
				return mLastProcessedChangedAxiom;
			}

			cint64 CExpressionDataBoxMapping::getLastBuildedIndividual() {
				return mLastBuildedIndividual;
			}

			cint64 CExpressionDataBoxMapping::getLastBuildedConcept() {
				return mLastBuildedConcept;
			}

			cint64 CExpressionDataBoxMapping::getLastBuildedRole() {
				return mLastBuildedRole;
			}


			cint64 CExpressionDataBoxMapping::getLastProcessedBuildIndividual() {
				return mLastProcessedBuildIndividual;
			}

			cint64 CExpressionDataBoxMapping::getLastProcessedBuildConcept() {
				return mLastProcessedBuildConcept;
			}

			cint64 CExpressionDataBoxMapping::getLastProcessedBuildRole() {
				return mLastProcessedBuildRole;
			}

			cint64 CExpressionDataBoxMapping::getLastProcessedInverseProperty() {
				return mLastProcessedInverseProperty;
			}

			cint64 CExpressionDataBoxMapping::getLastProcessedExpression() {
				return mLastProcessedExpression;
			}

			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastProcessedChangedAxiom(cint64 lastBuilded) {
				mLastProcessedChangedAxiom = lastBuilded;
				return this;
			}

			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastProcessedBuildIndividual(cint64 lastBuilded) {
				mLastProcessedBuildIndividual = lastBuilded;
				return this;
			}

			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastProcessedBuildConcept(cint64 lastBuilded) {
				mLastProcessedBuildConcept = lastBuilded;
				return this;
			}


			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastProcessedBuildRole(cint64 lastBuilded) {
				mLastProcessedBuildRole = lastBuilded;
				return this;
			}


			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastProcessedInverseProperty(cint64 lastBuilded) {
				mLastProcessedInverseProperty = lastBuilded;
				return this;
			}

			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastProcessedExpression(cint64 lastBuilded) {
				mLastProcessedExpression = lastBuilded;
				return this;
			}

			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastBuildedIndividual(cint64 lastBuilded) {
				mLastBuildedIndividual = lastBuilded;
				return this;
			}

			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastBuildedConcept(cint64 lastBuilded) {
				mLastBuildedConcept = lastBuilded;
				return this;
			}


			CExpressionDataBoxMapping* CExpressionDataBoxMapping::setLastBuildedRole(cint64 lastBuilded) {
				mLastBuildedRole = lastBuilded;
				return this;
			}


			CBUILDLIST<CClassTermExpression*>* CExpressionDataBoxMapping::getBuildedConceptList() {
				return mBuildedConceptList;
			}

			CBUILDLIST<CObjectPropertyTermExpression*>* CExpressionDataBoxMapping::getBuildedRoleList() {
				return mBuildedRoleList;
			}

			CBUILDLIST<CIndividualTermExpression*>* CExpressionDataBoxMapping::getBuildedIndividualList() {
				return mBuildedIndividualList;
			}

			CBUILDHASH<CExpressionHasher,CBuildExpression*>* CExpressionDataBoxMapping::getStructuralExpressionBuildHash() {
				return mExpressionBuildHash;
			}

			CBUILDLIST<CBuildExpression*>* CExpressionDataBoxMapping::getExpressionBuildListContainer() {
				return mExpressionBuildListContainer;
			}

			CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*>* CExpressionDataBoxMapping::getInverseObjectPropertyHash() {
				return mInverseObjectPropertyHash;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
