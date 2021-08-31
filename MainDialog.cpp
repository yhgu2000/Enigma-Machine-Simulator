#include "MainDialog.hpp"

#include "ui_MainDialog.h"
#include <QRandomGenerator>

MainDialog::MainDialog(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::MainDialog)
  , _em(this)
{
  ui->setupUi(this);
  ui->mainDialogLayout->addWidget(&_em);
}

MainDialog::~MainDialog()
{
  delete ui;
}
