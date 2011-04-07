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

#include <QString>

AxisInteractor::AxisInteractor(QWidget *parent) : QWidget(parent)
{
	this->ui.setupUi(this);
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
