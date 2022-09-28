/******************************************************************************
;  *   	@MCU				 : MC51F003A4
;  *   	@Create Date         : 2020.01.02
;  *   	@Author/Corporation  : Martin/SinoMCU
;  *   	@晟矽微技术支持QQ群   : 814641858
;  *    @晟矽微官方论坛       : http://bbs.sinomcu.com/
;  *   	@Copyright           : 2020 SINOMCU Corporation,All Rights Reserved.
;  *----------------------Abstract Description---------------------------------
;  *			          定义                              		
******************************************************************************/

#ifndef MC51F003A4_COMMON_H__
#define MC51F003A4_COMMON_H__

#include "MC51F003A4.h"
#include <intrins.h> 

typedef enum {RESET = 0, SET = !RESET} flagstatus, ITstatus;

#define __I 	volatile const /* defines 'read only' permissions */
#define __O 	volatile /* defines 'write only' permissions */
#define __IO 	volatile /* defines 'read / write' permissions */
    /* exact-width signed integer types */
typedef   signed	char			int8_t;
typedef   signed	int				int16_t;
typedef   signed	long 			int32_t;
    /* exact-width unsigned integer types */
typedef unsigned	char 			uint8_t;
typedef unsigned	int 			uint16_t;
typedef unsigned	long			uint32_t;
typedef unsigned	char			u8;
typedef unsigned	int 			u16;
typedef unsigned	long			u32;
typedef unsigned	char			uchar;
typedef unsigned	int 			uint;

#define DEF_SET_BIT0        		0x01
#define DEF_SET_BIT1        		0x02
#define DEF_SET_BIT2        		0x04
#define DEF_SET_BIT3        		0x08
#define DEF_SET_BIT4        		0x10
#define DEF_SET_BIT5        		0x20
#define DEF_SET_BIT6        		0x40
#define DEF_SET_BIT7        		0x80

#define DEF_CLR_BIT0        		0xFE
#define DEF_CLR_BIT1        		0xFD
#define DEF_CLR_BIT2        		0xFB
#define DEF_CLR_BIT3        		0xF7
#define DEF_CLR_BIT4        		0xEF
#define DEF_CLR_BIT5        		0xDF
#define DEF_CLR_BIT6        		0xBF
#define DEF_CLR_BIT7        		0x7F

#define ENABLE	           		1
#define DISABLE            		0

#define FAIL	           		1
#define PASS            		0

#define HIGH	            	1
#define LOW	            		0

#define ON	            		1
#define OFF	            		0

#define TRUE	            	1
#define FALSE	            	0

#define ERROR()					for(;;)					// endless loop for error

#define REG_CLR         0

#define NOP1()  _nop_()
#define NOP2()  NOP1(),NOP1()
#define NOP3()  NOP2(),NOP1()
#define NOP4()  NOP3(),NOP1()
#define NOP5()  NOP4(),NOP1()
#define NOP6()  NOP5(),NOP1()
#define NOP7()  NOP6(),NOP1()
#define NOP8()  NOP7(),NOP1()
#define NOP9()  NOP8(),NOP1()
#define NOP10() NOP9(),NOP1()
#define NOP11()  NOP10(),NOP1()
#define NOP12()  NOP11(),NOP1()
#define NOP13()  NOP12(),NOP1()
#define NOP14()  NOP13(),NOP1()
#define NOP15()  NOP14(),NOP1()
#define NOP16()  NOP15(),NOP1()
#define NOP17()  NOP16(),NOP1()
#define NOP18()  NOP17(),NOP1()
#define NOP19()  NOP18(),NOP1()
#define NOP20()  NOP19(),NOP1()
#define NOP(N)  NOP##N()

#define	clr_EA		EA=0	// 关闭总中断
#define set_EA		EA=1	// 使能总中断

#define SFR_SWITCH_ON()			_push_(DPS)				// save DPS				
#define SFR_SWITCH_OFF()		_pop_(DPS)				// restore DPS
#define SLK_SFR0()				SFRSL0=0				// SFR0 is selected
#define SLK_SFR1()				SFRSL0=1				// SFR1 is selected
#define USE_MUL16()				MUL16=1					// 16-bit hardware multiplier is used
#define NUS_MUL16()				MUL16=0					// 16-bit hardware multiplier is NOT used
#define USE_DIV16()				DIV16=1					// 16-bit hardware divider is used
#define NUS_DIV16()				MUL16=0					// 16-bit hardware divider is NOT used
#define SLK_DPTR()				SEL=0					// DPTR is selected
#define SLK_DPTR1()				SEL=1					// DPTR1 is selected

/*  MACRO  */ 
/*  SLPCR, 0x86H, SFR0  */  

/*  PCON, 0x87H, SFR0  */
#define SET_UART1EN()			PCON|=DEF_SET_BIT7			// PCON^7;
#define CLR_UART1EN()			PCON&=~DEF_SET_BIT7
//PCON^6;
//PCON^5;
#define SET_UART0EN()			PCON|=DEF_SET_BIT4			// PCON^4;
#define CLR_UART0EN()			PCON&=~DEF_SET_BIT4
#define SET_SMOD0()				PCON|=DEF_SET_BIT3			// PCON^3;
#define CLR_SMOD0()				PCON&=~DEF_SET_BIT3
#define SET_SSTAT0()			PCON|=DEF_SET_BIT2			// PCON^2;
#define CLR_SSTAT0()			PCON&=~DEF_SET_BIT2
#define SET_SLEEP()				PCON|=DEF_SET_BIT1			// PCON^1;  // protected by SLPCR & cleared by hardware
#define SET_STOP()				PCON|=DEF_SET_BIT0			// PCON^0;  // protected by SLPCR & cleared by hardware

/*  TMOD, 0x89H, SFR0  */
#define SET_GATE1()      		TMOD|=DEF_SET_BIT7			// TMOD^7;
#define CLR_GATE1()      		TMOD&=~DEF_SET_BIT7
#define SET_CT1()      			TMOD|=DEF_SET_BIT6      	// TMOD^6; 
#define CLR_CT1()      			TMOD&=~DEF_SET_BIT6 
#define SET_T1M1()      		TMOD|=DEF_SET_BIT5     		// TMOD^5; 
#define CLR_T1M1()      		TMOD&=~DEF_SET_BIT5
#define SET_T1M0()      		TMOD|=DEF_SET_BIT4			// TMOD^4; 
#define CLR_T1M0()      		TMOD&=~DEF_SET_BIT4
#define SET_GATE0()      		TMOD|=DEF_SET_BIT3			// TMOD^3; 
#define CLR_GATE0()      		TMOD&=~DEF_SET_BIT3
#define SET_CT0()      			TMOD|=DEF_SET_BIT2  	    // TMOD^2;
#define CLR_CT0()      			TMOD&=~DEF_SET_BIT2
#define SET_T0M1()      		TMOD|=DEF_SET_BIT1			// TMOD^1;
#define CLR_T0M1()      		TMOD&=~DEF_SET_BIT1
#define SET_T0M0()      		TMOD|=DEF_SET_BIT0			// TMOD^0;
#define CLR_T0M0()      		TMOD&=~DEF_SET_BIT0

/*  TL0, 0x8AH, SFR0  */

/*  TL1, 0x8BH, SFR0  */

/*  TH0, 0x8CH, SFR0  */

/*  TH1, 0x8DH, SFR0  */

