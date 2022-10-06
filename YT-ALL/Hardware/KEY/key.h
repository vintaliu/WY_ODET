#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
#include "delay.h"

#define IO_KEYPORT	GPIOA	//定义IO接口组
#define IO_KEY1IN	GPIO_Pin_0	//定义IO接口
#define IO_KEY2IN	GPIO_Pin_1	//定义IO接口


void KEY_Init(void);//初始化
u8 KEY1 (void);
u8 KEY2 (void);
		 				    
#endif
