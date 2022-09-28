  /******************************************************************************
  * @file    PT32X005_spi.c
  * @author  应用开发团队
  * @version V1.0.0
  * @date    2020/10/19
  * @brief    This file provides firmware functions to manage the following
  *          functionalities of the Serial peripheral interface (SPI):
  *           + Initialization and Configuration
  *           + Data transfers functions
  *           + Interrupts and flags management
  *            
  ******************************************************************************
  * @attention
  *
  *
  *****************************************************************************/
  
/* Includes ------------------------------------------------------------------*/
#include "PT32X005_spi.h"


/** @defgroup SPI
  * @brief SPI driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CR0_CLEAR_MASK       ((uint16_t)0x0030)
#define CR1_CLEAR_MASK2      ((uint16_t)0xFFFB)
#define CR2_LDMA_MASK        ((uint16_t)0x9FFF)

#define I2SCFGR_CLEAR_Mask   ((uint16_t)0xF040)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Fills each SPI_InitStruct member with its default value.
  * @param  SPI_InitStruct: pointer to a SPI_InitTypeDef structure which will be initialized.
  * @retval None
  */
void SPI_StructInit(SPI_InitTypeDef* SPI_InitStruct)
{
	/*--------------- Reset SPI init structure parameters values -----------------*/
	/* Initialize the SPI_Mode member */
	SPI_InitStruct->SPI_Mode = SPI_Mode_Slave;
	/* Initialize the SPI_DataSize member */
	SPI_InitStruct->SPI_DataSize = SPI_DataSize_8b;
	/* Initialize the SPI_CPOL member */
	SPI_InitStruct->SPI_CPOL = SPI_CPOL_Low;
	/* Initialize the SPI_CPHA member */
	SPI_InitStruct->SPI_CPHA = SPI_CPHA_1Edge;
	/* Initialize the SPI_NSS member */
	SPI_InitStruct->SPI_NSS = SPI_NSS_Hard;
	/* Initialize the SPI_BaudRatePrescaler member */
	SPI_InitStruct->SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	/* Initialize the SPI_BaudRatePrescaler member */
	SPI_InitStruct->SPI_BaudRatePostPrescaler = SPI_BaudRatePostPrescaler_1;
}

/**
  * @brief  Initializes the SPIx peripheral according to the specified
  *         parameters in the SPI_InitStruct.
  * @param  SPIx: where x can be 1 or 2 to select the SPI peripheral.
  * @note   SPI2 is not available for Taurus devices.
  * @param  SPI_InitStruct: pointer to a SPI_InitTypeDef structure that
  *         contains the configuration information for the specified SPI peripheral.
  * @retval None
  */
