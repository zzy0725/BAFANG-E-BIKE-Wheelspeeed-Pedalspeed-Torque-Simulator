#include "pwm.h"
#include "led.h"
#include "main.h"
#include "pwm_cap.h"
#include "key.h"
#include "can.h"

unsigned int Tim1count;
unsigned int Tim2count;
unsigned int Tim3count;
unsigned int Tim4count;
unsigned int Tim5count;
unsigned int Tim6count;
unsigned int Tim7count;
unsigned  char test_count;
unsigned char FAN_ZHUAN_FLAG=1;

MOS_OUT_GPIO mos_out_gpio[FUNCTION_NUM]={
		{NEWSPEED1_PORT,NEWSPEED1_PIN},
		{NEWSPEED2_PORT,NEWSPEED2_PIN},
		{OLDSPEED_PORT,OLDSPEED_PIN},
		{SPEED1_Z_PORT,SPEED1_Z_PIN},
		{SPEED2_Z_PORT,SPEED2_Z_PIN},
		{SPEED_L_PORT,SPEED_L_PIN},
		{OLD_MOMENT_PORT,OLD_MOMENT_PIN},
		{OLD_MOMENT_DAC_PORT,OLD_MOMENT_DAC_PIN},
		{NEW_MOMENT1_PORT,NEW_MOMENT1_PIN},
		{NEW_MOMENT2_PORT,NEW_MOMENT2_PIN}
};

//PWMÊä³ö³õÊ¼»¯
//arr£º×Ô¶¯ÖØ×°Öµ
//psc£ºÊ±ÖÓÔ¤·ÖÆµÊý
void mos_out_gpio_init()
{
	u8 i;
	GPIO_InitTypeDef GPIO_InitStructure;	
	for(i=0;i<FUNCTION_NUM;i++)
	{
		GPIO_InitStructure.GPIO_Pin = mos_out_gpio[i].PIN;                  
		GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(mos_out_gpio[i].PORT,&GPIO_InitStructure);
		GPIO_ResetBits(mos_out_gpio[i].PORT,mos_out_gpio[i].PIN);   //³õÊ¼»¯È«²¿ÖÃµÍµçÆ½
	}	
}

void TIM3_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //Ê¹ÄÜGPIOÍâÉèÊ±ÖÓÊ¹Ä
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //¸´ÓÃÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	                                                                 	
	TIM_TimeBaseStructure.TIM_Period = arr; //ÉèÖÃÔÚÏÂÒ»¸ö¸üÐÂÊÂ¼þ×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷ÖÜÆÚµÄÖµ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //ÉèÖÃÓÃÀ´×÷ÎªTIMxÊ±ÖÓÆµÂÊ³ýÊýµÄÔ¤·ÖÆµÖµ  ²»·ÖÆµ
	TIM_TimeBaseStructure.TIM_ClockDivision =0; //ÉèÖÃÊ±ÖÓ·Ö¸î:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIMÏòÉÏ¼ÆÊýÄ£Ê½
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //¸ù¾ÝTIM_TimeBaseInitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯TIMxµÄÊ±¼ä»ùÊýµ¥Î»
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //Ñ¡Ôñ¶¨Ê±Æ÷Ä£Ê½:TIMÂö³å¿í¶Èµ÷ÖÆÄ£Ê½2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //±È½ÏÊä³öÊ¹ÄÜ
	TIM_OCInitStructure.TIM_Pulse = 0; //ÉèÖÃ´ý×°Èë²¶»ñ±È½Ï¼Ä´æÆ÷µÄÂö³åÖµ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //Êä³ö¼«ÐÔ:TIMÊä³ö±È½Ï¼«ÐÔ¸ß
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //¸ù¾ÝTIM_OCInitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉèTIMx
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM2ÖÐ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //ÏÈÕ¼ÓÅÏÈ¼¶1¼¶
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //´ÓÓÅÏÈ¼¶0¼¶
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQÍ¨µÀ±»Ê¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);   //¸ù¾ÝNVIC_InitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉèNVIC¼Ä´æÆ÷ 
	
	 TIM_CtrlPWMOutputs(TIM4,ENABLE);	//MOE Ö÷Êä³öÊ¹ÄÜ	
	 
  	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH1Ô¤×°ÔØÊ¹ÄÜ	 
	
	 TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //ÔÊÐí¸üÐÂÖÐ¶Ï ,ÔÊÐíCC1IE²¶»ñÖÐ¶Ï
   TIM_Cmd(TIM3, ENABLE);   //Ê¹ÄÜTIM1	

}

