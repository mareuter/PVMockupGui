/*
 * axisinteractor.cpp
 *
 *  Created on: Apr 7, 2011
 *      Author: 2zr
 */
#include "axisinteractor.h"
#include "indicator.h"

#include "qwt_scale_draw.h"
#include "qwt_scale_engine.h"
#include "qwt_scale_map.h"
#include "qwt_scale_widget.h"

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QList>
#include <QMouseEvent>
#include <QString>

#include <iostream>
AxisInteractor::AxisInteractor(QWidget *parent) : QWidget(parent)
{
	this->ui.setupUi(this);
	this->scene = new QGraphicsScene(this);
	this->ui.graphicsView->setScene(this->scene);
	this->ui.scaleWidget->setAlignment(QwtScaleDraw::LeftScale);
	this->engine = new QwtLinearScaleEngine;
	this->transform = new QwtScaleTransformation(QwtScaleTransformation::Linear);
}

void AxisInteractor::setInformation(QString title, double min, double max)
{
	this->ui.scaleWidget->setTitle(title);
	this->ui.scaleWidget->setScaleDiv(transform,
			engine->divideScale(min, max, 10, 0));
}

void AxisInteractor::mousePressEvent(QMouseEvent *event)
{
	std::cout << event->pos().x() << "  " << event->pos().y() << std::endl;
	switch (event->button())
	{
	case Qt::RightButton:
	{
		QRect gv_rect = this->ui.graphicsView->geometry();
		QRect sw_rect = this->ui.scaleWidget->geometry();
		std::cout << "GV: " << gv_rect.left() << ", " << gv_rect.top() << ", " << gv_rect.right() << ", " << gv_rect.bottom() << std::endl;
		std::cout << "SW: " << sw_rect.left() << ", " << sw_rect.top() << ", " << sw_rect.right() << ", " << sw_rect.bottom() << std::endl;

		Indicator *tri = new Indicator();
		tri->setPoints(gv_rect.left(), event->pos().y(), gv_rect.width());
		std::cout << "Position: " << tri->pos().x() << ", " << tri->pos().y() << std::endl;
		std::cout << "Triangle Added: " << std::endl;
		tri->printSelf();
		this->scene->addItem(tri);
		QList<QGraphicsItem *> items = this->scene->items();
		std::cout << "Current Scene:" << std::endl;
		for (int j = 0; j < items.size(); j++)
		{
			Indicator *ind = qgraphicsitem_cast<Indicator *>(items.at(j));
			std::cout << "Triangle " << j << ":" << std::endl;
			ind->printSelf();
			std::cout << "Position: " << ind->pos().x() << ", " << ind->pos().y() << std::endl;
		}
	}
	default:
		QWidget::mousePressEvent(event);
	}
}
