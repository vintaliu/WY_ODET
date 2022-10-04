/*********************************************************************************************
模板制作：  杜洋工作室/洋桃电子
程序名：	按键控制LED程序
编写人：	杜洋	
编写时间：	2017年12月29日
硬件支持：	STM32F103C8   外部晶振8MHz RCC函数设置主频72MHz　  

修改日志：　　
1-	
	
							
说明：
 # 本模板加载了STM32F103内部的RCC时钟设置，并加入了利用滴答定时器的延时函数。
 # 可根据自己的需要增加或删减。

*********************************************************************************************/
#include "stm32f10x.h" //STM32头文件
#include "sys.h"
#include "delay.h"
#include "led.h"

#include "key.h" 

int main (void){//主程序
	u8 a; //定义变量
	//初始化程序
	RCC_Configuration(); //时钟设置
	LED_Init();//LED初始化

	//KEY_Init();//按键初始化

	//主循环
	while(1){

		//示例1：无锁存
		
		//GPIO_ResetBits(LEDPORT,LED3); //LED灯都为低电平（0） 
		delay_ms(10);
		GPIO_SetBits(LEDPORT,LED3); //LED灯都为高电平（1） 
    delay_ms(10);
		//示例2：无锁存
//		GPIO_WriteBit(LEDPORT,LED1,(BitAction)(!GPIO_ReadInputDataBit(KEYPORT,KEY1))); 

		//示例3：有锁存
//		if(!GPIO_ReadInputDataBit(KEYPORT,KEY1)){ //读按键接口的电平
//			delay_ms(20); //延时去抖动
//			if(!GPIO_ReadInputDataBit(KEYPORT,KEY1)){ //读按键接口的电平
//				GPIO_WriteBit(LEDPORT,LED1,(BitAction)(1-GPIO_ReadOutputDataBit(LEDPORT,LED1))); //LED取反
//				while(!GPIO_ReadInputDataBit(KEYPORT,KEY1)); //等待按键松开 
//			}
//		}

		//示例4：有锁存
//		if(!GPIO_ReadInputDataBit(KEYPORT,KEY1)){ //读按键接口的电平
//			delay_ms(20); //延时20ms去抖动
//			if(!GPIO_ReadInputDataBit(KEYPORT,KEY1)){ //读按键接口的电平
//				//在2个LED上显示二进制加法
//				a++; //变量加1
//				if(a>3){ //当变量大于3时清0
//					a=0; 
//				}
//				GPIO_Write(LEDPORT,a); //直接数值操作将变量值写入LED（LED在GPIOB组的PB0和PB1上）
//				while(!GPIO_ReadInputDataBit(KEYPORT,KEY1)); //等待按键松开 
//			}
//		}


	}
}


/*********************************************************************************************
 * 杜洋工作室 www.DoYoung.net
 * 洋桃电子 www.DoYoung.net/YT 
*********************************************************************************************/
/*

【变量定义】
u32     a; //定义32位无符号变量a
u16     a; //定义16位无符号变量a
u8     a; //定义8位无符号变量a
vu32     a; //定义易变的32位无符号变量a
vu16     a; //定义易变的 16位无符号变量a
vu8     a; //定义易变的 8位无符号变量a
uc32     a; //定义只读的32位无符号变量a
uc16     a; //定义只读 的16位无符号变量a
uc8     a; //定义只读 的8位无符号变量a

#define ONE  1   //宏定义

delay_us(1); //延时1微秒
delay_ms(1); //延时1毫秒
delay_s(1); //延时1秒

*/



