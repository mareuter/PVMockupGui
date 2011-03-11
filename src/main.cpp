#include <QApplication>
#include "pqPVApplicationCore.h"
#include "mpMainWindow.h"

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  pqPVApplicationCore appCore(argc, argv);
  mpMainWindow window;
  window.show();
  return app.exec();
}
