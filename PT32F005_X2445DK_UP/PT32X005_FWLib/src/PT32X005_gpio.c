  /******************************************************************************
  * @file    PT32X005_gpio.c
  * @author  应用开发团队
  * @version V1.0.0
  * @date    2020/10/19
  * @brief    This file provides firmware functions to manage the following
  *          functionalities of the GPIO peripheral:
  *           + Initialization and Configuration functions
  *           + GPIO Read and Write functions
  *           + GPIO Alternate functions configuration functions
  *          
  ******************************************************************************
  * @attention
  *
  *
  *****************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "PT32X005_gpio.h"


/** @defgroup GPIO
  * @brief GPIO driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/**
  * @brief  Initializes the CMSDK_Px peripheral according to the specified
  *         parameters in the GPIO_InitStruct.
  * @param  CMSDK_Px: where x can be (A, B, C, D, E or F) to select the GPIO peripheral.
  * @param  GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure that contains
  *         the configuration information for the specified GPIO peripheral.
  * @retval None
  */
void GPIO_Init(CMSDK_GPIO_TypeDef* CMSDK_Px, GPIO_InitTypeDef* GPIO_InitStruct)
{
	uint32_t currentpin = 0x00;
	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	assert_param(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin));
	assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
	assert_param(IS_GPIO_PUPD(GPIO_InitStruct->GPIO_PuPd));
	/*-------------------------- Configure the port pins -----------------------*/
	/*-- GPIO Mode Configuration --*/
	/* Get the port pins position */
	currentpin = GPIO_InitStruct->GPIO_Pin;
	if (((GPIO_InitStruct->GPIO_Mode) == GPIO_OType_PP) || ((GPIO_InitStruct->GPIO_Mode) == GPIO_OType_OD))
	{
		/* Output mode configuration */
		CMSDK_Px->OUTENABLESET = currentpin;
		CMSDK_Px->ANACLR = currentpin;
		if((GPIO_InitStruct->GPIO_Mode) == GPIO_OType_OD)
		{
			CMSDK_Px->ODRSET = currentpin;
		}
		else
		{
			CMSDK_Px->ODRCLR = currentpin;
		}
		if(GPIO_InitStruct->GPIO_SR == GPIO_Slew_Rate_High)
		{
			CMSDK_Px->SRCLR = currentpin;
			CMSDK_Px->DRCLR = currentpin;
		}
		else
		{
			CMSDK_Px->SRSET = currentpin;
			CMSDK_Px->DRSET = currentpin;
		}
	}
	else if ((GPIO_InitStruct->GPIO_Mode) == GPIO_Mode_AN)
	{
		CMSDK_Px->OUTENABLECLR = currentpin;
		CMSDK_Px->ANASET = currentpin;
	}
	else
	{
		CMSDK_Px->OUTENABLECLR = currentpin;
		CMSDK_Px->ANACLR = currentpin;
		switch(((uint32_t)GPIO_InitStruct->GPIO_PuPd))
		{
			case GPIO_PuPd_NOPULL :
				CMSDK_Px->PULLDOWNCLR = currentpin;
				CMSDK_Px->PULLUPCLR = currentpin;
				break;
			case GPIO_PuPd_UP :
				CMSDK_Px->PULLUPSET = currentpin;
				CMSDK_Px->PULLDOWNCLR = currentpin;
				break;
			case GPIO_PuPd_DOWN :
				CMSDK_Px->PULLDOWNSET = currentpin;
				CMSDK_Px->PULLUPCLR = currentpin;
				break;
		}
	}
}
/**
  * @brief  Fills each GPIO_InitStruct member with its default value.
  * @param  GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure which will
  *         be initialized.
  * @retval None
  */
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct)
{
	/* Reset GPIO init structure parameters values */
	GPIO_InitStruct->GPIO_Pin  = GPIO_Pin_All;
	GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct->GPIO_PuPd = GPIO_PuPd_NOPULL;
}

/**
  * @brief  Fills each GPIO_InitStruct member with its default value.
  * @param  GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure which will
  *         be initialized.
  * @retval None
  */
