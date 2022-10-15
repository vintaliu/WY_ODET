



#ifndef __DIRVERCONST_H_
#define __DIRVERCONST_H_//f1404z
#include "DriveBsp.h"
#include "DriverUart.h"
#include "DriverADC.h"
#include "DriverTimer.h"
#include "ControlUsedArry.h"
#include "Mlx90393.h"
//2020-02-21
#define MaxSlopPwm   590//���פ�¸�����PWM,������ݿͻ��Ĳ�ͬ����Ҫ�ģ�������С��ֵ���Խ��ͷ���
//#define MaxSlopPwm   590//���פ�¸�����PWM,������ݿͻ��Ĳ�ͬ����Ҫ�ģ�������С��ֵ���Խ��ͷ���
#define STM32  //AT32F421
//#define GD32
//#define HK32
//#define AutoBattIdenConfigDef  //�Ƿ�ʹ�õ������ʶ��

///////////////////////////////////////////////////////////////////////////////////////////////
#define UsedLinSuanTieLi    //���ʹ��������﮵�--�������Ԫ﮵硣Ҫע�͵�                ///@@@@
//#define UsedYingKe50A       //Ӣ��50A������                                               ///@@@@
#define UartTxOdMode        //���ڷ����ǿ�©ģʽ                                          ///@@@@
#define AutoPowerOffTime   20//�޲����Զ��ػ�ʱ�� ��λΪ����                             ///@@@@
///////////////////////////////////////////////////////////////////////////////////////////////


#define UseFuctionBreakLin // ����Ƕ����Ԫ����
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
//Ӳ����·���궨��
//#define ChcekMotorConnect                    	0x0001
//#define ChcekMotorBreakConnect           			0x0002//���ɲ��ϵͳ
#define CheckrRemoteContolorConnect   	      0x0004//���ң��
#define CheckRelayShortCircuit               	0x0008//���̵�����·���
#define CheckPowerVoltage                    	0x0010//���ϵͳ��Դ�Ƿ�����

//Is GPIOA Be Used


#define DriverPowerContolPort   				GPIOF
#define DriverPowerContolPin    				GPIO_Pins_7//�����̵�������    ����Դ

#define SysInfoOutUartTxPort    				GPIOA
#define SysInfoOutUartTxPin     				GPIO_Pins_12//Ӳ����·��TTL��ʽ���һЩ��Ϣ

#define PowerOnTestPort         				  GPIOF
#define PowerOnTestPin          			    GPIO_Pins_6//�ɳ�����ʹ�õ������� 

//Is GPIOB Be Used
#define LeftMotorCurrentSensePort    			GPIOB//PB0
#define LeftMotorCurrentSensePin     			GPIO_Pins_0//������������

#define RightMotorCurrentSensePort   			GPIOB//PB1
#define RightMotorCurrentSensePin    			GPIO_Pins_1//�ҵ����������

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
#define KeyOfOn_OffPin  					GPIO_Pins_2//���ػ���ť

#define BrakeBikePort           					GPIOA
#define BrakeBikePin            					GPIO_Pins_15  //ɲ���źſ��ƽӿ�

#define DriverOverCurrentPort   				GPIOB
#define DriverOverCurrentPin    				GPIO_Pins_7//�������������

#define C_LeftMotoBreak_Port             				GPIOB//ɲ�������˿�
#define C_LeftMotoBreak_Pin              				GPIO_Pins_8//ɲ�������˿�

#define C_ReadLeftMotoBreak_Pin_Low               (0 == GPIO_ReadInputDataBit(C_LeftMotoBreak_Port,C_LeftMotoBreak_Pin))

#define C_RightMotoBreak_Port             				GPIOB//ɲ�������˿�
#define C_RightMotoBreak_Pin              				GPIO_Pins_9//ɲ�������˿�

#define C_ReadRightMotoBreak_Pin_Low              (0 == GPIO_ReadInputDataBit(C_RightMotoBreak_Port,C_RightMotoBreak_Pin))

#define UsartTxPort                      				   GPIOB
#define UsartTxPin                        				   GPIO_Pins_10

#define UsartRxPort                     				   GPIOB
#define UsartRxPin                       				   GPIO_Pins_11

#define ExPortSpeekerPort            	GPIOB
#define ExPortSpeekerPin             	GPIO_Pins_11// ��Ӹ������� PB12