/*  TCON1, 0x8EH, SFR0  */
#define SET_T1FS()				TCON1|=DEF_SET_BIT7			// TCON1^7;
#define CLR_T1FS()				TCON1&=~DEF_SET_BIT7
#define SET_T1OE()				TCON1|=DEF_SET_BIT6			// TCON1^6;
#define CLR_T1OE()				TCON1&=~DEF_SET_BIT6
#define SET_T1CKS1()			TCON1|=DEF_SET_BIT5			// TCON1^5;
#define CLR_T1CKS1()			TCON1&=~DEF_SET_BIT5
#define SET_T1CKS0()			TCON1|=DEF_SET_BIT4			// TCON1^4;
#define CLR_T1CKS0()			TCON1&=~DEF_SET_BIT4
#define SET_T0FS()				TCON1|=DEF_SET_BIT3			// TCON1^3;
#define CLR_T0FS()				TCON1&=~DEF_SET_BIT3
#define SET_T0OE()				TCON1|=DEF_SET_BIT2			// TCON1^2;
#define CLR_T0OE()				TCON1&=~DEF_SET_BIT2
#define SET_T0CKS1()			TCON1|=DEF_SET_BIT1			// TCON1^1;
#define CLR_T0CKS1()			TCON1&=~DEF_SET_BIT1
#define SET_T0CKS0()			TCON1|=DEF_SET_BIT0			// TCON1^0;
#define CLR_T0CKS0()			TCON1&=~DEF_SET_BIT0

/*  T3CR, 0x8FH, SFR0  */
#define SET_TR3()	      		T3CR|=DEF_SET_BIT7			// T3CR^7;
#define CLR_TR3()	      		T3CR&=~DEF_SET_BIT7
#define SET_CT3()   	  		T3CR|=DEF_SET_BIT6			// T3CR^6;
#define CLR_CT3()   	  		T3CR&=~DEF_SET_BIT6
#define SET_T3F()    	  		T3CR|=DEF_SET_BIT5			// T3CR^5;
#define CLR_T3F()    	  		T3CR&=~DEF_SET_BIT5 
//#define -      = T3CR^4; 
#define SET_T3EN()  	  		T3CR|=DEF_SET_BIT3			// T3CR^3;
#define CLR_T3EN()  	  		T3CR&=~DEF_SET_BIT3
#define SET_T3CKS2()     		T3CR|=DEF_SET_BIT2			// T3CR^2;
#define CLR_T3CKS2()     		T3CR&=~DEF_SET_BIT2
#define SET_T3CKS1()     		T3CR|=DEF_SET_BIT1			// T3CR^1;
#define CLR_T3CKS1()     		T3CR&=~DEF_SET_BIT1 
#define SET_T3CKS0()     		T3CR|=DEF_SET_BIT0			// T3CR^0;
#define CLR_T3CKS0()     		T3CR&=~DEF_SET_BIT0

/*  P1MOD0, 0x91H, SFR0  */

/*  P1MOD1, 0x92H, SFR0  */

/*  P1PU, 0x93H, SFR0  */

/*  P1S, 0x94H, SFR0  */

/*  TL3, 0x96H, SFR0  */

/*  TH3, 0x97H, SFR0  */

/*  SBUF, 0x99H, SFR0  */

/*  SADDR, 0x9AH, SFR0  */

/*  SADEN, 0x9BH, SFR0  */

/*  SCON_1, 0x9CH, SFR0  */
#define SET_SM0_1()      		SCON_1|=DEF_SET_BIT7		// SCON_1^7;
#define CLR_SM0_1()      		SCON_1&=~DEF_SET_BIT7
#define SET_SM1_1()				SCON_1|=DEF_SET_BIT6		//  SCON_1^6;
#define CLR_SM1_1()				SCON_1&=~DEF_SET_BIT6
#define SET_SM2_1()				SCON_1|=DEF_SET_BIT5		//  SCON_1^5;
#define CLR_SM2_1()				SCON_1&=~DEF_SET_BIT5
#define SET_REN_1()				SCON_1|=DEF_SET_BIT4		//  SCON_1^4;
#define CLR_REN_1()				SCON_1&=~DEF_SET_BIT4
#define SET_TB8_1()				SCON_1|=DEF_SET_BIT3		//  SCON_1^3;
#define CLR_TB8_1()				SCON_1&=~DEF_SET_BIT3
#define SET_RB8_1()				SCON_1|=DEF_SET_BIT2		//  SCON_1^2;
#define CLR_RB8_1()				SCON_1&=~DEF_SET_BIT2
#define SET_TI_1()				SCON_1|=DEF_SET_BIT1		//  SCON_1^1;
#define CLR_TI_1()				SCON_1&=~DEF_SET_BIT1
#define SET_RI_1()				SCON_1|=DEF_SET_BIT0		//  SCON_1^0;
#define CLR_RI_1()				SCON_1&=~DEF_SET_BIT0

/*  SBUF_1, 0x9DH, SFR0  */

/*  P2MOD0, 0xA1H, SFR0  */

/*  P2MOD1, 0xA2H, SFR0  */

/*  P2PU, 0xA3H, SFR0  */

/*  P2S, 0xA4H, SFR0  */

/*  IE1, 0xA9H, SFR0  */
#define SET_CRCIE()      		IE1|=DEF_SET_BIT7			// IE1^7;
#define CLR_CRCIE()      		IE1&=~DEF_SET_BIT7
//#define -      	= IE1^6;
#define SET_PWM1FBIE()			IE1|=DEF_SET_BIT5			// IE1^5;
#define CLR_PWM1FBIE()			IE1&=~DEF_SET_BIT5
#define SET_PWM0IE()			  IE1|=DEF_SET_BIT4			// IE1^4;
#define CLR_PWM0IE()			  IE1&=~DEF_SET_BIT4
#define set_ES			        IE1|=DEF_SET_BIT3			// IE1^3;
#define clr_ES			        IE1&=~DEF_SET_BIT3
#define SET_PWM1IE()   			IE1|=DEF_SET_BIT2			// IE1^2;
#define CLR_PWM1IE()   			IE1&=~DEF_SET_BIT2
#define SET_T3IE()       		IE1|=DEF_SET_BIT1			// IE1^1;
#define CLR_T3IE()       		IE1&=~DEF_SET_BIT1
#define SET_T2IE()	      		IE1|=DEF_SET_BIT0			// IE1^0;
#define CLR_T2IE()	      		IE1&=~DEF_SET_BIT0

/*  EXTIS1, 0xABH, SFR0  */
//#define -        = EXTIS1^7;
#define SET_EXT1IF4()     		EXTIS1|=DEF_SET_BIT6		// EXTIS1^6;
#define CLR_EXT1IF4()     		EXTIS1&=~DEF_SET_BIT6
#define SET_EXT1IF3()			EXTIS1|=DEF_SET_BIT5		//  EXTIS1^5;
#define CLR_EXT1IF3()			EXTIS1&=~DEF_SET_BIT5
#define SET_EXT1IF2()			EXTIS1|=DEF_SET_BIT4		//  EXTIS1^4;
#define CLR_EXT1IF2()			EXTIS1&=~DEF_SET_BIT4
#define SET_EXT1IF1()			EXTIS1|=DEF_SET_BIT3		//  EXTIS1^3;
#define CLR_EXT1IF1()			EXTIS1&=~DEF_SET_BIT3
#define SET_EXT1IF0()			EXTIS1|=DEF_SET_BIT2		//  EXTIS1^2;
#define CLR_EXT1IF0()			EXTIS1&=~DEF_SET_BIT2
#define SET_EXT1IS1()			EXTIS1|=DEF_SET_BIT1		//  EXTIS1^1;
#define CLR_EXT1IS1()			EXTIS1&=~DEF_SET_BIT1
#define SET_EXT1IS0()			EXTIS1|=DEF_SET_BIT0		//  EXTIS1^0;
#define CLR_EXT1IS0()			EXTIS1&=~DEF_SET_BIT0


