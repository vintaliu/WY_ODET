
/*
       "LG_X2445M_V2"  Design By LG_qiuzhi
       V1.0   2021.04
       used mcu  AT32F421C8
*/
#include "DriverConst.h"
unsigned char ucRecFeedBackFlag = FALSE;//�յ���ȷ�Ļظ�֡
unsigned char ucRecTeacherFeedBackFlag = FALSE;//�յ���ȷ�Ļظ�֡
volatile unsigned int uiLastPower = 0;
unsigned char ucLastErro = 0;
unsigned char ucRec60Bytes[60] = {0};
unsigned char ucPressLaBa = KeyPressUp;
unsigned char ucTeacherRecData[RecBufferSize] = {0};
unsigned char  ucAnlyFree = FALSE;
unsigned char ucTeacherDataTemp[10] = {0};
union uniDirverAtt uniDiverInfo = {0};//���ڱ���һЩ���ù��ĵ������
#define TeachOrderAlarmTime {ucTag100ms = FALSE;while(FALSE == ucTag100ms);}
#define OverWaiteTime 20000
unsigned char ucMingRuiErrConst = 0;

void vDelayCom(u32 uiDelayTime)//72mHz
{
    u32 uiTemp, uiTemp2;
    for(uiTemp = 0; uiTemp < uiDelayTime; uiTemp++)
    {
        //400
        for(uiTemp2 = 0; uiTemp2 < 410; uiTemp2++);//410
    }
}

void vErroTime(void)
{
    vDelayCom(ReSendTimeAsMs);
}

void vErroTimeTheach(void)
{
    vDelayCom(ReSendTimeAsTheach);
}
unsigned char ucaUsarDmaTxdBuffer[DmaUsartSize] = {0};


