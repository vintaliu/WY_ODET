/**************************************************************************//**
 * @file     system_CM3DS_MPS2.c
 * @brief    CMSIS Device System Source File for
 *           CM3DS_MPS2 M3 Device
 * @version  V3.02
 * @date     15. November 2013
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2011-2013 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup pangu01_system
  * @{
  */


#include "PT32X005.h"

void Wait_ClockReady(void)
{//not modify the delay param...
	volatile int delay;
	for (delay = 0; delay < 10000; delay++);
}

/** @addtogroup pangu01_System_Private_Defines
  * @{
  */
//<<< Use Configuration Wizard in Context Menu >>>

/*--------------------- Clock Configuration ----------------------------------
//
//    <h> High Speed Internal RC Oscillator Configuration (8M)
//      <q0>    IOSC_24M Enable
//                <i> Default = ENABLE
//    </h>
//
//    <h> Low Speed Internal Crystal Oscillator Configuration (32.768KHz)
//      <q2>    IOSC_32K Enable
//                <i> Default = DISABLE: 如果关闭内部32K时钟模块，CPU上电复位将多大约8毫秒
//    </h>
//    <h> Frequency Double Clock Configuration
//      <e3>    FD_CLK Enable
//                <i> Default  = DISABLE
//        <o4>  Frequency Double Clock Source
//                <0=> Internal RC_24M
//                <1=> Internal RC_32K
//                <i> Default Frequency Double clock source = Internal RC_24M
//                <i> Frequency Double Clock = Source clock * 2
//      </e>
//    </h>
//    <h> SystemCoreClock Configuration (HCLK)
//      <o5>    SystemCoreClock Source
//                <0=> Internal RC_24M
//                <2=> Frequency Double Clock
//                <3=> Internal RC_32K
//                <i> Default SystemCoreClock source = Internal RC_24M
//      <o6>    SystemCoreClock Source Divider
//                <0=> 0
//                <1=> 1
//                <2=> 2
//                <3=> 3
//                <4=> 4
//                <5=> 5
//                <6=> 6
//                <7=> 7
//                <8=> 8
//                <9=> 9
//                <10=> 10
//                <11=> 11
//                <12=> 12
//                <13=> 13
//                <14=> 14
//                <15=> 15
//                <16=> 16
//                <17=> 17
//                <18=> 18
//                <19=> 19
//                <20=> 20
//                <21=> 21
//                <22=> 22
//                <23=> 23
//                <24=> 24
//                <25=> 25
//                <26=> 26
//                <27=> 27
//                <28=> 28
//                <29=> 29
//                <30=> 30
//                <31=> 31
//                <i> Default SystemCoreClock source divider = 0
//      <o7>    PCLK Divider
//                <0=> 0
//                <1=> 1
//                <2=> 2
//                <3=> 3
//                <4=> 4
//                <5=> 5
//                <6=> 6
//                <7=> 7
//                <8=> 8
//                <9=> 9
//                <10=> 10
//                <11=> 11
//                <12=> 12
//                <13=> 13
//                <14=> 14
//                <15=> 15
//                <16=> 16
//                <17=> 17
//                <18=> 18
//                <19=> 19
//                <20=> 20
//                <21=> 21
//                <22=> 22
//                <23=> 23
//                <24=> 24
//                <25=> 25
//                <26=> 26
//                <27=> 27
//                <28=> 28
//                <29=> 29
//                <30=> 30
//                <31=> 31
//                <i> Default PCLK divider = 0
//    </h>
//
//    <h> FLASH Configuration
//      <o8>   Wait state
//                <0=> 0 WS
//                <1=> 1 WS
//                <i> 0 WS:  1 KHz <= HCLK <= 24 MHz
//                <i> 1 WS: 24 MHz <  HCLK <= 48 MHz
//    </h>
//
//    <h> MCO Output Configuration
//      <e9>    MCO_OUTPUT Enable
//                <i> Default = DISABLE ,PC4 output systemclock
//        <o10>    MCO Clock Source Select
//                <0=> Internal RC_24M
//                <1=> Frequency Double Clock
//                <2=> Internal RC_32K
//                <3=> System clock
//                <i> Default SystemCoreClock source = Internal RC_24M
//        <o11>   MCO Divider
//                <0=> 1
//                <1=> 2
//                <2=> 4
//                <3=> 8
//                <4=> 16
*/

