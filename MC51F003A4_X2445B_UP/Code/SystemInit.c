#include "User_Def.h"
#include "intrins.h"
#include "stdio.h"
#include "SystemInit.h"
#include "function.h"
void vInitAdc(void)
{
    ADCCR0 = 0;
    ADCCR1 = 0;
    ADRIOS0 = 0;
    ADRIOS1 = 0; //�Ĵ�����0
	
	  ADCCR0 = 0x6C;     //  ѡ��ͨ��6  �ο���ѹΪVCC ��ʹ��ADC
    ADCCR1 = 0x81;     //  һ������ʱ��  ADCʱ��ΪFsys ���������4λ�Ͱ�λ
    ADRIOS0 = 0x40;    //  ʹ��ͨ��6
    ADRIOS1 = 0x00;
	  ADCCR0 = 0x6D;     //ʹ��ADC
	
	
}
void vSysSourceInit(void)
{
   	clr_EA;      //�ر��ж�
    GPIO_Init();
	  Time0_Init();    
    PWM1_Init();
    vInitAdc();
    Uart_Init();
	  set_EA;      //���ж�
//	while(1)
//	{
//		g_us1msTimes = 200;
//		while(g_us1msTimes);
//	vSendFeedBack();		
//	}

}

void GPIO_Init(void)
{

	  P0MOD0=0x05;   //P00 P01�������
	  P1MOD0=0x95;   //P10 P11 P12  �������   P13  ����
	  P1MOD1=0x25;   //P14 P15(����TX) �������  P16(����RX) ����
	  P2MOD0=0xA4;   //P21 P24  �������  P22  P23 ����
	  P2MOD1=0x55;    //P24 P25 P26 P27  �������
	
	  P15D=1;			//����TX�����  ��ʼ������TX�����ø� �������ݷ��ͻ��쳣WH
    POWER_SHOW_0;
    SPEED_LED_0;

}

void PWM1_Init(void)
{
	SLK_SFR1();				  //�л�SFR1
	PWM1CR0 = 0x80;     //ʹ��PWM1ģ��
	//��PWM1 �����,���������޸�Ƶ��,�����Ӱ�� PWM1 ��������;
	//���鰴Ϊ�ȹر�PWM1,�޸ĺ��ٴ�PWM1��˳�����
	PWM1CR1 = 0x23;																					//PWM1CLK=8��Ƶ 
	PWM1CR0 = 0x88;                                         //ʹ��ͨ��13
	PWM1PH = 1000 >> 8;
	PWM1PL = 1000 % 256;																		  //����
	PWM1CR2 = 0x80;
  PWM13DH = 0 >> 8;
	PWM13DL = 0 % 256;                                        //ռ�ձ�0%
	PWM1CR3 = 0x88;                                           //ʹ�ܱȽϹ���

	SLK_SFR0();	
}
#define Uart1BaudRate  	    9440
void Uart_Init(void)    //T1M = 1, SMOD = 1	   UART0_Timer1
{
	PCON |= DEF_SET_BIT7; //ʹ��UART1
	SCON_1 &= 0x3f;
	SCON_1 |= 0x50; //��ʽ1  �������

	TMOD &= 0xCF;
	TMOD |= DEF_SET_BIT5; //�ڲ�ʱ��  ��ʽ2
	TR1 = 0;
	TL1 = (256 - 16000000 / 16 / Uart1BaudRate);
	TH1 = (256 - 16000000 / 16 / Uart1BaudRate);
	TR1 = 1; //��T1

	IE1 |= DEF_SET_BIT3; //��UART1�ж�
	
}

void Time0_Init(void)
{
  TMOD = 0x01;	  //�ڲ�ʱ��  ��ʽ1
	TCON = 0x00;
	TCON1 = 0x03; //FT0=FSYS/16	��ֹ���IO��ת
	TR0 = 0;	  //�����Ƕ�����д TLx/THx ��ֵ��������ֹͣ(TR0=0;)
	TL0 = 0x18;   //	1ms
	TH0 = 0xFC;   //��дʱ���ȶ�дL����H

	IP = 0x02;
	IPH = 0x00; //�жϵȼ�1(��)
	T0IE = 1;
	TR0 = 1;
}//����TIM0��ʱ���ж�

