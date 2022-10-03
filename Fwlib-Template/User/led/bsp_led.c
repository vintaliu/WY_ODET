//bsp board support package �弶֧�ְ�
#include "bsp_led.h"



void LED_GPIO_Config(void){

	GPIO_InitTypeDef initStruct;
	
	RCC_APB2PeriphClockCmd(LED_Blue_GPIO_CLK, ENABLE);//����led��Ӧ��GPIO( PB )��ʱ��
	
	initStruct.GPIO_Pin = LED_Blue_GPIO_PIN;
	initStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	initStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(LED_Blue_GPIO_PORT, &initStruct);
	
}
