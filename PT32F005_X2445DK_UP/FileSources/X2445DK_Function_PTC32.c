

#include "X2445DK_Const.h"

unsigned short usSetPwmValue = 0;
unsigned int uiConvertTimes = 0;
unsigned int uiMaxAd = 0;
unsigned int uiMinAd = 0xFFFF;
unsigned long l_gAdTemp = 0;              //unsigned long 32位
unsigned int  uiAdVoltage = 0;
unsigned char ucBattType = 0;
unsigned char ucSpeedGrad;  //速度
unsigned char GLOBAL_SPEED_GRAD = 1;  //全局速度


unsigned char ucLessThanLowPower = FALSE; //电源电压小于23.740的标志
unsigned char ucLess18Power = FALSE;      //电源电压小于18的标志
unsigned char ucBuzzerPressedFlg = FALSE;    //蜂鸣器按下标志


unsigned char ucKeyPressFlag = TRUE;
unsigned char ucBuzzerTstFlg = FALSE; //蜂鸣器测试标志
unsigned char ucDebugSpeaker = 0;
//unsigned char PowerOFF = FALSE; //关机标志



Peripheral_str strPeripheral;
unsigned short usReadSpeakerPwm(void)
{
    return (PWM1->MR4);
}
void delay(unsigned long i)
{
    while (i--);
}

/*******获得AD的值*******/
//unsigned int ADC_GetConversionValue(void)
//{
//    return(((unsigned short)ADC_RES << 8) | ADC_RESL);
//}

/*************处理AD****************/
void vDealAd(void)
{
#define DelaySetAdcRegTime 20
    unsigned int uiAdValue = 0;
    if(ADC_GetFlagStatus(ADC, ADC_FLAG_ADRDY))ADC_StartOfConversion(ADC);	//启动转换
    if(ADC_GetFlagStatus(ADC, ADC_FLAG_EOC) == 0)return;
    ADC_ClearFlag(ADC, ADC_FLAG_EOC);
    for(uiAdValue = 0; uiAdValue < DelaySetAdcRegTime; uiAdValue++);
    uiAdValue = ADC_GetConversionValue(ADC);	//获取结果
    uiConvertTimes++;
    if(uiAdValue > uiMaxAd)uiMaxAd = uiAdValue;
    if(uiAdValue < uiMinAd)uiMinAd = uiAdValue;
    l_gAdTemp += uiAdValue;
    if(ConverTimes == uiConvertTimes)
    {
        uiAdVoltage = (l_gAdTemp - uiMaxAd - uiMinAd) >> 2; //把6次采样值  减去最大减去最小除以4
        uiMinAd = 0xFFFF;
        uiMaxAd = 0;
        l_gAdTemp = 0;
        uiConvertTimes = 0;
    }
}
#ifdef InDeBugMode
//const unsigned char code uiAutoAddFactData[60] =
//{
//    0xEF, 0x0A, 0x1C, 0x07, 0x07, 0x07, 0x62, 0x32, 0x05, 0x07, 0x05, 0x05,
//    0x05, 0x06, 0x14, 0x00, 0x00, 0x00, 0x00, 0x44, 0x14, 0x2D, 0x14, 0x00,
//    0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00,
//    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA, 0x55,
//};
unsigned char  uiAutoAddFactData[60] =
{
    0xEF, 0x0A, 0x00, 0x07, 0x07, 0x07, 0x62, 0x32, 0x05, 0x07, 0x05, 0x05,
    0x05, 0x06, 0x14, 0x00, 0x00, 0x00, 0x00, 0x44, 0x14, 0x2D, 0x14, 0x00,
    0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA, 0x55,
};
#endif
unsigned char ucDebugSwitch = 0;
void vCheckFactData(void)
{
    unsigned char uiCont = 0;
    unsigned char ucTemp = 0;
    unsigned char ucTempData = 0;
    uiCont = 0;
	  EEPROM_read_n(EEPROM_START_ADDRESS_FACT_PARE,&ucEepromBuf[0],60);
    for(ucTemp = 0; ucTemp < EEPROM_START_ADDRESS_FACT_PARE_Legth; ucTemp++)
    {
        ucTempData = ucEepromBuf[ucTemp];
        if(0xFF == ucTempData)uiCont++;

    }
    //		uiCont = 51;//0x19B0
    if(uiCont > 50)
    {
        for(ucTemp = 0; ucTemp < 60; ucTemp++)ucEepromBuf[ucTemp] = 0;
#ifdef InDeBugMode
        for(ucTemp = 0; ucTemp < 60; ucTemp++)ucEepromBuf[ucTemp] = uiAutoAddFactData[ucTemp];
#endif
        //vWrite60Bytes();
        strPeripheral.ucRecvWrite60Bytes_Flag = 1;
    }

}