/*  EXTEN1, 0xABH, SFR1  */
//#define -	      = EXTEN1^7;
#define SET_EXT1EN4()    		EXTEN1|=DEF_SET_BIT6		// EXTEN1^6;
#define CLR_EXT1EN4()    		EXTEN1&=~DEF_SET_BIT6
#define SET_EXT1EN3()    		EXTEN1|=DEF_SET_BIT5		// EXTEN1^5;
#define CLR_EXT1EN3()    		EXTEN1&=~DEF_SET_BIT5
#define SET_EXT1EN2()    		EXTEN1|=DEF_SET_BIT4		// EXTEN1^4;
#define CLR_EXT1EN2()    		EXTEN1&=~DEF_SET_BIT4
#define SET_EXT1EN1()    		EXTEN1|=DEF_SET_BIT3		// EXTEN1^3;
#define CLR_EXT1EN1()    		EXTEN1&=~DEF_SET_BIT3
#define SET_EXT1EN0()    		EXTEN1|=DEF_SET_BIT2		// EXTEN1^2;
#define CLR_EXT1EN0()    		EXTEN1&=~DEF_SET_BIT2
//#define -        = EXTEN1^1;
#define SET_EXT1A()      		EXTEN1|=DEF_SET_BIT0		// EXTEN1^0;
#define CLR_EXT1A()      		EXTEN1&=~DEF_SET_BIT0

/*  EXTIS, 0xACH, SFR0  */
//#define -        = EXTIS^7;
#define SET_EXT0IF4()     		EXTIS|=DEF_SET_BIT6			// EXTIS^6;
#define CLR_EXT0IF4()     		EXTIS&=~DEF_SET_BIT6
#define SET_EXT0IF3()     		EXTIS|=DEF_SET_BIT5			// EXTIS^5;
#define CLR_EXT0IF3()     		EXTIS&=~DEF_SET_BIT5
#define SET_EXT0IF2()	 		EXTIS|=DEF_SET_BIT4			// EXTIS^4;
#define CLR_EXT0IF2()	 		EXTIS&=~DEF_SET_BIT4
#define SET_EXT0IF1()	 		EXTIS|=DEF_SET_BIT3			// EXTIS^3;
#define CLR_EXT0IF1()	 		EXTIS&=~DEF_SET_BIT3
#define SET_EXT0IF0()     		EXTIS|=DEF_SET_BIT2			// EXTIS^2;
#define CLR_EXT0IF0()     		EXTIS&=~DEF_SET_BIT2
#define SET_EXT0IS1()			EXTIS|=DEF_SET_BIT1			//  EXTIS^1;
#define CLR_EXT0IS1()			EXTIS&=~DEF_SET_BIT1
#define SET_EXT0IS0()			EXTIS|=DEF_SET_BIT0			//  EXTIS^0;
#define CLR_EXT0IS0()			EXTIS&=~DEF_SET_BIT0


/*  EXTEN, 0xACH, SFR1  */
//#define -	      = EXTEN^7;
#define SET_EXT0EN4()    		EXTEN|=DEF_SET_BIT6			// EXTEN^6;
#define CLR_EXT0EN4()    		EXTEN&=~DEF_SET_BIT6
#define SET_EXT0EN3()    		EXTEN|=DEF_SET_BIT5			// EXTEN^5;
#define CLR_EXT0EN3()    		EXTEN&=~DEF_SET_BIT5
#define SET_EXT0EN2()    		EXTEN|=DEF_SET_BIT4			// EXTEN^4;
#define CLR_EXT0EN2()    		EXTEN&=~DEF_SET_BIT4
#define SET_EXT0EN1()    		EXTEN|=DEF_SET_BIT3			// EXTEN^3;
#define CLR_EXT0EN1()    		EXTEN&=~DEF_SET_BIT3
#define SET_EXT0EN0()    		EXTEN|=DEF_SET_BIT2			// EXTEN^2;
#define CLR_EXT0EN0()    		EXTEN&=~DEF_SET_BIT2
//#define -        = EXTEN^1;
#define SET_EXT0A()      		EXTEN|=DEF_SET_BIT0			// EXTEN^0;
#define CLR_EXT0A()      		EXTEN&=~DEF_SET_BIT0

/*  SPCON, 0xADH, SFR0  */
#define SET_DIR()        		SPCON|=DEF_SET_BIT7			// SPCON^7;
#define CLR_DIR()        		SPCON&=~DEF_SET_BIT7
#define SET_MSTR()				SPCON|=DEF_SET_BIT6			// SPCON^6;
#define CLR_MSTR()				SPCON&=~DEF_SET_BIT6
#define SET_CPHA()				SPCON|=DEF_SET_BIT5			// SPCON^5;
#define CLR_CPHA()				SPCON&=~DEF_SET_BIT5
#define SET_CPOL()		    	SPCON|=DEF_SET_BIT4			// SPCON^4;
#define CLR_CPOL()		    	SPCON&=~DEF_SET_BIT4
#define SET_SSDIS()				SPCON|=DEF_SET_BIT3			// SPCON^3;
#define CLR_SSDIS()				SPCON&=~DEF_SET_BIT3
#define SET_SPR2()				SPCON|=DEF_SET_BIT2			// SPCON^2;
#define CLR_SPR2()				SPCON&=~DEF_SET_BIT2
#define SET_SPR1()				SPCON|=DEF_SET_BIT1			// SPCON^1;
#define CLR_SPR1()				SPCON&=~DEF_SET_BIT1
#define SET_SPR0()	      		SPCON|=DEF_SET_BIT0			// SPCON^0;
#define CLR_SPR0()	      		SPCON&=~DEF_SET_BIT0

/*  SPSTA, 0xAEH, SFR0  */
#define SET_SPEN()        		SPSTA|=DEF_SET_BIT7			// SPSTA^7;
#define CLR_SPEN()        		SPSTA&=~DEF_SET_BIT7
#define SET_SPIF()	      		SPSTA|=DEF_SET_BIT6			// SPSTA^6;
#define CLR_SPIF()	      		SPSTA&=~DEF_SET_BIT6
#define SET_MODF()				SPSTA|=DEF_SET_BIT5			// SPSTA^5;
#define CLR_MODF()				SPSTA&=~DEF_SET_BIT5
#define SET_WCOL()		    	SPSTA|=DEF_SET_BIT4			// SPSTA^4;
#define CLR_WCOL()		    	SPSTA&=~DEF_SET_BIT4
#define SET_SPIOV()	    		SPSTA|=DEF_SET_BIT3			// SPSTA^3;
#define CLR_SPIOV()	    		SPSTA&=~DEF_SET_BIT3
//#define 	  	    	SPSTA|=DEF_SET_BIT2						// SPSTA^2;
//#define       		SPSTA|=DEF_SET_BIT1						// SPSTA^1;
//#define 	      		SPSTA|=DEF_SET_BIT0						// SPSTA^0;

