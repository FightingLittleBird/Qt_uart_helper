#include "dialog_pc_uart.h"
#include "ui_dialog_pc_uart.h"
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMainWindow>
#include <QTextCodec>
#include <QSaveFile>
#include <QFile>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDataStream>
#include <QTimer>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

static aperture_info_s aperture_info_d[] =                                                      //镜头直径和数据对照表
{
    {"F22", 0x1900},
    {"F20", 0x18AA},
    {"F18", 0x1855},
    {"F16", 0x1800},
    {"F14", 0x17AA},
    {"F13", 0x1755},
    {"F11", 0x1700},
    {"F10", 0x16AA},
    {"F9.0", 0x1655},
    {"F8.0", 0x1600},
    {"F7.1", 0x15AA},
    {"F6.3", 0x1555},
    {"F5.6", 0x1500},
    {"F5.0", 0x14AA},
    {"F4.5", 0x1455},
    {"F4.0", 0x1400},
    {"F3.5", 0x13AA},
    {"F3.2", 0x1355},
    {"F2.8", 0x1300},
    {"F2.5", 0x12AA},
    {"F2.2", 0x1255},
    {"F2.0", 0x1200},
    {"F1.8", 0x11BC},
};
static const int MAX_APERTURE_NUM = (sizeof(aperture_info_d)/sizeof(aperture_info_d[0]));

/* 构造函数 */
Dialog_pc_uart::Dialog_pc_uart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_pc_uart)
{
    ui->setupUi(this);

    ui_display_init();
    pushButton_send_item_init(false);                                                           //条发送按钮失能

    timer_rev_data = new QTimer(this);                                                          //创建接受一组数据定时器
    connect(timer_rev_data, SIGNAL(timeout()), this, SLOT(Time_out_uart_read_data()));

    timer_rev_data_2 = new QTimer(this);                                                          //创建接受一组数据定时器
    connect(timer_rev_data_2, SIGNAL(timeout()), this, SLOT(Time_out_uart_read_data_2()));

    timer_item_auto_send = new QTimer(this);                                                    //创建定时发送数据定时器
    connect(timer_item_auto_send, SIGNAL(timeout()), this, SLOT(Time_out_item_auto_send()));

//    QSqlDatabase database;
//    if (QSqlDatabase::contains("qt_sql_default_connection"))
//    {
//        database = QSqlDatabase::database("qt_sql_default_connection");
//    }
//    else
//    {
//        database = QSqlDatabase::addDatabase("QSQLITE");
//        database.setDatabaseName("MyDataBase.db");
//        database.setUserName("XingYeZhiXia");
//        database.setPassword("123456");
//    }

//    QSqlQuery sql_query;
//    QString create_sql = "create table student (id int primary key, name varchar(30), age int)";
//    sql_query.prepare(create_sql);
//    if(!sql_query.exec())
//    {
//        qDebug() << "Error: Fail to create table." << sql_query.lastError();
//    }
//    else
//    {
//        qDebug() << "Table created!";
//    }

    qDebug() << tr("构建串口对话框完成！");
}

/* 析构函数 */
Dialog_pc_uart::~Dialog_pc_uart()
{
    delete ui;
}

/* 控件更新槽 */
void Dialog_pc_uart::on_checkBox_hex_stateChanged(int arg1)                                     /* CheckBox更新槽 */
{
    qDebug( "%s发送数据", arg1?"十六进制":"字符发送");
    if(arg1) {
        ui->textEdit_write->setText("01 02 03 04");
    } else {
        ui->textEdit_write->setText("?\n");
    }
}
void Dialog_pc_uart::on_textBrowser_textChanged()                                               /* textChanged更新槽 */
{
    ui->textBrowser->moveCursor(QTextCursor::End);
}
void Dialog_pc_uart::on_textBrowserC2L_16_data_print_textChanged()
{
    ui->textBrowserC2L_16_data_print->moveCursor(QTextCursor::End);
}
void Dialog_pc_uart::on_textBrowserC2L_20_data_print_textChanged()
{
    ui->textBrowserC2L_20_data_print->moveCursor(QTextCursor::End);
}
void Dialog_pc_uart::on_textBrowser_2_textChanged()
{
    ui->textBrowser_2->moveCursor(QTextCursor::End);
}
void Dialog_pc_uart::on_spinBox_read_num_valueChanged(int arg1)                                 /* SpinBox更新槽 */
{
    qDebug("读字节数:%d", arg1);
}
void Dialog_pc_uart::on_spinBox_write_num_valueChanged(int arg1)
{
    qDebug("写字节数:%d", arg1);
}
void Dialog_pc_uart::on_checkBox_item_auto_send_stateChanged(int arg1)                          //自动发送条数据
{
    item_auto_send_handle(arg1);
}
void Dialog_pc_uart::on_radioButton_uart_clicked(bool checked)
{
    if(ui->pushButton_open_uart->text() == tr("0")) {
        pushButton_send_item_init(false);
    } else {
        pushButton_send_item_init(true);
    }
}
void Dialog_pc_uart::on_radioButton_uart2_clicked(bool checked)
{
    if(ui->pushButton_open_uart_2->text() == tr("0")) {
        pushButton_send_item_init(false);
    } else {
        pushButton_send_item_init(true);
    }
}

