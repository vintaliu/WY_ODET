/***********************

***********************/




void vSysSourceInit();
void GPIO_Init();//I/O�ڳ�ʼ��
void Uart_Init();//UART����
void Time0_Init();//��ʱ��2PWM�������


#define NO_USE 2
#define TRUE   1
#define FALSE  0
#define RESET  0
#define SET    1

#define TH0_INIT        0xFA //��ʱ��һ�����ж�һ�Σ�
#define TL0_INIT        0xCB

#define RxBufferSize 64

#define LabaKeyPressSpeekTime  2910//790//1999
#define LabaKeyPressSpeekPulse  1302//235//  finish

#define SpeedKeyLimitTime    63000//16100
#define SpeedKeyLimitPulse   1171

#define SpeedKeyTime     8045 //
#define SpeedKeyPulse    1229 //  finish

#define InBackTime    4220
#define InBackPulse   927

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



