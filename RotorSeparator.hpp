#pragma once

#include <QWidget>

namespace Ui {
class RotorSeparator;
}

class RotorSeparator : public QWidget
{
  Q_OBJECT

public:
  static QChar translate_position(uint8_t pos);

public:
  explicit RotorSeparator(QWidget *parent = nullptr);
  ~RotorSeparator();

public:
  void set_wireL2R(uint8_t pos);
  void set_wireR2L(uint8_t pos);

private:
  Ui::RotorSeparator *ui;
};

