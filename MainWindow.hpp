#pragma once

#include <EnigmaMachine.hpp>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

private:
  Ui::MainWindow* ui;
  EnigmaMachine _em;

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;

protected:
  virtual void keyPressEvent(QKeyEvent* e) override;

private slots:
  void when_em_charEncoded(QChar plain, QChar cipher);
  void when_em_backspace();

  void on_copyInButton_clicked();
  void on_copyOutButton_clicked();
};