//¶¨Ê±Æ÷4 pwmÊä³ö
void TIM4_Init(u16 arr,u16 psc)
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //ÉèÖÃÔÚÏÂÒ»¸ö¸üÐÂÊÂ¼þ×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷ÖÜÆÚµÄÖµ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //ÉèÖÃÓÃÀ´×÷ÎªTIMxÊ±ÖÓÆµÂÊ³ýÊýµÄÔ¤·ÖÆµÖµ  ²»·ÖÆµ
	TIM_TimeBaseStructure.TIM_ClockDivision =0; //ÉèÖÃÊ±ÖÓ·Ö¸î:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIMÏòÉÏ¼ÆÊýÄ£Ê½
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //¸ù¾ÝTIM_TimeBaseInitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯TIMxµÄÊ±¼ä»ùÊýµ¥Î»
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM2ÖÐ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //ÏÈÕ¼ÓÅÏÈ¼¶1¼¶
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //´ÓÓÅÏÈ¼¶0¼¶
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQÍ¨µÀ±»Ê¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);   //¸ù¾ÝNVIC_InitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉèNVIC¼Ä´æÆ÷ 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);//ÔÊÐí¸üÐÂÖÐ¶Ï ,ÔÊÐíCC1IE²¶»ñÖÐ¶Ï	
	TIM_Cmd(TIM4, ENABLE);  //Ê¹ÄÜTIM1
}

void TIM5_Init(u16 arr,u16 psc)
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //ÉèÖÃÔÚÏÂÒ»¸ö¸üÐÂÊÂ¼þ×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷ÖÜÆÚµÄÖµ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //ÉèÖÃÓÃÀ´×÷ÎªTIMxÊ±ÖÓÆµÂÊ³ýÊýµÄÔ¤·ÖÆµÖµ  ²»·ÖÆµ
	TIM_TimeBaseStructure.TIM_ClockDivision =0; //ÉèÖÃÊ±ÖÓ·Ö¸î:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIMÏòÉÏ¼ÆÊýÄ£Ê½
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //¸ù¾ÝTIM_TimeBaseInitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯TIMxµÄÊ±¼ä»ùÊýµ¥Î»
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;    //TIM2ÖÐ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //ÏÈÕ¼ÓÅÏÈ¼¶1¼¶
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //´ÓÓÅÏÈ¼¶0¼¶
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQÍ¨µÀ±»Ê¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);   //¸ù¾ÝNVIC_InitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉèNVIC¼Ä´æÆ÷ 
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //ÔÊÐí¸üÐÂÖÐ
//	TIM_Cmd(TIM5, ENABLE);  //Ê¹ÄÜTIM
}

void TIM6_Init(u16 arr,u16 psc)
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //ÉèÖÃÔÚÏÂÒ»¸ö¸üÐÂÊÂ¼þ×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷ÖÜÆÚµÄÖµ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //ÉèÖÃÓÃÀ´×÷ÎªTIMxÊ±ÖÓÆµÂÊ³ýÊýµÄÔ¤·ÖÆµÖµ  ²»·ÖÆµ
	TIM_TimeBaseStructure.TIM_ClockDivision =0; //ÉèÖÃÊ±ÖÓ·Ö¸î:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIMÏòÉÏ¼ÆÊýÄ£Ê½
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //¸ù¾ÝTIM_TimeBaseInitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯TIMxµÄÊ±¼ä»ùÊýµ¥Î»
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM2ÖÐ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //ÏÈÕ¼ÓÅÏÈ¼¶1¼¶
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  //´ÓÓÅÏÈ¼¶0¼¶
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQÍ¨µÀ±»Ê¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);   //¸ù¾ÝNVIC_InitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉèNVIC¼Ä´æÆ÷ 
	
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);//ÔÊÐí¸üÐÂÖÐ¶Ï ,ÔÊÐíCC1IE²¶»ñÖÐ¶Ï	
	//TIM_Cmd(TIM6, ENABLE);  //Ê¹ÄÜTIM1
}

