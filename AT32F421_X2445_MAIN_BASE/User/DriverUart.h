/*
       "LG_X2445M_V2"  Design By LG_qiuzhi
       V1.0   2021.04
       used mcu  AT32F421C8
*/
//file DirverUart.H

#ifndef __DIRVERUART_H_
#define __DIRVERUART_H_
#include <stdarg.h>
#include <stdio.h>
//#define UsedUartDmaRxData  暂时没有调试通过

#define UsedMdaSend  //使用DMA发送数据
#define RecDmaBufferSize 100

#define UsedUartID_UART1  //

#ifdef  UsedUartID_UART2
#define EVAL_COM  USART2
#define UsedUartIntDef  USART2_IRQn
#define UsedUartMdaChannle  DMA1_Channel4
#else
#define EVAL_COM  USART1
#define UsedUartIntDef  USART1_IRQn
#define UsedUartMdaChannle  DMA1_Channel2
#endif

#define DmaUsartSize 80
extern unsigned char ucaUsarDmaTxdBuffer[DmaUsartSize];

#define USART_FLAG_TC   USART_FLAG_TRAC

extern int fputc(int ch, FILE *f);
//extern void USART1_printf(USART_TypeDef *USARTx, uint8_t *Data,...);
extern void vPrintStsTemInfo(void);
extern void vPrintAdcValue(void);
extern void vSendSysPower(unsigned int uiPower);
// extern void vCalibrationRemote(void);
extern void vSendOneByteOrder(unsigned char ucOrder, unsigned char ucData);
extern void vSendMultBytesOrder(unsigned char ucOrder, unsigned char *ucSendData, unsigned char ucDataLeg);
extern void vSendSingleOrder(unsigned char ucSingleOrder);
extern void vAnalySerialData(void);
#define UsePrintSysInfo
#define UartBaudRate  	    9600

#define RecBufferSize  	    200
#define ErroSendTimes 	    5
//#define WaitFirstTimeAsMs   267
#define ReSendTimeAsMs 	    2000//500
#define ReSendTimeAsTheach  600//500
//SYSTICK 的时钟固定为HCLK时钟的1/8，在这里我们选用内部时钟源72M，所以SYSTICK的时钟为9M，即SYSTICK定时器以9M的频率递减。
//9000000 / 1000 000
#define  FAC_US  9
#define  TeacherBaudRate  2400
#define  RxBaudRateCompensates  10//
#define  TxBaudRateCompensates  20//

#define  CrcTeacherBaudRate (1.0/TeacherBaudRate)*1000000
#define  RxTeacherBaudRateDelay  (unsigned int)(CrcTeacherBaudRate+RxBaudRateCompensates)
#define  TxTeacherBaudRateDelay  (unsigned int)(CrcTeacherBaudRate+TxBaudRateCompensates)

#define TeacherBaudRatOne  400
#define TeacherPort GPIOA//单总线引脚PA12
#define TeacherPin  GPIO_Pins_12//单总线引脚PA12

#define SetTeacherPortOfOutPut  {TeacherPort->CRH &=0xFFF0FFFF;TeacherPort->CRH |=0x00010000;}
#define SetTeacherPortOfInPut   {TeacherPort->CRH &=0xFFF0FFFF;TeacherPort->CRH |=0x00080000;}

#define ReadTeacherPinH         (1 == GPIO_ReadInputDataBit(TeacherPort, TeacherPin))

#define SetTeacherPinH GPIO_SetBits(TeacherPort,TeacherPin)
#define SetTeacherPinL GPIO_ResetBits(TeacherPort, TeacherPin)

