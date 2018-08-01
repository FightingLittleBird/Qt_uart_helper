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
//ŽÆ
#define SPI_MODE_HARD_FDX       0 //
#define SPI_MODE_HARD_HDX       1 //
#define SPI_MODE_SOFT_HDX       2 //
#define SPI_MODE_SOFT_ONE_WIRE  3 //MOSI
//
#define SPI_MASTER      1 //
#define SPI_SLAVE       0 //
//¦Ë
#define SPI_MSB         0 //¦Ë
#define SPI_LSB         1 //¦Ë
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
    char   CPHA;            //¦Ë:0-SCK1-SCK
    char   LSBFirst;        //¦Ë:0-MSB1-LSB
    char   SelPolarity;     //:0-§µ1-
    unsigned int ClockSpeedHz;    //SPI:¦ËHZ50000000§³3906252
}SPI_CONFIG,*PSPI_CONFIG;

//SPI Flash¨°
typedef struct _SPI_FLASH_CONFIG{
    unsigned char CMD_WriteEnable;          //§Õ
    unsigned char CMD_WriteDisable;         //§Õ
    unsigned char CMD_WritePage;            //§Õ
    unsigned char WritePageAddressBytes;    //§Õ¦Ë
    unsigned char CMD_EraseSector;          //
    unsigned char EraseSectorAddressBytes;  //¦Ë
    unsigned char CMD_EraseBulk;            //
    unsigned char CMD_EraseChip;            //
    unsigned char CMD_ReadID;               //§àID
    unsigned char CMD_ReadData;             //
    unsigned char ReadDataAddressBytes;     //¦Ë
    unsigned char CMD_ReadFast;             //
    unsigned char ReadFastAddressBytes;     //¦Ë
    unsigned char CMD_ReadStatus;           //
    unsigned char CMD_WriteStatus;          //§Õ
    unsigned char ID[16];                   //§àID›¥
    unsigned char ID_Length;                //ID¦Ë
    int PageSize;                           //§³¦Ë
    int NumPages;                           //§à
    int SectorSize;                         //§³¦Ë
}SPI_FLASH_CONFIG,*PSPI_FLASH_CONFIG;

