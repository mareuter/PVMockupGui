#include "mpmainwindow.h"

#include "multisliceview.h"
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

mpMainWindow::mpMainWindow(QWidget *parent) : QMainWindow(parent)
{
  this->setupUi(this);

  new pqParaViewBehaviors(this, this);

  // We want the actionLoad to result in the showing up the ParaView's OpenData
  // dialog letting the user pick from one of the supported file formats.
  pqLoadDataReaction* dataLoader = new pqLoadDataReaction(this->action_Open);
  QObject::connect(dataLoader, SIGNAL(loadedData(pqPipelineSource*)),
    this, SLOT(onDataLoaded(pqPipelineSource*)));

  QObject::connect(this->modeControlWidget, SIGNAL(executeSwitchViews()),
		  this, SLOT(switchViews()));

  //pqUndoReaction
  //QObject::connect()

  // Commented this out to only use Mantid supplied readers
  // Initialize all readers available to ParaView. Now our application can load
  // all types of datasets supported by ParaView.
  vtkSMProxyManager::GetProxyManager()->GetReaderFactory()->RegisterPrototypes("sources");

  // Set the standard view as the default
  this->currentView = this->setMainViewWidget(this->viewWidget,
		  mpMainWindow::STANDARD);
  this->setMainWindowComponentsForView();

  // Set the three slice view as hidden view for later use
  //this->hiddenView = this->setMainViewWidget(this->viewWidget,
	//	  mpMainWindow::THREESLICE);
  this->hiddenView = this->setMainViewWidget(this->viewWidget,
		  mpMainWindow::MULTISLICE);
  this->hiddenView->hide();

  // Disable all view buttons until data load
  emit disableViewModes();
}

mpMainWindow::~mpMainWindow()
{

}

IView* mpMainWindow::setMainViewWidget(QWidget *container, Views v)
{
	IView *view;
	switch(v)
	{
	case mpMainWindow::STANDARD:
	{
		StandardView *sv = new StandardView(container);
		view = sv;
	}
	break;
	case mpMainWindow::THREESLICE:
	{
		ThreeSliceView *tsv = new ThreeSliceView(container);
		view = tsv;
	}
	break;
	case mpMainWindow::MULTISLICE:
	{
		MultiSliceView *msv = new MultiSliceView(container);
		view = msv;
	}
	break;
	}
	return view;
}

void mpMainWindow::setMainWindowComponentsForView()
{
	// Extra setup stuff to hook up standard view to other items
	this->proxyTabWidget->setView(this->currentView->getView());
	this->proxyTabWidget->setShowOnAccept(true);
	this->pipelineBrowser->setActiveView(this->currentView->getView());
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
  pqDataRepresentation *drep = builder->createDataRepresentation(
		  this->originSource->getOutputPort(0), this->currentView->getView());
  vtkSMPropertyHelper(drep->getProxy(), "Representation").Set(VTK_SURFACE);
  drep->getProxy()->UpdateVTKObjects();
  this->originSourceRepr = qobject_cast<pqPipelineRepresentation*>(drep);
  this->originSourceRepr->colorByArray("signal",
		  vtkDataObject::FIELD_ASSOCIATION_CELLS);
  
  this->currentView->render();
  //emit enableThreeSlice();
  emit enableMultiSlice();
}

void mpMainWindow::switchViews()
{
	this->currentView->hide();
	this->swapViews();
	this->currentView->show();
	this->setMainWindowComponentsForView();
	this->currentView->render();
}

void mpMainWindow::swapViews()
{
	IView *temp;
	temp = this->currentView;
	this->currentView = this->hiddenView;
	this->hiddenView = temp;
}
