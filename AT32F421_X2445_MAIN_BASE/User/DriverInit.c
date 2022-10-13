
/*
       "LG_X2445M_V2"  Design By LG_qiuzhi
       V1.0   2021.04
       used mcu  AT32F421C8
*/

#include "DriverConst.h"
void vInitPortAsActive(void);
static void ADC1_Mode_Config(void);
void DMA_Configuration(void);
volatile unsigned short vDmaContAdcVaule[OneChannelConvertTimes][TotalADCChannels];
static void SetBaudrate(unsigned short usBaudRate);
void vInitTimer(void);
void vSetTeacherPortInt(void);
unsigned int uiMotor1LeftHighPwmRegAdd = 0;
unsigned int uiMotor1RightHighPwmRegAdd = 0;
unsigned int uiMotor2LeftHighPwmRegAdd = 0;
unsigned int uiMotor2RightHighPwmRegAdd = 0;
unsigned char ucConnectBreakFlag = FALSE;//�ڿ������Ӳ����·��ʱ�򣬼��ɲ�����Ƿ�һ�����ӱ�־
#ifdef UsedUartDmaRxData
unsigned char ucUartDmaRxData[RecDmaBufferSize] = {0};
#endif

#ifdef UsedMlx90393
void vTime14Mxl90393(void);
#endif

void vInitPortAsWaiteMode(void)
{
    GPIO_InitType GPIO_InitStructure = {0};
    EXTI_InitType EXTI_InitStructure = {0};
    NVIC_InitType NVIC_InitStructure = {0};
//    System_Init();
    RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_GPIOB | RCC_AHBPERIPH_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pins = ResetStm8Pin ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_OD;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_2MHz;
    GPIO_Init(ResetStm8Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pins = ExPortSpeekerPin ;
    GPIO_Init(ExPortSpeekerPort, &GPIO_InitStructure);	//PB12 �������� ����  �ڴ�����ʱ��ǿ�����Ϊ�͵�ƽ
    C_EnResetStm8;
    CtrlExtSpeekerDis;

//	  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_2 ;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_PD;
//    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
//		
//		RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_SYSCFGCOMP, ENABLE);      ///<Enable SYSCFG clock
//	  SYSCFG_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinsSource2); ///ͣ��ģʽ�Ļ���
//    EXTI_StructInit(&EXTI_InitStructure);
//    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
//    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     /* �ж�ģʽ */
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; /* �½��ش��� */
//    EXTI_InitStructure.EXTI_LineEnable = ENABLE;
//    EXTI_Init(&EXTI_InitStructure); 

//    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_2_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure); //ͣ��ģʽ�Ļ���

//    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);//ͣ��ģʽ

}

//void EXTI3_2_IRQHandler(void)
//{
//    if (EXTI_GetIntStatus(EXTI_Line2) != RESET)
//    {
//        EXTI_ClearIntPendingBit(EXTI_Line2); ///
//    }
//}

void vNoKeyInStandbyMode(void)//���ϵ磬û�а����������
{
    C_EnResetStm8;
    RCC->CFG &= 0xFFFFFFFC;//ʱ�ӵ�ʹ�ã�ʹ��Ƶ�����
//	  RCC->CTRL &= ~RCC_CTRL_HSEEN;
//	  RCC->CTRL &= ~RCC_CTRL_PLLEN;//
    while(!ReadKeyOfOn_OffFlag)//Waite for Power Key Press Up !
    {
        vSoftDelayms(1);
    }
		RCC->CFG |= RCC_CFG_SYSCLKSEL_1;//ʱ�ӵ�ʹ�ã�ʹ��Ƶ�����
//	  RCC->CTRL |= RCC_CTRL_HSEEN;
//	  RCC->CTRL |= RCC_CTRL_PLLEN;//		
    C_DisEnResetStm8;
//		EXTI->INTEN &= ~EXTI_INTEN_LN2;
}





void vSysTemSourceAsActive(void)
{
    vInitPortAsActive();
    ADC1_Mode_Config();
//    bsp_DelayMS(2);
    DMA_Configuration();//��ADC��DMA
//    bsp_DelayMS(2);
    ADC_SoftwareStartConvCtrl(ADC1, ENABLE);
//    bsp_DelayMS(2);
    //SetBaudrate(USART2, UartBaudRate);
    SetBaudrate(9600);
//    bsp_DelayMS(10);
	  vInitTimer();	
//	  bsp_DelayMS(2);
    vSetTeacherPortInt();
#ifdef UsedMlx90393
//	  bsp_DelayMS(10);
    //vTime14Mxl90393();
#endif
}


//�����������Ź�
//ucPrer=4
//uiRlr=1250 2s  Tout=((4��2^ucPrer)��uiRlr) /40
void vInitWatchDog(unsigned int ucPrer, unsigned int uiRlr) //������Ƭ���Ŀ��Ź�
{
    //    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //ʹ�ܶԼĴ���IWDG_PR��IWDG_RLR��д����
    //    IWDG_SetPrescaler(ucPrer);  //����IWDGԤ��Ƶֵ:����IWDGԤ��ƵֵΪ64
    //    IWDG_SetReload(uiRlr);  //����IWDG��װ��ֵ
    //    IWDG_ReloadCounter();  //����IWDG��װ�ؼĴ�����ֵ��װ��IWDG������
    //    IWDG_Enable();  //ʹ��IWDG
}
void vInitPortAsActive(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* GPIO Ports Clock Enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_GPIOA | RCC_AHBPERIPH_GPIOB | RCC_AHBPERIPH_DMA1 |
                          RCC_AHBPERIPH_GPIOC | RCC_AHBPERIPH_GPIOF, ENABLE);

    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR3 , ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_TMR1 | RCC_APB2PERIPH_TMR15 | RCC_APB2PERIPH_ADC1, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_SYSCFGCOMP, ENABLE);		///<Enable SYSCFG clock

    /*Configure the LED pin as ouput push-pull*/
    GPIO_InitStructure.GPIO_Pins =  GPIO_Pins_0 | GPIO_Pins_1 | GPIO_Pins_2 | GPIO_Pins_4 | GPIO_Pins_5 | GPIO_Pins_6 | GPIO_Pins_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     //  ϵͳ ADC1-4  ADC6 ADC7
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);//ADC1-4   ADC6 ADC7


    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_12 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_PU;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;//PA12
    GPIO_Init(GPIOA, &GPIO_InitStructure);//ʾ��ģʽͨ��

    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_15 ;//�����������ӿ�
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pins =  GPIO_Pins_0 | GPIO_Pins_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     //  ϵͳ
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//

    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_PD;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//�����������

    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_3 ;//�����������ӿ�
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_8 | GPIO_Pins_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//ɲ�������

    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_6 | GPIO_Pins_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//PB 6 7 8 9

    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_10 | GPIO_Pins_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);                         //����EC�̵���

    GPIO_InitStructure.GPIO_Pins =  GPIO_Pins_11 | GPIO_Pins_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//

    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_14 ;//�ɳ�����ʹ�õ������� PB12
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_13 | GPIO_Pins_15 ;//ɲ������·
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_6 | GPIO_Pins_7;//�����̵�������  PB15  ����Դ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    C_SetS_MotoA_A_AdcAsM1_Left_Port_High;
    C_SetS_MotoA_B_AdcAsM1_Right_Port_High;
    C_SetS_MotoB_A_AdcAsM2_Left_Port_High;
    C_SetS_MotoB_B_AdcAsM2_Right_Port_High;

    CtlDriverPowerContolOFF;
    CtlBrakeBikeDisEn;
    CtlPowerOnTest_OFF;
    CtrlExtSpeekerDis;
    CtlBrakeBikeDisEn;
    FeedDogEn;
}

#define MotoTestTime 35
unsigned char ucCheckMotoConnect(void)//������������
{
    unsigned int uiTemp;
    unsigned short usMotoA_RightAdc[80] = {0};
    unsigned short usMotoA_LeftAdc[80] = {0};
    unsigned short usMotoB_RightAdc[80] = {0};
    unsigned short usMotoB_LeftAdc[80] = {0};
    unsigned char ucMotoA_RightCont = 0;
    unsigned char ucMotoA_LeftCont = 0;
    unsigned char ucMotoB_RightCont = 0;
    unsigned char ucMotoB_LeftCont = 0;
    ucEnAdcCalc  = FALSE;
    g_uiTimeNumCont1ms = 3;
    while(g_uiTimeNumCont1ms);//��ֹ����Ƕ�׵���
    SetTim1CC1andCCR2(0);
    SetTim1CC3andCCR4(0);
    SetTim4CC1andCCR2(0);
    SetTim4CC3andCCR4(0);
    C_SetS_MotoA_A_AdcAsM1_Left_Port_High;
    C_SetS_MotoA_B_AdcAsM1_Right_Port_High;
    C_SetS_MotoB_A_AdcAsM2_Left_Port_High;
    C_SetS_MotoB_B_AdcAsM2_Right_Port_High;//���õ��������������  ����ʹ��
    ucMotoA_RightCont = ucMotoA_LeftCont = ucMotoB_RightCont = ucMotoB_LeftCont = 0;
    for(uiTemp = 0; uiTemp < 70; uiTemp++)//��ż��1ms
    {
        vDealAdcVauleMotoAndCurrent();
        usMotoA_RightAdc[uiTemp]  = strMoto1Info.uiRightHBridgeHaveFilterAdc;
        usMotoA_LeftAdc[uiTemp]  = strMoto1Info.uiLeftHBridgeHaveFilterAdc;
        usMotoB_RightAdc[uiTemp]  = strMoto2Info.uiRightHBridgeHaveFilterAdc;
        usMotoB_LeftAdc[uiTemp]  = strMoto2Info.uiLeftHBridgeHaveFilterAdc;
    }
    for(uiTemp = 0; uiTemp < 70; uiTemp++)
    {
        if(usMotoA_RightAdc[uiTemp] < 300)ucMotoA_RightCont++;
        if(usMotoA_LeftAdc[uiTemp] < 300)ucMotoA_LeftCont++;
        if(usMotoB_RightAdc[uiTemp] < 300)ucMotoB_RightCont++;
        if(usMotoB_LeftAdc[uiTemp] < 300)ucMotoB_LeftCont++;//һ��ƽ��ֵΪ 800
    }//
    if((ucMotoA_RightCont > 40) || (ucMotoA_LeftCont > 40))//�����ѹ���� ˵����������������
    {
        CtlPowerOnTest_OFF;
        ucErroType = ErroMoto1HighBriageShort;
        if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor1Motor2)ucErroType = ErroMoto2HighBriageShort;
        ucEnAdcCalc = TRUE;
        return FALSE;
    }
    if((ucMotoB_RightCont > 40) || (ucMotoB_LeftCont > 40))//�����ѹ ������  ˵���������߲����������
    {
        CtlPowerOnTest_OFF;
        ucErroType = ErroMoto2HighBriageShort;
        if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor1Motor2)ucErroType = ErroMoto1HighBriageShort;
        ucEnAdcCalc = TRUE;
        return FALSE;
    }
    if(ucPowerKeyPressTimes >= 2)
    {
        ucEnAdcCalc = TRUE;
        vAutoPowerOffTimeFlag();
    }
