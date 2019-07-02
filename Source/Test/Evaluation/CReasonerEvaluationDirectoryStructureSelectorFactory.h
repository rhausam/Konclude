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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONDIRECTORYSTRUCTURESELECTORFACTORY_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONDIRECTORYSTRUCTURESELECTORFACTORY_H

// Libraries includes
#include <QString>
#include <QDir>


// Namespace includes
#include "EvaluationSettings.h"
#include "CReasonerEvaluationGroupRequestSelector.h"
#include "CReasonerEvaluationStringMatchGroupSelector.h"


// Other includes



// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\class		CReasonerEvaluationDirectoryStructureSelectorFactory
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationDirectoryStructureSelectorFactory {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationDirectoryStructureSelectorFactory(const QString& groupDirString, const QString& dirString, const QString& baseEvalName, const QString& baseOutputDir);


					CReasonerEvaluationGroupRequestSelector* getSelectors(CReasonerEvaluationGroupRequestSelector* prevSelector);


				// protected methods
				protected:
					CReasonerEvaluationGroupRequestSelector* getSelectors(CReasonerEvaluationGroupRequestSelector* prevSelector, const QString& dirString, const QList<CReasonerEvaluationStringMatchGroupSelector*>& subDirAddList);

				// protected variables
				protected:
					QString mGroupDirString;
					QString mDirectoryString;
					QString mBaseEvalName;
					QString mBaseOutputDir;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONDIRECTORYSTRUCTURESELECTORFACTORY_H
