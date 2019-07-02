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

#ifndef KONCLUDE_REASONER_PREPROCESS_CSUBROLETRANSFORMATIONPREPROCESS_H
#define KONCLUDE_REASONER_PREPROCESS_CSUBROLETRANSFORMATIONPREPROCESS_H

// Libraries includes
#include <QSet>

// Namespace includes
#include "CConcreteOntologyPreProcess.h"
#include "CConceptRoleIndividualLocator.h"


// Other includes
#include "Reasoner/Ontology/CBoxContext.h"
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CRoleDependence.h"
#include "Reasoner/Ontology/CMBox.h"

#include "Utilities/CSortedNegLinker.hpp"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Preprocess {

			/*! 
			 *
			 *		\class		CSubroleTransformationPreProcess
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CSubroleTransformationPreProcess : public CConcreteOntologyPreProcess {
				// public methods
				public:
					//! Constructor
					CSubroleTransformationPreProcess();

					//! Destructor
					virtual ~CSubroleTransformationPreProcess();


					virtual CConcreteOntology *preprocess(CConcreteOntology *ontology, CPreProcessContext* context);

				// protected methods
				protected:

					void addIndirectSuperRoles(bool inverse, CRole *modifingSubRole, CSortedNegLinker<CRole *> *superRoleLinker, QSet< QPair<CRole*,bool> > *singleSuperRoleAddSet);

					void makeUniversalSuperRole();
					void makeUniversalConnectionIndividual();

					void makeDataSuperRole();

					void makeBottomObjectRoleDomain();
					void makeBottomDataRoleDomain();
					void makeBottomRoleDomain(CRole* role);

					bool hasUniversalRolePropagation();

				// private methods
				private:
					CSubroleTransformationPreProcess* collectInverseEquivalentRoles(CRole* role, bool negated, QSet< QPair<CRole*,bool> >* roleSet);
					CSubroleTransformationPreProcess* updateDirectSuperRoles(CRole* role, bool negated, QSet< QPair<CRole*,bool> >* roleSet);

					CRole* getLocalizedRole(CRole* role);
					CIndividual* getLocalizedIndividual(CIndividual* individual);

				// private variables
				private:
					CABox* mABox;
					CTBox* mTBox;
					CRBox* mRBox;
					CMBox* mMBox;
					CIndividualVector* mIndiVec;
					CRoleVector* mRolesVector;
					CConceptVector* mConceptVector;
					CRoleChainVector* mRoleChainVector;
					CConcreteOntology* mOntology;
					CRole* mBottomObjectRole;
					CRole* mTopObjectRole;
					CRole* mBottomDataRole;
					CRole* mTopDataRole;

					CBUILDHASH<CRole*,CObjectPropertyTermExpression*>* roleObjPropTermHash;

					CMemoryAllocationManager* mMemMan;

			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CSUBROLETRANSFORMATIONPREPROCESS_H