
/*
       "LG_X2445M_V2"  Design By LG_qiuzhi
       V1.0   2021.04
       used mcu  AT32F421C8
*/
#include "DriverConst.h"
unsigned char ucRecFeedBackFlag = FALSE;//收到正确的回复帧
unsigned char ucRecTeacherFeedBackFlag = FALSE;//收到正确的回复帧
volatile unsigned int uiLastPower = 0;
unsigned char ucLastErro = 0;
unsigned char ucRec60Bytes[60] = {0};
unsigned char ucPressLaBa = KeyPressUp;
unsigned char ucTeacherRecData[RecBufferSize] = {0};
unsigned char  ucAnlyFree = FALSE;
unsigned char ucTeacherDataTemp[10] = {0};
union uniDirverAtt uniDiverInfo = {0};//用于保存一些设置过的电机参数
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


unsigned char ucUsartDmaSendData(unsigned char *pPoint, unsigned char ucBufLen)//使用DMA将数据抛出去
{
    unsigned char *pData = 0;
    pData = &ucaUsarDmaTxdBuffer[0];
    if (ucBufLen > DmaUsartSize)
    {
        return 0;
    }
    DMA_ChannelEnable(UsedUartMdaChannle, DISABLE);/* Disable USART2 DMA TX Channel */
    UsedUartMdaChannle->TCNT = ucBufLen ;
    while(ucBufLen--)*pData++ = *pPoint++;/* 复制待发送数据 */
    DMA_ChannelEnable(UsedUartMdaChannle, ENABLE); /* Enable USART2 DMA TX Channel */
    return 1;
}
void vSendFeedBack(void)
{
    //返回收到命令帧
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
#ifdef UsedMdaSend	//如果使用DMA发送的方式
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
void vSendSysPower(unsigned int uiPower)//发送系统电量
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
        if(usTemp > 3037)usTemp = 26280;//大于27.04 五个灯
        else if((usTemp > 2952) && (usTemp <= 3037))
        {
            usTemp = 25000; //大于25.86 小于27.04  四个灯
            g_unioBatterLowPowerCom.ucAllData = 0;
        }
        else if((usTemp > 2824) && (usTemp <= 2952))
        {
            usTemp = 24800; //大于25.20小于25.86  三个灯
            g_unioBatterLowPowerCom.ucAllData = 0;
        }
        else if((usTemp > 2712) && (usTemp <= 2824))
        {
            usTemp = 24200; //大于24.56 小于25.20 二个灯
        }
        else if((usTemp > 2600) && (usTemp <= 2712))
        {
            usTemp = 23800; //大于23.16小于24.56  一个灯
            g_unioBatterLowPowerCom.ucAllData = 0;
            g_unioBatterLowPowerCom.bitSet.bitLowBarSetp1 = TRUE;
        }
        else if((usTemp > 2256) && (usTemp <= 2600))
        {
            usTemp = 23000; //大于22.40小于23.16 红灯在闪烁
            g_unioBatterLowPowerCom.ucAllData = 0;
            g_unioBatterLowPowerCom.bitSet.bitLowBarSetp3 = TRUE;
        }
        else if(usTemp <= 2256)
        {
            usTemp = 17500; //小于22.40  电源指示灯在报警
            g_unioBatterLowPowerCom.ucAllData = 0;
            g_unioBatterLowPowerCom.bitSet.bitLowBarSetp4 = TRUE;
        }
#ifdef UsedLinSuanTieLi
        usTemp = uiPower;
        if(usTemp > 2880)usTemp = 26280;//大于27.04 五个灯
        else if((usTemp > 2810) && (usTemp <= 2880))
        {
            usTemp = 25000; //大于25.86 小于27.04  四个灯
            g_unioBatterLowPowerCom.ucAllData = 0;
        }
        else if((usTemp > 2740) && (usTemp <= 2810))
        {
            usTemp = 24800; //大于25.20小于25.86  三个灯
            g_unioBatterLowPowerCom.ucAllData = 0;
        }
        else if((usTemp > 2680) && (usTemp <= 2740))
        {
            usTemp = 24200; //大于24.56 小于25.20 二个灯

        }
        else if((usTemp > 2620) && (usTemp <= 2680))
        {
            usTemp = 23800; //大于23.16小于24.56  一个灯
            g_unioBatterLowPowerCom.ucAllData = 0;
            g_unioBatterLowPowerCom.bitSet.bitLowBarSetp1 = FALSE;
        }
        else if((usTemp > 2256) && (usTemp <= 2620))
        {
            usTemp = 23000; //大于22.40小于23.16 红灯在闪烁
            g_unioBatterLowPowerCom.ucAllData = 0;
            g_unioBatterLowPowerCom.bitSet.bitLowBarSetp1 = FALSE;
        }
        else if(usTemp <= 2256)
        {
            usTemp = 17500; //小于22.40  电源指示灯在报警
            g_unioBatterLowPowerCom.ucAllData = 0;
            g_unioBatterLowPowerCom.bitSet.bitLowBarSetp3 = TRUE;
        }
        if(usTemp <= 2086)//小于 18.5v  的时候
        {
            g_unioBatterLowPowerCom.ucAllData = 0;
            g_unioBatterLowPowerCom.bitSet.bitLowBarSetp3 = TRUE;
        }
#endif
    }
    else
    {
        if(uiPower > 2744)usTemp = 26280;//大于24.25 五个灯
        else if((uiPower > 2648) && (uiPower <= 2744))
        {
            usTemp = 25000; //大于23.41 小于24.25  四个灯
        }
        else if((uiPower > 2540) && (uiPower <= 2648))
        {
            usTemp = 24800; //大于23.41小于27.60   三个灯
        }
        else if((uiPower > 2457) && (uiPower <= 2540))
        {
            usTemp = 24200; //大于22.54小于22.54   二个灯
        }
        else if((uiPower > 2373) && (uiPower <= 2457))
        {
            usTemp = 23800; //大于21.73小于21.73   一个灯
            g_unioBatterLowPowerCom.ucAllData = 0;
            g_unioBatterLowPowerCom.bitSet.bitLowBarSetp1 = TRUE;
        }
        else if((uiPower > 2034) && (uiPower <= 2373))
        {
            usTemp = 23000; //大于18.00小于21.00   红灯在闪烁
            g_unioBatterLowPowerCom.ucAllData = 0;
            g_unioBatterLowPowerCom.bitSet.bitLowBarSetp3 = TRUE;
        }
        else if(uiPower <= 2034)
        {
            usTemp = 17500; //小于18.00  电源指示灯在报警
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
#ifdef UsedMdaSend	//如果使用DMA发送的方式
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
        for(usErroTimeDelay = 0; usErroTimeDelay < ReSendTimeAsMs; usErroTimeDelay++)//循环等待时间
        {
            vDelayCom(1);
            if(ucRecFeedBackFlag)break;
        }
        if(ucRecFeedBackFlag)break;
    }
}



