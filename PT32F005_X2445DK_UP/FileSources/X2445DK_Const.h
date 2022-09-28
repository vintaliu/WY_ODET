

#ifndef __DKCONST_H_
#define __DKCONST_H_

#include "PT32X005.h"
#include 		"function.h"
#include 		"SystemInit.h"
#include 		"uart.h"


//#define InDeBugMode//如果程序在开发阶段，如果是在调试阶段

/////////////////////////////////////////////////

void SpeedLedOn(unsigned char num);//显示速度指示灯
void PowerLedOn(unsigned short power);//显示电量指示灯
void PowerLedNumOn(unsigned char num);//显示电量指示灯
void vSpeedMiunsPressed(void);//速度减
void vSpeedPlusPressed(void);//速度加
void VBreathLED(void);//测试呼吸灯
void vBuzzerPressed(void);//按喇叭
void vPowerPressed(void);//按电源键关机
void vFirstStart(void);//第一次开机
void PrintString1(unsigned char *puts);

void EEPROM_read_n(unsigned short EE_address,unsigned char *DataAddress,unsigned short number);
void EEPROM_write_n(unsigned short EE_address,unsigned char *DataAddress,unsigned short number);
unsigned char ucReadEepromOneData(unsigned int u16_addr);
extern unsigned char ucBattType;

#endif
