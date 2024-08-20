#include "M0518.h"
volatile uint16_t t1ms, t1ms_1,t1ms_2,t1ms_3, tlms_4,tlms_5;
void Timer1_Init(void) //ms
{
	CLK->CLKSEL1 |= CLK_CLKSEL1_TMR1_S_Msk;
	CLK->APBCLK |= CLK_APBCLK_TMR1_EN_Msk;
	
	TIMER1->TCSR |= (1UL << TIMER_TCSR_MODE_Pos);
	TIMER1->TCSR &= ~TIMER_TCSR_PRESCALE_Msk;
	TIMER1->TCSR |= 11;//Prescale = 11
	TIMER1->TCMPR = 2000;//2304;
	TIMER1->TCSR |= TIMER_TCSR_IE_Msk;
	TIMER1->TISR = TIMER_TISR_TIF_Msk;
	//Enable Timer0 Interrupt
	NVIC_EnableIRQ(TMR1_IRQn);
	TIMER1->TCSR |= TIMER_TCSR_CRST_Msk;
	TIMER1->TCSR |= TIMER_TCSR_CEN_Msk;
	TIMER1->TCSR |= TIMER_TCSR_TDR_EN_Msk; 
}

void TMR1_IRQHandler(void)
{
//printf("TMR1_IRQHandler \n");
   	TIMER1->TISR = TIMER_TISR_TIF_Msk;
	if(t1ms > 0) t1ms--;
	if(t1ms_1 > 0) t1ms_1--;
	if(t1ms_2 > 0) t1ms_2--;
	if(t1ms_3 > 0) t1ms_3--;
	if(tlms_4 > 0) tlms_4--;
	if(tlms_5 > 0) tlms_5--;
}

void Delay_1ms(uint16_t t)
{
	t1ms = t;
	while(t1ms);
}

