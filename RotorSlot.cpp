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
  , _editRotorDialog(_forwardRotor)
  , _offset(0)
{
  ui->setupUi(this);
  ui->label->setText(_forwardRotor.hash());
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
  ui->label->setText(_forwardRotor.hash());
}

uint8_t
RotorSlot::forward_map(uint8_t pos)
{
  return _forwardRotor[(pos + _offset) % kRotorMod] - _offset;
}

uint8_t
RotorSlot::reverse_map(uint8_t pos)
{
  return _reverseRotor[(pos + _offset) % kRotorMod] - _offset;
}

void
RotorSlot::on_editButton_clicked()
{

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
{}
