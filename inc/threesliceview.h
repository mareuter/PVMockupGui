#ifndef THREESLICEVIEW_H
#define THREESLICEVIEW_H

#include <QtGui/QWidget>
#include <QPointer>
#include "ui_threesliceview.h"

class pqPipelineRepresentation;
class pqPipelineSource;
class pqRenderView;

class ThreeSliceView : public QWidget, public Ui::ThreeSliceView
{
    Q_OBJECT

public:
    ThreeSliceView(QWidget *parent = 0);
    virtual ~ThreeSliceView();

    pqRenderView* getView();
    void makeThreeSlice();
    void renderAll();

    enum Direction {X, Y, Z};

protected:
    pqRenderView *createRenderView(QWidget *container);
    pqRenderView *create2dRenderView(QWidget *container);

private:
    Q_DISABLE_COPY(ThreeSliceView);
    void makeSlice(ThreeSliceView::Direction i, pqRenderView *view,
    		pqPipelineSource *cut, pqPipelineRepresentation *repr);

    pqPipelineSource *origSource;

    QPointer<pqRenderView> mainView;
    QPointer<pqPipelineSource> xCut;
    QPointer<pqPipelineRepresentation> xCutRepr;
    QPointer<pqRenderView> xView;
    QPointer<pqPipelineSource> yCut;
    QPointer<pqPipelineRepresentation> yCutRepr;
    QPointer<pqRenderView> yView;
    QPointer<pqPipelineSource> zCut;
    QPointer<pqPipelineRepresentation> zCutRepr;
    QPointer<pqRenderView> zView;

};

#endif // THREESLICEVIEW_H