/*  SPDAT, 0xAFH, SFR0  */

/*  IPH, 0xB9H, SFR0  */
//#define -        = IP^7;
#define SET_ADCIPH()    		IPH|=DEF_SET_BIT6			// IP^6;
#define CLR_ADCIPH()    		IPH&=~DEF_SET_BIT6
#define SET_SPIIPH()      		IP1|=DEF_SET_BIT5			// IP1^5;
#define CLR_SPIIPH()      		IP1&=~DEF_SET_BIT5
#define SET_UART0IPH()   		IPH|=DEF_SET_BIT4			// IP^4;
#define CLR_UART0IPH()   		IPH&=~DEF_SET_BIT4
#define SET_T1IPH()      		IPH|=DEF_SET_BIT3			// IP^3;
#define CLR_T1IPH()      		IPH&=~DEF_SET_BIT3
#define SET_EXT1IPH()    		IPH|=DEF_SET_BIT2			// IP^2;
#define CLR_EXT1IPH()    		IPH&=~DEF_SET_BIT2
#define SET_T0IPH()	    		IPH|=DEF_SET_BIT1			// IP^1;
#define CLR_T0IPH()	    		IPH&=~DEF_SET_BIT1
#define SET_EXT0IPH()    		IPH|=DEF_SET_BIT0			// IP^0;
#define CLR_EXT0IPH()    		IPH&=~DEF_SET_BIT0

/*  IP1, 0xBAH, SFR0  */
#define SET_CRCIPL()    		IP1|=DEF_SET_BIT7			// IP1^7;
#define CLR_CRCIPL()    		IP1&=~DEF_SET_BIT7
//#define SET_()    		IP1|=DEF_SET_BIT6					// IP1^2;
//#define CLR_()    		IP1&=~DEF_SET_BIT6
#define SET_PWM1FBIPL()    		IP1|=DEF_SET_BIT5			// IP1^6;
#define CLR_PWM1FBIPL()    		IP1&=~DEF_SET_BIT5
#define SET_PWM0IPL()   		IP1|=DEF_SET_BIT4			// IP1^4;
#define CLR_PWM0IPL()   		IP1&=~DEF_SET_BIT4
#define SET_UART1IPL()   		IPH|=DEF_SET_BIT3			// IP^3;
#define CLR_UART1IPL()   		IPH&=~DEF_SET_BIT3
#define SET_PWM1IPL()   		IP1|=DEF_SET_BIT2			// IP1^3;
#define CLR_PWM1IPL()   		IP1&=~DEF_SET_BIT2
#define SET_T3IPL()	    		IP1|=DEF_SET_BIT1			// IP1^1;
#define CLR_T3IPL()	    		IP1&=~DEF_SET_BIT1
#define SET_T2IPL()    			IP1|=DEF_SET_BIT0			// IP1^0;
#define CLR_T2IPL()    			IP1&=~DEF_SET_BIT0

/*  IPH1, 0xBBH, SFR0  */
#define SET_CRCIPH()    		IPH1|=DEF_SET_BIT7			// IPH1^7;
#define CLR_CRCIPH()    		IPH1&=~DEF_SET_BIT7
//#define SET_()    		IPH1|=DEF_SET_BIT6					// IPH1^6;
//#define CLR_()    		IPH1&=~DEF_SET_BIT6
#define SET_PWM1FBIPH()    		IPH1|=DEF_SET_BIT5			// IPH1^5;
#define CLR_PWM1FBIPH()    		IPH1&=~DEF_SET_BIT5
#define SET_PWM0IPH()   		IPH1|=DEF_SET_BIT4			// IPH1^4;
#define CLR_PWM0IPH()   		IPH1&=~DEF_SET_BIT4
#define SET_UART1IPH()     		IPH1|=DEF_SET_BIT3			// IPH1^3;
#define CLR_UART1IPH()     		IPH1&=~DEF_SET_BIT3
#define SET_PWM1IPH()   		IPH1|=DEF_SET_BIT2			// IPH1^2;
#define CLR_PWM1IPH()   		IPH1&=~DEF_SET_BIT2
#define SET_T3IPH()	    		IPH1|=DEF_SET_BIT1			// IPH1^1;
#define CLR_T3IPH()	    		IPH1&=~DEF_SET_BIT1
#define SET_T2IPH()    			IPH1|=DEF_SET_BIT0			// IPH1^0;
#define CLR_T2IPH()    			IPH1&=~DEF_SET_BIT0

/*  P0MOOD0, 0xC1H, SFR0  */

/*  CRCCR, 0xC1H, SFR1  */
#define SET_CRCEN()    		CRCCR|=DEF_SET_BIT7				// CRCCR^7;
#define CLR_CRCEN()    		CRCCR&=~DEF_SET_BIT7
//#define SET_	    		CRCCR|=DEF_SET_BIT6					// CRCCR^6;
//#define CLR_	    		CRCCR&=~DEF_SET_BIT6
#define SET_CRCIF()   		CRCCR|=DEF_SET_BIT5				// CRCCR^5;
#define CLR_CRCIF()   		CRCCR&=~DEF_SET_BIT5
#define SET_CRCST()   		CRCCR|=DEF_SET_BIT4				// CRCCR^4;
#define CLR_CRCST()   		CRCCR&=~DEF_SET_BIT4
#define SET_CRCMD()    		CRCCR|=DEF_SET_BIT3				// CRCCR^3;
#define CLR_CRCMD()    		CRCCR&=~DEF_SET_BIT3
#define SET_CRCLCT()   		CRCCR|=DEF_SET_BIT2				// CRCCR^2;
#define CLR_CRCLCT()   		CRCCR&=~DEF_SET_BIT2
#define SET_CRCBIT()   		CRCCR|=DEF_SET_BIT1				// CRCCR^1;
#define CLR_CRCBIT()   		CRCCR&=~DEF_SET_BIT1
#define SET_CRCENC()		CRCCR|=DEF_SET_BIT0				// CRCCR^0;
#define CLR_CRCENC()		CRCCR&=~DEF_SET_BIT0

/*  CRCL, 0xC2H, SFR1  */


/*  P0PU, 0xC3H, SFR0  */
/*  CRCH, 0xC3H, SFR1  */

/*  P0S, 0xC4H, SFR0  */
/*  CRCSTL, 0xC4H, SFR1  */

/*  CRCSTH, 0xC5H, SFR1  */

/*  CRCEDL, 0xC6H, SFR1  */

/*  CRCEDH, 0xC7H, SFR1  */

/*  T2MOD, 0xC9H, SFR0  */
#define SET_LD2EN()    		T2MOD|=DEF_SET_BIT7				// T2MOD^7;
#define CLR_LD2EN()    		T2MOD&=~DEF_SET_BIT7
#define SET_T2CKS2()   		T2MOD|=DEF_SET_BIT6				// T2MOD^6;
#define CLR_T2CKS2()   		T2MOD&=~DEF_SET_BIT6
#define SET_T2CKS1()   		T2MOD|=DEF_SET_BIT5				// T2MOD^5;
#define CLR_T2CKS1()   		T2MOD&=~DEF_SET_BIT5
#define SET_T2CKS0()    	T2MOD|=DEF_SET_BIT4				// T2MOD^4;
#define CLR_T2CKS0()    	T2MOD&=~DEF_SET_BIT4
#define SET_T2OE()    		T2MOD|=DEF_SET_BIT3				// T2MOD^3;
#define CLR_T2OE()	   		T2MOD&=~DEF_SET_BIT3
#define SET_T2SL1()   		T2MOD|=DEF_SET_BIT2				// T2MOD^2;
#define CLR_T2SL1()   		T2MOD&=~DEF_SET_BIT2
#define SET_T2SL0()   		T2MOD|=DEF_SET_BIT1				// T2MOD^1;
#define CLR_T2SL0()   		T2MOD&=~DEF_SET_BIT1
#define SET_DC2EN()			T2MOD|=DEF_SET_BIT0				// T2MOD^0;
#define CLR_DC2EN()			T2MOD&=~DEF_SET_BIT0

