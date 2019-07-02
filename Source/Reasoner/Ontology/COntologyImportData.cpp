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

#include "COntologyImportData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			COntologyImportData::COntologyImportData() {
			}

			COntologyImportData* COntologyImportData::initOntologyImportData(const QString& ontoIRIString) {
				mOntoIRIString = ontoIRIString;
				mOntoLoaded = false;
				mOntoLoadingTried = false;
				return this;
			}


			QString COntologyImportData::getOntologyIRIString() {
				return mOntoIRIString;
			}

			QString COntologyImportData::getResolvedIRIOntoLoadString() {
				return mResolvedIRIOntoLoadString;
			}

			bool COntologyImportData::getOntologyLoaded() {
				return mOntoLoaded;
			}

			bool COntologyImportData::getOntologyLoadingTried() {
				return mOntoLoadingTried;
			}

			COntologyImportData* COntologyImportData::setOntologyLoaded(bool loaded) {
				mOntoLoaded = loaded;
				return this;
			}

			COntologyImportData* COntologyImportData::setOntologyLoadingTried(bool tried) {
				mOntoLoadingTried = tried;
				return this;
			}


			COntologyImportData* COntologyImportData::setResolvedIRIOntoLoadString(const QString& resolvedOntoIRIString) {
				mResolvedIRIOntoLoadString = resolvedOntoIRIString;
				return this;
			}

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
