/*
******************************************************************************
* @file     : USB2XXX_APP.cpp
* @Copyright: usbxyz
* @Revision :
* @Date     : 2018/03/05
* @brief    : USB2XXX test demo
* @attention hm

******************************************************************************
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <QDebug>

#include "usb_device.h"
#include "usb2spi.h"
#include "usb2uart.h"

void USB_to_Uart_test(void)
{
    DEVICE_INFO Dev_Info;
    UART_CONFIG UART_config;
    int DevHandle[2];
    int UARTIndex = 0;
    bool state;
    int ret;
    unsigned char WriteBuffer[256];
    unsigned char ReadBuffer[20480];

    qDebug() << "USB to Uart";
    /*  */
}
