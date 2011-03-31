#ifndef THREESLICEVIEW_H
#define THREESLICEVIEW_H

#include <QtGui/QWidget>
#include "ui_threesliceview.h"

class ThreeSliceView : public QWidget
{
    Q_OBJECT

public:
    ThreeSliceView(QWidget *parent = 0);
    ~ThreeSliceView();

private:
    Ui::ThreeSliceViewClass ui;
};

#endif // THREESLICEVIEW_H
