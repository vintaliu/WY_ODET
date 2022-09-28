


#ifndef __DIRVERADC_H_
#define __DIRVERADC_H_


#define OneChannelConvertTimes  10//每个通道转化次数
#define TotalADCChannels        10//总有AD通道数
extern volatile unsigned short vDmaContAdcVaule[OneChannelConvertTimes][TotalADCChannels];

struct strMotorInfo
{
    unsigned char unMotorOldDirection;//电机的转动方向
    unsigned int uiOldMotorPwmValue;//前一次电机的PWM值记录  这里只记百分比
    unsigned int uiNewMotorPwmValue;//电机最新的PWM  这里只记百分比
    unsigned int uiMotorCurrent;//当前电机的电流
    unsigned int uiMotorCurrentBase;//左右电机的电流基础
    //     unsigned int uiMotorSpeed;//电机当前的速度
    //     unsigned int uiMotorDriverCurrent;//电机驱动电流
    unsigned int uiMotorBreakDiv;//电机在刹车的时候 可以分的PWM档位
    unsigned int uiMotorBreakDistance;//要求刹车的距离
    unsigned int uiLeftHBridgeHaveFilterAdc;
    unsigned int uiRightHBridgeHaveFilterAdc;
	
	  unsigned int uiMotorVoltageBase;//左右电机的静止不动作电压基础
};
struct strSysTemInfo
{
    unsigned int uiSysTemPower;//系统供电电压值
    unsigned int uiRemoteMaxXPos;//标定出的遥感X最大值
    unsigned int uiRemoteMinXPos;//标定出的遥感X最小值
    unsigned int uiRemoteErroMaxValue;//充许摇杆的最大电压
    unsigned int uiRemoteErroMinValue;//充许摇杆的最小电压
    unsigned int uiRemoteMaxYPos;//标定出的遥感Y最大值
    unsigned int uiRemoteMinYPos;//标定出的遥感Y最小值
    unsigned int uiRemoteMiddleXPos;//标定出的遥感X中间值
    unsigned int uiRemoteMiddleYPos; //标定出的遥感Y中间值
    unsigned int uiRemoteNowXPos;//采样出当前X轴的值
    unsigned int uiRemoteNowYPos;//采样出当前Y轴的值

    unsigned int uiRemoteXP_Delt;//摇杆正方向的每个刻度的误差
    unsigned int uiRemoteXN_Delt;//摇杆负方向的每个刻度的误差
    unsigned int uiRemoteYP_Delt;//摇杆正方向的每个刻度的误差
    unsigned int uiRemoteYN_Delt;//摇杆负方向的每个刻度的误差

    unsigned int uiRemoteXP_First;//正方向上的第一个差值
    unsigned int uiRemoteXN_First;//负方向上的第一个差值
    unsigned int uiRemoteYP_First;//正方向上的第一个差值
    unsigned int uiRemoteYN_First;//负方向上的第一个差值

    unsigned int uiControlPareOfKp;
    unsigned int uiControlPareOfKd;
    unsigned int uiControlMinXDriverPwm;
    unsigned int uiControlMaxXDriverPwm;
    unsigned int uiControlMinYDriverPwm;
    unsigned int uiControlMaxYDriverPwm;
    unsigned int uiLastControlQuadrant;
    unsigned int uiLastRemotValue;
    unsigned int uiCalcSysShowPower;//计算出来的
    unsigned int uiBreakFeedBackCurrent;//刹车器反馈电流
		
	  unsigned int uiSysBreakFlag;//刹车时候标志位		
	  unsigned int uiCalcPwmInBreak;//计算出上次刹车时候 的PWM值 最大值
};

struct strBatterLowPowerCom//低电压的时候速度补偿
{
	unsigned char bitLowBarSetp1: 1;
	unsigned char bitLowBarSetp2: 1;
	unsigned char bitLowBarSetp3: 1;
	unsigned char bitLowBarSetp4: 1;
	unsigned char bitLowBarSetp5: 1;
};
union unioBatterLowPowerCom
{
	struct strBatterLowPowerCom bitSet;
	unsigned char ucAllData;
};
extern union unioBatterLowPowerCom g_unioBatterLowPowerCom;

#define FactoryPareKp  1
#define FactoryPareKd  1//加速时间

extern volatile struct strSysTemInfo strSysInfo;
extern volatile struct strMotorInfo strMoto1Info, strMoto2Info;

