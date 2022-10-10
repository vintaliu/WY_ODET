/*
       "LG_X2445M_V2"  Design By LG_qiuzhi
       V1.0   2013.6
       used mcu  STM32F103C8(6)x
*/
//file DriverContrl.C

#include "DriverConst.h"
#include "math.h"
void vDealAdcVaule(void);//处理AD
unsigned char  ucPowerOnCotrol(void);//首次
//void vCheckSystemInfo(void);//检查
void vContolRealize(void);//控制的实现
void vRunMotor(unsigned  char ucDirctionData);//电机运转的实现
void vCheckHardWareErro(void);//去检查硬件错误

#define FirstDownPwm 800//第一次减速所到的位置
volatile struct strSysTemInfo strSysInfo = {0};//系统一些状态结构体的定义
volatile struct strMotorInfo strMoto1Info = {0}, strMoto2Info = {0};//两个电机的一些信息  strMoto1Info->X电机   strMoto2Info—>Y电机
volatile unsigned char ucSpeedDowenFlag = FALSE;
// void vDelayms(u32 uiDelayTime);//
void vDelay30us(u32 uiDelayTime);
unsigned char ucLastDirection = NoDirection;//上次摇杆的方向
unsigned char ucMosFetCoolDownTime = 0;//MosFet冷却时间
unsigned char ucAutoRollingFlag = FALSE;
unsigned char ucPowerKeySate = KeyPressDown;//按键状态值的暂存变量，方便消抖动
unsigned char ucKeyUpTime = 0;// 按键消抖用的定时计数器
unsigned int uiCheckMotoBreakTimeCont = 0;//空闲时候 周期检查定时器
union unioBatterLowPowerCom g_unioBatterLowPowerCom = {0};

void vDealAdcVauleMotoAndCurrent(void);
//unsigned char ucBreakDelayTime = 0;
//unsigned int uiBreakPwmTab[3610] = {0};//刹车曲线数组表，只有在开机的时候或者是参数改变的情况下才去计算一下的
/*
延时函数 单位为30us 高精度
*/
int uiAngle = 0;
unsigned char ucFrontAutoSlopFlag = FALSE;//控制器下坡时候 自动驻坡产生标志全局变量
#ifdef STM32
void vSoftDelayms(unsigned int uiDelayTime)
{
    unsigned int uiTemp = 0;
    for(uiTemp = 0; uiTemp < uiDelayTime; uiTemp++)
    {
        vDelay30us(34);//320
    }
}
void vDelay30usAcc(u32 uiDelayTime)//72mHz
{
    u32 uiTemp, uiTemp2;
    for(uiTemp = 0; uiTemp < uiDelayTime; uiTemp++)
    {
        //400
        for(uiTemp2 = 0; uiTemp2 < 1650; uiTemp2++);//2016 09 26 520
    }//20160927 好哥  修改为 620  原 520.如果为520 则车子会有冲创的感觉
}
void vDelay30us(u32 uiDelayTime)//72mHz
{
    u32 uiTemp, uiTemp2;
    for(uiTemp = 0; uiTemp < uiDelayTime; uiTemp++)
    {
        //400
        for(uiTemp2 = 0; uiTemp2 < 410; uiTemp2++);//410
    }
}//于20150505  标定与 V1.7版本一致
void vDelay30usBreak(u32 uiDelayTime)//72mHz
{
    u32 uiTemp, uiTemp2;
    for(uiTemp = 0; uiTemp < uiDelayTime; uiTemp++)  
    {
        //400
        for(uiTemp2 = 0; uiTemp2 < 1500; uiTemp2++);//740
    }//780
}
#endif

//#ifdef HK32
//void vSoftDelayms(unsigned int uiDelayTime)
//{
//    unsigned int uiTemp = 0;
//    for(uiTemp = 0; uiTemp < uiDelayTime; uiTemp++)
//    {
//        vDelay30us(34);//320
//    }
//}
//void vDelay30usAcc(u32 uiDelayTime)//72mHz
//{
//    u32 uiTemp, uiTemp2;
//    for(uiTemp = 0; uiTemp < uiDelayTime; uiTemp++)
//    {
//        //400
//        for(uiTemp2 = 0; uiTemp2 < 380; uiTemp2++);//20190930以前 620
//    }//20160927 好哥  修改为 620  原 520.如果为520 则车子会有冲创的感觉
//}
//void vDelay30us(u32 uiDelayTime)//72mHz
//{
//    u32 uiTemp, uiTemp2;
//    for(uiTemp = 0; uiTemp < uiDelayTime; uiTemp++)
//    {
//        //400
//        for(uiTemp2 = 0; uiTemp2 < 410; uiTemp2++);//410
//    }
//}//于20150505  标定与 V1.7版本一致
//void vDelay30usBreak(u32 uiDelayTime)//72mHz
//{
//    u32 uiTemp, uiTemp2;
//    for(uiTemp = 0; uiTemp < uiDelayTime; uiTemp++)
//    {
//        //400
//        for(uiTemp2 = 0; uiTemp2 < 823; uiTemp2++);//20190930以前 740
//    }//780
//}
//#endif


#ifdef HK32
void vSoftDelayms(unsigned int uiDelayTime)
{
    unsigned int uiTemp = 0;
    for(uiTemp = 0; uiTemp < uiDelayTime; uiTemp++)
    {
        vDelay30us(34);//320
    }
}
void vDelay30usAcc(u32 uiDelayTime)//72mHz
{
    u32 uiTemp, uiTemp2;
    for(uiTemp = 0; uiTemp < uiDelayTime; uiTemp++)
    {
        //400
        for(uiTemp2 = 0; uiTemp2 < 800; uiTemp2++);//20190930以前 620
    }//20160927 好哥  修改为 620  原 520.如果为520 则车子会有冲创的感觉
}
void vDelay30us(u32 uiDelayTime)//72mHz
{
    u32 uiTemp, uiTemp2;
    for(uiTemp = 0; uiTemp < uiDelayTime; uiTemp++)
    {
        //400
        for(uiTemp2 = 0; uiTemp2 < 410; uiTemp2++);//410
    }
}//于20150505  标定与 V1.7版本一致
void vDelay30usBreak(u32 uiDelayTime)//72mHz
{
    u32 uiTemp, uiTemp2;
    for(uiTemp = 0; uiTemp < uiDelayTime; uiTemp++)
    {
        //400
        for(uiTemp2 = 0; uiTemp2 < 1700; uiTemp2++);//20190930以前 740
    }//780
}
#endif


#ifdef GD32
void vSoftDelayms(unsigned int uiDelayTime)
{
    unsigned int uiTemp = 0;
    for(uiTemp = 0; uiTemp < uiDelayTime; uiTemp++)
    {
        vDelay30us(34);//320
    }
}
void vDelay30usAcc(u32 uiDelayTime)//72mHz
{
    u32 uiTemp, uiTemp2;
    for(uiTemp = 0; uiTemp < uiDelayTime; uiTemp++)
    {
        //400
        for(uiTemp2 = 0; uiTemp2 < 1100; uiTemp2++);//410
    }
}
void vDelay30us(u32 uiDelayTime)//72mHz
{
    u32 uiTemp, uiTemp2;
    for(uiTemp = 0; uiTemp < uiDelayTime; uiTemp++)
    {
        //400
        for(uiTemp2 = 0; uiTemp2 < 620; uiTemp2++);//410
    }
}//于20150505  标定与 V1.7版本一致
void vDelay30usBreak(u32 uiDelayTime)//72mHz
{
    u32 uiTemp, uiTemp2;
    for(uiTemp = 0; uiTemp < uiDelayTime; uiTemp++)
    {
        //400
        for(uiTemp2 = 0; uiTemp2 < 1300; uiTemp2++);//  确定刹车 效果的一致性
    }//780
}
#endif

/*
判断遥感需要转动的方向
返回方向值
*/
unsigned char ucCalcDirection(void)
{
    unsigned char ucJudgeDirection = NoDirection;
    if(strSysInfo.uiRemoteNowYPos >= (DriverYRemoteAdcBase + strSysInfo.uiRemoteYP_First))ucJudgeDirection |= JudgeForward;
    if(strSysInfo.uiRemoteNowYPos <= (DriverYRemoteAdcBase - strSysInfo.uiRemoteYN_First))ucJudgeDirection |= JudgeBack;
    if(strSysInfo.uiRemoteNowXPos >= (DriverXRemoteAdcBase + strSysInfo.uiRemoteXP_First))ucJudgeDirection |= JudgeLef;
    if(strSysInfo.uiRemoteNowXPos <= (DriverXRemoteAdcBase - strSysInfo.uiRemoteXN_First))ucJudgeDirection |= JudgeRigt;
    return ucJudgeDirection;
}


/*
在刹车的过程中对电机运行状态的判断
返回一个数值
*/
#define MotoA 0x01
#define MotoB 0x02
unsigned int uiInDownSpeedCalcMotoSate(unsigned char ucMotoAorB, unsigned char ucCalcTimeMs)
{
    unsigned short usMoto_Sample[150] = {0};
    unsigned int uiTemp = 0;
    unsigned int uiCheckTime ;
    unsigned int uiOldMotoPWM = 0;
    if(MotoA == ucMotoAorB)//如果是对电机A的检测
    {
        if((ReDefTIM1CCR1) > 30)//如果是ccr1
        {
            uiOldMotoPWM = ReDefTIM1CCR1;//将原有值记录下来
            SetTim1CC1andCCR2Reg(SetPwmPinHighReg, 0);//不使能电机一下
        }
        else if((ReDefTIM1CCR3) > 30)
        {
            uiOldMotoPWM = ReDefTIM1CCR3;
            SetTim1CC3andCCR4Reg(SetPwmPinHighReg, 0);
        }
#ifdef HK32
        uiCheckTime = ucCalcTimeMs * 53;//时间的重新赋值  //2020-02-24 之前标定
        //uiCheckTime = ucCalcTimeMs * 62;//时间的重新赋值
#endif
#ifdef STM32
        uiCheckTime = ucCalcTimeMs * 65;//时间的重新赋值---AT32F421
#endif
        for(uiTemp = 0; uiTemp < uiCheckTime; uiTemp++)
        {
            vDealAdcVauleMotoAndCurrent();
            if((ReDefTIM1CCR1) > 30)usMoto_Sample[uiTemp] = strMoto1Info.uiRightHBridgeHaveFilterAdc;
            if((ReDefTIM1CCR3) > 30)usMoto_Sample[uiTemp] = strMoto1Info.uiLeftHBridgeHaveFilterAdc;
        }
        if((ReDefTIM1CCR1) > 30)
        {
            ReDefTIM1CCR1 = ReDefTIM1CCR2 = uiOldMotoPWM;//及时地还原数据
        }
        if((ReDefTIM1CCR3) > 30)
        {
            ReDefTIM1CCR3 = ReDefTIM1CCR4 = uiOldMotoPWM;//及时地还原数据
        }
        uiOldMotoPWM = 0;
        for(uiTemp = 0; uiTemp < uiCheckTime; uiTemp++)
        {
            uiOldMotoPWM += usMoto_Sample[uiTemp];
        }
        uiOldMotoPWM /= uiCheckTime;
        return(uiOldMotoPWM);
    }
    /////////////////////////////////////
    if(MotoB == ucMotoAorB)//如果是对电机A的检测
    {
        if((ReDefTIM4CCR1) > 30)//如果是ccr1
        {
            uiOldMotoPWM = ReDefTIM4CCR1;//将原有值记录下来
            SetTim4CC1andCCR2Reg(SetPwmPinHighReg, 0);//不使能电机一下
        }
        else if((ReDefTIM4CCR3) > 30)
        {
            uiOldMotoPWM = ReDefTIM4CCR3;
            SetTim4CC3andCCR4Reg(SetPwmPinHighReg, 0);
        }
        uiCheckTime = ucCalcTimeMs * 60;//时间的重新赋值
        for(uiTemp = 0; uiTemp < uiCheckTime; uiTemp++)
        {
            vDealAdcVauleMotoAndCurrent();
            if((ReDefTIM4CCR1) > 30)usMoto_Sample[uiTemp] = strMoto2Info.uiRightHBridgeHaveFilterAdc;
            if((ReDefTIM4CCR3) > 30)usMoto_Sample[uiTemp] = strMoto2Info.uiLeftHBridgeHaveFilterAdc;
        }
        if((ReDefTIM4CCR1) > 30)
        {
            ReDefTIM4CCR1 = ReDefTIM4CCR2 = uiOldMotoPWM;//及时地还原数据
        }
        if((ReDefTIM4CCR3) > 30)
        {
            ReDefTIM4CCR3 = ReDefTIM4CCR4 = uiOldMotoPWM;//及时地还原数据
        }
        uiOldMotoPWM = 0;
        for(uiTemp = 0; uiTemp < uiCheckTime; uiTemp++)
        {
            uiOldMotoPWM += usMoto_Sample[uiTemp];
        }
        uiOldMotoPWM /= uiCheckTime;
        return(uiOldMotoPWM);
    }
    return(uiOldMotoPWM);
}
unsigned short usInBreakMotoA_VoltageCon[30] = {0};
unsigned short usInBreakMotoB_VoltageCon[30] = {0};

/////////////////////////////////////////////////////////////
#define UsedPwmTab
//#define UsedStep
//#define UsedOld
//#endif
#ifdef UsedOld
void vDownSpeedCalcPwm(unsigned char ucFlag)//智能刹车
{
    unsigned char ucDirectionX = FALSE;
    unsigned char ucDirectionY = FALSE;
    u32 uiTemp, uiStepX = 0, uiStepY = 0; //
    u32 uiDeltSteps = 0;
    u32 uiOldPwmX = 0, uiOldPwmY = 0;
    unsigned int uiBreakDelayAct = 0;
    if(Moto1AStmPwmReg)//计算此时电机A的当前 PWM值
    {
        ucDirectionX = Forward;//确定上次的值与方向。
        uiOldPwmX = Moto1AStmPwmReg;
    }
    else
    {
        ucDirectionX = Reverse;
        uiOldPwmX = Moto1BStmPwmReg;
    }
    if(Moto2AStmPwmReg)//计算此时电机B的当前 PWM值
    {
        ucDirectionY = Forward;
        uiOldPwmY = Moto2AStmPwmReg;
    }
    else
    {
        ucDirectionY = Reverse;
        uiOldPwmY = Moto2BStmPwmReg;
    }
    if(uiOldPwmX > (uniDiverInfo.strDirverAtt.ucStopMinPwm * 10))
    {
        for(uiTemp = 0 ; uiTemp < TimerBase; uiTemp++)//找出当前时刻对应的，速度位置
        {
            if((uiBreakPwmTab[uiTemp + 1] <= uiOldPwmX) && (uiBreakPwmTab[uiTemp] >= uiOldPwmX))break;
        }
        uiOldPwmX = uiTemp;
        uiStepX = 3600 - uiTemp;//算出要减速的步数
    }
    else
    {
        uiStepX = uiOldPwmX = 0;
        //return;
    }

    if(uiOldPwmY > (uniDiverInfo.strDirverAtt.ucStopMinPwm * 10))//如果不够启动
    {
        for(uiTemp = 0 ; uiTemp < TimerBase; uiTemp++)
        {
            if((uiBreakPwmTab[uiTemp + 1] <= uiOldPwmY) && (uiBreakPwmTab[uiTemp] >= uiOldPwmY))break;
        }
        uiOldPwmY = uiTemp;
        uiStepY = 3600 - uiTemp;//算出要减速的步数
    }
    else
    {
        uiStepY = uiOldPwmY = 0;
        //return;
    }
    if(uiStepX > uiStepY)uiDeltSteps = uiStepX;
    else uiDeltSteps = uiStepY;//计算 哪一个电机要求刹车的时间更好长一点
    uiBreakDelayAct = uniDiverInfo.strDirverAtt.unDelayBreakActTime.all * ((float)(uiDeltSteps / 3600.0)) + 50;//动态计算，此次要延时关刹车器的时间
    for(uiTemp = 0; uiTemp < uiDeltSteps; uiTemp++)
    {
        if(uiStepX > 0)
        {
            uiStepX -- ;
            if(Forward == ucDirectionX)
            {
                CtlSetMotor1LeftPwmPercent(uiBreakPwmTab[uiOldPwmX + uiTemp]);
                CtlSetMotor1RightPwmPercent(0);
            }//看是正反转
            if(Reverse == ucDirectionX)
            {
                CtlSetMotor1LeftPwmPercent(0);
                CtlSetMotor1RightPwmPercent(uiBreakPwmTab[uiOldPwmX + uiTemp]);
            }
        }
        if(uiStepY > 0)
        {
            uiStepY -- ;
            if(Forward == ucDirectionY)
            {
                CtlSetMotor2LeftPwmPercent(uiBreakPwmTab[uiOldPwmY + uiTemp]);
                CtlSetMotor2RightPwmPercent(0);
            }
            if(Reverse == ucDirectionY)
            {
                CtlSetMotor2LeftPwmPercent(0);
                CtlSetMotor2RightPwmPercent(uiBreakPwmTab[uiOldPwmY + uiTemp]);
            }
        }
        vDelay30usBreak(uniDiverInfo.strDirverAtt.ucBreakAccPare);
        if(ErroNoErro == ucErroType)
        {
            ucNewDirectCont = ucCalcDirection();//如果在刹车的过程中一但有电机转动，立即退出当前的状态
            if(SysNoDirection != ucNewDirectCont) return;
        }
    }
    if(ucFlag)
    {
        for(uiTemp = 0; uiTemp < uiBreakDelayAct; uiTemp++)
        {
            vDelay30us(33);
            if(SysNoDirection != ucCalcDirection()) return;
        }
    }
    CtlSetMotor1RightPwmPercent(0);
    CtlSetMotor1LeftPwmPercent(0);
    CtlSetMotor2RightPwmPercent(0);
    CtlSetMotor2LeftPwmPercent(0);

    ucSpeedDowenFlag = TRUE;//已经刹车标志
}
#endif
////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
unsigned char ucSpeedDowenRuningFlag = FALSE;
//unsigned char ucSlopeRuningFlag = FALSE;
#ifdef UsedStep
#define  Fast_Step 0.95
#define  Slow_Step 0.96
float Speed_Step = Slow_Step;//PWM对应的减速度
double uiOldPwmX = 0, uiOldPwmY = 0;
double uiSetPwmX = 0, uiSetPwmY = 0;
unsigned int uiTextTime = 0;
unsigned int uiGlobleSpeedDowenTime = 0;
void vDownSpeedCalcPwm(unsigned char ucFlag)//智能刹车
{
    unsigned int ucBreakAccPare = 0;
    if(g_uiGlobleSpeedDowenTime1ms > 0)
    {
        uiGlobleSpeedDowenTime++;
        g_uiGlobleSpeedDowenTime1ms = 0;
        if((uniDiverInfo.strDirverAtt.ucBreakAccPare > 5 ) && (uniDiverInfo.strDirverAtt.ucBreakAccPare < 10))
        {
            ucBreakAccPare = uniDiverInfo.strDirverAtt.ucBreakAccPare;
            Speed_Step = Slow_Step;
        }
        else
        {
            ucBreakAccPare = uniDiverInfo.strDirverAtt.ucBreakAccPare;
            Speed_Step = Fast_Step;
            if(uniDiverInfo.strDirverAtt.ucBreakAccPare < 3 )
            {
                ucBreakAccPare = 1;
            }
        }
        if(ucSpeedDowenRuningFlag == FALSE)
        {
            if((((ReDefTIM1CCR1) > 50) || ((ReDefTIM1CCR3) > 50) || ((ReDefTIM4CCR1) > 50) || ((ReDefTIM4CCR3) > 50)) && (uiSetPwmX == 0) && (uiSetPwmY == 0))
            {
                ucSpeedDowenRuningFlag = TRUE;
            }
        }
        if(ucSpeedDowenRuningFlag)
        {

            if(uiGlobleSpeedDowenTime > ucBreakAccPare)
            {
                uiGlobleSpeedDowenTime = 0;
                if(uiTextTime == 0)
                {
                    CtrlExtSpeekerEn;
                    uiTextTime = 1;
                }
                else
                {
                    CtrlExtSpeekerDis;//不再这里处理了
                    uiTextTime = 0;
                }

                if(Moto2AStmPwmReg)//计算此时电机B的当前 PWM值
                {
                    uiOldPwmY = Moto2AStmPwmReg;
                }
                else
                {
                    uiOldPwmY = Moto2BStmPwmReg;
                }
                if(Moto1AStmPwmReg)//计算此时电机A的当前 PWM值
                {
                    uiOldPwmX = Moto1AStmPwmReg;
                }
                else
                {
                    uiOldPwmX = Moto1BStmPwmReg;
                }
                uiSetPwmX = uiOldPwmX * Speed_Step;
                uiSetPwmY = uiOldPwmY * Speed_Step;
                uiOldPwmX = uiSetPwmX;
                uiOldPwmY = uiSetPwmY;
                if((uiSetPwmX > 2.0) || (uiSetPwmY > 2.0))
                    //if(((ReDefTIM1CCR1) > 0) || ((ReDefTIM1CCR3) > 0) || ((ReDefTIM4CCR1) > 0) || ((ReDefTIM4CCR3) > 0))
                {
                    if(uiSetPwmX > strSysInfo.uiControlMaxXDriverPwm)
                        uiSetPwmX = strSysInfo.uiControlMaxXDriverPwm;
                    if(uiSetPwmY > strSysInfo.uiControlMaxYDriverPwm)
                        uiSetPwmY = strSysInfo.uiControlMaxXDriverPwm;
                    if(Moto1AStmPwmReg)
                    {
                        CtlSetMotor1LeftPwmPercent(uiSetPwmX);
                    }
                    else
                    {
                        CtlSetMotor1RightPwmPercent(uiSetPwmX);
                    }
                    if(Moto2AStmPwmReg)
                    {
                        CtlSetMotor2LeftPwmPercent(uiSetPwmY);
                    }
                    else
                    {
                        CtlSetMotor2RightPwmPercent(uiSetPwmY);
                    }
                }
                else
                {
                    uiOldPwmX = 0;
                    uiOldPwmY = 0;
                    uiSetPwmX = 0;
                    uiSetPwmY = 0;
                    CtlSetMotor1RightPwmPercent(0);
                    CtlSetMotor1LeftPwmPercent(0);
                    CtlSetMotor2RightPwmPercent(0);
                    CtlSetMotor2LeftPwmPercent(0);
                    ucSpeedDowenRuningFlag = FALSE;
                }
            }
        }
        else
        {
            ucSpeedDowenFlag = TRUE;//已经刹车标志
        }
    }
    if(SysNoDirection != ucCalcDirection()) return;
}
#endif
#ifdef UsedPwmTab


#ifdef HK32
unsigned int uiStartCalcPwmDataValue = 3910;//3300;  2020-03-24
//unsigned int uiStartCalcPwmDataValue = 3850;//3300;
////unsigned int uiStartCalcPwmDataValue = 3890;//3300;  2020-02-22
//unsigned int uiStartCalcPwmDataValue = 3990;//3300;  2020-02-22
#endif
#ifdef STM32
unsigned int uiStartCalcPwmDataValue = 3300;//3300;  2020-03-24  雅特力程序专用
//unsigned int uiStartCalcPwmDataValue = 3970;//3300;  2020-02-22
#endif

