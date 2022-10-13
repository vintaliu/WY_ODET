
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
unsigned char ucConnectBreakFlag = FALSE;//在开机检测硬件电路的时候，检测刹车器是否一定连接标志
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
    GPIO_Init(ExPortSpeekerPort, &GPIO_InitStructure);	//PB12 外置喇叭 配置  在待机的时候强制输出为低电平
    C_EnResetStm8;
    CtrlExtSpeekerDis;

//	  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_2 ;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_PD;
//    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
//		
//		RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_SYSCFGCOMP, ENABLE);      ///<Enable SYSCFG clock
//	  SYSCFG_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinsSource2); ///停机模式的唤醒
//    EXTI_StructInit(&EXTI_InitStructure);
//    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
//    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     /* 中断模式 */
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; /* 下降沿触发 */
//    EXTI_InitStructure.EXTI_LineEnable = ENABLE;
//    EXTI_Init(&EXTI_InitStructure); 

//    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_2_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure); //停机模式的唤醒

//    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);//停机模式

}

//void EXTI3_2_IRQHandler(void)
//{
//    if (EXTI_GetIntStatus(EXTI_Line2) != RESET)
//    {
//        EXTI_ClearIntPendingBit(EXTI_Line2); ///
//    }
//}

void vNoKeyInStandbyMode(void)//刚上电，没有按键的情况下
{
    C_EnResetStm8;
    RCC->CFG &= 0xFFFFFFFC;//时钟的使用，使其频率最低
//	  RCC->CTRL &= ~RCC_CTRL_HSEEN;
//	  RCC->CTRL &= ~RCC_CTRL_PLLEN;//
    while(!ReadKeyOfOn_OffFlag)//Waite for Power Key Press Up !
    {
        vSoftDelayms(1);
    }
		RCC->CFG |= RCC_CFG_SYSCLKSEL_1;//时钟的使用，使其频率最低
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
    DMA_Configuration();//与ADC的DMA
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


//看启独立看门狗
//ucPrer=4
//uiRlr=1250 2s  Tout=((4×2^ucPrer)×uiRlr) /40
void vInitWatchDog(unsigned int ucPrer, unsigned int uiRlr) //开启单片机的看门狗
{
    //    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //使能对寄存器IWDG_PR和IWDG_RLR的写操作
    //    IWDG_SetPrescaler(ucPrer);  //设置IWDG预分频值:设置IWDG预分频值为64
    //    IWDG_SetReload(uiRlr);  //设置IWDG重装载值
    //    IWDG_ReloadCounter();  //按照IWDG重装载寄存器的值重装载IWDG计数器
    //    IWDG_Enable();  //使能IWDG
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
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     //  系统 ADC1-4  ADC6 ADC7
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);//ADC1-4   ADC6 ADC7


    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_12 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_PU;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;//PA12
    GPIO_Init(GPIOA, &GPIO_InitStructure);//示教模式通信

    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_15 ;//上下拉采样接口
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pins =  GPIO_Pins_0 | GPIO_Pins_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     //  系统
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//

    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_PD;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//开机按键检测

    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_3 ;//上下拉采样接口
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_8 | GPIO_Pins_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//刹车器检测

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
    GPIO_Init(GPIOB, &GPIO_InitStructure);                         //驱动EC继电器

    GPIO_InitStructure.GPIO_Pins =  GPIO_Pins_11 | GPIO_Pins_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//

    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_14 ;//飞车测试使用到的引脚 PB12
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_13 | GPIO_Pins_15 ;//刹车器电路
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_6 | GPIO_Pins_7;//驱动继电器控制  PB15  主电源
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
unsigned char ucCheckMotoConnect(void)//检测电机连接情况
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
    while(g_uiTimeNumCont1ms);//防止程序嵌套调用
    SetTim1CC1andCCR2(0);
    SetTim1CC3andCCR4(0);
    SetTim4CC1andCCR2(0);
    SetTim4CC3andCCR4(0);
    C_SetS_MotoA_A_AdcAsM1_Left_Port_High;
    C_SetS_MotoA_B_AdcAsM1_Right_Port_High;
    C_SetS_MotoB_A_AdcAsM2_Left_Port_High;
    C_SetS_MotoB_B_AdcAsM2_Right_Port_High;//设置电机检测的上拉电阻  上拉使能
    ucMotoA_RightCont = ucMotoA_LeftCont = ucMotoB_RightCont = ucMotoB_LeftCont = 0;
    for(uiTemp = 0; uiTemp < 70; uiTemp++)//大概检测1ms
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
        if(usMotoB_LeftAdc[uiTemp] < 300)ucMotoB_LeftCont++;//一般平均值为 800
    }//
    if((ucMotoA_RightCont > 40) || (ucMotoA_LeftCont > 40))//如果电压过低 说明上拉电阻有问题
    {
        CtlPowerOnTest_OFF;
        ucErroType = ErroMoto1HighBriageShort;
        if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor1Motor2)ucErroType = ErroMoto2HighBriageShort;
        ucEnAdcCalc = TRUE;
        return FALSE;
    }
    if((ucMotoB_RightCont > 40) || (ucMotoB_LeftCont > 40))//如果电压 过低了  说明上拉或者采样有问题的
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
#define CheckMotoTestPwmAsSlop   3000//这里设置  电机的测试电压  由于检测的另一端是悬空的，没有任何问题。可以测试
    if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bCheckMotorConnect)//如果系统要求检测电机的连接
    {
        C_SetS_MotoA_A_AdcAsM1_Left_Port_Low;
        C_SetS_MotoA_B_AdcAsM1_Right_Port_Low;//设置电机的 检测电阻为下拉
        g_uiTimeNumCont1ms = 2;
        while(g_uiTimeNumCont1ms);
        ucMotoA_RightCont = ucMotoA_LeftCont = ucMotoB_RightCont = ucMotoB_LeftCont = 0;
        for(uiTemp = 0; uiTemp < 70; uiTemp++)//大概检测1ms
        {
            vDealAdcVauleMotoAndCurrent();
            usMotoA_RightAdc[uiTemp]  = strMoto1Info.uiRightHBridgeHaveFilterAdc;
            usMotoA_LeftAdc[uiTemp]  = strMoto1Info.uiLeftHBridgeHaveFilterAdc;
        }
        for(uiTemp = 0; uiTemp < 70; uiTemp++)
        {
            if(usMotoA_RightAdc[uiTemp] > 500)ucMotoA_RightCont++;
            if(usMotoA_LeftAdc[uiTemp] > 500)ucMotoA_LeftCont++;//如果没有将电压拉下来，说明单片机可能有黏连的情况
        }
        if((ucMotoA_RightCont > 40) || (ucMotoA_LeftCont > 40))//如果下拉电阻 没有效果
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
        SetTim4CC3andCCR4Reg(0, 0);//让电机A输出一下
        ucMotoA_RightCont = ucMotoA_LeftCont = ucMotoB_RightCont = ucMotoB_LeftCont = 0;
        for(uiTemp = 0; uiTemp < 70; uiTemp++)//大概检测1ms
        {
            vDealAdcVauleMotoAndCurrent();
            usMotoA_RightAdc[uiTemp]  = strMoto1Info.uiRightHBridgeHaveFilterAdc;
            usMotoA_LeftAdc[uiTemp]  = strMoto1Info.uiLeftHBridgeHaveFilterAdc;
        }
        for(uiTemp = 0; uiTemp < 70; uiTemp++)
        {
            if(usMotoA_RightAdc[uiTemp] < 50)ucMotoA_RightCont++;
            if(usMotoA_LeftAdc[uiTemp] < 50)ucMotoA_LeftCont++;//排序检测
        }
        if((ucMotoA_RightCont > 40) || (ucMotoA_LeftCont > 40))//说明有断线或者 是mos驱动问题  没有将应该输出的电压输出出来
        {
            CtlPowerOnTest_OFF;
            SetTim1CC1andCCR2Reg(0, 0);
            SetTim1CC3andCCR4Reg(0, 0);
            SetTim4CC1andCCR2Reg(0, 0);
            SetTim4CC3andCCR4Reg(0, 0);
            ucErroType = ErroMoto1HighBriageShort;
            if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor1Motor2)ucErroType = ErroMoto2HighBriageShort;
            ucEnAdcCalc = TRUE;//如果有错的情况就及时退出
            return FALSE;
        }
        SetTim1CC1andCCR2Reg(0, 0);
        SetTim1CC3andCCR4Reg(0, 0);
        SetTim4CC1andCCR2Reg(0, 0);
        SetTim4CC3andCCR4Reg(0, 0);//及时将电机的驱动信号还原
        if(ucPowerKeyPressTimes >= 2)
        {
            ucEnAdcCalc = TRUE;
            vAutoPowerOffTimeFlag();//如果有按键的动作，立即退出来
        }
        g_uiTimeNumCont1ms = 5;
        while(g_uiTimeNumCont1ms);//插入一个小延时，作异步检测
        SetTim1CC1andCCR2Reg(SetPwmPinHighReg, 0);
        SetTim1CC3andCCR4Reg(CheckMotoTestPwmAsSlop, CheckMotoTestPwmAsSlop);
        SetTim4CC1andCCR2Reg(0, 0);
        SetTim4CC3andCCR4Reg(0, 0);//让另一组测试开始
        ucMotoA_RightCont = ucMotoA_LeftCont = ucMotoB_RightCont = ucMotoB_LeftCont = 0;
        for(uiTemp = 0; uiTemp < 70; uiTemp++)//大概检测1ms
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
        if((ucMotoA_RightCont > 40) || (ucMotoA_LeftCont > 40))//说明有断线或者 是mos驱动问题
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
        C_SetS_MotoA_B_AdcAsM1_Right_Port_High;  //还原上下拉
        ////////////////////////////////////////////////   下面开始检测电机B
        C_SetS_MotoB_A_AdcAsM2_Left_Port_Low;
        C_SetS_MotoB_B_AdcAsM2_Right_Port_Low;
        g_uiTimeNumCont1ms = 5;
        while(g_uiTimeNumCont1ms);
        ucMotoA_RightCont = ucMotoA_LeftCont = ucMotoB_RightCont = ucMotoB_LeftCont = 0;
        for(uiTemp = 0; uiTemp < 70; uiTemp++)//大概检测1ms
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
        if((ucMotoA_RightCont > 40) || (ucMotoA_LeftCont > 40))//如果下拉电阻 没有效果
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
        for(uiTemp = 0; uiTemp < 70; uiTemp++)//大概检测1ms
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
        if((ucMotoA_RightCont > 40) || (ucMotoA_LeftCont > 40))//说明有断线或者 是mos驱动问题
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
        for(uiTemp = 0; uiTemp < 70; uiTemp++)//大概检测1ms
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
检查系统各功能模块性能及好坏
*@param checkMotoTestPwmFlag 是否检查电机PWM 
*        @value ENABLE 检查电源
*        @value DISABLE 不检查电源
*/
#define  ErroRemoteAlarmFlashLedTimeMs   120
#define  ErroRemoteAlarmWaiteTime         4
#define  RelaesPowerValue  1800//当电压小于 16.9v的时候 不要去释放电压了
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
//        //CtlPowerOnTest_OFF;//如果有摇杆 放在中间的位置 就去等候
//    }
//    if((strSysInfo.uiRemoteNowYPos < (strSysInfo.uiRemoteMiddleYPos - 2 * uiTempYDelt)) || (strSysInfo.uiRemoteNowYPos > (strSysInfo.uiRemoteMiddleYPos + 2 * uiTempYDelt)))
//    {
//        ucErroType = ErroNoRemote;
//        //CtlPowerOnTest_OFF;
//    }
//#endif
			//检查之前，先初始化错误类型，
			ucErroType = ErroNoCheckSys;
