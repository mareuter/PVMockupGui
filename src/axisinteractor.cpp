/*
 * axisinteractor.cpp
 *
 *  Created on: Apr 7, 2011
 *      Author: 2zr
 */
#include "axisinteractor.h"

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
		this->ui.scaleWidget->setTitle(QString("left"));
	}
	QRect g = this->ui.scaleWidget->geometry();
	std::cout << g.left() << "  " << g.right() << "  " << g.top() << "  " << g.bottom() << std::endl;
	std::cout << this->ui.scaleWidget->startBorderDist() << "  " << this->ui.scaleWidget->endBorderDist() << std::endl;
}
