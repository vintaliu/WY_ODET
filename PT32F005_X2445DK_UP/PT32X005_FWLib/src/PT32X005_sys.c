  /******************************************************************************
  * @file    PT32X005_sys.c
  * @author  应用开发团队
  * @version V1.0.0
  * @date    2020/10/19
  * @brief    This file provides firmware functions to manage the following
  *          functionalities of the SYS peripheral:
  *           + Initialization and Configuration
  *           + Interrupts and flags management
  *            
  ******************************************************************************
  * @attention
  *
  *
  *****************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "PT32X005_sys.h"


/** @defgroup SYS
  * @brief SYS driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief
  *
  *
  * @param  None
  * @retval None
  */
void System_GetClocksFreq(System_ClocksTypeDef * System_Clocks)
{
	uint8_t SystemCoreClockDiv;
	uint8_t PeripheralsClockDiv;
	uint8_t SystemCoreClockSrc;
	uint8_t DoubleClockSrc;
	/*   */
	SystemCoreClockSrc = (CMSDK_SYSCON->SYSCLK & SYSCON_SYSCLK_SYSCLKSEL);
	DoubleClockSrc =  (CMSDK_SYSCON->SYSCLK & SYSCON_SYSCLK_FDCLKSEL) >> 0x02;
	SystemCoreClockDiv = (CMSDK_SYSCON->SYSCLK & SYSCON_SYSCLK_HCLKDIV) >> 0x04;
	PeripheralsClockDiv = (CMSDK_SYSCON->SYSCLK & SYSCON_SYSCLK_PCLKDIV) >> 0x18;
	if (SystemCoreClockSrc == 0) /*   */
	{
		System_Clocks->SYSCLK_Frequency = IOSC_RC24M_VALUE / (SystemCoreClockDiv+1);
	}
	else if (SystemCoreClockSrc == 1) /*  */
	{
		System_Clocks->SYSCLK_Frequency = EOSC_CRYSTAL_VALUE/ (SystemCoreClockDiv+1);
	}
	else if (SystemCoreClockSrc == 2) /*  */
	{
		if(DoubleClockSrc == 0) /*  */
		{
			System_Clocks->SYSCLK_Frequency = (IOSC_RC24M_VALUE * 2) /(SystemCoreClockDiv+1);
		}
		else if(DoubleClockSrc == 1) /*  */
		{
			System_Clocks->SYSCLK_Frequency = (EOSC_CRYSTAL_VALUE * 2)/ (SystemCoreClockDiv+1);
		}
		else
		{
			System_Clocks->SYSCLK_Frequency = (I0SC_32K_VALUE * 2) /(SystemCoreClockDiv+1);
		}
	}
	else /*  */
	{
		System_Clocks->SYSCLK_Frequency = I0SC_32K_VALUE / (SystemCoreClockDiv+1);
	}
	System_Clocks->PCLK_Frequency = System_Clocks->SYSCLK_Frequency / (PeripheralsClockDiv+1);
}


/**
  * @brief  Returns the Remap Status.
  * @param  None
  * @retval Remap Status
  */
