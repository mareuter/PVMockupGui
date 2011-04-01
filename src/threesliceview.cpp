#include "threesliceview.h"

#include "pqActiveObjects.h"
#include "pqApplicationCore.h"
#include "pqDataRepresentation.h"
#include "pqObjectBuilder.h"
#include "pqPipelineBrowserWidget.h"
#include "pqPipelineRepresentation.h"
#include "pqPipelineSource.h"
#include "pqProxyTabWidget.h"
#include "pqRenderView.h"
#include "vtkDataObject.h"
#include "vtkSMPropertyHelper.h"
#include "vtkSMProxy.h"

#include <iostream>

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
  //pqActiveObjects::instance().setActiveView(view);

  // Place the widget for the render view in the frame provided.
  hbox->addWidget(view->getWidget());
  return view;
}

pqRenderView* ThreeSliceView::create2dRenderView(QWidget* widget)
{
	pqRenderView *view = this->createRenderView(widget);
	view->setCenterAxesVisibility(false);
	view->setOrientationAxesInteractivity(false);
	//view->setOrientationAxesVisibility(false);
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

void ThreeSliceView::makeSlice(ThreeSliceView::Direction i, pqRenderView *view,
		pqPipelineSource *cut, pqPipelineRepresentation *repr)
{
	this->pTab->setView(view);
	this->pBrow->setActiveView(view);

	pqObjectBuilder *builder = pqApplicationCore::instance()->getObjectBuilder();

	cut = builder->createFilter("filters", "Cut", this->origSource);
	pqDataRepresentation *trepr = builder->createDataRepresentation(
			cut->getOutputPort(0), view);
	repr = qobject_cast<pqPipelineRepresentation *>(trepr);
	vtkSMProxy *plane = vtkSMPropertyHelper(cut->getProxy(),
			"CutFunction").GetAsProxy();

	repr->colorByArray("signal", vtkDataObject::FIELD_ASSOCIATION_CELLS);

	double orient[3], up[3];
	switch(i)
	{
	case ThreeSliceView::X:
		orient[0] = 1.0;
		orient[1] = 0.0;
		orient[2] = 0.0;
		up[0] = 0.0;
		up[1] = 0.0;
		up[2] = 1.0;
		break;
	case ThreeSliceView::Y:
		orient[0] = 0.0;
		orient[1] = 1.0;
		orient[2] = 0.0;
		up[0] = 0.0;
		up[1] = 0.0;
		up[2] = 1.0;
		break;
	case ThreeSliceView::Z:
		orient[0] = 0.0;
		orient[1] = 0.0;
		orient[2] = 1.0;
		up[0] = 1.0;
		up[1] = 0.0;
		up[2] = 0.0;
		break;
	}
	vtkSMPropertyHelper(plane, "Normal").Set(orient, 3);
	trepr->getProxy()->UpdateVTKObjects();

	view->resetViewDirection(orient[0], orient[1], orient[2],
			up[0], up[1], up[2]);
}

void ThreeSliceView::makeThreeSlice(pqProxyTabWidget *ptw,
		pqPipelineBrowserWidget *pbw)
{
	this->pBrow = pbw;
	this->pTab = ptw;

	this->origSource = pqActiveObjects::instance().activeSource();

	this->makeSlice(ThreeSliceView::X, this->xView, this->xCut,
			this->xCutRepr);
	this->makeSlice(ThreeSliceView::Y, this->yView, this->yCut,
			this->yCutRepr);
	this->makeSlice(ThreeSliceView::Z, this->zView, this->zCut,
			this->zCutRepr);

	this->pBrow->setActiveView(this->mainView);
	this->pTab->setView(this->mainView);

	this->renderAll();
}

void ThreeSliceView::renderAll()
{
	this->mainView->resetDisplay();
	this->xView->resetDisplay();
	this->yView->resetDisplay();
	this->zView->resetDisplay();

	this->mainView->render();
	this->xView->render();
	this->yView->render();
	this->zView->render();
}