void GPIO_AnalogFunctionConfig(CMSDK_GPIO_TypeDef* CMSDK_Px, uint32_t GPIO_Pin, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	assert_param(IS_GPIO_PIN(GPIO_Pin));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		CMSDK_Px->ANASET = GPIO_Pin;
	}
	else
	{
		CMSDK_Px->ANACLR = GPIO_Pin;
	}
}
/**
  * @brief  Fills each GPIO_InitStruct member with its default value.
  * @param  GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure which will
  *         be initialized.
  * @retval None
  */
void GPIO_PullUpConfig(CMSDK_GPIO_TypeDef* CMSDK_Px, uint32_t GPIO_Pin, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	assert_param(IS_GPIO_PIN(GPIO_Pin));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		CMSDK_Px->PULLDOWNCLR = GPIO_Pin;
		CMSDK_Px->PULLUPSET = GPIO_Pin;
	}
	else
	{
		CMSDK_Px->PULLDOWNCLR = GPIO_Pin;
		CMSDK_Px->PULLUPCLR = GPIO_Pin;
	}
}

/**
  * @brief  Fills each GPIO_InitStruct member with its default value.
  * @param  GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure which will
  *         be initialized.
  * @retval None
  */
void GPIO_PullDownConfig(CMSDK_GPIO_TypeDef* CMSDK_Px, uint32_t GPIO_Pin, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	assert_param(IS_GPIO_PIN(GPIO_Pin));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		CMSDK_Px->PULLUPCLR = GPIO_Pin;
		CMSDK_Px->PULLDOWNSET = GPIO_Pin;
	}
	else
	{
		CMSDK_Px->PULLDOWNCLR = GPIO_Pin;
		CMSDK_Px->PULLUPCLR = GPIO_Pin;
	}
}


/**
  * @brief  Reads the specified input port pin.
  * @param  CMSDK_Px: where x can be (A, B, C, D, E or F) to select the GPIO peripheral.
  * @note
  * @note
  * @param  GPIO_Pin: specifies the port bit to read.
  * @note   This parameter can be GPIO_Pin_x where x can be:
  *         For PT32F031: (0..15) for CMSDK_PA, (0..7) for CMSDK_PB,(0..1) for CMSDK_PC
  * @retval The input port pin value.
  */
uint8_t GPIO_ReadInputDataBit(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin)
{
	uint8_t bitstatus = 0x00;
	/* 检查函数参数是否正确 */
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
	if ((CMSDK_Px->DATA & GPIO_Pin) != (uint32_t)Bit_RESET)
	{
		bitstatus = (uint8_t)Bit_SET;
	}
	else
	{
		bitstatus = (uint8_t)Bit_RESET;
	}
	return bitstatus;
}

/**
  * @brief  Reads the specified input port pin.
  * @param  CMSDK_Px: where x can be (A, B, C, D, E or F) to select the GPIO peripheral.
  * @note   CMSDK_PC,CMSDK_PD,CMSDK_PE is available only for PT32F031.
  * @note
  * @retval The input port pin value.
  */
uint16_t GPIO_ReadInputData(CMSDK_GPIO_TypeDef* CMSDK_Px)
{
	/* 检查函数参数是否正确 */
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	return ((uint16_t)CMSDK_Px->DATA);
}

/**
  * @brief  Reads the specified output data port bit.
  * @param  CMSDK_Px: where x can be (A, B, C, D, E or F) to select the GPIO peripheral.
  * @note
  * @param  GPIO_Pin: Specifies the port bit to read.
  * @note   This parameter can be GPIO_Pin_x where x can be:
  *         For PT32F031: (0..15) for CMSDK_PA, (0..7) for CMSDK_PB,(0..1) for CMSDK_PC
  *
  *
  * @retval The output port pin value.
  */
uint8_t GPIO_ReadOutputDataBit(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin)
{
	uint8_t bitstatus = 0x00;
	/* 检查函数参数是否正确 */
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
	if ((CMSDK_Px->DATA & GPIO_Pin) != (uint32_t)Bit_RESET)
	{
		bitstatus = (uint8_t)Bit_SET;
	}
	else
	{
		bitstatus = (uint8_t)Bit_RESET;
	}
	return bitstatus;
}

