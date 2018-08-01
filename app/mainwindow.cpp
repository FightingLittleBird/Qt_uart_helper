#include <QDebug>
#include <QTimer>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialog_pc_uart.h"

#include "usb_adapter.h"
#include "usb_device.h"
#include "usb2spi.h"
#include "usb2uart.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    DEVICE_INFO dev_info;
    ui->setupUi(this);

    qDebug() << "系统开始";

    if(!usb_adapter.USB_Adapter_Init(&dev_info))
    {
        qDebug() << "Usb adpter init error!";
    }
    else
    {
        qDebug() << "Usb adpter init succeed!";
    }

    usb_adapter.print_dev_info(&dev_info);

    /* UI显示固件信息 */
    ui->textEdit_read->append(QString().sprintf("Firmware Name:%s", dev_info.FirmwareName));
    ui->textEdit_read->append(QString().sprintf("Firmware Build Data:%s", dev_info.BuildDate));
    ui->textEdit_read->append(QString().sprintf("Firmware Version:v%d.%d.%d",
                                                (dev_info.FirmwareVersion>>24)&0xFF,
                                                (dev_info.FirmwareVersion>>16)&0xFF,
                                                dev_info.FirmwareVersion&0xFFFF));
    ui->textEdit_read->append(QString().sprintf("Hardware Version:v%d.%d.%d",
                                                (dev_info.HardwareVersion>>24)&0xFF,
                                                (dev_info.HardwareVersion>>16)&0xFF,
                                                dev_info.HardwareVersion&0xFFFF));
    ui->textEdit_read->append(QString().sprintf("Firmware Functions:%d", dev_info.Functions));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(delay_task()));
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* 按钮单击槽 */
void MainWindow::on_pushButton_clicked()                                            /* 获取系统信息 */
{
    DEVICE_INFO dev_info;

    qDebug() << "获取系统信息";
    if(!usb_adapter.Get_Dev_Info(&dev_info))
    {
        qDebug() << "Get device infomation error!";
    }
    else
    {
       usb_adapter.print_dev_info(&dev_info);
       /* UI显示固件信息 */
       ui->textEdit_read->append(QString().sprintf("Firmware Name:%s", dev_info.FirmwareName));
       ui->textEdit_read->append(QString().sprintf("Firmware Build Data:%s", dev_info.BuildDate));
       ui->textEdit_read->append(QString().sprintf("Firmware Version:v%d.%d.%d",
                                                   (dev_info.FirmwareVersion>>24)&0xFF,
                                                   (dev_info.FirmwareVersion>>16)&0xFF,
                                                   dev_info.FirmwareVersion&0xFFFF));
       ui->textEdit_read->append(QString().sprintf("Hardware Version:v%d.%d.%d",
                                                   (dev_info.HardwareVersion>>24)&0xFF,
                                                   (dev_info.HardwareVersion>>16)&0xFF,
                                                   dev_info.HardwareVersion&0xFFFF));
       ui->textEdit_read->append(QString().sprintf("Firmware Functions:%d", dev_info.Functions));
    }
}


void MainWindow::on_pushButton_open_uart_clicked()                                  /* 打开PC UART测试对话框 */
{
    Dialog_pc_uart *dialog_pc_uart = new Dialog_pc_uart(this);
    dialog_pc_uart->show();
}

/* 延时任务 */
void MainWindow::delay_task()
{
    Dialog_pc_uart *dialog_uart = new Dialog_pc_uart(this);                       //延时打开PC串口测试窗口
    dialog_uart->show();
    timer->stop();
}

/* 清除函数 */
void MainWindow::on_pushButton_clear_clicked()
{
    ui->textEdit_read->clear();
}

void MainWindow::on_pushButton_comopen_clicked()
{
}

void MainWindow::on_pushButton_send_clicked()
{
}

void MainWindow::on_pushButton_3_clicked()
{
}

