#include <stdio.h>
#include "M0518.h"
#include "spi.h"
#include "UserSpi.h"
#include "UserTimer1.h"
#include "logrecord.h"
#include "UserE84.h"
//#include "def_ram.h"


volatile uint8_t EPPROM_Head[16];
volatile uint8_t EPPROM_Data_Normal[16];
volatile uint8_t EPPROM_Data_Fail[16];
volatile uint32_t EPPROM_TLCnt;              //EPPROM log count
volatile uint32_t EPPROM_CLIdx;              //EPPROM log current index
volatile uint32_t EPPROM_TFCnt;              //EPPROM log failure Event count
volatile uint32_t EPPROM_CFIdx;              //EPPROM log failure Event index
volatile uint32_t EPPROM_RLCnt;              //EPPROM log failure Event index
volatile uint16_t EPPROM_Address;
volatile uint8_t 	E84_STEP;
volatile _Bool   LogEnable_flag;

struct E84_STEP_Time E84_TimeOut={2,2,2,2,2,60,60,60};
struct E84_STEP_Time E84_TimeCount=	{0,0,0,0,0,0,0,0};
	




void EPPROM_Head_ini(void)
{
	
	uint8_t i,j;
	EPPROM_TLCnt=3;
	EPPROM_Address=0x00+EPPROM_TLCnt*16;
	
	
	
	EPPROM_Head[0]=0x01;          //Head of EEPROM for E84 log

	
	EPPROM_Head[1]='P';          //E84 working Mode 
	
	EPPROM_Head[2]=0x00;         //Total Event log count 
	EPPROM_Head[3]=0x00;         
	EPPROM_Head[4]=0x00;
	
	EPPROM_Head[5]=0x00;         //Current/Recent event log on EEPROM location Index
	EPPROM_Head[6]=0x00;
	
	EPPROM_Head[7]=0x00;         //Total Failure Event count 
	EPPROM_Head[8]=0x00;
	
	EPPROM_Head[9]=0x00;         //Current/Recent failure log  on EEPROM location Index 
	
	EPPROM_Head[10]=0x00;        //Event log count from Power up 
	EPPROM_Head[11]=0x00;
	EPPROM_Head[12]=0x00;
	
	EPPROM_Head[13]=0xFF;
	EPPROM_Head[14]=0xFF;
	EPPROM_Head[15]=0xFF;
	
	
	for(i=0;i<16;i++)
	{
		EEPROM_Write(0x0000+i,EPPROM_Head[i]);
		
	}
	
}

void EPPROM_Head_Read(void)
{
	uint16_t address;
	uint8_t i,L_bit,M_bit,H_bit;
	uint8_t head_data_r[16];
	
	for(i=0;i<16;i++)
	{
		EPPROM_Head[i]=EEPROM_Read(0x0000+i);
		
	}
	
	
	
	
	EPPROM_TLCnt=EPPROM_Head[4]+(EPPROM_Head[3]<<8)+(EPPROM_Head[2]<<16);               //Total Event log count
	EPPROM_CLIdx=EPPROM_Head[6]+(EPPROM_Head[5]<<8);                                    //EPPROM log current index
	EPPROM_TFCnt=EPPROM_Head[8]+(EPPROM_Head[7]<<8);                                    //EPPROM log failure Event count
	EPPROM_CFIdx=EPPROM_Head[9];                                                        //EPPROM log failure Event index
	//EPPROM_RLCnt=EPPROM_Head[12]+(EPPROM_Head[11]<<8)+(EPPROM_Head[10]<<16);            //Event log count from Power up
	
}


void Log_ini(void )
{
	EPPROM_Head_Read();
	EPPROM_RLCnt=0;
	E84_STEP=1;
}