/**
  * @brief  Reads the specified GPIO output data port.
  * @param  CMSDK_Px: where x can be (A, B, C, D, E or F) to select the GPIO peripheral.
  * @note
  * @note
  * @retval GPIO output data port value.
  */
uint16_t GPIO_ReadOutputData(CMSDK_GPIO_TypeDef* CMSDK_Px)
{
	/* 检查函数参数是否正确 */
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	return ((uint16_t)CMSDK_Px->DATA);
}

/**
  * @brief  Sets the selected data port bits.
  * @param  CMSDK_Px: where x can be (A, B, C, D, E or F) to select the GPIO peripheral.
  * @note
  * @note
  * @param  GPIO_Pin: specifies the port bits to be written.
  * @note   This parameter can be GPIO_Pin_x where x can be:
  *         For PT32F031: (0..15) for CMSDK_PA, (0..7) for CMSDK_PB,(0..1) for CMSDK_PF
  *
  *
  * @retval None
  */
void GPIO_SetBits(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin)
{
	/* 检查函数参数是否正确 */
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	assert_param(IS_GPIO_PIN(GPIO_Pin));
	CMSDK_Px->DATA |= GPIO_Pin;
}

/**
  * @brief  Clears the selected data port bits.
  * @param  CMSDK_Px: where x can be (A, B, C, D, E or F) to select the GPIO peripheral.
  * @note
  * @note
  * @param  GPIO_Pin: specifies the port bits to be written.
  * @note   This parameter can be GPIO_Pin_x where x can be:
  *         For PT32F031: (0..15) for CMSDK_PA, (0..7) for CMSDK_PB,(0..1) for CMSDK_PF
  *
  *
  * @retval None
  */
void GPIO_ResetBits(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin)
{
	/* 检查函数参数是否正确 */
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	assert_param(IS_GPIO_PIN(GPIO_Pin));
	CMSDK_Px->DATA &= ~GPIO_Pin;
}
/**
  * @brief  Reverse the selected data port bits.
  * @param  CMSDK_Px: where x can be (A, B, C, D, E or F) to select the GPIO peripheral.
  * @note
  * @note
  * @param  GPIO_Pin: specifies the port bits to be written.
  * @note   This parameter can be GPIO_Pin_x where x can be:
  *         For PT32F031: (0..15) for CMSDK_PA, (0..7) for CMSDK_PB,(0..1) for CMSDK_PF
  *
  *
  * @retval None
  */
void GPIO_ReverseBits(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin)
{
	/* 检查函数参数是否正确 */
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	assert_param(IS_GPIO_PIN(GPIO_Pin));
	CMSDK_Px->DATA ^= GPIO_Pin;
}

/**
  * @brief  Sets or clears the selected data port bit.
  * @param  CMSDK_Px: where x can be (A, B, C, D, E or F) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bit to be written.
  * @param  BitVal: specifies the value to be written to the selected bit.
  *          This parameter can be one of the BitAction enumeration values:
  *            @arg Bit_RESET: to clear the port pin
  *            @arg Bit_SET: to set the port pin
  * @note   This parameter can be GPIO_Pin_x where x can be:
  *         For PT32F031: (0..15) for CMSDK_PA, (0..7) for CMSDK_PB,(0..1) for CMSDK_PF
  *
  *
  * @retval None
  */
void GPIO_WriteBit(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin, BitAction BitVal)
{
	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
	assert_param(IS_GPIO_BIT_ACTION(BitVal));
	if (BitVal != Bit_RESET)
	{
		CMSDK_Px->DATA |= GPIO_Pin;
	}
	else
	{
		CMSDK_Px->DATA &= ~GPIO_Pin;
	}
}

/**
  * @brief  Writes data to the specified GPIO data port.
  * @param  CMSDK_Px: where x can be (A, B, C, D, E or F) to select the GPIO peripheral.
  * @note
  * @note
  * @param  PortVal: specifies the value to be written to the port output data register.
  * @retval None
  */
void GPIO_Write(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t PortVal)
{
	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	CMSDK_Px->DATA = PortVal;
}