void SPI_Init(CMSDK_SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct)
{
	uint16_t tmpreg = 0;
	/* check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	/* Check the SPI parameters */
	assert_param(IS_SPI_MODE(SPI_InitStruct->SPI_Mode));
	assert_param(IS_SPI_DATA_SIZE(SPI_InitStruct->SPI_DataSize));
	assert_param(IS_SPI_CPOL(SPI_InitStruct->SPI_CPOL));
	assert_param(IS_SPI_CPHA(SPI_InitStruct->SPI_CPHA));
	//assert_param(IS_SPI_NSS(SPI_InitStruct->SPI_NSS));
	//assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_InitStruct->SPI_BaudRatePrescaler));
	//assert_param(IS_SPI_BAUDRATE_POSTPRESCALER(SPI_InitStruct->SPI_BaudRatePostPrescaler));
	/*---------------------------- SPIx CR1 Configuration ------------------------*/
	/* Get the SPIx CR0 value */
	tmpreg = SPIx->CR0;
	/* Clear DSS,CPOL,CPHA and post BaudRatePrescaler bits */
	tmpreg &= CR0_CLEAR_MASK;
	/* Configure SPIx: direction, NSS management, first transmitted bit, BaudRate prescaler
	master/slave mode, CPOL and CPHA */
	/* Set BIDImode, BIDIOE and RxONLY bits according to SPI_Direction value */
	/* Set SSM, SSI bit according to SPI_NSS values */
	/* Set LSBFirst bit according to SPI_FirstBit value */
	/* Set BR bits according to SPI_BaudRatePrescaler value */
	/* Set CPOL bit according to SPI_CPOL value */
	/* Set CPHA bit according to SPI_CPHA value */
	tmpreg |= (uint16_t)((uint32_t)SPI_InitStruct->SPI_DataSize | SPI_InitStruct->SPI_CPOL |
	                     SPI_InitStruct->SPI_CPHA | ((SPI_InitStruct->SPI_BaudRatePrescaler) << 8));
	/* Write to SPIx CR1 */
	SPIx->CR0 = tmpreg;
	/*-------------------------SPI Mode Configuration -----------------------*/
	/* Clear MS bits */
	SPIx->CR1 &= (uint16_t)~SPI_CR1_MSTR;
	/* Configure SPIx: Mode */
	SPIx->CR1 |= (uint16_t)(SPI_InitStruct->SPI_Mode);
	/*---------------------------- SPIx CPSR Configuration --------------------*/
	/* Write to SPIx Prescaler */
	SPIx->CPSR = SPI_InitStruct->SPI_BaudRatePostPrescaler;
	
	SPIx->CSCR = SPI_InitStruct->SPI_NSS;
}


/**
  * @brief  Enables or disables the specified SPI peripheral.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @param  NewState: new state of the SPIx peripheral.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_Cmd(CMSDK_SPI_TypeDef* SPIx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		/* Enable the selected SPI peripheral */
		SPIx->CR1 |= SPI_CR1_SPIEN;
	}
	else
	{
		/* Disable the selected SPI peripheral */
		SPIx->CR1 &= (uint16_t)~((uint16_t)SPI_CR1_SPIEN);
	}
}

/**
  * @brief  Enables or disables the TI Mode.
  *
  * @note   This function can be called only after the SPI_Init() function has
  *         been called.
  * @note   When TI mode is selected, the control bits SSM, SSI, CPOL and CPHA
  *         are not taken into consideration and are configured by hardware
  *         respectively to the TI mode requirements.
  *
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @param  NewState: new state of the selected SPI TI communication mode.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_TIModeCmd(CMSDK_SPI_TypeDef* SPIx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		/* Enable the TI mode for the selected SPI peripheral */
		SPIx->CR0 &= (uint16_t)~((uint16_t)SPI_CR0_FRF);
		SPIx->CR0 |= SPI_CR0_FRF_TI;
	}
	else
	{
		/* Disable the TI mode for the selected SPI peripheral */
		SPIx->CR0 &= (uint16_t)~((uint16_t)SPI_CR0_FRF);
	}
}

/**
  * @brief  Enables or disables the National Microwire Mode.
  *
  * @note   This function can be called only after the SPI_Init() function has
  *         been called.
  * @note   When Microwire mode is selected, the control bits SSM, SSI, CPOL and CPHA
  *         are not taken into consideration and are configured by hardware
  *         respectively to the Microwire mode requirements.
  *
  * @param  SPIx: where x can be 1 or 2 to select the SPI peripheral.
  * @note   SPI2 is not available for Tauru devices.
  * @param  NewState: new state of the selected SPI TI communication mode.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_MicroWireModeCmd(CMSDK_SPI_TypeDef* SPIx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		/* Enable the TI mode for the selected SPI peripheral */
		SPIx->CR0 &= (uint16_t)~((uint16_t)SPI_CR0_FRF);
		SPIx->CR0 |= SPI_CR0_FRF_MICROWIRE;
	}
	else
	{
		/* Disable the TI mode for the selected SPI peripheral */
		SPIx->CR0 &= (uint16_t)~((uint16_t)SPI_CR0_FRF);
	}
}