/*  RCAPL2, 0xCBH, SFR0  */

/*  RCAPH2, 0xCAH, SFR0  */

/*  TL2, 0xCCH, SFR0  */

/*  TH2, 0xCDH, SFR0  */

/*  T2CON1, 0xCEH, SFR0  */
#define SET_CM2ON()    		T2CON1|=DEF_SET_BIT7			// T2CON1^7;
#define CLR_CM2ON()    		T2CON1&=~DEF_SET_BIT7
#define SET_CM2()	    	T2CON1|=DEF_SET_BIT6			// T2CON1^6;
#define CLR_CM2()	    	T2CON1&=~DEF_SET_BIT6
#define SET_BR2EN()   		T2CON1|=DEF_SET_BIT5			// T2CON1^5;
#define CLR_BR2EN()   		T2CON1&=~DEF_SET_BIT5
#define SET_RCLKU1()   		T2CON1|=DEF_SET_BIT4			// T2CON1^4;
#define CLR_RCLKU1()   		T2CON1&=~DEF_SET_BIT4
#define SET_TCLKU1()    	T2CON1|=DEF_SET_BIT3			// T2CON1^3;
#define CLR_TCLKU1()    	T2CON1&=~DEF_SET_BIT3
#define SET_EX2SL()   		T2CON1|=DEF_SET_BIT2			// T2CON1^2;
#define CLR_EX2SL()   		T2CON1&=~DEF_SET_BIT2
// #define SET_   		T2CON1|=DEF_SET_BIT1				// T2CON1^1;
// #define CLR_   		T2CON1&=~DEF_SET_BIT1
#define SET_T2EN()			T2CON1|=DEF_SET_BIT0			// T2CON1^0;
#define CLR_T2EN()			T2CON1&=~DEF_SET_BIT0

/*  WDTCR, 0xCFH, SFR0  */
//#define SET_()    		WDTCR|=DEF_SET_BIT7				// WDTCR^7;
//#define CLR_()    		WDTCR&=~DEF_SET_BIT7
//#define SET_	    		WDTCR|=DEF_SET_BIT6				// WDTCR^6;
//#define CLR_	    		WDTCR&=~DEF_SET_BIT6
//#define SET_()   		WDTCR|=DEF_SET_BIT5				// WDTCR^5;
//#define CLR_()   		WDTCR&=~DEF_SET_BIT5
//#define SET_()   		WDTCR|=DEF_SET_BIT4				// WDTCR^4;
//#define CLR_()   		WDTCR&=~DEF_SET_BIT4
//#define SET_()    		WDTCR|=DEF_SET_BIT3				// WDTCR^3;
//#define CLR_()    		WDTCR&=~DEF_SET_BIT3
#define SET_WDTOT2()   		WDTCR|=DEF_SET_BIT2				// WDTCR^2;
#define CLR_WDTOT2()   		WDTCR&=~DEF_SET_BIT2
#define SET_WDTOT1()   		WDTCR|=DEF_SET_BIT1				// WDTCR^1;
#define CLR_WDTOT1()   		WDTCR&=~DEF_SET_BIT1
#define SET_WDTOT0()		WDTCR|=DEF_SET_BIT0				// WDTCR^0;
#define CLR_WDTOT0()		WDTCR&=~DEF_SET_BIT0

/*  LCDCR0, 0xD9H, SFR0  */
#define SET_LCDEN()    		LCDCR0|=DEF_SET_BIT7			// LCDCR0^7;
#define CLR_LCDEN()    		LCDCR0&=~DEF_SET_BIT7
#define SET_LCDMS()	    	LCDCR0|=DEF_SET_BIT6			// LCDCR0^6;
#define CLR_LCDMS()	    	LCDCR0&=~DEF_SET_BIT6
//#define SET_()   		LCDCR0|=DEF_SET_BIT5				// LCDCR0^5;
//#define CLR_()   		LCDCR0&=~DEF_SET_BIT5
//#define SET_()   		LCDCR0|=DEF_SET_BIT4				// LCDCR0^4;
//#define CLR_()   		LCDCR0&=~DEF_SET_BIT4
//#define SET_()    		LCDCR0|=DEF_SET_BIT3				// LCDCR0^3;
//#define CLR_()    		LCDCR0&=~DEF_SET_BIT3
//#define SET_()   		LCDCR0|=DEF_SET_BIT2				// LCDCR0^2;
//#define CLR_()   		LCDCR0&=~DEF_SET_BIT2
//#define SET_()   		LCDCR0|=DEF_SET_BIT1				// LCDCR0^1;
//#define CLR_()   		LCDCR0&=~DEF_SET_BIT1
//#define SET_()			LCDCR0|=DEF_SET_BIT0				// LCDCR0^0;
//#define CLR_()			LCDCR0&=~DEF_SET_BIT0


/*  LCDSW, 0xDAH, SFR0  */
#define SET_LCDRS1()    		LCDSW|=DEF_SET_BIT7			// LCDSW^7;
#define CLR_LCDRS1()    		LCDSW&=~DEF_SET_BIT7
#define SET_LCDRS0()	    	LCDSW|=DEF_SET_BIT6			// LCDSW^6;
#define CLR_LCDRS0()	    	LCDSW&=~DEF_SET_BIT6
//#define SET_()   		LCDSW|=DEF_SET_BIT5				// LCDSW^5;
//#define CLR_()   		LCDSW&=~DEF_SET_BIT5
#define SET_COM4IOS()   		LCDSW|=DEF_SET_BIT4			// LCDSW^4;
#define CLR_COM4IOS()   		LCDSW&=~DEF_SET_BIT4
#define SET_COM3IOS()    		LCDSW|=DEF_SET_BIT3			// LCDSW^3;
#define CLR_COM3IOS()    		LCDSW&=~DEF_SET_BIT3
#define SET_COM2IOS()   		LCDSW|=DEF_SET_BIT2			// LCDSW^2;
#define CLR_COM2IOS()   		LCDSW&=~DEF_SET_BIT2
#define SET_COM1IOS()   		LCDSW|=DEF_SET_BIT1			// LCDSW^1;
#define CLR_COM1IOS()   		LCDSW&=~DEF_SET_BIT1
#define SET_COM0IOS()			LCDSW|=DEF_SET_BIT0			// LCDSW^0;
#define CLR_COM0IOS()			LCDSW&=~DEF_SET_BIT0

