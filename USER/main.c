
//2019/04/07 增加正反转，长按3s以上加按键输出反转信号，长按3s以上减按键输出正转信号。默认正转
//2019/12/02 增加老法规力矩、老法规外测速

#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
#include "PWM_CAP.h"
#include "stdio.h"
#include "usart.h"
#include "can.h"
#include "key.h"
#include "STM32F10x_RCC.h"
#include "main.h"
#include "iwdg.h"

unsigned int pwm_value_1=250-1;//初始值
unsigned int pwm_value_2=500-1; //初始值 360
unsigned int dac_value_1=33;
unsigned int dac_value_2=33;

unsigned int pwm_frequency;     
unsigned char add_frequency=5; 
unsigned char dec_frequency=5;
unsigned char key_count=1;
unsigned char test[]={2,2,3,4,5,6};   
unsigned char old_moment_count;    //老法规数组计数
//幅值范围：750~3200                  半个周期：64/4=16 
//新法规模拟数组
unsigned int SIMU_ARRY[62]={0x02ee,0x0387,0x0420,0x04b9,0x0552,0x05eb,0x0684,0x07b6,0x084f,0x08eb,0x0981,0x0a1a,0x0ab3,0x0b4c,0x0be5,0x0c80,0x0be5,0x0b4c,0x0ab3,0x0a1a,0x0981,0x08eb,0x084f,0x07b6,0x0684,0x05eb,0x0553,0x0489,0x0420,0x0387,0x02ee,0x0387,0x0420,0x04b9,0x0552,0x05eb,0x0684,0x07b6,0x084f,0x08eb,0x0981,0x0a1a,0x0ab3,0x0b4c,0x0be5,0x0c80,0x0be5,0x0b4c,0x0ab3,0x0a1a,0x0981,0x08eb,0x084f,0x07b6,0x0684,0x05eb,0x0553,0x0489,0x0420,0x0387,0x02ee};
//老法规模拟数组
u16  old_EH[30]={75,105,120,136,151,166,197,212,228,243,258,273,289,304,320,304,289,273,258,243,228,212,197,166,151,136,120,105,90,75};	
unsigned char rev_long=0;
unsigned int TIM7_INIT_COUNT=79;   //初始值 3km/h
unsigned char first_flag;
//设置输出电压
//vol:0~330,代表0~3.3V
//设置输出电压
//vol:0~330,代表0~3.3V
void PWM_DAC1_Set(u16 vol)  
{
	float temp=vol;
	temp/=100;
	temp=temp*256/3.3;
	TIM_SetCompare1(TIM3,temp);
}  
	//  TIM_SetCompare1(TIM3,pwm_value_1);       //PWM1路输出      
	//	TIM_SetCompare2(TIM3,pwm_value_2);       //PWM2路输出
	//TIM2_Cap_Init(0XFFFF,72-1);	  //以1Mhz的频率计数   //输入捕获
	//	TIM2_Cap_Init(2,3599);         //113hz
 //	  PWM_DAC1_Set(dac_value_1);           //DAC_1 输出0.3V
 //	  PWM_DAC2_Set(dac_value_2);           //DAC_2 输出0.3V
RCC_ClocksTypeDef get_rcc_clock;  

int main(void)
{	   
		delay_init();	      	 //延时函数初始化	  
  	//LED_Init();		  	   //初始化与LED连接的硬件接口
		uart_init(9600);	 			//9600	
		KEY_Init();					   //按键初始化
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  	// 设置中断优先级分组1，1位抢占式优先级，3位响应式优先级 

		TIM2_Init(10,7199);           //1ms进入中断
		TIM3_Init(255,0);              //定时器3 DAC模拟数组    
		TIM4_Init(100,7199);        //10ms进入中断
		TIM5_Init(10,7199);          //1ms进入中断
		TIM6_Init(30,7199);         
    TIM7_Init(TIM7_INIT_COUNT,7199);   //新法规测速    1.5hz
   	mos_out_gpio_init();      //mos输出引脚初始化
	// 	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_3tq,12,CAN_Mode_Normal);	   //can外设初始化
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_3tq,6,CAN_Mode_Normal);	   //can外设初始化  500K
	  IWDG_Init(4,625);    //与分频数为64,重载值为625,溢出时间为1s	   	
	
		while(1)
		{				
			  IWDG_Feed();                    //喂狗
  	    check_hit();                                                     //检测自锁按键转态
		   	hit_process();                                                 //按键进程
				if(!fbits.SUBFLAG.SUBFLAG_1.NEW_MOMENT_FLAG)        //不按下，一直发750mv   
				{
				 			delay_ms(100);                                      
				  		Can_Send_Msg(4);                                             //发送can数据
				}
				if(fbits.SUBFLAG.SUBFLAG_1.OLD_MOMENT_FLAG)     //老法规发送模拟力矩信号
				{
						old_moment_count++;
						if(old_moment_count==30)  old_moment_count=0;
						PWM_DAC1_Set(old_EH[old_moment_count]);           //DAC模拟数组	
				}				
				else
				{
				   	PWM_DAC1_Set(old_EH[0]);
				}
	} 	
}

