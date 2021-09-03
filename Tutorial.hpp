#pragma once

#include <QStackedWidget>

namespace Ui {
class Tutorial;
}

class Tutorial : public QStackedWidget
{
  Q_OBJECT

    public:
             explicit Tutorial(QWidget *parent = nullptr);
  ~Tutorial();

private:
  Ui::Tutorial *ui;
};