unsigned char g_ucLowPwmIntoBeadk = FALSE; //小PWM进入刹车的时候
u32  ucCalcBrakeMotoACurrent = 0;
u32  ucCalcBrakeMotoBCurrent = 0;
void vDownSpeedCalcPwm(unsigned char ucFlag)//智能刹车
{

    unsigned char ucDirectionX = FALSE;
    unsigned char ucDirectionY = FALSE;
    unsigned char ucCalcBrakeMotoACurrentFlag = FALSE;//开始计算
    unsigned char ucCalcBrakeMotoBCurrentFlag = FALSE;
    unsigned int uiCalcMotoStateTimeCont_A = 0;//
    unsigned int uiCalcMotoStateTimeCont_B = 0;
    u32 uiTemp, uiStepX = 0, uiStepY = 0; //
    u32 uiDeltSteps = 0;
    u32 uiOldPwmX = 0, uiOldPwmY = 0;
    unsigned int uiBreakDelayAct = 0;
    //unsigned short usInBreakMotoA_VoltageCon[30] = {0};
    //unsigned short usInBreakMotoB_VoltageCon[30] = {0};
    unsigned char ucMotoA_SampleTimesCont = 0;
    unsigned char ucMotoB_SampleTimesCont = 0;
    unsigned char uiStepXAsTIM1OrTIM4 = 0;
    unsigned char uiStepYAsTIM1OrTIM4 = 0;
    unsigned char ucSlopFlag = FALSE;
    //				unsigned int uiStartCalcPwmDataValue = 2000;//3300;
    unsigned char ucLowPwmInBreak_MA = FALSE;
    unsigned char ucLowPwmInBreak_MB = FALSE;
    //				unsigned char uiTimeCont = 0;
    ucCalcBrakeMotoACurrentFlag = ucCalcBrakeMotoBCurrentFlag = FALSE;
    ucSlopFlag = uiCalcMotoStateTimeCont_A = uiCalcMotoStateTimeCont_B = 0;
    //				uiStartCalcPwmDataValue = 800;//4000;//201812

    if(Moto1AStmPwmReg)//计算此时电机A的当前 PWM值
    {
        ucDirectionX = Forward;//确定上次的值与方向。
        uiOldPwmX = Moto1AStmPwmReg;
    }
    else
    {
        ucDirectionX = Reverse;
        uiOldPwmX = Moto1BStmPwmReg;
    }

    if(Moto2AStmPwmReg)//计算此时电机B的当前 PWM值
    {
        ucDirectionY = Forward;
        uiOldPwmY = Moto2AStmPwmReg;
    }
    else
    {
        ucDirectionY = Reverse;
        uiOldPwmY = Moto2BStmPwmReg;
    }
    if(uiOldPwmX > (uniDiverInfo.strDirverAtt.ucStopMinPwm * 10))
    {
        for(uiTemp = 0 ; uiTemp < TimerBase; uiTemp++)//找出当前时刻对应的，速度位置
        {
            if((uiBreakPwmTab[uiTemp + 1] <= uiOldPwmX) && (uiBreakPwmTab[uiTemp] >= uiOldPwmX))break;
        }
        uiOldPwmX = uiTemp;
        uiStepX = 3600 - uiTemp;//算出要减速的步数
    }
    else
    {
        uiStepX = uiOldPwmX = 0;
    }

    if(uiOldPwmY > (uniDiverInfo.strDirverAtt.ucStopMinPwm * 10))//如果不够启动
    {
        for(uiTemp = 0 ; uiTemp < TimerBase; uiTemp++)

        {
            if((uiBreakPwmTab[uiTemp + 1] <= uiOldPwmY) && (uiBreakPwmTab[uiTemp] >= uiOldPwmY))break;
        }
        uiOldPwmY = uiTemp;
        uiStepY = 3600 - uiTemp;//算出要减速的步数
    }
    else
    {
        uiStepY = uiOldPwmY = 0;
    }
    g_ucLowPwmIntoBeadk = FALSE;
    if((ReDefTIM1CCR1) && (ReDefTIM1CCR1 < 500))
    {
        g_ucLowPwmIntoBeadk = TRUE;
    }
    else if((ReDefTIM1CCR3) && (ReDefTIM1CCR3 < 500))
    {
        g_ucLowPwmIntoBeadk = TRUE;
    }
    else if((ReDefTIM4CCR1) && (ReDefTIM4CCR1 < 500))
    {
        g_ucLowPwmIntoBeadk = TRUE;
    }
    else if((ReDefTIM4CCR3) && (ReDefTIM4CCR3 < 500))
    {
        g_ucLowPwmIntoBeadk = TRUE;
    }
    if(((ReDefTIM1CCR1) > 50) || ((ReDefTIM1CCR3) > 50) || ((ReDefTIM4CCR1) > 50) || ((ReDefTIM4CCR3) > 50))
    {
        //for(uiTemp = 0; uiTemp < 1600; uiTemp++)usTable[uiTemp] = uiBreakPwmTab[2048 + uiTemp];
        ucEnAdcCalc = FALSE;
        g_uiTimeNumCont1ms = 2;
        while(g_uiTimeNumCont1ms);

        if((uiMotor1LeftHighPwmRegAdd == Time1Ccr1Add) || (uiMotor1LeftHighPwmRegAdd == Time1Ccr3Add))uiStepXAsTIM1OrTIM4 = TIM1_UsedAsX;
        if((uiMotor1LeftHighPwmRegAdd == Time4Ccr1Add) || (uiMotor1LeftHighPwmRegAdd == Time4Ccr3Add))uiStepXAsTIM1OrTIM4 = TIM4_UsedAsY;

        if((uiMotor2LeftHighPwmRegAdd == Time1Ccr1Add) || (uiMotor2LeftHighPwmRegAdd == Time1Ccr3Add))uiStepYAsTIM1OrTIM4 = TIM1_UsedAsX;
        if((uiMotor2LeftHighPwmRegAdd == Time4Ccr1Add) || (uiMotor2LeftHighPwmRegAdd == Time4Ccr3Add))uiStepYAsTIM1OrTIM4 = TIM4_UsedAsY;

        //        if(((ReDefTIM1CCR1) < 550) && ((ReDefTIM1CCR3) < 550))//如果电机A的刹车PWM小于20% 则不再进行一次计算了
        //        {
        //            ucCalcBrakeMotoACurrentFlag = TRUE;
        //            ucLowPwmInBreak_MA = TRUE;
        //        }
        //        if(((ReDefTIM4CCR1) < 550) && ((ReDefTIM4CCR3) < 550))
        //        {
        //            ucCalcBrakeMotoBCurrentFlag = TRUE;
        //            ucLowPwmInBreak_MB = TRUE;
        //        }
        for(uiTemp = 0; uiTemp < 30; uiTemp++)
        {
            usInBreakMotoA_VoltageCon[uiTemp] = 0;
            usInBreakMotoB_VoltageCon[uiTemp] = 0;
        }

    }
    //				if(strMoto1Info.unMotorOldDirection != strMoto2Info.unMotorOldDirection)//如果转向不一致 201812
    //				{
    //						//如果在转向的时候，就将阈值设置小一点。就不让在平路或者坡道上停下来。一直减速
    //						uiStartCalcPwmDataValue = 800;
    //				}

    //    if(uiOldPwmX < 3550)uiOldPwmX += 3;
    //    if(uiOldPwmY < 3550)uiOldPwmY += 3;
    if(uiStepX > uiStepY)uiDeltSteps = uiStepX;
    else uiDeltSteps = uiStepY;//计算 哪一个电机要求刹车的时间更好长一点
    uiBreakDelayAct = uniDiverInfo.strDirverAtt.unDelayBreakActTime.all * ((float)(uiDeltSteps / 3600.0)) + 50;//动态计算，此次要延时关刹车器的时间
    for(uiTemp = 0; uiTemp < uiDeltSteps; uiTemp++)
    {
        if(uiStepX > 0)//如果是电X的减速情况
        {
            uiStepX -- ;
            if(Forward == ucDirectionX)
            {
                if(uiBreakPwmTab[uiOldPwmX + uiTemp] > strSysInfo.uiControlMaxXDriverPwm)//限幅输出
                {
                    CtlSetMotor1LeftPwmPercent(strSysInfo.uiControlMaxXDriverPwm);
                }
                else
                {
                    CtlSetMotor1LeftPwmPercent(uiBreakPwmTab[uiOldPwmX + uiTemp]);
                }
                CtlSetMotor1RightPwmPercent(0);
            }//看是正反转
            if(Reverse == ucDirectionX)
            {
                CtlSetMotor1LeftPwmPercent(0);
                if(uiBreakPwmTab[uiOldPwmX + uiTemp] > strSysInfo.uiControlMaxXDriverPwm)//限幅输出
                {
                    CtlSetMotor1RightPwmPercent(strSysInfo.uiControlMaxXDriverPwm);
                }
                else
                {
                    CtlSetMotor1RightPwmPercent(uiBreakPwmTab[uiOldPwmX + uiTemp]);
                }
            }

        }
        if(uiStepY > 0)
        {
            uiStepY -- ;
            if(Forward == ucDirectionY)
            {
                if(uiBreakPwmTab[uiOldPwmY + uiTemp] > strSysInfo.uiControlMaxYDriverPwm)//限幅输出
                {
                    CtlSetMotor2LeftPwmPercent(strSysInfo.uiControlMaxYDriverPwm);
                }
                else
                {
                    CtlSetMotor2LeftPwmPercent(uiBreakPwmTab[uiOldPwmY + uiTemp]);
                }
                CtlSetMotor2RightPwmPercent(0);
            }
            if(Reverse == ucDirectionY)
            {
                CtlSetMotor2LeftPwmPercent(0);
                if(uiBreakPwmTab[uiOldPwmY + uiTemp] > strSysInfo.uiControlMaxYDriverPwm)//限幅输出
                {
                    CtlSetMotor2RightPwmPercent(strSysInfo.uiControlMaxYDriverPwm);
                }
                else
                {
                    CtlSetMotor2RightPwmPercent(uiBreakPwmTab[uiOldPwmY + uiTemp]);
                }

            }

        }
        //////////////////////在这里要查看下电机在转运过程中的一些状态情况不对在，以上的语句中处理////////////////////////
        if(FALSE == ucCalcBrakeMotoACurrentFlag)//在这里要判断下电机是否有停下来的情况
        {
            if((ReDefTIM1CCR1) > 50)//如果是电机A的情况
            {
                if((ReDefTIM1CCR1) < CalSlopPWM)//20%的PWM
                {
                    ucCalcBrakeMotoACurrentFlag = TRUE;
                    ucMotoA_SampleTimesCont++;
                    usInBreakMotoA_VoltageCon[ucMotoA_SampleTimesCont] = uiInDownSpeedCalcMotoSate(MotoA, 2);
                }
            }
            if((ReDefTIM1CCR3) > 50)
            {
                if((ReDefTIM1CCR3) < CalSlopPWM)//20%的PWM
                {
                    ucCalcBrakeMotoACurrentFlag = TRUE;
                    ucMotoA_SampleTimesCont++;
                    usInBreakMotoA_VoltageCon[ucMotoA_SampleTimesCont] = uiInDownSpeedCalcMotoSate(MotoA, 2);
                }
            }
        }
        if(TRUE == ucCalcBrakeMotoACurrentFlag)
        {
            uiCalcMotoStateTimeCont_A++;
            if(uiCalcMotoStateTimeCont_A > 5000)uiCalcMotoStateTimeCont_A = 0;
            if(uiCalcMotoStateTimeCont_A > 210)
            {
                ucMotoA_SampleTimesCont++;
                if(ucMotoA_SampleTimesCont < 25)//防止数组溢出
                {
                    usInBreakMotoA_VoltageCon[ucMotoA_SampleTimesCont] = uiInDownSpeedCalcMotoSate(MotoA, 1);
                }
                uiCalcMotoStateTimeCont_A = 0;
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        if(FALSE == ucCalcBrakeMotoBCurrentFlag)//在这里要判断下电机是否有停下来的情况
        {
            if((ReDefTIM4CCR1) > 50)
            {
                if((ReDefTIM4CCR1) < CalSlopPWM)//20%的PWM
                {
                    ucMotoB_SampleTimesCont++;
                    ucCalcBrakeMotoBCurrentFlag = TRUE;
                    usInBreakMotoB_VoltageCon[ucMotoB_SampleTimesCont] = uiInDownSpeedCalcMotoSate(MotoB, 2);
                }
            }
            if((ReDefTIM4CCR3) > 50)
            {
                if((ReDefTIM4CCR3) < CalSlopPWM)//20%的PWM
                {
                    ucMotoB_SampleTimesCont++;
                    ucCalcBrakeMotoBCurrentFlag = TRUE;
                    usInBreakMotoB_VoltageCon[ucMotoB_SampleTimesCont] = uiInDownSpeedCalcMotoSate(MotoB, 2);
                }
            }
        }
        if(TRUE == ucCalcBrakeMotoBCurrentFlag)
        {
            uiCalcMotoStateTimeCont_B++;
            if(uiCalcMotoStateTimeCont_B > 5000)uiCalcMotoStateTimeCont_B = 0;
            if(uiCalcMotoStateTimeCont_B > 210)
            {
                ucMotoB_SampleTimesCont++;
                if(ucMotoB_SampleTimesCont < 25)//防止数组溢出
                {
                    usInBreakMotoB_VoltageCon[ucMotoB_SampleTimesCont] = uiInDownSpeedCalcMotoSate(MotoB, 1);
                }
                uiCalcMotoStateTimeCont_B = 0;
            }
        }
        ////////////////////////////////////////////////////////////////////////////
        if((ucMotoA_SampleTimesCont >= 2) && (usInBreakMotoA_VoltageCon[ucMotoA_SampleTimesCont] > 0))
        {
            if((usInBreakMotoA_VoltageCon[ucMotoA_SampleTimesCont] < uiStartCalcPwmDataValue) && (usInBreakMotoA_VoltageCon[ucMotoA_SampleTimesCont + 1] < uiStartCalcPwmDataValue))
            {
                if(strMoto1Info.unMotorOldDirection == strMoto2Info.unMotorOldDirection == Forward)//如果转向一致 20190218
                {
                    if((ReDefTIM1CCR1) > 50)//如果是电机A的情况
                    {
                        if((ReDefTIM1CCR1) < IntoSlopPWM)//MaxSlopPwm)//20%的PWM
                        {
                            //															ucCalcBrakeMotoACurrent= 0;
                            //															ucCalcBrakeMotoBCurrent = 0;
                            //															for(uiTimeCont = 0; uiTimeCont < OneChannelConvertTimes; uiTimeCont++)
                            //															{
                            //																ucCalcBrakeMotoACurrent+= vDmaContAdcVaule[uiTimeCont][Motor1DriveCurrent];
                            //																ucCalcBrakeMotoBCurrent+= vDmaContAdcVaule[uiTimeCont][Motor2DriveCurrent];
                            //															}
                            //															ucCalcBrakeMotoACurrent /= OneChannelConvertTimes;
                            //															ucCalcBrakeMotoBCurrent /= OneChannelConvertTimes;
                            //															 if(((ucCalcBrakeMotoACurrent>IfMotoACurrentAtStep1High)&&(ucCalcBrakeMotoBCurrent > IfMotoBCurrentAtStep2High))||
                            //															 ((ucCalcBrakeMotoBCurrent> IfMotoBCurrentAtStep1High)&&(ucCalcBrakeMotoACurrent > IfMotoACurrentAtStep2High)))
                            //																{
                            if(TIM1_UsedAsX == uiStepXAsTIM1OrTIM4)	uiStepX = 0;
                            if(TIM4_UsedAsY == uiStepXAsTIM1OrTIM4)	uiStepY = 0;
                            ucSlopFlag = TRUE;
                            break;
                            //																}
                        }
                    }
                    if((ReDefTIM1CCR3) > 50)
                    {
                        if((ReDefTIM1CCR3) < IntoSlopPWM)//MaxSlopPwm)//20%的PWM
                        {

                            //															ucCalcBrakeMotoACurrent= 0;
                            //															ucCalcBrakeMotoBCurrent = 0;
                            //															for(uiTimeCont = 0; uiTimeCont < OneChannelConvertTimes; uiTimeCont++)
                            //															{
                            //																ucCalcBrakeMotoACurrent+= vDmaContAdcVaule[uiTimeCont][Motor1DriveCurrent];
                            //																ucCalcBrakeMotoBCurrent+= vDmaContAdcVaule[uiTimeCont][Motor2DriveCurrent];
                            //															}
                            //															ucCalcBrakeMotoACurrent /= OneChannelConvertTimes;
                            //															ucCalcBrakeMotoBCurrent /= OneChannelConvertTimes;
                            //															 if(((ucCalcBrakeMotoACurrent>IfMotoACurrentAtStep1High)&&(ucCalcBrakeMotoBCurrent > IfMotoBCurrentAtStep2High))||
                            //															 ((ucCalcBrakeMotoBCurrent> IfMotoBCurrentAtStep1High)&&(ucCalcBrakeMotoACurrent > IfMotoACurrentAtStep2High)))
                            //																{
                            if(TIM1_UsedAsX == uiStepXAsTIM1OrTIM4)	uiStepX = 0;
                            if(TIM4_UsedAsY == uiStepXAsTIM1OrTIM4)	uiStepY = 0;
                            ucSlopFlag = TRUE;
                            break;
                            //																}
                        }
                    }
                }
            }
        }

        if((ucMotoB_SampleTimesCont >= 2) && (usInBreakMotoB_VoltageCon[ucMotoB_SampleTimesCont] > 0))
        {
            if((usInBreakMotoB_VoltageCon[ucMotoB_SampleTimesCont] < uiStartCalcPwmDataValue) && (usInBreakMotoB_VoltageCon[ucMotoB_SampleTimesCont + 1] < uiStartCalcPwmDataValue))
                //								if((usInBreakMotoB_VoltageCon[ucMotoB_SampleTimesCont] >IfMotoBSampleAdAtStep3High) && (usInBreakMotoB_VoltageCon[ucMotoB_SampleTimesCont - 1] >IfMotoBSampleAdAtStep3High))
            {
                if(strMoto1Info.unMotorOldDirection == strMoto2Info.unMotorOldDirection == Forward)//如果转向一致 20190218
                {
                    if((ReDefTIM4CCR1) > 50)//如果是电机A的情况
                    {
                        if((ReDefTIM4CCR1) < IntoSlopPWM)//MaxSlopPwm)//20%的PWM
                        {

                            //															ucCalcBrakeMotoACurrent= 0;
                            //															ucCalcBrakeMotoBCurrent = 0;
                            //															for(uiTimeCont = 0; uiTimeCont < OneChannelConvertTimes; uiTimeCont++)
                            //															{
                            //																ucCalcBrakeMotoACurrent+= vDmaContAdcVaule[uiTimeCont][Motor1DriveCurrent];
                            //																ucCalcBrakeMotoBCurrent+= vDmaContAdcVaule[uiTimeCont][Motor2DriveCurrent];
                            //															}
                            //															ucCalcBrakeMotoACurrent /= OneChannelConvertTimes;
                            //															ucCalcBrakeMotoBCurrent /= OneChannelConvertTimes;
                            //															 if(((ucCalcBrakeMotoACurrent>IfMotoACurrentAtStep1High)&&(ucCalcBrakeMotoBCurrent > IfMotoBCurrentAtStep2High))||
                            //															 ((ucCalcBrakeMotoBCurrent> IfMotoBCurrentAtStep1High)&&(ucCalcBrakeMotoACurrent > IfMotoACurrentAtStep2High)))
                            //																{
                            if(TIM1_UsedAsX == uiStepYAsTIM1OrTIM4)	uiStepX = 0;
                            if(TIM4_UsedAsY == uiStepYAsTIM1OrTIM4)	uiStepY = 0;
                            ucSlopFlag = TRUE;
                            break;
                            //																}
                        }
                    }
                    if((ReDefTIM4CCR3) > 50)
                    {
                        if((ReDefTIM4CCR3) < IntoSlopPWM)//MaxSlopPwm)//20%的PWM
                        {
                            //															ucCalcBrakeMotoACurrent= 0;
                            //															ucCalcBrakeMotoBCurrent = 0;
                            //															for(uiTimeCont = 0; uiTimeCont < OneChannelConvertTimes; uiTimeCont++)
                            //															{
                            //																ucCalcBrakeMotoACurrent+= vDmaContAdcVaule[uiTimeCont][Motor1DriveCurrent];
                            //																ucCalcBrakeMotoBCurrent+= vDmaContAdcVaule[uiTimeCont][Motor2DriveCurrent];
                            //															}
                            //															ucCalcBrakeMotoACurrent /= OneChannelConvertTimes;
                            //															ucCalcBrakeMotoBCurrent /= OneChannelConvertTimes;
                            //															 if(((ucCalcBrakeMotoACurrent>IfMotoACurrentAtStep1High)&&(ucCalcBrakeMotoBCurrent > IfMotoBCurrentAtStep2High))||
                            //															 ((ucCalcBrakeMotoBCurrent> IfMotoBCurrentAtStep1High)&&(ucCalcBrakeMotoACurrent > IfMotoACurrentAtStep2High)))
                            //																{

                            if(TIM1_UsedAsX == uiStepYAsTIM1OrTIM4)	uiStepX = 0;
                            if(TIM4_UsedAsY == uiStepYAsTIM1OrTIM4)	uiStepY = 0;
                            ucSlopFlag = TRUE;
                            break;
                            //																}
                        }
                    }
                }

            }
        }
        ////////////////////////////////////////////////////////////////////////////
        vDelay30usBreak(uniDiverInfo.strDirverAtt.ucBreakAccPare);
        if(ErroNoErro == ucErroType)
        {
					  strSysInfo.uiRemoteNowYPos = vDmaContAdcVaule[0][RemoteYAdcCh];//DMA直接采样的数据  
					  strSysInfo.uiRemoteNowXPos = vDmaContAdcVaule[0][RemoteXAdcCh];//DMA直接采样的数据  
            ucNewDirectCont = ucCalcDirection();//如果在刹车的过程中一但有电机转动，立即退出当前的状态
            //if(strSysInfo.uiCalcSysShowPower > CheckLowAlarmVoltage)//低电压依旧执行刹车过程
            {
                if(SysNoDirection != ucNewDirectCont) return;
            }
        }
    }
    if(TRUE == ucSlopFlag)//如果有驻坡的时候
    {
        ucSpeedDowenFlag = TRUE;//已经刹车标志
        ucEnAdcCalc = TRUE;
        if((TRUE == ucLowPwmInBreak_MA) || (TRUE == ucLowPwmInBreak_MB))//如果进入的PWM过小了
        {
            if(ReDefTIM1CCR1 > 50)
            {
                uiStepX = ReDefTIM1CCR1;
            }
            else uiStepX = ReDefTIM1CCR3;
            if(ReDefTIM4CCR1 > 50)
            {
                uiStepY = ReDefTIM4CCR1;
            }
            else uiStepY = ReDefTIM4CCR3;
            if(uiStepX >= uiStepY)uiDeltSteps = uiStepX;
            else uiDeltSteps = uiStepY;
            if(uiDeltSteps > 10)uiDeltSteps -= 10;
            for(uiTemp = 0; uiTemp < uiDeltSteps; uiTemp++)//在这里做一个减速过度一下
            {
                if(ReDefTIM1CCR1 > 6)
                {
                    ReDefTIM1CCR1 -= 5;
                    ReDefTIM1CCR2 = ReDefTIM1CCR1;
                }
                if(ReDefTIM1CCR3 > 6)
                {
                    ReDefTIM1CCR3 -= 5;
                    ReDefTIM1CCR4 = ReDefTIM1CCR3;
                }
                if(ReDefTIM4CCR1 > 6)
                {
                    ReDefTIM4CCR1 -= 5;
                    ReDefTIM4CCR2 = ReDefTIM4CCR1;
                }
                if(ReDefTIM4CCR3 > 6)
                {
                    ReDefTIM4CCR3 -= 5;
                    ReDefTIM4CCR4 = ReDefTIM4CCR3;
                }
                g_uiTimeNumCont1ms = 1;
                while(g_uiTimeNumCont1ms);
                {
                    ucNewDirectCont = ucCalcDirection();//如果在刹车的过程中一但有电机转动，立即退出当前的状态
                    if(SysNoDirection != ucNewDirectCont) return;
                }
            }
            ReDefTIM1CCR1 = ReDefTIM1CCR2 = ReDefTIM1CCR3 = ReDefTIM1CCR4 = 0;
            ReDefTIM4CCR1 = ReDefTIM4CCR2 = ReDefTIM4CCR3 = ReDefTIM4CCR4 = 0;
            uiCheckMotoBreakTimeCont = 0;//防止在刹车后，对驻坡的起作用
        }
        if(ReDefTIM1CCR1 > 50)//防止过小
        {
            if(ReDefTIM1CCR1 < LowSlopPWM)//230  20190216
            {
                ReDefTIM1CCR1 = ReDefTIM1CCR2 = 0;
            }
        }
        if(ReDefTIM1CCR3 > 50)//防止过小
        {
            if(ReDefTIM1CCR3 < LowSlopPWM)//230
            {
                ReDefTIM1CCR3 = ReDefTIM1CCR4 = 0;
            }
        }
        if(ReDefTIM4CCR1 > 50)//防止过小
        {
            if(ReDefTIM4CCR1 < LowSlopPWM)//230
            {
                ReDefTIM4CCR1 = ReDefTIM4CCR2 = 0;
            }
        }
        if(ReDefTIM4CCR3 > 50)//防止过小
        {
            if(ReDefTIM4CCR3 < LowSlopPWM)//230
            {
                ReDefTIM4CCR3 = ReDefTIM4CCR4 = 0;//防止在刹车后，对驻坡的起作用
            }
        }
        return;
    }
    ucEnAdcCalc = TRUE;
    if(FALSE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-刹车器程序或0-驻坡程序由参数表第6项决定20190110
    {
        uiBreakDelayAct = 0;//在驻坡控制器中，这个参数可以设置小一点
    }
    if(ucFlag)
    {
        for(uiTemp = 0; uiTemp < uiBreakDelayAct; uiTemp++)
        {
            vDelay30us(33);
            if(SysNoDirection != ucCalcDirection()) return;
        }
        uiCheckMotoBreakTimeCont = 0;
    }
    //////////////////////////////////////////////////////////////////////////
    if(FALSE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//如果没有刹车器
    {
        if(strMoto1Info.unMotorOldDirection != strMoto2Info.unMotorOldDirection)//如果转向不一致的情况
        {
            CtlSetMotor1RightPwmPercent(0);
            CtlSetMotor1LeftPwmPercent(0);
            CtlSetMotor2RightPwmPercent(0);
            CtlSetMotor2LeftPwmPercent(0);//
            for(uiTemp = 0; uiTemp < 300; uiTemp++)
            {
                vDelay30us(33);
                if(SysNoDirection != ucCalcDirection()) return;
            }
            strMoto2Info.unMotorOldDirection = strMoto1Info.unMotorOldDirection = NoDirection;//这里清转向的定义
        }
    }
    CtlSetMotor1RightPwmPercent(0);
    CtlSetMotor1LeftPwmPercent(0);
    CtlSetMotor2RightPwmPercent(0);
    CtlSetMotor2LeftPwmPercent(0);
    ucSpeedDowenFlag = TRUE;//已经刹车标志
    //		}
}
#endif
unsigned char ucRunChangeDirDeal(void)
{
    unsigned char ucTemp = 0;
    ucNewDirectCont = ucCalcDirection();
    if(ucNewDirectCont != ucOldDirectCont)
    {
        ucTemp = ucNewDirectCont & ucOldDirectCont;
        ucOldDirectCont = ucNewDirectCont;
        if((ucTemp == JudgeBack) || (ucTemp == JudgeForward) );
        else
        {
            vDownSpeedCalcPwm(FALSE) ;
            return 1;
        }
        return 0;
    }
    else
    {
        ucOldDirectCont = ucNewDirectCont;
        return 0;
    }
}



unsigned int uiPowerValueData = 0;
unsigned int uiPowerValueDataConter = 0; //电池电量累加容量器，电池电量次数累加器
unsigned int uiOldBattPowerValue = 0;

#define BattPowerAddTimes              500//在静止状态下，多少时间更新一次电量值  采集数据的样本容量
#define ShowPowerInRunTimes        2000  //26000   //在运行的状态下的，采样样本的大小
WORK_STATE_u Work_Status;
#define CheckLowBattContTime  8000
///////////////////////////////////////////////////////
void vCalcBataAsShow(unsigned char ucShowPowerInRunFlag)//在控制的时候 对电量的指示
{
    //如果是在运行的状态下的函数
    unsigned int uiTemp = 0;
    unsigned int uiTempTime = 0;
    static unsigned short usCheckLowBattAdcSum = 0;
    if(uniDiverInfo.strDirverAtt.unMotorInfo.bits.bLilonOrQiansuan)//锂电池与铅酸电池的区分
    {	
        if(strSysInfo.uiSysTemPower >= 3240)uiTemp = 3240;   //如果是铅酸电池，且是压大于25.3v 再高的电压值不再记录
        else if(strSysInfo.uiSysTemPower <= 2150)uiTemp = 2150;   //如果是铅酸电池，且电压小于23.5v 再高的电压值不再记录
        else uiTemp = strSysInfo.uiSysTemPower;
#ifndef UsedLinSuanTieLi					
        if(strSysInfo.uiSysTemPower <= 2530)//当电压低于22.3v的时候并且大于5秒就去关机
        {
            usCheckLowBattAdcSum++;
            if(usCheckLowBattAdcSum >= CheckLowBattContTime)
            {
                usCheckLowBattAdcSum = CheckLowBattContTime;
                ucErroType = ErroLowVoltage;
            }
        }
        else usCheckLowBattAdcSum = 0;
#endif				
    }
    else //如果是铅酸电池
    {
        if(strSysInfo.uiSysTemPower >= 2950)uiTemp = 2950;   //如果是铅酸电池，且是压大于25.3v 再高的电压值不再记录
        else if(strSysInfo.uiSysTemPower <= 2150)uiTemp = 2150;   //如果是铅酸电池，且电压小于22v 再高的电压值不再记录
        else uiTemp = strSysInfo.uiSysTemPower;//如果没有 超出一定的范围  就去将当时的电量赋值下
    }
    uiPowerValueDataConter++;//样本采样计数器
    uiPowerValueData += uiTemp;//样本容量
    if(TRUE == ucShowPowerInRunFlag)
    {
        if( Work_Status.bit.Runing == 0 )
        {
            Work_Status.bit.Slop = 0;
            Work_Status.bit.Ready = 0;
            Work_Status.bit.Runing = 1;
            uiPowerValueData = uiPowerValueDataConter = 0;//清空计数器
        }
        uiTempTime = ShowPowerInRunTimes;
    }
    else
    {
        if( Work_Status.bit.Ready == 0 )
        {
            Work_Status.bit.Slop = 0;
            Work_Status.bit.Ready = 1;
            Work_Status.bit.Runing = 0;
            uiPowerValueData = uiPowerValueDataConter = 0;//清空计数器
        }
        uiTempTime = BattPowerAddTimes;//判断在何种状态下的，的采样模式
    }
    if(uiPowerValueDataConter > uiTempTime)//如果采样次数到了
    {
        uiPowerValueData /= uiTempTime;//取一个均值-

        if(uniDiverInfo.strDirverAtt.unMotorInfo.bits.bLilonOrQiansuan)
        {
            uiOldBattPowerValue = (unsigned int)((float)(uiPowerValueData * 0.012) + (float)(uiOldBattPowerValue * 0.988));
        }
        else
        {
            uiOldBattPowerValue = (unsigned int)((float)(uiPowerValueData * 0.022) + (float)(uiOldBattPowerValue * 0.978));
        }
        strSysInfo.uiCalcSysShowPower = uiOldBattPowerValue;//更新一下计算出来的电量值
        uiPowerValueData = uiPowerValueDataConter = 0;//清空计数器
        //        if(strSysInfo.uiSysTemPower < CheckLowAlarmVoltage)//如果电压在18v以上 才去正确地指示了
        //        {
        //            strSysInfo.uiCalcSysShowPower =  strSysInfo.uiSysTemPower;//如果是低电压的情况 主动更数值。不再经过运算
        //        }

        if(strSysInfo.uiCalcSysShowPower < CheckLowAlarmVoltage)//如果电压在18v以上 才去正确地指示了  2020-02-22
        {
            strSysInfo.uiCalcSysShowPower =  strSysInfo.uiSysTemPower;//如果是低电压的情况 主动更数值。不再经过运算
        }

        //if(ucShowPowerInRunFlag)//如果是在运行的状态下 更新一次电量指示
        {
            vSendSysPower(strSysInfo.uiCalcSysShowPower);
        }
    }
}


void vContolRealize(void)//控制的实现
{
#define ErroPosTime 30//摇杆消抖的时间
    unsigned int uiTemp;
    ucNewDirectCont = ucCalcDirection();
    ucRunChangeDirDeal();
    if((SysNoDirection == ucNewDirectCont) && (ucFrontAutoSlopFlag == FALSE))vDownSpeedCalcPwm(FALSE); //20181215
    if(TRUE == ucSpeedDowenFlag )
    {
        if(TRUE == ucLastDirection)vSendSingleOrder(BikeOutBack);
        ucLastDirection = FALSE;
        ucNewDirectCont = ucCalcDirection();
        //				if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-刹车器程序或0-驻坡程序由参数表第6项决定20190529
        //				{
        //						if(SysNoDirection == ucNewDirectCont)
        //								CtlBrakeBikeDisEn;
        //			  }
    }
    if(SysNoDirection != ucNewDirectCont)
    {
        uiAutoPowerOffTimeCont = 0;
        //        if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)CtlBrakeBikeEn;
        for(uiTemp = 0; uiTemp < ErroPosTime; uiTemp++)
        {
            //如果方向有抖动
            ucTag10ms = FALSE;
            while(FALSE == ucTag10ms);
            ucNewDirectCont = ucCalcDirection();
            if(SysNoDirection == ucNewDirectCont)break;
        }
        if((uiTemp + 2) > (ErroPosTime))
        {
            ucSpeedDowenFlag = FALSE;
            if(ucAutoRollingFlag)//如果车子在溜车情况下
            {
                vSendOneByteOrder(OrderSpeekP, 0x02);//Alarm Test Is End
                ucAutoRollingFlag = FALSE;
            }
            uiPowerValueData = uiPowerValueDataConter = 0;//进入转动的时候  清电池电压计数器
            vRunMotor(ucNewDirectCont); //去转动
            //vDownSpeedCalcPwm(TRUE);//防止程序的嵌套调用
            uiPowerValueData = uiPowerValueDataConter = 0;//转动退出的时候  清电池电压计数器
            ucShowPowerCont = FALSE;//让电池电压稳定了再去显示下
            ucTag300ms = FALSE;//让电量指示的更平稳一点
            ucMosFetCoolDownTime = 0;
        }
        else
        {
            //            if(TRUE == ucSpeedDowenFlag )CtlBrakeBikeDisEn;20190529
        }
    }
}
unsigned short  uiOverCurrentCont = 0;   //超流计数器
void vRunMotor(unsigned char ucDirctionData)
{
#ifdef UsedYingKe50A
#define MaxContinueOutCurrent  2180 //最大持续输出电流，不管堵转多长时间都有电流输出
#else
#define MaxContinueOutCurrent  1850 //最大持续输出电流，不管堵转多长时间都有电流输出	
#endif
#define CurrentReduceDeltTime  80//逐级降电流，时间间隔
#define MaxCurrentHoldLonTime  5000//最大电流持续时间
#define AlarmAnlge 40
#define HighAlarmAngle 360 - AlarmAnlge
#define LowAlarmAngle 180 + AlarmAnlge
#define AlarmMinYValue  1400
#define ErrAngle 0.2//当有一个轴为其正轴的时候
#define FastAccTime 6
#define MiddleAccTime 6//中间的速度值
#define LowAccTime    6
#define ErroLeftAndRightPwmP  1 //当方向为第一二象限的时候调节比例
#define UsedMoto1AndMotor2P//如果使用电机1与电机2的互锁关系
    float fStep1SpeedPare, fStep2SpeedPare, fStep3SpeedPare, fStep4SpeedPare;//五档速度标量
    float fStep1BackSwSpeedPare, fStep2BackSwSpeedPare, fStep3BackSwSpeedPare, fStep4BackSwSpeedPare;//五档速度标量 后退与转弯
    unsigned int uiErroPare;
    unsigned char ucOverCurrentAskQuit = FALSE;
    unsigned int uiAngleTemp;
    u32 uiTempKpDelayCont = 0;
    u32 uiDeltSteps;
    u32 uiPwmXDelt = 0;
    u32 uiPwmYDelt = 0;
    u32 uiReMoteXTemp = 0;
    u32 uiReMoteYTemp = 0;
    u32 uiTemp, uiStepX, uiStepY; //
    s32 iDeltPwmX, iDeltPwmY;
    u8 ucDecIncSpeedX, ucDecIncSpeedY;
    u8 ucDirectionX = NoDirection, ucDirectionY = NoDirection;
    float fTemp;
    unsigned int uiAccTime  = uniDiverInfo.strDirverAtt.ucDriverAcc;//20140718  实时加速度值
    s32 iTempPwmX, iTempPwmY;//
    unsigned char ucNeedChangDirXFlag = FALSE;
    unsigned char ucNeedChangDirYFlag = FALSE; //是否要换向标志
    unsigned char ucRelationMoto1Mtor2 = FALSE;
    s32 uiMotor1AndMotor2P;//两个电机相互控制比例
    float fLowPowerRunMotoCom = 0;//当低电压的时候电量的补偿
    unsigned int uiStartPwm = 0;//启动PWM
    unsigned char ucOpenBrakeFlag = FALSE;
    unsigned short usMaxDriverCurrent = 2510;//最大驱动电流
    unsigned short usMaxDriverCurrentOld = 0;//最大驱动电流
    unsigned int uiOverCurrentCont = 0;   //超流计数器
    unsigned int uiOverCurrentCntNextReachTime = 0;   //超流计数器  下次超过的时间
    usMaxDriverCurrent = 2510;//最大驱动电流//给2435的电流

#ifdef UsedYingKe50A
    if(35 == uniDiverInfo.strDirverAtt.ucMaxDriverCurrent)usMaxDriverCurrent = 2510;//如果编程器的设置是35A
    else if(45 == uniDiverInfo.strDirverAtt.ucMaxDriverCurrent)usMaxDriverCurrent = 3800;//如果编程器的设置是45A
    else if(50 == uniDiverInfo.strDirverAtt.ucMaxDriverCurrent)usMaxDriverCurrent = 3800;////如果编程器的设置是50A
    else if(uniDiverInfo.strDirverAtt.ucMaxDriverCurrent > 30)usMaxDriverCurrent = 3800;//如果是沿用以前的设置方法
#else
    if(35 == uniDiverInfo.strDirverAtt.ucMaxDriverCurrent)usMaxDriverCurrent = 2510;//如果编程器的设置是35A
    else if(45 == uniDiverInfo.strDirverAtt.ucMaxDriverCurrent)usMaxDriverCurrent = 3000;//如果编程器的设置是45A
    else if(50 == uniDiverInfo.strDirverAtt.ucMaxDriverCurrent)usMaxDriverCurrent = 3000;////如果编程器的设置是50A
    else if(uniDiverInfo.strDirverAtt.ucMaxDriverCurrent > 30)usMaxDriverCurrent = 3000;//如果是沿用以前的设置方法
#endif

    uiOverCurrentCont = 0;
    usMaxDriverCurrentOld = usMaxDriverCurrent;
    uiOverCurrentCntNextReachTime = MaxCurrentHoldLonTime;
    fTemp = (1 - (uniDiverInfo.strDirverAtt.ucForwardMinSpeedP * 0.01)) / 4.0;
    fStep4SpeedPare = 1 - fTemp;
    fStep3SpeedPare = 1 - 2 * fTemp;
    fStep2SpeedPare = 1 - 3 * fTemp;
    fStep1SpeedPare = 1 - 4 * fTemp;//进入主程序，先算出几个速度标量的分度值

    if(uniDiverInfo.strDirverAtt.ucSwerveMaxSpeedP > uniDiverInfo.strDirverAtt.ucSwerveMinSpeedP)
    {
        iDeltPwmX = uniDiverInfo.strDirverAtt.ucSwerveMaxSpeedP - uniDiverInfo.strDirverAtt.ucSwerveMinSpeedP;
    }
    else iDeltPwmX = uniDiverInfo.strDirverAtt.ucSwerveMinSpeedP - uniDiverInfo.strDirverAtt.ucSwerveMaxSpeedP;
    fTemp = iDeltPwmX * 0.005;
    fStep4BackSwSpeedPare = 1 - fTemp;
    fStep3BackSwSpeedPare = 1 - 2 * fTemp;
    fStep2BackSwSpeedPare = 1 - 3 * fTemp;
    fStep1BackSwSpeedPare = 1 - 4 * fTemp;//进入主程序，先算出几个速度标量的分度值
    //uiErroPare = uniDiverInfo.strDirverAtt.ucLetRightMotorTiedP * 40;//
    uiErroPare = uniDiverInfo.strDirverAtt.ucLetRightMotorTiedP * 700;//700
    ucNewDirectCont = ucCalcDirection();
    if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-刹车器程序或0-驻坡程序由参数表第6项决定20190110
    {
        //uiStartPwm = uniDiverInfo.strDirverAtt.ucStarMinPwm * 36;//计算下开启刹车器的最小PWM
        uiStartPwm = strSysInfo.uiCalcPwmInBreak;//记录的上次值
        if(uiStartPwm > (uniDiverInfo.strDirverAtt.ucStarMinPwm * 36))uiStartPwm = uniDiverInfo.strDirverAtt.ucStarMinPwm * 36;
        ucOpenBrakeFlag = FALSE;
    }
    while(SysNoDirection != ucNewDirectCont)
    {
        uiAutoPowerOffTimeCont = 0;
        ucEnAdcCalc = TRUE;//基本实时地开启 AD采样
        if(ucErroType)
        {
            vAutoPowerOffTimeFlag();
            return;
        }
        if(strSysInfo.uiRemoteNowYPos < 2600)//如果摇杆归位了
        {
            usMaxDriverCurrent = usMaxDriverCurrentOld;//最大驱动电流还原
            uiOverCurrentCont = 0;//过电流计数器清空
            uiOverCurrentCntNextReachTime = MaxCurrentHoldLonTime;//逐级减电流计数器，再次置位
        }
        if(uniDiverInfo.strDirverAtt.ucEnIternSpeaker)
        {
            if((strSysInfo.uiLastRemotValue > LowAlarmAngle) && (strSysInfo.uiLastRemotValue < HighAlarmAngle) && (strSysInfo.uiRemoteNowYPos < AlarmMinYValue))
            {
                if(FALSE == ucLastDirection)vSendSingleOrder(BikeInBack);
                ucLastDirection = TRUE;
            }
            else
            {
                if(TRUE == ucLastDirection)vSendSingleOrder(BikeOutBack);
                ucLastDirection = FALSE;
            }
        }
        if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//带刹车器程序20190110
        {
            if(FALSE == ucOpenBrakeFlag)//如果没有开启刹车器的情况
            {
                if((ReDefTIM1CCR1 > uiStartPwm) || (ReDefTIM1CCR3 > uiStartPwm) || (ReDefTIM4CCR1 > uiStartPwm) || (ReDefTIM4CCR3 > uiStartPwm))
                {
                    //如果一但达到启动条件
                    CtlBrakeBikeEn;
                    ucOpenBrakeFlag = TRUE;//不再开启了
                }
            }
        }
        vCalcBataAsShow(TRUE);//动态计算出电池电压 的真实值
        ucNewDirectCont = ucCalcDirection();
        uiPwmYDelt = 0;
        uiPwmXDelt = 0;
        iTempPwmX = XDriverPwmDelt;//因iTempPwmX在后面的地方还会用到，故此赋值
        iTempPwmY = YDriverPwmDelt;//
        ucNeedChangDirXFlag = FALSE;
        ucNeedChangDirYFlag = FALSE;//初始化变量
        uiReMoteXTemp = strSysInfo.uiRemoteNowXPos;//当前要处理的摇杆值，可与在控制过程中如果摇杆值有变再次作插补
        uiReMoteYTemp = strSysInfo.uiRemoteNowYPos;
        for(uiTemp = 0 ; uiTemp < 40; uiTemp++)//算出当前应该控制电机的转速
        {
            if((uiReMoteXTemp >= (strSysInfo.uiRemoteMiddleXPos + uiTemp * strSysInfo.uiRemoteXP_Delt + strSysInfo.uiRemoteXP_First)) \
                    && (uiReMoteXTemp < (strSysInfo.uiRemoteMiddleXPos + (uiTemp + 1)*strSysInfo.uiRemoteXP_Delt + strSysInfo.uiRemoteXP_First)))
            {
                uiPwmXDelt = strSysInfo.uiControlMinXDriverPwm + (iTempPwmX * (uiTemp + 1));//算出X_PWM等级
            }//X正转的控制
            else if((uiReMoteXTemp >= (strSysInfo.uiRemoteMiddleXPos  -  (uiTemp + 1)*strSysInfo.uiRemoteXN_Delt - strSysInfo.uiRemoteXN_First)) \
                    && (uiReMoteXTemp < (strSysInfo.uiRemoteMiddleXPos - uiTemp * strSysInfo.uiRemoteXN_Delt - strSysInfo.uiRemoteXN_First)))
            {
                uiPwmXDelt = strSysInfo.uiControlMinXDriverPwm + (iTempPwmX * (uiTemp + 1));//算出X_PWM等级
            }//X反转的控制
            if(uiReMoteXTemp > strSysInfo.uiRemoteMaxXPos)
            {
                uiPwmXDelt = strSysInfo.uiControlMaxXDriverPwm;
            }
            if(uiReMoteXTemp < (strSysInfo.uiRemoteMinXPos ))
            {
                uiPwmXDelt = strSysInfo.uiControlMaxXDriverPwm;
            }
            if((uiReMoteYTemp >= (strSysInfo.uiRemoteMiddleYPos + uiTemp * strSysInfo.uiRemoteYP_Delt + strSysInfo.uiRemoteYP_First)) \
                    && (uiReMoteYTemp < (strSysInfo.uiRemoteMiddleYPos + (uiTemp + 1)*strSysInfo.uiRemoteYP_Delt + strSysInfo.uiRemoteYP_First)))
            {
                uiPwmYDelt = strSysInfo.uiControlMinYDriverPwm + (iTempPwmY * (uiTemp + 1));//算出Y_PWM等级Y
            }//Y正转的控制
            else if((uiReMoteYTemp >= (strSysInfo.uiRemoteMiddleYPos  -  (uiTemp + 1)*strSysInfo.uiRemoteYN_Delt - strSysInfo.uiRemoteYN_First)) \
                    && (uiReMoteYTemp < (strSysInfo.uiRemoteMiddleYPos - uiTemp * strSysInfo.uiRemoteYN_Delt - strSysInfo.uiRemoteYN_First)))
            {
                uiPwmYDelt = strSysInfo.uiControlMinYDriverPwm + (iTempPwmY * (uiTemp + 1));//算出Y_PWMPWM等级
            }//Y反转的控制
            if(uiReMoteYTemp > strSysInfo.uiRemoteMaxYPos)
            {
                uiPwmYDelt = strSysInfo.uiControlMaxYDriverPwm;
            }
            if(uiReMoteYTemp < (strSysInfo.uiRemoteMinYPos))
            {
                uiPwmYDelt = strSysInfo.uiControlMaxYDriverPwm;
            }
        }//找出摇杆值的一个准确位置
        if((strSysInfo.uiLastRemotValue < 150) && (strSysInfo.uiLastRemotValue > 30))//如果是在15-165度中间的值，速度按键有效，否则无效
        {
            fLowPowerRunMotoCom = 1;
            if(g_unioBatterLowPowerCom.bitSet.bitLowBarSetp1 )
            {
                fLowPowerRunMotoCom = 0.75;
            }
            else if(g_unioBatterLowPowerCom.bitSet.bitLowBarSetp2 )
            {
                fLowPowerRunMotoCom = 0.60;
            }
            else if(g_unioBatterLowPowerCom.bitSet.bitLowBarSetp3 )
            {
                fLowPowerRunMotoCom = 0.05;
            }
            else if(g_unioBatterLowPowerCom.bitSet.bitLowBarSetp4 )
            {
                fLowPowerRunMotoCom = 0.04;
            }
            else fLowPowerRunMotoCom = 1;

            if(SpeedGrad5 == ucSpeedGrad)	//速度标量分度
            {
                uiPwmXDelt = uiPwmXDelt * fLowPowerRunMotoCom;
                uiPwmYDelt = uiPwmYDelt * fLowPowerRunMotoCom;
            }
            else if(SpeedGrad4 == ucSpeedGrad)
            {
                uiPwmXDelt = uiPwmXDelt * fStep4SpeedPare * fLowPowerRunMotoCom;
                uiPwmYDelt = uiPwmYDelt * fStep4SpeedPare * fLowPowerRunMotoCom;
            }
            else if(SpeedGrad3 == ucSpeedGrad)
            {
                uiPwmXDelt = uiPwmXDelt * fStep3SpeedPare * fLowPowerRunMotoCom;
                uiPwmYDelt = uiPwmYDelt * fStep3SpeedPare * fLowPowerRunMotoCom;
            }
            else if(SpeedGrad2 == ucSpeedGrad)
            {
                uiPwmXDelt = uiPwmXDelt * fStep2SpeedPare * fLowPowerRunMotoCom;
                uiPwmYDelt = uiPwmYDelt * fStep2SpeedPare * fLowPowerRunMotoCom;
            }
            else if(SpeedGrad1 == ucSpeedGrad)
            {
                uiPwmXDelt = uiPwmXDelt * fStep1SpeedPare * fLowPowerRunMotoCom;
                uiPwmYDelt = uiPwmYDelt * fStep1SpeedPare * fLowPowerRunMotoCom;
            }
        }//20170605
        else
        {
            fLowPowerRunMotoCom = 1;
            if(g_unioBatterLowPowerCom.bitSet.bitLowBarSetp1 )
            {
                fLowPowerRunMotoCom = 0.75;
            }
            else if(g_unioBatterLowPowerCom.bitSet.bitLowBarSetp2 )
            {
                fLowPowerRunMotoCom = 0.60;
            }
            else if(g_unioBatterLowPowerCom.bitSet.bitLowBarSetp3 )
            {
                fLowPowerRunMotoCom = 0.05;
            }
            else if(g_unioBatterLowPowerCom.bitSet.bitLowBarSetp4 )
            {
                fLowPowerRunMotoCom = 0.04;
            }
            else fLowPowerRunMotoCom = 1;//在任何方向 的摇杆都的机会降速的

            if(SpeedGrad5 == ucSpeedGrad);	//速度标量分度
            else if(SpeedGrad4 == ucSpeedGrad)
            {
                uiPwmXDelt = uiPwmXDelt * fStep4BackSwSpeedPare;
                uiPwmYDelt = uiPwmYDelt * fStep4BackSwSpeedPare;
            }
            else if(SpeedGrad3 == ucSpeedGrad)
            {
                uiPwmXDelt = uiPwmXDelt * fStep3BackSwSpeedPare;
                uiPwmYDelt = uiPwmYDelt * fStep3BackSwSpeedPare;
            }
            else if(SpeedGrad2 == ucSpeedGrad)
            {
                uiPwmXDelt = uiPwmXDelt * fStep2BackSwSpeedPare;
                uiPwmYDelt = uiPwmYDelt * fStep2BackSwSpeedPare;
            }
            else if(SpeedGrad1 == ucSpeedGrad)
            {
                uiPwmXDelt = uiPwmXDelt * fStep1BackSwSpeedPare;
                uiPwmYDelt = uiPwmYDelt * fStep1BackSwSpeedPare;
            }
        }
        //        if(JudgeForward == (ucDirctionData & JudgeForward))//如果是在正方向上。
        //        {
        //            if(uiPwmXDelt > uiPwmYDelt)//算出死角误差
        //            {
        //                fTemp = (float)(uiPwmYDelt / (uiPwmXDelt * 1.0));//如果发现Y轴有过小的现象
        //                if(fTemp < ErrAngle) ucDirctionData &= (~JudgeForward);
        //            }
        //            else
        //            {
        //                fTemp = (float)(uiPwmXDelt / (uiPwmYDelt * 1.0));   //如果有发现X有过小的现象
        //                if(fTemp < ErrAngle) ucDirctionData = JudgeForward;
        //            }
        //        }	  //死角控制
        //        if(JudgeBack == (ucDirctionData & JudgeBack))
        //        {
        //            if(uiPwmXDelt > uiPwmYDelt)//算出死角误差
        //            {
        //                fTemp = (float)(uiPwmYDelt / (uiPwmXDelt * 1.0));
        //                if(fTemp < ErrAngle) ucDirctionData &= (~JudgeBack);
        //            }
        //            else
        //            {
        //                fTemp = (float)(uiPwmXDelt / (uiPwmYDelt * 1.0));
        //                if(fTemp < ErrAngle)
        //                {
        //                    ucDirctionData = JudgeBack;
        //                    if(strSysInfo.uiRemoteNowYPos < (strSysInfo.uiRemoteMinYPos + iTempPwmY))
        //                    {
        //                        uiPwmYDelt = strSysInfo.uiControlMaxYDriverPwm;
        //                    }
        //                }
        //            }
        //        }	  //死角控制
        switch(ucNewDirectCont)//判断摇杆在哪一个象限，并计算出对应的PWM的值
        {
        case (JudgeRigt | JudgeForward)://第一象限
        {
            uiTempKpDelayCont =  sqrt((uiPwmXDelt * uiPwmXDelt) + (uiPwmYDelt * uiPwmYDelt));
            fTemp = (uiPwmYDelt * 1.0) / ( uiTempKpDelayCont * 1.0);
            uiAngleTemp = (unsigned int)(asin( fTemp) * 57.296);

            if(FourQuadrantPosAsPwmArryMoto1_A[uiAngleTemp])
            {
                strMoto1Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto1_A[uiAngleTemp] ;
                strMoto1Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto1Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxXDriverPwm;
                ucDirectionX = Forward;	   //找出数组中对应的值并确正电机的正反转
            }
            else
            {
                strMoto1Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto1_B[uiAngleTemp] ;
                strMoto1Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto1Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxXDriverPwm;
                ucDirectionX = Reverse;	  //找出数组中对应的值并确正电机的正反转
            }
            if(FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp])
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp] ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Forward;		  //找出数组中对应的值并确正电机的正反转
            }
            else
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_B[uiAngleTemp] ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Reverse;		//找出数组中对应的值并确正电机的正反转
            }
            if(strMoto1Info.uiNewMotorPwmValue > strMoto2Info.uiNewMotorPwmValue)strMoto2Info.uiNewMotorPwmValue *= ErroLeftAndRightPwmP;
            else strMoto1Info.uiNewMotorPwmValue *= ErroLeftAndRightPwmP;
            strSysInfo.uiLastControlQuadrant = QuadrantOne;
            strSysInfo.uiLastRemotValue = uiAngleTemp;
        }
        break;
        case (JudgeForward)://如果正好为Y轴上
        {
            uiTempKpDelayCont =  sqrt((uiPwmXDelt * uiPwmXDelt) + (uiPwmYDelt * uiPwmYDelt));
            fTemp = (uiPwmYDelt * 1.0) / ( uiTempKpDelayCont * 1.0);
            uiAngleTemp = (unsigned int)(asin( fTemp) * 57.296);
            if(strSysInfo.uiRemoteNowXPos < (DriverXRemoteAdcBase - (strSysInfo.uiRemoteXN_First >> 2)))
            {
                uiAngleTemp -= 2;//20141106  before 4
            }
            if(strSysInfo.uiRemoteNowXPos > (DriverXRemoteAdcBase + (strSysInfo.uiRemoteXP_First >> 2)))
            {
                uiAngleTemp += 2;//20141106  before 4
            }
            if(FourQuadrantPosAsPwmArryMoto1_A[uiAngleTemp])
            {
                strMoto1Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto1_A[uiAngleTemp] ;
                strMoto1Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto1Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxXDriverPwm;
                ucDirectionX = Forward;			//找出数组中对应的值并确正电机的正反转
            }
            else
            {
                strMoto1Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto1_B[uiAngleTemp] ;
                strMoto1Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto1Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxXDriverPwm;
                ucDirectionX = Reverse;			   //找出数组中对应的值并确正电机的正反转
            }
            if(FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp])
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp] ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Forward;		   //找出数组中对应的值并确正电机的正反转
            }
            else
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_B[uiAngleTemp] ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Reverse;			//找出数组中对应的值并确正电机的正反转
            }
            //if(uiAngleTemp>70)uiAccTime = LowAccTime;
            //uiAccTime = MiddleAccTime;
            strSysInfo.uiLastControlQuadrant = QuadrantYPostive;
            strSysInfo.uiLastRemotValue = uiAngleTemp;

        }
        break;
        case (JudgeLef | JudgeForward)://第二象限
        {
            uiTempKpDelayCont =  sqrt((uiPwmXDelt * uiPwmXDelt) + (uiPwmYDelt * uiPwmYDelt));
            fTemp = (uiPwmYDelt * 1.0) / ( uiTempKpDelayCont * 1.0);
            uiAngleTemp = (unsigned int)(asin( fTemp) * 57.296);
            uiAngleTemp = 180 - uiAngleTemp;
            if(FourQuadrantPosAsPwmArryMoto1_A[uiAngleTemp])
            {
                strMoto1Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto1_A[uiAngleTemp] ;
                strMoto1Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto1Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxXDriverPwm;
                ucDirectionX = Forward;		//找出数组中对应的值并确正电机的正反转
            }
            else
            {
                strMoto1Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto1_B[uiAngleTemp] ;
                strMoto1Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto1Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxXDriverPwm;
                ucDirectionX = Reverse;	 //找出数组中对应的值并确正电机的正反转
            }
            if(FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp])
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp] ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Forward;	 //找出数组中对应的值并确正电机的正反转
            }
            else
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_B[uiAngleTemp] ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Reverse;	  //找出数组中对应的值并确正电机的正反转
            }
            //            if(uiAngleTemp < 130)uiAccTime = LowAccTime;
            //            else uiAccTime = MiddleAccTime;
            //            if(QuadrantTwo != strSysInfo.uiLastControlQuadrant)uiAccTime = FastAccTime;
            //            else
            //            {
            //                if(uiAngleTemp < 130)uiAccTime = LowAccTime;
            //                else uiAccTime = MiddleAccTime;
            //            }
            if(strMoto1Info.uiNewMotorPwmValue > strMoto2Info.uiNewMotorPwmValue)strMoto2Info.uiNewMotorPwmValue *= ErroLeftAndRightPwmP;
            else strMoto1Info.uiNewMotorPwmValue *= ErroLeftAndRightPwmP;
            strSysInfo.uiLastControlQuadrant = QuadrantTwo;
            strSysInfo.uiLastRemotValue = uiAngleTemp;
        }
        break;
        case (JudgeLef)://如果正好为X轴负方向上
        {
            uiTempKpDelayCont =  sqrt((uiPwmXDelt * uiPwmXDelt) + (uiPwmYDelt * uiPwmYDelt));
            fTemp = (uiPwmYDelt * 1.0) / ( uiTempKpDelayCont * 1.0);
            uiAngleTemp = (unsigned int)(asin( fTemp) * 57.296);
            strSysInfo.uiLastRemotValue = uiAngleTemp = 180 - uiAngleTemp;
            if(strSysInfo.uiRemoteNowYPos < (DriverYRemoteAdcBase - (strSysInfo.uiRemoteYN_First >> 2)))
            {
                uiAngleTemp += 4;
            }
            if(strSysInfo.uiRemoteNowYPos > (DriverYRemoteAdcBase + (strSysInfo.uiRemoteYP_First >> 2)))
            {
                uiAngleTemp -= 4;
            }
            if(FourQuadrantPosAsPwmArryMoto1_A[uiAngleTemp])
            {
                strMoto1Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto1_A[uiAngleTemp] ;
                strMoto1Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto1Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxXDriverPwm;
                ucDirectionX = Forward;		  //找出数组中对应的值并确正电机的正反转
            }
            else
            {
                strMoto1Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto1_B[uiAngleTemp]  ;
                strMoto1Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto1Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxXDriverPwm;
                ucDirectionX = Reverse;		//找出数组中对应的值并确正电机的正反转
            }
            if(FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp])
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp]  ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Forward;		//找出数组中对应的值并确正电机的正反转
            }
            else
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_B[uiAngleTemp]  ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Reverse;	   //找出数组中对应的值并确正电机的正反转
            }
        }
        break;
        case (JudgeLef | JudgeBack)://第三象限
        {
            uiTempKpDelayCont =  sqrt((uiPwmXDelt * uiPwmXDelt) + (uiPwmYDelt * uiPwmYDelt));
            fTemp = (uiPwmYDelt * 1.0) / ( uiTempKpDelayCont * 1.0);
            uiAngleTemp = (unsigned int)(asin( fTemp) * 57.296);
            strSysInfo.uiLastRemotValue = uiAngleTemp = 180 + uiAngleTemp;
            if(FourQuadrantPosAsPwmArryMoto1_A[uiAngleTemp])
            {
                strMoto1Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto1_A[uiAngleTemp] ;
                strMoto1Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto1Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxXDriverPwm;
                ucDirectionX = Forward;	  //找出数组中对应的值并确正电机的正反转
            }
            else
            {
                strMoto1Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto1_B[uiAngleTemp]  ;
                strMoto1Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto1Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxXDriverPwm;
                ucDirectionX = Reverse;		//找出数组中对应的值并确正电机的正反转
            }
            if(FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp])
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp]  ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Forward;	  //找出数组中对应的值并确正电机的正反转
            }
            else
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_B[uiAngleTemp]  ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Reverse;		//找出数组中对应的值并确正电机的正反转
            }
            //            if(uiAngleTemp > 220)uiAccTime = LowAccTime;
            //            else uiAccTime = MiddleAccTime;
            //            strMoto1Info.uiNewMotorPwmValue = (sqrt((uiPwmXDelt * uiPwmXDelt) + (uiPwmYDelt * uiPwmYDelt)) * 2) / 5;
            //            strMoto2Info.uiNewMotorPwmValue = (uiPwmYDelt * 2) / 5;
            //            if(strMoto1Info.uiNewMotorPwmValue > strSysInfo.uiControlMaxYDriverPwm)strMoto1Info.uiNewMotorPwmValue = strSysInfo.uiControlMaxYDriverPwm;
            //            if(strMoto1Info.uiNewMotorPwmValue < strSysInfo.uiControlMinYDriverPwm)strMoto1Info.uiNewMotorPwmValue = strSysInfo.uiControlMinYDriverPwm;
            //            ucDirectionX = Reverse;
            //            ucDirectionY = Reverse;
        }
        break;
        case (JudgeBack)://如果正好为Y负方向
        {
            uiTempKpDelayCont =  sqrt((uiPwmXDelt * uiPwmXDelt) + (uiPwmYDelt * uiPwmYDelt));
            fTemp = (uiPwmYDelt * 1.0) / ( uiTempKpDelayCont * 1.0);
            uiAngleTemp = (unsigned int)(asin( fTemp) * 57.296);
            strSysInfo.uiLastRemotValue = uiAngleTemp = 180 + uiAngleTemp;
            if(strSysInfo.uiRemoteNowXPos < (DriverXRemoteAdcBase - (strSysInfo.uiRemoteXN_First >> 2)))
            {
                uiAngleTemp += 1;
            }
            if(strSysInfo.uiRemoteNowXPos > (DriverXRemoteAdcBase + (strSysInfo.uiRemoteXP_First >> 2)))
            {
                uiAngleTemp -= 1;
            }
            if(FourQuadrantPosAsPwmArryMoto1_A[uiAngleTemp])
            {
                strMoto1Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto1_A[uiAngleTemp] ;
                strMoto1Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto1Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxXDriverPwm;
                ucDirectionX = Forward;
            }
            else
            {
                strMoto1Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto1_B[uiAngleTemp]  ;
                strMoto1Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto1Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxXDriverPwm;
                ucDirectionX = Reverse;
            }
            if(FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp])
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp]  ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Forward;
            }
            else
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_B[uiAngleTemp]  ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Reverse;
            }
            //            strMoto1Info.uiNewMotorPwmValue = (uiPwmYDelt * 2) / 4;
            //            strMoto2Info.uiNewMotorPwmValue = (uiPwmYDelt * 2) / 4;
            //            ucDirectionX = Reverse;
            //            ucDirectionY = Reverse;
        }
        break;
        case (JudgeRigt | JudgeBack)://第四象限
        {
            uiTempKpDelayCont =  sqrt((uiPwmXDelt * uiPwmXDelt) + (uiPwmYDelt * uiPwmYDelt));
            fTemp = (uiPwmYDelt * 1.0) / ( uiTempKpDelayCont * 1.0);
            uiAngleTemp = (unsigned int)(asin( fTemp) * 57.296);
            strSysInfo.uiLastRemotValue = uiAngleTemp = 360 - uiAngleTemp;
            if(FourQuadrantPosAsPwmArryMoto1_A[uiAngleTemp])
            {
                strMoto1Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto1_A[uiAngleTemp] ;
                strMoto1Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto1Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxXDriverPwm;
                ucDirectionX = Forward;	  //找出数组中对应的值并确正电机的正反转
            }
            else
            {
                strMoto1Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto1_B[uiAngleTemp]  ;
                strMoto1Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto1Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxXDriverPwm;
                ucDirectionX = Reverse;
            }
            if(FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp])
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp]  ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Forward;		 //找出数组中对应的值并确正电机的正反转
            }
            else
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_B[uiAngleTemp]  ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Reverse;	   //找出数组中对应的值并确正电机的正反转
            }
            //            if(uiAngleTemp < 320)uiAccTime = LowAccTime;
            //            else uiAccTime = MiddleAccTime;
            //            strMoto1Info.uiNewMotorPwmValue = (uiPwmYDelt * 2) / 5;
            //            strMoto2Info.uiNewMotorPwmValue = (sqrt((uiPwmXDelt * uiPwmXDelt) + (uiPwmYDelt * uiPwmYDelt)) * 2) / 5;
            //            if(strMoto2Info.uiNewMotorPwmValue > strSysInfo.uiControlMaxYDriverPwm)strMoto2Info.uiNewMotorPwmValue = strSysInfo.uiControlMaxYDriverPwm;
            //            if(strMoto2Info.uiNewMotorPwmValue < strSysInfo.uiControlMinYDriverPwm)strMoto2Info.uiNewMotorPwmValue = strSysInfo.uiControlMinYDriverPwm;
            //            ucDirectionX = Reverse;
            //            ucDirectionY = Reverse;
        }
        break;
        case (JudgeRigt)://如果正好在X负方向上
        {
            uiTempKpDelayCont =  sqrt((uiPwmXDelt * uiPwmXDelt) + (uiPwmYDelt * uiPwmYDelt));
            fTemp = (uiPwmYDelt * 1.0) / ( uiTempKpDelayCont * 1.0);
            uiAngleTemp = (unsigned int)(asin( fTemp) * 57.296);
            strSysInfo.uiLastRemotValue = uiAngleTemp = 359 - uiAngleTemp;
            if(strSysInfo.uiRemoteNowYPos < (DriverYRemoteAdcBase - (strSysInfo.uiRemoteYN_First >> 2)))
            {
                if(uiAngleTemp < 257)uiAngleTemp += 3;
            }
            if(strSysInfo.uiRemoteNowYPos > (DriverYRemoteAdcBase + (strSysInfo.uiRemoteYP_First >> 2)))
            {
                if(uiAngleTemp > 3)uiAngleTemp -= 3;
            }
            if(FourQuadrantPosAsPwmArryMoto1_A[uiAngleTemp])
            {
                strMoto1Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto1_A[uiAngleTemp] ;
                strMoto1Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto1Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxXDriverPwm;
                ucDirectionX = Forward;		  //找出数组中对应的值并确正电机的正反转
            }
            else
            {
                strMoto1Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto1_B[uiAngleTemp]  ;
                strMoto1Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto1Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxXDriverPwm;
                ucDirectionX = Reverse;		 //找出数组中对应的值并确正电机的正反转
            }
            if(FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp])
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp] ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Forward;		 //找出数组中对应的值并确正电机的正反转
            }
            else
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_B[uiAngleTemp]  ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Reverse;		  //找出数组中对应的值并确正电机的正反转
            }
        }
        break;
        default:
            break;
        }
        uiAngle = uiAngleTemp;
        if(strMoto1Info.uiNewMotorPwmValue > strMoto2Info.uiNewMotorPwmValue)
        {
            uiMotor1AndMotor2P =  (strMoto1Info.uiNewMotorPwmValue * uiErroPare) / strMoto2Info.uiNewMotorPwmValue ; //电机1与电机2控制比例
            ucRelationMoto1Mtor2 = 0x01;//只求正向比例	 //算出两电机的比例值
        }
        else
        {
            uiMotor1AndMotor2P =  (strMoto2Info.uiNewMotorPwmValue * uiErroPare) / strMoto1Info.uiNewMotorPwmValue ; //电机1与电机2控制比例
            ucRelationMoto1Mtor2 = 0x02;//只求正向比例  有反向比例
        }
        if((Forward == ucDirectionX) && (Forward == ucDirectionY));
        else ucRelationMoto1Mtor2 = FALSE;//如果两个电机不是在正转的状态，就不要去绑定

        if((Moto1AStmPwmReg) || (Moto1BStmPwmReg))
        {
            strMoto1Info.uiOldMotorPwmValue = (Moto1AStmPwmReg) > (Moto1BStmPwmReg) ? (Moto1AStmPwmReg) : (Moto1BStmPwmReg);
            if(Moto1AStmPwmReg) strMoto1Info.unMotorOldDirection = Forward;
            else strMoto1Info.unMotorOldDirection = Reverse;	  //求上次电机的PWM并确定出上次电机的转动方向
        }
        else
        {
            strMoto1Info.uiOldMotorPwmValue  = 0;
            strMoto1Info.unMotorOldDirection = NoDirection;
        }
        if((Moto2AStmPwmReg) || (Moto2BStmPwmReg))
        {
            strMoto2Info.uiOldMotorPwmValue = (Moto2AStmPwmReg) > (Moto2BStmPwmReg) ? (Moto2AStmPwmReg) : (Moto2BStmPwmReg);
            if(Moto2AStmPwmReg) strMoto2Info.unMotorOldDirection = Forward;
            else strMoto2Info.unMotorOldDirection = Reverse;   //求上次电机的PWM并确定出上次电机的转动方向
        }
        else
        {
            strMoto2Info.uiOldMotorPwmValue  = 0;
            strMoto2Info.unMotorOldDirection = NoDirection;
        }
        iDeltPwmX = strMoto1Info.uiNewMotorPwmValue - strMoto1Info.uiOldMotorPwmValue;//计算出与上次值的误差
        iDeltPwmY = strMoto2Info.uiNewMotorPwmValue - strMoto2Info.uiOldMotorPwmValue;//计算出与上次值的误差
        if(iDeltPwmX > 0)//计算出X加减速
        {
            ucDecIncSpeedX = IncSpeed;
        }
        else
        {
            //如果是减速
            ucDecIncSpeedX = DecSpeed;
            iDeltPwmX = 0 - iDeltPwmX; //变量符号
        }
        if(iDeltPwmY > 0)//计算出Y加减速
        {
            ucDecIncSpeedY = IncSpeed;
        }
        else
        {
            //如果是减速
            ucDecIncSpeedY = DecSpeed;
            iDeltPwmY = 0 - iDeltPwmY;
        }
        if(strMoto1Info.unMotorOldDirection && ucDirectionX && (ucDirectionX != strMoto1Info.unMotorOldDirection))
        {
            ucNeedChangDirXFlag = TRUE;
            //uiAccTime = FastAccTime;
            ucDecIncSpeedX = DecSpeed;
            iDeltPwmX =  strMoto1Info.uiOldMotorPwmValue;	 //此时速度要减为0
            ucDirectionX = strMoto1Info.unMotorOldDirection ;//是否要换向
        }
        if(strMoto2Info.unMotorOldDirection && ucDirectionY && (ucDirectionY != strMoto2Info.unMotorOldDirection))
        {
            ucNeedChangDirYFlag = TRUE;
            //uiAccTime = FastAccTime;
            ucDecIncSpeedY = DecSpeed;
            iDeltPwmY =  strMoto2Info.uiOldMotorPwmValue;	 //此时速度要减为0
            ucDirectionY = strMoto2Info.unMotorOldDirection ;
        }
        uiAccTime  = uniDiverInfo.strDirverAtt.ucDriverAcc;
        if((TRUE == ucNeedChangDirXFlag ) && (TRUE == ucNeedChangDirYFlag ))//如果是双向换向，是要求急停现象的
        {
            if((strSysInfo.uiLastRemotValue > 60) && (strSysInfo.uiLastRemotValue < 120)) //如果要求在60度到120度两电机同时换向，哪肯定是要急停了
            {
                //                uiAccTime = FastAccTime;
                uiAccTime = (unsigned int)(uniDiverInfo.strDirverAtt.ucSwerveAcc * 2.5);
                ucRelationMoto1Mtor2 = FALSE;//由后退直接切换到前进
            }
            if((strSysInfo.uiLastRemotValue > 240) && (strSysInfo.uiLastRemotValue < 300)) //如果要求在60度到120度两电机同时换向，哪肯定是要急停了
            {
                //                uiAccTime = FastAccTime;
                ucRelationMoto1Mtor2 = FALSE;//由前进直接切换到后退
                uiAccTime = (unsigned int)(uniDiverInfo.strDirverAtt.ucSwerveAcc * 0.8);
            }
        }
        else
        {
            ////////////////
            if((strSysInfo.uiLastRemotValue < 350) && (strSysInfo.uiLastRemotValue > 190))//如果是在一个夹角后退的时候
            {
                //如果在这个象限内说明是转弯或是在后退的过程中，说明要另外的加速度
                uiAccTime = (unsigned int)(uniDiverInfo.strDirverAtt.ucSwerveAcc * 1.3);
            }
        }
        uiStepX = iDeltPwmX / strSysInfo.uiControlPareOfKp;//算出加速步数
        uiStepY = iDeltPwmY / strSysInfo.uiControlPareOfKp;//算出加速步数
        if(uiStepX > uiStepY)uiDeltSteps = uiStepX;
        else uiDeltSteps = uiStepY;
        uiPwmXDelt = 0;
        uiPwmYDelt = 0;
        iTempPwmX = strMoto1Info.uiOldMotorPwmValue;
        iTempPwmY = strMoto2Info.uiOldMotorPwmValue;
        ucOverCurrentAskQuit = FALSE;
        for(uiTemp = 0; uiTemp < uiDeltSteps ; uiTemp++)//加速过程
        {
            uiPwmXDelt++;
            uiPwmYDelt++;
            if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-刹车器程序或0-驻坡程序由参数表第6项决定20190110
            {
                if(FALSE == ucOpenBrakeFlag)//如果没有开启刹车器的情况
                {
                    if((ReDefTIM1CCR1 > uiStartPwm) || (ReDefTIM1CCR3 > uiStartPwm) || (ReDefTIM4CCR1 > uiStartPwm) || (ReDefTIM4CCR3 > uiStartPwm))
                    {
                        //如果一但达到启动条件
                        if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)CtlBrakeBikeEn;
                        ucOpenBrakeFlag = TRUE;//不再开启了
                    }
                    uiAccTime = uniDiverInfo.strDirverAtt.ucSwerveAcc * 0.3; //用于一个快速启动的。
                }
                else uiAccTime = uniDiverInfo.strDirverAtt.ucSwerveAcc;//如果是加速完成
            }

            /////////////////////////////////////////////////////////////////////////////////////////  过流保护
            if(LeftLowMosFetPwmReg)//ReDefTIM1CCR3
            {
                if(strMoto1Info.uiMotorCurrent >= usMaxDriverCurrent)
                {
                    ucOverCurrentAskQuit = TRUE;
                    if(LeftLowMosFetPwmReg > 2)SetTim1CC3andCCR4(LeftLowMosFetPwmReg - 1);
                    vDelay30us(uniDiverInfo.strDirverAtt.ucDriverAcc);//加速度
                }
            }
            if(LeftHighMosFetPwmReg)//ReDefTIM1CCR1
            {
                if(strMoto1Info.uiMotorCurrent >= usMaxDriverCurrent)
                {
                    ucOverCurrentAskQuit = TRUE;
                    if(LeftHighMosFetPwmReg > 2)SetTim1CC1andCCR2(LeftHighMosFetPwmReg - 1);
                    vDelay30us(uniDiverInfo.strDirverAtt.ucDriverAcc);//加速度
                }
            }
            if(RightLowMosFetPwmReg)//ReDefTIM4CCR3
            {
                if(strMoto2Info.uiMotorCurrent >= usMaxDriverCurrent)
                {
                    ucOverCurrentAskQuit = TRUE;
                    if(RightLowMosFetPwmReg > 2)SetTim4CC3andCCR4(RightLowMosFetPwmReg - 1);
                    vDelay30us(uniDiverInfo.strDirverAtt.ucDriverAcc);//加速度
                }
            }
            if(RightHighMosFetPwmReg)//ReDefTIM4CCR1
            {
                if(strMoto2Info.uiMotorCurrent >= usMaxDriverCurrent)
                {
                    ucOverCurrentAskQuit = TRUE;
                    if(RightHighMosFetPwmReg > 2)SetTim4CC1andCCR2(RightHighMosFetPwmReg - 1);
                    vDelay30us(uniDiverInfo.strDirverAtt.ucDriverAcc);//加速度
                }
            }
            if(TRUE == ucOverCurrentAskQuit)break;
            if((TRUE == ucOverCurrentAskQuit) || ((strMoto2Info.uiMotorCurrent >= MaxContinueOutCurrent) && (strMoto1Info.uiMotorCurrent >= MaxContinueOutCurrent)))
            {
                uiOverCurrentCont ++;
                if(uiOverCurrentCont >= MaxCurrentHoldLonTime)//如果到了一个最大电流保持输出时间
                {
                    if(uiOverCurrentCont >= uiOverCurrentCntNextReachTime)//如果到了一个减电流周期
                    {
                        usMaxDriverCurrent -= 3;//逐级减电流输出
                        if(usMaxDriverCurrent <= MaxContinueOutCurrent)usMaxDriverCurrent = MaxContinueOutCurrent;//限幅输出
                        uiOverCurrentCntNextReachTime += CurrentReduceDeltTime;//下次减电流计数周期
                    }
                    break;
                }
                if(uiOverCurrentCont > 0xFFFFFFF)uiOverCurrentCont = 0xFFFFFFF;
            }
            /////////////////////////////////////////////////////////恒流
            if(uiStepX > 0)//先是计算出X加减速
            {
                //如果加减速没有加减完成
                uiStepX--;//每一次计算过后，就去对数据加减
                if(IncSpeed == ucDecIncSpeedX)/////////////////////////////////是X加速过程
                {
                    if((strMoto1Info.uiOldMotorPwmValue + uiPwmXDelt * strSysInfo.uiControlPareOfKp) > strSysInfo.uiControlMinXDriverPwm)
                    {
                        iTempPwmX = strMoto1Info.uiOldMotorPwmValue + uiPwmXDelt * strSysInfo.uiControlPareOfKp;//如果算出的过小
                    }
                    else
                    {
                        iTempPwmX = 0;
                    }
                    if((strMoto1Info.uiOldMotorPwmValue + uiPwmXDelt * strSysInfo.uiControlPareOfKp) >= strSysInfo.uiControlMaxXDriverPwm)
                    {
                        iTempPwmX = strSysInfo.uiControlMaxXDriverPwm;//如果PWM过大
                        //break;
                    }
                    else
                    {
                        iTempPwmX = strMoto1Info.uiOldMotorPwmValue + uiPwmXDelt * strSysInfo.uiControlPareOfKp;
                    }//
                }
                if(DecSpeed == ucDecIncSpeedX)//////////////////////////////////是X加减过程
                {
                    if((strMoto1Info.uiOldMotorPwmValue - uiPwmXDelt * strSysInfo.uiControlPareOfKp) >= strSysInfo.uiControlMinXDriverPwm)
                    {
                        iTempPwmX = strMoto1Info.uiOldMotorPwmValue - uiPwmXDelt * strSysInfo.uiControlPareOfKp; //如果算出的过小
                    }
                    else
                    {
                        iTempPwmX = 0;
                        //break;
                    }
                    if((strMoto1Info.uiOldMotorPwmValue - uiPwmXDelt * strSysInfo.uiControlPareOfKp) >= strSysInfo.uiControlMaxXDriverPwm)
                    {
                        iTempPwmX = strSysInfo.uiControlMaxXDriverPwm; //如果PWM过大
                    }
                    else
                    {
                        iTempPwmX = strMoto1Info.uiOldMotorPwmValue - uiPwmXDelt * strSysInfo.uiControlPareOfKp;
                    }
                }
#ifdef UsedMoto1AndMotor2P
                if(Forward == ucDirectionX)
                {
                    if(0x01 == ucRelationMoto1Mtor2)
                    {
                        //如果有一个正向的比例因数
                        if(iTempPwmX > iTempPwmY)//如果此时，有比例分配
                        {
                            //要求X>Y
                            if(((iTempPwmX * uiErroPare) / iTempPwmY) > (uiMotor1AndMotor2P + 1)) //如果比例合适
                            {
                                if(DecSpeed == ucDecIncSpeedX)
                                {
                                    CtlSetMotor1LeftPwmPercent(iTempPwmX);
                                }
                                else
                                {
                                    uiPwmXDelt--;
                                }
                            }
                            else //如果本次调节的比例过大
                            {
                                if(DecSpeed == ucDecIncSpeedX)
                                {
                                    uiPwmXDelt--;
                                }
                                else
                                {
                                    CtlSetMotor1LeftPwmPercent(iTempPwmX);
                                }
                            }
                        }
                        else//如果X<Y
                        {
                            if(DecSpeed == ucDecIncSpeedX)
                            {
                                uiPwmXDelt--;
                            }
                            else
                            {
                                CtlSetMotor1LeftPwmPercent(iTempPwmX);
                            }
                        }
                    }
                    else if(0x02 == ucRelationMoto1Mtor2)
                    {
                        //如果有一个反向的比例因数
                        if(iTempPwmY > iTempPwmX)//如果此时，有比例分配
                        {
                            //要求Y>X
                            if(((iTempPwmY * uiErroPare) / iTempPwmX) > (uiMotor1AndMotor2P + 1)) //如果比例合适
                            {
                                if(DecSpeed == ucDecIncSpeedX)
                                {
                                    uiPwmXDelt--;
                                }
                                else
                                {
                                    CtlSetMotor1LeftPwmPercent(iTempPwmX);   //如果本次调节的比例过大;//如果本次调节的比例过大
                                }
                            }
                            else //如果本次调节的比例过大
                            {
                                if(DecSpeed == ucDecIncSpeedX)
                                {
                                    CtlSetMotor1LeftPwmPercent(iTempPwmX);
                                }
                                else
                                {
                                    uiPwmXDelt--;   //如果本次调节的比例过大;//如果本次调节的比例过大
                                }
                            }
                        }
                        else//如果X>Y
                        {
                            //uiPwmXDelt--;   //如果本次调节的比例过大;//如果本次调节的比例过大
                            if(DecSpeed == ucDecIncSpeedX)
                            {
                                CtlSetMotor1LeftPwmPercent(iTempPwmX);
                            }
                            else
                            {
                                uiPwmXDelt--;   //如果本次调节的比例过大;//如果本次调节的比例过大
                            }
                        }
                    }
                    else CtlSetMotor1LeftPwmPercent(iTempPwmX);
                    CtlSetMotor1RightPwmPercent(0);
                    //当如果有邦定系数修正不过来的问题
                    //                     if((strMoto2Info.uiNewMotorPwmValue == strMoto1Info.uiNewMotorPwmValue)//如果新PWM要求
                    // 											&&(strMoto2Info.uiNewMotorPwmValue >3600)&&(strMoto1Info.uiNewMotorPwmValue >3600))
                    //                     {
                    //                         iDealErroPwm = strMoto2Info.uiOldMotorPwmValue - strMoto1Info.uiOldMotorPwmValue;
                    //                         if((iDealErroPwm < 100)&&iDealErroPwm)//如果误差在100以内
                    //                         {
                    //                             strMoto1Info.uiOldMotorPwmValue++;
                    //                             CtlSetMotor1LeftPwmPercent(strMoto1Info.uiOldMotorPwmValue);
                    //                         }
                    //                         else
                    //                         {
                    //                             iDealErroPwm = 0 - iDealErroPwm;
                    //                             if(iDealErroPwm < 100)//如果误差在100以内
                    //                             {
                    //                                 strMoto1Info.uiOldMotorPwmValue++;
                    //                                 CtlSetMotor1LeftPwmPercent(strMoto1Info.uiOldMotorPwmValue);
                    //                             }
                    //                         }
                    //                     }
                    //
                }//看是正反转
#endif
#ifndef  UsedMoto1AndMotor2P
                if(Forward == ucDirectionX)
                {
                    CtlSetMotor1LeftPwmPercent(iTempPwmX);
                    CtlSetMotor1RightPwmPercent(0);
                }
#endif
                if(Reverse == ucDirectionX)
                {
                    CtlSetMotor1LeftPwmPercent(0);
                    CtlSetMotor1RightPwmPercent(iTempPwmX);
                }
            }

            if(uiStepY > 0)//Y加减速过程
            {
                //如果加减速没有加减完成
                uiStepY--;//每计算一次对加减步数加减一次
                if(IncSpeed == ucDecIncSpeedY)///////////////////////////Y电机控制的实现。加速过程
                {
                    if((strMoto2Info.uiOldMotorPwmValue + uiPwmYDelt * strSysInfo.uiControlPareOfKp) > strSysInfo.uiControlMinYDriverPwm)
                    {
                        iTempPwmY = strMoto2Info.uiOldMotorPwmValue + uiPwmYDelt * strSysInfo.uiControlPareOfKp;//如果算出的过小
                    }
                    else
                    {
                        iTempPwmY = 0;
                    }
                    if((strMoto2Info.uiOldMotorPwmValue + uiPwmYDelt * strSysInfo.uiControlPareOfKp) >= strSysInfo.uiControlMaxYDriverPwm)
                    {
                        iTempPwmY = strSysInfo.uiControlMaxYDriverPwm;//如果PWM过大
                        //break;
                    }
                    else
                    {
                        iTempPwmY = strMoto2Info.uiOldMotorPwmValue + uiPwmYDelt * strSysInfo.uiControlPareOfKp;
                    }//
                }
                if(DecSpeed == ucDecIncSpeedY)/////////////////Y电机控制的实现，减速过程
                {
                    if((strMoto2Info.uiOldMotorPwmValue - uiPwmYDelt * strSysInfo.uiControlPareOfKp) >= strSysInfo.uiControlMinYDriverPwm)
                    {
                        iTempPwmY = strMoto2Info.uiOldMotorPwmValue - uiPwmYDelt * strSysInfo.uiControlPareOfKp; //如果算出的过小
                    }
                    else
                    {
                        iTempPwmY = 0;
                        //break;
                    }
                    if((strMoto2Info.uiOldMotorPwmValue - uiPwmYDelt * strSysInfo.uiControlPareOfKp) >= strSysInfo.uiControlMaxYDriverPwm)
                    {
                        iTempPwmY = strSysInfo.uiControlMaxYDriverPwm; //如果PWM过大
                    }
                    else
                    {
                        iTempPwmY = strMoto2Info.uiOldMotorPwmValue - uiPwmYDelt * strSysInfo.uiControlPareOfKp;
                    }
                }
#ifdef  UsedMoto1AndMotor2P
                if(Forward == ucDirectionY)
                {
                    if(0x01 == ucRelationMoto1Mtor2)
                    {
                        if(iTempPwmX > iTempPwmY)//要求X>Y
                        {
                            if(((iTempPwmX * uiErroPare) / iTempPwmY) > (uiMotor1AndMotor2P + 1))
                            {
                                if(DecSpeed == ucDecIncSpeedY)
                                {
                                    uiPwmYDelt--;
                                }
                                else
                                {
                                    CtlSetMotor2LeftPwmPercent(iTempPwmY);
                                }
                            }
                            else
                            {
                                if(DecSpeed == ucDecIncSpeedY)
                                {
                                    CtlSetMotor2LeftPwmPercent(iTempPwmY);
                                }
                                else
                                {
                                    uiPwmYDelt--;
                                }
                            }
                        }
                        else//如果X<Y
                        {
                            if(DecSpeed == ucDecIncSpeedY)
                            {
                                CtlSetMotor2LeftPwmPercent(iTempPwmY);
                            }
                            else
                            {
                                uiPwmYDelt--;
                            }
                        }
                    }
                    else  if(0x02 == ucRelationMoto1Mtor2)
                    {
                        if(iTempPwmY > iTempPwmX)//要求Y>X
                        {
                            if(((iTempPwmY * uiErroPare) / iTempPwmX) > (uiMotor1AndMotor2P + 1))
                            {
                                if(DecSpeed == ucDecIncSpeedY)
                                {
                                    CtlSetMotor2LeftPwmPercent(iTempPwmY);
                                }
                                else
                                {
                                    uiPwmYDelt--;
                                }
                            }
                            else
                            {
                                if(DecSpeed == ucDecIncSpeedY)
                                {
                                    uiPwmYDelt--;
                                }
                                else
                                {
                                    CtlSetMotor2LeftPwmPercent(iTempPwmY);
                                }
                            }
                        }
                        else//如果Y<X
                        {
                            if(DecSpeed == ucDecIncSpeedY)
                            {
                                uiPwmYDelt--;
                            }
                            else
                            {
                                CtlSetMotor2LeftPwmPercent(iTempPwmY);
                            }
                        }
                    }
                    else
                    {
                        CtlSetMotor2LeftPwmPercent(iTempPwmY);
                    }
                    CtlSetMotor2RightPwmPercent(0);
                }
#endif
#ifndef  UsedMoto1AndMotor2P
                if(Forward == ucDirectionY)
                {
                    CtlSetMotor2LeftPwmPercent(0);
                    CtlSetMotor2RightPwmPercent(iTempPwmY);
                }
#endif
                if(Reverse == ucDirectionY)
                {
                    CtlSetMotor2LeftPwmPercent(0);
                    CtlSetMotor2RightPwmPercent(iTempPwmY);
                }
            }
            for(uiTempKpDelayCont = 0; uiTempKpDelayCont < strSysInfo.uiControlPareOfKd; uiTempKpDelayCont++)
            {
                vDelay30usAcc(uiAccTime);//加速度20131206
            }
            if(ucNeedChangDirXFlag && (0 == uiStepX))
            {
                CtlSetMotor1LeftPwmPercent(0);
                CtlSetMotor1RightPwmPercent(0);
                break;
            }
            if(ucNeedChangDirYFlag && (0 == uiStepY))
            {
                CtlSetMotor2LeftPwmPercent(0);
                CtlSetMotor2RightPwmPercent(0);
                break;
            }
            //            if(SysNoDirection == ucCalcDirection())//如果在运动加减速的时候有摇杆复位
            if((SysNoDirection == ucCalcDirection()) || (strSysInfo.uiCalcSysShowPower < CheckLowAlarmVoltage)) //方向回零位或低电压时退出 进入刹车
            {
                //就去减正常减速，如果在减速的过程中再有加速可以继续加的
                vDownSpeedCalcPwm(TRUE);//不能再这里去调用刹车 算法会有大量的 局部变量存在的
                vSendSingleOrder(BikeOutBack);
                if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-刹车器程序或0-驻坡程序由参数表第6项决定20190110
                {
                    ucOpenBrakeFlag = FALSE;//如果在刹车回来后，再次有转动信号 可能会有刹车器关闭的情况  在这个函数里要再次判断
                }
                break;
            }
            iDeltPwmX = strSysInfo.uiRemoteNowXPos - uiReMoteXTemp;
            iDeltPwmY = strSysInfo.uiRemoteNowYPos - uiReMoteYTemp;
            if(iDeltPwmX > 0);//这里算出来有遥杆移动的时候 及时地作下处理
            else iDeltPwmX = 0 - iDeltPwmX;
            if(iDeltPwmY > 0);
            else iDeltPwmY = 0 - iDeltPwmY;
            if((iDeltPwmX > strSysInfo.uiRemoteXP_Delt) || (iDeltPwmY > strSysInfo.uiRemoteYP_Delt))
            {
                break;//如果有摇杆差值较大，就去退出
            }
            if(ucErroType)
            {
                vAutoPowerOffTimeFlag();
                return;
            }
        }
    }
}
/*
检测电源上电按键
*/
unsigned char ucPowerOnCotrol(void)
{

    unsigned char ucTemp;
    if(1)
    {
        if(1)
        {
            ucPowerKeySate = KeyPressDown;
            ucTag10ms = FALSE;
            while(FALSE == ucTag10ms);
            vSendSingleOrder(FirstOrderPowerOn);
            g_uiTimeNumCont1ms = 800;
            while(g_uiTimeNumCont1ms);
            ucStsTemState = SelfCheckMode;
            ucPowerKeySate = KeyPressDown;
            ucPowerKeyPressTimes = 0x01;
            return TRUE;
        }
    }
    ucTemp = 0;
    if((strMoto1Info.uiLeftHBridgeHaveFilterAdc > AllowAutoPowerMinVol) && (strMoto1Info.uiLeftHBridgeHaveFilterAdc < AllowAutoPowerMaxVol))ucTemp++;
    if((strMoto1Info.uiRightHBridgeHaveFilterAdc > AllowAutoPowerMinVol) && (strMoto1Info.uiRightHBridgeHaveFilterAdc < AllowAutoPowerMaxVol))ucTemp++;
    if((strMoto2Info.uiLeftHBridgeHaveFilterAdc > AllowAutoPowerMinVol) && (strMoto2Info.uiLeftHBridgeHaveFilterAdc < AllowAutoPowerMaxVol))ucTemp++;
    if((strMoto2Info.uiRightHBridgeHaveFilterAdc > AllowAutoPowerMinVol) && (strMoto2Info.uiRightHBridgeHaveFilterAdc < AllowAutoPowerMaxVol))ucTemp++;
    if((ucTemp >= 1) && (ucTemp <= 4))
    {
        ucStsTemState = SysInAutoPowerOn;
    }
    else//如果 在开机的时候 没有一个制定的模式，就指定一个唯一的寄存器值
    {
        uiMotor1LeftHighPwmRegAdd = Time1Ccr1Add;
        uiMotor1RightHighPwmRegAdd = Time1Ccr3Add;
        uiMotor2LeftHighPwmRegAdd = Time4Ccr1Add;
        uiMotor2RightHighPwmRegAdd = Time4Ccr3Add;
        vAutoPowerOffTimeFlag();
    }
    return FALSE;
}

/*
系统有自动关机到时
*/
//unsigned short usAdcMotoATime1CCR1 = 0;
//unsigned short usAdcMotoATime1CCR3 = 0;
//unsigned short usAdcMotoBTime4CCR1 = 0;
//unsigned short usAdcMotoBTime4CCR3 = 0;
//unsigned char usMotoATime1CCR1_ChiarInStopModeCnt = 0;
//unsigned char usMotoATime1CCR3_ChiarInStopModeCnt = 0;
//unsigned char usMotoBTime4CCR1_ChiarInStopModeCnt = 0;
//unsigned char usMotoBTime4CCR3_ChiarInStopModeCnt = 0;//电机运行检测计数器

//unsigned char usMotoATime1CCR1_ChiarBreakCnt = 0;
//unsigned char usMotoATime1CCR3_ChiarBreakCnt = 0;
//unsigned char usMotoBTime4CCR1_ChiarBreakCnt = 0;
//unsigned char usMotoBTime4CCR3_ChiarBreakCnt = 0;//电机断线计数器
#define PowerDownValue 225//大概是2v
#define MaxWaitTime               6000//180000    //   180000 3分钟关机   40秒关机  6分钟 蒋
void vAutoPowerOffTimeFlag(void)
{
#define  MotoInRunJudeDeltAdc      250//电机在缓慢溜坡的时候ADC差值
#define  MotoInIdelAdcBaseAdc      790
#define  MotoInBreakJudeAdc         3600//电机断线检测ADC
#define CheckInStopTimes               1    //共检测时间为600xCheckInStopTimes = ms
#define CheckBreakTimes                5    //共检测时间为600xCheckBreakTimes = ms
    unsigned char ucAskPowerOnNextTime = 0;
    unsigned int uiTemp = 0;
    unsigned int uiOldPwerValue = 0;


    unsigned short usTime1CCR1Cnt = 0;
    unsigned short usTime1CCR3Cnt = 0;
    unsigned short usTime4CCR1Cnt = 0;
    unsigned short usTime4CCR3Cnt = 0;

    unsigned short usAdcMotoATime1CCR1 = 0;
    unsigned short usAdcMotoATime1CCR3 = 0;
    unsigned short usAdcMotoBTime4CCR1 = 0;
    unsigned short usAdcMotoBTime4CCR3 = 0;
    unsigned char usMotoATime1CCR1_ChiarInStopModeCnt = 0;
    unsigned char usMotoATime1CCR3_ChiarInStopModeCnt = 0;
    unsigned char usMotoBTime4CCR1_ChiarInStopModeCnt = 0;
    unsigned char usMotoBTime4CCR3_ChiarInStopModeCnt = 0;//电机运行检测计数器

    unsigned char usMotoATime1CCR1_ChiarBreakCnt = 0;
    unsigned char usMotoATime1CCR3_ChiarBreakCnt = 0;
    unsigned char usMotoBTime4CCR1_ChiarBreakCnt = 0;
    unsigned char usMotoBTime4CCR3_ChiarBreakCnt = 0;//电机断线计数器
    vDownSpeedCalcPwm(TRUE);
    //  vSendSingleOrder(QuitErro);
    if(ucMingRuiErrConst)ucErroType = ErroNoBearker;//如果
    g_uiTimeNumCont1ms = 60;
    while(g_uiTimeNumCont1ms);
    vSendSingleOrder(OrderPowerOff);//关机
    g_uiTimeNumCont1ms = 60;
    while(g_uiTimeNumCont1ms);
    C_EnResetStm8;
    CtlSetMotor1LeftPwmPercent(0);
    CtlSetMotor1RightPwmPercent(0);
    CtlSetMotor2LeftPwmPercent(0);
    CtlSetMotor2RightPwmPercent(0);
    if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)ucErroType = ErroNoBearker;//如果有电电磁刹车直接退出
    for(uiTemp = 0; uiTemp < 200; uiTemp++)
    {
        g_uiTimeNumCont1ms = 2;
        while(g_uiTimeNumCont1ms);
#ifdef dSysPowerHold
        if((ErroSysPowerKeyPress != ucErroType) && ucErroType)break;
#else
        if((ErroSysPowerOff != ucErroType) && ucErroType)break;
#endif
    }
    while(ReadKeyOfOn_OffFlag)
    {
#ifdef dSysPowerHold
        if((ErroSysPowerKeyPress != ucErroType) && ucErroType)break;
#else
        if((ErroSysPowerOff != ucErroType) && ucErroType)break;
#endif

    }
    for(uiTemp = 0; uiTemp < MaxWaitTime; uiTemp++)
    {
        g_uiTimeNumCont1ms = 2;
        usTime1CCR1Cnt++;
        usTime1CCR3Cnt++;
        usTime4CCR1Cnt++;
        usTime4CCR3Cnt++;
        //////////////////////////////////////////////
        if(300 == usTime1CCR1Cnt )//如果检测周期到了
        {
            ReDefTIM1CCR1 = 35;
            usAdcMotoATime1CCR1 = uiInDownSpeedCalcMotoSate(MotoA, 1);
            ReDefTIM1CCR3 = ReDefTIM1CCR1 = ReDefTIM1CCR2 = ReDefTIM1CCR4 = 0;
            if((usAdcMotoATime1CCR1 < (MotoInIdelAdcBaseAdc + MotoInRunJudeDeltAdc)) && (usAdcMotoATime1CCR1 > (MotoInIdelAdcBaseAdc - MotoInRunJudeDeltAdc)))
            {
                usMotoATime1CCR1_ChiarInStopModeCnt ++;
            }
            else usMotoATime1CCR1_ChiarInStopModeCnt = 0;
            if(usAdcMotoATime1CCR1 > MotoInBreakJudeAdc)usMotoATime1CCR1_ChiarBreakCnt++;
            else usMotoATime1CCR1_ChiarBreakCnt = 0;
            if(usMotoATime1CCR1_ChiarInStopModeCnt > 250)usMotoATime1CCR1_ChiarInStopModeCnt = 250;
            if(usMotoATime1CCR1_ChiarBreakCnt > 250)usMotoATime1CCR1_ChiarBreakCnt = 250;
        }
        if(500 == usTime1CCR3Cnt)
        {
            usTime1CCR1Cnt = usTime1CCR3Cnt = 100;
            ReDefTIM1CCR3 = 35;
            usAdcMotoATime1CCR3 = uiInDownSpeedCalcMotoSate(MotoA, 1);
            ReDefTIM1CCR3 = ReDefTIM1CCR1 = ReDefTIM1CCR2 = ReDefTIM1CCR4 = 0;
            if((usAdcMotoATime1CCR3 < (MotoInIdelAdcBaseAdc + MotoInRunJudeDeltAdc)) && (usAdcMotoATime1CCR3 > (MotoInIdelAdcBaseAdc - MotoInRunJudeDeltAdc)))
            {
                usMotoATime1CCR3_ChiarInStopModeCnt ++;
            }
            else usMotoATime1CCR3_ChiarInStopModeCnt = 0;
            if(usAdcMotoATime1CCR3 > MotoInBreakJudeAdc)usMotoATime1CCR3_ChiarBreakCnt++;
            else usMotoATime1CCR3_ChiarBreakCnt = 0;
            if(usMotoATime1CCR3_ChiarInStopModeCnt > 250)usMotoATime1CCR3_ChiarInStopModeCnt = 250;
            if(usMotoATime1CCR3_ChiarBreakCnt > 250)usMotoATime1CCR3_ChiarBreakCnt = 250;
        }
        //////////////////////////////////////////////
        if(200 == usTime4CCR1Cnt )
        {
            ReDefTIM4CCR1 = 35;
            usAdcMotoBTime4CCR1 = uiInDownSpeedCalcMotoSate(MotoB, 1);
            ReDefTIM4CCR3 = ReDefTIM4CCR1 = ReDefTIM4CCR2 = ReDefTIM4CCR4 = 0;
            if((usAdcMotoBTime4CCR1 < (MotoInIdelAdcBaseAdc + MotoInRunJudeDeltAdc)) && (usAdcMotoBTime4CCR1 > (MotoInIdelAdcBaseAdc - MotoInRunJudeDeltAdc)))
            {
                usMotoBTime4CCR1_ChiarInStopModeCnt ++;
            }
            else usMotoBTime4CCR1_ChiarInStopModeCnt = 0;
            if(usAdcMotoBTime4CCR1 > MotoInBreakJudeAdc)usMotoBTime4CCR1_ChiarBreakCnt++;
            else usMotoBTime4CCR1_ChiarBreakCnt = 0;
            if(usMotoBTime4CCR1_ChiarInStopModeCnt > 250)usMotoBTime4CCR1_ChiarInStopModeCnt = 250;
            if(usMotoBTime4CCR1_ChiarBreakCnt > 250)usMotoBTime4CCR1_ChiarBreakCnt = 250;
        }
        if(400 == usTime4CCR3Cnt)
        {
            usTime4CCR1Cnt = usTime4CCR3Cnt = 0;
            ReDefTIM4CCR3 = 35;
            usAdcMotoBTime4CCR3 = uiInDownSpeedCalcMotoSate(MotoB, 1);
            ReDefTIM4CCR3 = ReDefTIM4CCR1 = ReDefTIM4CCR2 = ReDefTIM4CCR4 = 0;
            if((usAdcMotoBTime4CCR3 < (MotoInIdelAdcBaseAdc + MotoInRunJudeDeltAdc)) && (usAdcMotoBTime4CCR3 > (MotoInIdelAdcBaseAdc - MotoInRunJudeDeltAdc)))
            {
                usMotoBTime4CCR3_ChiarInStopModeCnt ++;
            }
            else usMotoBTime4CCR3_ChiarInStopModeCnt = 0;
            if(usAdcMotoBTime4CCR3 > MotoInBreakJudeAdc)usMotoBTime4CCR3_ChiarBreakCnt++;
            else usMotoBTime4CCR3_ChiarBreakCnt = 0;
            if(usMotoBTime4CCR3_ChiarInStopModeCnt > 250)usMotoBTime4CCR3_ChiarInStopModeCnt = 250;
            if(usMotoBTime4CCR3_ChiarBreakCnt > 250)usMotoBTime4CCR3_ChiarBreakCnt = 250;
        }
        //////////////////////////////////////////////
        if((usMotoATime1CCR1_ChiarInStopModeCnt >= CheckInStopTimes) && \
                (usMotoATime1CCR3_ChiarInStopModeCnt >= CheckInStopTimes) && \
                (usMotoBTime4CCR1_ChiarInStopModeCnt >= CheckInStopTimes) && \
                (usMotoBTime4CCR3_ChiarInStopModeCnt >= CheckInStopTimes))
        {
            usMotoBTime4CCR3_ChiarInStopModeCnt = usMotoBTime4CCR3_ChiarInStopModeCnt;
            ucErroType = ErroNoBearker;//如果
        }
        if((usMotoATime1CCR1_ChiarBreakCnt >= CheckBreakTimes) && \
                (usMotoATime1CCR3_ChiarBreakCnt >= CheckBreakTimes))
        {
            usMotoATime1CCR3_ChiarBreakCnt = usMotoATime1CCR3_ChiarBreakCnt;
            ucErroType = ErroNoBearker;//如果
        }
        if((usMotoBTime4CCR1_ChiarBreakCnt >= CheckBreakTimes) && \
                (usMotoBTime4CCR3_ChiarBreakCnt >= CheckBreakTimes))
        {
            usMotoBTime4CCR3_ChiarBreakCnt = usMotoBTime4CCR3_ChiarBreakCnt;
            ucErroType = ErroNoBearker;//如果
        }
        //////////////////////////////////////////////
        while(g_uiTimeNumCont1ms);
        if(ReadKeyOfOn_OffFlag)break;
#ifdef dSysPowerHold
        if((ErroSysPowerKeyPress != ucErroType) && ucErroType)break;
#else
        if((ErroSysPowerOff != ucErroType) && ucErroType)break;
#endif

    }

    if(uiTemp < MaxWaitTime - 3)
    {
        ucAskPowerOnNextTime = 2;
    }
    else ucAskPowerOnNextTime = 0;

