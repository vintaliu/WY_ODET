#ifndef __BSP_LED_H

#define __BSP_LED_H

#include "stm32f10x.h"

#define LED_Blue_GPIO_PIN  GPIO_Pin_2//led ÔÚPB2
#define LED_Blue_GPIO_PORT GPIOB
#define LED_Blue_GPIO_CLK  RCC_APB2Periph_GPIOB

void LED_GPIO_Config(void);

#endif