/* PushButton单击槽 */
void Dialog_pc_uart::on_pushButton_open_uart_all_clicked()                                      //打开串口
{
    qDebug() << "多选框状态" << ui->checkBox_uart_checked->isChecked();
    if(ui->pushButton_open_uart_all->text() == tr("打开选择串口")) {
        if( (ui->checkBox_uart_checked->isChecked()) &&                                         //串口1多选框选择并且串口1没有打开时
                (ui->pushButton_open_uart->text() == tr("0")) ) {
            open_uart(1, 1);
        }
        if( (ui->checkBox_uart2_checked->isChecked()) &&
                (ui->pushButton_open_uart_2->text() == tr("0")) ) {
            open_uart(2, 1);
        }

        ui->pushButton_open_uart_all->setText("关闭选择串口");
    }
    else {
        qDebug() << "关闭串口: 串口1状态" << ui->pushButton_open_uart->text();
        if( (ui->checkBox_uart_checked->isChecked()) &&                                       //串口1多选框选择并且串口1没有打开时
                (ui->pushButton_open_uart->text() == tr("1")) ) {
            open_uart(1, 0);
        }
        if( (ui->checkBox_uart2_checked->isChecked()) &&
                (ui->pushButton_open_uart_2->text() == tr("1")) ) {
            open_uart(2, 0);
        }

        ui->pushButton_open_uart_all->setText("打开选择串口");
    }
}
void Dialog_pc_uart::on_pushButton_clear_clicked()                                              //清除串口1数据
{
    ui->textEdit_read->clear();
    ui->textBrowser->clear();
}
void Dialog_pc_uart::on_pushButton_clear_2_clicked()                                            //清除串口2数据
{
    ui->textEdit_read_2->clear();
    ui->textBrowser_2->clear();
}
void Dialog_pc_uart::on_pushButton_uart2_3_clear_clicked()                                      //清除多条窗口下数据
{
    if(ui->checkBox_uart1_item_send->isChecked()) {
        ui->textEdit_read->clear();
        ui->textBrowser->clear();
    }
    if(ui->checkBox_uart2_item_send->isChecked()) {
        ui->textEdit_read_2->clear();
        ui->textBrowser_2->clear();
    }
}
void Dialog_pc_uart::on_pushButton_75_data_clear_clicked()                                      //清除75类型窗口下数据
{
    ui->textBrowser_75_len_head->clear();
    ui->textBrowser_75_len_c1->clear();
    ui->textBrowser_75_len_c2->clear();
    ui->textBrowser_75_len_print->clear();
    ui->textBrowser_75_len_data1->clear();
    ui->textBrowser_75_len_data2->clear();
}
void Dialog_pc_uart::on_pushButton_aper_win_clear_clicked()                                     //清除光圈窗口数据
{
    ui->textBrowser_aper_6_9->clear();
    ui->textBrowser_aper_38_43->clear();
    ui->textBrowser_aper_106_109->clear();
    ui->textBrowser_aper_106_110->clear();
}
void Dialog_pc_uart::on_pushButton_C2L_data_clear_clicked()                                     //清除C2L数据
{
    ui->textBrowserC2L_16_data_print->clear();
    ui->textBrowserC2L_20_data_print->clear();
    ui->textBrowserC2L_1d_data_print_2->clear();
}
void Dialog_pc_uart::on_pushButton_aper_clear_clicked()                                         //清除光圈相关数据
{
    ui->textBrowser_aper_6_9->clear();
    ui->textBrowser_aper_38_43->clear();
    ui->textBrowser_aper_106_109->clear();
    ui->textBrowser_aper_106_110->clear();
}
void Dialog_pc_uart::on_pushButton_format_print_clicked()                                       //清除格式化打印窗口数据
{
   ui->textBrowser_format_print->clear();
}
void Dialog_pc_uart::on_pushButton_save_file_clicked()                                          //保存按钮
{
    QString path = QFileDialog::getSaveFileName(this,
                                                tr("Open File"),
                                                "F:/hm/win_xp/QT/spi_test",
                                                tr("Text Files(*.txt)"));

    ui->lineEdit_sava_file->setText(path);

    if(!path.isEmpty()) {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Write File"),
                                 tr("Cannot open file:\n%1").arg(path));
            return;
        }
        QTextStream out(&file);
        out << ui->textEdit_read->toPlainText();
        file.close();
    } else {
        QMessageBox::warning(this, tr("Path"),
                             tr("You did not select any file."));
    }
}
void Dialog_pc_uart::on_pushButton_open_file_clicked()                                          //打开文件按钮
{
    QString path;
    path = QFileDialog::getOpenFileName(this, tr("Open File"),
                                        "C:\Qt\pro\PC_uart_helper\data\光圈数据.txt",
                                        tr("Text Files(*.txt)"));
    ui->lineEdit_sava_file->setText(path);

    if(!path.isEmpty()) {
        QFile file(path);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Read File"), tr("Cannot open file:\n%1").arg(path));
            return;
        }
        QTextStream in_data(&file);
        ui->textEdit_read->setText(in_data.readAll());
        file.close();
    } else {
        QMessageBox::warning(this, tr("Path"), tr("You did not select any file."));
    }
}
void Dialog_pc_uart::on_pushButton_item_clicked()                                               /* 发送条按钮槽函数 */
{
    on_pushButton_send_data_handle(0);
}
void Dialog_pc_uart::on_pushButton_item1_clicked()
{
    on_pushButton_send_data_handle(1);
}
void Dialog_pc_uart::on_pushButton_item2_clicked()
{
    on_pushButton_send_data_handle(2);
}
void Dialog_pc_uart::on_pushButton_item3_clicked()
{
    on_pushButton_send_data_handle(3);
}
void Dialog_pc_uart::on_pushButton_item4_clicked()
{
    on_pushButton_send_data_handle(4);
}
void Dialog_pc_uart::on_pushButton_item5_clicked()
{
    on_pushButton_send_data_handle(5);
}
void Dialog_pc_uart::on_pushButton_item6_clicked()
{
    on_pushButton_send_data_handle(6);
}
void Dialog_pc_uart::on_pushButton_item7_clicked()
{
    on_pushButton_send_data_handle(7);
}
void Dialog_pc_uart::on_pushButton_item8_clicked()
{
    on_pushButton_send_data_handle(8);
}
void Dialog_pc_uart::on_pushButton_item9_clicked()
{
    on_pushButton_send_data_handle(9);
}
void Dialog_pc_uart::on_pushButton_item10_clicked()
{
    on_pushButton_send_data_handle(10);
}
void Dialog_pc_uart::on_pushButton_item11_clicked()
{
    on_pushButton_send_data_handle(11);
}
void Dialog_pc_uart::on_pushButton_item12_clicked()
{
    on_pushButton_send_data_handle(12);
}
void Dialog_pc_uart::on_pushButton_item13_clicked()
{
    on_pushButton_send_data_handle(13);
}
void Dialog_pc_uart::on_pushButton_item14_clicked()
{
    on_pushButton_send_data_handle(14);
}
void Dialog_pc_uart::on_pushButton_item15_clicked()
{
    on_pushButton_send_data_handle(15);
}
void Dialog_pc_uart::on_pushButton_send_clicked()                                               //发送数据
{
//    unsigned char write_num;
    int i;

//    write_num = ui->spinBox_write_num->value();
    ui->textEdit_read->setTextColor(Qt::black);

    /* 发送十六进制数据 */
    if(ui->checkBox_hex->isChecked()) {
        unsigned char WriteBuffer[128];
        QByteArray hex_data;

        hex_data = QByteArray::fromHex(ui->textEdit_write->toPlainText().toLatin1().data());

//        /* 调试打印 */
//        for(i=0; i<write_num; i++){
//            WriteBuffer[i] = hex_data[i];
//        }
//        UI_Display_HEX_Data("TX(hex):   ", WriteBuffer, write_num);
        mySerialPort->write(hex_data);
    }
    /* 发送文本数据 */
    else {
        QByteArray txt_data;
        unsigned char WriteBuffer[128];

        txt_data = ui->textEdit_write->toPlainText().toLocal8Bit();                                     //返回windows操作系统设置的字符集gb18030的编码

        int bl=txt_data.size();                                                                         //取QByteArray 长度
        qDebug("bl=%d\r\n", bl);
        if(txt_data[bl-1]=='\n')                                                                        //判断最后一个字符是否为换行符“\n”
        {
            qDebug() << "有\n";
            txt_data.resize(bl+1);                                                                      //将QByteArray 长度加1
            txt_data[bl-1]='\r';                                                                        //插入一个回车符“\r”
            txt_data[bl]='\n';
        }

//        /* 调试打印 */
//        for(i=0; i<write_num; i++){
//            WriteBuffer[i] = txt_data[i];
//        }
//        ui->textEdit_read->setTextColor(Qt::black);
//        UI_Display_HEX_Data("TX(hex):   ", WriteBuffer, write_num);

        mySerialPort->write(txt_data);
    }
}
void Dialog_pc_uart::on_pushButton_send_2_clicked()
{
    int i;

    QByteArray txt_data;
    unsigned char WriteBuffer[128];

    ui->textEdit_read_2->setTextColor(Qt::black);
    txt_data = ui->textEdit_write_2->toPlainText().toLocal8Bit();                                     //返回windows操作系统设置的字符集gb18030的编码

    int bl=txt_data.size();                                                                         //取QByteArray 长度
//    qDebug("bl=%d\r\n", bl);
    if(txt_data[bl-1]=='\n')                                                                        //判断最后一个字符是否为换行符“\n”
    {
//        qDebug() << "有\n";
        txt_data.resize(bl+1);                                                                      //将QByteArray 长度加1
        txt_data[bl-1]='\r';                                                                        //插入一个回车符“\r”
        txt_data[bl]='\n';
    }
    mySerialPort2->write(txt_data);
}
void Dialog_pc_uart::on_pushButton_open_file_2_clicked()
{
    QString path;
    QString read_data_str;
    QByteArray read_data_byte;
    QString hex_buf;
    char binByte[100];


    path.append(ui->lineEdit_sava_file_path->text());

    if(!path.isEmpty()) {
        QFile file(path);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Read File"), tr("Cannot open file:\n%1").arg(path));
            return;
        }
//        QTextStream in_data(&file);
        QDataStream in(&file);
        in.setVersion (QDataStream::Qt_5_8);
        in.readRawData(binByte, 100);

//        read_data_str = in_data.readAll();
//        read_data_byte = read_data_str.toUtf8().data();
//        ui->textEdit_open_file->setText(read_data_str);

//        read_data_byte.toHex();
//        UI_Display_HEX_Data("B", (unsigned char *)read_data_str, 10);
        int i;

        /* 调试打印 */
        for(i=0; i<10; i++){
            qDebug() << QString().sprintf("%02X ", binByte[i]);
        }
//        qDebug() << read_data_byte;
        qDebug() << "BIN:" << binByte;

//        UI_Display_HEX_Data("bin:   ", hex_data, 100);

        file.close();
    } else {
        QMessageBox::warning(this, tr("Path"), tr("You did not select any file."));
    }
}
void Dialog_pc_uart::on_pushButton_save_file2_clicked()
{
    QString path;
    path.append(ui->lineEdit_sava_file_path->text());

     if(!path.isEmpty()) {
         QFile file(path);
         if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
             QMessageBox::warning(this, tr("Write File"),
                                        tr("Cannot open file:\n%1").arg(path));
             return;
         }
         QTextStream out(&file);
         out << ui->textEdit_open_file->toPlainText();
         file.close();
     } else {
         QMessageBox::warning(this, tr("Path"),
                              tr("You did not select any file."));
     }
}
void Dialog_pc_uart::on_pushButton_item_auto_send_clicked()
{
   if(ui->pushButton_item_auto_send->text() == tr("开始循环发送")) {

       item_auto_send_handle(1);
   } else {
       item_auto_send_handle(0);
   }
}

