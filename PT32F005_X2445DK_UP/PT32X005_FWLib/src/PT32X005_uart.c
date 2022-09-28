  /******************************************************************************
  * @file    PT32X005_uart.c
  * @author  应用开发团队
  * @version V1.0.0
  * @date    2020/10/19
  * @brief    This file provides firmware functions to manage the following
  *          functionalities of the Universal synchronous asynchronous receiver
  *          transmitter (USART):
  *           + Initialization and Configuration
  *           + STOP Mode
  *           + BaudRate
  *           + Data transfers
  *           + Multi-Processor Communication
  *           + Half-duplex mode
  *           + Smartcard mode
  *           + Interrupts and flags management
  *            
  ******************************************************************************
  * @attention
  *
  *
  *****************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "PT32X005_uart.h"


/** @defgroup UART
  * @brief UART driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Initializes the UARTx peripheral according to the specified
  *         parameters in the USART_InitStruct .
  * @param  UARTx: where x can be from 0 to 1 to select the UART peripheral.
  * @param  UART_InitStruct: pointer to a UART_InitTypeDef structure that contains
  *         the configuration information for the specified UART peripheral.
  * @retval None
  */
void UART_Init(CMSDK_UART_TypeDef* UARTx, UART_InitTypeDef* UART_InitStruct)
{
	uint32_t  tmpreg = 0, divider = 0, pclk = 0;
	System_ClocksTypeDef System_ClocksStatus;
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_BAUDRATE(UART_InitStruct->UART_BaudRate));
	assert_param(IS_UART_WORD_LENGTH(UART_InitStruct->UART_WordLength));
	assert_param(IS_UART_STOPBITS(UART_InitStruct->UART_StopBit));
	assert_param(IS_UART_SAMPLERATE(UART_InitStruct->UART_SampleRate));
	assert_param(IS_UART_RXSTATUS(UART_InitStruct->UART_RXEN));
	UARTx->RRR = UART_BRR_BRR;
	UARTx->TRR = UART_TXFR_TXFR;
	tmpreg &= ~(UART_CR_MODE | UART_CR_STOP | UART_CR_PBR);
	tmpreg |= (UART_InitStruct -> UART_WordLength | UART_InitStruct -> UART_StopBit | UART_InitStruct -> UART_Parity);
	UARTx->CR = tmpreg;
	if (UART_InitStruct -> UART_RXEN != DISABLE)
	{
		UARTx->CR |= UART_CR_RXEN;
	}
	else
	{
		UARTx->CR &= ~UART_CR_RXEN;
	}
	System_GetClocksFreq(&System_ClocksStatus);
	pclk = System_ClocksStatus.PCLK_Frequency;
	tmpreg  =  (pclk) % (UART_InitStruct->UART_SampleRate * UART_InitStruct->UART_BaudRate);
	divider =  (pclk) / (UART_InitStruct->UART_SampleRate * UART_InitStruct->UART_BaudRate);
	if (tmpreg > ((UART_InitStruct->UART_SampleRate / 2) * UART_InitStruct->UART_BaudRate))
	{
		UARTx->BRR = (uint16_t) (divider + 1);
	}
	else
	{
		UARTx->BRR = (uint16_t) divider;
	}
	switch (UART_InitStruct->UART_SampleRate)
	{
		case UART_SAMPLERATEX16 : /* 16 times sampling */
			UARTx->BRR &= ~UART_BRR_SR;
			UARTx->BRR |= UART_BRR_SRX16;
			break;
		case UART_SAMPLERATEX8 :  /* 8 times sampling */
			UARTx->BRR &= ~UART_BRR_SR;
			UARTx->BRR |= UART_BRR_SRX8;
			break;
		case UART_SAMPLERATEX4 :  /* 4 times sampling */
			UARTx->BRR &= ~UART_BRR_SR;
			UARTx->BRR |= UART_BRR_SRX4;
			break;
		default:
			break;
	}
}


/**
  * @brief  Fills each UART_InitStruct member with its default value.
  * @param  UART_InitStruct: pointer to a UART_InitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void UART_StructInit(UART_InitTypeDef* UART_InitStruct)
{
	/* USART_InitStruct members default value */
	UART_InitStruct->UART_BaudRate = 19200;
	UART_InitStruct->UART_WordLength = UARTM_8D_P;
	UART_InitStruct->UART_StopBit = UART1StopBit;
	UART_InitStruct->UART_Parity = UART_ODD_PARITY;
	UART_InitStruct->UART_RXEN = ENABLE;
	UART_InitStruct->UART_SampleRate = UART_SAMPLERATEX16;
}

