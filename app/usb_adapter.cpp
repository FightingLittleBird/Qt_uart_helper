#include "usb_adapter.h"
#include <QDebug>
#include "USB2XXX_APP.h"
#include "usb_device.h"
#include "usb2spi.h"
#include "usb2uart.h"
#include "sys_public.h"

static int DevHandle[10];

/* 构造函数 */
USB_Adapter::USB_Adapter(QWidget *parent) : QMainWindow(parent)
{

}

/* 配置SPI */
int USB_Adapter::USB_to_SPI_init(unsigned int spi_index, unsigned int spi_mode, unsigned int spi_clk, unsigned char spi_para)
{
    SPI_CONFIG SPI_config;

    SPI_config.Mode = spi_mode;
    SPI_config.ClockSpeedHz = spi_clk;
    SPI_config.CPHA = sys.Is_true(bit_istrue(spi_para, CPHA_BIT));
    SPI_config.CPOL = sys.Is_true(bit_istrue(spi_para, CPOL_BIT));
    SPI_config.LSBFirst = sys.Is_true(bit_istrue(spi_para, LSB_BIT));
    SPI_config.Master = sys.Is_true(bit_istrue(spi_para, MASTER_BIT));
    SPI_config.SelPolarity = sys.Is_true(bit_istrue(spi_para, CS_BIT));

    qDebug("Mode:%d, ClockSpeedHz:%d, CPHA:%d, CPOL:%d, LSBFirst:%d, Master:%d, SelPolarity:%d",
           SPI_config.Mode, SPI_config.ClockSpeedHz, SPI_config.CPHA, SPI_config.CPOL,
           SPI_config.LSBFirst, SPI_config.Master, SPI_config.SelPolarity);

    return SPI_Init(DevHandle[0], SPI1_CS0, &SPI_config);
}

/* SPI写字节数据 */
int USB_Adapter::USB_to_SPI_write_Byte(unsigned char *pWriteData,int DataSize)
{
    return SPI_WriteBytes(DevHandle[0], SPI1_CS0, pWriteData, DataSize);
}

/* SPI读字节数据 */
int USB_Adapter::USB_to_SPI_read_Byte(unsigned char *pReadData,int DataSize)
{
    return SPI_ReadBytes(DevHandle[0], SPI1_CS0, pReadData, DataSize);
}

/* SPI读写字节数据 */
int USB_Adapter::USB_to_SPI_RW_Byte(unsigned char *pWriteData, int Writelen, unsigned char *pReadData, int ReadLen, int TimeOutMs)
{
    return SPI_WriteReadBytes(DevHandle[0], SPI1_CS0, pWriteData, Writelen, pReadData, ReadLen, TimeOutMs);
}

/* 异步发送字节数据 */
int USB_Adapter::USB_to_SPI_Async_WriteBytes(unsigned char *pWriteData, int DataSize)
{
    return SPI_WriteBytesAsync(DevHandle[0], SPI1_CS0, pWriteData, DataSize);
}

/* 从机写字节数据 */
int USB_Adapter::USB_to_SPI_Slave_WriteBytes(unsigned char *pWriteData, int DataSize, int TimeOutMs)
{
    return SPI_SlaveWriteBytes(DevHandle[0], SPI1_CS0, pWriteData, DataSize, TimeOutMs);
}

/* 从机读字节数据 */
int USB_Adapter::USB_to_SPI_Slave_ReadBytes(unsigned char *pReadData, int TimeOutMs)
{
    return SPI_SlaveReadBytes(DevHandle[0], SPI1_CS0, pReadData, TimeOutMs);
}

/* 从机连续读字节 */
int USB_Adapter::USB_to_Uart_Slave_Continue_Read(PSPI_GET_DATA_HANDLE pSlaveReadDataHandle)
{
    return SPI_SlaveContinueRead(DevHandle[0], SPI1_CS0, pSlaveReadDataHandle);
}

/* 从机停止连续读字节 */
int USB_Adapter::USB_to_Uart_Slave_Continue_Read_Stop(void)
{
    return SPI_SlaveContinueReadStop(DevHandle[0], SPI1_CS0);
}

/* SPI块写字节数据 */
int USB_Adapter::USB_to_SPI_Block_Write_Byte(unsigned char *pWriteData, int block_size, int blocknum, int interval_time_us)
{
    return SPI_BlockWriteBytes(DevHandle[0], SPI1_CS0, pWriteData, block_size, blocknum, interval_time_us);
}

/* SPI块读字节数据 */
int USB_Adapter::USB_to_SPI_Block_Read_Byte(unsigned char *pReadData, int block_size, int blocknum, int interval_time_us)
{
    return SPI_BlockReadBytes(DevHandle[0], SPI1_CS0, pReadData, block_size, blocknum, interval_time_us);
}

/* 配置串口 */
int USB_Adapter::USB_to_Uart_init(int baud_rate, int uart_port)
{
    UART_CONFIG UART_config;

    UART_config.BaudRate = baud_rate;
    UART_config.Parity = UART_PARITY_NO;
    UART_config.StopBits = UART_STOP_BITS_1;
    UART_config.WordLength = UART_WORD_LENGTH_8BIT;
    qDebug("端口:%d, 波特率:%d", uart_port, UART_config.BaudRate);

    return UART_Init(DevHandle[0], (unsigned char)uart_port, &UART_config);
}

/* 串口写数据 */
int USB_Adapter::USB_to_Uart_write_Bytes(unsigned char *pWriteData,int DataSize)
{
    return UART_WriteBytes(DevHandle[0], 0, pWriteData, DataSize);
}

/* 串口读数据 */
int USB_Adapter::USB_to_Uart_read_Bytes(unsigned char *pReadData, int TimeOutMs)
{
    return UART_ReadBytes(DevHandle[0], 0, pReadData, TimeOutMs);
}

/* 打印系统信息 */
void USB_Adapter::print_dev_info(PDEVICE_INFO pDev_Info)
{
    qDebug() << "Firmware Name:" << pDev_Info->FirmwareName;
    qDebug() << "Firmware Build Data:" << pDev_Info->BuildDate;
    qDebug("Firmware Version:v%d.%d.%d",
           (pDev_Info->FirmwareVersion>>24)&0xFF,(pDev_Info->FirmwareVersion>>16)&0xFF,pDev_Info->FirmwareVersion&0xFFFF);
    qDebug("Hardware Version:v%d.%d.%d",
           (pDev_Info->HardwareVersion>>24)&0xFF,(pDev_Info->HardwareVersion>>16)&0xFF,pDev_Info->HardwareVersion&0xFFFF);
    qDebug() << "Firmware Functions:" << pDev_Info->Functions;
}

/* 获取固件信息 */
int USB_Adapter::Get_Dev_Info(PDEVICE_INFO pDevInfo)
{
    return DEV_GetDeviceInfo(DevHandle[0], pDevInfo, NULL);
}

/* USB适配器初始化 */
int USB_Adapter::USB_Adapter_Init(PDEVICE_INFO pDevInfo)
{
    /* 扫描设备 */
    if(0 >= USB_ScanDevice(DevHandle))
    {
        qDebug() << "No device connected!";
        return 0;
    }
    qDebug("DevHandle:%d", DevHandle[0]);

    /* 打开设备 */
    if(!USB_OpenDevice(DevHandle[0]))
    {
        qDebug() << "Open device error!";
        return 0;
    }

    return Get_Dev_Info(pDevInfo);
}


