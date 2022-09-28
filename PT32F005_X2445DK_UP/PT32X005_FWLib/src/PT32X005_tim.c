  /******************************************************************************
  * @file    PT32X005_tim.c
  * @author  应用开发团队
  * @version V1.0.0
  * @date    2020/10/19
  * @brief    This file provides firmware functions to manage the following
  *          functionalities of the TIM peripheral:
  *            + TimeBase management
  *            + Output Compare management
  *            + Input Capture management
  *            + Interrupts, flags management
  *            + Clocks management
  *            + Synchronization management
  *            + Specific interface management
  *            + Specific remapping management
  *            
  ******************************************************************************
  * @attention
  *
  *
  *****************************************************************************/
  

/* Includes ------------------------------------------------------------------*/
#include "PT32X005_tim.h"



/** @defgroup TIM
  * @brief TIM driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Initializes the TIMx Time Base Unit peripheral according to
  *         the specified parameters in the TIM_TimeBaseInitStruct.
  * @param  TIMx: where x can be 2, 3 and 4 to select the TIM peripheral
  * @note   TIM4 can only TIM_CNT_MODE_UP mode
  * @param  TimeBaseInit: pointer to a TIM_TimeBaseInitTypeDef
  *         structure that contains the configuration information for
  *         the specified TIM peripheral.
  * @retval None
  */

void TIM_TimeBaseInit(CMSDK_TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TimeBaseInit)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	assert_param(IS_TIM_COUNTER_MATCH(TimeBaseInit->MatchValue));
	assert_param(IS_TIM_PRESCALER(TimeBaseInit->Prescaler));
	assert_param(IS_TIM_COUNTERMODE(TimeBaseInit->CounterMode));
	assert_param(IS_FUNCTIONAL_STATE(TimeBaseInit->OVRSTEN));
	/* Set the counter match value */
	TIMx->MR0 = TimeBaseInit->MatchValue;
	/* Set the Prescaler value */
	TIMx->PR = TimeBaseInit->Prescaler;
	/* Select the Counter Mode */
	TIMx->MCR &= ~TIM_MCR_DIR;
	TIMx->MCR |=  (TimeBaseInit->CounterMode);
	if((TimeBaseInit->OVRSTEN) == ENABLE)
	{
		TIMx->MCR |= TIM_MCR_MR0R;
	}
	else
	{
		TIMx->MCR &= ~TIM_MCR_MR0R;
	}
}


/**
  * @brief  Fills each TIM_TimeBaseInitStruct member with its default value.
  * @param  TIM_TimeBaseInitStruct: pointer to a TIM_TimeBaseInitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void TIM_TimeBaseStructInit(TIM_TimeBaseInitTypeDef* TimeBaseInit)
{
	/* Set the default configuration */
	TimeBaseInit->MatchValue = 0xFFFF;
	TimeBaseInit->Prescaler = 0x0000;
	TimeBaseInit->CounterMode = TIM_CNT_MODE_UP;
	TimeBaseInit->OVRSTEN = ENABLE;
}


/**
  * @brief  Configures the TIMx Prescaler.
  * @param  TIMx: where x can be 0, 1 to select the TIM peripheral.
  * @param  Prescaler: specifies the Prescaler Register value (0~255)
  * @note   The Prescaler is loaded at the PC update event or immediatly
  * @retval None
  */
void TIM_PrescalerConfig(CMSDK_TIM_TypeDef* TIMx, uint8_t Prescaler)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	assert_param(IS_TIM_PRESCALER(Prescaler));
	/* Set the Prescaler value */
	TIMx->PR = Prescaler;
}


/**
  * @brief  Configures the TIMx Match Value.
  * @param  TIMx: where x can be 2, 3 and 4 to select the TIM peripheral
  * @param  Reload: specifies the Counter reload value (0~65535)
  * @note   The value is loaded at the update event or immediatly
  * @retval None
  */

void TIM_SetCounterMatch(CMSDK_TIM_TypeDef* TIMx, uint16_t Reload)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	assert_param( IS_TIM_COUNTER_MATCH(Reload));
	/* Set the counter reload value */
	TIMx->MR0 = Reload;
}

/**
  * @brief  Configures the TIMx Current Value.
  * @param  TIMx: where x can be 2, 3 to select the TIM peripheral
  * @param  Counter: specifies the Counter register new value.
  * @note   TIM4 is not allowed to set current cnt
  * @retval None
  */
void TIM_SetCounter(CMSDK_TIM_TypeDef* TIMx, uint16_t Counter)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	assert_param(IS_TIM_COUNTER_VALUE(Counter));
	/* Set the Counter Register value */
	TIMx->TC = Counter;
}

/**
  * @brief  Gets the TIMx Counter value.
  * @param  TIMx: where x can be 2, 3 and 4 to select the TIM peripheral
  * @retval Counter Register value.
  */
uint16_t TIM_GetCounter(CMSDK_TIM_TypeDef* TIMx)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	/* Get the Counter Register value */
	return  TIMx->TC;
}

