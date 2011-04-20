#ifndef MULTISLICEVIEW_H
#define MULTISLICEVIEW_H

#include <QtGui/QWidget>
#include <QPointer>
#include "iview.h"
#include "ui_multisliceview.h"

class pqPipelineRepresentation;
class pqPipelineSource;
class pqRenderView;

class MultiSliceView : public IView
{
    Q_OBJECT

public:
    MultiSliceView(QWidget *parent = 0);
    virtual ~MultiSliceView();

    pqRenderView* getView();
    void render();

protected slots:
	void makeXcut(double value);
	void makeYcut(double value);
	void makeZcut(double value);

signals:
	void sliceNamed(const QString &name);

private:
    Q_DISABLE_COPY(MultiSliceView);
    void makeCut(double origin[], double orient[]);
    void setupAxisInfo();
    void setupData();

    QPointer<pqRenderView> mainView;
    pqPipelineSource *origSource;
    QPointer<pqPipelineRepresentation> originSourceRepr;
    Ui::MultiSliceViewClass ui;
};

#endif // MULTISLICEVIEW_H