void TIM7_Init(u16 arr,u16 psc)
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //ÉèÖÃÔÚÏÂÒ»¸ö¸üÐÂÊÂ¼þ×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷ÖÜÆÚµÄÖµ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //ÉèÖÃÓÃÀ´×÷ÎªTIMxÊ±ÖÓÆµÂÊ³ýÊýµÄÔ¤·ÖÆµÖµ  ²»·ÖÆµ
	TIM_TimeBaseStructure.TIM_ClockDivision =0; //ÉèÖÃÊ±ÖÓ·Ö¸î:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIMÏòÉÏ¼ÆÊýÄ£Ê½
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //¸ù¾ÝTIM_TimeBaseInitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯TIMxµÄÊ±¼ä»ùÊýµ¥Î»
	TIM_ARRPreloadConfig(TIM7,ENABLE);  
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;  //TIM2ÖÐ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //ÏÈÕ¼ÓÅÏÈ¼¶1¼¶
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;  //´ÓÓÅÏÈ¼¶0¼¶
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQÍ¨µÀ±»Ê¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);   //¸ù¾ÝNVIC_InitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉèNVIC¼Ä´æÆ÷ 
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);//ÔÊÐí¸üÐÂÖÐ¶Ï ,ÔÊÐíCC1IE²¶»ñÖÐ¶Ï	
//TIM_Cmd(TIM7, ENABLE);  //Ê¹ÄÜTIM1
}

//void TIM1_UP_IRQHandler(void)       //10ms¼ì²âÒ»´Î
//{                      
//    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
//    {  
//        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //Çå³ýÖÐ¶Ï±êÖ¾Î»
//				
//       Tim3count++;
//			
//       if(Tim3count==1)
//			 {
//			   GPIO_SetBits(NEW_MOMENT1_PORT,NEW_MOMENT1_PIN);
//			 }
//			 if(Tim3count==2)
//			 {
//				  Tim3count=0;
//				 GPIO_ResetBits(NEW_MOMENT1_PORT,NEW_MOMENT1_PIN);
//			 }
//    }          
//}  

void TIM3_IRQHandler(void)     //ÀÏ·¨¹æÁ¦¾Ø
{ 
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		 TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		if(fbits.SUBFLAG.SUBFLAG_1.OLD_MOMENT_FLAG)
		{
			    	Tim3count++; 
				  	if(FAN_ZHUAN_FLAG)
						{
										 if(Tim3count==8000)      
										 {
												 GPIO_SetBits(OLD_MOMENT_PORT,OLD_MOMENT_PIN);
										 }
										else if(Tim3count==16000)
										{
													GPIO_SetBits(SPEED_L_PORT,SPEED_L_PIN);			
										}   
										else if(Tim3count==24000)
										{ 
													GPIO_ResetBits(OLD_MOMENT_PORT,OLD_MOMENT_PIN);
											
										}   
										else if(Tim3count==32000)
										{
													Tim3count=0;
													GPIO_ResetBits(SPEED_L_PORT,SPEED_L_PIN);
										}   
						}
						if(!FAN_ZHUAN_FLAG)
						{
										 if(Tim3count==8000)      
										 {		
													GPIO_SetBits(SPEED_L_PORT,SPEED_L_PIN);			
										 }
										else if(Tim3count==16000)
										{
													GPIO_SetBits(OLD_MOMENT_PORT,OLD_MOMENT_PIN);
										}   
										else if(Tim3count==24000)
										{
													GPIO_ResetBits(SPEED_L_PORT,SPEED_L_PIN);
										}   
										else if(Tim3count==32000)
										{
													Tim3count=0; 		
													GPIO_ResetBits(OLD_MOMENT_PORT,OLD_MOMENT_PIN);
										}   
						}
		}
	}
}

