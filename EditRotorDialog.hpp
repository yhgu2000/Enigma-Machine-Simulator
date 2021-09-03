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

signals:
  void applied();

private:
  // C++ 的类千万别声明在 .cpp 里，因为类不能是 static 的！
  // 用 private 类声明实现命名空间的保护！

  // 把 QCheckBox 改造为类似 React 里面的无状态受控组件
  // 区别在于，没有一个框架实现自动 DIFF，需要手动编写更新代码
  //
  // 其实受不受控说多了都是是狗屁，本质上就是，我希望把这个组件“按下就选中”
  // 的逻辑剥离出来，实现按下事件交给高层处理。
  //
  // 实践证明，完全受控、不完全受控、完全不受控各有各适用的场景，
  // 使用核心还是弄清楚：组件上哪些逻辑是自己能处理的；哪些是应该留给高层处理的；
  // 哪些是比较琐碎的、应该让低级组件自行处理的！
  class ControlledCheckBox : public QCheckBox
  {
  public:
    using OnClick = std::function<void(ControlledCheckBox&)>;

  private:
    OnClick _onClick;

  public:
    explicit ControlledCheckBox(OnClick onClick, QWidget* parent = nullptr)
      : QCheckBox(parent)
      , _onClick(onClick)
    {} // 另一种方式是用 Qt 信号，不过效率应该略低一些

  protected:
    virtual void nextCheckState() override {} // 设为空以去除逻辑
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
  };

  class ControlledRadioButton : public QRadioButton
  {
  public:
    using OnClick = std::function<void(ControlledRadioButton&)>;

  private:
    OnClick _onClick;

  public:
    explicit ControlledRadioButton(OnClick onClick, QWidget* parent)
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
  void set_rotor(const Rotor& rotor);

  void retranslate_ui();

private:
  std::array<QCheckBox*, kRotorMod> _lefts;
  std::array<QRadioButton*, kRotorMod> _rights;
  uint8_t _leftSelected; // 0xff 表明没有选中
  QPoint _mousePos;

private:
  // 经典 MVC 模式
  void sync_m2v(); // 同步 model 到 view，需要在每次修改 model 后调用

protected:
  virtual void paintEvent(QPaintEvent* event) override;
  virtual void mouseMoveEvent(QMouseEvent* event) override;

private slots:
  void on_randomButton_clicked();
  void on_clearButton_clicked();
  void on_copyButton_clicked();
  void on_pasteButton_clicked();
  void on_buttonBox_clicked(QAbstractButton* button);
};
