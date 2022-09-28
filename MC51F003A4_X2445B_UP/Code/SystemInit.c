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
    ADRIOS1 = 0; //寄存器清0
	
	  ADCCR0 = 0x6C;     //  选择通道6  参考电压为VCC 不使能ADC
    ADCCR1 = 0x81;     //  一个采样时钟  ADC时钟为Fsys 采样结果高4位低八位
    ADRIOS0 = 0x40;    //  使能通道6
    ADRIOS1 = 0x00;
	  ADCCR0 = 0x6D;     //使能ADC
	
	
}
void vSysSourceInit(void)
{
   	clr_EA;      //关闭中断
    GPIO_Init();
	  Time0_Init();    
    PWM1_Init();
    vInitAdc();
    Uart_Init();
	  set_EA;      //打开中断
//	while(1)
//	{
//		g_us1msTimes = 200;
//		while(g_us1msTimes);
//	vSendFeedBack();		
//	}

}

void GPIO_Init(void)
{

	  P0MOD0=0x05;   //P00 P01推挽输出
	  P1MOD0=0x95;   //P10 P11 P12  推挽输出   P13  输入
	  P1MOD1=0x25;   //P14 P15(串口TX) 推挽输出  P16(串口RX) 输入
	  P2MOD0=0xA4;   //P21 P24  推挽输出  P22  P23 输入
	  P2MOD1=0x55;    //P24 P25 P26 P27  推挽输出
	
	  P15D=1;			//串口TX输出高  初始化串口TX必须置高 否则数据发送会异常WH
    POWER_SHOW_0;
    SPEED_LED_0;

}

void PWM1_Init(void)
{
	SLK_SFR1();				  //切换SFR1
	PWM1CR0 = 0x80;     //使能PWM1模块
	//在PWM1 输出中,请勿随意修改频率,否则会影响 PWM1 的输出结果;
	//建议按为先关闭PWM1,修改后再打开PWM1的顺序操作
	PWM1CR1 = 0x23;																					//PWM1CLK=8分频 
	PWM1CR0 = 0x88;                                         //使能通道13
	PWM1PH = 1000 >> 8;
	PWM1PL = 1000 % 256;																		  //周期
	PWM1CR2 = 0x80;
  PWM13DH = 0 >> 8;
	PWM13DL = 0 % 256;                                        //占空比0%
	PWM1CR3 = 0x88;                                           //使能比较功能

	SLK_SFR0();	
}
#define Uart1BaudRate  	    9440
void Uart_Init(void)    //T1M = 1, SMOD = 1	   UART0_Timer1
{
	PCON |= DEF_SET_BIT7; //使能UART1
	SCON_1 &= 0x3f;
	SCON_1 |= 0x50; //方式1  允许接收

	TMOD &= 0xCF;
	TMOD |= DEF_SET_BIT5; //内部时钟  方式2
	TR1 = 0;
	TL1 = (256 - 16000000 / 16 / Uart1BaudRate);
	TH1 = (256 - 16000000 / 16 / Uart1BaudRate);
	TR1 = 1; //打开T1

	IE1 |= DEF_SET_BIT3; //打开UART1中断
	
}

void Time0_Init(void)
{
  TMOD = 0x01;	  //内部时钟  方式1
	TCON = 0x00;
	TCON1 = 0x03; //FT0=FSYS/16	禁止溢出IO翻转
	TR0 = 0;	  //无论是读或者写 TLx/THx 的值，必须先停止(TR0=0;)
	TL0 = 0x18;   //	1ms
	TH0 = 0xFC;   //读写时，先读写L，在H

	IP = 0x02;
	IPH = 0x00; //中断等级1(高)
	T0IE = 1;
	TR0 = 1;
}//启用TIM0定时器中断

