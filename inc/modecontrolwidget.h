#ifndef MODECONTROLWIDGET_H
#define MODECONTROLWIDGET_H

#include <QtGui/QWidget>
#include "ui_modecontrolwidget.h"

class ModeControlWidget : public QWidget
{
    Q_OBJECT

public:
    ModeControlWidget(QWidget *parent = 0);
    virtual ~ModeControlWidget();

signals:
	void executeSwitchViews();

protected slots:
	void disableAll();
	void enableMultiSliceButton();
	void enableThreeSliceButton();
	void onMultiSliceViewButtonClicked();
    void onStandardViewButtonClicked();
    void onThreeSliceViewButtonClicked();

private:
    Ui::ModeControlWidgetClass ui;
};

#endif // MODECONTROLWIDGET_H
