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

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QString>

#include <iostream>
AxisInteractor::AxisInteractor(QWidget *parent) : QWidget(parent)
{
	this->ui.setupUi(this);
	this->ui.graphicsView->setRenderHint(QPainter::Antialiasing, true);
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
	if (event->button()== Qt::RightButton)
	{
		this->ui.scaleWidget->setTitle(QString("right"));
	}
	if (event->button() == Qt::LeftButton)
	{
		Indicator *tri = new Indicator();
		QRect rect = this->ui.graphicsView->geometry();
		tri->setPoints(rect.left(), event->pos().y(), rect.width());
		this->scene->addItem(tri);
	}
}
