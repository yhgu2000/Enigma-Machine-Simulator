#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rotorslot.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_pushButton_extendSlots_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_extendSlots_clicked()
{
    auto index = rotorSlots.size();
    auto p = new RotorSlot(ui->groupBox_rotorSlots,"SLOT "+QString::number(index));
    dynamic_cast<QBoxLayout*>(ui->groupBox_rotorSlots->layout())->insertWidget(index,p);
    rotorSlots.append(p);
}

void MainWindow::on_pushButton_curtailSlots_clicked()
{
    auto p = rotorSlots.back();
    rotorSlots.pop_back();
    delete p;
}
