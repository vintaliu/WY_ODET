#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define IO_LEDPORT	GPIOB	//����IO�ӿ�
#define IO_LED1OUT	GPIO_Pin_0	//����IO�ӿ�
#define IO_LED2OUT	GPIO_Pin_1	//����IO�ӿ�



void LED_Init(void);//��ʼ��
void LED1(u8 a);
void LED2(u8 a);

		 				    
#endif
