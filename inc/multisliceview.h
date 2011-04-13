#ifndef MULTISLICEVIEW_H
#define MULTISLICEVIEW_H

#include <QtGui/QWidget>
#include "iview.h"
#include "ui_multisliceview.h"

class MultiSliceView : public IView
{
    Q_OBJECT

public:
    MultiSliceView(QWidget *parent = 0);
    virtual ~MultiSliceView();

private:
    Q_DISABLE_COPY(MultiSliceView);

    Ui::MultiSliceViewClass ui;
};

#endif // MULTISLICEVIEW_H
