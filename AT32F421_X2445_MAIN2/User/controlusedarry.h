/*
       "LG_X2445F"  Design By LG_qiuzhi
       V1.0   2013.6
       used mcu  STM32F103C8(6)x
*/
#ifndef __CONTROLUSEARRY_H_
#define __CONTROLUSEARRY_H_//f1404z
struct strRemotePosAsPwm_
{
    // 	unsigned int uiCurrentAngle[90];
    unsigned int uiPosAsMotor1_APwm[90];
    unsigned int uiPosAsMotor1_BPwm[90];
    unsigned int uiPosAsMotor2_APwm[90];
    unsigned int uiPosAsMotor2_BPwm[90];
};
extern struct strRemotePosAsPwm_ Quadrant1PosAsPwm, Quadrant2PosAsPwm, Quadrant3PosAsPwm, Quadrant4PosAsPwm;
extern  unsigned short FourQuadrantPosAsPwmArryMoto1_A[380];
extern  unsigned short FourQuadrantPosAsPwmArryMoto2_A[380];
extern  unsigned short FourQuadrantPosAsPwmArryMoto1_B[380];
extern  unsigned short FourQuadrantPosAsPwmArryMoto2_B[380];
extern unsigned char ucMoto1LeftAdc[60];
extern unsigned char ucMoto1RightAdc[60];
extern unsigned char ucMoto2LeftAdc[60];
extern unsigned char ucMoto2RightAdc[60];


#endif
