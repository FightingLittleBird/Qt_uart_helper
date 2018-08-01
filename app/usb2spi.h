/**
  ******************************************************************************
  * @file    usb2spi.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   usb2spi.
  ******************************************************************************
  * @attention
  *
  *<center><a href="http:\\www.usbxyz.com">http://www.usbxyz.com</a></center>
  *<center>All Rights Reserved</center></h3>
  *
  ******************************************************************************
  */
#ifndef __USB2SPI_H_
#define __USB2SPI_H_

#include <stdint.h>
#ifndef OS_UNIX
#include <Windows.h>
#else
#include <unistd.h>
#ifndef WINAPI
#define WINAPI
#endif
#endif
//SPI
#define SPI1        (0x00)
#define SPI1_CS0    (0x00)
#define SPI1_CS1    (0x10)
#define SPI1_CS2    (0x20)
#define SPI1_CS3    (0x30)
#define SPI1_CS4    (0x40)

#define SPI2        (0x01)
#define SPI2_CS0    (0x01)
#define SPI2_CS1    (0x11)
#define SPI2_CS2    (0x21)
#define SPI2_CS3    (0x31)
#define SPI2_CS4    (0x41)
//��
#define SPI_MODE_HARD_FDX       0 //
#define SPI_MODE_HARD_HDX       1 //
#define SPI_MODE_SOFT_HDX       2 //
#define SPI_MODE_SOFT_ONE_WIRE  3 //MOSI
//
#define SPI_MASTER      1 //
#define SPI_SLAVE       0 //
//��
#define SPI_MSB         0 //��
#define SPI_LSB         1 //��
//
#define SPI_SEL_LOW     0 //
#define SPI_SEL_HIGH    1 //

//EVENT,EVENTSPI
#define SPI_EVENT_P0    (1<<0)
#define SPI_EVENT_P1    (1<<1)
#define SPI_EVENT_P2    (1<<2)
#define SPI_EVENT_P3    (1<<3)
#define SPI_EVENT_P4    (1<<4)
#define SPI_EVENT_P5    (1<<5)
#define SPI_EVENT_P6    (1<<6)
#define SPI_EVENT_P7    (1<<7)
#define SPI_EVENT_P8    (1<<8)
#define SPI_EVENT_P9    (1<<9)
#define SPI_EVENT_P10    (1<<10)
#define SPI_EVENT_P11    (1<<11)
#define SPI_EVENT_P12    (1<<12)
#define SPI_EVENT_P13    (1<<13)
#define SPI_EVENT_P14    (1<<14)
#define SPI_EVENT_P15    (1<<15)

//
#define EVENT_TYPE_LOW      0x00
#define EVENT_TYPE_HIGH     0x11
#define EVENT_TYPE_RISING   0x01
#define EVENT_TYPE_FALLING  0x10

//
typedef  int (WINAPI *PSPI_GET_DATA_HANDLE)(int DevHandle,int SPIIndex,unsigned char *pData,int DataNum);//

//SPI
typedef struct _SPI_CONFIG{
    char   Mode;            //SPI:0-,1-2-,3-MOSI
    char   Master;          //:0-1-
    char   CPOL;            //:0-SCK1-SCK
    char   CPHA;            //��:0-SCK1-SCK
    char   LSBFirst;        //��:0-MSB1-LSB
    char   SelPolarity;     //:0-��1-
    unsigned int ClockSpeedHz;    //SPI:��HZ50000000��3906252
}SPI_CONFIG,*PSPI_CONFIG;

//SPI Flash��
typedef struct _SPI_FLASH_CONFIG{
    unsigned char CMD_WriteEnable;          //��
    unsigned char CMD_WriteDisable;         //��
    unsigned char CMD_WritePage;            //��
    unsigned char WritePageAddressBytes;    //�զ�
    unsigned char CMD_EraseSector;          //
    unsigned char EraseSectorAddressBytes;  //��
    unsigned char CMD_EraseBulk;            //
    unsigned char CMD_EraseChip;            //
    unsigned char CMD_ReadID;               //��ID
    unsigned char CMD_ReadData;             //
    unsigned char ReadDataAddressBytes;     //��
    unsigned char CMD_ReadFast;             //
    unsigned char ReadFastAddressBytes;     //��
    unsigned char CMD_ReadStatus;           //
    unsigned char CMD_WriteStatus;          //��
    unsigned char ID[16];                   //��ID��
    unsigned char ID_Length;                //ID��
    int PageSize;                           //����
    int NumPages;                           //��
    int SectorSize;                         //����
}SPI_FLASH_CONFIG,*PSPI_FLASH_CONFIG;

