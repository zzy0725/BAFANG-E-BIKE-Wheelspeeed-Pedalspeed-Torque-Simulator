#include "pwm_cap.h"
#include "can.h"
#include "main.h"
#include "key.h"
#include "pwm.h"

u8  TIM2CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u16	TIM2CH1_CAPTURE_VAL;	//输入捕获值
u8  TIM2CH2_CAPTURE_STA=0;	//输入捕获状态		    				
u16	TIM2CH2_CAPTURE_VAL;	//输入捕获值
u8  TIM2CH3_CAPTURE_STA=0;	//输入捕获状态		    				
u16	TIM2CH3_CAPTURE_VAL;	//输入捕获值

u16 Timer2_Counter;
u16 PRM_Count;
u16 PRM_Hflag;
u16 key_PRM_Count=2;
u8 old_en_count=0;

u16 timeflag;
u8 high_data_flag=0;
u16 start_time_count;

//u16  old_EH[62]={750,1056,1209,1362,1515,1668,1974,2127,2280,2433,2586,2739,2892,3045,3200,3045,2892,2739,2586,2433,2280,2127,1974,1668,1515,1362,1209,1056,903,750,1056,1209,1362,1515,1668,1974,2127,2280,2433,2586,2739,2892,3045,3200,3045,2892,2739,2586,2433,2280,2127,1974,1668,1515,1362,1209,1056,903,750};
//定时器2通道1输入捕获配置

void TIM2_Init(u16 arr,u16 psc)
{	 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	//初始化定时器2 TIM2	 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;    //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;   //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);   //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许更新中断 ,允许CC1IE捕获中断	
  //TIM_Cmd(TIM2,ENABLE); 	//使能定时器2
}
//定时器2中断服务程序	     1ms进入中断   20ms

/////////////////////////////////////////////////////////////////*******************************通用*************************////////////////////////////

void TIM2_IRQHandler(void)    //新法规力矩 
{ 
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
				 TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  
				 Timer2_Counter++;	
				 PRM_Count++;	
				 if(Timer2_Counter==5)    //按下后，30ms 发一次力矩数据
				 {		
						Timer2_Counter=0;
						Can_Send_Msg(8);	  //发送can 750-3200mv 数据
				 }
				 if(FAN_ZHUAN_FLAG)       //正转
				 {
										if(PRM_Count==5)  
										{	
											 GPIO_SetBits(NEW_MOMENT1_PORT,NEW_MOMENT1_PIN);
										}										
										if(PRM_Count==10)  
										{			
												GPIO_SetBits(NEW_MOMENT2_PORT,NEW_MOMENT2_PIN);
										}
										 if(PRM_Count==15)    
										{
												GPIO_ResetBits(NEW_MOMENT1_PORT,NEW_MOMENT1_PIN);
										}
										if(PRM_Count==20)   
										{
												PRM_Count=0;
												GPIO_ResetBits(NEW_MOMENT2_PORT,NEW_MOMENT2_PIN);
										}   
				 }
				 if(!FAN_ZHUAN_FLAG)        //反转
				 {
										if(PRM_Count==5)   
										{	
												GPIO_SetBits(NEW_MOMENT2_PORT,NEW_MOMENT2_PIN);
										}			
										if(PRM_Count==10)   
										{			
												GPIO_SetBits(NEW_MOMENT1_PORT,NEW_MOMENT1_PIN);
										}
										 if(PRM_Count==15)  
										{
												 GPIO_ResetBits(NEW_MOMENT2_PORT,NEW_MOMENT2_PIN);
										}
										if(PRM_Count==20)    
										{
												PRM_Count=0;
												GPIO_ResetBits(NEW_MOMENT1_PORT,NEW_MOMENT1_PIN);
										} 
				 }
	}		
}	

