



#ifndef __DIRVERCONST_H_
#define __DIRVERCONST_H_//f1404z
#include "DriveBsp.h"
#include "DriverUart.h"
#include "DriverADC.h"
#include "DriverTimer.h"
#include "ControlUsedArry.h"
#include "Mlx90393.h"
//2020-02-21
#define MaxSlopPwm   590//最大驻坡给定的PWM,这里跟据客户的不同所需要的，尽量减小此值，以降低风险
//#define MaxSlopPwm   590//最大驻坡给定的PWM,这里跟据客户的不同所需要的，尽量减小此值，以降低风险
#define STM32  //AT32F421
//#define GD32
//#define HK32
//#define AutoBattIdenConfigDef  //是否使用电池类型识别

///////////////////////////////////////////////////////////////////////////////////////////////
#define UsedLinSuanTieLi    //如果使用磷酸铁锂电--如果是三元锂电。要注释掉                ///@@@@
//#define UsedYingKe50A       //英科50A控制器                                               ///@@@@
#define UartTxOdMode        //串口发送是开漏模式                                          ///@@@@
#define AutoPowerOffTime   20//无操作自动关机时间 单位为分钟                             ///@@@@
///////////////////////////////////////////////////////////////////////////////////////////////


#define UseFuctionBreakLin // 如果是定义二元曲线
#define BreakTabUsed_Ram

#define UsedOurSelfRemote
#define WriteEepromData  0x55AA
#ifdef UsedOurSelfRemote
#define FactoryRemoteMidXPos  2115
#define FactoryRemoteMaxXPos  3810
#define FactoryRemoteMinXPos  410

#define FactoryRemoteMidYPos  2125
#define FactoryRemoteMaxYPos  3830
#define FactoryRemoteMinYPos  420//Control Board
#endif

#define ReDefTIM1CCR1      TMR1->CC1
#define ReDefTIM1CCR2      TMR1->CC2
#define ReDefTIM1CCR3      TMR1->CC3
#define ReDefTIM1CCR4      TMR1->CC4

#define ReDefTIM4CCR1      TMR3->CC1
#define ReDefTIM4CCR2      TMR3->CC2

#define ReDefTIM4CCR3      TMR15->CC1
#define ReDefTIM4CCR4      TMR15->CC2
//硬件电路检查宏定义
//#define ChcekMotorConnect                    	0x0001
//#define ChcekMotorBreakConnect           			0x0002//检测刹车系统
#define CheckrRemoteContolorConnect   	      0x0004//检查遥杆
#define CheckRelayShortCircuit               	0x0008//检测继电器短路情况
#define CheckPowerVoltage                    	0x0010//检测系统电源是否正常

//Is GPIOA Be Used


#define DriverPowerContolPort   				GPIOF
#define DriverPowerContolPin    				GPIO_Pins_7//驱动继电器控制    主电源

#define SysInfoOutUartTxPort    				GPIOA
#define SysInfoOutUartTxPin     				GPIO_Pins_12//硬件电路以TTL方式输出一些信息

#define PowerOnTestPort         				  GPIOF
#define PowerOnTestPin          			    GPIO_Pins_6//飞车测试使用到的引脚 

//Is GPIOB Be Used
#define LeftMotorCurrentSensePort    			GPIOB//PB0
#define LeftMotorCurrentSensePin     			GPIO_Pins_0//左电机电流采样

#define RightMotorCurrentSensePort   			GPIOB//PB1
#define RightMotorCurrentSensePin    			GPIO_Pins_1//右电机电流采样

#define ResetStm8Port    GPIOC
#define ResetStm8Pin     GPIO_Pins_14

#define MonitorSTM8Port    GPIOC
#define MonitorSTM8Pin     GPIO_Pins_15

#define C_EnResetStm8       GPIO_ResetBits(ResetStm8Port,ResetStm8Pin)
#define C_DisEnResetStm8    GPIO_SetBits(ResetStm8Port,ResetStm8Pin)

#define C_MonitorSTM8PinHigh      GPIO_SetBits(MonitorSTM8Port,MonitorSTM8Pin)
#define C_MonitorSTM8PinLow       GPIO_ResetBits(MonitorSTM8Port,MonitorSTM8Pin)
#define C_MonitorSTM8PinXor        MonitorSTM8Port->OPTDT ^= MonitorSTM8Pin;//AT32
//#define C_MonitorSTM8PinXor     GPIO_PinsReverse(MonitorSTM8Port,MonitorSTM8Pin)

#define KeyOfOn_OffPort 					GPIOB
#define KeyOfOn_OffPin  					GPIO_Pins_2//开关机按钮

#define BrakeBikePort           					GPIOA
#define BrakeBikePin            					GPIO_Pins_15  //刹车信号控制接口

#define DriverOverCurrentPort   				GPIOB
#define DriverOverCurrentPin    				GPIO_Pins_7//输出电流过大检测

