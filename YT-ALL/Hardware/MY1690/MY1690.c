

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
1-20170919 创建。


*/




#include "MY1690.h"

void MY1690_Init(void){ //初始化
	USART3_Init(9600);//串口3初始化并启动
	USART3_RX_STA=0xC000; //初始值设为有回车的状态，即显示一次欢迎词
	MY1690_STOP(); //上电初始化后发送一次指令激活MP3芯片
}
void MY1690_PLAY(void){ //播放
	USART3_printf("\x7e\x03\x11\x12\xef"); //其中 \x 后接十六进制数据
}
void MY1690_PAUSE(void){ //暂停
	USART3_printf("\x7e\x03\x12\x11\xef");
}
void MY1690_PREV(void){ //上一曲
	USART3_printf("\x7e\x03\x14\x17\xef");
}
void MY1690_NEXT(void){ //下一曲
	USART3_printf("\x7e\x03\x13\x10\xef");
}
void MY1690_VUP(void){ //音量加1
	USART3_printf("\x7e\x03\x15\x16\xef");
}
void MY1690_VDOWN(void){ //音量减1
	USART3_printf("\x7e\x03\x16\x15\xef");
}
void MY1690_STOP(void){ //停止
	USART3_printf("\x7e\x03\x1E\x1D\xef");
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
