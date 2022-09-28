
#include "SystemInit.h"

/////***************&&&&&&&&&&&&&&&&&&&&&&&&&***********************************
#define PowerOnSpeedGradeAt  4//���忪���ٶȵ�λ��1��2��3��4��5���������ݣ������ٶȵ�λ    
#define EnableCancelBackCarAlarm    //�Ƿ����������졣�����Ҫȡ����ע������  ��˫б��Ϊǰ�������±����³��� ��--��//#define EnableCanncelBackCarAlarm
/////***************&&&&&&&&&&&&&&&&&&&&&&&&&***********************************
#define DKVersion  0x01
#define BatteryAutoShiBie  ///���ʶ��
#define QianXiUart  

void EEPROM_Byte_Erase(unsigned int addar);
void EEPROM_Byte_Write(unsigned int addar, unsigned char W_data);
unsigned char EEPROM_Byte_Read(unsigned int addr);
void EEPROM_Read(unsigned int addr, unsigned char idata *R_data, unsigned int len);

void vArom_WriteBytes_Fact(unsigned char *pData);//ֱ��д��һ������ �����ٽ���������

//void Vdebug();
unsigned int ADC_GetConversionValue();
void SpeedLedOn(unsigned char num);//��ʾ�ٶ�ָʾ��
void PowerLedOn(unsigned short power);//��ʾ����ָʾ��
void PowerLedNumOn(unsigned char num);//��ʾ����ָʾ��
void vSpeedMiunsPressed(void);//�ٶȼ�
void vSpeedPlusPressed(void);//�ٶȼ�
void VBreathLED(void);//���Ժ�����
void vBuzzerPressed(void);//������
void vPowerPressed(void);//����Դ���ػ�
void vFirstStart(void);//��һ�ο���
//void vCheckInBack(void);//��⵹��
//void vCheckLowPower(void);//��Դ������
//void BEEP_Alarm(void);//����������
void delay(unsigned long i);
void vCheckInCharge(void);//���ģʽ
void vWrite60Bytes(void);//����д��60�ֽ�����

void vStartSucceed(void);//��һ�ο�����һ��
void DealKey(void);//��������
void vDealAd(void);//AD����
void vTestShowLed(unsigned int value); //���ԣ�����ʾ��Ƶ���
void EEPROM_WriteByte(unsigned int u16_addr, unsigned char u8_data); //EEPROMд������
unsigned char EEPROM_ReadByte(unsigned int code *Address);//EEPROM��������
void vAlarmLess18Power();
void vInitLedPin(void);
void vSendDKVersion(unsigned char uiVersion);
void SetPWM1_Out(unsigned int x, unsigned int y);
void PWM1_Init(void);
unsigned char vArom_ReadBytes(unsigned int Address, unsigned char *pData,  unsigned int uiDataLeg);
void vArom_WriteBytes(unsigned int Address, unsigned char pData, unsigned int uiDataLeg);

void InitialUART0_Timer1(unsigned long u32Baudrate) ;

//#define DebugPort  P22D
//#define XorDebugPin    DebugPort = ~DebugPort


extern unsigned int  uiAdVoltage;
extern unsigned char ucSpeedGrad;  //�ٶ�


extern unsigned char ucLessThanLowPower; //��Դ��ѹС��23.740�ı�־
extern unsigned char ucLess18Power;         //��Դ��ѹС��18�ı�־

extern unsigned char ucBuzzerTstFlg; //���������Ա�־



//* EEPROM��С��640 byte */
#define EEPROM_START_ADDRESS_FACT_PARE  0       //������������д��ַ  //�������һ�㲻��Ҫд�ģ�һ��Ҫ��ȫ���ٶ�ֵ����һ��������ͬһ��ҳ��
#define EEPROM_START_ADDRESS_FACT_PARE_Legth  60//������������д��ַ

#define SPEED_ADDRESS                        61//�ٶȰ���ֵ�� ROM�еĵ�ַ

#define EEPRO_One_Byte_ADDRESS  62//�����ֽ����ݵ�ַ

#define WriteBatteryTypeFlag_Addr      2//д������ͱ�־λ


//EF,0A,1C,07,07,07,62,32,05,07,05,05,05,06,14,00,00,00,00,44,14,2D,14,00,E8,03,00,00,00,00,00,00,29,
//00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,55,55,AA
#define RW_COUNT              60


//* ��Դ���� *
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


#define POWER_SHOW1_OFF   P21D = 0//P2.1    //WH�޸�IO��ʼ��
#define POWER_SHOW1_ON     P21D = 1
#define POWER_SHOW2_OFF   P24D = 0//P2.4
#define POWER_SHOW2_ON     P24D = 1
#define POWER_SHOW3_OFF   P27D = 0//P2.7
#define POWER_SHOW3_ON     P27D = 1
#define POWER_SHOW4_OFF   P26D = 0//P2.6
#define POWER_SHOW4_ON     P26D = 1
#define POWER_SHOW5_OFF   P10D = 0//P1.0
#define POWER_SHOW5_ON     P10D = 1