/*槽函数*/
void Dialog_pc_uart::uart_read_buf()                                                            //接收数据
{
    QByteArray buf;
//    int read_num;
    int i;

    ui->textEdit_read->setTextColor(Qt::blue);
//    read_num = ui->spinBox_read_num->value();
    buf = mySerialPort->readAll();

    /* 接收十六进制数据 */
    if(ui->checkBox_hex->isChecked()) {
        unsigned char WriteBuffer[2000];
        int rev_num = buf.size();

        /* 调试打印 */
        for(i=0; i<rev_num; i++){
            WriteBuffer[i] = buf[i];
            ui->textEdit_read->insertPlainText(QString().sprintf("%02X ", WriteBuffer[i]));
        }
    }
    /* 接收文本数据 */
    else {
        if(!buf.isEmpty()) {
            uart_rev_byte_data.append(buf);
            buf.clear();
            timer_rev_data->stop();
            timer_rev_data->start(100);
        }
    }
}
void Dialog_pc_uart::uart_read_buf_2()
{
    QByteArray buf;
    int i;

    ui->textEdit_read_2->setTextColor(Qt::blue);
    buf = mySerialPort2->readAll();

    if(!buf.isEmpty()) {
        uart_rev_byte_data2.append(buf);
        buf.clear();
        timer_rev_data_2->stop();
        timer_rev_data_2->start(100);
    }

}
void Dialog_pc_uart::Time_out_uart_read_data()                                                  //串口1间隔10ms无数据接受
{
    QString rev_str;

    rev_str = QString::fromLocal8Bit(uart_rev_byte_data);                                                   //gb18030的编码转换unicode编码

    if((rev_str.startsWith("End of acquisition", Qt::CaseSensitive)) ||
    (rev_str.startsWith("F  分组打印采集数据", Qt::CaseSensitive))) {
        uart_rev__data_parse(1, rev_str);
    }
    ui->textBrowser->textCursor().insertText(rev_str);                                     //显示浏览器插入串口接收文本
    ui->textBrowser->textCursor().insertText("\n_________________________________________________\n");
    uart_rev_byte_data.clear();
    rev_str.clear();
    timer_rev_data->stop();
}
void Dialog_pc_uart::Time_out_uart_read_data_2()                                                //串口2间隔10ms无数据接受
{
    QString rev_str;

    rev_str = QString::fromLocal8Bit(uart_rev_byte_data2);                                                   //gb18030的编码转换unicode编码

    if((rev_str.startsWith("End of acquisition", Qt::CaseSensitive)) ||
    (rev_str.startsWith("F  分组打印采集数据", Qt::CaseSensitive))) {
        uart_rev__data_parse(2, rev_str);
    }
    ui->textBrowser_2->textCursor().insertText(rev_str);                                     //显示浏览器插入串口接收文本
    ui->textBrowser_2->textCursor().insertText("\n_________________________________________________\n");
    uart_rev_byte_data2.clear();
    rev_str.clear();
    timer_rev_data_2->stop();
}
void Dialog_pc_uart::Time_out_item_auto_send()                                                  //循环发送条目
{
    int i;

    for(i=0; i<16; i++) {
        if(Is_checkBox_item_auto_send(item_auto_send_index)) {                                  //校验发送按钮是否被选中
            on_pushButton_send_data_handle(item_auto_send_index);                               //发送数据
            item_auto_send_index ++;
            qDebug() << "自动发送条目:" << item_auto_send_index;
            return;                                                                             //发送一次数据返回
        } else {
            item_auto_send_index ++;                                                            //发送按钮没有选中索引加一再循环
            if(item_auto_send_index >= 16) {
              item_auto_send_index = 0;
            }
        }
    }
    qDebug() << "没有条目选中 i:" << i;
}

