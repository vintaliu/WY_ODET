
#ifndef _MLX90393_H_
#define _MLX90393_H_

#define UsedMlx90393

extern void vInMeasurementNormal(void);
extern void vWriteRamOrNoramThings(void);
extern unsigned char ucFucCheck90393Add(void);
extern void vSetUpMlx90393(void);
extern volatile struct strMlxInfoData g_strMlxInfoData;
#define EnAbleMlxTimerReadTnterrupt     {TMR14->CTRL1 |= 0x0001;g_strMlxInfoData.strStateAndSet.bitStartTimerOpMlx = TRUE;}
#define DisEnAbleMlxTimerReadTnterrupt  {TMR14->CTRL1 &= 0x03FE;g_strMlxInfoData.strStateAndSet.bitStartTimerOpMlx = FALSE;}
#define ReadMlxReadTimerState            TMR14->CTRL1


//////////////////////////////////////////////////
#define Mlx_IIC_SDA_Port  GPIOA
#define Mlx_IIC_SDA_Pin   GPIO_Pins_3

#define Mlx_IIC_SCL_Port  GPIOB
#define Mlx_IIC_SCL_Pin   GPIO_Pins_12

#define C_Ctl90393_SCL_L        Mlx_IIC_SCL_Port->BRE = Mlx_IIC_SCL_Pin
#define C_Ctl90393_SCL_H        Mlx_IIC_SCL_Port->BSRE = Mlx_IIC_SCL_Pin

#define C_Ctl90393_SDA_L        Mlx_IIC_SDA_Port->BRE = Mlx_IIC_SDA_Pin
#define C_Ctl90393_SDA_H        Mlx_IIC_SDA_Port->BSRE = Mlx_IIC_SDA_Pin

#define C_Ctl90393_Read_SDA     Mlx_IIC_SDA_Port->IPTDT & Mlx_IIC_SDA_Pin

//#define C_CtlSet90393_Sda_AsInPut          {Mlx_IIC_SDA_Port->CRL &= 0xFFFFFFF0;Mlx_IIC_SDA_Port->CRL |= 0x00000008;}	//SDA_Port����ģʽ
//#define C_CtlSet90393_Sda_AsOutPut         {Mlx_IIC_SDA_Port->CRL &= 0xFFFFFFF0;Mlx_IIC_SDA_Port->CRL |= 0x00000001;}  //SDA_Port���ģʽ Mlx_IIC_SDA_Port->CRL |= 0x00000007//

//#define C_CtlSet90393_Sda_AsInPut          {Mlx_IIC_SDA_Port->CRL &= 0xFFFFF0FF;Mlx_IIC_SDA_Port->CRL |= 0x00000800;}//{Mlx_IIC_SDA_Port->CRL &= 0xFFFFFFF0;Mlx_IIC_SDA_Port->CRL |= 0x00000008;}	//SDA_Port����ģʽ
//#define C_CtlSet90393_Sda_AsOutPut         {Mlx_IIC_SDA_Port->CRL &= 0xFFFFF0FF;Mlx_IIC_SDA_Port->CRL |= 0x00000300;}//{Mlx_IIC_SDA_Port->CRL &= 0xFFFFFFF0;Mlx_IIC_SDA_Port->CRL |= 0x00000001;}  //SDA_Port���ģʽ

#define C_CtlSet90393_Sda_AsInPut          {Mlx_IIC_SDA_Port->PUPDR |= 0x00000100;Mlx_IIC_SDA_Port->MODER &= 0xFFFFFCFF;}//{Mlx_IIC_SDA_Port->CRL &= 0xFFFFFFF0;Mlx_IIC_SDA_Port->CRL |= 0x00000008;}	//SDA_Port����ģʽ
#define C_CtlSet90393_Sda_AsOutPut         {Mlx_IIC_SDA_Port->PUPDR &= 0xFFFFFCFF;Mlx_IIC_SDA_Port->MODER |= 0x00000100;}//{Mlx_IIC_SDA_Port->CRL &= 0xFFFFFFF0;Mlx_IIC_SDA_Port->CRL |= 0x00000001;}  //SDA_Port���ģʽ

//#define InDebugMlx90393 0//����д�߼���У��--д���ݵ�����ʧRAM��
//#define IfEnableCheckUseFulDataInIC   //�Ƿ�ȥ���洢��оƬ�� �û������Ƿ���ȷ
//////////////////////////////////////////////////


#define MLX90393slaveAddAndRead_011     0x19
#define MLX90393slaveAdd_011            0x18        //--

#define MLX90393slaveAddAndRead_012     0x21
#define MLX90393slaveAdd_012            0x20        //--

#define MLX90393slaveAddAndRead_013     0x29
#define MLX90393slaveAdd_013            0x28        //--

#define MLX90393slaveAddAndRead_014     0x31
#define MLX90393slaveAdd_014            0x30        //--

