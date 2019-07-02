/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#ifndef KONCLUDE_TEST_ANALYSE_CDOUBLEANALYSEVALUE_H
#define KONCLUDE_TEST_ANALYSE_CDOUBLEANALYSEVALUE_H

// Libraries includes
#include <QString>

// Namespace includes
#include "AnalyseSettings.h"
#include "CConvertIntegerAnalyseValue.h"
#include "CConvertDoubleAnalyseValue.h"
#include "CConvertStringAnalyseValue.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;

	namespace Test {

		namespace Analyse {


			/*! 
			 *
			 *		\class		CDoubleAnalyseValue
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDoubleAnalyseValue : virtual public CConvertDoubleAnalyseValue, virtual public CConvertIntegerAnalyseValue {
				// public methods
				public:
					//! Constructor
					CDoubleAnalyseValue();
					CDoubleAnalyseValue(double doubleValue);

					//! Destructor
					virtual ~CDoubleAnalyseValue();

					virtual QString getString();
					virtual bool parseFromString(const QString &string);

					virtual qint64 getInteger();
					virtual bool parseFromInteger(qint64 integer);

					virtual double getDouble();
					virtual bool parseFromDouble(double doubleValue);


					virtual CAnalyseValue *createCopy();

				// protected methods
				protected:

				// protected variables
				protected:
					double doubleVal;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_ANALYSE_CDOUBLEANALYSEVALUE_H