unsigned char ucUsartDmaSendData(unsigned char *pPoint, unsigned char ucBufLen)//ʹ��DMA�������׳�ȥ
{
    unsigned char *pData = 0;
    pData = &ucaUsarDmaTxdBuffer[0];
    if (ucBufLen > DmaUsartSize)
    {
        return 0;
    }
    DMA_ChannelEnable(UsedUartMdaChannle, DISABLE);/* Disable USART2 DMA TX Channel */
    UsedUartMdaChannle->TCNT = ucBufLen ;
    while(ucBufLen--)*pData++ = *pPoint++;/* ���ƴ��������� */
    DMA_ChannelEnable(UsedUartMdaChannle, ENABLE); /* Enable USART2 DMA TX Channel */
    return 1;
}
void vSendFeedBack(void)
{
    //�����յ�����֡
    unsigned char ucSendCont = 0;
    unsigned char ucSendBuff[20] = {0};
#ifndef UsedMdaSend
    unsigned char ucTemp;
    unsigned short usTemp;
#endif
    ucSendBuff[ucSendCont++] = SendHeard1;
    ucSendBuff[ucSendCont++] = SendHeard2;
    ucSendBuff[ucSendCont++] = FeedBack1;
    ucSendBuff[ucSendCont++] = SendTail1;
    ucSendBuff[ucSendCont++] = SendTail2;
    ucSendBuff[ucSendCont++] = SendTail3;
    ucSendBuff[ucSendCont++] = SendTail4;
#ifdef UsedMdaSend	//���ʹ��DMA���͵ķ�ʽ
    ucUsartDmaSendData(ucSendBuff, ucSendCont); //
#else
    for(ucTemp = 0; ucTemp < ucSendCont; ucTemp++)
    {
        USART_SendData(EVAL_COM, *(ucSendBuff + ucTemp));
        usTemp = 0;
        while( USART_GetFlagStatus(EVAL_COM, USART_FLAG_TC) == RESET )
        {
            usTemp++;
            if(usTemp > OverWaiteTime)return;
        }
    }
#endif
}
volatile unsigned short usLastPower = 0;
#define PowerThresholdValue 6//10
void vSendSysPower(unsigned int uiPower)//����ϵͳ����
{
    unsigned char ucSendCont = 0;
    unsigned char ucSendBuff[20] = {0};
#ifndef UsedMdaSend
    unsigned char ucTemp;
#endif
    unsigned short usTemp;
    unsigned char ucRecSendCont = 0;
    unsigned short usErroTimeDelay = 0;
    ucRecFeedBackFlag = FALSE;
    if((uiPower <= (usLastPower + PowerThresholdValue)) && (uiPower >= (usLastPower - PowerThresholdValue)))
    {
        return;
    }
    usLastPower = uiPower;
    ucSendBuff[ucSendCont++] = SendHeard1;
    ucSendBuff[ucSendCont++] = SendHeard2;
    ucSendBuff[ucSendCont++] = 0;
    ucSendBuff[ucSendCont++] = HostDefine;
    ucSendBuff[ucSendCont++] = OrderPower;
    if(uniDiverInfo.strDirverAtt.unMotorInfo.bits.bLilonOrQiansuan)
    {
        usTemp = uiPower;
        if(usTemp > 3037)usTemp = 26280;//����27.04 �����
        else if((usTemp > 2952) && (usTemp <= 3037))
        {
            usTemp = 25000; //����25.86 С��27.04  �ĸ���
            g_unioBatterLowPowerCom.ucAllData = 0;
        }
        else if((usTemp > 2824) && (usTemp <= 2952))
        {
            usTemp = 24800; //����25.20С��25.86  ������
            g_unioBatterLowPowerCom.ucAllData = 0;
        }
        else if((usTemp > 2712) && (usTemp <= 2824))
        {
            usTemp = 24200; //����24.56 С��25.20 ������
        }
        else if((usTemp > 2600) && (usTemp <= 2712))
        {
            usTemp = 23800; //����23.16С��24.56  һ����
            g_unioBatterLowPowerCom.ucAllData = 0;
            g_unioBatterLowPowerCom.bitSet.bitLowBarSetp1 = TRUE;
        }
        else if((usTemp > 2256) && (usTemp <= 2600))
        {
            usTemp = 23000; //����22.40С��23.16 �������˸
            g_unioBatterLowPowerCom.ucAllData = 0;
            g_unioBatterLowPowerCom.bitSet.bitLowBarSetp3 = TRUE;
        }
        else if(usTemp <= 2256)
        {
            usTemp = 17500; //С��22.40  ��Դָʾ���ڱ���
            g_unioBatterLowPowerCom.ucAllData = 0;
            g_unioBatterLowPowerCom.bitSet.bitLowBarSetp4 = TRUE;
        }
#ifdef UsedLinSuanTieLi
        usTemp = uiPower;
        if(usTemp > 2880)usTemp = 26280;//����27.04 �����
        else if((usTemp > 2810) && (usTemp <= 2880))
        {
            usTemp = 25000; //����25.86 С��27.04  �ĸ���
            g_unioBatterLowPowerCom.ucAllData = 0;
        }
        else if((usTemp > 2740) && (usTemp <= 2810))
        {
            usTemp = 24800; //����25.20С��25.86  ������
            g_unioBatterLowPowerCom.ucAllData = 0;
        }
        else if((usTemp > 2680) && (usTemp <= 2740))
        {
            usTemp = 24200; //����24.56 С��25.20 ������

        }
        else if((usTemp > 2620) && (usTemp <= 2680))
        {
            usTemp = 23800; //����23.16С��24.56  һ����
            g_unioBatterLowPowerCom.ucAllData = 0;
            g_unioBatterLowPowerCom.bitSet.bitLowBarSetp1 = FALSE;
        }
        else if((usTemp > 2256) && (usTemp <= 2620))
        {
            usTemp = 23000; //����22.40С��23.16 �������˸
            g_unioBatterLowPowerCom.ucAllData = 0;
            g_unioBatterLowPowerCom.bitSet.bitLowBarSetp1 = FALSE;
        }
        else if(usTemp <= 2256)
        {
            usTemp = 17500; //С��22.40  ��Դָʾ���ڱ���
            g_unioBatterLowPowerCom.ucAllData = 0;
            g_unioBatterLowPowerCom.bitSet.bitLowBarSetp3 = TRUE;
        }
        if(usTemp <= 2086)//С�� 18.5v  ��ʱ��
        {
            g_unioBatterLowPowerCom.ucAllData = 0;
            g_unioBatterLowPowerCom.bitSet.bitLowBarSetp3 = TRUE;
        }
#endif
    }
    else
    {
        if(uiPower > 2744)usTemp = 26280;//����24.25 �����
        else if((uiPower > 2648) && (uiPower <= 2744))
        {
            usTemp = 25000; //����23.41 С��24.25  �ĸ���
        }
        else if((uiPower > 2540) && (uiPower <= 2648))
        {
            usTemp = 24800; //����23.41С��27.60   ������
        }
        else if((uiPower > 2457) && (uiPower <= 2540))
        {
            usTemp = 24200; //����22.54С��22.54   ������
        }
        else if((uiPower > 2373) && (uiPower <= 2457))
        {
            usTemp = 23800; //����21.73С��21.73   һ����
            g_unioBatterLowPowerCom.ucAllData = 0;
            g_unioBatterLowPowerCom.bitSet.bitLowBarSetp1 = TRUE;
        }
        else if((uiPower > 2034) && (uiPower <= 2373))
        {
            usTemp = 23000; //����18.00С��21.00   �������˸
            g_unioBatterLowPowerCom.ucAllData = 0;
            g_unioBatterLowPowerCom.bitSet.bitLowBarSetp3 = TRUE;
        }
        else if(uiPower <= 2034)
        {
            usTemp = 17500; //С��18.00  ��Դָʾ���ڱ���
            g_unioBatterLowPowerCom.ucAllData = 0;
            g_unioBatterLowPowerCom.bitSet.bitLowBarSetp3 = TRUE;
        }
    }
    if(usTemp < LowVotageAutoPowerOff)
    {
        ucErroType = ErroLowVoltage;
    }
    ucSendBuff[ucSendCont++] = (usTemp & 0xFF00) >> 8;
    ucSendBuff[ucSendCont++] = (usTemp & 0x00FF) >> 0;
    ucSendBuff[ucSendCont++] = SendTail1;
    ucSendBuff[ucSendCont++] = SendTail2;
    ucSendBuff[ucSendCont++] = SendTail3;
    ucSendBuff[ucSendCont++] = SendTail4;
    ucSendBuff[2] = ucSendCont;
    for(ucRecSendCont = 0; ucRecSendCont < ErroSendTimes; ucRecSendCont++)
    {
#ifdef UsedMdaSend	//���ʹ��DMA���͵ķ�ʽ
        ucUsartDmaSendData(ucSendBuff, ucSendCont); //
#else
        for(ucTemp = 0; ucTemp < ucSendCont; ucTemp++)
        {
            USART_SendData(EVAL_COM, *(ucSendBuff + ucTemp));
            usTemp = 0;
            while( USART_GetFlagStatus(EVAL_COM, USART_FLAG_TC) == RESET )
            {
                usTemp++;
                if(usTemp > OverWaiteTime)return;
            }
        }
#endif
        for(usErroTimeDelay = 0; usErroTimeDelay < ReSendTimeAsMs; usErroTimeDelay++)//ѭ���ȴ�ʱ��
        {
            vDelayCom(1);
            if(ucRecFeedBackFlag)break;
        }
        if(ucRecFeedBackFlag)break;
    }
}



/*
���Ͷ��ֽں���
unsigned char ucOrder ����������
unsigned char *ucSendData ��������
unsigned char ucDataLeg ���ͳ���
*/
void vSendMultBytesOrder(unsigned char ucOrder, unsigned char *ucSendData, unsigned char ucDataLeg)//
{
    unsigned char ucSendCont = 0;
    unsigned char ucSendBuff[80] = {0};
    unsigned char ucTemp;
#ifndef UsedMdaSend
    unsigned short usTemp;
#endif
    unsigned char ucRecSendCont = 0;
    unsigned short usErroTimeDelay = 0;
    ucRecFeedBackFlag = FALSE;
    ucSendBuff[ucSendCont++] = SendHeard1;
    ucSendBuff[ucSendCont++] = SendHeard2;
    ucSendBuff[ucSendCont++] = 0;
    ucSendBuff[ucSendCont++] = HostDefine;
    ucSendBuff[ucSendCont++] = ucOrder;//����������
    for(ucTemp = 0; ucTemp < ucDataLeg; ucTemp++)
    {
        ucSendBuff[ucSendCont + ucTemp] = *(ucSendData + ucTemp); //��������
    }
    ucSendCont += ucDataLeg;
    ucSendBuff[ucSendCont++] = SendTail1;
    ucSendBuff[ucSendCont++] = SendTail2;
    ucSendBuff[ucSendCont++] = SendTail3;
    ucSendBuff[ucSendCont++] = SendTail4;
    ucSendBuff[2] = ucSendCont;
    for(ucRecSendCont = 0; ucRecSendCont < ErroSendTimes; ucRecSendCont++)
    {
#ifdef UsedMdaSend	//���ʹ��DMA���͵ķ�ʽ
        ucUsartDmaSendData(ucSendBuff, ucSendCont); //
#else
        for(ucTemp = 0; ucTemp < ucSendCont; ucTemp++)
        {
            USART_SendData(EVAL_COM, *(ucSendBuff + ucTemp));
            usTemp = 0;
            while( USART_GetFlagStatus(EVAL_COM, USART_FLAG_TC) == RESET )
            {
                usTemp++;
                if(usTemp > OverWaiteTime)return;
            }
        }
#endif
        for(usErroTimeDelay = 0; usErroTimeDelay < ReSendTimeAsMs; usErroTimeDelay++)//ѭ���ȴ�ʱ��
        {
            vDelayCom(1);
            if(ucRecFeedBackFlag)break;
        }
        if(Write60Bytes == ucOrder)//����Ƕ�ȡ60���ֽھͶ���ʱһ��
        {
            for(ucTemp = 0; ucTemp < 20; ucTemp++)
            {
                for(usErroTimeDelay = 0; usErroTimeDelay < ReSendTimeAsMs; usErroTimeDelay++)//ѭ���ȴ�ʱ��
                {
                    vDelayCom(1);
                    if(ucRecFeedBackFlag)break;
                }
            }
        }
        if(ucRecFeedBackFlag)break;
    }
}


