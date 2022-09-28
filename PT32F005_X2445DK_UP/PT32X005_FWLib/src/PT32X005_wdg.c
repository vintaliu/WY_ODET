  /******************************************************************************
  * @file    PT32X005_wdg.c
  * @author  应用开发团队
  * @version V1.0.0
  * @date    2020/10/19
  * @brief    This file provides firmware functions to manage the following
  *          functionalities of the Independent watchdog (IWDG) peripheral:
  *           + Prescaler and Counter configuration
  *           + IWDG activation
  *           + Flag management
  *            
  ******************************************************************************
  * @attention
  *
  *
  *****************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "PT32X005_wdg.h"

/** @defgroup IWDG
  * @brief IWDG driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define KR_KEY_DISABLE   ((uint16_t)0xFFFFFFFF)
#define KR_KEY_ENABLE    ((uint16_t)0x1ACCE551)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Enables or disables write access all registers.
  * @param  IWDG_WriteAccess: new state of write access to all registers.
  *          This parameter can be one of the following values:
  *            @arg IWDG_WriteAccess_Enable: Enable write access to IWDG_PR and IWDG_RLR registers
  *            @arg IWDG_WriteAccess_Disable: Disable write access to IWDG_PR and IWDG_RLR registers
  * @retval None
  */
void WDG_WriteAccessCmd(CMSDK_WDG_TypeDef* WDGx, uint32_t WDG_WriteAccess)
{
	/* Check the parameters */
	assert_param(IS_WDG_ALL_PERIPH(WDGx));
	assert_param(IS_WDG_WRITE_ACCESS(WDG_WriteAccess));
	WDGx->KR = WDG_WriteAccess;
}


/**
  * @brief  Sets IWDG Reload value.
  * @param  Reload: specifies the IWDG Reload value.
  *          This parameter must be a number between 0 and 0xFFFFFFFF.
  * @retval None
  */
void WDG_SetReload(CMSDK_WDG_TypeDef* WDGx, uint32_t Reload)
{
	/* Check the parameters */
	assert_param(IS_WDG_RELOAD(Reload));
	assert_param(IS_WDG_ALL_PERIPH(WDGx));
	WDGx->RLR = Reload;
}

/**
  * @brief  Reloads IWDG counter with value defined in the reload register
  *   (write access to all registers disabled).
  * @param  None
  * @retval None
  */
void WDG_ReloadCounter(CMSDK_WDG_TypeDef* WDGx)
{
	/* Check the parameters */
	assert_param(IS_WDG_ALL_PERIPH(WDGx));
	WDGx->FWDG = 0xFFFFFFFF;
}


/**
  * @brief  Enables IWDG (write access to all registers disabled).
  * @param  None
  * @retval None
  */
void WDG_AccessEnable(CMSDK_WDG_TypeDef* WDGx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_WDG_ALL_PERIPH(WDGx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		/* Enable the WDG */
		WDGx->KR = KR_KEY_ENABLE;
	}
	else
	{
		/* Disable the WDG */
		WDGx->KR = KR_KEY_DISABLE;
	}
}

/**
  * @brief  Checks whether the specified IWDG flag is set or not.
  * @param  IWDG_FLAG: specifies the flag to check.
  *          This parameter can be one of the following values:
  *            @arg IWDG_FLAG_PVU: Prescaler Value Update on going
  *            @arg IWDG_FLAG_RVU: Reload Value Update on going
  *            @arg IWDG_FLAG_WVU: Counter Window Value Update on going
  * @retval The new state of IWDG_FLAG (SET or RESET).
  */
FlagStatus WDG_GetLockStatus(CMSDK_WDG_TypeDef* WDGx)
{
	FlagStatus bitstatus = RESET;
	/* Check the parameters */
	assert_param(IS_WDG_ALL_PERIPH(WDGx));
	if ((WDGx->KR & WDG_KR_LOCK) != (uint32_t)RESET)
	{
		bitstatus = SET;
	}
	else
	{
		bitstatus = RESET;
	}
	/* Return the flag status */
	return bitstatus;
}


/**
  * @brief
  * @param
  * @param
  * @retval
  */
void WDG_Cmd(CMSDK_WDG_TypeDef* WDGx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_WDG_ALL_PERIPH(WDGx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		/* Enable the IWDG */
		WDGx->CR |= WDG_CR_INTE;
	}
	else
	{
		/* Disable the IWDG */
		WDGx->CR &= ~WDG_CR_INTE;
	}
}
/**
  * @brief
  * @param
  * @param
  * @retval
  */
void WDG_RstCmd(CMSDK_WDG_TypeDef* WDGx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_WDG_ALL_PERIPH(WDGx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		/* Enable the IWDG RESET */
		WDGx->CR |= WDG_CR_RSTE;
	}
	else
	{
		/* Disable the IWDG RESET*/
		WDGx->CR &= ~WDG_CR_RSTE;
	}
}

/**
  * @brief
  * @param
  * @param
  * @retval
  */
void WDG_DbgeRunCmd(CMSDK_WDG_TypeDef* WDGx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_WDG_ALL_PERIPH(WDGx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		/* Enable the IWDG RESET */
		WDGx->CR |= WDG_CR_DBGE;
	}
	else
	{
		/* Disable the IWDG RESET*/
		WDGx->CR &= ~WDG_CR_DBGE;
	}
}


/**
  * @brief  Checks whether the specified IWDG flag is set or not.
  * @param  IWDG_FLAG: specifies the flag to check.
  *          This parameter can be one of the following values:
  *            @arg IWDG_FLAG_PVU: Prescaler Value Update on going
  *            @arg IWDG_FLAG_RVU: Reload Value Update on going
  *            @arg IWDG_FLAG_WVU: Counter Window Value Update on going
  * @retval The new state of IWDG_FLAG (SET or RESET).
  */
FlagStatus WDG_GetFlagStatus(CMSDK_WDG_TypeDef* WDGx, uint16_t WDG_FLAG)
{
	FlagStatus bitstatus = RESET;
	/* Check the parameters */
	assert_param(IS_WDG_FLAG(WDG_FLAG));
	assert_param(IS_WDG_ALL_PERIPH(WDGx));
	if ((WDGx->RIS & WDG_FLAG) != (uint32_t)RESET)
	{
		bitstatus = SET;
	}
	else
	{
		bitstatus = RESET;
	}
	/* Return the flag status */
	return bitstatus;
}

/**
  * @brief
  * @param
  * @param
  * @retval
  */

ITStatus WDG_GetITStatus(CMSDK_WDG_TypeDef* WDGx, uint32_t WDG_IT)
{
	ITStatus bitstatus = RESET;
	uint32_t enablestatus = 0;
	/* Check the parameters */
	assert_param(IS_WDG_GET_IT(WDG_IT));
	assert_param(IS_WDG_ALL_PERIPH(WDGx));
	enablestatus = (uint32_t)(WDGx->CR & WDG_IT);
	if (((uint32_t)(WDGx->EIS & WDG_IT) != (uint32_t)RESET) && (enablestatus != (uint32_t)RESET))
	{
		bitstatus = SET;
	}
	else
	{
		bitstatus = RESET;
	}
	return  bitstatus;
}


/**
  * @}
  */

