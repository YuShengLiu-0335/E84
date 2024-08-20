#include "M0518.h"


#ifndef __LOG_HANDLE_H__
#define __LOG_HANDLE_H__

extern volatile uint8_t EPPROM_Head[16];
extern volatile uint8_t EPPROM_Data_Normal[16];
extern volatile uint8_t EPPROM_Data_Fail[16];
extern volatile uint32_t EPPROM_TLCnt;              //EPPROM log count
extern volatile uint32_t EPPROM_CLIdx;              //EPPROM log current index
extern volatile uint32_t EPPROM_TFCnt;              //EPPROM log failure Event count
extern volatile uint32_t EPPROM_CFIdx;              //EPPROM log failure Event index
extern volatile uint32_t EPPROM_RLCnt;
extern volatile uint8_t 	E84_STEP;
extern volatile _Bool   LogEnable_flag;

void Head_Write(void);
void EPPROM_Head_Read(void);
void EPPROM_Head_ini(void);
void Log_write(void );
void Log_Record(void);
void Log_ini(void );


struct E84_STEP_Time 
	{
		uint8_t TA1;
		uint8_t TA2;
		uint8_t TA3;
		uint8_t TP1;
		uint8_t TP2;
		uint8_t TP3;
		uint8_t TP4;
		uint8_t TP5; 
		
	};
	
extern struct E84_STEP_Time E84_TimeCount;
extern struct	E84_STEP_Time E84_TimeOut;
	

#endif