#define C_LeftMotoBreak_Port             				GPIOB//刹车器检测端口
#define C_LeftMotoBreak_Pin              				GPIO_Pins_8//刹车器检测端口

#define C_ReadLeftMotoBreak_Pin_Low               (0 == GPIO_ReadInputDataBit(C_LeftMotoBreak_Port,C_LeftMotoBreak_Pin))

#define C_RightMotoBreak_Port             				GPIOB//刹车器检测端口
#define C_RightMotoBreak_Pin              				GPIO_Pins_9//刹车器检测端口

#define C_ReadRightMotoBreak_Pin_Low              (0 == GPIO_ReadInputDataBit(C_RightMotoBreak_Port,C_RightMotoBreak_Pin))

#define UsartTxPort                      				   GPIOB
#define UsartTxPin                        				   GPIO_Pins_10

#define UsartRxPort                     				   GPIOB
#define UsartRxPin                       				   GPIO_Pins_11

#define ExPortSpeekerPort            	GPIOB
#define ExPortSpeekerPin             	GPIO_Pins_11// 外接高音喇叭 PB12

#define  TeachModeTestPort          	GPIOA
#define  TeachModeTestPin	            GPIO_Pins_12//当是，示教情况下的检测口

#define FeedDogPort                 	GPIOB
#define FeedDogPin                  	GPIO_Pins_14// 喂狗引脚 PB12


#define C_BreakControl_Port            	GPIOC
#define C_BreakControl_Pin             	GPIO_Pins_13//刹车器

#define C_DisBreakPort                  GPIO_ResetBits(C_BreakEnPort,C_BreakEnPin)

#define SetTeacherPortAsOutPut  {TeacherPort->CRH &=0xFFF0FFFF;TeacherPort->CRH |=0x00010000;};
#define SetTeacherPortAsInPut   {TeacherPort->CRH &=0xFFF0FFFF;TeacherPort->CRH |=0x00080000;};

/////////////////////////The Up Lists Show PA(x) Used Source
#define CtlDriverPowerContolON      	GPIO_SetBits(DriverPowerContolPort,DriverPowerContolPin)
#define CtlDriverPowerContolOFF     	GPIO_ResetBits(DriverPowerContolPort,DriverPowerContolPin)//驱动级，继电器PA12

#define CtlPowerOnTest_ON			    GPIO_SetBits(PowerOnTestPort,PowerOnTestPin)
#define CtlPowerOnTest_OFF          	GPIO_ResetBits(PowerOnTestPort,PowerOnTestPin)//置高位时，三极管有导通

//////////////////////////////////////////////////Up List are PB(x)s Be Used!
#define ReadKeyOfOn_OffFlag          (1 == GPIO_ReadInputDataBit(KeyOfOn_OffPort,KeyOfOn_OffPin))

#define CtlBrakeBikeEn   			GPIO_ResetBits(C_BreakControl_Port,C_BreakControl_Pin);
#define CtlBrakeBikeDisEn			GPIO_SetBits(C_BreakControl_Port,C_BreakControl_Pin)
#define CtlBrakeBikeXor       C_BreakControl_Port->ODR^=C_BreakControl_Pin;

#define ReadDriverOverCurrentFlag   	GPIO_ReadInputDataBit(DriverOverCurrentPort,DriverOverCurrentPin)//读出是否电流过载现象 电流过大的时候输出是 低电平 正常是 高 PB7


#define CtrlExtSpeekerEn              GPIO_SetBits(ExPortSpeekerPort,ExPortSpeekerPin)//外部喇叭
#define CtrlExtSpeekerDis             GPIO_ResetBits(ExPortSpeekerPort,ExPortSpeekerPin)//外部喇叭
#define CtrlExtSpeekerXor             GPIO_ReadOutputDataBit(ExPortSpeekerPort,ExPortSpeekerPin)?GPIO_ResetBits(ExPortSpeekerPort,ExPortSpeekerPin):GPIO_SetBits(ExPortSpeekerPort,ExPortSpeekerPin)

#define CtrlExtSpeekerEn_Debug              //GPIO_SetBits(ExPortSpeekerPort,ExPortSpeekerPin)//外部喇叭
#define CtrlExtSpeekerDis_Debug             //GPIO_ResetBits(ExPortSpeekerPort,ExPortSpeekerPin)//外部喇叭


#define ReadTeachPinIsConect		(0==GPIO_ReadInputDataBit(TeachModeTestPort,TeachModeTestPin))//读取是否在充电状态

#define FeedDogEn                     GPIO_ResetBits(FeedDogPort,FeedDogPin)// 喂狗引脚 PB14
#define FeedDogDisEn                  GPIO_SetBits(FeedDogPort,FeedDogPin)// 喂狗引脚 PB14

