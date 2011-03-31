#ifndef MPMAINWINDOW_H
#define MPMAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mpmainwindow.h"
#include <QPointer>

class StandardView;
class ThreeSliceView;

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
    void onStandardViewButtonClicked();
    void onThreeSliceViewButtonClicked();

private:
    Q_DISABLE_COPY(mpMainWindow);
    QPointer<pqPipelineSource> originSource;
    QPointer<pqPipelineRepresentation> originSourceRepr;
    ThreeSliceView *tsview;
    StandardView *sview;

    void setMainWindowComponentsForStandardView();
};

#endif // mpMAINWINDOW_H
