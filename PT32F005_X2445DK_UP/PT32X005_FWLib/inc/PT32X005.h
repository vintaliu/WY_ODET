/******************************************************************************
  * @file    PT32X005.h
  * @author  应用开发团队
  * @version V1.0.0
  * @date    2020/10/19
  * @brief   
  *          
  ******************************************************************************
  * @attention
  *
  *
  *****************************************************************************/

    
#ifndef PT32X005_H
#define PT32X005_H

#ifdef __cplusplus
 extern "C" {
#endif

#if defined (__CC_ARM)
#pragma anon_unions
#endif	



#define __CM0_REV		0 /*!< Core Revision r0p0	*/
#define __MPU_PRESENT		0 /*!< taurus01 do not provide MPU	*/
#define __NVIC_PRIO_BITS	2 /*!< taurus01 uses 2 Bits for the Priority Levels  */
#define __Vendor_SysTickConfig	0 /*!< Set to 1 if different SysTick Config is used  */

/**
 * @brief taurus01 Interrupt Number Definition
 */	
typedef enum IRQn
{
	/******  Cortex-M0 Processor Exceptions Numbers ***************************************************/
	NonMaskableInt_IRQn	= -14,	/*!< 2 Non Maskable Interrupt	*/
	HardFault_IRQn		= -13,	/*!< 3 Cortex-M0 Hard Fault Interrupt	*/
	SVCall_IRQn		= -5,	/*!< 11 Cortex-M0 SV Call Interrupt	*/
	PendSV_IRQn		= -2,	/*!< 14 Cortex-M0 Pend SV Interrupt	*/
	SysTick_IRQn		= -1,	/*!< 15 Cortex-M0 System Tick Interrupt	*/

	/******  Taurus01 Specific Interrupt Numbers *******************************************************/
	SWDG_IRQn		=  0 ,      /* WDG        Interrupt  */
	CLK_FAIL_IRQn		=  1 ,      /* CLK_FAIL    Interrupt 	*/
	FLASH_IRQn		=  3 ,      /* FLASH       Interrupt 	*/
	PA_IRQn			=  5 ,      /* PA          Interrupt 	*/
	PB_IRQn			=  6 ,      /* PB          Interrupt 	*/
	PC_IRQn			=  7,	/* PC          Interrupt 	*/
	PD_IRQn			=  8,	/* PD          Interrupt 	*/
	
	Comparator0_IRQn	=  9 ,      /* Comparator0 Interrupt 	*/
	Comparator1_IRQn	=  10,      /* Comparator1 Interrupt 	*/
	DAC_IRQn		=  11,      /* DAC         Interrupt 	*/
	ADC_IRQn		=  12,      /* ADC         Interrupt 	*/
	TIMER1_IRQn             =  13,      /* TIMER1      Interrupt 	*/ 
	TIMER0_IRQn             =  14,      /* TIMER0      Interrupt 	*/
	TIMER4_IRQn             =  15,      /* TIMER4      Interrupt 	*/
	TIMER3_IRQn             =  16,      /* TIMER3      Interrupt 	*/
	TIMER2_IRQn             =  17,      /* TIMER2      Interrupt 	*/
	PVD_IRQn		=  20,      /* PVD         Interrupt 	*/
	LVD_IRQn		=  21,      /* LVD         Interrupt 	*/
	I2C0_IRQn		=  23,      /* I2C0        Interrupt 	*/
	SPI0_IRQn		=  25,      /* SPI0        Interrupt 	*/
//	SPI1_IRQn		=  26,      /* SPI1        Interrupt 	*/
	UART0_IRQn		=  27,      /* UART0     Interrupt 	*/
	UART1_IRQn		=  28,      /* UART1       Interrupt 	*/
//	RTC_IRQn		=  29,      /* RTC         Interrupt 	*/
} IRQn_Type;



#include "core_cm0.h"
#include "PT32X005_Type.h"
#include "system_PT32X005.h"
#include <stdint.h>



/** @addtogroup Exported_types
  * @{
  */  
typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus, RemapStatus, ProtectStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

#define wb(addr, value)     (*((uint8_t  volatile *) (addr)) = value)
#define rb(addr)            (*((uint8_t  volatile *) (addr)))
#define whw(addr, value)    (*((uint16_t volatile *) (addr)) = value)
#define rhw(addr)           (*((uint16_t volatile *) (addr)))
#define ww(addr, value)     (*((uint32_t volatile *) (addr)) = value)
#define rw(addr)            (*((uint32_t volatile *) (addr)))

#define ResetBit_BB(Addr, BitNumber) (rw(Addr) &= ~(1UL << BitNumber))
#define SetBit_BB(Addr, BitNumber)   (rw(Addr) |= (1UL << BitNumber))
#define GetBit_BB(Addr, BitNumber)   ((rw(Addr) >> BitNumber) & 1UL)

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;
typedef enum {FALSE = 0, TRUE = !FALSE} bool;
/**
  * @}
  */


/** @addtogroup Peripheral_memory_map
  * @{
  */
typedef struct
{
	__IO uint32_t  REMAP;			/*!< Offset: 0x000 Remap Control Register (R/W) */
		uint32_t  	RESERVED0;		/*!< Offset: 0x004 RESERVED0*/
	__IO uint32_t  LOCKUP_RESET;	/*!< Offset: 0x008 Processor lock reset control register(R/W) */
	__IO uint32_t  SYSCLK;	   		/*!< Offset: 0x00C System Clock Selection Register  (R/W) */ 
	__IO uint32_t  RSTINFO;   		/*!< Offset: 0x010 Reset Information Register (R/W) */
	__IO uint32_t  RETRIMING;		/*!< Offset: 0x014 Reset Information Register (R/W) */
	__IO uint32_t  RSTENCTRL;		/*!< Offset: 0x018 RSTEN Ccontrol Register (R/W) */
		uint32_t  RESERVED1;		/*!< Offset: 0x01C RESERVED1*/
	__IO uint32_t  CUSTOMER_ID_INFO;	/*!< Offset: 0x020 Customer ID information Register (R/W) */
	__IO uint32_t  FLASH_PROTECT;  	/*!< Offset: 0x024 FLash Protect Status Register (R/W) */
		uint32_t  RESERVED2;		/*!< Offset: 0x028 RESERVED2*/
	__IO uint32_t  AGENT_ID_INFO;    	/*!< Offset: 0x02C UID information Register (R/W) */
	__IO uint32_t  UID_INFO1;     		/*!< Offset: 0x030 UID information Register (R/W) */
	__IO uint32_t  UID_INFO2;    		/*!< Offset: 0x034 UID information Register (R/W) */
	__IO uint32_t  UID_INFO3;   		/*!< Offset: 0x038 UID information Register (R/W) */
	__IO uint32_t  CID_INFO;       		/*!< Offset: 0x03C CID information Register (R/W) */
		uint32_t  RESERVED4[996];
	__IO uint32_t  PID4;                 /*!< Offset: 0xFD0 PID4 Register (R/W) */
	__IO uint32_t  PID5;                 /*!< Offset: 0xFD4 PID5 Register (R/W) */
	__IO uint32_t  PID6;                 /*!< Offset: 0xFD8 PID6 Register (R/W) */
	__IO uint32_t  PID7;                 /*!< Offset: 0xFDC PID7 Register (R/W) */
	__IO uint32_t  PID0;                 /*!< Offset: 0xFE0 PID0 Register (R/W) */
	__IO uint32_t  PID1;                 /*!< Offset: 0xFE4 PID1 Register (R/W) */
	__IO uint32_t  PID2;                 /*!< Offset: 0xFE8 PID2 Register (R/W) */
	__IO uint32_t  PID3;                 /*!< Offset: 0xFEC PID3 Register (R/W) */
	__IO uint32_t  CID0;                 /*!< Offset: 0xFF0 CID0 Register (R/W) */
	__IO uint32_t  CID1;                 /*!< Offset: 0xFF4 CID1 Register (R/W) */
	__IO uint32_t  CID2;                 /*!< Offset: 0xFF8 CID2 Register (R/W) */
	__IO uint32_t  CID3;                 /*!< Offset: 0xFFC CID3 Register (R/W) */
} CMSDK_SYSCON_TypeDef;

/** 
  * @brief General Purpose IO  
  */
typedef struct
{
	__IO uint32_t  DATA;        		/*!< Offset: 0x000 DATA Register (R/W) */
	__IO uint32_t  DATAOUT_NOUSE;         	/*!< Offset: 0x004 Data Output Latch Register (R/W) */
	__IO uint32_t  OUTENABLESET;	/*!< Offset: 0x008 Output Enable Set Register  (R/W) */
	__IO uint32_t  OUTENABLECLR; 	/*!< Offset: 0x00C Output Enable Clear Register  (R/W) */
	__IO uint32_t  ALTFUNCSET0;	/*!< Offset: 0x010 Alternate Function Set Register0  (R/W) */
		   uint32_t  RESERVED1;   	  /*!<  0x014 RESERVED1 */
	__IO uint32_t  ALTFUNCCLR;    	/*!< Offset: 0x018 Alternate Function Clear Register0  (R/W) */
	__IO uint32_t  INTENSET;     	/*!< Offset: 0x01C Interrupt Enable Set Register  (R/W) */
	__IO uint32_t  INTENCLR;    	/*!< Offset: 0x020 Interrupt Enable Clear Register  (R/W) */
	__IO uint32_t  INTTYPESET;      	/*!< Offset: 0x024 Interrupt Type Set Register0  (R/W) */
	__IO uint32_t  INTTYPECLR;      	/*!< Offset: 0x028 Interrupt Type Clear Register0  (R/W) */
	__IO uint32_t  INTTYPESET1;	/*!< Offset: 0x02C Interrupt Type Set Register1  (R/W) */
	__IO uint32_t  INTTYPECLR1;	/*!< Offset: 0x030 Interrupt Type Clear Register1  (R/W) */
	__IO uint32_t  INTPOLSET;       	/*!< Offset: 0x034 Interrupt Polarity Set Register  (R/W) */
	__IO uint32_t  INTPOLCLR;   	/*!< Offset: 0x038 Interrupt Polarity Clear Register  (R/W) */
	union
	{
		__I uint32_t  INTSTATUS;	/*!< Offset: 0x03C Interrupt Status Register (R/ ) */
		__O uint32_t  INTCLEAR;	/*!< Offset: 0x03C Interrupt Clear Register ( /W) */
	};
	__IO uint32_t  PULLUPSET;		/*!< Offset: 0x040 Pull Up Set Register  (R/W) */
	__IO uint32_t  PULLUPCLR;		/*!< Offset: 0x044 Pull Up Clear Register  (R/W) */
	__IO uint32_t  PULLDOWNSET;	/*!< Offset: 0x048 Pull Down Set Register  (R/W) */
	__IO uint32_t  PULLDOWNCLR;	/*!< Offset: 0x04C Pull Down Clear Register  (R/W) */
	__IO uint32_t  ODRSET;		/*!< Offset: 0x050 Open Drain Set Register  (R/W) */
	__IO uint32_t  ODRCLR;		/*!< Offset: 0x054 Open Drain Clear Register  (R/W) */
	__IO uint32_t  ANASET;		/*!< Offset: 0x058 Analog function Set Register  (R/W) */
	__IO uint32_t  ANACLR;		/*!< Offset: 0x05C Analog function Clear Register  (R/W) */
	__IO uint32_t  DRSET;			/*!< Offset: 0x060 Drive current halved Set Register(R/W) */
	__IO uint32_t  DRCLR;			/*!< Offset: 0x064 Drive current halved Clear Register (R/W) */
	__IO uint32_t  SRSET;			/*!< Offset: 0x068 Slow Rate set Register (R/W) */
	__IO uint32_t  SRCLR;			/*!< Offset: 0x06C Slow Rate Clear Register (R/W) */
	__IO uint32_t  CSSET;			/*!< Offset: 0x070 Sthmidt set Register (R/W) */
	__IO uint32_t  CSCLR;			/*!< Offset: 0x074 Sthmidt Clear Register (R/W) */
	
		uint32_t  RESERVED2[226];
	__IO uint32_t  LB_MASKED[256];	/*!< Offset: 0x400 - 0x7FC Lower byte Masked Access Register (R/W) */
	__IO uint32_t  UB_MASKED[256];	/*!< Offset: 0x800 - 0xBFC Upper byte Masked Access Register (R/W) */
} CMSDK_GPIO_TypeDef;

/** 
  * @brief PWM
  */
typedef struct	
{
	__IO uint32_t ISR;            			 /*!< offset: 0x000 Interrupt status Register   (R/W)         */
	__IO uint32_t TCR;            			 /*!< offset: 0x004 Control Register            (R/W)         */
	__IO uint32_t INT_RPT;            	 /*!< offset: 0x008 Interrupt Repeat Timers     (R/W)         */
	__IO uint32_t INT_RPTC;            	 /*!< offset: 0x00C Interrupt Repeat Timers CNT (R/W)         */
	__IO uint32_t PR;            		  	 /*!< offset: 0x010 Prescale Register           (R/W)         */
	__IO uint32_t PC;             			 /*!< offset: 0x014 Prescale Counter Register   (R/W)         */
	__IO uint32_t TC;             			 /*!< offset: 0x018 Timer Counter Register      (R/W)         */
	__IO uint32_t MCR;            			 /*!< offset: 0x01C Match Control Register      (R/W)         */
	__IO uint32_t MR0;            			 /*!< offset: 0x020 Match Value Register0       (R/W)         */
	__IO uint32_t MR1;            			 /*!< offset: 0x024 Match Value Register1       (R/W)         */	
	__IO uint32_t MR2;            			 /*!< offset: 0x028 Match Value Register2       (R/W)         */
	__IO uint32_t MR3;            		   /*!< offset: 0x02C Match Value Register3       (R/W)         */
	__IO uint32_t MR4;             			 /*!< offset: 0x030 Match Value Register4       (R/W)         */
	__IO uint32_t CCR;             			 /*!< offset: 0x034 Capture Control Register    (R/W)         */
	__IO uint32_t CR1;            			 /*!< offset: 0x038 Capture Value Register1     (R/W)         */
	__IO uint32_t CR2;            			 /*!< offset: 0x03C Capture Value Register2     (R/W)         */
	__IO uint32_t CR3;            			 /*!< offset: 0x040 Capture Value Register3     (R/W)         */
	__IO uint32_t CR4;            			 /*!< offset: 0x044 Capture Value Register4     (R/W)         */	
	__IO uint32_t CMR;            			 /*!< offset: 0x048 Compare Output Register     (R/W)         */
	__IO uint32_t DT;             		   /*!< offset: 0x04C Death Time Register         (R/W)         */	
} CMSDK_PWM_TypeDef;

/** 
  * @brief TIM
  */
typedef struct
{
	__IO uint32_t ISR;            			 /*!< offset: 0x000 Interrupt status Register   (R/W)         */
	__IO uint32_t TCR;            			 /*!< offset: 0x004 Control Register            (R/W)         */
	__IO uint32_t PR;            		  	 /*!< offset: 0x008 Prescale Register           (R/W)         */
	__IO uint32_t PC;             			 /*!< offset: 0x00C Prescale Counter Register   (R/W)         */
	__IO uint32_t TC;             			 /*!< offset: 0x010 Timer Counter Register      (R/W)         */
	__IO uint32_t MCR;            			 /*!< offset: 0x014 Match Control Register      (R/W)         */
	__IO uint32_t MR0;            			 /*!< offset: 0x018 Match Value Register0       (R/W)         */
} CMSDK_TIM_TypeDef;

/** 
  * @brief Independent WATCHDOG
  */
typedef struct
{
	__IO uint32_t RLR;   	/*!< IWDG Reload register,    Address offset: 0x00 */
	__I  uint32_t COUNT; 	/*!< IWDG COUNT register,     Address offset: 0x04 */
	__IO uint32_t CR;    	/*!< IWDG Control register,   Address offset: 0x08 */
	__IO uint32_t FWDG;  	/*!< IWDG Interrupt clear,    Address offset: 0x0C */
	__IO uint32_t RIS;   	/*!< IWDG Window register,    Address offset: 0x10 */
	__IO uint32_t EIS;   	/*!< IWDG Window register,    Address offset: 0x14 */
	uint32_t RESERVED0[250];        
	__IO uint32_t KR;    	/*!< IWDG Window register,    Address offset: 0x400 */		
} CMSDK_WDG_TypeDef;

/** 
  * @brief UART 
  */
typedef struct	
{
	__IO uint32_t BR;     	/*!< Offset: 0x000 Buffer Register 		(R/W) */
	__IO uint32_t CR;		/*!< Offset: 0x004 Control Register		(R/W) */
	__IO uint32_t BRR;     	/*!< Offset: 0x008 Baud Rate Register		(R/W) */
	__IO uint32_t IER;    	/*!< Offset: 0x00C Interrupt Enable Register	(R/W) */
	__IO uint32_t SR;     	/*!< Offset: 0x010 Status Register		(R/W) */
	__IO uint32_t GTR;  	/*!< Offset: 0x014 Guard Time Register  		(R/W) */
	__IO uint32_t TOR;   	/*!< Offset: 0x018 Time Out Register  		(R/W) */
	__IO uint32_t TRR; 	/*!< Offset: 0x01C TX Buffer Reset Register  	(WO)  */
	__IO uint32_t RRR;   	/*!< Offset: 0x020 RX Buffer Reset Register  	(WO)  */
	uint32_t RESERVED0;		
	__IO uint32_t IR_CR;			/*!< Offset: 0x028 Infra-red Control Register  	(R/W) */	
	__IO uint32_t IRTX_PWMC;		/*!< Offset: 0x02C IR TX PWM Control Register  	(R/W) */
} CMSDK_UART_TypeDef;

/** 
  * @brief Serial Peripheral Interface
  */
typedef struct		
{
	__IO uint32_t CR0;       /*!< SPI Control register 1                               Address offset: 0x00 */
	__IO uint32_t CR1;       /*!< SPI Control register 2,                              Address offset: 0x04 */
	__IO uint32_t DR;        	/*!< SPI data register,                                   Address offset: 0x08 */
	__IO uint32_t SR;        	/*!< SPI Status register,                                 Address offset: 0x0C */	
	__IO uint32_t CPSR; 	/*!< SPI Clock prescaler register                         Address offset: 0x10 */
	__IO uint32_t IE;        	/*!< SPI Interrupt ENABLE register                        Address offset: 0x14 */	
	__IO uint32_t RIS;       	/*!< SPI Interrupt Raw Int Status register                Address offset: 0x18 */	
	__IO uint32_t EIS;       	/*!< SPI Interrupt Int Status register                    Address offset: 0x1C */		
	__IO uint32_t ICR;       	/*!< SPI Interrupt Clear Register register                Address offset: 0x20 */	
	uint32_t RESERVED0;	/*0X24*/
	__IO uint32_t CSCR;      /*!< SPI Chip-Select Control register                     Address offset: 0x28*/	
} CMSDK_SPI_TypeDef;

/** 
  * @brief Inter-integrated Circuit Interface
  */
typedef struct		//同pangu,但只有IIC0，无IIC1
{
	__IO uint32_t CSR;		/*!< I2C Control Set register ,         Address offset: 0x00 */
	__I  uint32_t SR; 		/*!< I2C status register,               Address offset: 0x04 */
	__IO uint32_t DR;  	/*!< I2C data register,                 Address offset: 0x08 */	
	__IO uint32_t OAR;	/*!< I2C Own address register,          Address offset: 0x0C */
	__IO uint32_t RESERVED0; /*!< RESERVED0,      Address offset: 0x10*/	
	__IO uint32_t RESERVED1; /*!< RESERVED0,      Address offset: 0x14 */	
	__IO uint32_t CCR; 		/*!< I2C Control Reset register 2,      Address offset: 0x18 */	
} CMSDK_I2C_TypeDef;

/** 
  * @brief  Analog to Digital Converter  	  
  */
typedef struct
{
	__IO uint32_t CR;       	/*!< Offset: 0x000 ADC Control register */
	__IO uint32_t SR;    	/*!< Offset: 0x004 ADC Status Register  (R/W) */
	__IO uint32_t DR;    	/*!< Offset: 0x008 ADC data register (R/W) */
	__IO uint32_t SAMPLE;	/*!< Offset: 0x00C ADC SAMPLE TIMER register (R/W) */
} CMSDK_ADC_TypeDef;	




/**
 * @brief Cyclic Redundancy Check
 */
typedef struct
{                                                                              
	__IO uint32_t CR;               /*!< 0x000 CRC Control Register                                           */
	__IO uint32_t SDR;              /*!< 0x004 CRC Seed Register                                              */	
	__IO uint32_t POLY;             /*!< 0x008 CRC polynomial Register                                        */
	__IO uint32_t DIN;              /*!< 0x00C CRC Data INPUT Register                                        */
	__IO uint32_t DOUT;             /*!< 0x010 CRC Data INPUT Register                                        */
} CMSDK_CRC_TypeDef;

/** 
  * @brief Analog MIX Function  
  */
typedef struct 
{
	__IO uint32_t VDD_LVD_CON;	/*!< Offset: 0x000 VDD Low Voltage Detect Control Register (R/W) */
	__IO uint32_t LDO_LVD_CON;	/*!< Offset: 0x004 LDO Low Voltage Detect Control Register (R/W) */
    	 uint32_t RESERVED0[2];
	__IO uint32_t IOSC_24M_CON;	/*!< Offset: 0x010 24MHZ IOSC Control Register (R/W) */
	__IO uint32_t IOSC_32K_CON;	/*!< Offset: 0x014 32KHZ IOSC Control Register (R/W) */
	__IO uint32_t	FDCON;        /*!< Offset: 0x018 Frequency doubling module control register (R/W)   */
	__IO uint32_t DEGLITCH_CON;	/*!< Offset: 0x01C Deglitch Control Register (R/W) */
	     uint32_t RESERVED1;
	__IO uint32_t MCO_DIV_CON;	/*!< Offset: 0x024 MCO Frequency Division Control Register (R/W) */
}  CMSDK_ANAMIX_TypeDef;

/** 
  * @brief FLASH  
  */
typedef struct
{
	__IO uint32_t ACR; 	/*!< Offset: 0x000 Flash Command Register (R/W) */
	__IO uint32_t ISR;   	/*!< Offset: 0x004 Flash Interrupt status Register (R/W) */
	__IO uint32_t IER;  	/*!< Offset: 0x008 Flash Interrupt Enable Register  (R/W) */
	__IO uint32_t AR;    	/*!< Offset: 0x00C Flash Address Register (R/W) */
	__IO uint32_t DR;  	/*!< Offset: 0x010 Flash Programming Data Register0  (R/W) */
	uint32_t  RESERVED0[5];
	__IO uint32_t DIV;  	/*!< Offset: 0x028 Flash Erase Clock Division Register  (R/W) */
} CMSDK_FLASH_TypeDef;	




/*Peripheral and SRAM base address */
#define CMSDK_SRAM_BASE             (0x20000000)     /*!< (SRAM      ) Base Address */
#define CMSDK_FLASH_BASE            (0x00000000) 

#define CMSDK_APB_BASE              (0x40000000)
#define CMSDK_AHB_BASE              (0x48000000)


/* AHB peripherals */
#define CMSDK_PA_BASE               (CMSDK_AHB_BASE + 0x00000000)
#define CMSDK_PB_BASE               (CMSDK_AHB_BASE + 0x00001000)
#define CMSDK_PC_BASE               (CMSDK_AHB_BASE + 0x00002000)
#define CMSDK_PD_BASE               (CMSDK_AHB_BASE + 0x00003000)

/* APB peripherals */
#define CMSDK_FLASH0_CTRL_BASE	(CMSDK_APB_BASE + 0x00000000)
//#define CMSDK_COMPARATOR0_BASE	(CMSDK_APB_BASE + 0x00000800)
//#define CMSDK_COMPARATOR1_BASE	(CMSDK_APB_BASE + 0x00000C00)
#define CMSDK_TIMER2_BASE     	(CMSDK_APB_BASE + 0x00001000)
#define CMSDK_TIMER3_BASE     	(CMSDK_APB_BASE + 0x00001400)
#define CMSDK_ANA_MIX_BASE     	(CMSDK_APB_BASE + 0x00001800)
#define CMSDK_TIMER4_BASE     	(CMSDK_APB_BASE + 0x00001C00)
//#define CMSDK_SWATCHDOG_BASE	(CMSDK_APB_BASE + 0x00002000)
#define CMSDK_IWATCHDOG_BASE	(CMSDK_APB_BASE + 0x00003000)
#define CMSDK_SPI0_BASE             	(CMSDK_APB_BASE + 0x00013000)
#define CMSDK_CRC_BASE              	(CMSDK_APB_BASE + 0x00003C00)
#define CMSDK_UART0_BASE    	(CMSDK_APB_BASE + 0x00004400)
#define CMSDK_I2C0_BASE             	(CMSDK_APB_BASE + 0x00005400)
#define CMSDK_SPI1_BASE             	(CMSDK_APB_BASE + 0x00003800)
#define CMSDK_ADC_BASE           	(CMSDK_APB_BASE + 0x00012400)
//#define CMSDK_TIMER0_BASE		(CMSDK_APB_BASE + 0x00012800)
#define CMSDK_TIMER1_BASE  	(CMSDK_APB_BASE + 0x00012C00)
#define CMSDK_UART1_BASE   		(CMSDK_APB_BASE + 0x00013800)
//#define CMSDK_RTC_BASE   		(CMSDK_APB_BASE + 0x00013C00)
//#define CMSDK_DAC_BASE			(CMSDK_APB_BASE + 0x00014000)

//#define CMSDK_LCD_BASE			(CMSDK_APB_BASE + 0x00015000)
//#define CMSDK_LCD_DATA_BASE	(CMSDK_APB_BASE + 0x00015400)

#define CMSDK_SYSCTRL_BASE		(CMSDK_APB_BASE + 0x0001F000)

//#define CMSDK_BEEP_BASE             (CMSDK_APB_BASE + 0x00002800)

/**
  * @}
  */


/** @addtogroup Peripheral_declaration
  * @{
  */  
	
#define CMSDK_SYSCON	((CMSDK_SYSCON_TypeDef   *) CMSDK_SYSCTRL_BASE )
#define CMSDK_ANAMIX	((CMSDK_ANAMIX_TypeDef   *) CMSDK_ANA_MIX_BASE )

#define CMSDK_PA		((CMSDK_GPIO_TypeDef     *) CMSDK_PA_BASE )
#define CMSDK_PB		((CMSDK_GPIO_TypeDef     *) CMSDK_PB_BASE )
#define CMSDK_PC		((CMSDK_GPIO_TypeDef     *) CMSDK_PC_BASE )
#define CMSDK_PD		((CMSDK_GPIO_TypeDef     *) CMSDK_PD_BASE )

#define UART0                   ((CMSDK_UART_TypeDef     *) CMSDK_UART0_BASE )
#define UART1                   ((CMSDK_UART_TypeDef     *) CMSDK_UART1_BASE )

#define CMSDK_FLASH	((CMSDK_FLASH_TypeDef    *) CMSDK_FLASH0_CTRL_BASE )
#define ADC              		((CMSDK_ADC_TypeDef      *) CMSDK_ADC_BASE )

#define I2C0				((CMSDK_I2C_TypeDef      *) CMSDK_I2C0_BASE )

#define SPI0				((CMSDK_SPI_TypeDef      *) CMSDK_SPI0_BASE )
#define SPI1				((CMSDK_SPI_TypeDef      *) CMSDK_SPI1_BASE )

//#define CMPOPA0			((CMSDK_CMPOPA_TypeDef   *) CMSDK_COMPARATOR0_BASE )
//#define CMPOPA1			((CMSDK_CMPOPA_TypeDef   *) CMSDK_COMPARATOR1_BASE )

//#define PWM0			((CMSDK_PWM_TypeDef      *) CMSDK_TIMER0_BASE )
#define PWM1			((CMSDK_PWM_TypeDef      *) CMSDK_TIMER1_BASE )

#define TIM2				((CMSDK_TIM_TypeDef      *) CMSDK_TIMER2_BASE )
#define TIM3				((CMSDK_TIM_TypeDef      *) CMSDK_TIMER3_BASE )
#define TIM4				((CMSDK_TIM_TypeDef      *) CMSDK_TIMER4_BASE )

#define CRC				((CMSDK_CRC_TypeDef      *) CMSDK_CRC_BASE )
#define IWDG			((CMSDK_WDG_TypeDef      *) CMSDK_IWATCHDOG_BASE )
//#define SWDG			((CMSDK_WDG_TypeDef      *) CMSDK_SWATCHDOG_BASE )

//#define RTC				((CMSDK_RTC_TypeDef      *) CMSDK_RTC_BASE )
//#define DAC				((CMSDK_DAC_TypeDef      *) CMSDK_DAC_BASE )
//#define LCD				((CMSDK_LCD_TypeDef      *) CMSDK_LCD_BASE )
//#define LCD_DATA		((CMSDK_LCD_DATA_TypeDef      *) CMSDK_LCD_DATA_BASE )

//#define BEEP                    ((CMSDK_BEEP_TypeDef     *) CMSDK_BEEP_BASE )

/**
  * @}
  */



  /** @addtogroup Peripheral_Registers_Bits_Definition
  * @{
  */
    
/******************************************************************************/
/*                         Peripheral Registers Bits Definition               */
/******************************************************************************/
/******************************************************************************/
/*                                                                            */
/*                      System Control (SYSCON)                     */
/*                                                                            */
/******************************************************************************/
/********************  Bits definition for SYSCON_REMAP register  ******************/
#define SYSCON_REMAP_BOOT                        ((uint32_t)0x00000001)        /*!<  */

/********************  Bits definition for SYSCON_RESETOP register  ******************/
#define SYSCON_RESETOP_LOCKUPRST                 ((uint32_t)0x00000001)        /*!<  */

/********************  Bits definition for SYSCON_SYSCLK register  ******************/
#define SYSCON_SYSCLK_SYSCLKSEL                  ((uint32_t)0x00000003)       	 /*!<  */
#define SYSCON_SYSCLK_SYSCLKSEL_24MIOSC          ((uint32_t)0x00000000)        /*!<  */
#define SYSCON_SYSCLK_SYSCLKSEL_EXTCLK           ((uint32_t)0x00000001)        /*!<  */
#define SYSCON_SYSCLK_SYSCLKSEL_32KIOSC          ((uint32_t)0x00000002)        /*!<  */
#define SYSCON_SYSCLK_FDCLKSEL                   ((uint32_t)0x00000004)        /*!<  */
#define SYSCON_SYSCLK_FDCLKSEL_24MIOSC           ((uint32_t)0x00000000)        /*!<  */
#define SYSCON_SYSCLK_FDCLKSEL_32KIOSC           ((uint32_t)0x00000004)        /*!<  */
#define SYSCON_SYSCLK_HCLKDIV                    ((uint32_t)0x000001F0)        /*!<  */
#define SYSCON_SYSCLK_FDFAIL_DETECTEN            ((uint32_t)0x00004000)        /*!<  */
#define SYSCON_SYSCLK_FDFAIL_STATUS              ((uint32_t)0x00008000)        /*!<  */
#define SYSCON_SYSCLK_WAKE_DELAY                 ((uint32_t)0x00030000)        /*!<  */
#define SYSCON_SYSCLK_RECOVER_SEL                ((uint32_t)0x00100000)        /*!<  */
#define SYSCON_SYSCLK_PCLKDIV                    ((uint32_t)0x1F000000)        /*!<  */

/********************  Bits definition for SYSCON_RSTINFO register  ******************/
#define SYSCON_RSTINFO_SYSSOFTRST                ((uint32_t)0x00000001)        /*!<  */
#define SYSCON_RSTINFO_IWDGRST                   ((uint32_t)0x00000002)        /*!<  */
#define SYSCON_RSTINFO_LOCKUPRST                 ((uint32_t)0x00000004)        /*!<  */
#define SYSCON_RSTINFO_EOSFAILRST                ((uint32_t)0x00000010)        /*!<  */
#define SYSCON_RSTINFO_PORST                     ((uint32_t)0x00000040)        /*!<  */
#define SYSCON_RSTINFO_EXRST                     ((uint32_t)0x00000080)        /*!<  */
#define SYSCON_RSTINFO_LVD5VRST                  ((uint32_t)0x00000100)        /*!<  */
#define SYSCON_RSTINFO_LVD1V5RST                 ((uint32_t)0x00000200)        /*!<  */

/********************  Bits definition for SYSCON_RETRIMING register  ******************/
#define SYSCON_RETRIMING_CTRL                    ((uint32_t)0x0000FFFF)        /*!<  */
#define SYSCON_RETRIMING_KEY                     ((uint32_t)0x0000AB56)        /*!<  */

/********************  Bits definition for SYSCON_RSTENCTRL register  ******************/
#define SYSCON_RSTENCTRL_FDFAILRSTEN             ((uint32_t)0x00000020)        /*!<  */
#define SYSCON_RSTENCTRL_BGRSTEN                 ((uint32_t)0x00000040)        /*!<  */
#define SYSCON_RSTENCTRL_VDDLVDRSTEN             ((uint32_t)0x00000100)        /*!<  */
#define SYSCON_RSTENCTRL_LDOLVDRSTEN             ((uint32_t)0x00000200)        /*!<  */
#define SYSCON_RSTENCTRL_RETRIMINGEN             ((uint32_t)0x00000400)        /*!<  */

/********************  Bits definition for SYSCON_CUSTOMERID register  ******************/
#define SYSCON_CUSTOMERID_INFO                   ((uint32_t)0xFFFFFFFF)        /*!<  */

/********************  Bits definition for SYSCON_FLASHPROTECT register  ******************/
#define SYSCON_FLASHPROTECT_STATUS               ((uint32_t)0x00000001)        /*!<  */

/********************  Bits definition for SYSCON_AGENTID register  ******************/
#define SYSCON_AGENTID_INFO                      ((uint32_t)0xFFFFFFFF)        /*!<  */

/********************  Bits definition for SYSCON_UID register  ******************/
#define SYSCON_UID_INFO1                         ((uint32_t)0xFFFFFFFF)        /*!<  */

/********************  Bits definition for SYSCON_UID register  ******************/
#define SYSCON_UID_INFO2                         ((uint32_t)0xFFFFFFFF)        /*!<  */

/********************  Bits definition for SYSCON_UID register  ******************/
#define SYSCON_UID_INFO3                         ((uint32_t)0xFFFFFFFF)        /*!<  */

/********************  Bits definition for SYSCON_CID register  ******************/
#define SYSCON_CID_FLASHSIZE                     ((uint32_t)0x00000001)        /*!<  */
#define SYSCON_CID_SRAMSIZE                      ((uint32_t)0x00000010)        /*!<  */
#define SYSCON_CID_BOOTSIZE                      ((uint32_t)0x00000300)        /*!<  */
#define SYSCON_CID_INFO                          ((uint32_t)0xFFFFFC00)        /*!<  */

/********************  Bits definition for SYSCON_PID4 register  ******************/
#define SYSCON_PID4_JEP106CODE                   ((uint32_t)0x0000000F)        /*!<  */
#define SYSCON_PID4_BLOCKCOUNT                   ((uint32_t)0x000000F0)        /*!<  */

/********************  Bits definition for SYSCON_PID5 register  ******************/
#define SYSCON_PID5_INFO                         ((uint32_t)0xFFFFFFFF)        /*!<  */

/********************  Bits definition for SYSCON_PID6 register  ******************/
#define SYSCON_PID6_INFO                         ((uint32_t)0xFFFFFFFF)        /*!<  */

/********************  Bits definition for SYSCON_PID7 register  ******************/
#define SYSCON_PID7_INFO                         ((uint32_t)0xFFFFFFFF)        /*!<  */

/********************  Bits definition for SYSCON_PID0 register  ******************/
#define SYSCON_PID0_INFO                         ((uint32_t)0xFFFFFFFF)        /*!<  */

/********************  Bits definition for SYSCON_PID1 register  ******************/
#define SYSCON_PID1_INFO                         ((uint32_t)0xFFFFFFFF)        /*!<  */

/********************  Bits definition for SYSCON_PID2 register  ******************/
#define SYSCON_PID2_INFO                         ((uint32_t)0xFFFFFFFF)        /*!<  */

/********************  Bits definition for SYSCON_PID3 register  ******************/
#define SYSCON_PID3_INFO                         ((uint32_t)0xFFFFFFFF)        /*!<  */

/******************************************************************************/
/*                                                                            */
/*                      Analog MIX Function (ANAMIX)                     */
/*                                                                            */
/******************************************************************************/
/********************  Bits definition for ANAMIX_VDDLVDCTRL register  ******************/
#define ANAMIX_VDDLVDCTRL_EN                     ((uint32_t)0x00000001)        /*!<  */
#define ANAMIX_VDDLVDCTRL_TRIP                   ((uint32_t)0x0000000E)        /*!<  */
#define ANAMIX_VDDLVDCTRL_TRIP_1V6               ((uint32_t)0x00000000)        /*!<  */
#define ANAMIX_VDDLVDCTRL_TRIP_1V7               ((uint32_t)0x00000002)        /*!<  */
#define ANAMIX_VDDLVDCTRL_TRIP_2V2               ((uint32_t)0x00000004)        /*!<  */
#define ANAMIX_VDDLVDCTRL_TRIP_2V75              ((uint32_t)0x00000006)        /*!<  */
#define ANAMIX_VDDLVDCTRL_TRIP_3V5               ((uint32_t)0x00000008)        /*!<  */

/******************************************************************************/
/********************  Bits definition for ANAMIX_VDDLVDCTRL register  ******************/
#define ANAMIX_LDOLVDCTRL_TRIP                   ((uint32_t)0x00000007)        /*!<  */
#define ANAMIX_LDOLVDCTRL_TRIP_1V35              ((uint32_t)0x00000001)        /*!<  */
#define ANAMIX_LDOLVDCTRL_TRIP_1V4               ((uint32_t)0x00000002)        /*!<  */
#define ANAMIX_LDOLVDCTRL_TRIP_1V45              ((uint32_t)0x00000004)        /*!<  */


/******************************************************************************/
/********************  Bits definition for ANAMIX_IOSC24MCON register  ******************/
#define ANAMIX_IOSC24MCON_IOSCEN                 ((uint32_t)0x00000001)        /*!<  */

/******************************************************************************/
/********************  Bits definition for ANAMIX_IOSC32KCON register  ******************/
#define ANAMIX_IOSC32KCON_IOSCEN                 ((uint32_t)0x00000001)        /*!<  */

/********************  Bits definition for FD_CON register  ******************/
#define ANAMIX_FDCON_EN                          ((uint32_t)0x00000001)        /*!<  */

/******************************************************************************/
/********************  Bits definition for ANAMIX_DEGLITCHCON register  ******************/
#define ANAMIX_DEGLITCHCON_PA                    ((uint32_t)0x00000030)        /*!<  */
#define ANAMIX_DEGLITCHCON_PA_1US                ((uint32_t)0x00000000)        /*!<  */
#define ANAMIX_DEGLITCHCON_PA_2US                ((uint32_t)0x00000010)        /*!<  */
#define ANAMIX_DEGLITCHCON_PA_3US                ((uint32_t)0x00000020)        /*!<  */
#define ANAMIX_DEGLITCHCON_PA_4US                ((uint32_t)0x00000030)        /*!<  */
#define ANAMIX_DEGLITCHCON_PB                    ((uint32_t)0x000000C0)        /*!<  */
#define ANAMIX_DEGLITCHCON_PB_1US                ((uint32_t)0x00000000)        /*!<  */
#define ANAMIX_DEGLITCHCON_PB_2US                ((uint32_t)0x00000040)        /*!<  */
#define ANAMIX_DEGLITCHCON_PB_3US                ((uint32_t)0x00000080)        /*!<  */
#define ANAMIX_DEGLITCHCON_PB_4US                ((uint32_t)0x000000C0)        /*!<  */
#define ANAMIX_DEGLITCHCON_PC                    ((uint32_t)0x00000300)        /*!<  */
#define ANAMIX_DEGLITCHCON_PC_1US                ((uint32_t)0x00000000)        /*!<  */
#define ANAMIX_DEGLITCHCON_PC_2US                ((uint32_t)0x00000100)        /*!<  */
#define ANAMIX_DEGLITCHCON_PC_3US                ((uint32_t)0x00000200)        /*!<  */
#define ANAMIX_DEGLITCHCON_PC_4US                ((uint32_t)0x00000300)        /*!<  */
#define ANAMIX_DEGLITCHCON_PD                    ((uint32_t)0x00000C00)        /*!<  */
#define ANAMIX_DEGLITCHCON_PD_1US                ((uint32_t)0x00000000)        /*!<  */
#define ANAMIX_DEGLITCHCON_PD_2US                ((uint32_t)0x00000400)        /*!<  */
#define ANAMIX_DEGLITCHCON_PD_3US                ((uint32_t)0x00000800)        /*!<  */
#define ANAMIX_DEGLITCHCON_PD_4US                ((uint32_t)0x00000C00)        /*!<  */

/******************************************************************************/
/********************  Bits definition for ANAMIX_MCODIVCON register  ******************/
#define ANAMIX_MCODIVCON_MCODIVCON               ((uint32_t)0x00000007)        /*!<  */
#define ANAMIX_MCODIVCON_MCODIVCON_1DIV          ((uint32_t)0x00000000)        /*!<  */
#define ANAMIX_MCODIVCON_MCODIVCON_2DIV          ((uint32_t)0x00000001)        /*!<  */
#define ANAMIX_MCODIVCON_MCODIVCON_4DIV          ((uint32_t)0x00000002)        /*!<  */
#define ANAMIX_MCODIVCON_MCODIVCON_8DIV          ((uint32_t)0x00000003)        /*!<  */
#define ANAMIX_MCODIVCON_MCODIVCON_16DIV         ((uint32_t)0x00000004)        /*!<  */
#define ANAMIX_MCODIVCON_CLKSEL                  ((uint32_t)0x00000030)        /*!<  */
#define ANAMIX_MCODIVCON_CLKSEL_24MRC            ((uint32_t)0x00000000)        /*!<  */
#define ANAMIX_MCODIVCON_CLKSEL_FDCLK            ((uint32_t)0x00000010)        /*!<  */
#define ANAMIX_MCODIVCON_CLKSEL_32KRC            ((uint32_t)0x00000020)        /*!<  */
#define ANAMIX_MCODIVCON_CLKSEL_SYSCLK           ((uint32_t)0x00000030)        /*!<  */

/******************************************************************************/
/*                                                                            */
/*                       General Purpose IOs (GPIO)                           */
/*                                                                            */
/******************************************************************************/
/******************************************************************************/
/*!<******************  Bit definition for GPIO_DATA register  *******************/
#define GPIO_DATA_DATA0                        ((uint16_t)0x0001)               /*!< Port input data, bit 0 */
#define GPIO_DATA_DATA1                        ((uint16_t)0x0002)               /*!< Port input data, bit 1 */
#define GPIO_DATA_DATA2                        ((uint16_t)0x0004)               /*!< Port input data, bit 2 */
#define GPIO_DATA_DATA3                        ((uint16_t)0x0008)               /*!< Port input data, bit 3 */
#define GPIO_DATA_DATA4                        ((uint16_t)0x0010)               /*!< Port input data, bit 4 */
#define GPIO_DATA_DATA5                        ((uint16_t)0x0020)               /*!< Port input data, bit 5 */
#define GPIO_DATA_DATA6                        ((uint16_t)0x0040)               /*!< Port input data, bit 6 */
#define GPIO_DATA_DATA7                        ((uint16_t)0x0080)               /*!< Port input data, bit 7 */

/*******************  Bit definition for GPIO_DATAOUT register  *******************/
#define GPIO_DATAOUT_DATAOUT0                        ((uint16_t)0x0001)         /*!< Port output data, bit 0 */
#define GPIO_DATAOUT_DATAOUT1                        ((uint16_t)0x0002)         /*!< Port output data, bit 1 */
#define GPIO_DATAOUT_DATAOUT2                        ((uint16_t)0x0004)         /*!< Port output data, bit 2 */
#define GPIO_DATAOUT_DATAOUT3                        ((uint16_t)0x0008)         /*!< Port output data, bit 3 */
#define GPIO_DATAOUT_DATAOUT4                        ((uint16_t)0x0010)         /*!< Port output data, bit 4 */
#define GPIO_DATAOUT_DATAOUT5                        ((uint16_t)0x0020)         /*!< Port output data, bit 5 */
#define GPIO_DATAOUT_DATAOUT6                        ((uint16_t)0x0040)         /*!< Port output data, bit 6 */
#define GPIO_DATAOUT_DATAOUT7                        ((uint16_t)0x0080)         /*!< Port output data, bit 7 */

/*******************  Bit definition for GPIO_OES register  *******************/
#define GPIO_OES_OES0                        ((uint16_t)0x0001)                 /*!< Port output enable bit set, bit 0 */
#define GPIO_OES_OES1                        ((uint16_t)0x0002)                 /*!< Port output enable bit set, bit 1 */
#define GPIO_OES_OES2                        ((uint16_t)0x0004)                 /*!< Port output enable bit set, bit 2 */
#define GPIO_OES_OES3                        ((uint16_t)0x0008)                 /*!< Port output enable bit set, bit 3 */
#define GPIO_OES_OES4                        ((uint16_t)0x0010)                 /*!< Port output enable bit set, bit 4 */
#define GPIO_OES_OES5                        ((uint16_t)0x0020)                 /*!< Port output enable bit set, bit 5 */
#define GPIO_OES_OES6                        ((uint16_t)0x0040)                 /*!< Port output enable bit set, bit 6 */
#define GPIO_OES_OES7                        ((uint16_t)0x0080)                 /*!< Port output enable bit set, bit 7 */


/*******************  Bit definition for GPIO_OES register  *******************/
#define GPIO_OEC_OEC0                        ((uint16_t)0x0001)                 /*!< Port output enable bit reset, bit 0 */
#define GPIO_OEC_OEC1                        ((uint16_t)0x0002)                 /*!< Port output enable bit reset, bit 1 */
#define GPIO_OEC_OEC2                        ((uint16_t)0x0004)                 /*!< Port output enable bit reset, bit 2 */
#define GPIO_OEC_OEC3                        ((uint16_t)0x0008)                 /*!< Port output enable bit reset, bit 3 */
#define GPIO_OEC_OEC4                        ((uint16_t)0x0010)                 /*!< Port output enable bit reset, bit 4 */
#define GPIO_OEC_OEC5                        ((uint16_t)0x0020)                 /*!< Port output enable bit reset, bit 5 */
#define GPIO_OEC_OEC6                        ((uint16_t)0x0040)                 /*!< Port output enable bit reset, bit 6 */
#define GPIO_OEC_OEC7                        ((uint16_t)0x0080)                 /*!< Port output enable bit reset, bit 7 */


/****************** Bit definition for GPIO_AFS1 register  ********************/
#define GPIO_AFS1_AFS0            ((uint32_t)0x0000000F)
#define GPIO_AFS1_AFS1            ((uint32_t)0x000000F0)
#define GPIO_AFS1_AFS2            ((uint32_t)0x00000F00)
#define GPIO_AFS1_AFS3            ((uint32_t)0x0000F000)
#define GPIO_AFS1_AFS4            ((uint32_t)0x000F0000)
#define GPIO_AFS1_AFS5            ((uint32_t)0x00F00000)
#define GPIO_AFS1_AFS6            ((uint32_t)0x0F000000)
#define GPIO_AFS1_AFS7            ((uint32_t)0xF0000000)





/*******************  Bit definition for GPIO_AFC register  *******************/
#define GPIO_AFC_AFC0                        ((uint16_t)0x0001)                 /*!< Port alternate function bit reset, bit 0 */
#define GPIO_AFC_AFC1                        ((uint16_t)0x0002)                 /*!< Port alternate function bit reset, bit 1 */
#define GPIO_AFC_AFC2                        ((uint16_t)0x0004)                 /*!< Port alternate function bit reset, bit 2 */
#define GPIO_AFC_AFC3                        ((uint16_t)0x0008)                 /*!< Port alternate function bit reset, bit 3 */
#define GPIO_AFC_AFC4                        ((uint16_t)0x0010)                 /*!< Port alternate function bit reset, bit 4 */
#define GPIO_AFC_AFC5                        ((uint16_t)0x0020)                 /*!< Port alternate function bit reset, bit 5 */
#define GPIO_AFC_AFC6                        ((uint16_t)0x0040)                 /*!< Port alternate function bit reset, bit 6 */
#define GPIO_AFC_AFC7                        ((uint16_t)0x0080)                 /*!< Port alternate function bit reset, bit 7 */



/*******************  Bit definition for GPIO_IES register  *******************/                                       
#define GPIO_IES_IES0                        ((uint16_t)0x0001)                 /*!< Port interrupt enable bit set, bit 0 */   
#define GPIO_IES_IES1                        ((uint16_t)0x0002)                 /*!< Port interrupt enable bit set, bit 1 */   
#define GPIO_IES_IES2                        ((uint16_t)0x0004)                 /*!< Port interrupt enable bit set, bit 2 */   
#define GPIO_IES_IES3                        ((uint16_t)0x0008)                 /*!< Port interrupt enable bit set, bit 3 */   
#define GPIO_IES_IES4                        ((uint16_t)0x0010)                 /*!< Port interrupt enable bit set, bit 4 */   
#define GPIO_IES_IES5                        ((uint16_t)0x0020)                 /*!< Port interrupt enable bit set, bit 5 */   
#define GPIO_IES_IES6                        ((uint16_t)0x0040)                 /*!< Port interrupt enable bit set, bit 6 */   
#define GPIO_IES_IES7                        ((uint16_t)0x0080)                 /*!< Port interrupt enable bit set, bit 7 */    
                                                                                                                            
/*******************  Bit definition for GPIO_IES register  *******************/                                       
#define GPIO_IEC_IEC0                        ((uint16_t)0x0001)                 /*!< Port interrupt enable bit reset, bit 0 */ 
#define GPIO_IEC_IEC1                        ((uint16_t)0x0002)                 /*!< Port interrupt enable bit reset, bit 1 */ 
#define GPIO_IEC_IEC2                        ((uint16_t)0x0004)                 /*!< Port interrupt enable bit reset, bit 2 */ 
#define GPIO_IEC_IEC3                        ((uint16_t)0x0008)                 /*!< Port interrupt enable bit reset, bit 3 */ 
#define GPIO_IEC_IEC4                        ((uint16_t)0x0010)                 /*!< Port interrupt enable bit reset, bit 4 */ 
#define GPIO_IEC_IEC5                        ((uint16_t)0x0020)                 /*!< Port interrupt enable bit reset, bit 5 */ 
#define GPIO_IEC_IEC6                        ((uint16_t)0x0040)                 /*!< Port interrupt enable bit reset, bit 6 */ 
#define GPIO_IEC_IEC7                        ((uint16_t)0x0080)                 /*!< Port interrupt enable bit reset, bit 7 */ 


/*******************  Bit definition for GPIO_ITS0 register  *******************/                                           
#define GPIO_ITS0_ITS0                        ((uint16_t)0x0001)                /*!< Port interrupt type0 bit set, bit 0 */    
#define GPIO_ITS0_ITS1                        ((uint16_t)0x0002)                /*!< Port interrupt type0 bit set, bit 1 */    
#define GPIO_ITS0_ITS2                        ((uint16_t)0x0004)                /*!< Port interrupt type0 bit set, bit 2 */    
#define GPIO_ITS0_ITS3                        ((uint16_t)0x0008)                /*!< Port interrupt type0 bit set, bit 3 */    
#define GPIO_ITS0_ITS4                        ((uint16_t)0x0010)                /*!< Port interrupt type0 bit set, bit 4 */    
#define GPIO_ITS0_ITS5                        ((uint16_t)0x0020)                /*!< Port interrupt type0 bit set, bit 5 */    
#define GPIO_ITS0_ITS6                        ((uint16_t)0x0040)                /*!< Port interrupt type0 bit set, bit 6 */    
#define GPIO_ITS0_ITS7                        ((uint16_t)0x0080)                /*!< Port interrupt type0 bit set, bit 7 */      
                                                                                                                           
/*******************  Bit definition for GPIO_ITC0 register  *******************/                                           
#define GPIO_ITC0_ITC0                        ((uint16_t)0x0001)                /*!< Port interrupt type0 bit reset, bit 0 */  
#define GPIO_ITC0_ITC1                        ((uint16_t)0x0002)                /*!< Port interrupt type0 bit reset, bit 1 */  
#define GPIO_ITC0_ITC2                        ((uint16_t)0x0004)                /*!< Port interrupt type0 bit reset, bit 2 */  
#define GPIO_ITC0_ITC3                        ((uint16_t)0x0008)                /*!< Port interrupt type0 bit reset, bit 3 */  
#define GPIO_ITC0_ITC4                        ((uint16_t)0x0010)                /*!< Port interrupt type0 bit reset, bit 4 */  
#define GPIO_ITC0_ITC5                        ((uint16_t)0x0020)                /*!< Port interrupt type0 bit reset, bit 5 */  
#define GPIO_ITC0_ITC6                        ((uint16_t)0x0040)                /*!< Port interrupt type0 bit reset, bit 6 */  
#define GPIO_ITC0_ITC7                        ((uint16_t)0x0080)                /*!< Port interrupt type0 bit reset, bit 7 */  

/*******************  Bit definition for GPIO_ITS1 register  *******************/                                         
#define GPIO_ITS1_ITS0                        ((uint16_t)0x0001)                /*!< Port interrupt type1 bit set, bit 0 */    
#define GPIO_ITS1_ITS1                        ((uint16_t)0x0002)                /*!< Port interrupt type1 bit set, bit 1 */    
#define GPIO_ITS1_ITS2                        ((uint16_t)0x0004)                /*!< Port interrupt type1 bit set, bit 2 */    
#define GPIO_ITS1_ITS3                        ((uint16_t)0x0008)                /*!< Port interrupt type1 bit set, bit 3 */    
#define GPIO_ITS1_ITS4                        ((uint16_t)0x0010)                /*!< Port interrupt type1 bit set, bit 4 */    
#define GPIO_ITS1_ITS5                        ((uint16_t)0x0020)                /*!< Port interrupt type1 bit set, bit 5 */    
#define GPIO_ITS1_ITS6                        ((uint16_t)0x0040)                /*!< Port interrupt type1 bit set, bit 6 */    
#define GPIO_ITS1_ITS7                        ((uint16_t)0x0080)                /*!< Port interrupt type1 bit set, bit 7 */     
                                                                                                                          
/*******************  Bit definition for GPIO_ITC1 register  *******************/                                         
#define GPIO_ITC1_ITC0                        ((uint16_t)0x0001)                /*!< Port interrupt type1 bit reset, bit 0 */  
#define GPIO_ITC1_ITC1                        ((uint16_t)0x0002)                /*!< Port interrupt type1 bit reset, bit 1 */  
#define GPIO_ITC1_ITC2                        ((uint16_t)0x0004)                /*!< Port interrupt type1 bit reset, bit 2 */  
#define GPIO_ITC1_ITC3                        ((uint16_t)0x0008)                /*!< Port interrupt type1 bit reset, bit 3 */  
#define GPIO_ITC1_ITC4                        ((uint16_t)0x0010)                /*!< Port interrupt type1 bit reset, bit 4 */  
#define GPIO_ITC1_ITC5                        ((uint16_t)0x0020)                /*!< Port interrupt type1 bit reset, bit 5 */  
#define GPIO_ITC1_ITC6                        ((uint16_t)0x0040)                /*!< Port interrupt type1 bit reset, bit 6 */  
#define GPIO_ITC1_ITC7                        ((uint16_t)0x0080)                /*!< Port interrupt type1 bit reset, bit 7 */   

/*******************  Bit definition for GPIO_PLS register  *******************/                                         
#define GPIO_PLS_PLS0                        ((uint16_t)0x0001)                 /*!< Port interrupt polarity bit set, bit 0 */    
#define GPIO_PLS_PLS1                        ((uint16_t)0x0002)                 /*!< Port interrupt polarity bit set, bit 1 */    
#define GPIO_PLS_PLS2                        ((uint16_t)0x0004)                 /*!< Port interrupt polarity bit set, bit 2 */    
#define GPIO_PLS_PLS3                        ((uint16_t)0x0008)                 /*!< Port interrupt polarity bit set, bit 3 */    
#define GPIO_PLS_PLS4                        ((uint16_t)0x0010)                 /*!< Port interrupt polarity bit set, bit 4 */    
#define GPIO_PLS_PLS5                        ((uint16_t)0x0020)                 /*!< Port interrupt polarity bit set, bit 5 */    
#define GPIO_PLS_PLS6                        ((uint16_t)0x0040)                 /*!< Port interrupt polarity bit set, bit 6 */    
#define GPIO_PLS_PLS7                        ((uint16_t)0x0080)                 /*!< Port interrupt polarity bit set, bit 7 */    
  
                                                                                                                       
/*******************  Bit definition for GPIO_PLC register  *******************/                                         
#define GPIO_PLC_PLC0                        ((uint16_t)0x0001)                 /*!< Port interrupt polarity bit reset, bit 0 */  
#define GPIO_PLC_PLC1                        ((uint16_t)0x0002)                 /*!< Port interrupt polarity bit reset, bit 1 */  
#define GPIO_PLC_PLC2                        ((uint16_t)0x0004)                 /*!< Port interrupt polarity bit reset, bit 2 */  
#define GPIO_PLC_PLC3                        ((uint16_t)0x0008)                 /*!< Port interrupt polarity bit reset, bit 3 */  
#define GPIO_PLC_PLC4                        ((uint16_t)0x0010)                 /*!< Port interrupt polarity bit reset, bit 4 */  
#define GPIO_PLC_PLC5                        ((uint16_t)0x0020)                 /*!< Port interrupt polarity bit reset, bit 5 */  
#define GPIO_PLC_PLC6                        ((uint16_t)0x0040)                 /*!< Port interrupt polarity bit reset, bit 6 */  
#define GPIO_PLC_PLC7                        ((uint16_t)0x0080)                 /*!< Port interrupt polarity bit reset, bit 7 */  
 

/*******************  Bit definition for GPIO_PR register  *******************/                                         
#define GPIO_PR_PR0                        ((uint16_t)0x0001)                   /*!< Port interrupt pending bit set, bit 0 */    
#define GPIO_PR_PR1                        ((uint16_t)0x0002)                   /*!< Port interrupt pending bit set, bit 1 */    
#define GPIO_PR_PR2                        ((uint16_t)0x0004)                   /*!< Port interrupt pending bit set, bit 2 */    
#define GPIO_PR_PR3                        ((uint16_t)0x0008)                   /*!< Port interrupt pending bit set, bit 3 */    
#define GPIO_PR_PR4                        ((uint16_t)0x0010)                   /*!< Port interrupt pending bit set, bit 4 */    
#define GPIO_PR_PR5                        ((uint16_t)0x0020)                   /*!< Port interrupt pending bit set, bit 5 */    
#define GPIO_PR_PR6                        ((uint16_t)0x0040)                   /*!< Port interrupt pending bit set, bit 6 */    
#define GPIO_PR_PR7                        ((uint16_t)0x0080)                   /*!< Port interrupt pending bit set, bit 7 */    
 

/*******************  Bit definition for GPIO_PUS register  *******************/                                         
#define GPIO_PUS_PUS0                        ((uint16_t)0x0001)                 /*!< Port internal pull-up bit set, bit 0 */    
#define GPIO_PUS_PUS1                        ((uint16_t)0x0002)                 /*!< Port internal pull-up bit set, bit 1 */    
#define GPIO_PUS_PUS2                        ((uint16_t)0x0004)                 /*!< Port internal pull-up bit set, bit 2 */    
#define GPIO_PUS_PUS3                        ((uint16_t)0x0008)                 /*!< Port internal pull-up bit set, bit 3 */    
#define GPIO_PUS_PUS4                        ((uint16_t)0x0010)                 /*!< Port internal pull-up bit set, bit 4 */    
#define GPIO_PUS_PUS5                        ((uint16_t)0x0020)                 /*!< Port internal pull-up bit set, bit 5 */    
#define GPIO_PUS_PUS6                        ((uint16_t)0x0040)                 /*!< Port internal pull-up bit set, bit 6 */    
#define GPIO_PUS_PUS7                        ((uint16_t)0x0080)                 /*!< Port internal pull-up bit set, bit 7 */    

                                                                                                                       
/*******************  Bit definition for GPIO_PUC register  *******************/                                         
#define GPIO_PUC_PUC0                        ((uint16_t)0x0001)                 /*!< Port internal pull-up bit reset, bit 0 */  
#define GPIO_PUC_PUC1                        ((uint16_t)0x0002)                 /*!< Port internal pull-up bit reset, bit 1 */  
#define GPIO_PUC_PUC2                        ((uint16_t)0x0004)                 /*!< Port internal pull-up bit reset, bit 2 */  
#define GPIO_PUC_PUC3                        ((uint16_t)0x0008)                 /*!< Port internal pull-up bit reset, bit 3 */  
#define GPIO_PUC_PUC4                        ((uint16_t)0x0010)                 /*!< Port internal pull-up bit reset, bit 4 */  
#define GPIO_PUC_PUC5                        ((uint16_t)0x0020)                 /*!< Port internal pull-up bit reset, bit 5 */  
#define GPIO_PUC_PUC6                        ((uint16_t)0x0040)                 /*!< Port internal pull-up bit reset, bit 6 */  
#define GPIO_PUC_PUC7                        ((uint16_t)0x0080)                 /*!< Port internal pull-up bit reset, bit 7 */  

/*******************  Bit definition for GPIO_PDS register  *******************/                                           
#define GPIO_PDS_PDS0                        ((uint16_t)0x0001)                 /*!< Port internal pull-down bit set, bit 0 */    
#define GPIO_PDS_PDS1                        ((uint16_t)0x0002)                 /*!< Port internal pull-down bit set, bit 1 */    
#define GPIO_PDS_PDS2                        ((uint16_t)0x0004)                 /*!< Port internal pull-down bit set, bit 2 */    
#define GPIO_PDS_PDS3                        ((uint16_t)0x0008)                 /*!< Port internal pull-down bit set, bit 3 */    
#define GPIO_PDS_PDS4                        ((uint16_t)0x0010)                 /*!< Port internal pull-down bit set, bit 4 */    
#define GPIO_PDS_PDS5                        ((uint16_t)0x0020)                 /*!< Port internal pull-down bit set, bit 5 */    
#define GPIO_PDS_PDS6                        ((uint16_t)0x0040)                 /*!< Port internal pull-down bit set, bit 6 */    
#define GPIO_PDS_PDS7                        ((uint16_t)0x0080)                 /*!< Port internal pull-down bit set, bit 7 */     
                                                                                                                           
/*******************  Bit definition for GPIO_PDC register  *******************/                                           
#define GPIO_PDC_PDC0                        ((uint16_t)0x0001)                 /*!< Port internal pull-down bit reset, bit 0 */  
#define GPIO_PDC_PDC1                        ((uint16_t)0x0002)                 /*!< Port internal pull-down bit reset, bit 1 */  
#define GPIO_PDC_PDC2                        ((uint16_t)0x0004)                 /*!< Port internal pull-down bit reset, bit 2 */  
#define GPIO_PDC_PDC3                        ((uint16_t)0x0008)                 /*!< Port internal pull-down bit reset, bit 3 */  
#define GPIO_PDC_PDC4                        ((uint16_t)0x0010)                 /*!< Port internal pull-down bit reset, bit 4 */  
#define GPIO_PDC_PDC5                        ((uint16_t)0x0020)                 /*!< Port internal pull-down bit reset, bit 5 */  
#define GPIO_PDC_PDC6                        ((uint16_t)0x0040)                 /*!< Port internal pull-down bit reset, bit 6 */  
#define GPIO_PDC_PDC7                        ((uint16_t)0x0080)                 /*!< Port internal pull-down bit reset, bit 7 */  

/*******************  Bit definition for GPIO_ODS register  *******************/                                              
#define GPIO_ODS_ODS0                        ((uint16_t)0x0001)                 /*!< Port open-drain bit set, bit 0 */    
#define GPIO_ODS_ODS1                        ((uint16_t)0x0002)                 /*!< Port open-drain bit set, bit 1 */    
#define GPIO_ODS_ODS2                        ((uint16_t)0x0004)                 /*!< Port open-drain bit set, bit 2 */    
#define GPIO_ODS_ODS3                        ((uint16_t)0x0008)                 /*!< Port open-drain bit set, bit 3 */    
#define GPIO_ODS_ODS4                        ((uint16_t)0x0010)                 /*!< Port open-drain bit set, bit 4 */    
#define GPIO_ODS_ODS5                        ((uint16_t)0x0020)                 /*!< Port open-drain bit set, bit 5 */    
#define GPIO_ODS_ODS6                        ((uint16_t)0x0040)                 /*!< Port open-drain bit set, bit 6 */    
#define GPIO_ODS_ODS7                        ((uint16_t)0x0080)                 /*!< Port open-drain bit set, bit 7 */    
   
                                                                                                                             
/*******************  Bit definition for GPIO_ODC register  *******************/                                             
#define GPIO_ODC_ODC0                        ((uint16_t)0x0001)                 /*!< Port open-drain bit reset, bit 0 */  
#define GPIO_ODC_ODC1                        ((uint16_t)0x0002)                 /*!< Port open-drain bit reset, bit 1 */  
#define GPIO_ODC_ODC2                        ((uint16_t)0x0004)                 /*!< Port open-drain bit reset, bit 2 */  
#define GPIO_ODC_ODC3                        ((uint16_t)0x0008)                 /*!< Port open-drain bit reset, bit 3 */  
#define GPIO_ODC_ODC4                        ((uint16_t)0x0010)                 /*!< Port open-drain bit reset, bit 4 */  
#define GPIO_ODC_ODC5                        ((uint16_t)0x0020)                 /*!< Port open-drain bit reset, bit 5 */  
#define GPIO_ODC_ODC6                        ((uint16_t)0x0040)                 /*!< Port open-drain bit reset, bit 6 */  
#define GPIO_ODC_ODC7                        ((uint16_t)0x0080)                 /*!< Port open-drain bit reset, bit 7 */  


/*******************  Bit definition for GPIO_ANAS register  *******************/                                             
#define GPIO_ANAS_ANAS0                        ((uint16_t)0x0001)               /*!< Port analog function bit set, bit 0 */   
#define GPIO_ANAS_ANAS1                        ((uint16_t)0x0002)               /*!< Port analog function bit set, bit 1 */   
#define GPIO_ANAS_ANAS2                        ((uint16_t)0x0004)               /*!< Port analog function bit set, bit 2 */   
#define GPIO_ANAS_ANAS3                        ((uint16_t)0x0008)               /*!< Port analog function bit set, bit 3 */   
#define GPIO_ANAS_ANAS4                        ((uint16_t)0x0010)               /*!< Port analog function bit set, bit 4 */   
#define GPIO_ANAS_ANAS5                        ((uint16_t)0x0020)               /*!< Port analog function bit set, bit 5 */   
#define GPIO_ANAS_ANAS6                        ((uint16_t)0x0040)               /*!< Port analog function bit set, bit 6 */   
#define GPIO_ANAS_ANAS7                        ((uint16_t)0x0080)               /*!< Port analog function bit set, bit 7 */   
  
                                                                                                                             
/*******************  Bit definition for GPIO_ANAC register  *******************/                                             
#define GPIO_ANAC_ANAC0                        ((uint16_t)0x0001)               /*!< Port analog function bit reset, bit 0 */ 
#define GPIO_ANAC_ANAC1                        ((uint16_t)0x0002)               /*!< Port analog function bit reset, bit 1 */ 
#define GPIO_ANAC_ANAC2                        ((uint16_t)0x0004)               /*!< Port analog function bit reset, bit 2 */ 
#define GPIO_ANAC_ANAC3                        ((uint16_t)0x0008)               /*!< Port analog function bit reset, bit 3 */ 
#define GPIO_ANAC_ANAC4                        ((uint16_t)0x0010)               /*!< Port analog function bit reset, bit 4 */ 
#define GPIO_ANAC_ANAC5                        ((uint16_t)0x0020)               /*!< Port analog function bit reset, bit 5 */ 
#define GPIO_ANAC_ANAC6                        ((uint16_t)0x0040)               /*!< Port analog function bit reset, bit 6 */ 
#define GPIO_ANAC_ANAC7                        ((uint16_t)0x0080)               /*!< Port analog function bit reset, bit 7 */ 


/*******************  Bit definition for GPIO_DRS register  *******************/                                          
#define GPIO_DRS_DRS0                        ((uint16_t)0x0001)                 /*!< Port driver halved bit set, bit 0 */   
#define GPIO_DRS_DRS1                        ((uint16_t)0x0002)                 /*!< Port driver halved bit set, bit 1 */   
#define GPIO_DRS_DRS2                        ((uint16_t)0x0004)                 /*!< Port driver halved bit set, bit 2 */   
#define GPIO_DRS_DRS3                        ((uint16_t)0x0008)                 /*!< Port driver halved bit set, bit 3 */   
#define GPIO_DRS_DRS4                        ((uint16_t)0x0010)                 /*!< Port driver halved bit set, bit 4 */   
#define GPIO_DRS_DRS5                        ((uint16_t)0x0020)                 /*!< Port driver halved bit set, bit 5 */   
#define GPIO_DRS_DRS6                        ((uint16_t)0x0040)                 /*!< Port driver halved bit set, bit 6 */   
#define GPIO_DRS_DRS7                        ((uint16_t)0x0080)                 /*!< Port driver halved bit set, bit 7 */   
 
                                                                                                                           
/*******************  Bit definition for GPIO_DRC register  *******************/                                          
#define GPIO_DRC_DRC0                        ((uint16_t)0x0001)                 /*!< Port driver halved bit reset, bit 0 */ 
#define GPIO_DRC_DRC1                        ((uint16_t)0x0002)                 /*!< Port driver halved bit reset, bit 1 */ 
#define GPIO_DRC_DRC2                        ((uint16_t)0x0004)                 /*!< Port driver halved bit reset, bit 2 */ 
#define GPIO_DRC_DRC3                        ((uint16_t)0x0008)                 /*!< Port driver halved bit reset, bit 3 */ 
#define GPIO_DRC_DRC4                        ((uint16_t)0x0010)                 /*!< Port driver halved bit reset, bit 4 */ 
#define GPIO_DRC_DRC5                        ((uint16_t)0x0020)                 /*!< Port driver halved bit reset, bit 5 */ 
#define GPIO_DRC_DRC6                        ((uint16_t)0x0040)                 /*!< Port driver halved bit reset, bit 6 */ 
#define GPIO_DRC_DRC7                        ((uint16_t)0x0080)                 /*!< Port driver halved bit reset, bit 7 */ 



/******************************************************************************/
/*                                                                            */
/*                      FLASH Registers                      */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for FLASH_ACR register  ******************/
#define  FLASH_ACR_START   		                   ((uint32_t)0x00000001)        /*!< 启动FLASH操作 */
#define  FLASH_ACR_PERA                          ((uint32_t)0x00000002)        /*!< FLASH操作命令块擦除 */
#define  FLASH_ACR_MERA                        	 ((uint32_t)0x00000004)        /*!< FLASH操作命令主代码区擦除 */
#define  FLASH_ACR_LATENCY    	                 ((uint32_t)0x00000020)        /*!< FLASH访问潜伏期 */
#define  FLASH_ACR_MODE       	                 ((uint32_t)0x00000100)        /*!< FLASH读写访问方式 */
#define  FLASH_ACR_DPSTB        	               ((uint32_t)0x00000800)        /*!< FLASH深度休眠使能 */
#define  FLASH_ACR_AINC           	             ((uint32_t)0x00002000)        /*!< FLASH地址自动递增 */

#define  FLASH_MAIN_FKEY                         ((uint32_t)0xADEB0000)        /*!< FLASH主代码区擦写密码*/
#define  FLASH_USER_KEY0                         ((uint32_t)0x2D570000)        /*!< FLASH用户信息区 sector 0擦写密码*/
#define  FLASH_USER_KEY1                         ((uint32_t)0x58B40000)        /*!< FLASH用户信息区 sector 1擦写密码*/
#define  FLASH_USER_KEY2                         ((uint32_t)0x63F20000)        /*!< FLASH用户信息区 sector 2擦写密码*/
#define  FLASH_USER_KEY3                         ((uint32_t)0xDC490000)        /*!< FLASH用户信息区 sector 3擦写密码*/
#define  FLASH_USER_KEY4                         ((uint32_t)0x59BD0000)        /*!< FLASH用户信息区 sector 4擦写密码*/
#define  FLASH_USER_KEY5                         ((uint32_t)0x37A10000)        /*!< FLASH用户信息区 sector 5擦写密码*/
#define  FLASH_USER_KEY6                         ((uint32_t)0x4BF00000)        /*!< FLASH用户信息区 sector 6擦写密码*/
#define  FLASH_USER_KEY7                         ((uint32_t)0x71C30000)        /*!< FLASH用户信息区 sector 7擦写密码*/

/*******************  Bit definition for FLASH_ISR register  ******************/
																																								
#define  FLASH_ISR_WREND                     ((uint32_t)0x00000001)        /*!< 写命令完成 */
#define  FLASH_ISR_EREND                     ((uint32_t)0x00000002)        /*!< 块擦除命令完成 */
#define  FLASH_ISR_PREREND                   ((uint32_t)0x00000004)        /*!< FLASH解保护时擦除主程序完成(KEY=0xAD) */
#define  FLASH_ISR_BSY                       ((uint32_t)0x00000008)        /*!< FLASH忙标志 */
#define  FLASH_ISR_CMDER                     ((uint32_t)0x00000010)        /*!< FLASH操作命令错误标志 */
#define  FLASH_ISR_KEYER                     ((uint32_t)0x00000020)        /*!< FLASH密码错误标志 */
#define  FLASH_ISR_ADER                      ((uint32_t)0x00000040)        /*!< FLASH地址错误标志 */

/*******************  Bit definition for FLASH_IER register  ******************/
																																								
#define  FLASH_IER_WRENDIE                   ((uint32_t)0x00000001)        /*!< 写命令完成中断使能 */
#define  FLASH_IER_ERENDIE                   ((uint32_t)0x00000002)        /*!< 块擦除命令完成中断使能  */
#define  FLASH_IER_PRERENDIE                 ((uint32_t)0x00000004)        /*!< FLASH解保护时擦除主程序完成中断使能 (KEY=0xAD) */
#define  FLASH_IER_CMDERIE                   ((uint32_t)0x00000010)        /*!< FLASH操作命令错误中断使能 */
#define  FLASH_IER_KEYERIE                   ((uint32_t)0x00000020)        /*!< FLASH密码错误中断使能 */
#define  FLASH_IER_ADERIE                    ((uint32_t)0x00000040)        /*!< FLASH地址错误中断使能 */



/*******************  Bit definition for FLASH_AR register  *******************/
#define  FLASH_AR_BADDR                      ((uint32_t)0x00000003)
#define  FLASH_AR_FAR                        ((uint32_t)0x0000FFFC)        /*!< Flash Address */

/*******************  Bit definition for FLASH_DR register  *******************/
#define  FLASH_DR_FDR                        ((uint32_t)0xFFFFFFFF)        /*!< Flash Address */

/*******************  Bit definition for FLASH_DIV register  *******************/
#define  FLASH_DIV_FDIV                      ((uint32_t)0x0000000F)        /*!< Flash Address */

/******************************************************************************/
/*                                                                            */
/*      Universal Asynchronous Receiver Transmitter (UART)       */
/*                                                                            */
/******************************************************************************/
/******************  Bit definition for UART_BR register  *******************/
#define  UART_BR_BR                        ((uint16_t)0x01FF)               /*!< BR[8:0] bits (Receiver Transmitter Buffer value) */

/******************  Bit definition for UART_CR register  *******************/
#define  UART_CR_MODE                      ((uint32_t)0x00000007)           /*!< MODE[2:0] bits (工作模式选择) */
#define  UART_CR_STOP                      ((uint32_t)0x00000018)           /*!< STOP[1:0] bits (停止位长度选择) */
#define  UART_CR_PBR                       ((uint32_t)0x00000020)           /*!< 奇偶校验方式 */
#define  UART_CR_LPB                       ((uint32_t)0x00000040)           /*!< 回绕模式控制 */
#define  UART_CR_BRUN                      ((uint32_t)0x00000080)           /*!< 波特率发生器使能 */
#define  UART_CR_RXEN                      ((uint32_t)0x00000100)           /*!< 数据接收使能 */
#define  UART_CR_RXPOL                     ((uint32_t)0x00010000)           /*!< 数据接收极性控制 */
#define  UART_CR_TXPOL                     ((uint32_t)0x00020000)           /*!< 数据发送极性控制 */
#define  UART_CR_OWE                       ((uint32_t)0x00100000)           /*!< OneWire模式使能 */
#define  UART_CR_OWD                       ((uint32_t)0x00200000)           /*!< OneWire模式收发方向控制 */

/******************  Bit definition for UART_BRR register  *******************/
#define  UART_BRR_BRR                      ((uint32_t)0x0000FFFF)               /*!< BRR[15:0] bits (波特率配置) */
#define  UART_BRR_SR                       ((uint32_t)0x00030000)               /*!< 采样率配置 */
#define  UART_BRR_SRX16                    ((uint32_t)0x00000000)               /*!< 采样率配置 */
#define  UART_BRR_SRX8                     ((uint32_t)0x00010000)               /*!< 采样率配置 */
#define  UART_BRR_SRX4                     ((uint32_t)0x00020000)               /*!< 采样率配置 */

/******************  Bit definition for UART_IER register  *******************/
#define  UART_IER_RXNEIE                   ((uint32_t)0x00000001)        /*!< 接收缓冲队列非空中断允许控制位 */
#define  UART_IER_RXHFIE                   ((uint32_t)0x00000002)        /*!< 接收缓冲队列半满中断允许控制位 */
#define  UART_IER_RXFIE                    ((uint32_t)0x00000004)        /*!< 接收缓冲队列全满中断允许控制位 */
#define  UART_IER_TONEIE                   ((uint32_t)0x00000008)        /*!< 接收缓冲队列清空超时中断允许控制位 */
#define  UART_IER_TOIDLEIE                 ((uint32_t)0x00000010)        /*!< 空闲超时中断允许控制位 */
#define  UART_IER_PERRIE                   ((uint32_t)0x00000020)        /*!< 奇偶校验错误中断允许控制位 */      
#define  UART_IER_FERRIE                   ((uint32_t)0x00000040)        /*!< 帧错误中断允许控制位 */
#define  UART_IER_OVERRIE                  ((uint32_t)0x00000080)        /*!< 接收缓冲队列溢出中断允许控制位 */    
#define  UART_IER_TXEIE                    ((uint32_t)0x00000100)        /*!< 发送缓冲队列全空中断允许控制位 */         
#define  UART_IER_TXHEIE                   ((uint32_t)0x00000200)        /*!< 发送缓冲队列半空中断允许控制位 */       
#define  UART_IER_TXFIE                    ((uint32_t)0x00000400)        /*!< 发送缓冲队列全满中断允许控制位 */   
#define  UART_IER_TXENDIE                  ((uint32_t)0x00000800)        /*!< 发送全部完成中断允许控制位 */  


/******************  Bit definition for UART_ISR register  *******************/
#define  UART_ISR_RXNE                     ((uint32_t)0x00000001)        /*!< 接收缓冲队列非空中断标识位 */
#define  UART_ISR_RXHF                     ((uint32_t)0x00000002)        /*!< 接收缓冲队列半满中断标识位 */
#define  UART_ISR_RXF                      ((uint32_t)0x00000004)        /*!< 接收缓冲队列全满中断标识位 */
#define  UART_ISR_TONE                     ((uint32_t)0x00000008)        /*!< 接收缓冲队列清空超时中断标识位 */
#define  UART_ISR_TOIDLE                   ((uint32_t)0x00000010)        /*!< 空闲超时中断标识位 */
#define  UART_ISR_PERR                     ((uint32_t)0x00000020)        /*!< 奇偶校验错误中断标识位 */      
#define  UART_ISR_FERR                     ((uint32_t)0x00000040)        /*!< 帧错误中断标识位 */
#define  UART_ISR_OVERR                    ((uint32_t)0x00000080)        /*!< 接收缓冲队列溢出中断标识位 */    
#define  UART_ISR_TXE                      ((uint32_t)0x00000100)        /*!< 发送缓冲队列全空中断标识位 */         
#define  UART_ISR_TXHE                     ((uint32_t)0x00000200)        /*!< 发送缓冲队列半空中断标识位 */       
#define  UART_ISR_TXF                      ((uint32_t)0x00000400)        /*!< 发送缓冲队列全满中断标识位 */   
#define  UART_ISR_TXEND                    ((uint32_t)0x00000800)        /*!< 发送全部完成中断标识位 */  

/******************  Bit definition for UART_GTR register  *******************/
#define  UART_GTR_GTR                      ((uint8_t)0xFF)               /*!< GTR[8:0] bits (发送两个连续数据帧的间隔时间，单位为波特率) */

/******************  Bit definition for UART_TO register  *******************/
#define  UART_TO_TO                        ((uint8_t)0xFF)               /*!< TO[8:0] bits (接收超时计时寄存器，单位为波特率) */

/******************  Bit definition for UART_TXFR register  *******************/
#define  UART_TXFR_TXFR                    ((uint32_t)0xFFFFFFFF)        /*!< TXFR[31:0] bits (发送队列复位) */

/******************  Bit definition for UART_RXFR register  *******************/
#define  UART_RXFR_RXFR                    ((uint32_t)0xFFFFFFFF)        /*!< RXFR[31:0] bits (接收队列复位) */ 

/******************  Bit definition for UART_IRR register  *******************/
#define  UART_IRR_EN                       ((uint32_t)0x00000001)        /*!<红外功能使能控制 */
#define  UART_IRR_TXPOL                    ((uint32_t)0x00000002)        /*!<红外发送极性控制 */
#define  UART_IRR_SPD                      ((uint32_t)0x00000030)        /*!<SPD[1:0]红外速率选择控制 */
#define  UART_IRR_RST                      ((uint32_t)0x00000800)        /*!<红外调制软复位 */
#define  UART_IRR_SSBWITH                  ((uint32_t)0x000F0000)        /*!<SSBWITH[3:0] 红外解调起始位宽度缩减 */  


/******************  Bit definition for UART_IRPWMC register  *******************/
#define  UART_IRPWMC_IRPWMC                ((uint32_t)0x00000FFF)        /*!< PWMC[11:0] bits (红外调制占空比控制) */



/******************************************************************************/
/*                                                                            */
/*                      Analog to Digital Converter (ADC)                     */
/*                                                                            */
/******************************************************************************/
/********************  Bits definition for ADC_CR register  ******************/
#define ADC_CR_CHSEL                         ((uint32_t)0x0000000F)        /*!< Channel 11 selection */
#define ADC_CR_CHSEL_11                      ((uint32_t)0x0000000B)        /*!< Channel 11 selection */
#define ADC_CR_CHSEL_10                      ((uint32_t)0x0000000A)        /*!< Channel 10 selection */
#define ADC_CR_CHSEL_9                       ((uint32_t)0x00000009)        /*!< Channel 9 selection  */
#define ADC_CR_CHSEL_8                       ((uint32_t)0x00000008)        /*!< Channel 8 selection  */
#define ADC_CR_CHSEL_7                       ((uint32_t)0x00000007)        /*!< Channel 7 selection  */
#define ADC_CR_CHSEL_6                       ((uint32_t)0x00000006)        /*!< Channel 6 selection  */
#define ADC_CR_CHSEL_5                       ((uint32_t)0x00000005)        /*!< Channel 5 selection  */
#define ADC_CR_CHSEL_4                       ((uint32_t)0x00000004)        /*!< Channel 4 selection  */
#define ADC_CR_CHSEL_3                       ((uint32_t)0x00000003)        /*!< Channel 3 selection  */
#define ADC_CR_CHSEL_2                       ((uint32_t)0x00000002)        /*!< Channel 2 selection  */
#define ADC_CR_CHSEL_1                       ((uint32_t)0x00000001)        /*!< Channel 1 selection  */
#define ADC_CR_CHSEL_0                       ((uint32_t)0x00000000)        /*!< Channel 0 selection  */
#define ADC_CR_ADEN                          ((uint32_t)0x00000020)        /*!< ADC enable control   */
#define ADC_CR_START                         ((uint32_t)0x00000080)        /*!< ADC                  */
#define ADC_CR_CLKDIV                        ((uint32_t)0x00000300)        /*!< CLKDIV[9:8] bits( )  */
#define ADC_CR_CLKDIV_2                      ((uint32_t)0x00000000)
#define ADC_CR_CLKDIV_4                      ((uint32_t)0x00000100)
#define ADC_CR_CLKDIV_8                      ((uint32_t)0x00000200)
#define ADC_CR_CLKDIV_16                     ((uint32_t)0x00000300)
#define ADC_CR_MODE                          ((uint32_t)0x00001800)        /*!< ADC mode             */
#define ADC_CR_MODE_0                        ((uint32_t)0x00000000)
#define ADC_CR_MODE_1                        ((uint32_t)0x00000800)
#define ADC_CR_MODE_2                        ((uint32_t)0x00001000)
#define ADC_CR_ALIGN                         ((uint32_t)0x00002000)       /*!< Data Alignment */
#define ADC_CR_ANATESTSEL                    ((uint32_t)0x00004000)
#define ADC_CR_TMRSEL                        ((uint32_t)0x000F0000)
#define ADC_CR_TMRSEL_TIMER1                 ((uint32_t)0x00050000)
#define ADC_CR_TMRSEL_PWM1                   ((uint32_t)0x00060000)
#define ADC_CR_TMRSEL_PWM2                   ((uint32_t)0x00070000)
#define ADC_CR_TMRSEL_PWM3                   ((uint32_t)0x00080000)
#define ADC_CR_TMRSEL_PWM4                   ((uint32_t)0x00090000)
#define ADC_CR_TMRSEL_TIMER2                 ((uint32_t)0x000A0000)
#define ADC_CR_TMRSEL_TIMER3                 ((uint32_t)0x000B0000)


/********************  Bits definition for ADC_TRSTN register  ******************/
#define ADC_TRSTN_TRSTN                      ((uint32_t)0x000000FF)       /*!<  */

/********************  Bits definition for ADC_SR register  ******************/
#define ADC_SR_EOC                           ((uint32_t)0x00000001)       /*!< ADC End of Conversion */
#define ADC_SR_ADRDY                         ((uint32_t)0x00000002)       /*!< ADC Ready */

/********************  Bit definition for ADC_DR register  ********************/
#define  ADC_DR_DATA                         ((uint32_t)0x0000FFFF)        /*!< Regular data */

/******************************************************************************/
/*                                                                            */
/*                   Inter-integrated Circuit Interface (I2C)                 */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for I2C_CSR register  *******************/
#define  I2C_CSR_ACK                         ((uint32_t)0x00000004)        /*!< ACK generation (slave mode) */
#define  I2C_CSR_IS                          ((uint32_t)0x00000008)        /*!< Interrupt status */
#define  I2C_CSR_STOP                        ((uint32_t)0x00000010)        /*!< STOP generation (master mode) */
#define  I2C_CSR_START                       ((uint32_t)0x00000020)        /*!< START generation */
#define  I2C_CSR_EN                          ((uint32_t)0x00000040)        /*!< Peripheral enable */
#define  I2C_CSR_INFILTERDELAY               ((uint32_t)0x0000F000)        /*!< CLK filtering delay*/
#define  I2C_CSR_DIV                         ((uint32_t)0x03FF0000)        /*!< CLK DIV*/

/*******************  Bit definition for I2C_DR register  *******************/
#define  I2C_SR_SR                           ((uint32_t)0x000000F8)      
/*******************  Bit definition for I2C_DR register  *******************/
#define  I2C_DR_DATA                         ((uint32_t)0x000000FF)        /*!< Enable Broadcast addressing  */    

/*******************  Bit definition for I2C_OAR register  *******************/
#define  I2C_OAR_BCSTEN                      ((uint32_t)0x00000001)        /*!< Enable Broadcast addressing  */    
#define  I2C_OAR_ADDR                        ((uint32_t)0x000000FE)        /*!< Enable Broadcast addressing  */ 

/*******************  Bit definition for I2C_CCR register  *******************/
#define  I2C_CCR_MASK                        ((uint32_t)0x000000FF)        /*!<  */
#define  I2C_CCR_ACK                         ((uint32_t)0x00000004)        /*!< ACK generation (slave mode) */
#define  I2C_CCR_IS                          ((uint32_t)0x00000008)        /*!< Interrupt status */
#define  I2C_CCR_STOP                        ((uint32_t)0x00000010)        /*!< STOP generation (master mode) */
#define  I2C_CCR_START                       ((uint32_t)0x00000020)        /*!< START generation */
#define  I2C_CCR_EN                          ((uint32_t)0x00000040)        /*!< Peripheral enable */
#define  I2C_CCR_INFILTERDELAY               ((uint32_t)0x0000F000)        /*!< CLK filtering delay*/
#define  I2C_CCR_DIV                         ((uint32_t)0x03FF0000)        /*!< CLK DIV*/


/******************************************************************************/
/*                                                                            */
/*                        Serial Peripheral Interface (SPI)                   */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for SPI_CR0 register  ********************/
#define  SPI_CR0_DSS                         ((uint32_t)0x0000000F)         /*!< SPI_data_size */
#define  SPI_CR0_DSS_4B                      ((uint32_t)0x00000003)         /*!< Data frame length is 4 bits  */
#define  SPI_CR0_DSS_5B                      ((uint32_t)0x00000004)         /*!< Data frame length is 5 bits  */
#define  SPI_CR0_DSS_6B                      ((uint32_t)0x00000005)         /*!< Data frame length is 6 bits  */
#define  SPI_CR0_DSS_7B                      ((uint32_t)0x00000006)         /*!< Data frame length is 7 bits  */
#define  SPI_CR0_DSS_8B                      ((uint32_t)0x00000007)         /*!< Data frame length is 8 bits  */
#define  SPI_CR0_DSS_9B                      ((uint32_t)0x00000008)         /*!< Data frame length is 9 bits  */
#define  SPI_CR0_DSS_10B                     ((uint32_t)0x00000009)         /*!< Data frame length is 10 bits  */
#define  SPI_CR0_DSS_11B                     ((uint32_t)0x0000000A)         /*!< Data frame length is 11 bits  */
#define  SPI_CR0_DSS_12B                     ((uint32_t)0x0000000B)         /*!< Data frame length is 12 bits  */
#define  SPI_CR0_DSS_13B                     ((uint32_t)0x0000000C)         /*!< Data frame length is 13 bits  */
#define  SPI_CR0_DSS_14B                     ((uint32_t)0x0000000D)         /*!< Data frame length is 14 bits  */
#define  SPI_CR0_DSS_15B                     ((uint32_t)0x0000000E)         /*!< Data frame length is 15 bits  */
#define  SPI_CR0_DSS_16B                     ((uint32_t)0x0000000F)         /*!< Data frame length is 16 bits  */
#define  SPI_CR0_FRF                         ((uint32_t)0x00000030)         /*!< SPI_data frame format */
#define  SPI_CR0_FRF_SPI                     ((uint32_t)0x00000000)         /*!< SPI_data SPI Motorola frame format */
#define  SPI_CR0_FRF_TI                      ((uint32_t)0x00000010)         /*!< SPI_data_TI frame format */
#define  SPI_CR0_FRF_MICROWIRE               ((uint32_t)0x00000020)         /*!< SPI_data Microwire frame format */
#define  SPI_CR0_CPOL                        ((uint32_t)0x00000040)         /*!< Clock Polarity */
#define  SPI_CR0_CPHA                        ((uint32_t)0x00000080)         /*!< Clock Phase */
#define  SPI_CR0_SCR                         ((uint32_t)0x0000FF00)         /*!< Post-division factor */
/*******************  Bit definition for SPI_CR1 register  ********************/
#define  SPI_CR1_LOOPBACK                    ((uint32_t)0x00000001)         /*!< Loopback Mode */
#define  SPI_CR1_SPIEN                       ((uint32_t)0x00000002)         /*!< Loopback Mode */
#define  SPI_CR1_MSTR                        ((uint32_t)0x00000004)         /*!< Master-slave selection */
#define  SPI_CR1_SOD                         ((uint32_t)0x00000004)         /*!< Slave output disable */

/********************  Bit definition for SPI_DR register  ********************/
#define  SPI_DR_DR                           ((uint16_t)0xFFFF)            /*!< Data Register */

/********************  Bit definition for SPI_SR register  ********************/
#define  SPI_SR_TFE                          ((uint32_t)0x00000001)         /*!< Transmission FIFO is empty */
#define  SPI_SR_TFNF                         ((uint32_t)0x00000002)         /*!< Transmission FIFO is not empty */
#define  SPI_SR_RFNE                         ((uint32_t)0x00000004)         /*!< Receiver FIFO is not empty */
#define  SPI_SR_RFF                          ((uint32_t)0x00000008)         /*!< Receiver FIFO is Full */
#define  SPI_SR_BUSY                         ((uint32_t)0x00000010)         /*!< Transmission module is busy */	//错误，由08改为10

/********************  Bit definition for SPI_CPSR register  ********************/
#define  SPI_CPSR_CPSR                       ((uint32_t)0x000000FF)         /*!< Prescaler factor */

/********************  Bit definition for SPI_IE register  ********************/
#define  SPI_IE_RFOF                        ((uint32_t)0x00000001)         /*!< Receiver FIFO overflow */
#define  SPI_IE_RFNEOT                      ((uint32_t)0x00000002)         /*!< Receiver FIFO Non-empty timeout */
#define  SPI_IE_RFHF                        ((uint32_t)0x00000004)         /*!< Receiver FIFO Half-full */
#define  SPI_IE_TFHF                        ((uint32_t)0x00000008)         /*!< Transmission FIFO Half-full */

/********************  Bit definition for SPI_RIS register  ********************/
#define  SPI_RIS_RFOVF                       ((uint32_t)0x00000001)         /*!< Receiver FIFO overflow */
#define  SPI_RIS_RFNETO                      ((uint32_t)0x00000002)         /*!< Receiver FIFO Non-empty timeout */
#define  SPI_RIS_RFHF                        ((uint32_t)0x00000004)         /*!< Receiver FIFO Half-full */
#define  SPI_RIS_TFHF                        ((uint32_t)0x00000008)         /*!< Transmission FIFO Half-full */

/********************  Bit definition for SPI_EIS register  ********************/
#define  SPI_EIS_RFOVF                       ((uint32_t)0x00000001)         /*!< Receiver FIFO overflow */
#define  SPI_EIS_RFNETO                      ((uint32_t)0x00000002)         /*!< Receiver FIFO Non-empty timeout */
#define  SPI_EIS_RFHF                        ((uint32_t)0x00000004)         /*!< Receiver FIFO Half-full */
#define  SPI_EIS_TFHF                        ((uint32_t)0x00000008)         /*!< Transmission FIFO Half-full */

/********************  Bit definition for SPI_ICR register  ********************/
#define  SPI_ICR_RFOVF                       ((uint32_t)0x00000001)         /*!< Receiver FIFO overflow */
#define  SPI_ICR_RFNETO                      ((uint32_t)0x00000002)         /*!< Receiver FIFO Non-empty timeout */

/********************  Bit definition for SPI_CSCR register  ********************/
#define  SPI_CSCR_SWSEL                       ((uint32_t)0x00000004)         /*!< CS is controlled by software  */
#define  SPI_CSCR_SWCS                        ((uint32_t)0x00000008)         /*!< CS Set Bit by software  */



/******************************************************************************/
/*                                                                            */
/*                      TIM Registers                                     */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for TIM_ISR register  *******************/  
#define  TIM_ISR_OV                        ((uint32_t)0x00000001)       /*!<定时器与MR0值匹配中断标志位       */

/*******************  Bit definition for TIM_TCR register  *******************/  
#define  TIM_TCR_EN                        ((uint32_t)0x00000001)       /*!<定时器使能控制位        */
#define  TIM_TCR_UPDATE                    ((uint32_t)0x00000002)       /*!<定时器匹配值更新        */
#define  TIM_TCR_DBGS                      ((uint32_t)0x00000100)       /*!<定时器调试挂起控制位    */

/*******************  Bit definition for TIM_PR register  *******************/  
#define  TIM_PR                            ((uint8_t)0xFF)               /*!<PR[7:0]定时器预分频系数   */

/*******************  Bit definition for TIM_PC register  *******************/  
#define  TIM_PC                            ((uint8_t)0xFFFF)               /*!<PC[15:0]定时器预分频计数当前值   */

/*******************  Bit definition for TIM_TC register  *******************/  
#define  TIM_TC                            ((uint16_t)0xFFFF)            /*!<PC[15:0]定时器当前计数值   */

/*******************  Bit definition for TIM_MCR register  *******************/  
#define  TIM_MCR_MR0I                      ((uint32_t)0x00000001)        /*!<TIM_TC与MR0值匹配时产生中断控制位          */
#define  TIM_MCR_MR0R                      ((uint32_t)0x00000002)        /*!<TIM_TC与MR0值匹配时计数复位控制位          */
#define  TIM_MCR_MR0S                      ((uint32_t)0x00000004)        /*!<TIM_TC与MR0值匹配时计数器停止控制位        */
#define  TIM_MCR_DIR                       ((uint32_t)0x00000008)       /*!<TIM_TC计数方向控制位                       */


/*******************  Bit definition for PWM_MR0 register  *******************/  
#define  TIM_MR0                           ((uint16_t)0xFFFF)            /*!<MR0[15:0]定时器MR0匹配值   */



/******************************************************************************/
/*                                                                            */
/*                      PWM Registers                                     */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for PWM_ISR register  *******************/  
#define  PWM_ISR_MR0                       ((uint32_t)0x00000001)       /*!<定时器匹配0标志位       */
#define  PWM_ISR_MR1                       ((uint32_t)0x00000002)       /*!<定时器匹配1标志位       */
#define  PWM_ISR_MR2                       ((uint32_t)0x00000004)       /*!<定时器匹配2标志位       */
#define  PWM_ISR_MR3                       ((uint32_t)0x00000006)       /*!<定时器匹配3标志位       */
#define  PWM_ISR_MR4                       ((uint32_t)0x00000010)       /*!<定时器匹配4标志位       */
#define  PWM_ISR_CR1_R                     ((uint32_t)0x00000020)       /*!<定时器捕获1上升沿标志位 */
#define  PWM_ISR_CR1_F                    ((uint32_t)0x00000040)       /*!<定时器匹配1下降沿标志位 */
#define  PWM_ISR_CR2_R                     ((uint32_t)0x00000080)       /*!<定时器匹配2上升沿标志位 */
#define  PWM_ISR_CR2_F                     ((uint32_t)0x00000100)       /*!<定时器匹配2下降沿标志位 */
#define  PWM_ISR_CR3_R                     ((uint32_t)0x00000200)       /*!<定时器匹配3上升沿标志位 */
#define  PWM_ISR_CR3_F                     ((uint32_t)0x00000400)       /*!<定时器匹配3下降沿标志位 */
#define  PWM_ISR_CR4_R                     ((uint32_t)0x00000800)       /*!<定时器匹配4上升沿标志位 */
#define  PWM_ISR_CR4_F                     ((uint32_t)0x00001000)       /*!<定时器匹配4下降沿标志位 */
#define  PWM_ISR_BKI                       ((uint32_t)0x00002000)       /*!<定时器刹车输入标志位    */

/*******************  Bit definition for PWM_TCR register  *******************/  
#define  PWM_TCR_EN                        ((uint32_t)0x00000001)       /*!<定时器使能控制位           */
#define  PWM_TCR_UPDATE                    ((uint32_t)0x00000002)       /*!<定时器匹配值更新           */
#define  PWM_TCR_CLKSEL                    ((uint32_t)0x00000004)       /*!<定时器时钟选择控制         */
#define  PWM_TCR_BKISEL                    ((uint32_t)0x00000070)       /*!<定时器刹车输入源选择控制位 */
#define  PWM_TCR_BKISEL_GPIO               ((uint32_t)0x00000000)       
#define  PWM_TCR_BKISEL_LOCKUP             ((uint32_t)0x00000010)
#define  PWM_TCR_BKISEL_FDCLKDISABLE       ((uint32_t)0x00000030)
#define  PWM_TCR_BKISEL_LVD                ((uint32_t)0x00000060)
#define  PWM_TCR_BKISEL_SOFT               ((uint32_t)0x00000070)
#define  PWM_TCR_BKIS                      ((uint32_t)0x00000080)       /*!<定时器刹车输入停止使能控制位*/
#define  PWM_TCR_DBGS                      ((uint32_t)0x00000100)       /*!<定时器调试挂起控制位        */

/*******************  Bit definition for PWM_INT_RPT register  *******************/  
#define  PWM_INT_RPT                       ((uint8_t)0xF)               /*!<INT_RPT[3:0]定时器中断累计次数控制位   */

/*******************  Bit definition for PWM_INT_RPTC register  *******************/  
#define  PWM_INT_RPTC                      ((uint8_t)0xF)               /*!<INT_RPTC[3:0]定时器中断累计次数当前计数值   */

/*******************  Bit definition for PWM_PR register  *******************/  
#define  PWM_PR                            ((uint8_t)0xFFFF)               /*!<PR[7:0]定时器预分频系数   */

/*******************  Bit definition for PWM_PC register  *******************/  
#define  PWM_PC                            ((uint8_t)0xFFFF)               /*!<PC[15:0]定时器预分频计数当前值   */

/*******************  Bit definition for PWM_TC register  *******************/  
#define  PWM_TC                            ((uint16_t)0xFFFF)            /*!<PC[15:0]定时器当前计数值   */

/*******************  Bit definition for PWM_MCR register  *******************/  
#define  PWM_MCR_MR0I                      ((uint32_t)0x00000001)       /*!<PMW_TC和PWM_MR0匹配时产生中断控制位        */
#define  PWM_MCR_MR1I                      ((uint32_t)0x00000002)       /*!<PMW_TC和PWM_MR1匹配时产生中断控制位        */
#define  PWM_MCR_MR2I                      ((uint32_t)0x00000004)       /*!<PMW_TC和PWM_MR2匹配时产生中断控制位        */
#define  PWM_MCR_MR3I                      ((uint32_t)0x00000008)       /*!<PMW_TC和PWM_MR3匹配时产生中断控制位        */
#define  PWM_MCR_MR4I                      ((uint32_t)0x00000010)       /*!<PMW_TC和PWM_MR4匹配时产生中断控制位        */
#define  PWM_MCR_MR0S                      ((uint32_t)0x00000020)       /*!<PMW_TC和PWM_MR0匹配时计时器停止控制位      */
#define  PWM_MCR_DIR                       ((uint32_t)0x00000040)       /*!<PWM_TC计数方向控制位                       */
#define  PWM_MCR_DIR_RV                    ((uint32_t)0x00000080)       /*!<PWM_TC计数方向交替控制位                   */

/*******************  Bit definition for PWM_MR0 register  *******************/  
#define  PWM_MR0                           ((uint16_t)0xFFFF)            /*!<MR0[15:0]定时器MR0匹配值   */

/*******************  Bit definition for PWM_MR1 register  *******************/  
#define  PWM_MR1                           ((uint16_t)0xFFFF)            /*!<MR1[15:0]定时器MR1匹配值   */

/*******************  Bit definition for PWM_MR2 register  *******************/  
#define  PWM_MR2                           ((uint16_t)0xFFFF)            /*!<MR2[15:0]定时器MR2匹配值   */

/*******************  Bit definition for PWM_MR3 register  *******************/  
#define  PWM_MR3                           ((uint16_t)0xFFFF)            /*!<MR3[15:0]定时器MR3匹配值   */

/*******************  Bit definition for PWM_MR4 register  *******************/  
#define  PWM_MR4                           ((uint16_t)0xFFFF)            /*!<MR4[15:0]定时器MR4匹配值   */

/*******************  Bit definition for PWM_CCR register  *******************/  
#define  PWM_CCR_CAP1RE                    ((uint32_t)0x00000001)       /*!<通道1脉冲上升沿捕捉使能控制          */
#define  PWM_CCR_CAP1FE                    ((uint32_t)0x00000002)       /*!<通道1脉冲下降沿捕捉使能控制          */
#define  PWM_CCR_CAP1RST                   ((uint32_t)0x00000004)       /*!<通道1脉冲沿捕捉计数器复位使能控制    */
#define  PWM_CCR_CAP1IE                    ((uint32_t)0x00000008)       /*!<通道1脉冲沿捕捉中断使能控制          */
#define  PWM_CCR_CAP2RE                    ((uint32_t)0x00000010)       /*!<通道2脉冲上升沿捕捉使能控制          */
#define  PWM_CCR_CAP2FE                    ((uint32_t)0x00000020)       /*!<通道2脉冲下降沿捕捉使能控制          */
#define  PWM_CCR_CAP2RST                   ((uint32_t)0x00000040)       /*!<通道2脉冲沿捕捉计数器复位使能控制    */
#define  PWM_CCR_CAP2IE                    ((uint32_t)0x00000080)       /*!<通道2脉冲沿捕捉中断使能控制          */
#define  PWM_CCR_CAP3RE                    ((uint32_t)0x00000100)       /*!<通道3脉冲上升沿捕捉使能控制          */
#define  PWM_CCR_CAP3FE                    ((uint32_t)0x00000200)       /*!<通道3脉冲下降沿捕捉使能控制          */
#define  PWM_CCR_CAP3RST                   ((uint32_t)0x00000400)       /*!<通道3脉冲沿捕捉计数器复位使能控制    */
#define  PWM_CCR_CAP3IE                    ((uint32_t)0x00000800)       /*!<通道3脉冲沿捕捉中断使能控制          */
#define  PWM_CCR_CAP4RE                    ((uint32_t)0x00001000)       /*!<通道4脉冲上升沿捕捉使能控制          */
#define  PWM_CCR_CAP4FE                    ((uint32_t)0x00002000)       /*!<通道4脉冲下降沿捕捉使能控制          */
#define  PWM_CCR_CAP4RST                   ((uint32_t)0x00004000)       /*!<通道4脉冲沿捕捉计数器复位使能控制    */
#define  PWM_CCR_CAP4IE                    ((uint32_t)0x00008000)       /*!<通道4脉冲沿捕捉中断使能控制          */

/*******************  Bit definition for PWM_CR1 register  *******************/  
#define  PWM_CR1_CAP                       ((uint32_t)0x0000FFFF)       /*!<CAP[15:0]捕捉通道1沿变化计数值   */
#define  PWM_CR1_CAPEDGE                   ((uint32_t)0x00010000)       /*!<捕获沿标志位            */

/*******************  Bit definition for PWM_CR2 register  *******************/ 
#define  PWM_CR2_CAP                       ((uint32_t)0x0000FFFF)       /*!<CAP[15:0]捕捉通道2沿变化计数值   */
#define  PWM_CR2_CAPEDGE                   ((uint32_t)0x00010000)       /*!<捕获沿标志位            */

/*******************  Bit definition for PWM_CR3 register  *******************/  
#define  PWM_CR3_CAP                       ((uint32_t)0x0000FFFF)       /*!<CAP[15:0]捕捉通道3沿变化计数值   */
#define  PWM_CR3_CAPEDGE                   ((uint32_t)0x00010000)       /*!<捕获沿标志位            */

/*******************  Bit definition for PWM_CR4 register  *******************/  
#define  PWM_CR4_CAP                       ((uint32_t)0x0000FFFF)       /*!<CAP[15:0]捕捉通道4沿变化计数值   */
#define  PWM_CR4_CAPEDGE                   ((uint32_t)0x00010000)       /*!<捕获沿标志位            */

/*******************  Bit definition for PWM_CMR register  *******************/  
#define  PWM_CMR_MC1                       ((uint32_t)0x00000003)       /*!<MC1[2:0]  输出通道1比较输出模式选择    */
#define  PWM_CMR_PWM1POL                   ((uint32_t)0x00000004)       /*!<输出通道1极性选择                      */
#define  PWM_CMR_PWM1nPOL                  ((uint32_t)0x00000008)       /*!<输出通道1互补输出极性选择              */
#define  PWM_CMR_MC2                       ((uint32_t)0x00000030)       /*!<MC2[6:4]  输出通道2比较输出模式选择    */
#define  PWM_CMR_PWM2POL                   ((uint32_t)0x00000040)       /*!<输出通道2极性选择                      */
#define  PWM_CMR_PWM2nPOL                  ((uint32_t)0x00000080)       /*!<输出通道2互补输出极性选择              */
#define  PWM_CMR_MC3                       ((uint32_t)0x00000300)       /*!<MC3[10:8] 输出通道3比较输出模式选择    */
#define  PWM_CMR_PWM3POL                   ((uint32_t)0x00000400)       /*!<输出通道3极性选择                      */
#define  PWM_CMR_PWM3nPOL                  ((uint32_t)0x00000800)       /*!<输出通道3互补输出极性选择              */
#define  PWM_CMR_MC4                       ((uint32_t)0x00003000)       /*!<MC4[14:12]输出通道4比较输出模式选择    */
#define  PWM_CMR_PWM4POL                   ((uint32_t)0x00004000)       /*!<输出通道4极性选择                      */
#define  PWM_CMR_PWM4nPOL                  ((uint32_t)0x00008000)       /*!<输出通道4互补输出极性选择              */
#define  PWM_CMR_PWM1_IDLE                 ((uint32_t)0x00010000)       /*!<输出通道1初始值                        */
#define  PWM_CMR_PWM2_IDLE                 ((uint32_t)0x00020000)       /*!<输出通道2初始值                        */
#define  PWM_CMR_PWM3_IDLE                 ((uint32_t)0x00040000)       /*!<输出通道3初始值                        */
#define  PWM_CMR_PWM4_IDLE                 ((uint32_t)0x00080000)       /*!<输出通道4初始值                        */
#define  PWM_CMR_PWM1N_IDLE                ((uint32_t)0x00100000)       /*!<输出通道1初始值                        */
#define  PWM_CMR_PWM2N_IDLE                ((uint32_t)0x00200000)       /*!<输出通道2初始值                        */
#define  PWM_CMR_PWM3N_IDLE                ((uint32_t)0x00400000)       /*!<输出通道3初始值                        */
#define  PWM_CMR_PWM4N_IDLE                ((uint32_t)0x00800000)       /*!<输出通道4初始值                        */
#define  PWM_CMR_PWM1n_EN                  ((uint32_t)0x01000000)       /*!<输出通道1互补输出使能                  */
#define  PWM_CMR_PWM2n_EN                  ((uint32_t)0x02000000)       /*!<输出通道2互补输出使能                  */
#define  PWM_CMR_PWM3n_EN                  ((uint32_t)0x04000000)       /*!<输出通道3互补输出使能                  */
#define  PWM_CMR_PWM4n_EN                  ((uint32_t)0x08000000)       /*!<输出通道4互补输出使能                  */
#define  PWM_CMR_BKE                       ((uint32_t)0x10000000)       /*!<刹车输入使能控制                       */
#define  PWM_CMR_BKP                       ((uint32_t)0x20000000)       /*!<刹车输入有效电平控制                   */
#define  PWM_CMR_BKI                       ((uint32_t)0x40000000)       /*!<刹车输入中断使能控制                   */
#define  PWM_CMR_BK                       ((uint32_t)0x40000000)        /*!<软件刹车输入                           */


/*******************  Bit definition for PWM_DT register  *******************/  
#define  PWM_DT                            ((uint8_t)0xFF)              /*!<DT[7:0]死区时间控制   */


/******************************************************************************/
/*                                                                            */
/*                      CRC Registers                                     */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for CRC_CR register  *******************/  
#define CRC_CR_EN                          ((uint32_t)0x00000001)          /*!< CRC Enable */
#define CRC_CR_INIT                        ((uint32_t)0x00000002)          /*!< CRC Initial */
#define CRC_CR_DINWIDTH                    ((uint32_t)0x00000004)          /*!< CRC DataIn Width */
#define CRC_CR_INPUT_BITREV                ((uint32_t)0x00000008)          /*!< CRC DataIn Width */
#define CRC_CR_INPUT_BYTEREV               ((uint32_t)0x00000010)          /*!< CRC DataIn Width */
#define CRC_CR_OUTPUT_BITREV               ((uint32_t)0x00000020)          /*!< CRC DataIn Width */

/*******************  Bit definition for CRC_SEED register  *******************/  
#define CRC_SEED_MASK                       ((uint32_t)0x0000FFFF)          /*!< CRC SEED */

/*******************  Bit definition for CRC_POLY register  *******************/  
#define CRC_POLY_MASK                       ((uint32_t)0x0000FFFF)          /*!< CRC SEED */

/*******************  Bit definition for CRC_POLY register  *******************/  
#define CRC_DIN_MASK                        ((uint32_t)0x0000FFFF)          /*!< CRC DATA INPUT */

/*******************  Bit definition for CRC_POLY register  *******************/  
#define CRC_DOUT_MASK                       ((uint32_t)0x0000FFFF)          /*!< CRC DATA OUT */


/******************************************************************************/
/*                                                                            */
/*                        Independent WATCHDOG (WDG)                         */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for WDG_RLR register  *******************/
#define  WDG_RLR_RL                          ((uint32_t)0xFFFFFFFF)            /*!< Watchdog counter reload value */

/*******************  Bit definition for WDG_CR register  *******************/  
#define  WDG_CR_INTE                         ((uint32_t)0x00000001)       /*!< Enable IWDG        */
#define  WDG_CR_RSTE                         ((uint32_t)0x00000002)       /*!< RESET IWDG         */
#define  WDG_CR_DBGE                         ((uint32_t)0x00000004)       /*!< RESET IWDG         */

/*******************  Bit definition for WDG_RIS register  *******************/  
#define  WDG_RIS_RVU                         ((uint32_t)0x00000001)               /*!< Watchdog counter reload value update */

/*******************  Bit definition for WDG_KR register  ********************/
#define  WDG_KR_KEY                          ((uint32_t)0xFFFFFFFF)            /*!< Key value (write only, read 0000h) */
#define  WDG_KR_LOCK                         ((uint32_t)0x00000001)            /*!< IWDG LOCK        */

/*******************  Bit definition for WDG_RIS register  *******************/  
#define  WDG_EIS_RVU                         ((uint32_t)0x00000001)               /*!< Watchdog counter reload value update */



/**
  * @}
  */


#ifdef USE_STDPERIPH_DRIVER
  #include "PT32X005_conf.h"
#endif

  
#ifdef __cplusplus
}
#endif

#endif 



