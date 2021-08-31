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

private:
  Ui::EnigmaMachine* ui;

  QList<RotorSlot*> _rotorSlots;
  QList<RotorSeparator*> _rotorSeps;
  Rotor _reflector;

public:
  explicit EnigmaMachine(QWidget *parent = nullptr);
  ~EnigmaMachine();

public:
  void setFocus();

private slots:
  void on_machineIn_textEdited(const QString& arg1);
  void on_newRotorButton_clicked();
};
