


#ifndef __DIRVERADC_H_
#define __DIRVERADC_H_


#define OneChannelConvertTimes  10//ÿ��ͨ��ת������
#define TotalADCChannels        10//����ADͨ����
extern volatile unsigned short vDmaContAdcVaule[OneChannelConvertTimes][TotalADCChannels];

struct strMotorInfo
{
    unsigned char unMotorOldDirection;//�����ת������
    unsigned int uiOldMotorPwmValue;//ǰһ�ε����PWMֵ��¼  ����ֻ�ǰٷֱ�
    unsigned int uiNewMotorPwmValue;//������µ�PWM  ����ֻ�ǰٷֱ�
    unsigned int uiMotorCurrent;//��ǰ����ĵ���
    unsigned int uiMotorCurrentBase;//���ҵ���ĵ�������
    //     unsigned int uiMotorSpeed;//�����ǰ���ٶ�
    //     unsigned int uiMotorDriverCurrent;//�����������
    unsigned int uiMotorBreakDiv;//�����ɲ����ʱ�� ���Էֵ�PWM��λ
    unsigned int uiMotorBreakDistance;//Ҫ��ɲ���ľ���
    unsigned int uiLeftHBridgeHaveFilterAdc;
    unsigned int uiRightHBridgeHaveFilterAdc;
	
	  unsigned int uiMotorVoltageBase;//���ҵ���ľ�ֹ��������ѹ����
};
struct strSysTemInfo
{
    unsigned int uiSysTemPower;//ϵͳ�����ѹֵ
    unsigned int uiRemoteMaxXPos;//�궨����ң��X���ֵ
    unsigned int uiRemoteMinXPos;//�궨����ң��X��Сֵ
    unsigned int uiRemoteErroMaxValue;//����ҡ�˵�����ѹ
    unsigned int uiRemoteErroMinValue;//����ҡ�˵���С��ѹ
    unsigned int uiRemoteMaxYPos;//�궨����ң��Y���ֵ
    unsigned int uiRemoteMinYPos;//�궨����ң��Y��Сֵ
    unsigned int uiRemoteMiddleXPos;//�궨����ң��X�м�ֵ
    unsigned int uiRemoteMiddleYPos; //�궨����ң��Y�м�ֵ
    unsigned int uiRemoteNowXPos;//��������ǰX���ֵ
    unsigned int uiRemoteNowYPos;//��������ǰY���ֵ

    unsigned int uiRemoteXP_Delt;//ҡ���������ÿ���̶ȵ����
    unsigned int uiRemoteXN_Delt;//ҡ�˸������ÿ���̶ȵ����
    unsigned int uiRemoteYP_Delt;//ҡ���������ÿ���̶ȵ����
    unsigned int uiRemoteYN_Delt;//ҡ�˸������ÿ���̶ȵ����

    unsigned int uiRemoteXP_First;//�������ϵĵ�һ����ֵ
    unsigned int uiRemoteXN_First;//�������ϵĵ�һ����ֵ
    unsigned int uiRemoteYP_First;//�������ϵĵ�һ����ֵ
    unsigned int uiRemoteYN_First;//�������ϵĵ�һ����ֵ

    unsigned int uiControlPareOfKp;
    unsigned int uiControlPareOfKd;
    unsigned int uiControlMinXDriverPwm;
    unsigned int uiControlMaxXDriverPwm;
    unsigned int uiControlMinYDriverPwm;
    unsigned int uiControlMaxYDriverPwm;
    unsigned int uiLastControlQuadrant;
    unsigned int uiLastRemotValue;
    unsigned int uiCalcSysShowPower;//���������
    unsigned int uiBreakFeedBackCurrent;//ɲ������������
		
	  unsigned int uiSysBreakFlag;//ɲ��ʱ���־λ		
	  unsigned int uiCalcPwmInBreak;//������ϴ�ɲ��ʱ�� ��PWMֵ ���ֵ
};

struct strBatterLowPowerCom//�͵�ѹ��ʱ���ٶȲ���
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
#define FactoryPareKd  1//����ʱ��

extern volatile struct strSysTemInfo strSysInfo;
extern volatile struct strMotorInfo strMoto1Info, strMoto2Info;

