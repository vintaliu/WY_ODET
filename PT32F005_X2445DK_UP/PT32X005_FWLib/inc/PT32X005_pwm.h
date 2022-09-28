/******************************************************************************
* @file    PT32X005_pwm.h
* @author  应用开发团队
* @version V1.0.0
* @date    2020/10/19
* @brief    This file contains all the functions prototypes for the PWM firmware library.
*
******************************************************************************
* @attention
*
*
*****************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PT32X005_PWM_H
#define PT32X005_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "PT32X005.h"

/** @addtogroup PWM
  * @{
  */

/* Exported types ------------------------------------------------------------*/


/** @brief Definition of PWM timebase init structure. **/
typedef struct
{
	u16	CounterReload;         /*!< Period (Value for MR0 register)             */
	u8	ClockSel;              /*!< Counting Clock select:HCLK or PCLK          */
	u8 	BKISel;              /*!< Counting BKI Select:GPIO or LOCKUP or HCLK Fail or PLL Fail or COMP0 or COMP1 or LVD       */
	u8	Prescaler;             /*!< Prescaler (Value for PR register)           */
	u8	CounterMode;           /*!< Counter mode up-counting or down-counting   */
	u8	CounterDirRv_EN;       /*!< Counter Direction Reverse enable or disable */
} PWM_TimeBaseInitTypeDef;


/** @brief  PWM Init Structure definition **/
#define PCLK		((uint32_t)0x00000000)
#define HCLK		((uint32_t)0x00000004)

#define BKISel_GPIO		((uint32_t)0x00000000)
#define BKISel_LOCKUP	((uint32_t)0x00000010)
#define BKISel_HCLK_Fail	((uint32_t)0x00000020)
#define BKISel_PLL_Fail	((uint32_t)0x00000030)
#define BKISel_COMP0	((uint32_t)0x00000040)
#define BKISel_COMP1	((uint32_t)0x00000050)
#define BKISel_LVD		((uint32_t)0x00000060)

#define CAPTURE_FE	((uint32_t)0x00000000)
#define CAPTURE_RE	((uint32_t)0x00000001)


#define CAPTURE_RISEEDAGE_F	((uint32_t)0x00000001)
#define CAPTURE_FALLEDAGE_F	((uint32_t)0x00000002)

/** @brief Enumeration of PWM channel. **/
typedef enum
{
	PWM_CH_1 = 0,                /*!< PWM channel 1 */
	PWM_CH_2,                    /*!< PWM channel 2 */
	PWM_CH_3,                    /*!< PWM channel 3 */
	PWM_CH_4                     /*!< PWM channel 4 */
} PWM_CH_Enum;

/** @brief Enumeration of PWM channel output mode. **/
typedef enum
{
	NO_OUTPUT   = 0,                    /*!< PWM channel no output */
	COMPAER_MODE1,                      /*!< PWM channel output high when tc <= compare value,output low when tc > compare value */
	COMPAER_MODE2,                      /*!< PWM channel output low when tc <= compare value,output high when tc > compare value */
	COMPAER_MODE3                       /*!< PWM channel output reverse when tc match compare value */
} PWM_OM_Enum;

/** @brief Definition of PWM channel output init structure. **/
typedef struct
{
	PWM_CH_Enum       Channel;              /*!< Channel selection refer to \ref PWM_CH_Enum */
	PWM_OM_Enum       OutputMode;           /*!< Channel output mode selection refer to \ref MCTM_OM_Enum */
	uint32_t          ControlN;             /*!< CHxN output enble or disable refer to */
	uint32_t          IdleState;            /*!< CHx  Idle State refer to */
	uint16_t          CompareValue;         /*!< Value for MRx register */
	uint8_t           DeathTime;            /*!< Value for DT register */
} PWM_OutputInitTypeDef;


/** @brief Enumeration of PWM channel Capture mode. **/
#define CHAN_CAPTURE_RiseEdage_SEL	((uint16_t)0x00000001)
#define CHAN_CAPTURE_FallingEdage_SEL	((uint16_t)0x00000002)
#define CHAN_CAPTURE_ResetTC_SEL	((uint16_t)0x00000004)
#define CHAN_CAPTURE_Interrupt_Enable	((uint16_t)0x00000008)
#define CHAN_CAPTURE_Interrupt_Disable	((uint16_t)0x00000000)