/*自定义函数*/
void Dialog_pc_uart::uart_rev__data_parse(int port, QString rev_data)                           //解析分组数据
{
    int i;
    QString filter_data;                                                                            //过滤后的数据
    QString group_data;
    int data_num;

    /* 格式打印 */
    QByteArray byte_arrary_print;
    unsigned char uchar_data[200];
    QString hex_buf;
    int data_size;

    filter_data = rev_data.replace(QRegExp("\\\r\n"), "\n");                                        //采用QRegExp正则表达式替换掉\r
    if(port == 1) {
        ui->textEdit_read->clear();
        filter_data = filter_data.replace(QRegExp("\\\n\n"), "\n");                                     //采用QRegExp正则表达式替换掉空行
    }
    else if(port == 2) {
        ui->textEdit_read_2->clear();

    }

    for(i=1; i<100; i++) {                                                                          //迭代打印每组数据，去掉第一个，迭代最大100次
        group_data = filter_data.section('\n', i, i);   // str == "surname")
        data_num = group_data.length()/3;
//        task_count -=data_num;
//        qDebug() << "剩余数据量:" << task_count;

        group_data = filter_data.section('\n', i, i);
        if(group_data.startsWith("end", Qt::CaseSensitive)) {
            break;
        }

        if(port == 1) {
            //qDebug() << QString().sprintf("第%d组数据:", i) << data;
            ui->textEdit_read->setTextColor(Qt::red);
            ui->textEdit_read->insertPlainText(QString().sprintf("串口1第%d组数据长度%d:\n", i, data_num));
            ui->textEdit_read->setTextColor(Qt::black);
            ui->textEdit_read->insertPlainText(group_data);
            ui->textEdit_read->insertPlainText("\n");

            if(group_data.startsWith("f0 75", Qt::CaseSensitive)) {
                //qDebug() << "parse_75:" << group_data;
                parse_L2C_75_type_data(group_data);
            }
        } else if(port == 2) {
            ui->textEdit_read_2->setTextColor(Qt::blue);
            ui->textEdit_read_2->insertPlainText(QString().sprintf("串口2第%d组数据长度%d:\n", i, data_num));
            ui->textEdit_read_2->setTextColor(Qt::black);
            ui->textEdit_read_2->insertPlainText(group_data);
            ui->textEdit_read_2->insertPlainText("\n");

            if(group_data.startsWith("f0 16", Qt::CaseSensitive)) {
                parse_C2L_16_type_data(group_data);
            }

            if(group_data.startsWith("f0 20", Qt::CaseSensitive)) {
                parse_C2L_20_type_data(group_data);
            }

            if(group_data.startsWith("f0 1d", Qt::CaseSensitive)) {
                parse_C2L_1d_type_data(group_data);
            }
        }
    }
    //    tmp = uart_rev_buf.mid(13);
}
void Dialog_pc_uart::parse_L2C_75_type_data(QString data)
{
    QString str;
    int hex_data;
    int rev_data_num;
    bool ok;
    int i;

    /* 获取spinbox数据 */
    int start_bit = ui->spinBox_75_data_start_bit->value();
    int stop_bit = ui->spinBox_75_data_stop_bit->value();
    int print_start_bit = ui->spinBox_75_data_aper_print_start_bit->value();
    int print_start_bit2 = ui->spinBox_75_data_aper_print_start_bit2->value();
    int print_start_bit3 = ui->spinBox_75_data_aper_print_start_bit3->value();
    int print_start_bit4 = ui->spinBox_75_data_aper_print_start_bit4->value();
    int print_stop_bit = ui->spinBox_75_data_aper_print_stop_bit->value();
    int print_stop_bit2 = ui->spinBox_75_data_aper_print_stop_bit2->value();
    int print_stop_bit3 = ui->spinBox_75_data_aper_print_stop_bit3->value();
    int print_stop_bit4 = ui->spinBox_75_data_aper_print_stop_bit4->value();

    ui->textBrowser_75_len_head->append(data.left(6*3));                                              //头数据
    ui->textBrowser_75_len_data1->append(data.mid(6*3, (44-6)*3));
    ui->textBrowser_75_len_c1->append(data.mid(44*3, (50-44)*3));                                     //第45条数据
    ui->textBrowser_75_len_c2->append(data.mid(50*3, (60-50)*3));
    ui->textBrowser_75_len_data2->append(data.mid(60*3, 54*3));

    ui->textBrowser_75_len_print->append(data.mid(start_bit*3, (stop_bit-start_bit)*3));

    /* 光圈相关 */
//    ui->textBrowser_aper_6_9->clear();
//    ui->textBrowser_aper_38_43->clear();
    ui->textBrowser_aper_6_9->append(data.mid(print_start_bit*3, (print_stop_bit-print_start_bit)*3));
    ui->textBrowser_aper_38_43->append(data.mid(print_start_bit2*3, (print_stop_bit2-print_start_bit2)*3));
    ui->textBrowser_aper_106_109->append(data.mid(print_start_bit3*3, (print_stop_bit3-print_start_bit3)*3));

    ui->lcdNumber->display(get_lens_data(1, data.mid(6*3, 4*3)));                                  //LCD控件显示光圈

    if(get_camera_info(AF_MODE, data.mid(68*3, 3)) == tr("MF")) {
        ui->pushButton_lens_mode->setText("MF");
    } else {
        ui->pushButton_lens_mode->setText("AF");
    }
//    qDebug() << "镜头对焦模式" << get_camera_info(AF_MODE, data.mid(68*3, 3));

}
void Dialog_pc_uart::parse_C2L_16_type_data(QString data)
{
    QString str;
    /* 获取spinbox数据 */
    int start_bit = ui->spinBox_C2L_data_start_bit->value();
    int stop_bit = ui->spinBox_C2L_data_stop_bit->value();

    str = get_camera_info(SHUTTER, data.mid(12*3, (14-12)*3));
    ui->lineEdit_C2L_km->setText(str);
//    if(data.mid(12*3, (13-12)*3) == tr("44")) {

//        ui->lineEdit_C2L_km->setText("快门按下");
//    }
//    else {
//        ui->lineEdit_C2L_km->setText("");
//    }
    ui->textBrowserC2L_16_data_print->append(data.mid(start_bit*3, (stop_bit-start_bit)*3));

}
void Dialog_pc_uart::parse_C2L_20_type_data(QString data)
{
    /* 获取spinbox数据 */
    int start_bit = ui->spinBox_C2L_data_start_bit->value();
    int stop_bit = ui->spinBox_C2L_data_stop_bit->value();

    ui->textBrowserC2L_20_data_print->append(data.mid(start_bit*3, (stop_bit-start_bit)*3));
    ui->lcdNumber_C2L_aper_value->display(get_lens_data(1, data.mid(9*3, (11-9)*3)));                                  //LCD控件显示光圈

}
void Dialog_pc_uart::parse_C2L_1d_type_data(QString data)
{
    /* 获取spinbox数据 */
    int start_bit = ui->spinBox_C2L_data_start_bit->value();
    int stop_bit = ui->spinBox_C2L_data_stop_bit->value();

    ui->textBrowserC2L_1d_data_print_2->append(data.mid(start_bit*3, (stop_bit-start_bit)*3));
}
void Dialog_pc_uart::on_pushButton_send_data_handle(int index)                                  /* 发送条目串口数据 */
{
    QByteArray txt_data;

//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

    switch(index) {
        case 0:
            txt_data = ui->lineEdit_item->text().toLocal8Bit();                     //返回windows操作系统设置的字符集gb18030的编码
            break;
        case 1:
            txt_data = ui->lineEdit_item1->text().toLocal8Bit();
            break;
        case 2:
            txt_data = ui->lineEdit_item2->text().toLocal8Bit();
            break;
        case 3:
            txt_data = ui->lineEdit_item3->text().toLocal8Bit();
            break;
        case 4:
            txt_data = ui->lineEdit_item4->text().toLocal8Bit();
            break;
        case 5:
            txt_data = ui->lineEdit_item5->text().toLocal8Bit();
            break;
        case 6:
            txt_data = ui->lineEdit_item6->text().toLocal8Bit();
            break;
        case 7:
            txt_data = ui->lineEdit_item7->text().toLocal8Bit();
            break;
        case 8:
            txt_data = ui->lineEdit_item8->text().toLocal8Bit();
            break;
        case 9:
            txt_data = ui->lineEdit_item9->text().toLocal8Bit();
            break;
        case 10:
            txt_data = ui->lineEdit_item10->text().toLocal8Bit();
            break;
        case 11:
            txt_data = ui->lineEdit_item11->text().toLocal8Bit();
            break;
        case 12:
            txt_data = ui->lineEdit_item12->text().toLocal8Bit();
            break;
        case 13:
            txt_data = ui->lineEdit_item13->text().toLocal8Bit();
            break;
        case 14:
            txt_data = ui->lineEdit_item14->text().toLocal8Bit();
            break;
        case 15:
            txt_data = ui->lineEdit_item15->text().toLocal8Bit();
            break;
        default:
            break;
    }

    /* 尾部插入\r\n */
    int bl=txt_data.size();
    txt_data.resize(bl+2);
    txt_data[bl]='\r';
    txt_data[bl+1]='\n';

    if(ui->checkBox_uart1_item_send->isChecked()) {
        mySerialPort->write(txt_data);
    }
    if(ui->checkBox_uart2_item_send->isChecked()) {
        mySerialPort2->write(txt_data);
    }
}
void Dialog_pc_uart::pushButton_send_item_init(bool state)                                      /* 发送条按钮初始化 */
{
    ui->pushButton_item->setEnabled(state);
    ui->pushButton_item1->setEnabled(state);
    ui->pushButton_item2->setEnabled(state);
    ui->pushButton_item3->setEnabled(state);
    ui->pushButton_item4->setEnabled(state);
    ui->pushButton_item5->setEnabled(state);
    ui->pushButton_item6->setEnabled(state);
    ui->pushButton_item7->setEnabled(state);
    ui->pushButton_item8->setEnabled(state);
    ui->pushButton_item9->setEnabled(state);
    ui->pushButton_item10->setEnabled(state);
    ui->pushButton_item11->setEnabled(state);
    ui->pushButton_item12->setEnabled(state);
    ui->pushButton_item13->setEnabled(state);
    ui->pushButton_item14->setEnabled(state);
    ui->pushButton_item15->setEnabled(state);
    ui->checkBox_item_auto_send->setEnabled(state);
}
void Dialog_pc_uart::UI_Display_HEX_Data(QString str, unsigned char *pData, int DataSize)       //UI显示HEX数据
{
    QString Ui_Str;
    QString hex_buf;

    int i;
    hex_buf.append(str);

    /* 调试打印 */
    for(i=0; i<DataSize; i++){
        hex_buf.append(QString().sprintf("%02X ", pData[i]));
    }
    qDebug() << hex_buf;

    /* UI显示 */
    ui->textEdit_read->append(str);
    for(i=0; i<DataSize; i++){
        Ui_Str = QString().sprintf("%02X ", pData[i]);
        ui->textEdit_read->insertPlainText(Ui_Str);
    }
}
bool Dialog_pc_uart::Is_checkBox_item_auto_send(int index)                                      //判断选中框是否选中
{
    bool ret;

    switch (index) {
    case 0:
        ret = ui->checkBox->isChecked();
        break;
    case 1:
        ret = ui->checkBox_1->isChecked();
        break;
    case 2:
        ret = ui->checkBox_2->isChecked();
        break;
    case 3:
        ret = ui->checkBox_3->isChecked();
        break;
    case 4:
        ret = ui->checkBox_4->isChecked();
        break;
    case 5:
        ret = ui->checkBox_5->isChecked();
        break;
    case 6:
        ret = ui->checkBox_6->isChecked();
        break;
    case 7:
        ret = ui->checkBox_7->isChecked();
        break;
    case 8:
        ret = ui->checkBox_8->isChecked();
        break;
    case 9:
        ret = ui->checkBox_9->isChecked();
        break;
    case 10:
        ret = ui->checkBox_10->isChecked();
        break;
    case 11:
        ret = ui->checkBox_11->isChecked();
        break;
    case 12:
        ret = ui->checkBox_12->isChecked();
        break;
    case 13:
        ret = ui->checkBox_13->isChecked();
        break;
    case 14:
        ret = ui->checkBox_14->isChecked();
        break;
    case 15:
        ret = ui->checkBox_15->isChecked();
        break;
    default:
        ret = 0;
        break;
    }
    return ret;
}
QString Dialog_pc_uart::get_lens_data(int cmd, QString data)                                    /* 获取镜头数据 */
{
    int aperture_data = 0;
    int i;
    bool ok;

    aperture_data = data.mid(3, 3).toInt(&ok, 16) << 8;
    aperture_data = aperture_data | data.left(3).toInt(&ok, 16);
//    qDebug("光圈数据:%04X", aperture_data);

    for(i=0; i<MAX_APERTURE_NUM; i++)                                                           //查询镜头对照表
    {
        if(aperture_info_d[i].data == aperture_data)
        {
            return aperture_info_d[i].aperture_info;
        }
    }

    return "error";
}
QString Dialog_pc_uart::get_camera_info(int cmd, QString data)                                  /* 获取相机状态 */
{
    bool ok;
    QString ret;
    QString str;

    ret.clear();
    switch (cmd) {
    case AF_MODE:
        if(data.toInt(&ok, 16) == 0x01) {
//            ret = QString::sprintf("AF");
            ret.append("AF");
        }
        if(data.toInt(&ok, 16) == 0x03) {
//            sprintf((char *)out_string, "MF");
            ret.append("MF");
        }
        break;
    case AF_EN:
        if(!data.toInt(&ok, 16)) {
//            sprintf((char *)out_string, "AE_NO");
            ret.append("AE_NO");
        }
        if(data.toInt(&ok, 16) == 0x08) {
//            sprintf((char *)out_string, "AE_YES");
            ret.append("AE_YES");
        }
        break;
    case SHUTTER:
//        qDebug() << "快门状态" << data;
        ret.append("快门状态");
        ret.append(data);
//        if(data.left(6) == tr("33 1b ")) {
//            ret.append("按下快门");
//        }
//        if(data == tr("44 ff ")) {
//            ret.append("松开快门");
//        }
        break;
    default:
        break;
    }
    return ret;
}
void Dialog_pc_uart::ui_display_init()                                                          //构建类时初始化UI
{
     int i;

    /* spinBox */
    ui->spinBox_item_send_time->setRange(1,5000);
    ui->spinBox_item_send_time->setValue(1000);

    ui->spinBox_75_data_start_bit->setRange(0,117);
    ui->spinBox_75_data_start_bit->setValue(0);
    ui->spinBox_75_data_stop_bit->setRange(1,117);
    ui->spinBox_75_data_stop_bit->setValue(117);

    ui->spinBox_75_data_aper_print_start_bit->setRange(0,117);
    ui->spinBox_75_data_aper_print_start_bit->setValue(6);
    ui->spinBox_75_data_aper_print_stop_bit->setRange(1,117);
    ui->spinBox_75_data_aper_print_stop_bit->setValue(10);

    ui->spinBox_75_data_aper_print_start_bit2->setRange(0,117);
    ui->spinBox_75_data_aper_print_start_bit2->setValue(38);
    ui->spinBox_75_data_aper_print_stop_bit2->setRange(1,117);
    ui->spinBox_75_data_aper_print_stop_bit2->setValue(43);

    ui->spinBox_75_data_aper_print_start_bit3->setRange(0,117);
    ui->spinBox_75_data_aper_print_start_bit3->setValue(102);
    ui->spinBox_75_data_aper_print_stop_bit3->setRange(1,117);
    ui->spinBox_75_data_aper_print_stop_bit3->setValue(105);

    ui->spinBox_75_data_aper_print_start_bit4->setRange(0,117);
    ui->spinBox_75_data_aper_print_start_bit4->setValue(6);
    ui->spinBox_75_data_aper_print_stop_bit4->setRange(1,117);
    ui->spinBox_75_data_aper_print_stop_bit4->setValue(9);

    ui->spinBox_C2L_data_start_bit->setRange(0,100);
    ui->spinBox_C2L_data_start_bit->setValue(0);
    ui->spinBox_C2L_data_stop_bit->setRange(1,100);
    ui->spinBox_C2L_data_stop_bit->setValue(32);

    /* comBox */
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())                        /* 遍历端口号 */
    {
        ui->comboBox_portName->addItem(info.portName());
        ui->comboBox_portName_2->addItem(info.portName());
    }
    ui->comboBox_baudRate->setCurrentIndex(3);                                                      /* 设置波特率 */
    ui->comboBox_portName->setCurrentIndex(1);
    ui->comboBox_baudRate_2->setCurrentIndex(3);                                                    /* 设置波特率 */
    ui->comboBox_portName_2->setCurrentIndex(2);

    /* chenkBox */
    ui->checkBox_2->setChecked(true);                                                               //条发第3个选中
    ui->checkBox_uart_checked->setChecked(true);
    ui->checkBox_uart2_checked->setChecked(true);

    /* lineEdit */
    ui->lineEdit_write->setText("FC C0 C0 FF");
    ui->lineEdit_item->setText("A");
    ui->lineEdit_item1->setText("B");
    ui->lineEdit_item2->setText("C 开始采集数据");
    ui->lineEdit_item3->setText("D 打印采集数据");
    ui->lineEdit_item4->setText("E  清除缓存器");
    ui->lineEdit_item5->setText("F  分组打印采集数据");
    ui->lineEdit_item6->setText("H  横排打印解析数据");
    ui->lineEdit_item7->setText("I  横排打印HEX");
    ui->lineEdit_item8->setText("A G8 设置打印原始数据");
    ui->lineEdit_item9->setText("A G1 X0 Y0 Z0 ");
    ui->lineEdit_sava_file_path->setText("C:/Qt/pro/PC_uart_helper/data/1.bin");
    for(i=1; i<44; i++) {
        if(i<10) {
             ui->lineEdit_75_data->insert(QString().sprintf("0"));
             ui->lineEdit_C2L_ruler->insert(QString().sprintf("0"));
             ui->lineEdit_C2L_ruler_2->insert(QString().sprintf("0"));
             ui->lineEdit_C2L_ruler_3->insert(QString().sprintf("0"));
        }
        ui->lineEdit_75_data->insert(QString().sprintf("%d ", i));
        ui->lineEdit_C2L_ruler->insert(QString().sprintf("%d ", i));
        ui->lineEdit_C2L_ruler_2->insert(QString().sprintf("%d ", i));
        ui->lineEdit_C2L_ruler_3->insert(QString().sprintf("%d ", i));
    }

    /* textEdit */
    ui->textEdit_write->setText("?\n");
    ui->textEdit_write_2->setText("?\n");

    /* pushButton */
    ui->pushButton_send->setEnabled(false);                                                         /* 关闭发送按钮的使能 */
    ui->pushButton_send_2->setEnabled(false);                                                       /* 关闭发送按钮的使能 */
    QPalette pal;
    pal.setColor(QPalette::Button,Qt::black);
    ui->pushButton_open_uart->setPalette(pal);
    ui->pushButton_open_uart_2->setPalette(pal);
    ui->pushButton_item_auto_send->setEnabled(false);
    /* lcdNumber */
    ui->lcdNumber->setMode(QLCDNumber::Dec);
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumber->setDigitCount(4);
    ui->lcdNumber->display("F2.0");

    ui->lcdNumber_C2L_aper_value->setMode(QLCDNumber::Dec);
    ui->lcdNumber_C2L_aper_value->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumber_C2L_aper_value->setDigitCount(4);
    ui->lcdNumber_C2L_aper_value->display("F2.0");


