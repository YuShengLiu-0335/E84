#include"M0518.h"
#include <stdio.h>
#include "UserTimer1.h"
#include "UserTimer2.h"
#include "UserPWM1.h"
#include "UserPWM0.h"
#include "UserCONST.h"
#include "UserE84.h"

#define Mark_0 60 -10
#define Mark_1 100 -10
#define Gap 400 

 
volatile union E84_Serial E84_S;
volatile static uint8_t d0_cnt,d1_cnt,s_cnt,e_cnt,p_cnt,p_gap;
volatile uint16_t H_Period,L_Period,fgGotTrans;	


#define PBLEN 256
volatile uint16_t PB_idx;
volatile uint16_t Period_Buffet[PBLEN];

//debug tool
void Period_Buffer(void){
	if(PB_idx < 256) Period_Buffet[PB_idx++] = H_Period;
	else return;
}

void PWM1_PulseCntReset(void){
	d0_cnt = 0;
	d1_cnt = 0;
	s_cnt = 0;
	E84_S.data = 0;	 
}

uint8_t UserPWM1_CHK(void)
{
	volatile uint8_t even_odd_ok = EVEN_ODD == E84_S.u16bit.sdat_8 ? 1:0; 
	if( even_odd_ok)
	{
		TRANS_INFO;
		if(GO != GO_LED_ON) GO = GO_LED_ON;
	}
	else 
	{
		if(GO != GO_LED_OFF) GO = GO_LED_OFF;
	}	
	return even_odd_ok;
}	

void PWM1_IRQHandler(void)
{
       if((PWM_GetCaptureIntFlag(PWM1, 0) & 2) ==2){ 
        	PWM_ClearCaptureIntFlag(PWM1, 0, PWM_CAPTURE_INT_FALLING_LATCH );
        	H_Period = 0x10000-PWM_GET_CAPTURE_FALLING_DATA(PWM1, 0);
		p_cnt++;
		//Period_Buffer();
		if((H_Period > Mark_0)&&(H_Period < Mark_1)){
			d0_cnt ++;
			d1_cnt = 0;
        	}
        	
        	if((H_Period > Mark_1)&&(H_Period < Gap -100)){
			d1_cnt ++;
			d0_cnt = 0;
        	}

		if ((H_Period > Gap -50)){
			if((d0_cnt >= 8)&&(!P_START)){
				P_START = 1;
				s_cnt = 0;
			}
			else if ((d0_cnt >= 3)&&(P_START)&&(!P_END)){
				SDTA_BIT = 0;
			}	

			if ((d1_cnt >= 8)&&(!P_END)){
				P_END = 1;
			}	
			else if ((d1_cnt >= 3)&&(P_START)&&(!P_END)){
				SDTA_BIT = 1;
			}
					
			//s_cnt ++;
			d0_cnt = 0;
			d1_cnt = 0;
			p_cnt = 0;
        	}
		
		
        	if(P_START&&(H_Period > Gap -50)){ 			
			switch(s_cnt ){
			case 0:
				break;
			case 1:	
				E84_S.u16bit.sdat_0 =  SDTA_BIT;
				break;
			case 2:
				E84_S.u16bit.sdat_1 =  SDTA_BIT;	
				break;
			case 3:
				E84_S.u16bit.sdat_2 =  SDTA_BIT;	
				break;
			case 4:
				E84_S.u16bit.sdat_3 =  SDTA_BIT;	
				break;
			case 5:
				E84_S.u16bit.sdat_4 =  SDTA_BIT;	
				break;
			case 6:
				E84_S.u16bit.sdat_5 =  SDTA_BIT;	
				break;
			case 7:
				E84_S.u16bit.sdat_6 =  SDTA_BIT;	
				break;
			case 8:
				E84_S.u16bit.sdat_7 =  SDTA_BIT;
				break;
			case 9:
				E84_S.u16bit.sdat_8 =  SDTA_BIT;
				fgGotTrans = UserPWM1_CHK();
				//debug if(fgGotTrans)printf("%x\n",E84_S.data); //for debug
				if(!fgGotTrans) PWM1_PulseCntReset();
				break;		
			}	

			if(P_END) PWM1_PulseCntReset();
        		else
        		{
        			//s_cnt ++; //V21
				if(s_cnt < 9) s_cnt ++; //V23
				else PWM1_PulseCntReset();
        		}	
        	}
        }      
}

void UserPWM1_Init(void)
{
     	CLK_EnableModuleClock(PWM1_MODULE);
     	CLK_SetModuleClock(PWM1_MODULE, CLK_CLKSEL3_PWM1_S_PCLK, 0); //50M
     	SYS_ResetModule(PWM1_RST);

     	SYS->GPA_MFP = (SYS->GPA_MFP & (~SYS_GPA_MFP_PA2_Msk));
    	SYS->GPA_MFP |= SYS_GPA_MFP_PA2_PWM1_CH0;
     	SYS->ALT_MFP3 &= ~(SYS_ALT_MFP3_PA2_Msk);
     	SYS->ALT_MFP3 |= SYS_ALT_MFP3_PA2_PWM1_CH0;
}

void  UserPWM1_Capture_Start(void)
{
	PWM_ConfigCaptureChannel(PWM1, 0, 1000, 0); // 1us
	PWM_Start(PWM1, PWM_CH_0_MASK);
	PWM_EnableCapture(PWM1, PWM_CH_0_MASK);
	PWM1->CAPCTL |= PWM_CAPCTL_FCRLDEN0_Msk;
	PWM1->CAPCTL |= PWM_CAPCTL_RCRLDEN0_Msk;
	PWM_EnableCaptureInt(PWM1, 0,  PWM_CAPTURE_INT_FALLING_LATCH);
	NVIC_EnableIRQ(PWM1_IRQn);
	while((PWM1->CNT[0]) == 0);
}
	