#define IOSC_24M_ENABLE               (1)     /*!< 0: DISABLE,  1: ENABLE                                               */
#define EOSC_CLK_ENABLE               (0)     /*!< 0: DISABLE,  1: ENABLE                                               */
#define IOSC_32K_ENABLE               (0)     /*!< 0: DISABLE,  1: ENABLE                                               */
#define FD_CLK_ENABLE                 (0)     /*!< 0: DISABLE,  1: ENABLE                                               */
#define FD_CLK_SRC                    (0)     /*!< 0: IOCS_24M, 1: EOSC_CLK    2:IOSC_32K                               */
#define HCLK_SRC                      (0)     /*!< 0: IOCS_24M, 1: FD_CLK      2:IOSC_32K                  */
#define HCLK_DIV                      (0)     /*!< 0: DIV1,     1: DIV2,   2: DIV4,   3: DIV8,   4: DIV16,  5: DIV32    */
#define PCLK_DIV                      (0)     /*!< 0: DIV1,     1: DIV2,   2: DIV4,   3: DIV8,   4: DIV16,  5: DIV32    */
#define WAIT_STATE                    (1)     /*!< 0: 0 WS,     1: 1 WS                                   */
#define MCO_ENABLE                    (0)     /*!< 0: DISABLE,  1: ENABLE                                               */
#define MCO_SRCSEL                    (3)     
#define MCO_DIV                       (1)     /*!< 0: DIV1,     1: DIV2,   2: DIV4,   3: DIV8,   4: DIV16,  5: DIV32    */

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define __XTAL            (48000000UL)    /* Oscillator frequency             */

#define __SYSTEM_CLOCK    (IOSC_24M_VALUE )

#define  IOSC_24M_VALUE   24000000UL
#define  EOSC_CLK_VALUE   20000000UL
#define  I0SC_32K_VALUE   32768UL

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = __SYSTEM_CLOCK;/*!< System Clock Frequency (Core Clock)*/


/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
/**
 * Update SystemCoreClock variable
 *
 * @param  none
 * @return none
 *
 * @brief  Updates the SystemCoreClock with current core Clock
 *         retrieved from cpu registers.
 */
