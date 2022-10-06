/*********************************************************************************************
模板制作：  杜洋工作室/洋桃电子
程序名：	洋桃1号开发板出厂测试程序 V1
编写人：	杜洋	
编写时间：	2017年9月19日
硬件支持：	STM32F103C8   外部晶振8MHz RCC函数设置主频72MHz　  

修改日志：　　
1-	
	
							
说明：
 # 本模板加载了STM32F103内部的RCC时钟设置，并加入了利用滴答定时器的延时函数。
 # 可根据自己的需要增加或删减。

*********************************************************************************************/
#include "stm32f10x.h" //STM32头文件
#include "sys.h"
#include "rcc.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "rtc.h"
#include "buzzer.h"
#include "oled0561.h"
#include "lm75a.h"
#include "touch_key.h"
#include "adc.h"
#include "relay.h"
#include "step_motor.h"
#include "encoder.h"
#include "my1690.h"
#include "spi.h"
#include "ch376.h"
#include"filesys.h"
#include "TM1640.h"
#include "rs485.h"
#include "can.h"


extern vu16 ADC_ConvertedValue[4]; //声明外部变量
u8 buf[64];

void IO_Init (void){
	GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO的初始化枚举结构
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //选择端口号                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //选择IO接口工作方式 //上拉电阻       
	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //选择端口号（0~15或all）                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //选择IO接口工作方式       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）    
	GPIO_Init(GPIOA, &GPIO_InitStructure);
			
}

