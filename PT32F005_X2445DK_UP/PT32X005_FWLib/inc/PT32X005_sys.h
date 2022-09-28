  /******************************************************************************
  * @file    PT32X005_sys.h
  * @author  应用开发团队
  * @version V1.0.0
  * @date    2020/10/19
  * @brief    This file contains all the functions prototypes for the SYS firmware library.
  *          
  ******************************************************************************
  * @attention
  *
  *
  *****************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PT32X005_SYS_H
#define PT32X005_SYS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "PT32X005.h"


/** @addtogroup SYSTEM CONTROL
  * @{
  */

/* Exported types ------------------------------------------------------------*/
typedef struct
{
	uint32_t SYSCLK_Frequency;
	uint32_t PCLK_Frequency;
} System_ClocksTypeDef;


/* Exported constants --------------------------------------------------------*/
/** @brief Enumeration of Global reset status. **/
typedef enum
{
	RSTCU_FLAG_SYSRST = 0,
	RSTCU_FLAG_IWDTRST,
	RSTCU_FLAG_LOCKUPRST,
	RSTCU_FLAG_SWDTRST,
	RSTCU_FLAG_EOSFAILRST,
	RSTCU_FLAG_FDFAIL,
	RSTCU_FLAG_PORST,
	RSTCU_FLAG_EXTRST,
	RSTCU_FLAG_PVDRST,
	RSTCU_FLAG_LVDRST
} RSTCU_RSTF_TypeDef;


#define IS_RSTCU_FLAG(FLAG)       ((FLAG == RSTCU_FLAG_SYSRST)    || \
                                   (FLAG == RSTCU_FLAG_IWDTRST)   || \
                                   (FLAG == RSTCU_FLAG_LOCKUPRST) || \
                                   (FLAG == RSTCU_FLAG_SWDTRST)   || \
                                   (FLAG == RSTCU_FLAG_EOSFAILRST)|| \
                                   (FLAG == RSTCU_FLAG_FDFAIL)    || \
                                   (FLAG == RSTCU_FLAG_PORST)     || \
                                   (FLAG == RSTCU_FLAG_EXTRST)    || \
                                   (FLAG == RSTCU_FLAG_PVDRST)    || \
                                   (FLAG == RSTCU_FLAG_LVDRST))


/*  Definitions of APB peripheral 0 reset */
#define RSTEN_ESOC_FAIL            (1UL << 4)
#define RSTEN_FD_FAIL              (1UL << 5)
#define RSTEN_BG_FAIL              (1UL << 6)
#define RSTEN_PVD_FAIL             (1UL << 8)
#define RSTEN_LVD_FAIL             (1UL << 9)
#define RSTEN_RETRIMING            (1UL << 10)


#define IS_RSTEN_SOURCE(SOURCE)    ((SOURCE == RSTEN_ESOC_FAIL)   || \
                                    (SOURCE == RSTEN_FD_FAIL)    || \
                                    (SOURCE == RSTEN_BG_FAIL)    || \
                                    (SOURCE == RSTEN_PVD_FAIL)   || \
                                    (SOURCE == RSTEN_LVD_FAIL)   || \
                                    (SOURCE == RSTEN_RETRIMING))


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/
void System_GetClocksFreq(System_ClocksTypeDef* System_Clocks);
void System_GetClocksFreq(System_ClocksTypeDef * System_Clocks);
RemapStatus System_GetRemapStatus(void);
ProtectStatus System_GetFlashProtectStatus(void);
void System_ResetRemapBit(void);
void System_Retriming(void);
FlagStatus System_GetResetFlagStatus(RSTCU_RSTF_TypeDef RSTCU_RSTF);
void System_ClearResetFlag(RSTCU_RSTF_TypeDef RSTCU_RSTF);
void System_ClearAllResetFlag(void);
void System_RestConfig(uint32_t RstSource, FunctionalState NewState);
uint32_t System_GetCustomerID(void);
uint32_t System_GetAgentID(void);
uint32_t System_GetUID(uint32_t ID);
uint32_t System_GetCD(void);

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif 