#define  TeachModeTestPort          	GPIOA
#define  TeachModeTestPin	            GPIO_Pins_12//���ǣ�ʾ������µļ���

#define FeedDogPort                 	GPIOB
#define FeedDogPin                  	GPIO_Pins_14// ι������ PB12


#define C_BreakControl_Port            	GPIOC
#define C_BreakControl_Pin             	GPIO_Pins_13//ɲ����

#define C_DisBreakPort                  GPIO_ResetBits(C_BreakEnPort,C_BreakEnPin)

#define SetTeacherPortAsOutPut  {TeacherPort->CRH &=0xFFF0FFFF;TeacherPort->CRH |=0x00010000;};
#define SetTeacherPortAsInPut   {TeacherPort->CRH &=0xFFF0FFFF;TeacherPort->CRH |=0x00080000;};

/////////////////////////The Up Lists Show PA(x) Used Source
#define CtlDriverPowerContolON      	GPIO_SetBits(DriverPowerContolPort,DriverPowerContolPin)
#define CtlDriverPowerContolOFF     	GPIO_ResetBits(DriverPowerContolPort,DriverPowerContolPin)//���������̵���PA12

#define CtlPowerOnTest_ON			    GPIO_SetBits(PowerOnTestPort,PowerOnTestPin)
#define CtlPowerOnTest_OFF          	GPIO_ResetBits(PowerOnTestPort,PowerOnTestPin)//�ø�λʱ���������е�ͨ

//////////////////////////////////////////////////Up List are PB(x)s Be Used!
#define ReadKeyOfOn_OffFlag          (1 == GPIO_ReadInputDataBit(KeyOfOn_OffPort,KeyOfOn_OffPin))

#define CtlBrakeBikeEn   			GPIO_ResetBits(C_BreakControl_Port,C_BreakControl_Pin);
#define CtlBrakeBikeDisEn			GPIO_SetBits(C_BreakControl_Port,C_BreakControl_Pin)
#define CtlBrakeBikeXor       C_BreakControl_Port->ODR^=C_BreakControl_Pin;

#define ReadDriverOverCurrentFlag   	GPIO_ReadInputDataBit(DriverOverCurrentPort,DriverOverCurrentPin)//�����Ƿ������������ ���������ʱ������� �͵�ƽ ������ �� PB7


#define CtrlExtSpeekerEn              GPIO_SetBits(ExPortSpeekerPort,ExPortSpeekerPin)//�ⲿ����
#define CtrlExtSpeekerDis             GPIO_ResetBits(ExPortSpeekerPort,ExPortSpeekerPin)//�ⲿ����
#define CtrlExtSpeekerXor             GPIO_ReadOutputDataBit(ExPortSpeekerPort,ExPortSpeekerPin)?GPIO_ResetBits(ExPortSpeekerPort,ExPortSpeekerPin):GPIO_SetBits(ExPortSpeekerPort,ExPortSpeekerPin)

#define CtrlExtSpeekerEn_Debug              //GPIO_SetBits(ExPortSpeekerPort,ExPortSpeekerPin)//�ⲿ����
#define CtrlExtSpeekerDis_Debug             //GPIO_ResetBits(ExPortSpeekerPort,ExPortSpeekerPin)//�ⲿ����


#define ReadTeachPinIsConect		(0==GPIO_ReadInputDataBit(TeachModeTestPort,TeachModeTestPin))//��ȡ�Ƿ��ڳ��״̬

#define FeedDogEn                     GPIO_ResetBits(FeedDogPort,FeedDogPin)// ι������ PB14
#define FeedDogDisEn                  GPIO_SetBits(FeedDogPort,FeedDogPin)// ι������ PB14

#define FeedDogToggle                 GPIO_ReadOutputDataBit(FeedDogPort,FeedDogPin)?GPIO_ResetBits(FeedDogPort,FeedDogPin):GPIO_SetBits(FeedDogPort,FeedDogPin)// ι������ PB14