#define CheckMotoTestPwmAsSlop   3000//��������  ����Ĳ��Ե�ѹ  ���ڼ�����һ�������յģ�û���κ����⡣���Բ���
    if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bCheckMotorConnect)//���ϵͳҪ������������
    {
        C_SetS_MotoA_A_AdcAsM1_Left_Port_Low;
        C_SetS_MotoA_B_AdcAsM1_Right_Port_Low;//���õ���� ������Ϊ����
        g_uiTimeNumCont1ms = 2;
        while(g_uiTimeNumCont1ms);
        ucMotoA_RightCont = ucMotoA_LeftCont = ucMotoB_RightCont = ucMotoB_LeftCont = 0;
        for(uiTemp = 0; uiTemp < 70; uiTemp++)//��ż��1ms
        {
            vDealAdcVauleMotoAndCurrent();
            usMotoA_RightAdc[uiTemp]  = strMoto1Info.uiRightHBridgeHaveFilterAdc;
            usMotoA_LeftAdc[uiTemp]  = strMoto1Info.uiLeftHBridgeHaveFilterAdc;
        }
        for(uiTemp = 0; uiTemp < 70; uiTemp++)
        {
            if(usMotoA_RightAdc[uiTemp] > 500)ucMotoA_RightCont++;
            if(usMotoA_LeftAdc[uiTemp] > 500)ucMotoA_LeftCont++;//���û�н���ѹ��������˵����Ƭ����������������
        }
        if((ucMotoA_RightCont > 40) || (ucMotoA_LeftCont > 40))//����������� û��Ч��
        {
            CtlPowerOnTest_OFF;
            SetTim1CC1andCCR2Reg(0, 0);
            SetTim1CC3andCCR4Reg(0, 0);
            SetTim4CC1andCCR2Reg(0, 0);
            SetTim4CC3andCCR4Reg(0, 0);
            ucErroType = ErroMoto1HighBriageShort;
            if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor1Motor2)ucErroType = ErroMoto2HighBriageShort;
            ucEnAdcCalc = TRUE;
            return FALSE;
        }
        SetTim1CC1andCCR2Reg(CheckMotoTestPwmAsSlop, CheckMotoTestPwmAsSlop);
        SetTim1CC3andCCR4Reg(SetPwmPinHighReg, 0);
        SetTim4CC1andCCR2Reg(0, 0);
        SetTim4CC3andCCR4Reg(0, 0);//�õ��A���һ��
        ucMotoA_RightCont = ucMotoA_LeftCont = ucMotoB_RightCont = ucMotoB_LeftCont = 0;
        for(uiTemp = 0; uiTemp < 70; uiTemp++)//��ż��1ms
        {
            vDealAdcVauleMotoAndCurrent();
            usMotoA_RightAdc[uiTemp]  = strMoto1Info.uiRightHBridgeHaveFilterAdc;
            usMotoA_LeftAdc[uiTemp]  = strMoto1Info.uiLeftHBridgeHaveFilterAdc;
        }
        for(uiTemp = 0; uiTemp < 70; uiTemp++)
        {
            if(usMotoA_RightAdc[uiTemp] < 50)ucMotoA_RightCont++;
            if(usMotoA_LeftAdc[uiTemp] < 50)ucMotoA_LeftCont++;//������
        }
        if((ucMotoA_RightCont > 40) || (ucMotoA_LeftCont > 40))//˵���ж��߻��� ��mos��������  û�н�Ӧ������ĵ�ѹ�������
        {
            CtlPowerOnTest_OFF;
            SetTim1CC1andCCR2Reg(0, 0);
            SetTim1CC3andCCR4Reg(0, 0);
            SetTim4CC1andCCR2Reg(0, 0);
            SetTim4CC3andCCR4Reg(0, 0);
            ucErroType = ErroMoto1HighBriageShort;
            if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor1Motor2)ucErroType = ErroMoto2HighBriageShort;
            ucEnAdcCalc = TRUE;//����д������ͼ�ʱ�˳�
            return FALSE;
        }
        SetTim1CC1andCCR2Reg(0, 0);
        SetTim1CC3andCCR4Reg(0, 0);
        SetTim4CC1andCCR2Reg(0, 0);
        SetTim4CC3andCCR4Reg(0, 0);//��ʱ������������źŻ�ԭ
        if(ucPowerKeyPressTimes >= 2)
        {
            ucEnAdcCalc = TRUE;
            vAutoPowerOffTimeFlag();//����а����Ķ����������˳���
        }
        g_uiTimeNumCont1ms = 5;
        while(g_uiTimeNumCont1ms);//����һ��С��ʱ�����첽���
        SetTim1CC1andCCR2Reg(SetPwmPinHighReg, 0);
        SetTim1CC3andCCR4Reg(CheckMotoTestPwmAsSlop, CheckMotoTestPwmAsSlop);
        SetTim4CC1andCCR2Reg(0, 0);
        SetTim4CC3andCCR4Reg(0, 0);//����һ����Կ�ʼ
        ucMotoA_RightCont = ucMotoA_LeftCont = ucMotoB_RightCont = ucMotoB_LeftCont = 0;
        for(uiTemp = 0; uiTemp < 70; uiTemp++)//��ż��1ms
        {
            vDealAdcVauleMotoAndCurrent();
            usMotoA_RightAdc[uiTemp]  = strMoto1Info.uiRightHBridgeHaveFilterAdc;
            usMotoA_LeftAdc[uiTemp]  = strMoto1Info.uiLeftHBridgeHaveFilterAdc;
        }
        for(uiTemp = 0; uiTemp < 70; uiTemp++)
        {
            if(usMotoA_RightAdc[uiTemp] < 50)ucMotoA_RightCont++;
            if(usMotoA_LeftAdc[uiTemp] < 50)ucMotoA_LeftCont++;
        }
        if((ucMotoA_RightCont > 40) || (ucMotoA_LeftCont > 40))//˵���ж��߻��� ��mos��������
        {
            CtlPowerOnTest_OFF;
            SetTim1CC1andCCR2Reg(0, 0);
            SetTim1CC3andCCR4Reg(0, 0);
            SetTim4CC1andCCR2Reg(0, 0);
            SetTim4CC3andCCR4Reg(0, 0);
            ucErroType = ErroMoto1HighBriageShort;
            if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor1Motor2)ucErroType = ErroMoto2HighBriageShort;
            ucEnAdcCalc = TRUE;
            return FALSE;
        }
        SetTim1CC1andCCR2Reg(0, 0);
        SetTim1CC3andCCR4Reg(0, 0);
        SetTim4CC1andCCR2Reg(0, 0);
        SetTim4CC3andCCR4Reg(0, 0);
        if(ucPowerKeyPressTimes >= 2)
        {
            ucEnAdcCalc = TRUE;
            vAutoPowerOffTimeFlag();
        }
        C_SetS_MotoA_A_AdcAsM1_Left_Port_High;
        C_SetS_MotoA_B_AdcAsM1_Right_Port_High;  //��ԭ������
        ////////////////////////////////////////////////   ���濪ʼ�����B
        C_SetS_MotoB_A_AdcAsM2_Left_Port_Low;
        C_SetS_MotoB_B_AdcAsM2_Right_Port_Low;
        g_uiTimeNumCont1ms = 5;
        while(g_uiTimeNumCont1ms);
        ucMotoA_RightCont = ucMotoA_LeftCont = ucMotoB_RightCont = ucMotoB_LeftCont = 0;
        for(uiTemp = 0; uiTemp < 70; uiTemp++)//��ż��1ms
        {
            vDealAdcVauleMotoAndCurrent();
            usMotoA_RightAdc[uiTemp]  = strMoto2Info.uiRightHBridgeHaveFilterAdc;
            usMotoA_LeftAdc[uiTemp]  = strMoto2Info.uiLeftHBridgeHaveFilterAdc;
        }
        for(uiTemp = 0; uiTemp < 70; uiTemp++)
        {
            if(usMotoA_RightAdc[uiTemp] > 500)ucMotoA_RightCont++;
            if(usMotoA_LeftAdc[uiTemp] > 500)ucMotoA_LeftCont++;
        }
        if((ucMotoA_RightCont > 40) || (ucMotoA_LeftCont > 40))//����������� û��Ч��
        {
            CtlPowerOnTest_OFF;
            SetTim1CC1andCCR2Reg(0, 0);
            SetTim1CC3andCCR4Reg(0, 0);
            SetTim4CC1andCCR2Reg(0, 0);
            SetTim4CC3andCCR4Reg(0, 0);
            ucErroType = ErroMoto2HighBriageShort;
            if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor1Motor2)ucErroType = ErroMoto1HighBriageShort;
            ucEnAdcCalc = TRUE;
            return FALSE;
        }
        SetTim1CC1andCCR2Reg(0, 0);
        SetTim1CC1andCCR2Reg(0, 0);
        SetTim4CC1andCCR2Reg(CheckMotoTestPwmAsSlop, CheckMotoTestPwmAsSlop);
        SetTim4CC3andCCR4Reg(SetPwmPinHighReg, 0);
        ucMotoA_RightCont = ucMotoA_LeftCont = ucMotoB_RightCont = ucMotoB_LeftCont = 0;
        for(uiTemp = 0; uiTemp < 70; uiTemp++)//��ż��1ms
        {
            vDealAdcVauleMotoAndCurrent();
            usMotoA_RightAdc[uiTemp]  = strMoto2Info.uiRightHBridgeHaveFilterAdc;
            usMotoA_LeftAdc[uiTemp]  = strMoto2Info.uiLeftHBridgeHaveFilterAdc;
        }
        for(uiTemp = 0; uiTemp < 70; uiTemp++)
        {
            if(usMotoA_RightAdc[uiTemp] < 50)ucMotoA_RightCont++;
            if(usMotoA_LeftAdc[uiTemp] < 50)ucMotoA_LeftCont++;
        }
        if((ucMotoA_RightCont > 40) || (ucMotoA_LeftCont > 40))//˵���ж��߻��� ��mos��������
        {
            CtlPowerOnTest_OFF;
            SetTim1CC1andCCR2Reg(0, 0);
            SetTim1CC3andCCR4Reg(0, 0);
            SetTim4CC1andCCR2Reg(0, 0);
            SetTim4CC3andCCR4Reg(0, 0);
            ucErroType = ErroMoto2HighBriageShort;
            if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor1Motor2)ucErroType = ErroMoto1HighBriageShort;
            ucEnAdcCalc  = TRUE;
            return FALSE;
        }
        SetTim1CC1andCCR2Reg(0, 0);
        SetTim1CC3andCCR4Reg(0, 0);
        SetTim4CC1andCCR2Reg(0, 0);
        SetTim4CC3andCCR4Reg(0, 0);
        if(ucPowerKeyPressTimes >= 2)
        {
            ucEnAdcCalc = TRUE;
            vAutoPowerOffTimeFlag();
        }
        g_uiTimeNumCont1ms = 5;
        while(g_uiTimeNumCont1ms);
        SetTim1CC1andCCR2Reg(0, 0);
        SetTim1CC3andCCR4Reg(0, 0);
        SetTim4CC1andCCR2Reg(SetPwmPinHighReg, 0);
        SetTim4CC3andCCR4Reg(CheckMotoTestPwmAsSlop, CheckMotoTestPwmAsSlop);
        ucMotoA_RightCont = ucMotoA_LeftCont = ucMotoB_RightCont = ucMotoB_LeftCont = 0;
        for(uiTemp = 0; uiTemp < 70; uiTemp++)//��ż��1ms
        {
            vDealAdcVauleMotoAndCurrent();
            usMotoA_RightAdc[uiTemp]  = strMoto2Info.uiRightHBridgeHaveFilterAdc;
            usMotoA_LeftAdc[uiTemp]  = strMoto2Info.uiLeftHBridgeHaveFilterAdc;
        }
        for(uiTemp = 0; uiTemp < 70; uiTemp++)
        {
            if(usMotoA_RightAdc[uiTemp] < 50)ucMotoA_RightCont++;
            if(usMotoA_LeftAdc[uiTemp] < 50)ucMotoA_LeftCont++;
        }
        if((ucMotoA_RightCont > 40) || (ucMotoA_LeftCont > 40))
        {
            CtlPowerOnTest_OFF;
            SetTim1CC1andCCR2Reg(0, 0);
            SetTim1CC3andCCR4Reg(0, 0);
            SetTim4CC1andCCR2Reg(0, 0);
            SetTim4CC3andCCR4Reg(0, 0);
            ucErroType = ErroMoto2HighBriageShort;
            if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor1Motor2)ucErroType = ErroMoto1HighBriageShort;
            ucEnAdcCalc  = TRUE;
            return FALSE;
        }
        SetTim1CC1andCCR2Reg(0, 0);
        SetTim1CC3andCCR4Reg(0, 0);
        SetTim4CC1andCCR2Reg(0, 0);
        SetTim4CC3andCCR4Reg(0, 0);
        if(ucPowerKeyPressTimes >= 2)
        {
            ucEnAdcCalc  = TRUE;
            vAutoPowerOffTimeFlag();
        }
        C_SetS_MotoB_A_AdcAsM2_Left_Port_High;
        C_SetS_MotoB_B_AdcAsM2_Right_Port_High;
        SetTim1CC1andCCR2Reg(0, 0);
        SetTim1CC3andCCR4Reg(0, 0);
        SetTim4CC1andCCR2Reg(0, 0);
        SetTim4CC3andCCR4Reg(0, 0);
        ////////////////////////////////////////////////
    }
    ucEnAdcCalc  = TRUE;
    return TRUE;
}
/*
���ϵͳ������ģ�����ܼ��û�
*@param checkMotoTestPwmFlag �Ƿ�����PWM 
*        @value ENABLE ����Դ
*        @value DISABLE ������Դ
*/
#define  ErroRemoteAlarmFlashLedTimeMs   120
#define  ErroRemoteAlarmWaiteTime         4
#define  RelaesPowerValue  1800//����ѹС�� 16.9v��ʱ�� ��Ҫȥ�ͷŵ�ѹ��
void vCheckSystemInfo(FunctionalState checkMotoTestPwmFlag)
{
    u32 uiTempXDelt, uiTempYDelt ;
    unsigned int uiTime = 0;
    unsigned char ucTempArry[2] = {0};
    unsigned char ucWhenRomteErroWateSHowLedTab[10][2] = {{0x02, 0x01}, {0x03, 0x03}, {0x03, 0x87}, {0x03, 0xCF}, {0x03, 0xFF}, {0x00, 0x30}, {0x00, 0x78}, {0x00, 0xFC}, {0x01, 0xFE}, {0x03, 0xFF}};
//#ifdef CheckrRemoteContolorConnect
//    uiTempXDelt = ClacXPwmFirstRemotePosP;
//    uiTempYDelt = ClacYPwmFirstRemotePosP;
//    //     uniDiverInfo.strDirverAtt.unMotorInfo.bits.bDisHardWareDog = FALSE;
//    if((strSysInfo.uiRemoteNowXPos < (strSysInfo.uiRemoteMiddleXPos - 2 * uiTempXDelt)) || (strSysInfo.uiRemoteNowXPos > (strSysInfo.uiRemoteMiddleXPos + 2 * uiTempXDelt)))
//    {
//        ucErroType = ErroNoRemote;
//        //CtlPowerOnTest_OFF;//�����ҡ�� �����м��λ�� ��ȥ�Ⱥ�
//    }
//    if((strSysInfo.uiRemoteNowYPos < (strSysInfo.uiRemoteMiddleYPos - 2 * uiTempYDelt)) || (strSysInfo.uiRemoteNowYPos > (strSysInfo.uiRemoteMiddleYPos + 2 * uiTempYDelt)))
//    {
//        ucErroType = ErroNoRemote;
//        //CtlPowerOnTest_OFF;
//    }
//#endif
			//���֮ǰ���ȳ�ʼ���������ͣ�
			ucErroType = ErroNoCheckSys;
#ifdef CheckPowerVoltage

    vSendSysPower(4000);//��һ����������
    uiTempYDelt = uiTempXDelt = strSysInfo.uiSysTemPower;//��������ֵ ��ʱ����
    if(uiTempYDelt > RelaesPowerValue)//���EC�ڿ�����ʱ���ѹС���Ͳ�Ҫȥ�ͷ�һ�ε�ѹ��
    {
        uiTempXDelt *= 9;
        uiTempXDelt /= 10;
			  if(ENABLE == checkMotoTestPwmFlag)//�����Ҫ�ͷ� ��� ��ѹ��������һ�μ����Ҫ�ͷţ������󱨴�����в���Ҫ�ͷŵ�ѹ
				{
						CtlSetMotor1LeftPwmPercent(CheckMotoTestPwmValue);
						CtlSetMotor1RightPwmPercent(0);
						CtlSetMotor2LeftPwmPercent(CheckMotoTestPwmValue);
						CtlSetMotor2RightPwmPercent(0);
				}
        
        for(uiTime = 0; uiTime < 400; uiTime ++)
        {
            ucTag1ms = FALSE;
            while(FALSE == ucTag1ms);
            if(ucPowerKeyPressTimes >= 2)
            {
                CtlSetMotor1LeftPwmPercent(0);
                CtlSetMotor1RightPwmPercent(0);
                CtlSetMotor2LeftPwmPercent(0);
                CtlSetMotor2RightPwmPercent(0);
                vAutoPowerOffTimeFlag();
            }
            if(strSysInfo.uiSysTemPower < (uiTempXDelt + 150))//�����ʱEC��ѹһֱ���½�
            {
                uiTime = 8888;
            }
        }

        if((strSysInfo.uiSysTemPower > (uiTempXDelt + 150)) && (uiTempYDelt > NoPowerOnAdcValue)) //���û���Ͽ���ϵͳ�ĵ�Դ��ʱ�� �е�ѹ˵�����д��
        {
            CtlPowerOnTest_OFF;
            ucErroType = ErroRelayShort;	//˵���Ǽ̵����ж�·
            CtlSetMotor1LeftPwmPercent(0);
            CtlSetMotor1RightPwmPercent(0);
            CtlSetMotor2LeftPwmPercent(0);
            CtlSetMotor2RightPwmPercent(0);
            return;
        }
    }

    CtlBrakeBikeDisEn;//��ɲ�������£���̵���û���ϵ硣����������
    CtlSetMotor1LeftPwmPercent(0);
    CtlSetMotor1RightPwmPercent(0);
    CtlSetMotor2LeftPwmPercent(0);
    CtlSetMotor2RightPwmPercent(0);
    for(uiTime = 0; uiTime < 50; uiTime ++)
    {
        ucTag1ms = FALSE;
        while(FALSE == ucTag1ms);
        if(ucPowerKeyPressTimes >= 2 )
        {
            vAutoPowerOffTimeFlag();
        }
    }
    CtlPowerOnTest_ON;
    for(uiTime = 0; uiTime < 150; uiTime ++)
    {
        ucTag1ms = FALSE;
        while(FALSE == ucTag1ms);
        if(ucPowerKeyPressTimes >= 2)
        {
            vAutoPowerOffTimeFlag();
        }
    }

    if(strSysInfo.uiSysTemPower < (NoPowerOnAdcValue - 200)) //���û���Ͽ���ϵͳ�ĵ�Դ��ʱ�� �е�ѹ˵�����д��
    {
        CtlPowerOnTest_OFF;//����ϵ��ʱ��  û�е�ѹ�ı仯�������ǵ�·�ж�·�������
        ucErroType = ErroRelayBreak ; //�̵����Ͽ�
        return;
    }//LG_qiuzhi
#endif
    //20190517�ϵ��쳣�Ͳ��������
    //    CtlDriverPowerContolON;
    for(uiTime = 0; uiTime < 50; uiTime ++)
    {
        ucTag1ms = FALSE;
        while(FALSE == ucTag1ms);
        if(ucPowerKeyPressTimes >= 2)
        {
            vAutoPowerOffTimeFlag();
        }
    }
    if(strSysInfo.uiSysTemPower <= LowPowerVoltage)
    {
        CtlPowerOnTest_OFF;
        ucErroType = ErroLowVoltage ; //�̵����Ͽ�
        return;
    }//LG_qiuzhi
    //    CtlPowerOnTest_OFF;
#define    AlowBreakAdcErro  180
    if(FALSE == ucCheckMotoConnect())return;
    if(FALSE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-ɲ���������0-פ�³����ɲ������6�����20190110
    {
        if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bCheckBreakConnect)
        {
            uiTempXDelt = strSysInfo.uiSysTemPower * BreakLeftBreakAdcP; //��������ֵ
            uiTempYDelt = strSysInfo.uiSysTemPower * BreakRightBreakAdcP; //�ҵ������ֵ
            if((strSysInfo.uiBreakFeedBackCurrent >= (uiTempXDelt - AlowBreakAdcErro)) && (strSysInfo.uiBreakFeedBackCurrent <= (uiTempXDelt + AlowBreakAdcErro)))
            {
                ucErroType = ErroMoto2Break; //��ɲ��������
                return;
            }
            else if((strSysInfo.uiBreakFeedBackCurrent >= (uiTempYDelt - AlowBreakAdcErro)) && (strSysInfo.uiBreakFeedBackCurrent <= (uiTempYDelt + AlowBreakAdcErro)))
            {
                ucErroType = ErroMoto1Break; //��ɲ��������
                return;
            }
            else if(strSysInfo.uiBreakFeedBackCurrent < (strSysInfo.uiSysTemPower * NoBreakAdcP + AlowBreakAdcErro) )
            {
                ucErroType = ErroNoBearker ; //���������ɲ������������
                return;
            }
        }
        if(strSysInfo.uiBreakFeedBackCurrent > (strSysInfo.uiSysTemPower * BreadAdAndPowerP - 650) )
        {
            ucConnectBreakFlag = TRUE; //ϵͳ��⵽ɲ����
        }
        else ucConnectBreakFlag = FALSE;
    }
    else
    {
        if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bCheckBreakConnect)
        {
            if(C_ReadLeftMotoBreak_Pin_Low)
            {
                ucErroType = ErroMoto1Break; //��ɲ��������
                if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor1Motor2)ucErroType = ErroMoto2Break; //��ɲ��������
                return;
            }
            if(C_ReadRightMotoBreak_Pin_Low)
            {
                ucErroType = ErroMoto2Break; //��ɲ��������
                if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor1Motor2)ucErroType = ErroMoto1Break; //��ɲ��������

                return;
            }
            if(C_ReadLeftMotoBreak_Pin_Low && C_ReadRightMotoBreak_Pin_Low)
            {
                ucErroType = ErroNoBearker ; //���������ɲ������������
                return;
            }
        }
        if((!C_ReadLeftMotoBreak_Pin_Low) || (!C_ReadRightMotoBreak_Pin_Low))
        {
            ucConnectBreakFlag = TRUE; //ϵͳ��⵽ɲ����
        }
        else ucConnectBreakFlag = FALSE;
    }
    CtlDriverPowerContolON;//�̵�����
    CtlPowerOnTest_OFF;//���ϵ�ر�
    vSendSingleOrder(OutDispLed);
    g_uiTimeNumCont1ms = 5;
    while(g_uiTimeNumCont1ms);
