#include "EditRotorDialog.hpp"
#include "RotorSlot.hpp"
#include "ui_EditRotorDialog.h"
#include <QCheckBox>
#include <QDebug>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QRadioButton>

EditRotorDialog::EditRotorDialog(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::EditRotorDialog)
  , _rotor()
  , _leftSelected(0xff)
{
  ui->setupUi(this);
  this->setMouseTracking(true);

  for (uint8_t i = 0; i < kRotorMod; ++i) {
    QChar ch('A' + i);

    auto left = new QCheckBox(this);
    _lefts[i] = left;
    ui->leftLayout->addWidget(left);
    left->setText(ch);
    left->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
    connect(left, &QCheckBox::stateChanged, [this, i](int s) {
      switch (s) {
        case Qt::CheckState::Checked: {
          _leftSelected = i;
        } break;

        case Qt::CheckState::Unchecked: {
          _leftSelected = 0xff;
        } break;

        default:
          break;
      }
    });

    auto right = new QRadioButton(this);
    _rights[i] = right;
    ui->rightLayout->addWidget(right);
    right->setText(ch);
  }
}

EditRotorDialog::~EditRotorDialog()
{
  delete ui;
}

void
EditRotorDialog::paintEvent(QPaintEvent* event)
{}

void
EditRotorDialog::mouseMoveEvent(QMouseEvent* event)
{
  qDebug() << "shit " << _leftSelected;

  if (_leftSelected == 0xff)
    return;

  QPainter pt;

  auto p1 = _lefts[_leftSelected]->pos();
  auto p2 = event->pos();
  pt.drawLine(p1, p2);
}
