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

#include "CWriteOWLlinkOWL2XMLSatisfiableTestCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CWriteOWLlinkOWL2XMLSatisfiableTestCommand::CWriteOWLlinkOWL2XMLSatisfiableTestCommand(COntologyOWL2QtXMLRenderer* renderer, const QString& fileName, const QStringList& satTestClasses, const QString& testKBName, CCommand *parentSuperCommand) 
						: CWriteOntologyCommand(parentSuperCommand) {
					mRenderer = renderer;
					mWriteFileName = fileName;
					mSatTestClasses = satTestClasses;
					mTestKBName = testKBName;
				}


				CWriteOWLlinkOWL2XMLSatisfiableTestCommand::~CWriteOWLlinkOWL2XMLSatisfiableTestCommand() {
					delete mRenderer;
				}

				QString CWriteOWLlinkOWL2XMLSatisfiableTestCommand::getBriefCommandDescription() {
					return QString("Write-OWLlink-OWL-2-XML-Satisfiable-Test Command");
				}

				qint64 CWriteOWLlinkOWL2XMLSatisfiableTestCommand::getCommandTag() {
					return WRITEOWLLINKOWL2XMLSATISFIABLETESTCOMMAND;
				}

				COntologyRenderer* CWriteOWLlinkOWL2XMLSatisfiableTestCommand::getRenderer() {
					return mRenderer;
				}

				QString CWriteOWLlinkOWL2XMLSatisfiableTestCommand::getWriteFileName() {
					return mWriteFileName;
				}

				CCommand* CWriteOWLlinkOWL2XMLSatisfiableTestCommand::getCommand() {
					return this;
				}

				COntologyOWL2QtXMLRenderer* CWriteOWLlinkOWL2XMLSatisfiableTestCommand::getOWL2XMLRenderer() {
					return mRenderer;
				}

				QStringList CWriteOWLlinkOWL2XMLSatisfiableTestCommand::getSatisfiableTestClassesList() {
					return mSatTestClasses;
				}

				QString CWriteOWLlinkOWL2XMLSatisfiableTestCommand::getTestKBName() {
					return mTestKBName;
				}


			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude
