/*
       "LG_X2445M_V2"  Design By LG_qiuzhi
       V1.0   2013.6
       used mcu  STM32F103C8(6)x
*/
//file DriverContrl.C

#include "DriverConst.h"
#include "math.h"
void vDealAdcVaule(void);//����AD
unsigned char  ucPowerOnCotrol(void);//�״�
//void vCheckSystemInfo(void);//���
void vContolRealize(void);//���Ƶ�ʵ��
void vRunMotor(unsigned  char ucDirctionData);//�����ת��ʵ��
void vCheckHardWareErro(void);//ȥ���Ӳ������

#define FirstDownPwm 800//��һ�μ���������λ��
volatile struct strSysTemInfo strSysInfo = {0};//ϵͳһЩ״̬�ṹ��Ķ���
volatile struct strMotorInfo strMoto1Info = {0}, strMoto2Info = {0};//���������һЩ��Ϣ  strMoto1Info->X���   strMoto2Info��>Y���
volatile unsigned char ucSpeedDowenFlag = FALSE;
// void vDelayms(u32 uiDelayTime);//
void vDelay30us(u32 uiDelayTime);
unsigned char ucLastDirection = NoDirection;//�ϴ�ҡ�˵ķ���
unsigned char ucMosFetCoolDownTime = 0;//MosFet��ȴʱ��
unsigned char ucAutoRollingFlag = FALSE;
unsigned char ucPowerKeySate = KeyPressDown;//����״ֵ̬���ݴ����������������
unsigned char ucKeyUpTime = 0;// ���������õĶ�ʱ������
unsigned int uiCheckMotoBreakTimeCont = 0;//����ʱ�� ���ڼ�鶨ʱ��
union unioBatterLowPowerCom g_unioBatterLowPowerCom = {0};

void vDealAdcVauleMotoAndCurrent(void);
//unsigned char ucBreakDelayTime = 0;
//unsigned int uiBreakPwmTab[3610] = {0};//ɲ�����������ֻ���ڿ�����ʱ������ǲ����ı������²�ȥ����һ�µ�
/*
��ʱ���� ��λΪ30us �߾���
*/
int uiAngle = 0;
unsigned char ucFrontAutoSlopFlag = FALSE;//����������ʱ�� �Զ�פ�²�����־ȫ�ֱ���
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
    }//20160927 �ø�  �޸�Ϊ 620  ԭ 520.���Ϊ520 ���ӻ��г崴�ĸо�
}
void vDelay30us(u32 uiDelayTime)//72mHz
{
    u32 uiTemp, uiTemp2;
    for(uiTemp = 0; uiTemp < uiDelayTime; uiTemp++)
    {
        //400
        for(uiTemp2 = 0; uiTemp2 < 410; uiTemp2++);//410
    }
}//��20150505  �궨�� V1.7�汾һ��
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
//        for(uiTemp2 = 0; uiTemp2 < 380; uiTemp2++);//20190930��ǰ 620
//    }//20160927 �ø�  �޸�Ϊ 620  ԭ 520.���Ϊ520 ���ӻ��г崴�ĸо�
//}
//void vDelay30us(u32 uiDelayTime)//72mHz
//{
//    u32 uiTemp, uiTemp2;
//    for(uiTemp = 0; uiTemp < uiDelayTime; uiTemp++)
//    {
//        //400
//        for(uiTemp2 = 0; uiTemp2 < 410; uiTemp2++);//410
//    }
//}//��20150505  �궨�� V1.7�汾һ��
//void vDelay30usBreak(u32 uiDelayTime)//72mHz
//{
//    u32 uiTemp, uiTemp2;
//    for(uiTemp = 0; uiTemp < uiDelayTime; uiTemp++)
//    {
//        //400
//        for(uiTemp2 = 0; uiTemp2 < 823; uiTemp2++);//20190930��ǰ 740
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
        for(uiTemp2 = 0; uiTemp2 < 800; uiTemp2++);//20190930��ǰ 620
    }//20160927 �ø�  �޸�Ϊ 620  ԭ 520.���Ϊ520 ���ӻ��г崴�ĸо�
}
void vDelay30us(u32 uiDelayTime)//72mHz
{
    u32 uiTemp, uiTemp2;
    for(uiTemp = 0; uiTemp < uiDelayTime; uiTemp++)
    {
        //400
        for(uiTemp2 = 0; uiTemp2 < 410; uiTemp2++);//410
    }
}//��20150505  �궨�� V1.7�汾һ��
void vDelay30usBreak(u32 uiDelayTime)//72mHz
{
    u32 uiTemp, uiTemp2;
    for(uiTemp = 0; uiTemp < uiDelayTime; uiTemp++)
    {
        //400
        for(uiTemp2 = 0; uiTemp2 < 1700; uiTemp2++);//20190930��ǰ 740
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
}//��20150505  �궨�� V1.7�汾һ��
void vDelay30usBreak(u32 uiDelayTime)//72mHz
{
    u32 uiTemp, uiTemp2;
    for(uiTemp = 0; uiTemp < uiDelayTime; uiTemp++)
    {
        //400
        for(uiTemp2 = 0; uiTemp2 < 1300; uiTemp2++);//  ȷ��ɲ�� Ч����һ����
    }//780
}
#endif

/*
�ж�ң����Ҫת���ķ���
���ط���ֵ
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
��ɲ���Ĺ����жԵ������״̬���ж�
����һ����ֵ
*/
#define MotoA 0x01
#define MotoB 0x02
unsigned int uiInDownSpeedCalcMotoSate(unsigned char ucMotoAorB, unsigned char ucCalcTimeMs)
{
    unsigned short usMoto_Sample[150] = {0};
    unsigned int uiTemp = 0;
    unsigned int uiCheckTime ;
    unsigned int uiOldMotoPWM = 0;
    if(MotoA == ucMotoAorB)//����ǶԵ��A�ļ��
    {
        if((ReDefTIM1CCR1) > 30)//�����ccr1
        {
            uiOldMotoPWM = ReDefTIM1CCR1;//��ԭ��ֵ��¼����
            SetTim1CC1andCCR2Reg(SetPwmPinHighReg, 0);//��ʹ�ܵ��һ��
        }
        else if((ReDefTIM1CCR3) > 30)
        {
            uiOldMotoPWM = ReDefTIM1CCR3;
            SetTim1CC3andCCR4Reg(SetPwmPinHighReg, 0);
        }
#ifdef HK32
        uiCheckTime = ucCalcTimeMs * 53;//ʱ������¸�ֵ  //2020-02-24 ֮ǰ�궨
        //uiCheckTime = ucCalcTimeMs * 62;//ʱ������¸�ֵ
#endif
#ifdef STM32
        uiCheckTime = ucCalcTimeMs * 65;//ʱ������¸�ֵ---AT32F421
#endif
        for(uiTemp = 0; uiTemp < uiCheckTime; uiTemp++)
        {
            vDealAdcVauleMotoAndCurrent();
            if((ReDefTIM1CCR1) > 30)usMoto_Sample[uiTemp] = strMoto1Info.uiRightHBridgeHaveFilterAdc;
            if((ReDefTIM1CCR3) > 30)usMoto_Sample[uiTemp] = strMoto1Info.uiLeftHBridgeHaveFilterAdc;
        }
        if((ReDefTIM1CCR1) > 30)
        {
            ReDefTIM1CCR1 = ReDefTIM1CCR2 = uiOldMotoPWM;//��ʱ�ػ�ԭ����
        }
        if((ReDefTIM1CCR3) > 30)
        {
            ReDefTIM1CCR3 = ReDefTIM1CCR4 = uiOldMotoPWM;//��ʱ�ػ�ԭ����
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
    if(MotoB == ucMotoAorB)//����ǶԵ��A�ļ��
    {
        if((ReDefTIM4CCR1) > 30)//�����ccr1
        {
            uiOldMotoPWM = ReDefTIM4CCR1;//��ԭ��ֵ��¼����
            SetTim4CC1andCCR2Reg(SetPwmPinHighReg, 0);//��ʹ�ܵ��һ��
        }
        else if((ReDefTIM4CCR3) > 30)
        {
            uiOldMotoPWM = ReDefTIM4CCR3;
            SetTim4CC3andCCR4Reg(SetPwmPinHighReg, 0);
        }
        uiCheckTime = ucCalcTimeMs * 60;//ʱ������¸�ֵ
        for(uiTemp = 0; uiTemp < uiCheckTime; uiTemp++)
        {
            vDealAdcVauleMotoAndCurrent();
            if((ReDefTIM4CCR1) > 30)usMoto_Sample[uiTemp] = strMoto2Info.uiRightHBridgeHaveFilterAdc;
            if((ReDefTIM4CCR3) > 30)usMoto_Sample[uiTemp] = strMoto2Info.uiLeftHBridgeHaveFilterAdc;
        }
        if((ReDefTIM4CCR1) > 30)
        {
            ReDefTIM4CCR1 = ReDefTIM4CCR2 = uiOldMotoPWM;//��ʱ�ػ�ԭ����
        }
        if((ReDefTIM4CCR3) > 30)
        {
            ReDefTIM4CCR3 = ReDefTIM4CCR4 = uiOldMotoPWM;//��ʱ�ػ�ԭ����
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
void vDownSpeedCalcPwm(unsigned char ucFlag)//����ɲ��
{
    unsigned char ucDirectionX = FALSE;
    unsigned char ucDirectionY = FALSE;
    u32 uiTemp, uiStepX = 0, uiStepY = 0; //
    u32 uiDeltSteps = 0;
    u32 uiOldPwmX = 0, uiOldPwmY = 0;
    unsigned int uiBreakDelayAct = 0;
    if(Moto1AStmPwmReg)//�����ʱ���A�ĵ�ǰ PWMֵ
    {
        ucDirectionX = Forward;//ȷ���ϴε�ֵ�뷽��
        uiOldPwmX = Moto1AStmPwmReg;
    }
    else
    {
        ucDirectionX = Reverse;
        uiOldPwmX = Moto1BStmPwmReg;
    }
    if(Moto2AStmPwmReg)//�����ʱ���B�ĵ�ǰ PWMֵ
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
        for(uiTemp = 0 ; uiTemp < TimerBase; uiTemp++)//�ҳ���ǰʱ�̶�Ӧ�ģ��ٶ�λ��
        {
            if((uiBreakPwmTab[uiTemp + 1] <= uiOldPwmX) && (uiBreakPwmTab[uiTemp] >= uiOldPwmX))break;
        }
        uiOldPwmX = uiTemp;
        uiStepX = 3600 - uiTemp;//���Ҫ���ٵĲ���
    }
    else
    {
        uiStepX = uiOldPwmX = 0;
        //return;
    }

    if(uiOldPwmY > (uniDiverInfo.strDirverAtt.ucStopMinPwm * 10))//�����������
    {
        for(uiTemp = 0 ; uiTemp < TimerBase; uiTemp++)
        {
            if((uiBreakPwmTab[uiTemp + 1] <= uiOldPwmY) && (uiBreakPwmTab[uiTemp] >= uiOldPwmY))break;
        }
        uiOldPwmY = uiTemp;
        uiStepY = 3600 - uiTemp;//���Ҫ���ٵĲ���
    }
    else
    {
        uiStepY = uiOldPwmY = 0;
        //return;
    }
    if(uiStepX > uiStepY)uiDeltSteps = uiStepX;
    else uiDeltSteps = uiStepY;//���� ��һ�����Ҫ��ɲ����ʱ����ó�һ��
    uiBreakDelayAct = uniDiverInfo.strDirverAtt.unDelayBreakActTime.all * ((float)(uiDeltSteps / 3600.0)) + 50;//��̬���㣬�˴�Ҫ��ʱ��ɲ������ʱ��
    for(uiTemp = 0; uiTemp < uiDeltSteps; uiTemp++)
    {
        if(uiStepX > 0)
        {
            uiStepX -- ;
            if(Forward == ucDirectionX)
            {
                CtlSetMotor1LeftPwmPercent(uiBreakPwmTab[uiOldPwmX + uiTemp]);
                CtlSetMotor1RightPwmPercent(0);
            }//��������ת
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
            ucNewDirectCont = ucCalcDirection();//�����ɲ���Ĺ�����һ���е��ת���������˳���ǰ��״̬
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

    ucSpeedDowenFlag = TRUE;//�Ѿ�ɲ����־
}
#endif
////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
unsigned char ucSpeedDowenRuningFlag = FALSE;
//unsigned char ucSlopeRuningFlag = FALSE;
#ifdef UsedStep
#define  Fast_Step 0.95
#define  Slow_Step 0.96
float Speed_Step = Slow_Step;//PWM��Ӧ�ļ��ٶ�
double uiOldPwmX = 0, uiOldPwmY = 0;
double uiSetPwmX = 0, uiSetPwmY = 0;
unsigned int uiTextTime = 0;
unsigned int uiGlobleSpeedDowenTime = 0;
void vDownSpeedCalcPwm(unsigned char ucFlag)//����ɲ��
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
                    CtrlExtSpeekerDis;//�������ﴦ����
                    uiTextTime = 0;
                }

                if(Moto2AStmPwmReg)//�����ʱ���B�ĵ�ǰ PWMֵ
                {
                    uiOldPwmY = Moto2AStmPwmReg;
                }
                else
                {
                    uiOldPwmY = Moto2BStmPwmReg;
                }
                if(Moto1AStmPwmReg)//�����ʱ���A�ĵ�ǰ PWMֵ
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
            ucSpeedDowenFlag = TRUE;//�Ѿ�ɲ����־
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
unsigned int uiStartCalcPwmDataValue = 3300;//3300;  2020-03-24  ����������ר��
//unsigned int uiStartCalcPwmDataValue = 3970;//3300;  2020-02-22
#endif

unsigned char g_ucLowPwmIntoBeadk = FALSE; //СPWM����ɲ����ʱ��
u32  ucCalcBrakeMotoACurrent = 0;
u32  ucCalcBrakeMotoBCurrent = 0;
void vDownSpeedCalcPwm(unsigned char ucFlag)//����ɲ��
{

    unsigned char ucDirectionX = FALSE;
    unsigned char ucDirectionY = FALSE;
    unsigned char ucCalcBrakeMotoACurrentFlag = FALSE;//��ʼ����
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

    if(Moto1AStmPwmReg)//�����ʱ���A�ĵ�ǰ PWMֵ
    {
        ucDirectionX = Forward;//ȷ���ϴε�ֵ�뷽��
        uiOldPwmX = Moto1AStmPwmReg;
    }
    else
    {
        ucDirectionX = Reverse;
        uiOldPwmX = Moto1BStmPwmReg;
    }

    if(Moto2AStmPwmReg)//�����ʱ���B�ĵ�ǰ PWMֵ
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
        for(uiTemp = 0 ; uiTemp < TimerBase; uiTemp++)//�ҳ���ǰʱ�̶�Ӧ�ģ��ٶ�λ��
        {
            if((uiBreakPwmTab[uiTemp + 1] <= uiOldPwmX) && (uiBreakPwmTab[uiTemp] >= uiOldPwmX))break;
        }
        uiOldPwmX = uiTemp;
        uiStepX = 3600 - uiTemp;//���Ҫ���ٵĲ���
    }
    else
    {
        uiStepX = uiOldPwmX = 0;
    }

    if(uiOldPwmY > (uniDiverInfo.strDirverAtt.ucStopMinPwm * 10))//�����������
    {
        for(uiTemp = 0 ; uiTemp < TimerBase; uiTemp++)

        {
            if((uiBreakPwmTab[uiTemp + 1] <= uiOldPwmY) && (uiBreakPwmTab[uiTemp] >= uiOldPwmY))break;
        }
        uiOldPwmY = uiTemp;
        uiStepY = 3600 - uiTemp;//���Ҫ���ٵĲ���
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

        //        if(((ReDefTIM1CCR1) < 550) && ((ReDefTIM1CCR3) < 550))//������A��ɲ��PWMС��20% ���ٽ���һ�μ�����
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
    //				if(strMoto1Info.unMotorOldDirection != strMoto2Info.unMotorOldDirection)//���ת��һ�� 201812
    //				{
    //						//�����ת���ʱ�򣬾ͽ���ֵ����Сһ�㡣�Ͳ�����ƽ·�����µ���ͣ������һֱ����
    //						uiStartCalcPwmDataValue = 800;
    //				}

    //    if(uiOldPwmX < 3550)uiOldPwmX += 3;
    //    if(uiOldPwmY < 3550)uiOldPwmY += 3;
    if(uiStepX > uiStepY)uiDeltSteps = uiStepX;
    else uiDeltSteps = uiStepY;//���� ��һ�����Ҫ��ɲ����ʱ����ó�һ��
    uiBreakDelayAct = uniDiverInfo.strDirverAtt.unDelayBreakActTime.all * ((float)(uiDeltSteps / 3600.0)) + 50;//��̬���㣬�˴�Ҫ��ʱ��ɲ������ʱ��
    for(uiTemp = 0; uiTemp < uiDeltSteps; uiTemp++)
    {
        if(uiStepX > 0)//����ǵ�X�ļ������
        {
            uiStepX -- ;
            if(Forward == ucDirectionX)
            {
                if(uiBreakPwmTab[uiOldPwmX + uiTemp] > strSysInfo.uiControlMaxXDriverPwm)//�޷����
                {
                    CtlSetMotor1LeftPwmPercent(strSysInfo.uiControlMaxXDriverPwm);
                }
                else
                {
                    CtlSetMotor1LeftPwmPercent(uiBreakPwmTab[uiOldPwmX + uiTemp]);
                }
                CtlSetMotor1RightPwmPercent(0);
            }//��������ת
            if(Reverse == ucDirectionX)
            {
                CtlSetMotor1LeftPwmPercent(0);
                if(uiBreakPwmTab[uiOldPwmX + uiTemp] > strSysInfo.uiControlMaxXDriverPwm)//�޷����
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
                if(uiBreakPwmTab[uiOldPwmY + uiTemp] > strSysInfo.uiControlMaxYDriverPwm)//�޷����
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
                if(uiBreakPwmTab[uiOldPwmY + uiTemp] > strSysInfo.uiControlMaxYDriverPwm)//�޷����
                {
                    CtlSetMotor2RightPwmPercent(strSysInfo.uiControlMaxYDriverPwm);
                }
                else
                {
                    CtlSetMotor2RightPwmPercent(uiBreakPwmTab[uiOldPwmY + uiTemp]);
                }

            }

        }
        //////////////////////������Ҫ�鿴�µ����ת�˹����е�һЩ״̬��������ڣ����ϵ�����д���////////////////////////
        if(FALSE == ucCalcBrakeMotoACurrentFlag)//������Ҫ�ж��µ���Ƿ���ͣ���������
        {
            if((ReDefTIM1CCR1) > 50)//����ǵ��A�����
            {
                if((ReDefTIM1CCR1) < CalSlopPWM)//20%��PWM
                {
                    ucCalcBrakeMotoACurrentFlag = TRUE;
                    ucMotoA_SampleTimesCont++;
                    usInBreakMotoA_VoltageCon[ucMotoA_SampleTimesCont] = uiInDownSpeedCalcMotoSate(MotoA, 2);
                }
            }
            if((ReDefTIM1CCR3) > 50)
            {
                if((ReDefTIM1CCR3) < CalSlopPWM)//20%��PWM
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
                if(ucMotoA_SampleTimesCont < 25)//��ֹ�������
                {
                    usInBreakMotoA_VoltageCon[ucMotoA_SampleTimesCont] = uiInDownSpeedCalcMotoSate(MotoA, 1);
                }
                uiCalcMotoStateTimeCont_A = 0;
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        if(FALSE == ucCalcBrakeMotoBCurrentFlag)//������Ҫ�ж��µ���Ƿ���ͣ���������
        {
            if((ReDefTIM4CCR1) > 50)
            {
                if((ReDefTIM4CCR1) < CalSlopPWM)//20%��PWM
                {
                    ucMotoB_SampleTimesCont++;
                    ucCalcBrakeMotoBCurrentFlag = TRUE;
                    usInBreakMotoB_VoltageCon[ucMotoB_SampleTimesCont] = uiInDownSpeedCalcMotoSate(MotoB, 2);
                }
            }
            if((ReDefTIM4CCR3) > 50)
            {
                if((ReDefTIM4CCR3) < CalSlopPWM)//20%��PWM
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
                if(ucMotoB_SampleTimesCont < 25)//��ֹ�������
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
                if(strMoto1Info.unMotorOldDirection == strMoto2Info.unMotorOldDirection == Forward)//���ת��һ�� 20190218
                {
                    if((ReDefTIM1CCR1) > 50)//����ǵ��A�����
                    {
                        if((ReDefTIM1CCR1) < IntoSlopPWM)//MaxSlopPwm)//20%��PWM
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
                        if((ReDefTIM1CCR3) < IntoSlopPWM)//MaxSlopPwm)//20%��PWM
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
                if(strMoto1Info.unMotorOldDirection == strMoto2Info.unMotorOldDirection == Forward)//���ת��һ�� 20190218
                {
                    if((ReDefTIM4CCR1) > 50)//����ǵ��A�����
                    {
                        if((ReDefTIM4CCR1) < IntoSlopPWM)//MaxSlopPwm)//20%��PWM
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
                        if((ReDefTIM4CCR3) < IntoSlopPWM)//MaxSlopPwm)//20%��PWM
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
					  strSysInfo.uiRemoteNowYPos = vDmaContAdcVaule[0][RemoteYAdcCh];//DMAֱ�Ӳ���������  
					  strSysInfo.uiRemoteNowXPos = vDmaContAdcVaule[0][RemoteXAdcCh];//DMAֱ�Ӳ���������  
            ucNewDirectCont = ucCalcDirection();//�����ɲ���Ĺ�����һ���е��ת���������˳���ǰ��״̬
            //if(strSysInfo.uiCalcSysShowPower > CheckLowAlarmVoltage)//�͵�ѹ����ִ��ɲ������
            {
                if(SysNoDirection != ucNewDirectCont) return;
            }
        }
    }
    if(TRUE == ucSlopFlag)//�����פ�µ�ʱ��
    {
        ucSpeedDowenFlag = TRUE;//�Ѿ�ɲ����־
        ucEnAdcCalc = TRUE;
        if((TRUE == ucLowPwmInBreak_MA) || (TRUE == ucLowPwmInBreak_MB))//��������PWM��С��
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
            for(uiTemp = 0; uiTemp < uiDeltSteps; uiTemp++)//��������һ�����ٹ���һ��
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
                    ucNewDirectCont = ucCalcDirection();//�����ɲ���Ĺ�����һ���е��ת���������˳���ǰ��״̬
                    if(SysNoDirection != ucNewDirectCont) return;
                }
            }
            ReDefTIM1CCR1 = ReDefTIM1CCR2 = ReDefTIM1CCR3 = ReDefTIM1CCR4 = 0;
            ReDefTIM4CCR1 = ReDefTIM4CCR2 = ReDefTIM4CCR3 = ReDefTIM4CCR4 = 0;
            uiCheckMotoBreakTimeCont = 0;//��ֹ��ɲ���󣬶�פ�µ�������
        }
        if(ReDefTIM1CCR1 > 50)//��ֹ��С
        {
            if(ReDefTIM1CCR1 < LowSlopPWM)//230  20190216
            {
                ReDefTIM1CCR1 = ReDefTIM1CCR2 = 0;
            }
        }
        if(ReDefTIM1CCR3 > 50)//��ֹ��С
        {
            if(ReDefTIM1CCR3 < LowSlopPWM)//230
            {
                ReDefTIM1CCR3 = ReDefTIM1CCR4 = 0;
            }
        }
        if(ReDefTIM4CCR1 > 50)//��ֹ��С
        {
            if(ReDefTIM4CCR1 < LowSlopPWM)//230
            {
                ReDefTIM4CCR1 = ReDefTIM4CCR2 = 0;
            }
        }
        if(ReDefTIM4CCR3 > 50)//��ֹ��С
        {
            if(ReDefTIM4CCR3 < LowSlopPWM)//230
            {
                ReDefTIM4CCR3 = ReDefTIM4CCR4 = 0;//��ֹ��ɲ���󣬶�פ�µ�������
            }
        }
        return;
    }
    ucEnAdcCalc = TRUE;
    if(FALSE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-ɲ���������0-פ�³����ɲ������6�����20190110
    {
        uiBreakDelayAct = 0;//��פ�¿������У����������������Сһ��
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
    if(FALSE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//���û��ɲ����
    {
        if(strMoto1Info.unMotorOldDirection != strMoto2Info.unMotorOldDirection)//���ת��һ�µ����
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
            strMoto2Info.unMotorOldDirection = strMoto1Info.unMotorOldDirection = NoDirection;//������ת��Ķ���
        }
    }
    CtlSetMotor1RightPwmPercent(0);
    CtlSetMotor1LeftPwmPercent(0);
    CtlSetMotor2RightPwmPercent(0);
    CtlSetMotor2LeftPwmPercent(0);
    ucSpeedDowenFlag = TRUE;//�Ѿ�ɲ����־
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
unsigned int uiPowerValueDataConter = 0; //��ص����ۼ�����������ص��������ۼ���
unsigned int uiOldBattPowerValue = 0;

#define BattPowerAddTimes              500//�ھ�ֹ״̬�£�����ʱ�����һ�ε���ֵ  �ɼ����ݵ���������
#define ShowPowerInRunTimes        2000  //26000   //�����е�״̬�µģ����������Ĵ�С
WORK_STATE_u Work_Status;
#define CheckLowBattContTime  8000
///////////////////////////////////////////////////////
void vCalcBataAsShow(unsigned char ucShowPowerInRunFlag)//�ڿ��Ƶ�ʱ�� �Ե�����ָʾ
{
    //����������е�״̬�µĺ���
    unsigned int uiTemp = 0;
    unsigned int uiTempTime = 0;
    static unsigned short usCheckLowBattAdcSum = 0;
    if(uniDiverInfo.strDirverAtt.unMotorInfo.bits.bLilonOrQiansuan)//﮵����Ǧ���ص�����
    {	
        if(strSysInfo.uiSysTemPower >= 3240)uiTemp = 3240;   //�����Ǧ���أ�����ѹ����25.3v �ٸߵĵ�ѹֵ���ټ�¼
        else if(strSysInfo.uiSysTemPower <= 2150)uiTemp = 2150;   //�����Ǧ���أ��ҵ�ѹС��23.5v �ٸߵĵ�ѹֵ���ټ�¼
        else uiTemp = strSysInfo.uiSysTemPower;
#ifndef UsedLinSuanTieLi					
        if(strSysInfo.uiSysTemPower <= 2530)//����ѹ����22.3v��ʱ���Ҵ���5���ȥ�ػ�
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
    else //�����Ǧ����
    {
        if(strSysInfo.uiSysTemPower >= 2950)uiTemp = 2950;   //�����Ǧ���أ�����ѹ����25.3v �ٸߵĵ�ѹֵ���ټ�¼
        else if(strSysInfo.uiSysTemPower <= 2150)uiTemp = 2150;   //�����Ǧ���أ��ҵ�ѹС��22v �ٸߵĵ�ѹֵ���ټ�¼
        else uiTemp = strSysInfo.uiSysTemPower;//���û�� ����һ���ķ�Χ  ��ȥ����ʱ�ĵ�����ֵ��
    }
    uiPowerValueDataConter++;//��������������
    uiPowerValueData += uiTemp;//��������
    if(TRUE == ucShowPowerInRunFlag)
    {
        if( Work_Status.bit.Runing == 0 )
        {
            Work_Status.bit.Slop = 0;
            Work_Status.bit.Ready = 0;
            Work_Status.bit.Runing = 1;
            uiPowerValueData = uiPowerValueDataConter = 0;//��ռ�����
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
            uiPowerValueData = uiPowerValueDataConter = 0;//��ռ�����
        }
        uiTempTime = BattPowerAddTimes;//�ж��ں���״̬�µģ��Ĳ���ģʽ
    }
    if(uiPowerValueDataConter > uiTempTime)//���������������
    {
        uiPowerValueData /= uiTempTime;//ȡһ����ֵ-

        if(uniDiverInfo.strDirverAtt.unMotorInfo.bits.bLilonOrQiansuan)
        {
            uiOldBattPowerValue = (unsigned int)((float)(uiPowerValueData * 0.012) + (float)(uiOldBattPowerValue * 0.988));
        }
        else
        {
            uiOldBattPowerValue = (unsigned int)((float)(uiPowerValueData * 0.022) + (float)(uiOldBattPowerValue * 0.978));
        }
        strSysInfo.uiCalcSysShowPower = uiOldBattPowerValue;//����һ�¼�������ĵ���ֵ
        uiPowerValueData = uiPowerValueDataConter = 0;//��ռ�����
        //        if(strSysInfo.uiSysTemPower < CheckLowAlarmVoltage)//�����ѹ��18v���� ��ȥ��ȷ��ָʾ��
        //        {
        //            strSysInfo.uiCalcSysShowPower =  strSysInfo.uiSysTemPower;//����ǵ͵�ѹ����� ��������ֵ�����پ�������
        //        }

        if(strSysInfo.uiCalcSysShowPower < CheckLowAlarmVoltage)//�����ѹ��18v���� ��ȥ��ȷ��ָʾ��  2020-02-22
        {
            strSysInfo.uiCalcSysShowPower =  strSysInfo.uiSysTemPower;//����ǵ͵�ѹ����� ��������ֵ�����پ�������
        }

        //if(ucShowPowerInRunFlag)//����������е�״̬�� ����һ�ε���ָʾ
        {
            vSendSysPower(strSysInfo.uiCalcSysShowPower);
        }
    }
}


void vContolRealize(void)//���Ƶ�ʵ��
{
#define ErroPosTime 30//ҡ��������ʱ��
    unsigned int uiTemp;
    ucNewDirectCont = ucCalcDirection();
    ucRunChangeDirDeal();
    if((SysNoDirection == ucNewDirectCont) && (ucFrontAutoSlopFlag == FALSE))vDownSpeedCalcPwm(FALSE); //20181215
    if(TRUE == ucSpeedDowenFlag )
    {
        if(TRUE == ucLastDirection)vSendSingleOrder(BikeOutBack);
        ucLastDirection = FALSE;
        ucNewDirectCont = ucCalcDirection();
        //				if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-ɲ���������0-פ�³����ɲ������6�����20190529
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
            //��������ж���
            ucTag10ms = FALSE;
            while(FALSE == ucTag10ms);
            ucNewDirectCont = ucCalcDirection();
            if(SysNoDirection == ucNewDirectCont)break;
        }
        if((uiTemp + 2) > (ErroPosTime))
        {
            ucSpeedDowenFlag = FALSE;
            if(ucAutoRollingFlag)//����������ﳵ�����
            {
                vSendOneByteOrder(OrderSpeekP, 0x02);//Alarm Test Is End
                ucAutoRollingFlag = FALSE;
            }
            uiPowerValueData = uiPowerValueDataConter = 0;//����ת����ʱ��  ���ص�ѹ������
            vRunMotor(ucNewDirectCont); //ȥת��
            //vDownSpeedCalcPwm(TRUE);//��ֹ�����Ƕ�׵���
            uiPowerValueData = uiPowerValueDataConter = 0;//ת���˳���ʱ��  ���ص�ѹ������
            ucShowPowerCont = FALSE;//�õ�ص�ѹ�ȶ�����ȥ��ʾ��
            ucTag300ms = FALSE;//�õ���ָʾ�ĸ�ƽ��һ��
            ucMosFetCoolDownTime = 0;
        }
        else
        {
            //            if(TRUE == ucSpeedDowenFlag )CtlBrakeBikeDisEn;20190529
        }
    }
}
unsigned short  uiOverCurrentCont = 0;   //����������
void vRunMotor(unsigned char ucDirctionData)
{
#ifdef UsedYingKe50A
#define MaxContinueOutCurrent  2180 //������������������ܶ�ת�೤ʱ�䶼�е������
#else
#define MaxContinueOutCurrent  1850 //������������������ܶ�ת�೤ʱ�䶼�е������	
#endif
#define CurrentReduceDeltTime  80//�𼶽�������ʱ����
#define MaxCurrentHoldLonTime  5000//����������ʱ��
#define AlarmAnlge 40
#define HighAlarmAngle 360 - AlarmAnlge
#define LowAlarmAngle 180 + AlarmAnlge
#define AlarmMinYValue  1400
#define ErrAngle 0.2//����һ����Ϊ�������ʱ��
#define FastAccTime 6
#define MiddleAccTime 6//�м���ٶ�ֵ
#define LowAccTime    6
#define ErroLeftAndRightPwmP  1 //������Ϊ��һ�����޵�ʱ����ڱ���
#define UsedMoto1AndMotor2P//���ʹ�õ��1����2�Ļ�����ϵ
    float fStep1SpeedPare, fStep2SpeedPare, fStep3SpeedPare, fStep4SpeedPare;//�嵵�ٶȱ���
    float fStep1BackSwSpeedPare, fStep2BackSwSpeedPare, fStep3BackSwSpeedPare, fStep4BackSwSpeedPare;//�嵵�ٶȱ��� ������ת��
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
    unsigned int uiAccTime  = uniDiverInfo.strDirverAtt.ucDriverAcc;//20140718  ʵʱ���ٶ�ֵ
    s32 iTempPwmX, iTempPwmY;//
    unsigned char ucNeedChangDirXFlag = FALSE;
    unsigned char ucNeedChangDirYFlag = FALSE; //�Ƿ�Ҫ�����־
    unsigned char ucRelationMoto1Mtor2 = FALSE;
    s32 uiMotor1AndMotor2P;//��������໥���Ʊ���
    float fLowPowerRunMotoCom = 0;//���͵�ѹ��ʱ������Ĳ���
    unsigned int uiStartPwm = 0;//����PWM
    unsigned char ucOpenBrakeFlag = FALSE;
    unsigned short usMaxDriverCurrent = 2510;//�����������
    unsigned short usMaxDriverCurrentOld = 0;//�����������
    unsigned int uiOverCurrentCont = 0;   //����������
    unsigned int uiOverCurrentCntNextReachTime = 0;   //����������  �´γ�����ʱ��
    usMaxDriverCurrent = 2510;//�����������//��2435�ĵ���

#ifdef UsedYingKe50A
    if(35 == uniDiverInfo.strDirverAtt.ucMaxDriverCurrent)usMaxDriverCurrent = 2510;//����������������35A
    else if(45 == uniDiverInfo.strDirverAtt.ucMaxDriverCurrent)usMaxDriverCurrent = 3800;//����������������45A
    else if(50 == uniDiverInfo.strDirverAtt.ucMaxDriverCurrent)usMaxDriverCurrent = 3800;////����������������50A
    else if(uniDiverInfo.strDirverAtt.ucMaxDriverCurrent > 30)usMaxDriverCurrent = 3800;//�����������ǰ�����÷���
#else
    if(35 == uniDiverInfo.strDirverAtt.ucMaxDriverCurrent)usMaxDriverCurrent = 2510;//����������������35A
    else if(45 == uniDiverInfo.strDirverAtt.ucMaxDriverCurrent)usMaxDriverCurrent = 3000;//����������������45A
    else if(50 == uniDiverInfo.strDirverAtt.ucMaxDriverCurrent)usMaxDriverCurrent = 3000;////����������������50A
    else if(uniDiverInfo.strDirverAtt.ucMaxDriverCurrent > 30)usMaxDriverCurrent = 3000;//�����������ǰ�����÷���
#endif

    uiOverCurrentCont = 0;
    usMaxDriverCurrentOld = usMaxDriverCurrent;
    uiOverCurrentCntNextReachTime = MaxCurrentHoldLonTime;
    fTemp = (1 - (uniDiverInfo.strDirverAtt.ucForwardMinSpeedP * 0.01)) / 4.0;
    fStep4SpeedPare = 1 - fTemp;
    fStep3SpeedPare = 1 - 2 * fTemp;
    fStep2SpeedPare = 1 - 3 * fTemp;
    fStep1SpeedPare = 1 - 4 * fTemp;//��������������������ٶȱ����ķֶ�ֵ

    if(uniDiverInfo.strDirverAtt.ucSwerveMaxSpeedP > uniDiverInfo.strDirverAtt.ucSwerveMinSpeedP)
    {
        iDeltPwmX = uniDiverInfo.strDirverAtt.ucSwerveMaxSpeedP - uniDiverInfo.strDirverAtt.ucSwerveMinSpeedP;
    }
    else iDeltPwmX = uniDiverInfo.strDirverAtt.ucSwerveMinSpeedP - uniDiverInfo.strDirverAtt.ucSwerveMaxSpeedP;
    fTemp = iDeltPwmX * 0.005;
    fStep4BackSwSpeedPare = 1 - fTemp;
    fStep3BackSwSpeedPare = 1 - 2 * fTemp;
    fStep2BackSwSpeedPare = 1 - 3 * fTemp;
    fStep1BackSwSpeedPare = 1 - 4 * fTemp;//��������������������ٶȱ����ķֶ�ֵ
    //uiErroPare = uniDiverInfo.strDirverAtt.ucLetRightMotorTiedP * 40;//
    uiErroPare = uniDiverInfo.strDirverAtt.ucLetRightMotorTiedP * 700;//700
    ucNewDirectCont = ucCalcDirection();
    if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-ɲ���������0-פ�³����ɲ������6�����20190110
    {
        //uiStartPwm = uniDiverInfo.strDirverAtt.ucStarMinPwm * 36;//�����¿���ɲ��������СPWM
        uiStartPwm = strSysInfo.uiCalcPwmInBreak;//��¼���ϴ�ֵ
        if(uiStartPwm > (uniDiverInfo.strDirverAtt.ucStarMinPwm * 36))uiStartPwm = uniDiverInfo.strDirverAtt.ucStarMinPwm * 36;
        ucOpenBrakeFlag = FALSE;
    }
    while(SysNoDirection != ucNewDirectCont)
    {
        uiAutoPowerOffTimeCont = 0;
        ucEnAdcCalc = TRUE;//����ʵʱ�ؿ��� AD����
        if(ucErroType)
        {
            vAutoPowerOffTimeFlag();
            return;
        }
        if(strSysInfo.uiRemoteNowYPos < 2600)//���ҡ�˹�λ��
        {
            usMaxDriverCurrent = usMaxDriverCurrentOld;//�������������ԭ
            uiOverCurrentCont = 0;//���������������
            uiOverCurrentCntNextReachTime = MaxCurrentHoldLonTime;//�𼶼��������������ٴ���λ
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
        if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//��ɲ��������20190110
        {
            if(FALSE == ucOpenBrakeFlag)//���û�п���ɲ���������
            {
                if((ReDefTIM1CCR1 > uiStartPwm) || (ReDefTIM1CCR3 > uiStartPwm) || (ReDefTIM4CCR1 > uiStartPwm) || (ReDefTIM4CCR3 > uiStartPwm))
                {
                    //���һ���ﵽ��������
                    CtlBrakeBikeEn;
                    ucOpenBrakeFlag = TRUE;//���ٿ�����
                }
            }
        }
        vCalcBataAsShow(TRUE);//��̬�������ص�ѹ ����ʵֵ
        ucNewDirectCont = ucCalcDirection();
        uiPwmYDelt = 0;
        uiPwmXDelt = 0;
        iTempPwmX = XDriverPwmDelt;//��iTempPwmX�ں���ĵط������õ����ʴ˸�ֵ
        iTempPwmY = YDriverPwmDelt;//
        ucNeedChangDirXFlag = FALSE;
        ucNeedChangDirYFlag = FALSE;//��ʼ������
        uiReMoteXTemp = strSysInfo.uiRemoteNowXPos;//��ǰҪ�����ҡ��ֵ�������ڿ��ƹ��������ҡ��ֵ�б��ٴ����岹
        uiReMoteYTemp = strSysInfo.uiRemoteNowYPos;
        for(uiTemp = 0 ; uiTemp < 40; uiTemp++)//�����ǰӦ�ÿ��Ƶ����ת��
        {
            if((uiReMoteXTemp >= (strSysInfo.uiRemoteMiddleXPos + uiTemp * strSysInfo.uiRemoteXP_Delt + strSysInfo.uiRemoteXP_First)) \
                    && (uiReMoteXTemp < (strSysInfo.uiRemoteMiddleXPos + (uiTemp + 1)*strSysInfo.uiRemoteXP_Delt + strSysInfo.uiRemoteXP_First)))
            {
                uiPwmXDelt = strSysInfo.uiControlMinXDriverPwm + (iTempPwmX * (uiTemp + 1));//���X_PWM�ȼ�
            }//X��ת�Ŀ���
            else if((uiReMoteXTemp >= (strSysInfo.uiRemoteMiddleXPos  -  (uiTemp + 1)*strSysInfo.uiRemoteXN_Delt - strSysInfo.uiRemoteXN_First)) \
                    && (uiReMoteXTemp < (strSysInfo.uiRemoteMiddleXPos - uiTemp * strSysInfo.uiRemoteXN_Delt - strSysInfo.uiRemoteXN_First)))
            {
                uiPwmXDelt = strSysInfo.uiControlMinXDriverPwm + (iTempPwmX * (uiTemp + 1));//���X_PWM�ȼ�
            }//X��ת�Ŀ���
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
                uiPwmYDelt = strSysInfo.uiControlMinYDriverPwm + (iTempPwmY * (uiTemp + 1));//���Y_PWM�ȼ�Y
            }//Y��ת�Ŀ���
            else if((uiReMoteYTemp >= (strSysInfo.uiRemoteMiddleYPos  -  (uiTemp + 1)*strSysInfo.uiRemoteYN_Delt - strSysInfo.uiRemoteYN_First)) \
                    && (uiReMoteYTemp < (strSysInfo.uiRemoteMiddleYPos - uiTemp * strSysInfo.uiRemoteYN_Delt - strSysInfo.uiRemoteYN_First)))
            {
                uiPwmYDelt = strSysInfo.uiControlMinYDriverPwm + (iTempPwmY * (uiTemp + 1));//���Y_PWMPWM�ȼ�
            }//Y��ת�Ŀ���
            if(uiReMoteYTemp > strSysInfo.uiRemoteMaxYPos)
            {
                uiPwmYDelt = strSysInfo.uiControlMaxYDriverPwm;
            }
            if(uiReMoteYTemp < (strSysInfo.uiRemoteMinYPos))
            {
                uiPwmYDelt = strSysInfo.uiControlMaxYDriverPwm;
            }
        }//�ҳ�ҡ��ֵ��һ��׼ȷλ��
        if((strSysInfo.uiLastRemotValue < 150) && (strSysInfo.uiLastRemotValue > 30))//�������15-165���м��ֵ���ٶȰ�����Ч��������Ч
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

            if(SpeedGrad5 == ucSpeedGrad)	//�ٶȱ����ֶ�
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
            else fLowPowerRunMotoCom = 1;//���κη��� ��ҡ�˶��Ļ��ή�ٵ�

            if(SpeedGrad5 == ucSpeedGrad);	//�ٶȱ����ֶ�
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
        //        if(JudgeForward == (ucDirctionData & JudgeForward))//��������������ϡ�
        //        {
        //            if(uiPwmXDelt > uiPwmYDelt)//����������
        //            {
        //                fTemp = (float)(uiPwmYDelt / (uiPwmXDelt * 1.0));//�������Y���й�С������
        //                if(fTemp < ErrAngle) ucDirctionData &= (~JudgeForward);
        //            }
        //            else
        //            {
        //                fTemp = (float)(uiPwmXDelt / (uiPwmYDelt * 1.0));   //����з���X�й�С������
        //                if(fTemp < ErrAngle) ucDirctionData = JudgeForward;
        //            }
        //        }	  //���ǿ���
        //        if(JudgeBack == (ucDirctionData & JudgeBack))
        //        {
        //            if(uiPwmXDelt > uiPwmYDelt)//����������
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
        //        }	  //���ǿ���
        switch(ucNewDirectCont)//�ж�ҡ������һ�����ޣ����������Ӧ��PWM��ֵ
        {
        case (JudgeRigt | JudgeForward)://��һ����
        {
            uiTempKpDelayCont =  sqrt((uiPwmXDelt * uiPwmXDelt) + (uiPwmYDelt * uiPwmYDelt));
            fTemp = (uiPwmYDelt * 1.0) / ( uiTempKpDelayCont * 1.0);
            uiAngleTemp = (unsigned int)(asin( fTemp) * 57.296);

            if(FourQuadrantPosAsPwmArryMoto1_A[uiAngleTemp])
            {
                strMoto1Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto1_A[uiAngleTemp] ;
                strMoto1Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto1Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxXDriverPwm;
                ucDirectionX = Forward;	   //�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
            }
            else
            {
                strMoto1Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto1_B[uiAngleTemp] ;
                strMoto1Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto1Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxXDriverPwm;
                ucDirectionX = Reverse;	  //�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
            }
            if(FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp])
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp] ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Forward;		  //�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
            }
            else
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_B[uiAngleTemp] ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Reverse;		//�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
            }
            if(strMoto1Info.uiNewMotorPwmValue > strMoto2Info.uiNewMotorPwmValue)strMoto2Info.uiNewMotorPwmValue *= ErroLeftAndRightPwmP;
            else strMoto1Info.uiNewMotorPwmValue *= ErroLeftAndRightPwmP;
            strSysInfo.uiLastControlQuadrant = QuadrantOne;
            strSysInfo.uiLastRemotValue = uiAngleTemp;
        }
        break;
        case (JudgeForward)://�������ΪY����
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
                ucDirectionX = Forward;			//�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
            }
            else
            {
                strMoto1Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto1_B[uiAngleTemp] ;
                strMoto1Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto1Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxXDriverPwm;
                ucDirectionX = Reverse;			   //�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
            }
            if(FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp])
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp] ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Forward;		   //�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
            }
            else
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_B[uiAngleTemp] ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Reverse;			//�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
            }
            //if(uiAngleTemp>70)uiAccTime = LowAccTime;
            //uiAccTime = MiddleAccTime;
            strSysInfo.uiLastControlQuadrant = QuadrantYPostive;
            strSysInfo.uiLastRemotValue = uiAngleTemp;

        }
        break;
        case (JudgeLef | JudgeForward)://�ڶ�����
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
                ucDirectionX = Forward;		//�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
            }
            else
            {
                strMoto1Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto1_B[uiAngleTemp] ;
                strMoto1Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto1Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxXDriverPwm;
                ucDirectionX = Reverse;	 //�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
            }
            if(FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp])
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp] ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Forward;	 //�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
            }
            else
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_B[uiAngleTemp] ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Reverse;	  //�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
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
        case (JudgeLef)://�������ΪX�Ḻ������
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
                ucDirectionX = Forward;		  //�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
            }
            else
            {
                strMoto1Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto1_B[uiAngleTemp]  ;
                strMoto1Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto1Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxXDriverPwm;
                ucDirectionX = Reverse;		//�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
            }
            if(FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp])
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp]  ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Forward;		//�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
            }
            else
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_B[uiAngleTemp]  ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Reverse;	   //�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
            }
        }
        break;
        case (JudgeLef | JudgeBack)://��������
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
                ucDirectionX = Forward;	  //�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
            }
            else
            {
                strMoto1Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto1_B[uiAngleTemp]  ;
                strMoto1Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto1Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxXDriverPwm;
                ucDirectionX = Reverse;		//�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
            }
            if(FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp])
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp]  ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Forward;	  //�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
            }
            else
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_B[uiAngleTemp]  ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Reverse;		//�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
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
        case (JudgeBack)://�������ΪY������
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
        case (JudgeRigt | JudgeBack)://��������
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
                ucDirectionX = Forward;	  //�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
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
                ucDirectionY = Forward;		 //�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
            }
            else
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_B[uiAngleTemp]  ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Reverse;	   //�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
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
        case (JudgeRigt)://���������X��������
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
                ucDirectionX = Forward;		  //�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
            }
            else
            {
                strMoto1Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto1_B[uiAngleTemp]  ;
                strMoto1Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto1Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxXDriverPwm;
                ucDirectionX = Reverse;		 //�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
            }
            if(FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp])
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_A[uiAngleTemp] ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Forward;		 //�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
            }
            else
            {
                strMoto2Info.uiNewMotorPwmValue = FourQuadrantPosAsPwmArryMoto2_B[uiAngleTemp]  ;
                strMoto2Info.uiNewMotorPwmValue *= uiTempKpDelayCont;
                strMoto2Info.uiNewMotorPwmValue /=	  strSysInfo.uiControlMaxYDriverPwm;
                ucDirectionY = Reverse;		  //�ҳ������ж�Ӧ��ֵ��ȷ�����������ת
            }
        }
        break;
        default:
            break;
        }
        uiAngle = uiAngleTemp;
        if(strMoto1Info.uiNewMotorPwmValue > strMoto2Info.uiNewMotorPwmValue)
        {
            uiMotor1AndMotor2P =  (strMoto1Info.uiNewMotorPwmValue * uiErroPare) / strMoto2Info.uiNewMotorPwmValue ; //���1����2���Ʊ���
            ucRelationMoto1Mtor2 = 0x01;//ֻ���������	 //���������ı���ֵ
        }
        else
        {
            uiMotor1AndMotor2P =  (strMoto2Info.uiNewMotorPwmValue * uiErroPare) / strMoto1Info.uiNewMotorPwmValue ; //���1����2���Ʊ���
            ucRelationMoto1Mtor2 = 0x02;//ֻ���������  �з������
        }
        if((Forward == ucDirectionX) && (Forward == ucDirectionY));
        else ucRelationMoto1Mtor2 = FALSE;//������������������ת��״̬���Ͳ�Ҫȥ��

        if((Moto1AStmPwmReg) || (Moto1BStmPwmReg))
        {
            strMoto1Info.uiOldMotorPwmValue = (Moto1AStmPwmReg) > (Moto1BStmPwmReg) ? (Moto1AStmPwmReg) : (Moto1BStmPwmReg);
            if(Moto1AStmPwmReg) strMoto1Info.unMotorOldDirection = Forward;
            else strMoto1Info.unMotorOldDirection = Reverse;	  //���ϴε����PWM��ȷ�����ϴε����ת������
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
            else strMoto2Info.unMotorOldDirection = Reverse;   //���ϴε����PWM��ȷ�����ϴε����ת������
        }
        else
        {
            strMoto2Info.uiOldMotorPwmValue  = 0;
            strMoto2Info.unMotorOldDirection = NoDirection;
        }
        iDeltPwmX = strMoto1Info.uiNewMotorPwmValue - strMoto1Info.uiOldMotorPwmValue;//��������ϴ�ֵ�����
        iDeltPwmY = strMoto2Info.uiNewMotorPwmValue - strMoto2Info.uiOldMotorPwmValue;//��������ϴ�ֵ�����
        if(iDeltPwmX > 0)//�����X�Ӽ���
        {
            ucDecIncSpeedX = IncSpeed;
        }
        else
        {
            //����Ǽ���
            ucDecIncSpeedX = DecSpeed;
            iDeltPwmX = 0 - iDeltPwmX; //��������
        }
        if(iDeltPwmY > 0)//�����Y�Ӽ���
        {
            ucDecIncSpeedY = IncSpeed;
        }
        else
        {
            //����Ǽ���
            ucDecIncSpeedY = DecSpeed;
            iDeltPwmY = 0 - iDeltPwmY;
        }
        if(strMoto1Info.unMotorOldDirection && ucDirectionX && (ucDirectionX != strMoto1Info.unMotorOldDirection))
        {
            ucNeedChangDirXFlag = TRUE;
            //uiAccTime = FastAccTime;
            ucDecIncSpeedX = DecSpeed;
            iDeltPwmX =  strMoto1Info.uiOldMotorPwmValue;	 //��ʱ�ٶ�Ҫ��Ϊ0
            ucDirectionX = strMoto1Info.unMotorOldDirection ;//�Ƿ�Ҫ����
        }
        if(strMoto2Info.unMotorOldDirection && ucDirectionY && (ucDirectionY != strMoto2Info.unMotorOldDirection))
        {
            ucNeedChangDirYFlag = TRUE;
            //uiAccTime = FastAccTime;
            ucDecIncSpeedY = DecSpeed;
            iDeltPwmY =  strMoto2Info.uiOldMotorPwmValue;	 //��ʱ�ٶ�Ҫ��Ϊ0
            ucDirectionY = strMoto2Info.unMotorOldDirection ;
        }
        uiAccTime  = uniDiverInfo.strDirverAtt.ucDriverAcc;
        if((TRUE == ucNeedChangDirXFlag ) && (TRUE == ucNeedChangDirYFlag ))//�����˫������Ҫ��ͣ�����
        {
            if((strSysInfo.uiLastRemotValue > 60) && (strSysInfo.uiLastRemotValue < 120)) //���Ҫ����60�ȵ�120�������ͬʱ�����Ŀ϶���Ҫ��ͣ��
            {
                //                uiAccTime = FastAccTime;
                uiAccTime = (unsigned int)(uniDiverInfo.strDirverAtt.ucSwerveAcc * 2.5);
                ucRelationMoto1Mtor2 = FALSE;//�ɺ���ֱ���л���ǰ��
            }
            if((strSysInfo.uiLastRemotValue > 240) && (strSysInfo.uiLastRemotValue < 300)) //���Ҫ����60�ȵ�120�������ͬʱ�����Ŀ϶���Ҫ��ͣ��
            {
                //                uiAccTime = FastAccTime;
                ucRelationMoto1Mtor2 = FALSE;//��ǰ��ֱ���л�������
                uiAccTime = (unsigned int)(uniDiverInfo.strDirverAtt.ucSwerveAcc * 0.8);
            }
        }
        else
        {
            ////////////////
            if((strSysInfo.uiLastRemotValue < 350) && (strSysInfo.uiLastRemotValue > 190))//�������һ���нǺ��˵�ʱ��
            {
                //��������������˵����ת������ں��˵Ĺ����У�˵��Ҫ����ļ��ٶ�
                uiAccTime = (unsigned int)(uniDiverInfo.strDirverAtt.ucSwerveAcc * 1.3);
            }
        }
        uiStepX = iDeltPwmX / strSysInfo.uiControlPareOfKp;//������ٲ���
        uiStepY = iDeltPwmY / strSysInfo.uiControlPareOfKp;//������ٲ���
        if(uiStepX > uiStepY)uiDeltSteps = uiStepX;
        else uiDeltSteps = uiStepY;
        uiPwmXDelt = 0;
        uiPwmYDelt = 0;
        iTempPwmX = strMoto1Info.uiOldMotorPwmValue;
        iTempPwmY = strMoto2Info.uiOldMotorPwmValue;
        ucOverCurrentAskQuit = FALSE;
        for(uiTemp = 0; uiTemp < uiDeltSteps ; uiTemp++)//���ٹ���
        {
            uiPwmXDelt++;
            uiPwmYDelt++;
            if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-ɲ���������0-פ�³����ɲ������6�����20190110
            {
                if(FALSE == ucOpenBrakeFlag)//���û�п���ɲ���������
                {
                    if((ReDefTIM1CCR1 > uiStartPwm) || (ReDefTIM1CCR3 > uiStartPwm) || (ReDefTIM4CCR1 > uiStartPwm) || (ReDefTIM4CCR3 > uiStartPwm))
                    {
                        //���һ���ﵽ��������
                        if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)CtlBrakeBikeEn;
                        ucOpenBrakeFlag = TRUE;//���ٿ�����
                    }
                    uiAccTime = uniDiverInfo.strDirverAtt.ucSwerveAcc * 0.3; //����һ�����������ġ�
                }
                else uiAccTime = uniDiverInfo.strDirverAtt.ucSwerveAcc;//����Ǽ������
            }

            /////////////////////////////////////////////////////////////////////////////////////////  ��������
            if(LeftLowMosFetPwmReg)//ReDefTIM1CCR3
            {
                if(strMoto1Info.uiMotorCurrent >= usMaxDriverCurrent)
                {
                    ucOverCurrentAskQuit = TRUE;
                    if(LeftLowMosFetPwmReg > 2)SetTim1CC3andCCR4(LeftLowMosFetPwmReg - 1);
                    vDelay30us(uniDiverInfo.strDirverAtt.ucDriverAcc);//���ٶ�
                }
            }
            if(LeftHighMosFetPwmReg)//ReDefTIM1CCR1
            {
                if(strMoto1Info.uiMotorCurrent >= usMaxDriverCurrent)
                {
                    ucOverCurrentAskQuit = TRUE;
                    if(LeftHighMosFetPwmReg > 2)SetTim1CC1andCCR2(LeftHighMosFetPwmReg - 1);
                    vDelay30us(uniDiverInfo.strDirverAtt.ucDriverAcc);//���ٶ�
                }
            }
            if(RightLowMosFetPwmReg)//ReDefTIM4CCR3
            {
                if(strMoto2Info.uiMotorCurrent >= usMaxDriverCurrent)
                {
                    ucOverCurrentAskQuit = TRUE;
                    if(RightLowMosFetPwmReg > 2)SetTim4CC3andCCR4(RightLowMosFetPwmReg - 1);
                    vDelay30us(uniDiverInfo.strDirverAtt.ucDriverAcc);//���ٶ�
                }
            }
            if(RightHighMosFetPwmReg)//ReDefTIM4CCR1
            {
                if(strMoto2Info.uiMotorCurrent >= usMaxDriverCurrent)
                {
                    ucOverCurrentAskQuit = TRUE;
                    if(RightHighMosFetPwmReg > 2)SetTim4CC1andCCR2(RightHighMosFetPwmReg - 1);
                    vDelay30us(uniDiverInfo.strDirverAtt.ucDriverAcc);//���ٶ�
                }
            }
            if(TRUE == ucOverCurrentAskQuit)break;
            if((TRUE == ucOverCurrentAskQuit) || ((strMoto2Info.uiMotorCurrent >= MaxContinueOutCurrent) && (strMoto1Info.uiMotorCurrent >= MaxContinueOutCurrent)))
            {
                uiOverCurrentCont ++;
                if(uiOverCurrentCont >= MaxCurrentHoldLonTime)//�������һ���������������ʱ��
                {
                    if(uiOverCurrentCont >= uiOverCurrentCntNextReachTime)//�������һ������������
                    {
                        usMaxDriverCurrent -= 3;//�𼶼��������
                        if(usMaxDriverCurrent <= MaxContinueOutCurrent)usMaxDriverCurrent = MaxContinueOutCurrent;//�޷����
                        uiOverCurrentCntNextReachTime += CurrentReduceDeltTime;//�´μ�������������
                    }
                    break;
                }
                if(uiOverCurrentCont > 0xFFFFFFF)uiOverCurrentCont = 0xFFFFFFF;
            }
            /////////////////////////////////////////////////////////����
            if(uiStepX > 0)//���Ǽ����X�Ӽ���
            {
                //����Ӽ���û�мӼ����
                uiStepX--;//ÿһ�μ�����󣬾�ȥ�����ݼӼ�
                if(IncSpeed == ucDecIncSpeedX)/////////////////////////////////��X���ٹ���
                {
                    if((strMoto1Info.uiOldMotorPwmValue + uiPwmXDelt * strSysInfo.uiControlPareOfKp) > strSysInfo.uiControlMinXDriverPwm)
                    {
                        iTempPwmX = strMoto1Info.uiOldMotorPwmValue + uiPwmXDelt * strSysInfo.uiControlPareOfKp;//�������Ĺ�С
                    }
                    else
                    {
                        iTempPwmX = 0;
                    }
                    if((strMoto1Info.uiOldMotorPwmValue + uiPwmXDelt * strSysInfo.uiControlPareOfKp) >= strSysInfo.uiControlMaxXDriverPwm)
                    {
                        iTempPwmX = strSysInfo.uiControlMaxXDriverPwm;//���PWM����
                        //break;
                    }
                    else
                    {
                        iTempPwmX = strMoto1Info.uiOldMotorPwmValue + uiPwmXDelt * strSysInfo.uiControlPareOfKp;
                    }//
                }
                if(DecSpeed == ucDecIncSpeedX)//////////////////////////////////��X�Ӽ�����
                {
                    if((strMoto1Info.uiOldMotorPwmValue - uiPwmXDelt * strSysInfo.uiControlPareOfKp) >= strSysInfo.uiControlMinXDriverPwm)
                    {
                        iTempPwmX = strMoto1Info.uiOldMotorPwmValue - uiPwmXDelt * strSysInfo.uiControlPareOfKp; //�������Ĺ�С
                    }
                    else
                    {
                        iTempPwmX = 0;
                        //break;
                    }
                    if((strMoto1Info.uiOldMotorPwmValue - uiPwmXDelt * strSysInfo.uiControlPareOfKp) >= strSysInfo.uiControlMaxXDriverPwm)
                    {
                        iTempPwmX = strSysInfo.uiControlMaxXDriverPwm; //���PWM����
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
                        //�����һ������ı�������
                        if(iTempPwmX > iTempPwmY)//�����ʱ���б�������
                        {
                            //Ҫ��X>Y
                            if(((iTempPwmX * uiErroPare) / iTempPwmY) > (uiMotor1AndMotor2P + 1)) //�����������
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
                            else //������ε��ڵı�������
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
                        else//���X<Y
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
                        //�����һ������ı�������
                        if(iTempPwmY > iTempPwmX)//�����ʱ���б�������
                        {
                            //Ҫ��Y>X
                            if(((iTempPwmY * uiErroPare) / iTempPwmX) > (uiMotor1AndMotor2P + 1)) //�����������
                            {
                                if(DecSpeed == ucDecIncSpeedX)
                                {
                                    uiPwmXDelt--;
                                }
                                else
                                {
                                    CtlSetMotor1LeftPwmPercent(iTempPwmX);   //������ε��ڵı�������;//������ε��ڵı�������
                                }
                            }
                            else //������ε��ڵı�������
                            {
                                if(DecSpeed == ucDecIncSpeedX)
                                {
                                    CtlSetMotor1LeftPwmPercent(iTempPwmX);
                                }
                                else
                                {
                                    uiPwmXDelt--;   //������ε��ڵı�������;//������ε��ڵı�������
                                }
                            }
                        }
                        else//���X>Y
                        {
                            //uiPwmXDelt--;   //������ε��ڵı�������;//������ε��ڵı�������
                            if(DecSpeed == ucDecIncSpeedX)
                            {
                                CtlSetMotor1LeftPwmPercent(iTempPwmX);
                            }
                            else
                            {
                                uiPwmXDelt--;   //������ε��ڵı�������;//������ε��ڵı�������
                            }
                        }
                    }
                    else CtlSetMotor1LeftPwmPercent(iTempPwmX);
                    CtlSetMotor1RightPwmPercent(0);
                    //������аϵ������������������
                    //                     if((strMoto2Info.uiNewMotorPwmValue == strMoto1Info.uiNewMotorPwmValue)//�����PWMҪ��
                    // 											&&(strMoto2Info.uiNewMotorPwmValue >3600)&&(strMoto1Info.uiNewMotorPwmValue >3600))
                    //                     {
                    //                         iDealErroPwm = strMoto2Info.uiOldMotorPwmValue - strMoto1Info.uiOldMotorPwmValue;
                    //                         if((iDealErroPwm < 100)&&iDealErroPwm)//��������100����
                    //                         {
                    //                             strMoto1Info.uiOldMotorPwmValue++;
                    //                             CtlSetMotor1LeftPwmPercent(strMoto1Info.uiOldMotorPwmValue);
                    //                         }
                    //                         else
                    //                         {
                    //                             iDealErroPwm = 0 - iDealErroPwm;
                    //                             if(iDealErroPwm < 100)//��������100����
                    //                             {
                    //                                 strMoto1Info.uiOldMotorPwmValue++;
                    //                                 CtlSetMotor1LeftPwmPercent(strMoto1Info.uiOldMotorPwmValue);
                    //                             }
                    //                         }
                    //                     }
                    //
                }//��������ת
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

            if(uiStepY > 0)//Y�Ӽ��ٹ���
            {
                //����Ӽ���û�мӼ����
                uiStepY--;//ÿ����һ�ζԼӼ������Ӽ�һ��
                if(IncSpeed == ucDecIncSpeedY)///////////////////////////Y������Ƶ�ʵ�֡����ٹ���
                {
                    if((strMoto2Info.uiOldMotorPwmValue + uiPwmYDelt * strSysInfo.uiControlPareOfKp) > strSysInfo.uiControlMinYDriverPwm)
                    {
                        iTempPwmY = strMoto2Info.uiOldMotorPwmValue + uiPwmYDelt * strSysInfo.uiControlPareOfKp;//�������Ĺ�С
                    }
                    else
                    {
                        iTempPwmY = 0;
                    }
                    if((strMoto2Info.uiOldMotorPwmValue + uiPwmYDelt * strSysInfo.uiControlPareOfKp) >= strSysInfo.uiControlMaxYDriverPwm)
                    {
                        iTempPwmY = strSysInfo.uiControlMaxYDriverPwm;//���PWM����
                        //break;
                    }
                    else
                    {
                        iTempPwmY = strMoto2Info.uiOldMotorPwmValue + uiPwmYDelt * strSysInfo.uiControlPareOfKp;
                    }//
                }
                if(DecSpeed == ucDecIncSpeedY)/////////////////Y������Ƶ�ʵ�֣����ٹ���
                {
                    if((strMoto2Info.uiOldMotorPwmValue - uiPwmYDelt * strSysInfo.uiControlPareOfKp) >= strSysInfo.uiControlMinYDriverPwm)
                    {
                        iTempPwmY = strMoto2Info.uiOldMotorPwmValue - uiPwmYDelt * strSysInfo.uiControlPareOfKp; //�������Ĺ�С
                    }
                    else
                    {
                        iTempPwmY = 0;
                        //break;
                    }
                    if((strMoto2Info.uiOldMotorPwmValue - uiPwmYDelt * strSysInfo.uiControlPareOfKp) >= strSysInfo.uiControlMaxYDriverPwm)
                    {
                        iTempPwmY = strSysInfo.uiControlMaxYDriverPwm; //���PWM����
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
                        if(iTempPwmX > iTempPwmY)//Ҫ��X>Y
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
                        else//���X<Y
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
                        if(iTempPwmY > iTempPwmX)//Ҫ��Y>X
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
                        else//���Y<X
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
                vDelay30usAcc(uiAccTime);//���ٶ�20131206
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
            //            if(SysNoDirection == ucCalcDirection())//������˶��Ӽ��ٵ�ʱ����ҡ�˸�λ
            if((SysNoDirection == ucCalcDirection()) || (strSysInfo.uiCalcSysShowPower < CheckLowAlarmVoltage)) //�������λ��͵�ѹʱ�˳� ����ɲ��
            {
                //��ȥ���������٣�����ڼ��ٵĹ��������м��ٿ��Լ����ӵ�
                vDownSpeedCalcPwm(TRUE);//����������ȥ����ɲ�� �㷨���д����� �ֲ��������ڵ�
                vSendSingleOrder(BikeOutBack);
                if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-ɲ���������0-פ�³����ɲ������6�����20190110
                {
                    ucOpenBrakeFlag = FALSE;//�����ɲ���������ٴ���ת���ź� ���ܻ���ɲ�����رյ����  �����������Ҫ�ٴ��ж�
                }
                break;
            }
            iDeltPwmX = strSysInfo.uiRemoteNowXPos - uiReMoteXTemp;
            iDeltPwmY = strSysInfo.uiRemoteNowYPos - uiReMoteYTemp;
            if(iDeltPwmX > 0);//�����������ң���ƶ���ʱ�� ��ʱ�����´���
            else iDeltPwmX = 0 - iDeltPwmX;
            if(iDeltPwmY > 0);
            else iDeltPwmY = 0 - iDeltPwmY;
            if((iDeltPwmX > strSysInfo.uiRemoteXP_Delt) || (iDeltPwmY > strSysInfo.uiRemoteYP_Delt))
            {
                break;//�����ҡ�˲�ֵ�ϴ󣬾�ȥ�˳�
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
����Դ�ϵ簴��
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
    else//��� �ڿ�����ʱ�� û��һ���ƶ���ģʽ����ָ��һ��Ψһ�ļĴ���ֵ
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
ϵͳ���Զ��ػ���ʱ
*/
//unsigned short usAdcMotoATime1CCR1 = 0;
//unsigned short usAdcMotoATime1CCR3 = 0;
//unsigned short usAdcMotoBTime4CCR1 = 0;
//unsigned short usAdcMotoBTime4CCR3 = 0;
//unsigned char usMotoATime1CCR1_ChiarInStopModeCnt = 0;
//unsigned char usMotoATime1CCR3_ChiarInStopModeCnt = 0;
//unsigned char usMotoBTime4CCR1_ChiarInStopModeCnt = 0;
//unsigned char usMotoBTime4CCR3_ChiarInStopModeCnt = 0;//������м�������

//unsigned char usMotoATime1CCR1_ChiarBreakCnt = 0;
//unsigned char usMotoATime1CCR3_ChiarBreakCnt = 0;
//unsigned char usMotoBTime4CCR1_ChiarBreakCnt = 0;
//unsigned char usMotoBTime4CCR3_ChiarBreakCnt = 0;//������߼�����
#define PowerDownValue 225//�����2v
#define MaxWaitTime               6000//180000    //   180000 3���ӹػ�   40��ػ�  6���� ��
void vAutoPowerOffTimeFlag(void)
{
#define  MotoInRunJudeDeltAdc      250//����ڻ������µ�ʱ��ADC��ֵ
#define  MotoInIdelAdcBaseAdc      790
#define  MotoInBreakJudeAdc         3600//������߼��ADC
#define CheckInStopTimes               1    //�����ʱ��Ϊ600xCheckInStopTimes = ms
#define CheckBreakTimes                5    //�����ʱ��Ϊ600xCheckBreakTimes = ms
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
    unsigned char usMotoBTime4CCR3_ChiarInStopModeCnt = 0;//������м�������

    unsigned char usMotoATime1CCR1_ChiarBreakCnt = 0;
    unsigned char usMotoATime1CCR3_ChiarBreakCnt = 0;
    unsigned char usMotoBTime4CCR1_ChiarBreakCnt = 0;
    unsigned char usMotoBTime4CCR3_ChiarBreakCnt = 0;//������߼�����
    vDownSpeedCalcPwm(TRUE);
    //  vSendSingleOrder(QuitErro);
    if(ucMingRuiErrConst)ucErroType = ErroNoBearker;//���
    g_uiTimeNumCont1ms = 60;
    while(g_uiTimeNumCont1ms);
    vSendSingleOrder(OrderPowerOff);//�ػ�
    g_uiTimeNumCont1ms = 60;
    while(g_uiTimeNumCont1ms);
    C_EnResetStm8;
    CtlSetMotor1LeftPwmPercent(0);
    CtlSetMotor1RightPwmPercent(0);
    CtlSetMotor2LeftPwmPercent(0);
    CtlSetMotor2RightPwmPercent(0);
    if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)ucErroType = ErroNoBearker;//����е���ɲ��ֱ���˳�
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
        if(300 == usTime1CCR1Cnt )//���������ڵ���
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
            ucErroType = ErroNoBearker;//���
        }
        if((usMotoATime1CCR1_ChiarBreakCnt >= CheckBreakTimes) && \
                (usMotoATime1CCR3_ChiarBreakCnt >= CheckBreakTimes))
        {
            usMotoATime1CCR3_ChiarBreakCnt = usMotoATime1CCR3_ChiarBreakCnt;
            ucErroType = ErroNoBearker;//���
        }
        if((usMotoBTime4CCR1_ChiarBreakCnt >= CheckBreakTimes) && \
                (usMotoBTime4CCR3_ChiarBreakCnt >= CheckBreakTimes))
        {
            usMotoBTime4CCR3_ChiarBreakCnt = usMotoBTime4CCR3_ChiarBreakCnt;
            ucErroType = ErroNoBearker;//���
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

        vSendSysPower(strSysInfo.uiSysTemPower + 20);//�״�ָʾ�µ���
        g_uiTimeNumCont1ms = 60;
        while(g_uiTimeNumCont1ms);

        vSendSysPower(strSysInfo.uiSysTemPower - 20);//�״�ָʾ�µ���
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

    uiOldPwerValue = strSysInfo.uiSysTemPower;//��û�������̵���ǰ �ĵ���ֵ�ݴ���
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
        }//��������½�2v����
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
    uiOldPwerValue = strSysInfo.uiSysTemPower;//��û�������̵���ǰ �ĵ���ֵ�ݴ���
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
         }//��������½�2v����
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
    vConfigSysInStandbyMode();//��û��д��
}
/*
��ADCһЩADֵ��һЩ��ֵ���˲�����
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
        p->usTempData = vDmaContAdcVaule[ucTemp][AdcCh];//DMAֱ�Ӳ���������
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
void vDealAdcVaule(void)//����һ������413us
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
    //ɲ����������ѹ���CH0
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
//    uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��


		AdVoltageSample_RemoteYAdcCh.uiTempData = ((AdVoltageSample_RemoteYAdcCh.uiTempData - AdVoltageSample_RemoteYAdcCh.usMaxData - AdVoltageSample_RemoteYAdcCh.usMinData) /(OneChannelConvertTimes-2));
		AdVoltageSample_RemoteYAdcCh.uiTempData = (AdVoltageSample_RemoteYAdcCh.uiTempData >> 2); //�����ڶ�ʱ�������ĵ�ͨ�˲���,һ����Ҫ�ó˳���
    AdVoltageSample_RemoteYAdcCh.uiTempData += ((strSysInfo.uiRemoteNowYPos << 2) - strSysInfo.uiRemoteNowYPos) >> 2; // As C = S/4+(C*3)/4

    strSysInfo.uiRemoteNowYPos = AdVoltageSample_RemoteYAdcCh.uiTempData;


		AdVoltageSample_RemoteXAdcCh.uiTempData = ((AdVoltageSample_RemoteXAdcCh.uiTempData - AdVoltageSample_RemoteXAdcCh.usMaxData - AdVoltageSample_RemoteXAdcCh.usMinData) /(OneChannelConvertTimes-2));
		AdVoltageSample_RemoteXAdcCh.uiTempData = (AdVoltageSample_RemoteXAdcCh.uiTempData >> 2); //�����ڶ�ʱ�������ĵ�ͨ�˲���,һ����Ҫ�ó˳���
    AdVoltageSample_RemoteXAdcCh.uiTempData += ((strSysInfo.uiRemoteNowXPos << 2) - strSysInfo.uiRemoteNowXPos) >> 2; // As C = S/4+(C*3)/4
    strSysInfo.uiRemoteNowXPos = AdVoltageSample_RemoteXAdcCh.uiTempData;


 //   uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��
		AdVoltageSample_Motor1RightHBridgeVolHaveFilterCh.uiTempData = ((AdVoltageSample_Motor1RightHBridgeVolHaveFilterCh.uiTempData - AdVoltageSample_Motor1RightHBridgeVolHaveFilterCh.usMaxData - AdVoltageSample_Motor1RightHBridgeVolHaveFilterCh.usMinData) /(OneChannelConvertTimes-2));
    strMoto1Info.uiRightHBridgeHaveFilterAdc  = AdVoltageSample_Motor1RightHBridgeVolHaveFilterCh.uiTempData * 0.5 + strMoto1Info.uiRightHBridgeHaveFilterAdc * 0.5;

 //   uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��
		AdVoltageSample_Motor1LeftHBridgeVolHaveFilterCh.uiTempData = ((AdVoltageSample_Motor1LeftHBridgeVolHaveFilterCh.uiTempData - AdVoltageSample_Motor1LeftHBridgeVolHaveFilterCh.usMaxData - AdVoltageSample_Motor1LeftHBridgeVolHaveFilterCh.usMinData) /(OneChannelConvertTimes-2));
    strMoto1Info.uiLeftHBridgeHaveFilterAdc  = AdVoltageSample_Motor1LeftHBridgeVolHaveFilterCh.uiTempData * 0.5 + strMoto1Info.uiLeftHBridgeHaveFilterAdc * 0.5;

//    uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��

		AdVoltageSample_SysTemPowerAdcCh.uiTempData = ((AdVoltageSample_SysTemPowerAdcCh.uiTempData - AdVoltageSample_SysTemPowerAdcCh.usMaxData - AdVoltageSample_SysTemPowerAdcCh.usMinData) /(OneChannelConvertTimes-2));
		AdVoltageSample_SysTemPowerAdcCh.uiTempData = (AdVoltageSample_SysTemPowerAdcCh.uiTempData >> 2); //�����ڶ�ʱ�������ĵ�ͨ�˲���,һ����Ҫ�ó˳���
    AdVoltageSample_SysTemPowerAdcCh.uiTempData += ((strSysInfo.uiSysTemPower << 2) - strSysInfo.uiSysTemPower) >> 2; // As C = S/4+(C*3)/4
    strSysInfo.uiSysTemPower = AdVoltageSample_SysTemPowerAdcCh.uiTempData;

  //  uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��
		AdVoltageSample_Motor2LeftHBridgeVolHaveFilterCh.uiTempData = ((AdVoltageSample_Motor2LeftHBridgeVolHaveFilterCh.uiTempData - AdVoltageSample_Motor2LeftHBridgeVolHaveFilterCh.usMaxData - AdVoltageSample_Motor2LeftHBridgeVolHaveFilterCh.usMinData) /(OneChannelConvertTimes-2));
    strMoto2Info.uiLeftHBridgeHaveFilterAdc  = AdVoltageSample_Motor2LeftHBridgeVolHaveFilterCh.uiTempData * 0.5 + strMoto2Info.uiLeftHBridgeHaveFilterAdc * 0.5;

//    uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��
		AdVoltageSample_BreakFeedBackCh.uiTempData = ((AdVoltageSample_BreakFeedBackCh.uiTempData - AdVoltageSample_BreakFeedBackCh.usMaxData - AdVoltageSample_BreakFeedBackCh.usMinData) /(OneChannelConvertTimes-2));

    AdVoltageSample_BreakFeedBackCh.uiTempData = (AdVoltageSample_BreakFeedBackCh.uiTempData >> 2); //�����ڶ�ʱ�������ĵ�ͨ�˲���,һ����Ҫ�ó˳���
    AdVoltageSample_BreakFeedBackCh.uiTempData += ((strSysInfo.uiBreakFeedBackCurrent << 2) - strSysInfo.uiBreakFeedBackCurrent) >> 2; // As C = S/4+(C*3)/4
    strSysInfo.uiBreakFeedBackCurrent = AdVoltageSample_BreakFeedBackCh.uiTempData;

 //   uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��
		AdVoltageSample_Motor2RightHBridgeVolHaveFilterCh.uiTempData = ((AdVoltageSample_Motor2RightHBridgeVolHaveFilterCh.uiTempData - AdVoltageSample_Motor2RightHBridgeVolHaveFilterCh.usMaxData - AdVoltageSample_Motor2RightHBridgeVolHaveFilterCh.usMinData) /(OneChannelConvertTimes-2));
    strMoto2Info.uiRightHBridgeHaveFilterAdc  = AdVoltageSample_Motor2RightHBridgeVolHaveFilterCh.uiTempData * 0.5 + strMoto2Info.uiRightHBridgeHaveFilterAdc * 0.5;
    //////////////////////

 //   uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��
		AdVoltageSample_Motor2DriveCurrent.uiTempData = ((AdVoltageSample_Motor2DriveCurrent.uiTempData - AdVoltageSample_Motor2DriveCurrent.usMaxData - AdVoltageSample_Motor2DriveCurrent.usMinData) /(OneChannelConvertTimes-2));
    strMoto2Info.uiMotorCurrent = AdVoltageSample_Motor2DriveCurrent.uiTempData * 0.5 + strMoto2Info.uiMotorCurrent * 0.5;
    //////////////////////

  //  uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��
		AdVoltageSample_Motor1DriveCurrent.uiTempData = ((AdVoltageSample_Motor1DriveCurrent.uiTempData - AdVoltageSample_Motor1DriveCurrent.usMaxData - AdVoltageSample_Motor1DriveCurrent.usMinData) /(OneChannelConvertTimes-2));
    strMoto1Info.uiMotorCurrent = AdVoltageSample_Motor1DriveCurrent.uiTempData * 0.5 + strMoto1Info.uiMotorCurrent * 0.5;
}
*/
unsigned short usDebugX= 0;
unsigned short usDebugY= 0;

void vDealAdcVaule(void)//����һ������413us
{
    u16 usMaxData = 0, usMinData = 0xFFFF, usTempData;
    u32 uiTempData = 0;
    u8 ucTemp;
    //#define DividTimes 1
    //      		CtlTestLedON;
    //ɲ����������ѹ���CH0
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][RemoteYAdcCh];//DMAֱ�Ӳ���������  strSysInfo.uiRemoteNowYPos
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
    //uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��
    uiTempData = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    uiTempData = (uiTempData >> 2); //�����ڶ�ʱ�������ĵ�ͨ�˲���,һ����Ҫ�ó˳���
    uiTempData += ((strSysInfo.uiRemoteNowYPos << 2) - strSysInfo.uiRemoteNowYPos) >> 2; // As C = S/4+(C*3)/4
    strSysInfo.uiRemoteNowYPos = uiTempData;
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    //ϵͳ��ѹ����  CH1
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][RemoteXAdcCh];//DMAֱ�Ӳ���������
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
    //uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��


    uiTempData = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    uiTempData = (uiTempData >> 2); //�����ڶ�ʱ�������ĵ�ͨ�˲���,һ����Ҫ�ó˳���
    uiTempData += ((strSysInfo.uiRemoteNowXPos << 2) - strSysInfo.uiRemoteNowXPos) >> 2; // As C = S/4+(C*3)/4
    strSysInfo.uiRemoteNowXPos = uiTempData;
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    //����ң��X���ѹ���� CH2
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][Motor1RightHBridgeVolHaveFilterCh];//DMAֱ�Ӳ���������
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

    //uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��
    uiTempData = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    strMoto1Info.uiRightHBridgeHaveFilterAdc  = uiTempData * 0.5 + strMoto1Info.uiRightHBridgeHaveFilterAdc * 0.5;
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    //����ң��Y���ѹ���� CH3
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][Motor1LeftHBridgeVolHaveFilterCh];//DMAֱ�Ӳ���������
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
    //uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��
    uiTempData = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    strMoto1Info.uiLeftHBridgeHaveFilterAdc  = uiTempData * 0.5 + strMoto1Info.uiLeftHBridgeHaveFilterAdc * 0.5;
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    //���1������ѹ--������Ӳ���˲�  CH4
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][SysTemPowerAdcCh];//DMAֱ�Ӳ���������
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
    //uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��

    uiTempData = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    uiTempData = (uiTempData >> 2); //�����ڶ�ʱ�������ĵ�ͨ�˲���,һ����Ҫ�ó˳���
    uiTempData += ((strSysInfo.uiSysTemPower << 2) - strSysInfo.uiSysTemPower) >> 2; // As C = S/4+(C*3)/4
    strSysInfo.uiSysTemPower = uiTempData;
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    //���1������ѹ--������Ӳ���˲�  CH5
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][Motor2LeftHBridgeVolHaveFilterCh];//DMAֱ�Ӳ���������
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
    //uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��
    uiTempData = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    strMoto2Info.uiLeftHBridgeHaveFilterAdc  = uiTempData * 0.5 + strMoto2Info.uiLeftHBridgeHaveFilterAdc * 0.5;
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    //���2������ѹ--������Ӳ���˲�  CH6
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][BreakFeedBackCh];//DMAֱ�Ӳ���������
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
    //uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��
    uiTempData = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));

    uiTempData = (uiTempData >> 2); //�����ڶ�ʱ�������ĵ�ͨ�˲���,һ����Ҫ�ó˳���
    uiTempData += ((strSysInfo.uiBreakFeedBackCurrent << 2) - strSysInfo.uiBreakFeedBackCurrent) >> 2; // As C = S/4+(C*3)/4
    strSysInfo.uiBreakFeedBackCurrent = uiTempData;
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    //���2������ѹ--������Ӳ���˲�  CH7
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][Motor2RightHBridgeVolHaveFilterCh];//DMAֱ�Ӳ���������
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
    //uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��
    uiTempData = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    strMoto2Info.uiRightHBridgeHaveFilterAdc  = uiTempData * 0.5 + strMoto2Info.uiRightHBridgeHaveFilterAdc * 0.5;
    //////////////////////
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    ////PA0 ���2�ĵ������
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][Motor2DriveCurrent];//DMAֱ�Ӳ���������
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
    //uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��
    uiTempData = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    strMoto2Info.uiMotorCurrent = uiTempData * 0.5 + strMoto2Info.uiMotorCurrent * 0.5;
    //////////////////////
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    //PA5 ���1�ĵ������
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][Motor1DriveCurrent];//DMAֱ�Ӳ���������
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
    //uiTempData = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��
    uiTempData = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    strMoto1Info.uiMotorCurrent = uiTempData * 0.5 + strMoto1Info.uiMotorCurrent * 0.5;
}

extern unsigned char ucFirstStartOneTimeFlag;
void vCheckHardWareErro(void)//������˳�����°�����ֵ�������5ms
{
    if((ucFirstStartOneTimeFlag) && (strSysInfo.uiRemoteNowXPos < strSysInfo.uiRemoteErroMaxValue)
            && (strSysInfo.uiRemoteNowYPos < strSysInfo.uiRemoteErroMaxValue))
    {
        ucErroType = ErroNoRemote;//���ҡ��ֵ�Ĵ�
    }
    if(ReadKeyOfOn_OffFlag)
    {
        CtrlExtSpeekerDis;//����е�Դ���ذ�������ֹͣ���
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
    //							vShowErroToDis(ErroRelayShort);//����ڿ������������й����в����ĵ�����С��˵����������
    //						}
    //				    if((strMoto2Info.uiMotorCurrent < 500)||(strMoto2Info.uiMotorCurrent > 3500))
    //						{
    //							vShowErroToDis(ErroRelayShort);//����ڿ������������й����в����ĵ�����С��˵����������
    //						}
}
//��ʾ���״̬
void vShowPoweChange(void)
{
    vDownSpeedCalcPwm(TRUE);
    CtlSetMotor1RightPwmPercent(0);
    CtlSetMotor1LeftPwmPercent(0);
    CtlSetMotor2RightPwmPercent(0);
    CtlSetMotor2LeftPwmPercent(0);//�ڳ���ʱ���ͷ���������
    vSendSingleOrder(OrderInChange);
    while(ReadTeachPinIsConect)
    {
        ucTag10ms = FALSE;
        while(FALSE == ucTag10ms);//����ڴ˹������д��󼰰����������ȥ�ػ�
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
//�����й����м��Ӳ������
#define IdelModeCheckAdValue    60//�Լ���������ߵĵ�ѹ
#define RollingVoltage          23

//////////////////////////////////////////////�������궨������;

//�Զ�פ�¹���
//������������ӳ��� ��ƽ��ɲ���������ʱ��  ���Ӳ��ھ����Ƿ���Խ���ɲ������
//

#ifdef STM32
#define WaitNoneSampleSlopAd    20000  //���������ı궨
#endif

#ifdef GD32
#define WaitNoneSampleSlopAd    25000
#endif

#define NeedCurrentCloseLoop        //�Ե����ջ��Ƿ�����־
#define CloseLoopTime           500//390  һ���ջ��ļ���ʱ��



//�ڽ���פ�¹���ǰ���ڴ����ϴε�״̬   ��������һ��С�µ�������ô��
//�ڵȴ�ҡ�� �Ƿ������ĵ��ʱ���������һ�������ıջ�������ڴ��ڼ�û��һ������ֵ
//����������PWM�Ĵ����������������С��˵��������ƽ�ص�״̬�����һ��ҡ�˵��ͷ� ��ȥ�������Ķ���
//
/*
Ϊ��߲����ٶȣ����ﾡ������һЩ�˳��� ���㡣
Ҳֻ�����ؼ����ݣ�ҡ�˵��ź�Ϊ�˼�ʱ�˳�
������źż��������ź� ���ٸ��� ��ص�ѹ
*/
void vDealAdcVauleMotoAndCurrent(void)
{
    u16 usMaxData = 0, usMinData = 0xFFFF, usTempData;
    u32 uiTempData = 0;
    u8 ucTemp;
    //#define DividTimes 1
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][RemoteYAdcCh];//DMAֱ�Ӳ���������
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
    // strSysInfo.uiRemoteNowYPos = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��

    //    strSysInfo.uiRemoteNowYPos = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    //ϵͳ��ѹ����  CH1
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][RemoteXAdcCh];//DMAֱ�Ӳ���������
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
    // strSysInfo.uiRemoteNowXPos = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��
    //    strSysInfo.uiRemoteNowXPos = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    usMaxData = 0;
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][Motor1RightHBridgeVolHaveFilterCh];//DMAֱ�Ӳ���������
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
    // strMoto1Info.uiRightHBridgeHaveFilterAdc = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��
    strMoto1Info.uiRightHBridgeHaveFilterAdc = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2)); //�����OneChannelConvertTimesһ����10�ź�ʹ��


    //////////////////////
    //����ң��Y���ѹ���� CH3
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][Motor1LeftHBridgeVolHaveFilterCh];//DMAֱ�Ӳ���������
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
    // strMoto1Info.uiLeftHBridgeHaveFilterAdc = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��
    strMoto1Info.uiLeftHBridgeHaveFilterAdc = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    //���1������ѹ--������Ӳ���˲�  CH5
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][Motor2LeftHBridgeVolHaveFilterCh];//DMAֱ�Ӳ���������
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
    //  strMoto2Info.uiLeftHBridgeHaveFilterAdc = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��
    strMoto2Info.uiLeftHBridgeHaveFilterAdc = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    //���2������ѹ--������Ӳ���˲�  CH7
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][Motor2RightHBridgeVolHaveFilterCh];//DMAֱ�Ӳ���������
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
    // strMoto2Info.uiRightHBridgeHaveFilterAdc = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��
    strMoto2Info.uiRightHBridgeHaveFilterAdc = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    //////////////////////
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    ////PA0 ���2�ĵ������
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][Motor2DriveCurrent];//DMAֱ�Ӳ���������
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
    //strMoto2Info.uiMotorCurrent = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��
    strMoto2Info.uiMotorCurrent = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));
    //////////////////////
    usMaxData = 0;
    usMinData = 0xFFFF;
    uiTempData = 0;
    //////////////////////
    //PA5 ���1�ĵ������
    for(ucTemp = 0; ucTemp < OneChannelConvertTimes; ucTemp++)
    {
        usTempData = vDmaContAdcVaule[ucTemp][Motor1DriveCurrent];//DMAֱ�Ӳ���������
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
    //strMoto1Info.uiMotorCurrent = (uiTempData - usMaxData - usMinData) >> DividTimes;//�����OneChannelConvertTimesһ����10�ź�ʹ��
    strMoto1Info.uiMotorCurrent = ((uiTempData - usMaxData - usMinData) / (OneChannelConvertTimes - 2));

}




//
//��ͣ�µ�ʱ�� �����γ��Ĵ���������
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
    p->fVoltageSample = 0;//���ad����������� ƽ��ֵ
    p->uiReadVoltageTime = 0;

    for(uiTimeCont = 0; uiTimeCont < BigWheelJudgeMode_SetPwmHighTime; uiTimeCont++)
    {
        p->fVoltage[uiTimeCont] = 0;
    }
    p->uiSetPwmTimerCCR1_Flag = 0;
    p->uiSetPwmTimerCCR3_Flag = 0;//����PWM������
    p->fSetPwmStep = 0;//ÿһ������PWM��ֵ�Ķ���
    p->uiSampleAdJudgeMode_SetCnt = 0;
    p->uCheckMode_SetTime = 1;
    p->uiSampleAdSetPWMFinishFlag = 0;
    p->uiSampleAdSetIntoBreakCnt = 0;
    for(uiTimeCont = 0; uiTimeCont < CurrentReadTimes; uiTimeCont++)
    {
        p->iArryCurrentSample[uiTimeCont] = 0;//�����Ĳ�������
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
    unsigned char ucInSlopButDK_DisOff = FALSE;//�������פ���У�����Դ�����н��¡��Ͽ�ָʾ�Ʋ�����־
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
    if((FALSE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn) && (FALSE ==  g_ucLowPwmIntoBeadk))//��פ�³���汾
    {
        if((ReDefTIM1CCR1 >= 300) || (ReDefTIM1CCR3 >= 300) || (ReDefTIM4CCR1 >= 300) || (ReDefTIM4CCR3 >= 300)) //�����פ�µ�
        {
            for(uiTimeCont = 0 ; uiTimeCont < 380; uiTimeCont ++)
            {
                g_uiTimeNumCont1ms = 2;
                while(g_uiTimeNumCont1ms);
                if(SysNoDirection != ucCalcDirection())break;//����ڵȴ���ʱ���ɲ�����룬�����˳�����
            }
        }
        else
        {
            for(uiTimeCont = 0 ; uiTimeCont < 680; uiTimeCont ++)
            {
                g_uiTimeNumCont1ms = 2;
                while(g_uiTimeNumCont1ms);
                if(SysNoDirection != ucCalcDirection())break;//����ڵȴ���ʱ���ɲ�����룬�����˳�����
            }
        }
    }
    uiTimeCont = 0;
    //----------------------------------------------------���פ�¸���ֵ����----------------------------------------------------------/
    if(uniDiverInfo.strDirverAtt.ucBackMinSpeedP >= MaxSetSlopPWM)
    {
        uniDiverInfo.strDirverAtt.ucBackMinSpeedP = MaxSetSlopPWM;
    }
    MaxSlopPWM = uniDiverInfo.strDirverAtt.ucBackMinSpeedP;
    MaxSlopPWM = (unsigned int)(MaxSlopPWM * 36); //*3600/100
    //----------------------------------------------------���PWM����ֵ����----------------------------------------------------------/
    if(ReDefTIM1CCR1 > 50)//��ÿ�������PWM �ĸ�ֵ����
    {
        if(ReDefTIM1CCR1 > MaxSlopPWM)//���ռ�ձ��޷�
        {
            ReDefTIM1CCR1 = ReDefTIM1CCR2 = MaxSlopPWM;
        }
        MotoASlopModeAdjust.fSetCCR1CCR2PwmValue = ReDefTIM1CCR1;
        MotoASlopModeAdjust.fReadOldCCR1CCR2PwmValue = MotoASlopModeAdjust.fSetCCR1CCR2PwmValue;
    }
    if(ReDefTIM1CCR3 > 50)
    {
        if(ReDefTIM1CCR3 > MaxSlopPWM)//���ռ�ձ��޷�
        {
            ReDefTIM1CCR3 = ReDefTIM1CCR4 = MaxSlopPWM;
        }
        MotoASlopModeAdjust.fSetCCR3CCR4PwmValue = ReDefTIM1CCR3;
        MotoASlopModeAdjust.fReadOldCCR3CCR4PwmValue = ReDefTIM1CCR3;
    }
    if(ReDefTIM4CCR1 > 50)
    {
        if(ReDefTIM4CCR1 > MaxSlopPWM)//���ռ�ձ��޷�
        {
            ReDefTIM4CCR1 = ReDefTIM4CCR2 = MaxSlopPWM;
        }
        MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue = ReDefTIM4CCR1;
        MotoBSlopModeAdjust.fReadOldCCR1CCR2PwmValue = MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue;
    }
    if(ReDefTIM4CCR3 > 50)
    {
        if(ReDefTIM4CCR3 > MaxSlopPWM)//���ռ�ձ��޷�
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
        if(Globle_Timer_1MS.Bits.TestSlopTime > 0)		//0.5MSִ��һ��
        {
            //C_MonitorSTM8PinXor;
            Globle_Timer_1MS.Bits.TestSlopTime = 0;


            //----------------------------------------------------���ϴ���----------------------------------------------------------/
            //	   (Moto1AStmPwmReg) > (Moto1BStmPwmReg) ? (Moto1AStmPwmReg) : (Moto1BStmPwmReg);
            if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bCheckMotorConnect)
            {
                ucCont_MotoBreak = 0;
                ucCont_MotoBreak = MotoABreakFlagOne | MotoABreakFlagTwo;
                if((ucJudgeMotoBreakResult & ucCont_MotoBreak) == ucCont_MotoBreak) //ȷ��һ��Ψһ�Ĺ���
                {
                    FaultCode.bit.MotoAHighBriageBreak = 1;
                }
                else
                {
                    FaultCode.bit.MotoAHighBriageBreak = 0;
                }
                ucCont_MotoBreak = 0;
                ucCont_MotoBreak = MotoBBreakFlagOne | MotoBBreakFlagTwo;
                if((ucJudgeMotoBreakResult & ucCont_MotoBreak) == ucCont_MotoBreak) //ȷ��һ��Ψһ�Ĺ���
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
                        ucShowErro = 0x03; //��ɲ��������
                        if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor1Motor2)ucShowErro = 0x05; //��ɲ��������
                    }
                }
                else if(C_ReadRightMotoBreak_Pin_Low)
                {
                    ClearErrTime = 0;
                    MotoErrTime++;
                    if(MotoErrTime >= Timer_400MS)
                    {
                        MotoErrTime	= 	0;
                        ucShowErro = 0x05;  //��ɲ��������
                        if(TRUE == uniDiverInfo.strDirverAtt.unMotorInfo.bits.bChangeMotor1Motor2)ucShowErro = 0x03;  //��ɲ��������
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

            //----------------------------------------------------��������----------------------------------------------------------/
            if(((ReDefTIM1CCR1) < TimerBase) && ((ReDefTIM1CCR3) < TimerBase) && ((ReDefTIM4CCR1) < TimerBase) && ((ReDefTIM4CCR3) < TimerBase))//���PWMС
            {
                uiUartTimeCont++;
                if((Timer_5MS - 1) == uiUartTimeCont % Timer_5MS)
                {
                    vCalcBataAsShow(FALSE);//������ڿ��е�ʱ��
                }
                if((Timer_400MS - 1) == uiUartTimeCont % Timer_400MS) //����ڿ��е�״̬�£������ʾ������������������ʱ��Ҫȥ������
                {
                    if(ReadTeachPinIsConect)//����ǳ��״̬
                    {
                        ucInChargePinLowCont++;
                        if(ucInChargePinLowCont >= InChargePinLowCont)
                        {
                            vSendSingleOrder(OrderWatchDogClose);//�ؼ�ؿ��Ź�
                            vShowPoweChange();//��ָʾ��ѭ��ָʾ��
                        }
                    }
                    else ucInChargePinLowCont = 0;
                    if(ucAnlyFree)vAnalyTeacherDataOfTime();//
                }
                if(uiUartTimeCont >= Timer_800MS)
                {
                    //----------------------------------------------------���פ�¸���ֵ����----------------------------------------------------------/
                    if(uniDiverInfo.strDirverAtt.ucBackMinSpeedP >= MaxSetSlopPWM)
                    {
                        uniDiverInfo.strDirverAtt.ucBackMinSpeedP = MaxSetSlopPWM;
                    }
                    MaxSlopPWM = uniDiverInfo.strDirverAtt.ucBackMinSpeedP;
                    MaxSlopPWM = (unsigned int)(MaxSlopPWM * 36); //*3600/100
                    //----------------------------------------------------���͵���ֵ----------------------------------------------------------/
                    if(ucShowErro == 0)
                    {
                        ClearErrTime = 0;
                        uiUartTimeCont = 0;
                    }
                    vSendSysPower(strSysInfo.uiCalcSysShowPower);//��ص���
                }
                if(uiUartTimeCont >= Timer_1S)
                {
                    //----------------------------------------------------���ͺ��˳����͵�����ϸ澯----------------------------------------------------------/
                    uiUartTimeCont = 0;
                    if(ClearErrTime >= Timer_3S)
                    {
                        ClearErrTime = 0;
                        ucShowErro &= 0xF8;
                        vSendSysPower(strSysInfo.uiSysTemPower + 15);//�ٴο�����ʱ�� �õ�����ָʾ��
                        vSendSysPower(strSysInfo.uiSysTemPower - 15);//�������Ͽ�����һ����Ч��ָʾ
                    }
                    else if(ClearErrTime >= Timer_2S)
                    {
                        C_DisEnResetStm8;//�����Ͽظ�λ
                        vSendSingleOrder(FirstOrderPowerOn);

                    }
                    else if(ClearErrTime >= Timer_1S)
                    {
                        vSendSingleOrder(QuitErro); //������ʾ����
                        C_EnResetStm8;//������帴λ
                    }
                    else
                    {
                        vSendOneByteOrder(OrderErro, ucShowErro);	//���Ͱ����壬������ʾ�澯
                    }
                }
            }
            //----------------------------------------------------�˳�פ�´���----------------------------------------------------------/
						
#ifdef dSysPowerHold
            if((SysNoDirection != ucCalcDirection()) && (ucShowErro == 0) && (FALSE == ucInSlopButDK_DisOff)) //||(ucErroType) )//������ڹص�״̬����ҡ�˲��ٶ���
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
                if((ReDefTIM1CCR1 > MaxSlopPWM) || (ReDefTIM1CCR1 < LowSlopPWM)) //���ռ�ձ��޷� ��Ϊ�˳�פ��ʱ���������ڼ���Ƿ��ﳵʱ������ǰ����������ߵ�ƽ
                {
                    ReDefTIM1CCR1 = ReDefTIM1CCR2 = 0;
                }
                if((ReDefTIM1CCR3 > MaxSlopPWM) || (ReDefTIM1CCR3 < LowSlopPWM)) //���ռ�ձ��޷�
                {
                    ReDefTIM1CCR3 = ReDefTIM1CCR4 = 0;
                }
                if((ReDefTIM4CCR1 > MaxSlopPWM) || (ReDefTIM4CCR1 < LowSlopPWM)) //���ռ�ձ��޷�
                {
                    ReDefTIM4CCR1 = ReDefTIM4CCR2 = 0;
                }
                if((ReDefTIM4CCR3 > MaxSlopPWM) || (ReDefTIM4CCR3 < LowSlopPWM)) //���ռ�ձ��޷�
                {
                    ReDefTIM4CCR3 = ReDefTIM4CCR4 = 0;
                }
                if((Moto1AStmPwmReg) || (Moto1BStmPwmReg))
                {
                    strMoto1Info.uiOldMotorPwmValue = (Moto1AStmPwmReg) > (Moto1BStmPwmReg) ? (Moto1AStmPwmReg) : (Moto1BStmPwmReg);
                    if(Moto1AStmPwmReg) strMoto1Info.unMotorOldDirection = Forward;
                    else strMoto1Info.unMotorOldDirection = Reverse;	  //���ϴε����PWM��ȷ�����ϴε����ת������
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
                    else strMoto2Info.unMotorOldDirection = Reverse;   //���ϴε����PWM��ȷ�����ϴε����ת������
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

            //---------------------------------------------------�йػ������£��ٹػ�ģ�鴦��----------------------------------------------------------/
#ifdef dSysPowerHold
            if(TRUE == ucInSlopButDK_DisOff)//���������ʾ�岻����ʾ�����PWM�Ƚ�С��ʱ���ȥ�ػ�
            {
                //---------------------------------------------30���ӹػ�--------------------------------------------------//
                ucInSlopPowerOffTimeCnt++;
                if(ucInSlopPowerOffTimeCnt >= Timer_30Min)//30���ӹػ�Timer_30Min
                {
                    ucInSlopPowerOffTimeCnt = Timer_30Min;
                    ReDefTIM1CCR1 = ReDefTIM1CCR2 = 0;
                    ReDefTIM1CCR3 = ReDefTIM1CCR4 = 0;
                    ReDefTIM4CCR1 = ReDefTIM4CCR2 = 0;
                    ReDefTIM4CCR3 = ReDefTIM4CCR4 = 0;
                    //											  ucErroType = ErroSysPowerKeyPress;//��ȥ�ػ�
                    //												return;

                }
                //---------------------------------------------ռ�ձ�С�͹ػ�--------------------------------------------------//
                //									if(((ReDefTIM1CCR1) < PowerOffSlopPWM) && ((ReDefTIM1CCR3) < PowerOffSlopPWM) && ((ReDefTIM4CCR1) < PowerOffSlopPWM) && ((ReDefTIM4CCR3) < PowerOffSlopPWM))//���PWMС
                //									{
                //											ucErroType = ErroSysPowerKeyPress;//��ȥ�ػ�
                //									}
            }
            else
            {
                ucInSlopPowerOffTimeCnt = 0;
            }
            if(ucErroType)//����д���������  ����й��ϵ�ʱ��
            {
                if(ErroSysPowerKeyPress == ucErroType)//������а�������
                {
                    if(((ReDefTIM1CCR1) <= MaxSlopPWM) && ((ReDefTIM1CCR3) <= MaxSlopPWM) && ((ReDefTIM4CCR1) <= MaxSlopPWM) && ((ReDefTIM4CCR3) <= MaxSlopPWM))
                    {

                        if(FALSE == ucInSlopButDK_DisOff)//��������а��¹ر��Ͽ�
                        {
                            if(((ReDefTIM1CCR1) < PowerOffSlopPWM) && ((ReDefTIM1CCR3) < PowerOffSlopPWM) && ((ReDefTIM4CCR1) < PowerOffSlopPWM) && ((ReDefTIM4CCR3) < PowerOffSlopPWM))
                            {
                                return;//���PWMС���˳�
                            }
                            else//���PWM�Ƚϴ󣬾Ͳ�Ҫ�ػ���
                            {
                                ucErroType = ErroNoErro;//���ùػ�
                                C_EnResetStm8;//������帴λ
                                ucInSlopButDK_DisOff = TRUE;//����岻����ʾ��־
                                ucInSlopButDK_DisOff |= 0x80;//��ֹ��ִ�е�һ���ж����
                                while(ReadKeyOfOn_OffFlag);//�ȴ�������̧��
                                g_uiTimeNumCont1ms = 200;
                                while(g_uiTimeNumCont1ms);//�ȴ�һЩʱ��
                                ucErroType = ErroNoErro;//�尴����־
                            }
                        }
                        else if(TRUE == ucInSlopButDK_DisOff)//�ٴ����Ͽ�����ʾ
                        {
                            ucErroType = ErroNoErro;//���ùػ�
                            C_DisEnResetStm8;//�����Ͽظ�λ
                            ucInSlopButDK_DisOff = FALSE;//��־λ���
                            vSendSingleOrder(FirstOrderPowerOn);//�����Ͽ��ϵ�
                            g_uiTimeNumCont1ms = 300;
                            while(g_uiTimeNumCont1ms);//��ͣһ��ʱ��
                            vSendSysPower(strSysInfo.uiSysTemPower + 15);//�ٴο�����ʱ�� �õ�����ָʾ��
                            vSendSysPower(strSysInfo.uiSysTemPower - 15);//�������Ͽ�����һ����Ч��ָʾ
                            while(ReadKeyOfOn_OffFlag);
                            g_uiTimeNumCont1ms = 200;//�ȴ�200ms�ð�����һ���ȶ�ʱ��
                            while(g_uiTimeNumCont1ms);
                            g_uiTimeNumCont1ms = 1;
                            ucErroType = ErroNoErro;//��չػ���־
                        }
                        if(ucInSlopButDK_DisOff & 0x80)ucInSlopButDK_DisOff &= 0x0F;//��ռλ��Ϣ��յ�
                    }

                }
                else
                {
                    return;
                }
            }
#endif
            //----------------------------------------------------ɲ��������----------------------------------------------------------/
            if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-ɲ���������0-פ�³����ɲ������6�����20190110
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
                //-----------------------------------------------------פ�´���----------------------------------------------------------/

                MotoASlopModeAdjust.uiSampleAdCnt++;
                MotoBSlopModeAdjust.uiSampleAdCnt++;
                MotoASlopModeAdjust.uiSetPwmTimerCCR1_Flag = 0;
                MotoBSlopModeAdjust.uiSetPwmTimerCCR1_Flag = 0;
                MotoASlopModeAdjust.uiSetPwmTimerCCR3_Flag = 0;
                MotoBSlopModeAdjust.uiSetPwmTimerCCR3_Flag = 0;//����PWM������
                MotoASlopModeAdjust.fSetPwmStep = 0;//ÿһ������PWM��ֵ�Ķ���
                MotoBSlopModeAdjust.fSetPwmStep = 0;//ÿһ������PWM��ֵ�Ķ���

                if(ReDefTIM1CCR1 > MinSlopPWM)//��ÿ�������PWM �ĸ�ֵ����
                {

                    if(ReDefTIM1CCR1 > MaxSlopPWM)//���ռ�ձ��޷�
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
                    if(ReDefTIM1CCR3 > MaxSlopPWM)//���ռ�ձ��޷�
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

                    if(ReDefTIM4CCR1 > MaxSlopPWM)//���ռ�ձ��޷�
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

                    if(ReDefTIM4CCR3 > MaxSlopPWM)//���ռ�ձ��޷�
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
                if(MotoASlopModeAdjust.uiSampleAdCnt >= MotoASlopModeAdjust.uiReadSampleAdTime) //�������
                {

                    //if(MotoASlopModeAdjust.uiSampleAdMode == SampleAdCheckMode)//����ģʽ
                    if((MotoASlopModeAdjust.uiSampleAdMode == SampleAdCheckMode) &&
                            ((MotoASlopModeAdjust.fSetCCR1CCR2PwmValue > UserSetSlopMarkPWM) ||
                             (MotoASlopModeAdjust.fSetCCR3CCR4PwmValue > UserSetSlopMarkPWM)))
                    {
                        if(MotoASlopModeAdjust.uiSampleAdJudgeModeFinishFlag == 0)
                        {
                            MotoASlopModeAdjust.iArryCurrentSample[0] = strMoto1Info.uiMotorCurrent;//�����Ĳ�������
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
                            SetTim1CC1andCCR2Reg(SetPwmPinHighReg, 0); //�õ粻ʹ��һ��
                            SetTim1CC3andCCR4Reg(0, 0); //�õ粻ʹ��һ��
                            if(MotoASlopModeAdjust.uiSampleAdCnt > MotoASlopModeAdjust.uiReadSampleAdTime)//1MS��һ��ֵ
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
                            SetTim1CC1andCCR2Reg(0, 0); //�õ粻ʹ��һ��
                            SetTim1CC3andCCR4Reg(SetPwmPinHighReg, 0); //�õ粻ʹ��һ��
                            if(MotoASlopModeAdjust.uiSampleAdCnt > MotoASlopModeAdjust.uiReadSampleAdTime)//1MS��һ��ֵ
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
                        if(MotoASlopModeAdjust.uiSampleAdCnt >= (MotoASlopModeAdjust.uiReadSampleAdTime + MotoASlopModeAdjust.uiSetHighTime)) //���300+1MS ���ߵ�������100+3MS
                        {
                            MotoASlopModeAdjust.uiSampleAdCnt = 0;
                            MotoASlopModeAdjust.fVoltageSample = MotoASlopModeAdjust.fVoltage[MotoASlopModeAdjust.uiSetHighTime - 1];		//���µ�һ��ֵ
                            if(MotoASlopModeAdjust.fSetCCR1CCR2PwmValue > 0)//��ǰ����CCR1CCR2
                            {
                                ReDefTIM1CCR1 = ReDefTIM1CCR2 = 0;
                                MotoASlopModeAdjust.fSetCCR3CCR4PwmValue = 0;
                                if(MotoASlopModeAdjust.uiSampleAdMode == SampleAdJudgeMode) //����ģʽ
                                {
                                    MotoASlopModeAdjust.fLastVoltageSample	 = 	MotoASlopModeAdjust.fVoltageSample;//MotoASlopModeAdjust.fVoltage[2];

                                    if((MotoASlopModeAdjust.fVoltageSample >= IfMotoASampleAdAtStep1Low) && (MotoASlopModeAdjust.fVoltageSample <= IfMotoASampleAdAtStep1High)) //�������
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
                                    else//��������
                                    {
																			  ucJudgeMotoBreakResult &= ~MotoABreakFlagOne;//20210608
                                        MotoASlopModeAdjust.uiSetHighTime = MotoASlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                        if(MotoASlopModeAdjust.fVoltageSample > IfMotoASampleAdAtStep1High)//��ѹ��ʱ��
                                        {

                                            MotoASlopModeAdjust.uiSampleAdJudgeModeFinishFlag = 0;
                                            MotoASlopModeAdjust.uiSampleAdJudgeModeCnt = 0;
                                            if(MotoASlopModeAdjust.fVoltageSample > IfMotoASampleAdAtStep2High)
                                            {
                                                if((MotoASlopModeAdjust.fSetCCR1CCR2PwmValue >= TestSlopPWM) && (MotoASlopModeAdjust.fSetCCR1CCR2PwmValue <= LowSlopPWM))
                                                {
                                                    if(MotoASlopModeAdjust.fSetCCR1CCR2PwmValue <= ResetSlopPWM)
                                                    {
                                                        if(MotoASlopModeAdjust.fVoltageSample > IfMotoSampleAdAtErroVoltage)//�������
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
                                            if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-ɲ���������0-פ�³����ɲ������6�����20190110
                                            {
                                                MotoASlopModeAdjust.fSetPwmStep = (-MotoASlopModeAdjust.fSetCCR1CCR2PwmValue * JudgeModebBreakEnDecPareFast);
                                            }
                                            else
                                                MotoASlopModeAdjust.fSetPwmStep = (-MotoASlopModeAdjust.fSetCCR1CCR2PwmValue * JudgeModeSetMotoDecPareFast);
#endif
                                            MotoASlopModeAdjust.uiSetPwmTimerCCR1_Flag = TRUE;
                                            MotoASlopModeAdjust.uiSetHighTime = MotoASlopModeAdjust.uiJudgeMode_SetPwmHighTime;

                                        }
                                        else if((MotoASlopModeAdjust.uCheckMode_ReadFlag == 0) && (MotoASlopModeAdjust.fVoltageSample < IfMotoASampleAdAtStep1Low)) //��ѹ��ʱ��
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
                                else  if(MotoASlopModeAdjust.uiSampleAdMode == SampleAdCheckMode)//����ģʽ
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
                                if(MotoASlopModeAdjust.fSetCCR1CCR2PwmValue >= MaxSlopPWM)//���ռ�ձ��޷�
                                {
                                    MotoASlopModeAdjust.fSetCCR1CCR2PwmValue = MaxSlopPWM;
                                }
                                if(MotoASlopModeAdjust.fSetCCR1CCR2PwmValue < ResetSlopPWM)//��Сռ�ձ��޷�
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
                            else if(MotoASlopModeAdjust.fSetCCR3CCR4PwmValue > 0)//����CCR3CCR4
                            {
                                ReDefTIM1CCR3 = ReDefTIM1CCR4 = 0;
                                MotoASlopModeAdjust.fSetCCR1CCR2PwmValue = 0;
                                MotoASlopModeAdjust.fLastVoltageSample	 = 	MotoASlopModeAdjust.fVoltageSample;

                                if(MotoASlopModeAdjust.uiSampleAdMode == SampleAdJudgeMode) //����ģʽ
                                {

                                    if((MotoASlopModeAdjust.fVoltageSample >= IfMotoASampleAdAtStep1Low) && (MotoASlopModeAdjust.fVoltageSample <= IfMotoASampleAdAtStep1High)) //�������
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
                                    else//��������
                                    {
																			  ucJudgeMotoBreakResult &= ~MotoABreakFlagTwo;//20210608
                                        MotoASlopModeAdjust.uiSetHighTime = MotoASlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                        if(MotoASlopModeAdjust.fVoltageSample > IfMotoASampleAdAtStep1High)//��ѹ��ʱ��
                                        {

                                            if(MotoASlopModeAdjust.fVoltageSample > IfMotoASampleAdAtStep2High)
                                            {
                                                if((MotoASlopModeAdjust.fSetCCR3CCR4PwmValue >= TestSlopPWM) && (MotoASlopModeAdjust.fSetCCR3CCR4PwmValue <= LowSlopPWM))
                                                {
                                                    if(MotoASlopModeAdjust.fSetCCR3CCR4PwmValue <= ResetSlopPWM)
                                                    {
                                                        if(MotoASlopModeAdjust.fVoltageSample > IfMotoSampleAdAtErroVoltage)//�������
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
                                            if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-ɲ���������0-פ�³����ɲ������6�����20190110
                                            {
                                                MotoASlopModeAdjust.fSetPwmStep = (-MotoASlopModeAdjust.fSetCCR3CCR4PwmValue * JudgeModebBreakEnDecPareFast);
                                            }
                                            else
                                                MotoASlopModeAdjust.fSetPwmStep = (-MotoASlopModeAdjust.fSetCCR3CCR4PwmValue * JudgeModeSetMotoDecPareFast);
#endif
                                            MotoASlopModeAdjust.uiSetPwmTimerCCR3_Flag = TRUE;
                                            MotoASlopModeAdjust.uiSetHighTime = MotoASlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                        }
                                        else if((MotoASlopModeAdjust.uCheckMode_ReadFlag == 0) && (MotoASlopModeAdjust.fVoltageSample < IfMotoASampleAdAtStep1Low))	//��ѹ��ʱ��
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
                                else  if(MotoASlopModeAdjust.uiSampleAdMode == SampleAdCheckMode)//����ģʽ
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
                                if(MotoASlopModeAdjust.fSetCCR3CCR4PwmValue >= MaxSlopPWM)//���ռ�ձ��޷�
                                {
                                    MotoASlopModeAdjust.fSetCCR3CCR4PwmValue = MaxSlopPWM;
                                }
                                if(MotoASlopModeAdjust.fSetCCR3CCR4PwmValue < ResetSlopPWM)//��Сռ�ձ��޷�
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

                            MotoASlopModeAdjust.iArryCurrentSample[1] = strMoto1Info.uiMotorCurrent;//�����Ĳ�������MotoASlopModeAdjust.iArryCurrentSample[0];

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
                if(MotoBSlopModeAdjust.uiSampleAdCnt >= MotoBSlopModeAdjust.uiReadSampleAdTime) //�������
                {

                    //if(MotoBSlopModeAdjust.uiSampleAdMode == SampleAdCheckMode)//����ģʽ
                    if((MotoBSlopModeAdjust.uiSampleAdMode == SampleAdCheckMode) &&
                            ((MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue > UserSetSlopMarkPWM) ||
                             (MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue > UserSetSlopMarkPWM)))
                    {
                        if(MotoBSlopModeAdjust.uiSampleAdJudgeModeFinishFlag == 0)
                        {
                            MotoBSlopModeAdjust.iArryCurrentSample[0] = strMoto2Info.uiMotorCurrent;//�����Ĳ�������
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
                            SetTim4CC1andCCR2Reg(SetPwmPinHighReg, 0); //�õ粻ʹ��һ��
                            SetTim4CC3andCCR4Reg(0, 0); //�õ粻ʹ��һ��
                            if(MotoBSlopModeAdjust.uiSampleAdCnt > MotoBSlopModeAdjust.uiReadSampleAdTime)//1MS��һ��ֵ
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
                            SetTim4CC1andCCR2Reg(0, 0); //�õ粻ʹ��һ��
                            SetTim4CC3andCCR4Reg(SetPwmPinHighReg, 0); //�õ粻ʹ��һ��
                            if(MotoBSlopModeAdjust.uiSampleAdCnt > MotoBSlopModeAdjust.uiReadSampleAdTime)//1MS��һ��ֵ
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
                        if(MotoBSlopModeAdjust.uiSampleAdCnt >= (MotoBSlopModeAdjust.uiReadSampleAdTime + MotoBSlopModeAdjust.uiSetHighTime)) //���300+1MS ���ߵ�������100+3MS
                        {
                            MotoBSlopModeAdjust.uiSampleAdCnt = 0;
                            MotoBSlopModeAdjust.fVoltageSample = MotoBSlopModeAdjust.fVoltage[MotoBSlopModeAdjust.uiSetHighTime - 1];		//���µ�һ��ֵ
                            if(MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue > 0)//��ǰ����CCR1CCR2
                            {
                                ReDefTIM4CCR1 = ReDefTIM4CCR2 = 0;
                                MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue = 0;
                                if(MotoBSlopModeAdjust.uiSampleAdMode == SampleAdJudgeMode) //����ģʽ
                                {
                                    MotoBSlopModeAdjust.fLastVoltageSample	 = 	MotoBSlopModeAdjust.fVoltageSample;//MotoBSlopModeAdjust.fVoltage[2];

                                    if((MotoBSlopModeAdjust.fVoltageSample >= IfMotoBSampleAdAtStep1Low) && (MotoBSlopModeAdjust.fVoltageSample <= IfMotoBSampleAdAtStep1High)) //�������
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
                                    else//��������
                                    {
																			  ucJudgeMotoBreakResult &= ~MotoBBreakFlagOne;
                                        MotoBSlopModeAdjust.uiSetHighTime = MotoBSlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                        if(MotoBSlopModeAdjust.fVoltageSample > IfMotoBSampleAdAtStep1High)//��ѹ��ʱ��
                                        {
                                            MotoBSlopModeAdjust.uiSampleAdJudgeModeFinishFlag = 0;
                                            MotoBSlopModeAdjust.uiSampleAdJudgeModeCnt = 0;
                                            if(MotoBSlopModeAdjust.fVoltageSample > IfMotoBSampleAdAtStep2High)
                                            {
                                                if((MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue >= TestSlopPWM) && (MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue <= LowSlopPWM))
                                                {
                                                    if(MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue <= ResetSlopPWM)
                                                    {
                                                        if(MotoBSlopModeAdjust.fVoltageSample > IfMotoSampleAdAtErroVoltage)//�������
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
                                            if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-ɲ���������0-פ�³����ɲ������6�����20190110
                                            {
                                                MotoBSlopModeAdjust.fSetPwmStep = (-MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue * JudgeModebBreakEnDecPareFast);
                                            }
                                            else
                                                MotoBSlopModeAdjust.fSetPwmStep = (-MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue * JudgeModeSetMotoDecPareFast);
#endif
                                            MotoBSlopModeAdjust.uiSetPwmTimerCCR1_Flag = TRUE;
                                            MotoBSlopModeAdjust.uiSetHighTime = MotoBSlopModeAdjust.uiJudgeMode_SetPwmHighTime;

                                        }
                                        else if((MotoBSlopModeAdjust.uCheckMode_ReadFlag == 0) && (MotoBSlopModeAdjust.fVoltageSample < IfMotoBSampleAdAtStep1Low)) //��ѹ��ʱ��
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
                                else  if(MotoBSlopModeAdjust.uiSampleAdMode == SampleAdCheckMode)//����ģʽ
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
                                if(MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue >= MaxSlopPWM)//���ռ�ձ��޷�
                                {
                                    MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue = MaxSlopPWM;
                                }
                                if(MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue < ResetSlopPWM)//��Сռ�ձ��޷�
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
                            else if(MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue > 0)//����CCR3CCR4
                            {
                                ReDefTIM4CCR3 = ReDefTIM4CCR4 = 0;
                                MotoBSlopModeAdjust.fSetCCR1CCR2PwmValue = 0;
                                MotoBSlopModeAdjust.fLastVoltageSample	 = 	MotoBSlopModeAdjust.fVoltageSample;

                                if(MotoBSlopModeAdjust.uiSampleAdMode == SampleAdJudgeMode) //����ģʽ
                                {

                                    if((MotoBSlopModeAdjust.fVoltageSample >= IfMotoBSampleAdAtStep1Low) && (MotoBSlopModeAdjust.fVoltageSample <= IfMotoBSampleAdAtStep1High)) //�������
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
                                    else//��������
                                    {
																			  ucJudgeMotoBreakResult &= ~MotoBBreakFlagTwo;//20210608
                                        MotoBSlopModeAdjust.uiSetHighTime = MotoBSlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                        if(MotoBSlopModeAdjust.fVoltageSample > IfMotoBSampleAdAtStep1High)//��ѹ��ʱ��
                                        {

                                            if(MotoBSlopModeAdjust.fVoltageSample > IfMotoASampleAdAtStep2High)
                                            {
                                                if((MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue >= TestSlopPWM) && (MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue <= LowSlopPWM))
                                                {
                                                    if(MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue <= ResetSlopPWM)
                                                    {
                                                        if(MotoBSlopModeAdjust.fVoltageSample > IfMotoSampleAdAtErroVoltage)//�������
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
                                            if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-ɲ���������0-פ�³����ɲ������6�����20190110
                                            {
                                                MotoBSlopModeAdjust.fSetPwmStep = (-MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue * JudgeModebBreakEnDecPareFast);
                                            }
                                            else
                                                MotoBSlopModeAdjust.fSetPwmStep = (-MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue * JudgeModeSetMotoDecPareFast);
#endif
                                            MotoBSlopModeAdjust.uiSetPwmTimerCCR3_Flag = TRUE;
                                            MotoBSlopModeAdjust.uiSetHighTime = MotoBSlopModeAdjust.uiJudgeMode_SetPwmHighTime;
                                        }
                                        else if((MotoBSlopModeAdjust.uCheckMode_ReadFlag == 0) && (MotoBSlopModeAdjust.fVoltageSample < IfMotoASampleAdAtStep1Low))	//��ѹ��ʱ��
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
                                else  if(MotoBSlopModeAdjust.uiSampleAdMode == SampleAdCheckMode)//����ģʽ
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
                                if(MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue >= MaxSlopPWM)//���ռ�ձ��޷�
                                {
                                    MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue = MaxSlopPWM;
                                }
                                if(MotoBSlopModeAdjust.fSetCCR3CCR4PwmValue < ResetSlopPWM)//��Сռ�ձ��޷�
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

                            MotoBSlopModeAdjust.iArryCurrentSample[1] = strMoto2Info.uiMotorCurrent;//�����Ĳ�������MotoBSlopModeAdjust.iArryCurrentSample[0];

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
        }	//ɲ��

    }
}


unsigned char ucCheckRemoteTime(unsigned int uiTimes)
{
    unsigned int uiTimeCont = 0;
    for(uiTimeCont = 0; uiTimeCont < 200; uiTimeCont++)
    {
        g_uiTimeNumCont1ms = 1;//��ʱһ��
        while(g_uiTimeNumCont1ms)
        {
            vDealAdcVauleMotoAndCurrent();
            if((SysNoDirection != ucCalcDirection()) || ucErroType)//����д���������
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

void vAutoSlopeStopFuction(void)//�Զ�פ�¹��ܵĴ�����
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