/**
  * @brief  Writes data to the specified GPIO data port.
  * @param  CMSDK_Px: where x can be (A, B, C, D, E or F) to select the GPIO peripheral.
  * @note
  * @param  GPIO_PinSource: specifies the pin for the Alternate function.
  *          This parameter can be GPIO_PinSourcex where x can be (0..15) for CMSDK_PA and (0..7) for CMSDK_PB and (0..1) for CMSDK_PF
  * @param  GPIO_AF: selects the pin to used as Alternate function.
  *          This parameter can be one of the following value:
  *            @arg GPIO_AF_0:  OSC_IN，OSC_OUT，UART0_TX，UART0_RX，SPI0_CS，SPI0_SCK
  *                             SPI0_MISO，SPI0_MOSI，IR_TXD，MCO，CMP0_OUT，CMP1_OUT
  *                             I2C1_SCL，I2C1_SDA，SWDIO，SWCLK，TIM0_BKIN，TIM0_CH1
  *                             TIM0_CH2，TIM0_CH3，TIM0_CH4，BEEP
  *            @arg GPIO_AF_1: I2C0_SDA，I2C0_SCL，TIM0_CH1N，TIM0_CH2N，TIM0_CH3
  *                            TIM0_CH4N，UART1_TX，UART1_RX，TIM1_BKIN，TIM1_CH1
  *                            TIM1_CH2N，TIM1_CH3N，TIM1_CH1，TIM1_CH2，TIM1_CH3
  *                            TIM1_CH4，TIM1_CH4N，IR_TXD，SPI1_CS，SPI1_SCK
  *                            SPI1_MISO，SPI1_MOSI，I2C1_SCL，I2C1_SDA，MCO
  *            @arg GPIO_AF_2: LED-DATA_A，LED-DATA_B，LED-DATA_C，LED-DATA_D
  *                            LED-DATA_E，LED-DATA_F，LED_DATA_DP，LED-DATA_G
  *                            ADC_ETR，LED-COM0，LED-COM2，LED-COM3，I2C0_SCL
  *                            I2C0_SDA，UART1_TX，UART1_RX，UART1_TX，UART1_RX
  *                            LED-COM1
  *            @arg GPIO_AF_3: UART1_TX，UART1_RX，UART0_TX，UART0_RX，UART1_TX
  *                            UART1_RX，UART0_TX，UART0_RX，UART0_TX，UART0_RX
  *            @arg GPIO_AF_4: I2C1_SCL,I2C1_SDA,I2C0_SCL,I2C0_SDA
  *            @arg GPIO_AF_5: MCO
  *            @arg GPIO_AF_6: RESEVR
  *            @arg GPIO_AF_7: RESEVR
  * @note   The pin should already been configured in Alternate Function mode(AF)
  *         using GPIO_InitStruct->GPIO_Mode = GPIO_Mode_AF
  * @note   Refer to the Alternate function mapping table in the device datasheet
  *         for the detailed mapping of the system and peripherals'alternate
  *         function I/O pins.
  * @retval None
  */
void GPIO_PinAFConfig(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_PinSource, uint8_t GPIO_AF)
{
	uint32_t temp = 0x00;
	uint32_t temp_2 = 0x00;
	uint32_t afr_tmp[2] = { 0x00, 0x00 };
	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
	assert_param(IS_GPIO_AF(GPIO_AF));
	temp = ((uint32_t)(GPIO_AF) << ((uint32_t)((uint32_t)GPIO_PinSource & (uint32_t)0x07) * 4));
	afr_tmp[GPIO_PinSource >> 0x03] &= ~((uint32_t)0xF << ((uint32_t)((uint32_t)GPIO_PinSource & (uint32_t)0x07) * 4));
	temp_2 = afr_tmp[GPIO_PinSource >> 0x03] | temp;
	afr_tmp[GPIO_PinSource >> 0x03] = temp_2;
	CMSDK_Px->ALTFUNCCLR =(0x01<<GPIO_PinSource);
	CMSDK_Px->ALTFUNCSET0 = afr_tmp[0];
}


void GPIO_ClearPinAF(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
     
  CMSDK_Px->ALTFUNCCLR = GPIO_Pin;
}

