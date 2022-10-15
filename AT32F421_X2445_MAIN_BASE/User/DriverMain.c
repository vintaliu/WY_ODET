/*
       "LG_X2445M_V2"  Design By LG_qiuzhi
       V1.0   2021.04
       used mcu  AT32F421C8
*/
//file DriverMain.C

#include "DriverConst.h"
unsigned char ucEmgStopEn = 0;//�Ƿ��м�ͣ�¼��ķ���
unsigned char ucStsTemState = PowerDownMode;//ϵͳ״̬ȫ�ֱ���
unsigned char ucErroType = ErroNoCheckSys;//ϵͳ����ȫ�ֱ���
unsigned char ucNewDirectCont = SysNoDirection , ucOldDirectCont = SysNoDirection;//
unsigned char ucSpeedGrad = SpeedGrad5;//�ٶȿ̶�ȫ�ֱ���
unsigned char ucRunAsMode = SysInStandby;//ϵͳ��һ������ģʽָʾ����  �տ�����ʱ��  ������һ���͹��ĵ�ģʽ
unsigned char ucPowerKeyPressTimes = 0x01;//��Դ���´���������
unsigned char ucShowPowerCont = 0;//������ʾ������
unsigned char ucInChargePinLowCont = 0;
unsigned char ucGiveVoltageToOldOneTime = FALSE;
unsigned char ucEnAdcCalc = TRUE;
unsigned char ucAutoBattIdenConfigData = 0;//�Ե��ʶ�����ù���־
unsigned char ucFirstStartOneTimeFlag = FALSE;//�ػ����ΪFALSE���������ΪTRUE
//unsigned int ucFirstStartOneTime = 0;
#define UseHardWareWatchDog//ֻΪ���Է��㡣
/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: ��׼c�������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int main(void)
{
    unsigned char ucShowPowerTemp = 0;
    vInitPortAsWaiteMode();//�ڸ��ϵ��ʱ�� ���³�ʼ��һ���˿� �û���һֱ�ڵȴ�һ�����������ź�
    if(!ReadKeyOfOn_OffFlag)//���һֱ���޿��� ��Ϣ
    {
        vNoKeyInStandbyMode();//����տ�����ȥ����һ���͹��ĵ�ģʽ
    }	
		ucEnAdcCalc = TRUE;
    System_Init();
    vSysTemSourceAsActive();
    //if(C_ReadProgramEnPin)vDealReadProgramPin();//�����ʹ���ڱ༭״̬ 2.06_08ȡ��������ܸ����ڼ��ɲ����
    ucPowerOnCotrol();//�ϵ��һ�����ƣ����û�а������£�ǿ�в�������

    vReadSysTempPare(TRUE);//ϵͳ������ʹ�á���������Ĳ���ȷ��ǿ������д���µĳ�ʼ������������

    if(ReadTeachPinIsConect)
    {
        vShowPoweChange();//���������ʱ���⵽���ڳ�磬һֱ�ȳ�����
    }
    if((SelfCheckMode == ucStsTemState ) || (SysInAutoPowerOn == ucStsTemState )) //����ϵ�ɹ����¼����ϵͳ������ģ��
    {
        vCheckSystemInfo();//ȥ����ģ���Ƿ������ش���
        if(ErroNoCheckSys == ucErroType)ucErroType = ErroNoErro;//���һ�������������ʼ���Ĵ����־
    }
    else
    {
        while (1)vAutoPowerOffTimeFlag();//�������صȴ��󣬲��õ�����κ��¡�
    }
    vSetTeacherPortInt();//ֻ����û�д����ҳ�ʼ����ȷ������²�ʹ��ʾ�̿�
    if(ucErroType)vShowErroToDis(0);
#ifdef UseHardWareWatchDog
    //vInitWatchDog(4, 1250);//vInitWatchDog(4, 1250);
#endif
    //		while(1);
    //    vSetMonitorKDPort();//���ü�ؽŵ��������״̬
    if(FALSE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-ɲ���������0-פ�³����ɲ������6�����20190110
    {
        ucConnectBreakFlag = FALSE;

    }
    //////////////////////////////////////STM32 GD32 UID�����㷨��ʵ��
    //        if(ucCrcUidAes());//���������ȷ �����������������
    //        else//����������벻��ȷ��һֱ������������������
    //        {
    //            while (1)
    //            {
    //                while(ucErroType)//���Ӳ���д���һ��ȥ����
    //                {
    //                    vAutoPowerOffTimeFlag();//��Ҫ���κεĴ��������һ��Ҫ��ʱ����
    //                }
    //            }
    //        }
    /////////////////////////////////////
    //		while (1);
    if(FALSE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-ɲ���������0-פ�³����ɲ������6�����20190110
    {
        //פ�£�
        CtlBrakeBikeEn;//��λPC13
        strSysInfo.uiSysBreakFlag = 0;//0 �� פ��
    }
    else
    {
        CtlBrakeBikeDisEn;//��λPC13
        strSysInfo.uiSysBreakFlag = 1;//1 �ǵ��ɲ��
        PwmDataInit();
    }
    if(0 == (ReadMlxReadTimerState & 0x0001))
    {
        EnAbleMlxTimerReadTnterrupt;//ʹ��TMR14��ʱ��
    }		
    CtrlExtSpeekerDis;////ǿ����������(PB11)����
//		while (1)       
//		{
//			 if(ucTag400ms)//����ڿ��е�״̬�£������ʾ������������������ʱ��Ҫȥ������
//        {
//            if(ucAnlyFree)vAnalyTeacherDataOfTime();
//            ucTag400ms = FALSE;
//        }
//		}
    while (1)
    {
        if(ucTag5ms)//�����Ƿ���ҡ�˱仯���Դ���
        {
            if(FALSE == ucGiveVoltageToOldOneTime)
            {
                uiOldBattPowerValue = strSysInfo.uiCalcSysShowPower = strSysInfo.uiSysTemPower;//��������ֵ ��ֵ
#ifdef AutoBattIdenConfigDef
                if(0x00 == ucAutoBattIdenConfigData)//���û�����õ�����Ͳ��ҵ������Ϊ��
                {
                    if(strSysInfo.uiCalcSysShowPower > 3190)//��������ĵ�ѹ����28.3v˵����﮵�  ��ѹ�ȽϹ�ϵ  0.0088623046
                    {
                        ucAutoBattIdenConfigData |= AutoBattIdenConfigAsLion; //����Ϊ﮵�
                    }
                    else
                    {
                        ucAutoBattIdenConfigData |= AutoBattIdenConfigAsPb; //����ΪǦ��
                    }
                    ucAutoBattIdenConfigData |= AutoBattIdenConfigFlag;//���ù���־
                    BKP_WriteBackupRegister(BKP_DR5, 0xAA55);//д���ݼĴ������ݱ�־
                    BKP_WriteBackupRegister(BKP_DR4, ucAutoBattIdenConfigData);//д������ͱ�־
                }

                if(AutoBattIdenConfigFlag & ucAutoBattIdenConfigData)//����Ѿ�д������ͱ�־�������´ο���ʱ���ص�ѹ����ʱ�����Ϊ﮵�
                {
                    if(AutoBattIdenConfigAsPb & ucAutoBattIdenConfigData)//������õ���Ǧ�ᣬ���ǿ��������õ�
                    {
                        if(strSysInfo.uiCalcSysShowPower > 3190)//��������ĵ�ѹ����28v˵����﮵�  ��ѹ�ȽϹ�ϵ  0.0088623046
                        {
                            ucAutoBattIdenConfigData |= AutoBattIdenConfigAsLion; //����Ϊ﮵�
                        }
                    }
                    BKP_WriteBackupRegister(BKP_DR5, 0xAA55);
                    BKP_WriteBackupRegister(BKP_DR4, ucAutoBattIdenConfigData);
                }
                if(AutoBattIdenConfigAsLion & ucAutoBattIdenConfigData)//ǿ�Ƶ������ת��
                {
                    uniDiverInfo.strDirverAtt.unMotorInfo.bits.bLilonOrQiansuan = TRUE;
                    vSendOneByteOrder(OrderBatteryType, 0x01);//д�������  2020-03-03
                }
                else
                {
                    uniDiverInfo.strDirverAtt.unMotorInfo.bits.bLilonOrQiansuan = FALSE;
                    vSendOneByteOrder(OrderBatteryType, 0x00);//д�������  2020-03-03
                }
#endif
                g_uiTimeNumCont1ms = 20;
                while(g_uiTimeNumCont1ms);
                vSendSysPower(strSysInfo.uiCalcSysShowPower + 20);//�״�ָʾ�µ���
								g_uiTimeNumCont1ms = 20;
                while(g_uiTimeNumCont1ms);
                vSendSysPower(strSysInfo.uiCalcSysShowPower - 20);//�״�ָʾ�µ���
                ucGiveVoltageToOldOneTime = TRUE;//ֻ��ʾһ��
                CtrlExtSpeekerDis;//ǿ�Ʋ���
                if(ucFirstStartOneTimeFlag == 0)
                {
                    strMoto1Info.uiMotorVoltageBase = (strMoto1Info.uiRightHBridgeHaveFilterAdc + strMoto1Info.uiLeftHBridgeHaveFilterAdc) / 2;
                    strMoto2Info.uiMotorVoltageBase = (strMoto2Info.uiRightHBridgeHaveFilterAdc + strMoto2Info.uiLeftHBridgeHaveFilterAdc) / 2;

                    strMoto1Info.uiMotorCurrentBase = strMoto1Info.uiMotorCurrent;	//��� ��Դ�ȶ��ˣ�Ҫ����һ�������Ļ�׼ֵ
                    strMoto2Info.uiMotorCurrentBase = strMoto2Info.uiMotorCurrent;
                    ucFirstStartOneTimeFlag = 1;
                }
            }
            ucEnAdcCalc = TRUE;//����ʵʱ�ؿ��� AD����
            if((strSysInfo.uiCalcSysShowPower > CheckLowAlarmVoltage) && (ucFirstStartOneTimeFlag)) //�͵�ѹ����ִ��ɲ������
            {
                vContolRealize();//�����㷨��ʵ��
            }
            else
            {
                PwmDataInit();
            }
            vCalcBataAsShow(FALSE);//������ڿ��е�ʱ��
            ucTag5ms = FALSE;
            if((strMoto1Info.uiMotorCurrent < 500) || (strMoto1Info.uiMotorCurrent > 3500))
            {
                vShowErroToDis(ErroRelayShort);//����ڿ������������й����в����ĵ�����С��˵����������
            }
            if((strMoto2Info.uiMotorCurrent < 500) || (strMoto2Info.uiMotorCurrent > 3500))
            {
                vShowErroToDis(ErroRelayShort);//����ڿ������������й����в����ĵ�����С��˵����������
            }
        }
        if(ucTag300ms)
        {
            ucShowPowerCont++;
            ucShowPowerCont %= 100;
            //if((1 == ucShowPowerCont % 5)&&(TRUE == ucConnectBreakFlag))vCheckErroInRun(); //������ҡ������������£������������ѹ�����
            if(TRUE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-ɲ���������0-פ�³����ɲ������6�����20190110
            {
                // if(4 == ucShowPowerCont % 5)vCheckErroInRun(); //������ҡ������������£������������ѹ�����
                ucShowPowerTemp = 9;
            }
            else
            {
                ucShowPowerTemp = 1;
            }
            if(ucShowPowerTemp == ucShowPowerCont % 10)//�����Եؼ��Ӳ������
            {
                vSendSysPower(strSysInfo.uiCalcSysShowPower);//��ʾ�µ�
            }
            if(ReadTeachPinIsConect)//����ǳ��״̬
            {
                ucInChargePinLowCont++;
                if(ucInChargePinLowCont >= InChargePinLowCont)
                {
                    vSendSingleOrder(OrderWatchDogClose);//�ؼ�ؿ��Ź�
                    vShowPoweChange();//��ָʾ��ѭ��ָʾ��
                }
            }
            else ucInChargePinLowCont = 0;
            ucTag300ms = FALSE;
        }
        if(ucTag400ms)//����ڿ��е�״̬�£������ʾ������������������ʱ��Ҫȥ������
        {
            if(ucAnlyFree)vAnalyTeacherDataOfTime();
            ucTag400ms = FALSE;
        }
        while(ucErroType)//���Ӳ���д���һ��ȥ����
        {
            vAutoPowerOffTimeFlag();//��Ҫ���κεĴ��������һ��Ҫ��ʱ����
        }
        if(FALSE ==  uniDiverInfo.strDirverAtt.unMotorInfo.bits.bBreakEn)//1-ɲ���������0-פ�³����ɲ������6�����20190110
        {
					  //פ�£���λPC13
            GPIO_ResetBits(C_BreakControl_Port, C_BreakControl_Pin);
        }
        if((ucSpeedDowenFlag) && (ucFirstStartOneTimeFlag)) //(ucFirstStartOneTimeFlag == TRUE))//&& (FALSE == ucConnectBreakFlag)�Ѿ�ɲ����ɣ����Ҽ��ɲ�����Ѿ�������
        {
            vAutoSlopeStopFuction();
        }
    }
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

/********************************************* End of file **********************************************/
