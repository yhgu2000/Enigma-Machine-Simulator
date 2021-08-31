#include "MainDialog.hpp"

#include "ui_MainDialog.h"
#include <QKeyEvent>
#include <QRandomGenerator>

MainDialog::MainDialog(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::MainDialog)
  , _em(this)
{
  ui->setupUi(this);

  ui->mainDialogLayout->addWidget(&_em);
  connect(
    &_em, &EnigmaMachine::charEncoded, this, &MainDialog::when_em_charEncoded);
}

MainDialog::~MainDialog()
{
  delete ui;
}

void
MainDialog::keyPressEvent(QKeyEvent*)
{
  _em.setFocus();
}

void
MainDialog::when_em_charEncoded(QChar plain, QChar cipher)
{
  ui->inputs->setText(ui->inputs->text() + plain);
  ui->outputs->setText(ui->outputs->text() + cipher);
}