/**
  * @brief  Enables or disables the specified GPIO interrupts.
  * @param  CMSDK_Px: where x can be (A, B, C, D, E or F) to select the GPIO peripheral.
  * @param  GPIO_IT: specifies the GPIO interrupt sources to be enabled or disabled.
  *          This parameter can be one of the following values:
  *            @arg GPIO_IT_IT0  specifies the interrupt source for CMSDK_Px0 interrupt
  *            @arg GPIO_IT_IT1  specifies the interrupt source for CMSDK_Px1 interrupt
  *            @arg GPIO_IT_IT2  specifies the interrupt source for CMSDK_Px2 interrupt
  *            @arg GPIO_IT_IT3  specifies the interrupt source for CMSDK_Px3 interrupt
  *            @arg GPIO_IT_IT4  specifies the interrupt source for CMSDK_Px4 interrupt
  *            @arg GPIO_IT_IT5  specifies the interrupt source for CMSDK_Px5 interrupt
  *            @arg GPIO_IT_IT6  specifies the interrupt source for CMSDK_Px6.interrupt
  *            @arg GPIO_IT_IT7  specifies the interrupt source for CMSDK_Px7 interrupt
  *            @arg GPIO_IT_IT8  specifies the interrupt source for CMSDK_Px8 interrupt
  *            @arg GPIO_IT_IT9  specifies the interrupt source for CMSDK_Px9 interrupt
  *            @arg GPIO_IT_IT10 specifies the interrupt source for CMSDK_Px10 interrupt
  *            @arg GPIO_IT_IT11 specifies the interrupt source for CMSDK_Px11 interrupt
  *            @arg GPIO_IT_IT12 specifies the interrupt source for CMSDK_Px12 interrupt
  *            @arg GPIO_IT_IT13 specifies the interrupt source for CMSDK_Px13 interrupt
  *            @arg GPIO_IT_IT14 specifies the interrupt source for CMSDK_Px14 interrupt
  *            @arg GPIO_IT_IT15 specifies the interrupt source for CMSDK_Px15 interrupt
  * @param  NewState: new state of the specified GPIO interrupts.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void GPIO_ITConfig(CMSDK_GPIO_TypeDef* CMSDK_Px, uint32_t GPIO_IT, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	assert_param(IS_GPIO_CONFIG_IT(GPIO_IT));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		CMSDK_Px->INTENSET = GPIO_IT;
	}
	else
	{
		CMSDK_Px->INTENCLR = GPIO_IT;
	}
}


/**
  * @brief
  * @param
  * @note
  * @note
  * @param  GPIO_Pin: specifies the port bit to be written.
  * @param  BitVal: specifies the value to be written to the selected bit.
  *          This parameter can be one of the BitAction enumeration values:
  *            @arg Bit_RESET: to clear the port pin
  *            @arg Bit_SET: to set the port pin
  * @note   This parameter can be GPIO_Pin_x where x can be:
  *         For PT32F031: (0..15) for CMSDK_PA, (0..7) for CMSDK_PB,(0..1) for CMSDK_PF
  *
  *
  * @retval None
  */
void GPIO_TriTypeConfig(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin, uint32_t Trigger_Type)
{
	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
	assert_param(IS_GPIOTI_TRIGGER(Trigger_Type));
	switch( Trigger_Type )
	{
		case GPIOTI_Trigger_Rising :
			CMSDK_Px->INTTYPESET = GPIO_Pin;
			CMSDK_Px->INTPOLSET =  GPIO_Pin;
			CMSDK_Px->INTTYPECLR1 = GPIO_Pin;
			break;
		case GPIOTI_Trigger_Falling :
			CMSDK_Px->INTTYPESET = GPIO_Pin;
			CMSDK_Px->INTPOLCLR =  GPIO_Pin;
			CMSDK_Px->INTTYPECLR1 = GPIO_Pin;
			break;
		case GPIOTI_Trigger_Rising_Falling :
			CMSDK_Px->INTTYPESET1 = GPIO_Pin;
			break;
		case GPIOTI_Trigger_High_Level :
			CMSDK_Px->INTTYPECLR = GPIO_Pin;
			CMSDK_Px->INTPOLSET =  GPIO_Pin;
			CMSDK_Px->INTTYPECLR1 = GPIO_Pin;
			break;
		case GPIOTI_Trigger_Low_Level :
			CMSDK_Px->INTTYPECLR = GPIO_Pin;
			CMSDK_Px->INTPOLCLR =  GPIO_Pin;
			CMSDK_Px->INTTYPECLR1 = GPIO_Pin;
			break;
	}
}