#define FeedDogToggle                 GPIO_ReadOutputDataBit(FeedDogPort,FeedDogPin)?GPIO_ResetBits(FeedDogPort,FeedDogPin):GPIO_SetBits(FeedDogPort,FeedDogPin)// 喂狗引脚 PB14


#define CtlSysTemWorkPowerHoldON    GPIO_SetBits(SysTemWorkPowerHoldPort,SysTemWorkPowerHoldPin)
#define CtlSysTemWorkPowerHoldOFF   GPIO_ResetBits(SysTemWorkPowerHoldPort,SysTemWorkPowerHoldPin)//系统工作电源
#define MotoADPort           GPIOA
#define MotoAd0Pin           GPIO_Pins_4
#define MotoAd1Pin           GPIO_Pins_7

#define SetMonitAdPortAsADin                    MotoADPort->CRL &= 0x0FF0FFFF;//set PA4 PA5 PA6 PA7 ADin
#define SetMonitAdPortAsAD0outAD1outAD2inAD3in  MotoADPort->CRL |= 0x30030000; //set PA4 PA5 out    PA6 PA7 Adin

#define SetMotoAd0AsHigh  GPIO_SetBits(MotoADPort,MotoAd0Pin)
#define SetMotoAd1AsHigh  GPIO_SetBits(MotoADPort,MotoAd1Pin)

#define SetMotoAd0AsLow  GPIO_ResetBits(MotoADPort,MotoAd0Pin)
#define SetMotoAd1AsLow  GPIO_ResetBits(MotoADPort,MotoAd1Pin)


#define SetMotoAd0Ad1AsHigh  {SetMotoAd0AsHigh;SetMotoAd1AsHigh;}
#define SetMotoAd0Ad1AsLow   {SetMotoAd0AsLow;SetMotoAd1AsLow;}

#define SetS_MotoA_A_AdcAsM1_Left_Port   GPIOB
#define SetS_MotoA_A_AdcAsM1_Left_Pin    GPIO_Pins_10

#define SetS_MotoA_B_AdcAsM1_Right_Port  GPIOB
#define SetS_MotoA_B_AdcAsM1_Right_Pin   GPIO_Pins_3

#define SetS_MotoB_A_AdcAsM2_Left_Port   GPIOB
#define SetS_MotoB_A_AdcAsM2_Left_Pin    GPIO_Pins_13

#define SetS_MotoB_B_AdcAsM2_Right_Port  GPIOA
#define SetS_MotoB_B_AdcAsM2_Right_Pin   GPIO_Pins_15

#define C_SetS_MotoA_A_AdcAsM1_Left_Port_High     GPIO_SetBits(SetS_MotoA_A_AdcAsM1_Left_Port,SetS_MotoA_A_AdcAsM1_Left_Pin)//
#define C_SetS_MotoA_A_AdcAsM1_Left_Port_Low      GPIO_ResetBits(SetS_MotoA_A_AdcAsM1_Left_Port,SetS_MotoA_A_AdcAsM1_Left_Pin)//

#define C_SetS_MotoA_B_AdcAsM1_Right_Port_High    GPIO_SetBits(SetS_MotoA_B_AdcAsM1_Right_Port,SetS_MotoA_B_AdcAsM1_Right_Pin)//
#define C_SetS_MotoA_B_AdcAsM1_Right_Port_Low     GPIO_ResetBits(SetS_MotoA_B_AdcAsM1_Right_Port,SetS_MotoA_B_AdcAsM1_Right_Pin)//

#define C_SetS_MotoB_A_AdcAsM2_Left_Port_High     GPIO_SetBits(SetS_MotoB_A_AdcAsM2_Left_Port,SetS_MotoB_A_AdcAsM2_Left_Pin)//
#define C_SetS_MotoB_A_AdcAsM2_Left_Port_Low      GPIO_ResetBits(SetS_MotoB_A_AdcAsM2_Left_Port,SetS_MotoB_A_AdcAsM2_Left_Pin)//

#define C_SetS_MotoB_B_AdcAsM2_Right_Port_High    GPIO_SetBits(SetS_MotoB_B_AdcAsM2_Right_Port,SetS_MotoB_B_AdcAsM2_Right_Pin)//
#define C_SetS_MotoB_B_AdcAsM2_Right_Port_Low     GPIO_ResetBits(SetS_MotoB_B_AdcAsM2_Right_Port,SetS_MotoB_B_AdcAsM2_Right_Pin)//

#define SetAd0Ad1AsAD_Mode {UsedAD0Port->CRL &= 0x0FFFFFFF}

#define IfSampleAdOverStep1AtFastMode 		(strMoto1Info.uiMotorVoltageBase+50)
#define IfSampleAdOverStep1AtHazardMode     (strMoto1Info.uiMotorVoltageBase+300)