/** @brief Definition of PWM channel Capture init structure. **/
typedef struct
{
	PWM_CH_Enum       Channel;              /*!< Channel selection refer to \ref PWM_CH_Enum */
	uint16_t          CaptureMode;           /*!< Channel Capture mode selection*/
	uint16_t          Capture_Interrupt;	 /*!< Channel Capture Interrupt selection*/
} PWM_CaptureInitTypeDef;

/* Exported constants --------------------------------------------------------*/

/** @brief  PWM channel_N  control. **/
#define  PWM_CHNCTL_DISABLE           ((uint32_t)0x00000000)      /*!< PWM channel_N disable */
#define  PWM_CHNCTL_ENABLE             PWM_CMR_PWM1n_EN           /*!< PWM channel_N enable */

/** @brief  PWM channel output idle state. **/
#define  PWM_IDLE_LOW                  ((uint32_t)0x00000000)     /*!< PWM channel idle output low  */
#define  PWM_IDLE_HIGH                 PWM_CMR_PWM1_IDLE          /*!< PWM channel idle output high  */

/** @defgroup PWM_Count Mode **/
#define PWM_CNT_MODE_UP                ((uint32_t)0x00000000)
#define PWM_CNT_MODE_DOWN              ((uint32_t)0x00000040)
#define PWM_CounterDirRv_EN            ((uint32_t)0x00000080)
#define PWM_CounterDirRv_DIS           ((uint32_t)0x00000000)

#define PWM_INT_MR0                    PWM_MCR_MR0I
#define PWM_INT_MR1                    PWM_MCR_MR1I
#define PWM_INT_MR2                    PWM_MCR_MR2I
#define PWM_INT_MR3                    PWM_MCR_MR3I
#define PWM_INT_MR4                    PWM_MCR_MR4I
#define PWM_INT_CAP1                   PWM_CCR_CAP1IE
#define PWM_INT_CAP2                   PWM_CCR_CAP2IE
#define PWM_INT_CAP3                   PWM_CCR_CAP3IE
#define PWM_INT_CAP4                   PWM_CCR_CAP4IE
#define PWM_INT_BK                     PWM_CMR_BKI

#define PWM_FLAG_MR0                   PWM_ISR_MR0
#define PWM_FLAG_MR1                   PWM_ISR_MR1
#define PWM_FLAG_MR2                   PWM_ISR_MR2
#define PWM_FLAG_MR3                   PWM_ISR_MR3
#define PWM_FLAG_MR4                   PWM_ISR_MR4
#define PWM_FLAG_CAP1_RE               PWM_ISR_CR1_R
#define PWM_FLAG_CAP1_FE               PWM_ISR_CR1_F
#define PWM_FLAG_CAP2_RE               PWM_ISR_CR2_R
#define PWM_FLAG_CAP2_FE               PWM_ISR_CR2_F
#define PWM_FLAG_CAP3_RE               PWM_ISR_CR3_R
#define PWM_FLAG_CAP3_FE               PWM_ISR_CR3_F
#define PWM_FLAG_CAP4_RE               PWM_ISR_CR4_R
#define PWM_FLAG_CAP4_FE               PWM_ISR_CR4_F
#define PWM_FLAG_BK                    PWM_CMR_BKI


#define PWM_BKI_HIGHLEVEL              ((uint32_t)0x02000000)
#define PWM_BKI_LOWLEVEL               ((uint32_t)0x00000000)
#define IS_PWM_BIK_LEVEL(LEVEL)        (((LEVEL) == PWM_BKI_HIGHLEVEL) || \
                                        ((LEVEL) == PWM_BKI_LOWLEVEL))

#define IS_PWM_ALL_PERIPH(PERIPH)		((PERIPH) == PWM1)

#define IS_PWM_REPEATTIME(TIMES)             ((TIMES) <= 0xF)

#define IS_PWM_COUNTER_RELOAD(RELOADVALUE)   ((RELOADVALUE) <= 0xFFFF)

#define IS_PWM_COUNTER_VALUE(COUNTERVALUE)   ((COUNTERVALUE) <= 0xFFFF)

#define IS_PWM_CLOCKSEL(CLOCKSEL)            (((CLOCKSEL) == PCLK) || \
        ((CLOCKSEL) == HCLK))

