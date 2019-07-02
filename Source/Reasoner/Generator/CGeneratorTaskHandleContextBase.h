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

#ifndef KONCLUDE_REASONER_GENERATOR_CGENERATORTASKHANDLECONTEXTBASE_H
#define KONCLUDE_REASONER_GENERATOR_CGENERATORTASKHANDLECONTEXTBASE_H

// Libraries includes


// Namespace includes

// Other includes
#include "Scheduler/CTaskHandleContext.h"
#include "Scheduler/CTaskHandleMemoryPoolAllocationManager.h"

#include "Utilities/Memory/CNewAllocationMemoryPoolProvider.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Scheduler;
	using namespace Utilities::Memory;

	namespace Reasoner {

		namespace Generator {

			/*! 
			 *
			 *		\class		CGeneratorTaskHandleContextBase
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CGeneratorTaskHandleContextBase : public CTaskHandleContext {
				// public methods
				public:
					//! Constructor
					CGeneratorTaskHandleContextBase();

					//! Destructor
					virtual ~CGeneratorTaskHandleContextBase();

					virtual CTaskHandleMemoryAllocationManager* getTaskHandleMemoryAllocationManager();
					virtual CMemoryAllocationManager* getMemoryAllocationManager();


				// protected methods
				protected:

				// protected variables
				protected:					
					CTaskHandleMemoryPoolAllocationManager* mMemMan;
					CNewAllocationMemoryPoolProvider* mMemPoolProv;

				// private methods
				private:

				// private variables
				private:


			};

		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_GENERATOR_CGENERATORTASKHANDLECONTEXTBASE_H
