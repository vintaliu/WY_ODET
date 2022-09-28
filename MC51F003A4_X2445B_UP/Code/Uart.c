#include "User_Def.h"
#include "function.h"
#include "SystemInit.h"
#include "Uart.h"

unsigned char xdata Uart1RecvBuf[BufferSize] = {0};
unsigned char xdata Uart1SendBuf[BufferSize] = {0};


unsigned char Uart1RXPhase = Uart_Frame_Header;
bit Uart1RecvFlag = 0;
unsigned char Uart1RXCnt = 0;

unsigned char ucEnableSoftDogFlg = FALSE;
unsigned char idata ucEepromBuf[60];    //保存60字节的数据
unsigned char ucRecDataCodeAndData[2] = 0;
unsigned char ucAskDKVersion;
unsigned char g_ucRecFbData = FALSE;


void UART1_SendByte(unsigned char senddata)
{ 
		SBUF_1 = senddata;
		/* Loop until the end of transmission */
		while (!(SCON_1 & 0x02));
		CLR_TI_1();
}
/*****发送字符串*****/
void UART1_SendString(unsigned char *Data, unsigned int len)
{
    unsigned int i = 0;
    for(; i < len; i++) UART1_SendByte(Data[i]);
}

/*******接收数据*****/
unsigned char  UART1_ReceiveByte()
{
    unsigned char USART1_RX_BUF;
    USART1_RX_BUF = SBUF_1;
    return  USART1_RX_BUF;
}

/****返回收到命令帧****/
void vSendFeedBack(void)
{
    unsigned char ucSendCont = 0;
    Uart1SendBuf[ucSendCont++] = SendHeard1;
    Uart1SendBuf[ucSendCont++] = SendHeard2;
    Uart1SendBuf[ucSendCont++] = FeedBack1;
    Uart1SendBuf[ucSendCont++] = SendTail1;
    Uart1SendBuf[ucSendCont++] = SendTail2;
    Uart1SendBuf[ucSendCont++] = SendTail3;
    Uart1SendBuf[ucSendCont++] = SendTail4;
    UART1_SendString(Uart1SendBuf, ucSendCont);
}

//#define WaiteFbMaxTime 9
///*********按下喇叭按键时，发送命令********/
//void vSendBuzzerOrder(unsigned char arg)
//{
//    unsigned char ucSendCont = 0;

//    Uart1SendBuf[ucSendCont++] = SendHeard1;
//    Uart1SendBuf[ucSendCont++] = SendHeard2;
//    Uart1SendBuf[ucSendCont++] = 0;
//    Uart1SendBuf[ucSendCont++] = ShowDefine;
//    Uart1SendBuf[ucSendCont++] = OrderSpeekP;
//    Uart1SendBuf[ucSendCont++] = arg;
//    Uart1SendBuf[ucSendCont++] = SendTail1;
//    Uart1SendBuf[ucSendCont++] = SendTail2;
//    Uart1SendBuf[ucSendCont++] = SendTail3;
//    Uart1SendBuf[ucSendCont++] = SendTail4;
//    Uart1SendBuf[2] = ucSendCont;

//    UART1_SendString(Uart1SendBuf, ucSendCont);
//}

void vSendUartDelya(unsigned char ucDelay)
{
	while(ucDelay)
	{
		ucDelay --;
	}
}
#ifdef QianXiUart //延时发送数据					
            #define WaiteFbMaxTime 250
#else
					  #define WaiteFbMaxTime 30
#endif