/*  PWM0CR, 0xDDH, SFR0  */
#define SET_PWM0EN()    		PWM0CR|=DEF_SET_BIT7		// PWM0CR^7;
#define CLR_PWM0EN()    		PWM0CR&=~DEF_SET_BIT7
//#define SET_()	    	PWM0CR|=DEF_SET_BIT6					// PWM0CR^6;
//#define CLR_()	    	PWM0CR&=~DEF_SET_BIT6
#define SET_PWM0IF()   		PWM0CR|=DEF_SET_BIT5			// PWM0CR^5;
#define CLR_PWM0IF()   		PWM0CR&=~DEF_SET_BIT5
#define SET_PWM0OE()   		PWM0CR|=DEF_SET_BIT4			// PWM0CR^4;
#define CLR_PWM0OE()   		PWM0CR&=~DEF_SET_BIT4
#define SET_PWM0S()    		PWM0CR|=DEF_SET_BIT3			// PWM0CR^3;
#define CLR_PWM0S()    		PWM0CR&=~DEF_SET_BIT3
#define SET_PWM0C2()   		PWM0CR|=DEF_SET_BIT2			// PWM0CR^2;
#define CLR_PWM0C2()   		PWM0CR&=~DEF_SET_BIT2
#define SET_PWM0C1()   		PWM0CR|=DEF_SET_BIT1			// PWM0CR^1;
#define CLR_PWM0C1()   		PWM0CR&=~DEF_SET_BIT1
#define SET_PWM0C0()		PWM0CR|=DEF_SET_BIT0			// PWM0CR^0;
#define CLR_PWM0C0()		PWM0CR&=~DEF_SET_BIT0

/*  PWM0P, 0xDEH, SFR0  */

/*  PWM0D, 0xDFH, SFR0  */

/*  PWM1PH, 0xE1H, SFR1  */

/*  PWM1PL, 0xE2H, SFR1  */

/*  PWM1CR3, 0xE3H, SFR1  */
#define SET_PWM1CB()    	PWM1CR3|=DEF_SET_BIT7			// PWM1CR3^7;
#define CLR_PWM1CB()    	PWM1CR3&=~DEF_SET_BIT7
//#define SET_()	    	PWM1CR3|=DEF_SET_BIT6					// PWM1CR3^6;
//#define CLR_()	    	PWM1CR3&=~DEF_SET_BIT6
#define SET_PWM15B()   		PWM1CR3|=DEF_SET_BIT5			// PWM1CR3^5;
#define CLR_PWM15B()   		PWM1CR3&=~DEF_SET_BIT5
#define SET_PWM14B()   		PWM1CR3|=DEF_SET_BIT4			// PWM1CR3^4;
#define CLR_PWM14B()   		PWM1CR3&=~DEF_SET_BIT4
#define SET_PWM13B()   		PWM1CR3|=DEF_SET_BIT3			// PWM1CR3^3;
#define CLR_PWM13B()   		PWM1CR3&=~DEF_SET_BIT3
#define SET_PWM12B()   		PWM1CR3|=DEF_SET_BIT2			// PWM1CR3^2;
#define CLR_PWM12B()   		PWM1CR3&=~DEF_SET_BIT2
#define SET_PWM11B()   		PWM1CR3|=DEF_SET_BIT1			// PWM1CR3^1;
#define CLR_PWM11B()   		PWM1CR3&=~DEF_SET_BIT1
#define SET_PWM10B()		PWM1CR3|=DEF_SET_BIT0			// PWM1CR3^0;
#define CLR_PWM10B()		PWM1CR3&=~DEF_SET_BIT0

/*  PWM1CR2, 0xE4H, SFR1  */
#define SET_PWM1RL()    	PWM1CR2|=DEF_SET_BIT7			// PWM1CR2^7;
#define CLR_PWM1RL()    	PWM1CR2&=~DEF_SET_BIT7
#define SET_PWM145FEN()    	PWM1CR2|=DEF_SET_BIT6			// PWM1CR2^6;
#define CLR_PWM145FEN()    	PWM1CR2&=~DEF_SET_BIT6
#define SET_PWM123FEN()		PWM1CR2|=DEF_SET_BIT5			// PWM1CR2^5;
#define CLR_PWM123FEN()		PWM1CR2&=~DEF_SET_BIT5
#define SET_PWM101FEN()		PWM1CR2|=DEF_SET_BIT4			// PWM1CR2^4;
#define CLR_PWM101FEN()		PWM1CR2&=~DEF_SET_BIT4
//#define SET_()   		PWM1CR2|=DEF_SET_BIT3				// PWM1CR2^3;
//#define CLR_()   		PWM1CR2&=~DEF_SET_BIT3
#define SET_PWM145S()  		PWM1CR2|=DEF_SET_BIT2			// PWM1CR2^2;
#define CLR_PWM145S()  		PWM1CR2&=~DEF_SET_BIT2
#define SET_PWM123S()  		PWM1CR2|=DEF_SET_BIT1			// PWM1CR2^1;
#define CLR_PWM123S()  		PWM1CR2&=~DEF_SET_BIT1
#define SET_PWM101S()		PWM1CR2|=DEF_SET_BIT0			// PWM1CR2^0;
#define CLR_PWM101S()		PWM1CR2&=~DEF_SET_BIT0

/*  PWM1CR1, 0xE5H, SFR1  */
#define SET_PWM1CS()    	PWM1CR1|=DEF_SET_BIT7			// PWM1CR1^7;
#define CLR_PWM1CS()    	PWM1CR1&=~DEF_SET_BIT7
#define SET_PWM145MD()    	PWM1CR1|=DEF_SET_BIT6			// PWM1CR1^6;
#define CLR_PWM145MD()    	PWM1CR1&=~DEF_SET_BIT6
#define SET_PWM123MD() 		PWM1CR1|=DEF_SET_BIT5			// PWM1CR1^5;
#define CLR_PWM123MD() 		PWM1CR1&=~DEF_SET_BIT5
#define SET_PWM101MD() 		PWM1CR1|=DEF_SET_BIT4			// PWM1CR1^4;
#define CLR_PWM101MD() 		PWM1CR1&=~DEF_SET_BIT4
//#define SET_()  		PWM1CR1|=DEF_SET_BIT3					// PWM1CR1^3;
//#define CLR_()  		PWM1CR1&=~DEF_SET_BIT3
#define SET_PWM1CLK2() 		PWM1CR1|=DEF_SET_BIT2			// PWM1CR1^2;
#define CLR_PWM1CLK2() 		PWM1CR1&=~DEF_SET_BIT2
#define SET_PWM1CLK1() 		PWM1CR1|=DEF_SET_BIT1			// PWM1CR1^1;
#define CLR_PWM1CLK1() 		PWM1CR1&=~DEF_SET_BIT1
#define SET_PWM1CLK0()		PWM1CR1|=DEF_SET_BIT0			// PWM1CR1^0;
#define CLR_PWM1CLK0()		PWM1CR1&=~DEF_SET_BIT0

/*  PWM1CR0, 0xE6H, SFR1  */
#define SET_PWM1EN()    	PWM1CR0|=DEF_SET_BIT7			// PWM1CR0^7;
#define CLR_PWM1EN()    	PWM1CR0&=~DEF_SET_BIT7
//#define SET_()	    	PWM1CR0|=DEF_SET_BIT6					// PWM1CR0^6;
//#define CLR_()	    	PWM1CR0&=~DEF_SET_BIT6
#define SET_PWM15OE()  		PWM1CR0|=DEF_SET_BIT5			// PWM1CR0^5;
#define CLR_PWM15OE()  		PWM1CR0&=~DEF_SET_BIT5
#define SET_PWM14OE()  		PWM1CR0|=DEF_SET_BIT4			// PWM1CR0^4;
#define CLR_PWM14OE()  		PWM1CR0&=~DEF_SET_BIT4
#define SET_PWM13OE()  		PWM1CR0|=DEF_SET_BIT3			// PWM1CR0^3;
#define CLR_PWM13OE()  		PWM1CR0&=~DEF_SET_BIT3
#define SET_PWM12OE()  		PWM1CR0|=DEF_SET_BIT2			// PWM1CR0^2;
#define CLR_PWM12OE()  		PWM1CR0&=~DEF_SET_BIT2
#define SET_PWM11OE()  		PWM1CR0|=DEF_SET_BIT1			// PWM1CR0^1;
#define CLR_PWM11OE()  		PWM1CR0&=~DEF_SET_BIT1
#define SET_PWM10OE()		PWM1CR0|=DEF_SET_BIT0			// PWM1CR0^0;
#define CLR_PWM10OE()		PWM1CR0&=~DEF_SET_BIT0

