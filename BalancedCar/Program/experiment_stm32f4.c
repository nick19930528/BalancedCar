/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_usart.h"
#include "stm32f4_i2c.h"
#include "experiment_stm32f4.h"
#include "module_rs232.h"
#include "module_sensor.h"
#include "module_mpu6050.h"

/*=====================================================================================================*/
/*=====================================================================================================*/
#define KEY PBI(2)      // On:1  Off:0
#define LED_R PAO(11)   // On:0  Off:1
#define LED_G PAO(12)   // On:0  Off:1
#define Motor1 PAO(2)
#define Motor2 PAO(3)

#define PWM_MOTOR_MAX 2000  // 32~80%
#define PWM_MOTOR_MED 1400
#define PWM_MOTOR_MIN 0

#define PWM1 TIM2->CCR1
#define PWM2 TIM2->CCR2

#define MAX_STRLEN 1 // this is the maximum string length of our string in characters
u8 received_string[MAX_STRLEN]; // this will hold the recieved string

/*=====================================================================================================*/
/*=====================================================================================================*/
void GPIO_Config( void );
void PWM_Config( void );
/*=====================================================================================================*/
/*=====================================================================================================*/
int main( void )
{
	
	//uart
//  u8 i = 0;
//  u8 TrData[8] = {0};
	//mpu6050
//  u8 USART_BUF[24] = {0};
	
	SystemInit();
  GPIO_Config();
	PWM_Config();
	RS232_Uart1_Config();
	RS232_Uart3_Config();
// 	I2C_Config();

  //MPU6050_Init();
	PWM1 = PWM_MOTOR_MED;
	PWM2 = PWM_MOTOR_MED;
	
	while(1) {
		if(received_string[0] == 'a') {
			Motor1 = 1;
			Motor2 = 1;
			LED_R = 0;
		}
		if(received_string[0] == 'b') {
			Motor1 = 1;
			Motor2 = 0;
			LED_R = 1;
		}
		if(received_string[0] == 'c') {
			Motor1 = 0;
			Motor2 = 1;
			LED_G = 0;
		}
		if(received_string[0] == 'd') {
			Motor1 = 0;
			Motor2 = 0;
			LED_G = 1;
		}
	}
		

// 	if(SysTick_Config(420000)) {		// 168MHz / 420000 = 400Hz = 2.5ms
//		while(1);
//	}
//while(1);
//		//***test uart3 sent***
//  while(1) {
//    LED_G = ~LED_G;

//    i++;
//    if(i==255)  i = 0;

//    RS232_SendStr(USART3, (u8*)"i = ");
//    RS232_SendNum(USART3, Type_D, 3, i);
//    RS232_SendStr(USART3, (u8*)"\r\n");

//    Delay_100ms(1);
//  }
	
	//***test uart1 sent***
//  while(1) {
//    LED_G = ~LED_G;

//    i++;
//    if(i==255)  i = 0;

//    RS232_SendStr(USART1, (u8*)"i = ");
//    RS232_SendNum(USART1, Type_D, 3, i);
//    RS232_SendStr(USART1, (u8*)"\r\n");

//    Delay_100ms(3);
//  }

////***test mpu6050***
//	while(1) {
//		LED_G = ~LED_G;

//    USART_BUF[0]  = (u8)(Acc.X);
//    USART_BUF[1]  = (u8)(Acc.X >> 8);
//    USART_BUF[2]  = (u8)(Acc.Y);
//    USART_BUF[3]  = (u8)(Acc.Y >> 8);
//    USART_BUF[4]  = (u8)(Acc.Z);
//    USART_BUF[5]  = (u8)(Acc.Z >> 8);
//    USART_BUF[6]  = (u8)(Gyr.X);
//    USART_BUF[7]  = (u8)(Gyr.X >> 8);
//    USART_BUF[8]  = (u8)(Gyr.Y);
//    USART_BUF[9]  = (u8)(Gyr.Y >> 8);
//    USART_BUF[10] = (u8)(Gyr.Z);
//    USART_BUF[11] = (u8)(Gyr.Z >> 8);

//    RS232_VisualScope(USART1, USART_BUF, 8);
//	}
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void GPIO_Config( void )
{
	GPIO_InitTypeDef GPIO_InitStruct;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);

  /* LED */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
	
  /* directional control */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
	
  /* KEY */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  LED_G = 1;
  LED_R = 1;
	Motor1 = 0;
	Motor2 = 0;
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void USART3_IRQHandler(void){
	// check if the USART1 receive interrupt flag was set
	if( USART_GetITStatus(USART3, USART_IT_RXNE) ){
		char t = USART3->DR; 
		received_string[0] = t;
		RS232_SendStr(USART1, received_string);
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void PWM_Config( void )
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2  ,  ENABLE);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);


	/* TIM2 PWM1 PA0 */	/* TIM2 PWM2 PA1 */	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 ;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	TIM_DeInit(TIM2);


/************************** PWM Output **************************************/
	/* 設定 TIM2 TIM3 Time Base */
	TIM_TimeBaseStruct.TIM_Period = (2500-1);										// 週期 = 2.5ms, 400kHz
	TIM_TimeBaseStruct.TIM_Prescaler = (84-1);									// 除頻84 = 1M ( 1us )
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;		// 上數
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);

	/* 設定 TIM2 TIM3 OC */
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;							// 配置為 PWM1 模式
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;	// 致能 OC
	TIM_OCInitStruct.TIM_Pulse = PWM_MOTOR_MAX;									// 設置跳變值
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;			// 當計數值小於 PWM_MOTOR_MIN 時為高電平
	TIM_OC1Init(TIM2, &TIM_OCInitStruct);												// 初始化 TIM2 OC1
	TIM_OC2Init(TIM2, &TIM_OCInitStruct);												// 初始化 TIM2 OC2
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);						// 致能 TIM2 OC1 預裝載
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);						// 致能 TIM2 OC2 預裝載

	/* 啟動 */
	TIM_ARRPreloadConfig(TIM2, ENABLE);													// 致能 TIM2 重載寄存器ARR
	TIM_Cmd(TIM2, ENABLE);																			// 致能 TIM2
}
/*=====================================================================================================*/
/*=====================================================================================================*/