/**
  * @brief  Gets the TIMx Prescaler value.
  * @param  TIMx: where x can be 2, 3 and 4 to select the TIM peripheral
  * @retval Prescaler Register value.
  */
uint16_t TIM_GetPrescaler(CMSDK_TIM_TypeDef* TIMx)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	/* Get the Prescaler Register value */
	return  TIMx->PR;
}

/**
  * @brief  Gets the TIMx Prescaler CNT value.
  * @param  TIMx: where x can be 2, 3 and 4 to select the TIM peripheral
  * @retval Prescaler Current Register value.
  */
uint16_t TIM_GetPrescalerCnt(CMSDK_TIM_TypeDef* TIMx)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	/* Get the Prescaler Register value */
	return  TIMx->PC;
}

/**
  * @brief  Specifies the TIMx Counter Mode to be used.
  * @param  TIMx: where x can be 2, 3 to select the TIM peripheral
  * @note   TIM4 is not allowed to set counter mode
  * @param  CounterMode: specifies the Counter Mode to be used
  *          This parameter can be one of the following values:
  *            @arg TIM_CNT_MODE_UP: TIM Up Counting Mode
  *            @arg TIM_CNT_MODE_DOWN: TIM Down Counting Mode
  * @retval None
  */
void TIM_CounterModeConfig(CMSDK_TIM_TypeDef* TIMx, uint8_t CounterMode)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	assert_param(IS_TIM_COUNTERMODE(CounterMode));
	/* Select the Counter Mode */
	TIMx->MCR  &= ~TIM_MCR_DIR;
	TIMx->MCR  |=  CounterMode;
}

/**
  * @brief  Enable or Disable update settiongs of the TIMx
  * @param  TIMx: where x can be 2, 3 and 4 to select the TIM peripheral
  * @param  NewState: This parameter can be ENABLE or DISABLE.
  * @retval None
  */

void TIM_UpdateCmd(CMSDK_TIM_TypeDef* TIMx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState == ENABLE)
	{
		/* Set the update bit */
		TIMx->TCR |= TIM_TCR_UPDATE;
	}
	else
	{
		/* Reset the update bit */
		TIMx->TCR &= (~TIM_TCR_UPDATE);
	}
}

/**
  * @brief  Enable or Disable TIMx Counter
  * @param  TIMx: where x can be 2, 3 and 4 to select the TIM peripheral
  * @param  NewState: This parameter can be ENABLE or DISABLE.
  * @retval None
  */

void TIM_Cmd(CMSDK_TIM_TypeDef* TIMx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState == ENABLE)
	{
		/* Set the update bit */
		TIMx->TCR |= TIM_TCR_EN;
	}
	else
	{
		/* Reset the update bit */
		TIMx->TCR &= (~TIM_TCR_EN);
	}
}

/**
  * @brief  Specifies the TIMx Counter Mode to be used.
  * @param  TIMx: where x can be 2, 3 and 4 to select the TIM peripheral
  * @param  TIM_INT: Specify the TIM interrupts sources to be enabled or disabled.
  *          This parameter can be one of the following values:
  *        @arg TIM_INT_MR0      : TIM MR0 value match interrupt
  * @param  NewState: This parameter can be ENABLE or DISABLE.
  * @retval None
  */

void TIM_ITConfig(CMSDK_TIM_TypeDef* TIMx, uint32_t TIM_IT, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	assert_param(IS_TIM_INT(TIM_IT));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		/* Enable the interrupt sources */
		TIMx->MCR |= TIM_IT;
	}
	else
	{
		/* Disable the interrupt sources */
		TIMx->MCR &= ~TIM_IT;
	}
}

/**
  * @brief  Specifies the TIMx Counter Mode to be used.
  * @param  TIMx: where x can be 2, 3 and 4 to select the TIM peripheral
  * @param  TIM_FLAG: Specify the flag to be checked.
  *         This parameter can be one of the following values:
  *         @arg TIM_FLAG_MR0       : TIM MR0 value match flag
  * @return FlagStatus of TIM_FLAG (SET or RESET).
  */
FlagStatus TIM_GetFlagStatus(CMSDK_TIM_TypeDef* TIMx, uint32_t TIM_FLAG)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	assert_param(IS_TIM_FLAG(TIM_FLAG));
	if ((TIMx->ISR & TIM_FLAG) != 0)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/**
  * @brief  Specifies the TIMx Counter Mode to be used.
  * @param  TIMx: where x can be 2, 3 and 4 to select the TIM peripheral
  * @param  TIM_FLAG: Specify the flag to be cleared.
  *         This parameter can be one of the following values:
  *         @arg TIM_FLAG_MR0       : TIM MR0 value match flag
  * @retval None
  */
void TIM_ClearFlag(CMSDK_TIM_TypeDef* TIMx, uint32_t TIM_FLAG)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	assert_param(IS_TIM_FLAG(TIM_FLAG));
	/* Clear the flags */
	TIMx->ISR = TIM_FLAG;
}


/**
  * @}
  */
  