/*********按下喇叭按键时，发送命令********/
void vSendBuzzerOrder(unsigned char arg)
{
    unsigned char ucSendCont = 0;
    unsigned char ucRecSendCont = 0;
    unsigned char ucContTime = 0;
    g_ucRecFbData = FALSE;
    Uart1SendBuf[ucSendCont++] = SendHeard1;
    Uart1SendBuf[ucSendCont++] = SendHeard2;
    Uart1SendBuf[ucSendCont++] = 0;
    Uart1SendBuf[ucSendCont++] = ShowDefine;
    Uart1SendBuf[ucSendCont++] = OrderSpeekP;
    Uart1SendBuf[ucSendCont++] = arg;
    Uart1SendBuf[ucSendCont++] = SendTail1;
    Uart1SendBuf[ucSendCont++] = SendTail2;
    Uart1SendBuf[ucSendCont++] = SendTail3;
    Uart1SendBuf[ucSendCont++] = SendTail4;
    Uart1SendBuf[2] = ucSendCont;
    for(ucRecSendCont = 0; ucRecSendCont < ErroSendTimes; ucRecSendCont++)//发送五次数据
    {
        UART1_SendString(Uart1SendBuf, ucSendCont);
        for(ucContTime = 0; ucContTime < WaiteFbMaxTime; ucContTime++)     //等待发送九毫秒
        {
#ifdef QianXiUart //延时发送数据					
            vSendUartDelya(40);
#else
					  vSendUartDelya(40);
#endif					
            if(g_ucRecFbData)break;  //发送成功则跳出
        }
        if(g_ucRecFbData)break;
    }
}

void vSendDKVersion(unsigned char uiVersion)
{
    unsigned char ucSendCont = 0;

    Uart1SendBuf[ucSendCont++] = SendHeard1;
    Uart1SendBuf[ucSendCont++] = SendHeard2;
    Uart1SendBuf[ucSendCont++] = 0;
    Uart1SendBuf[ucSendCont++] = ShowDefine;
    Uart1SendBuf[ucSendCont++] = ReadDKVersion;
    Uart1SendBuf[ucSendCont++] = uiVersion;
    Uart1SendBuf[ucSendCont++] = SendTail1;
    Uart1SendBuf[ucSendCont++] = SendTail2;
    Uart1SendBuf[ucSendCont++] = SendTail3;
    Uart1SendBuf[ucSendCont++] = SendTail4;
    Uart1SendBuf[2] = ucSendCont;

    UART1_SendString(Uart1SendBuf, ucSendCont);

}

/****发送速度值****/
void vSendSpeedGrad(unsigned char ucSpeed)//发送速度
{

    unsigned char ucSendCont = 0;


    Uart1SendBuf[ucSendCont++] = SendHeard1;
    Uart1SendBuf[ucSendCont++] = SendHeard2;
    Uart1SendBuf[ucSendCont++] = 0;
    Uart1SendBuf[ucSendCont++] = ShowDefine;
    Uart1SendBuf[ucSendCont++] = SpeedGrad;
    Uart1SendBuf[ucSendCont++] = ucSpeed;
    Uart1SendBuf[ucSendCont++] = SendTail1;
    Uart1SendBuf[ucSendCont++] = SendTail2;
    Uart1SendBuf[ucSendCont++] = SendTail3;
    Uart1SendBuf[ucSendCont++] = SendTail4;
    Uart1SendBuf[2] = ucSendCont;

    UART1_SendString(Uart1SendBuf, ucSendCont);

}
/****发送按键板状态值20190807****/
//#define LedStatus         0x0A //按键板状态值下发给主板
void vSendLedStatus(unsigned char ucStatus, unsigned char ucData) //发送按键板状态值
{

    unsigned char ucSendCont = 0;


    Uart1SendBuf[ucSendCont++] = SendHeard1;
    Uart1SendBuf[ucSendCont++] = SendHeard2;
    Uart1SendBuf[ucSendCont++] = 0;
    Uart1SendBuf[ucSendCont++] = ShowDefine;
    Uart1SendBuf[ucSendCont++] = LedStatus;
    Uart1SendBuf[ucSendCont++] = ucStatus;
    Uart1SendBuf[ucSendCont++] = ucData;
    Uart1SendBuf[ucSendCont++] = SendTail1;
    Uart1SendBuf[ucSendCont++] = SendTail2;
    Uart1SendBuf[ucSendCont++] = SendTail3;
    Uart1SendBuf[ucSendCont++] = SendTail4;
    Uart1SendBuf[2] = ucSendCont;

    UART1_SendString(Uart1SendBuf, ucSendCont);
}

