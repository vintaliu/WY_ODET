
void UART1_SendByte(unsigned char senddata);//发送一个字节
void UART1_SendString(unsigned char *Data, unsigned int len); //发送一个字符串
unsigned char UART1_ReceiveByte(void);
void vSendSpeedGrad(unsigned char ucSpeed);
void vSendBuzzerOrder(unsigned char arg);
//void vSendRockAd(unsigned char ucSpeed);//发送速度   此函数没有实体怎么发送速度？？？？
void vSendFeedBack(void);//返回收到命令帧
//void vAnalySerialData(void);
void vSend60Bytes(void);
void vSendPowerOn(void);
void vSendOneBytes(void);//向主控发送一个Bytes


#define ErroSendTimes  4
#define	BufferSize  80     //接收缓冲区的大小
#define FeedBack1 0xFB

extern unsigned char ucEnableSoftDogFlg;
//extern unsigned char ucOrderInCharge; //是否在充电
//extern unsigned char ucBikeInBack; //是否在倒车中


extern unsigned char idata ucEepromBuf[60];
//extern unsigned char xdata ucUartRxData[RecBufferSize];

extern unsigned char ucSpeedGrad;

extern unsigned char ucRecDataCodeAndData[2];


#define SendHeard1           0xEB
#define SendHeard2           0x90
#define SendTail1             0xCC
#define SendTail2             0x33
#define SendTail3             0xC3
#define SendTail4             0x3C

#define HostDefine        0xA1 //下控板
#define ShowDefine        0xA2 //上板键盘板
#define OrderPower        0x01 //电量命令
#define OrderErro         0x02 //主控向显示板报错
#define OrderInCharge     0x03 //在充电中。。。
#define OrderOutCharge    0x04 //不在充电中。。。
#define BikeInBack        0x05 //车子正在倒退中
#define BikeOutBack       0x06 //车子不在倒退中
#define SpeedGrad         0x07 //速度
#define OrderPowerOnOff   0x08 //显示板向主控发送开关机命令
#define AskSpeedGrad      0x09 //主控询问速度值
#define LedStatus         0x0A //按键板状态值下发给主板 20190807

#define QuitErro          0x10 //解除报警状态
#define Write60Bytes      0x11 //向显示板写入60字节的数据
#define Read60Bytes       0x12 //读取EEPROM中数据，显示板向主控返回EEPROM中数据也用该命令
#define WriteOneByte      0x13 //向显示板写入单字节数据
#define ReadOneByte       0x14 //向显示板索要单字节数据
#define Disp10Led         0x15 //点亮LED命令。测试用
#define OutDispLed        0x16 //退出LED命令。测试用
#define OrderSpeekP        0x17//如果有喇叭键  显示板到主控板
#define OrderWatchDogOpen  0x18//让显示板起到开启硬件看门狗作用
#define OrderWatchDogClose 0x19//关闭显示板的硬件看门狗的作用
#define FirstOrderPowerOn  0x25//第一次开机的时候 的声响信号

#define ReadDKVersion      0x27//询问按键显示板的程序版本
#define OrderBatteryType  0x31//写电池类型 标志

#define UartMainControlBoardAddr       0xA1
#define UartLedShowBoardAddr           0xA2
#define UartHandSetBoardAddr           0xA3

#define Uart_Frame_Header    1
#define Uart_Frame_length    2
#define SCI_Frame_Address    3
#define Uart_Frame_command   4
#define Uart_Frame_Data      5
#define Uart_Frame_Crc 	     6



extern unsigned char xdata Uart1RecvBuf[BufferSize];
void UART1_RecvData(unsigned char rc_data);
void USART1_DataProcess(void);
void vSendLedStatus(unsigned char ucStatus, unsigned char ucData); //发送按键板状态值