#define CtlSysTemWorkPowerHoldON    GPIO_SetBits(SysTemWorkPowerHoldPort,SysTemWorkPowerHoldPin)
#define CtlSysTemWorkPowerHoldOFF   GPIO_ResetBits(SysTemWorkPowerHoldPort,SysTemWorkPowerHoldPin)//ϵͳ������Դ
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
extern void vDealAdcVauleMotoAndCurrent(void);//���ٴ��� ad��һЩ����
extern void vDealAdcVaule(void);//����ADC�����������ݣ���һ�ε�Ƶ�˲���
extern void vInitPort(void);//�Ե�Ƭ��io�ڵĳ�ʼ������
extern void vSysTemSourceAsActive(void);//��ϵͳ�õ�����Դ�ĳ�ʼ��
extern unsigned char ucPowerOnCotrol(void);//���ϵ繤���Ĵ�����Ҫ�ǰ���һ��
extern void vDealReadProgramPin(void);//��ȡ�ϵ�
extern void vCheckSystemInfo(void);//��ϵͳһЩ����ģ��Ĳ��Թ���

extern void vContolRealize(void);//�Կ��Ƶ�ʵ��
extern unsigned char ucCalcDirection(void);//����ҡ�˵Ŀ��Ʒ���
extern void vAutoPowerOffTimeFlag(void);//�Զ��ػ�ʱ�䵽�Ĵ���

extern void vCheckHardWareErro(void);//���Ӳ������
extern void vShowPoweChange(void);//��ʾ���״̬

extern void vDelay30us(unsigned int uiDelayTime);
extern void vDelayms(unsigned int uiDelayTime);
extern void vDelaymsSoft(unsigned int uiDelayTime);
extern void vShowErroToDis(unsigned char ucErroNum);
extern void vConfigSysInStandbyMode(void);
extern void vNoKeyInStandbyMode(void);
extern void vReadSysTempPare(unsigned char ucReadEepromFlag);
extern void SysTick_Configuration(void);
extern void vDownSpeedCalcPwm(unsigned char ucFlag);
//extern void vSetMonitorKDPort(void);//?a??��???���?����?��??��??
extern void vInitWatchDog(unsigned int ucPrer, unsigned int uiRlr); //?a??�̣�???����??��??1��
extern void vFeedIwdg(void);//?1?�������??��??1��
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
//ϵͳ����״̬�궨��
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

//ϵͳ���õ���ȫ�ֱ���
extern unsigned char ucStsTemState ;
extern unsigned char ucEmgStopEn;
extern unsigned char ucShowPowerCont;
extern unsigned char ucErroType;
extern unsigned char ucNewDirectCont, ucOldDirectCont;
extern unsigned char ucSpeedGrad;
extern unsigned char ucRunAsMode;//ϵͳ��ǰ���е�״̬
extern unsigned char ucPowerKeyPressTimes;
extern unsigned char  ucAnlyFree;
extern unsigned char ucLastErro;
extern unsigned char ucInChargePinLowCont;
#ifdef BreakTabUsed_Ram
	extern unsigned short uiBreakPwmTab[3605];//ɲ�����������ֻ���ڿ�����ʱ������ǲ����ı������²�ȥ����һ�µ�
#else
	extern const unsigned short uiBreakPwmTab[3605];//ɲ�����������ֻ���ڿ�����ʱ������ǲ����ı������²�ȥ����һ�µ�
#endif
//ϵͳ�����д�������ĺ궨��

#define ErroNoErro    		 0x00
#define ErroOverCurrent    0x01
#define ErroOverVoltage    0x02
#define ErroLowVoltage     0x03
#define ErroRightBeark     0x04
#define ErroLeftBeark		   0x05
#define ErroNoRemote     	 0x06
#define ErroRelayShort     0x07//�̵�����·
#define ErroRelayBreak     0x08//�̵�����·
#define ErroNoBearker      0x09
#define ErroMoto1Break	   0x0A//0x10
#define ErroMoto2Break     0x0B//0x11
#define ErroNoCheckSys     0xFE

#define ErroMoto1HighBriageShort   0x12//���1�����Ŷ�·��
#define ErroMoto2HighBriageShort   0x13//���2�����Ŷ�·��
#define ErroMoto1HighBriageBreak   0x14//���1�����Ŷ�·��
#define ErroMoto2HighBriageBreak   0x15//���2�����Ŷ�·��
#define ErroMoto1LowBriageShort    0x16//���1�����Ŷ�·��
#define ErroMoto2LowBriageShort    0x17//���2�����Ŷ�·��
#define ErroMoto1LowBriageBreak    0x18//���1�����Ŷ�·��
#define ErroMoto2LowBriageBreak    0x19//���2�����Ŷ�·��
#define ErroSysPowerOff            0x20//ϵͳҪ��ػ���ʱ��
#define ErroSysPowerKeyPress       0x21
//�����ң�з���Ķ���
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