#define SPEED_STEP1_OFF  P00D = 0//P0.0
#define SPEED_STEP1_ON    P00D = 1
#define SPEED_STEP2_OFF  P01D = 0//P0.1
#define SPEED_STEP2_ON    P01D = 1
#define SPEED_STEP3_OFF  P14D = 0//P1.4
#define SPEED_STEP3_ON    P14D =1
#define SPEED_STEP4_OFF  P12D = 0//P1.2
#define SPEED_STEP4_ON    P12D = 1
#define SPEED_STEP5_OFF  P11D = 0//P1.1
#define SPEED_STEP5_ON    P11D = 1


//#define SPEEKER_CTRL_OFF   P10 &=0
//#define SPEEKER_CTRL_ON    P10 |=1
#define SPEEKER_CTRL_OFF
#define SPEEKER_CTRL_ON

#define SPEED_LED_0  {SPEED_STEP1_OFF;SPEED_STEP2_OFF;SPEED_STEP3_OFF;SPEED_STEP4_OFF;SPEED_STEP5_OFF;}
#define SPEED_LED_1  {SPEED_STEP1_ON;SPEED_STEP2_OFF;SPEED_STEP3_OFF;SPEED_STEP4_OFF;SPEED_STEP5_OFF;}
#define SPEED_LED_2  {SPEED_STEP1_ON;SPEED_STEP2_ON;SPEED_STEP3_OFF;SPEED_STEP4_OFF;SPEED_STEP5_OFF;}
#define SPEED_LED_3  {SPEED_STEP1_ON;SPEED_STEP2_ON;SPEED_STEP3_ON;SPEED_STEP4_OFF;SPEED_STEP5_OFF;}
#define SPEED_LED_4  {SPEED_STEP1_ON;SPEED_STEP2_ON;SPEED_STEP3_ON;SPEED_STEP4_ON;SPEED_STEP5_OFF;}
#define SPEED_LED_5  {SPEED_STEP1_ON;SPEED_STEP2_ON;SPEED_STEP3_ON;SPEED_STEP4_ON;SPEED_STEP5_ON;}

#define POWER_SHOW_0  {POWER_SHOW1_OFF;POWER_SHOW2_OFF;POWER_SHOW3_OFF;POWER_SHOW4_OFF;POWER_SHOW5_OFF;}
#define POWER_SHOW_1  {POWER_SHOW1_ON;POWER_SHOW2_OFF;POWER_SHOW3_OFF;POWER_SHOW4_OFF;POWER_SHOW5_OFF;}
#define POWER_SHOW_2  {POWER_SHOW1_ON;POWER_SHOW2_ON;POWER_SHOW3_OFF;POWER_SHOW4_OFF;POWER_SHOW5_OFF;}
#define POWER_SHOW_3  {POWER_SHOW1_ON;POWER_SHOW2_ON;POWER_SHOW3_ON;POWER_SHOW4_OFF;POWER_SHOW5_OFF;}
#define POWER_SHOW_4  {POWER_SHOW1_ON;POWER_SHOW2_ON;POWER_SHOW3_ON;POWER_SHOW4_ON;POWER_SHOW5_OFF;}
#define POWER_SHOW_5  {POWER_SHOW1_ON;POWER_SHOW2_ON;POWER_SHOW3_ON;POWER_SHOW4_ON;POWER_SHOW5_ON;}

#define RedPowerLed  P21D
#define XorRedPowerLed    RedPowerLed = ~RedPowerLed

#define POWER_ON_KEY_PRESSED  (1==(P23D & 1))//PB4�˴�һ��Ҫ��PB4ָ���Ĵ������ֵ��

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
        unsigned short int CheckIDele :1;
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
        unsigned char SpeedChange: 1;	//����ٶ���Ҫ�޸ı�ʾλ

        unsigned char SpeedModify: 1;	//����ٶȵ���
        unsigned char PowerChange: 1;	//�л��������
        unsigned char SpeedDownKeyPressFlag: 1;//����ٶȵ��ڵ�ʱ���ٶȼ�����������Ч��
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
}PeripheralOutput_u;

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

    unsigned char ucRecvAlarmNum; //��������
    unsigned int  uiRecvPowerValue;   //����
    unsigned char ucRecvBikeInBack;
    unsigned char ucRecvOrderInCharge;
    unsigned char ucRecvTestFlag;

}Peripheral_str;

extern Peripheral_str strPeripheral;
extern GLOBLE_TIMER_1MS Globle_Timer_1MS;

extern void PeripheralInputHandle(void);
extern void PeripheralOutputHandle(void);
extern void PeripheralHandle(void);
extern void Peripheral_Init(Peripheral_str *p);

