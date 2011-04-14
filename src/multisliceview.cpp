#include "multisliceview.h"

#include "pqActiveObjects.h"
#include "pqApplicationCore.h"
#include "pqDataRepresentation.h"
#include "pqObjectBuilder.h"
#include "pqPipelineBrowserWidget.h"
#include "pqPipelineRepresentation.h"
#include "pqPipelineSource.h"
#include "pqRenderView.h"
#include "vtkDataObject.h"
#include "vtkProperty.h"
#include "vtkSMProperty.h"
#include "vtkSMPropertyHelper.h"
#include "vtkSMProxy.h"
#include "vtkSMViewProxy.h"

#include <iostream>
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

void MultiSliceView::setupData()
{
	pqObjectBuilder *builder = pqApplicationCore::instance()->getObjectBuilder();

	pqDataRepresentation *drep = builder->createDataRepresentation(
			this->origSource->getOutputPort(0), this->mainView);
	vtkSMPropertyHelper(drep->getProxy(), "Representation").Set(VTK_SURFACE);
	drep->getProxy()->UpdateVTKObjects();
	this->originSourceRepr = qobject_cast<pqPipelineRepresentation*>(drep);
	this->originSourceRepr->colorByArray("signal",
			vtkDataObject::FIELD_ASSOCIATION_CELLS);
}

void MultiSliceView::setupAxisInfo()
{
	const char *geomXML = vtkSMPropertyHelper(this->origSource->getProxy(),
			"InputGeometryXML").GetAsString();
}

void MultiSliceView::render()
{
	this->origSource = pqActiveObjects::instance().activeSource();
	this->setupData();
	this->setupAxisInfo();
	this->mainView->resetDisplay();
	this->mainView->render();
}
