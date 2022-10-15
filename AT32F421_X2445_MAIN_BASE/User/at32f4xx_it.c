/**
  ******************************************************************************
  * File   : Templates/at32f4xx_it.c
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  */
#include "DriverConst.h"
/* Includes ------------------------------------------------------------------*/
#include "at32f4xx_it.h"
GLOBLE_TIMER_1MS Globle_Timer_1MS;
volatile u32 g_uiTimeNumCont1ms = 0;
volatile u32 uiTimeCont = 0;

volatile u32 uiAutoPowerOffTimeCont = 0;//自动关机时间计时器
volatile unsigned char ucTag1ms = FALSE;
volatile unsigned char ucTag5ms = FALSE;
volatile unsigned char ucTag10ms = FALSE;
volatile unsigned char ucTag100ms = FALSE;

volatile unsigned char ucTag300ms = FALSE;
volatile unsigned char ucTag400ms = FALSE;

/** @addtogroup AT32F421_StdPeriph_Templates
  * @{
  */

/** @addtogroup GPIO_LED_Toggle
  * @{
  */

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}
unsigned char ucUartRxData[RecBufferSize] = {0};
unsigned char ucUartRxCont = 0;

#ifdef UsedUartID_UART2
void USART2_IRQHandler(void)
{
    unsigned short usTemp = 0;
    if(USART_GetFlagStatus(EVAL_COM, USART_FLAG_RDNE) != RESET)
    {
        ucUartRxData[ucUartRxCont++] = USART_ReceiveData(EVAL_COM);
        ucUartRxCont %= RecBufferSize;
        vAnalySerialData();//解帧
        uiAutoPowerOffTimeCont = 0;
        USART_ClearFlag(EVAL_COM, USART_FLAG_RDNE);
    }
    if(USART_GetFlagStatus(EVAL_COM, USART_FLAG_ORERR) != RESET)
    {
        usTemp = EVAL_COM->STS;
        usTemp = USART_ReceiveData(EVAL_COM);
        usTemp = usTemp;
        USART_ClearFlag(EVAL_COM, USART_FLAG_ORERR);
    }
}
#else
void USART1_IRQHandler(void)
{
    unsigned short usTemp = 0;
    if(USART_GetFlagStatus(EVAL_COM, USART_FLAG_RDNE) != RESET)
    {
        ucUartRxData[ucUartRxCont++] = USART_ReceiveData(EVAL_COM);
        ucUartRxCont %= RecBufferSize;
        vAnalySerialData();//解帧
        uiAutoPowerOffTimeCont = 0;
        USART_ClearFlag(EVAL_COM, USART_FLAG_RDNE);
    }
    if(USART_GetFlagStatus(EVAL_COM, USART_FLAG_ORERR) != RESET)
    {
        usTemp = EVAL_COM->STS;
        usTemp = USART_ReceiveData(EVAL_COM);
        usTemp = usTemp;
        USART_ClearFlag(EVAL_COM, USART_FLAG_ORERR);
    }
}
#endif

/*
*********************************************************************************************************
*	函 数 名: TMR6_GLOBAL_IRQHandler
*	功能说明: TIMR6中断
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
unsigned char ucAnlyTimeCont = 0;//
#ifdef UsedMlx90393
unsigned char ucsMlx90393Monitor = 0;
unsigned char ucsMlx90393RecNewDataCnt = 0;
#endif
void TMR6_GLOBAL_IRQHandler(void)//10ms一个中断
{

    if (TMR_GetINTStatus(TMR6, TMR_INT_Overflow) != RESET)
    {
        if(PowerOffTime)
        {
            uiAutoPowerOffTimeCont++;
            if(uiAutoPowerOffTimeCont >= PowerOffTime)
            {
                ucErroType = ErroSysPowerOff;
            }
            uiAutoPowerOffTimeCont %= (PowerOffTime + 10);
        }
        if(ucAnlyTimeCont++ >= 5)
        {
            vAnalyTeacherData();
            ucAnlyTimeCont = 0;
        }
        /////////////////////////////////////////////////////////////////////////////////////////
#ifdef UsedMlx90393
				C_MonitorSTM8PinXor;
#endif
        /////////////////////////////////////////////////////////////////////////////////////////
        TMR_ClearITPendingBit(TMR6, TMR_INT_Overflow);
    }
}
/*
*********************************************************************************************************
*	函 数 名: TMR14_GLOBAL_IRQHandler
*	功能说明: TIMR14中断
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
#ifdef UsedMlx90393
void TMR14_GLOBAL_IRQHandler(void)//Mlx90393Used
{
    if (TMR_GetINTStatus(TMR14, TMR_INT_Overflow) != RESET)
    {
			
//#ifdef UsedMlx90393
//        if(g_strMlxInfoData.strStateAndSet.bitStartTimerOpMlx)vReadMlx90393InTimerMode();
//#endif
        TMR_ClearITPendingBit(TMR14, TMR_INT_Overflow);
    }
}
#endif
/*
*********************************************************************************************************
*	函 数 名: TMR6_GLOBAL_IRQHandler
*	功能说明: TIMR6中断
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
unsigned short usCheckRemoteTimeCnt = 0;
void TMR16_GLOBAL_IRQHandler(void)//500us
{
    if (TMR_GetINTStatus(TMR16, TMR_INT_Overflow) != RESET)
    {
        Globle_Timer_1MS.All = 0xFF;
        if(g_uiTimeNumCont1ms)g_uiTimeNumCont1ms--;
        //C_MonitorSTM8PinXor;
        vCheckHardWareErro();
        if(ucEnAdcCalc)vDealAdcVaule();// 1ms
        uiTimeCont++;
			  usCheckRemoteTimeCnt++;
        uiTimeCont %= 100000;
        ucTag1ms = TRUE;
        if(4 == uiTimeCont % 5)ucTag5ms = TRUE;
        if(9 == uiTimeCont % 10)ucTag10ms = TRUE;
        if(99 == uiTimeCont % 100)ucTag100ms = TRUE;
        if(299 == uiTimeCont % 300)ucTag300ms = TRUE;
        if(399 == uiTimeCont % 400)ucTag400ms = TRUE;
			  if(ReadTeachPinIsConect)
				{
					if(++ucInChargePinLowCont >= InChargePinLowCont)ucInChargePinLowCont = InChargePinLowCont;
				}
				else ucInChargePinLowCont = 0;
			  
        TMR_ClearITPendingBit(TMR16, TMR_INT_Overflow);
    }
}
/*
*********************************************************************************************************
*	函 数 名: EXTI15_4_IRQHandler
*	功能说明: 外部中断4-15
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
unsigned char ucDisEnWatchDogFlag = FALSE;//关看门狗，标志变量
unsigned char ucTeacherRxCont  = 0;
void EXTI15_4_IRQHandler(void)
{
    if (EXTI_GetIntStatus(EXTI_Line12) != RESET)
    {
        if(FALSE == ucDisEnWatchDogFlag)//
        {
            vSendSingleOrder(OrderWatchDogClose);
            ucDisEnWatchDogFlag = TRUE;
        }
        ucTeacherRecData[ucTeacherRxCont++] = ucTeacherPortRevByte();
        ucTeacherRxCont %= RecBufferSize;
        uiAutoPowerOffTimeCont = 0;
        CtrlExtSpeekerDis;//强制让外置蜂鸣器不响
        EXTI_ClearIntPendingBit(EXTI_Line12); ///<Clear the  EXTI line 12 pending bit
    }
}
/******************************************************************************/
/*                 AT32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_at32f413_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


/**
  * @}
  */

/**
  * @}
  */

