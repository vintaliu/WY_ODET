/**
  ******************************************************************************
  * @file    system_PT32X005.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   CMSIS Cortex-M3 Device Peripheral Access Layer System Header File.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup taurus01_system
  * @{
  */

/**
  * @brief Define to prevent recursive inclusion
  */
#ifndef __SYSTEM_PT32X005_H
#define __SYSTEM_PT32X005_H

#ifdef __cplusplus
extern "C" {
#endif


/** @addtogroup taurus01_System_Exported_types
  * @{
  */


//typedef volatile union
//{
//	volatile unsigned int Word;
//	struct
//	{
//		unsigned sys_clksel         : 2;
//		unsigned double_clk_sel     : 2;
//		unsigned hclk_div_sel       : 3; //[6:4]
//		unsigned 		    : 1;
//		unsigned pclk_div_sel       : 3; //[10:8]
//		unsigned 		        : 1;
//		unsigned eosc_detect_en     : 1; //12
//		unsigned eosc_fail_flag     : 1; //13
//		unsigned fd_detect_en       : 1; //14
//		unsigned fd_fail_flag       : 1; //15
//		unsigned wake_delay_ctrl    : 2; //[17:16]
//		unsigned 		        : 2;
//		unsigned sysclk_recover_sel : 1; //[20]
//		unsigned                    : 11;
//	} Bits;
//} sys_clk_con_STR;
//#define _sys_clk_con         		 *( sys_clk_con_STR *)(CMSDK_SYSCTRL_BASE   +  0x0C)
////#define _sys_clk_con         		 *( sys_clk_con_STR *)(CMSDK_ANAMIX->EOSC_CON2)
//#define sys_clk_con_sys_clksel		 (_sys_clk_con).Bits.sys_clksel
//#define sys_clk_con_double_clk_sel	 (_sys_clk_con).Bits.double_clk_sel
//#define sys_clk_con_hclk_div_sel	 (_sys_clk_con).Bits.hclk_div_sel
//#define sys_clk_con_pclk_div_sel	 (_sys_clk_con).Bits.pclk_div_sel



//typedef volatile union
//{
//	volatile unsigned int Word;
//	struct
//	{
//		unsigned osc_en		: 1;
//		unsigned osc_en_alc		: 1;
//		unsigned ex_20m_clk_en	: 1;
//		unsigned osc_div2_en	: 1;
//		unsigned osc_range		: 1;
//		unsigned 			: 3;
//		unsigned osc_deglitch_pw_sel: 2;
//		unsigned osc_deglitch_bp_en	: 1;
//		unsigned 			: 20;
//		unsigned rdy_crystal_clock	: 1;
//	} Bits;
//} osc_con1_STR;
//#define _osc_con1			*( osc_con1_STR *)(CMSDK_ANA_MIX_BASE	+ 0x08)
////#define _osc_con1			*( osc_con1_STR *)(CMSDK_ANAMIX->EOSC_CON1)
//#define osc_con1_osc_en			(_osc_con1).Bits.osc_en
//#define osc_con1_ex_20m_clk_en		(_osc_con1).Bits.ex_20m_clk_en
//#define osc_con1_rdy_crystal_clock	(_osc_con1).Bits.rdy_crystal_clock


//typedef volatile union
//{
//	volatile unsigned int Word;
//	struct
//	{
//		unsigned iosc_24m_en	: 1;
//		unsigned 			: 7;
//		unsigned iosc_24m_trim	: 8;
//		unsigned iosc_24m_trim_user_on : 8;
//		unsigned 			: 8;
//	} Bits;
//} iosc_24m_con_STR;
//#define _iosc_24m_con				*( iosc_24m_con_STR *)(CMSDK_ANA_MIX_BASE	+ 0x10)
////#define _iosc_24m_con				*( iosc_24m_con_STR *)(CMSDK_ANAMIX->IOSC_8M_CON)
//#define iosc_24m_con_iosc_24m_en  		(_iosc_24m_con).Bits.iosc_24m_en


//typedef volatile union
//{
//	volatile unsigned int Word;
//	struct
//	{
//		unsigned iosc_32k_en	: 1;
//		unsigned 			: 3;
//		unsigned iosc_32k_trim	: 4;
//		unsigned iosc_32k_trim_user_on : 8;
//		unsigned 			: 16;
//	} Bits;
//} iosc_32k_con_STR;
//#define _iosc_32k_con				*( iosc_32k_con_STR *)(CMSDK_ANA_MIX_BASE	+ 0x14)
////#define _iosc_32k_con				*( iosc_32k_con_STR *)(CMSDK_ANAMIX->IOSC_32K_CON)
//#define iosc_32k_con_iosc_32k_en  		(_iosc_32k_con).Bits.iosc_32k_en

//typedef volatile union
//{
//	volatile unsigned int Word;
//	struct
//	{
//		unsigned fd_en		: 1;
//		unsigned       : 31;
//	} Bits;
//} fd_con_STR;
//#define _fd_con				*( fd_con_STR *)(CMSDK_ANA_MIX_BASE	+ 0x18)
////#define _fd_con				*( fd_con_STR *)(CMSDK_ANAMIX->FD_CON)
//#define fd_con_fd_en			(_fd_con).Bits.fd_en


//typedef volatile union
//{
//	volatile unsigned int Word;
//	struct
//	{
//		unsigned mco_div_sel  : 3;
//		unsigned mco_resv     : 1;
//		unsigned mco_clksel   : 2;
//		unsigned              : 26;
//	} Bits;
//} mco_con_STR;
//#define _mco_div_con			*( mco_con_STR *)(CMSDK_ANA_MIX_BASE	+ 0x24)
////#define _mco_div_con			*( mco_con_STR *)(CMSDK_ANAMIX->MCO_DIV_CON)
//#define mco_con_mco_div_sel		(_mco_div_con).Bits.mco_div_sel
//#define mco_con_mco_clk_sel		(_mco_div_con).Bits.mco_clksel



extern uint32_t SystemCoreClock;          /*!< System Clock Frequency (Core Clock) */
#define EOSC_TIMEOUT 0xFFFFFFFF
#define  IOSC_RC24M_VALUE     24000000UL
#define  EOSC_CRYSTAL_VALUE   8000000UL
#define  I0SC_32K_VALUE       32768UL
/**
  * @}
  */


/** @addtogroup taurus01_System_Exported_Functions
  * @{
  */

extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);

extern void	Clock_SwitchTo_Int32KRC(void);
extern void	Clock_SwitchTo_Int24MRC(void);
extern void	Clock_SwitchTo_Int48MRC(void);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__SYSTEM_taurus01_H */


