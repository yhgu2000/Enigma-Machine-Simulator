#include "EditRotorDialog.hpp"
#include "MainWindow.hpp"
#include "RotorSlot.hpp"

#include <QApplication>

int
main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  MainWindow w;
  w.show();

  return a.exec();
}
