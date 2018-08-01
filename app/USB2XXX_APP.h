#ifndef USB2XXX_APP_H
#define USB2XXX_APP_H

#include "usb_device.h"
#include "usb2spi.h"
#include "usb2uart.h"

int USB_to_Uart_test(void);
int Get_Dev_Info(void);
int USB_to_Uart_write_Bytes(unsigned char *pWriteData,int DataSize);
int USB_to_Uart_init_read(unsigned char *pReadData, int TimeOutMs);

#endif // USB2XXX_APP_H
