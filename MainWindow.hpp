#pragma once

#include "AboutDialog.hpp"
#include "Tutorial.hpp"
#include <EnigmaMachine.hpp>
#include <QMainWindow>
#include <QTranslator>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QTranslator;

class MainWindow : public QMainWindow
{
  Q_OBJECT

private:
  Ui::MainWindow* ui;
  EnigmaMachine _em;
  QTranslator _tsr;
  AboutDialog _about;
  Tutorial _tutorial;

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;

public:
  void retranslate_ui();

protected:
  virtual void keyPressEvent(QKeyEvent* e) override;
  virtual void changeEvent(QEvent* e) override;

private slots:
  void when_em_charEncoded(QChar plain, QChar cipher);
  void when_em_backspace();
  void when_langGroup_triggered(QAction* action);
  void when_actionReset_triggered();

  void on_copyInButton_clicked();
  void on_copyOutButton_clicked();
};
