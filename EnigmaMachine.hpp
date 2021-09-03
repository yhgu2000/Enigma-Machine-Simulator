#pragma once

#include <QWidget>
#include <Rotor.hpp>

namespace Ui {
class EnigmaMachine;
}

class RotorSlot;
class RotorSeparator;

class EnigmaMachine : public QWidget
{
  Q_OBJECT

signals:
  void charEncoded(QChar plain, QChar cipher);
  void backspace();

private:
  Ui::EnigmaMachine* ui;

  QList<RotorSlot*> _rotorSlots;
  QList<RotorSeparator*> _rotorSeps;
  Rotor _reflector;

public:
  explicit EnigmaMachine(QWidget *parent = nullptr);
  ~EnigmaMachine() override;

public:
  void step();
  void step_back();

public:
  void set_focus();
  void retranslate_ui();

private slots:
  void on_machineIn_textEdited(const QString& arg1);
  void on_newRotorButton_clicked();
  void on_editReflectorButton_clicked();
  void on_trashButton_clicked();
  void on_loadButton_clicked();
  void on_saveButton_clicked();
};
