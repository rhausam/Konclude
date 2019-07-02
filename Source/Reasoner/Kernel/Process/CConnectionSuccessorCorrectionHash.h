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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CCONNECTIONSUCCESSORCORRECTIONHASH_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CCONNECTIONSUCCESSORCORRECTIONHASH_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CIndividualLinkEdge.h"
#include "CProcessContext.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CConnectionSuccessorCorrectionHash
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CConnectionSuccessorCorrectionHash : public CPROCESSHASH<cint64,cint64> {
					// public methods
					public:
						//! Constructor
						CConnectionSuccessorCorrectionHash(CProcessContext* context = nullptr);

						//! Destructor
						virtual ~CConnectionSuccessorCorrectionHash();

						CConnectionSuccessorCorrectionHash* initConnectionSuccessorCorrectionHash(CConnectionSuccessorCorrectionHash* connSuccCorrHash);

						CConnectionSuccessorCorrectionHash* insertConnectionSuccessor(cint64 indiID);
						CConnectionSuccessorCorrectionHash* correctSuccessorConnection(cint64 indiID, cint64 correctedID);


					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mContext;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CCONNECTIONSUCCESSORCORRECTIONHASH_H