//    vSendSysPower(strSysInfo.uiSysTemPower);//��ʾ��	��  ���ﲻӦ��Ҫ�Ķ�Ӧ�����Զ�ʶ��
//    while(ErroNoRemote == ucErroType)
//    {

//        for(uiTime = 0; uiTime < 10; uiTime ++)
//        {
//            uiTempXDelt = 0;
//            while((strSysInfo.uiRemoteNowXPos > (strSysInfo.uiRemoteMiddleXPos - 2 * ClacXPwmFirstRemotePosP)) && (strSysInfo.uiRemoteNowXPos < (strSysInfo.uiRemoteMiddleXPos + 2 * ClacYPwmFirstRemotePosP))
//                    && (strSysInfo.uiRemoteNowYPos > (strSysInfo.uiRemoteMiddleYPos - 2 * ClacYPwmFirstRemotePosP)) && (strSysInfo.uiRemoteNowYPos < (strSysInfo.uiRemoteMiddleYPos + 2 * ClacYPwmFirstRemotePosP)))
//            {
//                uiTempXDelt ++;
//                g_uiTimeNumCont1ms = 5;
//                while(g_uiTimeNumCont1ms);
//                if(uiTempXDelt > 75)
//                {
//                    vSendSingleOrder(OutDispLed);
//                    ucErroType = ErroNoErro;
//                    break;
//                }
//            }
//            //if(ReadKeyOfOn_OffFlag)uiTempXDelt = 100;
//            if(uiTempXDelt > 50)
//            {
//                vSendSingleOrder(OutDispLed);
//                ucErroType = ErroNoErro;
//                break;
//            }
//            g_uiTimeNumCont1ms = ErroRemoteAlarmFlashLedTimeMs;
//            while(g_uiTimeNumCont1ms);
//            vSendMultBytesOrder(Disp10Led, &ucWhenRomteErroWateSHowLedTab[uiTime][0], 2);
//        }
//        if(ucPowerKeyPressTimes >= 2)
//        {
//            break;
//        }