void vSendPowerOn(void)//发送开机命令
{
    unsigned char ucSendCont = 0;


    Uart1SendBuf[ucSendCont++] = SendHeard1;
    Uart1SendBuf[ucSendCont++] = SendHeard2;
    Uart1SendBuf[ucSendCont++] = 0;
    Uart1SendBuf[ucSendCont++] = ShowDefine;
    Uart1SendBuf[ucSendCont++] = OrderPowerOnOff;
    Uart1SendBuf[ucSendCont++] = SendTail1;
    Uart1SendBuf[ucSendCont++] = SendTail2;
    Uart1SendBuf[ucSendCont++] = SendTail3;
    Uart1SendBuf[ucSendCont++] = SendTail4;
    Uart1SendBuf[2] = ucSendCont;

    UART1_SendString(Uart1SendBuf, ucSendCont);         //发送数组中包含开机命令信息

}

void vSendOneBytes(void)//向主控发送一个Bytes
{
    unsigned char ucSendCont = 0;

    Uart1SendBuf[ucSendCont++] = SendHeard1;
    Uart1SendBuf[ucSendCont++] = SendHeard2;
    Uart1SendBuf[ucSendCont++] = 0;
    Uart1SendBuf[ucSendCont++] = ShowDefine;
    Uart1SendBuf[ucSendCont++] = ReadOneByte;
    //从EEPROM中读取速度
    clr_EA;	//关闭所有中断，否则总是进入串口中断
    Uart1SendBuf[ucSendCont++] = EEPROM_Byte_Read(EEPRO_One_Byte_ADDRESS);
    set_EA; //开中断
    Uart1SendBuf[ucSendCont++] = SendTail1;
    Uart1SendBuf[ucSendCont++] = SendTail2;
    Uart1SendBuf[ucSendCont++] = SendTail3;
    Uart1SendBuf[ucSendCont++] = SendTail4;
    Uart1SendBuf[2] = ucSendCont;

    UART1_SendString(Uart1SendBuf, ucSendCont);

}
/*****向主控发送60Bytes*****/
void vSend60Bytes(void)
{
    unsigned char ucSendCont = 0;
    unsigned char i;

    Uart1SendBuf[ucSendCont++] = SendHeard1;
    Uart1SendBuf[ucSendCont++] = SendHeard2;
    Uart1SendBuf[ucSendCont++] = 0;
    Uart1SendBuf[ucSendCont++] = ShowDefine;
    Uart1SendBuf[ucSendCont++] = Read60Bytes;
    clr_EA;
    for (i = 0; i < RW_COUNT; i++)//从EEPROM中读取速度
    {
        Uart1SendBuf[ucSendCont++] = EEPROM_Byte_Read(EEPROM_START_ADDRESS_FACT_PARE+i); //这里的数据直接对应 FALSE中的数据
    }
    set_EA;
    Uart1SendBuf[ucSendCont++] = SendTail1;
    Uart1SendBuf[ucSendCont++] = SendTail2;
    Uart1SendBuf[ucSendCont++] = SendTail3;
    Uart1SendBuf[ucSendCont++] = SendTail4;
    Uart1SendBuf[2] = ucSendCont;

    UART1_SendString(Uart1SendBuf, ucSendCont);

}

