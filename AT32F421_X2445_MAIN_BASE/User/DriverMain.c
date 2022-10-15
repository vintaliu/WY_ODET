/*
       "LG_X2445M_V2"  Design By LG_qiuzhi
       V1.0   2021.04
       used mcu  AT32F421C8
*/
//file DriverMain.C

#include "DriverConst.h"
unsigned char ucEmgStopEn = 0;//是否有急停事件的发生
unsigned char ucStsTemState = PowerDownMode;//系统状态全局变量
unsigned char ucErroType = ErroNoCheckSys;//系统出错全局变量
unsigned char ucNewDirectCont = SysNoDirection , ucOldDirectCont = SysNoDirection;//
unsigned char ucSpeedGrad = SpeedGrad5;//速度刻度全局变量
unsigned char ucRunAsMode = SysInStandby;//系统的一个工作模式指示变量  刚开机的时候  机器处一个低功耗的模式
unsigned char ucPowerKeyPressTimes = 0x01;//电源按下次数，计数
unsigned char ucShowPowerCont = 0;//电量显示计数器
unsigned char ucInChargePinLowCont = 0;
unsigned char ucGiveVoltageToOldOneTime = FALSE;
unsigned char ucEnAdcCalc = TRUE;
unsigned char ucAutoBattIdenConfigData = 0;//自电池识别配置过标志
unsigned char ucFirstStartOneTimeFlag = FALSE;//关机后变为FALSE，开机后变为TRUE
//unsigned int ucFirstStartOneTime = 0;
#define UseHardWareWatchDog//只为调试方便。
/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: 标准c程序入口
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int main(void)
{
    unsigned char ucShowPowerTemp = 0;
    vInitPortAsWaiteMode();//在刚上电的时候 重新初始化一个端口 让机器一直在等待一个开机按键信号
    if(!ReadKeyOfOn_OffFlag)//如果一直有无开机 信息
    {
        vNoKeyInStandbyMode();//如果刚开机就去进入一个低功耗的模式
    }	
		ucEnAdcCalc = TRUE;
    System_Init();
    vSysTemSourceAsActive();
    //if(C_ReadProgramEnPin)vDealReadProgramPin();//如果是使能在编辑状态 2.06_08取消这个功能复用于检测刹车器
    ucPowerOnCotrol();//上电的一个控制，如果没有按键按下，强行不开机。

    vReadSysTempPare(TRUE);//系统参数的使用。如果读到的不正确，强制重新写入新的初始化参数。。。

    if(ReadTeachPinIsConect)
    {
        vShowPoweChange();//如果开机的时候检测到正在充电，一直等充电完成
    }
    if((SelfCheckMode == ucStsTemState ) || (SysInAutoPowerOn == ucStsTemState )) //如果上电成功就下检查下系统各功能模块
    {
        vCheckSystemInfo();//去检查各模块是否正常地待命
        if(ErroNoCheckSys == ucErroType)ucErroType = ErroNoErro;//如果一切正常，解除初始化的错误标志
    }
    else
    {
        while (1)vAutoPowerOffTimeFlag();//无条件地等错误，不让电机做任何事·
    }
    vSetTeacherPortInt();//只有在没有错误且初始化正确的情况下才使能示教口
    if(ucErroType)vShowErroToDis(0);
#ifdef UseHardWareWatchDog
    //vInitWatchDog(4, 1250);//vInitWatchDog(4, 1250);
#endif
    //		while(1);
    //    vSetMonitorKDPort();//设置监控脚的输入输出状态
    if(FALSE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-刹车器程序或0-驻坡程序由参数表第6项决定20190110
    {
        ucConnectBreakFlag = FALSE;

    }
    //////////////////////////////////////STM32 GD32 UID加密算法的实现
    //        if(ucCrcUidAes());//如果数组正确 可以做下面的事情了
    //        else//如果加密密码不正确，一直在做按键及其他事情
    //        {
    //            while (1)
    //            {
    //                while(ucErroType)//如果硬件有错误，一定去处理
    //                {
    //                    vAutoPowerOffTimeFlag();//如要的任何的错误情况，一定要及时处理
    //                }
    //            }
    //        }
    /////////////////////////////////////
    //		while (1);
    if(FALSE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-刹车器程序或0-驻坡程序由参数表第6项决定20190110
    {
        //驻坡，
        CtlBrakeBikeEn;//复位PC13
        strSysInfo.uiSysBreakFlag = 0;//0 是 驻坡
    }
    else
    {
        CtlBrakeBikeDisEn;//置位PC13
        strSysInfo.uiSysBreakFlag = 1;//1 是电磁刹车
        PwmDataInit();
    }
    if(0 == (ReadMlxReadTimerState & 0x0001))
    {
        EnAbleMlxTimerReadTnterrupt;//使能TMR14定时器
    }		
    CtrlExtSpeekerDis;////强制外置喇叭(PB11)不响
//		while (1)       
//		{
//			 if(ucTag400ms)//如果在空闲的状态下，如果有示教器发过来的数据这时候要去处理下
//        {
//            if(ucAnlyFree)vAnalyTeacherDataOfTime();
//            ucTag400ms = FALSE;
//        }
//		}
    while (1)
    {
        if(ucTag5ms)//看下是否有摇杆变化并对处理
        {
            if(FALSE == ucGiveVoltageToOldOneTime)
            {
                uiOldBattPowerValue = strSysInfo.uiCalcSysShowPower = strSysInfo.uiSysTemPower;//将电量的值 赋值
#ifdef AutoBattIdenConfigDef
                if(0x00 == ucAutoBattIdenConfigData)//如果没有配置电池类型并且电池类型为空
                {
                    if(strSysInfo.uiCalcSysShowPower > 3190)//如果开机的电压高于28.3v说明是锂电  电压比较关系  0.0088623046
                    {
                        ucAutoBattIdenConfigData |= AutoBattIdenConfigAsLion; //配置为锂电
                    }
                    else
                    {
                        ucAutoBattIdenConfigData |= AutoBattIdenConfigAsPb; //配置为铅酸
                    }
                    ucAutoBattIdenConfigData |= AutoBattIdenConfigFlag;//配置过标志
                    BKP_WriteBackupRegister(BKP_DR5, 0xAA55);//写备份寄存器数据标志
                    BKP_WriteBackupRegister(BKP_DR4, ucAutoBattIdenConfigData);//写电池类型标志
                }

                if(AutoBattIdenConfigFlag & ucAutoBattIdenConfigData)//如果已经写电池类型标志，并在下次开的时候电池电压过高时，标记为锂电
                {
                    if(AutoBattIdenConfigAsPb & ucAutoBattIdenConfigData)//如果配置的是铅酸，还是可以再配置的
                    {
                        if(strSysInfo.uiCalcSysShowPower > 3190)//如果开机的电压高于28v说明是锂电  电压比较关系  0.0088623046
                        {
                            ucAutoBattIdenConfigData |= AutoBattIdenConfigAsLion; //配置为锂电
                        }
                    }
                    BKP_WriteBackupRegister(BKP_DR5, 0xAA55);
                    BKP_WriteBackupRegister(BKP_DR4, ucAutoBattIdenConfigData);
                }
                if(AutoBattIdenConfigAsLion & ucAutoBattIdenConfigData)//强制电池类型转换
                {
                    uniDiverInfo.strDirverAtt.unMotorInfo.bits.bLilonOrQiansuan = TRUE;
                    vSendOneByteOrder(OrderBatteryType, 0x01);//写电池类型  2020-03-03
                }
                else
                {
                    uniDiverInfo.strDirverAtt.unMotorInfo.bits.bLilonOrQiansuan = FALSE;
                    vSendOneByteOrder(OrderBatteryType, 0x00);//写电池类型  2020-03-03
                }
#endif
                g_uiTimeNumCont1ms = 20;
                while(g_uiTimeNumCont1ms);
                vSendSysPower(strSysInfo.uiCalcSysShowPower + 20);//首次指示下电量
								g_uiTimeNumCont1ms = 20;
                while(g_uiTimeNumCont1ms);
                vSendSysPower(strSysInfo.uiCalcSysShowPower - 20);//首次指示下电量
                ucGiveVoltageToOldOneTime = TRUE;//只显示一次
                CtrlExtSpeekerDis;//强制不响
                if(ucFirstStartOneTimeFlag == 0)
                {
                    strMoto1Info.uiMotorVoltageBase = (strMoto1Info.uiRightHBridgeHaveFilterAdc + strMoto1Info.uiLeftHBridgeHaveFilterAdc) / 2;
                    strMoto2Info.uiMotorVoltageBase = (strMoto2Info.uiRightHBridgeHaveFilterAdc + strMoto2Info.uiLeftHBridgeHaveFilterAdc) / 2;

                    strMoto1Info.uiMotorCurrentBase = strMoto1Info.uiMotorCurrent;	//如果 电源稳定了，要建立一个电流的基准值
                    strMoto2Info.uiMotorCurrentBase = strMoto2Info.uiMotorCurrent;
                    ucFirstStartOneTimeFlag = 1;
                }
            }
            ucEnAdcCalc = TRUE;//基本实时地开启 AD采样
            if((strSysInfo.uiCalcSysShowPower > CheckLowAlarmVoltage) && (ucFirstStartOneTimeFlag)) //低电压依旧执行刹车过程
            {
                vContolRealize();//控制算法的实现
            }
            else
            {
                PwmDataInit();
            }
            vCalcBataAsShow(FALSE);//如果是在空闲的时候
            ucTag5ms = FALSE;
            if((strMoto1Info.uiMotorCurrent < 500) || (strMoto1Info.uiMotorCurrent > 3500))
            {
                vShowErroToDis(ErroRelayShort);//如果在开机或者在运行过程中采样的电流很小，说明出问题了
            }
            if((strMoto2Info.uiMotorCurrent < 500) || (strMoto2Info.uiMotorCurrent > 3500))
            {
                vShowErroToDis(ErroRelayShort);//如果在开机或者在运行过程中采样的电流很小，说明出问题了
            }
        }
        if(ucTag300ms)
        {
            ucShowPowerCont++;
            ucShowPowerCont %= 100;
            //if((1 == ucShowPowerCont % 5)&&(TRUE == ucConnectBreakFlag))vCheckErroInRun(); //检测如果摇杆正常的情况下，电机的驱动电压的情况
            if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-刹车器程序或0-驻坡程序由参数表第6项决定20190110
            {
                // if(4 == ucShowPowerCont % 5)vCheckErroInRun(); //检测如果摇杆正常的情况下，电机的驱动电压的情况
                ucShowPowerTemp = 9;
            }
            else
            {
                ucShowPowerTemp = 1;
            }
            if(ucShowPowerTemp == ucShowPowerCont % 10)//周期性地检测硬件错误
            {
                vSendSysPower(strSysInfo.uiCalcSysShowPower);//显示下电
            }
            if(ReadTeachPinIsConect)//如果是充电状态
            {
                ucInChargePinLowCont++;
                if(ucInChargePinLowCont >= InChargePinLowCont)
                {
                    vSendSingleOrder(OrderWatchDogClose);//关监控看门狗
                    vShowPoweChange();//让指示在循环指示下
                }
            }
            else ucInChargePinLowCont = 0;
            ucTag300ms = FALSE;
        }
        if(ucTag400ms)//如果在空闲的状态下，如果有示教器发过来的数据这时候要去处理下
        {
            if(ucAnlyFree)vAnalyTeacherDataOfTime();
            ucTag400ms = FALSE;
        }
        while(ucErroType)//如果硬件有错误，一定去处理
        {
            vAutoPowerOffTimeFlag();//如要的任何的错误情况，一定要及时处理
        }
        if(FALSE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-刹车器程序或0-驻坡程序由参数表第6项决定20190110
        {
					  //驻坡，复位PC13
            GPIO_ResetBits(C_BreakControl_Port, C_BreakControl_Pin);
        }
        if((ucSpeedDowenFlag) && (ucFirstStartOneTimeFlag)) //(ucFirstStartOneTimeFlag == TRUE))//&& (FALSE == ucConnectBreakFlag)已经刹车完成，并且检测刹车器已经连接了
        {
            vAutoSlopeStopFuction();
        }
    }
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

/********************************************* End of file **********************************************/