///////////////////////////////////////////////////////////////////////////////////////////////////
extern void vDealAdcVauleMotoAndCurrent(void);//快速处理 ad的一些事情
extern void vDealAdcVaule(void);//处理ADC采样来的数据，做一次低频滤波等
extern void vInitPort(void);//对单片机io口的初始化工作
extern void vSysTemSourceAsActive(void);//对系统用到的资源的初始化
extern unsigned char ucPowerOnCotrol(void);//对上电工作的处理，主要是按键一块
extern void vDealReadProgramPin(void);//读取上电
extern void vCheckSystemInfo(void);//对系统一些功能模块的测试工作

extern void vContolRealize(void);//对控制的实现
extern unsigned char ucCalcDirection(void);//计算摇杆的控制方向
extern void vAutoPowerOffTimeFlag(void);//自动关机时间到的处理

extern void vCheckHardWareErro(void);//检查硬件错误
extern void vShowPoweChange(void);//显示充电状态

extern void vDelay30us(unsigned int uiDelayTime);
extern void vDelayms(unsigned int uiDelayTime);
extern void vDelaymsSoft(unsigned int uiDelayTime);
extern void vShowErroToDis(unsigned char ucErroNum);
extern void vConfigSysInStandbyMode(void);
extern void vNoKeyInStandbyMode(void);
extern void vReadSysTempPare(unsigned char ucReadEepromFlag);
extern void SysTick_Configuration(void);
extern void vDownSpeedCalcPwm(unsigned char ucFlag);
//extern void vSetMonitorKDPort(void);//?a??é???°′?ü°?μ??à??
extern void vInitWatchDog(unsigned int ucPrer, unsigned int uiRlr); //?a??μ￥???úμ??′??1·
extern void vFeedIwdg(void);//?1?àá￠μ??′??1·
extern void vInitPortAsWaiteMode(void);
extern void vCalcBataAsShow(unsigned char ucShowPowerInRunFlag);
extern void vSoftDelayms(unsigned int uiDelayTime);
extern unsigned char ucCrcUidAes(void);
extern void vAutoSlopeStopFuction(void);

extern void vAdjustPwmAtSlopMode(void);
extern unsigned int uiMotor1LeftHighPwmRegAdd ;
extern unsigned int uiMotor1RightHighPwmRegAdd ;
extern unsigned int uiMotor2LeftHighPwmRegAdd ;
extern unsigned int uiMotor2RightHighPwmRegAdd ;
extern unsigned char ucRec60Bytes[60];
extern unsigned char ucConnectBreakFlag;
extern float fShowPowerPare;
extern unsigned int uiOldBattPowerValue;
extern volatile unsigned char ucSpeedDowenFlag;
extern unsigned char ucAutoSlopOneTimeFlag;
extern unsigned char ucCalcBrakeCurrentResult;
extern unsigned char ucEnAdcCalc;
//系统工作状态宏定义
#define PowerDownMode    	0x01
#define IdleMode         	0x02
#define ErroMode         	0x03
#define RunMode          	0x04
#define SelfCheckMode    	0x05
#define SysInActive   		0x06
#define SysInStandby  		0x07

#define SysAsPowerChange  0x08
#define KeyPressDown      0x01
#define KeyPressUp        0x02
#define SysInAutoPowerOn  0x80

//系统中用到的全局变量
extern unsigned char ucStsTemState ;
extern unsigned char ucEmgStopEn;
extern unsigned char ucShowPowerCont;
extern unsigned char ucErroType;
extern unsigned char ucNewDirectCont, ucOldDirectCont;
extern unsigned char ucSpeedGrad;
extern unsigned char ucRunAsMode;//系统当前运行的状态
extern unsigned char ucPowerKeyPressTimes;
extern unsigned char  ucAnlyFree;
extern unsigned char ucLastErro;
extern unsigned char ucInChargePinLowCont;
#ifdef BreakTabUsed_Ram
	extern unsigned short uiBreakPwmTab[3605];//刹车曲线数组表，只有在开机的时候或者是参数改变的情况下才去计算一下的
#else
	extern const unsigned short uiBreakPwmTab[3605];//刹车曲线数组表，只有在开机的时候或者是参数改变的情况下才去计算一下的
#endif
//系统中所有错误情况的宏定义

#define ErroNoErro    		 0x00
#define ErroOverCurrent    0x01
#define ErroOverVoltage    0x02
#define ErroLowVoltage     0x03
#define ErroRightBeark     0x04
#define ErroLeftBeark		   0x05
#define ErroNoRemote     	 0x06
#define ErroRelayShort     0x07//继电器短路
#define ErroRelayBreak     0x08//继电器断路
#define ErroNoBearker      0x09
#define ErroMoto1Break	   0x0A//0x10
#define ErroMoto2Break     0x0B//0x11
#define ErroNoCheckSys     0xFE

