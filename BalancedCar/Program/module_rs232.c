/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_usart.h"
#include "module_rs232.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
void RS232_Uart1_Config( void )
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;


  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,  ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,  GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

 	/* USART1 Tx PA9 */	/* USART1 Rx PA10 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStruct);
	USART_Cmd(USART1, ENABLE);
	
}
/*=====================================================================================================*/
/*=====================================================================================================*
**?? : RS232_Config
**?? : 
**?? : 
**?? : 
**?? : 
**=====================================================================================================*/
/*=====================================================================================================*/
void RS232_Uart3_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,  ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);

  /* USART1 Tx PB10 */	/* USART1 Rx PB11 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  USART_InitStruct.USART_BaudRate = 9600;
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  USART_InitStruct.USART_StopBits = USART_StopBits_1;
  USART_InitStruct.USART_Parity = USART_Parity_No;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART3, &USART_InitStruct);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // enable the USART1 receive interrupt 

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;		 // we want to configure the USART1 interrupts
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// this sets the priority group of the USART1 interrupts
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 // this sets the subpriority inside the group
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 // the USART1 interrupts are globally enabled
	NVIC_Init(&NVIC_InitStructure);							 // the properties are passed to the NVIC_Init function which takes care of the low level stuff	

  USART_Cmd(USART3, ENABLE);

}
/*=====================================================================================================*/
/*=====================================================================================================*
**?? : RS232_SendStr
**?? : ????
**?? : USARTx, *pWord
**?? : None
**?? : RS232_SendStr(USART1, (u8*)"Hellow World!");
**=====================================================================================================*/
/*=====================================================================================================*/
void RS232_SendStr( USART_TypeDef* USARTx, uc8 *pWord )
{
  while(*pWord != '\0') {
    USART_SendByte(USARTx, (u8*)pWord);
    pWord++;
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*
**?? : RS232_SendNum
**?? : ????????
**?? : USARTx, Type, NumLen, SendData
**?? : None
**?? : RS232_SendNum(USART1, Type_O, 6, 1024);
**=====================================================================================================*/
/*=====================================================================================================*/
void RS232_SendNum( USART_TypeDef* USARTx, u8 Type, u8 NumLen, s32 SendData )
{
  u8 TrData[32] = {0};
  u8 *pWord = TrData;

  NumToChar(Type, NumLen, TrData, SendData);
  
  while(*pWord != '\0') {
    USART_SendByte(USARTx, (u8*)pWord);
    pWord++;
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*
**?? : RS232_SendData
**?? : ????
**?? : USARTx, *SendData, DataLen
**?? : None
**?? : RS232_SendData(USART1, SendData, DataLen);
**=====================================================================================================*/
/*=====================================================================================================*/
void RS232_SendData( USART_TypeDef* USARTx, uc8 *SendData, u16 DataLen )
{
  do {
    USART_SendByte(USARTx, SendData);
    SendData++;
    DataLen--;
  } while(DataLen);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**?? : RS232_RecvStr
**?? : ????
**?? : USARTx, *pWord
**?? : None
**?? : RS232_RecvStr(USART1, Stirng);
**=====================================================================================================*/
/*=====================================================================================================*/
void RS232_RecvStr( USART_TypeDef* USARTx, u8 *pWord )
{
  do {
    *pWord = USART_RecvByte(USARTx);
    pWord++;
  } while(*(pWord-1) != '\0');
  *pWord = '\0';
}
/*=====================================================================================================*/
/*=====================================================================================================*
**?? : RS232_RecvData
**?? : ????
**?? : USARTx, *RecvData, DataLen
**?? : None
**?? : RS232_RecvData(USART1, RecvData, DataLen);
**=====================================================================================================*/
/*=====================================================================================================*/
void RS232_RecvData( USART_TypeDef* USARTx, u8 *RecvData, u16 DataLen )
{
  do {
    *RecvData = USART_RecvByte(USARTx);
    RecvData++;
    DataLen--;
  } while(DataLen);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : RS232_VisualScope_CRC16
**功能 : 
**輸入 : 
**輸出 : 
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
static u16 RS232_VisualScope_CRC16( u8 *Array, u16 Len )
{
	u16 USART_IX, USART_IY, USART_CRC;

	USART_CRC = 0xffff;
	for(USART_IX=0; USART_IX<Len; USART_IX++) {
		USART_CRC = USART_CRC^(u16)(Array[USART_IX]);
		for(USART_IY=0; USART_IY<=7; USART_IY++) {
			if((USART_CRC&1)!=0)
				USART_CRC = (USART_CRC>>1)^0xA001;
			else
				USART_CRC = USART_CRC>>1;
		}
	}
	return(USART_CRC);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : USART_VisualScope
**功能 : 
**輸入 : 
**輸出 : 
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
void RS232_VisualScope( USART_TypeDef* USARTx, u8 *pWord, u16 Len )
{
	u8 i = 0;
	u16 Temp = 0;

	Temp = RS232_VisualScope_CRC16(pWord, Len);
	pWord[8] = Temp&0x00ff;
	pWord[9] = (Temp&0xff00)>>8;

	for(i=0; i<10; i++) {
		USART_SendData(USARTx, *pWord);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
		pWord++;
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*/