void Log_Record(void)
{
	//EPPROM_Head[0]='P';
	 
	if(E84_SensorMode != E84_A_MODE)
	{
		switch(E84_STEP)
		 {
			 case 1:                                                    //CS0 or CS1 ON and Valid ON, TA1 start count
					if((P_OUT2==0 || P_OUT3==0 )&& P_OUT1==0)   
					{
						EPPROM_Data_Normal[2]=0xA1;
						EPPROM_Data_Normal[3]=0xA2;
						
						E84_STEP=2;
						t1ms_log=2000;
					}
					break;
			 case 2:
					if(t1ms_log <=0)                                        //TA1 timeout
					{
						EPPROM_Data_Normal[4]=0xF3;
						E84_STEP=10;
					}
					else if((P_IN1==0 || P_IN2==0 )&&t1ms_log >0)           // L_REQ or U_REQ on,TP1 start count
					{
						EPPROM_Data_Normal[4]=0xA3;
						t1ms_log=2000;
						E84_STEP=3;
					}
					break;
				
			 case 3:
					if(t1ms_log <=0)                                        //TP1 timeout
					{
						EPPROM_Data_Normal[5]=0xF4;
						E84_STEP=10;
					}
					else if(P_OUT5 ==0 &&t1ms_log >0)                       // TR_REQ on,TA2 start count
					{
						EPPROM_Data_Normal[5]=0xA4;
						t1ms_log=2000;
						E84_STEP=4;
					}					
					break;
					
			  case 4:
					if(t1ms_log <=0)                                        //TA2 timeout
					{
						EPPROM_Data_Normal[6]=0xF5;
						E84_STEP=10;
					}
					else if(P_IN4 ==0 &&t1ms_log >0)                       // READY on,TP2 start count
					{
						EPPROM_Data_Normal[6]=0xA5;
						t1ms_log=2000;
						E84_STEP=5;
					}					
					break;
					
				case 5:
					if(t1ms_log <=0)                                        //TP2 timeout
					{
						EPPROM_Data_Normal[7]=0xF6;
						E84_STEP=10;
					}
					else if(P_OUT6 ==0 &&t1ms_log >0)                       // BUSY on,TP3 start count
					{
						EPPROM_Data_Normal[7]=0xA6;
						t1ms_log=60000;
						E84_STEP=6;
					}					
					break;
					
				case 6:
					if(t1ms_log <=0)                                        //TP3 timeout
					{
						EPPROM_Data_Normal[8]=0xF7;
						E84_STEP=10;
					}
					else if((P_IN1==1 && P_IN2==1 ) && t1ms_log >0)         // L_REQ or U_REQ Off,TP4 start count
					{
						EPPROM_Data_Normal[8]=0xA7;
						t1ms_log=60000;
						E84_STEP=7;
					}					
					break;
					
				case 7:
					if(t1ms_log <=0)                                        //TP4 timeout
					{
						EPPROM_Data_Normal[9]=0xF8;
						EPPROM_Data_Normal[10]=0xF9;
						EPPROM_Data_Normal[11]=0xFA;
						E84_STEP=10;
					}
					else if((P_OUT6 == 1 && P_OUT5 ==1 && P_OUT7 ==0 ) && t1ms_log >0)         // BUSY and TR_REQ off and COMP On,TA3 start count
					{
						EPPROM_Data_Normal[9]=0xA8;
						EPPROM_Data_Normal[10]=0xA9;
						EPPROM_Data_Normal[11]=0xAA;
						t1ms_log=2000;
						E84_STEP=8;
					}					
					break;
					
				case 8:
					if(t1ms_log <=0)                                        //TA3 timeout
					{
						EPPROM_Data_Normal[12]=0xFB;
						E84_STEP=10;
					}
					else if(P_IN4 ==1  && t1ms_log >0)         // READY off ,TP5 start count
					{
						EPPROM_Data_Normal[12]=0xAB;
						t1ms_log=2000;
						E84_STEP=9;
					}					
					break;
					
				case 9:
					if(t1ms_log <=0)                                        //TP5 timeout
					{
						EPPROM_Data_Normal[13]=0xFC;
						EPPROM_Data_Normal[14]=0xFD;
						EPPROM_Data_Normal[15]=0xFE;
						E84_STEP=10;
					}
					else if(P_OUT7 ==1  && t1ms_log >0)         // COMP off 
					{
						EPPROM_Data_Normal[13]=0xAC;
						EPPROM_Data_Normal[14]=0xAD;
						EPPROM_Data_Normal[15]=0xAE;
						E84_STEP=10;
					}					
					break;
					
				case 10:
					
					if(EPPROM_TLCnt>=0xFFFFFF)
					{
						EPPROM_TLCnt=1;
					}
					else
					{
						EPPROM_TLCnt++;
					}
					
					if(EPPROM_RLCnt>=0xFFFFFF)
					{
						EPPROM_RLCnt=1;
					}
					else
					{
						EPPROM_RLCnt++;
					}
						
					Log_write();
          E84_STEP=1;
					break;
				
			 
		 }
		 
  }
	else
	{
	}


}

void Log_write(void  )
{
	uint16_t address;
	uint8_t i,L_bit,M_bit,H_bit;
	
	if(EPPROM_RLCnt<1023)
	{
		address=EPPROM_RLCnt*16;
	}
	else
	{
		address=(EPPROM_RLCnt%1023)*16;
	}
	if(address==0)
	{
		address=address+0x0A;
	}
	
	for(i=0;i<16;i++)
	{
		EEPROM_Write(address+i,EPPROM_Data_Normal[i]);
		EPPROM_Data_Normal[i]=0;
	}
	
	
	
	L_bit = EPPROM_TLCnt;
	M_bit = EPPROM_TLCnt >> 8;
	H_bit = EPPROM_TLCnt >>16;
	EEPROM_Write(0x0002,H_bit);
	EEPROM_Write(0x0003,M_bit);
	EEPROM_Write(0x0004,L_bit);
	
	L_bit = EPPROM_RLCnt;
	M_bit = EPPROM_RLCnt >> 8;
	H_bit = EPPROM_RLCnt >>16;
	EEPROM_Write(0x000A,H_bit);
	EEPROM_Write(0x000B,M_bit);
	EEPROM_Write(0x000C,L_bit);
	
	EPPROM_Head_Read();
	if(EPPROM_Head[0]==0)
	{
		EPPROM_Head[0]=1;
	}
	
	
}

void Head_Write(void)
{
	uint16_t address;
	uint8_t i,L_bit,M_bit,H_bit;
	
	for(i=0;i<16;i++)
	{
		EEPROM_Write(0x0000+i,EPPROM_Head[i]);
		
	}
	
	

}