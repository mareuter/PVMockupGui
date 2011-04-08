/*
 * indicator.cpp
 *
 *  Created on: Apr 7, 2011
 *      Author: 2zr
 */

#include "indicator.h"

#include <QBrush>
#include <QPen>
#include <QPointF>
#include <QPolygonF>

#include <iostream>
#include <vector>

Indicator::Indicator(QGraphicsItem *parent) : QGraphicsPolygonItem(parent)
{
	this->fillColor = Qt::blue;
	this->outlineColor = Qt::black;
	this->half_base = 5;
	this->setOpacity(1.0);
	this->setBrush(QBrush(this->fillColor));
	this->setPen(QPen(this->outlineColor));
	//this->setFlags(QGraphicsItem::ItemIsMovable & QGraphicsItem::ItemIsSelectable);
	this->setSelected(true);
}

void Indicator::setPoints(int tip, int level, int height)
{
	std::cout << "Pointer: " << tip << ", " << level << ", " << height << std::endl;
	int half_height = (height - 2) / 2;
	path << QPointF(-half_height, 0);
	path << QPointF(half_height, this->half_base);
	path << QPointF(half_height, -this->half_base);
	// Close the polygon
	path << QPointF(-half_height, 0);
	this->setPolygon(path);
	//this->setPos(QPointF(half_height, level));
}

void Indicator::printSelf()
{
	QPolygonF poly = this->polygon();
	int psize = poly.size();
	if (poly.isClosed())
	{
		--psize;
	}
	for(int i = 0; i < psize; i++)
	{
		std::cout << "Point " << i << ": " << poly.at(i).x();
		std::cout << ", " << poly.at(i).y() << std::endl;
	}
}
