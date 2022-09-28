#include <stdio.h>
#include "User_Def.h"
#include "function.h"
#include "SystemInit.h"
#include "uart.h"

//extern  unsigned int n;
void myFuncInit();
void MyFuncSpeaker();
void myFuncLed();
unsigned int i = 0;
int main(void)
{

	  
//    vFirstStart();//¿ª»ú	
//		i = 0;
//    while (1)
//    {
//			  i++;
//        USART1_DataProcess();
//        PeripheralHandle();

//        //				if(Globle_Timer_1MS.Bits.AdVoltage)
//        //				{
//        //						Globle_Timer_1MS.Bits.AdVoltage = 0;
//        //					  vDealAd();
//        //				}
//			
//			
//    }
	
	  myFuncInit();
	  i = 0;
	  while(1){
		  i++;
			myFuncLed();
			MyFuncSpeaker();
		}
}

/*******************         *****END OF FILE****/