#ifdef dSysPowerHold
    if((ErroSysPowerKeyPress == ucErroType) && (2 == ucAskPowerOnNextTime))
#else
    if((ErroSysPowerOff == ucErroType) && (2 == ucAskPowerOnNextTime))
#endif
    {
        C_DisEnResetStm8;
        vSendSingleOrder(FirstOrderPowerOn);
        ucAskPowerOnNextTime = 0;
        g_uiTimeNumCont1ms = 60;
        while(g_uiTimeNumCont1ms);

        vSendSingleOrder(FirstOrderPowerOn);
        g_uiTimeNumCont1ms = 60;
        while(g_uiTimeNumCont1ms);

        vSendSysPower(strSysInfo.uiSysTemPower + 20);//首次指示下电量
        g_uiTimeNumCont1ms = 60;
        while(g_uiTimeNumCont1ms);

        vSendSysPower(strSysInfo.uiSysTemPower - 20);//首次指示下电量
        g_uiTimeNumCont1ms = 60;
        while(g_uiTimeNumCont1ms);
        while(ReadKeyOfOn_OffFlag);
        g_uiTimeNumCont1ms = 150;
        while(g_uiTimeNumCont1ms);
        ucPowerKeyPressTimes = 0;
        ucErroType = ErroNoErro;
        //        BKP_WriteBackupRegister(BKP_DR1, ucAskPowerOnNextTime);
        return;
    }

    uiOldPwerValue = strSysInfo.uiSysTemPower;//将没有拉掉继电器前 的电量值暂存下
    CtlDriverPowerContolOFF;
    CtlSetMotor1LeftPwmPercent(PoweOffReleaseMosPWM);
    CtlSetMotor1RightPwmPercent(0);
    CtlSetMotor2LeftPwmPercent(PoweOffReleaseMosPWM);
    CtlSetMotor2RightPwmPercent(0);
    for(uiTemp = 0; uiTemp < 120; uiTemp++)
    {
        vSoftDelayms(1);
        if(uiOldPwerValue > strSysInfo.uiSysTemPower)
        {
            if((uiOldPwerValue - strSysInfo.uiSysTemPower) > PowerDownValue)uiTemp = 888;
        }//如果电量下降2v左右
        if(ucAskPowerOnNextTime || ReadKeyOfOn_OffFlag)break;
    }
    CtlSetMotor1LeftPwmPercent(0);
    CtlSetMotor1RightPwmPercent(0);
    CtlSetMotor2LeftPwmPercent(0);
    CtlSetMotor2RightPwmPercent(0);
    for(uiTemp = 0; uiTemp < 300; uiTemp++)
    {
        vSoftDelayms(1);
        if((uiOldPwerValue - strSysInfo.uiSysTemPower) > PowerDownValue)uiTemp = 888;
        if(ucAskPowerOnNextTime || ReadKeyOfOn_OffFlag)break;
    }
    /*
    uiOldPwerValue = strSysInfo.uiSysTemPower;//将没有拉掉继电器前 的电量值暂存下
     CtlDriverPowerContolOFF;
     CtlSetMotor1LeftPwmPercent(PoweOffReleaseMosPWM);
     CtlSetMotor1RightPwmPercent(0);
     CtlSetMotor2LeftPwmPercent(PoweOffReleaseMosPWM);
     CtlSetMotor2RightPwmPercent(0);
     for(uiTemp = 0; uiTemp < 120; uiTemp++)
     {
         vSoftDelayms(1);
         if(uiOldPwerValue > strSysInfo.uiSysTemPower)
         {
             if((uiOldPwerValue - strSysInfo.uiSysTemPower) > PowerDownValue)uiTemp = 888;
         }//如果电量下降2v左右
     }
     CtlSetMotor1LeftPwmPercent(0);
     CtlSetMotor1RightPwmPercent(0);
     CtlSetMotor2LeftPwmPercent(0);
     CtlSetMotor2RightPwmPercent(0);
     for(uiTemp = 0; uiTemp < 300; uiTemp++)
     {
         vSoftDelayms(1);
         if((uiOldPwerValue - strSysInfo.uiSysTemPower) > PowerDownValue)uiTemp = 888;
     }
     //printf("Stop2 ");
     */
#ifdef dSysPowerHold
    if((ErroSysPowerKeyPress != ucErroType) && ucErroType)ucAskPowerOnNextTime = 0;
#else
    if((ErroSysPowerOff != ucErroType) && ucErroType)ucAskPowerOnNextTime = 0;
#endif

    //    BKP_WriteBackupRegister(BKP_DR1, ucAskPowerOnNextTime);
    ucRunAsMode = SysInStandby;
    vConfigSysInStandbyMode();//还没有写到
}
/*
对ADC一些AD值做一些均值及滤波处理
*/

