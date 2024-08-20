
#include "M0518.h"
#include "def_const.h"
#include "def_exram.h"
#include "def_flag.h"

void adc_Init(void)
{
    /* reset ADC */
    SYS->IPRSTC2 |= SYS_IPRSTC2_ADC_RST_Msk;
    SYS->IPRSTC2 &= ~SYS_IPRSTC2_ADC_RST_Msk;

	//Set input mode
	//PA->PMD &= ~GPIO_PMD_PMD5_Msk;
	//PA->PMD &= ~GPIO_PMD_PMD6_Msk;
	//PA->PMD &= ~GPIO_PMD_PMD7_Msk;
	PA->OFFD |= (0xE0ul << GPIO_OFFD_OFFD_Pos);
	//Set ADC function
	SYS->GPA_MFP &= ~(SYS_GPA_MFP_PA5_Msk | SYS_GPA_MFP_PA6_Msk | SYS_GPA_MFP_PA7_Msk);
	SYS->GPA_MFP |= SYS_GPA_MFP_PA5_ADC5 | SYS_GPA_MFP_PA6_ADC6 | SYS_GPA_MFP_PA7_ADC7;
	
	SYS->ALT_MFP &= ~(SYS_ALT_MFP4_PA5_Msk | SYS_ALT_MFP4_PA6_Msk | SYS_ALT_MFP4_PA7_Msk);
	/* ADC clock source *///0:12M  1:PLL  2,3:22.1184
    CLK->CLKSEL1 |= CLK_CLKSEL1_ADC_S_Msk;//22.1184 MHz
    /* Set ADC divisor */
    CLK->CLKDIV &= ~CLK_CLKDIV_ADC_N_Msk;
	CLK->CLKDIV |= (49 << CLK_CLKDIV_ADC_N_Pos);//22.1184 MHz / (CLKDIV + 1)
    /* ADC engine clock enable */
    CLK->APBCLK |= CLK_APBCLK_ADC_EN_Msk;
    /* ADC enable */
    ADC->ADCR |= ADC_ADCR_ADEN_Msk;
	ADC->ADCR &= ~ADC_ADCR_DIFFEN_Msk;     /* single end input */
	//mode
	ADC->ADCR &= ~ADC_ADCR_ADMD_Msk;     
	ADC->ADCR |= (2ul << ADC_ADCR_ADMD_Pos);/* Single-cycle scan */
	//channel
    ADC->ADCHER &= ~ADC_ADCHER_CHEN_Msk;
	ADC->ADCHER |= (0xE0ul << ADC_ADCHER_CHEN_Pos);
	//CLEAR A/D INTERRUPT FLAG FOR SAFE , ENABLE ADC INTERRUPT
	ADC->ADSR |= ADC_ADSR_ADF_Msk;
	ADC->ADCR |= ADC_ADCR_ADIE_Msk;
	NVIC_EnableIRQ(ADC_IRQn);
}

void ADC_IRQHandler(void)
{
	static unsigned int AD_sum[3] = {0,0,0};
	static unsigned int adc_count = 0;
	unsigned int adc_buf[3];
	
    if((ADC->ADSR & ADC_ADSR_ADF_Msk) == ADC_ADSR_ADF_Msk)
    {
		adc_buf[0] = ADC->ADDR[7] & ADC_ADDR_RSLT_Msk;
		adc_buf[1] = ADC->ADDR[6] & ADC_ADDR_RSLT_Msk;
		adc_buf[2] = ADC->ADDR[5] & ADC_ADDR_RSLT_Msk;
		ADC->ADSR |= ADC_ADSR_ADF_Msk;
		AD_sum[0] += adc_buf[0];
		AD_sum[1] += adc_buf[1];
		AD_sum[2] += adc_buf[2];
		adc_count++;
		if(adc_count >= 800)
		{
			adc_count = 0;
			HR.ADC_sum[0] = AD_sum[0];
			HR.ADC_sum[1] = AD_sum[1];
			HR.ADC_sum[2] = AD_sum[2];
			AD_sum[0] = 0;
			AD_sum[1] = 0;
			AD_sum[2] = 0;
			Read_adc_fg = 1;
		}
    }
}

void ADC_AVG_sub(void)
{
	unsigned int adc_avg[3];
	long adc_value[3];
	adc_avg[0] = HR.ADC_sum[0] / 800;
	adc_avg[1] = HR.ADC_sum[1] / 800;
	adc_avg[2] = HR.ADC_sum[2] / 800;
	
	//InletPressure
	//1~5V-----0~1.0000
	adc_value[0] = adc_avg[0] * 10000 / 820; //4096/5 = 819.2
	if(adc_value[0] < 10000)
		adc_value[0] = 10000;
	adc_value[0] = (adc_value[0] - 10000) / 400;
	if(adc_value[0] <= 2)
		adc_value[0] = 0;
	HR.InletPressure[0] = adc_value[0] / 256;
	HR.InletPressure[1] = adc_value[0] % 256;
	
	//OutletFlow
	//1~5V-----0~200.00
	adc_value[1] = adc_avg[1] * 20000 / 820; //4096/5 = 819.2
	if(adc_value[1] < 20000)
		adc_value[1] = 20000;
	adc_value[1] = (adc_value[1] - 20000) / 4;
	if(adc_value[1] <= 30)
		adc_value[1] = 0;
	HR.OutletFlow[0] = adc_value[1] / 256;
	HR.OutletFlow[1] = adc_value[1] % 256;
	
	//O2Meter
	//1~5V-----0~25.00
	adc_value[2] = adc_avg[2] * 2500 / 820; //4096/5 = 819.2
	if(adc_value[2] < 2500)
		adc_value[2] = 2500;
	adc_value[2] = (adc_value[2] - 2500) / 40;
	if(adc_value[2] <= 2)
		adc_value[2] = 0;
	HR.O2Meter[0] = adc_value[2] / 256;
	HR.O2Meter[1] = adc_value[2] % 256;
}


