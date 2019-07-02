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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONANALYSECONTEXT_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONANALYSECONTEXT_H

// Libraries includes


// Namespace includes
#include "EvaluationSettings.h"


// Other includes
#include "Config/CConfiguration.h"
#include "Config/CConfigDataReader.h"

#include "Context/CContext.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;
	using namespace Context;

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\class		CReasonerEvaluationAnalyseContext
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationAnalyseContext : public CContext {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationAnalyseContext(CConfiguration* configuration = nullptr, const QString& additionalTitleString = QString(), const QString& additionalFileString = QString());

					//! Destructor
					virtual ~CReasonerEvaluationAnalyseContext();

					CMemoryAllocationManager* getMemoryAllocationManager();

					CConfiguration* getConfiguration();

					CReasonerEvaluationGNUPlotPlotter* getGNUPlotPlotter();
					CReasonerEvaluationHighchartPlotter* getHighchartPlotter();

					QString getAdditionalTitleString();
					QString getAdditionalFileString();


					bool isGNUPlottingActivated();
					bool isHighchartPlottingActivated();


				// protected methods
				protected:

				// protected variables
				protected:
					CConfiguration* mConfiguration;
					CReasonerEvaluationGNUPlotPlotter* mGNUPlotPlotter;
					CReasonerEvaluationHighchartPlotter* mHighchartPlotter;
					QString mAdditionalTitleString;
					QString mAdditionalFileString;
					bool mGNUPlot;
					bool mHighchartPlot;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONANALYSECONTEXT_H
