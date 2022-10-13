

#include "DriverConst.h"

volatile struct strMlxInfoData g_strMlxInfoData = {0};//Mlx90393���õ����ݽṹ�壬ȫ�ֱ���

void vMlx90393WaitTimes()//����������ʱ�� ��һ��ʱ�����ʱ���ɶ�̬���Ե��� DelayTime CLK->100KHzû������
{
#define DelayTime 200
    unsigned short usDelay = DelayTime;
    for(; usDelay > 0; usDelay--);
}

void vMlx90393WaitTimes_ms(unsigned short usDelayTimes)//�Ǿ�ȷһ����ʱ
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
//��ȡһ���ֽڣ������ķ�ʽ
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

//90393 ��ʼλ-�����ķ�ʽ
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

//90393 ֹͣλ-�����ķ�ʽ
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

//90393 ��ȡһ��Ack�ź� ������һ��״̬λ--һ������д����֮��
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

//90393 дһ���ֽ�
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
    staturs = ucMlx90393ReadAckBit();//д�����ݺ�Ҫ���һ��ACK �ź�
    return (staturs);
}

//90393 дһ�� ACK�ź�  һ���������ݶ�ȡ��ɺ��ٲ�����
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

//��һ��Nack�ź�
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

//�������ķ�ʽ ���Ĵ���
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

