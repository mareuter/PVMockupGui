#ifndef THREESLICEVIEW_H
#define THREESLICEVIEW_H

#include <QtGui/QWidget>
#include "ui_threesliceview.h"

class ThreeSliceView : public QWidget, public Ui::ThreeSliceView
{
    Q_OBJECT

public:
    ThreeSliceView(QWidget *parent = 0);
    virtual ~ThreeSliceView();

private:

};

#endif // THREESLICEVIEW_H
