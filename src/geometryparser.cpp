/*
 * geometryparser.cpp
 *
 *  Created on: Apr 14, 2011
 *      Author: 2zr
 */

#include <geometryparser.h>

#include <axisinformation.h>

#include "Poco/DOM/DOMParser.h"

GeometryParser::GeometryParser(const char *xml)
{
	Poco::XML::DOMParser parser;
	this->pDoc = parser.parseString(Poco::XML::XMLString(xml));
}