//    }
//    vSendSingleOrder(OutDispLed);
#ifdef CheckrRemoteContolorConnect
    EnAbleMlxTimerReadTnterrupt;
    g_uiTimeNumCont1ms = 80;
    while(g_uiTimeNumCont1ms);
    ucErroType = ErroNoRemote;
    uiTempYDelt = 0;
    while(ErroNoRemote == ucErroType)
    {
        for(uiTime = 0; uiTime < 10; uiTime ++)
        {
            uiTempXDelt = 0;
            while((strSysInfo.uiRemoteNowXPos > (strSysInfo.uiRemoteMiddleXPos - 2 * ClacXPwmFirstRemotePosP)) && (strSysInfo.uiRemoteNowXPos < (strSysInfo.uiRemoteMiddleXPos + 2 * ClacYPwmFirstRemotePosP))
                    && (strSysInfo.uiRemoteNowYPos > (strSysInfo.uiRemoteMiddleYPos - 2 * ClacYPwmFirstRemotePosP)) && (strSysInfo.uiRemoteNowYPos < (strSysInfo.uiRemoteMiddleYPos + 2 * ClacYPwmFirstRemotePosP)))
            {
                uiTempXDelt ++;
                g_uiTimeNumCont1ms = 5;
                while(g_uiTimeNumCont1ms);
                if(uiTempXDelt > 20)
                {
                    vSendSingleOrder(OutDispLed);
                    ucErroType = ErroNoErro;
                    break;
                }
            }
            //if(ReadKeyOfOn_OffFlag)uiTempXDelt = 100;
            if(uiTempXDelt > 20)
            {
                vSendSingleOrder(OutDispLed);
                ucErroType = ErroNoErro;
                break;
            }
            g_uiTimeNumCont1ms = ErroRemoteAlarmFlashLedTimeMs;
            while(g_uiTimeNumCont1ms);
            if(uiTempYDelt > 3)
						{
							vSendMultBytesOrder(Disp10Led, &ucWhenRomteErroWateSHowLedTab[uiTime][0], 2);
						}
            uiTempYDelt++;
            if(uiTempYDelt >= 150)
            {
                ucStsTemState = ErroMode;
                ucErroType = ErroNoRemote;
                vSendSingleOrder(OutDispLed);
                return;
            }
        }
        if(ucPowerKeyPressTimes >= 2)
        {
            break;
        }
    }
    DisEnAbleMlxTimerReadTnterrupt;
    vSendSingleOrder(OutDispLed);
#endif
    while(ReadKeyOfOn_OffFlag);//Waite for Power Key Press Up !
    if((ErroNoCheckSys == ucErroType) || (ErroNoErro == ucErroType))
    {
        ucTempArry[0] = LastErroSaveAtEepromAdd;
        ucTempArry[1] = ErroNoErro;
        vSendMultBytesOrder(WriteOneByte, ucTempArry, 2);
    }

    for(uiTime = 0; uiTime < 10; uiTime ++)
    {
        ucTag1ms = FALSE;
        while(FALSE == ucTag1ms);
        if(ucPowerKeyPressTimes >= 2 )
        {
            vAutoPowerOffTimeFlag();
        }
    }
    vSendSingleOrder(AskSpeedGrad);
}
/*
��ϵͳ�õ��Ĳ���������
*/

static void ADC1_Mode_Config(void)
{
    ADC_InitType ADC_InitStructure;
    /* ADC1 configuration */

    /* ʹ��ADC1ʱ�� */
    /* Configure ADCCLK such as ADCCLK = PCLK2/6 */
    RCC_ADCCLKConfig(RCC_APB2CLK_Div8);

    ADC_StructInit(&ADC_InitStructure);  //������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
    /* ADC1 configuration ------------------------------------------------------*/
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
    ADC_InitStructure.ADC_ScanMode = ENABLE;	        //ģ��ת��������ɨ��ģʽ
    ADC_InitStructure.ADC_ContinuousMode = ENABLE;	   //ģ��ת������������ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrig = ADC_ExternalTrig_None;	//�ⲿ����ת���ر�
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
    ADC_InitStructure.ADC_NumOfChannel = TotalADCChannels;	//˳����й���ת����ADCͨ������Ŀ
    ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���

    /* ADC1 regular channel11 configuration */
    //����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
    //ADC1,ADCͨ��x,�������˳��ֵΪy,����ʱ��Ϊ12.5����

    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_13_5 );//PA1 ң��Yλ��
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_13_5 );//PA2  ң��Xλ��
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 3, ADC_SampleTime_13_5 );//PB0 ���1�ҵ�ѹ���
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_13_5 );//PA4  ���1���ѹ���
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 5, ADC_SampleTime_13_5 );//PA6 ϵͳ�������
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 6, ADC_SampleTime_13_5 );//PA7���2���ѹ���
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 7, ADC_SampleTime_13_5 );//PB0 ɲ������ѹ���� --> ɲ������ѹ������ADC�� ֱ�Ӷ�ȡPB8,PB9��ѹ
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 8, ADC_SampleTime_13_5 );//PB1 ���2�ҵ�ѹ���
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 9, ADC_SampleTime_13_5 );//PA0  ���1�ĵ������ 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 10, ADC_SampleTime_13_5 );//PA5 �ҵ���������

    // ����ADC��DMA֧�֣�Ҫʵ��DMA���ܣ������������DMAͨ���Ȳ�����
    ADC_DMACtrl(ADC1, ENABLE);
    /* Enable ADC1 */
    ADC_Ctrl(ADC1, ENABLE);	   //ʹ��ָ����ADC1
    /* Enable ADC1 reset calibaration register */
    ADC_RstCalibration(ADC1);	  //��λָ����ADC1��У׼�Ĵ���
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));	//��ȡADC1��λУ׼�Ĵ�����״̬,����״̬��ȴ�
    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADC1);		//��ʼָ��ADC1��У׼״̬
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));		//��ȡָ��ADC1��У׼����,����״̬��ȴ�
}

void DMA_Configuration(void)
{
    DMA_InitType DMA_InitStructure;
    /* DMA clock enable */

    /* USART1_Tx_DMA_Channel (triggered by USART1 Tx event) Config */
    DMA_Reset(DMA1_Channel1); //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
    DMA_DefaultInitParaConfig(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->RDOR;  //DMA����ADC����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)vDmaContAdcVaule;       //DMA�ڴ����ַ
    DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALSRC;                  /* ���䷽��������Ϊ���ݴ����Ŀ�ĵ� */
    DMA_InitStructure.DMA_BufferSize = OneChannelConvertTimes * TotalADCChannels;   /* ���ݴ�С */
    DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;          /* �����ַ�Ĵ������� */
    DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;                   /* �ڴ��ַ�Ĵ������� */
    DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_HALFWORD; /* �������ݿ��Ϊ16λ */
    DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_HALFWORD;         /* �洢�����ݿ��Ϊ16λ */
    DMA_InitStructure.DMA_Mode = DMA_MODE_CIRCULAR;                             //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;                   /* �ǳ������ȼ� */
    DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;                        /* ���ڴ浽�ڴ� */
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    /* Enable USART1 DMA TX Channel */
    DMA_ChannelEnable(DMA1_Channel1, ENABLE);

}


/*
*********************************************************************************************************
*	�� �� ��: SetBaudrate
*	����˵��: ���ô��ڲ�����
*	��    ��: USARTx.���� Value.������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void SetBaudrate(unsigned short usBaudRate)
{
    USART_InitType USART_InitStructure = {0};
    GPIO_InitType GPIO_InitStructure = {0};
    NVIC_InitType NVIC_InitStructure = {0};
    DMA_InitType DMA_InitStructure = {0};

#ifdef  UsedUartID_UART2//���ʹ��UART2
    /* Enable USART2 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_USART2, ENABLE);//�������ڶ�ʱ��
    /* Connect PXx to USART2_Tx */
    GPIO_PinAFConfig(GPIOA, GPIO_PinsSource2, GPIO_AF_1);
    /* Connect PXx to USART2_Rx */
    GPIO_PinAFConfig(GPIOA, GPIO_PinsSource3, GPIO_AF_1);
    /* Configure USART2 Tx/Rx */
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_2 | GPIO_Pins_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#else //�����л���UART1
    /* Enable USART1 Clock */
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_USART1, ENABLE);
    /* Connect PXx to USART1_Tx */
    GPIO_PinAFConfig(GPIOB, GPIO_PinsSource6, GPIO_AF_0);//Tx
    /* Connect PXx to USART1_Rx */
    GPIO_PinAFConfig(GPIOB, GPIO_PinsSource7, GPIO_AF_0);
    /* Configure USART1 Tx/Rx */
	
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_6 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_OD;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);			
#endif

    /* Enable the USART2 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = UsedUartIntDef;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* Clear USART2 Interrupt Flag */
    USART_ClearFlag(EVAL_COM, USART_FLAG_RDNE);
    /* Enable the USART2 Receive Interrupt */
    USART_INTConfig(EVAL_COM, USART_INT_RDNE, ENABLE);

    USART_InitStructure.USART_BaudRate = usBaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    /* Configure USARTx */
    USART_Init(EVAL_COM, &USART_InitStructure);
    /* Enable USARTx */
    USART_Cmd(EVAL_COM, ENABLE);

//    bsp_DelayMS(5);
    /* Enable USART2 DMA TX request */
    USART_DMACmd(EVAL_COM, USART_DMAReq_Tx, ENABLE);

    /* USART2_Tx_DMA_Channel (triggered by USART2 Tx event) Config */
    DMA_Reset(UsedUartMdaChannle);
    DMA_DefaultInitParaConfig(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&EVAL_COM->DT;         /* DMA�������ַ */
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ucaUsarDmaTxdBuffer;       /* DMA�ڴ����ַ */
    DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALDST;                  /* ���䷽��������Ϊ���ݴ����Ŀ�ĵ� */
    DMA_InitStructure.DMA_BufferSize = 80;                             /* ���ݴ�С */
    DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;          /* �����ַ�Ĵ������� */
    DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;                   /* �ڴ��ַ�Ĵ������� */
    DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_BYTE; /* �������ݿ��Ϊ8λ */
    DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_BYTE;         /* �洢�����ݿ��Ϊ8λ */
    DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;                             /* ��ѭ��ģʽ */
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;                   /* �ǳ������ȼ� */
    DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;                        /* ���ڴ浽�ڴ� */
    DMA_Init(UsedUartMdaChannle, &DMA_InitStructure);

    DMA_ChannelEnable(UsedUartMdaChannle, ENABLE);

    /* Enable USART2 DMA TX request */
    USART_DMACmd(EVAL_COM, USART_DMAReq_Tx, ENABLE);//ucUartRxData

//    bsp_DelayMS(5);
#ifdef UsedUartDmaRxData
    /* USART2_Rx_DMA_Channel (triggered by USART2 Tx event) Config */
    DMA_Reset(DMA1_Channel5);
    DMA_DefaultInitParaConfig(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DT;         /* DMA�������ַ */
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ucUartDmaRxData;       /* DMA�ڴ����ַ */
    DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALSRC;              /* ���䷽��������Ϊ���ݴ����Ŀ�ĵ� */
    DMA_InitStructure.DMA_BufferSize = RecDmaBufferSize;                     /* ���ݴ�С */
    DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;          /* �����ַ�Ĵ������� */
    DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;                   /* �ڴ��ַ�Ĵ������� */
    DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_BYTE; /* �������ݿ��Ϊ8λ */
    DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_BYTE;         /* �洢�����ݿ��Ϊ8λ */
    DMA_InitStructure.DMA_Mode = DMA_MODE_CIRCULAR;                             /* ��ѭ��ģʽ */
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;                   /* �ǳ������ȼ� */
    DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;                        /* ���ڴ浽�ڴ� */
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);
    USART_DMACmd(EVAL_COM, USART_DMAReq_Rx, ENABLE);
    DMA_ChannelEnable(DMA1_Channel5, ENABLE);
    /* Enable USART2 DMA RX request */
    USART_DMACmd(EVAL_COM, USART_DMAReq_Rx, ENABLE);//ucUartRxData

    /* Clear USART2 Interrupt Flag */
    USART_ClearFlag(EVAL_COM, USART_FLAG_IDLEF);
    /* Enable the USART2 Receive Interrupt */
    USART_INTConfig(EVAL_COM, USART_INT_IDLEF, ENABLE);
#endif
}

void PWM_TIM15_Init(void)
{
    GPIO_InitType			GPIO_InitStructure = {0};
    TMR_TimerBaseInitType	TIM_TimeBaseStructure= {0};
    TMR_OCInitType			TIM_OCInitStructure = {0};

    GPIO_PinAFConfig(GPIOB, GPIO_PinsSource14, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinsSource15, GPIO_AF_1);
    //����GPIO�ܽŲ���
    GPIO_InitStructure.GPIO_Pins =  GPIO_Pins_14  | GPIO_Pins_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);

    TMR_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TMR_Period = 3600;					//�Զ���װ��ֵ
    TIM_TimeBaseStructure.TMR_DIV = 0;					//Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TMR_ClockDivision = 0; 					//ʱ�ӷָ�
    TIM_TimeBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;		//���ϼ���ģʽ
    TIM_TimeBaseStructure.TMR_RepetitionCounter = 0;
    TMR_TimeBaseInit(TMR15, &TIM_TimeBaseStructure); 				//��ʼ��TIM15ʱ��

    TMR_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TMR_OCMode = TMR_OCMode_PWM1;				//TIM�������ģʽ     //!!!!!!5line ʹ�����߽ӷ������Ը������ӷ���תPWM����
    TIM_OCInitStructure.TMR_OutputState = TMR_OutputState_Enable;	//�Ƚ����ʹ��
    TIM_OCInitStructure.TMR_Pulse =  0;								//��װ�벶��ȽϼĴ�������ֵ
    TIM_OCInitStructure.TMR_OCPolarity = TMR_OCPolarity_High;		//�������
    TIM_OCInitStructure.TMR_OCIdleState = TMR_OCIdleState_Set;

    TMR_OC1Init(TMR15, &TIM_OCInitStructure);
    TMR_OC2Init(TMR15, &TIM_OCInitStructure);
    TMR_Cmd(TMR15, ENABLE);
    TMR_CtrlPWMOutputs(TMR15, ENABLE);
}


