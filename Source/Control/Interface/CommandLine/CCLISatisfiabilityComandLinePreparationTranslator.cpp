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

#include "CCLISatisfiabilityComandLinePreparationTranslator.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace CommandLine {

				CCLISatisfiabilityComandLinePreparationTranslator::CCLISatisfiabilityComandLinePreparationTranslator(CLoaderFactory *loaderFactory) : CCLIComandLinePreparationTranslator(loaderFactory) {
					mOperationTypeString = QString("Satisfiability");
					mMinNecessaryArguments = true;
					mFirstArgumentTranslation = false;
					mLoaderTypeString = QString("-CLISatisfiabilityBatchProcessingLoader");
				}


				CCLISatisfiabilityComandLinePreparationTranslator::~CCLISatisfiabilityComandLinePreparationTranslator() {
				}

				bool CCLISatisfiabilityComandLinePreparationTranslator::hasNeccessaryParameters() {
					bool minNecessaryArguments = true;
					if (!mFirstArgumentTranslation) {
						minNecessaryArguments = false;
					}
					if (!mRequestFileTranslated) {
						LOG(ERROR,getLogDomain(),logTr("%1 request file not specified.").arg(mOperationTypeString),this);
						minNecessaryArguments = false;
					}
					if (!mIRITranslated) {
						LOG(ERROR,getLogDomain(),logTr("Class IRI not specified for satisfiability checking."),this);
						minNecessaryArguments = false;
					}
					return minNecessaryArguments;
				}

				bool CCLISatisfiabilityComandLinePreparationTranslator::translate(const QString& argumentText, QStringList& remainingArgList) {

					bool argumentTranslated = false;


					QString modArgText = argumentText.trimmed();
					if (!mSkipTranslation) {
						if (CCommandLinePreparationTranslator::translate(argumentText,remainingArgList)) {
							argumentTranslated = true;
						} else {
							if (modArgText.indexOf("-") == 0) {
							} else if (modArgText == "satisfiability" || modArgText == "sat") {
								argumentTranslated = true;
								mFirstArgumentTranslation = true;
							}
						}
					}
					return argumentTranslated;
				}

				bool CCLISatisfiabilityComandLinePreparationTranslator::canTranslate(const QString& string) {
					if (QString::compare(string,"satisfiability",Qt::CaseInsensitive) == 0 || QString::compare(string,"sat",Qt::CaseInsensitive) == 0) {
						return true;
					}
					return false;
				}


			}; // end namespace CommandLine

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
