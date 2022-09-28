/*
*********************************************************************************************************
*
*	模块名称 : BSP模块
*	文件名称 : bsp.h
*	版    本 : v1.0.00
*	说    明 : 硬件底层驱动程序的主文件
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

/* 供外部调用函数 */
void System_Init(void);
extern void vInitPortAsWaiteMode(void);
extern void vSysTemSourceAsActive(void);



#endif

/********************************************* End of file **********************************************/
