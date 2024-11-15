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

#include "COWLlinkHttpServerLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			COWLlinkHttpServerLoader::COWLlinkHttpServerLoader() {
				loaderConfig = 0;
			}



			COWLlinkHttpServerLoader::~COWLlinkHttpServerLoader() {
			}


			CLoader *COWLlinkHttpServerLoader::init(CLoaderFactory *loaderFactory, CConfiguration *config) {
				loaderConfig = config;
				mPort = CConfigDataReader::readConfigInteger(loaderConfig,"Konclude.OWLlink.Server.Port",8080);

				return this;
			}



			CLoader *COWLlinkHttpServerLoader::load() {
				mHanlderFactory = new COWLlinkHttpConnectionHandlerProcessorFactory(loaderConfig);
				mHandlerPool = new CQtHttpConnectionHandlerPooledFactory(mHanlderFactory);

				mHttpListner = new CQtHttpListner(mHanlderFactory,mPort);
				return this;
			}




			CLoader *COWLlinkHttpServerLoader::exit() {
				delete mHttpListner;
				delete mHandlerPool;
				delete mHanlderFactory;
				return this;
			}



		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
