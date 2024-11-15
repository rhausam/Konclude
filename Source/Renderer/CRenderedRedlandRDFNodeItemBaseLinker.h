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
#ifdef KONCLUDE_REDLAND_INTEGRATION

#ifndef KONCLUDE_RENDERER_CRENDEREDREDLANDRDFNODEITEMBASELINKER_H
#define KONCLUDE_RENDERER_CRENDEREDREDLANDRDFNODEITEMBASELINKER_H

// Libraries includes

// Namespace includes
#include "RendererSettings.h"
#include "CRenderedRDFNodeItemBaseLinker.h"


// Other includes
#include "redland.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;

namespace Konclude {

	namespace Renderer {



		/*! 
		*
		*		\class		CRenderedRedlandRDFNodeItemBaseLinker
		*		\author		Andreas Steigmiller
		*		\version	0.1
		*		\brief		TODO
		*
		*/
		class CRenderedRedlandRDFNodeItemBaseLinker : public CRenderedRDFNodeItemBaseLinker {
			// public methods
			public:
				//! Constructor
				CRenderedRedlandRDFNodeItemBaseLinker(librdf_node* node = nullptr);

				CRenderedRedlandRDFNodeItemBaseLinker* setRedlandNode(librdf_node* node);
				librdf_node* getRedlandNode();

				CRenderedRedlandRDFNodeItemBaseLinker* clearRedlandNode();

			// protected methods
			protected:

			// protected variables
			protected:
				librdf_node* mNode;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Renderer

}; // end namespace Konclude

#endif // KONCLUDE_RENDERER_CRENDEREDREDLANDRDFNODEITEMBASELINKER_H

#endif // !KONCLUDE_REDLAND_INTEGRATION
