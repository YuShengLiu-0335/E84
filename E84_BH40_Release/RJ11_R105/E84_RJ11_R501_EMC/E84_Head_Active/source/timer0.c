
#include "M0518.h"
#include "def_const.h"
#include "def_flag.h"
#include "def_exram.h"
#include "Motor.h"
#include <stdio.h>
#include <math.h>

volatile uint16_t Delay1Ms = 0; 
volatile uint16_t t1us;
volatile uint16_t t1ms;

void Timer0_Init(void) //us
{
#if 0
	//CLK->CLKSEL1 |= CLK_CLKSEL1_TMR0_S_Msk;
	CLK->CLKSEL1 &= ~CLK_CLKSEL1_TMR0_S_Msk;
	CLK->CLKSEL1 |= 2ul<<8;

	CLK->APBCLK |= CLK_APBCLK_TMR0_EN_Msk;
	
	TIMER0->TCSR |= (1UL << TIMER_TCSR_MODE_Pos);
	//TIMER0->TCSR =0x00000000;
	TIMER0->TCSR &= ~TIMER_TCSR_PRESCALE_Msk;
	TIMER0->TCSR |= 5;//SRC: HCLK = PLLFOUT/2 >=8MHz
	TIMER0->TCMPR = 80;//10us;
	TIMER0->TCSR |= TIMER_TCSR_IE_Msk;
	TIMER0->TISR = TIMER_TISR_TIF_Msk;
	//Enable Timer0 Interrupt
	//NVIC_EnableIRQ(TMR0_IRQn);
	TIMER0->TCSR |= TIMER_TCSR_CRST_Msk;
	TIMER0->TCSR |= TIMER_TCSR_CEN_Msk;
	TIMER0->TCSR |= TIMER_TCSR_TDR_EN_Msk;
#endif	
}
//#else //timer 0 capture mode
void Timer0_Capture_Init(void) 
{
    	SYS->GPB_MFP |= (SYS_GPB_MFP_PB8_TM0 | SYS_GPB_MFP_PB15_TM0_EXT);
    	SYS->ALT_MFP  = SYS_ALT_MFP_PB15_TM0_EXT;
	 CLK_EnableModuleClock(TMR0_MODULE);
	 CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0_S_HCLK, 0); // 50MHz
   	 TIMER_Open(TIMER0, TIMER_CONTINUOUS_MODE, 1);
    	 TIMER_SET_PRESCALE_VALUE(TIMER0, 50); // 1us
    	 TIMER_SET_CMP_VALUE(TIMER0, 0xFFFFFF);
	 TIMER_DisableEventCounter(TIMER0);
    	 TIMER_EnableCapture(TIMER0, TIMER_CAPTURE_FREE_COUNTING_MODE, TIMER_CAPTURE_RISING_EDGE);
    	 TIMER_EnableCaptureInt(TIMER0);
	 NVIC_EnableIRQ(TMR0_IRQn);
    	 TIMER_Start(TIMER0);
}

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

void TMR0_IRQHandler(void)
{
	uint32_t Pulse_time;
    	//printf("TIMER_GetCaptureIntFlag\n");
    	if(TIMER_GetCaptureIntFlag(TIMER0) == 1)
    	{
       	 	/* Clear Timer0 capture interrupt flag */
       		TIMER_ClearCaptureIntFlag(TIMER0);
       		Pulse_time = TIMER_GetCaptureData(TIMER0);
		printf("Pulse_time %lu \n",Pulse_time);
	 	/* Reset Timer0 */
		TIMER0->TCSR |= TIMER_TCSR_CRST_Msk;		
		/* Check CEN */
		while(TIMER0->TCSR & TIMER_TCSR_CEN_Msk);			
		/* Restart Timer0 */
		TIMER_Start(TIMER0);      
    }
}

void TMR1_IRQHandler(void)
{
//printf("TMR1_IRQHandler \n");
   	TIMER1->TISR = TIMER_TISR_TIF_Msk;
	if(t1ms > 0) t1ms--;
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
void Delay_1ms(uint16_t t)
{
	t1ms = t;
	while(t1ms);
}