/* AdVoltageSample_t AdVoltageSample_RemoteYAdcCh;
 AdVoltageSample_t AdVoltageSample_RemoteXAdcCh;
 AdVoltageSample_t AdVoltageSample_Motor1RightHBridgeVolHaveFilterCh;
 AdVoltageSample_t AdVoltageSample_Motor1LeftHBridgeVolHaveFilterCh;
 AdVoltageSample_t AdVoltageSample_SysTemPowerAdcCh;
 AdVoltageSample_t AdVoltageSample_Motor2RightHBridgeVolHaveFilterCh;
 AdVoltageSample_t AdVoltageSample_Motor2LeftHBridgeVolHaveFilterCh;

 AdVoltageSample_t AdVoltageSample_Motor1DriveCurrent;
 AdVoltageSample_t AdVoltageSample_Motor2DriveCurrent;
 AdVoltageSample_t AdVoltageSample_BreakFeedBackCh;
void vAdcVauleInit(AdVoltageSample_t *p)
{
		p->usMaxData  = 0;
		p->usMinData  =  0xFFFF;
		p->usTempData  = 0;
		p->uiTempData  = 0;
}
void vAdcVauleRead(AdVoltageSample_t *p,u8 ucTemp,u8 AdcCh)
{
        p->usTempData = vDmaContAdcVaule[ucTemp][AdcCh];//DMA直接采样的数据
        p->uiTempData += p->usTempData;
        if(p->usTempData > p->usMaxData)
        {
            p->usMaxData = p->usTempData;
        }
        if(p->usTempData < p->usMinData)
        {
            p->usMinData = p->usTempData;
        }
}
void vDealAdcVaule(void)//运算一个周期413us
{
//    u16 usMaxData = 0, usMinData = 0xFFFF, usTempData;
//    u32 uiTempData = 0;
    u8 ucTemp;
//#define DividTimes 1
	vAdcVauleInit(&AdVoltageSample_RemoteYAdcCh);
	vAdcVauleInit(&AdVoltageSample_RemoteXAdcCh);
	vAdcVauleInit(&AdVoltageSample_Motor1RightHBridgeVolHaveFilterCh);
	vAdcVauleInit(&AdVoltageSample_Motor1LeftHBridgeVolHaveFilterCh);
	vAdcVauleInit(&AdVoltageSample_SysTemPowerAdcCh);
	vAdcVauleInit(&AdVoltageSample_Motor2RightHBridgeVolHaveFilterCh);
	vAdcVauleInit(&AdVoltageSample_Motor2LeftHBridgeVolHaveFilterCh);
	vAdcVauleInit(&AdVoltageSample_Motor1DriveCurrent);
	vAdcVauleInit(&AdVoltageSample_Motor2DriveCurrent);
	vAdcVauleInit(&AdVoltageSample_BreakFeedBackCh);
    //      		CtlTestLedON;
    //刹车器动作电压检测CH0
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
			  vAdcVauleRead(&AdVoltageSample_RemoteYAdcCh,ucTemp,RemoteYAdcCh);
			  vAdcVauleRead(&AdVoltageSample_RemoteXAdcCh,ucTemp,RemoteXAdcCh);
			  vAdcVauleRead(&AdVoltageSample_Motor1RightHBridgeVolHaveFilterCh,ucTemp,Motor1RightHBridgeVolHaveFilterCh);
			  vAdcVauleRead(&AdVoltageSample_Motor1LeftHBridgeVolHaveFilterCh,ucTemp,Motor1LeftHBridgeVolHaveFilterCh);
			  vAdcVauleRead(&AdVoltageSample_SysTemPowerAdcCh,ucTemp,SysTemPowerAdcCh);
			  vAdcVauleRead(&AdVoltageSample_Motor2RightHBridgeVolHaveFilterCh,ucTemp,Motor2RightHBridgeVolHaveFilterCh);
			  vAdcVauleRead(&AdVoltageSample_Motor2LeftHBridgeVolHaveFilterCh,ucTemp,Motor2LeftHBridgeVolHaveFilterCh);
			  vAdcVauleRead(&AdVoltageSample_Motor1DriveCurrent,ucTemp,Motor1DriveCurrent);
			  vAdcVauleRead(&AdVoltageSample_Motor2DriveCurrent,ucTemp,Motor2DriveCurrent);
			  vAdcVauleRead(&AdVoltageSample_BreakFeedBackCh,ucTemp,BreakFeedBackCh);
    }
//    uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的


		AdVoltageSample_RemoteYAdcCh.uiTempData = ((AdVoltageSample_RemoteYAdcCh.uiTempData - AdVoltageSample_RemoteYAdcCh.usMaxData - AdVoltageSample_RemoteYAdcCh.usMinData) /(OneChannelConvertTimes-2));
		AdVoltageSample_RemoteYAdcCh.uiTempData = (AdVoltageSample_RemoteYAdcCh.uiTempData >> 2); //由于在定时器里做的低通滤波器,一定不要用乘除法
    AdVoltageSample_RemoteYAdcCh.uiTempData += ((strSysInfo.uiRemoteNowYPos << 2) - strSysInfo.uiRemoteNowYPos) >> 2; // As C = S/4+(C*3)/4

    strSysInfo.uiRemoteNowYPos = AdVoltageSample_RemoteYAdcCh.uiTempData;


		AdVoltageSample_RemoteXAdcCh.uiTempData = ((AdVoltageSample_RemoteXAdcCh.uiTempData - AdVoltageSample_RemoteXAdcCh.usMaxData - AdVoltageSample_RemoteXAdcCh.usMinData) /(OneChannelConvertTimes-2));
		AdVoltageSample_RemoteXAdcCh.uiTempData = (AdVoltageSample_RemoteXAdcCh.uiTempData >> 2); //由于在定时器里做的低通滤波器,一定不要用乘除法
    AdVoltageSample_RemoteXAdcCh.uiTempData += ((strSysInfo.uiRemoteNowXPos << 2) - strSysInfo.uiRemoteNowXPos) >> 2; // As C = S/4+(C*3)/4
    strSysInfo.uiRemoteNowXPos = AdVoltageSample_RemoteXAdcCh.uiTempData;


 //   uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的
		AdVoltageSample_Motor1RightHBridgeVolHaveFilterCh.uiTempData = ((AdVoltageSample_Motor1RightHBridgeVolHaveFilterCh.uiTempData - AdVoltageSample_Motor1RightHBridgeVolHaveFilterCh.usMaxData - AdVoltageSample_Motor1RightHBridgeVolHaveFilterCh.usMinData) /(OneChannelConvertTimes-2));
    strMoto1Info.uiRightHBridgeHaveFilterAdc  = AdVoltageSample_Motor1RightHBridgeVolHaveFilterCh.uiTempData * 0.5 + strMoto1Info.uiRightHBridgeHaveFilterAdc * 0.5;

 //   uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的
		AdVoltageSample_Motor1LeftHBridgeVolHaveFilterCh.uiTempData = ((AdVoltageSample_Motor1LeftHBridgeVolHaveFilterCh.uiTempData - AdVoltageSample_Motor1LeftHBridgeVolHaveFilterCh.usMaxData - AdVoltageSample_Motor1LeftHBridgeVolHaveFilterCh.usMinData) /(OneChannelConvertTimes-2));
    strMoto1Info.uiLeftHBridgeHaveFilterAdc  = AdVoltageSample_Motor1LeftHBridgeVolHaveFilterCh.uiTempData * 0.5 + strMoto1Info.uiLeftHBridgeHaveFilterAdc * 0.5;

//    uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的

		AdVoltageSample_SysTemPowerAdcCh.uiTempData = ((AdVoltageSample_SysTemPowerAdcCh.uiTempData - AdVoltageSample_SysTemPowerAdcCh.usMaxData - AdVoltageSample_SysTemPowerAdcCh.usMinData) /(OneChannelConvertTimes-2));
		AdVoltageSample_SysTemPowerAdcCh.uiTempData = (AdVoltageSample_SysTemPowerAdcCh.uiTempData >> 2); //由于在定时器里做的低通滤波器,一定不要用乘除法
    AdVoltageSample_SysTemPowerAdcCh.uiTempData += ((strSysInfo.uiSysTemPower << 2) - strSysInfo.uiSysTemPower) >> 2; // As C = S/4+(C*3)/4
    strSysInfo.uiSysTemPower = AdVoltageSample_SysTemPowerAdcCh.uiTempData;

  //  uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的
		AdVoltageSample_Motor2LeftHBridgeVolHaveFilterCh.uiTempData = ((AdVoltageSample_Motor2LeftHBridgeVolHaveFilterCh.uiTempData - AdVoltageSample_Motor2LeftHBridgeVolHaveFilterCh.usMaxData - AdVoltageSample_Motor2LeftHBridgeVolHaveFilterCh.usMinData) /(OneChannelConvertTimes-2));
    strMoto2Info.uiLeftHBridgeHaveFilterAdc  = AdVoltageSample_Motor2LeftHBridgeVolHaveFilterCh.uiTempData * 0.5 + strMoto2Info.uiLeftHBridgeHaveFilterAdc * 0.5;

//    uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的
		AdVoltageSample_BreakFeedBackCh.uiTempData = ((AdVoltageSample_BreakFeedBackCh.uiTempData - AdVoltageSample_BreakFeedBackCh.usMaxData - AdVoltageSample_BreakFeedBackCh.usMinData) /(OneChannelConvertTimes-2));

    AdVoltageSample_BreakFeedBackCh.uiTempData = (AdVoltageSample_BreakFeedBackCh.uiTempData >> 2); //由于在定时器里做的低通滤波器,一定不要用乘除法
    AdVoltageSample_BreakFeedBackCh.uiTempData += ((strSysInfo.uiBreakFeedBackCurrent << 2) - strSysInfo.uiBreakFeedBackCurrent) >> 2; // As C = S/4+(C*3)/4
    strSysInfo.uiBreakFeedBackCurrent = AdVoltageSample_BreakFeedBackCh.uiTempData;

 //   uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的
		AdVoltageSample_Motor2RightHBridgeVolHaveFilterCh.uiTempData = ((AdVoltageSample_Motor2RightHBridgeVolHaveFilterCh.uiTempData - AdVoltageSample_Motor2RightHBridgeVolHaveFilterCh.usMaxData - AdVoltageSample_Motor2RightHBridgeVolHaveFilterCh.usMinData) /(OneChannelConvertTimes-2));
    strMoto2Info.uiRightHBridgeHaveFilterAdc  = AdVoltageSample_Motor2RightHBridgeVolHaveFilterCh.uiTempData * 0.5 + strMoto2Info.uiRightHBridgeHaveFilterAdc * 0.5;
    //////////////////////

 //   uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的
		AdVoltageSample_Motor2DriveCurrent.uiTempData = ((AdVoltageSample_Motor2DriveCurrent.uiTempData - AdVoltageSample_Motor2DriveCurrent.usMaxData - AdVoltageSample_Motor2DriveCurrent.usMinData) /(OneChannelConvertTimes-2));
    strMoto2Info.uiMotorCurrent = AdVoltageSample_Motor2DriveCurrent.uiTempData * 0.5 + strMoto2Info.uiMotorCurrent * 0.5;
    //////////////////////

  //  uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的
		AdVoltageSample_Motor1DriveCurrent.uiTempData = ((AdVoltageSample_Motor1DriveCurrent.uiTempData - AdVoltageSample_Motor1DriveCurrent.usMaxData - AdVoltageSample_Motor1DriveCurrent.usMinData) /(OneChannelConvertTimes-2));
    strMoto1Info.uiMotorCurrent = AdVoltageSample_Motor1DriveCurrent.uiTempData * 0.5 + strMoto1Info.uiMotorCurrent * 0.5;
}
*/
unsigned short usDebugX= 0;
unsigned short usDebugY= 0;

void vDealAdcVaule(void)//运算一个周期413us
{
    u16 usMaxData = 0, usMinData = 0xFFFF, usTempData;
    u32 uiTempData = 0;
    u8 ucTemp;
    //#define DividTimes 1
    //      		CtlTestLedON;
    //刹车器动作电压检测CH0
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][RemoteYAdcCh];//DMA直接采样的数据  strSysInfo.uiRemoteNowYPos
        uiTempData += usTempData;
        if(usTempData > usMaxData)
        {
            usMaxData = usTempData;
        }
        if(usTempData < usMinData)
        {
            usMinData = usTempData;
        }
    }
    //uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的
    uiTempData = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    uiTempData = (uiTempData >> 2); //由于在定时器里做的低通滤波器,一定不要用乘除法
    uiTempData += ((strSysInfo.uiRemoteNowYPos << 2) - strSysInfo.uiRemoteNowYPos) >> 2; // As C = S/4+(C*3)/4
    strSysInfo.uiRemoteNowYPos = uiTempData;
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    //系统电压采样  CH1
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][RemoteXAdcCh];//DMA直接采样的数据
        uiTempData += usTempData;
        if(usTempData > usMaxData)
        {
            usMaxData = usTempData;
        }
        if(usTempData < usMinData)
        {
            usMinData = usTempData;
        }
    }
    //uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的


    uiTempData = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    uiTempData = (uiTempData >> 2); //由于在定时器里做的低通滤波器,一定不要用乘除法
    uiTempData += ((strSysInfo.uiRemoteNowXPos << 2) - strSysInfo.uiRemoteNowXPos) >> 2; // As C = S/4+(C*3)/4
    strSysInfo.uiRemoteNowXPos = uiTempData;
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    //控制遥感X轴电压采样 CH2
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][Motor1RightHBridgeVolHaveFilterCh];//DMA直接采样的数据
        uiTempData += usTempData;
        if(usTempData > usMaxData)
        {
            usMaxData = usTempData;
        }
        if(usTempData < usMinData)
        {
            usMinData = usTempData;
        }
    }

    //uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的
    uiTempData = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    strMoto1Info.uiRightHBridgeHaveFilterAdc  = uiTempData * 0.5 + strMoto1Info.uiRightHBridgeHaveFilterAdc * 0.5;
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    //控制遥感Y轴电压采样 CH3
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][Motor1LeftHBridgeVolHaveFilterCh];//DMA直接采样的数据
        uiTempData += usTempData;
        if(usTempData > usMaxData)
        {
            usMaxData = usTempData;
        }
        if(usTempData < usMinData)
        {
            usMinData = usTempData;
        }
    }
    //uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的
    uiTempData = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    strMoto1Info.uiLeftHBridgeHaveFilterAdc  = uiTempData * 0.5 + strMoto1Info.uiLeftHBridgeHaveFilterAdc * 0.5;
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    //电机1驱动电压--左桥有硬件滤波  CH4
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][SysTemPowerAdcCh];//DMA直接采样的数据
        uiTempData += usTempData;
        if(usTempData > usMaxData)
        {
            usMaxData = usTempData;
        }
        if(usTempData < usMinData)
        {
            usMinData = usTempData;
        }
    }
    //uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的

    uiTempData = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    uiTempData = (uiTempData >> 2); //由于在定时器里做的低通滤波器,一定不要用乘除法
    uiTempData += ((strSysInfo.uiSysTemPower << 2) - strSysInfo.uiSysTemPower) >> 2; // As C = S/4+(C*3)/4
    strSysInfo.uiSysTemPower = uiTempData;
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    //电机1驱动电压--右桥有硬件滤波  CH5
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][Motor2LeftHBridgeVolHaveFilterCh];//DMA直接采样的数据
        uiTempData += usTempData;
        if(usTempData > usMaxData)
        {
            usMaxData = usTempData;
        }
        if(usTempData < usMinData)
        {
            usMinData = usTempData;
        }
    }
    //uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的
    uiTempData = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    strMoto2Info.uiLeftHBridgeHaveFilterAdc  = uiTempData * 0.5 + strMoto2Info.uiLeftHBridgeHaveFilterAdc * 0.5;
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    //电机2驱动电压--左桥有硬件滤波  CH6
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][BreakFeedBackCh];//DMA直接采样的数据
        uiTempData += usTempData;
        if(usTempData > usMaxData)
        {
            usMaxData = usTempData;
        }
        if(usTempData < usMinData)
        {
            usMinData = usTempData;
        }
    }
    //uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的
    uiTempData = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));

    uiTempData = (uiTempData >> 2); //由于在定时器里做的低通滤波器,一定不要用乘除法
    uiTempData += ((strSysInfo.uiBreakFeedBackCurrent << 2) - strSysInfo.uiBreakFeedBackCurrent) >> 2; // As C = S/4+(C*3)/4
    strSysInfo.uiBreakFeedBackCurrent = uiTempData;
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    //电机2驱动电压--右桥有硬件滤波  CH7
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][Motor2RightHBridgeVolHaveFilterCh];//DMA直接采样的数据
        uiTempData += usTempData;
        if(usTempData > usMaxData)
        {
            usMaxData = usTempData;
        }
        if(usTempData < usMinData)
        {
            usMinData = usTempData;
        }
    }
    //uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的
    uiTempData = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    strMoto2Info.uiRightHBridgeHaveFilterAdc  = uiTempData * 0.5 + strMoto2Info.uiRightHBridgeHaveFilterAdc * 0.5;
    //////////////////////
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    ////PA0 电机2的电流检测
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][Motor2DriveCurrent];//DMA直接采样的数据
        uiTempData += usTempData;
        if(usTempData > usMaxData)
        {
            usMaxData = usTempData;
        }
        if(usTempData < usMinData)
        {
            usMinData = usTempData;
        }
    }
    //uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的
    uiTempData = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    strMoto2Info.uiMotorCurrent = uiTempData * 0.5 + strMoto2Info.uiMotorCurrent * 0.5;
    //////////////////////
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    //PA5 电机1的电流检测
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][Motor1DriveCurrent];//DMA直接采样的数据
        uiTempData += usTempData;
        if(usTempData > usMaxData)
        {
            usMaxData = usTempData;
        }
        if(usTempData < usMinData)
        {
            usMinData = usTempData;
        }
    }
    //uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的
    uiTempData = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    strMoto1Info.uiMotorCurrent = uiTempData * 0.5 + strMoto1Info.uiMotorCurrent * 0.5;
}

extern unsigned char ucFirstStartOneTimeFlag;
void vCheckHardWareErro(void)//函数里顺便做下按键的值检测周期5ms
{
    if((ucFirstStartOneTimeFlag) && (strSysInfo.uiRemoteNowXPos < strSysInfo.uiRemoteErroMaxValue)
            && (strSysInfo.uiRemoteNowYPos < strSysInfo.uiRemoteErroMaxValue))
    {
        ucErroType = ErroNoRemote;//如果摇杆值的错
    }
    if(ReadKeyOfOn_OffFlag)
    {
        CtrlExtSpeekerDis;//如果有电源开关按键立即停止输出
#ifdef dSysPowerHold
        if(!ucErroType)ucErroType = ErroSysPowerKeyPress;
#else
        if(!ucErroType)ucErroType = ErroSysPowerOff;
#endif
        ucKeyUpTime++;
        ucKeyUpTime %= 200;
        if(ucKeyUpTime >= KeyHoldTime)
        {
            if(KeyPressUp == ucPowerKeySate)ucPowerKeyPressTimes ++;
            ucKeyUpTime %= 200;
            ucPowerKeySate = KeyPressDown;
        }
    }
    else
    {
        ucPowerKeySate = KeyPressUp;
        ucKeyUpTime = 0;
    }
    //					if((strMoto1Info.uiMotorCurrent < 500)||(strMoto1Info.uiMotorCurrent > 3500))
    //						{
    //							vShowErroToDis(ErroRelayShort);//如果在开机或者在运行过程中采样的电流很小，说明出问题了
    //						}
    //				    if((strMoto2Info.uiMotorCurrent < 500)||(strMoto2Info.uiMotorCurrent > 3500))
    //						{
    //							vShowErroToDis(ErroRelayShort);//如果在开机或者在运行过程中采样的电流很小，说明出问题了
    //						}
}
//显示充电状态
void vShowPoweChange(void)
{
    vDownSpeedCalcPwm(TRUE);
    CtlSetMotor1RightPwmPercent(0);
    CtlSetMotor1LeftPwmPercent(0);
    CtlSetMotor2RightPwmPercent(0);
    CtlSetMotor2LeftPwmPercent(0);//在充电的时候释放所有力量
    vSendSingleOrder(OrderInChange);
    while(ReadTeachPinIsConect)
    {
        ucTag10ms = FALSE;
        while(FALSE == ucTag10ms);//如果在此过程中有错误及按键情况，就去关机
        if(ReadKeyOfOn_OffFlag || (ErroSysPowerOff == ucErroType))
        {
            //
            vSendSingleOrder(OrderOutChange);
            ucErroType = ErroSysPowerOff;
            ucStsTemState = ErroMode;
            return;
        }
    }
    vSendSingleOrder(OrderOutChange);
}
//在运行过程中检查硬件错误
#define IdelModeCheckAdValue    60//自检测电机连接线的电压
#define RollingVoltage          23

//////////////////////////////////////////////参数及宏定义如下;

//自动驻坡功能
//在这里如果车子出现 不平衡刹车的问题的时候  车子不在纠正是否可以进行刹车处理
//

#ifdef STM32
#define WaitNoneSampleSlopAd    20000  //采样次数的标定
#endif

#ifdef GD32
#define WaitNoneSampleSlopAd    25000
#endif

#define NeedCurrentCloseLoop        //对电流闭环是否加入标志
#define CloseLoopTime           500//390  一个闭环的计数时间



//在进入驻坡功能前，在待定上次的状态   可以允许一个小坡道的溜坡么？
//在等待摇杆 是否在中心点的时间里。可以做一个电流的闭环，如果在此期间没有一个电流值
//可以作减少PWM的处理，如果电流持续减小，说明车子在平地的状态。如果一旦摇杆的释放 就去做这样的动作
//
/*
为提高采样速度，这里尽量不做一些乘除法 运算。
也只采样关键数据，摇杆的信号为了及时退出
电机的信号及电流的信号 不再更新 电池电压
*/
void vDealAdcVauleMotoAndCurrent(void)
{
    u16 usMaxData = 0, usMinData = 0xFFFF, usTempData;
    u32 uiTempData = 0;
    u8 ucTemp;
    //#define DividTimes 1
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][RemoteYAdcCh];//DMA直接采样的数据
        uiTempData += usTempData;
        if(usTempData > usMaxData)
        {
            usMaxData = usTempData;
        }
        if(usTempData < usMinData)
        {
            usMinData = usTempData;
        }
    }
    // strSysInfo.uiRemoteNowYPos = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的

    //    strSysInfo.uiRemoteNowYPos = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    //系统电压采样  CH1
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][RemoteXAdcCh];//DMA直接采样的数据
        uiTempData += usTempData;
        if(usTempData > usMaxData)
        {
            usMaxData = usTempData;
        }
        if(usTempData < usMinData)
        {
            usMinData = usTempData;
        }
    }
    // strSysInfo.uiRemoteNowXPos = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的
    //    strSysInfo.uiRemoteNowXPos = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    usMaxData = 0;
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][Motor1RightHBridgeVolHaveFilterCh];//DMA直接采样的数据
        uiTempData += usTempData;
        if(usTempData > usMaxData)
        {
            usMaxData = usTempData;
        }
        if(usTempData < usMinData)
        {
            usMinData = usTempData;
        }
    }
    // strMoto1Info.uiRightHBridgeHaveFilterAdc = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的
    strMoto1Info.uiRightHBridgeHaveFilterAdc = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2)); //这里的OneChannelConvertTimes一定是10才好使的


    //////////////////////
    //控制遥感Y轴电压采样 CH3
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][Motor1LeftHBridgeVolHaveFilterCh];//DMA直接采样的数据
        uiTempData += usTempData;
        if(usTempData > usMaxData)
        {
            usMaxData = usTempData;
        }
        if(usTempData < usMinData)
        {
            usMinData = usTempData;
        }
    }
    // strMoto1Info.uiLeftHBridgeHaveFilterAdc = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的
    strMoto1Info.uiLeftHBridgeHaveFilterAdc = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    //电机1驱动电压--右桥有硬件滤波  CH5
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][Motor2LeftHBridgeVolHaveFilterCh];//DMA直接采样的数据
        uiTempData += usTempData;
        if(usTempData > usMaxData)
        {
            usMaxData = usTempData;
        }
        if(usTempData < usMinData)
        {
            usMinData = usTempData;
        }
    }
    //  strMoto2Info.uiLeftHBridgeHaveFilterAdc = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的
    strMoto2Info.uiLeftHBridgeHaveFilterAdc = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    //电机2驱动电压--右桥有硬件滤波  CH7
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][Motor2RightHBridgeVolHaveFilterCh];//DMA直接采样的数据
        uiTempData += usTempData;
        if(usTempData > usMaxData)
        {
            usMaxData = usTempData;
        }
        if(usTempData < usMinData)
        {
            usMinData = usTempData;
        }
    }
    // strMoto2Info.uiRightHBridgeHaveFilterAdc = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的
    strMoto2Info.uiRightHBridgeHaveFilterAdc = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    //////////////////////
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    ////PA0 电机2的电流检测
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][Motor2DriveCurrent];//DMA直接采样的数据
        uiTempData += usTempData;
        if(usTempData > usMaxData)
        {
            usMaxData = usTempData;
        }
        if(usTempData < usMinData)
        {
            usMinData = usTempData;
        }
    }
    //strMoto2Info.uiMotorCurrent = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的
    strMoto2Info.uiMotorCurrent = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    //////////////////////
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    //PA5 电机1的电流检测
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][Motor1DriveCurrent];//DMA直接采样的数据
        uiTempData += usTempData;
        if(usTempData > usMaxData)
        {
            usMaxData = usTempData;
        }
        if(usTempData < usMinData)
        {
            usMinData = usTempData;
        }
    }
    //strMoto1Info.uiMotorCurrent = (uiTempData - usMaxData - usMinData) >> DividTimes;//这里的OneChannelConvertTimes一定是10才好使的
    strMoto1Info.uiMotorCurrent = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));

}




//
//在停坡的时候 对轮椅车的大脉动调整
//

unsigned char ucJudgeMotoBreakResult = 0;

extern unsigned char ucInChargePinLowCont;

void SlopModeAdjus_Init(SLOPMODEADJUST_t *p)
{
    unsigned int uiTimeCont = 0;
    //	if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)
    //	{
    //		p->uiSampleAdMode = SampleAdCheckMode;
    //		p->uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
    //		p->uiReadSampleAdTime = SampleAdCheckMode_MotoATime;
    //	}
    //	else
    //	{
    p->uiSampleAdMode = SampleAdJudgeMode;
    p->uiSetHighTime = SmallWheelJudgeMode_SetPwmHighTime;
    p->uiReadSampleAdTime = SampleAdJudgeMode_MotoATime;
    //	}
    p->uiZeroTestPhase = 0;
    p->fSetCCR1CCR2PwmValue = 0.0;
    p->fSetCCR3CCR4PwmValue = 0.0;
    p->fReadOldCCR1CCR2PwmValue = 0.0;
    p->fReadOldCCR3CCR4PwmValue = 0.0;
    p->uiSampleAdCnt = 0;
    p->uiSampleAdSetPWMFinishCnt = 0;
    p->fVoltageSample = 0;//电机ad采样计算变量 平均值
    p->uiReadVoltageTime = 0;

    for(uiTimeCont = 0; uiTimeCont < BigWheelJudgeMode_SetPwmHighTime; uiTimeCont++)
    {
        p->fVoltage[uiTimeCont] = 0;
    }
    p->uiSetPwmTimerCCR1_Flag = 0;
    p->uiSetPwmTimerCCR3_Flag = 0;//给定PWM计数器
    p->fSetPwmStep = 0;//每一步减少PWM的值的定义
    p->uiSampleAdJudgeMode_SetCnt = 0;
    p->uCheckMode_SetTime = 1;
    p->uiSampleAdSetPWMFinishFlag = 0;
    p->uiSampleAdSetIntoBreakCnt = 0;
    for(uiTimeCont = 0; uiTimeCont < CurrentReadTimes; uiTimeCont++)
    {
        p->iArryCurrentSample[uiTimeCont] = 0;//电流的采样数组
    }
    p->uiSampleAdJudgeModeFinishFlag = 0;
    p->uiSampleAdJudgeModeCnt = 0;
    p->uiPeopleStandUpFlag = 0;
    p->uiJudgeMode_SetPwmHighTime = SmallWheelJudgeMode_SetPwmHighTime;
    p->uJudgeModeIntoCheckModeCnt = 0;
    p->uCheckMode_ReadTime = p->uCheckMode_ReadTime = 0;
}


void vAdjustPwmAtSlopModeFastAdjus(void)
{
    FAULTCODE_u FaultCode;
    //	  FaultCode.all = 0;
    float MaxSlopPWM = IntoSlopPWM;
    SLOPMODEADJUST_t  MotoASlopModeAdjust;
    SLOPMODEADJUST_t  MotoBSlopModeAdjust;
#ifdef dSysPowerHold
    static unsigned int ucInSlopPowerOffTimeCnt = 0;
    unsigned char ucInSlopButDK_DisOff = FALSE;//如果是在驻坡中，但电源按键有接下。上控指示灯不亮标志
#endif
    unsigned int uiTimeCont = 0;
    u32 uiUartTimeCont = 0;
    unsigned char  ucSlopeRuningFlag = 1;
    unsigned char ucShowErro = 0;
    unsigned int MotoErrTime = 0;
    unsigned int ClearErrTime = 0;
    unsigned char ucCont_MotoBreak = 0;
    strSysInfo.uiSysBreakFlag = 0;

    SlopModeAdjus_Init(&MotoASlopModeAdjust);
    SlopModeAdjus_Init(&MotoBSlopModeAdjust);
    if((FALSE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn) && (FALSE ==  g_ucLowPwmIntoBeadk))//是驻坡程序版本
    {
        if((ReDefTIM1CCR1 >= 300) || (ReDefTIM1CCR3 >= 300) || (ReDefTIM4CCR1 >= 300) || (ReDefTIM4CCR3 >= 300)) //如果的驻坡的
        {
            for(uiTimeCont = 0 ; uiTimeCont < 380; uiTimeCont ++)
            {
                g_uiTimeNumCont1ms = 2;
                while(g_uiTimeNumCont1ms);
                if(SysNoDirection != ucCalcDirection())break;//如果在等待的时候的刹车进入，立即退出操作
            }
        }
        else
        {
            for(uiTimeCont = 0 ; uiTimeCont < 680; uiTimeCont ++)
            {
                g_uiTimeNumCont1ms = 2;
                while(g_uiTimeNumCont1ms);
                if(SysNoDirection != ucCalcDirection())break;//如果在等待的时候的刹车进入，立即退出操作
            }
        }
    }
    uiTimeCont = 0;
    //----------------------------------------------------最大驻坡给定值处理----------------------------------------------------------/
    if(uniDiverInfo.strDirverAtt.ucBackMinSpeedP >= MaxSetSlopPWM)
    {
        uniDiverInfo.strDirverAtt.ucBackMinSpeedP = MaxSetSlopPWM;
    }
    MaxSlopPWM = uniDiverInfo.strDirverAtt.ucBackMinSpeedP;
    MaxSlopPWM = (unsigned int)(MaxSlopPWM * 36); //*3600/100
    //----------------------------------------------------最大PWM给定值处理----------------------------------------------------------/
    if(ReDefTIM1CCR1 > 50)//对每个电机的PWM 的赋值定义
    {
        if(ReDefTIM1CCR1 > MaxSlopPWM)//最大占空比限幅
        {
            ReDefTIM1CCR1 = ReDefTIM1CCR2 = MaxSlopPWM;
        }
        MotoASlopModeAdjust.fSetCCR1CCR2PwmValue = ReDefTIM1CCR1;
        MotoASlopModeAdjust.fReadOldCCR1CCR2PwmValue = MotoASlopModeAdjust.fSetCCR1CCR2PwmValue;
    }
    if(ReDefTIM1CCR3 > 50)
    {
        if(ReDefTIM1CCR3 > MaxSlopPWM)//最大占空比限幅
        {
            ReDefTIM1CCR3 = ReDefTIM1CCR4 = MaxSlopPWM;
        }
        MotoASlopModeAdjust.fSetCCR3CCR4PwmValue = ReDefTIM1CCR3;
        MotoASlopModeAdjust.fReadOldCCR3CCR4PwmValue = ReDefTIM1CCR3;
    }
    if(ReDefTIM4CCR1 > 50)
    {
        if(ReDefTIM4CCR1 > MaxSlopPWM)//最大占空比限幅
        {
            ReDefTIM4CCR1 = ReDefTIM4CCR2 = MaxSlopPWM;
        }
        MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue = ReDefTIM4CCR1;
        MotoBSlopModeAdjust.fReadOldCCR1CCR2PwmValue = MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue;
    }
    if(ReDefTIM4CCR3 > 50)
    {
        if(ReDefTIM4CCR3 > MaxSlopPWM)//最大占空比限幅
        {
            ReDefTIM4CCR3 = ReDefTIM4CCR4 = MaxSlopPWM;
        }
        MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue = ReDefTIM4CCR3;
        MotoBSlopModeAdjust.fReadOldCCR3CCR4PwmValue = MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue;
    }
    ucCalcBrakeMotoACurrent = 0;
    ucCalcBrakeMotoBCurrent = 0;
    for(uiTimeCont = 0; uiTimeCont < OneChannelConvertTimes; uiTimeCont++)
    {
        ucCalcBrakeMotoACurrent += vDmaContAdcVaule[uiTimeCont][Motor1DriveCurrent];
        ucCalcBrakeMotoBCurrent += vDmaContAdcVaule[uiTimeCont][Motor2DriveCurrent];
    }
    ucCalcBrakeMotoACurrent /= OneChannelConvertTimes;
    ucCalcBrakeMotoBCurrent /= OneChannelConvertTimes;
    if(((ucCalcBrakeMotoACurrent > IfMotoACurrentAtStep1High) && (ucCalcBrakeMotoBCurrent > IfMotoBCurrentAtStep2High)) || \
            ((ucCalcBrakeMotoBCurrent > IfMotoBCurrentAtStep1High) && (ucCalcBrakeMotoACurrent > IfMotoACurrentAtStep2High)))
    {
        if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)
        {
            MotoASlopModeAdjust.uiSampleAdSetPWMFinishFlag = 1;
            MotoBSlopModeAdjust.uiSampleAdSetPWMFinishFlag = 1;
        }
        MotoASlopModeAdjust.uiSampleAdMode = SampleAdCheckMode;
        MotoBSlopModeAdjust.uiSampleAdMode = SampleAdCheckMode;
        MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdCheckMode_MotoATime;
        MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdCheckMode_MotoBTime;
    }
    if((ReDefTIM1CCR1 == 0) && (ReDefTIM1CCR3 == 0) && (ReDefTIM4CCR1 == 0) && (ReDefTIM4CCR3 == 0))
    {
        if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)
        {
            MotoASlopModeAdjust.uiSampleAdSetPWMFinishFlag = 1;
            MotoBSlopModeAdjust.uiSampleAdSetPWMFinishFlag = 1;
        }
    }
    while(ucSlopeRuningFlag)
    {
        if(Globle_Timer_1MS.Bits.TestSlopTime > 0)		//0.5MS执行一次
        {
            //C_MonitorSTM8PinXor;
            Globle_Timer_1MS.Bits.TestSlopTime = 0;


            //----------------------------------------------------故障处理----------------------------------------------------------/
            //	   (Moto1AStmPwmReg) > (Moto1BStmPwmReg) ? (Moto1AStmPwmReg) : (Moto1BStmPwmReg);
            if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bCheckMotorConnect)
            {
                ucCont_MotoBreak = 0;
                ucCont_MotoBreak = MotoABreakFlagOne | MotoABreakFlagTwo;
                if((ucJudgeMotoBreakResult & ucCont_MotoBreak) == ucCont_MotoBreak) //确定一个唯一的故障
                {
                    FaultCode.bit.MotoAHighBriageBreak = 1;
                }
                else
                {
                    FaultCode.bit.MotoAHighBriageBreak = 0;
                }
                ucCont_MotoBreak = 0;
                ucCont_MotoBreak = MotoBBreakFlagOne | MotoBBreakFlagTwo;
                if((ucJudgeMotoBreakResult & ucCont_MotoBreak) == ucCont_MotoBreak) //确定一个唯一的故障
                {
                    FaultCode.bit.MotoBHighBriageBreak = 1;
                }
                else
                {
                    FaultCode.bit.MotoBHighBriageBreak = 0;
                }
            }
            else
            {
                FaultCode.bit.MotoAHighBriageBreak = 0;
                FaultCode.bit.MotoBHighBriageBreak = 0;
            }
            if(FaultCode.bit.MotoAHighBriageBreak)
            {
                ClearErrTime = 0;
                MotoErrTime++;
                if(MotoErrTime >= Timer_5MS);//Timer_400MS)
                {
                    MotoErrTime	= 	0;
                    ucShowErro = 0x02;
                }

            }
            else if(FaultCode.bit.MotoBHighBriageBreak)
            {
                ClearErrTime = 0;
                MotoErrTime++;
                if(MotoErrTime >= Timer_5MS);//Timer_400MS)
                {
                    MotoErrTime	= 	0;
                    ucShowErro = 0x04;
                }
            }
            else if((TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn) && (TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bCheckBreakConnect) && (strSysInfo.uiSysBreakFlag))
            {
                if(C_ReadLeftMotoBreak_Pin_Low)
                {
                    ClearErrTime = 0;
                    MotoErrTime++;
                    if(MotoErrTime >= Timer_400MS)
                    {
                        MotoErrTime	= 	0;
                        ucShowErro = 0x03; //左刹车器坏掉
                        if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor1Motor2)ucShowErro = 0x05; //右刹车器坏掉
                    }
                }
                else if(C_ReadRightMotoBreak_Pin_Low)
                {
                    ClearErrTime = 0;
                    MotoErrTime++;
                    if(MotoErrTime >= Timer_400MS)
                    {
                        MotoErrTime	= 	0;
                        ucShowErro = 0x05;  //右刹车器坏掉
                        if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor1Motor2)ucShowErro = 0x03;  //左刹车器坏掉
                    }
                }
                else
                {
                    MotoErrTime = 0;
                    ClearErrTime++;
                }
            }
            else
            {
                MotoErrTime = 0;
                ClearErrTime++;
            }

            //----------------------------------------------------电量处理----------------------------------------------------------/
            if(((ReDefTIM1CCR1) < TimerBase) && ((ReDefTIM1CCR3) < TimerBase) && ((ReDefTIM4CCR1) < TimerBase) && ((ReDefTIM4CCR3) < TimerBase))//如果PWM小
            {
                uiUartTimeCont++;
                if((Timer_5MS - 1) == uiUartTimeCont % Timer_5MS)
                {
                    vCalcBataAsShow(FALSE);//如果是在空闲的时候
                }
                if((Timer_400MS - 1) == uiUartTimeCont % Timer_400MS) //如果在空闲的状态下，如果有示教器发过来的数据这时候要去处理下
                {
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
                    if(ucAnlyFree)vAnalyTeacherDataOfTime();//
                }
                if(uiUartTimeCont >= Timer_800MS)
                {
                    //----------------------------------------------------最大驻坡给定值处理----------------------------------------------------------/
                    if(uniDiverInfo.strDirverAtt.ucBackMinSpeedP >= MaxSetSlopPWM)
                    {
                        uniDiverInfo.strDirverAtt.ucBackMinSpeedP = MaxSetSlopPWM;
                    }
                    MaxSlopPWM = uniDiverInfo.strDirverAtt.ucBackMinSpeedP;
                    MaxSlopPWM = (unsigned int)(MaxSlopPWM * 36); //*3600/100
                    //----------------------------------------------------发送电量值----------------------------------------------------------/
                    if(ucShowErro == 0)
                    {
                        ClearErrTime = 0;
                        uiUartTimeCont = 0;
                    }
                    vSendSysPower(strSysInfo.uiCalcSysShowPower);//电池电量
                }
                if(uiUartTimeCont >= Timer_1S)
                {
                    //----------------------------------------------------发送和退出发送电机故障告警----------------------------------------------------------/
                    uiUartTimeCont = 0;
                    if(ClearErrTime >= Timer_3S)
                    {
                        ClearErrTime = 0;
                        ucShowErro &= 0xF8;
                        vSendSysPower(strSysInfo.uiSysTemPower + 15);//再次开启的时候 让电量再指示下
                        vSendSysPower(strSysInfo.uiSysTemPower - 15);//这里让上控制有一个有效的指示
                    }
                    else if(ClearErrTime >= Timer_2S)
                    {
                        C_DisEnResetStm8;//不让上控复位
                        vSendSingleOrder(FirstOrderPowerOn);

                    }
                    else if(ClearErrTime >= Timer_1S)
                    {
                        vSendSingleOrder(QuitErro); //不再提示故障
                        C_EnResetStm8;//让上面板复位
                    }
                    else
                    {
                        vSendOneByteOrder(OrderErro, ucShowErro);	//发送按键板，故障提示告警
                    }
                }
            }
            //----------------------------------------------------退出驻坡处理----------------------------------------------------------/
						
