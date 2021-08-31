#ifndef ROTORSLOT_H
#define ROTORSLOT_H

#include <QWidget>

class Rotor;

namespace Ui {
class RotorSlot;
}

class RotorSlot : public QWidget
{
    Q_OBJECT

public:
    enum Status
    {
        kEmpty = 0,     // 空槽位
        kBadRotor = 1,  // 转子损坏
        kReady = 2,     // 就绪
        //kRunning = 3,   // 运行中
    };

public:
    RotorSlot(QWidget *parent = nullptr,const QString& title="SLOT");
    ~RotorSlot();

public:
    Status status(){return statusFlag;}
    int getPosition();  // 获得转子位置

public slots:
    void setPosition(int value);  // 设定转子位置

signals:
    void statusChanged(Status statusFlag);  // 转子状态改变信号

private slots:
    void on_pushButton_clicked();

private:
    Ui::RotorSlot *ui;
    Status statusFlag;
    Rotor* rotor;

private:
    void userLoadRotor();  // 打开对话框，让用户载入转子
    void updateStatus();  // 更新状态
};

#endif // ROTORSLOT_H
