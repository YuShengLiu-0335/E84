#include <inttypes.h>
#include "M0518.h"
#include "def_const.h"

#define RH_I2C_WR	0		
#define RH_I2C_RD	1

static void i2c_Delay(void)
{
	__NOP(); //0.2us //I2C spd around  250k
	__NOP();
	__NOP();
	__NOP();
	__NOP();
}

void i2c_Start(void)
{
	SDA1 = 1;
	SCK = 1;
	i2c_Delay();
	SDA1 = 0;
	i2c_Delay();
	SCK = 0;
	i2c_Delay();
}

void i2c_Stop(void)
{
	SDA1 = 0;
	SCK = 1;
	i2c_Delay();
	SDA1 = 1;
}

void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			SDA1 = 1;
		}
		else
		{
			SDA1 = 0;
		}
		i2c_Delay();
		SCK = 1;
		i2c_Delay();	
		SCK = 0;
		if (i == 7)
		{
			 SDA1 = 1; 
		}
		_ucByte <<= 1;	
		i2c_Delay();
	}
}

uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		SCK = 1;
		i2c_Delay();
		if (SDA1)
		{
			value++;
		}
		SCK = 0;
		i2c_Delay();
	}
	return value;
}

uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	SDA1 = 1;	
	i2c_Delay();
	SCK = 1;	
	i2c_Delay();
	if (SDA1)	
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	SCK = 0;
	i2c_Delay();
	return re;
}

void i2c_Ack(void)
{
	SDA1 = 0;	
	i2c_Delay();
	SCK = 1;	
	i2c_Delay();
	SCK = 0;
	i2c_Delay();
	SDA1 = 1;	
}

void i2c_NAck(void)
{
	SDA1 = 1;	
	i2c_Delay();
	SCK = 1;	
	i2c_Delay();
	SCK = 0;
	i2c_Delay();	
}


uint8_t i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;
	i2c_Start();		
	i2c_SendByte(_Address | RH_I2C_WR);
	ucAck = i2c_WaitAck();
	i2c_Stop();			
	return ucAck;
}
