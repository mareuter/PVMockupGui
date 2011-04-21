#include "mpmainwindow.h"

#include "modecontrolwidget.h"
#include "multisliceview.h"
#include "standardview.h"
#include "threesliceview.h"
#include "timecontrolwidget.h"

#include "pqActiveObjects.h"
#include "pqApplicationCore.h"
#include "pqDataRepresentation.h"
#include "pqLoadDataReaction.h"
#include "pqObjectBuilder.h"
#include "pqObjectInspectorWidget.h"
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
#include <QModelIndex>

#include <iostream>

mpMainWindow::mpMainWindow(QWidget *parent) : QMainWindow(parent)
{
  this->setupUi(this);

  // Unset the connections since the views aren't up yet.
  this->removeProxyTabWidgetConnections();

  new pqParaViewBehaviors(this, this);

  // We want the actionLoad to result in the showing up the ParaView's OpenData
  // dialog letting the user pick from one of the supported file formats.
  pqLoadDataReaction* dataLoader = new pqLoadDataReaction(this->action_Open);
  QObject::connect(dataLoader, SIGNAL(loadedData(pqPipelineSource*)),
    this, SLOT(onDataLoaded(pqPipelineSource*)));

  QObject::connect(this->modeControlWidget,
		  SIGNAL(executeSwitchViews(ModeControlWidget::Views)),
		  this, SLOT(switchViews(ModeControlWidget::Views)));

  //pqUndoReaction
  //QObject::connect()

  // Commented this out to only use Mantid supplied readers
  // Initialize all readers available to ParaView. Now our application can load
  // all types of datasets supported by ParaView.
  vtkSMProxyManager::GetProxyManager()->GetReaderFactory()->RegisterPrototypes("sources");

  // Set the standard view as the default
  this->currentView = this->setMainViewWidget(this->viewWidget,
		  ModeControlWidget::STANDARD);
  this->setMainWindowComponentsForView();

  // Set the three slice view as hidden view for later use
  //this->hiddenView = this->setMainViewWidget(this->viewWidget,
  //ModeControlWidget::THREESLICE);
  //this->hiddenView = this->setMainViewWidget(this->viewWidget,
	//	  ModeControlWidget::MULTISLICE);
  //this->hiddenView->hide();
}

mpMainWindow::~mpMainWindow()
{

}

void mpMainWindow::removeProxyTabWidgetConnections()
{
	QObject::disconnect(&pqActiveObjects::instance(), 0,
			this->proxyTabWidget, 0);
}

IView* mpMainWindow::setMainViewWidget(QWidget *container,
		ModeControlWidget::Views v)
{
	IView *view;
	switch(v)
	{
	case ModeControlWidget::STANDARD:
	{
		StandardView *sv = new StandardView(container);
		view = sv;
	}
	break;
	case ModeControlWidget::THREESLICE:
	{
		ThreeSliceView *tsv = new ThreeSliceView(container);
		view = tsv;
	}
	break;
	case ModeControlWidget::MULTISLICE:
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
	// Extra setup stuff to hook up view to other items
	this->proxyTabWidget->setupDefaultConnections();
	this->proxyTabWidget->setView(this->currentView->getView());
	this->proxyTabWidget->setShowOnAccept(true);
	this->pipelineBrowser->setActiveView(this->currentView->getView());
	if (this->currentView->inherits("MultiSliceView"))
	{
		QObject::connect(this->pipelineBrowser,
				SIGNAL(clicked(const QModelIndex &)),
				static_cast<MultiSliceView *>(this->currentView),
				SLOT(selectIndicator()));
		QObject::connect(this->proxyTabWidget->getObjectInspector(),
				SIGNAL(accepted()),
				static_cast<MultiSliceView *>(this->currentView),
				SLOT(updateSelectedIndicator()));
	}
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
  emit enableModeButtons();
}

void mpMainWindow::switchViews(ModeControlWidget::Views v)
{
	this->removeProxyTabWidgetConnections();
	this->hiddenView = this->setMainViewWidget(this->viewWidget, v);
	this->hiddenView->hide();
	this->swapViews();
	this->currentView->show();
	this->hiddenView->hide();
	this->setMainWindowComponentsForView();
	this->currentView->render();
	this->hiddenView->close();
	delete this->hiddenView;
	if (this->currentView->inherits("ThreeSliceView") ||
			this->currentView->inherits("StandardView"))
	{
		this->proxyTabWidget->getObjectInspector()->accept();
	}
}

void mpMainWindow::swapViews()
{
	IView *temp;
	temp = this->currentView;
	this->currentView = this->hiddenView;
	this->hiddenView = temp;
}