//void SystemCoreClockUpdate (void)
//{
//	UINT8 SystemCoreClockDiv;
//	UINT8 SystemCoreClockSrc;
//	UINT8 DoubleClockSrc;
//	SystemCoreClockDiv = sys_clk_con_hclk_div_sel;
//	SystemCoreClockSrc = sys_clk_con_sys_clksel;
//	DoubleClockSrc = sys_clk_con_double_clk_sel;
//	if (SystemCoreClockSrc == 0)
//	{
//		SystemCoreClock = IOSC_24M_VALUE >> SystemCoreClockDiv;
//	}
//	if (SystemCoreClockSrc == 1)
//	{
//		SystemCoreClock = EOSC_CLK_VALUE >> SystemCoreClockDiv;
//	}
//	if (SystemCoreClockSrc == 2)
//	{
//		if(DoubleClockSrc == 0)
//			SystemCoreClock = (IOSC_24M_VALUE * 2) >> SystemCoreClockDiv;
//		if(DoubleClockSrc == 1)
//			SystemCoreClock = (EOSC_CLK_VALUE * 2) >> SystemCoreClockDiv;
//		if(DoubleClockSrc == 2)
//			SystemCoreClock = (I0SC_32K_VALUE * 2) >> SystemCoreClockDiv;
//	}
//	if (SystemCoreClockSrc == 3)
//	{
//		SystemCoreClock = I0SC_32K_VALUE >> SystemCoreClockDiv;
//	}
//}

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
extern void Software_Delay(uint32_t timeBase10ms);
void SystemInit (void)
{
	CMSDK_ANAMIX->MCO_DIV_CON = 3;

#if  ((HCLK_SRC == 0)||(HCLK_SRC == 1))
	CMSDK_SYSCON->SYSCLK = 0;
	CMSDK_ANAMIX->FDCON &= 0xFFFFFFFE;			//FD enable
	#if (IOSC_24M_ENABLE == 0)
		#error "System clock source issue,Inter 24M RC has not been enabled(已经选择了内部24兆RC时钟为系统时钟源，但是内部24兆RC时钟未开启)"
	#else
	CMSDK_ANAMIX->IOSC_24M_CON |= 0x01;			//inter 24mHZ enable
	#endif
	#if (FD_CLK_ENABLE == 1)
		# warning "Double Frequency is Enable,will Increase Power(启用了倍频模块，但是未使用将增加IC耗电)"
		CMSDK_ANAMIX->FDCON |= 0x01;
	#endif
	#if (IOSC_32K_ENABLE == 1)
		# warning "Internal 32K RC clock is Enable,will Increase Power(内部32K时钟，如果未使用将增加IC耗电)"
		CMSDK_ANAMIX->IOSC_32K_CON |= 0x01;
	#else
		Wait_ClockReady();	//Wait_ClockReady() wait about 3ms ,then disable 32KHZ 
		CMSDK_ANAMIX->IOSC_32K_CON &= 0xFFFFFFFE;		//inter 32K enable
	#endif
#elif  (HCLK_SRC == 2)
	#if (FD_CLK_ENABLE == 0)
		#error "System clock source issue,Inter RC32K has not been enabled(已经选择了倍频时钟为系统时钟源，但是倍频模块未开启)"
	#else
		#if (FD_CLK_SRC == 0)
			#if (IOSC_32K_ENABLE == 0)
				# warning "Internal 32K RC clock always Enable (倍频模块使能，32K总是使能)"
			#endif
			#if (IOSC_24M_ENABLE == 0)
				#error "System clock source issue,Inter 24M RC has not been enabled(已经选择了内部24兆RC时钟为系统时钟源，但是内部24兆RC时钟未开启)"
			#else
				CMSDK_ANAMIX->IOSC_24M_CON |= 0x01;		//inter 24mHZ enable
				CMSDK_SYSCON->SYSCLK = 0;			//FD clocl source == 24mhz
				CMSDK_ANAMIX->FDCON = 0x01;			//FD enable  -- 
				CMSDK_SYSCON->SYSCLK |= 0x02;
			#endif
		#elif (FD_CLK_SRC == 1)
			#if (IOSC_32K_ENABLE == 0)
				#error "System clock source issue,Inter 32K RC has not been enabled(已经选择了内部32K RC时钟为系统时钟源，但是内部32K RC时钟未开启)"
			#else
				CMSDK_SYSCON->SYSCLK |= 0x04;
				CMSDK_ANAMIX->IOSC_32K_CON |= 0x01;		//inter 24mHZ enable
				CMSDK_ANAMIX->FDCON = 0x01;			//FD enable  -- 
				CMSDK_SYSCON->SYSCLK |= 0x02;
			#endif
			#if (IOSC_24M_ENABLE == 1)
				# warning "Internal 24M RC clock is Enable,will Increase Power(内部24兆时钟，如果未使用将增加IC耗电)"
				CMSDK_ANAMIX->IOSC_24M_CON |= 0x01;
			#else
				CMSDK_ANAMIX->IOSC_24M_CON = 0;		//inter 24mHZ disable
			#endif
		#endif
	#endif
#elif  (HCLK_SRC == 3)
	#if (IOSC_32K_ENABLE == 0)
		#error "System clock source issue,Double Frequency has not been enabled(请先使能内部32K RC模块，在指定CPU时钟源为内部32K时钟)"
	#else
		Wait_ClockReady();	//Wait_ClockReady() wait about 3ms ,then disable 32KHZ 
		CMSDK_ANAMIX->IOSC_32K_CON |= 0x01;		//inter 32K
	#endif
	#if (FD_CLK_ENABLE == 1)
		# warning "Double Frequency is Enable,will Increase Power(启用了倍频模块，但是未使用将增加IC耗电)"
		CMSDK_ANAMIX->FDCON |= 0x01;
	#else
		CMSDK_ANAMIX->FDCON = 0x00;
	#endif
	#if (IOSC_24M_ENABLE == 1)
		# warning "Internal 24M clock is Enable,will Increase Power(内部24M，将增加IC耗电)"
		CMSDK_ANAMIX->IOSC_24M_CON |= 0x01;
	#else
		CMSDK_SYSCON->SYSCLK |= 0x03;
		CMSDK_ANAMIX->IOSC_24M_CON = 0;		//inter 24mHZ disable
	#endif
#else
	#error "System clock source issue: System clock has not been enabled"
#endif
	
	CMSDK_FLASH ->ACR= ((FLASH_MAIN_FKEY) | (WAIT_STATE << 5)) ; //wait state

	CMSDK_SYSCON->SYSCLK |= (HCLK_DIV << 4);
	CMSDK_SYSCON->SYSCLK |= (PCLK_DIV << 24);

#if (MCO_ENABLE == 1)
	CMSDK_PC->ALTFUNCSET0 |= (1 << 16); //PC4
	CMSDK_ANAMIX->MCO_DIV_CON = (MCO_DIV | (MCO_SRCSEL << 4) );

	#if (MCO_SRCSEL == 0) & (IOSC_24M_ENABLE == 0)
		# warning "MCO has no Clock Output(IOSC_24M is Not Enable)(内部RC24兆时钟未开启，MCO无波形输出)"
	#elif (MCO_SRCSEL == 1) & (FD_CLK_ENABLE == 0)
		# warning "MCO has no Clock Output(Double Frequency is Not Enable)(倍频器未开启，MCO无波形输出)"
	#elif (MCO_SRCSEL == 2) & (IOSC_32K_ENABLE == 0)
		# warning "MCO has no Clock Output(IOSC_32K is Not Enable)(内部RC32K时钟未开启，MCO无波形输出)"
	#endif
#endif

}



