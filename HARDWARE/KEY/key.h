#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

typedef struct  tagbiit{
	union{
						struct{   
												unsigned char   NEW_SPEED_FLAG: 1;     //新法规测速标志位
												unsigned char   OLD_SPEED_FLAG: 1;   //老法规测速标志位
												unsigned char   ZHONGZHI_ZHULI_FLAG:1;  //中置助力标志位
												unsigned char 	LUGU_ZHULI_FLAG:1;         //轮毂助力标志位
												unsigned char  OLD_MOMENT_FLAG :1;    //老法规力矩标志位
												unsigned char NEW_MOMENT_FLAG:1;    //新法规力矩标志位
										}SUBFLAG_1 ;
					struct{   
												unsigned char flag;		
									}SUBFLAG_2;
	}SUBFLAG  ;
}FBITS;

#define  ture 1
#define flase   0

#define KEY_DEC_PORT  GPIOC
#define  KEY_DEC_PIN    GPIO_Pin_13

#define KEY_ADD_PORT  GPIOB
#define  KEY_ADD_PIN    GPIO_Pin_9

#define NEW_CHACK_SPEED   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)   //新法规测速
#define OLD_CHACK_SPEED   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)    //老法规测速
#define ZHONGZHI_ZHULI   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)     //速度助力中置
#define LUGU_ZHULI  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)        //速度助力轮毂
#define OLD_MOMENT GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)       //老法规力矩
#define NEW_MOMENT GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)    //新法规力矩

//2019/04/01 移植key状态机

#define PRESS_KEY1 0x01 
#define PRESS_KEY2 0x02
#define PRESS_KEY3 0x04
#define PRESS_KEY4 0x08

#define KEY_UP 0x10
#define KEY_DOWN 0x20
#define KEY_LONG 0x40 
#define NO_KEY 0x00

extern unsigned char key_sta;   //全局初始化按键变量
extern uint8_t key_value;
extern  FBITS  fbits;

void KEY_Init(void);  //IO初始化
u8 KEY_Scan(u8 mode);  	//按键扫描函数		
void check_hit(void);
void hit_process(void);

extern uint8_t ReadKeyValue(void);
extern uint8_t Get_Key(void);
#endif

