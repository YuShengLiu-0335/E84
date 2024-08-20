/*
//Version : V28
//Date : 2019,11,15
//1.add config for out mode of push pull 
*/
#include "M0518.h"
#include "UserE84.h"
#include <stdio.h>

//GPIO Input Define
#define M_BIT(n) (1<<n)
#define I_PA_PPMASK (M_BIT(13)|M_BIT(14)|M_BIT(15))
#define I_PB_PPMASK (M_BIT(15))
#define I_PC_PPMASK (M_BIT(7))
#define I_PD_PPMASK (M_BIT(7)|M_BIT(14)|M_BIT(15))
//V28 end 



void Init_E84_IN(void)
{
	/*
	P_IN1 = S_D1; 
	P_IN2 = S_D1;
	P_IN3 = S_D1;
	P_IN4 = S_D1;
	P_IN5 = S_D1;
	P_IN6 = S_D1;
	P_IN7 = S_D1;	
	P_IN8 = S_D1;
	
	P_IN1 = S_D0; 
	P_IN2 = S_D0;
	P_IN3 = S_D0;
	P_IN4 = S_D0;
	P_IN5 = S_D0;
	P_IN6 = S_D0;
	P_IN7 = S_D0;	
	P_IN8 = S_D0;
	*/
	//GPIO Input Define
	GPIO_SetMode(PA, I_PA_PPMASK, GPIO_PMD_INPUT);
	GPIO_SetMode(PB, I_PB_PPMASK, GPIO_PMD_INPUT);
	GPIO_SetMode(PC, I_PC_PPMASK, GPIO_PMD_INPUT);
	GPIO_SetMode(PD, I_PD_PPMASK, GPIO_PMD_INPUT);
	
	
	
}
//V28 start

#define M_PA_PPMASK (M_BIT(10)|M_BIT(11))
#define M_PB_PPMASK (M_BIT(1)|M_BIT(2)|M_BIT(3)|M_BIT(12))
#define M_PD_PPMASK (M_BIT(6))
#define M_PF_PPMASK (M_BIT(4)|M_BIT(5))
//V28 end 

void Init_E84_OUT(void)
{

        //V28 start
       GPIO_SetMode(PA, M_PA_PPMASK, GPIO_PMD_OUTPUT);
       GPIO_SetMode(PB, M_PB_PPMASK, GPIO_PMD_OUTPUT);
       GPIO_SetMode(PD, M_PD_PPMASK, GPIO_PMD_OUTPUT);
       GPIO_SetMode(PF,  M_PF_PPMASK, GPIO_PMD_OUTPUT);
        //V28 end 

	P_OUT1 = S_D0; 
	P_OUT2 = S_D0;
	P_OUT3 = S_D0;
	P_OUT4 = S_D0;
	P_OUT5 = S_D0;
	P_OUT6 = S_D0;
	P_OUT7 = S_D0;	
	P_OUT8 = S_D0;
}

