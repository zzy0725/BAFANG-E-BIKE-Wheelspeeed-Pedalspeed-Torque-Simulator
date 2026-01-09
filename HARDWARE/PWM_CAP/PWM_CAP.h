#ifndef __PWM_CAP_H
#define __PWM_CAP_H	

#include "sys.h"

extern u8  TIM2CH1_CAPTURE_STA;	//输入捕获状态		    				
extern u16	TIM2CH1_CAPTURE_VAL;	//输入捕获值

extern u8  TIM2CH2_CAPTURE_STA;	//输入捕获状态		    				
extern u16	TIM2CH2_CAPTURE_VAL;	//输入捕获值

extern u8  TIM2CH3_CAPTURE_STA;	//输入捕获状态		    				
extern u16	TIM2CH3_CAPTURE_VAL;	//输入捕获值
void TIM2_Init(u16 arr,u16 psc);

extern u16 PRM_Count;
extern u16 key_PRM_Count;
extern u8 high_data_flag;
extern u16 timeflag;



#endif
