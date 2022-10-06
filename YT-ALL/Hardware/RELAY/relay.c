

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





ע�⣺��������ռ�õ�GPIO�ӿ�PA13��PA14�ϵ��ΪJTAG���ܣ���Ҫ��RCC����������AFIOʱ�ӣ�
	����RELAY_Init��������룺
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);// �ı�ָ���ܽŵ�ӳ��,��ȫ����JTAG+SW-DP
	���ܽ�JATG�ӿ��ض���ΪGPIO

*/




#include "relay.h"

void RELAY_Init(void){ //�̵����Ľӿڳ�ʼ��
	GPIO_InitTypeDef  GPIO_InitStructure; 	
    GPIO_InitStructure.GPIO_Pin = RELAY1 | RELAY2; //ѡ��˿ںţ�0~15��all��                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //ѡ��IO�ӿڹ�����ʽ       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����IO�ӿ��ٶȣ�2/10/50MHz��    
	GPIO_Init(RELAYPORT, &GPIO_InitStructure);
	//���뽫����JTAG���ܲ�����GPIOʹ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);// �ı�ָ���ܽŵ�ӳ��,��ȫ����JTAG+SW-DP
	GPIO_ResetBits(RELAYPORT,RELAY1 | RELAY2); //��Ϊ�͵�ƽ��0�� ��ʼΪ�ؼ̵���							
}

void RELAY_Control(u8 c){ //�̵����Ŀ��Ƴ���c=0��2���̵�����c=1���̵���1��c=2���̵���2,c=3��2���̵�����
	GPIO_WriteBit(RELAYPORT,RELAY1,(BitAction)(c&0x01));//ͨ������ֵд��ӿ�
	GPIO_WriteBit(RELAYPORT,RELAY2,(BitAction)(c&0x02));//ͨ������ֵд��ӿ�
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