#define ErroMoto1HighBriageShort   0x12//电机1的上桥短路了
#define ErroMoto2HighBriageShort   0x13//电机2的上桥短路了
#define ErroMoto1HighBriageBreak   0x14//电机1的上桥断路了
#define ErroMoto2HighBriageBreak   0x15//电机2的上桥断路了
#define ErroMoto1LowBriageShort    0x16//电机1的下桥短路了
#define ErroMoto2LowBriageShort    0x17//电机2的下桥短路了
#define ErroMoto1LowBriageBreak    0x18//电机1的下桥断路了
#define ErroMoto2LowBriageBreak    0x19//电机2的下桥断路了
#define ErroSysPowerOff            0x20//系统要求关机延时到
#define ErroSysPowerKeyPress       0x21
//计算出遥感方向的定义
#define SysNoDirection          	0x00
#define SysInForward           		0x20//0010 0000
#define SysInBack               	0x10//0001 0000
#define SysInLeft               	0x80//1000 0000
#define SysInRight              	0x40//0100 0000
#define SysInLeftForward     		0xA0//1010 0000
#define SysInRightForward    		0x60//0110 0000
#define SysInRightBack         	0x50//0101 0000
#define SysInLeftBack           0x90//1001 0000

#define JudgeLef   			0x80
#define JudgeRigt   		0x40
#define JudgeForward   	0x20
#define JudgeBack   		0x10

#define AutoBattIdenConfigFlag  0x80//电池自动识别配置过标志
#define AutoBattIdenConfigAsPb     0x01//电池自动识为铅酸电池
#define AutoBattIdenConfigAsLion  0x02//电池自动识为锂电池
extern unsigned char ucAutoBattIdenConfigData;//电池自动识别数据
//系统输入的速度刻度值
#define SpeedGrad1  0x01
#define SpeedGrad2  0x02
#define SpeedGrad3  0x03
#define SpeedGrad4  0x04
#define SpeedGrad5  0x05

#define QuadrantOne 		0x01
#define QuadrantYPostive 	0x02
#define QuadrantTwo 		0x03

#define IncSpeed         	0x01
#define DecSpeed           	0x02

#define NoDirection       	0x00
#define Forward           	0x01
#define Reverse           	0x02

#define KeyHoldTime 25//按键消抖时间
#define InChargePinLowCont  20//只有充电插头插入2秒的样子的时候 才去指示充电的

#define PowerOffTime (AutoPowerOffTime*60*1000)/10

#define PoweOffReleaseMosPWM    100//当停止关机的时候  开启放电的PWM值
#define CheckMotoTestPwmValue   0x190//200
//#define WhenLeftRightUpLowMosOpenWithTestPwmAsPowerPare   0.07//当检测电机以测试PWM值时，对应调试值的比例系数
//#define WhenLeftRightUpOpenMosLowCloseMosWithTestPwmAsPowerPare   0.1//当检测电机以测试PWM值时，对应调试值的比例系数

#define SampleValueAsTureVotageP   0.008865  //采样到的电压值与真实值的比例关系
#define AlarmVAsRun              19//报警电池的阈值  19
#define CheckLowAlarmVoltage    (AlarmVAsRun/SampleValueAsTureVotageP)//对应的AD值

#define SlopModeAlarmVAsRun           11//报警电池的阈值  19
#define SlopModeCheckLowAlarmVoltage  (SlopModeAlarmVAsRun/SampleValueAsTureVotageP)//对应的AD值

#define MotoABreakFlagOne         0x01
#define MotoABreakFlagTwo         0x02
#define MotoBBreakFlagOne         0x10
#define MotoBBreakFlagTwo         0x20

#define IfMotoSampleAdAtErroVoltage        3700//此值电机断线
#define dSysPowerHold       1 //假关机

#define SampleAdCheckMode   0x01
#define SampleAdJudgeMode   0x02
#define SampleAdZeroTest    0x04

//#define SampleAdJudgeModeFinishMode_SetPwmHighTime     4 //2MS

#define SampleAdJudgeFinishCheckMode_SetPwmHighTime     3 //1.5MS
#define SampleAdCheckMode_SetPwmHighTime      					2 //1MS
#define SampleAdZeroMode_SetPwmHighTime       					2 //1MS
#define SmallWheelJudgeMode_SetPwmHighTime      			  6 //2.5MS
#define BigWheelJudgeMode_SetPwmHighTime      					6 //3MS


//#define SampleAdJudgeFinishCheckMode_SetPwmHighTime     3 //1.5MS
//#define SampleAdCheckMode_SetPwmHighTime      					2 //1MS
//#define SampleAdZeroMode_SetPwmHighTime       					2 //1MS
//#define SmallWheelJudgeMode_SetPwmHighTime      			  6 //2.5MS
//#define BigWheelJudgeMode_SetPwmHighTime      					6 //3MS


