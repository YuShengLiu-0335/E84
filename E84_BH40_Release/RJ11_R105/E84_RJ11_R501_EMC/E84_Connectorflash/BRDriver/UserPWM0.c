#include"M0518.h"
#include <stdio.h>
#include "UserPWM0.h"
#include "UserPWM1.h"
#include "UserTimer1.h"
#include "UserTimer2.h"

volatile union E84_Serial E84;
volatile static uint8_t cnt;
volatile static uint32_t Frequency,duty;
volatile uint8_t fgE84_IR_PWM_Trans;

void UserPWM_Init(void)
{
    	SYS->GPA_MFP &= ~(SYS_GPA_MFP_PA12_Msk);
    	SYS->GPA_MFP |= (SYS_GPA_MFP_PA12_PWM0_CH0);
    	SYS->ALT_MFP4 &= ~(SYS_ALT_MFP4_PA12_Msk);

	PWM_ConfigOutputChannel(PWM0, 0, 13000, 100);
    	PWM_EnableOutput(PWM0, PWM_CH_0_MASK);
    	PWM_DisablePeriodInt(PWM0, 0);
    	PWM_DisableDutyInt(PWM0,0);	 

    	NVIC_EnableIRQ(PWM0_IRQn);
    	PWM0->POLCTL |= 1;
    	PWM_Start(PWM0, PWM_CH_0_MASK);
}

void PWM_Waveform(uint8_t dat, uint8_t mark)
{	
	switch (dat)
	{
		case HEADER:
			cnt = 10+1;
			Frequency = FREQ_MARK_0;
			duty = DUTY_MARK_0;
			break;
		case END:
			cnt =10+1;
			Frequency = FREQ_MARK_1;
			duty = DUTY_MARK_1;
			break;
		case DAT:
			cnt = 5+1;
			if(mark == 0)
			{
				Frequency = FREQ_MARK_0;
				duty = DUTY_MARK_0;
			}
			else
			{
				Frequency = FREQ_MARK_1;	
				duty = DUTY_MARK_1;
			}
			break;
		case PWM_DELAY400US:
			cnt = 1;
			Frequency = 2500;
			duty = 100;
			break;
	}
	PWM_ConfigOutputChannel(PWM0, 0, Frequency, duty);
    	PWM_EnablePeriodInt(PWM0, 0, 0);
	PWM_Start(PWM0, PWM_CH_0_MASK);
	while(cnt);

}

void E84_PWM_Waveform(volatile union E84_Serial * E84)
{
	PWM_Waveform(HEADER,0);	
	Delay_10us(T1); 
	PWM_Waveform(DAT,(uint16_t)E84->u16bit.sdat_0 );
	Delay_10us(T1);
	PWM_Waveform(DAT,(uint16_t)E84->u16bit.sdat_1 );
	Delay_10us(T1);
	PWM_Waveform(DAT,(uint16_t)E84->u16bit.sdat_2 );
	Delay_10us(T1);
	PWM_Waveform(DAT,(uint16_t)E84->u16bit.sdat_3 );
	Delay_10us(T1);
	PWM_Waveform(DAT,(uint16_t)E84->u16bit.sdat_4 );
	Delay_10us(T1);
	PWM_Waveform(DAT,(uint16_t)E84->u16bit.sdat_5 );
	Delay_10us(T1);
	PWM_Waveform(DAT,(uint16_t)E84->u16bit.sdat_6 );
	Delay_10us(T1);
	PWM_Waveform(DAT,(uint16_t)E84->u16bit.sdat_7 );
	Delay_10us(T1);
	PWM_Waveform(DAT,(uint16_t)E84->u16bit.sdat_8 );
	Delay_10us(T1);
	PWM_Waveform(END,0);		
}

void E84_IR_PWM_Trans(volatile union E84_Serial * E84)
{
	PWM_DisableCaptureInt(PWM1, 0,  PWM_CAPTURE_INT_FALLING_LATCH);
	PWM1_PulseCntReset();
	Delay_10us(148);
	E84_PWM_Waveform(E84);
	PWM_EnableCaptureInt(PWM1, 0,  PWM_CAPTURE_INT_FALLING_LATCH);
}

void PWM0_IRQHandler(void)
{
    if(PWM_GetPeriodIntFlag(PWM0,PWM_CH_0_MASK)) cnt--;
 
    PWM_ClearPeriodIntFlag(PWM0, 0);

    if(!cnt)
    {
    	 PWM_Stop(PWM0, PWM_CH_0_MASK);
    	 PWM_DisablePeriodInt(PWM0, 0);
    }
} 
