#pragma once

#include <EnigmaMachine.hpp>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainDialog; }
QT_END_NAMESPACE

class MainDialog : public QDialog
{
  Q_OBJECT

private:
  Ui::MainDialog* ui;
  EnigmaMachine _em;

public:
  MainDialog(QWidget* parent = nullptr);
  ~MainDialog();
};