#ifdef CheckPowerVoltage

    vSendSysPower(4000);//发一个开机声音
    uiTempYDelt = uiTempXDelt = strSysInfo.uiSysTemPower;//将电量的值 暂时存下
    if(uiTempYDelt > RelaesPowerValue)//如果EC在开机的时候电压小，就不要去释放一次电压了
    {
        uiTempXDelt *= 9;
        uiTempXDelt /= 10;
			  if(ENABLE == checkMotoTestPwmFlag)//如果需要释放 电池 电压，开机第一次检查需要释放，开机后报错过程中不需要释放电压
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
            if(strSysInfo.uiSysTemPower < (uiTempXDelt + 150))//如果此时EC电压一直在下降
            {
                uiTime = 8888;
            }
        }

        if((strSysInfo.uiSysTemPower > (uiTempXDelt + 150)) && (uiTempYDelt > NoPowerOnAdcValue)) //如果没有上控制系统的电源的时候 有电压说明是有错的
        {
            CtlPowerOnTest_OFF;
            ucErroType = ErroRelayShort;	//说明是继电器有短路
            CtlSetMotor1LeftPwmPercent(0);
            CtlSetMotor1RightPwmPercent(0);
            CtlSetMotor2LeftPwmPercent(0);
            CtlSetMotor2RightPwmPercent(0);
            return;
        }
    }

    CtlBrakeBikeDisEn;//让刹车器打开下，因继电器没有上电。不会出问题的
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

    if(strSysInfo.uiSysTemPower < (NoPowerOnAdcValue - 200)) //如果没有上控制系统的电源的时候 有电压说明是有错的
    {
        CtlPowerOnTest_OFF;//如果上电的时候  没有电压的变化，可能是电路有短路的情况了
        ucErroType = ErroRelayBreak ; //继电器断开
        return;
    }//LG_qiuzhi
