#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QClipboard>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QToolTip>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , _em(this)
{
  ui->setupUi(this);

  ui->mainDialogLayout->addWidget(&_em);
  connect(
    &_em, &EnigmaMachine::charEncoded, this, &MainWindow::when_em_charEncoded);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void
MainWindow::keyPressEvent(QKeyEvent*)
{
  _em.setFocus();
}

void
MainWindow::when_em_charEncoded(QChar plain, QChar cipher)
{
  ui->inputs->setText(ui->inputs->text() + plain);
  ui->outputs->setText(ui->outputs->text() + cipher);
}

void
MainWindow::on_copyInButton_clicked()
{
  auto clipboard = QApplication::clipboard();
  clipboard->setText(ui->copyInButton->text());
  QToolTip::showText(
    ui->copyInButton->mapToGlobal(ui->copyInButton->rect().bottomLeft()),
    tr("outputs copied to clipboard"));
}

void
MainWindow::on_copyOutButton_clicked()
{
  auto clipboard = QApplication::clipboard();
  clipboard->setText(ui->copyOutButton->text());
  QToolTip::showText(
    ui->copyOutButton->mapToGlobal(ui->copyOutButton->rect().bottomLeft()),
    tr("outputs copied to clipboard"));
}
