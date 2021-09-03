#include "Tutorial.hpp"
#include "ui_Tutorial.h"
#include <QFile>

Tutorial::Tutorial(QWidget *parent) :
  QStackedWidget(parent),
  ui(new Ui::Tutorial)
{
  ui->setupUi(this);

  QFile f(QStringLiteral(":/tutorial.txt"));
  f.open(f.ReadOnly);
  ui->plainTextEdit->setPlainText(f.readAll());
}

Tutorial::~Tutorial()
{
  delete ui;
}
