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

  // Commented this out to only use Mantid supplied readers
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

  // Create the source parameter options
  QWidget *tab = new QWidget();
  QHBoxLayout *hbox = new QHBoxLayout(tab);
  pqObjectInspectorWidget* inspector = new pqObjectInspectorWidget(tab);
  hbox->addWidget(inspector);
  inspector->setProxy(this->ActiveSource);
  this->ui.tabWidget->addTab(tab, this->ActiveSource->getSMName());

  // Show the data
  pqDataRepresentation *drep = builder->createDataRepresentation(
          this->ActiveSource->getOutputPort(0), this->View);
  vtkSMPropertyHelper(drep->getProxy(), "Representation").Set(VTK_SURFACE);
  drep->getProxy()->UpdateVTKObjects();
  this->ActiveSourceRepr = qobject_cast<pqPipelineRepresentation*>(drep);
  this->ActiveSourceRepr->colorByArray("signal", vtkDataObject::FIELD_ASSOCIATION_CELLS);
  
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

    QWidget *tab = new QWidget();
    QHBoxLayout *hbox = new QHBoxLayout(tab);
    pqObjectInspectorWidget* inspector = new pqObjectInspectorWidget(tab);
    hbox->addWidget(inspector);
    inspector->setProxy(this->Slice);
    this->ui.tabWidget->addTab(tab, "cut");
    emit this->ui.tabWidget->setCurrentWidget(tab);

    QList<pq3DWidget *> widgets = pq3DWidget::createWidgets(this->Slice->getProxy(),
    		vtkSMPropertyHelper(this->Slice->getProxy(), "CutFunction").GetAsProxy());
    Q_ASSERT(widgets.size() == 1);
    this->PlaneWidget = widgets[0];
    this->PlaneWidget->setView(this->View);
    this->PlaneWidget->select();
    this->PlaneWidget->resetBounds();

    QObject::connect(inspector->findChild<QWidget *>("show3DWidget"), SIGNAL(toggled(bool)),
    		this->PlaneWidget, SLOT(setWidgetVisible(bool)));
    QObject::connect(this->PlaneWidget, SIGNAL(widgetEndInteraction()), this->PlaneWidget, SLOT(accept()));
    QObject::connect(this->PlaneWidget, SIGNAL(widgetEndInteraction()), this->View, SLOT(render()));

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

    QList<pq3DWidget *> widgets = pq3DWidget::createWidgets(this->RebinCut->getProxy(),
    		vtkSMPropertyHelper(this->RebinCut->getProxy(), "ClipFunction").GetAsProxy());
    Q_ASSERT(widgets.size() == 1);
    this->BoxWidget = widgets[0];
    this->BoxWidget->setView(this->View);
    this->BoxWidget->setWidgetVisible(true);
    this->BoxWidget->select();
    this->BoxWidget->resetBounds();

    QObject::connect(this->BoxWidget, SIGNAL(widgetEndInteraction()), this->BoxWidget, SLOT(accept()));
    QObject::connect(this->BoxWidget, SIGNAL(widgetEndInteraction()), this->View, SLOT(render()));

    QWidget *tab = new QWidget();
    QHBoxLayout *hbox = new QHBoxLayout(tab);
    pqObjectInspectorWidget* inspector = new pqObjectInspectorWidget(tab);
    hbox->addWidget(inspector);
    inspector->setProxy(this->RebinCut);
    this->ui.tabWidget->addTab(tab, "rebincut");
    emit this->ui.tabWidget->setCurrentWidget(tab);

}