#define SampleAdJudgeMode_FinishTime          2   //1次完成调节过程 电压值在730-930之间
#define SetPWMFinishCheckMode_SetTime         1    //完成后检测频率降低
#define SampleAdCheckMode_SetTime             1


#define SampleAdCheckMode_MotoATime           600  //300MS
#define SampleAdJudgeMode_MotoATime           200   //MS
#define SampleAdZeroMode_MotoATime            200

#define SampleAdCheckMode_MotoBTime       600  //MS
#define SampleAdJudgeMode_MotoBTime       200
#define SampleAdZeroMode_MotoBTime        200


//#define SetPwmStep1
#define SetPwmStep2
//#define SetPwmStep3


#ifdef SetPwmStep2
#define JudgeModebBreakEnDecPareFast       0.2  //电磁刹车程序参数
#define JudgeModeSetMotoDecPareFast        0.6   //驻坡程序参数
//0.6//0.75 此值与检测到的电压值有关
#define SmallWheelJudgeModeSetMotoDecPare  0.15
#define BigWheelJudgeModeSetMotoDecPare    0.2//0.15

#define JudgeModeSetMotoPlusPareFast       0.025// 0.1//0.15此值与检测到的电压值有关
#define JudgeModeSetMotoPlusPare           0.025//0.025//0.125 此值与检测到的电压值有关
#endif
#ifdef SetPwmStep3
#define JudgeModeSetMotoDecPareFast       0.7//0.75 此值与检测到的电压值有关
#define JudgeModeSetMotoDecPare           0.01//此值与检测到的电压值有关
#define JudgeModeSetMotoPlusPareFast      0.1//此值与检测到的电压值有关
#define JudgeModeSetMotoPlusPare          0.03//0.+1 此值与检测到的电压值有关
#endif
#ifdef SetPwmStep1
#define JudgeModeSetDecPareFast      MaxSlopPWM/2//此值与检测到的电压值有关
#endif

#define JudgeModeSetDecPare          6//6此值与检测到的电压值有关
#define JudgeModeSetPareFast         50//50  此值与检测到的电压值有关
#define JudgeModeSetPare             15//此值与检测到的电压值有关

#define LowSlopPWM          100//80
#define PowerOffSlopPWM     120//90//270//MaxSlopPWM/2//此值与检测到的电压值有关


#ifdef HK32
//#define CalSlopPWM                 950
//#define IntoSlopPWM                470//720
//#define UserSetSmallWheelSlopPWM   350
//#define UserSetBIGWheelSlopPWM     800
//#define UserSetSlopMarkPWM         180//500
//#define MaxSetSlopPWM              30

#define CalSlopPWM                 1000
#define IntoSlopPWM                400//720  原来500  2020-03-24
#define UserSetSmallWheelSlopPWM   350
#define UserSetBIGWheelSlopPWM     800
#define UserSetSlopMarkPWM         180//500  
#define MaxSetSlopPWM              30  //2020-02-24
#endif

#ifdef STM32
#define CalSlopPWM                 1000
#define IntoSlopPWM                400//720  原来500  2020-03-24
#define UserSetSmallWheelSlopPWM   350
#define UserSetBIGWheelSlopPWM     800
#define UserSetSlopMarkPWM         180//500  
#define MaxSetSlopPWM              30
#endif


#define ResetSlopPWM        20
#define MinSlopPWM          10
#define TestSlopPWM         10

#define IfMotoSampleAdAtStep4Low 0.5//0.0000001//0//



#ifdef STM32
//#define IfMotoASampleAdAtStep1High 		   (strMoto1Info.uiMotorVoltageBase + 22)
//#define IfMotoASampleAdAtStep2High 		   (strMoto1Info.uiMotorVoltageBase + 115)//调整灵敏度的阈值

//#define IfMotoBSampleAdAtStep1High 		   (strMoto2Info.uiMotorVoltageBase + 22) //30
//#define IfMotoBSampleAdAtStep2High 		   (strMoto2Info.uiMotorVoltageBase + 115)//  175  原200 lg_qiuzhi191216

#define IfMotoASampleAdAtStep1High 		   (strMoto1Info.uiMotorVoltageBase - 42)
#define IfMotoASampleAdAtStep2High 		   (strMoto1Info.uiMotorVoltageBase + 35)//调整灵敏度的阈值

#define IfMotoBSampleAdAtStep1High 		   (strMoto2Info.uiMotorVoltageBase - 42) //30
#define IfMotoBSampleAdAtStep2High 		   (strMoto2Info.uiMotorVoltageBase + 35)//  175  原200 lg_qiuzhi191216

//#define IfMotoASampleAdAtStep1Low 	 		   (strMoto1Info.uiMotorVoltageBase - 500)
//#define IfMotoASampleAdAtStep2Low 	 		   (strMoto1Info.uiMotorVoltageBase - 610)

