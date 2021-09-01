#include "EditRotorDialog.hpp"
#include "RotorSlot.hpp"
#include "ui_EditRotorDialog.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

EditRotorDialog::EditRotorDialog(const Rotor& r, QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::EditRotorDialog)
  , _rotor(r)
  , _leftSelected(0xff)
{
  ui->setupUi(this);

  // 必须下面两行都写才能无条件在鼠标移动时触发 mouseMoveEvent
  setMouseTracking(true);
  ui->mappingBox->setMouseTracking(true);

  // 创建界面
  for (uint8_t i = 0; i < kRotorMod; ++i) {
    using CS = Qt::CheckState;

    QChar ch('A' + i);

    auto left = new ControlledCheckBox(
      [this, i]() {
        if (_leftSelected == i)
          return CS::PartiallyChecked;
        if (_rotor[i] < kRotorMod)
          return CS::Checked;
        return CS::Unchecked;
      },
      [this, i](CS s) {
        switch (s) {
          case CS::Checked: {
            _rotor[i] = 0xff;
          } break;

          case CS::Unchecked: {
            _leftSelected = i;
          } break;

          case CS::PartiallyChecked: {
            _leftSelected = 0xff;
          } break;
        }
        update();
      },
      this); // 这就叫 Modern ！
    left->setText(ch);
    left->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
    ui->leftLayout->addWidget(left);
    _lefts[i] = left;

    auto right = new ControllableRadioButton(
      [this, i](ControllableRadioButton& self) {
        if (_leftSelected == 0xff)
          return;
        _rotor[_leftSelected] = i;
        _leftSelected = 0xff;
        self.setChecked(true);
      },
      this);
    right->setText(ch);
    //    right->setCheckable(false);
    right->setAutoExclusive(false);
    ui->rightLayout->addWidget(right);
    _rights[i] = right;
  }
}

EditRotorDialog::~EditRotorDialog()
{
  delete ui;
}

void
EditRotorDialog::paintEvent(QPaintEvent* event)
{
  //  QPainter pt(this);

  //  if (_leftSelected != 0xff) {
  //    auto& left = *_lefts[_leftSelected];
  //    auto start = left.pos();
  //    start.rx() += left.width();
  //    start.ry() += left.height() >> 1;

  //    // 得到的是相对父容器的坐标，需要转化为this的相对坐标
  //    start = left.parentWidget()->mapTo(this, start);
  //    pt.drawLine(start, _mousePos);
  //  }

  //  for (uint8_t i = 0; i < kRotorMod; ++i) {
  //    auto j = _rotor[i];
  //    if (j < kRotorMod) {
  //      auto& left = *_lefts[i];
  //      auto& right = *_rights[j];

  //      auto start = left.pos();
  //      start.rx() += left.width();
  //      start.ry() += left.height() >> 1;
  //      start = left.parentWidget()->mapTo(this, start);

  //      auto stop = right.pos();
  //      stop.ry() += right.height() >> 1;
  //      stop = right.parentWidget()->mapTo(this, stop);

  //      pt.drawLine(start, stop);
  //    }
  //  }

  qDebug() << "-1";
}

void
EditRotorDialog::mouseMoveEvent(QMouseEvent* event)
{
  // 记录鼠标位置并通知窗口更新
  _mousePos = event->pos();
  update();

  qDebug() << "0";
}

void
EditRotorDialog::ControlledCheckBox::paintEvent(QPaintEvent* event)
{
  // 下面两行导致 paintEvent 被无限循环调用，耗尽CPU资源！
  setCheckState(_getCheckState());
  QCheckBox::paintEvent(event);

  qDebug() << "1";
}

void
EditRotorDialog::ControlledCheckBox::mousePressEvent(QMouseEvent* event)
{
  _onClick(checkState());

  qDebug() << "2";
}

void
EditRotorDialog::ControllableRadioButton::mouseReleaseEvent(QMouseEvent* event)
{
  QRadioButton::mouseReleaseEvent(event);
  _onClick(*this);

  qDebug() << "3";
}