/**
  * @brief  Configures the data size for the selected SPI.
  * @param  SPIx: where x can be 0 or 1  to select the SPI peripheral.
  * @param  SPI_DataSize: specifies the SPI data size.
  *         For the SPIx peripheral this parameter can be one of the following values:
  *            @arg SPI_DataSize_4b: Set data size to 4 bits
  *            @arg SPI_DataSize_5b: Set data size to 5 bits
  *            @arg SPI_DataSize_6b: Set data size to 6 bits
  *            @arg SPI_DataSize_7b: Set data size to 7 bits
  *            @arg SPI_DataSize_8b: Set data size to 8 bits
  *            @arg SPI_DataSize_9b: Set data size to 9 bits
  *            @arg SPI_DataSize_10b: Set data size to 10 bits
  *            @arg SPI_DataSize_11b: Set data size to 11 bits
  *            @arg SPI_DataSize_12b: Set data size to 12 bits
  *            @arg SPI_DataSize_13b: Set data size to 13 bits
  *            @arg SPI_DataSize_14b: Set data size to 14 bits
  *            @arg SPI_DataSize_15b: Set data size to 15 bits
  *            @arg SPI_DataSize_16b: Set data size to 16 bits
  * @retval None
  */
void SPI_DataSizeConfig(CMSDK_SPI_TypeDef* SPIx, uint16_t SPI_DataSize)
{
	uint16_t tmpreg = 0;
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_SPI_DATA_SIZE(SPI_DataSize));
	/* Read the CR2 register */
	tmpreg = SPIx->CR0;
	/* Clear DS[3:0] bits */
	tmpreg &= (uint16_t)~SPI_CR0_DSS;
	/* Set new DS[3:0] bits value */
	tmpreg |= SPI_DataSize;
	SPIx->CR0 = tmpreg;
}


/**
  * @brief  Configures internally by software the NSS pin for the selected SPI.
  * @note   This function can be called only after the SPI_Init() function has
  *         been called.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @param  SPI_NSSInternalSoft: specifies the SPI NSS internal state.
  *          This parameter can be one of the following values:
  *            @arg SPI_NSSInternalSoft_Set: Set NSS pin internally
  *            @arg SPI_NSSInternalSoft_Reset: Reset NSS pin internally
  * @retval None
  */
void SPI_NSSInternalSoftwareConfig(CMSDK_SPI_TypeDef* SPIx, uint16_t SPI_NSSInternalSoft)
{
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_SPI_NSS_INTERNAL(SPI_NSSInternalSoft));
	if (SPI_NSSInternalSoft != SPI_NSSInternalSoft_Reset)
	{
		/* Set NSS pin internally by software */
		SPIx->CSCR |= SPI_NSSInternalSoft_Set;
	}
	else
	{
		/* Reset NSS pin internally by software */
		SPIx->CSCR &= SPI_NSSInternalSoft_Reset;
	}
}


/**
  * @brief  Transmits a Data through the SPIx/I2Sx peripheral.
  * @param  SPIx: where x can be 1 or 2 in SPI mode to select the SPI peripheral.
  * @param  Data: Data to be transmitted.
  * @retval None
  */
void SPI_SendData(CMSDK_SPI_TypeDef* SPIx, uint8_t Data)
{
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));

	/* Write in the DR register the data to be sent */
	SPIx->DR = Data;
}



/**
  * @brief  Returns the most recent received data by the SPIx/I2Sx peripheral.
  * @param  SPIx: where x can be 1 or 2 in SPI mode to select the SPI peripheral.
  * @retval The value of the received data.
  */
uint8_t SPI_ReceiveData(CMSDK_SPI_TypeDef* SPIx)
{
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));

	/* Return the data in the DR register */
	return SPIx->DR;
}