//    ui->lcdNumber->setMode(QLCDNumber::Hex);
//    ui->lcdNumber_mode->setSegmentStyle(QLCDNumber::Flat);
//    ui->lcdNumber_mode->setDigitCount(4);
//    ui->lcdNumber_mode->display("1234");

    /* radioButton */
    ui->checkBox_uart1_item_send->setChecked(true);
    ui->checkBox_uart2_item_send->setChecked(true);

    /* label */
    QDateTime time = QDateTime::currentDateTime();                                              //获取系统现在的时间
    QString str = time.toString("MM-dd dddd");                                                  //设置系统时间显示格式
    ui->label_time->setText(str);
}
void Dialog_pc_uart::open_uart(int port, int state)                                             //串口操作
{
    int ret;

    if(port == 1) {
        if(state == 1) {                                                                        //打开串口1
            mySerialPort = new QSerialPort(this);
            mySerialPort->setPortName(ui->comboBox_portName->currentText());
            ret = mySerialPort->open(QIODevice::ReadWrite);
            if(!ret) {
                QMessageBox::warning(this, tr("错误提示"),
                                     tr("串口被占用"));
                return;
            }
            mySerialPort->setBaudRate(ui->comboBox_baudRate->currentText().toInt());
            mySerialPort->setDataBits(QSerialPort::Data8);
            mySerialPort->setParity(QSerialPort::NoParity);
            mySerialPort->setStopBits(QSerialPort::OneStop);
            mySerialPort->setFlowControl(QSerialPort::NoFlowControl);
            connect(mySerialPort, &QSerialPort::readyRead, this, &Dialog_pc_uart::uart_read_buf);

            ui->comboBox_baudRate->setEnabled(false);
            ui->comboBox_portName->setEnabled(false);
            ui->pushButton_send->setEnabled(true);
            pushButton_send_item_init(true);
            QPalette pal;
            pal.setColor(QPalette::Button,Qt::red);
            ui->pushButton_open_uart->setPalette(pal);
            ui->pushButton_open_uart->setText("1");
            ui->pushButton_item_auto_send->setEnabled(true);
        } else {                                                                                //关闭串口1
            if(ui->checkBox_item_auto_send->isChecked()) {                      //如果自动发送数据先关闭后再关串口
                item_auto_send_index = 0;
                timer_item_auto_send->stop();
                ui->checkBox_item_auto_send->setChecked(false);
            }
            mySerialPort->clear();
            mySerialPort->close();
            mySerialPort->deleteLater();

            ui->comboBox_baudRate->setEnabled(true);
            ui->comboBox_portName->setEnabled(true);
            ui->pushButton_send->setEnabled(false);
            pushButton_send_item_init(false);
            QPalette pal;
            pal.setColor(QPalette::Button,Qt::black);
            ui->pushButton_open_uart->setPalette(pal);
            ui->pushButton_open_uart->setText("0");
            ui->pushButton_item_auto_send->setEnabled(false);
        }
    }
    else if(port == 2) {
        if(state == 1) {                                                                        //打开串口2
            mySerialPort2 = new QSerialPort(this);
            mySerialPort2->setPortName(ui->comboBox_portName_2->currentText());
            ret = mySerialPort2->open(QIODevice::ReadWrite);
            if(!ret) {
                QMessageBox::warning(this, tr("错误提示"),
                                           tr("串口被占用"));
                return;
            }
            mySerialPort2->setBaudRate(ui->comboBox_baudRate_2->currentText().toInt());
            mySerialPort2->setDataBits(QSerialPort::Data8);
            mySerialPort2->setParity(QSerialPort::NoParity);
            mySerialPort2->setStopBits(QSerialPort::OneStop);
            mySerialPort2->setFlowControl(QSerialPort::NoFlowControl);

            connect(mySerialPort2, &QSerialPort::readyRead, this, &Dialog_pc_uart::uart_read_buf_2);
            pushButton_send_item_init(true);
            ui->comboBox_baudRate_2->setEnabled(false);
            ui->comboBox_portName_2->setEnabled(false);
            ui->pushButton_send_2->setEnabled(true);
            QPalette pal;
            pal.setColor(QPalette::Button,Qt::red);
            ui->pushButton_open_uart_2->setPalette(pal);
            ui->pushButton_open_uart_2->setText("1");
            ui->pushButton_item_auto_send->setEnabled(true);
        } else {                                                                                //打开串口2
            mySerialPort2->clear();
            mySerialPort2->close();
            mySerialPort2->deleteLater();

            //        if(ui->checkBox_item_auto_send->isChecked()) {                                          //如果自动发送数据先关闭后再关串口
            //            item_auto_send_index = 0;
            //            timer_item_auto_send->stop();
            //            ui->checkBox_item_auto_send->setChecked(false);
            //        }
            pushButton_send_item_init(false);
            ui->comboBox_baudRate_2->setEnabled(true);
            ui->comboBox_portName_2->setEnabled(true);
            ui->pushButton_send_2->setEnabled(false);
            QPalette pal;
            pal.setColor(QPalette::Button,Qt::black);
            ui->pushButton_open_uart_2->setPalette(pal);
            ui->pushButton_open_uart_2->setText("0");
            ui->pushButton_item_auto_send->setEnabled(false);
        }
    }
}
void Dialog_pc_uart::item_auto_send_handle(int state)                                           //条自动循发送处理
{
    int item_send_time;
    item_send_time = ui->spinBox_item_send_time->value();
    if(state) {
        item_auto_send_index = 0;
        timer_item_auto_send->start(item_send_time);                                            //开启定时

        ui->checkBox_item_auto_send->setChecked(true);
        ui->pushButton_item_auto_send->setText("停止循环发送");

    } else {
        item_auto_send_index = 0;
        timer_item_auto_send->stop();

        ui->pushButton_item_auto_send->setText("开始循环发送");
        ui->checkBox_item_auto_send->setChecked(false);
    }
}




