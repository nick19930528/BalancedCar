/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
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

/*=====================================================================================================*/
/*=====================================================================================================*/
void GPIO_Config( void );
void PWM_Config( void );
/*=====================================================================================================*/
/*=====================================================================================================*/
int main( void )
{
	u32 i = PWM_MOTOR_MIN;

	SystemInit();
	GPIO_Config();
	PWM_Config();

	PWM1 = PWM_MOTOR_MIN;
	PWM2 = PWM_MOTOR_MIN;


  while(1) {
    LED_G = ~LED_G;

		while(KEY == 1) {
			PWM1 = i;
			PWM2 = i;
			
			Motor1 = 1;
			Motor2 = 1;
			i++;
			if(i>=PWM_MOTOR_MAX) {
				i = PWM_MOTOR_MIN;
				LED_G = ~LED_G;
			}
			Delay_100us(5);
		}

    Delay_100ms(1);
	}
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
	/* �]�w TIM2 TIM3 Time Base */
	TIM_TimeBaseStruct.TIM_Period = (2500-1);										// �g�� = 2.5ms, 400kHz
	TIM_TimeBaseStruct.TIM_Prescaler = (84-1);									// ���W84 = 1M ( 1us )
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;		// �W��
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);

	/* �]�w TIM2 TIM3 OC */
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;							// �t�m�� PWM1 �Ҧ�
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;	// �P�� OC
	TIM_OCInitStruct.TIM_Pulse = PWM_MOTOR_MAX;									// �]�m���ܭ�
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;			// ��p�ƭȤp�� PWM_MOTOR_MIN �ɬ����q��
	TIM_OC1Init(TIM2, &TIM_OCInitStruct);												// ��l�� TIM2 OC1
	TIM_OC2Init(TIM2, &TIM_OCInitStruct);												// ��l�� TIM2 OC2
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);						// �P�� TIM2 OC1 �w�˸�
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);						// �P�� TIM2 OC2 �w�˸�

	/* �Ұ� */
	TIM_ARRPreloadConfig(TIM2, ENABLE);													// �P�� TIM2 �����H�s��ARR
	TIM_Cmd(TIM2, ENABLE);																			// �P�� TIM2
}
/*=====================================================================================================*/
/*=====================================================================================================*/
