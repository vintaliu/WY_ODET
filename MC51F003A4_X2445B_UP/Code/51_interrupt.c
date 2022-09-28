/**********************************************

�жϷ����Ӻ�����

***********************************************/
#include "uart.h"
#include "User_Def.h"
#include "SystemInit.h"
#include "function.h"



GLOBLE_TIMER_1MS Globle_Timer_1MS;
unsigned short g_us1msTimes = 0;
void Timer0_ISR(void) interrupt T0_IRQn
{
	  SFR_INTERRUPT_BUFF_BIT_T0 = SFRSL0;		//���жϻ���SFR
    SLK_SFR0();
	  TR0 = 0; //�����Ƕ�����д TLx/THx ��ֵ��������ֹͣ(TR0=0;)
	  TL0 = 0x18;
	  TH0 = 0xFC; //��дʱ���ȶ�дL����H
	  TR0 = 1;
    SFRSL0 = SFR_INTERRUPT_BUFF_BIT_T0;		//���жϻ���SFR
	
    Globle_Timer_1MS.All = 0xFFFF;
  //  if(g_us1msTimes)g_us1msTimes--;
  //  if(usTickNum) usTickNum--;
    vDealAd();
		
}

void UART1_ISR(void) interrupt UART1_IRQn
{
    unsigned char Recvdata;
    if (((SCON_1 & DEF_SET_BIT0) == DEF_SET_BIT0))
    {
        Recvdata = UART1_ReceiveByte();
			  CLR_RI_1();
        UART1_RecvData(Recvdata);
    }
}

