#ifndef MPMAINWINDOW_H
#define MPMAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mpMainWindow.h"
#include <QPointer>

class pqRenderView;
class pqPipelineRepresentation;
class pqPipelineSource;

class mpMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    mpMainWindow(QWidget *parent = 0);
    ~mpMainWindow();

protected slots:
    void onDataLoaded(pqPipelineSource *);
    void onCutButtonClicked();

protected:
    pqRenderView *createRenderView(QWidget *container);

private:
    Q_DISABLE_COPY(mpMainWindow);
    Ui::mpMainWindow ui;
    QPointer<pqRenderView> View;
    QPointer<pqPipelineSource> ActiveSource;
    QPointer<pqPipelineSource> Slice;
    QPointer<pqPipelineRepresentation> ActiveSourceRepr;
    QPointer<pqPipelineRepresentation> SliceRepr;
};

#endif // mpMAINWINDOW_H
