/******************************************************************************
;  *   	@MCU				 : MC51F003A4
;  *   	@Create Date         : 2020.03.23
;  *   	@Author/Corporation  : Martin/SinoMCU
;  *   	@晟矽微技术支持QQ群   : 814641858
;  *    @晟矽微官方论坛       : http://bbs.sinomcu.com/
;  *   	@Copyright           : 2020 SINOMCU Corporation,All Rights Reserved.
;  *----------------------Abstract Description---------------------------------
;  *			                                        		
******************************************************************************/
#ifndef User_Def_H__
#define User_Def_H__

#include "MC51F003A4_COMMON.h"

#define FSYS_CLOCK	16000000


// //位定义操作方法一:
// //bdata(片内RAM位寻址区)操作
// __O uint8_t bdata Flag_Buff1;		
// sbit Flag_Test=Flag_Buff1^0;

////位定义操作方法二:
//bit	F_xxx;


//位定义操作方法三:
//联合体位域操作
typedef union
{
   	unsigned char byte;
   	struct
   	{
   	   	uint8_t bit0 : 1;
   	   	uint8_t bit1 : 1;
   	   	uint8_t bit2 : 1;
   	   	uint8_t bit3 : 1;
   	   	uint8_t bit4 : 1;
   	   	uint8_t bit5 : 1;
   	   	uint8_t bit6 : 1;
   	   	uint8_t bit7 : 1;
   	} bits;
}Flag; 	
extern __O Flag SFR_interrupt_buff_H;
extern __O Flag SFR_interrupt_buff_L;

#define FLAG_TIMER_100MS	 Flag1.bits.bit0

#define SFR_INTERRUPT_BUFF_BIT_INT0			SFR_interrupt_buff_H.bits.bit0	
#define SFR_INTERRUPT_BUFF_BIT_T0			SFR_interrupt_buff_H.bits.bit1
#define SFR_INTERRUPT_BUFF_BIT_INT1			SFR_interrupt_buff_H.bits.bit2
#define SFR_INTERRUPT_BUFF_BIT_T1			SFR_interrupt_buff_H.bits.bit3
#define SFR_INTERRUPT_BUFF_BIT_UART0		SFR_interrupt_buff_H.bits.bit4
#define SFR_INTERRUPT_BUFF_BIT_SPI			SFR_interrupt_buff_H.bits.bit5
#define SFR_INTERRUPT_BUFF_BIT_ADC			SFR_interrupt_buff_H.bits.bit6
#define SFR_INTERRUPT_BUFF_BIT_T2			SFR_interrupt_buff_H.bits.bit7
#define SFR_INTERRUPT_BUFF_BIT_T3			SFR_interrupt_buff_L.bits.bit0
#define SFR_INTERRUPT_BUFF_BIT_PWM1			SFR_interrupt_buff_L.bits.bit1	
#define SFR_INTERRUPT_BUFF_BIT_UART1		SFR_interrupt_buff_L.bits.bit2
#define SFR_INTERRUPT_BUFF_BIT_PWM0			SFR_interrupt_buff_L.bits.bit3
#define SFR_INTERRUPT_BUFF_BIT_PWM1_FB		SFR_interrupt_buff_L.bits.bit4
#define SFR_INTERRUPT_BUFF_BIT_CRC			SFR_interrupt_buff_L.bits.bit5

#endif

/**************************** end of file *********************************************/