//volatile unsigned char code cucSpeedData _at_ SPEED_ADDRESS;
void Peripheral_Init(Peripheral_str *p)
{

    p->Input.all = 0;
    p->Output.all = 0;

    p->SpeedDownKeyTime = 0;
    p->SpeedUpKeyTime = 0;
    p->SpeakerKeyTime = 0;
    p->AlarmTime = 0;

    p->BackSpeekTime = 0;
    p->PowerLEDShowTime = 0;

    p->ucPowerLedNum = 0;
    p->ucPowerLedShowPhase = 0;

    p->usRecvPowerON_Flag = 0;
    p->usRecvPowerOFF_Flag = 0;

    p->ucRecvData_Flag = 0;
    p->ucRecvPowerValue_Flag = 0;
    p->ucRecvWrite60Bytes_Flag = 0;

    p->ucRecvAlarmNum = 0;
    p->uiRecvPowerValue = 0;  //电量
    p->ucRecvBikeInBack = FALSE;
    p->ucRecvOrderInCharge  = NO_USE; //是否在充电

    p->ucRecvTestFlag  = FALSE;

}
/*****第一次开机，开机唤醒*********/
unsigned char ucBatType = 0;
void vFirstStart(void)
{
    //unsigned char ReadCnt = 0;
    vSysSourceInit();//系统初始化
    delay(10);
    __set_PRIMASK(1);//关闭中断//clr_EA;

    GLOBAL_SPEED_GRAD = ucReadEepromOneData(SPEED_ADDRESS);//cucSpeedData;//读取FLASE 中的数据内容
    ucSpeedGrad = GLOBAL_SPEED_GRAD;
    EEPROM_read_n(EEPROM_START_ADDRESS_FACT_PARE, &ucEepromBuf[0], EEPROM_START_ADDRESS_FACT_PARE_Legth);

    ucBattType = ucReadEepromOneData(WriteBatteryTypeFlag_Addr);
		ucBatType = ucEepromBuf[2];//将电池类型缓存下来
    if(0 == ucSpeedGrad)
    {
        ucSpeedGrad = 1;
    }
    else if((ucSpeedGrad >= 5) && (ucSpeedGrad < 100))
    {
        ucSpeedGrad = 5;
    }
    else if(ucSpeedGrad >= 100)//如果开机的时候是0xFF
    {
        ucSpeedGrad = 1;
    }
    if((PowerOnSpeedGradeAt > 0) && (PowerOnSpeedGradeAt < 6))ucSpeedGrad = PowerOnSpeedGradeAt;

    ucLessThanLowPower = FALSE;
    ucLess18Power = FALSE;
    strPeripheral.ucRecvOrderInCharge = NO_USE;
    strPeripheral.ucRecvBikeInBack = FALSE;
    strPeripheral.uiRecvPowerValue = 0;
    Peripheral_Init(&strPeripheral);//参数初始化
    vCheckFactData();//检测数据帧内容，如果没有数据自行烧写一次

    //////////////////////////////////以下仅为调试模式下有效果
#ifdef InDeBugMode
    strPeripheral.ucRecvData_Flag = 0x03;
    strPeripheral.uiRecvPowerValue = 40000;//电量不为零时才检测按键
    //
    SpeedLedOn(ucSpeedGrad);
    PowerLedOn(strPeripheral.uiRecvPowerValue); //显示电量
#endif

    __set_PRIMASK(0);//开启中断。//set_EA;//打开中断
}

/*****显示电量指示灯********/
void PowerLedNumOn(unsigned char num)
{

    switch (num)
    {
    case 0:
        POWER_SHOW_0;
        break;
    case 1:
        POWER_SHOW_1;
        break;
    case 2:
        POWER_SHOW_2;
        break;
    case 3:
        POWER_SHOW_3;
        break;
    case 4:
        POWER_SHOW_4;
        break;
    case 5:
        POWER_SHOW_5;
        break;
    default:
        break;
    }
}

/*******显示速度指示灯*************/
void SpeedLedOn(unsigned char num)
{
    switch (num)
    {
    case 1:
        SPEED_LED_1;
        break;
    case 2:
        SPEED_LED_2;
        break;
    case 3:
        SPEED_LED_3;
        break;
    case 4:
        SPEED_LED_4;
        break;
    case 5:
        SPEED_LED_5;
        break;
    default:
        break;
    }
}

void PowerLedOn(unsigned short power)    //unsigned short为16位
{
    if(power < 100)return;
    if (power < ALARM_POWER) ucLess18Power = TRUE;
    else ucLess18Power = FALSE;

    if (power <= LOW_POWER)
    {
        if(ucLessThanLowPower == FALSE)
        {
            ucLessThanLowPower = TRUE;
            POWER_SHOW_0;
        }
    }
    else if (power <= ONE_LED_POWER)
    {
        POWER_SHOW_1;
        ucLessThanLowPower = FALSE;
    }
    else if (power <= TWO_LED_POWER)
    {
        POWER_SHOW_2;
        ucLessThanLowPower = FALSE;
    }
    else if (power <= THREE_LED_POWER)
    {
        POWER_SHOW_3;
        ucLessThanLowPower = FALSE;
    }
    else if (power <= FOUR_LED_POWER)
    {
        POWER_SHOW_4;
        ucLessThanLowPower = FALSE;
    }
    else
    {
        POWER_SHOW_5;
        ucLessThanLowPower = FALSE;
    }
    if(TRUE == ucLessThanLowPower)//欠压
    {
        if(strPeripheral.PowerLEDShowTime > TIMER_200MS)
        {
            strPeripheral.PowerLEDShowTime = 0;
            XorRedPowerLed;
            if(ucLess18Power)//电压低于18V
            {
                if ((!strPeripheral.ucRecvBikeInBack) && (!ucBuzzerTstFlg) && (!ucBuzzerPressedFlg) && (!strPeripheral.ucRecvAlarmNum))
                {
                    switch(strPeripheral.ucPowerLedShowPhase)
                    {
                    case 0:
                        SetPWM1_Out(SpeedKeyTime, SpeedKeyPulse);
                        strPeripheral.ucPowerLedShowPhase = 1;
                        break;
                    case 1:
                        SetPWM1_Out(SpeedKeyTime, 0);ucDebugSpeaker = 1;
                        strPeripheral.ucPowerLedShowPhase = 0;
                        break;
                    default:
                        SetPWM1_Out(SpeedKeyTime, 0);ucDebugSpeaker = 2;
                        strPeripheral.ucPowerLedShowPhase = 0;
                        break;
                    }
                }
            }
            else
            {
                SetPWM1_Out(SpeedKeyTime, 0);ucDebugSpeaker = 3;
                strPeripheral.ucPowerLedShowPhase = 0;
            }
        }

    }
    else strPeripheral.PowerLEDShowTime = 0;
}




