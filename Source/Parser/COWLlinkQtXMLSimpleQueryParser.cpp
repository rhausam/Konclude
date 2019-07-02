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

#include "COWLlinkQtXMLSimpleQueryParser.h"


namespace Konclude {

	namespace Parser {



		COWLlinkQtXMLSimpleQueryParser::COWLlinkQtXMLSimpleQueryParser(CQueryBuilder *queryBuilder, CConcreteOntology* conOntology) : COWLlinkQueryParser(queryBuilder) {
			mConOntology = conOntology;
			LOG(INFO,"::Konclude::Parser::OWLlinkQtXMLSimpleQueryParser",logTr("OWLlink Query Parser initialized."),this);

		}


		COWLlinkQtXMLSimpleQueryParser::~COWLlinkQtXMLSimpleQueryParser() {
		}


		QList<CBuildExpression *> COWLlinkQtXMLSimpleQueryParser::parseOntologyChildNodes(QDomElement *node) {
			QList<CBuildExpression *> builds;
			QDomElement childNode = node->firstChildElement();
			while (!childNode.isNull()) {
				CBuildExpression *expression = nullptr;
				QString nodeString(childNode.tagName());
				if (nodeString == "Class" || nodeString == "owl:Class" ) {
					QString className(getEntityStringName(&childNode));
					CBUILDHASH<CStringRefStringHasher,CClassExpression*>* classStringBuildHash = mConOntology->getBuildData()->getClassEntityBuildHash();
					expression = classStringBuildHash->value(className,nullptr);
					if (!expression) {
						LOG(ERROR,"::Konclude::Parser::OWLlinkQtXMLSimpleQueryParser",logTr("Class name '%1' cannot be resolved in ontology '%2'.").arg(className).arg(mConOntology->getOntologyName()),this);
					}
				} else if (nodeString == "ObjectProperty" || nodeString == "owl:ObjectProperty" ) {
					QString objPropName(getEntityStringName(&childNode));
					CBUILDHASH<CStringRefStringHasher,CObjectPropertyExpression*>* objPropBuildHash = mConOntology->getBuildData()->getObjectPropertyEntityBuildHash();
					expression = objPropBuildHash->value(objPropName,nullptr);
					if (!expression) {
						LOG(ERROR,"::Konclude::Parser::OWLlinkQtXMLSimpleQueryParser",logTr("Object property name '%1' cannot be resolved in ontology '%2'.").arg(objPropName).arg(mConOntology->getOntologyName()),this);
					}
				} else if (nodeString == "NamedIndividual" || nodeString == "owl:NamedIndividual" ) {
					QString indiName(getEntityStringName(&childNode));
					CBUILDHASH<CStringRefStringHasher,CNamedIndividualExpression*>* indiStringBuildHash = mConOntology->getBuildData()->getIndividualEntityBuildHash();
					expression = indiStringBuildHash->value(indiName,nullptr);
					if (!expression) {
						LOG(ERROR,"::Konclude::Parser::OWLlinkQtXMLSimpleQueryParser",logTr("Individual name '%1' cannot be resolved in ontology '%2'.").arg(indiName).arg(mConOntology->getOntologyName()),this);
					}
				}

				if (expression) {
					builds.append(expression);
				}
				childNode = childNode.nextSiblingElement();
			}
			return builds;
		}


		QString COWLlinkQtXMLSimpleQueryParser::getEntityStringName(QDomElement *node) {
			QString entityName;

			bool abbreviated = false;

			QString attrName = node->attribute(QString("IRI"));
			if (attrName.isEmpty()) {
				attrName = node->attribute(QString("abbreviatedIRI"));
				abbreviated = true;
			}
			if (attrName.isEmpty()) {
				attrName = node->attribute(QString("owl:IRI"));
			}
			if (attrName.isEmpty()) {
				attrName = node->attribute(QString("owl:abbreviatedIRI"));
				abbreviated = true;
			}
			if (attrName.isEmpty()) {
				attrName = node->attribute(QString("URI"));
			} 
			if (attrName.isEmpty()) {
				attrName = node->attribute(QString("owl:URI"));
			}
			if (abbreviated) {
				entityName = resolveAbbreviationName(attrName);
			} else {
				entityName = attrName;
			}

			return entityName;
		}

		QString COWLlinkQtXMLSimpleQueryParser::resolveAbbreviationName(const QString& name) {
			QString resolvedString(name);
			QStringList nameList(name.split(":"));
			if (nameList.size() > 1) {
				QString abbName = nameList.first();
				QString idName = nameList.last();
				CMAPPINGHASH<QString,CNamePrefix*>* abbNameHash = mConOntology->getStringMapping()->getAbbreviatedNamePrefixHash(false);
				CNamePrefix* namePrefix = abbNameHash->value(abbName,nullptr);
				if (namePrefix) {
					resolvedString = namePrefix->getPrefixString()+idName;
				}
			}
			return resolvedString;
		}




	}; // end namespace Parser

}; // end namespace Konclude
