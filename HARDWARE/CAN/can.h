#ifndef __CAN_H
#define	__CAN_H

#include "stm32f10x.h" 

extern void CAN_INIT(void);
extern void can_tx(u8 Data1,u8 Data2);
extern u8 Can_Send_Msg(u8 len);
extern CanTxMsg TxMessage;
extern void CAN_GPIO_Config(void);
extern void	CAN_NVIC_Configuration(void);
extern u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);

extern u8 Can_Receive_Msg(u8 *buf);

extern unsigned char arrycount;
extern unsigned char	selfcount;
extern unsigned char RxBuf[8];

#endif /* __CAN_H */