/*
发送多字节函数
unsigned char ucOrder 命令字内容
unsigned char *ucSendData 发送内容
unsigned char ucDataLeg 发送长度
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
    ucSendBuff[ucSendCont++] = ucOrder;//发送命令字
    for(ucTemp = 0; ucTemp < ucDataLeg; ucTemp++)
    {
        ucSendBuff[ucSendCont + ucTemp] = *(ucSendData + ucTemp); //数据内容
    }
    ucSendCont += ucDataLeg;
    ucSendBuff[ucSendCont++] = SendTail1;
    ucSendBuff[ucSendCont++] = SendTail2;
    ucSendBuff[ucSendCont++] = SendTail3;
    ucSendBuff[ucSendCont++] = SendTail4;
    ucSendBuff[2] = ucSendCont;
    for(ucRecSendCont = 0; ucRecSendCont < ErroSendTimes; ucRecSendCont++)
    {
#ifdef UsedMdaSend	//如果使用DMA发送的方式
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
        for(usErroTimeDelay = 0; usErroTimeDelay < ReSendTimeAsMs; usErroTimeDelay++)//循环等待时间
        {
            vDelayCom(1);
            if(ucRecFeedBackFlag)break;
        }
        if(Write60Bytes == ucOrder)//如果是读取60个字节就多延时一会
        {
            for(ucTemp = 0; ucTemp < 20; ucTemp++)
            {
                for(usErroTimeDelay = 0; usErroTimeDelay < ReSendTimeAsMs; usErroTimeDelay++)//循环等待时间
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
    ucSendBuff[ucSendCont++] = ucOrder;//发送命令字
    for(ucTemp = 0; ucTemp < ucDataLeg; ucTemp++)
    {
        ucSendBuff[ucSendCont + ucTemp] = *(ucSendData + ucTemp); //数据内容
    }
    ucSendCont += ucDataLeg;
    ucSendBuff[ucSendCont++] = SendTail1;
    ucSendBuff[ucSendCont++] = SendTail2;
    ucSendBuff[ucSendCont++] = SendTail3;
    ucSendBuff[ucSendCont++] = SendTail4;
    ucSendBuff[2] = ucSendCont;
    vSetTeacherPortType(TRUE);
		__set_FAULTMASK(1);  /* 禁止全局中断*/
    for(ucRecSendCont = 0; ucRecSendCont < ErroSendTimes; ucRecSendCont++)
    {
        for(ucTemp = 0; ucTemp < ucSendCont; ucTemp++)
        {
            vTeacherPortSendByte( *(ucSendBuff + ucTemp));
        }
        vErroTimeTheach();//等待第一次回复帧的时间
        if(ucRecTeacherFeedBackFlag)break;
        vFeedIwdg();
        if(OrderReadPare == ucOrder)
        {
            if(1 == ucRecSendCont)break;
        }
    }
		__set_FAULTMASK(0);  /* 使能全局中断*/
		vSetTeacherPortType(FALSE);
}

