#include "EditRotorDialog.hpp"
#include "MainDialog.hpp"
#include "RotorSlot.hpp"

#include <QApplication>

int
main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  //  MainDialog w;
  //  w.show();

  EditRotorDialog d;
  d.show();

  return a.exec();
}