/*******按电源键关机****/
void vPowerPressed(void)
{
    if(TRUE == strPeripheral.usRecvPowerOFF_Flag)//将速度值写入EEPROM
    {
        if(GLOBAL_SPEED_GRAD != ucSpeedGrad)
        {
            EEPROM_WriteByte(SPEED_ADDRESS, ucSpeedGrad);
            GLOBAL_SPEED_GRAD = ucSpeedGrad;
        }
        strPeripheral.usRecvPowerOFF_Flag = TRUE;
        strPeripheral.ucRecvData_Flag = FALSE;
    }
    //如果有电源开机按键的时候就去 存储按键值 20181224
    if(POWER_ON_KEY_PRESSED)
    {
        __set_PRIMASK(1);//关闭中断//clr_EA;
        if(GLOBAL_SPEED_GRAD != ucSpeedGrad)
        {
            EEPROM_WriteByte(SPEED_ADDRESS, ucSpeedGrad);
            GLOBAL_SPEED_GRAD = ucSpeedGrad;
        }
        __set_PRIMASK(0);//开启中断。//set_EA;
    }
}


/**************EEPROM写入数据***********/


void vArom_WriteBytes_Fact(unsigned char *pData)//直接写入一个数据 不用再交换数据了
{

    EEPROM_write_n(EEPROM_START_ADDRESS_FACT_PARE, pData, EEPROM_START_ADDRESS_FACT_PARE_Legth);
}

/**********主控写入60字节数据**************/
void vWrite60Bytes(void)
{
    __set_PRIMASK(1);//关闭中断//clr_EA;
    vArom_WriteBytes_Fact(&ucEepromBuf[0]);
    __set_PRIMASK(0);//开启中断。//set_EA;
    UART_Cmd(UART0, ENABLE);//set_ES;// 打开串口中断使能--未调试
}

void vTestShowLed(unsigned int value)
{
    if (value & 0x0020)
    {
        SPEED_STEP1_ON;
    }
    else SPEED_STEP1_OFF;

    if (value & 0x0040)
    {
        SPEED_STEP2_ON;
    }
    else SPEED_STEP2_OFF;

    if (value & 0x0080)
    {
        SPEED_STEP3_ON;   //方便不开机的时候  快速区分按键版本的不同
    }
    else SPEED_STEP3_OFF;

    if (value & 0x0100)
    {
        SPEED_STEP4_ON;
    }
    else SPEED_STEP4_OFF;

    if (value & 0x0200)
    {
        SPEED_STEP5_ON;
    }
    else
    {
        SPEED_STEP5_OFF;
    }
}

void vInitLedPin(void)
{
    POWER_SHOW_0;
    SPEED_LED_0;
    //    GPIO_InitPT32();
}

void SetPWM1_Out(unsigned int x, unsigned int y)
{
	if(x > 0){
	  x = 2000;
	}
	if(y > 0){
	  y = 50;
	}
	
	PWM1->MR0 = x;
	PWM1->MR4 = y;
}