void vSendMultBytesTeacherOrder(unsigned char ucOrder, unsigned char *ucSendData, unsigned char ucDataLeg)//
{
    unsigned char ucSendCont = 0;
    unsigned char ucSendBuff[80] = {0};
    unsigned char ucTemp;
    unsigned char ucRecSendCont = 0;
    ucRecFeedBackFlag = FALSE;
    ucSendBuff[ucSendCont++] = SendHeard1;
    ucSendBuff[ucSendCont++] = SendHeard2;
    ucSendBuff[ucSendCont++] = 0;
    ucSendBuff[ucSendCont++] = HostDefine;
    ucSendBuff[ucSendCont++] = ucOrder;//����������
    for(ucTemp = 0; ucTemp < ucDataLeg; ucTemp++)
    {
        ucSendBuff[ucSendCont + ucTemp] = *(ucSendData + ucTemp); //��������
    }
    ucSendCont += ucDataLeg;
    ucSendBuff[ucSendCont++] = SendTail1;
    ucSendBuff[ucSendCont++] = SendTail2;
    ucSendBuff[ucSendCont++] = SendTail3;
    ucSendBuff[ucSendCont++] = SendTail4;
    ucSendBuff[2] = ucSendCont;
    vSetTeacherPortType(TRUE);
		__set_FAULTMASK(1);  /* ��ֹȫ���ж�*/
    for(ucRecSendCont = 0; ucRecSendCont < ErroSendTimes; ucRecSendCont++)
    {
        for(ucTemp = 0; ucTemp < ucSendCont; ucTemp++)
        {
            vTeacherPortSendByte( *(ucSendBuff + ucTemp));
        }
        vErroTimeTheach();//�ȴ���һ�λظ�֡��ʱ��
        if(ucRecTeacherFeedBackFlag)break;
        vFeedIwdg();
        if(OrderReadPare == ucOrder)
        {
            if(1 == ucRecSendCont)break;
        }
    }
		__set_FAULTMASK(0);  /* ʹ��ȫ���ж�*/
		vSetTeacherPortType(FALSE);
}

void vSendOneByteOrder(unsigned char ucOrder, unsigned char ucData) //����
{
    unsigned char ucSendCont = 0;
    unsigned char ucSendBuff[20] = {0};
#ifndef UsedMdaSend
    unsigned char ucTemp;
    unsigned short usTemp;
#endif
    unsigned char ucRecSendCont = 0;
    unsigned short usErroTimeDelay = 0;
    ucRecFeedBackFlag = FALSE;
    ucSendBuff[ucSendCont++] = SendHeard1;
    ucSendBuff[ucSendCont++] = SendHeard2;
    ucSendBuff[ucSendCont++] = 0;
    ucSendBuff[ucSendCont++] = HostDefine;
    ucSendBuff[ucSendCont++] = ucOrder;//����������
    ucSendBuff[ucSendCont++] = ucData;//��������
    ucSendBuff[ucSendCont++] = SendTail1;
    ucSendBuff[ucSendCont++] = SendTail2;
    ucSendBuff[ucSendCont++] = SendTail3;
    ucSendBuff[ucSendCont++] = SendTail4;
    ucSendBuff[2] = ucSendCont;
    if(OrderErro == ucOrder)ucMingRuiErrConst = ucData;
    for(ucRecSendCont = 0; ucRecSendCont < ErroSendTimes; ucRecSendCont++)
    {
#ifdef UsedMdaSend	//���ʹ��DMA���͵ķ�ʽ
        ucUsartDmaSendData(ucSendBuff, ucSendCont); //
#else
        for(ucTemp = 0; ucTemp < ucSendCont; ucTemp++)
        {
            USART_SendData(EVAL_COM, *(ucSendBuff + ucTemp));
            usTemp = 0;
            while( USART_GetFlagStatus(EVAL_COM, USART_FLAG_TC) == RESET )
            {
                usTemp++;
                if(usTemp > OverWaiteTime)return;
            }
        }
#endif
        for(usErroTimeDelay = 0; usErroTimeDelay < ReSendTimeAsMs; usErroTimeDelay++)
        {
            vDelayCom(1);
            if(ucRecFeedBackFlag)break;
        }
        if(ucRecFeedBackFlag)break;
    }
}

