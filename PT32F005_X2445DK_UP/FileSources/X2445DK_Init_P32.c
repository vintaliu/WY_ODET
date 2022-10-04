
#include "X2445DK_Const.h"

void GPIO_InitPT32(void);
void PWM1_Init(void);
void vInitAdc(void);
void Uart_Init(void);
void TimeBaseInit(void);
void vInitAdc(void);


void vSysSourceInit(void)
{
    GPIO_InitPT32();
    PWM1_Init();
    vInitAdc();
    Uart_Init();
    TimeBaseInit();
}

void GPIO_InitPT32(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    POWER_SHOW_0;
	  GPIO_PinAFConfig(CMSDK_PD, GPIO_PinSource1, GPIO_AF_0);	//GPIO_ClearPinAF(CMSDK_PD, GPIO_PinSource1);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;//电量指示灯1，速度指示灯1，2
    GPIO_InitStructure.GPIO_Mode = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_SR = GPIO_Slew_Rate_High;
    GPIO_Init(CMSDK_PA, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; //电量指示灯 2，3，4，5
    GPIO_Init(CMSDK_PC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5;//速度指示灯
    GPIO_Init(CMSDK_PD, &GPIO_InitStructure);

	  GPIO_ResetBits(CMSDK_PB, GPIO_Pin_4);
	  GPIO_SetBits(CMSDK_PB, GPIO_Pin_5);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;//
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(CMSDK_PB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(CMSDK_PB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;// 模拟按键输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_SR = GPIO_Slew_Rate_High;
    GPIO_Init(CMSDK_PD, &GPIO_InitStructure);
    //    POWER_SHOW_1;
    //    POWER_SHOW_2;
    //    POWER_SHOW_3;
    //    POWER_SHOW_4;
    //    POWER_SHOW_5;
    //    SPEED_STEP1_ON;
    //    SPEED_STEP2_ON;
    //    SPEED_STEP3_ON;
    //    SPEED_STEP4_ON;
    //    SPEED_STEP5_ON;
}

void vInitAdc(void)
{
    ADC_InitTypeDef  ADC_InitStruct;
    ADC_StructInit(&ADC_InitStruct);
    ADC_Init(ADC, &ADC_InitStruct);
    GPIO_AnalogFunctionConfig(CMSDK_PD, GPIO_Pin_3, ENABLE);		//PD03复用为ADC通道(AD-CH5)
    //ADC->CR=0x1430001;
    ADC_SetSampleTime(ADC, 128);
    ADC_ChannelConfig(ADC, ADC_Channel_5);				//ADC转换通道0
    ADC_Cmd(ADC, ENABLE);						//启动ADC外设功能
    while(!ADC_GetFlagStatus(ADC, ADC_FLAG_ADRDY));			//等待ADC启动完成
    ADC_StartOfConversion(ADC);	//启动转换
}

void PWM1_Init(void)
{

    PWM_TimeBaseInitTypeDef  TimeBaseInit;
    PWM_OutputInitTypeDef		OutPutInit;

    GPIO_PinAFConfig(CMSDK_PC, GPIO_PinSource4, GPIO_AF_2);	//PC4 TIM1_CH4	输出  
    TimeBaseInit.ClockSel = PCLK;
    TimeBaseInit.CounterReload = 10000;	// MR0 1MHZ/1000=1000HZ
    TimeBaseInit.Prescaler = 2;		// 8/8=1MHZ
    TimeBaseInit.CounterMode = PWM_CNT_MODE_UP;	//向上计数
    TimeBaseInit.CounterDirRv_EN = PWM_CounterDirRv_DIS;	//计数方向不变
    PMW_TimeBaseInit(PWM1, &TimeBaseInit);

    OutPutInit.Channel = PWM_CH_4;
    OutPutInit.OutputMode = COMPAER_MODE1;		//计数值大于占空比匹配寄存器值，则输出高电平
    OutPutInit.ControlN = PWM_CMR_PWM1n_EN;	//互补输出使能(库函数错误！)
    OutPutInit.IdleState = PWM_IDLE_LOW;	//PWM空闲时，输出0
    OutPutInit.DeathTime = 0;			//死区时间0
    OutPutInit.CompareValue = 0;		//占空比=MR0/MRx
    PWM_OutputInit(PWM1, &OutPutInit);

    PWM_Cmd(PWM1, ENABLE);		//使能定时器
}

void Uart_Init(void)
{
    UART_InitTypeDef  UART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

		GPIO_PinAFConfig(CMSDK_PD, GPIO_PinSource5, GPIO_AF_0);	//PA0 TX0
		GPIO_PinAFConfig(CMSDK_PD, GPIO_PinSource6, GPIO_AF_0);	//PA1 RX0	
	
    /*使能UART0的NVIC控制器*/
    NVIC_InitStruct.NVIC_IRQChannel = UART0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0x00;
    NVIC_Init(&NVIC_InitStruct);

    /*初始化UART0*/
    UART_InitStruct.UART_BaudRate = 9600;
    UART_InitStruct.UART_WordLength = UARTM_8D;
    UART_InitStruct.UART_StopBit = UART1StopBit;
    UART_InitStruct.UART_Parity = UART_ODD_PARITY;
    UART_InitStruct.UART_RXEN = ENABLE;
    UART_InitStruct.UART_SampleRate = UART_SAMPLERATEX16;
    UART_Init(UART0, &UART_InitStruct);

    UART_FifoReset(UART0,  Rx_FIFO);
    UART_FifoReset(UART0,  Tx_FIFO);
    UART_ITConfig(UART0, UART_IT_RXNEIE, ENABLE);
    /*开启UART0的收发功能*/
    UART_Cmd(UART0, ENABLE);
}

void TimeBaseInit(void)
{
    if (SysTick_Config(SystemCoreClock / 1000))	//1ms中断一次
    {
        /* Capture error */
        while (1);
    }
}