/**
  * @brief  Enables or disables the specified UART peripheral.
  * @param  UARTx: where x can be from 0 to 1 to select the UART peripheral.
  * @param  NewState: new state of the USARTx peripheral.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_Cmd(CMSDK_UART_TypeDef* UARTx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		/* Enable the selected USART by setting the UE bit in the CR1 register */
		UARTx->CR |= UART_CR_BRUN;
	}
	else
	{
		/* Disable the selected USART by clearing the UE bit in the CR1 register */
		UARTx->CR &= (uint32_t)~((uint32_t)UART_CR_BRUN);
	}
}

/**
  * @brief  Enables or disables the UART's  receiver.
  * @param  NewState: new state of the UART transfer direction.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_RxConfig(CMSDK_UART_TypeDef* UARTx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		/* Enable the UART's transfer interface as Receiver by setting the RXEN bits
		   in the UART CR register */
		UARTx->CR |= UART_CR_RXEN;
	}
	else
	{
		/* Disable the UART's transfer interface as Receiver by setting the RXEN bits
		   in the UART CR register */
		UARTx->CR &= (uint32_t)~UART_CR_RXEN;
	}
}

/**
  * @brief  Enables or disables the Pin(s) active level inversion.
  * @param  USARTx: where x can be from 1 to 0 to select the UART peripheral.
  * @param  USART_InvPin: specifies the USART pin(s) to invert.
  *          This parameter can be any combination of the following values:
  *            @arg UART_InvPin_Tx: UART Tx pin active level inversion.
  *            @arg UART_InvPin_Rx: UART Rx pin active level inversion.
  * @param  NewState: new active level status for the UART pin(s).
  *          This parameter can be:
  *            @arg ENABLE: pin(s) signal values are inverted (Vdd =0, Gnd =1).
  *            @arg DISABLE: pin(s) signal works using the standard logic levels (Vdd =1, Gnd =0).
  * @note   This function has to be called before calling UART_Cmd() function.
  * @retval None
  */

void USART_InvPinCmd(CMSDK_UART_TypeDef* UARTx, uint32_t UART_InvPin, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_INVERSTION_PIN(UART_InvPin));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		/* Enable the active level inversion for selected pins by setting the TXPOL
		   and/or RXPOL bits in the UART CR register */
		UARTx->CR |= UART_InvPin;
	}
	else
	{
		/* Disable the active level inversion for selected requests by clearing the
		   TXPOL and/or RXPOL bits in the USART CR2 register */
		UARTx->CR &= (uint32_t)~UART_InvPin;
	}
}

/**
  * @brief  Sets the receiver Time Out value.
  * @param  UARTx: where x can be 0 or 1 to select the UART peripheral.
  * @param  UART_ReceiverTimeOut: specifies the Receiver Time Out value.
  * @retval None
  */
void UART_SetReceiverTimeOut(CMSDK_UART_TypeDef* UARTx, uint32_t UART_ReceiverTimeOut)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_TIMEOUT(UART_ReceiverTimeOut));
	/* Clear the receiver Time Out value by clearing the RTO[23:0] bits in the RTOR
	   register  */
	UARTx->TOR &= (uint32_t)~((uint32_t)UART_TO_TO);
	/* Set the receiver Time Out value by setting the RTO[23:0] bits in the RTOR
	   register  */
	UARTx->TOR |= UART_ReceiverTimeOut;
}




/**
  * @brief  Sets the specified UART guard time.
  * @param  UARTx: where x can be 0 or 1 to select the UART peripheral.
  * @param  UART_GuardTime: specifies the guard time.
  * @retval None
  */
void UART_SetGuardTime(CMSDK_UART_TypeDef* UARTx, uint8_t UART_GuardTime)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	/* Clear the USART Guard time */
	UARTx->GTR &= ~UART_GTR_GTR;
	/* Set the USART guard time */
	UARTx->GTR |= (uint8_t)UART_GuardTime;
}



/**
  * @brief  Transmits single data through the UARTx peripheral.
  * @param  UARTx: where x can be from 0 or 1 to select the UART peripheral.
  * @param  Data: the data to transmit.
  * @retval None
  */
void UART_SendData(CMSDK_UART_TypeDef* UARTx, uint16_t Data)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_DATA(Data));
	/* While the TxFIFO contain 8 characters. */
	while((UARTx->SR & UART_ISR_TXF));
	/* Transmit Data */
	UARTx->BR = (Data & (uint16_t)0x01FF);
}

/**
  * @brief  Transmits single string through the UARTx peripheral.
  * @param  UARTx: where x can be from 0 or 1 to select the UART peripheral.
  * @param  String: Pointer to the string to transmit.
  * @retval None
  */