#ifdef dSysPowerHold
            if((SysNoDirection != ucCalcDirection()) && (ucShowErro == 0) && (FALSE == ucInSlopButDK_DisOff)) //||(ucErroType) )//如果灯在关的状态，动摇杆不再动作
#else
            if(((SysNoDirection != ucCalcDirection()) && (ucShowErro == 0)) || (ucErroType) || (ucPowerKeySate == KeyPressDown))
#endif
            {
                ucFrontAutoSlopFlag = FALSE;
                ucSlopeRuningFlag = 0;
                strSysInfo.uiSysBreakFlag = 0;
                if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)
                {
                    if((MotoASlopModeAdjust.fSetCCR1CCR2PwmValue > LowSlopPWM) && (MotoASlopModeAdjust.fSetCCR1CCR2PwmValue <= MaxSlopPWM))
                    {
                        ReDefTIM1CCR1 = ReDefTIM1CCR2 = MotoASlopModeAdjust.fSetCCR1CCR2PwmValue;
                        ReDefTIM1CCR3 = ReDefTIM1CCR4 = 0;
                    }
                    else if((MotoASlopModeAdjust.fSetCCR3CCR4PwmValue > LowSlopPWM) && (MotoASlopModeAdjust.fSetCCR3CCR4PwmValue <= MaxSlopPWM))
                    {
                        ReDefTIM1CCR1 = ReDefTIM1CCR2 = 0;
                        ReDefTIM1CCR3 = ReDefTIM1CCR4 = MotoASlopModeAdjust.fSetCCR3CCR4PwmValue;
                    }
                    else
                    {
                        ReDefTIM1CCR1 = ReDefTIM1CCR2 = 0;
                        ReDefTIM1CCR3 = ReDefTIM1CCR4 = 0;
                    }
                    if((MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue > LowSlopPWM) && (MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue <= MaxSlopPWM))
                    {
                        ReDefTIM4CCR1 = ReDefTIM4CCR2 = MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue;
                        ReDefTIM4CCR3 = ReDefTIM4CCR4 = 0;
                    }
                    else if((MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue > LowSlopPWM) && (MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue <= MaxSlopPWM))
                    {
                        ReDefTIM4CCR1 = ReDefTIM4CCR2 = 0;
                        ReDefTIM4CCR3 = ReDefTIM4CCR4 = MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue;
                    }
                    else
                    {
                        ReDefTIM4CCR1 = ReDefTIM4CCR2 = 0;
                        ReDefTIM4CCR3 = ReDefTIM4CCR4 = 0;
                    }
                }
                if((ReDefTIM1CCR1 > MaxSlopPWM) || (ReDefTIM1CCR1 < LowSlopPWM)) //最大占空比限幅 因为退出驻坡时可能是正在检测是否溜车时，而当前可能正输出高电平
                {
                    ReDefTIM1CCR1 = ReDefTIM1CCR2 = 0;
                }
                if((ReDefTIM1CCR3 > MaxSlopPWM) || (ReDefTIM1CCR3 < LowSlopPWM)) //最大占空比限幅
                {
                    ReDefTIM1CCR3 = ReDefTIM1CCR4 = 0;
                }
                if((ReDefTIM4CCR1 > MaxSlopPWM) || (ReDefTIM4CCR1 < LowSlopPWM)) //最大占空比限幅
                {
                    ReDefTIM4CCR1 = ReDefTIM4CCR2 = 0;
                }
                if((ReDefTIM4CCR3 > MaxSlopPWM) || (ReDefTIM4CCR3 < LowSlopPWM)) //最大占空比限幅
                {
                    ReDefTIM4CCR3 = ReDefTIM4CCR4 = 0;
                }
                if((Moto1AStmPwmReg) || (Moto1BStmPwmReg))
                {
                    strMoto1Info.uiOldMotorPwmValue = (Moto1AStmPwmReg) > (Moto1BStmPwmReg) ? (Moto1AStmPwmReg) : (Moto1BStmPwmReg);
                    if(Moto1AStmPwmReg) strMoto1Info.unMotorOldDirection = Forward;
                    else strMoto1Info.unMotorOldDirection = Reverse;	  //求上次电机的PWM并确定出上次电机的转动方向
                }
                else
                {
                    strMoto1Info.uiOldMotorPwmValue  = 0;
                    strMoto1Info.unMotorOldDirection = NoDirection;
                }
                if((Moto2AStmPwmReg) || (Moto2BStmPwmReg))
                {
                    strMoto2Info.uiOldMotorPwmValue = (Moto2AStmPwmReg) > (Moto2BStmPwmReg) ? (Moto2AStmPwmReg) : (Moto2BStmPwmReg);
                    if(Moto2AStmPwmReg) strMoto2Info.unMotorOldDirection = Forward;
                    else strMoto2Info.unMotorOldDirection = Reverse;   //求上次电机的PWM并确定出上次电机的转动方向
                }
                else
                {
                    strMoto2Info.uiOldMotorPwmValue  = 0;
                    strMoto2Info.unMotorOldDirection = NoDirection;
                }
                ucSlopeRuningFlag = 0;
                MotoASlopModeAdjust.uiSampleAdSetIntoBreakCnt = 0;
                MotoASlopModeAdjust.uiSampleAdSetPWMFinishFlag = 0;


                MotoBSlopModeAdjust.uiSampleAdSetIntoBreakCnt = 0;
                MotoBSlopModeAdjust.uiSampleAdSetPWMFinishFlag = 0;

                return;
            }

            //---------------------------------------------------有关机键按下，假关机模块处理----------------------------------------------------------/
#ifdef dSysPowerHold
            if(TRUE == ucInSlopButDK_DisOff)//如果按键显示板不再显示，如果PWM比较小的时候就去关机
            {
                //---------------------------------------------30分钟关机--------------------------------------------------//
                ucInSlopPowerOffTimeCnt++;
                if(ucInSlopPowerOffTimeCnt >= Timer_30Min)//30分钟关机Timer_30Min
                {
                    ucInSlopPowerOffTimeCnt = Timer_30Min;
                    ReDefTIM1CCR1 = ReDefTIM1CCR2 = 0;
                    ReDefTIM1CCR3 = ReDefTIM1CCR4 = 0;
                    ReDefTIM4CCR1 = ReDefTIM4CCR2 = 0;
                    ReDefTIM4CCR3 = ReDefTIM4CCR4 = 0;
                    //											  ucErroType = ErroSysPowerKeyPress;//就去关机
                    //												return;

                }
                //---------------------------------------------占空比小就关机--------------------------------------------------//
                //									if(((ReDefTIM1CCR1) < PowerOffSlopPWM) && ((ReDefTIM1CCR3) < PowerOffSlopPWM) && ((ReDefTIM4CCR1) < PowerOffSlopPWM) && ((ReDefTIM4CCR3) < PowerOffSlopPWM))//如果PWM小
                //									{
                //											ucErroType = ErroSysPowerKeyPress;//就去关机
                //									}
            }
            else
            {
                ucInSlopPowerOffTimeCnt = 0;
            }
            if(ucErroType)//如果有错误的情况的  如果有故障的时候
            {
                if(ErroSysPowerKeyPress == ucErroType)//如果是有按键动作
                {
                    if(((ReDefTIM1CCR1) <= MaxSlopPWM) && ((ReDefTIM1CCR3) <= MaxSlopPWM) && ((ReDefTIM4CCR1) <= MaxSlopPWM) && ((ReDefTIM4CCR3) <= MaxSlopPWM))
                    {

                        if(FALSE == ucInSlopButDK_DisOff)//如果按键有按下关闭上控
                        {
                            if(((ReDefTIM1CCR1) < PowerOffSlopPWM) && ((ReDefTIM1CCR3) < PowerOffSlopPWM) && ((ReDefTIM4CCR1) < PowerOffSlopPWM) && ((ReDefTIM4CCR3) < PowerOffSlopPWM))
                            {
                                return;//如果PWM小就退出
                            }
                            else//如果PWM比较大，就不要关机了
                            {
                                ucErroType = ErroNoErro;//不让关机
                                C_EnResetStm8;//让上面板复位
                                ucInSlopButDK_DisOff = TRUE;//上面板不再显示标志
                                ucInSlopButDK_DisOff |= 0x80;//防止再执行到一个判断语句
                                while(ReadKeyOfOn_OffFlag);//等待按键的抬起，
                                g_uiTimeNumCont1ms = 200;
                                while(g_uiTimeNumCont1ms);//等待一些时间
                                ucErroType = ErroNoErro;//清按键标志
                            }
                        }
                        else if(TRUE == ucInSlopButDK_DisOff)//再次让上控制显示
                        {
                            ucErroType = ErroNoErro;//不让关机
                            C_DisEnResetStm8;//不让上控复位
                            ucInSlopButDK_DisOff = FALSE;//标志位清空
                            vSendSingleOrder(FirstOrderPowerOn);//告诉上控上电
                            g_uiTimeNumCont1ms = 300;
                            while(g_uiTimeNumCont1ms);//暂停一个时间
                            vSendSysPower(strSysInfo.uiSysTemPower + 15);//再次开启的时候 让电量再指示下
                            vSendSysPower(strSysInfo.uiSysTemPower - 15);//这里让上控制有一个有效的指示
                            while(ReadKeyOfOn_OffFlag);
                            g_uiTimeNumCont1ms = 200;//等待200ms让按键有一个稳定时间
                            while(g_uiTimeNumCont1ms);
                            g_uiTimeNumCont1ms = 1;
                            ucErroType = ErroNoErro;//清空关机标志
                        }
                        if(ucInSlopButDK_DisOff & 0x80)ucInSlopButDK_DisOff &= 0x0F;//让占位信息清空掉
                    }

                }
                else
                {
                    return;
                }
            }
