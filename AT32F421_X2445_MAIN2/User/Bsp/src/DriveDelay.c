/*
*********************************************************************************************************
*
*	模块名称 : 系统延时模块
*	文件名称 : bsp_delay.c
*	版    本 : v1.0.00
*	说    明 : 
*
*********************************************************************************************************
*/

#include "DriveBsp.h"

/*delay variable*/
static __IO float fac_us;
static __IO float fac_ms;
/*delay macros*/
#define STEP_DELAY_MS 500

/**
  * @brief  initialize Delay function   
  * @param  None
  * @retval None
  */
void bsp_InitDelay(void)
{
    /*Config Systick*/
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    fac_us = (float)SystemCoreClock / (8 * 1000000);
    fac_ms = fac_us * 1000;
}

/**
  * @brief  Inserts a delay time.
  * @param  usNus: specifies the delay time length, in microsecond.
  * @retval None
  */
void bsp_DelayUS(uint32_t usNus)
{
    uint32_t ulTemp;

    SysTick->LOAD = (uint32_t)(usNus * fac_us);
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    do
    {
        ulTemp = SysTick->CTRL;
    } while ((ulTemp & 0x01) && !(ulTemp & (1 << 16)));

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0X00;
}

/**
  * @brief  Inserts a delay time.
  * @param  usNms: specifies the delay time length, in milliseconds.
  * @retval None
  */
void bsp_DelayMS(uint16_t usNms)
{
    uint32_t ulTemp;

    while (usNms)
    {
        if (usNms > STEP_DELAY_MS)
        {
            SysTick->LOAD = (uint32_t)(STEP_DELAY_MS * fac_ms);
            usNms -= STEP_DELAY_MS;
        }
        else
        {
            SysTick->LOAD = (uint32_t)(usNms * fac_ms);
            usNms = 0;
        }
        SysTick->VAL = 0x00;
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
        do
        {
            ulTemp = SysTick->CTRL;
        } while ((ulTemp & 0x01) && !(ulTemp & (1 << 16)));

        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
        SysTick->VAL = 0X00;
    }
}

/********************************************* End of file **********************************************/
