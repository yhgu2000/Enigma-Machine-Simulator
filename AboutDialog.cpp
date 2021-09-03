#include "AboutDialog.hpp"
#include "ui_AboutDialog.h"
#include <QFile>

AboutDialog::AboutDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::AboutDialog)
{
  ui->setupUi(this);

  QFile f(QStringLiteral(":/about.html"));
  f.open(f.ReadOnly);
  ui->textEdit->setText(f.readAll());
}

AboutDialog::~AboutDialog()
{
  delete ui;
}
