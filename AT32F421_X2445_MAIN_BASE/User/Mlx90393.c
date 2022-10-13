

#include "DriverConst.h"

volatile struct strMlxInfoData g_strMlxInfoData = {0};//Mlx90393所用的数据结构体，全局变量

void vMlx90393WaitTimes()//在阻塞操作时候 对一个时间的延时。可动态调试调整 DelayTime CLK->100KHz没有问题
{
#define DelayTime 200
    unsigned short usDelay = DelayTime;
    for(; usDelay > 0; usDelay--);
}

void vMlx90393WaitTimes_ms(unsigned short usDelayTimes)//非精确一个延时
{
#define DefData 20000
    unsigned short usDelay = DefData;
    do
    {
        for(; usDelay > 0; usDelay--);
        if(usDelayTimes)usDelayTimes--;
    }
    while(usDelayTimes);
}
//读取一个字节，阻塞的方式
unsigned char ucMlx90393ReadOnByte(void)
{
    /*read data*/
    unsigned char i;
    unsigned char ReadByte;
    C_CtlSet90393_Sda_AsInPut;  //Set-->SDA As InPutMode
    for(i = 0; i < 8; i++)
    {
        C_Ctl90393_SCL_H;
        ReadByte = ReadByte << 1;
        vMlx90393WaitTimes();
        if(C_Ctl90393_Read_SDA) ReadByte |= 0x01;
        else  ReadByte &= ~0x01;
        C_Ctl90393_SCL_L;
        vMlx90393WaitTimes();
    }
    return (ReadByte);
}

//90393 起始位-阻塞的方式
void vMlx90393IicStart(void)
{
    /*IIC START*/
    C_CtlSet90393_Sda_AsOutPut;  //Set-->SDA As OutPutMode
    C_Ctl90393_SDA_H;
    C_Ctl90393_SCL_H;
    vMlx90393WaitTimes();
    C_Ctl90393_SDA_L;
    vMlx90393WaitTimes();
    C_Ctl90393_SCL_L;
    vMlx90393WaitTimes();
}

//90393 停止位-阻塞的方式
void vMlx90393IicStop(void)
{
    /*IIC stop*/
    C_CtlSet90393_Sda_AsOutPut;  //Set-->SDA As OutPutMode
    C_Ctl90393_SDA_L;
    C_Ctl90393_SCL_L;
    vMlx90393WaitTimes();
    C_Ctl90393_SCL_H;
    vMlx90393WaitTimes();
    C_Ctl90393_SDA_H;
}

//90393 读取一个Ack信号 并返回一个状态位--一般用于写命令之后
unsigned char ucMlx90393ReadAckBit(void)
{
    /*wait ACK*/
    unsigned char staturs;
    C_CtlSet90393_Sda_AsInPut;  //Set-->SDA As InPutMode
    C_Ctl90393_SCL_H;
    vMlx90393WaitTimes();
    staturs = C_Ctl90393_Read_SDA;//PIN_getInputValue(IIC_SDA);
    C_Ctl90393_SCL_L;
    vMlx90393WaitTimes();
    return (staturs);
}

//90393 写一个字节
unsigned char ucMlx90393WriteOneByte(unsigned char WriteByte)
{
    unsigned char i;
    unsigned char staturs;
    C_CtlSet90393_Sda_AsOutPut;  //Set-->SDA As OutPutMode
    for(i = 0; i < 8; i++)
    {
        if(WriteByte & 0x80)C_Ctl90393_SDA_H;
        else  C_Ctl90393_SDA_L;
        vMlx90393WaitTimes();
        C_Ctl90393_SCL_H;
        vMlx90393WaitTimes();
        C_Ctl90393_SCL_L;
        vMlx90393WaitTimes();
        WriteByte = WriteByte << 1;
    }
    staturs = ucMlx90393ReadAckBit();//写完数据后，要求读一下ACK 信号
    return (staturs);
}

//90393 写一下 ACK信号  一般用于数据读取完成后再操作的
void vMlx90393WriteAck(void)
{
    /*write ACK*/
    C_CtlSet90393_Sda_AsOutPut;  //Set-->SDA As OutPutMode
    C_Ctl90393_SDA_L;
    vMlx90393WaitTimes();
    //IO_SDA_OUT;
    C_Ctl90393_SCL_H;
    vMlx90393WaitTimes();
    C_Ctl90393_SCL_L;
    vMlx90393WaitTimes();
}

//读一个Nack信号
void vMlx90393ReadNack(void)
{
    /*write ACK*/
    C_CtlSet90393_Sda_AsOutPut;  //Set-->SDA As OutPutMode
    C_Ctl90393_SDA_H;
    //IO_SDA_OUT;
    C_Ctl90393_SCL_H;
    vMlx90393WaitTimes();
    C_Ctl90393_SCL_L;
    vMlx90393WaitTimes();
}

//用阻塞的方式 读寄存器
unsigned short usMlx90393ReadReg(unsigned char RegAddress, unsigned char SlaveAdd)
{
    unsigned int Data;
    RegAddress = RegAddress << 2;
    vMlx90393IicStart();
    ucMlx90393WriteOneByte(SlaveAdd);
    ucMlx90393WriteOneByte(MLX90393_ReadRegCommand);
    ucMlx90393WriteOneByte(RegAddress);

    vMlx90393IicStart();
    SlaveAdd	|= 0x01; 			//read data slave address
    ucMlx90393WriteOneByte(SlaveAdd);

    g_strMlxInfoData.ucMlx90393StatusByte = ucMlx90393ReadOnByte();
    /*wait NACK*/
    vMlx90393WriteAck();

    Data = ucMlx90393ReadOnByte();
    /*wait NACK*/
    vMlx90393WriteAck();
    Data = Data << 8;
    Data |= ucMlx90393ReadOnByte();
    /*wait NACK*/
    vMlx90393WriteAck();
    /*IIC stop*/
    vMlx90393IicStop();
    return (Data);
}

