#include "M0518.h"
#include "UserE84_C.h"
#include <stdio.h>

static volatile uint8_t E84Step = 0;
static volatile uint16_t TACount = 0;
static volatile uint8_t FgE84Test = 0;

void CheckE84Step(void)
{
  	switch(E84Step)
  	{
    		case 1:
       			if(E84_CS== D0) 
	  		{
	    			E84_CS = D1;
				printf("Set CS_0 On.    \n");
	   		}
	   		else
	   		{
	     			E84Step=2;
	   		}
	   		break;

		case 2:
       			if(E84_VALID ==D0)
	   		{
	    			E84_VALID=D1;
				printf("Set Valid On.   \n");
	   		}
	   		else
	   		{
	     			E84Step=3;
	   		}	   
	   		break;

		case 3:
       			if(Load_Start == 0)
	   		{
		 		if(E84_LOAD == 0)
		 		{
		   			printf("L_REQ ON.       \n");
		   			E84Step=4;
		 		}
	   		}

	  		if(Unload_Start == 0)
	   		{
		 		if(E84_ULOAD == 0)
		 		{
		   			printf("U_REQ ON.       \n");
		   			E84Step=4;
		 		}
	   		}

       			TACount+=1;
	   		printf("TACount %x \n",TACount);
	   		if(TACount > TA1Alarm)
	   		{
	     			printf("TA1 TimeOut!!   \n");
		 		E84Step=99;		   
	   		}
	   		break;

		case 4:
       			if(E84_TR_REQ == D0)
	   		{
	    			E84_TR_REQ = D1;
				printf("Set TR_REQ On.  \n");	  
	   		}
	   		else
	  		{
	     			TACount=0;
	     			E84Step=5;
	   		}	   
	   		break;

		case 5:
		 	if(E84_READY == 0)
		 	{
		   		printf("READY ON.       \n");
		   		E84Step=6;
		 	}

       			TACount+=1;
	   		printf("TACount %x \n",TACount);
	   		if(TACount > TA2Alarm)
	   		{
	     			//displine2();
	     			printf("TA2 TimeOut!!   \n");
		 		E84Step=99;		   
	   		}
	   		break;

    		case 6:
       			if(E84_BUSY==D0)
	   		{
	    			E84_BUSY=D1;
				printf("Set BUSY On.    \n");	  
	   		}
	   		else
	   		{
	     			TACount=0;
	     			E84Step=7;
	   		}	   
	   		break;

    		case 7:
       			if(Load_Start == 0)
	   		{
		 		if(E84_LOAD== 1)
		 		{
		   			printf("L_REQ OFF.      \n");
		   			E84Step=8;
		 		}
	   		}

	   		if(Unload_Start == 0)
	   		{
				if(E84_ULOAD == 1)
		 		{
		   			printf("U_REQ OFF.      \n");
		   			E84Step=8;
		 		}
	   		}

       			TACount+=1;
	   		printf("TACount %x \n",TACount);

	   		if(TACount > TP3Alarm)
	   		{
	     			printf("Carrier TimeOut!\n");
		 		E84Step=99;		   
	   		}
	   		break;
	   
   		case 8:
       			if(E84_BUSY==D1)
	   		{
	    			E84_BUSY=D0;
				printf("Set BUSY Off.   \n");	  
	   		}
	   		else
	   		{
	    	 		E84Step=9; 
	   		}	   
	   		break;  

		case 9:
       			if(E84_TR_REQ==D1)
	   		{
	    			E84_TR_REQ=D0;
				printf("Set TR_REQ Off. \n");	  
	   		}
       			else if(E84_COMPT==D0)
	   		{
	    			E84_COMPT=D1;
				printf("Set COMPT On.   \n");		     
	   		}
	   		else
	   		{
	    		 	TACount=0;
	     			E84Step=11;
	   		}
	   		break;  	       

		case 11:
		 	if(E84_READY == 1)// 1
		 	{
		   		printf("READY OFF.      \n");
		   		E84Step=12;
		 	}
       			TACount+=1;
	   		printf("TACount %x \n",TACount);
	   		if(TACount > TA3Alarm)
	   		{
	     			printf("TA3 TimeOut!!   \n");
		 		E84Step=99;		   
	   		}
	   		break; 
	   
		case 12:
	  		if((E84_VALID==D1) || (E84_COMPT==D1) || (E84_CS==D1))
	  		{
	    			E84_VALID=D0;
				E84_COMPT=D0;
				E84_CS =D0;
	    			printf("Valid&COMPT&CS_0\n");
	  		}
	  		else
	  		{
	    			E84Step=13;
	  		}
	  		break;

		case 13:
	    		printf("E84 Finished.   \n");
			break;

    		default:
      			printf("Alarm!!!        \n");
      			E84_VALID =D0;
	  		E84_CS  =D0;
	  		E84_TR_REQ=D0;
	  		E84_BUSY  =D0;
	  		E84_COMPT  =D0;
	  		break;
  	}
}

void E84_Controller(void)
{
        if((Load_Start == 0) || (Unload_Start == 0))
	{
		 if(FgE84Test == 0)
		{
			FgE84Test=1;
		      	if(Load_Start==0) printf("E84 Load On.    \n"); 
		      	else printf("E84 UnLoad On.  \n");

		  	TACount=0;
			E84Step=1;
		}
	}
	else
	{
		if(FgE84Test == 1)
		{
			FgE84Test=0;
		      	printf("E84 Test Stop.  \n");
		      	E84_VALID =D0;
			E84_CS  =D0;
			E84_TR_REQ=D0;
	  		E84_BUSY  =D0;
	  		E84_COMPT  =D0;
		  }
	}

	if (FgE84Test == 1)CheckE84Step();
}

void InitE84_C_Signal(void)
{
	E84_VALID = D0;
	E84_CS = D0;
	E84_IN3 = D0;
	E84_IN4 = D0;
 	E84_TR_REQ = D0;
	E84_BUSY = D0;
	E84_COMPT = D0;
	E84_IN8 = D0;
}
void E84_Info_Print(uint8_t print)
{
	if(print)
	{
    printf("2019.05.28 - V%d\n",VERSION);
		printf("PLL CLOCK %ld \n",PllClock);
		printf("SystemCoreClock %ld \n",SystemCoreClock);
		printf("PLLCON %lu \n",CLK->PLLCON);
		printf("PWRCON %lu \n",CLK->PWRCON);
	}	
}

void DBG_CodeStop(uint8_t enable)
{
	while(enable);
}	