#define MLX90393_BurstReadCommand   0x16        //-yx-
#define MLX90393_WeekUpCommand      0x26        //-yx-
#define MLX90393_SingleReadCommand  0x36        //-yx-
#define MLX90393_ReadMeausreCommand 0x46        //-yx-
#define MLX90393_ReadRegCommand     0x50
#define MLX90393_WriteRegCommand    0x60
#define MLX90393_ExitCommand        0x80
#define MLX90393_MemRecallCommand   0xd0
#define MLX90393_MemStoreCommand    0xe0
#define MLX90393_ResetCommand       0xf0
#define MLX90393_StartMeasurement   0x36        //-yx-


#define UseSetMlxReg0Data           0x801C
//#define UseSetMlxReg0Data           0x805C
#define UseSetMlxReg1Data           0xE180
#define UseSetMlxReg2Data           0x01E5

#define MaxCmdErroTimes   6//����������
#define IfMlxStatusByteErroWaiteNextCmdTime_ms   10 //���MLX �����������д������´���ʱ������ʱ�䶨��

#define MlxErro_InCallMemoryCall   0x01
#define MlxErro_InReadRegErro      0x02
#define MlxErro_InCmdExitErro      0x03
#define MlxErro_InWriteRegErro     0x04
#define MlxErro_InStoreRegErro     0x05
#define MlxErro_CheckUseFulDataErro 0x06
#define MlxInReg_Mode              1
#define MlxInNoneReg_Mode          0
#define MlxCycOprtationIcDelayTime 100//����һ�£�ѭ����ICʱ�������

#define MlxErro_StatusByteErro     0xFF

#define FALSE                      0
#define TRUE                       1


#pragma pack(1)
struct strMlxStateAndSetBit
{
	unsigned char bitCheckMlxIcSaveUseFulDataOk : 1;//���оƬ�û��洢�Ƿ����õ���Ϣ
	unsigned char bitStartTimerOpMlx : 1;//ϵͳ�Ƿ�����ʱ��������ʽ����оƬ
	
};
struct strMLX90393CmdAdd
{
	unsigned char ucMLX90393_WriteCmdAdd;//90393дһ������ĵ�ַ--�������Ĳ�ͬ����ĵ�ַ--�ǲ�ͬ��
	unsigned char ucMLX90393_ReadCmdAdd;//90393дһ������ĵ�ַ--�������Ĳ�ͬ����ĵ�ַ--�ǲ�ͬ��
};

union uniRecUsefulMemoryData__
{
    unsigned short usRecUsefulMemoryData[10];
    struct strMlxAdjust__
    {
        unsigned short usMiddleX : 16;
        unsigned short usMiddleY : 16;
        unsigned short usLeftX : 16;
        unsigned short usLeftY : 16;
        unsigned short usRightX : 16;
        unsigned short usRightY : 16;
        unsigned short usUpX : 16;
        unsigned short usUpY : 16;
        unsigned short usDownX : 16;
        unsigned short usDownY : 16;
    } strMlxAdjust;
};

struct strMlxInfoData
{
    union uniRecUsefulMemoryData__  uniRecUsefulMemoryData;//�м�ֵ �� �� �� �� ��2������ֵ��Ϣ
    unsigned char ucMlx90393State;//Mlx90393������״̬
    unsigned char ucMlx90393StatusByte;//Mlx90393 ״̬��
    unsigned char ucMlx90393ErroType;//Mlx90393������״̬
    unsigned short usMlx90393StatusErroTimes;//��������ȡ���ݵĹ����� ���״̬֡�д�ʱ����ۼ���
    struct strMlxStateAndSetBit strStateAndSet;//оƬ״̬������λ��Ϣ
    struct strMLX90393CmdAdd strCmdAdr;//оƬ�Ķ�̬��ַ-��оƬ���ͺŲ�һ������ַҲ�����仯��
    unsigned char ucMlx90393AssemblyN_DownFlag;//�ż�����װ���־
    //struct strSampleInfo_ strSampleData;
    unsigned char ucUpDataNewFlag;//�Ƿ������ݸ��±�־
    union uniUni1_//����һ�����ݽṹ��
    {
        unsigned short usData;//��ǰ��ȡ��������
        struct strData1_
        {
            unsigned char ucData1: 8;//һ��أ���λ��ǰ
            unsigned char ucData2: 8;
        }strData;
    } uniusRecXdata;
    union uniUni2_//����һ�����ݽṹ��
    {
        unsigned short usData;//��ǰ��ȡ��������
        struct strData2_
        {
            unsigned char ucData1: 8;//һ��أ���λ��ǰ
            unsigned char ucData2: 8;
        }strData;
    } uniusRecYdata;
};

struct strMlx9039InTimerOp//���ھ�̬�� �ֲ�����
{
    unsigned char ucClkCnt;//��ǰ�����ʱ�Ӽ�����
    unsigned char ucSetpsModeIndex;//���ֲ������ڵ�ǰģʽ��
    unsigned char ucRecDataTemp;//��ǰ��ȡ������ʱֵ
    unsigned short usCycOprtationIcCnt;//ѭ���������оƬ������
    unsigned char *ucPtr;//��һ������ת����
} ;

#pragma pack()



#endif

