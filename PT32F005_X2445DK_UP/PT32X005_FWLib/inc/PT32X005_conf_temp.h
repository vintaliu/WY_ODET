/******************************************************************************
  * @file   PT32X005_conf.h
  * @author  应用开发团队
  * @version V1.0.0
  * @date    2020/12/22
  * @brief    
  *          
  ******************************************************************************
  * @attention
  *
  *
  *****************************************************************************/

  

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PT32X005_CONF_H
#define PT32X005_CONF_H


/* Includes ------------------------------------------------------------------*/
/* Comment the line below to disable peripheral header file inclusion */
#include "PT32X005_gpio.h"
#include "PT32X005_tim.h"
#include "PT32X005_misc.h"
#include "PT32X005_uart.h"
#include "PT32X005_sys.h"
#include "PT32X005_pwm.h"
#include "PT32X005_cmp_opa.h"
#include "PT32X005_wdg.h"
#include "PT32X005_wdg.h"
#include "PT32X005_i2c.h"
#include "PT32X005_spi.h"
#include "PT32X005_crc.h"
//#include "PT32X005_ana.h"
#include "PT32X005_rtc.h"
#include "PT32X005_flash.h"
#include "PT32X005_adc.h"
#include "PT32X005_dac.h"
//#include "PT32X005_lcd.h"
//#include "PT32X005_beep.h"
#include "PT32X005_pwr.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/


#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports 
  *         the name of the source file and the source line number of the call 
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif 


