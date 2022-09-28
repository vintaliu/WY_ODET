
#include "SystemInit.h"
/////***************&&&&&&&&&&&&&&&&&&&&&&&&&***********************************
#define PowerOnSpeedGradeAt  2//定义开机速度档位，1，2，3，4，5除此外数据，保存速度档位    
#define EnableCancelBackCarAlarm    //是否允许倒车声响。如果想要取消，注释这行  以双斜杠为前，再重新编译下程序 如--》//#define EnableCanncelBackCarAlarm
/////***************&&&&&&&&&&&&&&&&&&&&&&&&&***********************************

//#define UsedSimulate//如果使用了仿真器模式
#define DKVersion  0x01
#define BatteryAutoShiBie  ///电池识别
#define QianXiUart  


//void Vdebug();
unsigned int ADC_GetConversionValueData();
void SpeedLedOn(unsigned char num);//显示速度指示灯
void PowerLedOn(unsigned short power);//显示电量指示灯
void PowerLedNumOn(unsigned char num);//显示电量指示灯
void vSpeedMiunsPressed(void);//速度减
void vSpeedPlusPressed(void);//速度加
void VBreathLED(void);//测试呼吸灯
void vBuzzerPressed(void);//按喇叭
void vPowerPressed(void);//按电源键关机
void vFirstStart(void);//第一次开机
//void vCheckInBack(void);//检测倒车
//void vCheckLowPower(void);//电源电量低
//void BEEP_Alarm(void);//蜂鸣器报警
void delay(unsigned long i);
void vCheckInCharge(void);//充电模式
void vWrite60Bytes(void);//主控写入60字节数据

void vStartSucceed(void);//第一次开机响一下
void DealKey(void);//按键处理
void vDealAd(void);//AD处理
void vTestShowLed(unsigned int value); //测试，让显示板灯点亮
void EEPROM_WriteByte(unsigned int u16_addr, unsigned char u8_data); //EEPROM写入数据
unsigned char EEPROM_ReadByte(unsigned int *Address);//EEPROM读出数据
void vAlarmLess18Power();
void vInitLedPin(void);
void vSendDKVersion(unsigned char uiVersion);
void SetPWM1_Out(unsigned int x, unsigned int y);
void PWM1_Init(void);
unsigned char vArom_ReadBytes(unsigned int Address, unsigned char *pData,  unsigned int uiDataLeg);
void vArom_WriteBytes(unsigned int Address, unsigned char pData, unsigned int uiDataLeg);

void InitialUART0_Timer1(unsigned long u32Baudrate) ;

//#define DebugPort  P14
//#define XorDebugPin    DebugPort = ~DebugPort
extern unsigned char ucDebugSpeaker;


extern unsigned int  uiAdVoltage;
extern unsigned char ucSpeedGrad;  //速度


extern unsigned char ucLessThanLowPower; //电源电压小于23.740的标志
extern unsigned char ucLess18Power;         //电源电压小于18的标志

extern unsigned char ucBuzzerTstFlg; //蜂鸣器测试标志



//* EEPROM大小：640 byte */
#define EEPROM_START_ADDRESS_FACT_PARE  0x00006000// IAP 17K开始的 工厂参数  以8K  FLASH
#define EEPROM_START_ADDRESS_FACT_PARE_Legth  65//工厂参数的烧写地址

#define SPEED_ADDRESS           0x00006400//速度按键值在 512防止写数据到 工厂参数里了

#define EEPRO_One_Byte_ADDRESS  0x00006800 //读单字节数据地址

#define WriteBatteryTypeFlag_Addr      0x00006C00//写电池类型标志位

extern unsigned char  uiAutoAddFactData[60];
//EF,0A,1C,07,07,07,62,32,05,07,05,05,05,06,14,00,00,00,00,44,14,2D,14,00,E8,03,00,00,00,00,00,00,29,
//00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,55,55,AA
#define RW_COUNT              60

//* 电源电量 *
#define LOW_POWER        23740
#define ONE_LED_POWER    24000
#define TWO_LED_POWER    24500
#define THREE_LED_POWER  24930
#define FOUR_LED_POWER   25280
#define ALARM_POWER      18000

#define ADC_ADCCON0_ADCS  ((unsigned char)0x40)
#define ConverTimes   6

