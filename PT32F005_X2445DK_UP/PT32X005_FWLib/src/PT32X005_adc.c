/******************************************************************************
  * @file    PT32X005_adc.c
  * @author  应用开发团队
  * @version V1.0.0
  * @date    2020/10/19
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the ADC peripheral:
  *           + Initialization and Configuration
  *           + Interrupts and flags management
  *          
  ******************************************************************************
  * @attention
  *
  *
  *****************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "PT32X005_adc.h"


/** @defgroup ADC
  * @brief ADC driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define INICFR_CLEAR_MASK           ((uint32_t)0x00003B0F)
#define CALIBRATION_TIMEOUT         ((uint32_t)0x00000FFF)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Initializes the ADCx peripheral according to the specified parameters
  *         in the ADC_InitStruct.
  * @note   This function is used to configure the global features of the ADC (
  *         Resolution, Data Alignment, continuous mode activation, External
  *         trigger source and edge, Sequence Scan Direction).
  * @param  ADCx: where x can be 1 to select the ADC peripheral.
  * @param  ADC_InitStruct: pointer to an ADC_InitTypeDef structure that contains
  *         the configuration information for the specified ADC peripheral.
  * @retval None
  */
void ADC_Init(CMSDK_ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct)
{
	uint32_t tmpreg = 0;
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_CLOCKMODE(ADC_InitStruct->ADC_SynClkDiv));
	assert_param(IS_ADC_ContinuousConvMode(ADC_InitStruct->ADC_ContinuousConvMode));
	assert_param(IS_ADC_DATA_ALIGN(ADC_InitStruct->ADC_DataAlign));
	assert_param(ADC_ANA_Channel_Set(ADC_InitStruct->ADC_DataAlign));
	/* Get the ADCx CFGR value */
	tmpreg = ADCx->CR;
	/* Clear SCANDIR, RES[1:0], ALIGN, EXTSEL[2:0], EXTEN[1:0] and CONT bits */
	tmpreg &= INICFR_CLEAR_MASK;
	/*---------------------------- ADCx CFGR Configuration ---------------------*/
	/* Set MODE bit according to ADC_ContinuousConvMode value */
	/* Set EXTSEL[5:2] bits according to ADC_ExternalTrigConv value */
	/* Set DISH bit according to ADC_OffsetCalibration value */
	/* Set ALIGN bit according to ADC_DataAlign value */
	tmpreg  |= (ADC_InitStruct->ADC_ContinuousConvMode) |
	           	( ADC_InitStruct->ADC_ExternalTrigConv ) |
	           	(ADC_InitStruct->ADC_DataAlign)|
			(ADC_InitStruct->ADC_ANA_Channel)|
			(ADC_InitStruct->ADC_ExternalTrigConv)|
			(ADC_InitStruct->ADC_SynClkDiv);
	/* Write to ADCx CFGR */
	ADCx->CR = tmpreg;
}

/**
  * @brief  Fills each ADC_InitStruct member with its default value.
  * @note   This function is used to initialize the global features of the ADC (
  *         Resolution, Data Alignment, continuous mode activation, External
  *         trigger source and edge, Sequence Scan Direction).
  * @param  ADC_InitStruct: pointer to an ADC_InitTypeDef structure which will
  *         be initialized.
  * @retval None
  */
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct)
{
	/* Reset ADC init structure parameters values */
	ADC_InitStruct->ADC_SynClkDiv=ADC_ClockMode_SynClkDiv16;
	/* Initialize the ADC_ContinuousConvMode member */
	ADC_InitStruct->ADC_ContinuousConvMode = ADC_Single_Conversion;
	/* Initialize the ADC_DataAlign member */
	ADC_InitStruct->ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct->ADC_ANA_Channel=ADC_ANA_Vbg_3v;
	/* Initialize the ADC_ExternalTrigConv member */
	ADC_InitStruct->ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

}

/**
  * @brief  Set ADC sample times the specified ADC peripheral.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  sampletime: sample time value (2~255).
  * @retval None
  */
void ADC_SetSampleTime(CMSDK_ADC_TypeDef* ADCx, uint32_t sampletime)
{
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param((sampletime > 2)&&(sampletime < 256));
	ADCx->SAMPLE = sampletime;
}



