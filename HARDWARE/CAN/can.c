#include "can.h"
#include "stdio.h"
#include "main.h"
#include "stdlib.h"
#include "pwm_cap.h"
#include "key.h"

unsigned char arrycount;
unsigned char	selfcount;
unsigned char RxBuf[8];

typedef struct _tagCAN_TX_MESSAGE_MAP
{
  uint32_t   StdId;
  union
  {
    uint32_t   ExtId;
    struct
    {
      uint32_t nFn:8;
      uint32_t nAfn:8;
      uint32_t nCmd:3;
      uint32_t nDestId:5;
      uint32_t nSrcId:5;
      uint32_t nHeard:3;
    } sDet;
  } uExtId;
  uint8_t    IDE;
  uint8_t    RTR;
  uint8_t    DLC;
  uint8_t    Data[8];
} CAN_TX_MESSAGE_MAP,*pCAN_TX_MESSAGE_MAP;

typedef struct _tagCAN_RX_FRAME_MAP
{
  uint32_t   StdId;
  union
  {
    uint32_t   ExtId;
    struct
    {
      uint32_t nFn:8;
      uint32_t nAfn:8;
      uint32_t nCmd:3;
      uint32_t nDestId:5;
      uint32_t nSrcId:5;
      uint32_t nHeard:3;
    } sDet;
  } uExtId;
  uint8_t    IDE;
  uint8_t    RTR;
  uint8_t    DLC;
  uint8_t    Data[8];
  uint8_t FMI;
} CAN_RX_FRAME_MAP,*pCAN_RX_FRAME_MAP;

CAN_RX_FRAME_MAP rxd;    //接受数据

u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef        CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;

    NVIC_InitTypeDef  NVIC_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //??PORTA??
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); //??CAN1??
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//????
    GPIO_Init(GPIOA, &GPIO_InitStructure);		//???IO
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //????
    GPIO_Init(GPIOA, &GPIO_InitStructure); //???IO
	
    CAN_InitStructure.CAN_TTCM=DISABLE;						 //?????????  //
    CAN_InitStructure.CAN_ABOM=ENABLE;						 //????????	 //
    CAN_InitStructure.CAN_AWUM=DISABLE;						 //??????????(??CAN->MCR?SLEEP?)//
    CAN_InitStructure.CAN_NART=DISABLE;						 	//???????? //
    CAN_InitStructure.CAN_RFLM=DISABLE;						 //?????,?????? //
    CAN_InitStructure.CAN_TXFP=DISABLE;						 //??????????? //
    CAN_InitStructure.CAN_Mode= mode;	         //????: mode:0,????;1,????; //
    //?????
    CAN_InitStructure.CAN_SJW=tsjw;				//????????(Tsjw)?tsjw+1?????  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
    CAN_InitStructure.CAN_BS1=tbs1;       //Tbs1=tbs1+1?????CAN_BS1_1tq ~CAN_BS1_16tq
    CAN_InitStructure.CAN_BS2=tbs2;       //Tbs2=tbs2+1?????CAN_BS2_1tq ~	CAN_BS2_8tq
    CAN_InitStructure.CAN_Prescaler=brp;            //????(Fdiv)?brp+1	//
    CAN_Init(CAN1, &CAN_InitStructure);            // ???CAN1

    CAN_FilterInitStructure.CAN_FilterNumber=0;	   //???0   //   ??14????
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; //????????
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;  //1?32bit????
		
		//CAN_FilterId和CAN_FilterMaskId是配合使用，CAN_FilterId指出需要屏蔽什么ID内容，什么格式。
	 //CAN_FilterMaskId是指CAN_FilterId每一位是否需要过滤，若CAN_FilterMaskId在某位是1的话，ID对应位上的数值就必须和CAN_FilterId该位上的一样，保持一样
		
		CAN_FilterInitStructure.CAN_FilterIdHigh=0X00;
		CAN_FilterInitStructure.CAN_FilterIdLow=(((u32)0x01F8<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF;	
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xFFFF;
		CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xFFFF; 
		
//    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
//    CAN_FilterInitStructure.CAN_FilterIdLow =0x0000;
//    CAN_FilterInitStructure.CAN_FilterMaskIdHigh  = 0x0000;
//    CAN_FilterInitStructure.CAN_FilterMaskIdLow   = 0x0000;

    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//???0???FIFO0
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //?????0
    CAN_FilterInit(&CAN_FilterInitStructure);//??????
    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);  //FIFO0????????.

    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;     // ?????1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // ?????0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		
    return 0;
}