//��
#define SPI_SUCCESS             (0)   //��
#define SPI_ERR_NOT_SUPPORT     (-1)  //��
#define SPI_ERR_USB_WRITE_FAIL  (-2)  //USB��
#define SPI_ERR_USB_READ_FAIL   (-3)  //USB
#define SPI_ERR_CMD_FAIL        (-4)  //
#define SPI_ERR_PARAMETER       (-5)  //
#ifdef __cplusplus
extern "C"
{
#endif
/**
  * @brief  SPI��
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  pConfig SPI
  * @retval ��0��
  */
int WINAPI SPI_Init(int DevHandle,int SPIIndex, PSPI_CONFIG pConfig);

/**
  * @brief  SPI
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  pWriteData 
  * @param  WriteLen 
  * @retval ��0��
  */
int WINAPI SPI_WriteBytes(int DevHandle,int SPIIndex,unsigned char *pWriteData,int WriteLen);

/**
  * @brief  SPI��
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  pWriteData 
  * @param  WriteLen 
  * @retval ��0��
  */
int WINAPI SPI_WriteBytesAsync(int DevHandle,int SPIIndex,unsigned char *pWriteData,int WriteLen);

/**
  * @brief  SPI
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  pReadData 
  * @param  ReadLen 
  * @retval ��0��
  */
int WINAPI SPI_ReadBytes(int DevHandle,int SPIIndex,unsigned char *pReadData,int ReadLen);

/**
  * @brief  SPI����������
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  pWriteData 
  * @param  WriteLen 
  * @param  pReadData 
  * @param  ReadLen 
  * @param  IntervalTime ��
  * @retval ��0��
  */
int WINAPI SPI_WriteReadBytes(int DevHandle,int SPIIndex,unsigned char *pWriteData,int WriteLen,unsigned char *pReadData,int ReadLen,int IntervalTimeUs);

/**
  * @brief  ��SPI
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  pWriteData 
  * @param  WriteLen 
  * @param  EventPin GPIO
  * @param  EventType 0x00-0x11-0x10-0x01
  * @param  TimeOutOfMs ��
  * @retval ��0��
  */
int WINAPI SPI_WriteBytesOfEvent(int DevHandle,int SPIIndex,unsigned char *pWriteData,int WriteLen,int EventPin,unsigned char EventType,int TimeOutOfMs);


/**
  * @brief  ��SPI
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  pReadData 
  * @param  ReadLen 
  * @param  EventPin GPIO
  * @param  EventType 0x00-0x11-0x10-0x01
  * @param  TimeOutOfMs ��
  * @retval ��0��
  */
int WINAPI SPI_ReadBytesOfEvent(int DevHandle,int SPIIndex,unsigned char *pReadData,int ReadLen,int EventPin,unsigned char EventType,int TimeOutOfMs);

/**
  * @brief  ��SPI����������
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  pWriteData 
  * @param  WriteLen 
  * @param  pReadData 
  * @param  ReadLen 
  * @param  IntervalTime ��
  * @param  EventPin GPIO
  * @param  EventType 0x00-0x11-0x10-0x01
  * @param  TimeOutOfMs ��
  * @retval ��0��
  */
int WINAPI SPI_WriteReadBytesOfEvent(int DevHandle,int SPIIndex,unsigned char *pWriteData,int WriteLen,unsigned char *pReadData,int ReadLen,int IntervalTimeUs,int EventPin,unsigned char EventType,int TimeOutOfMs);


/**
  * @brief  �ը�bit
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  pWriteBitStr ��"100110011"
  * @retval ��0��
  */
int WINAPI SPI_WriteBits(int DevHandle,int SPIIndex,char *pWriteBitStr);

/**
  * @brief  ��bit
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  pReadBitStr ��"100110011"
  * @param  ReadBitsNum bit
  * @retval ��0��
  */
int WINAPI SPI_ReadBits(int DevHandle,int SPIIndex,char *pReadBitStr,int ReadBitsNum);

/**
  * @brief  �ը���bit
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  pWriteBitStr �ջ�"100110011"
  * @param  pReadBitStr ��"100110011"
  * @param  ReadBitsNum bit
  * @retval ��0��
  */
int WINAPI SPI_WriteReadBits(int DevHandle,int SPIIndex,char *pWriteBitStr,char *pReadBitStr,int ReadBitsNum);

/**
  * @brief  SPI��
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  pWriteData 
  * @param  WriteLen 
  * @param  TimeOutMs ��0
  * @retval ��0��
  */
int WINAPI SPI_SlaveWriteBytes(int DevHandle,int SPIIndex,unsigned char *pWriteData,int WriteLen,int TimeOutMs);

/**
  * @brief  SPI
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  pReadData 
  * @param  pReadLen 
  * @retval ��0��
  */
int WINAPI SPI_SlaveReadBytes(int DevHandle,int SPIIndex,unsigned char *pReadData,int TimeOutMs);

/**
  * @brief  SPI,SPI
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  pSlaveReadDataHandle 
  * @retval ��0��
  */
int WINAPI SPI_SlaveContinueRead(int DevHandle,int SPIIndex,PSPI_GET_DATA_HANDLE pSlaveReadDataHandle);

/**
  * @brief  SPI
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  pReadData 
  * @param  BufferSize ��
  * @retval 
  */
int WINAPI SPI_SlaveGetBytes(int DevHandle,int SPIIndex,unsigned char *pReadData,int BufferSize);
/**
  * @brief  SPI
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @retval ��0��
  */
int WINAPI SPI_SlaveContinueReadStop(int DevHandle,int SPIIndex);
/**
  * @brief  ��SPI-Flash
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  ClockSpeed SPI��Hz,50MHz��
  * @param  pConfig SPI-Flash
  * @retval ��0��
  */
int WINAPI SPI_FlashInit(int DevHandle,int SPIIndex,int ClockSpeed, PSPI_FLASH_CONFIG pConfig);

/**
  * @brief  SPI-Flash��ID
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  pID ��ID��SPI_FLASH_CONFIG��ID_Length
  * @retval ��0��
  */
int WINAPI SPI_FlashReadID(int DevHandle,int SPIIndex,unsigned char *pID);

/**
  * @brief  SPI-Flash
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  StartSector 
  * @param  NumSector 
  * @retval ��0��
  */
int WINAPI SPI_FlashEraseSector(int DevHandle,int SPIIndex,int StartSector,int NumSector);

/**
  * @brief  SPI-Flash��
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @retval ��0��
  */
int WINAPI SPI_FlashEraseChip(int DevHandle,int SPIIndex);


/**
  * @brief  SPI-Flash��
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  StartAddr ��
  * @param  pWriteData ��
  * @param  WriteLen ��
  * @retval ��0��
  */
int WINAPI SPI_FlashWrite(int DevHandle,int SPIIndex,int StartAddr,unsigned char *pWriteData,int WriteLen);

/**
  * @brief  SPI-Flash
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  StartAddr ��
  * @param  pReadData 
  * @param  ReadLen 
  * @retval ��0��
  */
int WINAPI SPI_FlashRead(int DevHandle,int SPIIndex,int StartAddr,unsigned char *pReadData,int ReadLen);

/**
  * @brief  SPI-Flash
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  StartAddr ��
  * @param  pReadData 
  * @param  ReadLen 
  * @retval ��0��
  */
int WINAPI SPI_FlashReadFast(int DevHandle,int SPIIndex,int StartAddr,unsigned char *pReadData,int ReadLen);

/**
  * @brief  Block��
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  pWriteData ��
  * @param  BlockSize ��
  * @param  BlockNum ��
  * @param  IntervalTime �զ�
  * @retval ��0��
  */
int WINAPI SPI_BlockWriteBytes(int DevHandle,int SPIIndex,unsigned char *pWriteData,int BlockSize,int BlockNum,int IntervalTimeUs);

/**
  * @brief  Block
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  pReadData 
  * @param  BlockSize ��
  * @param  BlockNum 
  * @param  IntervalTime �զ�
  * @retval ��0��
  */
int WINAPI SPI_BlockReadBytes(int DevHandle,int SPIIndex,unsigned char *pReadData,int BlockSize,int BlockNum,int IntervalTimeUs);

/**
  * @brief  Block��
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  pWriteData ��
  * @param  WriteBlockSize ��
  * @param  pReadData 
  * @param  ReadBlockSize ��
  * @param  BlockNum 
  * @param  IntervalTime �զ�
  * @retval ��0��
  */
int WINAPI SPI_BlockWriteReadBytes(int DevHandle,int SPIIndex,unsigned char *pWriteData,int WriteBlockSize,unsigned char *pReadData,int ReadBlockSize,int BlockNum,int IntervalTimeUs);

/**
  * @brief  Block��
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  pWriteData ��
  * @param  BlockSize ��
  * @param  BlockNum ��
  * @param  EventPin GPIO
  * @param  EventType 0x00-0x11-0x10-0x01
  * @param  TimeOutOfMs ��
  * @retval ��0��
  */
int WINAPI SPI_BlockWriteBytesOfEvent(int DevHandle,int SPIIndex,unsigned char *pWriteData,int BlockSize,int BlockNum,int EventPin,unsigned char EventType,int TimeOutOfMs);

/**
  * @brief  Block
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  pReadData 
  * @param  BlockSize ��
  * @param  BlockNum 
  * @param  EventPin GPIO
  * @param  EventType 0x00-0x11-0x10-0x01
  * @param  TimeOutOfMs ��
  * @retval ��0��
  */
int WINAPI SPI_BlockReadBytesOfEvent(int DevHandle,int SPIIndex,unsigned char *pReadData,int BlockSize,int BlockNum,int EventPin,unsigned char EventType,int TimeOutOfMs);

/**
  * @brief  Block��
  * @param  DevHandle ��
  * @param  SPIIndex SPI01
  * @param  pWriteData ��
  * @param  WriteBlockSize ��
  * @param  pReadData 
  * @param  ReadBlockSize ��
  * @param  BlockNum 
  * @param  EventPin GPIO
  * @param  EventType 0x00-0x11-0x10-0x01
  * @param  TimeOutOfMs ��
  * @retval ��0��
  */
int WINAPI SPI_BlockWriteReadBytesOfEvent(int DevHandle,int SPIIndex,unsigned char *pWriteData,int WriteBlockSize,unsigned char *pReadData,int ReadBlockSize,int BlockNum,int EventPin,unsigned char EventType,int TimeOutOfMs);

#ifdef __cplusplus
}
#endif

#endif

