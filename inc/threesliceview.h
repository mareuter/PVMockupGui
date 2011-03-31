#ifndef THREESLICEVIEW_H
#define THREESLICEVIEW_H

#include <QtGui/QWidget>
#include <QPointer>
#include "ui_threesliceview.h"

class pqPipelineSource;
class pqRenderView;

class ThreeSliceView : public QWidget, public Ui::ThreeSliceView
{
    Q_OBJECT

public:
    ThreeSliceView(QWidget *parent = 0);
    virtual ~ThreeSliceView();

    pqRenderView* getView();

protected:
    pqRenderView *createRenderView(QWidget *container);
    pqRenderView *create2dRenderView(QWidget *container);

private:
    QPointer<pqRenderView> mainView;
    QPointer<pqPipelineSource> xCut;
    QPointer<pqRenderView> xView;
    QPointer<pqPipelineSource> yCut;
    QPointer<pqRenderView> yView;
    QPointer<pqPipelineSource> zCut;
    QPointer<pqRenderView> zView;
};

#endif // THREESLICEVIEW_H