void Peripheral_GPIOInput(unsigned int  Voltage)
{
    if(Globle_Timer_1MS.Bits.CheckKeyPress)
    {
        Globle_Timer_1MS.Bits.CheckKeyPress = 0;
        if(Voltage > 3350)//4096
        {
            ucKeyPressFlag = FALSE;
            strPeripheral.SpeedUpKeyTime = 0;
            strPeripheral.SpeedDownKeyTime = 0;
            strPeripheral.SpeakerKeyTime = 0;
            strPeripheral.SpeedDownAndUpTime = 0;
            strPeripheral.SpeakerAndSpeedUpTime = 0;
            strPeripheral.Input.Bits.SpeedDownKeyPressFlag =  0;
#ifdef EnableCancelBackCarAlarm
            strPeripheral.Input.all &= 0xB0;
#else
            strPeripheral.Input.all &= 0x30;
#endif
        }
        else if((FALSE == ucKeyPressFlag) && (Voltage < 3300) && (Voltage > 2819))//else if((FALSE == ucKeyPressFlag) && (Voltage < 3300) && (Voltage > 2800))//+  //3300  2800  -->ADC as 2908  MC51F003  2940
        {
            strPeripheral.SpeedUpKeyTime++;
            strPeripheral.SpeedDownKeyTime = 0;
            strPeripheral.SpeedDownAndUpTime = 0;
            strPeripheral.Input.Bits.SpeedDownKeyPressFlag =  0;
            if(strPeripheral.SpeedUpKeyTime > TIMER_20MS)
            {
                strPeripheral.SpeedUpKeyTime = TIMER_20MS;
                strPeripheral.Input.Bits.SpeedUpKey = TRUE;
                ucKeyPressFlag = TRUE;
            }
        }
        else if((FALSE == ucKeyPressFlag) && (Voltage < 2715) && (Voltage > 2514))//else if((FALSE == ucKeyPressFlag) && (Voltage < 2680) && (Voltage > 2450))//-  // 2750 2450   -->ADC as 2569    MC51F003 2590
        {
            strPeripheral.SpeedUpKeyTime = 0;
            strPeripheral.SpeedDownAndUpTime = 0;
            strPeripheral.SpeedDownKeyTime++;
            if(strPeripheral.SpeedDownKeyTime > TIMER_20MS)
            {
                strPeripheral.SpeedDownKeyTime = TIMER_20MS;
                strPeripheral.Input.Bits.SpeedDownKey = TRUE;
                strPeripheral.Input.Bits.SpeedDownKeyPressFlag = TRUE;
                ucKeyPressFlag = TRUE;
            }
        }
        else if((TRUE == strPeripheral.Input.Bits.SpeedDownKeyPressFlag) && (Voltage < 2435) && (Voltage > 2265))//else if((TRUE == strPeripheral.Input.Bits.SpeedDownKeyPressFlag) && (Voltage < 2437) && (Voltage > 2237)) //+ -  同时按下2450 2200  -->ADC as 2337    MC51F003  2355
        {
            strPeripheral.SpeedDownAndUpTime++;  //必须按键减先按下去
            if(strPeripheral.SpeedDownAndUpTime > TIMER_4000MS)
            {
                strPeripheral.SpeedDownAndUpTime = TIMER_4000MS;
                if(strPeripheral.Input.Bits.PowerChange == FALSE)//20190923修改切换电池类型不进行修改最高时速时
                    strPeripheral.Input.Bits.SpeedChange = TRUE;
                ucKeyPressFlag = TRUE;
                POWER_SHOW_5;
                SPEED_LED_5;
                SetPWM1_Out(SpeedKeyTime, SpeedKeyPulse);  //输出控制蜂鸣器叫
            }
        }
        else if((Voltage < 2164) && (Voltage > 1944))//else if((Voltage < 2200) && (Voltage > 1900)) //2054 喇叭2200 1900    -->ADC as 2049    MC51F003  2074
        {
            strPeripheral.SpeakerKeyTime++;
            strPeripheral.SpeedDownAndUpTime = 0;
            strPeripheral.SpeakerAndSpeedUpTime = 0;
            strPeripheral.Input.Bits.SpeedDownKeyPressFlag =  0;
            if(strPeripheral.SpeakerKeyTime > TIMER_20MS)
            {
                strPeripheral.SpeakerKeyTime = TIMER_20MS;
                strPeripheral.Input.Bits.SpeakerKey = TRUE;
                //ucKeyPressFlag = TRUE;
            }
        }
        else if((TRUE == strPeripheral.Input.Bits.SpeakerKey) && (Voltage < 1852) && (Voltage > 1680)) //else if((TRUE == strPeripheral.Input.Bits.SpeakerKey) && (Voltage < 1900) && (Voltage > 1650)) //1706喇叭 +  同时按下1900  1650  -->ADC as1702    MC51F003  1722
        {
            //必须按键加先按下去
            strPeripheral.SpeakerAndSpeedUpTime++;
            if(strPeripheral.SpeakerAndSpeedUpTime > 1000)
            {
                strPeripheral.SpeakerAndSpeedUpTime = 1000;
                strPeripheral.Input.Bits.SpeakerKey = FALSE;
                ucKeyPressFlag = TRUE;

                if((TRUE != strPeripheral.ucRecvOrderInCharge) && (0 == strPeripheral.ucRecvAlarmNum) && \
                        (strPeripheral.Input.Bits.SpeedModify == FALSE))//20190923修改切换最高时速时不进行修改电池类型
                {
                    if(strPeripheral.Input.Bits.PowerChange == FALSE)//20190807 修改电池类型 重新上电后有效
                    {

                        if((ucEepromBuf[2] & 0x40) == 0x40)//锂电池
                        {
                            ucEepromBuf[2] &= 0xBF;//铅酸
                        }
                        else
                        {
                            ucEepromBuf[2] |= 0x40;	//锂电池
                        }
                        vWrite60Bytes();
                        strPeripheral.Input.Bits.PowerChange = TRUE;
                        vSendLedStatus(strPeripheral.Input.all, ucEepromBuf[2]);
                    }
                    SetPWM1_Out(SpeedKeyTime, SpeedKeyPulse);
                    SpeedLedOn(ucSpeedGrad);
                    strPeripheral.Output.Bits.Alarm = TRUE;
                }
            }
        }
#ifdef EnableCancelBackCarAlarm
        else if((TRUE == strPeripheral.Input.Bits.SpeakerKey) && (Voltage < 1650) && (Voltage > 1400)) //else if((TRUE == strPeripheral.Input.Bits.SpeakerKey) && (Voltage < 1900) && (Voltage > 1650)) //1706喇叭 +  同时按下1900  1650  -->ADC as1702    MC51F003  1722
        {
            //必须按键加先按下去
            strPeripheral.SpeedDownKeyTime++;
            if(strPeripheral.SpeedDownKeyTime > 1000)
            {
                strPeripheral.SpeedDownKeyTime = 1000;
                strPeripheral.Input.Bits.SpeakerKey = FALSE;
                ucKeyPressFlag = TRUE;

                if((TRUE != strPeripheral.ucRecvOrderInCharge) && (0 == strPeripheral.ucRecvAlarmNum) && \
                        (strPeripheral.Input.Bits.BackCarAlarmModify == FALSE))//20190923修改切换最高时速时不进行修改电池类型
                {
                    if(strPeripheral.Input.Bits.PowerChange == FALSE)//20190807 修改电池类型 重新上电后有效
                    {
                        if(ucEepromBuf[0])ucEepromBuf[0] = 0;//如果内置喇叭有声响了，就不要再响了
                        else ucEepromBuf[0] = 1;	//如果内置喇叭无声响了，再次开启
                        strPeripheral.Input.Bits.PowerChange = TRUE;
                        strPeripheral.Input.Bits.BackCarAlarmModify = TRUE;
                        vWrite60Bytes();
                    }
                    SetPWM1_Out(SpeedKeyTime, SpeedKeyPulse);
                    SpeedLedOn(ucSpeedGrad);
                    strPeripheral.Output.Bits.Alarm = TRUE;
                }
            }
        }
#endif
        //        else if((FALSE == ucKeyPressFlag) && (Voltage < 1650) && (Voltage > 1400)) //1584 喇叭 -  同时按下
        //        {//取消原喇叭与速度减一起调节速度功能
        //            strPeripheral.SpeedDownKeyTime++;
        //            if(strPeripheral.SpeedDownKeyTime > TIMER_100MS)
        //            {
        //                strPeripheral.SpeedDownKeyTime = TIMER_100MS;
        //                strPeripheral.Input.Bits.SpeedChange = FALSE;
        //                strPeripheral.Input.Bits.SpeakerKey = FALSE;
        //                ucKeyPressFlag = TRUE;
        //            }
        //        }
        else if(Voltage <  1400) //同时按下
        {
            if(strPeripheral.Input.Bits.SpeakerKey)
            {
                strPeripheral.Input.Bits.SpeakerKey = FALSE;
            }
        }
        /*if(strPeripheral.Input.Bits.SpeakerKey)
        {
        		if(Voltage > 300)
        		{
        			strPeripheral.Input.Bits.SpeakerKey = FALSE;
        		}
        }
        if(Voltage < 300)
        {
        		strPeripheral.SpeakerKeyTime++;
        		if(strPeripheral.SpeakerKeyTime > TIMER_10MS)
        		{
        				strPeripheral.SpeakerKeyTime = TIMER_10MS;
        				strPeripheral.Input.Bits.SpeakerKey = TRUE;
        		}
        }*/
    }
}

