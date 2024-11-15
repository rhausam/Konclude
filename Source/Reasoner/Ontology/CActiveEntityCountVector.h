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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CACTIVEENTITYCOUNTVECTOR_H
#define KONCLUDE_REASONER_ONTOLOGY_CACTIVEENTITYCOUNTVECTOR_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "CConcept.h"
#include "CBoxContext.h"
#include "CActiveEntityCountItem.h"
#include "CActiveEntityIterator.h"


// Other includes
#include "Utilities/Container/CDefaultDynamicReferenceVector.h"



// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CActiveEntityCountVector
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CActiveEntityCountVector {
				// public methods
				public:
					//! Constructor
					CActiveEntityCountVector(CContext* context = nullptr);

					CActiveEntityCountVector* initActiveEntityCountVector(CActiveEntityCountVector* vector);


					CActiveEntityCountVector* incActiveEntityCount(CExpressionEntity* entity, cint64 incCount = 1);
					CActiveEntityCountVector* decActiveEntityCount(CExpressionEntity* entity, cint64 decCount = 1);


					CActiveEntityCountItem* getActiveEntityCountItem(CExpressionEntity* entity);
					CActiveEntityCountVector* clearActiveEntityCount();

					bool increaseVectorSize(cint64 newSize);
					cint64 getVectorSize();

					CActiveEntityIterator getActiveEntityIterator();

				// protected methods
				protected:

				// protected variables
				protected:
					CContext* mContext;
					CActiveEntityCountItem* mCountVector;
					cint64 mVectorSize;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CACTIVEENTITYCOUNTVECTOR_H
