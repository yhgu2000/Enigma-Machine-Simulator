#pragma once

#include "EditRotorDialog.hpp"
#include <QFrame>
#include <array>

namespace Ui {
class RotorSlot;
}

class RotorSlot : public QFrame
{
  Q_OBJECT

signals:
  void handButton_pressed();
  void handButton_released();

public:
  explicit RotorSlot(QWidget* parent = nullptr);
  ~RotorSlot();

private:
  uint8_t _offset;
  Rotor _forwardRotor;
  Rotor _reverseRotor;

public: // 属性方法
  uint8_t get_offset() { return _offset; }
  void set_offset(uint8_t offset);

  const Rotor& get_rotor() { return _forwardRotor; }
  void set_rotor(const Rotor& rot);

public: // 功能性方法
  uint8_t forward_map(uint8_t pos);
  uint8_t reverse_map(uint8_t pos);

  /**
   * @brief step
   * @return 有进位返回 true，否则返回 false
   */
  bool step()
  {
    set_offset(_offset + 1);
    return _offset == 0;
  }

private:
  Ui::RotorSlot* ui;
  EditRotorDialog _editRotorDialog;

private slots:
  void on_editButton_clicked();
  void on_handButton_pressed();
  void on_handButton_released();
  void on_spinBox_valueChanged(int arg1);
  void on_dial_valueChanged(int value);

  void when_editRotorDialog_accepted();
};
