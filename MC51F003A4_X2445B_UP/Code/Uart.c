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
unsigned char idata ucEepromBuf[60];    //����60�ֽڵ�����
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
/*****�����ַ���*****/
void UART1_SendString(unsigned char *Data, unsigned int len)
{
    unsigned int i = 0;
    for(; i < len; i++) UART1_SendByte(Data[i]);
}

/*******��������*****/
unsigned char  UART1_ReceiveByte()
{
    unsigned char USART1_RX_BUF;
    USART1_RX_BUF = SBUF_1;
    return  USART1_RX_BUF;
}

/****�����յ�����֡****/
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
///*********�������Ȱ���ʱ����������********/
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
#ifdef QianXiUart //��ʱ��������					
            #define WaiteFbMaxTime 250
#else
					  #define WaiteFbMaxTime 30
#endif

/*********�������Ȱ���ʱ����������********/
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
    for(ucRecSendCont = 0; ucRecSendCont < ErroSendTimes; ucRecSendCont++)//�����������
    {
        UART1_SendString(Uart1SendBuf, ucSendCont);
        for(ucContTime = 0; ucContTime < WaiteFbMaxTime; ucContTime++)     //�ȴ����;ź���
        {
#ifdef QianXiUart //��ʱ��������					
            vSendUartDelya(40);
#else
					  vSendUartDelya(40);
#endif					
            if(g_ucRecFbData)break;  //���ͳɹ�������
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

/****�����ٶ�ֵ****/
void vSendSpeedGrad(unsigned char ucSpeed)//�����ٶ�
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
/****���Ͱ�����״ֵ̬20190807****/
//#define LedStatus         0x0A //������״ֵ̬�·�������
void vSendLedStatus(unsigned char ucStatus, unsigned char ucData) //���Ͱ�����״ֵ̬
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

void vSendPowerOn(void)//���Ϳ�������
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

    UART1_SendString(Uart1SendBuf, ucSendCont);         //���������а�������������Ϣ

}

void vSendOneBytes(void)//�����ط���һ��Bytes
{
    unsigned char ucSendCont = 0;

    Uart1SendBuf[ucSendCont++] = SendHeard1;
    Uart1SendBuf[ucSendCont++] = SendHeard2;
    Uart1SendBuf[ucSendCont++] = 0;
    Uart1SendBuf[ucSendCont++] = ShowDefine;
    Uart1SendBuf[ucSendCont++] = ReadOneByte;
    //��EEPROM�ж�ȡ�ٶ�
    clr_EA;	//�ر������жϣ��������ǽ��봮���ж�
    Uart1SendBuf[ucSendCont++] = EEPROM_Byte_Read(EEPRO_One_Byte_ADDRESS);
    set_EA; //���ж�
    Uart1SendBuf[ucSendCont++] = SendTail1;
    Uart1SendBuf[ucSendCont++] = SendTail2;
    Uart1SendBuf[ucSendCont++] = SendTail3;
    Uart1SendBuf[ucSendCont++] = SendTail4;
    Uart1SendBuf[2] = ucSendCont;

    UART1_SendString(Uart1SendBuf, ucSendCont);

}
/*****�����ط���60Bytes*****/
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
    for (i = 0; i < RW_COUNT; i++)//��EEPROM�ж�ȡ�ٶ�
    {
        Uart1SendBuf[ucSendCont++] = EEPROM_Byte_Read(EEPROM_START_ADDRESS_FACT_PARE+i); //���������ֱ�Ӷ�Ӧ FALSE�е�����
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
            case OrderPower://����ǵ���֡
            if (FALSE == strPeripheral.ucRecvTestFlag)
            {
                strPeripheral.uiRecvPowerValue = ((unsigned int)Uart1RecvBuf[5] << 8) + Uart1RecvBuf[6];//����16λ����
            }
            strPeripheral.ucRecvPowerValue_Flag = TRUE;

            break;
        case OrderPowerOnOff:  //�ػ�����
            ucLessThanLowPower = FALSE; //��ֹ��Դ����˸
            strPeripheral.usRecvPowerOFF_Flag = TRUE;
            SPEED_LED_0;   //�ر��ٶȵ�
            POWER_SHOW_0;  //�رյ�Դ��
            break;
        case OrderErro:  //���󱨾�
            if (FALSE == strPeripheral.ucRecvTestFlag)
            {
                strPeripheral.ucRecvAlarmNum = Uart1RecvBuf[5];
            }
            break;
        case QuitErro:  //�������
            if (FALSE == strPeripheral.ucRecvTestFlag)
            {
                strPeripheral.ucRecvAlarmNum = 0;
                SPEEKER_CTRL_OFF;
            }

            break;
        case AskSpeedGrad:  //����ѯ���ٶ�

            vSendSpeedGrad(ucSpeedGrad);
            break;
        case FirstOrderPowerOn:  //������Ӳ�������ź�
            strPeripheral.usRecvPowerOFF_Flag = FALSE;
            if (FALSE == strPeripheral.ucRecvTestFlag)
            {
                strPeripheral.ucRecvAlarmNum = 0;
                SPEEKER_CTRL_OFF;
            }
            if(FALSE == strPeripheral.usRecvPowerON_Flag)strPeripheral.usRecvPowerON_Flag = TRUE;

            break;
        case BikeInBack:  //������
            strPeripheral.ucRecvBikeInBack = TRUE;
            break;
        case BikeOutBack:  //���ڵ�����
            SetPWM1_Out(SpeedKeyTime, 0);//ucDebugSpeaker = 15;
            strPeripheral.ucRecvBikeInBack = FALSE;
            SPEEKER_CTRL_OFF;

            break;
        case OrderInCharge:  //�����
            strPeripheral.ucRecvOrderInCharge = TRUE;

            break;
        case OrderOutCharge:  //���ڳ����
            strPeripheral.ucRecvOrderInCharge = FALSE;
            SpeedLedOn(ucSpeedGrad);

            break;
        case Write60Bytes:  //д��60�ֽڵ����ݡ���д����ʾ��
            clr_ES;
				    clr_EA;
            for (i = 0; i < RW_COUNT; i++)
            {
                ucEepromBuf[i] = Uart1RecvBuf[ 5 + i];
            }
            //vWrite60Bytes();
						strPeripheral.ucPowerLedShowPhase = 0;
            strPeripheral.ucRecvWrite60Bytes_Flag = 1;
						set_ES;//�ٴο��� �����ж�
						set_EA;
            break;
        case Read60Bytes:  //���ض�ȡ60�ֽڵ����ݼ����͸�����

            vSend60Bytes();

            break;
        case WriteOneByte:  //д��1�ֽڵ����ݡ���д����ʾ��
            clr_ES;
				    clr_EA;
            ucRecDataCodeAndData[0] = WriteOneByte;
            ucRecDataCodeAndData[1] = Uart1RecvBuf[6];//��ֹ����Ƕ��
            EEPROM_WriteByte(EEPRO_One_Byte_ADDRESS, ucRecDataCodeAndData[1]);
            ucRecDataCodeAndData[0] = ucRecDataCodeAndData[1] = 0;
            set_EA;
            set_ES;//�ٴο��� �����ж�

            break;

        case OrderBatteryType:  //д��1�ֽڵ����ݡ���д����ʾ��
//            clr_ES;
//            clr_EA;
//            ucDataBase = EEPROM_Byte_Read(WriteBatteryTypeFlag_Addr);
//            if(EEPROM_Byte_Read(WriteBatteryTypeFlag_Addr) !=  Uart1RecvBuf[5])
//            {
//                EEPROM_WriteByte(WriteBatteryTypeFlag_Addr, Uart1RecvBuf[5]);
//            }
//            set_EA;
//            set_ES;//�ٴο��� �����ж�
            break;

        case ReadOneByte:  //��ȡ1�ֽڵ����ݡ������͸�����
            vSendOneBytes();//����һ���ֽ�����
            break;
        case Disp10Led:  //����LED���������
            strPeripheral.ucRecvTestFlag = TRUE;
            strPeripheral.usRecvLedTmp = ((unsigned int)Uart1RecvBuf[5] << 8) + Uart1RecvBuf[6];
            //vTestShowLed(strPeripheral.usRecvLedTmp);

            break;
        case OutDispLed:  //�˳�LED���������
            strPeripheral.ucRecvTestFlag = FALSE;
            PowerLedOn(strPeripheral.uiRecvPowerValue);  //��ԴLED
            SpeedLedOn(ucSpeedGrad);   //�ٶ�LED
            SetPWM1_Out(0, 0); //����������
            break;
        case OrderSpeekP:  //����
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
        case OrderWatchDogOpen:  //����ʾ���𵽿���Ӳ�����Ź�����
            ucEnableSoftDogFlg = TRUE;

            break;
        case OrderWatchDogClose:  //�ر���ʾ���Ӳ�����Ź�������
            ucEnableSoftDogFlg = FALSE;
            break;
        case ReadDKVersion:  //�����ѯ�ʳ���汾�ŵ�
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



