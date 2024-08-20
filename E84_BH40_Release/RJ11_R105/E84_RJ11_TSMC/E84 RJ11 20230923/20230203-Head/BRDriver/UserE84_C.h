#ifndef E84_C_H 
	#define  E84_C_H
	#include "UserE84.h"
	#include "UserM518.h"

	void E84_Controller(void);
	void InitE84_C_Signal(void);

	//#define E84_DEBUG //V28
        #define VERSION 28
	#ifdef E84_DEBUG
		#define PRINT_ON 1
		void E84_Info_Print(uint8_t print);
		void DBG_CodeStop(uint8_t enable);
		#define ENABLE 1
		#define DISABLE 0
		#define CHECK DISABLE
	#endif

	#define fgE84_Controller PA0
	#define E84_CONTROLLER_ON 0

	#define C_IN1 P08  
	#define C_IN2 P09  
	#define C_IN3 P25 
	#define C_IN4 P15  
	#define C_IN5 P26 
	#define C_IN6 P27 
	#define C_IN7 P16 
	#define C_IN8 P17  

	#define C_OUT1 P01  
	#define C_OUT2 P04  
	#define C_OUT3 P43 
	#define C_OUT4 P47 
	#define C_OUT5 P05  
	#define C_OUT6 P06   
	#define C_OUT7 P07   
	#define C_OUT8 P48 
	
	#define E84_VALID C_OUT1  
	#define E84_CS C_OUT2 
	#define E84_IN3 C_OUT3
	#define E84_IN4 C_OUT4
	#define E84_TR_REQ C_OUT5  
	#define E84_BUSY C_OUT6   
	#define E84_COMPT C_OUT7  
	#define E84_IN8 C_OUT8

	#define E84_LOAD C_IN1  
	#define E84_ULOAD C_IN2  
	#define E84_OUT3 C_IN3
	#define E84_READY C_IN4  
	#define E84_OUT5 C_IN5
	#define E84_OUT6 C_IN6
	#define E84_HOAVBL C_IN7  
	#define E84_ES C_IN8 

	#define Load_Start PF4
	#define Unload_Start PF5
	#define TA1Alarm 20
	#define TA2Alarm 20
	#define TA3Alarm 20
	#define TP3Alarm 600

	//#define IC_ULN2003
	#ifdef IC_ULN2003
	#define D1 1
	#define D0 0
	#else
	#define D1 0
	#define D0 1
	#endif
#endif	