//#define IfMotoBSampleAdAtStep1Low 	 		   (strMoto2Info.uiMotorVoltageBase - 500)//   195原200 lg_qiuzhi191216
//#define IfMotoBSampleAdAtStep2Low 	 		   (strMoto2Info.uiMotorVoltageBase - 610) //600

#define IfMotoASampleAdAtStep1Low 	 		   (strMoto1Info.uiMotorVoltageBase - 140)
#define IfMotoASampleAdAtStep2Low 	 		   (strMoto1Info.uiMotorVoltageBase - 410)

#define IfMotoBSampleAdAtStep1Low 	 		   (strMoto2Info.uiMotorVoltageBase - 140)//   195原200 lg_qiuzhi191216
#define IfMotoBSampleAdAtStep2Low 	 		   (strMoto2Info.uiMotorVoltageBase - 410) //600

#define IfMotoACurrentAtStep1High    			 (strMoto1Info.uiMotorCurrentBase + 170)
#define IfMotoBCurrentAtStep1High    			 (strMoto2Info.uiMotorCurrentBase + 175)

#define IfMotoACurrentAtStep2High    			 (strMoto1Info.uiMotorCurrentBase + 100)
#define IfMotoBCurrentAtStep2High    			 (strMoto2Info.uiMotorCurrentBase + 100)  //2020-02-23 之前
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define IfMotoASampleAdAtStep1High 		   (strMoto1Info.uiMotorVoltageBase + 30)
//#define IfMotoASampleAdAtStep2High 		   (strMoto1Info.uiMotorVoltageBase + 300)

//#define IfMotoASampleAdAtStep1Low 	 		   (strMoto1Info.uiMotorVoltageBase - 200)
//#define IfMotoASampleAdAtStep2Low 	 		   (strMoto1Info.uiMotorVoltageBase - 600)


//#define IfMotoBSampleAdAtStep1High 		   (strMoto2Info.uiMotorVoltageBase + 30)
//#define IfMotoBSampleAdAtStep2High 		   (strMoto2Info.uiMotorVoltageBase + 300)

//#define IfMotoBSampleAdAtStep1Low 	 		   (strMoto2Info.uiMotorVoltageBase - 200)
//#define IfMotoBSampleAdAtStep2Low 	 		  (strMoto2Info.uiMotorVoltageBase - 600)

//#define IfMotoACurrentAtStep1High    			 (strMoto1Info.uiMotorCurrentBase + 80)
//#define IfMotoBCurrentAtStep1High    			 (strMoto2Info.uiMotorCurrentBase + 80)

//#define IfMotoACurrentAtStep2High    			 (strMoto1Info.uiMotorCurrentBase + 20)
//#define IfMotoBCurrentAtStep2High    			 (strMoto2Info.uiMotorCurrentBase + 20)

//#define IfMotoASampleAdAtStep1High 		   (strMoto1Info.uiMotorVoltageBase + 30)
//#define IfMotoASampleAdAtStep2High 		   (strMoto1Info.uiMotorVoltageBase + 175)

//#define IfMotoASampleAdAtStep1Low 	 		   (strMoto1Info.uiMotorVoltageBase - 200)
//#define IfMotoASampleAdAtStep2Low 	 		   (strMoto1Info.uiMotorVoltageBase - 410)


//#define IfMotoBSampleAdAtStep1High 		   (strMoto2Info.uiMotorVoltageBase + 22) //30
//#define IfMotoBSampleAdAtStep2High 		   (strMoto2Info.uiMotorVoltageBase + 175)//  175  原200 lg_qiuzhi191216

//#define IfMotoBSampleAdAtStep1Low 	 		   (strMoto2Info.uiMotorVoltageBase - 130)//   195原200 lg_qiuzhi191216
//#define IfMotoBSampleAdAtStep2Low 	 		   (strMoto2Info.uiMotorVoltageBase - 410) //600

//#define IfMotoACurrentAtStep1High    			 (strMoto1Info.uiMotorCurrentBase + 80)
//#define IfMotoBCurrentAtStep1High    			 (strMoto2Info.uiMotorCurrentBase + 80)

//#define IfMotoACurrentAtStep2High    			 (strMoto1Info.uiMotorCurrentBase + 20)
//#define IfMotoBCurrentAtStep2High    			 (strMoto2Info.uiMotorCurrentBase + 20)
#endif


