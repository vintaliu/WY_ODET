/*
       "LG_X2445M_V2"  Design By LG_qiuzhi
       V1.0   2021.04
       used mcu  AT32F421C8
*/
//file DirverUart.H

#ifndef __DIRVERUART_H_
#define __DIRVERUART_H_
#include <stdarg.h>
#include <stdio.h>
//#define UsedUartDmaRxData  ��ʱû�е���ͨ��

#define UsedMdaSend  //ʹ��DMA��������
#define RecDmaBufferSize 100

#define UsedUartID_UART1  //

#ifdef  UsedUartID_UART2
#define EVAL_COM  USART2
#define UsedUartIntDef  USART2_IRQn
#define UsedUartMdaChannle  DMA1_Channel4
#else
#define EVAL_COM  USART1
#define UsedUartIntDef  USART1_IRQn
#define UsedUartMdaChannle  DMA1_Channel2
#endif

#define DmaUsartSize 80
extern unsigned char ucaUsarDmaTxdBuffer[DmaUsartSize];

#define USART_FLAG_TC   USART_FLAG_TRAC

extern int fputc(int ch, FILE *f);
//extern void USART1_printf(USART_TypeDef *USARTx, uint8_t *Data,...);
extern void vPrintStsTemInfo(void);
extern void vPrintAdcValue(void);
extern void vSendSysPower(unsigned int uiPower);
// extern void vCalibrationRemote(void);
extern void vSendOneByteOrder(unsigned char ucOrder, unsigned char ucData);
extern void vSendMultBytesOrder(unsigned char ucOrder, unsigned char *ucSendData, unsigned char ucDataLeg);
extern void vSendSingleOrder(unsigned char ucSingleOrder);
extern void vAnalySerialData(void);
#define UsePrintSysInfo
#define UartBaudRate  	    9600

#define RecBufferSize  	    200
#define ErroSendTimes 	    5
//#define WaitFirstTimeAsMs   267
#define ReSendTimeAsMs 	    2000//500
#define ReSendTimeAsTheach  600//500
//SYSTICK ��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8������������ѡ���ڲ�ʱ��Դ72M������SYSTICK��ʱ��Ϊ9M����SYSTICK��ʱ����9M��Ƶ�ʵݼ���
//9000000 / 1000 000
#define  FAC_US  9
#define  TeacherBaudRate  2400
#define  RxBaudRateCompensates  10//
#define  TxBaudRateCompensates  20//

#define  CrcTeacherBaudRate (1.0/TeacherBaudRate)*1000000
#define  RxTeacherBaudRateDelay  (unsigned int)(CrcTeacherBaudRate+RxBaudRateCompensates)
#define  TxTeacherBaudRateDelay  (unsigned int)(CrcTeacherBaudRate+TxBaudRateCompensates)

#define TeacherBaudRatOne  400
#define TeacherPort GPIOA//����������PA12
#define TeacherPin  GPIO_Pins_12//����������PA12

#define SetTeacherPortOfOutPut  {TeacherPort->CRH &=0xFFF0FFFF;TeacherPort->CRH |=0x00010000;}
#define SetTeacherPortOfInPut   {TeacherPort->CRH &=0xFFF0FFFF;TeacherPort->CRH |=0x00080000;}

#define ReadTeacherPinH         (1 == GPIO_ReadInputDataBit(TeacherPort, TeacherPin))

#define SetTeacherPinH GPIO_SetBits(TeacherPort,TeacherPin)
#define SetTeacherPinL GPIO_ResetBits(TeacherPort, TeacherPin)

#define DisEnInterrupt              {EXTI->INTEN &= ~EXTI_INTEN_LN12;}  //�����ж�����
#define EnInterrupt                 {EXTI->INTEN |= EXTI_INTEN_LN12;}  //���ж�����
extern void vSetTeacherPortType(unsigned char ucType);
extern  unsigned char ucTeacherRxCont;
extern unsigned char ucRecFeedBackFlag;
extern unsigned char ucTeacherRecData[RecBufferSize];
extern void vTeacherPortSendByte(unsigned char ucByte);
extern unsigned char ucTeacherPortRevByte(void); //����һ���ֽ�
// extern void vTeacherPortSendStr(unsigned char *ucBuf, unsigned char ucLen); //�����ַ���
extern void vInitSysTick(void);
extern void vSetTeacherPortInt(void);//ʾ�̿ڵĳ�ʼ��
extern unsigned char ucCheckMotoConnect(void);
extern void vAnalyTeacherData(void);
extern void vSendTeacherFeedBack(void);
extern void vAnalyTeacherDataOfTime(void);
extern void vSendMultBytesTeacherOrder(unsigned char ucOrder, unsigned char *ucSendData, unsigned char ucDataLeg);
extern unsigned char ucUartRxData[RecBufferSize];
extern unsigned char ucUartRxCont;
extern unsigned char ucPressLaBa;

