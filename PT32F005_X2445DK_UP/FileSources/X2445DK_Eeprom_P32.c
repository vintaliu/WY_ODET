

#include "X2445DK_Const.h"

//========================================================================
// ����: void   ISP_Disable(void)
// ����: ��ֹ����ISP/IAP.
// ����: non.
// ����: non.
// �汾: V1.0, 2012-10-22
//========================================================================
void DisableEEPROM(void)
{

}



/******************** ������������ *****************/
//========================================================================
// ����: void EEPROM_SectorErase(unsigned short EE_address)
// ����: ��ָ����ַ��EEPROM��������.
// ����: EE_address:  Ҫ����������EEPROM�ĵ�ַ.
// ����: non.
// �汾: V1.0, 2013-5-10
//========================================================================
void EEPROM_SectorErase(unsigned short EE_address)
{

}


void EEPROM_WriteByte(unsigned int u16_addr, unsigned char u8_data)
{
    unsigned int uiTemp = 0;
    FLASH_Status FLASHStatus = FLASH_COMPLETE; 	//Flash�����Ľ��
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
// ����: void EEPROM_write_n(unsigned short EE_address,unsigned char *DataAddress,unsigned short number)
// ����: �ѻ����n���ֽ�д��ָ���׵�ַ��EEPROM.
// ����: EE_address:  д��EEPROM���׵�ַ.
//       DataAddress: д��Դ���ݵĻ�����׵�ַ.
//       number:      д����ֽڳ���.
// ����: non.
// �汾: V1.0, 2012-10-22
//========================================================================
void EEPROM_write_n(unsigned short EE_address, unsigned char *DataAddress, unsigned short number)
{
    unsigned int uiTemp = 0;
    unsigned int Address = EE_address;
    FLASH_Status FLASHStatus = FLASH_COMPLETE; 	//Flash�����Ľ��
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
            Address = Address + 4;	//Flashд����������4ByteΪ��λ����
        }
    }
}

//========================================================================
// ����: void EEPROM_read_n(unsigned short EE_address,unsigned char *DataAddress,unsigned short number)
// ����: ��ָ��EEPROM�׵�ַ����n���ֽڷ�ָ���Ļ���.
// ����: EE_address:  ����EEPROM���׵�ַ.
//       DataAddress: �������ݷŻ�����׵�ַ.
//       number:      �������ֽڳ���.
// ����: non.
// �汾: V1.0, 2012-10-22
//========================================================================
void EEPROM_read_n(unsigned short EE_address, unsigned char *DataAddress, unsigned short number)
{
    unsigned int uiTemp = 0;
    unsigned int Address = EE_address;
    for(uiTemp = 0; uiTemp < number; uiTemp++)
    {
        *DataAddress++ = FLASH_ReadWord(Address);
        Address = Address + 4;	//Flashд����������4ByteΪ��λ����
    }
}