/*  PWM1IER, 0xE7H, SFR1  */
#define SET_PWM1IF()    	PWM1IER|=DEF_SET_BIT7			// PWM1IER^7;
#define CLR_PWM1IF()    	PWM1IER&=~DEF_SET_BIT7
#define SET_PWM1FBIF()    	PWM1IER|=DEF_SET_BIT6			// PWM1IER^6;
#define CLR_PWM1FBIF()    	PWM1IER&=~DEF_SET_BIT6
//#define SET_()		 		PWM1IER|=DEF_SET_BIT5			// PWM1IER^5;
//#define CLR_()		  		PWM1IER&=~DEF_SET_BIT5
//#define SET_()		 		PWM1IER|=DEF_SET_BIT4			// PWM1IER^4;
//#define CLR_()		  		PWM1IER&=~DEF_SET_BIT4
//#define SET_()		 		PWM1IER|=DEF_SET_BIT3			// PWM1IER^3;
//#define CLR_()		  		PWM1IER&=~DEF_SET_BIT3
//#define SET_()		 		PWM1IER|=DEF_SET_BIT2			// PWM1IER^2;
//#define CLR_()		  		PWM1IER&=~DEF_SET_BIT2
#define SET_PWM1FBIS() 		PWM1IER|=DEF_SET_BIT1			// PWM1IER^1;
#define CLR_PWM1FBIS() 		PWM1IER&=~DEF_SET_BIT1
#define SET_PWM1FBEN()		PWM1IER|=DEF_SET_BIT0			// PWM1IER^0;
#define CLR_PWM1FBEN()		PWM1IER&=~DEF_SET_BIT0

/*  IAPCR, 0xE9H, SFR0  */
#define SET_IAPSTR()    	IAPCR|=DEF_SET_BIT7				// PWM1IER^7;
#define CLR_IAPSTR()    	IAPCR&=~DEF_SET_BIT7
//#define SET_()			    	IAPCR|=DEF_SET_BIT6				// PWM1IER^6;
//#define CLR_()    			IAPCR&=~DEF_SET_BIT6
//#define SET_() 				IAPCR|=DEF_SET_BIT5				// PWM1IER^5;
//#define CLR_()  				IAPCR&=~DEF_SET_BIT5
//#define SET_() 				IAPCR|=DEF_SET_BIT4				// PWM1IER^4;
//#define CLR_()  				IAPCR&=~DEF_SET_BIT4
#define SET_IAPTRIG2() 		IAPCR|=DEF_SET_BIT3				// PWM1IER^3;
#define CLR_IAPTRIG2() 		IAPCR&=~DEF_SET_BIT3
#define SET_IAPTRIG1() 		IAPCR|=DEF_SET_BIT2				// PWM1IER^2;
#define CLR_IAPTRIG1() 		IAPCR&=~DEF_SET_BIT2
#define SET_IAPTRIG0() 		IAPCR|=DEF_SET_BIT1				// PWM1IER^2;
#define CLR_IAPTRIG0() 		IAPCR&=~DEF_SET_BIT1
#define SET_EERD()			IAPCR|=DEF_SET_BIT0				// PWM1IER^0;
#define CLR_EERD()			IAPCR&=~DEF_SET_BIT0

/*  PWM12DH, 0xE9H, SFR1  */

/*  IAPADL, 0xEAH, SFR0  */

/*  PWM12DL, 0xEAH, SFR1  */

/*  IAPADH, 0xEBH, SFR0  */

/*  PWM101F, 0xEBH, SFR1  */

/*  IAPDB, 0xECH, SFR0  */

/*  PWM11DH, 0xECH, SFR1  */

/*  PWM11DL, 0xEDH, SFR1  */

/*  IAPMD, 0xEEH, SFR0  */
#define SET_IAPEN()			IAPMD|=DEF_SET_BIT7			// IAPMD^7;
#define CLR_IAPEN()			IAPMD&=~DEF_SET_BIT7
// #define SET_				|=DEF_SET_BIT6				// IAPMD^6;
// #define CLR_				&=~DEF_SET_BIT6
// #define SET_				|=DEF_SET_BIT5				// IAPMD^5;
// #define CLR_				&=~DEF_SET_BIT5
// #define SET_				|=DEF_SET_BIT4				// IAPMD^4;
// #define CLR_				&=~DEF_SET_BIT4
#define SET_ECHIPERS()		IAPMD|=DEF_SET_BIT3			// IAPMD^3;
#define CLR_ECHIPERS()		IAPMD&=~DEF_SET_BIT3
#define SET_EPAGEERS()		IAPMD|=DEF_SET_BIT2			// IAPMD^2;
#define CLR_EPAGEERS()		IAPMD&=~DEF_SET_BIT2
#define SET_EPAGEWR()		IAPMD|=DEF_SET_BIT1			// IAPMD^1;
#define CLR_EPAGEWR()		IAPMD&=~DEF_SET_BIT1
// #define SET_				|=DEF_SET_BIT0				// IAPMD^0;
// #define CLR_				&=~DEF_SET_BIT0

/*  PWM10DH, 0xEEH, SFR1  */

/*  IAPKEY, 0xEFH, SFR0  */

/*  PWM10DL, 0xEFH, SFR1  */

/*  AUXC, 0xF1H, SFR0  */

/*  PWM15DH, 0xF1H, SFR1  */

/*  PWM15DL, 0xF2H, SFR1  */

/*  PWM14DH, 0xF3H, SFR1  */

/*  PWM14DL, 0xF4H, SFR1  */

/*  PWM123F, 0xF5H, SFR1  */

/*  PWM13DH, 0xF6H, SFR1  */

/*  PWM13DL, 0xF7H, SFR1  */

/*  ADCCR0, 0xF9H, SFR0  */
#define SET_ADCHS3()		ADCCR0|=DEF_SET_BIT7			// ADCCR0^7;
#define CLR_ADCHS3()		ADCCR0&=~DEF_SET_BIT7
#define SET_ADCHS2()	    ADCCR0|=DEF_SET_BIT6			// ADCCR0^6;
#define CLR_ADCHS2()	    ADCCR0&=~DEF_SET_BIT6
#define SET_ADCHS1()		ADCCR0|=DEF_SET_BIT5			// ADCCR0^5;
#define CLR_ADCHS1()		ADCCR0&=~DEF_SET_BIT5
#define SET_ADCHS0()		ADCCR0|=DEF_SET_BIT4			// ADCCR0^4;
#define CLR_ADCHS0()		ADCCR0&=~DEF_SET_BIT4
#define SET_VRS1()			ADCCR0|=DEF_SET_BIT3			// ADCCR0^3;
#define CLR_VRS1()			ADCCR0&=~DEF_SET_BIT3
#define SET_VRS0()			ADCCR0|=DEF_SET_BIT2			// ADCCR0^2;
#define CLR_VRS0()			ADCCR0&=~DEF_SET_BIT2
#define SET_ADEOC()			ADCCR0|=DEF_SET_BIT1			// ADCCR0^1;
#define CLR_ADEOC()			ADCCR0&=~DEF_SET_BIT1
#define SET_ADEN()			ADCCR0|=DEF_SET_BIT0			// ADCCR0^0;
#define CLR_ADEN()			ADCCR0&=~DEF_SET_BIT0

