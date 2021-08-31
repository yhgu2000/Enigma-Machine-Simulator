#include "RotorSeparator.hpp"
#include "RotorSlot.hpp"
#include "ui_RotorSeparator.h"

RotorSeparator::RotorSeparator(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::RotorSeparator)
{
  ui->setupUi(this);
}

RotorSeparator::~RotorSeparator()
{
  delete ui;
}

QChar
RotorSeparator::translate_position(uint8_t pos)
{
  return 'A' + pos % kRotorMod;
}

void
RotorSeparator::set_wireL2R(uint8_t pos)
{
  ui->wireL2R->setText(QString(translate_position(pos)));
}

void
RotorSeparator::set_wireR2L(uint8_t pos)
{
  ui->wireR2L->setText(QString(translate_position(pos)));
}