void vSendOneByteOrder(unsigned char ucOrder, unsigned char ucData) //发送
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
    ucSendBuff[ucSendCont++] = ucOrder;//发送命令字
    ucSendBuff[ucSendCont++] = ucData;//数据内容
    ucSendBuff[ucSendCont++] = SendTail1;
    ucSendBuff[ucSendCont++] = SendTail2;
    ucSendBuff[ucSendCont++] = SendTail3;
    ucSendBuff[ucSendCont++] = SendTail4;
    ucSendBuff[2] = ucSendCont;
    if(OrderErro == ucOrder)ucMingRuiErrConst = ucData;
    for(ucRecSendCont = 0; ucRecSendCont < ErroSendTimes; ucRecSendCont++)
    {
#ifdef UsedMdaSend	//如果使用DMA发送的方式
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

void vSendSingleOrder(unsigned char ucSingleOrder)//发送一个单命令
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
    ucSendBuff[ucSendCont++] = ucSingleOrder;//发送单命令内容
    ucSendBuff[ucSendCont++] = SendTail1;
    ucSendBuff[ucSendCont++] = SendTail2;
    ucSendBuff[ucSendCont++] = SendTail3;
    ucSendBuff[ucSendCont++] = SendTail4;
    ucSendBuff[2] = ucSendCont;
    if(QuitErro == ucSingleOrder)ucMingRuiErrConst = 0;
    for(ucRecSendCont = 0; ucRecSendCont < ErroSendTimes; ucRecSendCont++)
    {

#ifdef UsedMdaSend	//如果使用DMA发送的方式
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
        for(usErroTimeDelay = 0; usErroTimeDelay < ReSendTimeAsMs; usErroTimeDelay++)//循环等待时间
        {
            vDelayCom(1);
            if(ucRecFeedBackFlag)break;
        }
				if(ucRecFeedBackFlag)break;
    }
}

void vTeacherPortSendByte(unsigned char ucByte)  //发送一个字节
{
    unsigned char ucCont;
    SetTeacherPinL;
    bsp_DelayUS(TeacherBaudRatOne + 10);
    for (ucCont = 0; ucCont < 8; ucCont++)//发送8位数据位
    {
        if (ucByte & 0x01) SetTeacherPinH; //先传低位
        else SetTeacherPinL;
        bsp_DelayUS(TeacherBaudRatOne + 10);
        ucByte >>= 1;
    }
    SetTeacherPinH;//发送校验位(无)//发送结束位
    bsp_DelayUS(TeacherBaudRatOne + 10);
}

