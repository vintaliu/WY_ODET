#ifndef __STEP_MOTOR_H
#define __STEP_MOTOR_H	 
#include "sys.h"
#include "delay.h"


#define STEP_MOTOR_PORT	GPIOB	//定义IO接口所在组
#define STEP_MOTOR_A	GPIO_Pin_3	//定义IO接口
#define STEP_MOTOR_B	GPIO_Pin_4	//定义IO接口
#define STEP_MOTOR_C	GPIO_Pin_8	//定义IO接口
#define STEP_MOTOR_D	GPIO_Pin_9	//定义IO接口



void STEP_MOTOR_Init(void);//初始化
void MOTOLOOP_OFF (void);
void MOTOLOOP_4_S (u8 speed);
void MOTOLOOP_4_R (u8 speed);
void MOTOLOOP_4_L (u8 speed);
void MOTOLOOP_8_R (u8 speed);
void MOTOLOOP_8_L (u8 speed);
void MOTOLOOP_4_F (u8 speed);
void MOTOLOOP_2_F (u8 speed);


		 				    
#endif
