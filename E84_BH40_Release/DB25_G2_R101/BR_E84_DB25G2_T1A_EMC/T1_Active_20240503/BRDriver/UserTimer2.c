#include "M0518.h"
volatile uint16_t t1us;
void Timer2_Init(void) //us
{
	//CLK->CLKSEL1 |= CLK_CLKSEL1_TMR0_S_Msk;
	CLK->CLKSEL1 &= ~CLK_CLKSEL1_TMR2_S_Msk;
	CLK->CLKSEL1 |= 2ul<<16;

	CLK->APBCLK |= CLK_APBCLK_TMR2_EN_Msk;
	
	TIMER2->TCSR |= (1UL << TIMER_TCSR_MODE_Pos);
	//TIMER0->TCSR =0x00000000;
	TIMER2->TCSR &= ~TIMER_TCSR_PRESCALE_Msk;
	TIMER2->TCSR |= 5;//SRC: HCLK = PLLFOUT/2 >=8MHz
	TIMER2->TCMPR = 80;//10us;
	TIMER2->TCSR |= TIMER_TCSR_IE_Msk;
	TIMER2->TISR = TIMER_TISR_TIF_Msk;
	NVIC_EnableIRQ(TMR2_IRQn);
	TIMER2->TCSR |= TIMER_TCSR_CRST_Msk;
	TIMER2->TCSR |= TIMER_TCSR_CEN_Msk;
	TIMER2->TCSR |= TIMER_TCSR_TDR_EN_Msk;
}

void TMR2_IRQHandler(void)
{  
//printf("TMR0_IRQHandler \n");
   	TIMER2->TISR = TIMER_TISR_TIF_Msk;
   	if(t1us > 0) t1us--;	
}

void Delay_10us(uint16_t t)
{
	t1us = t;
	while(t1us);
}


