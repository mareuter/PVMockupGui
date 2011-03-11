#include "mpMainWindow.h"

#include "TimeControlWidget.h"
#include "pqActiveObjects.h"
#include "pqApplicationCore.h"
#include "pqDefaultViewBehavior.h"
#include "pqLoadDataReaction.h"
#include "pqObjectBuilder.h"
#include "pqOutputPort.h"
#include "pqParaViewBehaviors.h"
#include "pqPipelineSource.h"
#include "pqRenderView.h"
#include "pqServer.h"
#include "vtkSMProxyManager.h"
#include "vtkSMReaderFactory.h"

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
  this->ActiveSource = source;

  // Show the data
  builder->createDataRepresentation(this->ActiveSource->getOutputPort(0), this->View);

  QPointer<pqPipelineSource> slice = builder->createFilter("filters", "Cut",
            source);
  builder->createDataRepresentation(slice->getOutputPort(0), this->View);

  // Reset the camera to ensure that the data is visible.
  this->View->resetDisplay();
  
  // Trigger renders.
  this->View->render();
}

void mpMainWindow::onCutButtonClicked()
{
    pqObjectBuilder* builder = pqApplicationCore::instance()->getObjectBuilder();
    QPointer<pqPipelineSource> slice = builder->createFilter("filters", "Cut",
            this->ActiveSource);
    builder->createDataRepresentation(slice->getOutputPort(0), this->View);
    this->View->render();
}