void UART_StringSend(CMSDK_UART_TypeDef* UARTx, uint8_t *String)
{
	while(*String != '\0')
	{
		while((UARTx->SR & UART_ISR_TXF));
		UART_SendData(UARTx, *String);
		String++;
	}
	UART_SendData(UARTx, 0x00);
}


/**
  * @brief  Returns the most recent received data by the UARTx peripheral.
  * @param  UARTx: where x can be from 0 to 1 to select the UART peripheral.
  * @retval The received data.
  */
uint16_t UART_ReceiveData(CMSDK_UART_TypeDef* UARTx)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	/* Receive Data */
	return (uint16_t)(UARTx->BR & (uint16_t)0x01FF);
}

/**
  * @brief  Returns the most recent received data by the UARTx peripheral.
  * @param  UARTx: where x can be from 0 to 1 to select the UART peripheral.
  * @Data   Data: Pointer to the receivde data.
  * @param  UART_ReceiverTimeOut: The time out period.
  * @retval The received data.
  */
uint16_t UART_ReceiveData_Timeout(CMSDK_UART_TypeDef* UARTx, uint16_t *Data, uint32_t UART_ReceiverTimeOut)
{
	uint16_t wStatus;
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	/* Reload the Timeout counter */
	UART_SetReceiverTimeOut(UARTx, UART_ReceiverTimeOut);
	/* while the RxFIFO is empty and no Timeoutidle */
	while (!((wStatus = UARTx->SR) & (UART_ISR_TOIDLE | UART_ISR_RXHF | UART_ISR_RXNE))); // while the RxFIFO is empty and no Timeoutidle
	/* Receive Data */
	*Data = (uint16_t)(UARTx->BR & (uint16_t)0x01FF);
	return (wStatus);
}

/**
  * @brief
  * @param  UARTx: where x can be from 0 to 1 to select the UART peripheral.
  * @Data   Data: Pointer to the reveive data.
  * @retval The received data.
  */
uint16_t UART_StringReceive(CMSDK_UART_TypeDef* UARTx, uint8_t *Data)
{
	uint8_t *pSTRING;
	uint16_t wStatus;
	pSTRING = Data;
	do
	{
		/* while the RxFIFO is empty and no Timeoutidle */
		while (!((wStatus = UARTx->SR) & (UART_ISR_RXHF | UART_ISR_RXNE))); //
		/* Read the Receive Buffer */
		*(pSTRING++) = (uint8_t)UARTx->BR;
	}
	while((*(pSTRING - 1) != 0x0D) & (*(pSTRING - 1) != '\0'));
	*(pSTRING - 1) = '\0';
	return wStatus;
}

/**
  * @brief  This function the loop back mode of the selected UART.
  * @param  UARTx: where x can be from 0 to 1 to select the UART peripheral.
  * @param  NewState: new active level status for the UART pin(s).
  *          This parameter can be:
  *            @arg ENABLE: Loop back mode .
  *            @arg DISABLE: Normal communication mode .
  * @retval The received data.
  */
void UART_LoopBackConfig(CMSDK_UART_TypeDef* UARTx, FunctionalState NewStatus)
{
	if (NewStatus != DISABLE)
	{
		UARTx->CR |= UART_CR_LPB;
	}
	else
	{
		UARTx->CR &= ~UART_CR_LPB;
	}
}

/**
  * @brief  This function reset the Rx and the Tx FIFOs
  * @param  UARTx: where x can be from 0 to 1 to select the UART peripheral.
  * @param  FIFO: Transmit FIFO or receive FIFO for the UART .
  *          This parameter can be:
  *            @arg Rx_FIFO: Receive FIFO .
  *            @arg Tx_FIFO: Transmit FIFO .
  * @retval None.
  */
void UART_FifoReset(CMSDK_UART_TypeDef * UARTx,  UART_FifoTypeDef FIFO)
{
	switch (FIFO)
	{
		case Rx_FIFO :
			UARTx->RRR = UART_RXFR_RXFR;
			break;
		case Tx_FIFO :
			UARTx->TRR = UART_TXFR_TXFR;
			break;
	}
}


/**
  * @brief  Enables or disables the UART's IrDA interface.
  * @param  UARTx: where x can be 0 to select the UART peripheral.
  * @param  NewState: new state of the IrDA mode.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_IrDACmd(CMSDK_UART_TypeDef* UARTx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_USART_0_PERIPH(UARTx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		/* Enable the IrDA mode by setting the EN bit in the IRR register */
		UARTx->IR_CR |= UART_IRR_EN;
	}
	else
	{
		/* Disable the IrDA mode by clearing the EN bit in the IRR register */
		UARTx->IR_CR &= (uint32_t)~((uint32_t)UART_IRR_EN);
	}
}

