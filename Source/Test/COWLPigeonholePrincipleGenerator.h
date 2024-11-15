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

#ifndef KONCLUDE_TEST_COWLPigeonholePrincipleGenerator_H
#define KONCLUDE_TEST_COWLPigeonholePrincipleGenerator_H

// Libraries includes
#include <QDomDocument>

// Namespace includes
#include "Utilities/UtilitiesSettings.h"

// Other includes
#include <cmath>


// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"



namespace Konclude {

	using namespace Logger;
	using namespace Utilities;

	namespace Test {


		/*! 
		 *
		 *		\class		COWLPigeonholePrincipleGenerator
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class COWLPigeonholePrincipleGenerator : public CLogIdentifier {

			// public methods
			public:
				//! Constructor
				COWLPigeonholePrincipleGenerator();

				//! Destructor
				virtual ~COWLPigeonholePrincipleGenerator();

				void generatePigeonholePrinciple(cint64 size, const QString& filename);

			// protected methods
			protected:
				QDomElement getOWLClassDeclaration(const QString& className);
				QDomElement getOWLClass(const QString& className);
				QDomElement getOWLComplementClass(const QString& className);
				QDomElement getOWLClass(const QString& className, bool negated);

				QDomElement getOWLUnion(const QList<QDomElement>& list);
				QDomElement getOWLUnion(const QDomElement& u1, const QDomElement& u2);
				QDomElement getOWLIntersection(const QList<QDomElement>& list);
				QDomElement getOWLEquivalentClass(const QDomElement& eq1, const QDomElement& eq2);


			// protected variables
			protected:
				QString owllinkNS;
				QString owlNS;
				QDomDocument document;
				QDomElement rootNode;


			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_COWLPigeonholePrincipleGenerator_H
