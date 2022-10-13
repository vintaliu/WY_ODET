/*
       "LG_X2445F"  Design By LG_qiuzhi
       V1.0   2013.6
       used mcu  STM32F103C8(6)x
*/
//file DriverTimer.H
#ifndef __DIRVERTIME_H_
#define __DIRVERTIME_H_//f1404z



#define TIM1_UsedAsX  0x01
#define TIM4_UsedAsY  0x02

//#define Time1Ccr1Add 0x40012C3C
//#define Time1Ccr3Add 0x40012C34
//#define Time4Ccr1Add 0x4000083C
//#define Time4Ccr3Add 0x40000834//PWM对应的寄存器地址
#define Time1Ccr1Add 0x40012C3C//雅特力 TMR1 CCR1 地址
#define Time1Ccr3Add 0x40012C34//雅特力 TMR1 CCR3 地址
#define Time4Ccr1Add 0x40014034
#define Time4Ccr3Add 0x40000434//PWM对应的寄存器地址  

// #define Moto1AStmPwmReg *(volatile unsigned short *)Moto1APwmAdd
#define Moto1AStmPwmReg *(volatile unsigned short *)uiMotor1LeftHighPwmRegAdd//电机1左驱动PWM高逻辑地址
#define Moto1AStmLowPwmReg *(volatile unsigned short *)(uiMotor1LeftHighPwmRegAdd+4)//电机1左驱动PWM低逻辑地址

#define Moto1BStmPwmReg *(volatile unsigned short *)uiMotor1RightHighPwmRegAdd//电机1右驱动PWM高逻辑地址
#define Moto1BStmLowPwmReg *(volatile unsigned short *)(uiMotor1RightHighPwmRegAdd+4)//电机1右驱动PWM低逻辑地址

#define Moto2AStmPwmReg *(volatile unsigned short *)uiMotor2LeftHighPwmRegAdd//电机2左驱动PWM高逻辑地址
#define Moto2AStmLowPwmReg *(volatile unsigned short *)(uiMotor2LeftHighPwmRegAdd+4)//电机2左驱动PWM低逻辑地址

#define Moto2BStmPwmReg *(volatile unsigned short *)uiMotor2RightHighPwmRegAdd//电机2右驱动PWM高逻辑地址
#define Moto2BStmLowPwmReg *(volatile unsigned short *)(uiMotor2RightHighPwmRegAdd+4)//电机2右驱动PWM低逻辑地址

#define LeftHighMosFetPwmReg  ReDefTIM1CCR1
#define LeftLowMosFetPwmReg   ReDefTIM1CCR3
#define RightHighMosFetPwmReg  ReDefTIM4CCR1
#define RightLowMosFetPwmReg   ReDefTIM4CCR3

#define SetTim1CC1andCCR2(x) {ReDefTIM1CCR1 = x;ReDefTIM1CCR2 = x;}
#define SetTim1CC3andCCR4(x) {ReDefTIM1CCR3 = x;ReDefTIM1CCR4 = x;}
#define SetTim4CC1andCCR2(x) {ReDefTIM4CCR1 = x;ReDefTIM4CCR2 = x;}
#define SetTim4CC3andCCR4(x) {ReDefTIM4CCR3 = x;ReDefTIM4CCR4 = x;}


#define C_MaG1HoPwmReg  ReDefTIM1CCR3//(PA10)
#define C_MaG1LoPwmReg   ReDefTIM1CCR4//(PA11)

#define C_MaG2HoPwmReg  ReDefTIM1CCR1//(PA8)
#define C_MaG2LoPwmReg   ReDefTIM1CCR2//(PA9)

typedef union
{
	unsigned int All;
	struct
	{
		  unsigned int MainTime:1;			
			unsigned int TestSlopTime:1;
		  unsigned int SpeedDowenTime:1;
		  unsigned int :13;		
	}Bits;
} GLOBLE_TIMER_1MS; //?????


extern volatile unsigned char ucTag1ms ;
extern volatile unsigned char ucTag5ms ;
extern volatile unsigned char ucTag10ms ;
extern volatile unsigned char ucTag100ms ;
// extern volatile unsigned char ucTag200ms ;
extern volatile unsigned char ucTag300ms ;
extern volatile unsigned char ucTag400ms ;
// extern volatile unsigned char ucTag500ms ;
extern volatile unsigned int uiAutoPowerOffTimeCont ;
// volatile unsigned char ucTag600ms ;
// volatile unsigned char ucTag700ms ;
// volatile unsigned char ucTag800ms ;
// extern volatile unsigned char ucTag5s ;
// extern volatile unsigned char ucTag1s ;
// extern volatile unsigned char ucTag10s ;
extern volatile unsigned int g_uiTimeNumCont1ms;

extern GLOBLE_TIMER_1MS Globle_Timer_1MS;


#define SysTemClock 72000000 //72000000Hz = 72mHz
#define PwmClock    20000    //20000Hz = 20kHz 

#define TimerBase   3600   //36mHz  原来是1800 - 1  因为百分比除不尽
#define TimeRad     TimerBase/16//16细分等级
#define SetPwmPinHighReg   TimerBase + 1   //设置高电平输出

#define BreakPwmTimBase    100
#define SetBreakPwmRegHigh  BreakPwmTimBase + 1//90bEFORE 

#define CtlSetMotor1LeftPwmPercent(x)    {Moto1AStmPwmReg = x;Moto1AStmLowPwmReg = x;}
#define CtlSetMotor1RightPwmPercent(x)   {Moto1BStmPwmReg = x;Moto1BStmLowPwmReg = x;}//
#define CtlSetMotor2LeftPwmPercent(x)    {Moto2AStmPwmReg = x;Moto2AStmLowPwmReg = x;}//
#define CtlSetMotor2RightPwmPercent(x)   {Moto2BStmPwmReg = x;Moto2BStmLowPwmReg = x;}//

#define SetTim1CC1andCCR2Reg(x,y) {ReDefTIM1CCR1 = x;ReDefTIM1CCR2 = y;}
#define SetTim1CC3andCCR4Reg(x,y) {ReDefTIM1CCR3 = x;ReDefTIM1CCR4 = y;}
#define SetTim4CC1andCCR2Reg(x,y) {ReDefTIM4CCR1 = x;ReDefTIM4CCR2 = y;}
#define SetTim4CC3andCCR4Reg(x,y) {ReDefTIM4CCR3 = x;ReDefTIM4CCR4 = y;}

#endif
