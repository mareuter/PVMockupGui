/*
 * axisinteractor.h
 *
 *  Created on: Apr 7, 2011
 *      Author: 2zr
 */

#ifndef AXISINTERACTOR_H_
#define AXISINTERACTOR_H_

#include <QtGui/QWidget>
#include "ui_axisinteractor.h"

class QMouseEvent;
class QString;
class QwtScaleEngine;
class QwtScaleTransformation;

class AxisInteractor : public QWidget
{
	Q_OBJECT

public:
	AxisInteractor(QWidget *parent = 0);
	virtual ~AxisInteractor() {}
	void setInformation(QString title, double min, double max);

protected:
	void mousePressEvent(QMouseEvent *event);

private:
	Ui::AxisInteractor ui;
	QwtScaleEngine *engine;
	QwtScaleTransformation *transform;
};

#endif /* AXISINTERACTOR_H_ */