#define Motor1Test  0x01
#define Motor2Test  0x02
#define Motor1Forward  0x10
#define Motor2Forward  0x20

#define SendHeard1  		   0xEB
#define SendHeard2  		   0x90
#define HostDefine  		   0xA1//�¿ذ�
#define ShowDefine  		   0xA2//�ϰ���̰�
#define TeacherAdd                    0xA3//����ʾ�����ĵ�ַ
#define OrderPower  		   0x01//����֮ʱ����
#define OrderErro   		   0x02//��������ʾ�屨��
#define OrderInChange  	   0x03//�ڳ���С�����
#define OrderOutChange 	   0x04//���ڳ���С�����
#define BikeInBack		     0x05//�������ڵ�����
#define BikeOutBack    	   0x06//���Ӳ��ڵ�����
#define SpeedGrad      	   0x07//�ٶ�
#define OrderPowerOff  	   0x08//��������ʾ�巢�Ϳ��ػ�����
#define AskSpeedGrad   	   0x09//����ѯ���ٶ�ֵ
#define QuitErro			     0x10//�������״̬
#define Write60Bytes   	   0x11//����ʾ��д��60�ֽڵ�����
#define Read60Bytes    	   0x12//��ȡEEPROM�����ݣ���ʾ�������ط���EEPROM������Ҳ�ø�����
#define WriteOneByte   	   0x13//����ʾ��д�뵥�ֽ�����
#define ReadOneByte    	   0x14//����ʾ����Ҫ���ֽ�����
#define Disp10Led      		 0x15//����LED���
#define OutDispLed      	 0x16//�˳�LED����
#define OrderSpeekP        0x17//��������ȼ�  ��ʾ�嵽���ذ�
#define OrderWatchDogOpen  0x18//����ʾ���𵽿���Ӳ�����Ź�����
#define OrderWatchDogClose 0x19//�ر���ʾ���Ӳ�����Ź�������
#define OrderBreak         0x20//ɲ�������ز���
#define OrderMotor         0x21//�����������
#define OrderReadPare      0x22//�ϴ�������ʾ�̺�
#define OrderSendPare      0x23//�·�ʾ�̲�����������
#define OrderRunTest       0x24//�ϻ�����
#define FirstOrderPowerOn  0x25//��һ�ο�����ʱ�� �������ź�
#define OrderBatteryType  0x31//д������� ��־

#define SendTail1      		0xCC
#define SendTail2      		0x33
#define SendTail3      		0xC3
#define SendTail4      		0x3C//֡β�Ĺ̶�����
#define FeedBack1    		  0xFB//�ظ�֡��־λ

#define LastErroSaveAtEepromAdd   60//�ϴδ������EERPOM�ڵĵ�ַ ֻ��60 01 62
//#define OrderDisLed1    0x0001
//#define OrderDisLed2    0x0002
//#define OrderDisLed3    0x0004
//#define OrderDisLed4    0x0008
//#define OrderDisLed5    0x0010//D0--D4
//#define OrderDisLed6    0x0020//
//#define OrderDisLed7    0x0040
//#define OrderDisLed8    0x0080
//#define OrderDisLed9    0x0100
//#define OrderDisLed10   0x0200


#define UartMainControlBoardAddr       0xA1
#define UartLedShowBoardAddr           0xA2
#define UartHandSetBoardAddr           0xA3

#define Uart_Frame_Header    1
#define Uart_Frame_length    2
#define SCI_Frame_Address    3
#define Uart_Frame_command   4
#define Uart_Frame_Data      5
#define Uart_Frame_Crc 	     6


extern void UART1_RecvData(unsigned char rc_data);
extern void USART1_DataProcess(void);


















#endif
