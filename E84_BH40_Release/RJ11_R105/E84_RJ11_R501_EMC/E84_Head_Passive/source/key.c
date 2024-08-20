#include "M0518.h"
#include "Def_const.h"
#include "Def_flag.h"
#include "Motor.h"
#include "UserTimer0.h"
void DeGlitch(unsigned int time) // ms
{		
	Delay1Ms = time ;	
	while(Delay1Ms);
}

unsigned char KeyAction_Detecting(unsigned char enable)
{
	if(!enable) return 0;
	if(!key_dn && !KEY ){
		DeGlitch(20);  //around 30ms
		if(!KEY) key_dn = 1;
		else key_dn = 0;
	}
		
	if(key_dn && KEY ) {
		DeGlitch(400); // around 0.5s 
		if(KEY) key_up = 1;
		else key_up = 0;
	}	
	else key_up = 0;
       
	return key_up;
}
void KeyAction_Reset(void)
{
	KEY = 1;
	key_dn = 0,
	key_up = 0;
}


