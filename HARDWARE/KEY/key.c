#include "key.h"
#include "delay.h"
#include "pwm.h"

FBITS  fbits;
unsigned char key_sta;
unsigned char test_key=0;
uint8_t key_value;
//按键初始化函数 
//PA15和PC5 设置成输入
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD,ENABLE);//使能PORTA,PORTC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;                 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   //自锁按键下拉
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   //自锁按键下拉
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   //自锁按键下拉
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //自锁按键下拉
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //自锁按键下拉
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //自锁按键下拉
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = KEY_DEC_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //点动按键上拉
 	GPIO_Init(KEY_DEC_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = KEY_ADD_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //点动按键上拉
 	GPIO_Init(KEY_ADD_PORT, &GPIO_InitStructure);
} 

uint8_t Get_Key(void)       //按键检测
{
        uint8_t ReadKey;
        if(!GPIO_ReadInputDataBit(KEY_DEC_PORT,KEY_DEC_PIN))    //  减按键
        {
                        ReadKey |= PRESS_KEY1;       //赋值0x01
        } 				
        if(!GPIO_ReadInputDataBit(KEY_ADD_PORT,KEY_ADD_PIN))     //加按键
        {
                        ReadKey |= PRESS_KEY2;   //赋值0x02
        }
         return ReadKey;
}

uint8_t ReadKeyValue(void)   //按键值返回
{
     // static uint8_t Key_Check;
        static uint8_t Key_State;  //按键状态
        static uint16_t Key_LongCheck;
        static uint8_t Key_Prev = 0; //上一次按键值    
        uint8_t Key_press=0;        //当前按键按下
        uint8_t Key_return = 0;  //按键返回
        Key_press = Get_Key();     
		
                switch (Key_State)
                {
                        case 0:
                                if(Key_press != 0)
                                {
                                        Key_Prev = Key_press;																      
                                        Key_State = 1;														    									
                                }
                                break;
                        case 1:
                                if(Key_press == Key_Prev)           
                                {
                                        Key_State = 2;
                                        Key_return= Key_Prev | KEY_DOWN; //按下按键
                                }
                               else                                         
                                {
                                        Key_State = 0;
                                }
                                break;
                        case 2:
                                if(Key_press != Key_Prev)
                                {                                        
                                        Key_State = 0;
                                        Key_LongCheck = 0;
                                        Key_return = Key_Prev | KEY_UP;  //松开按键
                                        return Key_return;                                
                                }
                                if(Key_press == Key_Prev) 
                                {
                                        Key_LongCheck++;
                                        if(Key_LongCheck >= 100)    //长按按键
                                        {                        
                                                Key_LongCheck = 0;
                                                Key_State = 3;
                                                Key_return= Key_press | KEY_LONG;  
                                                return Key_return;        
                                        }                                
                                }
                                break;
                                case 3:
                                if(Key_press != Key_Prev)
                                {
                                        Key_State = 0;
                                }
                                break;        
                }
        return  NO_KEY;
}

void check_hit(void)               //检测自锁按键状态
{
			 //自锁按键逻辑
		if(NEW_CHACK_SPEED)    //检测是否按下
		{
				fbits.SUBFLAG.SUBFLAG_1.NEW_SPEED_FLAG=ture;
		}
		else
		{
			fbits.SUBFLAG.SUBFLAG_1.NEW_SPEED_FLAG=flase;
		}
		if(OLD_CHACK_SPEED)   
		{	
			fbits.SUBFLAG.SUBFLAG_1.OLD_SPEED_FLAG=ture;	   
		}else
		{
				fbits.SUBFLAG.SUBFLAG_1.OLD_SPEED_FLAG=flase;	
		}
		if(ZHONGZHI_ZHULI)  //no
		{
					fbits.SUBFLAG.SUBFLAG_1.ZHONGZHI_ZHULI_FLAG=ture;
		}
		else
		{
					fbits.SUBFLAG.SUBFLAG_1.ZHONGZHI_ZHULI_FLAG=flase;
		}
		if(LUGU_ZHULI)    //no
		{
				fbits.SUBFLAG.SUBFLAG_1.LUGU_ZHULI_FLAG=ture;
		}
		else
		{
				fbits.SUBFLAG.SUBFLAG_1.LUGU_ZHULI_FLAG=flase;
		}
		if(OLD_MOMENT)
		{
		    fbits.SUBFLAG.SUBFLAG_1.OLD_MOMENT_FLAG=ture;
		}else
		{
				fbits.SUBFLAG.SUBFLAG_1.OLD_MOMENT_FLAG=flase;
		}
		if(NEW_MOMENT)    //no
		{
				fbits.SUBFLAG.SUBFLAG_1.NEW_MOMENT_FLAG=ture;
		}
		else
		{
				fbits.SUBFLAG.SUBFLAG_1.NEW_MOMENT_FLAG=flase;
		}
}

void hit_process(void)
{
		if(fbits.SUBFLAG.SUBFLAG_1.NEW_SPEED_FLAG)      //新法规测速标志位
		{
				TIM_Cmd(TIM7,ENABLE); 	//使能定时器1	  
		}
		else
		{
				TIM_Cmd(TIM7,DISABLE); 	//使能定时器1
		}
		if(fbits.SUBFLAG.SUBFLAG_1.OLD_SPEED_FLAG)    //老法规测速标志位
		{
			  TIM_Cmd(TIM6,ENABLE); 	//使能定时器2
		}
		else
		{
				TIM_Cmd(TIM6,DISABLE); 	//使能定时器2		
		}
		if(	fbits.SUBFLAG.SUBFLAG_1.ZHONGZHI_ZHULI_FLAG)    //中置电机助力
		{
			    TIM_Cmd(TIM5,ENABLE); 	//使能定时器3
		}
		else
		{
				TIM_Cmd(TIM5,DISABLE); 	//使能定时器3
		}
		
		if(fbits.SUBFLAG.SUBFLAG_1.LUGU_ZHULI_FLAG)          //轮毂电机助力
		{
			TIM_Cmd(TIM5,ENABLE); 	//使能定时器5
		}
		else
		{
			TIM_Cmd(TIM5,DISABLE);   	//使能定时器5
		}
		if(fbits.SUBFLAG.SUBFLAG_1.OLD_MOMENT_FLAG)    //老法规力矩
		{
		    //	TIM_Cmd(TIM3,ENABLE);     	//使能定时器3
		}
		else
		{
	   	//	 TIM_Cmd(TIM3,DISABLE); 	//使能定时器3
		}
		if(	fbits.SUBFLAG.SUBFLAG_1.NEW_MOMENT_FLAG)   //新法规力矩
		{
			TIM_Cmd(TIM2,ENABLE); 	//使能定时器6
		}
		else
		{
			 TIM_Cmd(TIM2,DISABLE); 	//使能定时器6
		}
}