u8 Can_Send_Msg(u8 len)
{        
	 unsigned int i=0;
	 unsigned char temp_h=0;
	 unsigned char temp_l=0;
   u8 mbox;  
	 CAN_TX_MESSAGE_MAP *txd;
   txd=malloc(sizeof(CAN_TX_MESSAGE_MAP));
   txd->uExtId.sDet.nSrcId=0x01;
	 txd->uExtId.sDet.nDestId=0x1f;
	 txd->uExtId.sDet.nCmd=0x00;
	 txd->uExtId.sDet.nAfn=0x31;
	 txd->uExtId.sDet.nFn=0x00;
	 txd->DLC=len;
	 txd->RTR=CAN_RTR_DATA;
	 txd->IDE=CAN_ID_EXT;
	
	 temp_h=SIMU_ARRY[arrycount];
   temp_l=SIMU_ARRY[arrycount]>>8;
	
   txd->Data[0]=temp_h;       //数据高字节
   txd->Data[1]=temp_l;       //数据低字节
   txd->Data[2]=31;              //rpm
	 txd->Data[3]=selfcount;    //累加 ----0至255 	 
	 	 
	if(!fbits.SUBFLAG.SUBFLAG_1.NEW_MOMENT_FLAG)        //一直发送0x02ee （750mv）
	{
		 	arrycount=0;   //750mv
	}
	else                                                        //循环发送750mv-3200mv
	{                                       
			arrycount++; 
	}
	selfcount++;
	if(arrycount==61)arrycount=0;
	if(selfcount==255) selfcount=0;
  mbox= CAN_Transmit(CAN1, (CanTxMsg*)txd);
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;   //??????
	free(txd);
	txd=NULL;
  if(i>=0XFFF) return 1;
  return 0; 
}

///* USB中`断和CAN接收中断服务程序，USB跟CAN公用I/O，这里只用到CAN的中断。 */
void USB_LP_CAN1_RX0_IRQHandler(void)
{	
	 unsigned char i;
  CanRxMsg RxMessage;
  CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);
	for(i=0;i<8;i++)
	{
	   RxBuf[i]=RxMessage.Data[i];  
	}
} 

u8 Can_Receive_Msg(u8 *buf)
{                  
    unsigned char i;
    CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;      //???????,???? 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);       //???? 
    for(i=0;i<8;i++)
    buf[i]=RxMessage.Data[i];  
    return RxMessage.DLC;   
}     

//void USB_LP_CAN1_RX0_IRQHandler(void)
//{
//    CanRxMsg RxMessage;
//    int i=0;
//    CAN_Receive(CAN1, 0, &RxMessage);
//    for(i=0;i<8;i++)
//    printf("rxbuf[%d]:%d\r\n",i,RxMessage.Data[i]);
//}

//u8 Can_Send_Msg(u8* msg,u8 len)
//{   
//  u8 mbox;
//  u16 i=0;
//  CanTxMsg TxMessage;
//  TxMessage.StdId=0x12;                  // ????? 
//  TxMessage.ExtId=0x12;                // ??????? 
//  TxMessage.IDE=CAN_Id_Standard; // ???
//  TxMessage.RTR=CAN_RTR_Data;        // ???
//  TxMessage.DLC=len;                        // ????????
//  for(i=0;i<len;i++)
//  TxMessage.Data[i]=msg[i];                   
//  mbox= CAN_Transmit(CAN1, &TxMessage);   
//  i=0;
//  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;   //??????
//  if(i>=0XFFF)return 1;
//  return 0;     
//}

