#include "SystemInit.h"
#include "function.h"
#include "intrins.h"
#include "stdio.h"
#include "Uart.h"
#include "User_Def.h"

__O Flag SFR_interrupt_buff_H;
__O Flag SFR_interrupt_buff_L;   //WH

#define InDeBugMode
unsigned int uiConvertTimes = 0;
unsigned int uiMaxAd = 0;
unsigned int uiMinAd = 0xFFFF;
unsigned long l_gAdTemp = 0;              //unsigned long 32λ
unsigned int  uiAdVoltage = 0;

unsigned char ucSpeedGrad;  //�ٶ�
unsigned char GLOBAL_SPEED_GRAD = 1;  //ȫ���ٶ�


unsigned char ucLessThanLowPower = FALSE; //��Դ��ѹС��23.740�ı�־
unsigned char ucLess18Power = FALSE;      //��Դ��ѹС��18�ı�־
unsigned char ucBuzzerPressedFlg = FALSE;    //���������±�־


unsigned char ucKeyPressFlag = TRUE;
unsigned char ucBuzzerTstFlg = FALSE; //���������Ա�־
//unsigned char //ucDebugSpeaker = 0;
//unsigned char PowerOFF = FALSE; //�ػ���־
Peripheral_str strPeripheral;
unsigned short usReadSpeakerPwm(void)
{
    unsigned short usTemp = 0;
    SFRSL0 = 1;				                   //�л�SFR1
    usTemp = PWM13DH << 8;
    usTemp |= PWM13DL;
    SFRSL0 = 0;				                   //�л�SFR0
    return (usTemp);
}
void delay(unsigned long i)
{
    while (i--);
}

/*******���AD��ֵ*******/
unsigned int ADC_GetConversionValue(void)
{
    unsigned int uiTemp = 0;
    uiTemp = (unsigned int)(ADRH << 8);
    uiTemp |= ADRL;
    CLR_ADEOC(); //����ת��
    return uiTemp;
}

/*************����AD****************/
void vDealAd(void)
{
#define DelaySetAdcRegTime 20
    unsigned int uiAdValue = 0;

    if(0 == (ADCCR0 & 0x02))return;

    uiAdValue = ADC_GetConversionValue();
    uiConvertTimes++;
    if(uiAdValue > uiMaxAd)uiMaxAd = uiAdValue;
    if(uiAdValue < uiMinAd)uiMinAd = uiAdValue;
    l_gAdTemp += uiAdValue;
    if(ConverTimes == uiConvertTimes)
    {
        uiAdVoltage = (l_gAdTemp - uiMaxAd - uiMinAd) >> 2; //��6�β���ֵ  ��ȥ����ȥ��С����4
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
const unsigned char code uiAutoAddFactData[60] =
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
    for(ucTemp = 0; ucTemp < EEPROM_START_ADDRESS_FACT_PARE_Legth; ucTemp++)
    {
        ucTempData = ucEepromBuf[ucTemp] = EEPROM_Byte_Read(EEPROM_START_ADDRESS_FACT_PARE + ucTemp);
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
    p->uiRecvPowerValue = 0;  //����
    p->ucRecvBikeInBack = FALSE;
    p->ucRecvOrderInCharge  = NO_USE; //�Ƿ��ڳ��

    p->ucRecvTestFlag  = FALSE;

}
/*****��һ�ο�������������*********/
unsigned char ucBatType = 0;
void vFirstStart(void)
{
    vSysSourceInit();//ϵͳ��ʼ��
    delay(10);
    clr_EA;
    GLOBAL_SPEED_GRAD = EEPROM_Byte_Read(SPEED_ADDRESS);;//��ȡFLASE �е���������
    ucSpeedGrad = GLOBAL_SPEED_GRAD;
    EEPROM_Read(EEPROM_START_ADDRESS_FACT_PARE, ucEepromBuf, EEPROM_START_ADDRESS_FACT_PARE_Legth);
    set_EA;
    ucBatType = EEPROM_Byte_Read(WriteBatteryTypeFlag_Addr);;//��ȡFLASE �е���������
    if(0 == ucSpeedGrad)
    {
        ucSpeedGrad = 1;
    }
    else if((ucSpeedGrad >= 5) && (ucSpeedGrad < 100))
    {
        ucSpeedGrad = 5;
    }
    else if(ucSpeedGrad >= 100)//���������ʱ����0xFF
    {
        ucSpeedGrad = 1;
    }

		if((PowerOnSpeedGradeAt > 0) && (PowerOnSpeedGradeAt < 6))ucSpeedGrad = PowerOnSpeedGradeAt;

    ucLessThanLowPower = FALSE;
    ucLess18Power = FALSE;
    strPeripheral.ucRecvOrderInCharge = NO_USE;
    strPeripheral.ucRecvBikeInBack = FALSE;
    strPeripheral.uiRecvPowerValue = 0;
    Peripheral_Init(&strPeripheral);//������ʼ��
    vCheckFactData();//�������֡���ݣ����û������������дһ��

    //////////////////////////////////���½�Ϊ����ģʽ����Ч��
#ifdef InDeBugMode
//    strPeripheral.ucRecvData_Flag = 0x03;
//    strPeripheral.uiRecvPowerValue = 40000;//������Ϊ��ʱ�ż�ⰴ��
//    //
//    SpeedLedOn(ucSpeedGrad);
//    PowerLedOn(strPeripheral.uiRecvPowerValue); //��ʾ����
		
#endif

}




/*****��ʾ����ָʾ��********/
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

/*******��ʾ�ٶ�ָʾ��*************/
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

void PowerLedOn(unsigned short power)    //unsigned shortΪ16λ
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
    if(TRUE == ucLessThanLowPower)//Ƿѹ
    {
        if(strPeripheral.PowerLEDShowTime > TIMER_200MS)
        {
            strPeripheral.PowerLEDShowTime = 0;
            XorRedPowerLed;
            if(ucLess18Power)//��ѹ����18V
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
                        SetPWM1_Out(SpeedKeyTime, 0);//ucDebugSpeaker = 1;
                        strPeripheral.ucPowerLedShowPhase = 0;
                        break;
                    default:
                        SetPWM1_Out(SpeedKeyTime, 0);//ucDebugSpeaker = 2;
                        strPeripheral.ucPowerLedShowPhase = 0;
                        break;
                    }
                }
            }
            else
            {
                SetPWM1_Out(SpeedKeyTime, 0);//ucDebugSpeaker = 3;
                strPeripheral.ucPowerLedShowPhase = 0;
            }
        }

    }
    else strPeripheral.PowerLEDShowTime = 0;
}