//向90393写一个命令
unsigned char ucMLX90393WriteCommand(unsigned char ucWriteCmd, unsigned char ucSlaveAdd)
{
    unsigned char ucReadStatusData;//读取出来的 状态字
    vMlx90393IicStart();
    ucMlx90393WriteOneByte(ucSlaveAdd);
    ucMlx90393WriteOneByte(ucWriteCmd);

    vMlx90393IicStart();
    ucSlaveAdd |= 0x01; //read data slave address
    ucMlx90393WriteOneByte(ucSlaveAdd);

    ucReadStatusData = ucMlx90393ReadOnByte();
    vMlx90393WriteAck();
    /*IIC stop*/
    vMlx90393IicStop();
    return (ucReadStatusData);
}

unsigned char ucMLX90393WriteReg(unsigned char ucRegAddress, unsigned short usWriteData, unsigned char ucSlaveAdd)
{
    unsigned char ucReadStatusData;//读取出来的 状态字
    vMlx90393IicStart();
    ucMlx90393WriteOneByte(ucSlaveAdd);
    ucMlx90393WriteOneByte(MLX90393_WriteRegCommand);
    ucMlx90393WriteOneByte(usWriteData >> 8);
    ucMlx90393WriteOneByte(usWriteData);
    ucMlx90393WriteOneByte(ucRegAddress << 2);
    vMlx90393IicStart();
    ucSlaveAdd |= 0x01; //read data slave address
    ucMlx90393WriteOneByte(ucSlaveAdd);
    ucReadStatusData = ucMlx90393ReadOnByte();
    vMlx90393WriteAck();
    /*IIC stop*/
    vMlx90393IicStop();
    return(ucReadStatusData);
}

//命令MLX90393 重新从非易失存储器，加载到易失存储器中  返回真 出错了
unsigned char ucCheckReadMlxStatusErro(unsigned char ucMlxStatusData, unsigned char ucCmdMode)
{
    if(0xFF == ucMlxStatusData)return 1;//如果一个所有的CLK周期里，SDA被长时间地拉到高电平，说明数据位是错的
    else if(0x10 & ucMlxStatusData)return 1;//如果状态寄存器有错误存在的
    if(MlxInReg_Mode == ucCmdMode)//如果是读寄存器里中的数据
    {
        if(0x00 == ucMlxStatusData)return 0;
        else if(0x10 & ucMlxStatusData)return 1;
    }
    else
    {
        if(0x00 == ucMlxStatusData)return 1;//如果一个所有的CLK周期里，SDA被长时间地拉到高电平，说明数据位是错的
    }
    return 0;
}

//写命令并检测数据是否是出错的，如果出错允许强制多次写  最多时间为 由MaxCmdErroTimes定义的
void vCmdMlxAndWaiteErroTimes(unsigned char command, unsigned char SlaveAdd, unsigned char ucErroType)
{
    unsigned char ucReadCycCnt = 0;
    for(ucReadCycCnt = 0; ucReadCycCnt < MaxCmdErroTimes; ucReadCycCnt++)//最大允许错误计数
    {
        g_strMlxInfoData.ucMlx90393StatusByte = ucMLX90393WriteCommand(command, SlaveAdd);//写一次数据
        if(ucCheckReadMlxStatusErro(g_strMlxInfoData.ucMlx90393StatusByte, MlxInReg_Mode))
        {
            g_uiTimeNumCont1ms = IfMlxStatusByteErroWaiteNextCmdTime_ms;
            vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
        }
        else return;
    }
    if(ucReadCycCnt > 4)g_strMlxInfoData.ucMlx90393ErroType = ucErroType;
}

//用户写芯片寄存器，并等待下
void vCmdMlxWriteRegAndWaite(unsigned char RegAddress, unsigned short usWriteData , unsigned char SlaveAdd, unsigned char ucErroType)
{
    unsigned char ucReadCycCnt = 0;
    for(ucReadCycCnt = 0; ucReadCycCnt < MaxCmdErroTimes; ucReadCycCnt++)
    {
        g_strMlxInfoData.ucMlx90393StatusByte = ucMLX90393WriteReg(RegAddress, usWriteData, SlaveAdd);
        if(ucCheckReadMlxStatusErro(g_strMlxInfoData.ucMlx90393StatusByte, MlxInReg_Mode))
        {
            g_uiTimeNumCont1ms = IfMlxStatusByteErroWaiteNextCmdTime_ms;
            vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
        }
        else return;
    }
    if(ucReadCycCnt > 4)g_strMlxInfoData.ucMlx90393ErroType = ucErroType;
}

//为了多次编程的需要测试用，擦写易失非易失
void vWriteRamOrNoramThings(void)
{
#define IfMemoryRecallOkWaiteTimeTest_ms    80//如果不同逻辑操作的命令之间的等待延时
    unsigned char ucTemp = 0;
    vCmdMlxAndWaiteErroTimes(MLX90393_ExitCommand, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InCmdExitErro);//退出一个所有模式
    g_uiTimeNumCont1ms = IfMemoryRecallOkWaiteTimeTest_ms;
    vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的//无条件等待
    //C_CtlRunLedXor;
    if(g_strMlxInfoData.ucMlx90393ErroType)return;
    else
    {
        g_uiTimeNumCont1ms = IfMemoryRecallOkWaiteTimeTest_ms;
        vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
        vCmdMlxWriteRegAndWaite(0, 0x000C, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InWriteRegErro);//第一个寄存器强写入一个标准值
    }
    //C_CtlRunLedXor;
    for(ucTemp = 1; ucTemp < 30; ucTemp++)//循环写入值
    {
        if(g_strMlxInfoData.ucMlx90393ErroType)return;
        else
        {
            g_uiTimeNumCont1ms = IfMemoryRecallOkWaiteTimeTest_ms;
            vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
            vCmdMlxWriteRegAndWaite(ucTemp, 0x0000, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InWriteRegErro);
        }
        //C_CtlRunLedXor;
    }

    if(g_strMlxInfoData.ucMlx90393ErroType)return;
    else
    {
        g_uiTimeNumCont1ms = IfMemoryRecallOkWaiteTimeTest_ms;
        vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
        vCmdMlxAndWaiteErroTimes(MLX90393_MemStoreCommand, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InStoreRegErro);
    }
    //C_CtlRunLedXor;
    g_uiTimeNumCont1ms = IfMemoryRecallOkWaiteTimeTest_ms;
    vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
    //C_CtlRunLedXor;
    vCmdMlxAndWaiteErroTimes(MLX90393_ExitCommand, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InCmdExitErro);
    if(g_strMlxInfoData.ucMlx90393ErroType)return;
    g_uiTimeNumCont1ms = IfMemoryRecallOkWaiteTimeTest_ms;
    vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
}