//#define SPEED_STEP1_Port  PC_ODR
//#define SPEED_STEP1_Pin   0x10
#define POWER_SHOW1_OFF    GPIO_ResetBits(CMSDK_PA, GPIO_Pin_3);
#define POWER_SHOW1_ON      GPIO_SetBits(CMSDK_PA, GPIO_Pin_3);
#define POWER_SHOW2_OFF    GPIO_ResetBits(CMSDK_PC, GPIO_Pin_3);
#define POWER_SHOW2_ON      GPIO_SetBits(CMSDK_PC, GPIO_Pin_3);
#define POWER_SHOW3_OFF    GPIO_ResetBits(CMSDK_PC, GPIO_Pin_6);
#define POWER_SHOW3_ON      GPIO_SetBits(CMSDK_PC, GPIO_Pin_6);
#define POWER_SHOW4_OFF    GPIO_ResetBits(CMSDK_PC, GPIO_Pin_5);
#define POWER_SHOW4_ON      GPIO_SetBits(CMSDK_PC, GPIO_Pin_5);
#define POWER_SHOW5_OFF    GPIO_ResetBits(CMSDK_PC, GPIO_Pin_7);
#define POWER_SHOW5_ON      GPIO_SetBits(CMSDK_PC, GPIO_Pin_7);

#define SPEED_STEP1_OFF   GPIO_ResetBits(CMSDK_PA, GPIO_Pin_1);
#define SPEED_STEP1_ON     GPIO_SetBits(CMSDK_PA, GPIO_Pin_1);
#define SPEED_STEP2_OFF   GPIO_ResetBits(CMSDK_PA, GPIO_Pin_2);
#define SPEED_STEP2_ON     GPIO_SetBits(CMSDK_PA, GPIO_Pin_2);
#define SPEED_STEP3_OFF   GPIO_ResetBits(CMSDK_PD, GPIO_Pin_4);
#define SPEED_STEP3_ON     GPIO_SetBits(CMSDK_PD, GPIO_Pin_4);
#define SPEED_STEP4_OFF   GPIO_ResetBits(CMSDK_PD, GPIO_Pin_2);
#define SPEED_STEP4_ON     GPIO_SetBits(CMSDK_PD, GPIO_Pin_2);
#define SPEED_STEP5_OFF   GPIO_ResetBits(CMSDK_PD, GPIO_Pin_1);
#define SPEED_STEP5_ON     GPIO_SetBits(CMSDK_PD, GPIO_Pin_1);
//
/*#define POWER_SHOW1_OFF   P15 = 1//P1.5
#define POWER_SHOW1_ON     P15 = 0
#define POWER_SHOW2_OFF   P12 = 1//P1.2
#define POWER_SHOW2_ON     P12 = 0
#define POWER_SHOW3_OFF   P00 = 1//P0.0
#define POWER_SHOW3_ON     P00 = 0
#define POWER_SHOW4_OFF   P10 = 1//P1.0
#define POWER_SHOW4_ON     P10 = 0
#define POWER_SHOW5_OFF   P01 = 1//P0.1
#define POWER_SHOW5_ON     P01 = 0

#define SPEED_STEP1_OFF  P30 = 1//P3.0
#define SPEED_STEP1_ON    P30 = 0
#define SPEED_STEP2_OFF  P17 = 1//P1.7
#define SPEED_STEP2_ON    P17 = 0
#define SPEED_STEP3_OFF  P05 = 1//P0.5
#define SPEED_STEP3_ON    P05 =0
#define SPEED_STEP4_OFF  P03 = 1//P0.3
#define SPEED_STEP4_ON    P03 = 0
#define SPEED_STEP5_OFF  P02 = 1//P0.2
#define SPEED_STEP5_ON    P02 = 0
*/
//#define SPEEKER_CTRL_OFF   P10 &=0
//#define SPEEKER_CTRL_ON    P10 |=1
#define SPEEKER_CTRL_OFF
#define SPEEKER_CTRL_ON

#define SPEED_LED_0  {SPEED_STEP1_OFF;SPEED_STEP2_OFF;SPEED_STEP3_OFF;SPEED_STEP4_OFF;SPEED_STEP5_OFF;}
#define SPEED_LED_1  {SPEED_STEP1_ON;SPEED_STEP2_OFF;SPEED_STEP3_OFF;SPEED_STEP4_OFF;SPEED_STEP5_OFF;}
#define SPEED_LED_2  {SPEED_STEP1_ON;SPEED_STEP2_ON;SPEED_STEP3_OFF;SPEED_STEP4_OFF;SPEED_STEP5_OFF;}
#define SPEED_LED_3  {SPEED_STEP1_ON;SPEED_STEP2_ON;SPEED_STEP3_ON;SPEED_STEP4_OFF;SPEED_STEP5_OFF;}
#define SPEED_LED_4  {SPEED_STEP1_ON;SPEED_STEP2_ON;SPEED_STEP3_ON;SPEED_STEP4_ON;SPEED_STEP5_OFF;}
#ifdef UsedSimulate
#define SPEED_LED_5  {SPEED_STEP1_ON;SPEED_STEP2_ON;SPEED_STEP3_ON;SPEED_STEP4_ON;SPEED_STEP5_ON;}
#else
#define SPEED_LED_5  {CMSDK_PD->ALTFUNCCLR = 0x02;SPEED_STEP1_ON;SPEED_STEP2_ON;SPEED_STEP3_ON;SPEED_STEP4_ON;SPEED_STEP5_ON;}
#endif

