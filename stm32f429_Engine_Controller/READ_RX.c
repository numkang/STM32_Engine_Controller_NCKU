#include "READ_RX.h"

uint32_t rx_timebaseCapture_prev = 0;
uint32_t rx_timebaseCapture_current = 0;
uint32_t rx_timebaseCapture_output = 0;

void TIM3_Initialization(void)
{

  GPIO_InitTypeDef GPIO_InitStructure;

	/*-------------------------- GPIO Configuration ----------------------------*/
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

  	/* Connect TIM3 pin to PA6 */
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);

  NVIC_InitTypeDef NVIC_InitStructure;

  	/*-------------------------- NVIC Configuration ----------------------------*/
  	/* Enable the TIM3 global Interrupt */
  	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;

  	/*-------------------------- TIM Configuration ----------------------------*/
  	TIM_DeInit(TIM3);
  	TIM_TimeBaseStruct.TIM_Period = 20000;            
  	TIM_TimeBaseStruct.TIM_Prescaler = 90-1;      
  	TIM_TimeBaseStruct.TIM_ClockDivision = 0;
  	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;    // Counter Up
  	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStruct);

  TIM_ICInitTypeDef  TIM_ICInitStructure;

  	/*-------------------------- Input Capture Configuration ----------------------------*/
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; 				  //PB4 (TIMER3_CH1)
  	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;       //POLARITY!!!!
  	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;   
  	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;             //Prescaler
  	TIM_ICInitStructure.TIM_ICFilter = 0x0;
  	TIM_ICInit(TIM3, &TIM_ICInitStructure);

  	/* TIM enable counter */
  	TIM_Cmd(TIM3, ENABLE);
  	/* Enable the CC1 Interrupt Request */
  	TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE); //TIMER1_CH1
}

void TIM3_IRQHandler()
{
  if (TIM_GetITStatus(TIM3, TIM_IT_CC1) == SET) {
    /* Clear TIM2 Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);

    /* Get the Input Capture value */
    rx_timebaseCapture_prev = rx_timebaseCapture_current;
    rx_timebaseCapture_current = TIM_GetCapture1(TIM3);

    uint16_t tmpccer = 0;

    if(TIM3->CCER == (uint16_t)(TIM_ICPolarity_Rising | (uint16_t)TIM_CCER_CC1E))
    {
      /* Disable the Channel 1: Reset the CC1E Bit */
      TIM3->CCER &= (uint16_t)~TIM_CCER_CC1E;
      tmpccer = TIM3->CCER;

      /* Select the Polarity and set the CC1E Bit */
      tmpccer &= (uint16_t)~(TIM_CCER_CC1P | TIM_CCER_CC1NP);
      tmpccer |= (uint16_t)(TIM_ICPolarity_Falling | (uint16_t)TIM_CCER_CC1E);

      /* Write to TIMx CCMR1 and CCER registers */
      TIM3->CCER = tmpccer;
    }
    else if(TIM3->CCER == (uint16_t)(TIM_ICPolarity_Falling | (uint16_t)TIM_CCER_CC1E))
    {
      /* Disable the Channel 1: Reset the CC1E Bit */
      TIM3->CCER &= (uint16_t)~TIM_CCER_CC1E;
      tmpccer = TIM3->CCER;

      /* Select the Polarity and set the CC1E Bit */
      tmpccer &= (uint16_t)~(TIM_CCER_CC1P | TIM_CCER_CC1NP);
      tmpccer |= (uint16_t)(TIM_ICPolarity_Rising | (uint16_t)TIM_CCER_CC1E);

      /* Write to TIMx CCMR1 and CCER registers */
      TIM3->CCER = tmpccer;

      if(rx_timebaseCapture_current > rx_timebaseCapture_prev)
      {
        rx_timebaseCapture_output = (rx_timebaseCapture_current - rx_timebaseCapture_prev);
      }
      else
      {
        rx_timebaseCapture_output = (TIM3->ARR - rx_timebaseCapture_prev + rx_timebaseCapture_current);
      }
    }
  }
}

uint16_t return_RX(){
	return (uint16_t)rx_timebaseCapture_output;
}