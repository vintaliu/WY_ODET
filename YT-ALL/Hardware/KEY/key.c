

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
1-201708202312 ������


*/



#include "key.h"

void KEY_Init(void){ //΢�����صĽӿڳ�ʼ��
	GPIO_InitTypeDef  GPIO_InitStructure; //����GPIO�ĳ�ʼ��ö�ٽṹ	
    GPIO_InitStructure.GPIO_Pin = IO_KEY1IN | IO_KEY2IN; //ѡ��˿ںţ�0~15��all��                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //ѡ��IO�ӿڹ�����ʽ //��������       
	GPIO_Init(IO_KEYPORT,&GPIO_InitStructure);	
}
 
u8 KEY1 (void){
	if(!GPIO_ReadInputDataBit(IO_KEYPORT,IO_KEY1IN)){ //�������ӿڵĵ�ƽ
		delay_ms(20); //��ʱ20msȥ����
		if(!GPIO_ReadInputDataBit(IO_KEYPORT,IO_KEY1IN)){ //�������ӿڵĵ�ƽ
			return 1;
		}
	}
return 0;
}
u8 KEY2 (void){
	if(!GPIO_ReadInputDataBit(IO_KEYPORT,IO_KEY2IN)){ //�������ӿڵĵ�ƽ
		delay_ms(20); //��ʱ20msȥ����
		if(!GPIO_ReadInputDataBit(IO_KEYPORT,IO_KEY2IN)){ //�������ӿڵĵ�ƽ
			return 1;
		}
	}
return 0;
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