/**
  * @brief  Checks whether the specified GPIO interrupt flag is set or not.
  * @param  CMSDK_Px: where x can be (A, B, C, D, E or F) to select the GPIO peripheral.
  * @param  GPIO_IT_FLAG: specifies the flag to check.
  *          This parameter can be one of the following values:
  *            @arg GPIO_IT_PR0    Port interrupt pending bit set, bit 0
  *            @arg GPIO_IT_PR1    Port interrupt pending bit set, bit 1
  *            @arg GPIO_IT_PR2    Port interrupt pending bit set, bit 2
  *            @arg GPIO_IT_PR3    Port interrupt pending bit set, bit 3
  *            @arg GPIO_IT_PR4    Port interrupt pending bit set, bit 4
  *            @arg GPIO_IT_PR5    Port interrupt pending bit set, bit 5
  *            @arg GPIO_IT_PR6    Port interrupt pending bit set, bit 6
  *            @arg GPIO_IT_PR7    Port interrupt pending bit set, bit 7
  *            @arg GPIO_IT_PR8    Port interrupt pending bit set, bit 8
  *            @arg GPIO_IT_PR9    Port interrupt pending bit set, bit 9
  *            @arg GPIO_IT_PR10   Port interrupt pending bit set, bit 10
  *            @arg GPIO_IT_PR11   Port interrupt pending bit set, bit 11
  *            @arg GPIO_IT_PR12   Port interrupt pending bit set, bit 12
  *            @arg GPIO_IT_PR13   Port interrupt pending bit set, bit 13
  *            @arg GPIO_IT_PR14   Port interrupt pending bit set, bit 14
  *            @arg GPIO_IT_PR15   Port interrupt pending bit set, bit 15
  * @retval The new state of GPIO_IT_FLAG (SET or RESET).
  */