void vSendSingleOrder(unsigned char ucSingleOrder)//����һ��������
{
    unsigned char ucSendCont = 0;
    unsigned char ucSendBuff[20] = {0};
#ifndef UsedMdaSend
    unsigned char ucTemp;
    unsigned short usTemp;
#endif
    unsigned char ucRecSendCont = 0;
    unsigned short usErroTimeDelay = 0;
    FeedDogEn;
    ucRecFeedBackFlag = FALSE;
    ucSendBuff[ucSendCont++] = SendHeard1;
    ucSendBuff[ucSendCont++] = SendHeard2;
    ucSendBuff[ucSendCont++] = 0;
    ucSendBuff[ucSendCont++] = HostDefine;
    ucSendBuff[ucSendCont++] = ucSingleOrder;//���͵���������
    ucSendBuff[ucSendCont++] = SendTail1;
    ucSendBuff[ucSendCont++] = SendTail2;
    ucSendBuff[ucSendCont++] = SendTail3;
    ucSendBuff[ucSendCont++] = SendTail4;
    ucSendBuff[2] = ucSendCont;
    if(QuitErro == ucSingleOrder)ucMingRuiErrConst = 0;
    for(ucRecSendCont = 0; ucRecSendCont < ErroSendTimes; ucRecSendCont++)
    {

#ifdef UsedMdaSend	//���ʹ��DMA���͵ķ�ʽ
        ucUsartDmaSendData(ucSendBuff, ucSendCont); //
#else
        for(ucTemp = 0; ucTemp < ucSendCont; ucTemp++)
        {
            USART_SendData(EVAL_COM, *(ucSendBuff + ucTemp));
            usTemp = 0;
            while( USART_GetFlagStatus(EVAL_COM, USART_FLAG_TC) == RESET )
            {
                usTemp++;
                if(usTemp > OverWaiteTime)return;
            }
        }
#endif
        for(usErroTimeDelay = 0; usErroTimeDelay < ReSendTimeAsMs; usErroTimeDelay++)//ѭ���ȴ�ʱ��
        {
            vDelayCom(1);
            if(ucRecFeedBackFlag)break;
        }
				if(ucRecFeedBackFlag)break;
    }
}

void vTeacherPortSendByte(unsigned char ucByte)  //����һ���ֽ�
{
    unsigned char ucCont;
    SetTeacherPinL;
    bsp_DelayUS(TeacherBaudRatOne + 10);
    for (ucCont = 0; ucCont < 8; ucCont++)//����8λ����λ
    {
        if (ucByte & 0x01) SetTeacherPinH; //�ȴ���λ
        else SetTeacherPinL;
        bsp_DelayUS(TeacherBaudRatOne + 10);
        ucByte >>= 1;
    }
    SetTeacherPinH;//����У��λ(��)//���ͽ���λ
    bsp_DelayUS(TeacherBaudRatOne + 10);
}

//�������������
void vSendTeacherFeedBack(void)
{
    //�����յ�����֡
    unsigned char ucSendCont = 0;
    unsigned char ucSendBuff[20] = {0};
    unsigned int uiTemp;
    ucSendBuff[ucSendCont++] = SendHeard1;
    ucSendBuff[ucSendCont++] = SendHeard2;
    ucSendBuff[ucSendCont++] = FeedBack1;
    ucSendBuff[ucSendCont++] = SendTail1;
    ucSendBuff[ucSendCont++] = SendTail2;
    ucSendBuff[ucSendCont++] = SendTail3;
    ucSendBuff[ucSendCont++] = SendTail4;
    vSetTeacherPortType(TRUE);
		__set_FAULTMASK(1);  /* ��ֹȫ���ж�*/
    for(uiTemp = 0; uiTemp < ucSendCont; uiTemp++)
    {
        vTeacherPortSendByte( *(ucSendBuff + uiTemp));
    }
		__set_FAULTMASK(0);  /* ʹ��ȫ���ж�*/
    vSetTeacherPortType(FALSE);
}

unsigned char ucTeacherPortRevByte(void)  //����һ���ֽ�
{
    unsigned char ucValue = 0;
    unsigned char ucCont;
    __set_FAULTMASK(1);  /* ��ֹȫ���ж�*/
    bsp_DelayUS(TeacherBaudRatOne + 45); //������ʼλ�����ʱ��Ҫ��Щ
    for (ucCont = 0; ucCont < 8; ucCont++)
    {
        ucValue >>= 1;
        if (ReadTeacherPinH) ucValue |= 0x80;
        bsp_DelayUS(TeacherBaudRatOne + 45);
    }
    __set_FAULTMASK(0);  /* ʹ��ȫ���ж�*/
    return ucValue;
}

void vInternSpeekTest(unsigned char ucTime)
{
    unsigned short usTemp;
    usTemp = 100 * ucTime;
    while (usTemp--)
    {
        ucTag10ms = FALSE;
        while(FALSE == ucTag10ms);
        if(ReadKeyOfOn_OffFlag || (ErroSysPowerOff == ucErroType))vAutoPowerOffTimeFlag();
    }
    vSendOneByteOrder(OrderSpeekP, 0x02);
}

void vExternSpeekTest(unsigned char ucTime, unsigned char ucStep)
{
    unsigned int uiTemp, uiLastExSpeedP;
    uiTemp = 100 * ucTime;
    vSendOneByteOrder(OrderSpeekP, 0x01);
    TeachOrderAlarmTime;
    vSendOneByteOrder(OrderSpeekP, 0x02);
    uiLastExSpeedP = uniDiverInfo.strDirverAtt.ucExternSpeakerVolume ;//�����ϴε�ֵ
    uniDiverInfo.strDirverAtt.ucExternSpeakerVolume  = ucStep;
    ucPressLaBa = KeyPressDown;
    DisEnInterrupt;
    while (uiTemp--)
    {
        ucTag10ms = FALSE;
        while(FALSE == ucTag10ms);
        if(ReadKeyOfOn_OffFlag || (ErroSysPowerOff == ucErroType))vAutoPowerOffTimeFlag();
    }
    ucPressLaBa = KeyPressUp;
    uniDiverInfo.strDirverAtt.ucExternSpeakerVolume  = uiLastExSpeedP;
    EnInterrupt;
    vSendOneByteOrder(OrderSpeekP, 0x01);
    TeachOrderAlarmTime;
    vSendOneByteOrder(OrderSpeekP, 0x02);
}