/**
  * @brief  Enables or disables the specified SPI interrupts.
  * @param  SPIx: where x can be 0 or 1 in SPI mode to select
  *         the SPI peripheral.
  * @param  SPI_IT: specifies the SPI interrupt source to be enabled or disabled.
  *          This parameter can be one of the following values:
  *            @arg SPI_IT_RFOF: Tx buffer empty interrupt mask
  *            @arg SPI_IT_RFNEOT: Rx buffer not empty interrupt mask
  *            @arg SPI_IT_RFHF: Error interrupt mask
  *            @arg SPI_IT_TFHF: Error interrupt mask
  * @param  NewState: new state of the specified SPI interrupt.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_ITConfig(CMSDK_SPI_TypeDef* SPIx, uint8_t SPI_IT, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	assert_param(IS_SPI_CONFIG_IT(SPI_IT));
	if (NewState != DISABLE)
	{
		/* Enable the selected SPI interrupt */
		SPIx->IE |= SPI_IT;
	}
	else
	{
		/* Disable the selected SPI interrupt */
		SPIx->IE &= (uint16_t)~SPI_IT;
	}
}


/**
  * @brief  Checks whether the specified SPI flag is set or not.
  * @param  SPIx: where x can be 1 or 2 in SPI mode to select
  *         the SPI peripheral.
  * @param  SPI_FLAG: specifies the SPI flag to check.
  *          This parameter can be one of the following values:
  *            @arg SPI_FLAG_TFE: Transmit buffer empty flag.
  *            @arg SPI_FLAG_TFNF: Receive buffer not empty flag.
  *            @arg SPI_FLAG_BUSY: Busy flag.
  *            @arg SPI_FLAG_RFNE: Overrun flag.
  *            @arg SPI_FLAG_RFF: Mode Fault flag.
  * @retval The new state of SPI_FLAG (SET or RESET).
  */
FlagStatus SPI_GetFlagStatus(CMSDK_SPI_TypeDef* SPIx, uint16_t SPI_FLAG)
{
	FlagStatus bitstatus = RESET;
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_SPI_GET_FLAG(SPI_FLAG));
	/* Check the status of the specified SPI flag */
	if ((SPIx->SR & SPI_FLAG) != (uint16_t)RESET)
	{
		/* */
		bitstatus = SET;
	}
	else
	{
		/* */
		bitstatus = RESET;
	}
	/*  */
	return  bitstatus;
}

/**
  * @brief  Clears the SPIx flag.
  * @param  SPIx: where x can be 1 or 2 to select the SPI peripheral.
  * @param  SPI_FLAG: specifies the SPI flag to clear.
  *         This function clears only CRCERR flag.
  * @note   OVR (OverRun error) flag is cleared by software sequence: a read
  *         operation to SPI_DR register (SPI_I2S_ReceiveData()) followed by
  *         a read operation to SPI_SR register (SPI_I2S_GetFlagStatus()).
  * @note   MODF (Mode Fault) flag is cleared by software sequence: a read/write
  *         operation to SPI_SR register (SPI_I2S_GetFlagStatus()) followed by
  *         a write operation to SPI_CR1 register (SPI_Cmd() to enable the SPI).
  * @retval None
  */
void SPI_ClearFlag(CMSDK_SPI_TypeDef* SPIx, uint16_t SPI_FLAG)
{
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_SPI_CLEAR_FLAG(SPI_FLAG));
	/* Clear the selected SPI CRC Error (CRCERR) flag */
	SPIx->ICR |= SPI_FLAG;
}

/**
  * @brief
  * @param
  * @param
  * @retval
  */
ITStatus SPI_GetITStatus(CMSDK_SPI_TypeDef* SPIx, uint32_t SPI_IT)
{
	ITStatus bitstatus = RESET;
	uint32_t enablestatus = 0;
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_SPI_GET_IT(SPI_IT));
	/*  */
	enablestatus = (uint32_t)(SPIx->IE & SPI_IT);
	/* */
	if (((uint32_t)(SPIx->EIS & SPI_IT) != (uint32_t)RESET) && (enablestatus != (uint32_t)RESET))
	{
		/*  */
		bitstatus = SET;
	}
	else
	{
		/* */
		bitstatus = RESET;
	}
	/*  */
	return  bitstatus;
}

/**
  * @}
  */

