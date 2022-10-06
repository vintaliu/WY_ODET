

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


#include "step_motor.h"

 


void STEP_MOTOR_Init(void){ //LED�ƵĽӿڳ�ʼ��
	GPIO_InitTypeDef  GPIO_InitStructure; 	
    GPIO_InitStructure.GPIO_Pin = STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D; //ѡ��˿�                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //ѡ��IO�ӿڹ�����ʽ       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����IO�ӿ��ٶȣ�2/10/50MHz��    
	GPIO_Init(STEP_MOTOR_PORT, &GPIO_InitStructure);
	
	MOTOLOOP_OFF(); //��ʼ״̬�Ƕϵ�״̬ 			
}

void MOTOLOOP_OFF (void){//����ϵ�
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D);//���ӿ���0
}

void MOTOLOOP_4_S (u8 speed){//����̶�λ��
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A| STEP_MOTOR_C);	//���ӿ���0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_D); //���ӿ���1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D);
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_D);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_C); //1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D);
	delay_ms(speed); //��ʱ
	MOTOLOOP_OFF(); //����ϵ�״̬�����������
}

void MOTOLOOP_4_R (u8 speed){//���˳ʱ�룬4�ģ��ٶȿ죬��С
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);//1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A| STEP_MOTOR_D);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);//1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);//1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_D);//1
	delay_ms(speed); //��ʱ
	MOTOLOOP_OFF(); //����ϵ�״̬�����������
}

void MOTOLOOP_4_L (u8 speed){//�����ʱ�룬4�ģ��ٶȿ죬��С
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);//1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_D);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);//1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);//1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_D);//1
	delay_ms(speed); //��ʱ
	MOTOLOOP_OFF(); //����ϵ�״̬�����������
}


void MOTOLOOP_8_R (u8 speed){//���˳ʱ�룬8�ģ��Ƕ�С���ٶ���������
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A);//1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);//1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_C | STEP_MOTOR_D);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_B);//1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_D);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);//1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_D);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_C);//1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);//1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_D);//1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_D);//1
	delay_ms(speed); //��ʱ
	MOTOLOOP_OFF(); //����ϵ�״̬�����������
}

void MOTOLOOP_8_L (u8 speed){//�����ʱ�룬8�ģ��Ƕ�С���ٶ���������
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_D);//1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);//1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_D);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_C);//1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_D);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);//1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_C | STEP_MOTOR_D);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_B);//1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);//1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A);//1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_D);//1
	delay_ms(speed); //��ʱ
	MOTOLOOP_OFF(); //����ϵ�״̬�����������
}

void MOTOLOOP_4_F (u8 speed){//������ģ�������ֹ�����ֵ���
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);//1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);//1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_D);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);//1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_D);//1
	delay_ms(speed); //��ʱ
	MOTOLOOP_OFF(); //����ϵ�״̬�����������
}

void MOTOLOOP_2_F (u8 speed){//���2�ģ�����ת�������ֵ���
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);//1
	delay_ms(speed); //��ʱ
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);//0
	GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);//1
	delay_ms(speed); //��ʱ
	MOTOLOOP_OFF(); //����ϵ�״̬�����������
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