void vAnalySerialData(void)//���յ�������֡���з���
{
    unsigned char ucTemp;
    unsigned char ucDataBase = 0;
    unsigned char ucRecCont = 0;
    for(ucTemp = 0; ucTemp < RecBufferSize; ucTemp++)//��һ���������֡����
    {
        if((SendHeard1 == ucUartRxData[ucTemp]) && (SendHeard2 == ucUartRxData[ucTemp + 1]) && (ShowDefine == ucUartRxData[ucTemp + 3])\
                && (SendTail1 == ucUartRxData[ucTemp + ucUartRxData[ucTemp + 2] - 4]) && (SendTail2 == ucUartRxData[ucTemp + ucUartRxData[ucTemp + 2] - 3]) && (SendTail3 == ucUartRxData[ucTemp + ucUartRxData[ucTemp + 2] - 2]))
        {
            //����ҵ�֡ͷ��֡β
            ucDataBase = 0x88;
            vSendFeedBack();
            ucUartRxCont = 0;
            ucUartRxData[ucTemp] = 0;
            ucUartRxData[ucTemp + 1] = 0;//������Ҫ���֡
            ucUartRxData[ucTemp + ucUartRxData[ucTemp + 2] - 4] = 0;
            ucUartRxData[ucTemp + ucUartRxData[ucTemp + 2] - 3] = 0; //������Ҫ���֡
            ucUartRxData[ucTemp + ucUartRxData[ucTemp + 2] - 2] = 0;
            ucUartRxData[ucTemp + ucUartRxData[ucTemp + 2] - 1] = 0; //������Ҫ���֡
        }
        if((SendHeard1 == ucUartRxData[ucTemp]) && (SendHeard2 == ucUartRxData[ucTemp + 1]) && (FeedBack1 == ucUartRxData[ucTemp + 2]) && (SendTail1 == ucUartRxData[ucTemp + 3]) && (SendTail2 == ucUartRxData[ucTemp + 4])\
                && (SendTail3 == ucUartRxData[ucTemp + 5]))
        {
            ucUartRxCont = 0;
            ucUartRxData[ucTemp] = 0;//����յ�һ����ȷ��֡���ݣ�Ҫ����ǰ�յ�֡��־���
            ucUartRxData[ucTemp + 1] = 0; //������Ҫ���֡
            ucUartRxData[ucTemp + 2] = 0;
            ucUartRxData[ucTemp + 3] = 0;
            ucUartRxData[ucTemp + 4] = 0;
            ucUartRxData[ucTemp + 5] = 0;
            ucRecFeedBackFlag = TRUE;
        }
        if(ucDataBase)break;//���������֡
    }

    if(ucDataBase)
    {
        switch(ucUartRxData[ucTemp + 4])  //�鿴�ؼ���
        {
        case SpeedGrad://������ٶ�֡
        {
            ucSpeedGrad = ucUartRxData[ucTemp + 5]; ///��֡
            break;
        }
        case OrderPowerOff://�ػ�����
        {
            break;
        }
        case Read60Bytes://��60���ֽ�
        {
            for(ucRecCont = 0; ucRecCont < 60; ucRecCont++)
                ucRec60Bytes[ucRecCont] = ucUartRxData[ucTemp + 5 + ucRecCont];
            break;
        }
        case ReadOneByte://�������ֽ�
        {
            ucLastErro = ucUartRxData[ucTemp + 5]; //��֡
            break;
        }
        case OrderSpeekP://�������ֽ�
        {
            if(KeyPressDown == ucUartRxData[ucTemp + 5])
            {
                ucPressLaBa = KeyPressDown;
                CtrlExtSpeekerEn;
            }
            else
            {
                ucPressLaBa = KeyPressUp;
                CtrlExtSpeekerDis;
            }
            break;
        }
        default:
            break;
        }
    }
}

void vBreakTest(unsigned char ucTime)
{
    unsigned int uiTemp;
    uiTemp = 100 * ucTime;
    vSendOneByteOrder(OrderSpeekP, 0x01);
    TeachOrderAlarmTime;
    vSendOneByteOrder(OrderSpeekP, 0x02);
    DisEnInterrupt;
    CtlBrakeBikeEn;
    while (uiTemp--)
    {
        ucTag10ms = FALSE;
        while(FALSE == ucTag10ms);
        if(ReadKeyOfOn_OffFlag || (ErroSysPowerOff == ucErroType))vAutoPowerOffTimeFlag();
    }
    CtlBrakeBikeDisEn;
    EnInterrupt;
    vSendOneByteOrder(OrderSpeekP, 0x01);
    TeachOrderAlarmTime;
    vSendOneByteOrder(OrderSpeekP, 0x02);
}

/*
ucMotor12Ar ���һ�������� ת������
ucMoto1Pwm���1 PWM
ucMoto2Pwm���2 PWM
uiTestTimeSecond ���Ե���ʱ��
*/
void vMotorTest(unsigned char ucMotor12Ar, unsigned char ucMoto1Pwm, unsigned char ucMoto2Pwm, unsigned int uiTestTimeSecond) //û����ֲ
{
    unsigned int uiTemp = 0, uiPwmMoto1T = 0, uiPwmMoto2T = 0, uiPwmCont1 = 0, uiPwmCont2 = 0 ;
    if(0 == (ucMoto1Pwm || ucMoto2Pwm))return;//if PWM is 0   End Test
    uiTemp = 100 * uiTestTimeSecond;
    if(0 == uiTemp)return;
    uiAutoPowerOffTimeCont = 0;//���ϻ���ʱ���ùػ�
    vSendOneByteOrder(OrderSpeekP, 0x01);//Alarm Test Is Begin
    TeachOrderAlarmTime;
    vSendOneByteOrder(OrderSpeekP, 0x02);
    if(((Motor1Test | Motor2Test) == (ucMotor12Ar & 0x0F)) && (TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn))
        CtlBrakeBikeEn;//������һ��ͬʱ���Կ���ɲ����  �����ǿ���ɲ����������µ�
    if(ucMotor12Ar & Motor1Test)//������Ե��һ
    {
        uiPwmMoto1T = 36 * ucMoto1Pwm;
        if(uiPwmMoto1T > 3420)uiPwmMoto1T = 3420;
    }
    if(ucMotor12Ar & Motor2Test)//������Ե����
    {
        uiPwmMoto2T = 36 * ucMoto2Pwm;
        if(uiPwmMoto2T > 3420)uiPwmMoto2T = 3420;
    }
    DisEnInterrupt;
		
    uiPwmCont1 =  uiPwmCont2 = 0;

    ReDefTIM1CCR1 = ReDefTIM1CCR2 = ReDefTIM1CCR3 = ReDefTIM1CCR4 = 0;
    ReDefTIM4CCR1 = ReDefTIM4CCR2 = ReDefTIM4CCR3 = ReDefTIM4CCR4 = 0;
    while (uiTemp--)
    {
        uiAutoPowerOffTimeCont = 0;//���ϻ���ʱ���ùػ�
        ucTag10ms = FALSE;
        while(FALSE == ucTag10ms);
        uiPwmCont1 += 12;
        uiPwmCont2 += 12;
        if(uiPwmCont1 < uiPwmMoto1T)
        {
            if(ucMotor12Ar & Motor1Forward)//�������ת�ķ���
            {
                Moto1AStmPwmReg = uiPwmCont1;
                Moto1AStmLowPwmReg = uiPwmCont1;
            }
            else
            {
                Moto1BStmPwmReg = uiPwmCont1;
                Moto1BStmLowPwmReg = uiPwmCont1;
            }
        }
        if(uiPwmCont2 < uiPwmMoto2T)//����Ƿ�ת�ķ���
        {
            if(ucMotor12Ar & Motor2Forward)
            {
                Moto2AStmPwmReg = uiPwmCont2;//��PWM�ֱ�ֵ
                Moto2AStmLowPwmReg = uiPwmCont2;
            }
            else
            {
                Moto2BStmPwmReg = uiPwmCont2;
                Moto2BStmLowPwmReg = uiPwmCont2;
            }
        }
        if(ReadKeyOfOn_OffFlag || (ErroSysPowerOff == ucErroType))vAutoPowerOffTimeFlag();
    }
    vDownSpeedCalcPwm(TRUE);
    CtlBrakeBikeDisEn;
    EnInterrupt;
    vSendOneByteOrder(OrderSpeekP, 0x01);//Alarm Test Is Begin
    TeachOrderAlarmTime;
    vSendOneByteOrder(OrderSpeekP, 0x02);

}
/*
�ϻ�����  ucTestPwm ����PWM
uiForwardTime �������ʱ��
uiBackTime �������ʱ��
uiTestTotalTime �ܲ���ʱ��
*/
void vDriverTest(unsigned char ucTestPwm , unsigned int uiForwardTime , unsigned int uiBackTime , unsigned int uiTestTotalTime) //û����ֲ
{
    //
    unsigned int uiTotalTimeTemp;
    uiTotalTimeTemp = 0;
    vSendOneByteOrder(OrderSpeekP, 0x01);//Alarm Test Is Begin
    TeachOrderAlarmTime;
    vSendOneByteOrder(OrderSpeekP, 0x02);
    if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)CtlBrakeBikeEn;
    while(uiTotalTimeTemp <= uiTestTotalTime)
    {
        vMotorTest(Motor1Test | Motor2Test | Motor1Forward | Motor2Forward, ucTestPwm, ucTestPwm, uiForwardTime * 120);
        uiTotalTimeTemp += uiForwardTime;
        vMotorTest(Motor1Test | Motor2Test & (~Motor1Forward) & (~Motor2Forward), ucTestPwm, ucTestPwm, uiBackTime * 120);
        uiTotalTimeTemp += uiBackTime;
    }
    CtlBrakeBikeDisEn;
}
//���ñ�̿�������������ж�����ģʽ
void vSetTeacherPortType(unsigned char ucType)
{
    if(ucType)
    {
			  DisEnInterrupt;
			  GPIOA->ODRVR &= 0x01000000;
			  GPIOA->MODER |= 0x01000000;
    }
    else
    {
			  EnInterrupt;
				GPIOA->ODRVR &= 0xFCFFFFFF;
        GPIOA->MODER &= 0xFCFFFFFF;
    }
}