//AD通道的定义
#define RemoteYAdcCh                           0//PA1 遥感Y位置
#define RemoteXAdcCh                           1//PA2  遥感X位置
#define Motor1RightHBridgeVolHaveFilterCh      2//PA3 电机1右电压检测
#define Motor1LeftHBridgeVolHaveFilterCh  	   3//PA4  电机1左电压检测
#define SysTemPowerAdcCh  	                   4//PA6 系统电量检测
#define Motor2LeftHBridgeVolHaveFilterCh  	   5//PA7电机2左电压检测
#define BreakFeedBackCh  			                 6  //PB0 刹车器电压反馈
#define Motor2RightHBridgeVolHaveFilterCh      7  //PB1 电机2右电压检测
#define Motor2DriveCurrent                     8  //PA0 电机2的电流检测
#define Motor1DriveCurrent                     9  //PA5 电机1的电流检测


#define ClacXPwmFirstRemotePosP  (FactoryRemoteMaxXPos -FactoryRemoteMidXPos)/6
#define ClacXPwmFirstRemotePosN  (FactoryRemoteMidXPos -FactoryRemoteMinXPos)/6

#define ClacYPwmFirstRemotePosP  (FactoryRemoteMaxYPos -FactoryRemoteMidYPos)/6
#define ClacYPwmFirstRemotePosN  (FactoryRemoteMidYPos -FactoryRemoteMinYPos)/6

#define DriverYMotorStep 				5//电机分为16个等级的速度参数
#define DriverYRemoteAdcBase    strSysInfo.uiRemoteMiddleYPos
#define DriverXMotorStep 				5//电机分为16个等级的速度参数
#define DriverXRemoteAdcBase    strSysInfo.uiRemoteMiddleXPos

#define DriverXRemoteAdcDeltPositive     (strSysInfo.uiRemoteMaxXPos - ClacXPwmFirstRemotePosP - strSysInfo.uiRemoteMiddleXPos)>>DriverXMotorStep;
#define DriverXRemoteAdcDeltNegative     (strSysInfo.uiRemoteMiddleXPos - ClacXPwmFirstRemotePosN - strSysInfo.uiRemoteMinXPos)>>DriverXMotorStep;

#define DriverYRemoteAdcDeltPositive     (strSysInfo.uiRemoteMaxYPos - ClacYPwmFirstRemotePosP- strSysInfo.uiRemoteMiddleYPos)>>DriverYMotorStep;
#define DriverYRemoteAdcDeltNegative     (strSysInfo.uiRemoteMiddleYPos- ClacYPwmFirstRemotePosN- strSysInfo.uiRemoteMinYPos)>>DriverYMotorStep;

#define FactoryPareMinXPwm  8
#define FactoryPareMaxXPwm  98//这里调的是占空比

#define FactoryPareMinYPwm  8
#define FactoryPareMaxYPwm  98

#define FactoryPareMinXDriverPwm  (TimerBase*FactoryPareMinXPwm)/100
#define FactoryPareMaxXDriverPwm  (TimerBase*FactoryPareMaxXPwm)/100

#define FactoryPareMinYDriverPwm  (TimerBase*FactoryPareMinYPwm)/100
#define FactoryPareMaxYDriverPwm  (TimerBase*FactoryPareMaxYPwm)/100

#define XDriverPwmDelt   (strSysInfo.uiControlMaxXDriverPwm - strSysInfo.uiControlMinXDriverPwm)>>DriverXMotorStep;
#define YDriverPwmDelt   (strSysInfo.uiControlMaxYDriverPwm - strSysInfo.uiControlMinYDriverPwm)>>DriverYMotorStep;

#define MaxPowerAdcValue  	2876
#define MinPowerAdcValue   	2700//这里标定出的  23.74v
#define NoPowerOnAdcValue    MinPowerAdcValue/3//当系统没有上电的时候的ADC值
#define LowPowerVoltage        1989//17.5  如果电压低于这个值的话，不让开机并报错电压低
#define RunLowPower              LowPowerVoltage/3//如果在运行的时候电压过于底，要去报错的