void TIM4_IRQHandler(void)  //Õý·´×ªÇÐ»» 
{ 
	uint8_t Key_return=0;
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
	{
			Tim4count++;
		  if(Tim4count==200)Tim4count=0;
		  TIM_ClearITPendingBit(TIM4, TIM_IT_Update);		
			 Key_return=ReadKeyValue();         //°´¼ü¼ì²â
		   	if(Key_return & KEY_UP)						//here can adjust period of Dual-Hall signal.
				/*
				5-150ms;6-175ms;9-250ms;11-300ms;12-325ms;30-775ms;79-2000ms;   times=26
				*/
				{ 
								if(Key_return & PRESS_KEY1)
								{
															key_PRM_Count--;
														 if(key_PRM_Count==1)
														 {
																key_PRM_Count=2;
														 }	 
														 if(key_PRM_Count==2)
														 {	
															  TIM7->ARR=120;
														  	TIM6->ARR=120;
														 }
														 if(key_PRM_Count==3)
														 {
																TIM7->ARR=100;
																TIM6->ARR=100;
														 }
														 if(key_PRM_Count==4)
														 {
											
																TIM7->ARR=80;
														    TIM6->ARR=80;
														 }
														 if(key_PRM_Count==5)
														 {	
																TIM7->ARR=60;
											      	 	TIM6->ARR=60;
														 }
														 if(key_PRM_Count==6)
														 {
																TIM7->ARR=41;
						  					   			TIM6->ARR=41;
														 }
														 if(key_PRM_Count==7)
														 {
																TIM7->ARR=36;
											  		 		TIM6->ARR=36;
														 }
														 if(key_PRM_Count==8)
														 {
																TIM7->ARR=33;
								       			 		TIM6->ARR=33;
														 }
														 if(key_PRM_Count==9)
														 {
																TIM7->ARR=28;
								       			 		TIM6->ARR=28;
														 }
														 if(key_PRM_Count==10)
														 {
																TIM7->ARR=24;
								       			 		TIM6->ARR=24;
														 }
														 if(key_PRM_Count==11)
														 {
																TIM7->ARR=19;
								       			 		TIM6->ARR=19;
														 }
														 if(key_PRM_Count==12)
														 {
																TIM7->ARR=15;
								       			 		TIM6->ARR=15;
														 }	
														 if(key_PRM_Count==13)
														 {
																TIM7->ARR=11;
								       			 		TIM6->ARR=11;
														 }
														 if(key_PRM_Count==14)
														 {
																TIM7->ARR=7;
								       			 		TIM6->ARR=7;
														 }
														 if(key_PRM_Count==15)
														 {
																TIM7->ARR=4;
								       			 		TIM6->ARR=4;
														 }														 
								}
								if(Key_return & PRESS_KEY2)
								{
														 key_PRM_Count++;		 
														 if(key_PRM_Count==2)
															 {
															  TIM7->ARR=120;																 
											  				TIM6->ARR=120;
															 }
														 if(key_PRM_Count==3)
														 {
													    	TIM7->ARR=100;
									        			TIM6->ARR=100;
														 }
														 if(key_PRM_Count==4)
														 {
												  
															 	TIM7->ARR=80;
											      	 	TIM6->ARR=80;					 
														 }
														 if(key_PRM_Count==5)
														 {	
																TIM7->ARR=60;
											        	TIM6->ARR=60;
														 }
														 if(key_PRM_Count==6)
														 {
																TIM7->ARR=41;
											        	TIM6->ARR=41;
														 }
														 if(key_PRM_Count==7)
														 {
																TIM7->ARR=36;
									          	 	TIM6->ARR=36;
														 }
														 if(key_PRM_Count==8)
														 {
																TIM7->ARR=33;
									     				 	TIM6->ARR=33;
														 }
														 if(key_PRM_Count==9)
														 {
																TIM7->ARR=28;
									     				 	TIM6->ARR=28;
														 }
														 if(key_PRM_Count==10)
														 {
																TIM7->ARR=24;
									     				 	TIM6->ARR=24;
														 }
														 if(key_PRM_Count==11)
														 {
																TIM7->ARR=19;
									     				 	TIM6->ARR=19;
														 }
													   if(key_PRM_Count==12)
														 {
																TIM7->ARR=15;
									     				 	TIM6->ARR=15;
														 }
														 if(key_PRM_Count==13)
														 {
																TIM7->ARR=11;
									     				 	TIM6->ARR=11;
														 }
														 if(key_PRM_Count==14)
														 {
																TIM7->ARR=7;
									     				 	TIM6->ARR=7;
														 }
													   if(key_PRM_Count==15)
														 {
																TIM7->ARR=4;
									     				 	TIM6->ARR=4;
														 }
														 if(key_PRM_Count==16)
														 {
																	key_PRM_Count=15;
														 }
								}
				}
				 	if(Key_return & KEY_LONG)	
				{
								if(Key_return & PRESS_KEY1)  
								{
								      FAN_ZHUAN_FLAG=1;          //Õý×ª   Ä¬ÈÏÕý×ª
								}
								if(Key_return & PRESS_KEY2)  
								{
						  			 FAN_ZHUAN_FLAG=0;					 //·´×ª
								}
				}
	}
}
   
