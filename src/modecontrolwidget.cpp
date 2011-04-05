#include "modecontrolwidget.h"

ModeControlWidget::ModeControlWidget(QWidget *parent)
    : QWidget(parent)
{
	this->ui.setupUi(this);

	QObject::connect(parent->parent(), SIGNAL(disableViewModes()), this,
			SLOT(disableAll()));
	QObject::connect(parent->parent(), SIGNAL(enableThreeSlice()), this,
			SLOT(enableThreeSliceButton()));
	QObject::connect(this->ui.standardButton, SIGNAL(clicked()),
			this, SLOT(onStandardViewButtonClicked()));
	QObject::connect(this->ui.threeSliceButton, SIGNAL(clicked()),
			this, SLOT(onThreeSliceViewButtonClicked()));
}

ModeControlWidget::~ModeControlWidget()
{

}

void ModeControlWidget::disableAll()
{
	this->ui.standardButton->setEnabled(false);
	this->ui.threeSliceButton->setEnabled(false);
}

void ModeControlWidget::enableThreeSliceButton()
{
	this->ui.threeSliceButton->setEnabled(true);
}

void ModeControlWidget::onStandardViewButtonClicked()
{
	emit executeSwitchViews();
	this->ui.standardButton->setEnabled(false);
	this->ui.threeSliceButton->setEnabled(true);
}

void ModeControlWidget::onThreeSliceViewButtonClicked()
{
	emit executeSwitchViews();
	this->ui.threeSliceButton->setEnabled(false);
	this->ui.standardButton->setEnabled(true);
}