int main (void){//主程序
	u8 s,i;
	u8 buf_encoder;
	u8 c=0x01;
	u8 MP3=0;
	u8 MENU;
   	u8 TarName[64];
	u8 Temp_buffer[3]; 
	u8 buff[8];
	u8 x;
	x=0;
	RCC_Configuration(); //时钟设置
	RTC_Config();  //RTC初始化并启动
	RELAY_Init(); //继电器初始化
	STEP_MOTOR_Init(); //步进电机初始化
	TM1640_Init(); //TM640初始化
	BUZZER_Init(); //蜂鸣器初始化
	BUZZER_BEEP1(); //蜂鸣器简单响一声
	USART1_Init(115200);
	LED_Init();	//LED初始化
	TOUCH_KEY_Init(); //触摸按键初始化
	ADC_Configuration(); //ADC初始化设置
	I2C_Configuration();//I2C初始化
	OLED0561_Init(); //OLED初始化
	//显示OLED上的固定字符
	OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    "); //显示字符串
	OLED_DISPLAY_8x16_BUFFER(3,"  ALL TEST P1   "); //显示字符串
	OLED_DISPLAY_8x16_BUFFER(5,"                "); //显示字符串
//-OLED屏显示标尺--------------|0123456789012345|------
	MY1690_Init(); //MP3芯片初始化

	//继电器测试
	LED1(1);
	LED2(1);
	RELAY_Control(3);
	delay_ms(500);
	RELAY_Control(2);
	delay_ms(500);
	RELAY_Control(1);
	delay_ms(500);
	RELAY_Control(0);
	delay_ms(500);


	LED1(0);//LED灯关
	LED2(0);
	//显示OLED上的固定字符
	OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    "); //显示字符串
	OLED_DISPLAY_8x16_BUFFER(2,"A4:0000  A5:0000"); //显示字符串
	OLED_DISPLAY_8x16_BUFFER(4,"A6:0000  A7:0000"); //显示字符串
	OLED_DISPLAY_8x16_BUFFER(6,"  Temp:         "); //显示字符串
	MENU=0;
	IO_Init();
	while(1){
		if(MENU==0){//独立按键，独立LED，光敏，电位器，OLED屏，蜂鸣器，数码管
			GPIO_WriteBit(GPIOA,GPIO_Pin_15,(BitAction)(1));//收到数据LED1亮一下
			delay_ms(100); //延时
			GPIO_WriteBit(GPIOA,GPIO_Pin_15,(BitAction)(0));//


			LED1(KEY1());//用按键控制ＬＥＤ
			LED2(KEY2());
	
			//LM75A温度显示
			LM75A_GetTemp(Temp_buffer); //读取LM75A的温度数据	
			if(Temp_buffer[0])OLED_DISPLAY_8x16(6,7*8,'-'); //如果第1组为1即是负温度
			OLED_DISPLAY_8x16(6,8*8,Temp_buffer[1]/10+0x30);//显示温度值
			OLED_DISPLAY_8x16(6,9*8,Temp_buffer[1]%10+0x30);//
			OLED_DISPLAY_8x16(6,10*8,'.');//
			OLED_DISPLAY_8x16(6,11*8,Temp_buffer[2]/10+0x30);//
			OLED_DISPLAY_8x16(6,12*8,Temp_buffer[2]%10+0x30);//
			OLED_DISPLAY_8x16(6,13*8,'C');//
	//		delay_ms(500); //延时
	
			//将电位器的ADC数据显示在OLED上
			OLED_DISPLAY_8x16(2,3*8,ADC_ConvertedValue[0]/1000+0x30);//
			OLED_DISPLAY_8x16(2,4*8,ADC_ConvertedValue[0]%1000/100+0x30);//
			OLED_DISPLAY_8x16(2,5*8,ADC_ConvertedValue[0]%100/10+0x30);//
			OLED_DISPLAY_8x16(2,6*8,ADC_ConvertedValue[0]%10+0x30);//
			//将光敏电阻的ADC数据显示在OLED上
			OLED_DISPLAY_8x16(2,12*8,ADC_ConvertedValue[1]/1000+0x30);//
			OLED_DISPLAY_8x16(2,13*8,ADC_ConvertedValue[1]%1000/100+0x30);//
			OLED_DISPLAY_8x16(2,14*8,ADC_ConvertedValue[1]%100/10+0x30);//
			OLED_DISPLAY_8x16(2,15*8,ADC_ConvertedValue[1]%10+0x30);//
			//将摇杆X轴的ADC数据显示在OLED上
			OLED_DISPLAY_8x16(4,3*8,ADC_ConvertedValue[2]/1000+0x30);//
			OLED_DISPLAY_8x16(4,4*8,ADC_ConvertedValue[2]%1000/100+0x30);//
			OLED_DISPLAY_8x16(4,5*8,ADC_ConvertedValue[2]%100/10+0x30);//
			OLED_DISPLAY_8x16(4,6*8,ADC_ConvertedValue[2]%10+0x30);//
			//将摇杆Y轴的ADC数据显示在OLED上
			OLED_DISPLAY_8x16(4,12*8,ADC_ConvertedValue[3]/1000+0x30);//
			OLED_DISPLAY_8x16(4,13*8,ADC_ConvertedValue[3]%1000/100+0x30);//
			OLED_DISPLAY_8x16(4,14*8,ADC_ConvertedValue[3]%100/10+0x30);//
			OLED_DISPLAY_8x16(4,15*8,ADC_ConvertedValue[3]%10+0x30);//

			if(RTC_Get()==0){ //读出RTC时间
				TM1640_display(0,rday/10);	//天
				TM1640_display(1,rday%10+10);
				TM1640_display(2,rhour/10); //时
				TM1640_display(3,rhour%10+10);
				TM1640_display(4,rmin/10);	//分
				TM1640_display(5,rmin%10+10);
				TM1640_display(6,rsec/10); //秒
				TM1640_display(7,rsec%10);
	
				TM1640_led(c); //与TM1640连接的8个LED全亮
				c<<=1; //数据左移 流水灯
				if(c==0x00)c=0x01; //8个灯显示完后重新开始
				//delay_ms(125); //延时
			}
			if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2))OLED_DISPLAY_8x16(0,0,'Y');//

			if(TOUCH_KEY_READ()==0x01)OLED_DISPLAY_8x16(0,0,'A');//
			if(TOUCH_KEY_READ()==0x02)OLED_DISPLAY_8x16(0,0,'B');//
			if(TOUCH_KEY_READ()==0x04)OLED_DISPLAY_8x16(0,0,'C');//
			if(TOUCH_KEY_READ()==0x08)MENU=2;
	   }

	   if(MENU==2){	//
			ENCODER_Init();	//旋转编码器初始化
	   		BUZZER_BEEP1(); //蜂鸣器简单响一声
			//显示OLED上的固定字符
			OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    "); //显示字符串
			OLED_DISPLAY_8x16_BUFFER(2," TEST: encoder  "); //显示字符串
			OLED_DISPLAY_8x16_BUFFER(4,"   MP3 PLAY     "); //显示字符串
			OLED_DISPLAY_8x16_BUFFER(6,"    D:NEXT      "); //显示字符串
			MENU=3;
	   }
	   if(MENU==3){	//MP3 ，旋转编码器
			if(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)==0    //判断4个按键是否按下
				||GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)==0 
				||GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)==0 
				||GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)==0){
				delay_ms(20); //延时
				if(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)==0){	//4个按键：A上一曲，B下一曲，
					MY1690_PREV(); //上一曲
					OLED_DISPLAY_8x16_BUFFER(6,"  -- PREV --    "); //显示字符串
					delay_ms(500); //延时
					OLED_DISPLAY_8x16_BUFFER(6,"  -- PLAY --    "); //显示字符串
				}
				if(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)==0){
					MY1690_NEXT(); //下一曲
					OLED_DISPLAY_8x16_BUFFER(6,"  -- NEXT --    "); //显示字符串
					delay_ms(500); //延时
					OLED_DISPLAY_8x16_BUFFER(6,"  -- PLAY --    "); //显示字符串
				}
				while(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)==0	//等待按键放开
				||GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)==0 
				||GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)==0 
				||GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)==0);
			}
			buf_encoder=ENCODER_READ();	//读出旋转编码器按键值	
			if(buf_encoder==1){MY1690_VUP();} //右转音量加。
			if(buf_encoder==2){MY1690_VDOWN();}//左转音量减。
			if(buf_encoder==3){	//按下播放或暂停
				if(MP3==0){	//判断当前是播放还是暂停
					MP3=1;MY1690_PLAY();
					OLED_DISPLAY_8x16_BUFFER(6,"  -- PLAY --    "); //显示字符串
				}else if(MP3==1){
					MP3=0;MY1690_PAUSE();
					OLED_DISPLAY_8x16_BUFFER(6,"  -- PAUSE --   "); //显示字符串
				}
			}
			if(USART3_RX_STA==1){ //如果标志位是1 表示收到STOP
				MP3=0;
				OLED_DISPLAY_8x16_BUFFER(6,"  -- STOP --    "); //显示字符串
				USART3_RX_STA=0; //将串口数据标志位清0
			}else if(USART3_RX_STA==2){	//如果标志位是1 表示收到OK
				//加入相关的处理程序
				USART3_RX_STA=0; //将串口数据标志位清0
			}
			if(TOUCH_KEY_READ()==0x08)MENU=4;
	   }
	   if(MENU==4){	//步进电机
	   		BUZZER_BEEP1(); //蜂鸣器简单响一声
			//显示OLED上的固定字符
			OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    "); //显示字符串
			OLED_DISPLAY_8x16_BUFFER(2,"     TEST:      "); //显示字符串
			OLED_DISPLAY_8x16_BUFFER(4,"   step_motor   "); //显示字符串
			OLED_DISPLAY_8x16_BUFFER(6,"    A:NEXT      "); //显示字符串
			MENU=5;
	   }
	   if(MENU==5){	//步进电机
			MOTOLOOP_8_R(2); //驱动步进电机转动
			if(TOUCH_KEY_READ()==0x01)MENU=6;
	   }
	   if(MENU==6){	//RS485 CAN
	   		BUZZER_BEEP1(); //蜂鸣器简单响一声
			//显示OLED上的固定字符
			OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    "); //显示字符串
			OLED_DISPLAY_8x16_BUFFER(2,"A:RS232/485     "); //显示字符串
			OLED_DISPLAY_8x16_BUFFER(4,"B:CAN           "); //显示字符串
			OLED_DISPLAY_8x16_BUFFER(6,"    D:NEXT      "); //显示字符串
			USART3_RX_STA=0;
			CAN1_Configuration(); //CAN总线初始化 返回0表示成功
			USART3_Init(115200);//串口3初始化并启动
			USART3_RX_STA=0xC000; //初始值设为有回车的状态，即显示一次欢迎词
			RS485_Init();//RS485总线初始化，需要跟在USART3初始化下方
			MENU=7;
			s=1;i=1;

	   }
	   if(MENU==7){	//RS485 CAN
			if(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)==0){    //
				RS485_printf("%x",i);
				i++;
				LED1(1);//发送数据LED1亮一下
				delay_ms(100); //延时
				LED1(0);
				while(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)==0);
			}
			if(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)==0){    //
				buff[0]=s;
				CAN_Send_Msg(buff,1);//CAN数据发送参数（数组，数量），最多8个数据
				s++;
				LED1(1);//发送数据LED1亮一下
				delay_ms(100); //延时
				LED1(0);
				while(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)==0);
			}

			//CAN查寻方式的接收处理
			x = CAN_Receive_Msg(buff); //检查是否收到数据
			if(x){ //判断接收数据的数量，不为0表示有收到数据
				OLED_DISPLAY_8x16(4,9*8,buff[0]/10+0x30);//显示	
				OLED_DISPLAY_8x16(4,10*8,buff[0]%10+0x30);//显示
			}

			if(USART3_RX_STA){
				OLED_DISPLAY_8x16(2,13*8,USART3_RX_STA/10+0x30);//显示	
				OLED_DISPLAY_8x16(2,14*8,USART3_RX_STA%10+0x30);//显示
				USART3_RX_STA=0;
			}
			if(TOUCH_KEY_READ()==0x08)MENU=8;
	   }
	   if(MENU==8){	//测试CH376，U盘接口，
	   		BUZZER_BEEP1(); //蜂鸣器简单响一声
			//显示OLED上的固定字符
			OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    "); //显示字符串
			OLED_DISPLAY_8x16_BUFFER(2,"                "); //显示字符串
			OLED_DISPLAY_8x16_BUFFER(4,"  U DISK TEST   "); //显示字符串
			OLED_DISPLAY_8x16_BUFFER(6,"    PUT IN!!    "); //显示字符串
			SPI2_Init();//SPI接口初始化
			printf( "Start\r\n" );
			delay_ms(100);//延时100毫秒
			s = mInitCH376Host();//CH376初始化		
		////////////////////////////////////////////////////////////////
		 	printf("i=%02x \r\n",(unsigned short)s);
		    printf( "Wait Disk\r\n" );
		    while ( CH376DiskConnect( ) != USB_INT_SUCCESS ){/* 检查U盘是否连接,等待U盘插入,对于SD卡,可以由单片机直接查询SD卡座的插拔状态引脚 */
		      		delay_ms( 100 );
		    }
		   	delay_ms( 200 );  // 对于检测到USB设备的,最多等待100*50mS,主要针对有些MP3太慢,对于检测到USB设备
		                           //并且连接DISK_MOUNTED的,最多等待5*50mS,主要针对DiskReady不过的  
		         for ( i = 0; i < 100; i ++ ) 
		            {   
		               delay_ms( 50 );
		               printf( "Ready ?\r\n" );
		               s = CH376DiskMount( );  //初始化磁盘并测试磁盘是否就绪.   
		               if ( s == USB_INT_SUCCESS ) /* 准备好 */
		                  break;                                          
		               else if ( s == ERR_DISK_DISCON )/* 检测到断开,重新检测并计时 */
		                  break;  
		               if ( CH376GetDiskStatus( ) >= DEF_DISK_MOUNTED && i >= 5 ) /* 有的U盘总是返回未准备好,不过可以忽略,只要其建立连接MOUNTED且尝试5*50mS */
		                  break; 
		            }          
		         if ( s == ERR_DISK_DISCON ) /* 检测到断开,重新检测并计时 */
		            {  
		               printf( "Device gone\r\n" );
		               //continue;
		            }		
		         if ( CH376GetDiskStatus( ) < DEF_DISK_MOUNTED ) /* 未知USB设备,例如USB键盘、打印机等 */
		            {  
		                printf( "Unknown device\r\n" );
		            }
		/////////////////////////////////////获取出厂信息
		         i = CH376ReadBlock( buf );  /* 如果需要,可以读取数据块CH376_CMD_DATA.DiskMountInq,返回长度 */
		            if ( i == sizeof( INQUIRY_DATA ) )  /* U盘的厂商和产品信息 */
		            {  
		                  buf[ i ] = 0;
		                  printf( "UdiskInfo: %s\r\n", ((P_INQUIRY_DATA)buf) -> VendorIdStr );
		            }
				printf( "DiskQuery: " );		/* 检查U盘或者SD卡的剩余空间 */
				s = CH376DiskQuery( (PUINT32)buf );	/* 查询磁盘剩余空间信息,扇区数 */
				printf("s=%02x \r\n",(unsigned short)s );
				printf( "free cap = %ld MB\n", *(PUINT32)buf / ( 1000000 / DEF_SECTOR_SIZE ) );
		/////////////////////////////////////  创建
				printf( "Create file :" );
				strcpy( (char *)TarName, "\\YoungTalk.TXT" ); /* 目标文件名 */
		      	s = CH376FileCreatePath( TarName );  	/* 新建多级目录下的文件,支持多级目录路径,输入缓冲区必须在RAM中 */
		 		printf("s=%02x \r\n",(unsigned short)s );
		////////////////////////////////////// 写入
		      	printf( "Write ：" );
		      	strcpy((char *)buf, "  OK  " );
				s = CH376ByteWrite( buf, strlen((const char *)buf), NULL ); /* 以字节为单位向当前位置写入数据块 */
		 		printf("s=%02x \n",(unsigned short)s );
		
		 		printf("close file " ); 
		 		s = CH376FileClose( TRUE );   /* 关闭文件,对于字节读写建议自动更新文件长度 */
		 		printf("s=%02x \r\n",(unsigned short)s );
		
				printf( "Take out\n" );
				while ( CH376DiskConnect( ) == USB_INT_SUCCESS ) {  /* 检查U盘是否连接,等待U盘拔出 */
					delay_ms( 100 );
				}
			BUZZER_BEEP1(); //蜂鸣器简单响一声
			//显示OLED上的固定字符
			OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    "); //显示字符串
			OLED_DISPLAY_8x16_BUFFER(2,"                "); //显示字符串
			OLED_DISPLAY_8x16_BUFFER(4,"   U DISK OK!   "); //显示字符串
			OLED_DISPLAY_8x16_BUFFER(6,"                "); //显示字符串
			MENU=9;
	   }
	   if(MENU==9){	//
			//显示OLED上的固定字符
			OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    "); //显示字符串
			OLED_DISPLAY_8x16_BUFFER(2,"     TEST:      "); //显示字符串
			OLED_DISPLAY_8x16_BUFFER(4,"  RESET Button  "); //显示字符串
			OLED_DISPLAY_8x16_BUFFER(6,"                "); //显示字符串
	   		BUZZER_BEEP1(); //蜂鸣器简单响一声
	   		BUZZER_BEEP1(); //蜂鸣器简单响一声
	   		BUZZER_BEEP1(); //蜂鸣器简单响一声
			
	   }
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



