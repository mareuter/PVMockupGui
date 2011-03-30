#ifndef MPMAINWINDOW_H
#define MPMAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mpMainWindow.h"
#include <QPointer>

class pqRenderView;
class pqPipelineRepresentation;
class pqPipelineSource;

class mpMainWindow : public QMainWindow, public Ui::mpMainWindow
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
    QPointer<pqRenderView> View;
    QPointer<pqPipelineSource> ActiveSource;
    QPointer<pqPipelineSource> Slice;
    QPointer<pqPipelineSource> RebinCut;
    QPointer<pqPipelineRepresentation> ActiveSourceRepr;
};

#endif // mpMAINWINDOW_H
