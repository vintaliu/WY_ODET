  /******************************************************************************
  * @file    PT32X005_wdg.h
  * @author  应用开发团队
  * @version V1.0.0
  * @date    2020/10/19
  * @brief    This file contains all the functions prototypes for the WDG firmware library.
  *          
  ******************************************************************************
  * @attention
  *
  *
  *****************************************************************************/
    

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PT32X005_WDG_H
#define PT32X005_WDG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "PT32X005.h"


/** @addtogroup WDG
  * @{
  */
  
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define IS_WDG_ALL_PERIPH(PERIPH)     (PERIPH) == IWDG

#define WDG_WriteAccess_Enable     ((uint32_t)0x1ACCE551)
#define WDG_WriteAccess_Disable    ((uint32_t)0x00000000)
#define IS_WDG_WRITE_ACCESS(ACCESS) (((ACCESS) == WDG_WriteAccess_Enable) || \
                                     ((ACCESS) == WDG_WriteAccess_Disable))


/** @defgroup IWDG_prescaler **/
#define WDG_Prescaler_4            ((uint8_t)0x00)
#define WDG_Prescaler_8            ((uint8_t)0x01)
#define WDG_Prescaler_16           ((uint8_t)0x02)
#define WDG_Prescaler_32           ((uint8_t)0x03)
#define WDG_Prescaler_64           ((uint8_t)0x04)
#define WDG_Prescaler_128          ((uint8_t)0x05)
#define WDG_Prescaler_256          ((uint8_t)0x06)
#define IS_WDG_PRESCALER(PRESCALER) (((PRESCALER) == WDG_Prescaler_4)  || \
                                     ((PRESCALER) == WDG_Prescaler_8)  || \
                                     ((PRESCALER) == WDG_Prescaler_16) || \
                                     ((PRESCALER) == WDG_Prescaler_32) || \
                                     ((PRESCALER) == WDG_Prescaler_64) || \
                                     ((PRESCALER) == WDG_Prescaler_128)|| \
                                     ((PRESCALER) == WDG_Prescaler_256))
                                     

/** @defgroup IWDG_Flag **/
#define WDG_FLAG_RVU                 WDG_RIS_RVU
#define IS_WDG_FLAG(FLAG)           (((FLAG) == WDG_FLAG_RVU))

#define WDG_IT_RVU                   WDG_EIS_RVU
#define IS_WDG_FLAG(FLAG)           (((FLAG) == WDG_FLAG_RVU))
#define IS_WDG_GET_IT(IT) (((IT) == WDG_IT_RVU))

#define IS_WDG_RELOAD(RELOAD) ((RELOAD) <= 0xFFFFFFFF)


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void WDG_WriteAccessCmd(CMSDK_WDG_TypeDef* WDGx, uint32_t WDG_WriteAccess);
void WDG_SetReload(CMSDK_WDG_TypeDef* WDGx, uint32_t Reload);
void WDG_ReloadCounter(CMSDK_WDG_TypeDef* WDGx);
void WDG_AccessEnable(CMSDK_WDG_TypeDef* WDGx, FunctionalState NewState);
void WDG_Cmd(CMSDK_WDG_TypeDef* WDGx, FunctionalState NewState);
FlagStatus WDG_GetFlagStatus(CMSDK_WDG_TypeDef* WDGx, uint16_t WDG_FLAG);
ITStatus WDG_GetITStatus(CMSDK_WDG_TypeDef* WDGx, uint32_t WDG_IT);
FlagStatus WDG_GetLockStatus(CMSDK_WDG_TypeDef* WDGx);
void WDG_RstCmd(CMSDK_WDG_TypeDef* WDGx, FunctionalState NewState);
void WDG_DbgeRunCmd(CMSDK_WDG_TypeDef* WDGx, FunctionalState NewState);

/**
  * @}
  */

  
#ifdef __cplusplus
}
#endif

#endif 