/**
  * @brief  Configures the IrDA's Pin polarity
  * @param  UARTx: where x can be 0 to select the UART peripheral.
  * @param  NewState: new defined levels for the USART data.
  *          This parameter can be:
  *            @arg ENABLE: pin(s) signal values are inverted (Vdd =0, Gnd =1).
  *            @arg pin(s) signal works using the standard logic levels (Vdd =1, Gnd =0).
  * @note   This function has to be called before calling UART_Cmd() function.
  * @retval None
  */
void UART_IrDAInvPinCmd(CMSDK_UART_TypeDef* UARTx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_USART_0_PERIPH(UARTx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		/* Enable the binary data inversion feature by setting the DATAINV bit in
		   the CR2 register */
		UARTx->IR_CR |= UART_IRR_EN;
	}
	else
	{
		/* Disable the binary data inversion feature by clearing the DATAINV bit in
		   the CR2 register */
		UARTx->IR_CR &= (uint32_t)~((uint32_t)UART_IRR_EN);
	}
}

/**
  * @brief  Sets the system clock prescaler.
  * @param  USARTx: where x can be 1, 2 or 3 to select the USART peripheral.
  * @param  USART_Prescaler: specifies the prescaler clock.
  * @note   This function has to be called before calling USART_Cmd() function.
  * @retval None
  */
void UART_SetDutyCycle(CMSDK_UART_TypeDef* UARTx, uint8_t UART_DutyCycle)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_DUTYCYCLE(UART_DutyCycle));
	/* Clear the IrDA's modulation PWM duty cycle */
	UARTx->IRTX_PWMC &= ~UART_IRPWMC_IRPWMC;
	/* Set the IrDA's modulation PWM duty cycle*/
	UARTx->IRTX_PWMC |= (uint32_t)((uint32_t)UART_DutyCycle);
}


/**
  * @brief  Enables or disables the UART's Half Duplex communication.
  * @param  UARTx: where x can be from 0 to 1 to select the UART peripheral.
  * @param  NewState: new state of the UART Communication.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_HalfDuplexCmd(CMSDK_UART_TypeDef* UARTx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		/* Enable the Half-Duplex mode by setting the OWE bit in the CR register */
		UARTx->CR |= UART_CR_OWE;
	}
	else
	{
		/* Disable the Half-Duplex mode by clearing the OWE bit in the CR register */
		UARTx->CR  &= (uint32_t)~((uint32_t)UART_CR_OWE);
	}
}

/**
  * @brief  Enables or disables the UART's transmitter or receiver.
  * @param  UARTx: where x can be from 0 to 1 to select the UART peripheral.
  * @param  UART_Direction: specifies the UART direction.
  *          This parameter can be any combination of the following values:
  *            @arg UART_Mode_Tx: USART Transmitter
  *            @arg UART_Mode_Rx: USART Receiver
  * @param  NewState: new state of the USART transfer direction.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_HalfDuplex_DirectionModeCmd(CMSDK_UART_TypeDef* UARTx, uint32_t UART_DirectionMode)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_MODE(UART_DirectionMode));
	switch (UART_DirectionMode)
	{
		/* Enable the UART's transfer interface as Transmitter by setting the OWD bits
		   in the UART CR register */
		case UART_Mode_Tx :
			UARTx->CR |= UART_CR_OWD;
			break;
		/* Disable the UART's transfer interface as Receiver by clearing OWD bits
		   in the USART CR register */
		case UART_Mode_Rx :
			UARTx->CR &= (uint32_t)~UART_CR_OWD;
			break;
		default :
			break;
	}
}


