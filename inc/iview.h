#ifndef IVIEW_H_
#define IVIEW_H_

#include <QtGui/QWidget>

class pqRenderView;

class IView : public QWidget {
public:
	IView(QWidget *parent = 0);
	virtual ~IView() {}
	virtual pqRenderView *createRenderView(QWidget *container);
	virtual pqRenderView *getView() = 0;
	virtual void render() = 0;
};

#endif /* IVIEW_H_ */
