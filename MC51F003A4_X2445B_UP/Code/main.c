#include "User_Def.h"
#include "function.h"
#include "SystemInit.h"
#include "uart.h"

//extern  unsigned int n;

int main(void)
{
    vFirstStart();//¿ª»ú	
    while (1)
    {
        USART1_DataProcess();
        PeripheralHandle();

        //				if(Globle_Timer_1MS.Bits.AdVoltage)
        //				{
        //						Globle_Timer_1MS.Bits.AdVoltage = 0;
        //					  vDealAd();
        //				}

    }
}

/*******************         *****END OF FILE****/
