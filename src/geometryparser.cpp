/*
 * geometryparser.cpp
 *
 *  Created on: Apr 14, 2011
 *      Author: 2zr
 */

#include "geometryparser.h"

#include "axisinformation.h"

#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/Node.h"
#include "Poco/DOM/NodeList.h"

#include <iostream>
GeometryParser::GeometryParser(const char *xml)
{
	Poco::XML::DOMParser parser;
	this->pDoc = parser.parseString(Poco::XML::XMLString(xml));
}

AxisInformation *GeometryParser::getAxisInfo(const std::string dimension)
{
	AxisInformation *axis = new AxisInformation();

	Poco::XML::NodeList *pNodes = this->pDoc->getElementsByTagName(dimension);
	Poco::XML::Node *pNode = pNodes->item(0)->childNodes()->item(0);
	Poco::XML::XMLString label = pNode->innerText();
	std::cout << "A: " << label << std::endl;
	Poco::XML::Element *elem = pDoc->getElementById(label);
	if (elem)
	{
		std::cout << "C: " << elem->nodeName() << std::endl;
		std::cout << "B: " << elem->getChildElement("Name")->innerText() << std::endl;
	}

	return axis;
}
