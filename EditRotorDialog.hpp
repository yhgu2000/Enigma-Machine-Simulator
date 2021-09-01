#pragma once

#include "Rotor.hpp"
#include <QCheckBox>
#include <QDialog>
#include <QRadioButton>
#include <functional>

namespace Ui {
class EditRotorDialog;
}

class QCheckBox;
class QRadioButton;

class EditRotorDialog : public QDialog
{
  Q_OBJECT

private:
  // 把 QCheckBox 改造为类似 React 里面的无状态完全受控组件
  class ControlledCheckBox : public QCheckBox
  {
  public:
    using GetCheckState = std::function<Qt::CheckState()>;
    using OnClick = std::function<void(Qt::CheckState)>;

  private:
    GetCheckState _getCheckState;
    OnClick _onClick;

  public:
    explicit ControlledCheckBox(GetCheckState getCheckState,
                                OnClick onClick,
                                QWidget* parent = nullptr)
      : QCheckBox(parent)
      , _getCheckState(getCheckState)
      , _onClick(onClick)
    {} // 另一种方式是用 Qt 信号，不过效率应该略低一些

  protected:
    virtual void nextCheckState() override {}
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
  };
  // C++ 的类千万别声明在 .cpp 里，因为类不能是 static 的！
  // 用 private 类声明实现命名空间的保护！

  // 尽管组件和上面的类一样，不会私自对用户的操作进行响应，
  // 但和区别在于，组件自己持有状态，即，状态是分散的。
  // 或许这就叫“不完全受控”吧！
  // 实践证明，完全受控、不完全受控、完全不受控各有各适用的场景，
  // 使用核心还是弄清楚：组件上哪些逻辑是自己能处理的；哪些是应该留给高层处理的；
  // 哪些是比较琐碎的、应该让低级组件自行处理的！
  class ControllableRadioButton : public QRadioButton
  {
  public:
    using OnClick = std::function<void(ControllableRadioButton&)>;

  private:
    OnClick _onClick;

  public:
    explicit ControllableRadioButton(OnClick onClick, QWidget* parent)
      : QRadioButton(parent)
      , _onClick(onClick)
    {}

  protected:
    virtual void nextCheckState() override {}
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
  };

private:
  Ui::EditRotorDialog* ui;

public:
  explicit EditRotorDialog(const Rotor& r = Rotor::kDefault,
                           QWidget* parent = nullptr);
  ~EditRotorDialog() override;

private:
  Rotor _rotor; // 0xff 保留，表示未连接

public: // 属性方法
  const Rotor& get_rotor() { return _rotor; }

private:
  std::array<QCheckBox*, kRotorMod> _lefts;
  std::array<QRadioButton*, kRotorMod> _rights;
  uint8_t _leftSelected; // 0xff 表明没有选中
  QPoint _mousePos;

protected:
  virtual void paintEvent(QPaintEvent* event) override;
  virtual void mouseMoveEvent(QMouseEvent* event) override;
};