#define BKISel_GPIO		((uint32_t)0x00000000)
#define BKISel_LOCKUP	((uint32_t)0x00000010)
#define BKISel_HCLK_Fail	((uint32_t)0x00000020)
#define BKISel_PLL_Fail	((uint32_t)0x00000030)
#define BKISel_COMP0	((uint32_t)0x00000040)
#define BKISel_COMP1	((uint32_t)0x00000050)
#define BKISel_LVD		((uint32_t)0x00000060)

#define BKI(Source)	(((Source) == BKISel_GPIO) || \
       				((Source) == BKISel_LOCKUP)|| \
       				((Source) == BKISel_HCLK_Fail)|| \
       				((Source) == BKISel_PLL_Fail)|| \
       				((Source) == BKISel_COMP0)|| \
       				((Source) == BKISel_COMP1)|| \
       				((Source) == BKISel_LVD))


#define IS_PWM_PRESCALER(PRESCALER)           ((PRESCALER) <= 0xFF)

#define IS_PWM_COUNTERMODE(COUNTERMODE)      (((COUNTERMODE) == PWM_CNT_MODE_UP) || \
        ((COUNTERMODE) == PWM_CNT_MODE_DOWN))

#define IS_PWM_COUNTERDIRRV_EN(COUNTERDIRRV_EN) (((COUNTERDIRRV_EN) == PWM_CounterDirRv_EN) || \
        ((COUNTERDIRRV_EN) == PWM_CounterDirRv_DIS))

#define IS_PWM_CHCHANNEL(CHANNEL)            (((CHANNEL) == PWM_CH_1) || ((CHANNEL) == PWM_CH_2) ||((CHANNEL) == PWM_CH_3) || ((CHANNEL) == PWM_CH_4))

#define IS_PWM_OM(MODE)                      (((MODE) == NO_OUTPUT)     || ((MODE) == COMPAER_MODE1) || \
        ((MODE) == COMPAER_MODE2) || ((MODE) == COMPAER_MODE3))
	

#define IS_PWM_CM(MODE)                      (MODE<8)

#define IS_PWM_CMINT(INTSEL)                 (((INTSEL) == CHAN_CAPTURE_Interrupt_Enable) || (INTSEL) == CHAN_CAPTURE_Interrupt_Disable)

#define IS_PWM_CHNCTL(CHNCTL)                (((CHNCTL) == PWM_CHNCTL_DISABLE) || \
        ((CHNCTL) == PWM_CHNCTL_ENABLE))

#define IS_PWM_IDLE(IDLE)                    (((IDLE) == PWM_IDLE_LOW)    || \
        ((IDLE) == PWM_IDLE_HIGH))

#define IS_PWM_COMPAREVALUE(VALUE)           ((VALUE) <= 0xFFFF)

#define IS_PWM_DEATHTIME(VALUE)              ((VALUE) <= 0xFF)

#define IS_PWM_PWM_INT(PWM_INT)              (((PWM_INT) == PWM_INT_MR0)  || ((PWM_INT) == PWM_INT_MR1)  || \
        ((PWM_INT) == PWM_INT_MR2)  || ((PWM_INT) == PWM_INT_MR3)  || \
        ((PWM_INT) == PWM_INT_MR4)  || ((PWM_INT) == PWM_INT_BK))

#define IS_PWM_CAP_INT(PWM_CAP_INT)			 (((PWM_CAP_INT) ==PWM_INT_CAP1) ||	((PWM_CAP_INT) ==PWM_INT_CAP2)	||\
                                              ((PWM_CAP_INT) ==PWM_INT_CAP3) || ((PWM_CAP_INT) ==PWM_INT_CAP4))

#define IS_PWM_PWM_FLAG(PWM_FLAG)            (((PWM_FLAG) == PWM_FLAG_MR0)     || ((PWM_FLAG) == PWM_FLAG_MR1)     || \
        ((PWM_FLAG) == PWM_FLAG_MR2)     || ((PWM_FLAG) == PWM_FLAG_MR3)     || \
        ((PWM_FLAG) == PWM_FLAG_MR4)     || ((PWM_FLAG) == PWM_FLAG_CAP1_RE) || \
        ((PWM_FLAG) == PWM_FLAG_CAP1_FE) || ((PWM_FLAG) == PWM_FLAG_CAP2_RE) || \
        ((PWM_FLAG) == PWM_FLAG_CAP2_FE) || ((PWM_FLAG) == PWM_FLAG_CAP3_RE) || \
        ((PWM_FLAG) == PWM_FLAG_CAP3_FE) || ((PWM_FLAG) == PWM_FLAG_CAP4_RE) || \
        ((PWM_FLAG) == PWM_FLAG_CAP4_FE) || ((PWM_FLAG) == PWM_FLAG_BK))
