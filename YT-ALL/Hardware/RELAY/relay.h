#ifndef __RELAY_H
#define __RELAY_H	 
#include "sys.h"


#define RELAYPORT	GPIOA	//����IO�ӿ�
#define RELAY1	GPIO_Pin_14	//����IO�ӿ�
#define RELAY2	GPIO_Pin_13	//����IO�ӿ�



void RELAY_Init(void);//��ʼ��
void RELAY_Control(u8 c);//�̵�������
		 				    
#endif
