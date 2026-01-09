#ifndef __PWM_H
#define __PWM_H
#include "sys.h"

#define FUNCTION_NUM  10

#define NEWSPEED1_PORT  GPIOB
#define NEWSPEED1_PIN     GPIO_Pin_13

#define NEWSPEED2_PORT  GPIOB
#define NEWSPEED2_PIN     GPIO_Pin_14

#define OLDSPEED_PORT  GPIOB
#define OLDSPEED_PIN     GPIO_Pin_12

#define SPEED1_Z_PORT  GPIOB
#define SPEED1_Z_PIN     GPIO_Pin_0

#define SPEED2_Z_PORT  GPIOB
#define SPEED2_Z_PIN     GPIO_Pin_1

#define SPEED_L_PORT  GPIOC
#define SPEED_L_PIN     GPIO_Pin_5

#define OLD_MOMENT_PORT  GPIOC
#define OLD_MOMENT_PIN     GPIO_Pin_4

#define OLD_MOMENT_DAC_PORT  GPIOC
#define OLD_MOMENT_DAC_PIN    GPIO_Pin_4

#define NEW_MOMENT1_PORT  GPIOA
#define NEW_MOMENT1_PIN    GPIO_Pin_4

#define NEW_MOMENT2_PORT  GPIOA
#define NEW_MOMENT2_PIN    GPIO_Pin_5

typedef struct MOS_OUT_GPIO     //封装gpio结构体
{
	GPIO_TypeDef *   PORT;
	uint16_t         PIN;
}MOS_OUT_GPIO;                 

extern unsigned int Tim2count;
extern unsigned int Tim3count;
extern unsigned int Tim4count;
extern unsigned int Tim5count;
extern unsigned int Tim6count;
extern unsigned int Tim7count;
extern unsigned char FAN_ZHUAN_FLAG;   //正反转标志位

void TIM1_Init(u16 arr,u16 psc);
void TIM4_Init(u16 arr,u16 psc);
void TIM3_Init(u16 arr,u16 psc);
void TIM5_Init(u16 arr,u16 psc);
void TIM6_Init(u16 arr,u16 psc);
void TIM7_Init(u16 arr,u16 psc);

void mos_out_gpio_init(void);

#endif