#define IS_PWM_CLEAR_FLAG(FLAG) ((FLAG)  <= 0X3FFF)


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/
void PMW_TimeBaseInit(CMSDK_PWM_TypeDef* PWMx, PWM_TimeBaseInitTypeDef* TimeBaseInit);
void PWM_TimeBaseStructInit(PWM_TimeBaseInitTypeDef* TimeBaseInit);
void PWM_OutputInit(CMSDK_PWM_TypeDef* PWMx, PWM_OutputInitTypeDef* OutInit);
void PWM_INT_RepeatTimes(CMSDK_PWM_TypeDef* PWMx, uint8_t RepeatTimes);
void PWM_ClockSourceConfig(CMSDK_PWM_TypeDef* PWMx, uint8_t PWM_ClockSel);
void PWM_PrescalerConfig(CMSDK_PWM_TypeDef* PWMx, uint8_t Prescaler);
void PWM_SetCounterReload(CMSDK_PWM_TypeDef* PWMx, uint16_t Reload);
void PWM_SetCounter(CMSDK_PWM_TypeDef* PWMx, uint16_t Counter);
void PWM_CounterModeConfig(CMSDK_PWM_TypeDef* PWMx, uint8_t CounterMode, uint8_t DirRv_EN);
void PWM_UpdateCmd(CMSDK_PWM_TypeDef* PWMx);
void PWM_Cmd(CMSDK_PWM_TypeDef* PWMx, FunctionalState NewState);
void PWM_IntConfig(CMSDK_PWM_TypeDef* PWMx, uint32_t PWM_INT, FunctionalState NewState);
void PWM_ClearFlag(CMSDK_PWM_TypeDef* PWMx, uint32_t PWM_FLAG);
void PWM_RE_CaptureConfig(CMSDK_PWM_TypeDef* PWMx, uint8_t CHx, FunctionalState NewState);
void PWM_FE_CaptureConfig(CMSDK_PWM_TypeDef* PWMx, uint8_t CHx, FunctionalState NewState);
void PWM_RST_CaptureConfig(CMSDK_PWM_TypeDef* PWMx, uint8_t CHx, FunctionalState NewState);
void PWM_OD_Config(CMSDK_PWM_TypeDef* PWMx, uint8_t CHx, uint32_t Mode);
void PWM_IDLE_Config(CMSDK_PWM_TypeDef* PWMx, uint8_t CHx, uint32_t Idle);
void PWM_CHN_Config(CMSDK_PWM_TypeDef* PWMx, uint8_t CHx, uint32_t CHNCTL);
void PWM_DT_Config(CMSDK_PWM_TypeDef* PWMx, uint8_t CHx, uint8_t DeathTime);
void PWM_SetCompare(CMSDK_PWM_TypeDef* PWMx, uint8_t CHx, uint16_t Cmp);
FlagStatus PWM_GetFlagStatus(CMSDK_PWM_TypeDef* PWMx, uint32_t PWM_FLAG);
uint16_t PWM_GetCaptureCounter(CMSDK_PWM_TypeDef* PWMx, uint8_t CHx);
uint16_t PWM_GetCaptureEdge(CMSDK_PWM_TypeDef* PWMx, uint8_t CHx);
uint16_t PWM_GetCounter(CMSDK_PWM_TypeDef* PWMx);
uint16_t PWM_GetPrescaler(CMSDK_PWM_TypeDef* PWMx);
uint16_t PWM_GetPrescalerCnt(CMSDK_PWM_TypeDef* PWMx);
uint8_t PWM_GetINT_RepeatTimes(CMSDK_PWM_TypeDef* PWMx);
void PWM_BreakInput_Cmd(CMSDK_PWM_TypeDef* PWMx, FunctionalState NewState);
void PWM_BKI_LevelConfig(CMSDK_PWM_TypeDef* PWMx, uint32_t BKI_Level);
void PWM_CaptureInit(CMSDK_PWM_TypeDef* PWMx, PWM_CaptureInitTypeDef* CapInit);
void PWM_CapITConfig(CMSDK_PWM_TypeDef* PWMx,uint32_t PWM_CAP_INT,FunctionalState NewState);

/**
  * @}
  */

  
#ifdef __cplusplus
}
#endif

#endif


