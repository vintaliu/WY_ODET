

#include "X2445DK_Const.h"

//========================================================================
// 函数: void   ISP_Disable(void)
// 描述: 禁止访问ISP/IAP.
// 参数: non.
// 返回: non.
// 版本: V1.0, 2012-10-22
//========================================================================
void DisableEEPROM(void)
{

}



/******************** 扇区擦除函数 *****************/
//========================================================================
// 函数: void EEPROM_SectorErase(unsigned short EE_address)
// 描述: 把指定地址的EEPROM扇区擦除.
// 参数: EE_address:  要擦除的扇区EEPROM的地址.
// 返回: non.
// 版本: V1.0, 2013-5-10
//========================================================================
void EEPROM_SectorErase(unsigned short EE_address)
{

}


void EEPROM_WriteByte(unsigned int u16_addr, unsigned char u8_data)
{
    unsigned int uiTemp = 0;
    FLASH_Status FLASHStatus = FLASH_COMPLETE; 	//Flash操作的结果
    FLASH_ClearFlag(FLASH_FLAG_WREND | FLASH_FLAG_EREND | FLASH_FLAG_PREREND | FLASH_FLAG_CMDER | FLASH_FLAG_ADER);

    if(FLASHStatus == FLASH_COMPLETE)
    {
        FLASHStatus = FLASH_ErasePage(u16_addr);
    }

    for(uiTemp = 0; uiTemp < 10000; uiTemp++);

    if(FLASHStatus == FLASH_COMPLETE)
    {
        FLASHStatus = FLASH_ProgramWord(u16_addr, u8_data);
    }

}

unsigned char ucReadEepromOneData(unsigned int u16_addr)
{
    return (FLASH_ReadWord(u16_addr));
}

//========================================================================
// 函数: void EEPROM_write_n(unsigned short EE_address,unsigned char *DataAddress,unsigned short number)
// 描述: 把缓冲的n个字节写入指定首地址的EEPROM.
// 参数: EE_address:  写入EEPROM的首地址.
//       DataAddress: 写入源数据的缓冲的首地址.
//       number:      写入的字节长度.
// 返回: non.
// 版本: V1.0, 2012-10-22
//========================================================================
void EEPROM_write_n(unsigned short EE_address, unsigned char *DataAddress, unsigned short number)
{
    unsigned int uiTemp = 0;
    unsigned int Address = EE_address;
    FLASH_Status FLASHStatus = FLASH_COMPLETE; 	//Flash操作的结果
    FLASH_ClearFlag(FLASH_FLAG_WREND | FLASH_FLAG_EREND | FLASH_FLAG_PREREND | FLASH_FLAG_CMDER | FLASH_FLAG_ADER);

    if(FLASHStatus == FLASH_COMPLETE)
    {
        FLASHStatus = FLASH_ErasePage(EE_address);
    }

    for(uiTemp = 0; uiTemp < 10000; uiTemp++);

    if(FLASHStatus == FLASH_COMPLETE)
    {
        for(uiTemp = 0; uiTemp < number; uiTemp++)
        {
            FLASHStatus = FLASH_ProgramWord(Address, *DataAddress++);
            Address = Address + 4;	//Flash写操作，总以4Byte为单位进行
        }
    }
}

//========================================================================
// 函数: void EEPROM_read_n(unsigned short EE_address,unsigned char *DataAddress,unsigned short number)
// 描述: 从指定EEPROM首地址读出n个字节放指定的缓冲.
// 参数: EE_address:  读出EEPROM的首地址.
//       DataAddress: 读出数据放缓冲的首地址.
//       number:      读出的字节长度.
// 返回: non.
// 版本: V1.0, 2012-10-22
//========================================================================
void EEPROM_read_n(unsigned short EE_address, unsigned char *DataAddress, unsigned short number)
{
    unsigned int uiTemp = 0;
    unsigned int Address = EE_address;
    for(uiTemp = 0; uiTemp < number; uiTemp++)
    {
        *DataAddress++ = FLASH_ReadWord(Address);
        Address = Address + 4;	//Flash写操作，总以4Byte为单位进行
    }
}


