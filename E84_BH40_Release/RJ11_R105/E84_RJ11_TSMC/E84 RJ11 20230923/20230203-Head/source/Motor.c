#include "M0518.h"
#include "Def_const.h"
#include "Motor.h"
#include "def_flag.h"
#include "Key.h"
union uint_data MotorD, Motor_Flash;

void Motor(unsigned char active)
{
	Motor1 = active;
	Motor2 = active;
}
void Motor_LED(unsigned char y_led,unsigned char g_led)
{
	if(Y_LED != y_led) Y_LED = y_led;	
	if(G_LED != g_led) G_LED = g_led;
}	
void Motor_Delay(uint32_t time) // min : sec
{		
	MotorD.time = time ;	
	while(MotorD.time)
	{
		UNLOCKREG();
		WDT->WTCR |= WDT_WTCR_WTR_Msk;
		LOCKREG();
		
		if(!fg_motor_break) 
			fg_motor_break = KeyAction_Detecting(ON_1); //low active
			
		if(((IR1 == OFF_1)&&(IR2 == OFF_1)) ||fg_motor_break) 
		{
			KeyAction_Reset();
			MotorD.time = 0;
			Motor_LED(L_OFF, L_ON);
			break;
		}	
	}
}
void Motor_Active(uint32_t time)
{
	Motor(M_ON);
	Motor_LED(L_ON, L_OFF);
	Motor_Delay(time);
}
void Motor_Standby(uint32_t time)
{
	Motor(M_OFF);
	Motor_LED(L_ON, L_OFF);
	Motor_Delay(time);
}
void Motor_Idle(uint32_t time)
{
	Motor(M_ON);
	Motor_Delay(time);
}
void Motor_Off(void)
{
	Motor(M_OFF);
}
void Motor_Init(void)
{
	RTS_LO;

	//ULN2003 L_off
	DO_01 = 0;//#define DO_01		PF5
	DO_02 = 0;//#define DO_02		PB12
	DO_03 = 0;//#define DO_03		PB8
	DO_04 = 0;//#define DO_04		PF8
	DO_05 = 0;//#define DO_05		PB15

	// 3V LED 
	LED_RUN = 1;//#define LED_RUN		PC6
	LED_IDLE = 0;//#define LED_IDLE		PA14
	LED_PURGE = 0;//#define LED_PURGE	PA13
	LED_ALARM = 0;//#define LED_ALARM	PA12
	LED_LINK = 0;//#define LED_LINK		PA15

	//VAL_VAC
	PC0 = L_OFF;
	PC1 = L_OFF;
	
	//TMS3705
	TXCT_LO;//#define TXCT_LO 		PB0 = 0
	//#define SCIO_GET 	PB1

	//I2C
	SCK = 1;//#define SCK 			PA0	//SHT75 pin1
	SDA1 = 1;//#define SDA1 			PA1	//SHT75 pin4
	SDA2 = 1;//#define SDA2 			PA1	//SHT75 pin4
	
	// 24V Motor
	Motor1 = M_OFF;//#define Motor1 		PC2
	Motor2 = M_OFF;//#define Motor2		PC1
	G_LED = L_OFF;//#define G_LED		PC3
	Y_LED = L_OFF;//#define Y_LED		PD15

	fg_motor_on = 0;
	fg_motor_break = 0;
	Motor_Off();
}