void PWM_TIM3_Init(void)
{
    GPIO_InitType			GPIO_InitStructure = {0};
    TMR_TimerBaseInitType	TIM_TimeBaseStructure = {0};
    TMR_OCInitType			TIM_OCInitStructure = {0};

    GPIO_PinAFConfig(GPIOB, GPIO_PinsSource4, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinsSource5, GPIO_AF_1);
//		bsp_DelayMS(15);
    //����GPIO�ܽŲ���
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_4 | GPIO_Pins_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);

    TMR_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TMR_Period = 3600;					//�Զ���װ��ֵ
    TIM_TimeBaseStructure.TMR_DIV = 0;					//Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TMR_ClockDivision = 0; 					//ʱ�ӷָ�
    TIM_TimeBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;		//���ϼ���ģʽ
    TIM_TimeBaseStructure.TMR_RepetitionCounter = 0;
    TMR_TimeBaseInit(TMR3, &TIM_TimeBaseStructure); 				//��ʼ��TIM15ʱ��

    TMR_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TMR_OCMode = TMR_OCMode_PWM1;				//TIM�������ģʽ
    TIM_OCInitStructure.TMR_OutputState = TMR_OutputState_Enable;	//�Ƚ����ʹ��
    TIM_OCInitStructure.TMR_Pulse = 0 ;								//��װ�벶��ȽϼĴ�������ֵ
    TIM_OCInitStructure.TMR_OCPolarity = TMR_OCPolarity_High;		//�������
    TIM_OCInitStructure.TMR_OCIdleState = TMR_OCIdleState_Set;

    TMR_OC1Init(TMR3, &TIM_OCInitStructure);
    TMR_OC2Init(TMR3, &TIM_OCInitStructure);
    TMR_Cmd(TMR3, ENABLE);
}

void PWM_TIM1_Init(void)
{
    GPIO_InitType			GPIO_InitStructure = {0};
    TMR_TimerBaseInitType	TIM_TimeBaseStructure = {0};
    TMR_OCInitType			TIM_OCInitStructure = {0};

    GPIO_PinAFConfig(GPIOA, GPIO_PinsSource8, GPIO_AF_2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinsSource9, GPIO_AF_2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinsSource10, GPIO_AF_2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinsSource11, GPIO_AF_2);
    //����GPIO�ܽŲ���
    GPIO_InitStructure.GPIO_Pins =  GPIO_Pins_8 | GPIO_Pins_9 | GPIO_Pins_10 | GPIO_Pins_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;//�������PWM

    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TMR_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TMR_Period = 3600;					//�Զ���װ��ֵ
    TIM_TimeBaseStructure.TMR_DIV = 0;					//Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TMR_ClockDivision = 0; 					//ʱ�ӷָ�
    TIM_TimeBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;		//���ϼ���ģʽ
    TIM_TimeBaseStructure.TMR_RepetitionCounter = 0;
    TMR_TimeBaseInit(TMR1, &TIM_TimeBaseStructure); 				//��ʼ��TIM15ʱ��

    TMR_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TMR_OCMode = TMR_OCMode_PWM1;				//TIM�������ģʽ     //!!!!!!5line ʹ�����߽ӷ������Ը������ӷ���תPWM����
    TIM_OCInitStructure.TMR_OutputState = TMR_OutputState_Enable;	//�Ƚ����ʹ��
    TIM_OCInitStructure.TMR_Pulse =  0;								//��װ�벶��ȽϼĴ�������ֵ
    TIM_OCInitStructure.TMR_OCPolarity = TMR_OCPolarity_High;		//�������
    TIM_OCInitStructure.TMR_OCIdleState = TMR_OCIdleState_Set;

    TMR_OC1Init(TMR1, &TIM_OCInitStructure);
    TMR_OC2Init(TMR1, &TIM_OCInitStructure);
    TMR_OC3Init(TMR1, &TIM_OCInitStructure);
    TMR_OC4Init(TMR1, &TIM_OCInitStructure);
    TMR_Cmd(TMR1, ENABLE);
    TMR_CtrlPWMOutputs(TMR1, ENABLE);
}

#ifdef UsedMlx90393
void vTime14Mxl90393(void)
{
    TMR_TimerBaseInitType TMR_TMReBaseStructure;
    NVIC_InitType NVIC_InitStructure;

//    bsp_DelayMS(5);

    /* TMR6 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR14, ENABLE);

    /* TMR6 base configuration */
    TMR_TimeBaseStructInit(&TMR_TMReBaseStructure);
    TMR_TMReBaseStructure.TMR_Period = 2000;//500usһ���ж�
    TMR_TMReBaseStructure.TMR_DIV = 0;
    TMR_TMReBaseStructure.TMR_ClockDivision = 0;
    TMR_TMReBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;
    TMR_TimeBaseInit(TMR14, &TMR_TMReBaseStructure);

    /* Prescaler configuration */
    TMR_DIVConfig(TMR14, 0, TMR_DIVReloadMode_Immediate);

    /* TMR IT enable */
    TMR_INTConfig(TMR14, TMR_INT_Overflow, ENABLE);

    /* TMR16 enable counter */
    TMR_Cmd(TMR14, DISABLE);

    /* Enable the TMR6 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TMR14_GLOBAL_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
//    bsp_DelayMS(5);
}
#endif

void vTimeBaseInit(void)
{
    TMR_TimerBaseInitType TMR_TMReBaseStructure;
    NVIC_InitType NVIC_InitStructure;

    /* TMR6 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR6, ENABLE);
    /* Compute the prescaler value */

    /* TMR6 base configuration */
    TMR_TimeBaseStructInit(&TMR_TMReBaseStructure);
    TMR_TMReBaseStructure.TMR_Period = 60000;//10msһ���ж�
    TMR_TMReBaseStructure.TMR_DIV = 0;
    TMR_TMReBaseStructure.TMR_ClockDivision = 0;
    TMR_TMReBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;
    TMR_TimeBaseInit(TMR6, &TMR_TMReBaseStructure);

    /* Prescaler configuration */
    TMR_DIVConfig(TMR6, 11, TMR_DIVReloadMode_Immediate);//72mHz  72/(11+1) = 6mHz

    /* TMR IT enable */
    TMR_INTConfig(TMR6, TMR_INT_Overflow, ENABLE);

    /* TMR6 enable counter */
    TMR_Cmd(TMR6, ENABLE);

    /* Enable the TMR6 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TMR6_GLOBAL_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);

//    bsp_DelayMS(5);

    /* TMR6 clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_TMR16, ENABLE);
    /* Compute the prescaler value
       TMR6 counter clock at 12 MHz */

    /* TMR6 base configuration */
    TMR_TimeBaseStructInit(&TMR_TMReBaseStructure);
    TMR_TMReBaseStructure.TMR_Period = 6000;//500usһ���ж�
    TMR_TMReBaseStructure.TMR_DIV = 0;
    TMR_TMReBaseStructure.TMR_ClockDivision = 0;
    TMR_TMReBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;
    TMR_TimeBaseInit(TMR16, &TMR_TMReBaseStructure);

    /* Prescaler configuration */
    TMR_DIVConfig(TMR16, 5, TMR_DIVReloadMode_Immediate);

    /* TMR IT enable */
    TMR_INTConfig(TMR16, TMR_INT_Overflow, ENABLE);

    /* TMR16 enable counter */
    TMR_Cmd(TMR16, ENABLE);

    /* Enable the TMR6 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TMR16_GLOBAL_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

void vInitTimer(void)//PWM��ʱ��
{
    PWM_TIM1_Init();
//    bsp_DelayMS(15);
    PWM_TIM3_Init();
//    bsp_DelayMS(15);
    PWM_TIM15_Init();
 //   bsp_DelayMS(15);
    vTimeBaseInit();
}


void vSetTeacherPortInt(void)
{
    EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure = {0};
    SYSCFG_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinsSource12); ///<Connect EXTI12 Line to PC13 pin
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line12;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     /* �ж�ģʽ */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; /* �½��ش��� */
    EXTI_InitStructure.EXTI_LineEnable = ENABLE;
    EXTI_Init(&EXTI_InitStructure); ///<Configure EXTI13 line

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); ///<Enable and set EXTI13 Interrupt to the lowest priority
}