/*******����Դ���ػ�****/
void vPowerPressed(void)
{

    if(TRUE == strPeripheral.usRecvPowerOFF_Flag)//���ٶ�ֵд��EEPROM
    {
        if(GLOBAL_SPEED_GRAD != ucSpeedGrad)
        {
            EEPROM_WriteByte(SPEED_ADDRESS, ucSpeedGrad);
            ucSpeedGrad = GLOBAL_SPEED_GRAD;
        }
        //		  SPEED_LED_0;   //�ر��ٶȵ�
        //		  POWER_SHOW_0;  //�رյ�Դ��
        strPeripheral.usRecvPowerOFF_Flag = TRUE;
        strPeripheral.ucRecvData_Flag = FALSE;
    }
    //����е�Դ����������ʱ���ȥ �洢����ֵ 20181224
    if(POWER_ON_KEY_PRESSED)
    {
        clr_EA;
        if(GLOBAL_SPEED_GRAD != ucSpeedGrad)
        {
            EEPROM_WriteByte(SPEED_ADDRESS, ucSpeedGrad);
            ucSpeedGrad = GLOBAL_SPEED_GRAD;
        }
        set_EA;
    }
}


/**************EEPROMд������***********/
void EEPROM_WriteByte(unsigned int u16_addr, unsigned char u8_data)
{
    EEPROM_Byte_Erase(u16_addr);
    EEPROM_Byte_Write(u16_addr, u8_data);
}

void vArom_WriteBytes_Fact(unsigned char *pData)//ֱ��д��һ������ �����ٽ���������
{

    unsigned char i = 0;
    clr_EA;
    for(i = 0; i < EEPROM_START_ADDRESS_FACT_PARE_Legth; i++)
    {
        EEPROM_WriteByte(EEPROM_START_ADDRESS_FACT_PARE + i, pData[i]);
    }
    set_EA;
    set_ES;//�ٴο��� �����ж�
}

//void vArom_WriteBytes_Fact(unsigned char *pData)//ֱ��д��һ������ �����ٽ���������
//{
//	  uint8_t cnt;
//	  uint16_t addr_60Byte = EEPROM_START_ADDRESS_FACT_PARE;
//	  clr_EA;
//
//    SET_IAPEN();
//    SET_IAPEN(); //��FALSH IAP���ܣ�����д������Ч
//    SET_EPAGEERS();
//    SET_EPAGEERS(); //ҳ��ģʽ
//    IAPKEY = 0x5a;
//    IAPKEY = 0xa5;
//    SET_IAPSTR(); //IAPSTRλ��һ������IAP����
//    for (cnt = 0; cnt < 61; cnt++)
//    {
//			  IAPADH = addr_60Byte >> 8;
//        IAPADL = addr_60Byte;
//        IAPDB = 0;
//			  addr_60Byte++;
//    }
//    IAPKEY = 0x5a;
//    IAPKEY = 0xa5;
//    SET_IAPTRIG0(); //IAPTRIG0��һ
//    IAPKEY = 0x5a;
//    IAPKEY = 0xa5;
//    SET_IAPTRIG1(); //IAPTRIG1��һ
//    IAPKEY = 0x5a;
//    IAPKEY = 0xa5;
//    SET_IAPTRIG2(); //IAPTRIG1��һ
//    while (IAPCR & 0x80);

