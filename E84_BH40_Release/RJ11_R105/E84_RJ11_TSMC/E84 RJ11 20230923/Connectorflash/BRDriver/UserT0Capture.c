#include "M0518.h"
#include <stdio.h>
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
