

/*
//�������ҳ�Ʒ
//����ϵ�п�����Ӧ�ó���
//��ע΢�Ź��ںţ����ҵ���
//���ҿ������������� www.DoYoung.net/YT 
//������ѿ����н�ѧ��Ƶ�����ؼ������ϣ�������������
//�������ݾ��� ����������ҳ www.doyoung.net
*/

/*
���޸���־��
1-201708202309 ������


*/




#include "led.h"

void LED_Init(void){ //LED�ƵĽӿڳ�ʼ��
	GPIO_InitTypeDef  GPIO_InitStructure; 	
    GPIO_InitStructure.GPIO_Pin = IO_LED1OUT | IO_LED2OUT; //ѡ��˿ںţ�0~15��all��                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //ѡ��IO�ӿڹ�����ʽ       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����IO�ӿ��ٶȣ�2/10/50MHz��    
	GPIO_Init(IO_LEDPORT, &GPIO_InitStructure);
	
	GPIO_ResetBits(IO_LEDPORT,IO_LED1OUT|IO_LED2OUT); //LED�ƶ�Ϊ�͵�ƽ��0�� ��ʼΪ�ص�			
}
void LED1(u8 a){ //LED�ƿ��� ����Ϊ0����
	if(a)GPIO_SetBits(IO_LEDPORT,IO_LED1OUT);
	else GPIO_ResetBits(IO_LEDPORT,IO_LED1OUT);
}
void LED2(u8 a){ //LED�ƿ��� ����Ϊ0����
	if(a)GPIO_SetBits(IO_LEDPORT,IO_LED2OUT);
	else GPIO_ResetBits(IO_LEDPORT,IO_LED2OUT);
}

/*********************************************************************************************
 * �������� www.DoYoung.net
 * ���ҵ��� www.DoYoung.net/YT 
*********************************************************************************************/


/*
ѡ��IO�ӿڹ�����ʽ��
GPIO_Mode_AIN ģ������
GPIO_Mode_IN_FLOATING ��������
GPIO_Mode_IPD ��������
GPIO_Mode_IPU ��������
GPIO_Mode_Out_PP �������
GPIO_Mode_Out_OD ��©���
GPIO_Mode_AF_PP �����������
GPIO_Mode_AF_OD ���ÿ�©���
*/
