#include "READ_PULSE.h"

uint32_t pulse_timebaseCapture_prev = 0;
uint32_t pulse_timebaseCapture_current = 0;
uint32_t pulse_timebaseCapture_output = 0;

void TIM2_Initialization(void)
{

  GPIO_InitTypeDef GPIO_InitStructure;

	/*-------------------------- GPIO Configuration ----------------------------*/
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

  	/* Connect TIM pin to AF5 */
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_TIM2);

  NVIC_InitTypeDef NVIC_InitStructure;

  	/*-------------------------- NVIC Configuration ----------------------------*/
  	/* Enable the TIM2 global Interrupt */
  	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;

  	/*-------------------------- TIM Configuration ----------------------------*/
  	TIM_DeInit(TIM2);
  	TIM_TimeBaseStruct.TIM_Period = 60000;            
  	TIM_TimeBaseStruct.TIM_Prescaler = 75-1;      
  	TIM_TimeBaseStruct.TIM_ClockDivision = 0;
  	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;    // Counter Up
  	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);

  TIM_ICInitTypeDef  TIM_ICInitStructure;

  	/*-------------------------- Input Capture Configuration ----------------------------*/
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; 				  //PA5 (TIMER2_CH1)
  	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;       //POLARITY!!!!
  	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;   
  	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;             //Prescaler
  	TIM_ICInitStructure.TIM_ICFilter = 0x0;
  	TIM_ICInit(TIM2, &TIM_ICInitStructure);

  	/* TIM enable counter */
  	TIM_Cmd(TIM2, ENABLE);
  	/* Enable the CC1 Interrupt Request */
  	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE); //TIMER2_CH1
}

void TIM2_IRQHandler()
{
  if (TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET) {
    /* Clear TIM2 Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

    /* Get the Input Capture value */
    pulse_timebaseCapture_prev = pulse_timebaseCapture_current;
    pulse_timebaseCapture_current = TIM_GetCapture1(TIM2);

	  if(pulse_timebaseCapture_current > pulse_timebaseCapture_prev)
	  {
      pulse_timebaseCapture_output = (pulse_timebaseCapture_current - pulse_timebaseCapture_prev);
    }
    else
    {
    	pulse_timebaseCapture_output  = (TIM2->ARR - pulse_timebaseCapture_prev + pulse_timebaseCapture_current);
    }
  }
}

float return_pulse(){
	return 90000000.0/(float)TIM2->CR1/(float)(TIM2->PSC+1)/(float)pulse_timebaseCapture_output;
}