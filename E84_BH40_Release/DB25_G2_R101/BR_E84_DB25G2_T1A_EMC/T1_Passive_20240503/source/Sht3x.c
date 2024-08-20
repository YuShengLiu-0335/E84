#include <inttypes.h>
#include "M0518.h"
#include "def_const.h"
#include "def_struct.h"
#include "I2C_GPIO.h"
#include <stdio.h>

#define Sht3x_DEV_ADDR			0x88
#define Sht3x_BUF_SIZE			6
#define Sht3x_I2C_WR				0		
#define Sht3x_I2C_RD				1		

uint16_t HUMI;
uint16_t temperature;
uint16_t SHT3X[2];
extern HR_struct	HR; 

uint8_t Sht3x_CheckDevice(void)
{
	if (i2c_CheckDevice(Sht3x_DEV_ADDR) == 0)
	{
		return 1;
	}
	else
	{
		i2c_Stop();		
		return 0;
	}
}
/*--------------------------------------------------------------------------------------------------*/
uint8_t Sht3x_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i;
	i2c_Start();
	
	i2c_SendByte(Sht3x_DEV_ADDR | Sht3x_I2C_RD);
	 
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	
	}
	for (i = 0; i < _usSize; i++)
	{
		_pReadBuf[i] = i2c_ReadByte();	
		if (i != _usSize - 1)
		{
			i2c_Ack();	
		}
		else
		{
			i2c_NAck();	
		}
	}
	i2c_Stop();
	return 1;	

cmd_fail: 
	
	i2c_Stop();
	return 0;
}
/*--------------------------------------------------------------------------------------------------*/
uint8_t Sht3x_WriteByte(uint16_t _usAddress, uint8_t _pWriteBuf)
{
	uint16_t m;
	uint16_t usAddr;
	
	usAddr = _usAddress;	
	i2c_Stop();
	for (m = 0; m < 1000; m++)
	{				
		i2c_Start();
		i2c_SendByte(Sht3x_DEV_ADDR | Sht3x_I2C_WR);	
		if (i2c_WaitAck() == 0)
		{
			break;
		}
	}
	if (m  == 1000)
	{
		goto cmd_fail;	
	}
		
	i2c_SendByte((uint8_t)usAddr);
			
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	
	}
	
	i2c_SendByte(_pWriteBuf);
	
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	
	}
	
	i2c_Stop();
	return 1;

cmd_fail: 
	i2c_Stop();
	return 0;
}

/*--------------------------------------------------------------------------------------------------*/
static void Sht3x_Delay(__IO uint32_t nCount) //unit 50ums	 
{
	for(; nCount != 0; nCount--); 
}
/*-----------------------------------------------------------------------------------*/
uint8_t ReadSht3xSensor(void) 
{
  	uint8_t read_buf[Sht3x_BUF_SIZE];

  	if (Sht3x_WriteByte(0x2C, 0x06) == 0) //clk stretching disable 
	{
		return 0;
	}
	else
	{		
	}
  	//Sht3x_Delay(0x0FFFFF); //0.5s
  	Sht3x_Delay(0x00FFFF); //32ms

  	if (Sht3x_ReadBytes(read_buf, 0, Sht3x_BUF_SIZE) == 0)
	{
		return 0;
	}
	else
	{	

	       temperature = (uint16_t)(read_buf[0]*16*16)+read_buf[1];
	       HUMI =(uint16_t) (read_buf[3]*16*16)+read_buf[4];

              SHT3X[0]=HUMI*10/65;
	       SHT3X[1]=(temperature*100/374)-4500;		
	       return 1;
	}
}
/*-----------------------------------------------------------------------------------*/  
void Sht3x_SoftReset(void)
{
	Sht3x_WriteByte(0x30, 0xA2);
}
//----------------------------------------------------------------------------------
#if 0
void s_measure(void)
{ 
	ReadSht3xSensor();
}
#endif
//----------------------------------------------------------------------------------
void calc_sht3x(void)
{
	HR.Humidity[0] = SHT3X[0] / 256;
	HR.Humidity[1] = SHT3X[0] % 256;
	HR.Temperature[0] = SHT3X[1] / 256;
	HR.Temperature[1] = SHT3X[1] % 256;

}
//----------------------------------------------------------------------------------
void s_measure(void)
{ 
	ReadSht3xSensor();
	calc_sht3x();
}

