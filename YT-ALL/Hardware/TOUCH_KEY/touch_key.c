

/*
//杜洋工作室出品
//洋桃系列开发板应用程序
//关注微信公众号：洋桃电子
//洋桃开发板资料下载 www.DoYoung.net/YT 
//即可免费看所有教学视频，下载技术资料，技术疑难提问
//更多内容尽在 杜洋工作室主页 www.doyoung.net
*/

/*
《修改日志》
1-201708202312 创建。


*/


#include "touch_key.h"

void TOUCH_KEY_Init(void){ //微动开关的接口初始化
	GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO的初始化枚举结构	
    GPIO_InitStructure.GPIO_Pin = TOUCH_KEY_A | TOUCH_KEY_B | TOUCH_KEY_C | TOUCH_KEY_D; //选择端口                       
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //选择IO接口工作方式 //上拉电阻       
	GPIO_Init(TOUCH_KEYPORT,&GPIO_InitStructure);			
}
 
u8 TOUCH_KEY_READ (void){  //触摸按键读取 
	u8 a=0;
	if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A) ||!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B) ||!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C) ||!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //读按键接口的电平
		delay_ms(20); //延时20ms去抖动
		if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))a|=0x01; //读按键接口的电平
		if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))a|=0x02; //读按键接口的电平
		if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C))a|=0x04; //读按键接口的电平
		if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))a|=0x08; //读按键接口的电平
			
		
	}
return a;
}


/*********************************************************************************************
 * 杜洋工作室 www.DoYoung.net
 * 洋桃电子 www.DoYoung.net/YT 
*********************************************************************************************/



/*
选择IO接口工作方式：
GPIO_Mode_AIN 模拟输入
GPIO_Mode_IN_FLOATING 浮空输入
GPIO_Mode_IPD 下拉输入
GPIO_Mode_IPU 上拉输入
GPIO_Mode_Out_PP 推挽输出
GPIO_Mode_Out_OD 开漏输出
GPIO_Mode_AF_PP 复用推挽输出
GPIO_Mode_AF_OD 复用开漏输出
*/