/**
  * @brief  Enables or disables the specified USART interrupts.
  * @param  UARTx: where x can be from 1 to 8 to select the USART peripheral.
  * @param  UART_IT: specifies the UART interrupt sources to be enabled or disabled.
  *          This parameter can be one of the following values:
  *            @arg UART_IT_RXNEIE: specifies the interrupt source for Receiver FIFO buffer non-empty interrupt.
  *            @arg UART_IT_RXHFIE: specifies the interrupt source for Receiver FIFO buffer half-full interrupt.
  *            @arg UART_IT_RXFIE: specifies the interrupt source for Receiver FIFO buffer full interrupt.
  *            @arg UART_IT_TONEIE: specifies the interrupt source for Receive Receiver FIFO buffer cleared timeout interrupt.
  *            @arg UART_IT_TOIDLEIE: specifies the interrupt source for Receiver FIFO buffer idel interrupt.
  *            @arg UART_IT_PERRIE: specifies the interrupt source for Parity error interrupt.
  *            @arg UART_IT_FERRIE: specifies the interrupt source for Frame error interrupt.
  *            @arg UART_IT_OVERRIE: specifies the interrupt source for Receiver FIFO buffer overflow interrupt.
  *            @arg UART_IT_TXEIE: specifies the interrupt source for Transmitter FIFO buffer empty interrupt.
  *            @arg UART_IT_TXHEIE: specifies the interrupt source for Transmitter FIFO buffer half-empty interrupt.
  *            @arg UART_IT_TXFIE: specifies the interrupt source for Transmitter FIFO buffer full interrupt.
  *            @arg UART_IT_TXENDIE: specifies the interrupt source for Transfer completed interrupt
  * @param  NewState: new state of the specified USARTx interrupts.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_ITConfig(CMSDK_UART_TypeDef* UARTx, uint32_t UART_IT, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_CONFIG_IT(UART_IT));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		UARTx->IER |= UART_IT;
	}
	else
	{
		UARTx->IER &= ~UART_IT;
	}
}


/**
  * @brief  Checks whether the specified UART flag is set or not.
  * @param  UARTx: where x can be from 0 to 1 to select the UART peripheral.
  * @param  UART_FLAG: specifies the flag to check.
  *          This parameter can be one of the following values:
  *            @arg UART_FLAG_RXNE:  Receiver FIFO buffer non-empty flag.
  *            @arg UART_FLAG_RXHF:  Receiver FIFO buffer half-full flag.
  *            @arg UART_FLAG_RXF:   Receiver FIFO buffer full.
  *            @arg UART_FLAG_TONE:  Receiver FIFO buffer cleared timeout.
  *            @arg UART_FLAG_TOIDLE:  Receiver FIFO buffer idel.
  *            @arg UART_FLAG_PERR:  Parity error.
  *            @arg UART_FLAG_FERR:  Frame error.
  *            @arg UART_FLAG_OVERR: Receiver FIFO buffer overflow.
  *            @arg UART_FLAG_TXE:   Transmitter FIFO buffer empty.
  *            @arg UART_FLAG_TXHE:  Transmitter FIFO buffer half-empty.
  *            @arg UART_FLAG_TXF:   Transmitter FIFO buffer full.
  *            @arg UART_FLAG_TXEND: Transfer completed.
  * @retval The new state of UART_FLAG (SET or RESET).
  */
FlagStatus UART_GetFlagStatus(CMSDK_UART_TypeDef* UARTx, uint32_t UART_FLAG)
{
	FlagStatus bitstatus = RESET;
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_FLAG(UART_FLAG));
	if ((UARTx->SR & UART_FLAG) != (uint16_t)RESET)
	{
		bitstatus = SET;
	}
	else
	{
		bitstatus = RESET;
	}
	return bitstatus;
}

/**
  * @brief  Clears the UARTx's pending flags.
  * @param  UARTx: where x can be from 0 to 1 to select the USART peripheral.
  * @param  USART_FLAG: specifies the flag to clear.
  *          This parameter can be any combination of the following values:
  *            @arg UART_CLEAR_FLAG_PERR:  Parity error.
  *            @arg UART_CLEAR_FLAG_FERR:  Frame error.
  *            @arg UART_CLEAR_FLAG_OVERR: Receiver FIFO buffer overflow.
  * @retval None
  */
void UART_ClearFlag(CMSDK_UART_TypeDef* UARTx, uint32_t UART_CLEAR_FLAG)
{
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_CLEAR_FLAG(UART_CLEAR_FLAG));
	UARTx->SR = UART_CLEAR_FLAG;
}

/**
  * @brief
  * @param
  * @param
  * @retval
  */
ITStatus UART_GetITStatus(CMSDK_UART_TypeDef* UARTx, uint32_t UART_IT)
{
	ITStatus bitstatus = RESET;
	uint32_t enablestatus = 0;
	/* Check the parameters */
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_GET_IT(UART_IT));
	/**/
	enablestatus = (uint32_t)(UARTx->IER & UART_IT);
	/*  */
	if (((uint32_t)(UARTx->SR & UART_IT) != (uint32_t)RESET) && (enablestatus != (uint32_t)RESET))
	{
		/* */
		bitstatus = SET;
	}
	else
	{
		/* */
		bitstatus = RESET;
	}
	/**/
	return  bitstatus;
}

/**
  * @}
  */

