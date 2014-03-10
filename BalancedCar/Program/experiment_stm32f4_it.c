/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_i2c.h"
#include "module_sensor.h"
#include "module_mpu6050.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
void SysTick_Handler( void )
{
 u8 IMU_Buf[16] = {0};

	/* 400Hz ���� */
  I2C_DMA_ReadReg(IMU_Buf, MPU6050_I2C_ADDR, 0x3B, 14);

	/* ��ƦX�� */
	Acc.X = (s16)((IMU_Buf[0]  << 8) | IMU_Buf[1]);
	Acc.Y = (s16)((IMU_Buf[2]  << 8) | IMU_Buf[3]);
	Acc.Z = (s16)((IMU_Buf[4]  << 8) | IMU_Buf[5]);
	Gyr.X = (s16)((IMU_Buf[8]  << 8) | IMU_Buf[9]);
	Gyr.Y = (s16)((IMU_Buf[10] << 8) | IMU_Buf[11]);
	Gyr.Z = (s16)((IMU_Buf[12] << 8) | IMU_Buf[13]);
// 	Meg.X = (s16)((IMU_Buf[12] << 8) | IMU_Buf[13]);
// 	Meg.Y = (s16)((IMU_Buf[14] << 8) | IMU_Buf[15]);
// 	Meg.Z = (s16)((IMU_Buf[16] << 8) | IMU_Buf[17]);
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void DMA1_Stream0_IRQHandler( void )
{
	I2C1_Recv_DMA_IRQ();
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void DMA1_Stream6_IRQHandler( void )
{
	I2C1_Send_DMA_IRQ();
}
/*=====================================================================================================*/
/*=====================================================================================================*/

//void USART1_IRQHandler(void){

//	// check if the USART1 receive interrupt flag was set
////	if( USART_GetITStatus(USART1, USART_IT_RXNE) ){
//		LED_G = ~LED_G;
//		Delay_100ms(2);
//	USART_ClearFlag(USART1,USART_FLAG_RXNE);
////	}

void HardFault_Handler( void )
{
  while(1);
}
void MemManage_Handler( void )
{
  while(1);
}
void BusFault_Handler( void )
{
  while(1);
}
void UsageFault_Handler( void )
{
  while(1);
}
void SVC_Handler( void )
{
  while(1);
}
void DebugMon_Handler( void )
{
  while(1);
}
void PendSV_Handler( void )
{
  while(1);
}
void NMI_Handler( void )
{
  while(1);
}
/*=====================================================================================================*/
/*=====================================================================================================*/