/***********************

***********************/




void vSysSourceInit();
void GPIO_Init();//I/O�ڳ�ʼ��
void Uart_Init();//UART����
void Time0_Init();//��ʱ��2PWM�������
void vInitAdc(void);

#define NO_USE 2
#define TRUE   1
#define FALSE  0
//#define RESET  0    //��MC51F003A4_COMMON.h��ʼ���ظ�
//#define SET    1

#define TH0_INIT        0xFA //��ʱ��һ�����ж�һ�Σ�
#define TL0_INIT        0xCB

#define RxBufferSize 64

#define LabaKeyPressSpeekTime  726//1999
#define LabaKeyPressSpeekPulse  325//  finish

#define SpeedKeyLimitTime    16100
#define SpeedKeyLimitPulse   500

#define SpeedKeyTime     2010 //
#define SpeedKeyPulse    305 //  finish

#define InBackTime    1060
#define InBackPulse   233

#define BreathLEDTime  1000


//#define LabaKeyPressSpeekTime  0x07CF
//#define LabaKeyPressSpeekPulse  0x190

//#define SpeedKeyLimitTime    0x07CF
//#define SpeedKeyLimitPulse   0x05F0

//#define SpeedKeyTime     0x07CF  //1999
//#define SpeedKeyPulse    0x0190

//#define InBackTime    500
//#define InBackPulse   110

//#define BreathLEDTime  1000