#define NoBreakEnAdcValue       400
#define MosHighBriageOnAd       1500//300//当以0x170的PWM时采样ADC的最会值
#define DefaultFactryID       0 //1114010001
#define HaltMosMaxPower  2300//当停机的时候，mos管的最大值电压 
//  Vx =((x/4096)*3.3）*(8.2+33))/8.2  as x=2300 Vx = 9.3
#define BreadAdAndPowerP  0.645
#define BreakLeftBreakAdcP  0.533
#define BreakRightBreakAdcP  0.445
#define NoBreakAdcP  0.333
#define AllowAutoPowerMaxVol   3000//充许自动激活的最高电压   
#define AllowAutoPowerMinVol  400 //充许自动激活的小电压
#define PbBattlePare   8.836//铅酸电池的电压与strSysInfo.uiSysTemPower 关系
#define LowVotageAutoPowerOff   16500//16.5当电池电压 在电机无动作时候 如果检测到电池的电压过低于LowVotageAutoPowerOff 就自动关机


#define AutoLowPowerAlarmLilonVoltage    23 //如果电压低于此值时候的自动关机
#define AutoLowPowerAlarmLilonVoltageADC (unsigned int)(AutoLowPowerAlarmLilonVoltage/SampleValueAsTureVotageP)

#define AutoLowPowerAlarmQiansuanVoltage    17.5 //如果电压低于此值时候的自动关机
#define AutoLowPowerAlarmQiansuanVoltageADC (unsigned int)(AutoLowPowerAlarmQiansuanVoltage/SampleValueAsTureVotageP)

struct stucDataUI
{
    unsigned char ucData1: 8;
    unsigned char ucData2: 8;
};
struct ucstrBreakTime
{
    unsigned short data1: 8;
    unsigned short data2: 8;
};
union unDelayBreakTime
{
    unsigned short all;
    struct ucstrBreakTime bits;
};
union UINT//共用一个数据结构体
{
    unsigned int uiData;
    struct stucDataUI ucData;   //Data Type Define :If define ucXXXXX meaning "unsigned char" Data Type;
};
struct unMotorAttributeBits//电机属性一些信息
{
    unsigned char bChangeMotor1DirEn: 1; //0,不切换  1:切换电机1的正反转
    unsigned char bChangeMotor2DirEn: 1; //0,不切换  1:切换电机2的正反转
    unsigned char bChangeMotor1Motor2: 1; //0,不切换 1:掉换电机1、2
    unsigned char bBreakEn: 1;           //开启刹车器
    unsigned char bCheckMotorConnect: 1; //开机检查电机是否连接
    unsigned char bCheckBreakConnect: 1; //开机检查刹车器是否连接
    unsigned char bLilonOrQiansuan: 1; //铅酸还是锂电1是锂电
    unsigned char bEnAutoPowerOn: 1; //自动溜车功能的激活
};
union unMotorAttribute
{
    unsigned char all;
    struct unMotorAttributeBits bits;
};
#define DirverAttLength  26
struct strDriverInfo//驱动器动作信息
{
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
};

union uniDirverAtt//定义一个与电机一些特性有关的数据结构
{
    struct strDriverInfo strDirverAtt;//所有数据的操作
    unsigned char ucDirverAttDataBuff[DirverAttLength];//共用一个数组空间
};
extern union uniDirverAtt uniDiverInfo;

//typedef 	struct
//{
//    u16 usMaxData;	
//	  u16 usMinData;	 
//	  u16 usTempData;	
//    unsigned int uiTempData;	

//}AdVoltageSample_t;
//extern AdVoltageSample_t AdVoltageSample_RemoteYAdcCh;
//extern AdVoltageSample_t AdVoltageSample_RemoteXAdcCh;
//extern AdVoltageSample_t AdVoltageSample_Motor1RightHBridgeVolHaveFilterCh;
//extern AdVoltageSample_t AdVoltageSample_Motor1LeftHBridgeVolHaveFilterCh;
//extern AdVoltageSample_t AdVoltageSample_SysTemPowerAdcCh;

//extern AdVoltageSample_t AdVoltageSample_Motor2RightHBridgeVolHaveFilterCh;
//extern AdVoltageSample_t AdVoltageSample_Motor2LeftHBridgeVolHaveFilterCh;

//extern AdVoltageSample_t AdVoltageSample_Motor1DriveCurrent;
//extern AdVoltageSample_t AdVoltageSample_Motor2DriveCurrent;
//extern AdVoltageSample_t AdVoltageSample_BreakFeedBackCh;

#endif
