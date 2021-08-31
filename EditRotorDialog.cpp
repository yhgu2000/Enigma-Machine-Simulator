#include "EditRotorDialog.hpp"
#include "RotorSlot.hpp"
#include "ui_EditRotorDialog.h"

EditRotorDialog::EditRotorDialog(Rotor& r, QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::EditRotorDialog)
  , _r(r)
{
  ui->setupUi(this);
}

EditRotorDialog::~EditRotorDialog()
{
  delete ui;
}