/**
  * @brief  Enables or disables the specified ADC peripheral.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  NewState: new state of the ADCx peripheral.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_Cmd(CMSDK_ADC_TypeDef* ADCx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		/* Set the ADEN bit to Enable the ADC peripheral */
		ADCx->CR |= ((uint32_t)0x00000020);
	}
	else
	{
		/* Reset the ADEN to Disable the ADC peripheral */
		ADCx->CR &=((uint32_t)~0x00000020);
	}
}


/**
  * @brief  Configures for the selected ADC and its sampling time.
  * @param  ADCx: where x can be 1 to select the ADC peripheral.
  * @param  ADC_Channel: the ADC channel to configure.
  *          This parameter can be any combination of the following values:
  *            @arg ADC_Channel_0: ADC Channel0 selected
  *            @arg ADC_Channel_1: ADC Channel1 selected
  *            @arg ADC_Channel_2: ADC Channel2 selected
  *            @arg ADC_Channel_3: ADC Channel3 selected
  *            @arg ADC_Channel_4: ADC Channel4 selected
  *            @arg ADC_Channel_5: ADC Channel5 selected
  *            @arg ADC_Channel_6: ADC Channel6 selected
  *            @arg ADC_Channel_7: ADC Channel7 selected
  *            @arg ADC_Channel_8: ADC Channel8 selected
  *            @arg ADC_Channel_Vout_1V5: ADC_Channel_Vout_2V 
  *            @arg ADC_Channel_VddA: ADC_Channel_VddA 
  * @retval None
  */
void ADC_ChannelConfig(CMSDK_ADC_TypeDef* ADCx, uint32_t ADC_Channel)
{
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_CHANNEL(ADC_Channel));
	/* Clear the ADC Channel Selection bits */
	ADCx->CR &= (uint32_t)~0x0000001F;
	/* Configure the ADC Channel */
	ADCx->CR |= (uint32_t)ADC_Channel;
}

/**
  * @brief  Stop the on going conversions for the selected ADC.
  * @note   When ADSTP is set, any on going conversion is aborted, and the ADC
  *         data register is not updated with current conversion.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @retval None
  */
void ADC_StopOfConversion(CMSDK_ADC_TypeDef* ADCx)
{
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	ADCx->CR &= (uint32_t)~0x00000080;
}

/**
  * @brief  Start Conversion for the selected ADC channels.
  * @note   In continuous mode, ADSTART is not cleared by hardware with the
  *         assertion of EOSEQ because the sequence is automatic relaunched
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @retval None
  */
void ADC_StartOfConversion(CMSDK_ADC_TypeDef* ADCx)
{
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	ADCx->CR |= (uint32_t)0x00000080;
}

/**
  * @brief  Returns the last ADCx conversion result data for ADC channel.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @retval The Data conversion value.
  */
uint16_t ADC_GetConversionValue(CMSDK_ADC_TypeDef* ADCx)
{
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	/* Return the selected ADC conversion value */
	return (uint16_t) ADCx->DR;
}

/**
  * @brief  Checks whether the specified ADC flag is set or not.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  ADC_FLAG: specifies the flag to check.
  *          This parameter can be one of the following values:
  *            @arg ADC_FLAG_EOC: End of conversion flag
  *            @arg ADC_FLAG_ADRDY: ADC Ready flag
  * @retval The new state of ADC_FLAG (SET or RESET).
  */
FlagStatus ADC_GetFlagStatus(CMSDK_ADC_TypeDef* ADCx, uint32_t ADC_FLAG)
{
	FlagStatus bitstatus = RESET;
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_GET_FLAG(ADC_FLAG));
	/* Check the status of the specified ADC flag */
	if ((ADCx->SR & ADC_FLAG) != (uint32_t)RESET)
	{
		/* ADC_FLAG is set */
		bitstatus = SET;
	}
	else
	{
		/* ADC_FLAG is reset */
		bitstatus = RESET;
	}
	/* Return the ADC_FLAG status */
	return  bitstatus;
}

/**
  * @brief  Clears the ADCx's pending flags.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  ADC_FLAG: specifies the flag to clear.
  *          This parameter can be any combination of the following values:
  *            @arg ADC_FLAG_EOC: End of conversion flag
  * @retval None
  */
void ADC_ClearFlag(CMSDK_ADC_TypeDef* ADCx, uint32_t ADC_FLAG)
{
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_CLEAR_FLAG(ADC_FLAG));
	/* Clear the selected ADC flags */
	ADCx->SR = (uint32_t)ADC_FLAG;
}


/**
  * @}
  */
