
#include "M0518.h"
#include "def_const.h"
#include "def_exram.h"
#include <math.h> //Keil library
#include <stdio.h> //Keil library

//----------------------------------------------------------------------------------
// modul-var
//----------------------------------------------------------------------------------
enum {TEMP,HUMI,REBOOT};

#define noACK 0
#define ACK 1

//adr command r/w
#define STATUS_REG_W 0x06 //000 0011 0
#define STATUS_REG_R 0x07 //000 0011 1
#define MEASURE_TEMP 0x03 //000 0001 1
#define MEASURE_HUMI 0x05 //000 0010 1
#define RESET 0x1e //000 1111 0

void delay_nop(void)
{
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
}
//----------------------------------------------------------------------------------
char s_write_byte(unsigned char value)
//----------------------------------------------------------------------------------
// writes a byte on the Sensibus and checks the acknowledge 
{ 
	unsigned char i,error=0;  
	for(i = 0x80 ; i > 0 ; i /= 2)      //shift bit for masking
	{
		if(i & value)
			SDA2 = 1;                   //masking value with i , write to SENSI-BUS
		else
			SDA2 = 0;
		delay_nop();                        
		SCK = 1;                        //clk for SENSI-BUS
		delay_nop();                    //pulswith approx. 5 us
		delay_nop();  	
		SCK = 0;
	}
	delay_nop();
	SDA2 = 1;                           //release DATA-line
	SCK = 1;                            //clk #9 for ack 
	error = SDA1;                       //check ack (DATA will be pulled down by SHT11)
	delay_nop();
	SCK = 0;        
	return error;                      //error=1 in case of no acknowledge
}

//----------------------------------------------------------------------------------
char s_read_byte(unsigned char ack)
//----------------------------------------------------------------------------------
// reads a byte form the Sensibus and gives an acknowledge in case of "ack=1" 
{ 
	unsigned char i,val = 0;
	SDA2 = 1;                           //release DATA-line
	for(i = 0x80 ; i > 0 ; i /= 2)      //shift bit for masking
	{
		SCK = 1;                        //clk for SENSI-BUS
		if(SDA1)
			val = (val | i);            //read bit
		delay_nop();  
		SCK = 0;  					 
	}
	delay_nop();
	SDA2 = !ack;                        //in case of "ack==1" pull down DATA-Line  
	SCK = 1;                            //clk #9 for ack
	delay_nop();                        //pulswith approx. 5 us 
	delay_nop();
	SCK = 0;						    
	SDA2 = 1;                           //release DATA-line
	return val;
}

//----------------------------------------------------------------------------------
void s_transstart(void)
//----------------------------------------------------------------------------------
// generates a transmission start 
//       _____         ________
// DATA:      |_______|
//           ___     ___
// SCK : ___|   |___|   |______
{  
	SDA2 = 1;
	SCK = 0;                   //Initial state
	delay_nop();
	SCK = 1;
	delay_nop();
	SDA2 = 0;
	delay_nop();
	SCK = 0;  
	delay_nop();
	delay_nop();
	SCK = 1;
	delay_nop();
	SDA2 = 1;		   
	delay_nop();
	SCK = 0;		   
}

//----------------------------------------------------------------------------------
void s_connectionreset(void)
//----------------------------------------------------------------------------------
// communication reset: DATA-line=1 and at least 9 SCK cycles followed by transstart
//       _____________________________________________________         ________
// DATA:                                                      |_______|
//          _    _    _    _    _    _    _    _    _        ___     ___
// SCK : __| |__| |__| |__| |__| |__| |__| |__| |__| |______|   |___|   |______
{  
	unsigned char i; 
	SDA2 = 1;
	SCK = 0;                          //Initial state
	delay_nop();
	for(i = 0 ; i < 9 ; i++)          //9 SCK cycles
	{
		SCK = 1;
		delay_nop();
		SCK = 0;
	}
	s_transstart();                   //transmission start
}
 							   
//----------------------------------------------------------------------------------
unsigned int s_measure(unsigned char mode)
//----------------------------------------------------------------------------------
// makes a measurement (humidity/temperature) with checksum
{ 
	unsigned char checksum , p_value_h , p_value_l;
	unsigned int	p_value;
	p_value_h = s_read_byte(ACK);    //read the first byte (MSB)
	p_value_l = s_read_byte(ACK);    //read the second byte (LSB)
	checksum = s_read_byte(noACK);   //read checksum
	p_value = 256 * p_value_h + p_value_l;
	s_transstart();                  //transmission start
	switch(mode)
	{
		case 0:
			s_write_byte(MEASURE_TEMP);//send command to sensor
			break;
		case 1:
			s_write_byte(MEASURE_HUMI);
			break;
		default:
			break;	 
	}
	return(p_value);
}

void calc_sth75(void)
{
//----------------------------------------------------------------------------------
// calculates temperature [C] and humidity [%RH]
// input : humi [Ticks] (12 bit)
// temp [Ticks] (14 bit)
// output: humi [%RH]
// temp [C]
	const float C1 = -2.0468; 		// for 12 Bit RH
	const float C2 = +0.0367; 		// for 12 Bit RH
	const float C3 = -0.0000015955; // for 12 Bit RH
	const float T1 = +0.01; 		// for 12 Bit RH
	const float T2 = +0.00008; 		// for 12 Bit RH
	
	float rh = HR.humi_val_i; // rh: Humidity [Ticks] 12 Bit
	float rh_lin;  // rh_lin: Humidity linear
	float rh_true; // rh_true: Temperature compensated humidity
	float t_C;     // t_C : Temperature [C]
	
	unsigned int temp_val , humi_val;

	t_C = HR.temp_val_i * 0.01 - 40.1; //calc. temperature[C]from 14 bit temp.ticks @5V
	if(t_C >= 0.0 && t_C <= 100.0)
	{
		rh_lin = C3*rh*rh + C2*rh + C1; //calc. humidity from ticks to [%RH]
		rh_true = (t_C-25)*(T1+T2*rh) + rh_lin; //calc. temperature compensated humidity[%RH]
		if(rh_true > 100)
			rh_true = 100;       //cut if the value is outside of
		if(rh_true < 0.1)
			rh_true = 0.1;       //the physical possible range
		temp_val = t_C*100;      //return temperature [C]
		humi_val = rh_true*100;  //return humidity[%RH]
		HR.Humidity[0] = humi_val / 256;
		HR.Humidity[1] = humi_val % 256;
		HR.Temperature[0] = temp_val / 256;
		HR.Temperature[1] = temp_val % 256;
	}
}



