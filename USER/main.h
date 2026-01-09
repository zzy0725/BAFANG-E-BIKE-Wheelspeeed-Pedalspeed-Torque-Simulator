#ifndef __MAIN_H
#define	__MAIN_H
#include "sys.h"

extern unsigned int pwm_value_1;  //初始值
extern unsigned int pwm_value_2;  //初始值
extern unsigned int dac_value_1;
extern unsigned int dac_value_2;
extern unsigned char DAC_ARRY[64];
extern void PWM_DAC1_Set(u16 vol);
extern void PWM_DAC2_Set(u16 vol);
extern unsigned int SIMU_ARRY[62];

extern  u16  old_EH[];
extern unsigned char key_count;
extern unsigned int TIM7_INIT_COUNT;

#endif

