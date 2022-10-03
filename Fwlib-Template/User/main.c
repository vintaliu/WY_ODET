
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_systick.h"

int main(void){
	
	LED_GPIO_Config();
	
	while(1){
	
		GPIO_SetBits(LED_Blue_GPIO_PORT,LED_Blue_GPIO_PIN);
		SysTick_Delay_ms(1000);
		GPIO_ResetBits(LED_Blue_GPIO_PORT,LED_Blue_GPIO_PIN);
		SysTick_Delay_ms(1000);
		
	}
	
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