//对90393的上电初始化工作
void vSetUpMlx90393(void)
{

#define IfMemoryRecallOkWaiteTime_ms 5//如果不同逻辑操作的命令之间的等待延时
#define IfReadNextRegWaite_ms        2//每个相同命令之间的时间间隔
    unsigned char ucReadMlxRegNumIndex = 0;//读取芯片，寄存器计数器
    unsigned char ucReadCycCnt = 0;//循环读计数器
    unsigned short usReg0_3DataArr[3] = {0};
    g_strMlxInfoData.ucMlx90393ErroType = 0;//清故障信息
    vCmdMlxAndWaiteErroTimes(MLX90393_MemRecallCommand, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InCallMemoryCall);//让芯片去从非易失到易失RAM中加载一次
    if(g_strMlxInfoData.ucMlx90393ErroType)return;//如果 IC读帧有错误
    else
    {
        g_uiTimeNumCont1ms = IfReadNextRegWaite_ms;
        vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的//无条件延时下，等待芯片的稳定性
        vCmdMlxAndWaiteErroTimes(MLX90393_ExitCommand, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InCmdExitErro);//让其提出所有状态位
        g_uiTimeNumCont1ms = IfMemoryRecallOkWaiteTime_ms;
        vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
    }
    if(g_strMlxInfoData.ucMlx90393ErroType)return;//如果 IC读帧有错误

    for(ucReadMlxRegNumIndex = 0; ucReadMlxRegNumIndex < 10; ucReadMlxRegNumIndex++)//读下，用户标定在芯片中的数据
    {
        g_uiTimeNumCont1ms = IfReadNextRegWaite_ms;
        vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
        vCmdMlxAndWaiteErroTimes(MLX90393_ExitCommand, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InCmdExitErro);//推出所有操作模式
        if(g_strMlxInfoData.ucMlx90393ErroType)return;
        g_uiTimeNumCont1ms = IfReadNextRegWaite_ms;
        vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
        for(ucReadCycCnt = 0; ucReadCycCnt < MaxCmdErroTimes; ucReadCycCnt++)//如果不成功 多次读取
        {
            g_strMlxInfoData.uniRecUsefulMemoryData.usRecUsefulMemoryData[ucReadMlxRegNumIndex] = usMlx90393ReadReg(ucReadMlxRegNumIndex + 10, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd);
            if(ucCheckReadMlxStatusErro(g_strMlxInfoData.ucMlx90393StatusByte, MlxInReg_Mode))
            {
                g_uiTimeNumCont1ms = IfReadNextRegWaite_ms;
                vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的//在错误帧里的循环时间
            }
            else break;//如果本次操作是对的，就退出循环
        }
        if(ucReadCycCnt > 4)//如果多次读取仍然会出错的。就置位错误
        {
            g_strMlxInfoData.ucMlx90393ErroType = MlxErro_InReadRegErro;
            return;
        }
    }
    if(g_strMlxInfoData.ucMlx90393ErroType)return;
    g_uiTimeNumCont1ms = IfMemoryRecallOkWaiteTime_ms;
    vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
    vCmdMlxAndWaiteErroTimes(MLX90393_ResetCommand, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InCmdExitErro);
    if(g_strMlxInfoData.ucMlx90393ErroType)return;
    C_CtlSet90393_Sda_AsOutPut;  //Set-->SDA As OutPutMode
    C_Ctl90393_SDA_H;
    g_uiTimeNumCont1ms = IfMemoryRecallOkWaiteTime_ms;
    vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
    g_uiTimeNumCont1ms = IfMemoryRecallOkWaiteTime_ms;
    vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
    ///////////////////////////////ReadReg0-3
    for(ucReadMlxRegNumIndex = 0; ucReadMlxRegNumIndex < 3; ucReadMlxRegNumIndex++)
    {
        g_uiTimeNumCont1ms = IfReadNextRegWaite_ms;
        vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
        vCmdMlxAndWaiteErroTimes(MLX90393_ExitCommand, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InCmdExitErro);
        if(g_strMlxInfoData.ucMlx90393ErroType)return;
        g_uiTimeNumCont1ms = IfReadNextRegWaite_ms;
        vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
        for(ucReadCycCnt = 0; ucReadCycCnt < MaxCmdErroTimes; ucReadCycCnt++)
        {
            usReg0_3DataArr[ucReadMlxRegNumIndex] = usMlx90393ReadReg(ucReadMlxRegNumIndex, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd);
            if(ucCheckReadMlxStatusErro(g_strMlxInfoData.ucMlx90393StatusByte, MlxInReg_Mode))
            {
                g_uiTimeNumCont1ms = IfReadNextRegWaite_ms;
                vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
            }
            else break;
        }
        if(ucReadCycCnt > 4)
        {
            g_strMlxInfoData.ucMlx90393ErroType = MlxErro_InReadRegErro;
            return;
        }
    }
    if(g_strMlxInfoData.ucMlx90393ErroType)return;//

#ifdef IfEnableCheckUseFulDataInIC	 /////在这里要检测下存储在 10-20芯片地址数据是否是正确的
    ucReadCycCnt = 0;
    for(ucReadMlxRegNumIndex = 0; ucReadMlxRegNumIndex < 10; ucReadMlxRegNumIndex++)
    {
        if((0xFF == g_strMlxInfoData.usRecUsefulMemoryData[ucReadMlxRegNumIndex]) ||
                (0x00 == g_strMlxInfoData.usRecUsefulMemoryData[ucReadMlxRegNumIndex]))ucReadCycCnt++;
    }
    if(ucReadCycCnt >= 6)
    {
        g_strMlxInfoData.strStateAndSet.bitCheckMlxIcSaveUseFulDataOk = FALSE;
        g_strMlxInfoData.ucMlx90393ErroType = MlxErro_CheckUseFulDataErro;
    }
    else g_strMlxInfoData.strStateAndSet.bitCheckMlxIcSaveUseFulDataOk = TRUE;
    if(g_strMlxInfoData.ucMlx90393ErroType)return;//

#endif
    ////////////////////////////////////////////////////////////////////
#ifdef 	InDebugMlx90393//测试下存储在非易失中的数据是否可以操作
    usReg0_3DataArr[1] ++;
#endif
    if((UseSetMlxReg0Data != usReg0_3DataArr[0]) || (UseSetMlxReg1Data != usReg0_3DataArr[1]) ||
            (UseSetMlxReg2Data != usReg0_3DataArr[2]))//如果数据与预期不一样，就让其烧写下芯片
    {
        g_uiTimeNumCont1ms = IfReadNextRegWaite_ms;
        vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
        vCmdMlxWriteRegAndWaite(0, UseSetMlxReg0Data, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InWriteRegErro);
        if(g_strMlxInfoData.ucMlx90393ErroType)return;
        else
        {
            g_uiTimeNumCont1ms = IfReadNextRegWaite_ms;
            vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
            vCmdMlxWriteRegAndWaite(1, UseSetMlxReg1Data, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InWriteRegErro);
        }
        if(g_strMlxInfoData.ucMlx90393ErroType)return;
        else
        {
            g_uiTimeNumCont1ms = IfReadNextRegWaite_ms;
            vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
            vCmdMlxWriteRegAndWaite(2, UseSetMlxReg2Data, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InWriteRegErro);
        }
        if(g_strMlxInfoData.ucMlx90393ErroType)return;
        else
        {
            g_uiTimeNumCont1ms = IfReadNextRegWaite_ms;
            vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
            vCmdMlxAndWaiteErroTimes(MLX90393_MemStoreCommand, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InStoreRegErro);
        }
        g_uiTimeNumCont1ms = IfMemoryRecallOkWaiteTime_ms;
        vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
        vCmdMlxAndWaiteErroTimes(MLX90393_ExitCommand, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InCmdExitErro);
        if(g_strMlxInfoData.ucMlx90393ErroType)return;
        g_uiTimeNumCont1ms = IfReadNextRegWaite_ms;
        vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
    }

    g_uiTimeNumCont1ms = 10;
    vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
    if(g_strMlxInfoData.ucMlx90393ErroType)return;
    vCmdMlxAndWaiteErroTimes(MLX90393_StartMeasurement, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InCallMemoryCall);//让其触发一次采样
		g_strMlxInfoData.ucMlx90393ErroType = FALSE;
    g_uiTimeNumCont1ms = IfMemoryRecallOkWaiteTime_ms;
    vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
}