//��90393дһ������
unsigned char ucMLX90393WriteCommand(unsigned char ucWriteCmd, unsigned char ucSlaveAdd)
{
    unsigned char ucReadStatusData;//��ȡ������ ״̬��
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
    unsigned char ucReadStatusData;//��ȡ������ ״̬��
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

//����MLX90393 ���´ӷ���ʧ�洢�������ص���ʧ�洢����  ������ ������
unsigned char ucCheckReadMlxStatusErro(unsigned char ucMlxStatusData, unsigned char ucCmdMode)
{
    if(0xFF == ucMlxStatusData)return 1;//���һ�����е�CLK�����SDA����ʱ��������ߵ�ƽ��˵������λ�Ǵ��
    else if(0x10 & ucMlxStatusData)return 1;//���״̬�Ĵ����д�����ڵ�
    if(MlxInReg_Mode == ucCmdMode)//����Ƕ��Ĵ������е�����
    {
        if(0x00 == ucMlxStatusData)return 0;
        else if(0x10 & ucMlxStatusData)return 1;
    }
    else
    {
        if(0x00 == ucMlxStatusData)return 1;//���һ�����е�CLK�����SDA����ʱ��������ߵ�ƽ��˵������λ�Ǵ��
    }
    return 0;
}

//д�����������Ƿ��ǳ���ģ������������ǿ�ƶ��д  ���ʱ��Ϊ ��MaxCmdErroTimes�����
void vCmdMlxAndWaiteErroTimes(unsigned char command, unsigned char SlaveAdd, unsigned char ucErroType)
{
    unsigned char ucReadCycCnt = 0;
    for(ucReadCycCnt = 0; ucReadCycCnt < MaxCmdErroTimes; ucReadCycCnt++)//�������������
    {
        g_strMlxInfoData.ucMlx90393StatusByte = ucMLX90393WriteCommand(command, SlaveAdd);//дһ������
        if(ucCheckReadMlxStatusErro(g_strMlxInfoData.ucMlx90393StatusByte, MlxInReg_Mode))
        {
            g_uiTimeNumCont1ms = IfMlxStatusByteErroWaiteNextCmdTime_ms;
            vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
        }
        else return;
    }
    if(ucReadCycCnt > 4)g_strMlxInfoData.ucMlx90393ErroType = ucErroType;
}

//�û�доƬ�Ĵ��������ȴ���
void vCmdMlxWriteRegAndWaite(unsigned char RegAddress, unsigned short usWriteData , unsigned char SlaveAdd, unsigned char ucErroType)
{
    unsigned char ucReadCycCnt = 0;
    for(ucReadCycCnt = 0; ucReadCycCnt < MaxCmdErroTimes; ucReadCycCnt++)
    {
        g_strMlxInfoData.ucMlx90393StatusByte = ucMLX90393WriteReg(RegAddress, usWriteData, SlaveAdd);
        if(ucCheckReadMlxStatusErro(g_strMlxInfoData.ucMlx90393StatusByte, MlxInReg_Mode))
        {
            g_uiTimeNumCont1ms = IfMlxStatusByteErroWaiteNextCmdTime_ms;
            vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
        }
        else return;
    }
    if(ucReadCycCnt > 4)g_strMlxInfoData.ucMlx90393ErroType = ucErroType;
}

//Ϊ�˶�α�̵���Ҫ�����ã���д��ʧ����ʧ
void vWriteRamOrNoramThings(void)
{
#define IfMemoryRecallOkWaiteTimeTest_ms    80//�����ͬ�߼�����������֮��ĵȴ���ʱ
    unsigned char ucTemp = 0;
    vCmdMlxAndWaiteErroTimes(MLX90393_ExitCommand, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InCmdExitErro);//�˳�һ������ģʽ
    g_uiTimeNumCont1ms = IfMemoryRecallOkWaiteTimeTest_ms;
    vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��//�������ȴ�
    //C_CtlRunLedXor;
    if(g_strMlxInfoData.ucMlx90393ErroType)return;
    else
    {
        g_uiTimeNumCont1ms = IfMemoryRecallOkWaiteTimeTest_ms;
        vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
        vCmdMlxWriteRegAndWaite(0, 0x000C, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InWriteRegErro);//��һ���Ĵ���ǿд��һ����׼ֵ
    }
    //C_CtlRunLedXor;
    for(ucTemp = 1; ucTemp < 30; ucTemp++)//ѭ��д��ֵ
    {
        if(g_strMlxInfoData.ucMlx90393ErroType)return;
        else
        {
            g_uiTimeNumCont1ms = IfMemoryRecallOkWaiteTimeTest_ms;
            vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
            vCmdMlxWriteRegAndWaite(ucTemp, 0x0000, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InWriteRegErro);
        }
        //C_CtlRunLedXor;
    }

    if(g_strMlxInfoData.ucMlx90393ErroType)return;
    else
    {
        g_uiTimeNumCont1ms = IfMemoryRecallOkWaiteTimeTest_ms;
        vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
        vCmdMlxAndWaiteErroTimes(MLX90393_MemStoreCommand, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InStoreRegErro);
    }
    //C_CtlRunLedXor;
    g_uiTimeNumCont1ms = IfMemoryRecallOkWaiteTimeTest_ms;
    vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
    //C_CtlRunLedXor;
    vCmdMlxAndWaiteErroTimes(MLX90393_ExitCommand, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InCmdExitErro);
    if(g_strMlxInfoData.ucMlx90393ErroType)return;
    g_uiTimeNumCont1ms = IfMemoryRecallOkWaiteTimeTest_ms;
    vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
}

//��90393���ϵ��ʼ������
void vSetUpMlx90393(void)
{

#define IfMemoryRecallOkWaiteTime_ms 5//�����ͬ�߼�����������֮��ĵȴ���ʱ
#define IfReadNextRegWaite_ms        2//ÿ����ͬ����֮���ʱ����
    unsigned char ucReadMlxRegNumIndex = 0;//��ȡоƬ���Ĵ���������
    unsigned char ucReadCycCnt = 0;//ѭ����������
    unsigned short usReg0_3DataArr[3] = {0};
    g_strMlxInfoData.ucMlx90393ErroType = 0;//�������Ϣ
    vCmdMlxAndWaiteErroTimes(MLX90393_MemRecallCommand, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InCallMemoryCall);//��оƬȥ�ӷ���ʧ����ʧRAM�м���һ��
    if(g_strMlxInfoData.ucMlx90393ErroType)return;//��� IC��֡�д���
    else
    {
        g_uiTimeNumCont1ms = IfReadNextRegWaite_ms;
        vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��//��������ʱ�£��ȴ�оƬ���ȶ���
        vCmdMlxAndWaiteErroTimes(MLX90393_ExitCommand, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InCmdExitErro);//�����������״̬λ
        g_uiTimeNumCont1ms = IfMemoryRecallOkWaiteTime_ms;
        vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
    }
    if(g_strMlxInfoData.ucMlx90393ErroType)return;//��� IC��֡�д���

    for(ucReadMlxRegNumIndex = 0; ucReadMlxRegNumIndex < 10; ucReadMlxRegNumIndex++)//���£��û��궨��оƬ�е�����
    {
        g_uiTimeNumCont1ms = IfReadNextRegWaite_ms;
        vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
        vCmdMlxAndWaiteErroTimes(MLX90393_ExitCommand, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InCmdExitErro);//�Ƴ����в���ģʽ
        if(g_strMlxInfoData.ucMlx90393ErroType)return;
        g_uiTimeNumCont1ms = IfReadNextRegWaite_ms;
        vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
        for(ucReadCycCnt = 0; ucReadCycCnt < MaxCmdErroTimes; ucReadCycCnt++)//������ɹ� ��ζ�ȡ
        {
            g_strMlxInfoData.uniRecUsefulMemoryData.usRecUsefulMemoryData[ucReadMlxRegNumIndex] = usMlx90393ReadReg(ucReadMlxRegNumIndex + 10, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd);
            if(ucCheckReadMlxStatusErro(g_strMlxInfoData.ucMlx90393StatusByte, MlxInReg_Mode))
            {
                g_uiTimeNumCont1ms = IfReadNextRegWaite_ms;
                vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��//�ڴ���֡���ѭ��ʱ��
            }
            else break;//������β����ǶԵģ����˳�ѭ��
        }
        if(ucReadCycCnt > 4)//�����ζ�ȡ��Ȼ�����ġ�����λ����
        {
            g_strMlxInfoData.ucMlx90393ErroType = MlxErro_InReadRegErro;
            return;
        }
    }
    if(g_strMlxInfoData.ucMlx90393ErroType)return;
    g_uiTimeNumCont1ms = IfMemoryRecallOkWaiteTime_ms;
    vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
    vCmdMlxAndWaiteErroTimes(MLX90393_ResetCommand, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InCmdExitErro);
    if(g_strMlxInfoData.ucMlx90393ErroType)return;
    C_CtlSet90393_Sda_AsOutPut;  //Set-->SDA As OutPutMode
    C_Ctl90393_SDA_H;
    g_uiTimeNumCont1ms = IfMemoryRecallOkWaiteTime_ms;
    vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
    g_uiTimeNumCont1ms = IfMemoryRecallOkWaiteTime_ms;
    vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
    ///////////////////////////////ReadReg0-3
    for(ucReadMlxRegNumIndex = 0; ucReadMlxRegNumIndex < 3; ucReadMlxRegNumIndex++)
    {
        g_uiTimeNumCont1ms = IfReadNextRegWaite_ms;
        vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
        vCmdMlxAndWaiteErroTimes(MLX90393_ExitCommand, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InCmdExitErro);
        if(g_strMlxInfoData.ucMlx90393ErroType)return;
        g_uiTimeNumCont1ms = IfReadNextRegWaite_ms;
        vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
        for(ucReadCycCnt = 0; ucReadCycCnt < MaxCmdErroTimes; ucReadCycCnt++)
        {
            usReg0_3DataArr[ucReadMlxRegNumIndex] = usMlx90393ReadReg(ucReadMlxRegNumIndex, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd);
            if(ucCheckReadMlxStatusErro(g_strMlxInfoData.ucMlx90393StatusByte, MlxInReg_Mode))
            {
                g_uiTimeNumCont1ms = IfReadNextRegWaite_ms;
                vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
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

#ifdef IfEnableCheckUseFulDataInIC	 /////������Ҫ����´洢�� 10-20оƬ��ַ�����Ƿ�����ȷ��
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
#ifdef 	InDebugMlx90393//�����´洢�ڷ���ʧ�е������Ƿ���Բ���
    usReg0_3DataArr[1] ++;
#endif
    if((UseSetMlxReg0Data != usReg0_3DataArr[0]) || (UseSetMlxReg1Data != usReg0_3DataArr[1]) ||
            (UseSetMlxReg2Data != usReg0_3DataArr[2]))//���������Ԥ�ڲ�һ������������д��оƬ
    {
        g_uiTimeNumCont1ms = IfReadNextRegWaite_ms;
        vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
        vCmdMlxWriteRegAndWaite(0, UseSetMlxReg0Data, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InWriteRegErro);
        if(g_strMlxInfoData.ucMlx90393ErroType)return;
        else
        {
            g_uiTimeNumCont1ms = IfReadNextRegWaite_ms;
            vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
            vCmdMlxWriteRegAndWaite(1, UseSetMlxReg1Data, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InWriteRegErro);
        }
        if(g_strMlxInfoData.ucMlx90393ErroType)return;
        else
        {
            g_uiTimeNumCont1ms = IfReadNextRegWaite_ms;
            vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
            vCmdMlxWriteRegAndWaite(2, UseSetMlxReg2Data, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InWriteRegErro);
        }
        if(g_strMlxInfoData.ucMlx90393ErroType)return;
        else
        {
            g_uiTimeNumCont1ms = IfReadNextRegWaite_ms;
            vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
            vCmdMlxAndWaiteErroTimes(MLX90393_MemStoreCommand, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InStoreRegErro);
        }
        g_uiTimeNumCont1ms = IfMemoryRecallOkWaiteTime_ms;
        vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
        vCmdMlxAndWaiteErroTimes(MLX90393_ExitCommand, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InCmdExitErro);
        if(g_strMlxInfoData.ucMlx90393ErroType)return;
        g_uiTimeNumCont1ms = IfReadNextRegWaite_ms;
        vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
    }

    g_uiTimeNumCont1ms = 10;
    vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
    if(g_strMlxInfoData.ucMlx90393ErroType)return;
    vCmdMlxAndWaiteErroTimes(MLX90393_StartMeasurement, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd, MlxErro_InCallMemoryCall);//���䴥��һ�β���
		g_strMlxInfoData.ucMlx90393ErroType = FALSE;
    g_uiTimeNumCont1ms = IfMemoryRecallOkWaiteTime_ms;
    vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
}

//�Ƕ�ʱ��ʱ��������µģ�����ʽ����
void vInMeasurementNormal(void)
{
    vMlx90393IicStart();//д��ʼλ
    ucMlx90393WriteOneByte(g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd);//д�ӻ���ַ
    ucMlx90393WriteOneByte(MLX90393_ReadMeausreCommand);//д������
    vMlx90393IicStart();//��ʼ�ź�
    ucMlx90393WriteOneByte(g_strMlxInfoData.strCmdAdr.ucMLX90393_ReadCmdAdd);//������
    g_strMlxInfoData.ucMlx90393StatusByte = ucMlx90393ReadOnByte();//read data slave address ״̬λ
    vMlx90393WriteAck();
    g_strMlxInfoData.uniusRecXdata.strData.ucData2 = ucMlx90393ReadOnByte();//��λ����
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
        g_strMlxInfoData.usMlx90393StatusErroTimes++;//���״̬λ������ �ۼӴ���֡������
    }
    g_strMlxInfoData.ucMlx90393StatusByte = ucMLX90393WriteCommand(MLX90393_StartMeasurement, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd);
    if(ucCheckReadMlxStatusErro(g_strMlxInfoData.ucMlx90393StatusByte, MlxInNoneReg_Mode))//дһ�������
    {
        g_strMlxInfoData.usMlx90393StatusErroTimes++;//���״̬λ������ �ۼӴ���֡������
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//�ڶ�ʱ������������ʱ���һ����ʼ�ź� CLK�źų�Щ
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

//�ڶ�ʱ������������-доƬ������
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

//�ڶ�ʱ������������-��ȡһ��ACK�ź�---һ������д���ݺ�Ĳ���
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

//�ڶ�ʱ������������-дһ��ACK�ź�---һ�����ڶ�ȡ���ݺ�Ĳ���
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

//�ڶ�ʱ������������-���״�����ʱ��� IIC��ʼ�ź�λ
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

//�ڶ�ʱ������������-��ȡ��8bitλ -2clkһ��
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

//�ڶ�ʱ��--��IIC ����һ��ֹͣλ
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
        return TRUE;//���������λ
    }
    return FALSE;
}
struct strMlx9039InTimerOp strInTimeOp = {0};//��̬����-�����RAM ������˽�б���
//�˺���Ӧ���ڶ�ʱ���е���-������ʽ����
void vReadMlx90393InTimerMode(void)
{
//    static struct strMlx9039InTimerOp strInTimeOp = {0};//��̬����-�����RAM ������˽�б���
    strInTimeOp.ucPtr = &strInTimeOp.ucRecDataTemp;//���洢���ݸ�--ָ����� ���ֻ��ʼ��һ���أ�who can tell me?
    strInTimeOp.usCycOprtationIcCnt++;//ѭ������ʱ�������
    if(strInTimeOp.usCycOprtationIcCnt < MlxCycOprtationIcDelayTime)return;//����ǵ�����ʱ�䲻�����κ�����
    //C_CtlRunLedXor;//����Ч��ָʾ��-IO���Բ鿴��������Ч��
    if(0 == strInTimeOp.ucSetpsModeIndex)//IIC_Start
    {
        if(ucMlxIicBegin_StartInTimer(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(1 == strInTimeOp.ucSetpsModeIndex)//Write 0x18  slaveAdd  00011000
    {
        if(ucMlxIicWriteCmdInTimer(strInTimeOp.ucClkCnt, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(2 == strInTimeOp.ucSetpsModeIndex)//waite Read ACK
    {
        if(ucMlxIicReadAck(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(3 == strInTimeOp.ucSetpsModeIndex)//Write 0x46  slaveAdd  01000110
    {
        if(ucMlxIicWriteCmdInTimer(strInTimeOp.ucClkCnt, MLX90393_ReadMeausreCommand))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(4 == strInTimeOp.ucSetpsModeIndex)//waite Read ACK
    {
        if(ucMlxIicReadAck(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    if(5 == strInTimeOp.ucSetpsModeIndex)//IIC_Start
    {
        if(ucMlxIicSendStart(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(6 == strInTimeOp.ucSetpsModeIndex)//Write 0x19  slaveAdd 00011001
    {
        if(ucMlxIicWriteCmdInTimer(strInTimeOp.ucClkCnt, g_strMlxInfoData.strCmdAdr.ucMLX90393_ReadCmdAdd))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(7 == strInTimeOp.ucSetpsModeIndex)//waite Read ACK
    {
        if(ucMlxIicReadAck(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(8 == strInTimeOp.ucSetpsModeIndex)//Read 8 Bits Status -  IC FeedBack
    {
        if(0 == strInTimeOp.ucClkCnt)strInTimeOp.ucRecDataTemp = 0;//�������
        if(ucMlxIicRead8BitsInTimer(strInTimeOp.ucClkCnt, strInTimeOp.ucPtr))//����ȥ��ȡһ��
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(9 == strInTimeOp.ucSetpsModeIndex)//waite Write ACK
    {
        if(0 == strInTimeOp.ucClkCnt)
        {
            if(ucCheckReadMlxStatusErro(strInTimeOp.ucRecDataTemp, MlxInNoneReg_Mode))//���״̬�ֻ᲻������⣬����������ȥ����һ��
            {
                g_strMlxInfoData.usMlx90393StatusErroTimes++;//���״̬λ������ �ۼӴ���֡������
            }
            strInTimeOp.ucRecDataTemp = 0;//����ʱ����
        }
        if(ucMlxIicWriteAck(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(10 == strInTimeOp.ucSetpsModeIndex)//Read 8 Bits X High
    {
        if(ucMlxIicRead8BitsInTimer(strInTimeOp.ucClkCnt, strInTimeOp.ucPtr))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(11 == strInTimeOp.ucSetpsModeIndex)//waite Write ACK
    {
        if(0 == strInTimeOp.ucClkCnt)//����ȡ��X��8λ��������
        {
            g_strMlxInfoData.uniusRecXdata.strData.ucData2 = strInTimeOp.ucRecDataTemp;
            strInTimeOp.ucRecDataTemp = 0;
        }
        if(ucMlxIicWriteAck(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(12 == strInTimeOp.ucSetpsModeIndex)//Read 8 Bits X Low
    {
        if(ucMlxIicRead8BitsInTimer(strInTimeOp.ucClkCnt, strInTimeOp.ucPtr))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(13 == strInTimeOp.ucSetpsModeIndex)//waite Write ACK
    {
        if(0 == strInTimeOp.ucClkCnt)//����8λ������
        {
            g_strMlxInfoData.uniusRecXdata.strData.ucData1 = strInTimeOp.ucRecDataTemp;
            strInTimeOp.ucRecDataTemp = 0;
        }
        if(ucMlxIicWriteAck(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(14 == strInTimeOp.ucSetpsModeIndex)//Read 8 Bits Y High
    {
        if(ucMlxIicRead8BitsInTimer(strInTimeOp.ucClkCnt, strInTimeOp.ucPtr))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
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
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(16 == strInTimeOp.ucSetpsModeIndex)//Read 8 Bits Y Low
    {
        if(ucMlxIicRead8BitsInTimer(strInTimeOp.ucClkCnt, strInTimeOp.ucPtr))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
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
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(18 == strInTimeOp.ucSetpsModeIndex)//waite Write Stop
    {
        if(ucMlxIicSendStop(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    if(19 == strInTimeOp.ucSetpsModeIndex)//IIC_Start
    {
        if(ucMlxIicSendStart(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(20 == strInTimeOp.ucSetpsModeIndex)//Write 0x18  slaveAdd		   00011000
    {
        if(ucMlxIicWriteCmdInTimer(strInTimeOp.ucClkCnt, g_strMlxInfoData.strCmdAdr.ucMLX90393_WriteCmdAdd))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(21 == strInTimeOp.ucSetpsModeIndex)//waite Read ACK
    {
        if(ucMlxIicReadAck(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(22 == strInTimeOp.ucSetpsModeIndex)//Write 0x36 slaveAdd  00110110
    {
        if(ucMlxIicWriteCmdInTimer(strInTimeOp.ucClkCnt, MLX90393_StartMeasurement))

        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(23 == strInTimeOp.ucSetpsModeIndex)//waite Read ACK
    {
        if(ucMlxIicReadAck(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(24 == strInTimeOp.ucSetpsModeIndex)//IIC_Start
    {
        if(ucMlxIicSendStart(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(25 == strInTimeOp.ucSetpsModeIndex)//Write 0x19  slaveAdd		   00011001
    {
        if(ucMlxIicWriteCmdInTimer(strInTimeOp.ucClkCnt, g_strMlxInfoData.strCmdAdr.ucMLX90393_ReadCmdAdd))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(26 == strInTimeOp.ucSetpsModeIndex)//waite Read ACK
    {
        if(ucMlxIicReadAck(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(27 == strInTimeOp.ucSetpsModeIndex)//Read 8 Bits Status -		   IC FeedBack
    {
        if(ucMlxIicRead8BitsInTimer(strInTimeOp.ucClkCnt, strInTimeOp.ucPtr))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(28 == strInTimeOp.ucSetpsModeIndex)//waite Write ACK
    {
        if(0 == strInTimeOp.ucClkCnt)
        {
            if(ucCheckReadMlxStatusErro(strInTimeOp.ucRecDataTemp, MlxInNoneReg_Mode))//���״̬�ֻ᲻������⣬����������ȥ����һ��
            {
                g_strMlxInfoData.usMlx90393StatusErroTimes++;//���״̬λ������ �ۼӴ���֡������
            }
            strInTimeOp.ucRecDataTemp = 0;
        }
        if(ucMlxIicWriteAck(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex ++;//�����ɱ��β�����ת����ѭ����
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
        return;
    }
    else if(29 == strInTimeOp.ucSetpsModeIndex)//waite Write Stop
    {
        if(ucMlxIicSendStop(strInTimeOp.ucClkCnt))
        {
            strInTimeOp.usCycOprtationIcCnt = strInTimeOp.ucClkCnt = 0;// ����ڱ��β���������������в����岽��������������´�ʹ��
            strInTimeOp.ucSetpsModeIndex = 0;//���һ��������ѭ�� �� �ٴο�����������
            return;
        }
        strInTimeOp.ucClkCnt++;//δ��ɣ��ۼӼ������������˳�
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
            if(ucCheckReadMlxStatusErro(g_strMlxInfoData.ucMlx90393StatusByte, MlxInNoneReg_Mode))//дһ�������
            {
                g_strMlxInfoData.usMlx90393StatusErroTimes++;//���״̬λ������ �ۼӴ���֡������
            }
            g_uiTimeNumCont1ms = Check90393DelayTime_ms;
            vMlx90393WaitTimes_ms(g_uiTimeNumCont1ms);//while(g_uiTimeNumCont1ms);//�滻Ϊ����ʽ��
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
        g_strMlxInfoData.ucMlx90393AssemblyN_DownFlag = FALSE;//�ж�Ψһ�Ĵż�װ���߼�
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
                usSampleX_90393 = FactoryRemoteMidXPos - fXmin * (g_strMlxInfoData.uniRecUsefulMemoryData.strMlxAdjust.usMiddleX - g_strMlxInfoData.uniusRecXdata.usData);//����һ�¼������߼���ϵ
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