void	Clock_SwitchTo_Int32KRC(void)
{
	CMSDK_ANAMIX->IOSC_32K_CON |= 0x01;		//inter 32K
	CMSDK_SYSCON->SYSCLK |= 0x03;
	CMSDK_ANAMIX->FDCON = 0x00;
	CMSDK_ANAMIX->IOSC_24M_CON = 0;			//inter 24mHZ disable
}


void	Clock_SwitchTo_Int24MRC(void)
{
	CMSDK_ANAMIX->IOSC_24M_CON = 1;			//inter 24mHZ disable
	CMSDK_SYSCON->SYSCLK |= 0x04;
	CMSDK_ANAMIX->FDCON = 0x00;
	CMSDK_SYSCON->SYSCLK &= 0xFFFFFFFC;
	CMSDK_ANAMIX->IOSC_32K_CON = 0x00;		//inter 32K
}

void	Clock_SwitchTo_Int48MRC(void)
{
	CMSDK_ANAMIX->IOSC_24M_CON = 1;			//inter 24mHZ disable
	CMSDK_SYSCON->SYSCLK |= 0x04;
	CMSDK_ANAMIX->FDCON = 0x01;
	CMSDK_SYSCON->SYSCLK &= 0xFFFFFFFC;
	CMSDK_SYSCON->SYSCLK |= 0x02;
	CMSDK_ANAMIX->IOSC_32K_CON = 0x00;		//inter 32K
}


//<<< end of configuration section >>>

/**
  * @}
  */


/**
  * @}
  */
