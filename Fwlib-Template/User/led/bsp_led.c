//bsp board support package 板级支持包
#include "bsp_led.h"



void LED_GPIO_Config(void){

	GPIO_InitTypeDef initStruct;
	
	RCC_APB2PeriphClockCmd(LED_Blue_GPIO_CLK, ENABLE);//开启led对应的GPIO( PB )的时钟
	
	initStruct.GPIO_Pin = LED_Blue_GPIO_PIN;
	initStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	initStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(LED_Blue_GPIO_PORT, &initStruct);
	
}
