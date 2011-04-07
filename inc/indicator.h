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
class QPolygonF;

class Indicator: public QGraphicsPolygonItem
{
public:
	Indicator(QGraphicsItem *parent = 0);
	virtual ~Indicator() {}
	void setPoints(int tip, int level, int height);

private:
	QColor fillColor;
	QColor outlineColor;
	QPolygonF path;
	int half_base;
};

#endif /* INDICATOR_H_ */