//Žï
#define SPI_SUCCESS             (0)   //§Ô
#define SPI_ERR_NOT_SUPPORT     (-1)  //¨²
#define SPI_ERR_USB_WRITE_FAIL  (-2)  //USB§Õ
#define SPI_ERR_USB_READ_FAIL   (-3)  //USB
#define SPI_ERR_CMD_FAIL        (-4)  //
#define SPI_ERR_PARAMETER       (-5)  //
#ifdef __cplusplus
extern "C"
{
#endif
/**
  * @brief  SPI¨²
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  pConfig SPI
  * @retval §³0§Ô
  */
int WINAPI SPI_Init(int DevHandle,int SPIIndex, PSPI_CONFIG pConfig);

/**
  * @brief  SPI
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  pWriteData 
  * @param  WriteLen 
  * @retval §³0§Ô
  */
int WINAPI SPI_WriteBytes(int DevHandle,int SPIIndex,unsigned char *pWriteData,int WriteLen);

/**
  * @brief  SPI¡Â
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  pWriteData 
  * @param  WriteLen 
  * @retval §³0§Ô
  */
int WINAPI SPI_WriteBytesAsync(int DevHandle,int SPIIndex,unsigned char *pWriteData,int WriteLen);

/**
  * @brief  SPI
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  pReadData 
  * @param  ReadLen 
  * @retval §³0§Ô
  */
int WINAPI SPI_ReadBytes(int DevHandle,int SPIIndex,unsigned char *pReadData,int ReadLen);

/**
  * @brief  SPI¡¤§µ¡¤§µ§¹
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  pWriteData 
  * @param  WriteLen 
  * @param  pReadData 
  * @param  ReadLen 
  * @param  IntervalTime ¦Ë
  * @retval §³0§Ô
  */
int WINAPI SPI_WriteReadBytes(int DevHandle,int SPIIndex,unsigned char *pWriteData,int WriteLen,unsigned char *pReadData,int ReadLen,int IntervalTimeUs);

/**
  * @brief  §¹SPI
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  pWriteData 
  * @param  WriteLen 
  * @param  EventPin GPIO
  * @param  EventType 0x00-0x11-0x10-0x01
  * @param  TimeOutOfMs ¦Ë
  * @retval §³0§Ô
  */
int WINAPI SPI_WriteBytesOfEvent(int DevHandle,int SPIIndex,unsigned char *pWriteData,int WriteLen,int EventPin,unsigned char EventType,int TimeOutOfMs);


/**
  * @brief  §¹SPI
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  pReadData 
  * @param  ReadLen 
  * @param  EventPin GPIO
  * @param  EventType 0x00-0x11-0x10-0x01
  * @param  TimeOutOfMs ¦Ë
  * @retval §³0§Ô
  */
int WINAPI SPI_ReadBytesOfEvent(int DevHandle,int SPIIndex,unsigned char *pReadData,int ReadLen,int EventPin,unsigned char EventType,int TimeOutOfMs);

/**
  * @brief  §¹SPI¡¤§µ¡¤§µ§¹
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  pWriteData 
  * @param  WriteLen 
  * @param  pReadData 
  * @param  ReadLen 
  * @param  IntervalTime ¦Ë
  * @param  EventPin GPIO
  * @param  EventType 0x00-0x11-0x10-0x01
  * @param  TimeOutOfMs ¦Ë
  * @retval §³0§Ô
  */
int WINAPI SPI_WriteReadBytesOfEvent(int DevHandle,int SPIIndex,unsigned char *pWriteData,int WriteLen,unsigned char *pReadData,int ReadLen,int IntervalTimeUs,int EventPin,unsigned char EventType,int TimeOutOfMs);


/**
  * @brief  §Õ¨²bit
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  pWriteBitStr »Ç"100110011"
  * @retval §³0§Ô
  */
int WINAPI SPI_WriteBits(int DevHandle,int SPIIndex,char *pWriteBitStr);

/**
  * @brief  ¨²bit
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  pReadBitStr »Ç"100110011"
  * @param  ReadBitsNum bit
  * @retval §³0§Ô
  */
int WINAPI SPI_ReadBits(int DevHandle,int SPIIndex,char *pReadBitStr,int ReadBitsNum);

/**
  * @brief  §Õ¨²§Õbit
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  pWriteBitStr §Õ»Ç"100110011"
  * @param  pReadBitStr »Ç"100110011"
  * @param  ReadBitsNum bit
  * @retval §³0§Ô
  */
int WINAPI SPI_WriteReadBits(int DevHandle,int SPIIndex,char *pWriteBitStr,char *pReadBitStr,int ReadBitsNum);

/**
  * @brief  SPI§Õ
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  pWriteData 
  * @param  WriteLen 
  * @param  TimeOutMs §³0
  * @retval §³0§Ô
  */
int WINAPI SPI_SlaveWriteBytes(int DevHandle,int SPIIndex,unsigned char *pWriteData,int WriteLen,int TimeOutMs);

/**
  * @brief  SPI
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  pReadData 
  * @param  pReadLen 
  * @retval §³0§Ô
  */
int WINAPI SPI_SlaveReadBytes(int DevHandle,int SPIIndex,unsigned char *pReadData,int TimeOutMs);

/**
  * @brief  SPI,SPI
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  pSlaveReadDataHandle 
  * @retval §³0§Ô
  */
int WINAPI SPI_SlaveContinueRead(int DevHandle,int SPIIndex,PSPI_GET_DATA_HANDLE pSlaveReadDataHandle);

/**
  * @brief  SPI
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  pReadData 
  * @param  BufferSize §³
  * @retval 
  */
int WINAPI SPI_SlaveGetBytes(int DevHandle,int SPIIndex,unsigned char *pReadData,int BufferSize);
/**
  * @brief  SPI
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @retval §³0§Ô
  */
int WINAPI SPI_SlaveContinueReadStop(int DevHandle,int SPIIndex);
/**
  * @brief  §ÕSPI-Flash
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  ClockSpeed SPI¦ËHz,50MHz¦Ã
  * @param  pConfig SPI-Flash
  * @retval §³0§Ô
  */
int WINAPI SPI_FlashInit(int DevHandle,int SPIIndex,int ClockSpeed, PSPI_FLASH_CONFIG pConfig);

/**
  * @brief  SPI-Flash§àID
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  pID §àID›¥SPI_FLASH_CONFIG§ÖID_Length
  * @retval §³0§Ô
  */
int WINAPI SPI_FlashReadID(int DevHandle,int SPIIndex,unsigned char *pID);

/**
  * @brief  SPI-Flash
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  StartSector 
  * @param  NumSector 
  * @retval §³0§Ô
  */
int WINAPI SPI_FlashEraseSector(int DevHandle,int SPIIndex,int StartSector,int NumSector);

/**
  * @brief  SPI-Flash§à
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @retval §³0§Ô
  */
int WINAPI SPI_FlashEraseChip(int DevHandle,int SPIIndex);


/**
  * @brief  SPI-Flash§Õ
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  StartAddr §Õ
  * @param  pWriteData §Õ
  * @param  WriteLen §Õ
  * @retval §³0§Ô
  */
int WINAPI SPI_FlashWrite(int DevHandle,int SPIIndex,int StartAddr,unsigned char *pWriteData,int WriteLen);

/**
  * @brief  SPI-Flash
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  StartAddr §Õ
  * @param  pReadData 
  * @param  ReadLen 
  * @retval §³0§Ô
  */
int WINAPI SPI_FlashRead(int DevHandle,int SPIIndex,int StartAddr,unsigned char *pReadData,int ReadLen);

/**
  * @brief  SPI-Flash
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  StartAddr §Õ
  * @param  pReadData 
  * @param  ReadLen 
  * @retval §³0§Ô
  */
int WINAPI SPI_FlashReadFast(int DevHandle,int SPIIndex,int StartAddr,unsigned char *pReadData,int ReadLen);

/**
  * @brief  Block§Õ
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  pWriteData §Õ
  * @param  BlockSize §Õ
  * @param  BlockNum §Õ
  * @param  IntervalTime §Õ¦Ë
  * @retval §³0§Ô
  */
int WINAPI SPI_BlockWriteBytes(int DevHandle,int SPIIndex,unsigned char *pWriteData,int BlockSize,int BlockNum,int IntervalTimeUs);

/**
  * @brief  Block
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  pReadData 
  * @param  BlockSize ¦Æ
  * @param  BlockNum 
  * @param  IntervalTime §Õ¦Ë
  * @retval §³0§Ô
  */
int WINAPI SPI_BlockReadBytes(int DevHandle,int SPIIndex,unsigned char *pReadData,int BlockSize,int BlockNum,int IntervalTimeUs);

/**
  * @brief  Block§Õ
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  pWriteData §Õ
  * @param  WriteBlockSize ¦Æ
  * @param  pReadData 
  * @param  ReadBlockSize ¦Æ
  * @param  BlockNum 
  * @param  IntervalTime §Õ¦Ë
  * @retval §³0§Ô
  */
int WINAPI SPI_BlockWriteReadBytes(int DevHandle,int SPIIndex,unsigned char *pWriteData,int WriteBlockSize,unsigned char *pReadData,int ReadBlockSize,int BlockNum,int IntervalTimeUs);

/**
  * @brief  Block§Õ
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  pWriteData §Õ
  * @param  BlockSize §Õ
  * @param  BlockNum §Õ
  * @param  EventPin GPIO
  * @param  EventType 0x00-0x11-0x10-0x01
  * @param  TimeOutOfMs ¦Ë
  * @retval §³0§Ô
  */
int WINAPI SPI_BlockWriteBytesOfEvent(int DevHandle,int SPIIndex,unsigned char *pWriteData,int BlockSize,int BlockNum,int EventPin,unsigned char EventType,int TimeOutOfMs);

/**
  * @brief  Block
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  pReadData 
  * @param  BlockSize ¦Æ
  * @param  BlockNum 
  * @param  EventPin GPIO
  * @param  EventType 0x00-0x11-0x10-0x01
  * @param  TimeOutOfMs ¦Ë
  * @retval §³0§Ô
  */
int WINAPI SPI_BlockReadBytesOfEvent(int DevHandle,int SPIIndex,unsigned char *pReadData,int BlockSize,int BlockNum,int EventPin,unsigned char EventType,int TimeOutOfMs);

/**
  * @brief  Block§Õ
  * @param  DevHandle õô
  * @param  SPIIndex SPI01
  * @param  pWriteData §Õ
  * @param  WriteBlockSize ¦Æ
  * @param  pReadData 
  * @param  ReadBlockSize ¦Æ
  * @param  BlockNum 
  * @param  EventPin GPIO
  * @param  EventType 0x00-0x11-0x10-0x01
  * @param  TimeOutOfMs ¦Ë
  * @retval §³0§Ô
  */
int WINAPI SPI_BlockWriteReadBytesOfEvent(int DevHandle,int SPIIndex,unsigned char *pWriteData,int WriteBlockSize,unsigned char *pReadData,int ReadBlockSize,int BlockNum,int EventPin,unsigned char EventType,int TimeOutOfMs);

#ifdef __cplusplus
}
#endif

#endif

