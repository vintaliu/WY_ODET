/*
*********************************************************************************************************
*
*	ģ������ : BSPģ��
*	�ļ����� : bsp.h
*	��    �� : v1.0.00
*	˵    �� : Ӳ���ײ�������������ļ�
*
*********************************************************************************************************
*/

#ifndef __BSP_H
#define __BSP_H

#include <at32f4xx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "DriveDelay.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* ���ⲿ���ú��� */
void System_Init(void);
extern void vInitPortAsWaiteMode(void);
extern void vSysTemSourceAsActive(void);



#endif

/********************************************* End of file **********************************************/