#endif
    //20190517上电异常就不检测电机了
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
        ucErroType = ErroLowVoltage ; //继电器断开
        return;
    }//LG_qiuzhi
    //    CtlPowerOnTest_OFF;
#define    AlowBreakAdcErro  180
    if(FALSE == ucCheckMotoConnect())return;
    if(FALSE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-刹车器程序或0-驻坡程序由参数表第6项决定20190110
    {
        if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bCheckBreakConnect)
        {
            uiTempXDelt = strSysInfo.uiSysTemPower * BreakLeftBreakAdcP; //左电机比例值
            uiTempYDelt = strSysInfo.uiSysTemPower * BreakRightBreakAdcP; //右电机比例值
            if((strSysInfo.uiBreakFeedBackCurrent >= (uiTempXDelt - AlowBreakAdcErro)) && (strSysInfo.uiBreakFeedBackCurrent <= (uiTempXDelt + AlowBreakAdcErro)))
            {
                ucErroType = ErroMoto2Break; //左刹车器坏掉
                return;
            }
            else if((strSysInfo.uiBreakFeedBackCurrent >= (uiTempYDelt - AlowBreakAdcErro)) && (strSysInfo.uiBreakFeedBackCurrent <= (uiTempYDelt + AlowBreakAdcErro)))
            {
                ucErroType = ErroMoto1Break; //左刹车器坏掉
                return;
            }
            else if(strSysInfo.uiBreakFeedBackCurrent < (strSysInfo.uiSysTemPower * NoBreakAdcP + AlowBreakAdcErro) )
            {
                ucErroType = ErroNoBearker ; //如果是两个刹车器都坏掉了
                return;
            }
        }
        if(strSysInfo.uiBreakFeedBackCurrent > (strSysInfo.uiSysTemPower * BreadAdAndPowerP - 650) )
        {
            ucConnectBreakFlag = TRUE; //系统检测到刹车器
        }
        else ucConnectBreakFlag = FALSE;
    }
    else
    {
        if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bCheckBreakConnect)
        {
            if(C_ReadLeftMotoBreak_Pin_Low)
            {
                ucErroType = ErroMoto1Break; //左刹车器坏掉
                if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor1Motor2)ucErroType = ErroMoto2Break; //左刹车器坏掉
                return;
            }
            if(C_ReadRightMotoBreak_Pin_Low)
            {
                ucErroType = ErroMoto2Break; //左刹车器坏掉
                if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor1Motor2)ucErroType = ErroMoto1Break; //左刹车器坏掉

                return;
            }
            if(C_ReadLeftMotoBreak_Pin_Low && C_ReadRightMotoBreak_Pin_Low)
            {
                ucErroType = ErroNoBearker ; //如果是两个刹车器都坏掉了
                return;
            }
        }
        if((!C_ReadLeftMotoBreak_Pin_Low) || (!C_ReadRightMotoBreak_Pin_Low))
        {
            ucConnectBreakFlag = TRUE; //系统检测到刹车器
        }
        else ucConnectBreakFlag = FALSE;
    }
    CtlDriverPowerContolON;//继电器打开
    CtlPowerOnTest_OFF;//缓上电关闭
    vSendSingleOrder(OutDispLed);
    g_uiTimeNumCont1ms = 5;
    while(g_uiTimeNumCont1ms);
