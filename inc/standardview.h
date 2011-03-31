#ifndef STANDARDVIEW_H
#define STANDARDVIEW_H

#include <QtGui/QWidget>
#include <QPointer>
#include "ui_standardview.h"

class pqPipelineSource;
class pqRenderView;

class StandardView : public QWidget, public Ui::StandardView
{
    Q_OBJECT

public:
    StandardView(QWidget *parent = 0);
    virtual ~StandardView();

    pqRenderView* getView();

protected slots:
    void onCutButtonClicked();
    void onRebinButtonClicked();

protected:
    pqRenderView *createRenderView(QWidget *container);

private:
    QPointer<pqPipelineSource> rebinCut;
    QPointer<pqRenderView> view;
};

#endif // STANDARDVIEW_H