FlagStatus GPIO_GetFlagStatus(CMSDK_GPIO_TypeDef* CMSDK_Px, uint32_t GPIO_IT_FLAG)
{
	FlagStatus bitstatus = RESET;
	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	assert_param(IS_GPIO_GET_FLAG(GPIO_IT_FLAG));
	if ((CMSDK_Px->INTSTATUS & GPIO_IT_FLAG) != (uint16_t)RESET)
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
  * @brief  Clears the GPIO's pending flags.
  * @param  CMSDK_Px: where x can be (A, B, C, D, E or F) to select the GPIO peripheral.
  * @param  GPIO_IT_FLAG: specifies the flag to clear.
  *          This parameter can be any combination of the following values:
  *            @arg GPIO_IT_PR0    Port interrupt pending bit set, bit 0
  *            @arg GPIO_IT_PR1    Port interrupt pending bit set, bit 1
  *            @arg GPIO_IT_PR2    Port interrupt pending bit set, bit 2
  *            @arg GPIO_IT_PR3    Port interrupt pending bit set, bit 3
  *            @arg GPIO_IT_PR4    Port interrupt pending bit set, bit 4
  *            @arg GPIO_IT_PR5    Port interrupt pending bit set, bit 5
  *            @arg GPIO_IT_PR6    Port interrupt pending bit set, bit 6
  *            @arg GPIO_IT_PR7    Port interrupt pending bit set, bit 7
  *            @arg GPIO_IT_PR8    Port interrupt pending bit set, bit 8
  *            @arg GPIO_IT_PR9    Port interrupt pending bit set, bit 9
  *            @arg GPIO_IT_PR10   Port interrupt pending bit set, bit 10
  *            @arg GPIO_IT_PR11   Port interrupt pending bit set, bit 11
  *            @arg GPIO_IT_PR12   Port interrupt pending bit set, bit 12
  *            @arg GPIO_IT_PR13   Port interrupt pending bit set, bit 13
  *            @arg GPIO_IT_PR14   Port interrupt pending bit set, bit 14
  *            @arg GPIO_IT_PR15   Port interrupt pending bit set, bit 15
  * @retval None
  */
void GPIO_ClearFlag(CMSDK_GPIO_TypeDef* CMSDK_Px, uint32_t GPIO_IT_FLAG) 
{
	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	assert_param(IS_GPIO_CLEAR_FLAG(GPIO_IT_FLAG));
	CMSDK_Px->INTCLEAR = GPIO_IT_FLAG;
}

/**
  * @brief
  * @param
  * @param
  * @retval
  */
ITStatus GPIO_GetITStatus(CMSDK_GPIO_TypeDef* CMSDK_Px, uint32_t GPIO_IT)
{
	ITStatus bitstatus = RESET;
	uint32_t enablestatus = 0;
	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	assert_param(IS_GPIO_GET_IT(GPIO_IT));
	/* */
	enablestatus = (uint32_t)(CMSDK_Px->INTENSET & GPIO_IT);
	/* */
	if (((uint32_t)(CMSDK_Px->INTSTATUS & GPIO_IT) != (uint32_t)RESET) && (enablestatus != (uint32_t)RESET))
	{
		/* */
		bitstatus = SET;
	}
	else
	{
		/* */
		bitstatus = RESET;
	}
	/* */
	return  bitstatus;
}

/**
  * @brief
  * @param
  * @param

  * @retval None
  */
void GPIO_ClearITPendingBit(CMSDK_GPIO_TypeDef* CMSDK_Px, uint32_t GPIO_IT)
{
	/* */
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	assert_param(IS_GPIO_CLEAR_IT(GPIO_IT));
	/*  */
	CMSDK_Px->INTCLEAR = (uint32_t)GPIO_IT;
}


/**
  * @brief
  * @param
  * @param
  * 同时对一个IO端口的低8位(bit0~bit7)进行置位操作(1~8位),同时不受中断干扰
  * @retval None
  */
void GPIO_LowByte_SetBit(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin)
{
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	CMSDK_Px->LB_MASKED[GPIO_Pin & 0xFF] = GPIO_Pin;
}
/**
  * @brief
  * @param
  * @param
  * 同时对一个IO端口的低8位(bit0~bit7)进行置位清除操作(1~8位),同时不受中断干扰
  * @retval None
  */
void GPIO_LowByte_ClrBit(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin)
{
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	CMSDK_Px->LB_MASKED[GPIO_Pin & 0xFF] = ~GPIO_Pin;
}
/**
  * @brief
  * @param
  * @param
  * 同时对一个IO端口的低8位(bit0~bit7)进行置位写入操作(1~8位),同时不受中断干扰
  * @retval None
  */
void GPIO_LowByte_WriteBit(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin,uint8_t bitDat)
{
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	CMSDK_Px->LB_MASKED[GPIO_Pin & 0xFF] = bitDat;
}

/**
  * @brief
  * @param
  * @param
  * 同时对一个IO端口的高8位(bit0~bit7)进行置位操作(1~8位),同时不受中断干扰
  * @retval None
  */
void GPIO_HigByte_SetBit(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin)
{
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	CMSDK_Px->UB_MASKED[GPIO_Pin & 0xFF] = GPIO_Pin;
}
/**
  * @brief
  * @param
  * @param
  * 同时对一个IO端口的高8位(bit0~bit7)进行置位清除操作(1~8位),同时不受中断干扰
  * @retval None
  */
void GPIO_HigByte_ClrBit(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin)
{
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	CMSDK_Px->UB_MASKED[GPIO_Pin & 0xFF] = ~GPIO_Pin;
}
/**
  * @brief
  * @param
  * @param
  * 同时对一个IO端口的高8位(bit0~bit7)进行置位写入操作(1~8位),同时不受中断干扰
  * @retval None
  */
void GPIO_HigByte_WriteBit(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin,uint8_t bitDat)
{
	assert_param(IS_GPIO_ALL_PERIPH(CMSDK_Px));
	CMSDK_Px->UB_MASKED[GPIO_Pin & 0xFF] = bitDat;
}

/**
  * @}
  */

