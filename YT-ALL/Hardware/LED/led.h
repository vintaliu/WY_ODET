#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define IO_LEDPORT	GPIOB	//定义IO接口
#define IO_LED1OUT	GPIO_Pin_0	//定义IO接口
#define IO_LED2OUT	GPIO_Pin_1	//定义IO接口



void LED_Init(void);//初始化
void LED1(u8 a);
void LED2(u8 a);

		 				    
#endif
