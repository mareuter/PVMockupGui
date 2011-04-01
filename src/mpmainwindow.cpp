#include "mpmainwindow.h"

#include "standardview.h"
#include "threesliceview.h"
#include "timecontrolwidget.h"

#include "pqActiveObjects.h"
#include "pqApplicationCore.h"
#include "pqDataRepresentation.h"
#include "pqLoadDataReaction.h"
#include "pqObjectBuilder.h"
#include "pqParaViewBehaviors.h"
#include "pqPipelineRepresentation.h"
#include "pqPipelineSource.h"
#include "pqPluginManager.h"
#include "pqRenderView.h"
#include "pqServerResource.h"
#include "pqStandardViewModules.h"
#include "vtkDataObject.h"
#include "vtkProperty.h"
#include "vtkPVDataInformation.h"
#include "vtkSMPropertyHelper.h"
#include "vtkSMProxyManager.h"
#include "vtkSMReaderFactory.h"

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

  QObject::connect(this->standardViewButton, SIGNAL(clicked()),
		  this, SLOT(onStandardViewButtonClicked()));
  QObject::connect(this->threeSliceViewButton, SIGNAL(clicked()),
  		  this, SLOT(onThreeSliceViewButtonClicked()));

  //pqUndoReaction
  //QObject::connect()

  // Commented this out to only use Mantid supplied readers
  // Initialize all readers available to ParaView. Now our application can load
  // all types of datasets supported by ParaView.
  vtkSMProxyManager::GetProxyManager()->GetReaderFactory()->RegisterPrototypes("sources");

  // Set the standard view as the default
  //this->sview = new StandardView(this->viewWidget);
  //this->setMainWindowComponentsForStandardView();

  //Set the three slice view as the default
  this->tsview = new ThreeSliceView(this->viewWidget);
  this->setMainWindowComponentsForThreeSliceView();

  // Shutting off buttons for now to understand behavior
  this->standardViewButton->setEnabled(false);
  this->threeSliceViewButton->setEnabled(false);
}

mpMainWindow::~mpMainWindow()
{

}

void mpMainWindow::setMainWindowComponentsForStandardView()
{
	// Extra setup stuff to hook up standard view to other items
	this->proxyTabWidget->setView(this->sview->getView());
	this->proxyTabWidget->setShowOnAccept(true);
	this->pipelineBrowser->setActiveView(this->sview->getView());
}

void mpMainWindow::setMainWindowComponentsForThreeSliceView()
{
	// Extra setup stuff to hook up three slice view to other items
	this->proxyTabWidget->setView(this->tsview->getView());
	this->proxyTabWidget->setShowOnAccept(true);
	this->pipelineBrowser->setActiveView(this->tsview->getView());
}

void mpMainWindow::onDataLoaded(pqPipelineSource* source)
{
  pqObjectBuilder* builder = pqApplicationCore::instance()->getObjectBuilder();
  if (this->originSource)
  {
      builder->destroy(this->originSource);
  }
  this->originSource = source;

  // Show the data
  //pqDataRepresentation *drep = builder->createDataRepresentation(
  //        this->originSource->getOutputPort(0), this->sview->getView());
  pqDataRepresentation *drep = builder->createDataRepresentation(
		  this->originSource->getOutputPort(0), this->tsview->getView());
  vtkSMPropertyHelper(drep->getProxy(), "Representation").Set(VTK_SURFACE);
  drep->getProxy()->UpdateVTKObjects();
  this->originSourceRepr = qobject_cast<pqPipelineRepresentation*>(drep);
  this->originSourceRepr->colorByArray("signal", vtkDataObject::FIELD_ASSOCIATION_CELLS);
  
  this->tsview->makeThreeSlice(this->proxyTabWidget, this->pipelineBrowser);

  // Reset the camera to ensure that the data is visible.
  //this->sview->getView()->resetDisplay();
  //this->tsview->getView()->resetDisplay();
  // Trigger renders.
  //this->sview->getView()->render();
  //this->tsview->getView()->render();
}

void mpMainWindow::onStandardViewButtonClicked()
{
	this->tsview->hide();
	if (this->sview)
	{
		this->sview = new StandardView(this->viewWidget);
	}
	else
	{
		this->sview->show();
	}
	this->setMainWindowComponentsForStandardView();
	this->standardViewButton->setEnabled(false);
	this->threeSliceViewButton->setEnabled(true);
}

void mpMainWindow::onThreeSliceViewButtonClicked()
{
	this->sview->hide();
	if (this->tsview)
	{
		this->tsview = new ThreeSliceView(this->viewWidget);
	}
	else
	{
		this->tsview->show();
	}
	this->setMainWindowComponentsForThreeSliceView();
	this->threeSliceViewButton->setEnabled(false);
	this->standardViewButton->setEnabled(true);
}
