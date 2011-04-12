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
class QGraphicsSceneMouseEvent;
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

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
	int fixVerticalPos(int ylevel);

	QColor fillColor;
	QColor outlineColor;
	QPolygonF path;
	int half_base;
	int left_edge;
};

#endif /* INDICATOR_H_ */
