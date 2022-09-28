/******************************************************************************
  * @file    PT32X005_crc.h
  * @author  应用开发团队
  * @version V1.0.0
  * @date    2020/10/19
  * @brief    This file contains all the functions prototypes for the CRC firmware library.
  *          
  ******************************************************************************
  * @attention
  *
  *
  *****************************************************************************/

  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PT32X005_CRC_H
#define PT32X005_CRC_H

#ifdef __cplusplus
extern "C" {
#endif

/*!< Includes ------------------------------------------------------------*/
#include "PT32X005.h"


/** @addtogroup CRC
  * @{
  */

/* Exported types ---------------------------------------------------------*/

/** @brief  LED Init structure definition **/
typedef struct
{
	uint32_t CRC_DIN_Width;	/*!< Defines the source of Comparator positive input. This parametercan be a value of @ref  */
	FunctionalState Bit_Input_Reverse;
	FunctionalState Byte_Input_Reverse;	/*!< Specifies whether the ADC data alignment is left or right.This parameter can be a value of @ref ADC_data_align */
	FunctionalState Bit_Output_Reverse;	/*!< Specifies whether the ADC data alignment is left or right.This parameter can be a value of @ref ADC_data_align */
	uint32_t CRC_Seed;	/*!< Defines the source of Comparator positive input. This parameter can be a value of @ref  */
	uint32_t CRC_Poly;	/*!< Defines the source of Comparator positive input. This parameter can be a value of @ref  */
} CRC_InitTypeDef;

/* Exported constants --------------------------------------------------------*/
#define IS_CRC_SEED(SEED) ((SEED) <= 0xFFFF)
#define IS_CRC_POLY(POLY) ((POLY) <= 0xFFFF)

#define CRC_DATAIN_X8                     0x00000000
#define CRC_DATAIN_X16                    0x00000004
#define IS_CRC_DINWIDTH(WIDTH) ((WIDTH == CRC_DATAIN_X8) || (WIDTH == CRC_DATAIN_X16))

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void CRC_Init(CRC_InitTypeDef* CRC_InitStruct);
void CRC_Cmd(FunctionalState NewState);
void CRC_ReloadSeed(void);
uint16_t CRC_CCITT(uint16_t seed, uint8_t *buffer, uint32_t length);
uint16_t CRC_16(uint16_t seed, uint8_t *buffer, uint32_t length);
void CRC_ReverseOutputDataBitCmd(FunctionalState NewState);
void CRC_ReverseInputDataBitCmd(FunctionalState NewState);
void CRC_ReverseInputDataCmd(FunctionalState NewState);
void CRC_Seed(uint32_t CRC_Seed);
void CRC_SetPolynomial(uint32_t CRC_Pol);
uint32_t CRC_CalcCRC16bits(uint16_t CRC_Data);
uint32_t CRC_CalcCRC8bits(uint8_t CRC_Data);
uint32_t CRC_CalcBlockCRC(uint16_t pBuffer[], uint32_t BufferLength);
uint32_t CRC_GetCRC(void);

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif


