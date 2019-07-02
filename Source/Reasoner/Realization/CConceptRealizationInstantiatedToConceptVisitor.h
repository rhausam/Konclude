/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_REALIZATION_CCONCEPTREALIZATIONINSTANTIATEDTOCONCEPTVISITOR_H
#define KONCLUDE_REASONER_REALIZATION_CCONCEPTREALIZATIONINSTANTIATEDTOCONCEPTVISITOR_H

// Libraries includes


// Namespace includes
#include "RealizationSettings.h"
#include "CConceptRealizationConceptVisitor.h"
#include "CConceptRealizationInstantiatedVisitor.h"
#include "CConceptRealization.h"

// Other includes
#include "Reasoner/Ontology/CConcept.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Realization {

			/*! 
			 *
			 *		\class		CConceptRealizationInstantiatedToConceptVisitor
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConceptRealizationInstantiatedToConceptVisitor : public CConceptRealizationConceptVisitor, public CConceptRealizationInstantiatedVisitor {
				// public methods
				public:
					//! Constructor
					CConceptRealizationInstantiatedToConceptVisitor();

					virtual bool visitType(CConceptInstantiatedItem* item, CConceptRealization* conRealization);

				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realization

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZATION_CCONCEPTREALIZATIONINSTANTIATEDTOCONCEPTVISITOR_H
