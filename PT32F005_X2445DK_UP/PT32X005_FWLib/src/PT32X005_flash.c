/******************************************************************************
  * @file    PT32X005_flash.c
  * @author  应用开发团队
  * @version V1.0.0
  * @date    2020/10/19
  * @brief    This file provides firmware functions to manage the following
  *          functionalities of the FLASH peripheral:
  *           + Initialization and Configuration
  *           + Flash Erase and Read\Write Function
  *           + Interrupts and flags management
  *          
  ******************************************************************************
  * @attention
  *
  *
  *****************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "PT32X005_flash.h"


/** @defgroup FLASH
  * @brief FLASH driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/**
  * @brief  Sets the code latency value.
  * @param  FLASH_Latency: specifies the FLASH Latency value.
  *          This parameter can be one of the following values:
  *             @arg FLASH_Latency_0: FLASH Zero Latency cycle
  *             @arg FLASH_Latency_1: FLASH One Latency cycle
  * @retval None
  */
void FLASH_SetLatency(uint32_t FLASH_Latency)
{
	uint32_t tmpreg = 0;
	/* Check the parameters */
	assert_param(IS_FLASH_LATENCY(FLASH_Latency));
	/* Read the ACR register */
	tmpreg = CMSDK_FLASH->ACR;
	/* Sets the Latency value */
	tmpreg &= (uint32_t) (~((uint32_t)FLASH_ACR_LATENCY));
	tmpreg |= FLASH_Latency;
	/* Write the ACR register */
	CMSDK_FLASH->ACR = tmpreg;
}

/**
  * @brief  Returns the FLASH Status.
  * @param  None
  * @retval FLASH Status: The returned value can be:
  *         FLASH_BUSY, FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP or FLASH_COMPLETE.
  */
FLASH_Status FLASH_GetStatus(void)
{
	FLASH_Status FLASHstatus = FLASH_COMPLETE;
	if((CMSDK_FLASH->ISR & FLASH_FLAG_BUSY) == FLASH_FLAG_BUSY)
	{
		FLASHstatus = FLASH_BUSY;
	}
	else
	{
		if((CMSDK_FLASH->ISR & (uint32_t)(FLASH_FLAG_KEYER)) != (uint32_t)0x00)
		{
			FLASHstatus = FLASH_KEY_ERR;
		}
		else
		{
			if((CMSDK_FLASH->ISR & (uint32_t)(FLASH_FLAG_ADER)) != (uint32_t)0x00)
			{
				FLASHstatus = FLASH_ADDR_ERR;
			}
			else
			{
				FLASHstatus = FLASH_COMPLETE;
			}
		}
	}
	/* Return the FLASH Status */
	return FLASHstatus;
}

/**
  * @brief  Waits for a FLASH operation to complete or a TIMEOUT to occur.
  * @param  Timeout: FLASH programming Timeout
  * @retval FLASH Status: The returned value can be: FLASH_BUSY,
  *         FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_WaitForLastOperation(uint32_t Timeout)
{
	FLASH_Status status = FLASH_COMPLETE;
	/* Check for the FLASH Status */
	status = FLASH_GetStatus();
	/* Wait for a FLASH operation to complete or a TIMEOUT to occur */
	while((status == FLASH_BUSY) && (Timeout != 0x00))
	{
		status = FLASH_GetStatus();
		Timeout--;
	}
	if(Timeout == 0x00 )
	{
		status = FLASH_TIMEOUT;
	}
	/* Return the operation status */
	return status;
}


/**
  * @brief  Erases a specified page in program memory.
  * @note   To correctly run this function, the FLASH_Unlock() function must be called before.
  * @note   Call the FLASH_Lock() to disable the flash memory access (recommended
  *         to protect the FLASH memory against possible unwanted operation)
  * @param  Page_Address: The page address in program memory to be erased.
  * @note   A Page is erased in the Program memory only if the address to load
  *         is the start address of a page (multiple of 1024 bytes).
  * @retval FLASH Status: The returned value can be:
  *         FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ErasePage(uint32_t Page_Address)
{
  uint32_t tmpreg = 0;	
	FLASH_Status status = FLASH_COMPLETE;
	/* Check the parameters */
	assert_param(IS_FLASH_PROGRAM_ADDRESS(Page_Address));
	/* Wait for last operation to be completed */
	status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);	//等待上一个操作完毕
  tmpreg = CMSDK_FLASH->ACR;

  tmpreg &= ~ (0x000010F);	
		
	if(status == FLASH_COMPLETE)
	{
		CMSDK_FLASH->AR  = Page_Address;
		if(((Page_Address) <  MAINCODE_END_ADDR))	//擦除地址小于Boot 程序存放地址范围(即主程序区域内的地址)
		{
			/* The main code area is erased by page */
			tmpreg |= FLASH_MAIN_FKEY | FLASH_OPMOD_WR | FLASH_OPCMD_PERA | FLASH_OPSTR_STRT;
			CMSDK_FLASH->ACR =tmpreg;
			status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
		}
		else											//擦除地址大于Boot 程序存放地址范围(即用户信息区域内的地址)
		{
				status = FLASH_ADDR_ERR;
		}
	}
	/* Return the Erase Status */
	return status;
}