void PeripheralInputHandle(void)
{

    Peripheral_GPIOInput(uiAdVoltage);


    if(strPeripheral.Input.Bits.SpeedUpKey)
    {
        if ( (TRUE != strPeripheral.ucRecvOrderInCharge) && (0 == strPeripheral.ucRecvAlarmNum))
        {
            strPeripheral.Input.Bits.SpeedUpKey = 0;
            ucSpeedGrad++;
            if(ucSpeedGrad > 5)
            {
                ucSpeedGrad = 5;
                SetPWM1_Out(SpeedKeyLimitTime, SpeedKeyLimitPulse);
                if(strPeripheral.Input.Bits.SpeedModify)//20190807 修改最高时速 重新上电后有效
                {
                    ucEepromBuf[6] = 87;
                    vWrite60Bytes();
                }
            }
            else
            {
                SetPWM1_Out(SpeedKeyTime, SpeedKeyPulse);
                SpeedLedOn(ucSpeedGrad);
                if(strPeripheral.Input.Bits.SpeedModify)//20190807 修改最高时速 重新上电后有效
                {
                    if(ucEepromBuf[6] < 87)
                    {
                        ucEepromBuf[6] += 10;
                    }
                    vWrite60Bytes();
                    vSendLedStatus(strPeripheral.Input.all, ucEepromBuf[6]);
                }
                else vSendSpeedGrad(ucSpeedGrad);
            }
            strPeripheral.Output.Bits.Alarm = TRUE;

        }
        else strPeripheral.Input.Bits.SpeedUpKey = 0;
    }
    if(strPeripheral.Input.Bits.SpeedDownKey)
    {
        strPeripheral.Input.Bits.SpeedDownKey = 0;

        if((TRUE != strPeripheral.ucRecvOrderInCharge) && (0 == strPeripheral.ucRecvAlarmNum))
        {
            ucSpeedGrad--;
            if(0 == ucSpeedGrad)
            {
                ucSpeedGrad = 1;
                SetPWM1_Out(SpeedKeyLimitTime, SpeedKeyLimitPulse);
                if(strPeripheral.Input.Bits.SpeedModify)//20190807 修改最高时速 重新上电后有效
                {
                    ucEepromBuf[6] = 47;
                    vWrite60Bytes();
                }
            }
            else
            {
                SetPWM1_Out(SpeedKeyTime, SpeedKeyPulse);
                SpeedLedOn(ucSpeedGrad);

                if(strPeripheral.Input.Bits.SpeedModify)//20190807 修改最高时速 重新上电后有效
                {
                    if(ucEepromBuf[6] > 47)
                    {
                        ucEepromBuf[6] -= 10;
                    }
                    vWrite60Bytes();
                    vSendLedStatus(strPeripheral.Input.all, ucEepromBuf[6]);
                }
                else vSendSpeedGrad(ucSpeedGrad);
            }
            strPeripheral.Output.Bits.Alarm = TRUE;

        }
        else strPeripheral.Input.Bits.SpeedDownKey = 0;
    }
    if(strPeripheral.Input.Bits.SpeedChange)//修改最高时速
    {
        if(0 == strPeripheral.ucRecvAlarmNum)
        {

            if(ucEepromBuf[6] >= 87)
            {
                ucSpeedGrad = 5;
                ucEepromBuf[6] = 87;
            }
            else if(ucEepromBuf[6] <= 47)
            {
                ucSpeedGrad = 1;
                ucEepromBuf[6] = 47;
            }
            else if(ucEepromBuf[6] <= 57)
            {
                ucSpeedGrad = 2;
                ucEepromBuf[6] = 57;
            }
            else if(ucEepromBuf[6] <= 67)
            {
                ucSpeedGrad = 3;
                ucEepromBuf[6] = 67;
            }
            else if(ucEepromBuf[6] <= 77)
            {
                ucSpeedGrad = 4;
                ucEepromBuf[6] = 77;
            }
            SetPWM1_Out(SpeedKeyTime, SpeedKeyPulse);
            SpeedLedOn(ucSpeedGrad);

            strPeripheral.Input.Bits.SpeedChange = 0;
            strPeripheral.Input.Bits.SpeedModify = 1;
            strPeripheral.ucPowerLedNum = 0;
            strPeripheral.Output.Bits.Alarm = TRUE;
            vSendLedStatus(strPeripheral.Input.all, ucEepromBuf[6]);
        }
        else
        {
            strPeripheral.Input.Bits.SpeedChange = 0;
            strPeripheral.Input.Bits.SpeedModify = 0;
        }
    }
    if(strPeripheral.Input.Bits.SpeakerKey)
    {
        if (TRUE != strPeripheral.ucRecvOrderInCharge)
        {
            strPeripheral.Output.Bits.Alarm = TRUE;
            SetPWM1_Out(LabaKeyPressSpeekTime, LabaKeyPressSpeekPulse);
            if(FALSE == ucBuzzerPressedFlg)vSendBuzzerOrder(0x01);
            ucBuzzerPressedFlg = TRUE;
        }
    }
    else
    {
        if(TRUE == ucBuzzerPressedFlg) vSendBuzzerOrder(0x02);
        ucBuzzerPressedFlg = FALSE;
    }
    //		}
}

