#ifndef MPMAINWINDOW_H
#define MPMAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mpMainWindow.h"
#include <QPointer>

class pq3DWidget;
class pqRenderView;
class pqPipelineRepresentation;
class pqPipelineSource;

class mpMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    mpMainWindow(QWidget *parent = 0);
    virtual ~mpMainWindow();

protected slots:
    void onDataLoaded(pqPipelineSource *);
    void onCutButtonClicked();
    void onRebinButtonClicked();

protected:
    pqRenderView *createRenderView(QWidget *container);

private:
    Q_DISABLE_COPY(mpMainWindow);
    Ui::mpMainWindow ui;
    QPointer<pqRenderView> View;
    QPointer<pqPipelineSource> ActiveSource;
    QPointer<pqPipelineSource> Slice;
    QPointer<pqPipelineSource> RebinCut;
    QPointer<pqPipelineRepresentation> ActiveSourceRepr;
    QPointer<pqPipelineRepresentation> SliceRepr;
    QPointer<pqPipelineRepresentation> RebinCutRepr;
    QPointer<pq3DWidget> PlaneWidget;
    QPointer<pq3DWidget> BoxWidget;
};

#endif // mpMAINWINDOW_H
