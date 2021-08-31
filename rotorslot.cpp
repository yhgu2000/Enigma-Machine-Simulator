#include "rotorslot.h"
#include "ui_rotorslot.h"
#include <QFileDialog>
#include "palettes.h"
#include "rotor.h"

RotorSlot::RotorSlot(QWidget *parent,const QString& title) :
    QWidget(parent),
    ui(new Ui::RotorSlot),
    statusFlag(kEmpty),
    rotor(nullptr)
{
    ui->setupUi(this);
    ui->label_title->setText(title);
    ui->label_status->setPalette(g_warningPalette);
    connect(ui->dial,SIGNAL(valueChanged(int)),this,SLOT(setPosition(int)));
    connect(ui->spinBox,SIGNAL(valueChanged(int)),this,SLOT(setPosition(int)));
}

RotorSlot::~RotorSlot()
{
    delete ui;
}

void RotorSlot::setPosition(int value)
{
    ui->dial->setValue(value);
    ui->spinBox->setValue(value);
}

void RotorSlot::on_pushButton_clicked()
{
    switch (statusFlag)
    {
    case kEmpty:
        userLoadRotor();
        if(rotor == nullptr)
            statusFlag = kBadRotor;
        else
            statusFlag = kReady;
        break;

    case kBadRotor:
    case kReady:
        statusFlag = kEmpty;
        break;

    }
    updateStatus();
}

void RotorSlot::userLoadRotor()
{
    // 打开对话框，获取用户选择的文件
    auto s = QFileDialog::getOpenFileName(
                this,
                tr("Select Rotor File"),
                "",
                tr("EM Rotor files(*.emr)"));
    if(s.isNull())
        return;  // 取消则返回

    // 导入转子
    rotor = Rotor::load(s);
}

void RotorSlot::updateStatus()
{
    switch (statusFlag)
    {
    case kEmpty:
        ui->dial->setEnabled(false);
        ui->spinBox->setEnabled(false);
        ui->label_status->setText(tr("EMPTY"));
        ui->label_status->setPalette(g_warningPalette);
        ui->pushButton->setText(tr("LOAD"));
        break;

    case kBadRotor:
        ui->dial->setEnabled(false);
        ui->spinBox->setEnabled(false);
        ui->label_status->setText(tr("BAD ROTOR"));
        ui->label_status->setPalette(g_badPalette);
        ui->pushButton->setText(tr("UNLOAD"));
        break;

    case kReady:
        ui->dial->setEnabled(true);
        ui->spinBox->setEnabled(true);
        ui->label_status->setText(tr("READY"));
        ui->label_status->setPalette(g_goodPalette);
        ui->pushButton->setText(tr("UNLOAD"));
    }
}
