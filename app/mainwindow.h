#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "usb_adapter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_pushButton_clicked();
    void on_pushButton_comopen_clicked();
    void on_pushButton_send_clicked();
    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer;//（用于计时）
    USB_Adapter usb_adapter;

private slots:
    void delay_task();
    void on_pushButton_clear_clicked();
    void on_pushButton_open_uart_clicked();
};

#endif // MAINWINDOW_H
