#pragma once

#include "Rotor.hpp"
#include <QDialog>

namespace Ui {
class EditRotorDialog;
}

class EditRotorDialog : public QDialog
{
  Q_OBJECT

public:
  explicit EditRotorDialog(Rotor& r, QWidget* parent = nullptr);
  ~EditRotorDialog();

private:
  Ui::EditRotorDialog *ui;
  Rotor& _r;
};
