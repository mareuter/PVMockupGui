#include "threesliceview.h"

#include "pqActiveObjects.h"
#include "pqApplicationCore.h"
#include "pqObjectBuilder.h"
#include "pqRenderView.h"
#include "vtkSMPropertyHelper.h"

ThreeSliceView::ThreeSliceView(QWidget *parent) : QWidget(parent)
{
	this->setupUi(this);

	this->mainView = this->createRenderView(this->mainRenderFrame);
	this->xView = this->create2dRenderView(this->xRenderFrame);
	this->yView = this->create2dRenderView(this->yRenderFrame);
	this->zView = this->create2dRenderView(this->zRenderFrame);
}

ThreeSliceView::~ThreeSliceView()
{

}

pqRenderView* ThreeSliceView::createRenderView(QWidget* widget)
{
  QHBoxLayout *hbox = new QHBoxLayout(widget);
  hbox->setMargin(0);

  // Create a new render view.
  pqObjectBuilder* builder = pqApplicationCore::instance()->getObjectBuilder();
  pqRenderView *view = qobject_cast<pqRenderView*>(
    builder->createView(pqRenderView::renderViewType(),
      pqActiveObjects::instance().activeServer()));
  pqActiveObjects::instance().setActiveView(view);

  // Place the widget for the render view in the frame provided.
  hbox->addWidget(view->getWidget());
  return view;
}

pqRenderView* ThreeSliceView::create2dRenderView(QWidget* widget)
{
	pqRenderView *view = this->createRenderView(widget);
	view->setCenterAxesVisibility(false);
	view->setOrientationAxesInteractivity(false);
	// Remove roll/rotate interactions from 2D view
	vtkSMPropertyHelper helper(view->getProxy(), "CameraManipulators");
	for (unsigned int cm = 0; cm < helper.GetNumberOfElements(); cm++)
	{
		vtkSMProxy* manip = helper.GetAsProxy(cm);
		if (manip &&
				(strcmp(manip->GetXMLName(), "TrackballRotate") == 0 ||
						strcmp(manip->GetXMLName(), "TrackballRoll") == 0))
		{
			helper.Remove(manip);
			cm--;
		}
	}

	return view;
}
pqRenderView* ThreeSliceView::getView()
{
	return this->mainView.data();
}