/**
  * @brief  Erases all FLASH pages.
  * @note   To correctly run this function, the FLASH_Unlock() function must be called before.
  * @note   Call the FLASH_Lock() to disable the flash memory access (recommended
  *         to protect the FLASH memory against possible unwanted operation)
  * @param  None
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EraseAllPages(void)
{
  uint32_t tmpreg = 0;	
	FLASH_Status status = FLASH_COMPLETE;
  tmpreg = CMSDK_FLASH->ACR;	
  tmpreg &= ~ (0x000010F);		
	/* Wait for last operation to be completed */
	status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
	if(status == FLASH_COMPLETE)
	{
		/* if the previous operation is completed, proceed to erase all pages */
	  tmpreg |= FLASH_MAIN_FKEY | FLASH_OPMOD_WR | FLASH_OPCMD_MERA | FLASH_OPSTR_STRT;
		CMSDK_FLASH->ACR =tmpreg;		
		/* Wait for last operation to be completed */
		status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
	}
	/* Return the Erase Status */
	return status;
}


/**
  * @brief  Programs a word at a specified address.
  * @note   To correctly run this function, the FLASH_Unlock() function must be called before.
  * @note   Call the FLASH_Lock() to disable the flash memory access (recommended
  *         to protect the FLASH memory against possible unwanted operation)
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
	FLASH_Status status = FLASH_COMPLETE;
	__IO uint32_t tmp = 0;
  uint32_t tmpreg = 0;	
  tmpreg = CMSDK_FLASH->ACR;	
  tmpreg &= ~ (0x000010F);	
	/* Check the parameters */
	assert_param(IS_FLASH_PROGRAM_ADDRESS(Address));
	/* Wait for last operation to be completed */
	status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
	if(status == FLASH_COMPLETE)
	{
		/* if the previous operation is completed, proceed to erase all pages */
		CMSDK_FLASH->AR  = Address;
		CMSDK_FLASH->DR  = Data;
		if(((Address) < MAINCODE_END_ADDR) && ((Address & 0x03) == 0))
		{
			/* Data is programming into the main code area */
			tmpreg |=FLASH_MAIN_FKEY | FLASH_OPMOD_WR | FLASH_OPCMD_PRO | FLASH_OPSTR_STRT;
			CMSDK_FLASH->ACR = tmpreg;
			status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
		}
		else
		{
				status = FLASH_ADDR_ERR;
		}
	}
	/* Return the Program Status */
	return status;
}


/**
  * @brief  Read a word at a specified address.
  * @note
  * @note
  *
  * @param
  * @note
  *
  * @retval
  *
  */
uint32_t FLASH_ReadWord(uint32_t Address)
{
	return *(__IO uint32_t*) Address;
}
/**
  * @brief  Read a half word at a specified address.
  * @note
  * @note
  *
  * @param
  * @param
  * @retval
  *
  */
uint16_t FLASH_ReadHalfWord(uint32_t Address)
{
	return *(__IO uint16_t*) Address;
}

/**
  * @brief Read a Byte at a specified address.
  * @note
  * @note
  *
  * @param
  * @param
  * @retval
  *
  */
uint8_t FLASH_ReadByte(uint32_t Address)
{
	return *(__IO uint8_t*) Address;
}


/**
  * @brief  Enables or disables the specified FLASH interrupts.
  * @param  FLASH_IT: specifies the FLASH interrupt sources to be enabled or
  *         disabled.
  *          This parameter can be any combination of the following values:
  *             @arg FLASH_IT_EOP: FLASH end of programming Interrupt
  *             @arg FLASH_IT_ERR: FLASH Error Interrupt
  * @retval None
  */
void FLASH_ITConfig(uint32_t FLASH_IT, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_FLASH_IT(FLASH_IT));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if(NewState != DISABLE)
	{
		/* Enable the interrupt sources */
		CMSDK_FLASH->IER |= FLASH_IT;
	}
	else
	{
		/* Disable the interrupt sources */
		CMSDK_FLASH->IER &= ~(uint32_t)FLASH_IT;
	}
}

/**
  * @brief  Checks whether the specified FLASH flag is set or not.
  * @param  FLASH_FLAG: specifies the FLASH flag to check.
  *          This parameter can be one of the following values:
  *             @arg FLASH_FLAG_BSY: FLASH write/erase operations in progress flag
  *             @arg FLASH_FLAG_PGERR: FLASH Programming error flag flag
  *             @arg FLASH_FLAG_WRPERR: FLASH Write protected error flag
  *             @arg FLASH_FLAG_EOP: FLASH End of Programming flag
  * @retval The new state of FLASH_FLAG (SET or RESET).
  */
FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG)
{
	FlagStatus bitstatus = RESET;
	/* Check the parameters */
	assert_param(IS_FLASH_GET_FLAG(FLASH_FLAG));
	if((CMSDK_FLASH->ISR & FLASH_FLAG) != (uint32_t)RESET)
	{
		bitstatus = SET;
	}
	else
	{
		bitstatus = RESET;
	}
	/* Return the new state of FLASH_FLAG (SET or RESET) */
	return bitstatus;
}

/**
  * @brief  Clears the FLASH's pending flags.
  * @param  FLASH_FLAG: specifies the FLASH flags to clear.
  *          This parameter can be any combination of the following values:
  *             @arg FLASH_FLAG_PGERR: FLASH Programming error flag flag
  *             @arg FLASH_FLAG_WRPERR: FLASH Write protected error flag
  *             @arg FLASH_FLAG_EOP: FLASH End of Programming flag
  * @retval None
  */
void FLASH_ClearFlag(uint32_t FLASH_FLAG)
{
	/* Check the parameters */
	assert_param(IS_FLASH_CLEAR_FLAG(FLASH_FLAG));
	/* Clear the flags */
	CMSDK_FLASH->ISR = FLASH_FLAG;
}



/**
  * @}
  */