/*  ADCCR1, 0xFAH, SFR0  */
#define SET_ADRSEL()		ADCCR1|=DEF_SET_BIT7			// ADCCR1^7;
#define CLR_ADRSEL()		ADCCR1&=~DEF_SET_BIT7
#define SET_ADCKS2()	    ADCCR1|=DEF_SET_BIT6			// ADCCR1^6;
#define CLR_ADCKS2()	    ADCCR1&=~DEF_SET_BIT6
#define SET_ADCKS1()		ADCCR1|=DEF_SET_BIT5			// ADCCR1^5;
#define CLR_ADCKS1()		ADCCR1&=~DEF_SET_BIT5
#define SET_ADCKS0()		ADCCR1|=DEF_SET_BIT4			// ADCCR1^4;
#define CLR_ADCKS0()		ADCCR1&=~DEF_SET_BIT4
#define SET_ADSPS3()		ADCCR1|=DEF_SET_BIT3			// ADCCR1^3;
#define CLR_ADSPS3()		ADCCR1&=~DEF_SET_BIT3
#define SET_ADSPS2()		ADCCR1|=DEF_SET_BIT2			// ADCCR1^2;
#define CLR_ADSPS2()		ADCCR1&=~DEF_SET_BIT2
#define SET_ADSPS1()		ADCCR1|=DEF_SET_BIT1			// ADCCR1^1;
#define CLR_ADSPS1()		ADCCR1&=~DEF_SET_BIT1
#define SET_ADSPS0()		ADCCR1|=DEF_SET_BIT0			// ADCCR1^0;
#define CLR_ADSPS0()		ADCCR1&=~DEF_SET_BIT0

/*  ADRIOS0, 0xFBH, SFR0  */
#define SET_AN7EN()			ADRIOS0|=DEF_SET_BIT7			// ADRIOS0^7;
#define CLR_AN7EN()			ADRIOS0&=~DEF_SET_BIT7
#define SET_AN6EN()	    	ADRIOS0|=DEF_SET_BIT6			// ADRIOS0^6;
#define CLR_AN6EN()	    	ADRIOS0&=~DEF_SET_BIT6
#define SET_AN5EN()			ADRIOS0|=DEF_SET_BIT5			// ADRIOS0^5;
#define CLR_AN5EN()			ADRIOS0&=~DEF_SET_BIT5
#define SET_AN4EN()			ADRIOS0|=DEF_SET_BIT4			// ADRIOS0^4;
#define CLR_AN4EN()			ADRIOS0&=~DEF_SET_BIT4
#define SET_AN3EN()			ADRIOS0|=DEF_SET_BIT3			// ADRIOS0^3;
#define CLR_AN3EN()			ADRIOS0&=~DEF_SET_BIT3
#define SET_AN2EN()			ADRIOS0|=DEF_SET_BIT2			// ADRIOS0^2;
#define CLR_AN2EN()			ADRIOS0&=~DEF_SET_BIT2
#define SET_AN1EN()			ADRIOS0|=DEF_SET_BIT1			// ADRIOS0^1;
#define CLR_AN1EN()			ADRIOS0&=~DEF_SET_BIT1
#define SET_AN0EN()			ADRIOS0|=DEF_SET_BIT0			// ADRIOS0^0;
#define CLR_AN0EN()			ADRIOS0&=~DEF_SET_BIT0
	
/*  ADRIOS1, 0xFCH, SFR0  */
//#define SET_()		ADRIOS1|=DEF_SET_BIT7					// ADRIOS0^7;
//#define CLR_()		ADRIOS1&=~DEF_SET_BIT7
//#define SET_()	    	ADRIOS1|=DEF_SET_BIT6					// ADRIOS0^6;
//#define CLR_()	    	ADRIOS1&=~DEF_SET_BIT6
#define SET_EX_VREF_EN()	ADRIOS1|=DEF_SET_BIT5			// ADRIOS0^5;
#define CLR_EX_VREF_EN()	ADRIOS1&=~DEF_SET_BIT5
//#define SET_()		ADRIOS1|=DEF_SET_BIT4					// ADRIOS0^4;
//#define CLR_()		ADRIOS1&=~DEF_SET_BIT4
//#define SET_()		ADRIOS1|=DEF_SET_BIT3					// ADRIOS0^3;
//#define CLR_()		ADRIOS1&=~DEF_SET_BIT3
//#define SET_()		ADRIOS1|=DEF_SET_BIT2					// ADRIOS0^2;
//#define CLR_()		ADRIOS1&=~DEF_SET_BIT2
#define SET_AN9EN()			ADRIOS1|=DEF_SET_BIT1			// ADRIOS0^1;
#define CLR_AN9EN()			ADRIOS1&=~DEF_SET_BIT1
#define SET_AN8EN()			ADRIOS1|=DEF_SET_BIT0			// ADRIOS0^0;
#define CLR_AN8EN()			ADRIOS1&=~DEF_SET_BIT0

/*  ADRL, 0xFDH, SFR0  */

/*  ADRH, 0xFEH, SFR0  */

/*  OSADJCR, 0xFFH, SFR0  */
#define SET_OSADJEN()		OSADJCR|=DEF_SET_BIT7			// OSADJCR^7;
#define CLR_OSADJEN()		OSADJCR&=~DEF_SET_BIT7
//#define SET_()	    	OSADJCR|=DEF_SET_BIT6					// OSADJCR^6;
//#define CLR_()	    	OSADJCR&=~DEF_SET_BIT6
//#define SET_()		OSADJCR|=DEF_SET_BIT5					// OSADJCR^5;
//#define CLR_()		OSADJCR&=~DEF_SET_BIT5
#define SET_OSADJTR4()		OSADJCR|=DEF_SET_BIT4			// OSADJCR^4;
#define CLR_OSADJTR4()		OSADJCR&=~DEF_SET_BIT4
#define SET_OSADJTR3()		OSADJCR|=DEF_SET_BIT3			// OSADJCR^3;
#define CLR_OSADJTR3()		OSADJCR&=~DEF_SET_BIT3
#define SET_OSADJTR2()		OSADJCR|=DEF_SET_BIT2			// OSADJCR^2;
#define CLR_OSADJTR2()		OSADJCR&=~DEF_SET_BIT2
#define SET_OSADJTR1()		OSADJCR|=DEF_SET_BIT1			// OSADJCR^1;
#define CLR_OSADJTR1()		OSADJCR&=~DEF_SET_BIT1
#define SET_OSADJTR0()		OSADJCR|=DEF_SET_BIT0			// OSADJCR^0;
#define CLR_OSADJTR0()		OSADJCR&=~DEF_SET_BIT0

/*  PWM145F, 0xFFH, SFR1  */




#endif

/*************************************end of file*******************************************/