//编程器发送数据
void vSendTeacherFeedBack(void)
{
    //返回收到命令帧
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
		__set_FAULTMASK(1);  /* 禁止全局中断*/
    for(uiTemp = 0; uiTemp < ucSendCont; uiTemp++)
    {
        vTeacherPortSendByte( *(ucSendBuff + uiTemp));
    }
		__set_FAULTMASK(0);  /* 使能全局中断*/
    vSetTeacherPortType(FALSE);
}

unsigned char ucTeacherPortRevByte(void)  //发送一个字节
{
    unsigned char ucValue = 0;
    unsigned char ucCont;
    __set_FAULTMASK(1);  /* 禁止全局中断*/
    bsp_DelayUS(TeacherBaudRatOne + 45); //跳过起始位，这个时间要大些
    for (ucCont = 0; ucCont < 8; ucCont++)
    {
        ucValue >>= 1;
        if (ReadTeacherPinH) ucValue |= 0x80;
        bsp_DelayUS(TeacherBaudRatOne + 45);
    }
    __set_FAULTMASK(0);  /* 使能全局中断*/
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
    uiLastExSpeedP = uniDiverInfo.strDirverAtt.ucExternSpeakerVolume ;//保存上次的值
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


void vAnalySerialData(void)//对收到的数据帧进行分析
{
    unsigned char ucTemp;
    unsigned char ucDataBase = 0;
    unsigned char ucRecCont = 0;
    for(ucTemp = 0; ucTemp < RecBufferSize; ucTemp++)//找一组数据里的帧内容
    {
        if((SendHeard1 == ucUartRxData[ucTemp]) && (SendHeard2 == ucUartRxData[ucTemp + 1]) && (ShowDefine == ucUartRxData[ucTemp + 3])\
                && (SendTail1 == ucUartRxData[ucTemp + ucUartRxData[ucTemp + 2] - 4]) && (SendTail2 == ucUartRxData[ucTemp + ucUartRxData[ucTemp + 2] - 3]) && (SendTail3 == ucUartRxData[ucTemp + ucUartRxData[ucTemp + 2] - 2]))
        {
            //如果找到帧头，帧尾
            ucDataBase = 0x88;
            vSendFeedBack();
            ucUartRxCont = 0;
            ucUartRxData[ucTemp] = 0;
            ucUartRxData[ucTemp + 1] = 0;//不再需要这个帧
            ucUartRxData[ucTemp + ucUartRxData[ucTemp + 2] - 4] = 0;
            ucUartRxData[ucTemp + ucUartRxData[ucTemp + 2] - 3] = 0; //不再需要这个帧
            ucUartRxData[ucTemp + ucUartRxData[ucTemp + 2] - 2] = 0;
            ucUartRxData[ucTemp + ucUartRxData[ucTemp + 2] - 1] = 0; //不再需要这个帧
        }
        if((SendHeard1 == ucUartRxData[ucTemp]) && (SendHeard2 == ucUartRxData[ucTemp + 1]) && (FeedBack1 == ucUartRxData[ucTemp + 2]) && (SendTail1 == ucUartRxData[ucTemp + 3]) && (SendTail2 == ucUartRxData[ucTemp + 4])\
                && (SendTail3 == ucUartRxData[ucTemp + 5]))
        {
            ucUartRxCont = 0;
            ucUartRxData[ucTemp] = 0;//如果收到一个正确的帧内容，要把以前收到帧标志清空
            ucUartRxData[ucTemp + 1] = 0; //不再需要这个帧
            ucUartRxData[ucTemp + 2] = 0;
            ucUartRxData[ucTemp + 3] = 0;
            ucUartRxData[ucTemp + 4] = 0;
            ucUartRxData[ucTemp + 5] = 0;
            ucRecFeedBackFlag = TRUE;
        }
        if(ucDataBase)break;//进行下面解帧
    }

    if(ucDataBase)
    {
        switch(ucUartRxData[ucTemp + 4])  //查看关键字
        {
        case SpeedGrad://如果是速度帧
        {
            ucSpeedGrad = ucUartRxData[ucTemp + 5]; ///解帧
            break;
        }
        case OrderPowerOff://关机命令
        {
            break;
        }
        case Read60Bytes://读60个字节
        {
            for(ucRecCont = 0; ucRecCont < 60; ucRecCont++)
                ucRec60Bytes[ucRecCont] = ucUartRxData[ucTemp + 5 + ucRecCont];
            break;
        }
        case ReadOneByte://读单个字节
        {
            ucLastErro = ucUartRxData[ucTemp + 5]; //解帧
            break;
        }
        case OrderSpeekP://读单个字节
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
ucMotor12Ar 电机一二的属性 转动方向
ucMoto1Pwm电机1 PWM
ucMoto2Pwm电机2 PWM
uiTestTimeSecond 测试的总时间
*/
void vMotorTest(unsigned char ucMotor12Ar, unsigned char ucMoto1Pwm, unsigned char ucMoto2Pwm, unsigned int uiTestTimeSecond) //没有移植
{
    unsigned int uiTemp = 0, uiPwmMoto1T = 0, uiPwmMoto2T = 0, uiPwmCont1 = 0, uiPwmCont2 = 0 ;
    if(0 == (ucMoto1Pwm || ucMoto2Pwm))return;//if PWM is 0   End Test
    uiTemp = 100 * uiTestTimeSecond;
    if(0 == uiTemp)return;
    uiAutoPowerOffTimeCont = 0;//在老化的时候不让关机
    vSendOneByteOrder(OrderSpeekP, 0x01);//Alarm Test Is Begin
    TeachOrderAlarmTime;
    vSendOneByteOrder(OrderSpeekP, 0x02);
    if(((Motor1Test | Motor2Test) == (ucMotor12Ar & 0x0F)) && (TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn))
        CtlBrakeBikeEn;//如果电机一二同时测试开启刹车器  并且是开启刹车器的情况下的
    if(ucMotor12Ar & Motor1Test)//如果测试电机一
    {
        uiPwmMoto1T = 36 * ucMoto1Pwm;
        if(uiPwmMoto1T > 3420)uiPwmMoto1T = 3420;
    }
    if(ucMotor12Ar & Motor2Test)//如果测试电机二
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
        uiAutoPowerOffTimeCont = 0;//在老化的时候不让关机
        ucTag10ms = FALSE;
        while(FALSE == ucTag10ms);
        uiPwmCont1 += 12;
        uiPwmCont2 += 12;
        if(uiPwmCont1 < uiPwmMoto1T)
        {
            if(ucMotor12Ar & Motor1Forward)//如果是正转的方向
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
        if(uiPwmCont2 < uiPwmMoto2T)//如果是反转的方向
        {
            if(ucMotor12Ar & Motor2Forward)
            {
                Moto2AStmPwmReg = uiPwmCont2;//对PWM分别赋值
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
老化测试  ucTestPwm 测试PWM
uiForwardTime 正向测试时间
uiBackTime 反向测试时间
uiTestTotalTime 总测试时间
*/
void vDriverTest(unsigned char ucTestPwm , unsigned int uiForwardTime , unsigned int uiBackTime , unsigned int uiTestTotalTime) //没有移植
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
//配置编程口是输出，还是中断输入模式
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
    unsigned char ucEnIternSpeaker;//是否开启内喇叭
    unsigned char ucExternSpeakerVolume;//外置喇叭的音量
    union unMotorAttribute unMotorInfo;//电机有一些属性，是否要求掉换
    unsigned char ucFirstLineRatePare;//第一次刹车曲线的切线值
    unsigned char ucFistLineTurnSecondPos;//第一次刹车转换到第二次刹车的拐点值
    unsigned char ucBreakAccPare;//刹车器的减速度
    unsigned char ucForwardSpeedP;//前进速度比例系数
    unsigned char ucBackSpeedP;//后退速度比例系数
    unsigned char ucDriverAcc;//前进后退加速度
    unsigned char ucSwerveAcc;//转弯加速度
    unsigned char ucStarMinPwm;//启动最小PWM
    unsigned char ucStopMinPwm;//停止最小PWM
    unsigned char ucLetRightMotorP;//左右电机比例系数
    unsigned char ucLetRightMotorTiedP;//左右电机绑定系数
    unsigned char ucMaxDriverCurrent;//最大驱动电流
    unsigned char ucFactryIdData1;//出厂设置32位，最低8位
    unsigned char ucFactryIdData2;//出厂设置32位，低8位
    unsigned char ucFactryIdData3;//出厂设置32位，高8位
    unsigned char ucFactryIdData4;//出厂设置32位，最高8位
    unsigned char ucForwardMinSpeedP;//前进最小速度
    unsigned char ucBackMinSpeedP;//后退最小速度
    unsigned char ucSwerveMaxSpeedP;//转弯最大速度
    unsigned char ucSwerveMinSpeedP;//转弯最小速度
    union unDelayBreakTime unDelayBreakActTime;//延迟关闭刹车器时间
*/
void vSaveTeacherData(unsigned char ucDataStartByte)//没有移植
{
    unsigned char ucTemp = 0 ;
    //if(ucTeacherRecData[ucTemp + ucDataStartByte] > 1)return; //是否开启内喇叭
    //     if(ucTeacherRecData[ucTemp + ucDataStartByte + 1] > 10)return; //1//外置喇叭的音量
    //     if(ucTeacherRecData[ucTemp + ucDataStartByte + 2] > 0x3F)return; // 5位//电机有一些属性，是否要求掉换
    if((ucTeacherRecData[ucTemp + ucDataStartByte + 3] > 10) || (ucTeacherRecData[ucTemp + ucDataStartByte + 3] < 3))return; //6//第一次刹车曲线的切线值
    if((ucTeacherRecData[ucTemp + ucDataStartByte + 4] > 10) || (ucTeacherRecData[ucTemp + ucDataStartByte + 4] < 3))return; //7//第一次刹车转换到第二次刹车的拐点值
    if((ucTeacherRecData[ucTemp + ucDataStartByte + 5] > 10) || (ucTeacherRecData[ucTemp + ucDataStartByte + 5] < 3))return; //8//刹车器的减速度
    if(ucTeacherRecData[ucTemp + ucDataStartByte + 6] > 100)return; //9//前进速度比例系数
    if(ucTeacherRecData[ucTemp + ucDataStartByte + 7] > 100)return; //10//后退速度比例系数
    if((ucTeacherRecData[ucTemp + ucDataStartByte + 8] > 10) || (0 == ucTeacherRecData[ucTemp + ucDataStartByte + 8]))return; //11//前进后退加速度
    if((ucTeacherRecData[ucTemp + ucDataStartByte + 9] > 10) || (0 == ucTeacherRecData[ucTemp + ucDataStartByte + 9] ))return;	//12//转弯加速度
    if(ucTeacherRecData[ucTemp + ucDataStartByte + 10] > 100)return;	//13//启动最小PWM
    if(ucTeacherRecData[ucTemp + ucDataStartByte + 11] > 100)return;	//14//停止最小PWM
    if(ucTeacherRecData[ucTemp + ucDataStartByte + 12] > 10)return;	//15//左右电机比例系数
    if(ucTeacherRecData[ucTemp + ucDataStartByte + 13] > 10)return;	//16//左右电机绑定系数
    if(ucTeacherRecData[ucTemp + ucDataStartByte + 14] > 50)return;	//17//最大驱动电流

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
    ucRec60Bytes[59] = (WriteEepromData >> 8) & 0xFF; //写入被占用标志
    vSendMultBytesOrder(Write60Bytes, ucRec60Bytes, 60);
    ucTag400ms = FALSE;
    while(FALSE == ucTag400ms);
    for(ucTemp = 0; ucTemp < 60; ucTemp++)TeachOrderAlarmTime;
    vSendOneByteOrder(OrderSpeekP, 0x01);//Alarm Test Is Begin
    TeachOrderAlarmTime;TeachOrderAlarmTime;
    vSendOneByteOrder(OrderSpeekP, 0x02);
    TeachOrderAlarmTime
    vReadSysTempPare(FALSE);// 重新加载一下系统的一些参数，只是不需要读EEPROM
}


void vAnalyTeacherDataOfTime(void)
{
    unsigned char ucTemp = 0;
    ucAnlyFree = FALSE;//
		if(ucTeacherDataTemp[0])CtrlExtSpeekerDis;//20200327 蜂蜜器 强制不输出
    switch(ucTeacherDataTemp[0])//查看关键字
    {
    case OrderSpeekP://喇叭测试
    {
        if(0x01 == ucTeacherDataTemp[1])
            vInternSpeekTest( ucTeacherDataTemp[2]);
        if(0x02 == ucTeacherDataTemp[1])
            vExternSpeekTest( ucTeacherDataTemp[2], ucTeacherDataTemp[3]);
        for(ucTemp = 0; ucTemp < 5; ucTemp++)ucTeacherDataTemp[ucTemp] = 0x00;
        break;
    }
    case OrderBreak://刹车器测试
    {
        if(0x01 == ucTeacherDataTemp[1])
            vBreakTest( ucTeacherDataTemp[2]);
        if(0x02 == ucTeacherDataTemp[1])CtlBrakeBikeDisEn;
        for(ucTemp = 0; ucTemp < 5; ucTemp++)ucTeacherDataTemp[ucTemp] = 0x00;
        break;
    }
    case OrderMotor://电机测试
    {
        vMotorTest(ucTeacherDataTemp[1], ucTeacherDataTemp[2], ucTeacherDataTemp[3], ucTeacherDataTemp[4]);
        for(ucTemp = 0; ucTemp < 5; ucTemp++)ucTeacherDataTemp[ucTemp] = 0x00;
        break;
    }
    case OrderRunTest://老化测试
    {
        vDriverTest(ucTeacherDataTemp[1], (ucTeacherDataTemp[2] << 8) + ucTeacherDataTemp[3], (ucTeacherDataTemp[4] << 8) + ucTeacherDataTemp[5],
                    (ucTeacherDataTemp[6] << 8) + ucTeacherDataTemp[7]);
        for(ucTemp = 0; ucTemp < 5; ucTemp++)ucTeacherDataTemp[ucTemp] = 0x00;
        break;
    }
    case OrderSendPare://
    {
        vSaveTeacherData(ucTeacherDataTemp[1]);//没有移植
        for(ucTemp = 0; ucTemp < 5; ucTemp++)ucTeacherDataTemp[ucTemp] = 0x00;
        //ucCheckMotoConnect();
        if(ucErroType)vShowErroToDis(0);
        break;
    }
    case OrderReadPare:// 请求数据
    {
        ucTemp = uniDiverInfo.strDirverAtt.ucEnIternSpeaker;//
        if(ucTemp)uniDiverInfo.strDirverAtt.ucEnIternSpeaker = TRUE;
        else uniDiverInfo.strDirverAtt.ucEnIternSpeaker = FALSE;
        //以下用于识别硬件版本，HK32 或STM32   202002212011
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
    for(ucTemp = 0; ucTemp < RecBufferSize; ucTemp++)//找一组数据里的帧内容
    {
        if((SendHeard1 == ucTeacherRecData[ucTemp]) && (SendHeard2 == ucTeacherRecData[ucTemp + 1]) && (TeacherAdd == ucTeacherRecData[ucTemp + 3])\
                && (SendTail1 == ucTeacherRecData[ucTemp + ucTeacherRecData[ucTemp + 2] - 4]) && (SendTail2 == ucTeacherRecData[ucTemp + ucTeacherRecData[ucTemp + 2] - 3]) \
                && (SendTail3 == ucTeacherRecData[ucTemp + ucTeacherRecData[ucTemp + 2] - 2]))
        {
            //如果找到帧头，帧尾
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
            ucTeacherRecData[ucTemp + 1] = 0; //不再需要这个帧
            ucTeacherRecData[ucTemp + ucTeacherRecData[ucTemp + 2] - 4] = 0;
            ucTeacherRecData[ucTemp + ucTeacherRecData[ucTemp + 2] - 3] = 0; //不再需要这个帧
            ucTeacherRecData[ucTemp + ucTeacherRecData[ucTemp + 2] - 2] = 0;
            ucTeacherRecData[ucTemp + ucTeacherRecData[ucTemp + 2] - 1] = 0; //不再需要这个帧
        }
        if((SendHeard1 == ucTeacherRecData[ucTemp]) && (SendHeard2 == ucTeacherRecData[ucTemp + 1]) && (FeedBack1 == ucTeacherRecData[ucTemp + 2]) \
                && (SendTail1 == ucTeacherRecData[ucTemp + 3]) && (SendTail2 == ucTeacherRecData[ucTemp + 4])\
                && (SendTail3 == ucTeacherRecData[ucTemp + 5]) && (SendTail4 == ucTeacherRecData[ucTemp + 6]) )
        {
            ucTeacherRxCont = 0;
            ucTeacherRecData[ucTemp] = 0;//如果收到一个正确的帧内容，要把以前收到帧标志清空
            ucTeacherRecData[ucTemp + 1] = 0; //不再需要这个帧
            ucTeacherRecData[ucTemp + 2] = 0;
            ucTeacherRecData[ucTemp + 3] = 0;
            ucTeacherRecData[ucTemp + 4] = 0;
            ucTeacherRecData[ucTemp + 5] = 0;
            ucRecTeacherFeedBackFlag = TRUE;
        }
        if(ucDataBase)break;///进行下面解帧
    }

    if(ucDataBase)
    {
        switch(ucTeacherRecData[ucTemp + 4]) //查看关键字
        {
        case Disp10Led://点亮发光二极管
        {
            ucTempArry[0] = ucTeacherRecData[5];
            ucTempArry[1] =  ucTeacherRecData[6];
            vSendMultBytesOrder(Disp10Led, ucTempArry, 2);
            break;
        }
        case OutDispLed://退出Led显示界面
        {
            vSendSingleOrder(OutDispLed);
            break;
        }
        case OrderMotor://电机测试
        {
            ucTeacherDataTemp[0] = ucTeacherRecData[ucTemp + 4];
            ucTeacherDataTemp[1] = ucTeacherRecData[ucTemp + 5];
            ucTeacherDataTemp[2] = ucTeacherRecData[ucTemp + 6];
            ucTeacherDataTemp[3] = ucTeacherRecData[ucTemp + 7];
            ucTeacherDataTemp[4] = ucTeacherRecData[ucTemp + 8];
            ucAnlyFree = TRUE;
            break;
        }
        case OrderRunTest://驱动器老化测试
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
        case OrderBreak://刹车器测试
        {
            ucTeacherDataTemp[0] = ucTeacherRecData[ucTemp + 4];
            ucTeacherDataTemp[1] = ucTeacherRecData[ucTemp + 5];
            ucTeacherDataTemp[2] = ucTeacherRecData[ucTemp + 6];
            ucTeacherDataTemp[3] = ucTeacherRecData[ucTemp + 7];
            ucAnlyFree = TRUE;
            break;
        }
        case OrderSpeekP://测试喇叭音量
        {
            ucTeacherDataTemp[0] = ucTeacherRecData[ucTemp + 4];
            ucTeacherDataTemp[1] = ucTeacherRecData[ucTemp + 5];
            ucTeacherDataTemp[2] = ucTeacherRecData[ucTemp + 6];
            ucTeacherDataTemp[3] = ucTeacherRecData[ucTemp + 7];
            ucAnlyFree = TRUE;
            break;
        }
        case OrderSendPare://发送用户设置的信息内容
        {
            ucTeacherDataTemp[0] = ucTeacherRecData[ucTemp + 4];
            ucTeacherDataTemp[1] = ucTemp + 5;
            ucAnlyFree = TRUE;
            break;
        }
        case OrderReadPare:// 请求数据
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


