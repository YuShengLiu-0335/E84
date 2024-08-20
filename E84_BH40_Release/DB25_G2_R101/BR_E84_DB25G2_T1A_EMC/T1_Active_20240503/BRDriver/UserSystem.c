#include "M0518.h"

#define PLL_CLOCK 50000000
//#define INT_CLK
void UserSystemClk(void)
{
	#ifdef INT_CLK
	//HIRC CLK PWR ON
    	CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);
    	CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);

    	//PLLFOUT
    	CLK_SetCoreClock(PLL_CLOCK);
	CLK_SetSysTickClockSrc(CLK_CLKSEL0_STCLK_S_HCLK_DIV2);

	//PWM
	CLK_EnableModuleClock(PWM0_MODULE);
    	CLK_SetModuleClock(PWM0_MODULE, CLK_CLKSEL3_PWM0_S_PLL, 0);
	SYS_ResetModule(PWM0_RST);
    	SystemCoreClockUpdate();
	#else
	CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);
    	CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);

	CLK_SetCoreClock(PLL_CLOCK);
	CLK_SetSysTickClockSrc(CLK_CLKSEL0_STCLK_S_HCLK_DIV2);

	CLK_EnableModuleClock(PWM0_MODULE);
    	CLK_SetModuleClock(PWM0_MODULE, CLK_CLKSEL3_PWM0_S_PLL, 0);
	SYS_ResetModule(PWM0_RST);
    	SystemCoreClockUpdate();
	#endif
}
