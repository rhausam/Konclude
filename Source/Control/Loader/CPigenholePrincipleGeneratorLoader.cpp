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

#include "CPigenholePrincipleGeneratorLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CPigenholePrincipleGeneratorLoader::CPigenholePrincipleGeneratorLoader()  {
				mConfiguration = 0;
				generator = 0;
			}



			CPigenholePrincipleGeneratorLoader::~CPigenholePrincipleGeneratorLoader() {
			}


			CLoader* CPigenholePrincipleGeneratorLoader::init(CLoaderFactory* loaderFactory, CConfiguration* config) {
				mConfiguration = config;
				return this;
			}




			CLoader *CPigenholePrincipleGeneratorLoader::load() {
				generator = new COWLPigeonholePrincipleGenerator();
				QString filename = CConfigDataReader::readConfigString(mConfiguration,"Konclude.Test.PigeonholePrincipleGenerator.OWLlinkOutputFile","PHP-request-file.xml");
				cint64 probSize = CConfigDataReader::readConfigInteger(mConfiguration,"Konclude.Test.PigeonholePrincipleGenerator.ProblemSize",2);
				generator->generatePigeonholePrinciple(probSize,filename);
				delete generator;
				return this;
			}


			CLoader* CPigenholePrincipleGeneratorLoader::exit() {
				return this;
			}




		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
