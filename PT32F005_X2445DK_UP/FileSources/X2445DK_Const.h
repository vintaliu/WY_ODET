

#ifndef __DKCONST_H_
#define __DKCONST_H_

#include "PT32X005.h"
#include 		"function.h"
#include 		"SystemInit.h"
#include 		"uart.h"


//#define InDeBugMode//��������ڿ����׶Σ�������ڵ��Խ׶�

/////////////////////////////////////////////////

void SpeedLedOn(unsigned char num);//��ʾ�ٶ�ָʾ��
void PowerLedOn(unsigned short power);//��ʾ����ָʾ��
void PowerLedNumOn(unsigned char num);//��ʾ����ָʾ��
void vSpeedMiunsPressed(void);//�ٶȼ�
void vSpeedPlusPressed(void);//�ٶȼ�
void VBreathLED(void);//���Ժ�����
void vBuzzerPressed(void);//������
void vPowerPressed(void);//����Դ���ػ�
void vFirstStart(void);//��һ�ο���
void PrintString1(unsigned char *puts);

void EEPROM_read_n(unsigned short EE_address,unsigned char *DataAddress,unsigned short number);
void EEPROM_write_n(unsigned short EE_address,unsigned char *DataAddress,unsigned short number);
unsigned char ucReadEepromOneData(unsigned int u16_addr);
extern unsigned char ucBattType;

#endif
