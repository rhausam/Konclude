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

#ifndef KONCLUDE_PARSER_CSPARQLSIMPLEBUILDINGPARSER_H
#define KONCLUDE_PARSER_CSPARQLSIMPLEBUILDINGPARSER_H

// Libraries includes


// Namespace includes
#include "CSPARQLSimpleParser.h"
#include "COntologyBuilder.h"


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	using namespace Reasoner::Ontology;

	namespace Parser {

		/*! 
		 *
		 *		\class		CSPARQLSimpleBuildingParser
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CSPARQLSimpleBuildingParser : public CSPARQLSimpleParser {
			// public methods
			public:
				//! Constructor
				CSPARQLSimpleBuildingParser(COntologyBuilder* ontoBuilder, CConcreteOntology* ontology);

				//! Destructor
				virtual ~CSPARQLSimpleBuildingParser();


			// protected methods
			protected:
				virtual QList<CAxiomExpression*> getTripleOWLAxioms(QHash<QString,CRDFHahsedSubjectData>* subjectTripleHash, bool anonymousIndividualsAsVariables = false);

				virtual QList<CAxiomExpression*> getTripleOWLAxioms(QList<CRDFStringTriple>* tripleStringList, bool anonymousIndividualsAsVariables = false, QSet<CExpressionVariable*>* varExpSet = nullptr);


				CIndividualTermExpression* getIndividualTermExpression(const QString& subjectString, bool anonymousIndividualsAsVariables, QSet<CExpressionVariable*>* varExpSet = nullptr);
				CDataLiteralTermExpression* getDataLiteralTermExpression(const QString &objectString, QSet<CExpressionVariable*>* varExpSet = nullptr);
				CClassTermExpression* getClassTermExpression(const QString &objectString, QSet<CExpressionVariable*>* varExpSet = nullptr);
				CDataPropertyTermExpression* getDataPropertyTermExpression(const QString &objectString, QSet<CExpressionVariable*>* varExpSet = nullptr);
				CObjectPropertyTermExpression* getObjectPropertyTermExpression(const QString &objectString, QSet<CExpressionVariable*>* varExpSet = nullptr);

				bool isDataProperty(const QString& propertyName);

				QString getVariableName(const QString& variableName);



			// protected variables
			protected:
				COntologyBuilder* mOntoBuilder;
				QSet<CExpressionVariable*> mVarExpSet;
				QHash<QString,CExpressionVariable*> mNameVarHash;

				CConcreteOntology* mOntology;
				
			// private methods
			private:


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CSPARQLSIMPLEBUILDINGPARSER_H