//	  addr_60Byte = EEPROM_START_ADDRESS_FACT_PARE;

//    SET_IAPEN();
//    SET_IAPEN(); //��FALSH IAP���ܣ�����д������Ч
//    SET_EPAGEWR();
//    SET_EPAGEWR(); //ҳдģʽ
//    IAPKEY = 0x5a;
//    IAPKEY = 0xa5;
//    SET_IAPSTR(); //IAPSTRλ��һ,����IAP����
//    for (cnt = 0; cnt < 61; cnt++)
//    {
//			  IAPADH = addr_60Byte >> 8;
//        IAPADL = addr_60Byte;
//        IAPDB = pData[cnt];
//			  addr_60Byte++;
//    }
//    IAPKEY = 0x5a;
//    IAPKEY = 0xa5;
//    SET_IAPTRIG0(); //IAPTRIG0��һ
//    IAPKEY = 0x5a;
//    IAPKEY = 0xa5;
//    SET_IAPTRIG1(); //IAPTRIG1��һ
//    IAPKEY = 0x5a;
//    IAPKEY = 0xa5;
//    SET_IAPTRIG2(); //IAPTRIG1��һ
//    while (IAPCR & 0x80); //IAPSTR

//	  set_EA;
//}
/**********����д��60�ֽ�����**************/
void vWrite60Bytes(void)
{
    clr_EA;
    vArom_WriteBytes_Fact(&ucEepromBuf[0]);
    set_EA;
}

void vTestShowLed(unsigned int value)
{
    //    if (value & 0x0001) POWER_SHOW1_ON;
    //    else POWER_SHOW1_OFF;

    //    if (value & 0x0002) POWER_SHOW2_ON;
    //    else POWER_SHOW2_OFF;

    //    if (value & 0x0004) POWER_SHOW3_ON;
    //    else POWER_SHOW3_OFF;

    //    if (value & 0x0008) POWER_SHOW4_ON;
    //
    //    else POWER_SHOW4_OFF;

    //    if (value & 0x0010) POWER_SHOW5_ON;
    //    else POWER_SHOW5_OFF;

    if (value & 0x0020) SPEED_STEP1_ON;
    else SPEED_STEP1_OFF;

    if (value & 0x0040) SPEED_STEP2_ON;
    else SPEED_STEP2_OFF;

    if (value & 0x0080) SPEED_STEP3_ON;//���㲻������ʱ��  �������ְ����汾�Ĳ�ͬ
    else SPEED_STEP3_OFF;

    if (value & 0x0100) SPEED_STEP4_ON;
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
    GPIO_Init();
}