void TIM5_IRQHandler(void)      //ËÙ¶ÈÖúÁ¦ÖÐÖÃ &ÂÖì±µç»úÖúÁ¦
{ 
	if(TIM_GetITStatus(TIM5, TIM_IT_Update) == SET)       
	{
		 TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  
		       Tim5count++;
		   
					if(FAN_ZHUAN_FLAG)
					{
							if(Tim5count==100)
							 {
										GPIO_SetBits(SPEED2_Z_PORT,SPEED2_Z_PIN);
							 }
							else if(Tim5count==200)
							{					
								 GPIO_SetBits(SPEED1_Z_PORT,SPEED1_Z_PIN);
							}
							else if(Tim5count==300)
							{
									GPIO_ResetBits(SPEED2_Z_PORT,SPEED2_Z_PIN);
							}
							else if(Tim5count==400)
							{
									Tim5count=0;
								
								GPIO_ResetBits(SPEED1_Z_PORT,SPEED1_Z_PIN);
							}
				}
					if(!FAN_ZHUAN_FLAG)
					{
							  		if(Tim5count==100)
										 {
												GPIO_SetBits(SPEED1_Z_PORT,SPEED1_Z_PIN);
										 }
										else if(Tim5count==200)
										{
												GPIO_SetBits(SPEED2_Z_PORT,SPEED2_Z_PIN);
										}
										else if(Tim5count==300)
										{
												GPIO_ResetBits(SPEED1_Z_PORT,SPEED1_Z_PIN);
										}
										else if(Tim5count==400)
										{
												Tim5count=0;
											
											GPIO_ResetBits(SPEED2_Z_PORT,SPEED2_Z_PIN);
										}
					}
	}
}
void TIM6_IRQHandler(void)     //ÀÏ·¨¹æ²âËÙ
{ 
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
	{
		   TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  
		   Tim6count++;
		   if(Tim6count==5)
			 {
					GPIO_SetBits(OLDSPEED_PORT,OLDSPEED_PIN);
			 }
			 else if (Tim6count==250)
				{
					Tim6count=0;
					GPIO_ResetBits(OLDSPEED_PORT,OLDSPEED_PIN);
			 }
	}
}

void TIM7_IRQHandler(void)    //ÐÂ·¨¹æ²âËÙ
{ 
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) == SET)
	{
		 TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  
		 Tim7count++;
    if(Tim7count==5)                              //speed1 speed2 ÐÅºÅphase=90deg-------add by Ziyu Zhang 24/01/15
		{
				GPIO_ResetBits(NEWSPEED1_PORT,NEWSPEED1_PIN); 	   //speed1ÐÅºÅ
				GPIO_SetBits(NEWSPEED2_PORT,NEWSPEED2_PIN);	   //speed2ÐÅºÅ
		}	
		else if(Tim7count==10) 
		{
				GPIO_ResetBits(NEWSPEED1_PORT,NEWSPEED1_PIN); 	   //speed1ÐÅºÅ
				GPIO_ResetBits(NEWSPEED2_PORT,NEWSPEED2_PIN);	   //speed2ÐÅºÅ
		}	
		else if(Tim7count==15) 
		{
				GPIO_SetBits(NEWSPEED1_PORT,NEWSPEED1_PIN); 	   //speed1ÐÅºÅ
				GPIO_ResetBits(NEWSPEED2_PORT,NEWSPEED2_PIN);	   //speed2ÐÅºÅ
		}	
		else if(Tim7count==20) 
		{
				GPIO_SetBits(NEWSPEED1_PORT,NEWSPEED1_PIN); 	   //speed1ÐÅºÅ
				GPIO_SetBits(NEWSPEED2_PORT,NEWSPEED2_PIN);	   //speed2ÐÅºÅ
		}	
		else if(Tim7count==250)                       //ÖÜÆÚ¼ä¸ô250ms
		{
			  Tim7count=0;
		}
	}
	/*original code------SPD1 and SPD2 is in same phase
		{
		 TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  
		 Tim7count++;
    if(Tim7count==125)                              //speed1 speed2 ÐÅºÅÒ»ÖÂ
		{
				GPIO_SetBits(NEWSPEED1_PORT,NEWSPEED1_PIN); 	   //speed1ÐÅºÅ
				GPIO_SetBits(NEWSPEED2_PORT,NEWSPEED2_PIN);	   //speed2ÐÅºÅ
		}	
	  else if(Tim7count==250)                       //ÖÜÆÚ¼ä¸ô250ms
		{
			  Tim7count=0;
	  		GPIO_ResetBits(NEWSPEED1_PORT,NEWSPEED1_PIN);	 //speed1ÐÅºÅ
				GPIO_ResetBits(NEWSPEED2_PORT,NEWSPEED2_PIN);	 //speed2ÐÅºÅ
		}
	}
		*/
}