void vShowErroToDis(unsigned char ucErroNum)
{
    unsigned char ucDataBuf[5] = {0};
    unsigned int uiTimeCont = 0;
    unsigned char ucShowErro = 0;

    if(ucErroNum)ucErroType = ucErroNum;
    CtlDriverPowerContolOFF;
    CtlBrakeBikeDisEn;
    CtlPowerOnTest_OFF;
    CtrlExtSpeekerDis;
    if((ErroLowVoltage == ucErroType) || (ErroOverVoltage == ucErroType))
    {
        ucShowErro = 0x01;
    }
    else if((ErroMoto1HighBriageBreak == ucErroType) || (ErroMoto1HighBriageShort == ucErroType))
    {
        ucShowErro = 0x02;
    }
    else if((ErroMoto2HighBriageBreak == ucErroType) || (ErroMoto2HighBriageShort == ucErroType))
    {
        ucShowErro = 0x04;
    }
    else if((ErroMoto1Break == ucErroType) || (ErroNoBearker == ucErroType))
    {
        ucShowErro = 0x03;
    }
    else if(ErroMoto2Break == ucErroType)
    {
        ucShowErro = 0x05;
    }
    else if((ErroRelayShort == ucErroType) || (ErroRelayBreak == ucErroType))
    {
        ucShowErro = 0x08;
    }
    else if(ErroNoRemote == ucErroType)
    {
        ucShowErro = 0x07;
    }
    else if(ErroNoCheckSys != ucErroType)
    {
        ucDataBuf[0] = LastErroSaveAtEepromAdd;
        ucDataBuf[1] = 11;
    }
    ucDataBuf[0] = LastErroSaveAtEepromAdd;
    if((0x0F & ucLastErro) == ucShowErro) //����ϴεĴ����뱾�δ���һ��  ��¼�ϴδ���Ĵ���
    {
        if(0x0F != (ucLastErro >> 4))//����֡�ĸ����ֽ����ڷ�  ����Ĵ���
        {
            ucDataBuf[1] = (ucLastErro >> 4) + 1;//�Դ���������ۼ�
            ucDataBuf[1] <<= 4;
        }
        ucDataBuf[1] |= ucShowErro;
    }
    else ucDataBuf[1] = ucShowErro;
    vSendMultBytesOrder(WriteOneByte, ucDataBuf, 2);

		for(uiTimeCont = 0; uiTimeCont < 5; uiTimeCont++)//�����ӳ�ʱ��
		{
			   ucTag100ms = FALSE;
				while(FALSE == ucTag100ms);
				vSendOneByteOrder(OrderErro, ucShowErro);
		}

    ucShowErro = FALSE;
    for(uiTimeCont = 0; uiTimeCont < 40000; uiTimeCont++)//�����ӳ�ʱ��
    {
        ucTag1ms = FALSE;
        while(FALSE == ucTag1ms);
			
			  //����� Ӳ������ģ�飩�쳣
			  if( ErroNoCheckSys != ucErroType || ErroNoErro != ucErroType)
				{
					 //��� Ӳ������ģ�飩�쳣�Ƿ�ָ�
				   vCheckSystemInfo(DISABLE);//ȥ����ģ���Ƿ������ش���
          
				}
			
        if(ReadKeyOfOn_OffFlag || ucAnlyFree)//����е�Դ����������ʾ��ͨ��
        {
            if(FALSE == ucShowErro) vSendSingleOrder(QuitErro);//ֻ�˳�һ�α���
            ucShowErro = TRUE;//����´λ���ʾ�����ݡ��Ͳ�ȥ������
            if(ucAnlyFree)//�����ʾ������
            {
                uiTimeCont = 0;//�����ʱ����
                vAnalyTeacherDataOfTime();//�����ʾ������������������ʱ��Ҫȥ������
            }
            if(ReadKeyOfOn_OffFlag) break;
        }
        if(ReadKeyOfOn_OffFlag) break;
        if(ucPowerKeyPressTimes >= 2)
        {
            break;
        }
				if(ErroNoErro == ucErroType)
        {
					break;
				}
    }
    vSendSingleOrder(QuitErro);
    if(ErroNoErro != ucErroType  && ErroNoCheckSys != ucErroType )
    {
			 //����������������쳣�͹ػ�
		   vAutoPowerOffTimeFlag();
		}
}
void vReadEepromData(void)
{
    union UINT unuiData = {0};
    unsigned char ucTemp = 0, ucTimeCont = 0;
    unsigned char ucCrcTime = 0, ucAutoSelfConfig = FALSE;
    for(ucCrcTime = 0; ucCrcTime < 100; ucCrcTime++)//20151102 ǰ for(ucCrcTime = 0; ucCrcTime < 100; ucCrcTime++)
    {
        ucRec60Bytes[5] = ucRec60Bytes[58] = ucRec60Bytes[59] = 0x00;
        while(ucTemp < 70)//20151102 ǰ  while(ucTemp < 80)
        {
            ucTemp++;
            unuiData.ucData.ucData1 = ucRec60Bytes[58];
            unuiData.ucData.ucData2 = ucRec60Bytes[59];//���ռ����
            for(ucTimeCont = 0; ucTimeCont < 10; ucTimeCont++) //
            {
                ucTag1ms = FALSE;
                while(FALSE == ucTag1ms);
                //                if(ReadKeyOfOn_OffFlag)//20190102
                //                {
                //                    vAutoPowerOffTimeFlag();//�����������Ӧ����
                //                }
            }
            if(ucPowerKeyPressTimes >= 2)
            {
                vAutoPowerOffTimeFlag();
            }
            if(WriteEepromData == (unuiData.uiData & WriteEepromData))//�����ȡ���ؼ���
            {
                ucTemp = 211;
                ucCrcTime = 211;
                if(ucAutoSelfConfig)//˵�����Զ���д�¼���
                {
                    for(unuiData.ucData.ucData1 = 0; unuiData.ucData.ucData1 < DirverAttLength; unuiData.ucData.ucData1++)//�ٴμ���д����Ƿ�һ����
                    {
                        if(uniDiverInfo.ucDirverAttDataBuff[unuiData.ucData.ucData1] != ucRec60Bytes[unuiData.ucData.ucData1]) //���û�ж��Ķ�
                        {
                            ucTemp = 0;//Ҫ����ȥ������д
                            ucCrcTime = 0;
                            break;
                        }
                    }
                }
                break;
            }
            else
            {
                vSendSingleOrder(Read60Bytes);
            }
            if(211 == ucTemp)break;
        }
        if(211 != ucTemp) //�����Ƭ���ڵ�һ���ϵ磬ǿ��д���ʼֵ
        {
            ucAutoSelfConfig = TRUE;
            //uniDiverInfo.strDirverAtt.ucEnIternSpeaker  = TRUE;
            uniDiverInfo.strDirverAtt.ucEnIternSpeaker  = 0xEF;
            uniDiverInfo.strDirverAtt.ucExternSpeakerVolume = 10;
            uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor1DirEn = FALSE;
            uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor2DirEn = FALSE;
            uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor1Motor2 = FALSE;
            uniDiverInfo.strDirverAtt.unMotorInfo.bits.bCheckMotorConnect = TRUE;//
            uniDiverInfo.strDirverAtt.unMotorInfo.bits.bCheckBreakConnect = FALSE;
            uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn = FALSE;
            uniDiverInfo.strDirverAtt.unMotorInfo.bits.bLilonOrQiansuan = FALSE;//Ĭ����Ǧ����
            uniDiverInfo.strDirverAtt.ucFirstLineRatePare = 5;//��һ��ɲ�����ߵ�����ֵ
            uniDiverInfo.strDirverAtt.ucFistLineTurnSecondPos = 6;//��һ��ɲ��ת�����ڶ���ɲ���Ĺյ�ֵ
            uniDiverInfo.strDirverAtt.ucBreakAccPare = 6;//ɲ�����ļ��ٶ�
            uniDiverInfo.strDirverAtt.ucForwardSpeedP = 87;//ǰ���ٶȱ���ϵ��
            uniDiverInfo.strDirverAtt.ucBackSpeedP = 26;//�����ٶȱ���ϵ��
            uniDiverInfo.strDirverAtt.ucDriverAcc = 6;//ǰ�����˼��ٶ�
            uniDiverInfo.strDirverAtt.ucSwerveAcc = 6;//ת����ٶ�
            uniDiverInfo.strDirverAtt.ucStarMinPwm = 8;//������СPWM
            uniDiverInfo.strDirverAtt.ucStopMinPwm = 8;//ֹͣ��СPWM
            uniDiverInfo.strDirverAtt.ucLetRightMotorP = 5;//���ҵ������ϵ��
            uniDiverInfo.strDirverAtt.ucLetRightMotorTiedP = 6;//���ҵ����ϵ��
            uniDiverInfo.strDirverAtt.ucMaxDriverCurrent = 20;//�����������
            uniDiverInfo.strDirverAtt.ucFactryIdData1 = DefaultFactryID & 0xFF;
            uniDiverInfo.strDirverAtt.ucFactryIdData1 = (DefaultFactryID >> 8) & 0xFF;
            uniDiverInfo.strDirverAtt.ucFactryIdData1 = (DefaultFactryID >> 16) & 0xFF;
            uniDiverInfo.strDirverAtt.ucFactryIdData1 = (DefaultFactryID >> 24) & 0xFF;
            uniDiverInfo.strDirverAtt.ucForwardMinSpeedP = 40;//ǰ����С�ٶ�
            uniDiverInfo.strDirverAtt.ucBackMinSpeedP = 21;	//������С�ٶ�
            uniDiverInfo.strDirverAtt.ucSwerveMaxSpeedP = 26;		//ת������ٶ�
            uniDiverInfo.strDirverAtt.ucSwerveMinSpeedP = 15;//ת����С�ٶ�
            uniDiverInfo.strDirverAtt.unDelayBreakActTime.all = 550;//Ĭ����400msɲ����ʱ
            for(ucTemp = 0; ucTemp < DirverAttLength; ucTemp++)ucRec60Bytes[ucTemp] = uniDiverInfo.ucDirverAttDataBuff[ucTemp];
            ucRec60Bytes[58] = 0xFF & WriteEepromData;
            ucRec60Bytes[59] = 0xFF & (WriteEepromData >> 8); //д�뱻ռ�ñ�־
            vSendMultBytesOrder(Write60Bytes, ucRec60Bytes, 60);
            ucRec60Bytes[58] = ucRec60Bytes[59] = 0x00;
        }
    }
    if(211 == ucTemp)//�������ĵ�ȷ����
    {
        for(unuiData.uiData = 0; unuiData.uiData < DirverAttLength; unuiData.uiData++)
        {
            uniDiverInfo.ucDirverAttDataBuff[unuiData.uiData] = ucRec60Bytes[unuiData.uiData];
        }
    }
}

