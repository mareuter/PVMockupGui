/*
 * indicator.h
 *
 *  Created on: Apr 7, 2011
 *      Author: 2zr
 */

#ifndef INDICATOR_H_
#define INDICATOR_H_

#include <QGraphicsItem>

class QColor;
class QPoint;
class QPolygonF;
class QRect;

class Indicator: public QGraphicsPolygonItem
{
public:
	Indicator(QGraphicsItem *parent = 0);
	virtual ~Indicator() {}
	void setPoints(const QPoint &eloc, const QRect &rect);
	void printSelf();

private:
	QColor fillColor;
	QColor outlineColor;
	QPolygonF path;
	int half_base;
};

#endif /* INDICATOR_H_ */