//    vSendSysPower(strSysInfo.uiSysTemPower);//显示电	量  这里不应该要的对应电量自动识别
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
对系统用到的参数的设置
*/

static void ADC1_Mode_Config(void)
{
    ADC_InitType ADC_InitStructure;
    /* ADC1 configuration */

    /* 使能ADC1时钟 */
    /* Configure ADCCLK such as ADCCLK = PCLK2/6 */
    RCC_ADCCLKConfig(RCC_APB2CLK_Div8);

    ADC_StructInit(&ADC_InitStructure);  //将外设 ADC1 的全部寄存器重设为缺省值
    /* ADC1 configuration ------------------------------------------------------*/
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
    ADC_InitStructure.ADC_ScanMode = ENABLE;	        //模数转换工作在扫描模式
    ADC_InitStructure.ADC_ContinuousMode = ENABLE;	   //模数转换工作在连续转换模式
    ADC_InitStructure.ADC_ExternalTrig = ADC_ExternalTrig_None;	//外部触发转换关闭
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
    ADC_InitStructure.ADC_NumOfChannel = TotalADCChannels;	//顺序进行规则转换的ADC通道的数目
    ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器

    /* ADC1 regular channel11 configuration */
    //设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
    //ADC1,ADC通道x,规则采样顺序值为y,采样时间为12.5周期

    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_13_5 );//PA1 遥感Y位置
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_13_5 );//PA2  遥感X位置
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 3, ADC_SampleTime_13_5 );//PB0 电机1右电压检测
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_13_5 );//PA4  电机1左电压检测
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 5, ADC_SampleTime_13_5 );//PA6 系统电量检测
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 6, ADC_SampleTime_13_5 );//PA7电机2左电压检测
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 7, ADC_SampleTime_13_5 );//PB0 刹车器电压反馈 --> 刹车器电压不再走ADC， 直接读取PB8,PB9电压
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 8, ADC_SampleTime_13_5 );//PB1 电机2右电压检测
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 9, ADC_SampleTime_13_5 );//PA0  电机1的电流检测 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 10, ADC_SampleTime_13_5 );//PA5 右电机电流检测

    // 开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数）
    ADC_DMACtrl(ADC1, ENABLE);
    /* Enable ADC1 */
    ADC_Ctrl(ADC1, ENABLE);	   //使能指定的ADC1
    /* Enable ADC1 reset calibaration register */
    ADC_RstCalibration(ADC1);	  //复位指定的ADC1的校准寄存器
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));	//获取ADC1复位校准寄存器的状态,设置状态则等待
    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADC1);		//开始指定ADC1的校准状态
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));		//获取指定ADC1的校准程序,设置状态则等待
}