//非定时器时操作情况下的，阻塞式操作
void vInMeasurementNormal(void)
{
    vMlx90393IicStart();//写起始位
    ucMlx90393WriteOneByte(g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd);//写从机地址
    ucMlx90393WriteOneByte(MLX90393_ReadMeausreCommand);//写读命令
    vMlx90393IicStart();//开始信号
    ucMlx90393WriteOneByte(g_strMlxInfoData.strCmdAdr.ucMLX90393_ReadCmdAdd);//读操作
    g_strMlxInfoData.ucMlx90393StatusByte = ucMlx90393ReadOnByte();//read data slave address 状态位
    vMlx90393WriteAck();
    g_strMlxInfoData.uniusRecXdata.strData.ucData2 = ucMlx90393ReadOnByte();//低位数据
    vMlx90393WriteAck();
    g_strMlxInfoData.uniusRecXdata.strData.ucData1 = ucMlx90393ReadOnByte();
    vMlx90393WriteAck();
    g_strMlxInfoData.uniusRecYdata.strData.ucData2 = ucMlx90393ReadOnByte();
    vMlx90393WriteAck();
    g_strMlxInfoData.uniusRecYdata.strData.ucData1 = ucMlx90393ReadOnByte();
    vMlx90393WriteAck();
    vMlx90393IicStop();/*IIC stop*/
    if(ucCheckReadMlxStatusErro(g_strMlxInfoData.ucMlx90393StatusByte, MlxInNoneReg_Mode))
    {
        g_strMlxInfoData.usMlx90393StatusErroTimes++;//如果状态位有问题 累加错误帧计数器
    }
    g_strMlxInfoData.ucMlx90393StatusByte = ucMLX90393WriteCommand(MLX90393_StartMeasurement, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd);
    if(ucCheckReadMlxStatusErro(g_strMlxInfoData.ucMlx90393StatusByte, MlxInNoneReg_Mode))//写一个读命令，
    {
        g_strMlxInfoData.usMlx90393StatusErroTimes++;//如果状态位有问题 累加错误帧计数器
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//在定时器非阻塞运行时间的一个起始信号 CLK信号长些
unsigned char ucMlxIicBegin_StartInTimer(unsigned char ucDoStep)
{
    if(0 == ucDoStep)//Set As Out
    {
        C_CtlSet90393_Sda_AsOutPut;
    }
    else if(2 == ucDoStep)
    {
        C_Ctl90393_SDA_H;
        C_Ctl90393_SCL_H;
    }
    else if(4 == ucDoStep)
    {
        C_Ctl90393_SDA_L;
    }
    else if(6 == ucDoStep)
    {
        C_Ctl90393_SCL_L;
    }
    else if(8 == ucDoStep)
    {
        return TRUE;
    }
    return FALSE;
}

//在定时器非阻塞运行-写芯片命令字
unsigned char ucMlxIicWriteCmdInTimer(unsigned char ucDoStep, unsigned char ucCmd)
{
    if(0 == ucDoStep)//Set Out mode
    {
        C_CtlSet90393_Sda_AsOutPut;
    }
    else if(1 == ucDoStep)//0x80
    {
        if(ucCmd & 0x80)C_Ctl90393_SDA_H;
        else C_Ctl90393_SDA_L;
    }//bit1
    else if(2 == ucDoStep)
    {
        C_Ctl90393_SCL_H;
    }
    else if(3 == ucDoStep)
    {
        C_Ctl90393_SCL_L;
    }
    else if(4 == ucDoStep)//0x40
    {
        if(ucCmd & 0x40)C_Ctl90393_SDA_H;
        else C_Ctl90393_SDA_L;
    }//bit2
    else if(5 == ucDoStep)
    {
        C_Ctl90393_SCL_H;
    }
    else if(6 == ucDoStep)
    {
        C_Ctl90393_SCL_L;
    }
    else if(7 == ucDoStep)//0x20
    {
        if(ucCmd & 0x20)C_Ctl90393_SDA_H;
        else C_Ctl90393_SDA_L;
    }//bit3
    else if(8 == ucDoStep)
    {
        C_Ctl90393_SCL_H;
    }
    else if(9 == ucDoStep)
    {
        C_Ctl90393_SCL_L;
    }
    else if(10 == ucDoStep)//x010
    {
        if(ucCmd & 0x10)C_Ctl90393_SDA_H;
        else C_Ctl90393_SDA_L;
    }//bit4
    else if(11 == ucDoStep)
    {
        C_Ctl90393_SCL_H;
    }
    else if(12 == ucDoStep)
    {
        C_Ctl90393_SCL_L;
    }
    else if(13 == ucDoStep)//0x08
    {
        if(ucCmd & 0x08)C_Ctl90393_SDA_H;
        else C_Ctl90393_SDA_L;
    }//bit5
    else if(14 == ucDoStep)
    {
        C_Ctl90393_SCL_H;
    }
    else if(15 == ucDoStep)
    {
        C_Ctl90393_SCL_L;
    }
    else if(16 == ucDoStep)//0x04
    {
        if(ucCmd & 0x04)C_Ctl90393_SDA_H;
        else C_Ctl90393_SDA_L;
    }//bit6
    else if(17 == ucDoStep)
    {
        C_Ctl90393_SCL_H;
    }
    else if(18 == ucDoStep)
    {
        C_Ctl90393_SCL_L;
    }
    else if(19 == ucDoStep)//0x02
    {
        if(ucCmd & 0x02)C_Ctl90393_SDA_H;
        else C_Ctl90393_SDA_L;
    }//bit7
    else if(20 == ucDoStep)
    {
        C_Ctl90393_SCL_H;
    }
    else if(21 == ucDoStep)
    {
        C_Ctl90393_SCL_L;
    }
    else if(22 == ucDoStep) //0x01
    {
        if(ucCmd & 0x01)C_Ctl90393_SDA_H;
        else C_Ctl90393_SDA_L;
    }//bit7
    else if(23 == ucDoStep)
    {
        C_Ctl90393_SCL_H;
    }
    else if(24 == ucDoStep)
    {
        C_Ctl90393_SCL_L;
    }
    else if(25 == ucDoStep)//
    {
        return TRUE;
    }
    return FALSE;
}

//在定时器非阻塞运行-读取一个ACK信号---一般用于写数据后的操作
unsigned char ucMlxIicReadAck(unsigned char ucDoStep)
{
    if(0 == ucDoStep)
    {
        C_CtlSet90393_Sda_AsInPut;
        C_Ctl90393_SCL_H;
    }
    else if(1 == ucDoStep)
    {
        C_Ctl90393_Read_SDA;
        C_Ctl90393_SCL_L;
    }
    else if(2 == ucDoStep)
    {
        return TRUE;
    }
    return FALSE;
}

//在定时器非阻塞运行-写一个ACK信号---一般用于读取数据后的操作
unsigned char ucMlxIicWriteAck(unsigned char ucDoStep)
{
    if(0 == ucDoStep)
    {
        C_CtlSet90393_Sda_AsOutPut;
        C_Ctl90393_SDA_L;
    }
    else if(1 == ucDoStep)
    {
        C_Ctl90393_SCL_H;
    }
    else if(2 == ucDoStep)
    {
        C_Ctl90393_SCL_L;
    }
    else if(3 == ucDoStep)
    {
        return TRUE;
    }
    return FALSE;
}

//在定时器非阻塞运行-非首次运行时间的 IIC起始信号位
unsigned char ucMlxIicSendStart(unsigned char ucDoStep)
{
    if(0 == ucDoStep)
    {
        C_CtlSet90393_Sda_AsOutPut;
        C_Ctl90393_SDA_H;
        C_Ctl90393_SCL_H;
    }
    else if(1 == ucDoStep)
    {
        C_Ctl90393_SDA_L;
    }
    else if(2 == ucDoStep)
    {
        C_Ctl90393_SCL_L;
    }
    else if(3 == ucDoStep)
    {
        return TRUE;
    }
    return FALSE;
}

//在定时器非阻塞运行-读取个8bit位 -2clk一次
unsigned char ucMlxIicRead8BitsInTimer(unsigned char ucDoStep, unsigned char *ucReadData)
{
    if(0 == ucDoStep)//0
    {
        C_CtlSet90393_Sda_AsInPut;
    }
    else if(1 == ucDoStep)//0
    {
        C_Ctl90393_SCL_H;
    }
    else if(2 == ucDoStep)
    {
        if(C_Ctl90393_Read_SDA)*ucReadData |= 0x80;
        C_Ctl90393_SCL_L;
    }
    else if(3 == ucDoStep)//00
    {
        C_Ctl90393_SCL_H;
    }
    else if(4 == ucDoStep)
    {
        if(C_Ctl90393_Read_SDA)*ucReadData |= 0x40;
        C_Ctl90393_SCL_L;
    }
    else if(5 == ucDoStep)//000
    {
        C_Ctl90393_SCL_H;
    }
    else if(6 == ucDoStep)
    {
        if(C_Ctl90393_Read_SDA)*ucReadData |= 0x20;
        C_Ctl90393_SCL_L;
    }
    else if(7 == ucDoStep)//0001
    {
        C_Ctl90393_SCL_H;
    }
    else if(8 == ucDoStep)
    {
        if(C_Ctl90393_Read_SDA)*ucReadData |= 0x10;
        C_Ctl90393_SCL_L;
    }
    else if(9 == ucDoStep)//00011
    {
        C_Ctl90393_SCL_H;
    }
    else if(10 == ucDoStep)
    {
        if(C_Ctl90393_Read_SDA)*ucReadData |= 0x08;
        C_Ctl90393_SCL_L;
    }
    else if(11 == ucDoStep)//000110
    {
        C_Ctl90393_SCL_H;
    }
    else if(12 == ucDoStep)
    {
        if(C_Ctl90393_Read_SDA)*ucReadData |= 0x04;
        C_Ctl90393_SCL_L;
    }
    else if(13 == ucDoStep)//0001100
    {
        C_Ctl90393_SCL_H;
    }
    else if(14 == ucDoStep)
    {
        if(C_Ctl90393_Read_SDA)*ucReadData |= 0x02;
        C_Ctl90393_SCL_L;
    }
    else if(15 == ucDoStep)//00011001
    {
        C_Ctl90393_SCL_H;
    }
    else if(16 == ucDoStep)//
    {
        if(C_Ctl90393_Read_SDA)*ucReadData |= 0x01;
        C_Ctl90393_SCL_L;
    }
    else if(17 == ucDoStep)//
    {
        return TRUE;
    }
    return FALSE;
}

//在定时器--让IIC 产生一个停止位
unsigned char ucMlxIicSendStop(unsigned char ucDoStep)
{
    if(0 == ucDoStep)
    {
        C_CtlSet90393_Sda_AsOutPut;
        C_Ctl90393_SDA_L;
        C_Ctl90393_SCL_L;
    }
    if(1 == ucDoStep)
    {
        C_Ctl90393_SCL_H;
    }
    else if(2 == ucDoStep)
    {
        C_Ctl90393_SDA_H;
    }
    else if(3 == ucDoStep)
    {
        return TRUE;//操作完成置位
    }
    return FALSE;
}
struct strMlx9039InTimerOp strInTimeOp = {0};//静态数据-存放在RAM 本函数私有变量
//此函数应该在定时器中调用-非阻塞式操作
void vReadMlx90393InTimerMode(void)
{
//    static struct strMlx9039InTimerOp strInTimeOp = {0};//静态数据-存放在RAM 本函数私有变量
    strInTimeOp.ucPtr = &strInTimeOp.ucRecDataTemp;//给存储数据赋--指针变量 如何只初始化一次呢？who can tell me?
    strInTimeOp.usCycOprtationIcCnt++;//循环操作时间计数器
    if(strInTimeOp.usCycOprtationIcCnt < MlxCycOprtationIcDelayTime)return;//如果非到本次时间不操作任何数据
    //C_CtlRunLedXor;//运行效率指示灯-IO可以查看程序运行效率
    if(0 == strInTimeOp.ucSetpsModeIndex)//IIC_Start
    {
        if(ucMlxIicBegin_StartInTimer(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(1 == strInTimeOp.ucSetpsModeIndex)//Write 0x18  slaveAdd  00011000
    {
        if(ucMlxIicWriteCmdInTimer(strInTimeOp.ucClkCnt, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(2 == strInTimeOp.ucSetpsModeIndex)//waite Read ACK
    {
        if(ucMlxIicReadAck(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(3 == strInTimeOp.ucSetpsModeIndex)//Write 0x46  slaveAdd  01000110
    {
        if(ucMlxIicWriteCmdInTimer(strInTimeOp.ucClkCnt, MLX90393_ReadMeausreCommand))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(4 == strInTimeOp.ucSetpsModeIndex)//waite Read ACK
    {
        if(ucMlxIicReadAck(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    if(5 == strInTimeOp.ucSetpsModeIndex)//IIC_Start
    {
        if(ucMlxIicSendStart(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(6 == strInTimeOp.ucSetpsModeIndex)//Write 0x19  slaveAdd 00011001
    {
        if(ucMlxIicWriteCmdInTimer(strInTimeOp.ucClkCnt, g_strMlxInfoData.strCmdAdr.ucMLX90393_ReadCmdAdd))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(7 == strInTimeOp.ucSetpsModeIndex)//waite Read ACK
    {
        if(ucMlxIicReadAck(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(8 == strInTimeOp.ucSetpsModeIndex)//Read 8 Bits Status -  IC FeedBack
    {
        if(0 == strInTimeOp.ucClkCnt)strInTimeOp.ucRecDataTemp = 0;//清计数器
        if(ucMlxIicRead8BitsInTimer(strInTimeOp.ucClkCnt, strInTimeOp.ucPtr))//尝试去读取一次
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(9 == strInTimeOp.ucSetpsModeIndex)//waite Write ACK
    {
        if(0 == strInTimeOp.ucClkCnt)
        {
            if(ucCheckReadMlxStatusErro(strInTimeOp.ucRecDataTemp, MlxInNoneReg_Mode))//检查状态字会不会出问题，如果出问题就去计数一次
            {
                g_strMlxInfoData.usMlx90393StatusErroTimes++;//如果状态位有问题 累加错误帧计数器
            }
            strInTimeOp.ucRecDataTemp = 0;//清临时变量
        }
        if(ucMlxIicWriteAck(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(10 == strInTimeOp.ucSetpsModeIndex)//Read 8 Bits X High
    {
        if(ucMlxIicRead8BitsInTimer(strInTimeOp.ucClkCnt, strInTimeOp.ucPtr))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(11 == strInTimeOp.ucSetpsModeIndex)//waite Write ACK
    {
        if(0 == strInTimeOp.ucClkCnt)//将或取到X高8位存入起来
        {
            g_strMlxInfoData.uniusRecXdata.strData.ucData2 = strInTimeOp.ucRecDataTemp;
            strInTimeOp.ucRecDataTemp = 0;
        }
        if(ucMlxIicWriteAck(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(12 == strInTimeOp.ucSetpsModeIndex)//Read 8 Bits X Low
    {
        if(ucMlxIicRead8BitsInTimer(strInTimeOp.ucClkCnt, strInTimeOp.ucPtr))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(13 == strInTimeOp.ucSetpsModeIndex)//waite Write ACK
    {
        if(0 == strInTimeOp.ucClkCnt)//将低8位存起来
        {
            g_strMlxInfoData.uniusRecXdata.strData.ucData1 = strInTimeOp.ucRecDataTemp;
            strInTimeOp.ucRecDataTemp = 0;
        }
        if(ucMlxIicWriteAck(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(14 == strInTimeOp.ucSetpsModeIndex)//Read 8 Bits Y High
    {
        if(ucMlxIicRead8BitsInTimer(strInTimeOp.ucClkCnt, strInTimeOp.ucPtr))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(15 == strInTimeOp.ucSetpsModeIndex)//waite Write ACK
    {
        if(0 == strInTimeOp.ucClkCnt)
        {
            g_strMlxInfoData.uniusRecYdata.strData.ucData2 = strInTimeOp.ucRecDataTemp;
			g_strMlxInfoData.ucUpDataNewFlag = TRUE;
            strInTimeOp.ucRecDataTemp = 0;
        }
        if(ucMlxIicWriteAck(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(16 == strInTimeOp.ucSetpsModeIndex)//Read 8 Bits Y Low
    {
        if(ucMlxIicRead8BitsInTimer(strInTimeOp.ucClkCnt, strInTimeOp.ucPtr))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(17 == strInTimeOp.ucSetpsModeIndex)//waite Write ACK
    {
        if(0 == strInTimeOp.ucClkCnt)
        {
            g_strMlxInfoData.uniusRecYdata.strData.ucData1 = strInTimeOp.ucRecDataTemp;
            strInTimeOp.ucRecDataTemp = 0;
        }
        if(ucMlxIicWriteAck(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(18 == strInTimeOp.ucSetpsModeIndex)//waite Write Stop
    {
        if(ucMlxIicSendStop(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    if(19 == strInTimeOp.ucSetpsModeIndex)//IIC_Start
    {
        if(ucMlxIicSendStart(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(20 == strInTimeOp.ucSetpsModeIndex)//Write 0x18  slaveAdd		   00011000
    {
        if(ucMlxIicWriteCmdInTimer(strInTimeOp.ucClkCnt, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(21 == strInTimeOp.ucSetpsModeIndex)//waite Read ACK
    {
        if(ucMlxIicReadAck(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(22 == strInTimeOp.ucSetpsModeIndex)//Write 0x36 slaveAdd  00110110
    {
        if(ucMlxIicWriteCmdInTimer(strInTimeOp.ucClkCnt, MLX90393_StartMeasurement))

        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(23 == strInTimeOp.ucSetpsModeIndex)//waite Read ACK
    {
        if(ucMlxIicReadAck(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(24 == strInTimeOp.ucSetpsModeIndex)//IIC_Start
    {
        if(ucMlxIicSendStart(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(25 == strInTimeOp.ucSetpsModeIndex)//Write 0x19  slaveAdd		   00011001
    {
        if(ucMlxIicWriteCmdInTimer(strInTimeOp.ucClkCnt, g_strMlxInfoData.strCmdAdr.ucMLX90393_ReadCmdAdd))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(26 == strInTimeOp.ucSetpsModeIndex)//waite Read ACK
    {
        if(ucMlxIicReadAck(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(27 == strInTimeOp.ucSetpsModeIndex)//Read 8 Bits Status -		   IC FeedBack
    {
        if(ucMlxIicRead8BitsInTimer(strInTimeOp.ucClkCnt, strInTimeOp.ucPtr))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(28 == strInTimeOp.ucSetpsModeIndex)//waite Write ACK
    {
        if(0 == strInTimeOp.ucClkCnt)
        {
            if(ucCheckReadMlxStatusErro(strInTimeOp.ucRecDataTemp, MlxInNoneReg_Mode))//检查状态字会不会出问题，如果出问题就去计数一次
            {
                g_strMlxInfoData.usMlx90393StatusErroTimes++;//如果状态位有问题 累加错误帧计数器
            }
            strInTimeOp.ucRecDataTemp = 0;
        }
        if(ucMlxIicWriteAck(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex ++;//如果完成本次操作，转入下循环中
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
    else if(29 == strInTimeOp.ucSetpsModeIndex)//waite Write Stop
    {
        if(ucMlxIicSendStop(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.usCycOprtationIcCnt = strInTimeOp.ucClkCnt = 0;// 如果在本次操作环节中完成所有步序，清步序计数器，方便下次使用
            strInTimeOp.ucSetpsModeIndex = 0;//完成一个读数据循环 及 再次开启测量功能
            return;
        }
        strInTimeOp.ucClkCnt++;//未完成，累加计数器。立即退出
        return;
    }
}


unsigned char ucFucCheck90393Add(void)
{
    unsigned char ucCheckIndex = 0;
    unsigned char ucReSendTimeCnt = 0;
#define Check90393DelayTime_ms     4
    for(ucCheckIndex = 1; ucCheckIndex <= 4; ucCheckIndex ++)
    {
        if(1 == ucCheckIndex)
        {
            g_strMlxInfoData.strCmdAdr.ucMLX90393_ReadCmdAdd = MLX90393slaveAddAndRead_011;
            g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd = MLX90393slaveAdd_011;
        }
        else if(2 == ucCheckIndex)
        {
            g_strMlxInfoData.strCmdAdr.ucMLX90393_ReadCmdAdd = MLX90393slaveAddAndRead_014;
            g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd = MLX90393slaveAdd_014;
        }
        else if(3 == ucCheckIndex)
        {
            g_strMlxInfoData.strCmdAdr.ucMLX90393_ReadCmdAdd = MLX90393slaveAddAndRead_012;
            g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd = MLX90393slaveAdd_012;
        }
        else if(4 == ucCheckIndex)
        {
            g_strMlxInfoData.strCmdAdr.ucMLX90393_ReadCmdAdd = MLX90393slaveAddAndRead_013;
            g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd = MLX90393slaveAdd_013;
        }
        g_strMlxInfoData.usMlx90393StatusErroTimes = 0;
        for(ucReSendTimeCnt = 0; ucReSendTimeCnt < 4; ucReSendTimeCnt ++)
        {
            g_strMlxInfoData.ucMlx90393StatusByte = ucMLX90393WriteCommand(MLX90393_StartMeasurement, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd);
            if(ucCheckReadMlxStatusErro(g_strMlxInfoData.ucMlx90393StatusByte, MlxInNoneReg_Mode))//写一个读命令，
            {
                g_strMlxInfoData.usMlx90393StatusErroTimes++;//如果状态位有问题 累加错误帧计数器
            }
            g_uiTimeNumCont1ms = Check90393DelayTime_ms;
            vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//替换为阻塞式的
        }
        if(g_strMlxInfoData.usMlx90393StatusErroTimes < 2)break;
    }
    if(4 == g_strMlxInfoData.usMlx90393StatusErroTimes)return FALSE;
    else return TRUE;
}
unsigned short usSampleX_90393 = 0;
unsigned short usSampleY_90393 = 0;

float fXmin = 0;
float fXmax = 0;
float fYmin = 0;
float fYmax = 0;
void vCalcIniMlx90393Data(void)
{
    float fOffset = 0;
    fOffset = (FactoryRemoteMaxYPos - FactoryRemoteMidYPos) * 1.0000001;
    if(g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usLeftX >= g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usRightX)
    {
        fXmin = fOffset / (g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleX - g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usRightX);
        fXmax = fOffset / (g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usLeftX - g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleX);
        g_strMlxInfoData.ucMlx90393AssemblyN_DownFlag = FALSE;//判断唯一的磁极装配逻辑
    }
    else
    {
        fXmin = fOffset / (g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleX - g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usLeftX);
        fXmax = fOffset / (g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usRightX - g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleX);
        g_strMlxInfoData.ucMlx90393AssemblyN_DownFlag = TRUE;
    }
    if(g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usUpY >=  g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usDownY)
    {
        fYmin = fOffset / (g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleY - g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usDownY);
        fYmax = fOffset / (g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usUpY - g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleY);
    }
    else
    {
        fYmin = fOffset / (g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleY - g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usUpY);
        fYmax = fOffset / (g_strMlxInfoData.uniRecUsefulMemoryData.usRecUsefulMemoryData[9] - g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleY);
    }
}
void vAcquireHallAsRealCoordinateAxes(void)
{
#define AllowDeltIIC  50
    if((g_strMlxInfoData.uniusRecXdata.usData >= (g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleX - AllowDeltIIC)) &&
            (g_strMlxInfoData.uniusRecXdata.usData <= (g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleX + AllowDeltIIC)))
    {
        usSampleX_90393 = FactoryRemoteMidXPos;//1963
    }
    else
    {
        if(g_strMlxInfoData.uniusRecXdata.usData > (g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleX + AllowDeltIIC))
        {
            if(g_strMlxInfoData.ucMlx90393AssemblyN_DownFlag)
            {
                usSampleX_90393 = FactoryRemoteMidXPos - fXmax * (g_strMlxInfoData.uniusRecXdata.usData - g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleX);
            }
            else usSampleX_90393 = FactoryRemoteMidXPos + fXmax * (g_strMlxInfoData.uniusRecXdata.usData - g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleX);
        }
        if(g_strMlxInfoData.uniusRecXdata.usData < (g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleX - AllowDeltIIC))
        {
            if(g_strMlxInfoData.ucMlx90393AssemblyN_DownFlag)
            {
                usSampleX_90393 = FactoryRemoteMidXPos + fXmin * (g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleX - g_strMlxInfoData.uniusRecXdata.usData);
            }
            else
            {
                usSampleX_90393 = FactoryRemoteMidXPos - fXmin * (g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleX - g_strMlxInfoData.uniusRecXdata.usData);//调换一下极坐标逻辑关系
            }
        }
    }
    if((g_strMlxInfoData.uniusRecYdata.usData >= (g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleY - AllowDeltIIC)) &&
            (g_strMlxInfoData.uniusRecYdata.usData <= (g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleY + AllowDeltIIC)))
    {
        usSampleY_90393 = FactoryRemoteMidYPos;//1963
    }
    else
    {
        if(g_strMlxInfoData.uniusRecYdata.usData > (g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleY + AllowDeltIIC))
        {
            if(g_strMlxInfoData.ucMlx90393AssemblyN_DownFlag)
            {
                usSampleY_90393 = FactoryRemoteMidYPos + fYmax * (g_strMlxInfoData.uniusRecYdata.usData - g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleY);
            }
            else
            {
                usSampleY_90393 = FactoryRemoteMidYPos - fYmax * (g_strMlxInfoData.uniusRecYdata.usData - g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleY);
            }
        }
        if(g_strMlxInfoData.uniusRecYdata.usData < (g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleY - AllowDeltIIC))
        {
            if(g_strMlxInfoData.ucMlx90393AssemblyN_DownFlag)
            {
                usSampleY_90393 = FactoryRemoteMidYPos - fYmin * (g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleY - g_strMlxInfoData.uniusRecYdata.usData);
            }
            else
            {
                usSampleY_90393 = FactoryRemoteMidYPos + fYmin * (g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleY - g_strMlxInfoData.uniusRecYdata.usData);
            }
        }
    }
		strSysInfo.uiRemoteNowXPos = usSampleX_90393;
		strSysInfo.uiRemoteNowYPos = usSampleY_90393;
}


