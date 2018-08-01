#ifndef USB_ADAPTER_H
#define USB_ADAPTER_H

#include <QMainWindow>
#include "usb_device.h"
#include "usb2spi.h"
#include "usb2uart.h"
#include "sys_public.h"

class USB_Adapter : public QMainWindow
{
    Q_OBJECT

public:
    explicit USB_Adapter(QWidget *parent = 0);

    int USB_to_SPI_init(unsigned int spi_index, unsigned int spi_mode,
                        unsigned int spi_clk, unsigned char spi_para);                               //SPI初始化
    int USB_to_SPI_write_Byte(unsigned char *pWriteData,int DataSize);
    int USB_to_SPI_read_Byte(unsigned char *pReadData,int DataSize);
    int USB_to_SPI_RW_Byte(unsigned char *pWriteData, int Writelen,
                             unsigned char *pReadData, int ReadLen, int TimeOutMs);
    int USB_to_SPI_Async_WriteBytes(unsigned char *pWriteData,int DataSize);
    int USB_to_SPI_Slave_WriteBytes(unsigned char *pWriteData,int DataSize, int TimeOutMs);
    int USB_to_SPI_Slave_ReadBytes(unsigned char *pReadData, int TimeOutMs);
    int USB_to_SPI_Block_Write_Byte(unsigned char *pWriteData, int block_size, int blocknum, int interval_time_us);
    int USB_to_SPI_Block_Read_Byte(unsigned char *pReadData, int block_size, int blocknum, int interval_time_us);

    int USB_to_Uart_Slave_Continue_Read(PSPI_GET_DATA_HANDLE pSlaveReadDataHandle);
    int USB_to_Uart_Slave_Continue_Read_Stop(void);
    int USB_to_Uart_init(int baud_rate, int uart_port);                                             //串口初始化
    int USB_to_Uart_write_Bytes(unsigned char *pWriteData,int DataSize);
    int USB_to_Uart_read_Bytes(unsigned char *pReadData, int TimeOutMs);

    int Get_Dev_Info(PDEVICE_INFO pDevInfo);                                                         //适配器初始化
    void print_dev_info(PDEVICE_INFO pDev_Info);
    int USB_Adapter_Init(PDEVICE_INFO pDevInfo);

    sys_public sys;

private:


signals:

public slots:
};

#endif // USB_ADAPTER_H
