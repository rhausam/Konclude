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

#include "CNewAllocationIncreasingMemoryPoolProvider.h"


namespace Konclude {

	namespace Utilities {

		namespace Memory {


			CNewAllocationIncreasingMemoryPoolProvider::CNewAllocationIncreasingMemoryPoolProvider(cint64 defaultPoolSize, cint64 maxPoolSize, double increasingFactor) : CNewAllocationMemoryPoolProvider(defaultPoolSize) {
				mMaxPoolSize = maxPoolSize;
				mIncreasingFactor = increasingFactor;
			}

			CNewAllocationIncreasingMemoryPoolProvider::~CNewAllocationIncreasingMemoryPoolProvider() {
			}


			CMemoryPool* CNewAllocationIncreasingMemoryPoolProvider::acquireMemoryPool(cint64 minPoolSize) {
				mDefaultPoolSize *= mIncreasingFactor;
				mDefaultPoolSize = qMin(mDefaultPoolSize, mMaxPoolSize);
				return CNewAllocationMemoryPoolProvider::acquireMemoryPool(minPoolSize);
			}




		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude

