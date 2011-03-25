#include "mpMainWindow.h"

#include "TimeControlWidget.h"
#include "pq3DWidgetFactory.h"
#include "pqActiveObjects.h"
#include "pqApplicationCore.h"
#include "pqDefaultViewBehavior.h"
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
  ui.setupUi(this);
  new pqParaViewBehaviors(this, this);
  // We want the actionLoad to result in the showing up the ParaView's OpenData
  // dialog letting the user pick from one of the supported file formats.
  pqLoadDataReaction* dataLoader = new pqLoadDataReaction(ui.action_Open);
  QObject::connect(dataLoader, SIGNAL(loadedData(pqPipelineSource*)),
    this, SLOT(onDataLoaded(pqPipelineSource*)));
  
  // Set the cut button to create a slice on the data
  QObject::connect(ui.cutButton, SIGNAL(clicked()), this,
          SLOT(onCutButtonClicked()));

  // Set the rebin button to create the RebinCutter operator
  QObject::connect(ui.rebinButton, SIGNAL(clicked()), this, 
          SLOT(onRebinButtonClicked()));
  //pqUndoReaction
  //QObject::connect()

  // Initialize all readers available to ParaView. Now our application can load
  // all types of datasets supported by ParaView.
  vtkSMProxyManager::GetProxyManager()->GetReaderFactory()->RegisterPrototypes("sources");

  // Create the view.
  this->View = this->createRenderView(ui.renderFrame);
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
  if (this->ActiveSource)
  {
      builder->destroy(this->ActiveSource);
  }
  if (this->Slice)
  {
      builder->destroy(this->Slice);
  }
  this->ActiveSource = source;

  // Show the data
  pqDataRepresentation *drep = builder->createDataRepresentation(
          this->ActiveSource->getOutputPort(0), this->View);
  this->ActiveSourceRepr = qobject_cast<pqPipelineRepresentation*>(drep);
  
  // Reset the camera to ensure that the data is visible.
  this->View->resetDisplay();
  
  // Trigger renders.
  this->View->render();
}

void mpMainWindow::onCutButtonClicked()
{
    // Apply cut to currently viewed data
    pqObjectBuilder* builder = pqApplicationCore::instance()->getObjectBuilder();
    this->Slice = builder->createFilter("filters", "Cut", this->ActiveSource);
    pqDataRepresentation *srep = builder->createDataRepresentation(
            this->Slice->getOutputPort(0), this->View); 
    this->SliceRepr = qobject_cast<pqPipelineRepresentation *>(srep);
    this->ActiveSourceRepr->setVisible(false);

    QHBoxLayout *hbox = new QHBoxLayout(this->ui.tab);
    pqObjectInspectorWidget* inspector = new pqObjectInspectorWidget(this->ui.tab);
    hbox->addWidget(inspector);
    inspector->setProxy(this->Slice);
    /*
    vtkSMNewWidgetRepresentationProxy* widget =
            pqApplicationCore::instance()->get3DWidgetFactory()->
            get3DWidget("ImplicitPlaneWidgetRepresentation", 
            pqActiveObjects::instance().activeServer());
    vtkSMPropertyHelper(widget, "Enabled").Set(0, true);
    vtkSMPropertyHelper(widget, "Visibility").Set(0, true);
    vtkSMPropertyHelper(widget, "DrawPlane").Set(0, true);
    std::cout << "Help" << std::endl;
    */
    //widget->UpdateVTKObjects();
    //widget->UpdatePropertyInformation();

    this->View->render();
}

void mpMainWindow::onRebinButtonClicked()
{
    pqObjectBuilder* builder = pqApplicationCore::instance()->getObjectBuilder();
    this->RebinCut = builder->createFilter("filters", "RebinningCutter", 
            this->ActiveSource);
    pqDataRepresentation *srep = builder->createDataRepresentation(
            this->RebinCut->getOutputPort(0), this->View); 
    this->RebinCutRepr = qobject_cast<pqPipelineRepresentation *>(srep);
    //this->ActiveSourceRepr->setVisible(false);

    QHBoxLayout *hbox = new QHBoxLayout(this->ui.tab_2);
    pqObjectInspectorWidget* inspector = new pqObjectInspectorWidget(this->ui.tab_2);
    hbox->addWidget(inspector);
    inspector->setProxy(this->RebinCut);

}
