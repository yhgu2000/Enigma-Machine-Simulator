#include "RotorSlot.hpp"

#include "ui_RotorSlot.h"
#include <QDebug>


/** 一点感想
 * 此处的写法，每创建一个对象，就要拷贝两次 kDefaultRotor，而大部分情形下，
 * 紧接着创建者就会调用 set_rotor 重设，看上去好像是无意义的性能损耗，其实
 * 是为了代码安全付出的必要代价，就好像工厂为了安全生产而制定繁琐的守则一样。
 *
 * 经验表明，安全就是效率，这点付出是值得的。
 */
RotorSlot::RotorSlot(QWidget* parent)
  : QFrame(parent)
  , ui(new Ui::RotorSlot)
  , _offset(0)
{
  ui->setupUi(this);
  set_rotor(Rotor::kDefault);

  connect(&_editRotorDialog,
          &EditRotorDialog::accepted,
          this,
          &RotorSlot::when_editRotorDialog_accepted);
  connect(&_editRotorDialog,
          &EditRotorDialog::applied,
          this,
          &RotorSlot::when_editRotorDialog_accepted);
}

RotorSlot::~RotorSlot()
{
  delete ui;
}

void
RotorSlot::set_offset(uint8_t offset)
{
  _offset = offset %= kRotorMod;
  ui->spinBox->setValue(_offset);
  ui->dial->setValue(_offset);
}

void
RotorSlot::set_rotor(const Rotor& rot)
{
  _forwardRotor = rot;
  _forwardRotor.reverse_to(_reverseRotor);
  ui->label->setText(_forwardRotor.hash());
}

void
RotorSlot::retranslate_ui()
{
  ui->retranslateUi(this);
  _editRotorDialog.retranslate_ui();
}

uint8_t
RotorSlot::forward_map(uint8_t pos)
{
  uint8_t t = _forwardRotor[(pos + _offset) % kRotorMod] - _offset;
  return (t + kRotorMod) % kRotorMod;
}

uint8_t
RotorSlot::reverse_map(uint8_t pos)
{
  uint8_t t = _reverseRotor[(pos + _offset) % kRotorMod] - _offset;
  return (t + kRotorMod) % kRotorMod;
}

void
RotorSlot::on_editButton_clicked()
{
  _editRotorDialog.setWindowTitle(ui->label->text());
  _editRotorDialog.set_rotor(_forwardRotor);
  _editRotorDialog.show();
  _editRotorDialog.activateWindow();
}

void
RotorSlot::on_handButton_pressed()
{
  emit handButton_pressed();
}

void
RotorSlot::on_handButton_released()
{
  emit handButton_released();
}

void
RotorSlot::on_spinBox_valueChanged(int arg1)
{
  set_offset(uint8_t(arg1)); // 这叫 Modern C++（美滋滋）
}

void
RotorSlot::on_dial_valueChanged(int value)
{
  set_offset(uint8_t(value));
}

void
RotorSlot::when_editRotorDialog_accepted()
{
  set_rotor(_editRotorDialog.get_rotor());
  _editRotorDialog.setWindowTitle(ui->label->text());
}