//#define UseSoftSimulate
void vReadSysTempPare(unsigned char ucReadEepromFlag)//��ȡϵͳ�еĲ���
{
#define M1AM2A_MaxAsPwm  7500//�������Ϊ����൱��ʱ�����PWMֵ
    unsigned int ucPosAngleCool[16] = {0, 4, 15, 60, 86, 94, 120, 165, 176, 184, 225, 266, 274, 315, 356, 360};//��������
    float fPostivePwm;
    float fKx1, fKx2; //б��
    int iTemp = 0;
    unsigned int uiTemp, uiTemp2;
    unsigned int uiBackSpeedP;
    unsigned int uiSwerveMaxSpeed;//�����ת�������ٶ�
    uiMotor1LeftHighPwmRegAdd = Time1Ccr1Add;
    uiMotor1RightHighPwmRegAdd = Time1Ccr3Add;
    uiMotor2LeftHighPwmRegAdd = Time4Ccr1Add;
    uiMotor2RightHighPwmRegAdd = Time4Ccr3Add;
#ifndef UseSoftSimulate
    ucRecFeedBackFlag = FALSE;
    ucSpeedGrad = 0;
    vSendSingleOrder(AskSpeedGrad);
    for(uiTemp = 0; uiTemp < 10; uiTemp++)
    {
        ucTag10ms = FALSE;
        while(FALSE == ucTag10ms);
        vSendSingleOrder(AskSpeedGrad);
        if(ucSpeedGrad && ucRecFeedBackFlag)break;
    }
    if(uiTemp > 5)
    {
        ucErroType = ErroSysPowerOff;
        ucStsTemState = ErroMode;
        return;
    }
    vSendSingleOrder(Read60Bytes);
    if(TRUE == ucReadEepromFlag)//ֻ��ȡһ��
    {
        vReadEepromData();
    }
#endif
		if(FALSE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//�����פ���Ϳ�������ǿ�Ʋ����ɲ����
		{
			 uniDiverInfo.strDirverAtt.unMotorInfo.bits.bCheckBreakConnect = FALSE;
		}
			
    fPostivePwm = uniDiverInfo.strDirverAtt.ucBackSpeedP * 0.01;
    uiBackSpeedP =  (unsigned int)(fPostivePwm * M1AM2A_MaxAsPwm);
    fPostivePwm = uniDiverInfo.strDirverAtt.ucSwerveMaxSpeedP * 0.01;
    uiSwerveMaxSpeed =  (unsigned int)(fPostivePwm * M1AM2A_MaxAsPwm);

    fKx1 = 0;
    fKx2 = 0;
    for(uiTemp = ucPosAngleCool[0]; uiTemp < ucPosAngleCool[1] ; uiTemp++)
    {
        //0 - 4
        FourQuadrantPosAsPwmArryMoto1_A[uiTemp] = uiSwerveMaxSpeed;
        FourQuadrantPosAsPwmArryMoto1_B[uiTemp] = 0;
        FourQuadrantPosAsPwmArryMoto2_A[uiTemp] = 0;
        FourQuadrantPosAsPwmArryMoto2_B[uiTemp] = uiSwerveMaxSpeed;
    }            //Y2              Y1                   X2 60��             X1 4��
    fKx1 = ((M1AM2A_MaxAsPwm - uiSwerveMaxSpeed) * 1.0) / (ucPosAngleCool[3] - ucPosAngleCool[1]);
    fKx2 = (((signed int)(0 - uiSwerveMaxSpeed)) * 1.0) / (ucPosAngleCool[2] - ucPosAngleCool[1]); //���Kx  4 //�� 15
    for(uiTemp = ucPosAngleCool[1]; uiTemp < ucPosAngleCool[2] ; uiTemp++)
    {
        //4 - 15
        iTemp = (uiTemp + 1) * fKx1 - ucPosAngleCool[1] * fKx1 + uiSwerveMaxSpeed; //y = {(y2-y1)/(x2-x1)}*(X-x1)+y1
        if(iTemp < 0) iTemp = 0;
        if(iTemp > M1AM2A_MaxAsPwm) iTemp = M1AM2A_MaxAsPwm;
        FourQuadrantPosAsPwmArryMoto1_A[uiTemp] = iTemp;
        FourQuadrantPosAsPwmArryMoto1_B[uiTemp] = 0;
        FourQuadrantPosAsPwmArryMoto2_A[uiTemp] = 0;
        iTemp = (uiTemp + 1) * fKx2 - ucPosAngleCool[1] * fKx2 + uiSwerveMaxSpeed; //y = {(y2-y1)/(x2-x1)}*(X-x1)+y1
        if(iTemp < 0) iTemp = 0;
        if(iTemp > M1AM2A_MaxAsPwm) iTemp = M1AM2A_MaxAsPwm;
        FourQuadrantPosAsPwmArryMoto2_B[uiTemp] = iTemp;
    }
    fKx1 = ((M1AM2A_MaxAsPwm - uiSwerveMaxSpeed) * 1.0) / (ucPosAngleCool[3] - ucPosAngleCool[1]);
    fKx2 = ((M1AM2A_MaxAsPwm - 0) * 1.0) / (ucPosAngleCool[4] - ucPosAngleCool[2]); //���kd 86 15
    for(uiTemp = ucPosAngleCool[2]; uiTemp < ucPosAngleCool[3] ; uiTemp++)
    {
        //15 - 60
        iTemp = (uiTemp + 1) * fKx1 - ucPosAngleCool[1] * fKx1 + uiSwerveMaxSpeed; //y = {(y2-y1)/(x2-x1)}*(X-x1)+y1
        if(iTemp < 0) iTemp = 0;
        if(iTemp > M1AM2A_MaxAsPwm) iTemp = M1AM2A_MaxAsPwm;
        FourQuadrantPosAsPwmArryMoto1_A[uiTemp] = iTemp;
        FourQuadrantPosAsPwmArryMoto1_B[uiTemp] = 0;
        iTemp = (uiTemp + 1) * fKx2 - ucPosAngleCool[2] * fKx2 + 0; //y = {(y2-y1)/(x2-x1)}*(X-x1)+y1
        if(iTemp < 0) iTemp = 0;
        if(iTemp > M1AM2A_MaxAsPwm) iTemp = M1AM2A_MaxAsPwm;
        FourQuadrantPosAsPwmArryMoto2_A[uiTemp] = iTemp;
        FourQuadrantPosAsPwmArryMoto2_B[uiTemp] = 0;
    }
    fKx1 = 0;
    fKx2 = ((M1AM2A_MaxAsPwm - 0) * 1.0) / (ucPosAngleCool[4] - ucPosAngleCool[2]); //���kd 86 15
    for(uiTemp = ucPosAngleCool[3]; uiTemp < ucPosAngleCool[4] ; uiTemp++)
    {
        //60 - 86
        FourQuadrantPosAsPwmArryMoto1_A[uiTemp] = M1AM2A_MaxAsPwm;
        FourQuadrantPosAsPwmArryMoto1_B[uiTemp] = 0;
        iTemp = (uiTemp + 1) * fKx2 - ucPosAngleCool[2] * fKx2 + 0; //y = {(y2-y1)/(x2-x1)}*(X-x1)+y1
        if(iTemp < 0) iTemp = 0;
        if(iTemp > M1AM2A_MaxAsPwm) iTemp = M1AM2A_MaxAsPwm;
        FourQuadrantPosAsPwmArryMoto2_A[uiTemp] = iTemp;
        FourQuadrantPosAsPwmArryMoto2_B[uiTemp] = 0;
    }
    fKx1 = 0;
    fKx2 = 0;
    for(uiTemp = ucPosAngleCool[4]; uiTemp < ucPosAngleCool[5] ; uiTemp++)
    {
        //86 - 94
        FourQuadrantPosAsPwmArryMoto1_A[uiTemp] = M1AM2A_MaxAsPwm;
        FourQuadrantPosAsPwmArryMoto1_B[uiTemp] = 0;
        FourQuadrantPosAsPwmArryMoto2_A[uiTemp] = M1AM2A_MaxAsPwm;
        FourQuadrantPosAsPwmArryMoto2_B[uiTemp] = 0;
    }
    fKx1 = ((0 - M1AM2A_MaxAsPwm ) * 1.0) / (ucPosAngleCool[7] - ucPosAngleCool[5]);
    fKx2 = 0;
    for(uiTemp = ucPosAngleCool[5]; uiTemp < ucPosAngleCool[6] ; uiTemp++)
    {
        //94 - 120
        iTemp = (uiTemp + 1) * fKx1 - ucPosAngleCool[7] * fKx1 + 0; //y = {(y2-y1)/(x2-x1)}*(X-x1)+y1
        if(iTemp < 0) iTemp = 0;
        if(iTemp > M1AM2A_MaxAsPwm) iTemp = M1AM2A_MaxAsPwm;
        FourQuadrantPosAsPwmArryMoto1_A[uiTemp] = iTemp;
        FourQuadrantPosAsPwmArryMoto1_B[uiTemp] = 0;
        FourQuadrantPosAsPwmArryMoto2_A[uiTemp] = M1AM2A_MaxAsPwm;
        FourQuadrantPosAsPwmArryMoto2_B[uiTemp] = 0;
    }
    fKx1 = ((0 - M1AM2A_MaxAsPwm ) * 1.0) / (ucPosAngleCool[7] - ucPosAngleCool[5]);
    fKx2 = (((signed int)(uiSwerveMaxSpeed - M1AM2A_MaxAsPwm )) * 1.0) / (ucPosAngleCool[8] - ucPosAngleCool[6]);
    for(uiTemp = ucPosAngleCool[6]; uiTemp < ucPosAngleCool[7] ; uiTemp++)
    {
        //120 - 165
        iTemp = (uiTemp + 1) * fKx1 - ucPosAngleCool[7] * fKx1 + 0; //y = {(y2-y1)/(x2-x1)}*(X-x1)+y1
        if(iTemp < 0) iTemp = 0;
        if(iTemp > M1AM2A_MaxAsPwm) iTemp = M1AM2A_MaxAsPwm;
        FourQuadrantPosAsPwmArryMoto1_A[uiTemp] = iTemp;
        FourQuadrantPosAsPwmArryMoto1_B[uiTemp] = 0;
        iTemp = (uiTemp + 1) * fKx2 - ucPosAngleCool[6] * fKx2 + M1AM2A_MaxAsPwm; //y = {(y2-y1)/(x2-x1)}*(X-x1)+y1
        if(iTemp < 0) iTemp = 0;
        if(iTemp > M1AM2A_MaxAsPwm) iTemp = M1AM2A_MaxAsPwm;
        FourQuadrantPosAsPwmArryMoto2_A[uiTemp] = iTemp;
        FourQuadrantPosAsPwmArryMoto2_B[uiTemp] = 0;
    }
    fKx1 = ((uiSwerveMaxSpeed - 0 ) * 1.0) / (ucPosAngleCool[8] - ucPosAngleCool[7]);
    fKx2 = (((signed int)(uiSwerveMaxSpeed - M1AM2A_MaxAsPwm )) * 1.0) / (ucPosAngleCool[8] - ucPosAngleCool[6]);
    for(uiTemp = ucPosAngleCool[7]; uiTemp < ucPosAngleCool[8] ; uiTemp++)
    {
        //165 - 176
        FourQuadrantPosAsPwmArryMoto1_A[uiTemp] = 0;
        iTemp = (uiTemp + 1) * fKx1 - ucPosAngleCool[7] * fKx1 + 0; //y = {(y2-y1)/(x2-x1)}*(X-x1)+y1
        if(iTemp < 0) iTemp = 0;
        if(iTemp > M1AM2A_MaxAsPwm) iTemp = M1AM2A_MaxAsPwm;
        FourQuadrantPosAsPwmArryMoto1_B[uiTemp] = iTemp;
        iTemp = (uiTemp + 1) * fKx2 - ucPosAngleCool[6] * fKx2 + M1AM2A_MaxAsPwm; //y = {(y2-y1)/(x2-x1)}*(X-x1)+y1
        if(iTemp < 0) iTemp = 0;
        if(iTemp > M1AM2A_MaxAsPwm) iTemp = M1AM2A_MaxAsPwm;
        FourQuadrantPosAsPwmArryMoto2_A[uiTemp] = iTemp;
        FourQuadrantPosAsPwmArryMoto2_B[uiTemp] = 0;
    }
    fKx1 = 0;
    fKx2 = 0;
    for(uiTemp = ucPosAngleCool[8]; uiTemp < ucPosAngleCool[9] ; uiTemp++)
    {
        //176 - 184
        FourQuadrantPosAsPwmArryMoto1_A[uiTemp] = 0;
        FourQuadrantPosAsPwmArryMoto1_B[uiTemp] = uiSwerveMaxSpeed;
        FourQuadrantPosAsPwmArryMoto2_A[uiTemp] = uiSwerveMaxSpeed;
        FourQuadrantPosAsPwmArryMoto2_B[uiTemp] = 0;
    }
    fKx1 = (((signed int)(uiBackSpeedP / 2.0 - uiSwerveMaxSpeed / 2.0)) * 1.0) / (ucPosAngleCool[10] - ucPosAngleCool[9]);
    fKx2 = (((signed int)(0 - uiSwerveMaxSpeed )) * 1.0) / (ucPosAngleCool[10] - ucPosAngleCool[9]);
    for(uiTemp = ucPosAngleCool[9]; uiTemp < ucPosAngleCool[10] ; uiTemp++)
    {
        //184 - 225
        FourQuadrantPosAsPwmArryMoto1_A[uiTemp] = 0;
        iTemp = (uiTemp + 1) * fKx1 - ucPosAngleCool[9] * fKx1 + uiSwerveMaxSpeed; //y = {(y2-y1)/(x2-x1)}*(X-x1)+y1
        if(iTemp < 0) iTemp = 0;
        if(iTemp > M1AM2A_MaxAsPwm) iTemp = M1AM2A_MaxAsPwm;
        uiTemp2 = FourQuadrantPosAsPwmArryMoto1_B[uiTemp] = iTemp;
        iTemp = (uiTemp + 1) * fKx2 - ucPosAngleCool[10] * fKx2 + 0; //y = {(y2-y1)/(x2-x1)}*(X-x1)+y1
        if(iTemp < 0) iTemp = 0;
        if(iTemp > M1AM2A_MaxAsPwm) iTemp = M1AM2A_MaxAsPwm;
        FourQuadrantPosAsPwmArryMoto2_A[uiTemp] = iTemp;
        FourQuadrantPosAsPwmArryMoto2_B[uiTemp] = 0;
    }
    fKx1 = (((signed int)(uiBackSpeedP / 2.0 - uiSwerveMaxSpeed / 2.0)) * 1.0) / (ucPosAngleCool[11] - ucPosAngleCool[10]);
    fKx2 = (((signed int)(uiBackSpeedP - 0 )) * 1.0) / (ucPosAngleCool[11] - ucPosAngleCool[10]);
    for(uiTemp = ucPosAngleCool[10]; uiTemp < ucPosAngleCool[11] ; uiTemp++)
    {
        //225 - 266
        FourQuadrantPosAsPwmArryMoto1_A[uiTemp] = 0;
        iTemp = (uiTemp + 1) * fKx1 - ucPosAngleCool[10] * fKx1 + uiTemp2; //y = {(y2-y1)/(x2-x1)}*(X-x1)+y1
        if(iTemp < 0) iTemp = 0;
        if(iTemp > M1AM2A_MaxAsPwm) iTemp = M1AM2A_MaxAsPwm;
        FourQuadrantPosAsPwmArryMoto1_B[uiTemp] = iTemp;
        FourQuadrantPosAsPwmArryMoto2_A[uiTemp] = 0;
        iTemp = (uiTemp + 1) * fKx2 - ucPosAngleCool[10] * fKx2 + 0; //y = {(y2-y1)/(x2-x1)}*(X-x1)+y1
        if(iTemp < 0) iTemp = 0;
        if(iTemp > M1AM2A_MaxAsPwm) iTemp = M1AM2A_MaxAsPwm;
        FourQuadrantPosAsPwmArryMoto2_B[uiTemp] = iTemp;
    }
    fKx1 = 0;
    fKx2 = 0;
    for(uiTemp = ucPosAngleCool[11]; uiTemp < ucPosAngleCool[12] ; uiTemp++)
    {
        //266 - 274
        FourQuadrantPosAsPwmArryMoto1_A[uiTemp] = 0;
        FourQuadrantPosAsPwmArryMoto1_B[uiTemp] = uiBackSpeedP;
        FourQuadrantPosAsPwmArryMoto2_A[uiTemp] = 0;
        FourQuadrantPosAsPwmArryMoto2_B[uiTemp] = uiBackSpeedP;
    }
    fKx1 = (((signed int)(0 - uiBackSpeedP) * 1.0)) / (ucPosAngleCool[13] - ucPosAngleCool[12]);
    fKx2 = (((signed int)(uiSwerveMaxSpeed / 2.0 - uiBackSpeedP / 2.0)) * 1.0) / (ucPosAngleCool[13] - ucPosAngleCool[12]);
    for(uiTemp = ucPosAngleCool[12]; uiTemp < ucPosAngleCool[13] ; uiTemp++)
    {
        //274 - 315
        FourQuadrantPosAsPwmArryMoto1_A[uiTemp] = 0;
        iTemp = (uiTemp + 1) * fKx1 - ucPosAngleCool[13] * fKx1 + 0; //y = {(y2-y1)/(x2-x1)}*(X-x1)+y1
        if(iTemp < 0) iTemp = 0;
        if(iTemp > M1AM2A_MaxAsPwm) iTemp = M1AM2A_MaxAsPwm;
        FourQuadrantPosAsPwmArryMoto1_B[uiTemp] = iTemp;
        FourQuadrantPosAsPwmArryMoto2_A[uiTemp] = 0;
        uiTemp2 = iTemp = (uiTemp + 1) * fKx2 - ucPosAngleCool[13] * fKx2 + uiSwerveMaxSpeed / 2.0 + uiBackSpeedP / 2.0; //y = {(y2-y1)/(x2-x1)}*(X-x1)+y1
        if(iTemp < 0) iTemp = 0;
        if(iTemp > M1AM2A_MaxAsPwm) iTemp = M1AM2A_MaxAsPwm;
        FourQuadrantPosAsPwmArryMoto2_B[uiTemp] = iTemp;
    }
    fKx1 = (((signed int)(uiSwerveMaxSpeed - 0)) * 1.0) / (ucPosAngleCool[14] - ucPosAngleCool[13]);
    //fKx2 = (((signed int)(uiSwerveMaxSpeed - uiBackSpeedP )) * 1.0) / (ucPosAngleCool[14] - ucPosAngleCool[13]);
    for(uiTemp = ucPosAngleCool[13]; uiTemp < ucPosAngleCool[14] ; uiTemp++)
    {
        //315 - 356
        iTemp = (uiTemp + 1) * fKx1 - ucPosAngleCool[13] * fKx1 + 0; //y = {(y2-y1)/(x2-x1)}*(X-x1)+y1
        if(iTemp < 0) iTemp = 0;
        if(iTemp > M1AM2A_MaxAsPwm) iTemp = M1AM2A_MaxAsPwm;
        FourQuadrantPosAsPwmArryMoto1_A[uiTemp] = iTemp;

        FourQuadrantPosAsPwmArryMoto1_B[uiTemp] = 0;
        FourQuadrantPosAsPwmArryMoto2_A[uiTemp] = 0;
        iTemp = (uiTemp + 1) * fKx2 - ucPosAngleCool[13] * fKx2 + uiTemp2; //y = {(y2-y1)/(x2-x1)}*(X-x1)+y1
        if(iTemp < 0) iTemp = 0;
        if(iTemp > M1AM2A_MaxAsPwm) iTemp = M1AM2A_MaxAsPwm;
        FourQuadrantPosAsPwmArryMoto2_B[uiTemp] = iTemp;
    }
    fKx1 = 0;
    fKx2 = 0;
    for(uiTemp = ucPosAngleCool[14]; uiTemp < ucPosAngleCool[15] ; uiTemp++)
    {
        //356 - 360
        FourQuadrantPosAsPwmArryMoto1_A[uiTemp] = uiSwerveMaxSpeed;
        FourQuadrantPosAsPwmArryMoto1_B[uiTemp] = 0;
        FourQuadrantPosAsPwmArryMoto2_A[uiTemp] = 0;
        FourQuadrantPosAsPwmArryMoto2_B[uiTemp] = uiSwerveMaxSpeed;
    }


    fPostivePwm =  uniDiverInfo.strDirverAtt.ucForwardSpeedP * 0.004875;//�����ñ���ϵ��Ϊ80��ʱ���ӦΪ 0.75  �������ϵ��������ĸ�
    for(uiTemp = 0; uiTemp < 360; uiTemp++)
    {
        FourQuadrantPosAsPwmArryMoto1_A[uiTemp] = (unsigned short)(FourQuadrantPosAsPwmArryMoto1_A[uiTemp] * fPostivePwm);
        FourQuadrantPosAsPwmArryMoto2_A[uiTemp] = (unsigned short)(FourQuadrantPosAsPwmArryMoto2_A[uiTemp] * fPostivePwm);
        FourQuadrantPosAsPwmArryMoto1_B[uiTemp] = (unsigned short)(FourQuadrantPosAsPwmArryMoto1_B[uiTemp] * fPostivePwm);
        FourQuadrantPosAsPwmArryMoto2_B[uiTemp] = (unsigned short)(FourQuadrantPosAsPwmArryMoto2_B[uiTemp] * fPostivePwm);
    }
    //
    if(uniDiverInfo.strDirverAtt.ucLetRightMotorP > 5)
    {
        fPostivePwm = 1 - (uniDiverInfo.strDirverAtt.ucLetRightMotorP - 5) * 0.025;//20190813 *0.017
        for(uiTemp = 0; uiTemp < 180; uiTemp++)
        {
            FourQuadrantPosAsPwmArryMoto1_A[uiTemp] = (unsigned short)(FourQuadrantPosAsPwmArryMoto1_A[uiTemp] * fPostivePwm);
        }
    }
    if(uniDiverInfo.strDirverAtt.ucLetRightMotorP < 5)
    {
        fPostivePwm = 1 - (5 - uniDiverInfo.strDirverAtt.ucLetRightMotorP) * 0.025;//20190813 *0.017
        for(uiTemp = 0; uiTemp < 180; uiTemp++)
        {
            FourQuadrantPosAsPwmArryMoto2_A[uiTemp] = (unsigned short)(FourQuadrantPosAsPwmArryMoto2_A[uiTemp] * fPostivePwm);
        }
    }

    //
    if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor1DirEn)
    {
        uiMotor1LeftHighPwmRegAdd = Time1Ccr3Add;
        uiMotor1RightHighPwmRegAdd = Time1Ccr1Add;//����ǵ��һ�ķ���ı䣬�Ͱ���Ӧ�Ĵ�����ַ����
    }
    if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor2DirEn)
    {
        uiMotor2LeftHighPwmRegAdd = Time4Ccr3Add;
        uiMotor2RightHighPwmRegAdd = Time4Ccr1Add;//����ǵ��һ�ķ���ı䣬�Ͱ���Ӧ�Ĵ�����ַ����
    }
    if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor1Motor2)
    {
        //�����������������Ͱ�����PWM �Ĵ�����ַ��һ��
        uiTemp = uiMotor1LeftHighPwmRegAdd;
        uiTemp2 = uiMotor1RightHighPwmRegAdd;
        uiMotor1LeftHighPwmRegAdd = uiMotor2LeftHighPwmRegAdd;
        uiMotor1RightHighPwmRegAdd = uiMotor2RightHighPwmRegAdd;
        uiMotor2LeftHighPwmRegAdd = uiTemp;
        uiMotor2RightHighPwmRegAdd = uiTemp2;
    }
    strSysInfo.uiRemoteMaxXPos = FactoryRemoteMaxXPos;
    strSysInfo.uiRemoteMinXPos = FactoryRemoteMinXPos;
    strSysInfo.uiRemoteMiddleXPos = FactoryRemoteMidXPos;

    strSysInfo.uiRemoteMaxYPos = FactoryRemoteMaxYPos;
    strSysInfo.uiRemoteMinYPos = FactoryRemoteMinYPos;
    strSysInfo.uiRemoteMiddleYPos = FactoryRemoteMidYPos;

    strSysInfo.uiControlPareOfKp = FactoryPareKp;
    strSysInfo.uiControlPareOfKd = FactoryPareKd;

    strSysInfo.uiControlMaxXDriverPwm = FactoryPareMaxXDriverPwm;
    strSysInfo.uiControlMinXDriverPwm = FactoryPareMinXDriverPwm;

    strSysInfo.uiControlMaxYDriverPwm = FactoryPareMaxYDriverPwm;
    strSysInfo.uiControlMinYDriverPwm = FactoryPareMinYDriverPwm;

    strSysInfo.uiRemoteXP_First = ClacXPwmFirstRemotePosP;
    strSysInfo.uiRemoteXN_First = ClacXPwmFirstRemotePosN;
    strSysInfo.uiRemoteYP_First = ClacYPwmFirstRemotePosP;
    strSysInfo.uiRemoteYN_First = ClacYPwmFirstRemotePosN;

    strSysInfo.uiRemoteXP_Delt = DriverXRemoteAdcDeltPositive;
    strSysInfo.uiRemoteXN_Delt = DriverXRemoteAdcDeltNegative;
    strSysInfo.uiRemoteYP_Delt = DriverYRemoteAdcDeltPositive;
    strSysInfo.uiRemoteYN_Delt = DriverXRemoteAdcDeltNegative;
    if(strSysInfo.uiRemoteMaxXPos > strSysInfo.uiRemoteMaxYPos)strSysInfo.uiRemoteErroMaxValue = strSysInfo.uiRemoteMaxXPos + 3 * strSysInfo.uiRemoteXP_First;
    else strSysInfo.uiRemoteErroMaxValue = strSysInfo.uiRemoteMaxYPos + 3 * strSysInfo.uiRemoteYP_First;
    if(strSysInfo.uiRemoteMinXPos < strSysInfo.uiRemoteMinYPos)strSysInfo.uiRemoteErroMaxValue = strSysInfo.uiRemoteMinXPos / 3;
    else strSysInfo.uiRemoteErroMaxValue = strSysInfo.uiRemoteMinYPos / 3;
    //����ɲ������  �ֶ��������ߡ�
#ifdef UseSoftSimulate
    uniDiverInfo.strDirverAtt.ucFirstLineRatePare = 7;
    uniDiverInfo.strDirverAtt.ucFistLineTurnSecondPos = 7;
    uniDiverInfo.strDirverAtt.ucBreakAccPare = 8;
    uniDiverInfo.strDirverAtt.ucStopMinPwm = 20;
#endif

#ifndef UseFuctionBreakLin//���û�ж���  ��Ԫ����ɲ��Ч��
    uiBackSpeedP = uniDiverInfo.strDirverAtt.ucFistLineTurnSecondPos * 230 + 100;//��һ�׶���ڶ��׶εĹյ�ֵ200-1100
    if(uniDiverInfo.strDirverAtt.ucFirstLineRatePare >= 6 )uiTemp = uniDiverInfo.strDirverAtt.ucFirstLineRatePare - 2;
    else if(uniDiverInfo.strDirverAtt.ucFirstLineRatePare >= 4 )uiTemp = uniDiverInfo.strDirverAtt.ucFirstLineRatePare - 1;
    fKx1 = 0.27135 * uiTemp; //��һ��ɲ��б���趨ɲ��б����6 - 1.2 ��һ�׶�
    uiTemp2 = (unsigned int)(uiBackSpeedP / fKx1); //��һ�׶���ڶ��׶εĹյ�ֵx����ֵ
    for(uiTemp = 0; uiTemp < uiTemp2; uiTemp++)//�����ֹ��ģ�xֵ
    {
        uiBreakPwmTab[uiTemp] = (unsigned int)(-fKx1 * uiTemp + TimerBase);
    }
    iTemp = uniDiverInfo.strDirverAtt.ucStopMinPwm * 10; //�ڶ��׶ν�ֹ�ٶ�(0-1000)
    uiSwerveMaxSpeed = (uiBackSpeedP > iTemp) ? iTemp : uiBackSpeedP; //����յ����ֹ�ٶȵ���Сֵ
    uiBreakPwmTab[3608] = TimerBase - uiSwerveMaxSpeed;//��Ҫ���ٲ���
    fKx2 = (((signed int)(iTemp - uiBackSpeedP)) * 1.0) / (uiBreakPwmTab[3608] - uiTemp2);//�ڶ��׶ε�б��
    for(; uiTemp < uiBreakPwmTab[3608];)
    {
        if(fKx2 < 0)uiBreakPwmTab[uiTemp++] = (unsigned int)(fKx2 * (uiTemp - uiTemp2) + uiBackSpeedP); //����Ǽ���������ȥ��ֵ
        else uiBreakPwmTab[uiTemp++] = uiSwerveMaxSpeed;
    }
#endif

#ifdef UseFuctionBreakLin//������� ��Ԫ����ɲ����ָ��ɲ�� ���ϵ��
    //uiBreakPwmTab[3608] = 3600;
#endif

    if(TRUE == ucReadEepromFlag)vSendOneByteOrder(ReadOneByte, LastErroSaveAtEepromAdd);
}
void vFeedIwdg(void)//ι�����Ŀ��Ź�
{
    IWDG_ReloadCounter();//reload
}

void vConfigSysInStandbyMode(void)
{
    vSendSingleOrder(OrderPowerOff);//�ػ�
    ucTag100ms = FALSE;
    while(FALSE == ucTag100ms);

    if(ErroNoCheckSys != ucErroType)
    {
        if(ucErroType)//���������
        {
            vInitWatchDog(3, 100);
        }
    }

    while(ReadKeyOfOn_OffFlag)//Waite for Power Key Press Up !
    {
        //vSoftDelayms(10);
    }

    __set_FAULTMASK(1);      // �ر������ж�
    NVIC_SystemReset();// ��λ

}


