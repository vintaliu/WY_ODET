#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
#include "delay.h"

#define IO_KEYPORT	GPIOA	//����IO�ӿ���
#define IO_KEY1IN	GPIO_Pin_0	//����IO�ӿ�
#define IO_KEY2IN	GPIO_Pin_1	//����IO�ӿ�


void KEY_Init(void);//��ʼ��
u8 KEY1 (void);
u8 KEY2 (void);
		 				    
#endif