//unsigned char strPeripheral.ucPowerLedShowPhase = 0;
char ucBackSpeekFlag = FALSE;
char ucBeep_Alarm_Phase = 0;
char ucBeep_Alarm_Num = 0;
//char ucTestBeep = 0;
void PeripheralOutputHandle(void)
{
    /*******电源灯*******/
    if(Globle_Timer_1MS.Bits.PowerOn)
    {
        Globle_Timer_1MS.Bits.PowerOn = 0;
        if(strPeripheral.PowerLEDShowTime <= TIMER_1S)
        {
            strPeripheral.PowerLEDShowTime++;
        }
        else strPeripheral.PowerLEDShowTime = 0;

        //////////////////////////////////////
    }
    /*******检测是否写EEPROM*******/
    if(strPeripheral.ucRecvWrite60Bytes_Flag)
    {
        if(strPeripheral.PowerLEDShowTime > TIMER_200MS)
        {
            strPeripheral.PowerLEDShowTime = 0;
            switch(strPeripheral.ucPowerLedShowPhase)
            {
            case 0:
                SPEED_LED_0;
                POWER_SHOW_0;
                SetPWM1_Out(SpeedKeyTime, 0);ucDebugSpeaker = 4;
                strPeripheral.ucPowerLedShowPhase = 1;
                vWrite60Bytes();
                strPeripheral.ucPowerLedNum = 5;
                break;
            case 1:
                if(strPeripheral.ucPowerLedNum > 0)
                {
                    PowerLedNumOn(strPeripheral.ucPowerLedNum--);
                    SetPWM1_Out(SpeedKeyTime, SpeedKeyPulse);
                }
                else strPeripheral.ucPowerLedShowPhase = 2;
                break;
            case 2:
                SetPWM1_Out(SpeedKeyTime, 0);ucDebugSpeaker = 5;
                POWER_SHOW_0;  //关闭电源灯
                SpeedLedOn(ucSpeedGrad);	//显示速度
                strPeripheral.ucPowerLedShowPhase = 0;
                strPeripheral.ucRecvWrite60Bytes_Flag = 0;
                break;
            default:
                SetPWM1_Out(SpeedKeyTime, 0);ucDebugSpeaker = 6;
                strPeripheral.ucPowerLedShowPhase = 0;
                strPeripheral.ucRecvWrite60Bytes_Flag = 0;
                break;
            }
        }
    }
    else if(0x03 == strPeripheral.ucRecvData_Flag)    //串口通信赋值0X03
    {
        if(TRUE == strPeripheral.usRecvPowerON_Flag)   //  增加一个开机命令
        {
            if(strPeripheral.PowerLEDShowTime >= TIMER_100MS)
            {
                strPeripheral.PowerLEDShowTime = 0;
                switch(strPeripheral.ucPowerLedShowPhase)
                {
                case 0:
                    POWER_SHOW_0;
                    SPEED_LED_0;

                    SetPWM1_Out(SpeedKeyTime, 0);  ucDebugSpeaker = 7;//输出控制蜂鸣器叫
                    strPeripheral.ucPowerLedShowPhase = 1;
                    strPeripheral.ucPowerLedNum = 0;
                    break;
                case 1:
                    if(0x40 == (ucBatType & 0x40)) //如果是锂电模式

                    {
                        if(strPeripheral.ucPowerLedNum < 2)
                        {
                            if(strPeripheral.ucPowerLedNum  == 0)
                            {
                                POWER_SHOW_5;
                                SPEED_LED_5;
                                SetPWM1_Out(SpeedKeyTime, SpeedKeyPulse);  //输出控制蜂鸣器叫
                            }
                            else
                            {
                                POWER_SHOW_0;
                                SPEED_LED_0;
                                SetPWM1_Out(SpeedKeyTime, 0);ucDebugSpeaker = 8;
                            }
                            strPeripheral.ucPowerLedNum++;
                        }
                        else
                        {
                            strPeripheral.ucPowerLedNum = 0;
                            SetPWM1_Out(SpeedKeyTime, SpeedKeyPulse);  //输出控制蜂鸣器叫
                            strPeripheral.ucPowerLedShowPhase = 2;
                        }
                    }
                    else//铅酸电池鸣叫一次
                    {
                        POWER_SHOW_5;
                        SPEED_LED_5;
                        SetPWM1_Out(SpeedKeyTime, SpeedKeyPulse);  //输出控制蜂鸣器叫
                        strPeripheral.ucPowerLedShowPhase = 2;
                    }

                    break;
                case 2:
                    //电量灯,速度灯全灭
                    POWER_SHOW_0;
                    SetPWM1_Out(SpeedKeyTime, 0);ucDebugSpeaker = 9;
                    strPeripheral.ucPowerLedShowPhase = 3;
                    break;
                case 3:
                    //电量灯
                    POWER_SHOW_5;
                    //PowerLedOn(strPeripheral.uiRecvPowerValue); //显示电量
                    //点亮速度灯
                    SpeedLedOn(ucSpeedGrad);	 //
                    strPeripheral.ucPowerLedShowPhase = 0;
                    strPeripheral.usRecvPowerON_Flag = 0x03;      //只执行一次（开机时执行）
                    strPeripheral.ucPowerLedShowPhase = 0;
                    ucBeep_Alarm_Phase = 0;
                    ucBeep_Alarm_Num = 0;
                    ucLessThanLowPower = 0;
                    strPeripheral.ucPowerLedNum  = 0;
                    break;
                }
            }
        }
        /*******检测是否充电中*******/
        else if (TRUE == strPeripheral.ucRecvOrderInCharge)
        {
            if(strPeripheral.PowerLEDShowTime > TIMER_300MS)
            {
                strPeripheral.ucPowerLedNum %= 5;
                strPeripheral.ucPowerLedNum++;
                SPEED_LED_5;
                PowerLedNumOn(strPeripheral.ucPowerLedNum);
                strPeripheral.PowerLEDShowTime = 0;
            }
        }
        else if(strPeripheral.ucRecvAlarmNum)//有故障
        {
            switch(ucBeep_Alarm_Phase)
            {
            case 0:
                SetPWM1_Out(SpeedKeyTime, 0);ucDebugSpeaker = 10;
                SPEED_LED_0;
                POWER_SHOW_0;
                ucBeep_Alarm_Phase = 1;
                break;
            case 1:
                if(strPeripheral.PowerLEDShowTime >= TIMER_400MS)
                {
                    SetPWM1_Out(SpeedKeyTime, SpeedKeyPulse); //蜂鸣器响一声
                    POWER_SHOW_5;                      //电量灯全亮
                    strPeripheral.PowerLEDShowTime = 0;
                    ucBeep_Alarm_Phase = 2;
                }
                break;
            case 2:
                if(strPeripheral.ucRecvAlarmNum < 0x80)
                {
                    if(strPeripheral.PowerLEDShowTime >= TIMER_400MS)
                    {
                        SetPWM1_Out(0, 0); //蜂鸣器响一声
                        POWER_SHOW_0;                      //电量灯全亮
                        strPeripheral.PowerLEDShowTime = 0;
                        ucBeep_Alarm_Num++;
                        if(ucBeep_Alarm_Num < strPeripheral.ucRecvAlarmNum)
                            ucBeep_Alarm_Phase = 1;
                        else
                            ucBeep_Alarm_Phase = 3;
                    }
                }
                else
                {
                    if(strPeripheral.PowerLEDShowTime >= TIMER_200MS)
                    {
                        SetPWM1_Out(0, 0); //蜂鸣器响一声
                        POWER_SHOW_0;                      //电量灯全亮
                        strPeripheral.PowerLEDShowTime = 0;
                        ucBeep_Alarm_Num++;
                        if(ucBeep_Alarm_Num < (strPeripheral.ucRecvAlarmNum - 0x80))
                            ucBeep_Alarm_Phase = 1;
                        else
                            ucBeep_Alarm_Phase = 3;
                    }
                }
                break;
            case 3:
                if(strPeripheral.PowerLEDShowTime >= TIMER_1S)
                {
                    ucBeep_Alarm_Phase = 0;
                    strPeripheral.PowerLEDShowTime = 0;
                    ucBeep_Alarm_Num = 0;
                }
                break;
            }
        }
        else if(strPeripheral.ucRecvTestFlag)
        {
            switch(ucBeep_Alarm_Phase)
            {
            case 0:
                if(strPeripheral.PowerLEDShowTime >= TIMER_400MS)
                {
                    SetPWM1_Out(SpeedKeyTime, 0);ucDebugSpeaker = 11;
                    ucBeep_Alarm_Phase = 1;
                    strPeripheral.PowerLEDShowTime = 0;
                }
                break;
            case 1:
                if(strPeripheral.PowerLEDShowTime >= TIMER_20MS)
                {
                    SetPWM1_Out(SpeedKeyTime, SpeedKeyPulse); //蜂鸣器响一声
                    strPeripheral.PowerLEDShowTime = 0;
                    ucBeep_Alarm_Phase = 2;
                }
                break;
            case 2:
                if(strPeripheral.PowerLEDShowTime >= TIMER_20MS)
                {
                    SetPWM1_Out(0, 0); //蜂鸣器响一声
                    strPeripheral.PowerLEDShowTime = 0;
                    ucBeep_Alarm_Num++;
                    if(ucBeep_Alarm_Num < 7)
                        ucBeep_Alarm_Phase = 1;
                    else
                        ucBeep_Alarm_Phase = 3;
                }

                break;
            case 3:
                if(strPeripheral.PowerLEDShowTime >= TIMER_1S)
                {
                    ucBeep_Alarm_Phase = 0;
                    strPeripheral.PowerLEDShowTime = 0;
                    ucBeep_Alarm_Num = 0;
                }
                break;
            default:
                ucBeep_Alarm_Phase = 0;
                strPeripheral.PowerLEDShowTime = 0;
                ucBeep_Alarm_Num = 0;
                break;
            }
            vTestShowLed(strPeripheral.usRecvLedTmp);
        }
#ifdef EnableCancelBackCarAlarm
        else if((strPeripheral.Input.Bits.SpeedModify) || (strPeripheral.Input.Bits.PowerChange) || (strPeripheral.Input.Bits.BackCarAlarmModify))
        {
            if(strPeripheral.PowerLEDShowTime > TIMER_200MS)
            {
                strPeripheral.PowerLEDShowTime = 0;
                if(strPeripheral.Input.Bits.BackCarAlarmModify)
                {
                    if(ucEepromBuf[0])
                    {
                        if(strPeripheral.ucPowerLedNum < 6)
                        {
                            strPeripheral.ucPowerLedNum++;
                        }
                        else strPeripheral.ucPowerLedNum = 0;
                    }
                    else
                    {
                        if(strPeripheral.ucPowerLedNum > 0)
                        {
                            strPeripheral.ucPowerLedNum--;
                        }
                        else strPeripheral.ucPowerLedNum = 5;
                    }
                    ucSpeedGrad = strPeripheral.ucPowerLedNum;
                    SpeedLedOn(ucSpeedGrad);	//显示速度
                    PowerLedNumOn(strPeripheral.ucPowerLedNum);
                }
                else
                {
                    if((ucEepromBuf[2] & 0x40) == 0x40)//锂电池
                    {
                        if(strPeripheral.ucPowerLedNum < 6)
                        {
                            strPeripheral.ucPowerLedNum++;
                        }
                        else strPeripheral.ucPowerLedNum = 0;
                    }
                    else//铅酸电池
                    {
                        if(strPeripheral.ucPowerLedNum > 0)
                        {
                            strPeripheral.ucPowerLedNum--;
                        }
                        else strPeripheral.ucPowerLedNum = 5;
                    }
                    PowerLedNumOn(strPeripheral.ucPowerLedNum);
                }
            }
        }
#else
        else if((strPeripheral.Input.Bits.SpeedModify) || (strPeripheral.Input.Bits.PowerChange))
        {
            if(strPeripheral.PowerLEDShowTime > TIMER_200MS)
            {
                strPeripheral.PowerLEDShowTime = 0;
                if((ucEepromBuf[2] & 0x40) == 0x40)//锂电池
                {
                    if(strPeripheral.ucPowerLedNum < 6)
                    {
                        strPeripheral.ucPowerLedNum++;
                    }
                    else strPeripheral.ucPowerLedNum = 0;
                }
                else//铅酸电池
                {
                    if(strPeripheral.ucPowerLedNum > 0)
                    {
                        strPeripheral.ucPowerLedNum--;
                    }
                    else strPeripheral.ucPowerLedNum = 5;

                }
                PowerLedNumOn(strPeripheral.ucPowerLedNum);
            }
        }
#endif
        else //不在充电中 并且无故障 正常显示电量
        {
            ucBeep_Alarm_Phase = 0;
            ucBeep_Alarm_Num = 0;
            if(TRUE == strPeripheral.usRecvPowerOFF_Flag)//关机
            {
                if(GLOBAL_SPEED_GRAD != ucSpeedGrad)
                {
                    EEPROM_WriteByte(SPEED_ADDRESS, ucSpeedGrad);
                    GLOBAL_SPEED_GRAD = ucSpeedGrad;
                }
                SPEED_LED_0;   //关闭速度灯
                POWER_SHOW_0;  //关闭电源灯
                strPeripheral.ucPowerLedShowPhase = 0;
                strPeripheral.ucRecvData_Flag = FALSE;
                strPeripheral.PowerLEDShowTime = 0;
                strPeripheral.ucRecvPowerValue_Flag = 0;
            }
            else if(strPeripheral.ucRecvPowerValue_Flag)//(strPeripheral.PowerLEDShowTime > TIMER_300MS)
            {
                PowerLedOn(strPeripheral.uiRecvPowerValue); //显示电量
                SpeedLedOn(ucSpeedGrad);	//显示速度
            }
        }
        /*******蜂鸣器*******/

        if(Globle_Timer_1MS.Bits.Beep_Alarm)
        {
            Globle_Timer_1MS.Bits.Beep_Alarm = 0;
            if ((TRUE == strPeripheral.ucRecvBikeInBack) && (TRUE != strPeripheral.ucRecvOrderInCharge) && (FALSE == ucBuzzerPressedFlg) && (0 == strPeripheral.ucRecvAlarmNum) )
            {
                strPeripheral.BackSpeekTime++;
            }
            if(strPeripheral.Output.Bits.Alarm)
            {
                strPeripheral.AlarmTime++;
            }
        }
        if(strPeripheral.BackSpeekTime > TIMER_600MS)
        {
            strPeripheral.BackSpeekTime = 0;
            if(ucBackSpeekFlag)
            {
                SetPWM1_Out(InBackTime, InBackPulse);
                ucBackSpeekFlag = FALSE;
            }
            else
            {
                SetPWM1_Out(SpeedKeyTime, 0);ucDebugSpeaker = 12;
                ucBackSpeekFlag = TRUE;
            }
        }
        if(strPeripheral.AlarmTime > TIMER_100MS)
        {
            strPeripheral.AlarmTime = 0;
            if(FALSE == strPeripheral.Input.Bits.SpeakerKey)SetPWM1_Out(SpeedKeyTime, 0);ucDebugSpeaker = 13;
            strPeripheral.Output.Bits.Alarm = 0;
        }
    }//串口通信后执行
}
void vSpeakershutOut(void)
{
    if(usReadSpeakerPwm() && Globle_Timer_1MS.Bits.CheckIDele)
    {
        Globle_Timer_1MS.Bits.CheckIDele = 0;
        if((0 == strPeripheral.ucRecvBikeInBack) && (0 == ucLess18Power) &&
                (0 == strPeripheral.ucRecvAlarmNum) &&  (FALSE == ucBuzzerTstFlg) && (FALSE == ucBuzzerPressedFlg) &&
                (0 == ucKeyPressFlag) &&  (0 == strPeripheral.ucRecvWrite60Bytes_Flag) && (0x03 == strPeripheral.usRecvPowerON_Flag) &&
                (0 == strPeripheral.SpeakerAndSpeedUpTime) && (0 == strPeripheral.SpeedDownAndUpTime) && (0 == strPeripheral.SpeedDownKeyTime) &&
                (0 == strPeripheral.SpeedUpKeyTime) && (FALSE == strPeripheral.ucRecvTestFlag))
        {
            SetPWM1_Out(SpeedKeyTime, 0);ucDebugSpeaker = 14;
            for(ucKeyPressFlag = 0; ucKeyPressFlag < 60; ucKeyPressFlag++)
            {
                Globle_Timer_1MS.Bits.CheckIDele  = 0;
                while(0 == Globle_Timer_1MS.Bits.CheckIDele);
            }
            vSendBuzzerOrder(0x02);
            Globle_Timer_1MS.Bits.CheckIDele = ucKeyPressFlag = 0;
        }
    }
}
#ifdef BatteryAutoShiBie
void PeripheralHandle(void)
{
    PeripheralOutputHandle();//电量指示灯、速度指示灯、蜂鸣器、EEPROM

    if(0x03 == strPeripheral.ucRecvData_Flag)    //串口通信赋值0X03
    {
        if(strPeripheral.uiRecvPowerValue)//电量不为零时才检测按键
        {
            PeripheralInputHandle();
        }

        ////////////////////
        //				if(4 == ucSpeedGrad)
        //				{
        //					ucTestBeep = 100;
        //				}
        //        if(ucTestBeep)
        //        {
        //            SetPWM1_Out(SpeedKeyTime, ucTestBeep);
        //            ucTestBeep = 0;
        //        }
        ////////////////////////
        vSpeakershutOut();
    }
}
#endif

#ifdef OldZuHeAnjian
void PeripheralHandle(void)
{
    PeripheralOutputHandle();//电量指示灯、速度指示灯、蜂鸣器、EEPROM

    if(0x03 == strPeripheral.ucRecvData_Flag)    //串口通信赋值0X03
    {
        if(strPeripheral.uiRecvPowerValue)//电量不为零时才检测按键
        {
            PeripheralInputHandle();
        }
        vSpeakershutOut();
    }
}
#endif

#ifdef YueYueZuHeAnjian
void PeripheralHandle(void)
{
    PeripheralOutputHandle();//电量指示灯、速度指示灯、蜂鸣器、EEPROM

    if(0x03 == strPeripheral.ucRecvData_Flag)    //串口通信赋值0X03
    {
        if(strPeripheral.uiRecvPowerValue)//电量不为零时才检测按键
        {
            PeripheralInputHandle();
        }
        vSpeakershutOut();
    }
}
#endif

void myFuncInit(void){


}
