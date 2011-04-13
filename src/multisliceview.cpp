#include "multisliceview.h"

#include "pqRenderView.h"

MultiSliceView::MultiSliceView(QWidget *parent) : IView(parent)
{
	ui.setupUi(this);

	this->mainView = this->createRenderView(this->ui.renderFrame);
}

MultiSliceView::~MultiSliceView()
{

}

pqRenderView* MultiSliceView::getView()
{
	return this->mainView.data();
}

void MultiSliceView::render()
{
	this->mainView->resetDisplay();
	this->mainView->render();
}
