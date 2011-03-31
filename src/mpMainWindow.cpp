#include "mpMainWindow.h"

#include "TimeControlWidget.h"
#include "pq3DWidget.h"
#include "pq3DWidgetFactory.h"
#include "pqActiveObjects.h"
#include "pqApplicationCore.h"
#include "pqDefaultViewBehavior.h"
#include "pqImplicitPlaneWidget.h"
#include "pqLoadDataReaction.h"
#include "pqObjectBuilder.h"
#include "pqObjectInspectorWidget.h"
#include "pqOutputPort.h"
#include "pqParaViewBehaviors.h"
#include "pqPipelineRepresentation.h"
#include "pqPipelineSource.h"
#include "pqRenderView.h"
#include "pqServer.h"
#include "pqServerResource.h"
#include "vtkDataObject.h"
#include "vtkProperty.h"
#include "vtkPVDataInformation.h"
#include "vtkSMImplicitPlaneRepresentationProxy.h"
#include "vtkSMNewWidgetRepresentationProxy.h"
#include "vtkSMPropertyHelper.h"
#include "vtkSMProxyManager.h"
#include "vtkSMReaderFactory.h"
#include "vtkSMRenderViewProxy.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"

#include <QHBoxLayout>

#include <iostream>

mpMainWindow::mpMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
  this->setupUi(this);
  new pqParaViewBehaviors(this, this);
  // We want the actionLoad to result in the showing up the ParaView's OpenData
  // dialog letting the user pick from one of the supported file formats.
  pqLoadDataReaction* dataLoader = new pqLoadDataReaction(this->action_Open);
  QObject::connect(dataLoader, SIGNAL(loadedData(pqPipelineSource*)),
    this, SLOT(onDataLoaded(pqPipelineSource*)));
  
  // Set the cut button to create a slice on the data
  QObject::connect(this->cutButton, SIGNAL(clicked()), this,
          SLOT(onCutButtonClicked()));

  // Set the rebin button to create the RebinCutter operator
  QObject::connect(this->rebinButton, SIGNAL(clicked()), this,
          SLOT(onRebinButtonClicked()));
  //pqUndoReaction
  //QObject::connect()

  // Commented this out to only use Mantid supplied readers
  // Initialize all readers available to ParaView. Now our application can load
  // all types of datasets supported by ParaView.
  vtkSMProxyManager::GetProxyManager()->GetReaderFactory()->RegisterPrototypes("sources");

  // Create the view.
  this->View = this->createRenderView(this->renderFrame);

  // Extra setup stuff to hook up view to other items
  this->proxyTabWidget->setView(this->View);
  this->proxyTabWidget->setShowOnAccept(true);
  this->pipelineBrowser->setActiveView(this->View);
}

mpMainWindow::~mpMainWindow()
{

}

pqRenderView* mpMainWindow::createRenderView(QWidget* widget)
{
  QHBoxLayout *hbox = new QHBoxLayout(widget);
  hbox->setMargin(0);

  // Create a new render view.
  pqObjectBuilder* builder = pqApplicationCore::instance()->getObjectBuilder();
  pqRenderView *view = qobject_cast<pqRenderView*>(
    builder->createView(pqRenderView::renderViewType(),
      pqActiveObjects::instance().activeServer()));

  // Place the widget for the render view in the frame provided.
  hbox->addWidget(view->getWidget());
  return view;
}

void mpMainWindow::onDataLoaded(pqPipelineSource* source)
{
  pqObjectBuilder* builder = pqApplicationCore::instance()->getObjectBuilder();
  if (this->OriginSource)
  {
      builder->destroy(this->OriginSource);
  }
  if (this->Slice)
  {
      builder->destroy(this->Slice);
  }
  if (this->RebinCut)
  {
	  builder->destroy(this->RebinCut);
  }
  this->OriginSource = source;

  // Show the data
  pqDataRepresentation *drep = builder->createDataRepresentation(
          this->OriginSource->getOutputPort(0), this->View);
  vtkSMPropertyHelper(drep->getProxy(), "Representation").Set(VTK_SURFACE);
  drep->getProxy()->UpdateVTKObjects();
  this->OriginSourceRepr = qobject_cast<pqPipelineRepresentation*>(drep);
  this->OriginSourceRepr->colorByArray("signal", vtkDataObject::FIELD_ASSOCIATION_CELLS);
  
  // Reset the camera to ensure that the data is visible.
  this->View->resetDisplay();
  
  // Trigger renders.
  this->View->render();
}

void mpMainWindow::onCutButtonClicked()
{
    // Apply cut to currently viewed data
    pqObjectBuilder* builder = pqApplicationCore::instance()->getObjectBuilder();
    this->Slice = builder->createFilter("filters", "Cut", pqActiveObjects::instance().activeSource());
}

void mpMainWindow::onRebinButtonClicked()
{
    pqObjectBuilder* builder = pqApplicationCore::instance()->getObjectBuilder();
    this->RebinCut = builder->createFilter("filters", "RebinningCutter",
    		pqActiveObjects::instance().activeSource());
}