void DMA_Configuration(void)
{
    DMA_InitType DMA_InitStructure;
    /* DMA clock enable */

    /* USART1_Tx_DMA_Channel (triggered by USART1 Tx event) Config */
    DMA_Reset(DMA1_Channel1); //将DMA的通道1寄存器重设为缺省值
    DMA_DefaultInitParaConfig(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->RDOR;  //DMA外设ADC基地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)vDmaContAdcVaule;       //DMA内存基地址
    DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALSRC;                  /* 传输方向，外设作为数据传输的目的地 */
    DMA_InitStructure.DMA_BufferSize = OneChannelConvertTimes * TotalADCChannels;   /* 数据大小 */
    DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;          /* 外设地址寄存器不变 */
    DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;                   /* 内存地址寄存器递增 */
    DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_HALFWORD; /* 外设数据宽度为16位 */
    DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_HALFWORD;         /* 存储器数据宽度为16位 */
    DMA_InitStructure.DMA_Mode = DMA_MODE_CIRCULAR;                             //DMA通道x没有设置为内存到内存传输
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;                   /* 非常高优先级 */
    DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;                        /* 非内存到内存 */
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    /* Enable USART1 DMA TX Channel */
    DMA_ChannelEnable(DMA1_Channel1, ENABLE);

}


/*
*********************************************************************************************************
*	函 数 名: SetBaudrate
*	功能说明: 配置串口波特率
*	形    参: USARTx.串口 Value.波特率
*	返 回 值: 无
*********************************************************************************************************
*/
static void SetBaudrate(unsigned short usBaudRate)
{
    USART_InitType USART_InitStructure = {0};
    GPIO_InitType GPIO_InitStructure = {0};
    NVIC_InitType NVIC_InitStructure = {0};
    DMA_InitType DMA_InitStructure = {0};

#ifdef  UsedUartID_UART2//如果使用UART2
    /* Enable USART2 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_USART2, ENABLE);//开启串口定时器
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
#else //否则切换到UART1
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
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&EVAL_COM->DT;         /* DMA外设基地址 */
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ucaUsarDmaTxdBuffer;       /* DMA内存基地址 */
    DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALDST;                  /* 传输方向，外设作为数据传输的目的地 */
    DMA_InitStructure.DMA_BufferSize = 80;                             /* 数据大小 */
    DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;          /* 外设地址寄存器不变 */
    DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;                   /* 内存地址寄存器递增 */
    DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_BYTE; /* 外设数据宽度为8位 */
    DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_BYTE;         /* 存储器数据宽度为8位 */
    DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;                             /* 非循环模式 */
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;                   /* 非常高优先级 */
    DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;                        /* 非内存到内存 */
    DMA_Init(UsedUartMdaChannle, &DMA_InitStructure);

    DMA_ChannelEnable(UsedUartMdaChannle, ENABLE);

    /* Enable USART2 DMA TX request */
    USART_DMACmd(EVAL_COM, USART_DMAReq_Tx, ENABLE);//ucUartRxData

//    bsp_DelayMS(5);
#ifdef UsedUartDmaRxData
    /* USART2_Rx_DMA_Channel (triggered by USART2 Tx event) Config */
    DMA_Reset(DMA1_Channel5);
    DMA_DefaultInitParaConfig(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DT;         /* DMA外设基地址 */
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ucUartDmaRxData;       /* DMA内存基地址 */
    DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALSRC;              /* 传输方向，外设作为数据传输的目的地 */
    DMA_InitStructure.DMA_BufferSize = RecDmaBufferSize;                     /* 数据大小 */
    DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;          /* 外设地址寄存器不变 */
    DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;                   /* 内存地址寄存器递增 */
    DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_BYTE; /* 外设数据宽度为8位 */
    DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_BYTE;         /* 存储器数据宽度为8位 */
    DMA_InitStructure.DMA_Mode = DMA_MODE_CIRCULAR;                             /* 非循环模式 */
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;                   /* 非常高优先级 */
    DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;                        /* 非内存到内存 */
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
    //配置GPIO管脚参数
    GPIO_InitStructure.GPIO_Pins =  GPIO_Pins_14  | GPIO_Pins_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);

    TMR_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TMR_Period = 3600;					//自动重装载值
    TIM_TimeBaseStructure.TMR_DIV = 0;					//预分频值
    TIM_TimeBaseStructure.TMR_ClockDivision = 0; 					//时钟分割
    TIM_TimeBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;		//向上计数模式
    TIM_TimeBaseStructure.TMR_RepetitionCounter = 0;
    TMR_TimeBaseInit(TMR15, &TIM_TimeBaseStructure); 				//初始化TIM15时基

    TMR_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TMR_OCMode = TMR_OCMode_PWM1;				//TIM脉宽调制模式     //!!!!!!5line 使用五线接法，所以跟正常接法反转PWM极性
    TIM_OCInitStructure.TMR_OutputState = TMR_OutputState_Enable;	//比较输出使能
    TIM_OCInitStructure.TMR_Pulse =  0;								//待装入捕获比较寄存器脉冲值
    TIM_OCInitStructure.TMR_OCPolarity = TMR_OCPolarity_High;		//输出极性
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
    //配置GPIO管脚参数
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_4 | GPIO_Pins_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);

    TMR_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TMR_Period = 3600;					//自动重装载值
    TIM_TimeBaseStructure.TMR_DIV = 0;					//预分频值
    TIM_TimeBaseStructure.TMR_ClockDivision = 0; 					//时钟分割
    TIM_TimeBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;		//向上计数模式
    TIM_TimeBaseStructure.TMR_RepetitionCounter = 0;
    TMR_TimeBaseInit(TMR3, &TIM_TimeBaseStructure); 				//初始化TIM15时基

    TMR_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TMR_OCMode = TMR_OCMode_PWM1;				//TIM脉宽调制模式
    TIM_OCInitStructure.TMR_OutputState = TMR_OutputState_Enable;	//比较输出使能
    TIM_OCInitStructure.TMR_Pulse = 0 ;								//待装入捕获比较寄存器脉冲值
    TIM_OCInitStructure.TMR_OCPolarity = TMR_OCPolarity_High;		//输出极性
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
    //配置GPIO管脚参数
    GPIO_InitStructure.GPIO_Pins =  GPIO_Pins_8 | GPIO_Pins_9 | GPIO_Pins_10 | GPIO_Pins_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;//电机驱动PWM

    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TMR_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TMR_Period = 3600;					//自动重装载值
    TIM_TimeBaseStructure.TMR_DIV = 0;					//预分频值
    TIM_TimeBaseStructure.TMR_ClockDivision = 0; 					//时钟分割
    TIM_TimeBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;		//向上计数模式
    TIM_TimeBaseStructure.TMR_RepetitionCounter = 0;
    TMR_TimeBaseInit(TMR1, &TIM_TimeBaseStructure); 				//初始化TIM15时基

    TMR_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TMR_OCMode = TMR_OCMode_PWM1;				//TIM脉宽调制模式     //!!!!!!5line 使用五线接法，所以跟正常接法反转PWM极性
    TIM_OCInitStructure.TMR_OutputState = TMR_OutputState_Enable;	//比较输出使能
    TIM_OCInitStructure.TMR_Pulse =  0;								//待装入捕获比较寄存器脉冲值
    TIM_OCInitStructure.TMR_OCPolarity = TMR_OCPolarity_High;		//输出极性
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
    TMR_TMReBaseStructure.TMR_Period = 2000;//500us一个中断
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
    TMR_TMReBaseStructure.TMR_Period = 60000;//10ms一个中断
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
    TMR_TMReBaseStructure.TMR_Period = 6000;//500us一个中断
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

void vInitTimer(void)//PWM定时器
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
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     /* 中断模式 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; /* 下降沿触发 */
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
    if((0x0F & ucLastErro) == ucShowErro) //如果上次的错误与本次错误一致  记录上次错误的次数
    {
        if(0x0F != (ucLastErro >> 4))//错误帧的高四字节用于放  错误的次数
        {
            ucDataBuf[1] = (ucLastErro >> 4) + 1;//对错误次数的累加
            ucDataBuf[1] <<= 4;
        }
        ucDataBuf[1] |= ucShowErro;
    }
    else ucDataBuf[1] = ucShowErro;
    vSendMultBytesOrder(WriteOneByte, ucDataBuf, 2);

		for(uiTimeCont = 0; uiTimeCont < 5; uiTimeCont++)//报警延长时间
		{
			   ucTag100ms = FALSE;
				while(FALSE == ucTag100ms);
				vSendOneByteOrder(OrderErro, ucShowErro);
		}

    ucShowErro = FALSE;
    for(uiTimeCont = 0; uiTimeCont < 40000; uiTimeCont++)//报警延长时间
    {
        ucTag1ms = FALSE;
        while(FALSE == ucTag1ms);
			
			  //如果有 硬件（各模块）异常
			  if( ErroNoCheckSys != ucErroType || ErroNoErro != ucErroType)
				{
					 //检测 硬件（各模块）异常是否恢复
				   vCheckSystemInfo(DISABLE);//去检查各模块是否正常地待命
          
				}
			
        if(ReadKeyOfOn_OffFlag || ucAnlyFree)//如果有电源按键或者是示教通信
        {
            if(FALSE == ucShowErro) vSendSingleOrder(QuitErro);//只退出一次报警
            ucShowErro = TRUE;//如果下次还有示教内容。就不去报警了
            if(ucAnlyFree)//如果有示教内容
            {
                uiTimeCont = 0;//清空延时变量
                vAnalyTeacherDataOfTime();//如果有示教器发过来的数据这时候要去处理下
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
			 //如果报警结束还有异常就关机
		   vAutoPowerOffTimeFlag();
		}
}
void vReadEepromData(void)
{
    union UINT unuiData = {0};
    unsigned char ucTemp = 0, ucTimeCont = 0;
    unsigned char ucCrcTime = 0, ucAutoSelfConfig = FALSE;
    for(ucCrcTime = 0; ucCrcTime < 100; ucCrcTime++)//20151102 前 for(ucCrcTime = 0; ucCrcTime < 100; ucCrcTime++)
    {
        ucRec60Bytes[5] = ucRec60Bytes[58] = ucRec60Bytes[59] = 0x00;
        while(ucTemp < 70)//20151102 前  while(ucTemp < 80)
        {
            ucTemp++;
            unuiData.ucData.ucData1 = ucRec60Bytes[58];
            unuiData.ucData.ucData2 = ucRec60Bytes[59];//清空占用字
            for(ucTimeCont = 0; ucTimeCont < 10; ucTimeCont++) //
            {
                ucTag1ms = FALSE;
                while(FALSE == ucTag1ms);
                //                if(ReadKeyOfOn_OffFlag)//20190102
                //                {
                //                    vAutoPowerOffTimeFlag();//在这里快速响应按键
                //                }
            }
            if(ucPowerKeyPressTimes >= 2)
            {
                vAutoPowerOffTimeFlag();
            }
            if(WriteEepromData == (unuiData.uiData & WriteEepromData))//如果读取到关键字
            {
                ucTemp = 211;
                ucCrcTime = 211;
                if(ucAutoSelfConfig)//说明有自动烧写事件了
                {
                    for(unuiData.ucData.ucData1 = 0; unuiData.ucData.ucData1 < DirverAttLength; unuiData.ucData.ucData1++)//再次检验写与读是否一致性
                    {
                        if(uniDiverInfo.ucDirverAttDataBuff[unuiData.ucData.ucData1] != ucRec60Bytes[unuiData.ucData.ucData1]) //如果没有读的对
                        {
                            ucTemp = 0;//要求，再去重新烧写
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
        if(211 != ucTemp) //如果单片机在第一次上电，强行写入初始值
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
            uniDiverInfo.strDirverAtt.unMotorInfo.bits.bLilonOrQiansuan = FALSE;//默认是铅酸电池
            uniDiverInfo.strDirverAtt.ucFirstLineRatePare = 5;//第一次刹车曲线的切线值
            uniDiverInfo.strDirverAtt.ucFistLineTurnSecondPos = 6;//第一次刹车转换到第二次刹车的拐点值
            uniDiverInfo.strDirverAtt.ucBreakAccPare = 6;//刹车器的减速度
            uniDiverInfo.strDirverAtt.ucForwardSpeedP = 87;//前进速度比例系数
            uniDiverInfo.strDirverAtt.ucBackSpeedP = 26;//后退速度比例系数
            uniDiverInfo.strDirverAtt.ucDriverAcc = 6;//前进后退加速度
            uniDiverInfo.strDirverAtt.ucSwerveAcc = 6;//转弯加速度
            uniDiverInfo.strDirverAtt.ucStarMinPwm = 8;//启动最小PWM
            uniDiverInfo.strDirverAtt.ucStopMinPwm = 8;//停止最小PWM
            uniDiverInfo.strDirverAtt.ucLetRightMotorP = 5;//左右电机比例系数
            uniDiverInfo.strDirverAtt.ucLetRightMotorTiedP = 6;//左右电机绑定系数
            uniDiverInfo.strDirverAtt.ucMaxDriverCurrent = 20;//最大驱动电流
            uniDiverInfo.strDirverAtt.ucFactryIdData1 = DefaultFactryID & 0xFF;
            uniDiverInfo.strDirverAtt.ucFactryIdData1 = (DefaultFactryID >> 8) & 0xFF;
            uniDiverInfo.strDirverAtt.ucFactryIdData1 = (DefaultFactryID >> 16) & 0xFF;
            uniDiverInfo.strDirverAtt.ucFactryIdData1 = (DefaultFactryID >> 24) & 0xFF;
            uniDiverInfo.strDirverAtt.ucForwardMinSpeedP = 40;//前进最小速度
            uniDiverInfo.strDirverAtt.ucBackMinSpeedP = 21;	//后退最小速度
            uniDiverInfo.strDirverAtt.ucSwerveMaxSpeedP = 26;		//转弯最大速度
            uniDiverInfo.strDirverAtt.ucSwerveMinSpeedP = 15;//转弯最小速度
            uniDiverInfo.strDirverAtt.unDelayBreakActTime.all = 550;//默认是400ms刹车延时
            for(ucTemp = 0; ucTemp < DirverAttLength; ucTemp++)ucRec60Bytes[ucTemp] = uniDiverInfo.ucDirverAttDataBuff[ucTemp];
            ucRec60Bytes[58] = 0xFF & WriteEepromData;
            ucRec60Bytes[59] = 0xFF & (WriteEepromData >> 8); //写入被占用标志
            vSendMultBytesOrder(Write60Bytes, ucRec60Bytes, 60);
            ucRec60Bytes[58] = ucRec60Bytes[59] = 0x00;
        }
    }
    if(211 == ucTemp)//如果较验的的确无误
    {
        for(unuiData.uiData = 0; unuiData.uiData < DirverAttLength; unuiData.uiData++)
        {
            uniDiverInfo.ucDirverAttDataBuff[unuiData.uiData] = ucRec60Bytes[unuiData.uiData];
        }
    }
}

//#define UseSoftSimulate
void vReadSysTempPare(unsigned char ucReadEepromFlag)//读取系统中的参数
{
#define M1AM2A_MaxAsPwm  7500//当两电机为最大相当是时候的邋PWM值
    unsigned int ucPosAngleCool[16] = {0, 4, 15, 60, 86, 94, 120, 165, 176, 184, 225, 266, 274, 315, 356, 360};//分量度数
    float fPostivePwm;
    float fKx1, fKx2; //斜率
    int iTemp = 0;
    unsigned int uiTemp, uiTemp2;
    unsigned int uiBackSpeedP;
    unsigned int uiSwerveMaxSpeed;//计算出转弯的最大速度
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
    if(TRUE == ucReadEepromFlag)//只读取一次
    {
        vReadEepromData();
    }
#endif
		if(FALSE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//如果是驻坡型控制器，强制不检测刹车器
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
    }            //Y2              Y1                   X2 60度             X1 4度
    fKx1 = ((M1AM2A_MaxAsPwm - uiSwerveMaxSpeed) * 1.0) / (ucPosAngleCool[3] - ucPosAngleCool[1]);
    fKx2 = (((signed int)(0 - uiSwerveMaxSpeed)) * 1.0) / (ucPosAngleCool[2] - ucPosAngleCool[1]); //算出Kx  4 //到 15
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
    fKx2 = ((M1AM2A_MaxAsPwm - 0) * 1.0) / (ucPosAngleCool[4] - ucPosAngleCool[2]); //算出kd 86 15
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
    fKx2 = ((M1AM2A_MaxAsPwm - 0) * 1.0) / (ucPosAngleCool[4] - ucPosAngleCool[2]); //算出kd 86 15
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


    fPostivePwm =  uniDiverInfo.strDirverAtt.ucForwardSpeedP * 0.004875;//当设置比例系数为80的时候对应为 0.75  反向比例系数比正向的高
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
        uiMotor1RightHighPwmRegAdd = Time1Ccr1Add;//如果是电机一的方向改变，就把相应的存器地址改下
    }
    if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor2DirEn)
    {
        uiMotor2LeftHighPwmRegAdd = Time4Ccr3Add;
        uiMotor2RightHighPwmRegAdd = Time4Ccr1Add;//如果是电机一的方向改变，就把相应的存器地址改下
    }
    if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor1Motor2)
    {
        //如果两个电机调换，就把两个PWM 寄存器地址换一下
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
    //计算刹车曲线  分段线性曲线。
#ifdef UseSoftSimulate
    uniDiverInfo.strDirverAtt.ucFirstLineRatePare = 7;
    uniDiverInfo.strDirverAtt.ucFistLineTurnSecondPos = 7;
    uniDiverInfo.strDirverAtt.ucBreakAccPare = 8;
    uniDiverInfo.strDirverAtt.ucStopMinPwm = 20;
#endif

#ifndef UseFuctionBreakLin//如果没有定义  二元函数刹车效果
    uiBackSpeedP = uniDiverInfo.strDirverAtt.ucFistLineTurnSecondPos * 230 + 100;//第一阶段与第二阶段的拐点值200-1100
    if(uniDiverInfo.strDirverAtt.ucFirstLineRatePare >= 6 )uiTemp = uniDiverInfo.strDirverAtt.ucFirstLineRatePare - 2;
    else if(uniDiverInfo.strDirverAtt.ucFirstLineRatePare >= 4 )uiTemp = uniDiverInfo.strDirverAtt.ucFirstLineRatePare - 1;
    fKx1 = 0.27135 * uiTemp; //第一次刹车斜率设定刹车斜率在6 - 1.2 第一阶段
    uiTemp2 = (unsigned int)(uiBackSpeedP / fKx1); //第一阶段与第二阶段的拐点值x坐标值
    for(uiTemp = 0; uiTemp < uiTemp2; uiTemp++)//求出截止点的，x值
    {
        uiBreakPwmTab[uiTemp] = (unsigned int)(-fKx1 * uiTemp + TimerBase);
    }
    iTemp = uniDiverInfo.strDirverAtt.ucStopMinPwm * 10; //第二阶段截止速度(0-1000)
    uiSwerveMaxSpeed = (uiBackSpeedP > iTemp) ? iTemp : uiBackSpeedP; //算出拐点与截止速度的最小值
    uiBreakPwmTab[3608] = TimerBase - uiSwerveMaxSpeed;//共要加速步数
    fKx2 = (((signed int)(iTemp - uiBackSpeedP)) * 1.0) / (uiBreakPwmTab[3608] - uiTemp2);//第二阶段的斜率
    for(; uiTemp < uiBreakPwmTab[3608];)
    {
        if(fKx2 < 0)uiBreakPwmTab[uiTemp++] = (unsigned int)(fKx2 * (uiTemp - uiTemp2) + uiBackSpeedP); //如果是减函数，就去赋值
        else uiBreakPwmTab[uiTemp++] = uiSwerveMaxSpeed;
    }
#endif

#ifdef UseFuctionBreakLin//如果定义 二元函数刹车，指定刹车 最大系数
    //uiBreakPwmTab[3608] = 3600;
#endif

    if(TRUE == ucReadEepromFlag)vSendOneByteOrder(ReadOneByte, LastErroSaveAtEepromAdd);
}
void vFeedIwdg(void)//喂独立的看门狗
{
    IWDG_ReloadCounter();//reload
}

void vConfigSysInStandbyMode(void)
{
    vSendSingleOrder(OrderPowerOff);//关机
    ucTag100ms = FALSE;
    while(FALSE == ucTag100ms);

    if(ErroNoCheckSys != ucErroType)
    {
        if(ucErroType)//如果出错了
        {
            vInitWatchDog(3, 100);
        }
    }

    while(ReadKeyOfOn_OffFlag)//Waite for Power Key Press Up !
    {
        //vSoftDelayms(10);
    }

    __set_FAULTMASK(1);      // 关闭所有中端
    NVIC_SystemReset();// 复位

}


