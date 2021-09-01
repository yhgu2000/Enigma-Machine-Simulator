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
      [this, i](ControlledCheckBox& self) {
        switch (self.checkState()) {
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

    auto right = new ControlledRadioButton(
      [this, i](ControlledRadioButton& self) {
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
EditRotorDialog::paintEvent(QPaintEvent*)
{
  QPainter pt(this);

  for (uint8_t i = 0; i < kRotorMod; ++i) {
    //    _rights[i]->setChecked(false);
  }

  for (uint8_t i = 0; i < kRotorMod; ++i) {
    auto& left = *_lefts[i];
    auto j = _rotor[i];
    if (j < kRotorMod) {
      auto& right = *_rights[j];

      //      left.setChecked(true);
      //      right.setChecked(true);

      auto start = left.pos();
      start.rx() += left.width();
      start.ry() += left.height() >> 1;
      start = left.parentWidget()->mapTo(this, start);

      auto stop = right.pos();
      stop.ry() += right.height() >> 1;
      stop = right.parentWidget()->mapTo(this, stop);

      pt.drawLine(start, stop);

    } else {
      //      left.setChecked(false);
    }
  }

  if (_leftSelected != 0xff) {
    auto& left = *_lefts[_leftSelected];
    auto start = left.pos();
    start.rx() += left.width();
    start.ry() += left.height() >> 1;

    // 得到的是相对父容器的坐标，需要转化为this的相对坐标
    start = left.parentWidget()->mapTo(this, start);
    pt.drawLine(start, _mousePos);
  }
}

void
EditRotorDialog::mouseMoveEvent(QMouseEvent* event)
{
  // 记录鼠标位置并通知窗口更新
  _mousePos = event->pos();
  update();
}

void
EditRotorDialog::ControlledCheckBox::mouseReleaseEvent(QMouseEvent* event)
{
  QCheckBox::mouseReleaseEvent(event);
  _onClick(*this);
}

void
EditRotorDialog::ControlledRadioButton::mouseReleaseEvent(QMouseEvent* event)
{
  // 父函数放在前面，可能会导致它先行修改了组件的状态；
  // 放在后面，又担心它可能会覆盖掉 _onClick 的修改。
  // 总之，没有万全方法呀！
  //
  // 通常写在前面要好一点，因为 _onClick 通常是基于父类的行为来编写的，
  // 是在父类的行为之后追加一些行为，所以把 _onClick 的调用写在后面。
  QRadioButton::mouseReleaseEvent(event);
  _onClick(*this);
}
