/*
 * axisinteractor.cpp
 *
 *  Created on: Apr 7, 2011
 *      Author: 2zr
 */
#include "axisinteractor.h"
#include "indicator.h"
#include "scalepicker.h"

#include "qwt_scale_draw.h"
#include "qwt_scale_engine.h"
#include "qwt_scale_map.h"
#include "qwt_scale_widget.h"

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QList>
#include <QMouseEvent>
#include <QString>

#include <iostream>
AxisInteractor::AxisInteractor(QWidget *parent) : QWidget(parent)
{
	this->scene = new QGraphicsScene(this);
	this->scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	this->isSceneGeomInit = false;
	this->ui.setupUi(this);
	this->ui.graphicsView->setScene(this->scene);
	this->ui.scaleWidget->setAlignment(QwtScaleDraw::LeftScale);
	this->engine = new QwtLinearScaleEngine;
	this->transform = new QwtScaleTransformation(QwtScaleTransformation::Linear);
	this->scalePicker = new ScalePicker(this->ui.scaleWidget);
}

void AxisInteractor::setInformation(QString title, double min, double max)
{
	this->ui.scaleWidget->setTitle(title);
	this->ui.scaleWidget->setScaleDiv(transform,
			engine->divideScale(min, max, 10, 0));
}

void AxisInteractor::mousePressEvent(QMouseEvent *event)
{
	switch (event->button())
	{
	case Qt::RightButton:
	{
		QRect gv_rect = this->ui.graphicsView->geometry();
		if (! this->isSceneGeomInit)
		{
			this->scene->setSceneRect(gv_rect);
			this->isSceneGeomInit = true;
		}
		Indicator *tri = new Indicator();
		tri->setPoints(event->pos(), gv_rect);
		this->scene->addItem(tri);
	}
	default:
		QWidget::mousePressEvent(event);
	}
}