#define AutoBattIdenConfigFlag  0x80//����Զ�ʶ�����ù���־
#define AutoBattIdenConfigAsPb     0x01//����Զ�ʶΪǦ����
#define AutoBattIdenConfigAsLion  0x02//����Զ�ʶΪ﮵��
extern unsigned char ucAutoBattIdenConfigData;//����Զ�ʶ������
//ϵͳ������ٶȿ̶�ֵ
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

#define KeyHoldTime 25//��������ʱ��
#define InChargePinLowCont  20//ֻ�г���ͷ����2������ӵ�ʱ�� ��ȥָʾ����

#define PowerOffTime (AutoPowerOffTime*60*1000)/10

#define PoweOffReleaseMosPWM    100//��ֹͣ�ػ���ʱ��  �����ŵ��PWMֵ
#define CheckMotoTestPwmValue   0x190//200
//#define WhenLeftRightUpLowMosOpenWithTestPwmAsPowerPare   0.07//��������Բ���PWMֵʱ����Ӧ����ֵ�ı���ϵ��
//#define WhenLeftRightUpOpenMosLowCloseMosWithTestPwmAsPowerPare   0.1//��������Բ���PWMֵʱ����Ӧ����ֵ�ı���ϵ��

#define SampleValueAsTureVotageP   0.008865  //�������ĵ�ѹֵ����ʵֵ�ı�����ϵ
#define AlarmVAsRun              19//������ص���ֵ  19
#define CheckLowAlarmVoltage    (AlarmVAsRun/SampleValueAsTureVotageP)//��Ӧ��ADֵ

#define SlopModeAlarmVAsRun           11//������ص���ֵ  19
#define SlopModeCheckLowAlarmVoltage  (SlopModeAlarmVAsRun/SampleValueAsTureVotageP)//��Ӧ��ADֵ

#define MotoABreakFlagOne         0x01
#define MotoABreakFlagTwo         0x02
#define MotoBBreakFlagOne         0x10
#define MotoBBreakFlagTwo         0x20

#define IfMotoSampleAdAtErroVoltage        3700//��ֵ�������
#define dSysPowerHold       1 //�ٹػ�

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


#define SampleAdJudgeMode_FinishTime          2   //1����ɵ��ڹ��� ��ѹֵ��730-930֮��
#define SetPWMFinishCheckMode_SetTime         1    //��ɺ���Ƶ�ʽ���
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
#define JudgeModebBreakEnDecPareFast       0.2  //���ɲ���������
#define JudgeModeSetMotoDecPareFast        0.6   //פ�³������
//0.6//0.75 ��ֵ���⵽�ĵ�ѹֵ�й�
#define SmallWheelJudgeModeSetMotoDecPare  0.15
#define BigWheelJudgeModeSetMotoDecPare    0.2//0.15

#define JudgeModeSetMotoPlusPareFast       0.025// 0.1//0.15��ֵ���⵽�ĵ�ѹֵ�й�
#define JudgeModeSetMotoPlusPare           0.025//0.025//0.125 ��ֵ���⵽�ĵ�ѹֵ�й�
#endif
#ifdef SetPwmStep3
#define JudgeModeSetMotoDecPareFast       0.7//0.75 ��ֵ���⵽�ĵ�ѹֵ�й�
#define JudgeModeSetMotoDecPare           0.01//��ֵ���⵽�ĵ�ѹֵ�й�
#define JudgeModeSetMotoPlusPareFast      0.1//��ֵ���⵽�ĵ�ѹֵ�й�
#define JudgeModeSetMotoPlusPare          0.03//0.+1 ��ֵ���⵽�ĵ�ѹֵ�й�
#endif
#ifdef SetPwmStep1
#define JudgeModeSetDecPareFast      MaxSlopPWM/2//��ֵ���⵽�ĵ�ѹֵ�й�
#endif

#define JudgeModeSetDecPare          6//6��ֵ���⵽�ĵ�ѹֵ�й�
#define JudgeModeSetPareFast         50//50  ��ֵ���⵽�ĵ�ѹֵ�й�
#define JudgeModeSetPare             15//��ֵ���⵽�ĵ�ѹֵ�й�

#define LowSlopPWM          100//80
#define PowerOffSlopPWM     120//90//270//MaxSlopPWM/2//��ֵ���⵽�ĵ�ѹֵ�й�


#ifdef HK32
//#define CalSlopPWM                 950
//#define IntoSlopPWM                470//720
//#define UserSetSmallWheelSlopPWM   350
//#define UserSetBIGWheelSlopPWM     800
//#define UserSetSlopMarkPWM         180//500
//#define MaxSetSlopPWM              30

#define CalSlopPWM                 1000
#define IntoSlopPWM                400//720  ԭ��500  2020-03-24
#define UserSetSmallWheelSlopPWM   350
#define UserSetBIGWheelSlopPWM     800
#define UserSetSlopMarkPWM         180//500  
#define MaxSetSlopPWM              30  //2020-02-24
#endif

#ifdef STM32
#define CalSlopPWM                 1000
#define IntoSlopPWM                400//720  ԭ��500  2020-03-24
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
//#define IfMotoASampleAdAtStep2High 		   (strMoto1Info.uiMotorVoltageBase + 115)//���������ȵ���ֵ

//#define IfMotoBSampleAdAtStep1High 		   (strMoto2Info.uiMotorVoltageBase + 22) //30
//#define IfMotoBSampleAdAtStep2High 		   (strMoto2Info.uiMotorVoltageBase + 115)//  175  ԭ200 lg_qiuzhi191216

#define IfMotoASampleAdAtStep1High 		   (strMoto1Info.uiMotorVoltageBase - 42)
#define IfMotoASampleAdAtStep2High 		   (strMoto1Info.uiMotorVoltageBase + 35)//���������ȵ���ֵ

#define IfMotoBSampleAdAtStep1High 		   (strMoto2Info.uiMotorVoltageBase - 42) //30
#define IfMotoBSampleAdAtStep2High 		   (strMoto2Info.uiMotorVoltageBase + 35)//  175  ԭ200 lg_qiuzhi191216

//#define IfMotoASampleAdAtStep1Low 	 		   (strMoto1Info.uiMotorVoltageBase - 500)
//#define IfMotoASampleAdAtStep2Low 	 		   (strMoto1Info.uiMotorVoltageBase - 610)

//#define IfMotoBSampleAdAtStep1Low 	 		   (strMoto2Info.uiMotorVoltageBase - 500)//   195ԭ200 lg_qiuzhi191216
//#define IfMotoBSampleAdAtStep2Low 	 		   (strMoto2Info.uiMotorVoltageBase - 610) //600

#define IfMotoASampleAdAtStep1Low 	 		   (strMoto1Info.uiMotorVoltageBase - 140)
#define IfMotoASampleAdAtStep2Low 	 		   (strMoto1Info.uiMotorVoltageBase - 410)

#define IfMotoBSampleAdAtStep1Low 	 		   (strMoto2Info.uiMotorVoltageBase - 140)//   195ԭ200 lg_qiuzhi191216
#define IfMotoBSampleAdAtStep2Low 	 		   (strMoto2Info.uiMotorVoltageBase - 410) //600

#define IfMotoACurrentAtStep1High    			 (strMoto1Info.uiMotorCurrentBase + 170)
#define IfMotoBCurrentAtStep1High    			 (strMoto2Info.uiMotorCurrentBase + 175)

#define IfMotoACurrentAtStep2High    			 (strMoto1Info.uiMotorCurrentBase + 100)
#define IfMotoBCurrentAtStep2High    			 (strMoto2Info.uiMotorCurrentBase + 100)  //2020-02-23 ֮ǰ
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
//#define IfMotoBSampleAdAtStep2High 		   (strMoto2Info.uiMotorVoltageBase + 175)//  175  ԭ200 lg_qiuzhi191216

