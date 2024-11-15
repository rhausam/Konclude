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

#include "CConfigDescription.h"


namespace Konclude {

	namespace Config {


		CConfigDescription::CConfigDescription(const QString &name, const QString &description, CConfigType *takeConfigType) {
			type = takeConfigType;
			configProperty = name;
			propertyDescription = description;
			configPropertyID = 0;

			stringConvertable = false;
			integerConvertable = false;
			booleanConvertable = false;
			stringListConvertable = false;
			doubleConvertable = false;
			if (dynamic_cast<CConvertStringConfigType *>(type)) {
				stringConvertable = true;
			}
			if (dynamic_cast<CConvertIntegerConfigType *>(type)) {
				integerConvertable = true;
			}
			if (dynamic_cast<CConvertBooleanConfigType *>(type)) {
				booleanConvertable = true;
			}
			if (dynamic_cast<CConvertDoubleConfigType *>(type)) {
				doubleConvertable = true;
			}
			if (dynamic_cast<CConvertStringListConfigType *>(type)) {
				stringListConvertable = true;
			}
		}


		CConfigDescription::~CConfigDescription() {
			delete type;
		}


		bool CConfigDescription::supportsStringConvertion() {
			return stringConvertable;
		}

		bool CConfigDescription::supportsIntegerConvertion() {
			return integerConvertable;
		}

		bool CConfigDescription::supportsBooleanConvertion() {
			return booleanConvertable;
		}

		bool CConfigDescription::supportsStringListConvertion() {
			return stringListConvertable;
		}

		bool CConfigDescription::supportsDoubleConvertion() {
			return doubleConvertable;
		}

		CConfigType *CConfigDescription::getConfigType() {
			return type;
		}
		
		CConfigDescription *CConfigDescription::setConfigType(CConfigType *takeConfigType) {
			type = takeConfigType;
			return this;
		}

		CConfigDescription *CConfigDescription::setConfigName(const QString &name) {
			configProperty = name;
			return this;
		}

		CConfigDescription *CConfigDescription::setConfigDescription(const QString &description) {
			propertyDescription = description;
			return this;
		}

		CConfigDescription *CConfigDescription::setConfigID(qint64 id) {
			configPropertyID = id;
			return this;
		}


		QString CConfigDescription::getConfigName() {
			return configProperty;
		}

		QString CConfigDescription::getConfigDescription() {
			return propertyDescription;
		}

		qint64 CConfigDescription::getConfigID() {
			return configPropertyID;
		}


	}; // end namespace Config

}; // end namespace Konclude