#define POWER_SHOW_0  {POWER_SHOW1_OFF;POWER_SHOW2_OFF;POWER_SHOW3_OFF;POWER_SHOW4_OFF;POWER_SHOW5_OFF;}
#define POWER_SHOW_1  {POWER_SHOW1_ON;POWER_SHOW2_OFF;POWER_SHOW3_OFF;POWER_SHOW4_OFF;POWER_SHOW5_OFF;}
#define POWER_SHOW_2  {POWER_SHOW1_ON;POWER_SHOW2_ON;POWER_SHOW3_OFF;POWER_SHOW4_OFF;POWER_SHOW5_OFF;}
#define POWER_SHOW_3  {POWER_SHOW1_ON;POWER_SHOW2_ON;POWER_SHOW3_ON;POWER_SHOW4_OFF;POWER_SHOW5_OFF;}
#define POWER_SHOW_4  {POWER_SHOW1_ON;POWER_SHOW2_ON;POWER_SHOW3_ON;POWER_SHOW4_ON;POWER_SHOW5_OFF;}
#define POWER_SHOW_5  {POWER_SHOW1_ON;POWER_SHOW2_ON;POWER_SHOW3_ON;POWER_SHOW4_ON;POWER_SHOW5_ON;}

//#define RedPowerLed  P17
#define XorRedPowerLed   GPIO_ReverseBits(CMSDK_PA, GPIO_Pin_3);

#define POWER_ON_KEY_PRESSED  GPIO_ReadInputDataBit(CMSDK_PB, GPIO_Pin_4)//PB4此处一定要和PB4指定寄存器相的值与

#define TIMER_1MS 1
#define TIMER_5MS 5
#define TIMER_10MS 10
#define TIMER_20MS 20

#define TIMER_100MS 100
#define TIMER_200MS 200
#define TIMER_300MS 300
#define TIMER_400MS 400
#define TIMER_600MS 600
#define TIMER_4000MS   4000

#define TIMER_1S    1000
typedef union
{
    unsigned short int All;
    struct
    {
        unsigned short int PowerOn: 1;
        unsigned short int Beep_Alarm: 1;
        unsigned short int AdVoltage: 1;
        unsigned short int CheckKeyPress: 1;
        unsigned short int CheckIDele : 1;
        
        unsigned short int : 11;
    } Bits;
} GLOBLE_TIMER_1MS;

extern unsigned short g_us1msTimes;

typedef union
{
    unsigned char all;
    struct
    {
        unsigned char SpeedDownKey: 1;
        unsigned char SpeedUpKey: 1;
        unsigned char SpeakerKey: 1;
        unsigned char SpeedChange: 1;	//最大速度需要修改标示位

        unsigned char SpeedModify: 1;	//最大速度调节
        unsigned char PowerChange: 1;	//切换电池类型
        unsigned char SpeedDownKeyPressFlag: 1;//最大速度调节的时候。速度减按键，先有效果
        unsigned char BackCarAlarmModify: 1;
    } Bits;
} PeripheralInput_u;

typedef union
{
    unsigned char all;
    struct
    {
        unsigned char PowerLed: 1;
        unsigned char SpeedLed: 1;
        unsigned char Alarm: 1;
        unsigned char Rev: 5;
    } Bits;
} PeripheralOutput_u;

typedef struct
{
    PeripheralInput_u   Input;
    PeripheralOutput_u  Output;

    unsigned short int SpeedDownAndUpTime;
    unsigned short int SpeakerAndSpeedUpTime;
    unsigned short int SpeedDownKeyTime;
    unsigned short int SpeedUpKeyTime;
    unsigned short int SpeakerKeyTime;
    unsigned short int AlarmTime;

    unsigned short int BackSpeekTime;
    unsigned short int PowerLEDShowTime;

    unsigned char ucPowerLedNum;
    unsigned char ucPowerLedShowPhase;

    unsigned int usRecvLedTmp;
    unsigned char usRecvPowerON_Flag;
    unsigned char usRecvPowerOFF_Flag;

    unsigned char ucRecvData_Flag;
    unsigned char ucRecvPowerValue_Flag;
    unsigned char ucRecvWrite60Bytes_Flag;

    unsigned char ucRecvAlarmNum; //报警次数
    unsigned int  uiRecvPowerValue;   //电量
    unsigned char ucRecvBikeInBack;
    unsigned char ucRecvOrderInCharge;
    unsigned char ucRecvTestFlag;

} Peripheral_str;

extern Peripheral_str strPeripheral;
extern GLOBLE_TIMER_1MS Globle_Timer_1MS;

extern void PeripheralInputHandle(void);
extern void PeripheralOutputHandle(void);
extern void PeripheralHandle(void);
extern void Peripheral_Init(Peripheral_str *p);