//#define IfMotoBSampleAdAtStep1Low 	 		   (strMoto2Info.uiMotorVoltageBase - 130)//   195ԭ200 lg_qiuzhi191216
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
    double fVoltage[BigWheelJudgeMode_SetPwmHighTime];//���ad����������� ƽ��ֵ
    float fVoltageSample;//���ad����������� ƽ��ֵ
    float fLastVoltageSample;//���ad����������� ƽ��ֵ
    float fSetPwmStep;//ÿһ������PWM��ֵ�Ķ���

    unsigned char uiSetPwmTimerCCR1_Flag;
    unsigned char uiSetPwmTimerCCR3_Flag;//����PWM������
    float fSetCCR1CCR2PwmValue;
    float fSetCCR3CCR4PwmValue;
    float fReadOldCCR1CCR2PwmValue;
    float fReadOldCCR3CCR4PwmValue;

    unsigned short uiSampleAdCnt;
    unsigned char  uiZeroTestPhase;
    unsigned char uCheckMode_SetTime; //����ģʽ��ƽ����730ʱ���Ǹ���900��ֵʱ�����������ͽ������ģʽ

    unsigned char uiSampleAdSetPWMFinishFlag; //������ɱ�־λ
    unsigned char uiSampleAdSetIntoBreakCnt; //������ɱ�־λ
    unsigned char uiSampleAdSetPWMFinishCnt; //������ɴ���
    unsigned char uiSampleAdJudgeMode_SetCnt;	//����ģʽ����Ƿ�������ģʽ����

    unsigned char uiSampleAdJudgeModeCnt; //��Ҫ���ڴ���
    unsigned char uiSampleAdJudgeModeFinishFlag; //������ɱ�־λ
    int iArryCurrentSample[CurrentReadTimes];

    unsigned char uiPeopleStandUpFlag; //��վ������־λ
    int iCalCurrentSample;

    unsigned char uiJudgeMode_SetPwmHighTime;
    unsigned char uJudgeModeIntoCheckModeCnt; //��Ҫ���ڴ���
    //		unsigned int iMotoCurrentSample;
    unsigned char uCheckMode_ReadTime;
    unsigned char uCheckMode_ReadFlag;

} SLOPMODEADJUST_t;

#ifdef HK32
#define AdJudgeModeTimes          2
#define IntoJudgeModeCurrent       8//8//   //35
#define PlusIntoJudgeModeCurrent   6//6//
#define IntoBreakTimer          1 //��ɵ��ڴ���
#endif
#ifdef STM32
#define AdJudgeModeTimes          2
#define IntoJudgeModeCurrent       12//8//   //35
#define PlusIntoJudgeModeCurrent   12//6//
#define IntoBreakTimer          1 //��ɵ��ڴ���
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
        unsigned short Ready: 1;     //�ŷ�����  servo ready
        unsigned short Runing: 1;      //�ŷ�����  servo on
        unsigned short Slop: 1;     //פ��ģʽ
        unsigned short ZeroSpeed: 1;     //���ټ�� zero speed
        unsigned short TargetSpeed: 1;	   //Ŀ���ٶȵ��� target speed
        unsigned short TargetPosition: 1;     //Ŀ��λ�õ��� target position
        unsigned short TorqueLimit: 1;		//ת��������  torque limit
        unsigned short Alarm: 1;     //����      alarm
        unsigned short BreakOutput: 1;     //�ƶ���� break
        unsigned short OverGateCurrent: 1;      //�������ż�  over limit ?
        unsigned short Warn: 1;     //����?
        unsigned short CmdOK: 1;    //�������
        unsigned short CWLock: 1;   //�����ת  cw lock
        unsigned short CCWLock: 1;  //�����ת  ccw lock
        unsigned short NP: 1;	   //������
        unsigned short Zpulse: 1;	//z�������
    } bit;

} WORK_STATE_u;  //�ŷ�����״̬

typedef union
{
    unsigned short all;
    struct
    {
        unsigned short OverCurrent: 1;     //����
        unsigned short OverVoltage: 1;      //��ѹ
        unsigned short LowVoltage: 1;     //Ƿѹ
        unsigned short MotoAHighBriageBreak: 1;     //MotoA���ű۶�·

        unsigned short MotoBHighBriageBreak: 1;	   //MotoB���ű۶�·
        unsigned short Resever: 11;     //

    } bit;

} FAULTCODE_u; //���ϴ���
extern FAULTCODE_u FaultCode;
extern WORK_STATE_u Work_Status;
extern SLOPMODEADJUST_t  MotoASlopModeAdjust;
extern SLOPMODEADJUST_t  MotoBSlopModeAdjust;

extern unsigned char ucMingRuiErrConst;
extern void PwmDataInit(void);

extern unsigned short usCheckRemoteTimeCnt;
#endif

