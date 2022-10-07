#ifndef __RELAY_H
#define __RELAY_H	 
#include "sys.h"


#define RELAYPORT	GPIOA	//定义IO接口
#define RELAY1	GPIO_Pin_14	//定义IO接口
#define RELAY2	GPIO_Pin_13	//定义IO接口



void RELAY_Init(void);//初始化
void RELAY_Control(u8 c);//继电器控制
		 				    
#endif
