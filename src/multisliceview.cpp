#include "multisliceview.h"

#include "axisinformation.h"
#include "geometryparser.h"
#include "scalepicker.h"

#include "pqActiveObjects.h"
#include "pqApplicationCore.h"
#include "pqDataRepresentation.h"
#include "pqObjectBuilder.h"
#include "pqPipelineBrowserWidget.h"
#include "pqPipelineRepresentation.h"
#include "pqPipelineSource.h"
#include "pqRenderView.h"
#include "pqServerManagerSelectionModel.h"
#include "vtkDataObject.h"
#include "vtkProperty.h"
#include "vtkSMProperty.h"
#include "vtkSMPropertyHelper.h"
#include "vtkSMProxy.h"
#include "vtkSMViewProxy.h"

#include <QModelIndex>
#include <QString>

#include <iostream>
MultiSliceView::MultiSliceView(QWidget *parent) : IView(parent)
{
	this->ui.setupUi(this);

	this->mainView = this->createRenderView(this->ui.renderFrame);

	QObject::connect(this->ui.xAxisWidget->getScalePicker(),
			SIGNAL(clicked(double)), this, SLOT(makeXcut(double)));
	QObject::connect(this->ui.yAxisWidget->getScalePicker(),
			SIGNAL(clicked(double)), this, SLOT(makeYcut(double)));
	QObject::connect(this->ui.zAxisWidget->getScalePicker(),
			SIGNAL(clicked(double)), this, SLOT(makeZcut(double)));

	QObject::connect(this, SIGNAL(sliceNamed(const QString &)),
			this->ui.xAxisWidget, SLOT(setIndicatorName(const QString &)));
	QObject::connect(this, SIGNAL(sliceNamed(const QString &)),
			this->ui.yAxisWidget, SLOT(setIndicatorName(const QString &)));
	QObject::connect(this, SIGNAL(sliceNamed(const QString &)),
			this->ui.zAxisWidget, SLOT(setIndicatorName(const QString &)));
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
	GeometryParser parser(geomXML);
	AxisInformation *xinfo = parser.getAxisInfo("XDimension");
	AxisInformation *yinfo = parser.getAxisInfo("YDimension");
	AxisInformation *zinfo = parser.getAxisInfo("ZDimension");

	this->ui.xAxisWidget->setInformation(QString(xinfo->getTitle().c_str()),
				xinfo->getMinimum(), xinfo->getMaximum());
	this->ui.yAxisWidget->setInformation(QString(yinfo->getTitle().c_str()),
				yinfo->getMinimum(), yinfo->getMaximum());
	this->ui.zAxisWidget->setInformation(QString(zinfo->getTitle().c_str()),
			zinfo->getMinimum(), zinfo->getMaximum());

	delete xinfo, yinfo, zinfo;
}

void MultiSliceView::render()
{
	this->origSource = pqActiveObjects::instance().activeSource();
	this->setupData();
	this->setupAxisInfo();
	this->mainView->resetDisplay();
	this->mainView->render();
}

void MultiSliceView::makeXcut(double value)
{
	double origin[3], orient[3];
	origin[0] = value;
	origin[1] = 0.0;
	origin[2] = 0.0;
	orient[0] = 1.0;
	orient[1] = 0.0;
	orient[2] = 0.0;
	this->makeCut(origin, orient);
}

void MultiSliceView::makeYcut(double value)
{
	double origin[3], orient[3];
	origin[0] = 0.0;
	origin[1] = value;
	origin[2] = 0.0;
	orient[0] = 0.0;
	orient[1] = 1.0;
	orient[2] = 0.0;
	this->makeCut(origin, orient);
}

void MultiSliceView::makeZcut(double value)
{
	double origin[3], orient[3];
	origin[0] = 0.0;
	origin[1] = 0.0;
	origin[2] = value;
	orient[0] = 0.0;
	orient[1] = 0.0;
	orient[2] = 1.0;
	this->makeCut(origin, orient);
}

void MultiSliceView::makeCut(double origin[], double orient[])
{
	pqObjectBuilder *builder = pqApplicationCore::instance()->getObjectBuilder();

	pqPipelineSource *cut = builder->createFilter("filters", "Cut",
			this->origSource);
	emit sliceNamed(cut->getSMName());
	pqDataRepresentation *trepr = builder->createDataRepresentation(
			cut->getOutputPort(0),this->mainView);
	pqPipelineRepresentation *repr = qobject_cast<pqPipelineRepresentation *>(trepr);
	vtkSMProxy *plane = vtkSMPropertyHelper(cut->getProxy(),
			"CutFunction").GetAsProxy();

	repr->colorByArray("signal", vtkDataObject::FIELD_ASSOCIATION_CELLS);

	vtkSMPropertyHelper(plane, "Origin").Set(origin, 3);
	vtkSMPropertyHelper(plane, "Normal").Set(orient, 3);
	trepr->getProxy()->UpdateVTKObjects();
	// This is how to get the name of the slice object in the current
	// pipeline.
	//std::cout << "Cut: " << cut->getSMName().toStdString() << std::endl;
}

void MultiSliceView::selectIndicator()
{
	pqServerManagerSelectionModel *smsModel = pqApplicationCore::instance()->getSelectionModel();
	pqPipelineSource *source = qobject_cast<pqPipelineSource *>(smsModel->currentItem());
	QString name = source->getSMName();
	this->ui.xAxisWidget->selectIndicator(name);
	this->ui.yAxisWidget->selectIndicator(name);
	this->ui.zAxisWidget->selectIndicator(name);
}