/*
    unsigned char ucEnIternSpeaker;//�Ƿ���������
    unsigned char ucExternSpeakerVolume;//�������ȵ�����
    union unMotorAttribute unMotorInfo;//�����һЩ���ԣ��Ƿ�Ҫ�����
    unsigned char ucFirstLineRatePare;//��һ��ɲ�����ߵ�����ֵ
    unsigned char ucFistLineTurnSecondPos;//��һ��ɲ��ת�����ڶ���ɲ���Ĺյ�ֵ
    unsigned char ucBreakAccPare;//ɲ�����ļ��ٶ�
    unsigned char ucForwardSpeedP;//ǰ���ٶȱ���ϵ��
    unsigned char ucBackSpeedP;//�����ٶȱ���ϵ��
    unsigned char ucDriverAcc;//ǰ�����˼��ٶ�
    unsigned char ucSwerveAcc;//ת����ٶ�
    unsigned char ucStarMinPwm;//������СPWM
    unsigned char ucStopMinPwm;//ֹͣ��СPWM
    unsigned char ucLetRightMotorP;//���ҵ������ϵ��
    unsigned char ucLetRightMotorTiedP;//���ҵ����ϵ��
    unsigned char ucMaxDriverCurrent;//�����������
    unsigned char ucFactryIdData1;//��������32λ�����8λ
    unsigned char ucFactryIdData2;//��������32λ����8λ
    unsigned char ucFactryIdData3;//��������32λ����8λ
    unsigned char ucFactryIdData4;//��������32λ�����8λ
    unsigned char ucForwardMinSpeedP;//ǰ����С�ٶ�
    unsigned char ucBackMinSpeedP;//������С�ٶ�
    unsigned char ucSwerveMaxSpeedP;//ת������ٶ�
    unsigned char ucSwerveMinSpeedP;//ת����С�ٶ�
    union unDelayBreakTime unDelayBreakActTime;//�ӳٹر�ɲ����ʱ��
*/
void vSaveTeacherData(unsigned char ucDataStartByte)//û����ֲ
{
    unsigned char ucTemp = 0 ;
    //if(ucTeacherRecData[ucTemp + ucDataStartByte] > 1)return; //�Ƿ���������
    //     if(ucTeacherRecData[ucTemp + ucDataStartByte + 1] > 10)return; //1//�������ȵ�����
    //     if(ucTeacherRecData[ucTemp + ucDataStartByte + 2] > 0x3F)return; // 5λ//�����һЩ���ԣ��Ƿ�Ҫ�����
    if((ucTeacherRecData[ucTemp + ucDataStartByte + 3] > 10) || (ucTeacherRecData[ucTemp + ucDataStartByte + 3] < 3))return; //6//��һ��ɲ�����ߵ�����ֵ
    if((ucTeacherRecData[ucTemp + ucDataStartByte + 4] > 10) || (ucTeacherRecData[ucTemp + ucDataStartByte + 4] < 3))return; //7//��һ��ɲ��ת�����ڶ���ɲ���Ĺյ�ֵ
    if((ucTeacherRecData[ucTemp + ucDataStartByte + 5] > 10) || (ucTeacherRecData[ucTemp + ucDataStartByte + 5] < 3))return; //8//ɲ�����ļ��ٶ�
    if(ucTeacherRecData[ucTemp + ucDataStartByte + 6] > 100)return; //9//ǰ���ٶȱ���ϵ��
    if(ucTeacherRecData[ucTemp + ucDataStartByte + 7] > 100)return; //10//�����ٶȱ���ϵ��
    if((ucTeacherRecData[ucTemp + ucDataStartByte + 8] > 10) || (0 == ucTeacherRecData[ucTemp + ucDataStartByte + 8]))return; //11//ǰ�����˼��ٶ�
    if((ucTeacherRecData[ucTemp + ucDataStartByte + 9] > 10) || (0 == ucTeacherRecData[ucTemp + ucDataStartByte + 9] ))return;	//12//ת����ٶ�
    if(ucTeacherRecData[ucTemp + ucDataStartByte + 10] > 100)return;	//13//������СPWM
    if(ucTeacherRecData[ucTemp + ucDataStartByte + 11] > 100)return;	//14//ֹͣ��СPWM
    if(ucTeacherRecData[ucTemp + ucDataStartByte + 12] > 10)return;	//15//���ҵ������ϵ��
    if(ucTeacherRecData[ucTemp + ucDataStartByte + 13] > 10)return;	//16//���ҵ����ϵ��
    if(ucTeacherRecData[ucTemp + ucDataStartByte + 14] > 50)return;	//17//�����������

    for(ucTemp = 0; ucTemp < DirverAttLength; ucTemp++)
    {
        if(uniDiverInfo.ucDirverAttDataBuff[ucTemp] != ucTeacherRecData[ucTemp + ucDataStartByte])break;
    }
    if(DirverAttLength == ucTemp)return;
    for(ucTemp = 0; ucTemp < DirverAttLength; ucTemp++)
    {
        ucRec60Bytes[ucTemp] =  uniDiverInfo.ucDirverAttDataBuff[ucTemp] = ucTeacherRecData[ucTemp + ucDataStartByte];
    }
    ucRec60Bytes[58] = WriteEepromData & 0xFF;
    ucRec60Bytes[59] = (WriteEepromData >> 8) & 0xFF; //д�뱻ռ�ñ�־
    vSendMultBytesOrder(Write60Bytes, ucRec60Bytes, 60);
    ucTag400ms = FALSE;
    while(FALSE == ucTag400ms);
    for(ucTemp = 0; ucTemp < 60; ucTemp++)TeachOrderAlarmTime;
    vSendOneByteOrder(OrderSpeekP, 0x01);//Alarm Test Is Begin
    TeachOrderAlarmTime;TeachOrderAlarmTime;
    vSendOneByteOrder(OrderSpeekP, 0x02);
    TeachOrderAlarmTime
    vReadSysTempPare(FALSE);// ���¼���һ��ϵͳ��һЩ������ֻ�ǲ���Ҫ��EEPROM
}


