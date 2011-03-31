#include "standardview.h"

#include "pqActiveObjects.h"
#include "pqApplicationCore.h"
#include "pqObjectBuilder.h"
#include "pqPipelineSource.h"
#include "pqRenderView.h"

#include <QHBoxLayout>

StandardView::StandardView(QWidget *parent) : QWidget(parent)
{
	this->setupUi(this);

	// Set the cut button to create a slice on the data
	QObject::connect(this->cutButton, SIGNAL(clicked()), this,
			SLOT(onCutButtonClicked()));

	// Set the rebin button to create the RebinCutter operator
	QObject::connect(this->rebinButton, SIGNAL(clicked()), this,
			SLOT(onRebinButtonClicked()));

	this->view = this->createRenderView(this->renderFrame);
}

StandardView::~StandardView()
{

}

pqRenderView* StandardView::createRenderView(QWidget* widget)
{
  QHBoxLayout *hbox = new QHBoxLayout(widget);
  hbox->setMargin(0);

  // Create a new render view.
  pqObjectBuilder* builder = pqApplicationCore::instance()->getObjectBuilder();
  pqRenderView *view = qobject_cast<pqRenderView*>(
    builder->createView(pqRenderView::renderViewType(),
      pqActiveObjects::instance().activeServer()));
  pqActiveObjects::instance().setActiveView(view);

  // Place the widget for the render view in the frame provided.
  hbox->addWidget(view->getWidget());
  return view;
}

pqRenderView* StandardView::getView()
{
	return this->view.data();
}

void StandardView::onCutButtonClicked()
{
    // Apply cut to currently viewed data
    pqObjectBuilder* builder = pqApplicationCore::instance()->getObjectBuilder();
    builder->createFilter("filters", "Cut", pqActiveObjects::instance().activeSource());
}

void StandardView::onRebinButtonClicked()
{
    pqObjectBuilder* builder = pqApplicationCore::instance()->getObjectBuilder();
    this->rebinCut = builder->createFilter("filters", "RebinningCutter",
    		pqActiveObjects::instance().activeSource());
}
