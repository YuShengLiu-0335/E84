#ifndef UserPWM1_H
	#define UserPWM1_H
	
	#include "UserE84.h"
	void UserPWM1_Init(void);
	void  UserPWM1_Capture_Start(void);
	uint8_t UserPWM1_CHK(void);
	void PWM1_PulseCntReset(void);

	extern volatile union E84_Serial E84_S;
	#define P_START E84_S.u16bit.sdat_10
	#define P_END E84_S.u16bit.sdat_9
	#define SDTA_BIT E84_S.u16bit.sdat_11
	#define EVEN_ODD (E84_S.u16bit.sdat_0+E84_S.u16bit.sdat_1+E84_S.u16bit.sdat_2+E84_S.u16bit.sdat_3+E84_S.u16bit.sdat_4+E84_S.u16bit.sdat_5+E84_S.u16bit.sdat_6+E84_S.u16bit.sdat_7)%2

	extern volatile uint16_t H_Period,L_Period,fgGotTrans;
#endif
