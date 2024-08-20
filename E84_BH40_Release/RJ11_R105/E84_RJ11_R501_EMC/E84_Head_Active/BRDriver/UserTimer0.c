#include "M0518.h"
#include "def_const.h"
#include "def_flag.h"
#include "def_exram.h"
#include "Motor.h"
#include <stdio.h>

unsigned int Delay1Ms = 0; 

void Timer0_Init(void)
{
	CLK->CLKSEL1 |= CLK_CLKSEL1_TMR0_S_Msk;
	CLK->APBCLK |= CLK_APBCLK_TMR0_EN_Msk;
	
	TIMER0->TCSR |= (1UL << TIMER_TCSR_MODE_Pos);
	TIMER0->TCSR &= ~TIMER_TCSR_PRESCALE_Msk;
	TIMER0->TCSR |= 11;//Prescale = 11
	TIMER0->TCMPR = 2304;
	TIMER0->TCSR |= TIMER_TCSR_IE_Msk;
	TIMER0->TISR = TIMER_TISR_TIF_Msk;
	//Enable Timer0 Interrupt
	NVIC_EnableIRQ(TMR0_IRQn);
	TIMER0->TCSR |= TIMER_TCSR_CRST_Msk;
	TIMER0->TCSR |= TIMER_TCSR_CEN_Msk;
	TIMER0->TCSR |= TIMER_TCSR_TDR_EN_Msk;
}

void TMR0_IRQHandler(void)
{  
	static unsigned int  time_min = 48000;
	static unsigned int  time_ones = 800;
	static unsigned int  time_500ms = 400;
	
   	TIMER0->TISR = TIMER_TISR_TIF_Msk;
	ADC->ADCR |= ADC_ADCR_ADST_Msk;
	time_ones--;
	if(time_ones == 0)
	{
		time_ones = 800;
		ones_fg = 1;
	}

	if(Delay1Ms !=0) Delay1Ms--;
		
	if(fg_motor_on)
	{     
		if((time_ones == 800)&&(MotorD.T.sec != 0))
		{
			MotorD.T.sec --;
		}
		if( MotorD.T.sec == 0)
		{
			time_min--;
			if(time_min == 0)
			{
				time_min = 48000;
				if(MotorD.T.min != 0)
				{
					MotorD.T.min--;
				}	
			}
			if(!MotorD.T.min) MotorD.time = 0;

			//if(Motor_Dmin == 0) Motor_Dtime = 0; //should use union				
		}
	}

	time_500ms--;
	if(time_500ms == 0)
	{
		time_500ms = 400;
		t500ms_fg = 1;
	}
//--------------------------------------------------
//Rx0 delay time
	if(SCI.Rx_Dly_Time > 0)
  	{
    	SCI.Rx_Dly_Time--;
    	if(SCI.Rx_Dly_Time == 0)
    	{
      		Stop_Rx_Fg = 1;
      		SCI.Rx_Map_Byte = 0;
      		if(SCI.Rx_count < 259)
        		Rx_Last_Fg = 1;
	      	else 
      		{ 
        		SCI.Rx_count = 0;
        		Stop_Rx_Fg = 0;
      		}
    	}
  	}
//RTS Timer_on
	if(SCI.RTS_OnTmr > 0)
	{
		SCI.RTS_OnTmr--;
		if(SCI.RTS_OnTmr == 0)
		{
			SCI.TxCount = 0;
			//Enable interrupt
			UART3->IER |= UART_IER_THRE_IEN_Msk;
			
		}
	}
//RTS Timer_off
	if(SCI.RTS_OffTmr > 0)
	{
		SCI.RTS_OffTmr--;
		if(SCI.RTS_OffTmr == 0)
		{
			Stop_Rx_Fg = 0;
			RTS_LO;
		}
	}
//-------------------------------------------------- 
//--------------------------------------------------
//Rx1 delay time
	if(SCI1.Rx_Dly_Time > 0)
  	{
    	SCI1.Rx_Dly_Time--;
    	if(SCI1.Rx_Dly_Time == 0)
    	{
      		Stop_Rx1_Fg = 1;
      		SCI1.Rx_Map_Byte = 0;
      		if(SCI1.Rx_count < 259)
        		Rx1_Last_Fg = 1;
	      	else 
      		{ 
        		SCI1.Rx_count = 0;
        		Stop_Rx1_Fg = 0;
      		}
    	}
  	}
//RTS Timer_on
	if(SCI1.RTS_OnTmr > 0)
	{
		SCI1.RTS_OnTmr--;
		if(SCI1.RTS_OnTmr == 0)
		{
			SCI1.TxCount = 0;
			//Enable interrupt
			UART1->IER |= UART_IER_THRE_IEN_Msk;
		}
	}
//RTS Timer_off
	if(SCI1.RTS_OffTmr > 0)
	{
		SCI1.RTS_OffTmr--;
		if(SCI1.RTS_OffTmr == 0)
		{
			Stop_Rx1_Fg = 0;
		}
	}
}


