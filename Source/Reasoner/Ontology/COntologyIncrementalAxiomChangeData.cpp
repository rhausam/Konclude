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

#include "COntologyIncrementalAxiomChangeData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			COntologyIncrementalAxiomChangeData::COntologyIncrementalAxiomChangeData(COntologyContext* ontoContext) {
				mOntoContext = ontoContext;

				mAddedABoxAxiomCount = 0;
				mAddedTBoxAxiomCount = 0;
				mAddedRBoxAxiomCount = 0;
				mDeletedABoxAxiomCount = 0;
				mDeletedTBoxAxiomCount = 0;
				mDeletedRBoxAxiomCount = 0;
			}

			cint64 COntologyIncrementalAxiomChangeData::getAddedABoxAxiomCount() {
				return mAddedABoxAxiomCount;
			}

			cint64 COntologyIncrementalAxiomChangeData::getAddedTBoxAxiomCount() {
				return mAddedTBoxAxiomCount;
			}

			cint64 COntologyIncrementalAxiomChangeData::getAddedRBoxAxiomCount() {
				return mAddedRBoxAxiomCount;
			}

			cint64 COntologyIncrementalAxiomChangeData::getDeletedABoxAxiomCount() {
				return mDeletedABoxAxiomCount;
			}

			cint64 COntologyIncrementalAxiomChangeData::getDeletedTBoxAxiomCount() {
				return mDeletedTBoxAxiomCount;
			}

			cint64 COntologyIncrementalAxiomChangeData::getDeletedRBoxAxiomCount() {
				return mDeletedRBoxAxiomCount;
			}



			COntologyIncrementalAxiomChangeData* COntologyIncrementalAxiomChangeData::incAddedABoxAxiomCount(cint64 incCount) {
				mAddedABoxAxiomCount += incCount;
				return this;
			}

			COntologyIncrementalAxiomChangeData* COntologyIncrementalAxiomChangeData::incAddedTBoxAxiomCount(cint64 incCount) {
				mAddedTBoxAxiomCount += incCount;
				return this;
			}

			COntologyIncrementalAxiomChangeData* COntologyIncrementalAxiomChangeData::incAddedRBoxAxiomCount(cint64 incCount) {
				mAddedRBoxAxiomCount += incCount;
				return this;
			}

			COntologyIncrementalAxiomChangeData* COntologyIncrementalAxiomChangeData::incDeletedABoxAxiomCount(cint64 incCount) {
				mDeletedABoxAxiomCount += incCount;
				return this;
			}

			COntologyIncrementalAxiomChangeData* COntologyIncrementalAxiomChangeData::incDeletedTBoxAxiomCount(cint64 incCount) {
				mDeletedTBoxAxiomCount += incCount;
				return this;
			}

			COntologyIncrementalAxiomChangeData* COntologyIncrementalAxiomChangeData::incDeletedRBoxAxiomCount(cint64 incCount) {
				mDeletedRBoxAxiomCount += incCount;
				return this;
			}



			bool COntologyIncrementalAxiomChangeData::hasAddedABoxAxioms() {
				return mAddedABoxAxiomCount > 0;
			}

			bool COntologyIncrementalAxiomChangeData::hasAddedTBoxAxioms() {
				return mAddedTBoxAxiomCount > 0;
			}

			bool COntologyIncrementalAxiomChangeData::hasAddedRBoxAxioms() {
				return mAddedRBoxAxiomCount > 0;
			}

			bool COntologyIncrementalAxiomChangeData::hasDeletedABoxAxioms() {
				return mDeletedABoxAxiomCount > 0;
			}

			bool COntologyIncrementalAxiomChangeData::hasDeletedTBoxAxioms() {
				return mDeletedTBoxAxiomCount > 0;
			}

			bool COntologyIncrementalAxiomChangeData::hasDeletedRBoxAxioms() {
				return mDeletedRBoxAxiomCount > 0;
			}




			bool COntologyIncrementalAxiomChangeData::hasChangedABoxAxioms() {
				return hasAddedABoxAxioms() || hasDeletedABoxAxioms();
			}

			bool COntologyIncrementalAxiomChangeData::hasChangedTBoxAxioms() {
				return hasAddedTBoxAxioms() || hasDeletedTBoxAxioms();
			}

			bool COntologyIncrementalAxiomChangeData::hasChangedRBoxAxioms() {
				return hasAddedRBoxAxioms() || hasDeletedRBoxAxioms();
			}


			bool COntologyIncrementalAxiomChangeData::hasAxiomAdditions() {
				return hasAddedABoxAxioms() || hasAddedRBoxAxioms() || hasAddedTBoxAxioms();
			}

			bool COntologyIncrementalAxiomChangeData::hasAxiomDeletions() {
				return hasDeletedABoxAxioms() || hasDeletedTBoxAxioms() || hasDeletedRBoxAxioms();
			}


			bool COntologyIncrementalAxiomChangeData::hasChangedAxioms() {
				return hasChangedABoxAxioms() || hasChangedRBoxAxioms() || hasChangedTBoxAxioms();
			}


			bool COntologyIncrementalAxiomChangeData::hasOnlyChangedABoxAxioms() {
				if (hasChangedRBoxAxioms() || hasChangedTBoxAxioms()) {
					return false;
				}
				return hasChangedABoxAxioms();
			}

			bool COntologyIncrementalAxiomChangeData::hasOnlyChangedTBoxAxioms() {
				if (hasChangedABoxAxioms() || hasChangedRBoxAxioms()) {
					return false;
				}
				return hasChangedTBoxAxioms();
			}

			bool COntologyIncrementalAxiomChangeData::hasOnlyChangedRBoxAxioms() {
				if (hasChangedABoxAxioms() || hasChangedTBoxAxioms()) {
					return false;
				}
				return hasChangedRBoxAxioms();
			}

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