void vAnalyTeacherDataOfTime(void)
{
    unsigned char ucTemp = 0;
    ucAnlyFree = FALSE;//
		if(ucTeacherDataTemp[0])CtrlExtSpeekerDis;//20200327 ������ ǿ�Ʋ����
    switch(ucTeacherDataTemp[0])//�鿴�ؼ���
    {
    case OrderSpeekP://���Ȳ���
    {
        if(0x01 == ucTeacherDataTemp[1])
            vInternSpeekTest( ucTeacherDataTemp[2]);
        if(0x02 == ucTeacherDataTemp[1])
            vExternSpeekTest( ucTeacherDataTemp[2], ucTeacherDataTemp[3]);
        for(ucTemp = 0; ucTemp < 5; ucTemp++)ucTeacherDataTemp[ucTemp] = 0x00;
        break;
    }
    case OrderBreak://ɲ��������
    {
        if(0x01 == ucTeacherDataTemp[1])
            vBreakTest( ucTeacherDataTemp[2]);
        if(0x02 == ucTeacherDataTemp[1])CtlBrakeBikeDisEn;
        for(ucTemp = 0; ucTemp < 5; ucTemp++)ucTeacherDataTemp[ucTemp] = 0x00;
        break;
    }
    case OrderMotor://�������
    {
        vMotorTest(ucTeacherDataTemp[1], ucTeacherDataTemp[2], ucTeacherDataTemp[3], ucTeacherDataTemp[4]);
        for(ucTemp = 0; ucTemp < 5; ucTemp++)ucTeacherDataTemp[ucTemp] = 0x00;
        break;
    }
    case OrderRunTest://�ϻ�����
    {
        vDriverTest(ucTeacherDataTemp[1], (ucTeacherDataTemp[2] << 8) + ucTeacherDataTemp[3], (ucTeacherDataTemp[4] << 8) + ucTeacherDataTemp[5],
                    (ucTeacherDataTemp[6] << 8) + ucTeacherDataTemp[7]);
        for(ucTemp = 0; ucTemp < 5; ucTemp++)ucTeacherDataTemp[ucTemp] = 0x00;
        break;
    }
    case OrderSendPare://
    {
        vSaveTeacherData(ucTeacherDataTemp[1]);//û����ֲ
        for(ucTemp = 0; ucTemp < 5; ucTemp++)ucTeacherDataTemp[ucTemp] = 0x00;
        //ucCheckMotoConnect();
        if(ucErroType)vShowErroToDis(0);
        break;
    }
    case OrderReadPare:// ��������
    {
        ucTemp = uniDiverInfo.strDirverAtt.ucEnIternSpeaker;//
        if(ucTemp)uniDiverInfo.strDirverAtt.ucEnIternSpeaker = TRUE;
        else uniDiverInfo.strDirverAtt.ucEnIternSpeaker = FALSE;
        //��������ʶ��Ӳ���汾��HK32 ��STM32   202002212011
#ifdef HK32
        uniDiverInfo.strDirverAtt.unMotorInfo.bits.bEnAutoPowerOn = TRUE;
        uniDiverInfo.strDirverAtt.ucStopMinPwm = 11;
        uniDiverInfo.strDirverAtt.ucExternSpeakerVolume = 4;
#endif
#ifdef STM32
        uniDiverInfo.strDirverAtt.unMotorInfo.bits.bEnAutoPowerOn = FALSE;
        uniDiverInfo.strDirverAtt.ucStopMinPwm = 12;
        uniDiverInfo.strDirverAtt.ucExternSpeakerVolume = 5;
#endif

        vSendMultBytesTeacherOrder(OrderReadPare, &uniDiverInfo.ucDirverAttDataBuff[0], DirverAttLength);
        uniDiverInfo.strDirverAtt.ucEnIternSpeaker = ucTemp;//
        for(ucTemp = 0; ucTemp < 5; ucTemp++)ucTeacherDataTemp[ucTemp] = 0x00;
        break;
    }
    //    {
    //        vSendMultBytesTeacherOrder(OrderReadPare, &uniDiverInfo.ucDirverAttDataBuff[0], DirverAttLength);
    //        for(ucTemp = 0; ucTemp < 5; ucTemp++)ucTeacherDataTemp[ucTemp] = 0x00;
    //        break;
    //    }
    default:
        break;
    }
}
unsigned char ucDisEnWatchDogFlagA = FALSE;
void vAnalyTeacherData(void)
{
    unsigned char ucTemp;
    unsigned char ucDataBase = 0;
    unsigned char ucTempArry[10] = {0};
    for(ucTemp = 0; ucTemp < RecBufferSize; ucTemp++)//��һ���������֡����
    {
        if((SendHeard1 == ucTeacherRecData[ucTemp]) && (SendHeard2 == ucTeacherRecData[ucTemp + 1]) && (TeacherAdd == ucTeacherRecData[ucTemp + 3])\
                && (SendTail1 == ucTeacherRecData[ucTemp + ucTeacherRecData[ucTemp + 2] - 4]) && (SendTail2 == ucTeacherRecData[ucTemp + ucTeacherRecData[ucTemp + 2] - 3]) \
                && (SendTail3 == ucTeacherRecData[ucTemp + ucTeacherRecData[ucTemp + 2] - 2]))
        {
            //����ҵ�֡ͷ��֡β
            ucDataBase = 0x88;
            vDelay30us(1000);//1700
            FeedDogEn;
            if(FALSE == ucDisEnWatchDogFlagA)
            {
                vSendSingleOrder(OrderWatchDogClose);
                ucDisEnWatchDogFlagA = TRUE;
            }
            vSendTeacherFeedBack();
            ucTeacherRxCont = 0;
            ucTeacherRecData[ucTemp] = 0;
            ucTeacherRecData[ucTemp + 1] = 0; //������Ҫ���֡
            ucTeacherRecData[ucTemp + ucTeacherRecData[ucTemp + 2] - 4] = 0;
            ucTeacherRecData[ucTemp + ucTeacherRecData[ucTemp + 2] - 3] = 0; //������Ҫ���֡
            ucTeacherRecData[ucTemp + ucTeacherRecData[ucTemp + 2] - 2] = 0;
            ucTeacherRecData[ucTemp + ucTeacherRecData[ucTemp + 2] - 1] = 0; //������Ҫ���֡
        }
        if((SendHeard1 == ucTeacherRecData[ucTemp]) && (SendHeard2 == ucTeacherRecData[ucTemp + 1]) && (FeedBack1 == ucTeacherRecData[ucTemp + 2]) \
                && (SendTail1 == ucTeacherRecData[ucTemp + 3]) && (SendTail2 == ucTeacherRecData[ucTemp + 4])\
                && (SendTail3 == ucTeacherRecData[ucTemp + 5]) && (SendTail4 == ucTeacherRecData[ucTemp + 6]) )
        {
            ucTeacherRxCont = 0;
            ucTeacherRecData[ucTemp] = 0;//����յ�һ����ȷ��֡���ݣ�Ҫ����ǰ�յ�֡��־���
            ucTeacherRecData[ucTemp + 1] = 0; //������Ҫ���֡
            ucTeacherRecData[ucTemp + 2] = 0;
            ucTeacherRecData[ucTemp + 3] = 0;
            ucTeacherRecData[ucTemp + 4] = 0;
            ucTeacherRecData[ucTemp + 5] = 0;
            ucRecTeacherFeedBackFlag = TRUE;
        }
        if(ucDataBase)break;///���������֡
    }

    if(ucDataBase)
    {
        switch(ucTeacherRecData[ucTemp + 4]) //�鿴�ؼ���
        {
        case Disp10Led://�������������
        {
            ucTempArry[0] = ucTeacherRecData[5];
            ucTempArry[1] =  ucTeacherRecData[6];
            vSendMultBytesOrder(Disp10Led, ucTempArry, 2);
            break;
        }
        case OutDispLed://�˳�Led��ʾ����
        {
            vSendSingleOrder(OutDispLed);
            break;
        }
        case OrderMotor://�������
        {
            ucTeacherDataTemp[0] = ucTeacherRecData[ucTemp + 4];
            ucTeacherDataTemp[1] = ucTeacherRecData[ucTemp + 5];
            ucTeacherDataTemp[2] = ucTeacherRecData[ucTemp + 6];
            ucTeacherDataTemp[3] = ucTeacherRecData[ucTemp + 7];
            ucTeacherDataTemp[4] = ucTeacherRecData[ucTemp + 8];
            ucAnlyFree = TRUE;
            break;
        }
        case OrderRunTest://�������ϻ�����
        {
            ucTeacherDataTemp[0] = ucTeacherRecData[ucTemp + 4];
            ucTeacherDataTemp[1] = ucTeacherRecData[ucTemp + 5];
            ucTeacherDataTemp[2] = ucTeacherRecData[ucTemp + 6];
            ucTeacherDataTemp[3] = ucTeacherRecData[ucTemp + 7];
            ucTeacherDataTemp[4] = ucTeacherRecData[ucTemp + 8];
            ucTeacherDataTemp[5] = ucTeacherRecData[ucTemp + 9];
            ucTeacherDataTemp[6] = ucTeacherRecData[ucTemp + 10];
            ucTeacherDataTemp[7] = ucTeacherRecData[ucTemp + 11];
            ucAnlyFree = TRUE;
            break;
        }
        case OrderBreak://ɲ��������
        {
            ucTeacherDataTemp[0] = ucTeacherRecData[ucTemp + 4];
            ucTeacherDataTemp[1] = ucTeacherRecData[ucTemp + 5];
            ucTeacherDataTemp[2] = ucTeacherRecData[ucTemp + 6];
            ucTeacherDataTemp[3] = ucTeacherRecData[ucTemp + 7];
            ucAnlyFree = TRUE;
            break;
        }
        case OrderSpeekP://������������
        {
            ucTeacherDataTemp[0] = ucTeacherRecData[ucTemp + 4];
            ucTeacherDataTemp[1] = ucTeacherRecData[ucTemp + 5];
            ucTeacherDataTemp[2] = ucTeacherRecData[ucTemp + 6];
            ucTeacherDataTemp[3] = ucTeacherRecData[ucTemp + 7];
            ucAnlyFree = TRUE;
            break;
        }
        case OrderSendPare://�����û����õ���Ϣ����
        {
            ucTeacherDataTemp[0] = ucTeacherRecData[ucTemp + 4];
            ucTeacherDataTemp[1] = ucTemp + 5;
            ucAnlyFree = TRUE;
            break;
        }
        case OrderReadPare:// ��������
        {
            ucTeacherDataTemp[0] = ucTeacherRecData[ucTemp + 4];
            ucAnlyFree = TRUE;
            break;
        }
        default:
            break;
        }
    }
}