//ADͨ���Ķ���
#define RemoteYAdcCh                           0//PA1 ң��Yλ��
#define RemoteXAdcCh                           1//PA2  ң��Xλ��
#define Motor1RightHBridgeVolHaveFilterCh      2//PA3 ���1�ҵ�ѹ���
#define Motor1LeftHBridgeVolHaveFilterCh  	   3//PA4  ���1���ѹ���
#define SysTemPowerAdcCh  	                   4//PA6 ϵͳ�������
#define Motor2LeftHBridgeVolHaveFilterCh  	   5//PA7���2���ѹ���
#define BreakFeedBackCh  			                 6  //PB0 ɲ������ѹ����
#define Motor2RightHBridgeVolHaveFilterCh      7  //PB1 ���2�ҵ�ѹ���
#define Motor2DriveCurrent                     8  //PA0 ���2�ĵ������
#define Motor1DriveCurrent                     9  //PA5 ���1�ĵ������


#define ClacXPwmFirstRemotePosP  (FactoryRemoteMaxXPos -FactoryRemoteMidXPos)/6
#define ClacXPwmFirstRemotePosN  (FactoryRemoteMidXPos -FactoryRemoteMinXPos)/6

#define ClacYPwmFirstRemotePosP  (FactoryRemoteMaxYPos -FactoryRemoteMidYPos)/6
#define ClacYPwmFirstRemotePosN  (FactoryRemoteMidYPos -FactoryRemoteMinYPos)/6

#define DriverYMotorStep 				5//�����Ϊ16���ȼ����ٶȲ���
#define DriverYRemoteAdcBase    strSysInfo.uiRemoteMiddleYPos
#define DriverXMotorStep 				5//�����Ϊ16���ȼ����ٶȲ���
#define DriverXRemoteAdcBase    strSysInfo.uiRemoteMiddleXPos

#define DriverXRemoteAdcDeltPositive     (strSysInfo.uiRemoteMaxXPos - ClacXPwmFirstRemotePosP - strSysInfo.uiRemoteMiddleXPos)>>DriverXMotorStep;
#define DriverXRemoteAdcDeltNegative     (strSysInfo.uiRemoteMiddleXPos - ClacXPwmFirstRemotePosN - strSysInfo.uiRemoteMinXPos)>>DriverXMotorStep;

#define DriverYRemoteAdcDeltPositive     (strSysInfo.uiRemoteMaxYPos - ClacYPwmFirstRemotePosP- strSysInfo.uiRemoteMiddleYPos)>>DriverYMotorStep;
#define DriverYRemoteAdcDeltNegative     (strSysInfo.uiRemoteMiddleYPos- ClacYPwmFirstRemotePosN- strSysInfo.uiRemoteMinYPos)>>DriverYMotorStep;

#define FactoryPareMinXPwm  8
#define FactoryPareMaxXPwm  98//���������ռ�ձ�

#define FactoryPareMinYPwm  8
#define FactoryPareMaxYPwm  98

#define FactoryPareMinXDriverPwm  (TimerBase*FactoryPareMinXPwm)/100
#define FactoryPareMaxXDriverPwm  (TimerBase*FactoryPareMaxXPwm)/100

#define FactoryPareMinYDriverPwm  (TimerBase*FactoryPareMinYPwm)/100
#define FactoryPareMaxYDriverPwm  (TimerBase*FactoryPareMaxYPwm)/100

#define XDriverPwmDelt   (strSysInfo.uiControlMaxXDriverPwm - strSysInfo.uiControlMinXDriverPwm)>>DriverXMotorStep;
#define YDriverPwmDelt   (strSysInfo.uiControlMaxYDriverPwm - strSysInfo.uiControlMinYDriverPwm)>>DriverYMotorStep;

#define MaxPowerAdcValue  	2876
#define MinPowerAdcValue   	2700//����궨����  23.74v
#define NoPowerOnAdcValue    MinPowerAdcValue/3//��ϵͳû���ϵ��ʱ���ADCֵ
#define LowPowerVoltage        1989//17.5  �����ѹ�������ֵ�Ļ������ÿ����������ѹ��
#define RunLowPower              LowPowerVoltage/3//��������е�ʱ���ѹ���ڵף�Ҫȥ�����

#define NoBreakEnAdcValue       400
#define MosHighBriageOnAd       1500//300//����0x170��PWMʱ����ADC�����ֵ
#define DefaultFactryID       0 //1114010001
#define HaltMosMaxPower  2300//��ͣ����ʱ��mos�ܵ����ֵ��ѹ 
//  Vx =((x/4096)*3.3��*(8.2+33))/8.2  as x=2300 Vx = 9.3
#define BreadAdAndPowerP  0.645
#define BreakLeftBreakAdcP  0.533
#define BreakRightBreakAdcP  0.445
#define NoBreakAdcP  0.333
#define AllowAutoPowerMaxVol   3000//�����Զ��������ߵ�ѹ   
#define AllowAutoPowerMinVol  400 //�����Զ������С��ѹ
#define PbBattlePare   8.836//Ǧ���صĵ�ѹ��strSysInfo.uiSysTemPower ��ϵ
#define LowVotageAutoPowerOff   16500//16.5����ص�ѹ �ڵ���޶���ʱ�� �����⵽��صĵ�ѹ������LowVotageAutoPowerOff ���Զ��ػ�


#define AutoLowPowerAlarmLilonVoltage    23 //�����ѹ���ڴ�ֵʱ����Զ��ػ�
#define AutoLowPowerAlarmLilonVoltageADC (unsigned int)(AutoLowPowerAlarmLilonVoltage/SampleValueAsTureVotageP)

#define AutoLowPowerAlarmQiansuanVoltage    17.5 //�����ѹ���ڴ�ֵʱ����Զ��ػ�
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
union UINT//����һ�����ݽṹ��
{
    unsigned int uiData;
    struct stucDataUI ucData;   //Data Type Define :If define ucXXXXX meaning "unsigned char" Data Type;
};
struct unMotorAttributeBits//�������һЩ��Ϣ
{
    unsigned char bChangeMotor1DirEn: 1; //0,���л�  1:�л����1������ת
    unsigned char bChangeMotor2DirEn: 1; //0,���л�  1:�л����2������ת
    unsigned char bChangeMotor1Motor2: 1; //0,���л� 1:�������1��2
    unsigned char bBreakEn: 1;           //����ɲ����
    unsigned char bCheckMotorConnect: 1; //����������Ƿ�����
    unsigned char bCheckBreakConnect: 1; //�������ɲ�����Ƿ�����
    unsigned char bLilonOrQiansuan: 1; //Ǧ�ỹ��﮵�1��﮵�
    unsigned char bEnAutoPowerOn: 1; //�Զ��ﳵ���ܵļ���
};
union unMotorAttribute
{
    unsigned char all;
    struct unMotorAttributeBits bits;
};
#define DirverAttLength  26
struct strDriverInfo//������������Ϣ
{
    unsigned char ucEnIternSpeaker;//�Ƿ���������
    unsigned char ucExternSpeakerVolume;//�������ȵ�����
    union unMotorAttribute unMotorInfo;//�����һЩ���ԣ��Ƿ�Ҫ�����
    unsigned char ucFirstLineRatePare;//��һ��ɲ�����ߵ�����ֵ
    unsigned char ucFistLineTurnSecondPos;//��һ��ɲ��ת�����ڶ���ɲ���Ĺյ�ֵ
    unsigned char ucBreakAccPare;//ɲ�����ļ��ٶ�
    unsigned char ucForwardSpeedP;//ǰ���ٶȱ���ϵ��
    unsigned char ucBackSpeedP;//�����ٶȱ���ϵ��
    unsigned char ucDriverAcc;//ǰ�����˼��ٶ�
    unsigned char ucSwerveAcc;//ת����ٶ�
    unsigned char ucStarMinPwm;//������СPWM
    unsigned char ucStopMinPwm;//ֹͣ��СPWM
    unsigned char ucLetRightMotorP;//���ҵ������ϵ��
    unsigned char ucLetRightMotorTiedP;//���ҵ����ϵ��
    unsigned char ucMaxDriverCurrent;//�����������
    unsigned char ucFactryIdData1;//��������32λ�����8λ
    unsigned char ucFactryIdData2;//��������32λ����8λ
    unsigned char ucFactryIdData3;//��������32λ����8λ
    unsigned char ucFactryIdData4;//��������32λ�����8λ
    unsigned char ucForwardMinSpeedP;//ǰ����С�ٶ�
    unsigned char ucBackMinSpeedP;//������С�ٶ�
    unsigned char ucSwerveMaxSpeedP;//ת������ٶ�
    unsigned char ucSwerveMinSpeedP;//ת����С�ٶ�
    union unDelayBreakTime unDelayBreakActTime;//�ӳٹر�ɲ����ʱ��
};

union uniDirverAtt//����һ������һЩ�����йص����ݽṹ
{
    struct strDriverInfo strDirverAtt;//�������ݵĲ���
    unsigned char ucDirverAttDataBuff[DirverAttLength];//����һ������ռ�
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
