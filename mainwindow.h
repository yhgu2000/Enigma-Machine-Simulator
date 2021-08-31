#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class RotorSlot;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_extendSlots_clicked();

    void on_pushButton_curtailSlots_clicked();

private:
    Ui::MainWindow *ui;
    QVector<RotorSlot*> rotorSlots;
};
#endif // MAINWINDOW_H