#define DisEnInterrupt              {EXTI->INTEN &= ~EXTI_INTEN_LN12;}  //屏蔽中断请求
#define EnInterrupt                 {EXTI->INTEN |= EXTI_INTEN_LN12;}  //打开中断请求
extern void vSetTeacherPortType(unsigned char ucType);
extern  unsigned char ucTeacherRxCont;
extern unsigned char ucRecFeedBackFlag;
extern unsigned char ucTeacherRecData[RecBufferSize];
extern void vTeacherPortSendByte(unsigned char ucByte);
extern unsigned char ucTeacherPortRevByte(void); //接收一个字节
// extern void vTeacherPortSendStr(unsigned char *ucBuf, unsigned char ucLen); //发送字符串
extern void vInitSysTick(void);
extern void vSetTeacherPortInt(void);//示教口的初始化
extern unsigned char ucCheckMotoConnect(void);
extern void vAnalyTeacherData(void);
extern void vSendTeacherFeedBack(void);
extern void vAnalyTeacherDataOfTime(void);
extern void vSendMultBytesTeacherOrder(unsigned char ucOrder, unsigned char *ucSendData, unsigned char ucDataLeg);
extern unsigned char ucUartRxData[RecBufferSize];
extern unsigned char ucUartRxCont;
extern unsigned char ucPressLaBa;

#define Motor1Test  0x01
#define Motor2Test  0x02
#define Motor1Forward  0x10
#define Motor2Forward  0x20

#define SendHeard1  		   0xEB
#define SendHeard2  		   0x90
#define HostDefine  		   0xA1//下控板
#define ShowDefine  		   0xA2//上板键盘板
#define TeacherAdd                    0xA3//定义示教器的地址
#define OrderPower  		   0x01//电量之时命令
#define OrderErro   		   0x02//主控向显示板报错
#define OrderInChange  	   0x03//在充电中。。。
#define OrderOutChange 	   0x04//不在充电中。。。
#define BikeInBack		     0x05//车子正在倒退中
#define BikeOutBack    	   0x06//车子不在倒退中
#define SpeedGrad      	   0x07//速度
#define OrderPowerOff  	   0x08//主控向显示板发送开关机命令
#define AskSpeedGrad   	   0x09//主控询问速度值
#define QuitErro			     0x10//解除报警状态
#define Write60Bytes   	   0x11//向显示板写入60字节的数据
#define Read60Bytes    	   0x12//读取EEPROM中数据，显示板向主控返回EEPROM中数据也用该命令
#define WriteOneByte   	   0x13//向显示板写入单字节数据
#define ReadOneByte    	   0x14//向显示板索要单字节数据
#define Disp10Led      		 0x15//点亮LED命令。
#define OutDispLed      	 0x16//退出LED命令
#define OrderSpeekP        0x17//如果有喇叭键  显示板到主控板
#define OrderWatchDogOpen  0x18//让显示板起到开启硬件看门狗作用
#define OrderWatchDogClose 0x19//关闭显示板的硬件看门狗的作用
#define OrderBreak         0x20//刹车器开关测试
#define OrderMotor         0x21//电机驱动测试
#define OrderReadPare      0x22//上传参数到示教盒
#define OrderSendPare      0x23//下发示教参数到驱动器
#define OrderRunTest       0x24//老化测试
#define FirstOrderPowerOn  0x25//第一次开机的时候 的声响信号
#define OrderBatteryType  0x31//写电池类型 标志

#define SendTail1      		0xCC
#define SendTail2      		0x33
#define SendTail3      		0xC3
#define SendTail4      		0x3C//帧尾的固定内容
#define FeedBack1    		  0xFB//回复帧标志位

#define LastErroSaveAtEepromAdd   60//上次错误存在EERPOM在的地址 只有60 01 62
//#define OrderDisLed1    0x0001
//#define OrderDisLed2    0x0002
//#define OrderDisLed3    0x0004
//#define OrderDisLed4    0x0008
//#define OrderDisLed5    0x0010//D0--D4
//#define OrderDisLed6    0x0020//
//#define OrderDisLed7    0x0040
//#define OrderDisLed8    0x0080
//#define OrderDisLed9    0x0100
//#define OrderDisLed10   0x0200


#define UartMainControlBoardAddr       0xA1
#define UartLedShowBoardAddr           0xA2
#define UartHandSetBoardAddr           0xA3

#define Uart_Frame_Header    1
#define Uart_Frame_length    2
#define SCI_Frame_Address    3
#define Uart_Frame_command   4
#define Uart_Frame_Data      5
#define Uart_Frame_Crc 	     6


extern void UART1_RecvData(unsigned char rc_data);
extern void USART1_DataProcess(void);


















#endif
