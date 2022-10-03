
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_systick.h"


int main(void){
	
	u8 menu;
	u16 i,t;
	u16 T = 401;
	
	LED_GPIO_Config();
	
	menu = 0;
	i = 0;
	t = 1;
	while(1){
		
		if(menu == 0){
		 
			for(i=0;i<2;i++){
			   
				GPIO_SetBits(LED_Blue_GPIO_PORT,LED_Blue_GPIO_PIN);
				SysTick_Delay_us(t);
				GPIO_ResetBits(LED_Blue_GPIO_PORT,LED_Blue_GPIO_PIN);
				SysTick_Delay_us(T - t);
			}
			
			t++;
			if( t == ( T-1 ) ){
			   menu = 1;
			}
			
		}
	  
		if(menu == 1){
		  for(i=0;i<2;i++){
			   
				GPIO_SetBits(LED_Blue_GPIO_PORT,LED_Blue_GPIO_PIN);
				SysTick_Delay_us(t);
				GPIO_ResetBits(LED_Blue_GPIO_PORT,LED_Blue_GPIO_PIN);
				SysTick_Delay_us(T - t);
			}
			
			t--;
			if( t < 100){
			  SysTick_Delay_us(170);
			}
			if( t == 1){
			   menu = 0;
				
			}
			
		}
		
		
	}
	
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
