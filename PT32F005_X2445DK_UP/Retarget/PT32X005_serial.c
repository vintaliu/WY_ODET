/*********************************************************************************************************//**
 * @file    PT32X005_serial.c
 * @version V1.00
 * @date    07/10/2020
 * @brief   This file provides all the Low level serial routines for pangu01
 *************************************************************************************************************

 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "PT32X005.h"



/** @defgroup SERIAL SERIAL
  * @brief Serial related functions
  * @{
  */

/*********************************************************************************************************//**
 * @brief  Put char to UART.
 * @param  ch: The char put to UART.
 * @retval The char put to UART.
 ************************************************************************************************************/
UINT8 SERIAL_PutChar(UINT8 ch)
{
#ifdef UART_USED_UART0
	while(GetBit_BB(0x40004410, 10));
	UART0->BR = ch;
#else
	while(GetBit_BB(0x40013810, 10));
	UART1->BR = ch;
#endif
	return ch;
}

/*********************************************************************************************************//**
 * @brief  Get char from UART.
 * @retval The char got from UART.
 ************************************************************************************************************/
UINT8 SERIAL_GetChar(void)
{
#ifdef UART_USED_UART0
	while ((UART0->SR & 0x0001) == 0); // Wait if Receive Holding register is empty
	return (UART0->BR);
#else
	while ((UART1->SR & 0x0001) == 0); // Wait if Receive Holding register is empty
	return (UART1->BR);
#endif
}

/**
  * @}
  */


