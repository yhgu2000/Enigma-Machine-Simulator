#include "EnigmaMachine.hpp"
#include "RotorSeparator.hpp"
#include "RotorSlot.hpp"
#include "ui_EnigmaMachine.h"
#include <QDebug>
#include <QToolTip>

EnigmaMachine::EnigmaMachine(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::EnigmaMachine)
{
  ui->setupUi(this);
}

EnigmaMachine::~EnigmaMachine()
{
  delete ui;
}

void
EnigmaMachine::setFocus()
{
  ui->machineIn->setFocus();
}

void
EnigmaMachine::on_machineIn_textEdited(const QString& arg1)
{
  if (arg1.size() == 0) {
    return;
  }

  QChar plain = arg1.at(arg1.size() - 1);
  if (!plain.isLetter()) {
    ui->machineIn->clear();
    QToolTip::showText(
      ui->machineIn->mapToGlobal(QPoint(0, ui->machineIn->height() >> 1)),
      "⚠ enigma only accepts letters",
      this,
      QRect(),
      1000);
    return;
  }
  ui->machineIn->setText(plain);

  // 推进一步
  for (auto i : _rotorSlots) {
    if (!i->step())
      break;
  }

  bool isLower = plain.isLower();
  uint8_t x;
  if (isLower)
    x = uint8_t(plain.toLatin1() - 'a');
  else
    x = uint8_t(plain.toLatin1() - 'A');

  // 正射
  for (int i = 0; i < _rotorSlots.size(); ++i) {
    auto& rsl = *(_rotorSlots[i]);
    auto& rse = *(_rotorSeps[i]);

    rse.set_wireL2R(x);
    x = rsl.forward_map(x);
  }

  // 反射器
  ui->reflectorIn->setText(QChar(x + 'A'));
  x = _reflector[x];
  ui->reflectorOut->setText(QChar(x + 'A'));

  // 反射
  for (auto i = _rotorSlots.size(); --i != -1;) {
    auto& rsl = *(_rotorSlots[i]);
    auto& rse = *(_rotorSeps[i]);

    rse.set_wireR2L(x);
    x = rsl.reverse_map(x);
  }

  // 输出
  QChar cipher;
  if (isLower)
    cipher = QChar(x + 'a');
  else
    cipher = QChar(x + 'A');

  ui->machineOut->setText(plain);

  emit charEncoded(plain, cipher);
}

void
EnigmaMachine::on_newRotorButton_clicked()
{
  auto rsl = new RotorSlot();
  auto rse = new RotorSeparator();

  _rotorSlots.push_front(rsl);
  _rotorSeps.push_front(rse);

  ui->rotorSetLayout->insertWidget(0, rsl);
  ui->rotorSetLayout->insertWidget(0, rse);
}