RemapStatus System_GetRemapStatus(void)
{
	uint32_t tmp;
	tmp = (CMSDK_SYSCON->REMAP & (0x00000001));
	if (tmp != RESET)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/**
  * @brief  Returns the Remap Status.
  * @param  None
  * @retval Remap Status
  */
ProtectStatus System_GetFlashProtectStatus(void)
{
	uint32_t tmp;
	tmp = (CMSDK_SYSCON->FLASH_PROTECT & (0x00000001));
	if (tmp != RESET)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}


/**
  * @brief  Reset Remap Bit.
  * @param  None
  * @retval None
  */
void System_ResetRemapBit(void)
{
	CMSDK_SYSCON->REMAP = RESET;
}


/**
  * @brief  Reset Remap Bit.
  * @param  None
  * @retval None
  */
void System_Retriming(void)
{
	CMSDK_SYSCON->RETRIMING = SYSCON_RETRIMING_KEY;
}


/*********************************************************************************************************//**
 * @brief Check whether the specific global reset flag is set or not.
 * @param RSTCU_RSTF: specify the reset flag.
 *        This parameter can be:
 *        @arg RSTCU_FLAG_SYSRST: Get system reset flag
 *        @arg RSTCU_FLAG_EXTRST: Get external pin reset flag
 *        @arg RSTCU_FLAG_WDTRST: Get WDT reset flag
 *        @arg RSTCU_FLAG_PORST : Get power on reset flag
 * @retval SET or RESET
 ************************************************************************************************************/
FlagStatus System_GetResetFlagStatus(RSTCU_RSTF_TypeDef RSTCU_RSTF)
{
	uint32_t tmp;
	/* Check the parameters */
	assert_param(IS_RSTCU_FLAG(RSTCU_RSTF));
	tmp = (CMSDK_SYSCON->RSTINFO & (1UL << RSTCU_RSTF));
	if (tmp != RESET)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/*********************************************************************************************************//**
 * @brief Clear the specific global reset flag.
 * @param RSTCU_RSTF: specify the reset flag.
 *        This parameter can be:
 *        @arg RSTCU_FLAG_SYSRST: Clear system reset flag
 *        @arg RSTCU_FLAG_EXTRST: Clear external pin reset flag
 *        @arg RSTCU_FLAG_WDTRST: Clear WDT reset flag
 *        @arg RSTCU_FLAG_PORST : Clear power on reset flag
 * @retval None
 ************************************************************************************************************/
void System_ClearResetFlag(RSTCU_RSTF_TypeDef RSTCU_RSTF)
{
	/* Check the parameters */
	assert_param(IS_RSTCU_FLAG(RSTCU_RSTF));
	CMSDK_SYSCON->RSTINFO = 1UL << RSTCU_RSTF;  /* Write 1 to clear */
}


/*********************************************************************************************************//**
 * @brief Clear all of the global reset flag.
 * @retval None
 ************************************************************************************************************/
void System_ClearAllResetFlag(void)
{
	CMSDK_SYSCON->RSTINFO = (uint32_t)0xFF ;   /* Write 1 to clear */
}


/**
  * @brief  Enables or disables the specified Reset Source.

  * @param  RstSource: specifies the Reset source to be enabled or disabled.
  *          This parameter can be one of the following values:
  *            @arg
  *            @arg
  *            @arg
  *            @arg
  * @param  NewState: new state of the specified RSTEN SOURCE
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void System_RestConfig(uint32_t RstSource, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	assert_param(IS_RSTEN_SOURCE(RstSource));
	if (NewState != DISABLE)
	{
		CMSDK_SYSCON->RSTENCTRL |= RstSource;
	}
	else
	{
		CMSDK_SYSCON->RSTENCTRL &= (uint16_t)~RstSource;
	}
}


/**
  * @brief  Returns the Remap Status.
  * @param  None
  * @retval Remap Status
  */
uint32_t System_GetCustomerID(void)
{
	return ( CMSDK_SYSCON->CUSTOMER_ID_INFO );
}


/**
  * @brief  Returns the Remap Status.
  * @param  None
  * @retval Remap Status
  */
uint32_t System_GetAgentID(void)
{
	return ( CMSDK_SYSCON->AGENT_ID_INFO );
}


/**
  * @brief  Returns the Remap Status.
  * @param  None
  * @retval Remap Status
  */
uint32_t System_GetUID(uint32_t ID)
{
	return ( (*(uint32_t*)(CMSDK_SYSCTRL_BASE + 0x030 + ID)));
}

/**
  * @brief  Returns the Remap Status.
  * @param  None
  * @retval Remap Status
  */
uint32_t System_GetCD(void)
{
	return ( CMSDK_SYSCON->CID_INFO );
}


/**
  * @}
  */