void USART1_DataProcess(void)
{
    unsigned char i;
    unsigned char ucDataBase = 0;

    if(Uart1RecvFlag)
    {
        if(strPeripheral.ucRecvData_Flag == 0)
        {
            strPeripheral.ucPowerLedNum = 0;
            strPeripheral.ucPowerLedShowPhase = 0;
            strPeripheral.ucRecvData_Flag = 0x03;
        }
        if(Uart1RecvBuf[2] != FeedBack1)
        {
            vSendFeedBack();
        }

        switch(Uart1RecvBuf[4])
        {
            case OrderPower://如果是电量帧
            if (FALSE == strPeripheral.ucRecvTestFlag)
            {
                strPeripheral.uiRecvPowerValue = ((unsigned int)Uart1RecvBuf[5] << 8) + Uart1RecvBuf[6];//发送16位数据
            }
            strPeripheral.ucRecvPowerValue_Flag = TRUE;

            break;
        case OrderPowerOnOff:  //关机命令
            ucLessThanLowPower = FALSE; //禁止电源灯闪烁
            strPeripheral.usRecvPowerOFF_Flag = TRUE;
            SPEED_LED_0;   //关闭速度灯
            POWER_SHOW_0;  //关闭电源灯
            break;
        case OrderErro:  //错误报警
            if (FALSE == strPeripheral.ucRecvTestFlag)
            {
                strPeripheral.ucRecvAlarmNum = Uart1RecvBuf[5];
            }
            break;
        case QuitErro:  //解除报警
            if (FALSE == strPeripheral.ucRecvTestFlag)
            {
                strPeripheral.ucRecvAlarmNum = 0;
                SPEEKER_CTRL_OFF;
            }

            break;
        case AskSpeedGrad:  //主控询问速度

            vSendSpeedGrad(ucSpeedGrad);
            break;
        case FirstOrderPowerOn:  //开机后，硬件给入信号
            strPeripheral.usRecvPowerOFF_Flag = FALSE;
            if (FALSE == strPeripheral.ucRecvTestFlag)
            {
                strPeripheral.ucRecvAlarmNum = 0;
                SPEEKER_CTRL_OFF;
            }
            if(FALSE == strPeripheral.usRecvPowerON_Flag)strPeripheral.usRecvPowerON_Flag = TRUE;

            break;
        case BikeInBack:  //倒车中
            strPeripheral.ucRecvBikeInBack = TRUE;
            break;
        case BikeOutBack:  //不在倒车中
            SetPWM1_Out(SpeedKeyTime, 0);//ucDebugSpeaker = 15;
            strPeripheral.ucRecvBikeInBack = FALSE;
            SPEEKER_CTRL_OFF;

            break;
        case OrderInCharge:  //充电中
            strPeripheral.ucRecvOrderInCharge = TRUE;

            break;
        case OrderOutCharge:  //不在充电中
            strPeripheral.ucRecvOrderInCharge = FALSE;
            SpeedLedOn(ucSpeedGrad);

            break;
        case Write60Bytes:  //写入60字节的数据——写入显示板
            clr_ES;
				    clr_EA;
            for (i = 0; i < RW_COUNT; i++)
            {
                ucEepromBuf[i] = Uart1RecvBuf[ 5 + i];
            }
            //vWrite60Bytes();
						strPeripheral.ucPowerLedShowPhase = 0;
            strPeripheral.ucRecvWrite60Bytes_Flag = 1;
						set_ES;//再次开启 串口中断
						set_EA;
            break;
        case Read60Bytes:  //主控读取60字节的数据即发送给主控

            vSend60Bytes();

            break;
        case WriteOneByte:  //写入1字节的数据——写入显示板
            clr_ES;
				    clr_EA;
            ucRecDataCodeAndData[0] = WriteOneByte;
            ucRecDataCodeAndData[1] = Uart1RecvBuf[6];//防止程序嵌套
            EEPROM_WriteByte(EEPRO_One_Byte_ADDRESS, ucRecDataCodeAndData[1]);
            ucRecDataCodeAndData[0] = ucRecDataCodeAndData[1] = 0;
            set_EA;
            set_ES;//再次开启 串口中断

            break;

        case OrderBatteryType:  //写入1字节的数据——写入显示板
//            clr_ES;
//            clr_EA;
//            ucDataBase = EEPROM_Byte_Read(WriteBatteryTypeFlag_Addr);
//            if(EEPROM_Byte_Read(WriteBatteryTypeFlag_Addr) !=  Uart1RecvBuf[5])
//            {
//                EEPROM_WriteByte(WriteBatteryTypeFlag_Addr, Uart1RecvBuf[5]);
//            }
//            set_EA;
//            set_ES;//再次开启 串口中断
            break;

        case ReadOneByte:  //读取1字节的数据——发送给主控
            vSendOneBytes();//发送一个字节数据
            break;
        case Disp10Led:  //点亮LED命令。测试用
            strPeripheral.ucRecvTestFlag = TRUE;
            strPeripheral.usRecvLedTmp = ((unsigned int)Uart1RecvBuf[5] << 8) + Uart1RecvBuf[6];
            //vTestShowLed(strPeripheral.usRecvLedTmp);

            break;
        case OutDispLed:  //退出LED命令。测试用
            strPeripheral.ucRecvTestFlag = FALSE;
            PowerLedOn(strPeripheral.uiRecvPowerValue);  //电源LED
            SpeedLedOn(ucSpeedGrad);   //速度LED
            SetPWM1_Out(0, 0); //蜂鸣器不响
            break;
        case OrderSpeekP:  //喇叭
            if(0x01 == Uart1RecvBuf[5])
            {
                ucBuzzerTstFlg = TRUE;
                SetPWM1_Out(SpeedKeyTime, SpeedKeyPulse);
            }
            else if(0x02 == Uart1RecvBuf[5])
            {
                ucBuzzerTstFlg = FALSE;
                SetPWM1_Out(SpeedKeyTime, 0);//ucDebugSpeaker = 16;
            }

            break;
        case OrderWatchDogOpen:  //让显示板起到开启硬件看门狗作用
            ucEnableSoftDogFlg = TRUE;

            break;
        case OrderWatchDogClose:  //关闭显示板的硬件看门狗的作用
            ucEnableSoftDogFlg = FALSE;
            break;
        case ReadDKVersion:  //如果是询问程序版本号的
            ucAskDKVersion = 0x01;
            vSendDKVersion(DKVersion); //DKVersion=0x00
            break;
        default:
            break;
        }
        Uart1RecvFlag = FALSE;
    }

}
void UART1_RecvData(unsigned char rc_data)
{
    switch(Uart1RXPhase)
    {
    case Uart_Frame_Header:
        if(rc_data == 0xEB)
        {
            Uart1RXCnt = 0;
            Uart1RecvBuf[Uart1RXCnt] = rc_data;
            Uart1RXCnt++;
        }
        else if(Uart1RXCnt == 1)
        {
            Uart1RecvBuf[Uart1RXCnt++] = rc_data;
            Uart1RXPhase = Uart_Frame_length;
        }
        else
        {
            Uart1RXCnt = 0;
        }
        break;
    case Uart_Frame_length:
        Uart1RecvBuf[Uart1RXCnt++]	= rc_data;
        if(rc_data == FeedBack1)
        {
            Uart1RXPhase = Uart_Frame_Crc;
            g_ucRecFbData = TRUE;
        }
        else
        {
            Uart1RXPhase = SCI_Frame_Address;
        }
        break;
    case SCI_Frame_Address:
        Uart1RecvBuf[Uart1RXCnt++]	= rc_data;
        Uart1RXPhase = Uart_Frame_command;
        break;
    case Uart_Frame_command:
        Uart1RecvBuf[Uart1RXCnt++]	= rc_data;
        Uart1RXPhase = Uart_Frame_Data;
        break;
    case Uart_Frame_Data:
        Uart1RecvBuf[Uart1RXCnt++]	= rc_data;
        if(Uart1RXCnt < 80)
        {
            if((Uart1RXCnt + 3) == Uart1RecvBuf[2]	)
            {
                Uart1RXPhase = Uart_Frame_Crc;
            }
        }
        else
        {
            Uart1RXCnt = 0;
            Uart1RecvBuf[Uart1RXCnt] = 0;
            Uart1RXPhase = Uart_Frame_Header;
        }
        break;
    case Uart_Frame_Crc:
        Uart1RecvBuf[Uart1RXCnt++] = rc_data;
        if (Uart1RXCnt >= 7)
        {
            if((SendTail4 == Uart1RecvBuf[Uart1RXCnt - 1]) && (SendTail3 == Uart1RecvBuf[Uart1RXCnt - 2]) && (SendTail2 == Uart1RecvBuf[Uart1RXCnt - 3]) && (SendTail1 == Uart1RecvBuf[Uart1RXCnt - 4]))
            {
                Uart1RXCnt = 0;
                Uart1RecvBuf[Uart1RXCnt] = 0;
                Uart1RXPhase = Uart_Frame_Header;
                Uart1RecvFlag = TRUE;
            }
        }
        break;
    default:
        Uart1RXPhase = Uart_Frame_Header;
        break;
    }
}



