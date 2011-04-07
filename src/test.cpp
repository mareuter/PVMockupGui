#include <QApplication>
#include <QString>
#include "axisinteractor.h"

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  AxisInteractor *ai = new AxisInteractor();
  ai->setInformation(QString("test"), 0, 10);
  ai->show();
  return app.exec();
}
