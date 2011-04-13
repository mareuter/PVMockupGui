#ifndef MULTISLICEVIEW_H
#define MULTISLICEVIEW_H

#include <QtGui/QWidget>
#include <QPointer>
#include "iview.h"
#include "ui_multisliceview.h"

class pqRenderView;

class MultiSliceView : public IView
{
    Q_OBJECT

public:
    MultiSliceView(QWidget *parent = 0);
    virtual ~MultiSliceView();

    pqRenderView* getView();
    void render();

private:
    Q_DISABLE_COPY(MultiSliceView);

    Ui::MultiSliceViewClass ui;
    QPointer<pqRenderView> mainView;
};

#endif // MULTISLICEVIEW_H
