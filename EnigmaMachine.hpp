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
  void inputted(QChar plain, QChar cipher);

private:
  Ui::EnigmaMachine* ui;

  QList<RotorSlot*> _rotorSlots;
  QList<RotorSeparator*> _rotorSeps;
  Rotor _reflector;

public:
  explicit EnigmaMachine(QWidget *parent = nullptr);
  ~EnigmaMachine();

private slots:
  void on_machineIn_textChanged(const QString& arg1);

  void on_newRotorButton_clicked();
};
