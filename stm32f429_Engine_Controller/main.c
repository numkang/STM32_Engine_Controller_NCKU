#include "main.h"

char text_main[100];

static inline void Delay_1us(uint32_t nCnt_1us)
{
  volatile uint32_t nCnt;

  for (; nCnt_1us != 0; nCnt_1us--)
    for (nCnt = 13; nCnt != 0; nCnt--);
}

void RCC_Configuration(void)
{
      /* --------------------------- System Clocks Configuration -----------------*/
      /* GPIOx clock enable */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //TIM1 & TIM2 & TIM3 & USART1 GPIO
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);  //LED GPIO
      /* TIMx clock enable */
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);   //TIM1 write PWM
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   //TIM2 read pulse
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   //TIM3 read RX
      /* USARTx clock enable */
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //USART1
}

/**************************************************************************************/
int main(void)
{
    RCC_Configuration();
    LED_Initialization();    //PG13 (YELLOW) & PG14 (RED)
    TIM1_Initialization();   //PA8 (TIM1_CH1)
    TIM2_Initialization();   //PA5 (TIM2_CH1)
    TIM3_Initialization();   //PA6 (TIM3_CH1)
    USART1_Initialization(); //PA9 (TX) & PA10 (RX)
    
    USART1_puts("\r\nHello World\r\n");

    while(1)
    {
        TIM1->CCR1 = 1500;

        LED3_On();

        sprintf(text_main,"Pulse: %.4f RX: %d\r\n", return_pulse(), return_RX());
        USART1_puts(text_main);
        Delay_1us(5000);
    }

    while(1); // Don't want to exit
}

