
void UART1_SendByte(unsigned char senddata);//����һ���ֽ�
void UART1_SendString(unsigned char *Data, unsigned int len); //����һ���ַ���
unsigned char UART1_ReceiveByte(void);
void vSendSpeedGrad(unsigned char ucSpeed);
void vSendBuzzerOrder(unsigned char arg);
//void vSendRockAd(unsigned char ucSpeed);//�����ٶ�   �˺���û��ʵ����ô�����ٶȣ�������
void vSendFeedBack(void);//�����յ�����֡
//void vAnalySerialData(void);
void vSend60Bytes(void);
void vSendPowerOn(void);
void vSendOneBytes(void);//�����ط���һ��Bytes


#define ErroSendTimes  4
#define	BufferSize  80     //���ջ������Ĵ�С
#define FeedBack1 0xFB

extern unsigned char ucEnableSoftDogFlg;
//extern unsigned char ucOrderInCharge; //�Ƿ��ڳ��
//extern unsigned char ucBikeInBack; //�Ƿ��ڵ�����


extern unsigned char idata ucEepromBuf[60];
//extern unsigned char xdata ucUartRxData[RecBufferSize];

extern unsigned char ucSpeedGrad;

extern unsigned char ucRecDataCodeAndData[2];


#define SendHeard1           0xEB
#define SendHeard2           0x90
#define SendTail1             0xCC
#define SendTail2             0x33
#define SendTail3             0xC3
#define SendTail4             0x3C

#define HostDefine        0xA1 //�¿ذ�
#define ShowDefine        0xA2 //�ϰ���̰�
#define OrderPower        0x01 //��������
#define OrderErro         0x02 //��������ʾ�屨��
#define OrderInCharge     0x03 //�ڳ���С�����
#define OrderOutCharge    0x04 //���ڳ���С�����
#define BikeInBack        0x05 //�������ڵ�����
#define BikeOutBack       0x06 //���Ӳ��ڵ�����
#define SpeedGrad         0x07 //�ٶ�
#define OrderPowerOnOff   0x08 //��ʾ�������ط��Ϳ��ػ�����
#define AskSpeedGrad      0x09 //����ѯ���ٶ�ֵ
#define LedStatus         0x0A //������״ֵ̬�·������� 20190807

#define QuitErro          0x10 //�������״̬
#define Write60Bytes      0x11 //����ʾ��д��60�ֽڵ�����
#define Read60Bytes       0x12 //��ȡEEPROM�����ݣ���ʾ�������ط���EEPROM������Ҳ�ø�����
#define WriteOneByte      0x13 //����ʾ��д�뵥�ֽ�����
#define ReadOneByte       0x14 //����ʾ����Ҫ���ֽ�����
#define Disp10Led         0x15 //����LED���������
#define OutDispLed        0x16 //�˳�LED���������
#define OrderSpeekP        0x17//��������ȼ�  ��ʾ�嵽���ذ�
#define OrderWatchDogOpen  0x18//����ʾ���𵽿���Ӳ�����Ź�����
#define OrderWatchDogClose 0x19//�ر���ʾ���Ӳ�����Ź�������
#define FirstOrderPowerOn  0x25//��һ�ο�����ʱ�� �������ź�

#define ReadDKVersion      0x27//ѯ�ʰ�����ʾ��ĳ���汾
#define OrderBatteryType  0x31//д������� ��־

#define UartMainControlBoardAddr       0xA1
#define UartLedShowBoardAddr           0xA2
#define UartHandSetBoardAddr           0xA3

#define Uart_Frame_Header    1
#define Uart_Frame_length    2
#define SCI_Frame_Address    3
#define Uart_Frame_command   4
#define Uart_Frame_Data      5
#define Uart_Frame_Crc 	     6



extern unsigned char xdata Uart1RecvBuf[BufferSize];
void UART1_RecvData(unsigned char rc_data);
void USART1_DataProcess(void);
void vSendLedStatus(unsigned char ucStatus, unsigned char ucData); //���Ͱ�����״ֵ̬
