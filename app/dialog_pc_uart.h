#ifndef DIALOG_PC_UART_H
#define DIALOG_PC_UART_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

typedef enum {
    AF_MODE,
    AF_EN,
    SHUTTER,
} get_camera_info_e;

namespace Ui {
class Dialog_pc_uart;
}

typedef struct{
    QString aperture_info;
    int data;
}aperture_info_s;

class Dialog_pc_uart : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_pc_uart(QWidget *parent = 0);
    ~Dialog_pc_uart();

private slots:
    void on_pushButton_open_uart_all_clicked();
    void on_pushButton_send_clicked();
    void on_pushButton_send_2_clicked();
    void on_pushButton_clear_clicked();
    void on_pushButton_clear_2_clicked();
    void on_pushButton_uart2_3_clear_clicked();
    void on_pushButton_item_clicked();
    void on_pushButton_item1_clicked();
    void on_pushButton_item2_clicked();
    void on_pushButton_item3_clicked();
    void on_pushButton_item4_clicked();
    void on_pushButton_item5_clicked();
    void on_pushButton_item6_clicked();
    void on_pushButton_item7_clicked();
    void on_pushButton_item8_clicked();
    void on_pushButton_item9_clicked();
    void on_pushButton_item10_clicked();
    void on_pushButton_item11_clicked();
    void on_pushButton_item12_clicked();
    void on_pushButton_item13_clicked();
    void on_pushButton_item14_clicked();
    void on_pushButton_item15_clicked();
    void on_pushButton_save_file_clicked();
    void on_pushButton_open_file_clicked();
    void on_pushButton_open_file_2_clicked();
    void on_pushButton_save_file2_clicked();

    void on_pushButton_75_data_clear_clicked();
    void on_pushButton_aper_win_clear_clicked();
    void on_pushButton_item_auto_send_clicked();
    void on_pushButton_C2L_data_clear_clicked();
    void on_pushButton_format_print_clicked();

    void on_checkBox_item_auto_send_stateChanged(int arg1);
    void on_checkBox_hex_stateChanged(int arg1);
    void on_spinBox_read_num_valueChanged(int arg1);
    void on_spinBox_write_num_valueChanged(int arg1);
    void on_textBrowser_textChanged();
    void on_textBrowser_2_textChanged();
    void on_textBrowserC2L_16_data_print_textChanged();
    void on_textBrowserC2L_20_data_print_textChanged();
    void on_radioButton_uart2_clicked(bool checked);
    void on_radioButton_uart_clicked(bool checked);

    void UI_Display_HEX_Data(QString str, unsigned char *pData, int DataSize);
    void uart_read_buf();
    void uart_read_buf_2();
    void Time_out_uart_read_data();
    void Time_out_uart_read_data_2();
    void Time_out_item_auto_send();


    void on_pushButton_aper_clear_clicked();

private:
    Ui::Dialog_pc_uart *ui;
    QSerialPort *mySerialPort;
    QSerialPort *mySerialPort2;
//    QString uart_rev_buf;
//    QString uart_rev_buf_2;
    QByteArray uart_rev_byte_data;
    QByteArray uart_rev_byte_data2;
    QTimer *timer_rev_data;
    QTimer *timer_rev_data_2;
    QTimer *timer_item_auto_send;
    int item_auto_send_index;

    void on_pushButton_send_data_handle(int index);
    void pushButton_send_item_init(bool state);
    void uart_rev__data_parse(int port, QString rev_data);
    void parse_L2C_75_type_data(QString data);
    void parse_C2L_16_type_data(QString data);
    void parse_C2L_20_type_data(QString data);
    void parse_C2L_1d_type_data(QString data);
    QString get_lens_data(int cmd, QString data);
    bool Is_checkBox_item_auto_send(int index);
    QString get_camera_info(int cmd, QString data);
    void ui_display_init(void);
    void open_uart(int port, int state);
    void item_auto_send_handle(int state);
};

#endif // DIALOG_PC_UART_H
