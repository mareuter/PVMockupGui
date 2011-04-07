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

Indicator::Indicator(QGraphicsItem *parent) : QGraphicsPolygonItem(parent)
{
	this->fillColor = Qt::blue;
	this->outlineColor = Qt::black;
	this->half_base = 5;
	this->setOpacity(1.0);
	this->setBrush(QBrush(this->fillColor));
	this->setPen(QPen(this->outlineColor));
	this->setFlags(QGraphicsItem::ItemIsMovable & QGraphicsItem::ItemIsSelectable);
}

void Indicator::setPoints(int tip, int level, int height)
{
	path << QPointF(tip, level);
	int base_pos = tip + height;
	path << QPointF(base_pos, level - this->half_base);
	path << QPointF(base_pos, level + this->half_base);
	// Close the polygon
	path << QPointF(tip, level);
	this->setPolygon(path);
}