#endif
            //----------------------------------------------------刹车器处理----------------------------------------------------------/
            if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-刹车器程序或0-驻坡程序由参数表第6项决定20190110
            {
                if((MotoASlopModeAdjust.uiSampleAdSetPWMFinishFlag) && (MotoBSlopModeAdjust.uiSampleAdSetPWMFinishFlag))
                {
                    strSysInfo.uiSysBreakFlag = 1;
                }
            }
            else
            {
                strSysInfo.uiSysBreakFlag = 0;
                if(((ReDefTIM1CCR1 >= PowerOffSlopPWM) && (ReDefTIM1CCR1 <= MaxSlopPWM)) || ((ReDefTIM1CCR3 >= PowerOffSlopPWM) && (ReDefTIM1CCR3 <= MaxSlopPWM)) || \
                        ((ReDefTIM4CCR1 >= PowerOffSlopPWM) && (ReDefTIM4CCR1 <= MaxSlopPWM)) || ((ReDefTIM4CCR3 >= PowerOffSlopPWM) && (ReDefTIM4CCR3 <= MaxSlopPWM)))
                {
                    uiAutoPowerOffTimeCont = 0;
                }
            }
            if(strSysInfo.uiSysBreakFlag)//||(TRUE == ucInSlopButDK_DisOff))
            {
                CtlBrakeBikeDisEn;
                ReDefTIM1CCR1 = ReDefTIM1CCR2 = 0;
                ReDefTIM1CCR3 = ReDefTIM1CCR4 = 0;
                ReDefTIM4CCR1 = ReDefTIM4CCR2 = 0;
                ReDefTIM4CCR3 = ReDefTIM4CCR4 = 0;
            }
            else
            {
                CtlBrakeBikeEn;
                //-----------------------------------------------------驻坡处理----------------------------------------------------------/

                MotoASlopModeAdjust.uiSampleAdCnt++;
                MotoBSlopModeAdjust.uiSampleAdCnt++;
                MotoASlopModeAdjust.uiSetPwmTimerCCR1_Flag = 0;
                MotoBSlopModeAdjust.uiSetPwmTimerCCR1_Flag = 0;
                MotoASlopModeAdjust.uiSetPwmTimerCCR3_Flag = 0;
                MotoBSlopModeAdjust.uiSetPwmTimerCCR3_Flag = 0;//给定PWM计数器
                MotoASlopModeAdjust.fSetPwmStep = 0;//每一步减少PWM的值的定义
                MotoBSlopModeAdjust.fSetPwmStep = 0;//每一步减少PWM的值的定义

                if(ReDefTIM1CCR1 > MinSlopPWM)//对每个电机的PWM 的赋值定义
                {

                    if(ReDefTIM1CCR1 > MaxSlopPWM)//最大占空比限幅
                    {
                        MotoASlopModeAdjust.fSetCCR1CCR2PwmValue = MotoASlopModeAdjust.fReadOldCCR1CCR2PwmValue;
                    }
                    else
                    {
                        MotoASlopModeAdjust.fReadOldCCR1CCR2PwmValue = MotoASlopModeAdjust.fSetCCR1CCR2PwmValue;
                        MotoASlopModeAdjust.fSetCCR1CCR2PwmValue = ReDefTIM1CCR1;
                    }

                }
                if(ReDefTIM1CCR3 > MinSlopPWM)
                {
                    if(ReDefTIM1CCR3 > MaxSlopPWM)//最大占空比限幅
                    {
                        MotoASlopModeAdjust.fSetCCR3CCR4PwmValue = MotoASlopModeAdjust.fReadOldCCR3CCR4PwmValue;
                    }
                    else
                    {
                        MotoASlopModeAdjust.fReadOldCCR3CCR4PwmValue = MotoASlopModeAdjust.fSetCCR3CCR4PwmValue;
                        MotoASlopModeAdjust.fSetCCR3CCR4PwmValue = ReDefTIM1CCR3;
                    }
                }
                if(ReDefTIM4CCR1 > MinSlopPWM)
                {

                    if(ReDefTIM4CCR1 > MaxSlopPWM)//最大占空比限幅
                    {
                        MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue = MotoBSlopModeAdjust.fReadOldCCR1CCR2PwmValue;
                    }
                    else
                    {
                        MotoBSlopModeAdjust.fReadOldCCR1CCR2PwmValue = MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue;
                        MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue = ReDefTIM4CCR1;
                    }
                }
                if(ReDefTIM4CCR3 > MinSlopPWM)
                {

                    if(ReDefTIM4CCR3 > MaxSlopPWM)//最大占空比限幅
                    {
                        MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue = MotoBSlopModeAdjust.fReadOldCCR3CCR4PwmValue;
                    }
                    else
                    {
                        MotoBSlopModeAdjust.fReadOldCCR3CCR4PwmValue = MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue;
                        MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue = ReDefTIM4CCR3;
                    }
                }
                //-------------------------------------------------------------------------------//
                if(MotoASlopModeAdjust.uiSampleAdCnt >= MotoASlopModeAdjust.uiReadSampleAdTime) //检测周期
                {

                    //if(MotoASlopModeAdjust.uiSampleAdMode == SampleAdCheckMode)//测试模式
                    if((MotoASlopModeAdjust.uiSampleAdMode == SampleAdCheckMode) &&
                            ((MotoASlopModeAdjust.fSetCCR1CCR2PwmValue > UserSetSlopMarkPWM) ||
                             (MotoASlopModeAdjust.fSetCCR3CCR4PwmValue > UserSetSlopMarkPWM)))
                    {
                        if(MotoASlopModeAdjust.uiSampleAdJudgeModeFinishFlag == 0)
                        {
                            MotoASlopModeAdjust.iArryCurrentSample[0] = strMoto1Info.uiMotorCurrent;//电流的采样数组
                            MotoASlopModeAdjust.uiSampleAdJudgeModeFinishFlag = 1;
                        }
                        MotoASlopModeAdjust.uiSampleAdCnt = 0;
                        MotoASlopModeAdjust.uiSampleAdSetIntoBreakCnt++;
                        if(MotoASlopModeAdjust.uiSampleAdSetIntoBreakCnt >= IntoBreakTimer)
                        {
                            MotoASlopModeAdjust.uiSampleAdSetIntoBreakCnt = IntoBreakTimer;
                            MotoASlopModeAdjust.uiSampleAdSetPWMFinishFlag = 1;
                        }

                    }
                    else
                    {
                        MotoASlopModeAdjust.uJudgeModeIntoCheckModeCnt = 0;
                        if(MotoASlopModeAdjust.fSetCCR1CCR2PwmValue > 0)//||((uiSetMotoB_PwmTim3CCR4==0)&&(uiSampleAdSetCCR3PWMFinishFlag)))
                        {
                            SetTim1CC1andCCR2Reg(SetPwmPinHighReg, 0); //让电不使能一下
                            SetTim1CC3andCCR4Reg(0, 0); //让电不使能一下
                            if(MotoASlopModeAdjust.uiSampleAdCnt > MotoASlopModeAdjust.uiReadSampleAdTime)//1MS读一次值
                            {

                                for(uiTimeCont = 0; uiTimeCont < OneChannelConvertTimes; uiTimeCont++)
                                {
                                    MotoASlopModeAdjust.fVoltage[MotoASlopModeAdjust.uiReadVoltageTime] += vDmaContAdcVaule[uiTimeCont][Motor1RightHBridgeVolHaveFilterCh];
                                }
                                MotoASlopModeAdjust.fVoltage[MotoASlopModeAdjust.uiReadVoltageTime] /= OneChannelConvertTimes;
                                MotoASlopModeAdjust.uiReadVoltageTime ++;
                            }
                            else
                            {
                                MotoASlopModeAdjust.uiReadVoltageTime = 0;
                                for(uiTimeCont = 0; uiTimeCont < BigWheelJudgeMode_SetPwmHighTime; uiTimeCont++)
                                {
                                    MotoASlopModeAdjust.fVoltage[uiTimeCont] = 0;
                                    //												    MotoASlopModeAdjust.iArryReadCurrentSample[uiTimeCont]=0;
                                }

                            }
                            if(MotoASlopModeAdjust.fSetCCR1CCR2PwmValue <= UserSetSlopMarkPWM)
                            {
                                MotoASlopModeAdjust.uiJudgeMode_SetPwmHighTime = SmallWheelJudgeMode_SetPwmHighTime;
                            }
                            else
                            {
                                MotoASlopModeAdjust.uiJudgeMode_SetPwmHighTime = BigWheelJudgeMode_SetPwmHighTime;
                            }
                        }
                        else if(MotoASlopModeAdjust.fSetCCR3CCR4PwmValue > 0)
                        {
                            SetTim1CC1andCCR2Reg(0, 0); //让电不使能一下
                            SetTim1CC3andCCR4Reg(SetPwmPinHighReg, 0); //让电不使能一下
                            if(MotoASlopModeAdjust.uiSampleAdCnt > MotoASlopModeAdjust.uiReadSampleAdTime)//1MS读一次值
                            {
                                for(uiTimeCont = 0; uiTimeCont < OneChannelConvertTimes; uiTimeCont++)
                                {
                                    MotoASlopModeAdjust.fVoltage[MotoASlopModeAdjust.uiReadVoltageTime] += vDmaContAdcVaule[uiTimeCont][Motor1LeftHBridgeVolHaveFilterCh];
                                }
                                MotoASlopModeAdjust.fVoltage[MotoASlopModeAdjust.uiReadVoltageTime] /= OneChannelConvertTimes;
                                MotoASlopModeAdjust.uiReadVoltageTime ++;
                            }
                            else
                            {
                                MotoASlopModeAdjust.uiReadVoltageTime = 0;
                                for(uiTimeCont = 0; uiTimeCont < BigWheelJudgeMode_SetPwmHighTime; uiTimeCont++)
                                {
                                    MotoASlopModeAdjust.fVoltage[uiTimeCont] = 0;
                                }

                            }
                            if(MotoASlopModeAdjust.fSetCCR1CCR2PwmValue <= UserSetSlopMarkPWM)
                            {
                                MotoASlopModeAdjust.uiJudgeMode_SetPwmHighTime = SmallWheelJudgeMode_SetPwmHighTime;
                            }
                            else
                            {
                                MotoASlopModeAdjust.uiJudgeMode_SetPwmHighTime = BigWheelJudgeMode_SetPwmHighTime;
                            }

                        }
                        if(MotoASlopModeAdjust.uiSampleAdCnt >= (MotoASlopModeAdjust.uiReadSampleAdTime + MotoASlopModeAdjust.uiSetHighTime)) //检测300+1MS 或者调整过程100+3MS
                        {
                            MotoASlopModeAdjust.uiSampleAdCnt = 0;
                            MotoASlopModeAdjust.fVoltageSample = MotoASlopModeAdjust.fVoltage[MotoASlopModeAdjust.uiSetHighTime - 1];		//最新的一个值
                            if(MotoASlopModeAdjust.fSetCCR1CCR2PwmValue > 0)//当前调节CCR1CCR2
                            {
                                ReDefTIM1CCR1 = ReDefTIM1CCR2 = 0;
                                MotoASlopModeAdjust.fSetCCR3CCR4PwmValue = 0;
                                if(MotoASlopModeAdjust.uiSampleAdMode == SampleAdJudgeMode) //调节模式
                                {
                                    MotoASlopModeAdjust.fLastVoltageSample	 = 	MotoASlopModeAdjust.fVoltageSample;//MotoASlopModeAdjust.fVoltage[2];

                                    if((MotoASlopModeAdjust.fVoltageSample >= IfMotoASampleAdAtStep1Low) && (MotoASlopModeAdjust.fVoltageSample <= IfMotoASampleAdAtStep1High)) //调整完成
                                    {
                                        ucJudgeMotoBreakResult &= (~(MotoABreakFlagOne | MotoABreakFlagTwo));
                                        MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt++;
                                        if(MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt >= SampleAdJudgeMode_FinishTime)
                                        {
                                            MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                            MotoASlopModeAdjust.uiSampleAdMode =	SampleAdCheckMode;
                                            MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdCheckMode_MotoATime;
                                            MotoASlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                            MotoASlopModeAdjust.uCheckMode_SetTime = SetPWMFinishCheckMode_SetTime;
                                            MotoASlopModeAdjust.uCheckMode_ReadFlag = 0;
                                        }
                                    }
                                    else//调整过程
                                    {
																			  ucJudgeMotoBreakResult &= ~MotoABreakFlagOne;//20210608
                                        MotoASlopModeAdjust.uiSetHighTime = MotoASlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                        if(MotoASlopModeAdjust.fVoltageSample > IfMotoASampleAdAtStep1High)//电压高时减
                                        {

                                            MotoASlopModeAdjust.uiSampleAdJudgeModeFinishFlag = 0;
                                            MotoASlopModeAdjust.uiSampleAdJudgeModeCnt = 0;
                                            if(MotoASlopModeAdjust.fVoltageSample > IfMotoASampleAdAtStep2High)
                                            {
                                                if((MotoASlopModeAdjust.fSetCCR1CCR2PwmValue >= TestSlopPWM) && (MotoASlopModeAdjust.fSetCCR1CCR2PwmValue <= LowSlopPWM))
                                                {
                                                    if(MotoASlopModeAdjust.fSetCCR1CCR2PwmValue <= ResetSlopPWM)
                                                    {
                                                        if(MotoASlopModeAdjust.fVoltageSample > IfMotoSampleAdAtErroVoltage)//电机断线
                                                        {
                                                            ucJudgeMotoBreakResult |= MotoABreakFlagOne;
                                                        }
                                                        else
                                                        {
                                                            ucJudgeMotoBreakResult &= (~(MotoABreakFlagOne | MotoABreakFlagTwo));
                                                        }
                                                    }
                                                    MotoASlopModeAdjust.fSetCCR1CCR2PwmValue = 0;
                                                    MotoASlopModeAdjust.fSetCCR3CCR4PwmValue = ResetSlopPWM;
                                                }

                                            }
#ifdef SetPwmStep1
                                            MotoASlopModeAdjust.fSetPwmStep = (-JudgeModeSetDecPareFast);
#else
                                            if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-刹车器程序或0-驻坡程序由参数表第6项决定20190110
                                            {
                                                MotoASlopModeAdjust.fSetPwmStep = (-MotoASlopModeAdjust.fSetCCR1CCR2PwmValue * JudgeModebBreakEnDecPareFast);
                                            }
                                            else
                                                MotoASlopModeAdjust.fSetPwmStep = (-MotoASlopModeAdjust.fSetCCR1CCR2PwmValue * JudgeModeSetMotoDecPareFast);
#endif
                                            MotoASlopModeAdjust.uiSetPwmTimerCCR1_Flag = TRUE;
                                            MotoASlopModeAdjust.uiSetHighTime = MotoASlopModeAdjust.uiJudgeMode_SetPwmHighTime;

                                        }
                                        else if((MotoASlopModeAdjust.uCheckMode_ReadFlag == 0) && (MotoASlopModeAdjust.fVoltageSample < IfMotoASampleAdAtStep1Low)) //电压低时加
                                        {
                                            ucJudgeMotoBreakResult &= (~(MotoABreakFlagOne | MotoABreakFlagTwo));
                                            if(MotoASlopModeAdjust.fSetCCR1CCR2PwmValue < UserSetSmallWheelSlopPWM)//LowSlopPWM)
                                            {
                                                MotoASlopModeAdjust.uiSetPwmTimerCCR1_Flag = TRUE;
                                                MotoASlopModeAdjust.fSetPwmStep = JudgeModeSetPareFast;
                                            }
                                            else
                                            {
                                                if(MotoASlopModeAdjust.fVoltageSample > 0)
                                                {
                                                    MotoASlopModeAdjust.fSetPwmStep = JudgeModeSetPare;
                                                    MotoASlopModeAdjust.uiSampleAdJudgeModeCnt = 0;
                                                    MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                                    MotoASlopModeAdjust.uiSampleAdMode =	SampleAdCheckMode;
                                                    MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdCheckMode_MotoATime;
                                                    MotoASlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                                    MotoASlopModeAdjust.uCheckMode_SetTime = SetPWMFinishCheckMode_SetTime;
                                                    MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                                }
                                                else
                                                {
                                                    if(MotoASlopModeAdjust.uiPeopleStandUpFlag)
                                                    {
                                                        MotoASlopModeAdjust.uiSampleAdJudgeModeCnt ++;
                                                    }
                                                    if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)
                                                    {
                                                        MotoASlopModeAdjust.fSetPwmStep = UserSetSlopMarkPWM;
                                                    }
                                                    else
                                                        //MotoASlopModeAdjust.fSetPwmStep = JudgeModeSetPare;
                                                        MotoASlopModeAdjust.fSetPwmStep = (MotoASlopModeAdjust.fSetCCR1CCR2PwmValue * JudgeModeSetMotoPlusPareFast);
                                                }

                                                MotoASlopModeAdjust.uiSetPwmTimerCCR1_Flag = TRUE;
                                                MotoASlopModeAdjust.uiSampleAdJudgeModeFinishFlag = 0;

                                                if(MotoASlopModeAdjust.uiSampleAdJudgeModeCnt > AdJudgeModeTimes)
                                                {
                                                    MotoASlopModeAdjust.uiPeopleStandUpFlag = 0;
                                                    MotoASlopModeAdjust.uiSampleAdJudgeModeCnt = 0;
                                                    MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                                    MotoASlopModeAdjust.uiSampleAdMode =	SampleAdCheckMode;
                                                    MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdCheckMode_MotoATime;
                                                    MotoASlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                                    MotoASlopModeAdjust.uCheckMode_SetTime = SetPWMFinishCheckMode_SetTime;
                                                    MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                                    MotoASlopModeAdjust.fSetPwmStep  = 0;
                                                }
                                            }

                                        }
                                        else
                                        {

                                            MotoASlopModeAdjust.uCheckMode_ReadFlag = 0;
                                            MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                            MotoASlopModeAdjust.uiSampleAdMode =	SampleAdCheckMode;
                                            MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdCheckMode_MotoATime;
                                            MotoASlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                            MotoASlopModeAdjust.uCheckMode_SetTime = SetPWMFinishCheckMode_SetTime;
                                            MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                        }

                                    }
                                    if((MotoASlopModeAdjust.fSetCCR1CCR2PwmValue == MaxSlopPWM) && (MotoASlopModeAdjust.fVoltageSample < IfMotoASampleAdAtStep1Low))
                                    {
                                        MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                        MotoASlopModeAdjust.uiSampleAdMode =	SampleAdCheckMode;
                                        MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdCheckMode_MotoATime;
                                        MotoASlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                        MotoASlopModeAdjust.uiSampleAdJudgeMode_SetCnt = 0;
                                        MotoASlopModeAdjust.uCheckMode_SetTime = SetPWMFinishCheckMode_SetTime;
                                    }
                                }
                                else  if(MotoASlopModeAdjust.uiSampleAdMode == SampleAdCheckMode)//测试模式
                                {

                                    MotoASlopModeAdjust.uiSetPwmTimerCCR1_Flag = 0;
                                    MotoASlopModeAdjust.fSetPwmStep  = 0;
                                    if(MotoASlopModeAdjust.uiSampleAdJudgeMode_SetCnt >= (MotoASlopModeAdjust.uCheckMode_SetTime + 1))
                                    {
                                        MotoASlopModeAdjust.uiSampleAdJudgeMode_SetCnt = 0;
                                        if((MotoASlopModeAdjust.fVoltageSample < IfMotoASampleAdAtStep2Low) || (MotoASlopModeAdjust.fVoltageSample > IfMotoASampleAdAtStep1High))
                                        {
                                            MotoASlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                            if(MotoASlopModeAdjust.fVoltageSample > IfMotoASampleAdAtStep1High)
                                            {
                                                MotoASlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                                MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoATime;

                                                MotoASlopModeAdjust.uiSetHighTime = MotoASlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                                MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = 0;
                                            }
                                            else if(MotoASlopModeAdjust.fVoltageSample < IfMotoASampleAdAtStep2Low)
                                            {
                                                if(MotoASlopModeAdjust.fSetCCR1CCR2PwmValue != MaxSlopPWM)
                                                {
                                                    if(MotoASlopModeAdjust.fSetCCR1CCR2PwmValue < PowerOffSlopPWM)
                                                        //																														 if((MotoASlopModeAdjust.fSetCCR1CCR2PwmValue <LowSlopPWM)||(MotoASlopModeAdjust.fVoltageSample <=IfMotoSampleAdAtStep4Low))
                                                    {

                                                        MotoASlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                                        MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoATime;
                                                        MotoASlopModeAdjust.uiSetHighTime = MotoASlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                                        MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = 0;

                                                    }
                                                }
                                                else //if(MotoASlopModeAdjust.fVoltageSample < IfMotoASampleAdAtStep2Low)
                                                {
                                                    //																											 if((MotoBSlopModeAdjust.uiSampleAdMode == SampleAdJudgeMode)||(MotoASlopModeAdjust.fVoltageSample >=IfMotoSampleAdAtStep4Low))
                                                    if(MotoASlopModeAdjust.fVoltageSample >= IfMotoSampleAdAtStep4Low)
                                                    {
                                                        MotoASlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                                        MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoATime;
                                                        MotoASlopModeAdjust.uiSetHighTime = MotoASlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                                        MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = 0;
                                                    }

                                                }
                                            }
                                            else
                                            {
                                                MotoASlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                            }
                                        }
                                        else
                                        {
                                            MotoASlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                        }
                                    }
                                    else if(MotoASlopModeAdjust.uiSampleAdJudgeMode_SetCnt >= MotoASlopModeAdjust.uCheckMode_SetTime)
                                    {
                                        if(MotoASlopModeAdjust.fVoltageSample > IfMotoASampleAdAtStep1High)
                                        {
                                            MotoASlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                            MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoATime;

                                            MotoASlopModeAdjust.uiSetHighTime = MotoASlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                            MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = 0;
                                        }
                                        else
                                            MotoASlopModeAdjust.uiSetHighTime = SampleAdJudgeFinishCheckMode_SetPwmHighTime;
                                    }
                                    MotoASlopModeAdjust.uiSampleAdJudgeMode_SetCnt++;

                                    MotoASlopModeAdjust.uiSampleAdSetIntoBreakCnt++;
                                    if(MotoASlopModeAdjust.uiSampleAdSetIntoBreakCnt >= IntoBreakTimer)
                                    {
                                        MotoASlopModeAdjust.uiSampleAdSetIntoBreakCnt = IntoBreakTimer;
                                        MotoASlopModeAdjust.uiSampleAdSetPWMFinishFlag = 1;
                                    }
                                }
                                else if(MotoASlopModeAdjust.uiSampleAdMode == SampleAdZeroTest)//CCR1==0,CCR2==0,CCR3==0,CCR4==0
                                {
                                    if (MotoASlopModeAdjust.fVoltageSample > IfMotoASampleAdAtStep2High)
                                    {
                                        MotoASlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                        MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoATime;
                                        MotoASlopModeAdjust.uiSetHighTime = MotoASlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                        MotoASlopModeAdjust.fSetCCR1CCR2PwmValue = 0;
                                        MotoASlopModeAdjust.fSetCCR3CCR4PwmValue = ResetSlopPWM;
                                        MotoASlopModeAdjust.uiSampleAdSetIntoBreakCnt = 0;
                                        MotoASlopModeAdjust.uiSampleAdSetPWMFinishFlag = 0;
                                    }
                                    else
                                    {
                                        MotoASlopModeAdjust.uiSampleAdSetIntoBreakCnt++;
                                        if(MotoASlopModeAdjust.uiSampleAdSetIntoBreakCnt >= IntoBreakTimer)
                                        {
                                            MotoASlopModeAdjust.uiSampleAdSetIntoBreakCnt = IntoBreakTimer;
                                            MotoASlopModeAdjust.uiSampleAdSetPWMFinishFlag = 1;
                                        }
                                        MotoASlopModeAdjust.fSetCCR1CCR2PwmValue = 0;
                                        ReDefTIM1CCR1 = ReDefTIM1CCR2 = 0;
                                        MotoASlopModeAdjust.fSetCCR3CCR4PwmValue = 0;
                                        ReDefTIM1CCR3 = ReDefTIM1CCR4 = 0;
                                        switch(MotoASlopModeAdjust.uiZeroTestPhase)
                                        {
                                        case 0:
                                            MotoASlopModeAdjust.uiZeroTestPhase = 1;
                                            break;
                                        case 1:
                                            MotoASlopModeAdjust.uiZeroTestPhase = 0;
                                            break;
                                        default:
                                            break;

                                        }
                                    }
                                }
                                if(MotoASlopModeAdjust.uiSetPwmTimerCCR1_Flag)
                                {
                                    MotoASlopModeAdjust.uiSetPwmTimerCCR1_Flag = 0;
                                    MotoASlopModeAdjust.fSetCCR1CCR2PwmValue = MotoASlopModeAdjust.fSetCCR1CCR2PwmValue + MotoASlopModeAdjust.fSetPwmStep;

                                }
                                if(MotoASlopModeAdjust.fSetCCR1CCR2PwmValue >= MaxSlopPWM)//最大占空比限幅
                                {
                                    MotoASlopModeAdjust.fSetCCR1CCR2PwmValue = MaxSlopPWM;
                                }
                                if(MotoASlopModeAdjust.fSetCCR1CCR2PwmValue < ResetSlopPWM)//最小占空比限幅
                                {
                                    if(MotoASlopModeAdjust.fSetCCR1CCR2PwmValue == MinSlopPWM)
                                    {
                                        MotoASlopModeAdjust.uiSampleAdMode = SampleAdZeroTest;
                                        MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdZeroMode_MotoATime;
                                        MotoASlopModeAdjust.uiSetHighTime = SampleAdZeroMode_SetPwmHighTime;
                                    }
                                    MotoASlopModeAdjust.fSetCCR1CCR2PwmValue = 0;
                                }
                            }
                            else if(MotoASlopModeAdjust.fSetCCR3CCR4PwmValue > 0)//调节CCR3CCR4
                            {
                                ReDefTIM1CCR3 = ReDefTIM1CCR4 = 0;
                                MotoASlopModeAdjust.fSetCCR1CCR2PwmValue = 0;
                                MotoASlopModeAdjust.fLastVoltageSample	 = 	MotoASlopModeAdjust.fVoltageSample;

                                if(MotoASlopModeAdjust.uiSampleAdMode == SampleAdJudgeMode) //调节模式
                                {

                                    if((MotoASlopModeAdjust.fVoltageSample >= IfMotoASampleAdAtStep1Low) && (MotoASlopModeAdjust.fVoltageSample <= IfMotoASampleAdAtStep1High)) //调整完成
                                    {
                                        ucJudgeMotoBreakResult &= (~(MotoABreakFlagOne | MotoABreakFlagTwo));
                                        MotoASlopModeAdjust. uiSampleAdSetPWMFinishCnt++;
                                        if(MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt >= SampleAdJudgeMode_FinishTime)
                                        {
                                            MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                            MotoASlopModeAdjust.uiSampleAdMode =	SampleAdCheckMode;
                                            MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdCheckMode_MotoATime;
                                            MotoASlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                            MotoASlopModeAdjust.uCheckMode_SetTime = SetPWMFinishCheckMode_SetTime;
                                            MotoASlopModeAdjust.uCheckMode_ReadFlag = 0;

                                        }
                                    }
                                    else//调整过程
                                    {
																			  ucJudgeMotoBreakResult &= ~MotoABreakFlagTwo;//20210608
                                        MotoASlopModeAdjust.uiSetHighTime = MotoASlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                        if(MotoASlopModeAdjust.fVoltageSample > IfMotoASampleAdAtStep1High)//电压高时减
                                        {

                                            if(MotoASlopModeAdjust.fVoltageSample > IfMotoASampleAdAtStep2High)
                                            {
                                                if((MotoASlopModeAdjust.fSetCCR3CCR4PwmValue >= TestSlopPWM) && (MotoASlopModeAdjust.fSetCCR3CCR4PwmValue <= LowSlopPWM))
                                                {
                                                    if(MotoASlopModeAdjust.fSetCCR3CCR4PwmValue <= ResetSlopPWM)
                                                    {
                                                        if(MotoASlopModeAdjust.fVoltageSample > IfMotoSampleAdAtErroVoltage)//电机断线
                                                        {
                                                            ucJudgeMotoBreakResult |= MotoABreakFlagTwo;
                                                        }
                                                        else
                                                        {
                                                            ucJudgeMotoBreakResult &= (~(MotoABreakFlagOne | MotoABreakFlagTwo));
                                                        }
                                                    }
                                                    MotoASlopModeAdjust.fSetCCR1CCR2PwmValue = ResetSlopPWM;
                                                    MotoASlopModeAdjust.fSetCCR3CCR4PwmValue = 0;
                                                }
                                            }
#ifdef SetPwmStep1
                                            MotoASlopModeAdjust.fSetPwmStep = (-JudgeModeSetDecPareFast);
#else
                                            if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-刹车器程序或0-驻坡程序由参数表第6项决定20190110
                                            {
                                                MotoASlopModeAdjust.fSetPwmStep = (-MotoASlopModeAdjust.fSetCCR3CCR4PwmValue * JudgeModebBreakEnDecPareFast);
                                            }
                                            else
                                                MotoASlopModeAdjust.fSetPwmStep = (-MotoASlopModeAdjust.fSetCCR3CCR4PwmValue * JudgeModeSetMotoDecPareFast);
#endif
                                            MotoASlopModeAdjust.uiSetPwmTimerCCR3_Flag = TRUE;
                                            MotoASlopModeAdjust.uiSetHighTime = MotoASlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                        }
                                        else if((MotoASlopModeAdjust.uCheckMode_ReadFlag == 0) && (MotoASlopModeAdjust.fVoltageSample < IfMotoASampleAdAtStep1Low))	//电压低时加
                                        {
                                            ucJudgeMotoBreakResult &= (~(MotoABreakFlagOne | MotoABreakFlagTwo));
                                            if(MotoASlopModeAdjust.fSetCCR3CCR4PwmValue < UserSetSmallWheelSlopPWM)//LowSlopPWM)
                                            {
                                                MotoASlopModeAdjust.uiSetPwmTimerCCR3_Flag = TRUE;
                                                MotoASlopModeAdjust.fSetPwmStep = JudgeModeSetPareFast;
                                            }
                                            else //if(MotoASlopModeAdjust.fVoltageSample <= 0)//<= IfMotoSampleAdAtStep4Low)
                                            {
                                                if(MotoASlopModeAdjust.fVoltageSample > 0)
                                                {
                                                    MotoASlopModeAdjust.fSetPwmStep = JudgeModeSetPare;
                                                    MotoASlopModeAdjust.uiSampleAdJudgeModeCnt = 0;
                                                    MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                                    MotoASlopModeAdjust.uiSampleAdMode =	SampleAdCheckMode;
                                                    MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdCheckMode_MotoATime;
                                                    MotoASlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                                    MotoASlopModeAdjust.uCheckMode_SetTime = SetPWMFinishCheckMode_SetTime;
                                                    MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                                }
                                                else
                                                {
                                                    if(MotoASlopModeAdjust.uiPeopleStandUpFlag)
                                                    {
                                                        MotoASlopModeAdjust.uiSampleAdJudgeModeCnt ++;
                                                    }
                                                    if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)
                                                    {
                                                        MotoASlopModeAdjust.fSetPwmStep = UserSetSlopMarkPWM;
                                                    }
                                                    else
                                                        //MotoASlopModeAdjust.fSetPwmStep = JudgeModeSetPare;
                                                        MotoASlopModeAdjust.fSetPwmStep = (MotoASlopModeAdjust.fSetCCR3CCR4PwmValue * JudgeModeSetMotoPlusPareFast);
                                                }
                                                MotoASlopModeAdjust.uiSetPwmTimerCCR3_Flag = TRUE;
                                                MotoASlopModeAdjust.uiSampleAdJudgeModeFinishFlag = 0;

                                                if(MotoASlopModeAdjust.uiSampleAdJudgeModeCnt > AdJudgeModeTimes)
                                                {
                                                    MotoASlopModeAdjust.uiPeopleStandUpFlag = 0;
                                                    MotoASlopModeAdjust.uiSampleAdJudgeModeCnt = 0;
                                                    MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                                    MotoASlopModeAdjust.uiSampleAdMode =	SampleAdCheckMode;
                                                    MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdCheckMode_MotoATime;
                                                    MotoASlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                                    MotoASlopModeAdjust.uCheckMode_SetTime = SetPWMFinishCheckMode_SetTime;
                                                    MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                                    MotoASlopModeAdjust.fSetPwmStep  = 0;
                                                }
                                            }

                                        }
                                        else
                                        {
                                            MotoASlopModeAdjust.uCheckMode_ReadFlag = 0;
                                            MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                            MotoASlopModeAdjust.uiSampleAdMode =	SampleAdCheckMode;
                                            MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdCheckMode_MotoATime;
                                            MotoASlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                            MotoASlopModeAdjust.uCheckMode_SetTime = SetPWMFinishCheckMode_SetTime;
                                            MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                        }

                                    }
                                    if((MotoASlopModeAdjust.fSetCCR3CCR4PwmValue == MaxSlopPWM) && (MotoASlopModeAdjust.fVoltageSample < IfMotoASampleAdAtStep1Low))
                                    {
                                        MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                        MotoASlopModeAdjust.uiSampleAdMode =	SampleAdCheckMode;
                                        MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdCheckMode_MotoATime;
                                        MotoASlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;

                                        MotoASlopModeAdjust.uiSampleAdJudgeMode_SetCnt = 0;
                                        MotoASlopModeAdjust.uCheckMode_SetTime = SetPWMFinishCheckMode_SetTime;

                                    }
                                }
                                else  if(MotoASlopModeAdjust.uiSampleAdMode == SampleAdCheckMode)//测试模式
                                {
                                    MotoASlopModeAdjust.uiSetPwmTimerCCR3_Flag = 0;
                                    MotoASlopModeAdjust.fSetPwmStep  = 0;
                                    if(MotoASlopModeAdjust.uiSampleAdJudgeMode_SetCnt >= (MotoASlopModeAdjust.uCheckMode_SetTime + 1))
                                    {
                                        MotoASlopModeAdjust.uiSampleAdJudgeMode_SetCnt = 0;
                                        MotoASlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                        if((MotoASlopModeAdjust.fVoltageSample < IfMotoASampleAdAtStep2Low) || (MotoASlopModeAdjust.fVoltageSample > IfMotoASampleAdAtStep1High))
                                        {
                                            if(MotoASlopModeAdjust.fVoltageSample > IfMotoASampleAdAtStep1High)
                                            {
                                                MotoASlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                                MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoATime;
                                                MotoASlopModeAdjust.uiSetHighTime = MotoASlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                                MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = 0;
                                            }
                                            else if(MotoASlopModeAdjust.fVoltageSample < IfMotoASampleAdAtStep2Low)
                                            {
                                                if(MotoASlopModeAdjust.fSetCCR3CCR4PwmValue != MaxSlopPWM)
                                                {
                                                    if(MotoASlopModeAdjust.fSetCCR3CCR4PwmValue < PowerOffSlopPWM)
                                                        //																													 if((MotoASlopModeAdjust.fSetCCR3CCR4PwmValue <LowSlopPWM)||(MotoASlopModeAdjust.fVoltageSample <=IfMotoSampleAdAtStep4Low))
                                                    {
                                                        MotoASlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                                        MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoATime;
                                                        MotoASlopModeAdjust.uiSetHighTime = MotoASlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                                        MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = 0;

                                                    }
                                                }
                                                else
                                                {
                                                    //																											 if((MotoBSlopModeAdjust.uiSampleAdMode == SampleAdJudgeMode)||(MotoASlopModeAdjust.fVoltageSample >=IfMotoSampleAdAtStep4Low))
                                                    if(MotoASlopModeAdjust.fVoltageSample >= IfMotoSampleAdAtStep4Low)
                                                    {
                                                        MotoASlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                                        MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoATime;
                                                        MotoASlopModeAdjust.uiSetHighTime =  MotoASlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                                        MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = 0;
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                MotoASlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                            }
                                            MotoASlopModeAdjust.uiSampleAdSetPWMFinishFlag = 0;

                                        }
                                        else
                                        {

                                            MotoASlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                        }
                                    }
                                    else if(MotoASlopModeAdjust.uiSampleAdJudgeMode_SetCnt >= MotoASlopModeAdjust.uCheckMode_SetTime)
                                    {
                                        if(MotoASlopModeAdjust.fVoltageSample  > IfMotoASampleAdAtStep1High)
                                        {
                                            MotoASlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                            MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoATime;
                                            MotoASlopModeAdjust.uiSetHighTime = MotoASlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                            MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = 0;
                                        }
                                        else		MotoASlopModeAdjust.uiSetHighTime = SampleAdJudgeFinishCheckMode_SetPwmHighTime;
                                    }
                                    MotoASlopModeAdjust.uiSampleAdJudgeMode_SetCnt++;

                                    MotoASlopModeAdjust.uiSampleAdSetIntoBreakCnt++;
                                    if(MotoASlopModeAdjust.uiSampleAdSetIntoBreakCnt >= IntoBreakTimer)
                                    {
                                        MotoASlopModeAdjust.uiSampleAdSetIntoBreakCnt = IntoBreakTimer;
                                        MotoASlopModeAdjust.uiSampleAdSetPWMFinishFlag = 1;
                                    }

                                    //																	if(MotoBSlopModeAdjust.uiSampleAdMode == SampleAdJudgeMode)
                                    //																	 {
                                    //																				MotoASlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                    //																				MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoATime;
                                    //																				MotoASlopModeAdjust.uiSetHighTime = uiJudgeMode_SetPwmHighTime;//SampleAdJudgeModeFinishMode_SetPwmHighTime;
                                    //																				MotoASlopModeAdjust.uiSampleAdSetPWMFinishCnt = 0;
                                    //																	 }
                                }
                                else if(MotoASlopModeAdjust.uiSampleAdMode == SampleAdZeroTest)//CCR1==0,CCR2==0,CCR3==0,CCR4==0
                                {

                                    if (MotoASlopModeAdjust.fVoltageSample > IfMotoASampleAdAtStep2High)
                                    {
                                        MotoASlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                        MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoATime;
                                        MotoASlopModeAdjust.uiSetHighTime = MotoASlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                        MotoASlopModeAdjust.fSetCCR1CCR2PwmValue = ResetSlopPWM;
                                        MotoASlopModeAdjust.fSetCCR3CCR4PwmValue = 0;
                                        MotoASlopModeAdjust.uiSampleAdSetIntoBreakCnt = 0;
                                        MotoASlopModeAdjust.uiSampleAdSetPWMFinishFlag = 0;
                                    }
                                    else
                                    {
                                        MotoASlopModeAdjust.uiSampleAdSetIntoBreakCnt++;
                                        if(MotoASlopModeAdjust.uiSampleAdSetIntoBreakCnt >= IntoBreakTimer)
                                        {
                                            MotoASlopModeAdjust.uiSampleAdSetIntoBreakCnt = IntoBreakTimer;
                                            MotoASlopModeAdjust.uiSampleAdSetPWMFinishFlag = 1;
                                        }
                                        MotoASlopModeAdjust.fSetCCR1CCR2PwmValue = 0;
                                        ReDefTIM1CCR1 = ReDefTIM1CCR2 = 0;
                                        MotoASlopModeAdjust.fSetCCR3CCR4PwmValue = 0;
                                        ReDefTIM1CCR3 = ReDefTIM1CCR4 = 0;
                                        switch(	MotoASlopModeAdjust.uiZeroTestPhase)
                                        {
                                        case 0:
                                            MotoASlopModeAdjust.uiZeroTestPhase = 1;
                                            break;
                                        case 1:
                                            MotoASlopModeAdjust.uiZeroTestPhase = 0;
                                            break;
                                        default:
                                            break;

                                        }
                                    }
                                }
                                if(MotoASlopModeAdjust.uiSetPwmTimerCCR3_Flag)
                                {
                                    MotoASlopModeAdjust.uiSetPwmTimerCCR3_Flag = 0;
                                    MotoASlopModeAdjust.fSetCCR3CCR4PwmValue = MotoASlopModeAdjust.fSetCCR3CCR4PwmValue + MotoASlopModeAdjust.fSetPwmStep;

                                }
                                if(MotoASlopModeAdjust.fSetCCR3CCR4PwmValue >= MaxSlopPWM)//最大占空比限幅
                                {
                                    MotoASlopModeAdjust.fSetCCR3CCR4PwmValue = MaxSlopPWM;
                                }
                                if(MotoASlopModeAdjust.fSetCCR3CCR4PwmValue < ResetSlopPWM)//最小占空比限幅
                                {
                                    if(MotoASlopModeAdjust.fSetCCR3CCR4PwmValue == MinSlopPWM)
                                    {
                                        MotoASlopModeAdjust.uiSampleAdMode = SampleAdZeroTest;
                                        MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdZeroMode_MotoATime;
                                        MotoASlopModeAdjust.uiSetHighTime = SampleAdZeroMode_SetPwmHighTime;
                                    }
                                    else
                                        MotoASlopModeAdjust.fSetCCR3CCR4PwmValue = 0;
                                }
                            }
                            else//CCR1==0,CCR2==0,CCR3==0,CCR4==0
                            {

                                MotoASlopModeAdjust.uiSampleAdMode = SampleAdZeroTest;
                                MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdZeroMode_MotoATime;
                                MotoASlopModeAdjust.uiSetHighTime = SampleAdZeroMode_SetPwmHighTime;
                                switch(MotoASlopModeAdjust.uiZeroTestPhase)
                                {
                                case 0:
                                    MotoASlopModeAdjust.fSetCCR1CCR2PwmValue = TestSlopPWM;
                                    MotoASlopModeAdjust.fSetCCR3CCR4PwmValue = 0;
                                    MotoASlopModeAdjust.fReadOldCCR1CCR2PwmValue = TestSlopPWM;
                                    MotoASlopModeAdjust.fReadOldCCR3CCR4PwmValue = 0;
                                    break;
                                case 1:
                                    MotoASlopModeAdjust.fSetCCR1CCR2PwmValue = 0;
                                    MotoASlopModeAdjust.fSetCCR3CCR4PwmValue = TestSlopPWM;
                                    MotoASlopModeAdjust.fReadOldCCR1CCR2PwmValue = 0;
                                    MotoASlopModeAdjust.fReadOldCCR3CCR4PwmValue = TestSlopPWM;
                                    break;
                                default:
                                    break;

                                }

                            }
                            ReDefTIM1CCR1 = ReDefTIM1CCR2 = MotoASlopModeAdjust.fSetCCR1CCR2PwmValue;
                            ReDefTIM1CCR3 = ReDefTIM1CCR4 = MotoASlopModeAdjust.fSetCCR3CCR4PwmValue;
                        }
                    }
                }
                else
                {
                    if(MotoASlopModeAdjust.uiSampleAdMode == SampleAdCheckMode)
                    {
                        if((MotoASlopModeAdjust.uiSampleAdCnt >= 300) && (MotoASlopModeAdjust.uiSampleAdJudgeModeFinishFlag))
                        {
                            MotoASlopModeAdjust.uiSampleAdCnt = 0;
                            MotoASlopModeAdjust.uCheckMode_ReadTime++;

                            MotoASlopModeAdjust.iArryCurrentSample[1] = strMoto1Info.uiMotorCurrent;//电流的采样数组MotoASlopModeAdjust.iArryCurrentSample[0];

                            MotoASlopModeAdjust.iCalCurrentSample = (MotoASlopModeAdjust.iArryCurrentSample[0] - MotoASlopModeAdjust.iArryCurrentSample[1]);
                            if((MotoASlopModeAdjust.iCalCurrentSample < (0 - PlusIntoJudgeModeCurrent))
                                    || (MotoASlopModeAdjust.iCalCurrentSample  > IntoJudgeModeCurrent)
                                    || (MotoASlopModeAdjust.uCheckMode_ReadTime >= 10))
                            {

                                MotoASlopModeAdjust.uiSampleAdJudgeModeFinishFlag = 0;
                                if(MotoASlopModeAdjust.iCalCurrentSample  > IntoJudgeModeCurrent)
                                {
                                    MotoASlopModeAdjust.uiPeopleStandUpFlag = 1;
                                    MotoBSlopModeAdjust.uiPeopleStandUpFlag = 1;
                                }
                                MotoASlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoATime;
                                if(MotoASlopModeAdjust.uCheckMode_ReadTime >= 10)
                                {
                                    MotoASlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                    MotoASlopModeAdjust.uCheckMode_ReadFlag = 1;
                                }
                                else
                                    MotoASlopModeAdjust.uiSetHighTime = BigWheelJudgeMode_SetPwmHighTime;
                                MotoASlopModeAdjust.uiSampleAdCnt	=	(SampleAdJudgeMode_MotoATime - 2);	//0;//
                                MotoASlopModeAdjust.uiSampleAdJudgeModeCnt = 0;

                                //                                MotoBSlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                //                                MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoBTime;
                                //                                if(MotoASlopModeAdjust.uCheckMode_ReadTime >= 10)
                                //                                {
                                //                                    MotoBSlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                //                                    MotoBSlopModeAdjust.uCheckMode_ReadFlag = 1;
                                //                                }
                                //                                else
                                //                                    MotoBSlopModeAdjust.uiSetHighTime = BigWheelJudgeMode_SetPwmHighTime;
                                //                                MotoBSlopModeAdjust.uiSampleAdCnt	=	(SampleAdJudgeMode_MotoBTime - 2);
                                //                                MotoBSlopModeAdjust.uiSampleAdJudgeModeCnt = 0;

                                MotoASlopModeAdjust.uCheckMode_ReadTime = 0;
                            }

                        }
                    }
                }
                //----------------------------------------MOTOB---------------------------------------//
                if(MotoBSlopModeAdjust.uiSampleAdCnt >= MotoBSlopModeAdjust.uiReadSampleAdTime) //检测周期
                {

                    //if(MotoBSlopModeAdjust.uiSampleAdMode == SampleAdCheckMode)//测试模式
                    if((MotoBSlopModeAdjust.uiSampleAdMode == SampleAdCheckMode) &&
                            ((MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue > UserSetSlopMarkPWM) ||
                             (MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue > UserSetSlopMarkPWM)))
                    {
                        if(MotoBSlopModeAdjust.uiSampleAdJudgeModeFinishFlag == 0)
                        {
                            MotoBSlopModeAdjust.iArryCurrentSample[0] = strMoto2Info.uiMotorCurrent;//电流的采样数组
                            MotoBSlopModeAdjust.uiSampleAdJudgeModeFinishFlag = 1;
                        }
                        MotoBSlopModeAdjust.uiSampleAdCnt = 0;
                        MotoBSlopModeAdjust.uiSampleAdSetIntoBreakCnt++;
                        if(MotoBSlopModeAdjust.uiSampleAdSetIntoBreakCnt >= IntoBreakTimer)
                        {
                            MotoBSlopModeAdjust.uiSampleAdSetIntoBreakCnt = IntoBreakTimer;
                            MotoBSlopModeAdjust.uiSampleAdSetPWMFinishFlag = 1;
                        }

                    }
                    else
                    {
                        MotoBSlopModeAdjust.uJudgeModeIntoCheckModeCnt = 0;
                        if(MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue > 0)//||((uiSetMotoB_PwmTim3CCR4==0)&&(uiSampleAdSetCCR3PWMFinishFlag)))
                        {
                            SetTim4CC1andCCR2Reg(SetPwmPinHighReg, 0); //让电不使能一下
                            SetTim4CC3andCCR4Reg(0, 0); //让电不使能一下
                            if(MotoBSlopModeAdjust.uiSampleAdCnt > MotoBSlopModeAdjust.uiReadSampleAdTime)//1MS读一次值
                            {

                                for(uiTimeCont = 0; uiTimeCont < OneChannelConvertTimes; uiTimeCont++)
                                {
                                    MotoBSlopModeAdjust.fVoltage[MotoBSlopModeAdjust.uiReadVoltageTime] += vDmaContAdcVaule[uiTimeCont][Motor2RightHBridgeVolHaveFilterCh];
                                }
                                MotoBSlopModeAdjust.fVoltage[MotoBSlopModeAdjust.uiReadVoltageTime] /= OneChannelConvertTimes;
                                MotoBSlopModeAdjust.uiReadVoltageTime ++;
                            }
                            else
                            {
                                MotoBSlopModeAdjust.uiReadVoltageTime = 0;
                                for(uiTimeCont = 0; uiTimeCont < BigWheelJudgeMode_SetPwmHighTime; uiTimeCont++)
                                {
                                    MotoBSlopModeAdjust.fVoltage[uiTimeCont] = 0;
                                    //MotoBSlopModeAdjust.iArryReadCurrentSample[uiTimeCont]=0;
                                }

                            }
                            if(MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue <= UserSetSlopMarkPWM)
                            {
                                MotoBSlopModeAdjust.uiJudgeMode_SetPwmHighTime = SmallWheelJudgeMode_SetPwmHighTime;
                            }
                            else
                            {
                                MotoBSlopModeAdjust.uiJudgeMode_SetPwmHighTime = BigWheelJudgeMode_SetPwmHighTime;
                            }
                        }
                        else if(MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue > 0)
                        {
                            SetTim4CC1andCCR2Reg(0, 0); //让电不使能一下
                            SetTim4CC3andCCR4Reg(SetPwmPinHighReg, 0); //让电不使能一下
                            if(MotoBSlopModeAdjust.uiSampleAdCnt > MotoBSlopModeAdjust.uiReadSampleAdTime)//1MS读一次值
                            {
                                for(uiTimeCont = 0; uiTimeCont < OneChannelConvertTimes; uiTimeCont++)
                                {
                                    MotoBSlopModeAdjust.fVoltage[MotoBSlopModeAdjust.uiReadVoltageTime] += vDmaContAdcVaule[uiTimeCont][Motor2LeftHBridgeVolHaveFilterCh];
                                }
                                MotoBSlopModeAdjust.fVoltage[MotoBSlopModeAdjust.uiReadVoltageTime] /= OneChannelConvertTimes;
                                MotoBSlopModeAdjust.uiReadVoltageTime ++;
                            }
                            else
                            {
                                MotoBSlopModeAdjust.uiReadVoltageTime = 0;
                                for(uiTimeCont = 0; uiTimeCont < BigWheelJudgeMode_SetPwmHighTime; uiTimeCont++)
                                {
                                    MotoBSlopModeAdjust.fVoltage[uiTimeCont] = 0;
                                }

                            }
                            if(MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue <= UserSetSlopMarkPWM)
                            {
                                MotoBSlopModeAdjust.uiJudgeMode_SetPwmHighTime = SmallWheelJudgeMode_SetPwmHighTime;
                            }
                            else
                            {
                                MotoBSlopModeAdjust.uiJudgeMode_SetPwmHighTime = BigWheelJudgeMode_SetPwmHighTime;
                            }

                        }
                        if(MotoBSlopModeAdjust.uiSampleAdCnt >= (MotoBSlopModeAdjust.uiReadSampleAdTime + MotoBSlopModeAdjust.uiSetHighTime)) //检测300+1MS 或者调整过程100+3MS
                        {
                            MotoBSlopModeAdjust.uiSampleAdCnt = 0;
                            MotoBSlopModeAdjust.fVoltageSample = MotoBSlopModeAdjust.fVoltage[MotoBSlopModeAdjust.uiSetHighTime - 1];		//最新的一个值
                            if(MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue > 0)//当前调节CCR1CCR2
                            {
                                ReDefTIM4CCR1 = ReDefTIM4CCR2 = 0;
                                MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue = 0;
                                if(MotoBSlopModeAdjust.uiSampleAdMode == SampleAdJudgeMode) //调节模式
                                {
                                    MotoBSlopModeAdjust.fLastVoltageSample	 = 	MotoBSlopModeAdjust.fVoltageSample;//MotoBSlopModeAdjust.fVoltage[2];

                                    if((MotoBSlopModeAdjust.fVoltageSample >= IfMotoBSampleAdAtStep1Low) && (MotoBSlopModeAdjust.fVoltageSample <= IfMotoBSampleAdAtStep1High)) //调整完成
                                    {
                                        ucJudgeMotoBreakResult &= (~(MotoBBreakFlagOne | MotoBBreakFlagTwo));
                                        MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt++;
                                        if(MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt >= SampleAdJudgeMode_FinishTime)
                                        {
                                            MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                            MotoBSlopModeAdjust.uiSampleAdMode =	SampleAdCheckMode;
                                            MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdCheckMode_MotoATime;
                                            MotoBSlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                            MotoBSlopModeAdjust.uCheckMode_SetTime = SetPWMFinishCheckMode_SetTime;
                                            MotoBSlopModeAdjust.uCheckMode_ReadFlag = 0;
                                        }
                                    }
                                    else//调整过程
                                    {
																			  ucJudgeMotoBreakResult &= ~MotoBBreakFlagOne;
                                        MotoBSlopModeAdjust.uiSetHighTime = MotoBSlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                        if(MotoBSlopModeAdjust.fVoltageSample > IfMotoBSampleAdAtStep1High)//电压高时减
                                        {
                                            MotoBSlopModeAdjust.uiSampleAdJudgeModeFinishFlag = 0;
                                            MotoBSlopModeAdjust.uiSampleAdJudgeModeCnt = 0;
                                            if(MotoBSlopModeAdjust.fVoltageSample > IfMotoBSampleAdAtStep2High)
                                            {
                                                if((MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue >= TestSlopPWM) && (MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue <= LowSlopPWM))
                                                {
                                                    if(MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue <= ResetSlopPWM)
                                                    {
                                                        if(MotoBSlopModeAdjust.fVoltageSample > IfMotoSampleAdAtErroVoltage)//电机断线
                                                        {
                                                            ucJudgeMotoBreakResult |= MotoBBreakFlagOne;
                                                        }
                                                        else
                                                        {
                                                            ucJudgeMotoBreakResult &= (~(MotoBBreakFlagOne | MotoBBreakFlagTwo));
                                                        }
                                                    }
                                                    MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue = 0;
                                                    MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue = ResetSlopPWM;
                                                }

                                            }
#ifdef SetPwmStep1
                                            MotoBSlopModeAdjust.fSetPwmStep = (-JudgeModeSetDecPareFast);
#else
                                            if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-刹车器程序或0-驻坡程序由参数表第6项决定20190110
                                            {
                                                MotoBSlopModeAdjust.fSetPwmStep = (-MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue * JudgeModebBreakEnDecPareFast);
                                            }
                                            else
                                                MotoBSlopModeAdjust.fSetPwmStep = (-MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue * JudgeModeSetMotoDecPareFast);
#endif
                                            MotoBSlopModeAdjust.uiSetPwmTimerCCR1_Flag = TRUE;
                                            MotoBSlopModeAdjust.uiSetHighTime = MotoBSlopModeAdjust.uiJudgeMode_SetPwmHighTime;

                                        }
                                        else if((MotoBSlopModeAdjust.uCheckMode_ReadFlag == 0) && (MotoBSlopModeAdjust.fVoltageSample < IfMotoBSampleAdAtStep1Low)) //电压低时加
                                        {
                                            ucJudgeMotoBreakResult &= (~(MotoBBreakFlagOne | MotoBBreakFlagTwo));
                                            if(MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue < UserSetSmallWheelSlopPWM)//LowSlopPWM)
                                            {
                                                MotoBSlopModeAdjust.uiSetPwmTimerCCR1_Flag = TRUE;
                                                MotoBSlopModeAdjust.fSetPwmStep = JudgeModeSetPareFast;
                                            }
                                            else
                                            {
                                                if(MotoBSlopModeAdjust.fVoltageSample > 0)
                                                {
                                                    MotoBSlopModeAdjust.fSetPwmStep = JudgeModeSetPare;
                                                    MotoBSlopModeAdjust.uiSampleAdJudgeModeCnt = 0;
                                                    MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                                    MotoBSlopModeAdjust.uiSampleAdMode =	SampleAdCheckMode;
                                                    MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdCheckMode_MotoBTime;
                                                    MotoBSlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                                    MotoBSlopModeAdjust.uCheckMode_SetTime = SetPWMFinishCheckMode_SetTime;
                                                    MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                                }
                                                else
                                                {
                                                    if(MotoBSlopModeAdjust.uiPeopleStandUpFlag)
                                                    {
                                                        MotoBSlopModeAdjust.uiSampleAdJudgeModeCnt ++;
                                                    }
                                                    if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)
                                                    {
                                                        MotoBSlopModeAdjust.fSetPwmStep = UserSetSlopMarkPWM;
                                                    }
                                                    else
                                                        //MotoBSlopModeAdjust.fSetPwmStep = JudgeModeSetPare;
                                                        MotoBSlopModeAdjust.fSetPwmStep = (MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue * JudgeModeSetMotoPlusPareFast);
                                                }

                                                MotoBSlopModeAdjust.uiSetPwmTimerCCR1_Flag = TRUE;
                                                MotoBSlopModeAdjust.uiSampleAdJudgeModeFinishFlag = 0;

                                                if(MotoBSlopModeAdjust.uiSampleAdJudgeModeCnt > AdJudgeModeTimes)
                                                {
                                                    MotoBSlopModeAdjust.uiPeopleStandUpFlag = 0;
                                                    MotoBSlopModeAdjust.uiSampleAdJudgeModeCnt = 0;
                                                    MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                                    MotoBSlopModeAdjust.uiSampleAdMode =	SampleAdCheckMode;
                                                    MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdCheckMode_MotoBTime;
                                                    MotoBSlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                                    MotoBSlopModeAdjust.uCheckMode_SetTime = SetPWMFinishCheckMode_SetTime;
                                                    MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                                    MotoBSlopModeAdjust.fSetPwmStep  = 0;
                                                }
                                            }

                                        }
                                        else
                                        {

                                            MotoBSlopModeAdjust.uCheckMode_ReadFlag = 0;
                                            MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                            MotoBSlopModeAdjust.uiSampleAdMode =	SampleAdCheckMode;
                                            MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdCheckMode_MotoBTime;
                                            MotoBSlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                            MotoBSlopModeAdjust.uCheckMode_SetTime = SetPWMFinishCheckMode_SetTime;
                                            MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                        }

                                    }
                                    if((MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue == MaxSlopPWM) && (MotoBSlopModeAdjust.fVoltageSample < IfMotoBSampleAdAtStep1Low))
                                    {
                                        MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                        MotoBSlopModeAdjust.uiSampleAdMode =	SampleAdCheckMode;
                                        MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdCheckMode_MotoBTime;
                                        MotoBSlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                        MotoBSlopModeAdjust.uiSampleAdJudgeMode_SetCnt = 0;
                                        MotoBSlopModeAdjust.uCheckMode_SetTime = SetPWMFinishCheckMode_SetTime;
                                    }
                                }
                                else  if(MotoBSlopModeAdjust.uiSampleAdMode == SampleAdCheckMode)//测试模式
                                {

                                    MotoBSlopModeAdjust.uiSetPwmTimerCCR1_Flag = 0;
                                    MotoBSlopModeAdjust.fSetPwmStep  = 0;
                                    if(MotoBSlopModeAdjust.uiSampleAdJudgeMode_SetCnt >= (MotoBSlopModeAdjust.uCheckMode_SetTime + 1))
                                    {
                                        MotoBSlopModeAdjust.uiSampleAdJudgeMode_SetCnt = 0;
                                        if((MotoBSlopModeAdjust.fVoltageSample < IfMotoBSampleAdAtStep2Low) || (MotoBSlopModeAdjust.fVoltageSample > IfMotoBSampleAdAtStep1High))
                                        {
                                            MotoBSlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                            if(MotoBSlopModeAdjust.fVoltageSample > IfMotoBSampleAdAtStep1High)
                                            {
                                                MotoBSlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                                MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoATime;

                                                MotoBSlopModeAdjust.uiSetHighTime = MotoBSlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                                MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = 0;
                                            }
                                            else if(MotoBSlopModeAdjust.fVoltageSample < IfMotoBSampleAdAtStep2Low)
                                            {
                                                if(MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue != MaxSlopPWM)
                                                {
                                                    if(MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue < PowerOffSlopPWM)
                                                        //																														 if((MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue <LowSlopPWM)||(MotoBSlopModeAdjust.fVoltageSample <=IfMotoSampleAdAtStep4Low))
                                                    {

                                                        MotoBSlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                                        MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoATime;
                                                        MotoBSlopModeAdjust.uiSetHighTime = MotoBSlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                                        MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = 0;

                                                    }
                                                }
                                                else //if(MotoBSlopModeAdjust.fVoltageSample < IfMotoASampleAdAtStep2Low)
                                                {
                                                    //																											 if((MotoBSlopModeAdjust.uiSampleAdMode == SampleAdJudgeMode)||(MotoBSlopModeAdjust.fVoltageSample >=IfMotoSampleAdAtStep4Low))
                                                    if(MotoBSlopModeAdjust.fVoltageSample >= IfMotoSampleAdAtStep4Low)
                                                    {
                                                        MotoBSlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                                        MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoATime;
                                                        MotoBSlopModeAdjust.uiSetHighTime = MotoBSlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                                        MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = 0;
                                                    }

                                                }
                                            }
                                            else
                                            {
                                                MotoBSlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                            }
                                        }
                                        else
                                        {
                                            MotoBSlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                        }
                                    }
                                    else if(MotoBSlopModeAdjust.uiSampleAdJudgeMode_SetCnt >= MotoBSlopModeAdjust.uCheckMode_SetTime)
                                    {
                                        if(MotoBSlopModeAdjust.fVoltageSample > IfMotoBSampleAdAtStep1High)
                                        {
                                            MotoBSlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                            MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoATime;

                                            MotoBSlopModeAdjust.uiSetHighTime = MotoBSlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                            MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = 0;
                                        }
                                        else
                                            MotoBSlopModeAdjust.uiSetHighTime = SampleAdJudgeFinishCheckMode_SetPwmHighTime;
                                    }
                                    MotoBSlopModeAdjust.uiSampleAdJudgeMode_SetCnt++;

                                    MotoBSlopModeAdjust.uiSampleAdSetIntoBreakCnt++;
                                    if(MotoBSlopModeAdjust.uiSampleAdSetIntoBreakCnt >= IntoBreakTimer)
                                    {
                                        MotoBSlopModeAdjust.uiSampleAdSetIntoBreakCnt = IntoBreakTimer;
                                        MotoBSlopModeAdjust.uiSampleAdSetPWMFinishFlag = 1;
                                    }
                                }
                                else if(MotoBSlopModeAdjust.uiSampleAdMode == SampleAdZeroTest)//CCR1==0,CCR2==0,CCR3==0,CCR4==0
                                {
                                    if (MotoBSlopModeAdjust.fVoltageSample > IfMotoBSampleAdAtStep2High)
                                    {
                                        MotoBSlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                        MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoBTime;
                                        MotoBSlopModeAdjust.uiSetHighTime = MotoBSlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                        MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue = 0;
                                        MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue = ResetSlopPWM;
                                        MotoBSlopModeAdjust.uiSampleAdSetIntoBreakCnt = 0;
                                        MotoBSlopModeAdjust.uiSampleAdSetPWMFinishFlag = 0;
                                    }
                                    else
                                    {
                                        MotoBSlopModeAdjust.uiSampleAdSetIntoBreakCnt++;
                                        if(MotoBSlopModeAdjust.uiSampleAdSetIntoBreakCnt >= IntoBreakTimer)
                                        {
                                            MotoBSlopModeAdjust.uiSampleAdSetIntoBreakCnt = IntoBreakTimer;
                                            MotoBSlopModeAdjust.uiSampleAdSetPWMFinishFlag = 1;
                                        }
                                        MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue = 0;
                                        ReDefTIM4CCR1 = ReDefTIM4CCR2 = 0;
                                        MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue = 0;
                                        ReDefTIM4CCR3 = ReDefTIM4CCR4 = 0;
                                        switch(MotoBSlopModeAdjust.uiZeroTestPhase)
                                        {
                                        case 0:
                                            MotoBSlopModeAdjust.uiZeroTestPhase = 1;
                                            break;
                                        case 1:
                                            MotoBSlopModeAdjust.uiZeroTestPhase = 0;
                                            break;
                                        default:
                                            break;

                                        }
                                    }
                                }
                                if(MotoBSlopModeAdjust.uiSetPwmTimerCCR1_Flag)
                                {
                                    MotoBSlopModeAdjust.uiSetPwmTimerCCR1_Flag = 0;
                                    MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue = MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue + MotoBSlopModeAdjust.fSetPwmStep;

                                }
                                if(MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue >= MaxSlopPWM)//最大占空比限幅
                                {
                                    MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue = MaxSlopPWM;
                                }
                                if(MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue < ResetSlopPWM)//最小占空比限幅
                                {
                                    if(MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue == MinSlopPWM)
                                    {
                                        MotoBSlopModeAdjust.uiSampleAdMode = SampleAdZeroTest;
                                        MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdZeroMode_MotoATime;
                                        MotoBSlopModeAdjust.uiSetHighTime = SampleAdZeroMode_SetPwmHighTime;
                                    }
                                    MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue = 0;
                                }
                            }
                            else if(MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue > 0)//调节CCR3CCR4
                            {
                                ReDefTIM4CCR3 = ReDefTIM4CCR4 = 0;
                                MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue = 0;
                                MotoBSlopModeAdjust.fLastVoltageSample	 = 	MotoBSlopModeAdjust.fVoltageSample;

                                if(MotoBSlopModeAdjust.uiSampleAdMode == SampleAdJudgeMode) //调节模式
                                {

                                    if((MotoBSlopModeAdjust.fVoltageSample >= IfMotoBSampleAdAtStep1Low) && (MotoBSlopModeAdjust.fVoltageSample <= IfMotoBSampleAdAtStep1High)) //调整完成
                                    {
                                        ucJudgeMotoBreakResult &= (~(MotoBBreakFlagOne | MotoBBreakFlagTwo));
                                        MotoBSlopModeAdjust. uiSampleAdSetPWMFinishCnt++;
                                        if(MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt >= SampleAdJudgeMode_FinishTime)
                                        {
                                            MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                            MotoBSlopModeAdjust.uiSampleAdMode =	SampleAdCheckMode;
                                            MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdCheckMode_MotoBTime;
                                            MotoBSlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                            MotoBSlopModeAdjust.uCheckMode_SetTime = SetPWMFinishCheckMode_SetTime;
                                            MotoBSlopModeAdjust.uCheckMode_ReadFlag = 0;

                                        }
                                    }
                                    else//调整过程
                                    {
																			  ucJudgeMotoBreakResult &= ~MotoBBreakFlagTwo;//20210608
                                        MotoBSlopModeAdjust.uiSetHighTime = MotoBSlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                        if(MotoBSlopModeAdjust.fVoltageSample > IfMotoBSampleAdAtStep1High)//电压高时减
                                        {

                                            if(MotoBSlopModeAdjust.fVoltageSample > IfMotoASampleAdAtStep2High)
                                            {
                                                if((MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue >= TestSlopPWM) && (MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue <= LowSlopPWM))
                                                {
                                                    if(MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue <= ResetSlopPWM)
                                                    {
                                                        if(MotoBSlopModeAdjust.fVoltageSample > IfMotoSampleAdAtErroVoltage)//电机断线
                                                        {
                                                            ucJudgeMotoBreakResult |= MotoBBreakFlagTwo;
                                                        }
                                                        else
                                                        {
                                                            ucJudgeMotoBreakResult &= (~(MotoBBreakFlagOne | MotoBBreakFlagTwo));
                                                        }
                                                    }
                                                    MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue = ResetSlopPWM;
                                                    MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue = 0;
                                                }
                                            }
#ifdef SetPwmStep1
                                            MotoBSlopModeAdjust.fSetPwmStep = (-JudgeModeSetDecPareFast);
#else
                                            if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-刹车器程序或0-驻坡程序由参数表第6项决定20190110
                                            {
                                                MotoBSlopModeAdjust.fSetPwmStep = (-MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue * JudgeModebBreakEnDecPareFast);
                                            }
                                            else
                                                MotoBSlopModeAdjust.fSetPwmStep = (-MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue * JudgeModeSetMotoDecPareFast);
#endif
                                            MotoBSlopModeAdjust.uiSetPwmTimerCCR3_Flag = TRUE;
                                            MotoBSlopModeAdjust.uiSetHighTime = MotoBSlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                        }
                                        else if((MotoBSlopModeAdjust.uCheckMode_ReadFlag == 0) && (MotoBSlopModeAdjust.fVoltageSample < IfMotoASampleAdAtStep1Low))	//电压低时加
                                        {
                                            ucJudgeMotoBreakResult &= (~(MotoBBreakFlagOne | MotoBBreakFlagTwo));
                                            if(MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue < UserSetSmallWheelSlopPWM)//LowSlopPWM)
                                            {
                                                MotoBSlopModeAdjust.uiSetPwmTimerCCR3_Flag = TRUE;
                                                MotoBSlopModeAdjust.fSetPwmStep = JudgeModeSetPareFast;
                                            }
                                            else //if(MotoBSlopModeAdjust.fVoltageSample <= 0)//<= IfMotoSampleAdAtStep4Low)
                                            {
                                                if(MotoBSlopModeAdjust.fVoltageSample > 0)
                                                {
                                                    MotoBSlopModeAdjust.fSetPwmStep = JudgeModeSetPare;
                                                    MotoBSlopModeAdjust.uiSampleAdJudgeModeCnt = 0;
                                                    MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                                    MotoBSlopModeAdjust.uiSampleAdMode =	SampleAdCheckMode;
                                                    MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdCheckMode_MotoBTime;
                                                    MotoBSlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                                    MotoBSlopModeAdjust.uCheckMode_SetTime = SetPWMFinishCheckMode_SetTime;
                                                    MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                                }
                                                else
                                                {
                                                    if(MotoBSlopModeAdjust.uiPeopleStandUpFlag)
                                                    {
                                                        MotoBSlopModeAdjust.uiSampleAdJudgeModeCnt ++;
                                                    }
                                                    if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)
                                                    {
                                                        MotoBSlopModeAdjust.fSetPwmStep = UserSetSlopMarkPWM;
                                                    }
                                                    else
                                                        //MotoBSlopModeAdjust.fSetPwmStep = JudgeModeSetPare;
                                                        MotoBSlopModeAdjust.fSetPwmStep = (MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue * JudgeModeSetMotoPlusPareFast);
                                                }
                                                MotoBSlopModeAdjust.uiSetPwmTimerCCR3_Flag = TRUE;
                                                MotoBSlopModeAdjust.uiSampleAdJudgeModeFinishFlag = 0;

                                                if(MotoBSlopModeAdjust.uiSampleAdJudgeModeCnt > AdJudgeModeTimes)
                                                {
                                                    MotoBSlopModeAdjust.uiPeopleStandUpFlag = 0;
                                                    MotoBSlopModeAdjust.uiSampleAdJudgeModeCnt = 0;
                                                    MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                                    MotoBSlopModeAdjust.uiSampleAdMode =	SampleAdCheckMode;
                                                    MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdCheckMode_MotoBTime;
                                                    MotoBSlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                                    MotoBSlopModeAdjust.uCheckMode_SetTime = SetPWMFinishCheckMode_SetTime;
                                                    MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                                    MotoBSlopModeAdjust.fSetPwmStep  = 0;
                                                }
                                            }

                                        }
                                        else
                                        {
                                            MotoBSlopModeAdjust.uCheckMode_ReadFlag = 0;
                                            MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                            MotoBSlopModeAdjust.uiSampleAdMode =	SampleAdCheckMode;
                                            MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdCheckMode_MotoBTime;
                                            MotoBSlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                            MotoBSlopModeAdjust.uCheckMode_SetTime = SetPWMFinishCheckMode_SetTime;
                                            MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                        }

                                    }
                                    if((MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue == MaxSlopPWM) && (MotoBSlopModeAdjust.fVoltageSample < IfMotoBSampleAdAtStep1Low))
                                    {
                                        MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = SampleAdJudgeMode_FinishTime;
                                        MotoBSlopModeAdjust.uiSampleAdMode =	SampleAdCheckMode;
                                        MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdCheckMode_MotoBTime;
                                        MotoBSlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;

                                        MotoBSlopModeAdjust.uiSampleAdJudgeMode_SetCnt = 0;
                                        MotoBSlopModeAdjust.uCheckMode_SetTime = SetPWMFinishCheckMode_SetTime;

                                    }
                                }
                                else  if(MotoBSlopModeAdjust.uiSampleAdMode == SampleAdCheckMode)//测试模式
                                {
                                    MotoBSlopModeAdjust.uiSetPwmTimerCCR3_Flag = 0;
                                    MotoBSlopModeAdjust.fSetPwmStep  = 0;
                                    if(MotoBSlopModeAdjust.uiSampleAdJudgeMode_SetCnt >= (MotoBSlopModeAdjust.uCheckMode_SetTime + 1))
                                    {
                                        MotoBSlopModeAdjust.uiSampleAdJudgeMode_SetCnt = 0;
                                        MotoBSlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                        if((MotoBSlopModeAdjust.fVoltageSample < IfMotoBSampleAdAtStep2Low) || (MotoBSlopModeAdjust.fVoltageSample > IfMotoBSampleAdAtStep1High))
                                        {
                                            if(MotoBSlopModeAdjust.fVoltageSample > IfMotoBSampleAdAtStep1High)
                                            {
                                                MotoBSlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                                MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoATime;
                                                MotoBSlopModeAdjust.uiSetHighTime = MotoBSlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                                MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = 0;
                                            }
                                            else if(MotoBSlopModeAdjust.fVoltageSample < IfMotoBSampleAdAtStep2Low)
                                            {
                                                if(MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue != MaxSlopPWM)
                                                {
                                                    if(MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue < PowerOffSlopPWM)
                                                        //																													 if((MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue <LowSlopPWM)||(MotoBSlopModeAdjust.fVoltageSample <=IfMotoSampleAdAtStep4Low))
                                                    {
                                                        MotoBSlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                                        MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoATime;
                                                        MotoBSlopModeAdjust.uiSetHighTime = MotoBSlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                                        MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = 0;

                                                    }
                                                }
                                                else
                                                {
                                                    //																											 if((MotoBSlopModeAdjust.uiSampleAdMode == SampleAdJudgeMode)||(MotoBSlopModeAdjust.fVoltageSample >=IfMotoSampleAdAtStep4Low))
                                                    if(MotoBSlopModeAdjust.fVoltageSample >= IfMotoSampleAdAtStep4Low)
                                                    {
                                                        MotoBSlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                                        MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoATime;
                                                        MotoBSlopModeAdjust.uiSetHighTime =  MotoBSlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                                        MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = 0;
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                MotoBSlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                            }
                                            MotoBSlopModeAdjust.uiSampleAdSetPWMFinishFlag = 0;

                                        }
                                        else
                                        {

                                            MotoBSlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                        }
                                    }
                                    else if(MotoBSlopModeAdjust.uiSampleAdJudgeMode_SetCnt >= MotoBSlopModeAdjust.uCheckMode_SetTime)
                                    {
                                        if(MotoBSlopModeAdjust.fVoltageSample  > IfMotoBSampleAdAtStep1High)
                                        {
                                            MotoBSlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                            MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoATime;
                                            MotoBSlopModeAdjust.uiSetHighTime = MotoBSlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                            MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = 0;
                                        }
                                        else		MotoBSlopModeAdjust.uiSetHighTime = SampleAdJudgeFinishCheckMode_SetPwmHighTime;
                                    }
                                    MotoBSlopModeAdjust.uiSampleAdJudgeMode_SetCnt++;

                                    MotoBSlopModeAdjust.uiSampleAdSetIntoBreakCnt++;
                                    if(MotoBSlopModeAdjust.uiSampleAdSetIntoBreakCnt >= IntoBreakTimer)
                                    {
                                        MotoBSlopModeAdjust.uiSampleAdSetIntoBreakCnt = IntoBreakTimer;
                                        MotoBSlopModeAdjust.uiSampleAdSetPWMFinishFlag = 1;
                                    }

                                    //																	if(MotoBSlopModeAdjust.uiSampleAdMode == SampleAdJudgeMode)
                                    //																	 {
                                    //																				MotoBSlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                    //																				MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoATime;
                                    //																				MotoBSlopModeAdjust.uiSetHighTime = uiJudgeMode_SetPwmHighTime;//SampleAdJudgeModeFinishMode_SetPwmHighTime;
                                    //																				MotoBSlopModeAdjust.uiSampleAdSetPWMFinishCnt = 0;
                                    //																	 }
                                }
                                else if(MotoBSlopModeAdjust.uiSampleAdMode == SampleAdZeroTest)//CCR1==0,CCR2==0,CCR3==0,CCR4==0
                                {

                                    if (MotoBSlopModeAdjust.fVoltageSample > IfMotoASampleAdAtStep2High)
                                    {
                                        MotoBSlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                        MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoBTime;
                                        MotoBSlopModeAdjust.uiSetHighTime = MotoBSlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                        MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue = ResetSlopPWM;
                                        MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue = 0;
                                        MotoBSlopModeAdjust.uiSampleAdSetIntoBreakCnt = 0;
                                        MotoBSlopModeAdjust.uiSampleAdSetPWMFinishFlag = 0;
                                    }
                                    else
                                    {
                                        MotoBSlopModeAdjust.uiSampleAdSetIntoBreakCnt++;
                                        if(MotoBSlopModeAdjust.uiSampleAdSetIntoBreakCnt >= IntoBreakTimer)
                                        {
                                            MotoBSlopModeAdjust.uiSampleAdSetIntoBreakCnt = IntoBreakTimer;
                                            MotoBSlopModeAdjust.uiSampleAdSetPWMFinishFlag = 1;
                                        }
                                        MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue = 0;
                                        ReDefTIM4CCR1 = ReDefTIM4CCR2 = 0;
                                        MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue = 0;
                                        ReDefTIM4CCR3 = ReDefTIM4CCR4 = 0;
                                        switch(	MotoBSlopModeAdjust.uiZeroTestPhase)
                                        {
                                        case 0:
                                            MotoBSlopModeAdjust.uiZeroTestPhase = 1;
                                            break;
                                        case 1:
                                            MotoBSlopModeAdjust.uiZeroTestPhase = 0;
                                            break;
                                        default:
                                            break;

                                        }
                                    }
                                }
                                if(MotoBSlopModeAdjust.uiSetPwmTimerCCR3_Flag)
                                {
                                    MotoBSlopModeAdjust.uiSetPwmTimerCCR3_Flag = 0;
                                    MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue = MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue + MotoBSlopModeAdjust.fSetPwmStep;

                                }
                                if(MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue >= MaxSlopPWM)//最大占空比限幅
                                {
                                    MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue = MaxSlopPWM;
                                }
                                if(MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue < ResetSlopPWM)//最小占空比限幅
                                {
                                    if(MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue == MinSlopPWM)
                                    {
                                        MotoBSlopModeAdjust.uiSampleAdMode = SampleAdZeroTest;
                                        MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdZeroMode_MotoBTime;
                                        MotoBSlopModeAdjust.uiSetHighTime = SampleAdZeroMode_SetPwmHighTime;
                                    }
                                    else
                                        MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue = 0;
                                }
                            }
                            else//CCR1==0,CCR2==0,CCR3==0,CCR4==0
                            {

                                MotoBSlopModeAdjust.uiSampleAdMode = SampleAdZeroTest;
                                MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdZeroMode_MotoBTime;
                                MotoBSlopModeAdjust.uiSetHighTime = SampleAdZeroMode_SetPwmHighTime;
                                switch(MotoBSlopModeAdjust.uiZeroTestPhase)
                                {
                                case 0:
                                    MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue = TestSlopPWM;
                                    MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue = 0;
                                    MotoBSlopModeAdjust.fReadOldCCR1CCR2PwmValue = TestSlopPWM;
                                    MotoBSlopModeAdjust.fReadOldCCR3CCR4PwmValue = 0;
                                    break;
                                case 1:
                                    MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue = 0;
                                    MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue = TestSlopPWM;
                                    MotoBSlopModeAdjust.fReadOldCCR1CCR2PwmValue = 0;
                                    MotoBSlopModeAdjust.fReadOldCCR3CCR4PwmValue = TestSlopPWM;
                                    break;
                                default:
                                    break;

                                }

                            }
                            ReDefTIM4CCR1 = ReDefTIM4CCR2 = MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue;
                            ReDefTIM4CCR3 = ReDefTIM4CCR4 = MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue;
                        }
                    }
                }
                else
                {
                    if(MotoBSlopModeAdjust.uiSampleAdMode == SampleAdCheckMode)
                    {
                        if((MotoBSlopModeAdjust.uiSampleAdCnt >= 300) && (MotoBSlopModeAdjust.uiSampleAdJudgeModeFinishFlag))
                        {
                            MotoBSlopModeAdjust.uiSampleAdCnt = 0;
                            MotoBSlopModeAdjust.uCheckMode_ReadTime++;

                            MotoBSlopModeAdjust.iArryCurrentSample[1] = strMoto2Info.uiMotorCurrent;//电流的采样数组MotoBSlopModeAdjust.iArryCurrentSample[0];

                            MotoBSlopModeAdjust.iCalCurrentSample = (MotoBSlopModeAdjust.iArryCurrentSample[0] - MotoBSlopModeAdjust.iArryCurrentSample[1]);
                            if((MotoBSlopModeAdjust.iCalCurrentSample < (0 - PlusIntoJudgeModeCurrent))
                                    || (MotoBSlopModeAdjust.iCalCurrentSample  > IntoJudgeModeCurrent)
                                    || (MotoBSlopModeAdjust.uCheckMode_ReadTime >= 10))
                            {

                                MotoBSlopModeAdjust.uiSampleAdJudgeModeFinishFlag = 0;
                                if(MotoASlopModeAdjust.iCalCurrentSample  > IntoJudgeModeCurrent)
                                {
                                    MotoBSlopModeAdjust.uiPeopleStandUpFlag = 1;
                                    MotoBSlopModeAdjust.uiPeopleStandUpFlag = 1;
                                }
                                MotoBSlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoATime;
                                if(MotoBSlopModeAdjust.uCheckMode_ReadTime >= 10)
                                {
                                    MotoBSlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                    MotoBSlopModeAdjust.uCheckMode_ReadFlag = 1;
                                }
                                else
                                    MotoBSlopModeAdjust.uiSetHighTime = BigWheelJudgeMode_SetPwmHighTime;
                                MotoBSlopModeAdjust.uiSampleAdCnt	=	(SampleAdJudgeMode_MotoATime - 2);	//0;//
                                MotoBSlopModeAdjust.uiSampleAdJudgeModeCnt = 0;

                                MotoBSlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                                MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoBTime;
                                if(MotoBSlopModeAdjust.uCheckMode_ReadTime >= 10)
                                {
                                    MotoBSlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                                    MotoBSlopModeAdjust.uCheckMode_ReadFlag = 1;
                                }
                                else
                                    MotoBSlopModeAdjust.uiSetHighTime = BigWheelJudgeMode_SetPwmHighTime;
                                MotoBSlopModeAdjust.uiSampleAdCnt	=	(SampleAdJudgeMode_MotoBTime - 2);
                                MotoBSlopModeAdjust.uiSampleAdJudgeModeCnt = 0;

                                MotoBSlopModeAdjust.uCheckMode_ReadTime = 0;
                            }
                            //			                                if(MotoASlopModeAdjust.iCalCurrentSample  > IntoJudgeModeCurrent)
                            //			                                {
                            //			                                    MotoASlopModeAdjust.uiPeopleStandUpFlag = 1;
                            //			                                    MotoBSlopModeAdjust.uiPeopleStandUpFlag = 1;
                            //			                                }
                            //			                                MotoASlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                            //			                                MotoASlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoATime;
                            //			                                if(MotoASlopModeAdjust.uCheckMode_ReadTime >= 10)
                            //			                                {
                            //			                                    MotoASlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                            //			                                    MotoASlopModeAdjust.uCheckMode_ReadFlag = 1;
                            //			                                }
                            //			                                else
                            //			                                    MotoASlopModeAdjust.uiSetHighTime = BigWheelJudgeMode_SetPwmHighTime;
                            //			                                MotoASlopModeAdjust.uiSampleAdCnt	=	(SampleAdJudgeMode_MotoATime - 2);	//0;//
                            //			                                MotoASlopModeAdjust.uiSampleAdJudgeModeCnt = 0;

                            //			                                MotoBSlopModeAdjust.uiSampleAdMode = SampleAdJudgeMode;
                            //			                                MotoBSlopModeAdjust.uiReadSampleAdTime = SampleAdJudgeMode_MotoBTime;
                            //			                                if(MotoASlopModeAdjust.uCheckMode_ReadTime >= 10)
                            //			                                {
                            //			                                    MotoBSlopModeAdjust.uiSetHighTime = SampleAdCheckMode_SetPwmHighTime;
                            //			                                    MotoBSlopModeAdjust.uCheckMode_ReadFlag = 1;
                            //			                                }
                            //			                                else
                            //			                                    MotoBSlopModeAdjust.uiSetHighTime = BigWheelJudgeMode_SetPwmHighTime;
                            //			                                MotoBSlopModeAdjust.uiSampleAdCnt	=	(SampleAdJudgeMode_MotoBTime - 2);
                            //			                                MotoBSlopModeAdjust.uiSampleAdJudgeModeCnt = 0;

                            //			                                MotoBSlopModeAdjust.uCheckMode_ReadTime = 0;

                        }
                    }
                }                //-------------------------------------------------------------------------------//
            }
        }	//刹车

    }
}


unsigned char ucCheckRemoteTime(unsigned int uiTimes)
{
    unsigned int uiTimeCont = 0;
    for(uiTimeCont = 0; uiTimeCont < 200; uiTimeCont++)
    {
        g_uiTimeNumCont1ms = 1;//延时一下
        while(g_uiTimeNumCont1ms)
        {
            vDealAdcVauleMotoAndCurrent();
            if((SysNoDirection != ucCalcDirection()) || ucErroType)//如果有错误的情况的
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

void vAutoSlopeStopFuction(void)//自动驻坡功能的处理函数
{
    ucFrontAutoSlopFlag = FALSE;

    vAdjustPwmAtSlopModeFastAdjus();

}
void PwmDataInit(void)
{
    ReDefTIM1CCR1 = ReDefTIM1CCR2 = 0;
    ReDefTIM1CCR3 = ReDefTIM1CCR4 = 0;
    ReDefTIM4CCR1 = ReDefTIM4CCR2 = 0;
    ReDefTIM4CCR3 = ReDefTIM4CCR4 = 0;
}
