
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

//#define C_CtlSet90393_Sda_AsInPut          {Mlx_IIC_SDA_Port->CRL &= 0xFFFFFFF0;Mlx_IIC_SDA_Port->CRL |= 0x00000008;}	//SDA_Port输入模式
//#define C_CtlSet90393_Sda_AsOutPut         {Mlx_IIC_SDA_Port->CRL &= 0xFFFFFFF0;Mlx_IIC_SDA_Port->CRL |= 0x00000001;}  //SDA_Port输出模式 Mlx_IIC_SDA_Port->CRL |= 0x00000007//

//#define C_CtlSet90393_Sda_AsInPut          {Mlx_IIC_SDA_Port->CRL &= 0xFFFFF0FF;Mlx_IIC_SDA_Port->CRL |= 0x00000800;}//{Mlx_IIC_SDA_Port->CRL &= 0xFFFFFFF0;Mlx_IIC_SDA_Port->CRL |= 0x00000008;}	//SDA_Port输入模式
//#define C_CtlSet90393_Sda_AsOutPut         {Mlx_IIC_SDA_Port->CRL &= 0xFFFFF0FF;Mlx_IIC_SDA_Port->CRL |= 0x00000300;}//{Mlx_IIC_SDA_Port->CRL &= 0xFFFFFFF0;Mlx_IIC_SDA_Port->CRL |= 0x00000001;}  //SDA_Port输出模式

#define C_CtlSet90393_Sda_AsInPut          {Mlx_IIC_SDA_Port->PUPDR |= 0x00000100;Mlx_IIC_SDA_Port->MODER &= 0xFFFFFCFF;}//{Mlx_IIC_SDA_Port->CRL &= 0xFFFFFFF0;Mlx_IIC_SDA_Port->CRL |= 0x00000008;}	//SDA_Port输入模式
#define C_CtlSet90393_Sda_AsOutPut         {Mlx_IIC_SDA_Port->PUPDR &= 0xFFFFFCFF;Mlx_IIC_SDA_Port->MODER |= 0x00000100;}//{Mlx_IIC_SDA_Port->CRL &= 0xFFFFFFF0;Mlx_IIC_SDA_Port->CRL |= 0x00000001;}  //SDA_Port输出模式

//#define InDebugMlx90393 0//测试写逻辑与校验--写数据到非易失RAM中
//#define IfEnableCheckUseFulDataInIC   //是否去检测存储在芯片中 用户数据是否正确
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

#define MaxCmdErroTimes   6//最大错误容限
#define IfMlxStatusByteErroWaiteNextCmdTime_ms   10 //如果MLX 读出的数据有错，允许下次延时操作的时间定义

#define MlxErro_InCallMemoryCall   0x01
#define MlxErro_InReadRegErro      0x02
#define MlxErro_InCmdExitErro      0x03
#define MlxErro_InWriteRegErro     0x04
#define MlxErro_InStoreRegErro     0x05
#define MlxErro_CheckUseFulDataErro 0x06
#define MlxInReg_Mode              1
#define MlxInNoneReg_Mode          0
#define MlxCycOprtationIcDelayTime 100//定义一下，循环读IC时间计数器

#define MlxErro_StatusByteErro     0xFF

#define FALSE                      0
#define TRUE                       1


#pragma pack(1)
struct strMlxStateAndSetBit
{
	unsigned char bitCheckMlxIcSaveUseFulDataOk : 1;//检测芯片用户存储是否有用的信息
	unsigned char bitStartTimerOpMlx : 1;//系统是否允许定时器非阻塞式操作芯片
	
};
struct strMLX90393CmdAdd
{
	unsigned char ucMLX90393_WriteCmdAdd;//90393写一个命令的地址--因器件的不同这里的地址--是不同的
	unsigned char ucMLX90393_ReadCmdAdd;//90393写一个命令的地址--因器件的不同这里的地址--是不同的
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
    union uniRecUsefulMemoryData__  uniRecUsefulMemoryData;//中间值 左 右 上 下 各2个坐标值信息
    unsigned char ucMlx90393State;//Mlx90393所处的状态
    unsigned char ucMlx90393StatusByte;//Mlx90393 状态字
    unsigned char ucMlx90393ErroType;//Mlx90393所处的状态
    unsigned short usMlx90393StatusErroTimes;//在正常读取数据的过程中 如果状态帧有错时候的累加器
    struct strMlxStateAndSetBit strStateAndSet;//芯片状态或设置位信息
    struct strMLX90393CmdAdd strCmdAdr;//芯片的动态地址-因芯片的型号不一定，地址也有所变化的
    unsigned char ucMlx90393AssemblyN_DownFlag;//磁极向下装配标志
    //struct strSampleInfo_ strSampleData;
    unsigned char ucUpDataNewFlag;//是否有数据更新标志
    union uniUni1_//共用一个数据结构体
    {
        unsigned short usData;//当前读取到的数据
        struct strData1_
        {
            unsigned char ucData1: 8;//一般地，低位在前
            unsigned char ucData2: 8;
        }strData;
    } uniusRecXdata;
    union uniUni2_//共用一个数据结构体
    {
        unsigned short usData;//当前读取到的数据
        struct strData2_
        {
            unsigned char ucData1: 8;//一般地，低位在前
            unsigned char ucData2: 8;
        }strData;
    } uniusRecYdata;
};

struct strMlx9039InTimerOp//用于静态的 局部变量
{
    unsigned char ucClkCnt;//当前步骤的时钟计数器
    unsigned char ucSetpsModeIndex;//何种操作，在当前模式下
    unsigned char ucRecDataTemp;//当前读取数据临时值
    unsigned short usCycOprtationIcCnt;//循环间隔操作芯片计数器
    unsigned char *ucPtr;//做一个数据转递用
} ;

#pragma pack()



#endif