#define CurrentReadTimes 2
typedef 	struct
{
    unsigned char uiSampleAdMode;
    unsigned int uiSetHighTime;
    unsigned int uiReadSampleAdTime;

    unsigned int uiReadVoltageTime;	//
    double fVoltage[BigWheelJudgeMode_SetPwmHighTime];//电机ad采样计算变量 平均值
    float fVoltageSample;//电机ad采样计算变量 平均值
    float fLastVoltageSample;//电机ad采样计算变量 平均值
    float fSetPwmStep;//每一步减少PWM的值的定义

    unsigned char uiSetPwmTimerCCR1_Flag;
    unsigned char uiSetPwmTimerCCR3_Flag;//给定PWM计数器
    float fSetCCR1CCR2PwmValue;
    float fSetCCR3CCR4PwmValue;
    float fReadOldCCR1CCR2PwmValue;
    float fReadOldCCR3CCR4PwmValue;

    unsigned short uiSampleAdCnt;
    unsigned char  uiZeroTestPhase;
    unsigned char uCheckMode_SetTime; //测试模式电平低于730时或是高于900的值时，几次连续低进入调整模式

    unsigned char uiSampleAdSetPWMFinishFlag; //调整完成标志位
    unsigned char uiSampleAdSetIntoBreakCnt; //调整完成标志位
    unsigned char uiSampleAdSetPWMFinishCnt; //调整完成次数
    unsigned char uiSampleAdJudgeMode_SetCnt;	//测试模式检测是否进入调整模式次数

    unsigned char uiSampleAdJudgeModeCnt; //需要调节次数
    unsigned char uiSampleAdJudgeModeFinishFlag; //调整完成标志位
    int iArryCurrentSample[CurrentReadTimes];

    unsigned char uiPeopleStandUpFlag; //人站起来标志位
    int iCalCurrentSample;

    unsigned char uiJudgeMode_SetPwmHighTime;
    unsigned char uJudgeModeIntoCheckModeCnt; //需要调节次数
    //		unsigned int iMotoCurrentSample;
    unsigned char uCheckMode_ReadTime;
    unsigned char uCheckMode_ReadFlag;

} SLOPMODEADJUST_t;

#ifdef HK32
#define AdJudgeModeTimes          2
#define IntoJudgeModeCurrent       8//8//   //35
#define PlusIntoJudgeModeCurrent   6//6//
#define IntoBreakTimer          1 //完成调节次数
#endif
#ifdef STM32
#define AdJudgeModeTimes          2
#define IntoJudgeModeCurrent       12//8//   //35
#define PlusIntoJudgeModeCurrent   12//6//
#define IntoBreakTimer          1 //完成调节次数
#endif


#define Timer_5MS               10 //5MS
#define Timer_400MS             800  //400MS
#define Timer_800MS             1600  //800MS
#define Timer_1S           2000  //1S
#define Timer_2S           4000  //2S
#define Timer_3S           6000  //2S
#define Timer_5S           10000  //5S

#define Timer_2Min         240000
#define Timer_5Min         600000
#define Timer_10Min        1200000
#define Timer_20Min        2400000
#define Timer_30Min        3600000
//#define Timer_30Min        600000
typedef union
{
    unsigned short all;
    struct
    {
        unsigned short Ready: 1;     //伺服就绪  servo ready
        unsigned short Runing: 1;      //伺服运行  servo on
        unsigned short Slop: 1;     //驻坡模式
        unsigned short ZeroSpeed: 1;     //零速检出 zero speed
        unsigned short TargetSpeed: 1;	   //目标速度到达 target speed
        unsigned short TargetPosition: 1;     //目标位置到达 target position
        unsigned short TorqueLimit: 1;		//转矩限制中  torque limit
        unsigned short Alarm: 1;     //警告      alarm
        unsigned short BreakOutput: 1;     //制动输出 break
        unsigned short OverGateCurrent: 1;      //超过载门槛  over limit ?
        unsigned short Warn: 1;     //错误报?
        unsigned short CmdOK: 1;    //命令完成
        unsigned short CWLock: 1;   //反向堵转  cw lock
        unsigned short CCWLock: 1;  //正向堵转  ccw lock
        unsigned short NP: 1;	   //正反向
        unsigned short Zpulse: 1;	//z脉冲输出
    } bit;

} WORK_STATE_u;  //伺服工作状态

typedef union
{
    unsigned short all;
    struct
    {
        unsigned short OverCurrent: 1;     //过流
        unsigned short OverVoltage: 1;      //过压
        unsigned short LowVoltage: 1;     //欠压
        unsigned short MotoAHighBriageBreak: 1;     //MotoA上桥臂断路

        unsigned short MotoBHighBriageBreak: 1;	   //MotoB上桥臂断路
        unsigned short Resever: 11;     //

    } bit;

} FAULTCODE_u; //故障代码
extern FAULTCODE_u FaultCode;
extern WORK_STATE_u Work_Status;
extern SLOPMODEADJUST_t  MotoASlopModeAdjust;
extern SLOPMODEADJUST_t  MotoBSlopModeAdjust;

extern unsigned char ucMingRuiErrConst;
extern void PwmDataInit(void);

extern unsigned short usCheckRemoteTimeCnt;
#endif

