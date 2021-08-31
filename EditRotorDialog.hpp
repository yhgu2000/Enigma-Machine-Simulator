#pragma once

#include "Rotor.hpp"
#include <QDialog>

namespace Ui {
class EditRotorDialog;
}

class QCheckBox;
class QRadioButton;

class EditRotorDialog : public QDialog
{
  Q_OBJECT

private:
  Ui::EditRotorDialog* ui;

public:
  explicit EditRotorDialog(QWidget* parent = nullptr);
  ~EditRotorDialog() override;

private:
  Rotor _rotor;

public: // 属性方法
  const Rotor& get_rotor() { return _rotor; }

private:
  uint8_t _leftSelected;
  std::array<QCheckBox*, kRotorMod> _lefts;
  std::array<QRadioButton*, kRotorMod> _rights;

protected:
  virtual void paintEvent(QPaintEvent* event) override;
  virtual void mouseMoveEvent(QMouseEvent* event) override;
};
