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
#include "vtkSMPropertyHelper.h"
#include "vtkSMProxy.h"

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

void MultiSliceView::render()
{
	this->origSource = pqActiveObjects::instance().activeSource();

	pqObjectBuilder *builder = pqApplicationCore::instance()->getObjectBuilder();

	pqDataRepresentation *drep = builder->createDataRepresentation(
			this->origSource->getOutputPort(0), this->mainView);
	vtkSMPropertyHelper(drep->getProxy(), "Representation").Set(VTK_SURFACE);
	drep->getProxy()->UpdateVTKObjects();
	pqPipelineRepresentation *originSourceRepr = qobject_cast<pqPipelineRepresentation*>(drep);
	originSourceRepr->colorByArray("signal",
			vtkDataObject::FIELD_ASSOCIATION_CELLS);

	this->mainView->resetDisplay();
	this->mainView->render();
}