void SetPWM1_Out(unsigned int x, unsigned int y)
{
    SFRSL0 = 1;				                   //�л�SFR1

    PWM1PH = (unsigned char)(x >> 8);
    PWM1PL = (unsigned char)(x);
    PWM13DH =  (unsigned char)(y >> 8);
    PWM13DL =  (unsigned char)(y);

    SFRSL0 = 0;						              //�л�SFR0
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
        else if((TRUE == strPeripheral.Input.Bits.SpeedDownKeyPressFlag) && (Voltage < 2435) && (Voltage > 2265))//else if((TRUE == strPeripheral.Input.Bits.SpeedDownKeyPressFlag) && (Voltage < 2437) && (Voltage > 2237)) //+ -  ͬʱ����2450 2200  -->ADC as 2337    MC51F003  2355
        {
            strPeripheral.SpeedDownAndUpTime++;  //���밴�����Ȱ���ȥ
            if(strPeripheral.SpeedDownAndUpTime > TIMER_4000MS)
            {
                strPeripheral.SpeedDownAndUpTime = TIMER_4000MS;
                if(strPeripheral.Input.Bits.PowerChange == FALSE)//20190923�޸��л�������Ͳ������޸����ʱ��ʱ
                    strPeripheral.Input.Bits.SpeedChange = TRUE;
                ucKeyPressFlag = TRUE;
                POWER_SHOW_5;
                SPEED_LED_5;
                SetPWM1_Out(SpeedKeyTime, SpeedKeyPulse);  //������Ʒ�������
            }
        }
        else if((Voltage < 2164) && (Voltage > 1944))//else if((Voltage < 2200) && (Voltage > 1900)) //2054 ����2200 1900    -->ADC as 2049    MC51F003  2074
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
        else if((TRUE == strPeripheral.Input.Bits.SpeakerKey) && (Voltage < 1852) && (Voltage > 1680)) //else if((TRUE == strPeripheral.Input.Bits.SpeakerKey) && (Voltage < 1900) && (Voltage > 1650)) //1706���� +  ͬʱ����1900  1650  -->ADC as1702    MC51F003  1722
        {
            //���밴�����Ȱ���ȥ
            strPeripheral.SpeakerAndSpeedUpTime++;
            if(strPeripheral.SpeakerAndSpeedUpTime > 1000)
            {
                strPeripheral.SpeakerAndSpeedUpTime = 1000;
                strPeripheral.Input.Bits.SpeakerKey = FALSE;
                ucKeyPressFlag = TRUE;

                if((TRUE != strPeripheral.ucRecvOrderInCharge) && (0 == strPeripheral.ucRecvAlarmNum) && \
                        (strPeripheral.Input.Bits.SpeedModify == FALSE))//20190923�޸��л����ʱ��ʱ�������޸ĵ������
                {
                    if(strPeripheral.Input.Bits.PowerChange == FALSE)//20190807 �޸ĵ������ �����ϵ����Ч
                    {

                        if((ucEepromBuf[2] & 0x40) == 0x40)//﮵��
                        {
                            ucEepromBuf[2] &= 0xBF;//Ǧ��
                        }
                        else
                        {
                            ucEepromBuf[2] |= 0x40;	//﮵��
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
        else if((TRUE == strPeripheral.Input.Bits.SpeakerKey) && (Voltage < 1650) && (Voltage > 1400)) //else if((TRUE == strPeripheral.Input.Bits.SpeakerKey) && (Voltage < 1900) && (Voltage > 1650)) //1706���� +  ͬʱ����1900  1650  -->ADC as1702    MC51F003  1722
        {
            //���밴�����Ȱ���ȥ
            strPeripheral.SpeedDownKeyTime++;
            if(strPeripheral.SpeedDownKeyTime > 1000)
            {
                strPeripheral.SpeedDownKeyTime = 1000;
                strPeripheral.Input.Bits.SpeakerKey = FALSE;
                ucKeyPressFlag = TRUE;

                if((TRUE != strPeripheral.ucRecvOrderInCharge) && (0 == strPeripheral.ucRecvAlarmNum) && \
                        (strPeripheral.Input.Bits.BackCarAlarmModify == FALSE))//20190923�޸��л����ʱ��ʱ�������޸ĵ������
                {
                    if(strPeripheral.Input.Bits.PowerChange == FALSE)//20190807 �޸ĵ������ �����ϵ����Ч
                    {
												if(ucEepromBuf[0])ucEepromBuf[0] = 0;//������������������ˣ��Ͳ�Ҫ������
                        else ucEepromBuf[0] = 1;	//������������������ˣ��ٴο���
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
        //        else if((FALSE == ucKeyPressFlag) && (Voltage < 1650) && (Voltage > 1400)) //1584 ���� -  ͬʱ����
        //        {//ȡ��ԭ�������ٶȼ�һ������ٶȹ���
        //            strPeripheral.SpeedDownKeyTime++;
        //            if(strPeripheral.SpeedDownKeyTime > TIMER_100MS)
        //            {
        //                strPeripheral.SpeedDownKeyTime = TIMER_100MS;
        //                strPeripheral.Input.Bits.SpeedChange = FALSE;
        //                strPeripheral.Input.Bits.SpeakerKey = FALSE;
        //                ucKeyPressFlag = TRUE;
        //            }
        //        }
        else if(Voltage <  1400) //ͬʱ����
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
                if(strPeripheral.Input.Bits.SpeedModify)//20190807 �޸����ʱ�� �����ϵ����Ч
                {
                    ucEepromBuf[6] = 87;
                    vWrite60Bytes();
                }
            }
            else
            {
                SetPWM1_Out(SpeedKeyTime, SpeedKeyPulse);
                SpeedLedOn(ucSpeedGrad);
                if(strPeripheral.Input.Bits.SpeedModify)//20190807 �޸����ʱ�� �����ϵ����Ч
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
                if(strPeripheral.Input.Bits.SpeedModify)//20190807 �޸����ʱ�� �����ϵ����Ч
                {
                    ucEepromBuf[6] = 47;
                    vWrite60Bytes();
                }
            }
            else
            {
                SetPWM1_Out(SpeedKeyTime, SpeedKeyPulse);
                SpeedLedOn(ucSpeedGrad);

                if(strPeripheral.Input.Bits.SpeedModify)//20190807 �޸����ʱ�� �����ϵ����Ч
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
    if(strPeripheral.Input.Bits.SpeedChange)//�޸����ʱ��
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
    /*******��Դ��*******/
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
    /*******����Ƿ�дEEPROM*******/
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
                SetPWM1_Out(SpeedKeyTime, 0);//ucDebugSpeaker = 4;
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
                SetPWM1_Out(SpeedKeyTime, 0);//ucDebugSpeaker = 5;
                POWER_SHOW_0;  //�رյ�Դ��
                SpeedLedOn(ucSpeedGrad);	//��ʾ�ٶ�
                strPeripheral.ucPowerLedShowPhase = 0;
                strPeripheral.ucRecvWrite60Bytes_Flag = 0;
                break;
            default:
                SetPWM1_Out(SpeedKeyTime, 0);//ucDebugSpeaker = 6;
                strPeripheral.ucPowerLedShowPhase = 0;
                strPeripheral.ucRecvWrite60Bytes_Flag = 0;
                break;
            }
        }
    }
    else if(0x03 == strPeripheral.ucRecvData_Flag)    //����ͨ�Ÿ�ֵ0X03
    {
        if(TRUE == strPeripheral.usRecvPowerON_Flag)   //  ����һ����������
        {
            if(strPeripheral.PowerLEDShowTime >= TIMER_100MS)
            {
                strPeripheral.PowerLEDShowTime = 0;
                switch(strPeripheral.ucPowerLedShowPhase)
                {
                case 0:
                    POWER_SHOW_0;
                    SPEED_LED_0;

                    SetPWM1_Out(SpeedKeyTime, 0);  //ucDebugSpeaker = 7;//������Ʒ�������
                    strPeripheral.ucPowerLedShowPhase = 1;
                    strPeripheral.ucPowerLedNum = 0;
                    break;
                case 1:
                    if(0x40 == (ucBatType & 0x40)) //�����﮵�ģʽ
                    {
                        if(strPeripheral.ucPowerLedNum < 2)
                        {
                            if(strPeripheral.ucPowerLedNum  == 0)
                            {
                                POWER_SHOW_5;
                                SPEED_LED_5;
                                SetPWM1_Out(SpeedKeyTime, SpeedKeyPulse);  //������Ʒ�������
                            }
                            else
                            {
                                POWER_SHOW_0;
                                SPEED_LED_0;
                                SetPWM1_Out(SpeedKeyTime, 0);//ucDebugSpeaker = 8;
                            }
                            strPeripheral.ucPowerLedNum++;
                        }
                        else
                        {
                            strPeripheral.ucPowerLedNum = 0;
                            SetPWM1_Out(SpeedKeyTime, SpeedKeyPulse);  //������Ʒ�������
                            strPeripheral.ucPowerLedShowPhase = 2;
                        }
                    }
                    else//Ǧ��������һ��
                    {
                        POWER_SHOW_5;
                        SPEED_LED_5;
                        SetPWM1_Out(SpeedKeyTime, SpeedKeyPulse);  //������Ʒ�������
                        strPeripheral.ucPowerLedShowPhase = 2;
                    }
                    //														if(strPeripheral.ucPowerLedNum < 6)
                    //														{
                    //																PowerLedNumOn(strPeripheral.ucPowerLedNum);
                    //																SpeedLedOn(ucSpeedGrad);
                    //																SetPWM1_Out(SpeedKeyTime, SpeedKeyPulse);  //������Ʒ�������
                    //															    strPeripheral.ucPowerLedNum ++;
                    //														}
                    //														else	strPeripheral.ucPowerLedShowPhase = 2;
                    break;
                case 2:
                    //������,�ٶȵ�ȫ��
                    POWER_SHOW_0;
                    SetPWM1_Out(SpeedKeyTime, 0);//ucDebugSpeaker = 9;
                    strPeripheral.ucPowerLedShowPhase = 3;
                    break;
                case 3:
                    //������
                    POWER_SHOW_5;
                    //PowerLedOn(strPeripheral.uiRecvPowerValue); //��ʾ����
                    //�����ٶȵ�
                    SpeedLedOn(ucSpeedGrad);	 //
                    strPeripheral.ucPowerLedShowPhase = 0;
                    strPeripheral.usRecvPowerON_Flag = 0x03;      //ִֻ��һ�Σ�����ʱִ�У�
                    strPeripheral.ucPowerLedShowPhase = 0;
                    ucBeep_Alarm_Phase = 0;
                    ucBeep_Alarm_Num = 0;
                    ucLessThanLowPower = 0;
                    strPeripheral.ucPowerLedNum  = 0;
                    break;
                }
            }
        }
        /*******����Ƿ�����*******/
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
        else if(strPeripheral.ucRecvAlarmNum)//�й���
        {
            switch(ucBeep_Alarm_Phase)
            {
            case 0:
                SetPWM1_Out(SpeedKeyTime, 0);//ucDebugSpeaker = 10;
                SPEED_LED_0;
                POWER_SHOW_0;
                ucBeep_Alarm_Phase = 1;
                break;
            case 1:
                if(strPeripheral.PowerLEDShowTime >= TIMER_400MS)
                {
                    SetPWM1_Out(SpeedKeyTime, SpeedKeyPulse); //��������һ��
                    POWER_SHOW_5;                      //������ȫ��
                    strPeripheral.PowerLEDShowTime = 0;
                    ucBeep_Alarm_Phase = 2;
                }
                break;
            case 2:
                if(strPeripheral.ucRecvAlarmNum < 0x80)
                {
                    if(strPeripheral.PowerLEDShowTime >= TIMER_400MS)
                    {
                        SetPWM1_Out(0, 0); //��������һ��
                        POWER_SHOW_0;                      //������ȫ��
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
                        SetPWM1_Out(0, 0); //��������һ��
                        POWER_SHOW_0;                      //������ȫ��
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
                    SetPWM1_Out(SpeedKeyTime, 0);//ucDebugSpeaker = 11;
                    ucBeep_Alarm_Phase = 1;
                    strPeripheral.PowerLEDShowTime = 0;
                }
                break;
            case 1:
                if(strPeripheral.PowerLEDShowTime >= TIMER_20MS)
                {
                    SetPWM1_Out(SpeedKeyTime, SpeedKeyPulse); //��������һ��
                    strPeripheral.PowerLEDShowTime = 0;
                    ucBeep_Alarm_Phase = 2;
                }
                break;
            case 2:
                if(strPeripheral.PowerLEDShowTime >= TIMER_20MS)
                {
                    SetPWM1_Out(0, 0); //��������һ��
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
        else if((strPeripheral.Input.Bits.SpeedModify) || (strPeripheral.Input.Bits.PowerChange)|| (strPeripheral.Input.Bits.BackCarAlarmModify))
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
												SpeedLedOn(ucSpeedGrad);	//��ʾ�ٶ�
												PowerLedNumOn(strPeripheral.ucPowerLedNum);
								}
								else
								{
										if((ucEepromBuf[2] & 0x40) == 0x40)//﮵��
										{
												if(strPeripheral.ucPowerLedNum < 6)
												{
														strPeripheral.ucPowerLedNum++;
												}
												else strPeripheral.ucPowerLedNum = 0;
										}
										else//Ǧ����
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
                if((ucEepromBuf[2] & 0x40) == 0x40)//﮵��
                {
                    if(strPeripheral.ucPowerLedNum < 6)
                    {
                        strPeripheral.ucPowerLedNum++;
                    }
                    else strPeripheral.ucPowerLedNum = 0;
                }
                else//Ǧ����
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
        else //���ڳ���� �����޹��� ������ʾ����
        {
            ucBeep_Alarm_Phase = 0;
            ucBeep_Alarm_Num = 0;
            if(TRUE == strPeripheral.usRecvPowerOFF_Flag)//�ػ�
            {
                if(GLOBAL_SPEED_GRAD != ucSpeedGrad)
                {
                    EEPROM_WriteByte(SPEED_ADDRESS, ucSpeedGrad);
                    ucSpeedGrad = GLOBAL_SPEED_GRAD;
                }
                SPEED_LED_0;   //�ر��ٶȵ�
                POWER_SHOW_0;  //�رյ�Դ��
                strPeripheral.ucPowerLedShowPhase = 0;
                strPeripheral.ucRecvData_Flag = FALSE;
                strPeripheral.PowerLEDShowTime = 0;
                strPeripheral.ucRecvPowerValue_Flag = 0;
            }
            else if(strPeripheral.ucRecvPowerValue_Flag)//(strPeripheral.PowerLEDShowTime > TIMER_300MS)
            {
                PowerLedOn(strPeripheral.uiRecvPowerValue); //��ʾ����
                SpeedLedOn(ucSpeedGrad);	//��ʾ�ٶ�
            }
        }
        /*******������*******/

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
                SetPWM1_Out(SpeedKeyTime, 0);//ucDebugSpeaker = 12;
                ucBackSpeekFlag = TRUE;
            }
        }
        if(strPeripheral.AlarmTime > TIMER_100MS)
        {
            strPeripheral.AlarmTime = 0;
            if(FALSE == strPeripheral.Input.Bits.SpeakerKey)SetPWM1_Out(SpeedKeyTime, 0);//ucDebugSpeaker = 13;
            strPeripheral.Output.Bits.Alarm = 0;
        }
    }//����ͨ�ź�ִ��
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
                (0 == strPeripheral.SpeedUpKeyTime) )
        {
            SetPWM1_Out(SpeedKeyTime, 0);//ucDebugSpeaker = 14;
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
    PeripheralOutputHandle();//����ָʾ�ơ��ٶ�ָʾ�ơ���������EEPROM

    if(0x03 == strPeripheral.ucRecvData_Flag)    //����ͨ�Ÿ�ֵ0X03
    {
        if(strPeripheral.uiRecvPowerValue)//������Ϊ��ʱ�ż�ⰴ��
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
    PeripheralOutputHandle();//����ָʾ�ơ��ٶ�ָʾ�ơ���������EEPROM

    if(0x03 == strPeripheral.ucRecvData_Flag)    //����ͨ�Ÿ�ֵ0X03
    {
        if(strPeripheral.uiRecvPowerValue)//������Ϊ��ʱ�ż�ⰴ��
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
    PeripheralOutputHandle();//����ָʾ�ơ��ٶ�ָʾ�ơ���������EEPROM

    if(0x03 == strPeripheral.ucRecvData_Flag)    //����ͨ�Ÿ�ֵ0X03
    {
        if(strPeripheral.uiRecvPowerValue)//������Ϊ��ʱ�ż�ⰴ��
        {
            PeripheralInputHandle();
        }
        vSpeakershutOut();
    }
}
#endif

/************************************************
;  *    @Function Name       : EEPROM_Byte_Erase
;  *    @Description         : eeprom�ֲ���ģʽ��ʼ��
;  *    @IN_Parameter      	 :addar 0-0x3ff W_data ����
;  *    @Return parameter    :
;  ***********************************************/
void EEPROM_Byte_Erase(unsigned int addar)
{
    clr_EA;
    SET_IAPEN();
    SET_IAPEN(); //��FALSH IAP���ܣ�����д������Ч
    SET_EPAGEERS();
    SET_EPAGEERS(); //ҳ��ģʽ
    IAPKEY = 0x5a;
    IAPKEY = 0xa5;
    SET_IAPSTR(); //IAPSTRλ��һ������IAP����
    IAPADH = (addar >> 8) & 0xff;
    IAPADL = addar & 0xff;
    IAPDB = 0;
    IAPADL &= 0xf0;
    IAPKEY = 0x5a;
    IAPKEY = 0xa5;
    SET_IAPTRIG0(); //IAPTRIG0��һ
    IAPKEY = 0x5a;
    IAPKEY = 0xa5;
    SET_IAPTRIG1(); //IAPTRIG1��һ
    IAPKEY = 0x5a;
    IAPKEY = 0xa5;
    SET_IAPTRIG2(); //IAPTRIG1��һ
    while (IAPCR & 0x80);

}
/************************************************
;  *    @Function Name       : EEPROM_Init_Byte_Write
;  *    @Description         : eeprom�ֽ�д
;  *    @IN_Parameter        : addar 0-0x3ff W_data ����
;  *    @Return parameter    :
;  ***********************************************/
void EEPROM_Byte_Write(unsigned int addar, unsigned char W_data)
{

    SET_IAPEN();
    SET_IAPEN(); //��FALSH IAP���ܣ�����д������Ч
    SET_EPAGEWR();
    SET_EPAGEWR(); //ҳдģʽ
    IAPKEY = 0x5a;
    IAPKEY = 0xa5;
    SET_IAPSTR(); //IAPSTRλ��һ������IAP����
    IAPADH = (addar >> 8) & 0xff;
    IAPADL = addar & 0xff;
    IAPDB = W_data; //W_data
    IAPADL &= 0xf0;
    IAPKEY = 0x5a;
    IAPKEY = 0xa5;
    SET_IAPTRIG0(); //IAPTRIG0��һ
    IAPKEY = 0x5a;
    IAPKEY = 0xa5;
    SET_IAPTRIG1(); //IAPTRIG1��һ
    IAPKEY = 0x5a;
    IAPKEY = 0xa5;
    SET_IAPTRIG2(); //IAPTRIG1��һ
    while (IAPCR & 0x80);
    set_EA;
}
/************************************************
;  *    @Function Name       : EEPROM_Byte_Read
;  *    @Description         : eeprom�ֽڶ�
;  *    @IN_Parameter        : addar 0-0x3ff
;  *    @Return parameter    : ����
;  ***********************************************/
uint8_t EEPROM_Byte_Read(unsigned int addr)
{
    uint8_t R_data;
    clr_EA;
    SET_EERD(); //EERD��1
    R_data = *(uint8_t __IO code *)addr;
    CLR_EERD(); //EERD��0
    set_EA;
    return R_data;

}
/************************************************
;  *    @Function Name       : EEPROM_Read
;  *    @Description         : EEPROM���ճ��ȶ�ȡ
;  *    @IN_Parameter        :
;  *    @Return parameter    :
;  ***********************************************/
void EEPROM_Read(unsigned int addr, unsigned char idata *R_data, unsigned int len)
{
    uint16_t cnt;
    clr_EA;
    SET_EERD(); //EERD��1
    for (cnt = 0; cnt < len; cnt++)
    {
        R_data[cnt] = *(uint8_t __IO code *)addr;
        addr++;
    }
    CLR_EERD(); //EERD��0
    set_EA;
}


void myFuncInit(){
    clr_EA;      //�ر��ж�
    GPIO_Init();
	  Time0_Init();    
    PWM1_Init();
    vInitAdc();
    //Uart_Init();
	  set_EA;      //���ж�
}
void myFuncLed(){
  //��Դ��
	//P21D = 1;//��1
	P24D = 1;//��2
	//P27D = 1;//��3
	P26D = 1;//��4
	//P10D = 1;//��5
	
	//�ٶȵ�
	P00D = 1;//��1
	//P01D = 1;//��2
	P14D =1;//��3
	//P12D = 1;//��4
	P11D = 1;//��5
}
void MyFuncSpeaker(){
	
	unsigned short usReadSpeakerPwmFlag = 0;
	
	if(Globle_Timer_1MS.Bits.CheckKeyPress)
  {
      Globle_Timer_1MS.Bits.CheckKeyPress = 0;
			
			if(uiAdVoltage > 3350)//4096
			{
					ucKeyPressFlag = FALSE;
					strPeripheral.SpeedUpKeyTime = 0;
					strPeripheral.SpeedDownKeyTime = 0;
					strPeripheral.SpeakerKeyTime = 0;
					strPeripheral.SpeedDownAndUpTime = 0;
					strPeripheral.SpeakerAndSpeedUpTime = 0;
					strPeripheral.Input.Bits.SpeedDownKeyPressFlag =  0;
          ucBuzzerPressedFlg = FALSE;
			}
			else if((uiAdVoltage < 2164) && (uiAdVoltage > 1944))//
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
			
			
			if(strPeripheral.Input.Bits.SpeakerKey)
				{
						if (TRUE != strPeripheral.ucRecvOrderInCharge)
						{
								strPeripheral.Output.Bits.Alarm = TRUE;
								SetPWM1_Out(LabaKeyPressSpeekTime, LabaKeyPressSpeekPulse);
								//if(FALSE == ucBuzzerPressedFlg)vSendBuzzerOrder(0x01);
								ucBuzzerPressedFlg = TRUE;
						}else{
								ucBuzzerPressedFlg = FALSE;
						}
				}
		}
	  usReadSpeakerPwmFlag =  usReadSpeakerPwm();
  	if(usReadSpeakerPwmFlag && 0 == strPeripheral.SpeakerKeyTime)
    {
			SetPWM1_Out(0, 0);
//        Globle_Timer_1MS.Bits.CheckIDele = 0;
//        if((0 == strPeripheral.ucRecvBikeInBack) 
//					&& (0 == ucLess18Power) 
//				  &&(0 == strPeripheral.ucRecvAlarmNum) 
//				  && (FALSE == ucBuzzerTstFlg) 
//				  && (FALSE == ucBuzzerPressedFlg) 
//				  && (0 == ucKeyPressFlag) 
//				  && (0 == strPeripheral.ucRecvWrite60Bytes_Flag) 
//				  && (0x03 == strPeripheral.usRecvPowerON_Flag) 
//				  && (0 == strPeripheral.SpeakerAndSpeedUpTime) 
//				  && (0 == strPeripheral.SpeedDownAndUpTime) 
//				  && (0 == strPeripheral.SpeedDownKeyTime) 
//				  && (0 == strPeripheral.SpeedUpKeyTime) )
//        {
//            SetPWM1_Out(SpeedKeyTime, 0);//ucDebugSpeaker = 14;
//            for(ucKeyPressFlag = 0; ucKeyPressFlag < 60; ucKeyPressFlag++)
//            {
//                Globle_Timer_1MS.Bits.CheckIDele  = 0;
//                while(0 == Globle_Timer_1MS.Bits.CheckIDele);
//            }
//            vSendBuzzerOrder(0x02);
//            Globle_Timer_1MS.Bits.CheckIDele = ucKeyPressFlag = 0;
//        }
    }
	
}