#include "WRITE_PWM.h"

void TIM1_Initialization(void)
{

  GPIO_InitTypeDef GPIO_InitStructure;

	/*-------------------------- GPIO Configuration ----------------------------*/
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

  	/* Connect TIM1 pin to PA8 */
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);

  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;

  	/*-------------------------- TIM Configuration ----------------------------*/
  	TIM_DeInit(TIM1);
  	TIM_TimeBaseStruct.TIM_Period = 2000;            
  	TIM_TimeBaseStruct.TIM_Prescaler = 200-1;      
  	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;    // Counter Up
  	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStruct);

  TIM_OCInitTypeDef TIM_OCInitStruct;

  	/*-------------------------- Output Compare Configuration ----------------------------*/
  	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;               //PWM Edge mode
  	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_Pulse = 1000;
  	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;        // Output polarity High
  	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;      // Complementary output polarity :Not used
  	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;     // No output polarity : reset (low)
  	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;    // Complementary idle output : reset (not used)

  	TIM_OC1Init(TIM1, &TIM_OCInitStruct);
  	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

  	TIM_ARRPreloadConfig(TIM1, ENABLE);       // Put ARR value into register
  	TIM_Cmd(TIM1, ENABLE);                    // Enable Timer 1
  	TIM_CtrlPWMOutputs(TIM1, ENABLE);         // Enable output (To GPIO)
}