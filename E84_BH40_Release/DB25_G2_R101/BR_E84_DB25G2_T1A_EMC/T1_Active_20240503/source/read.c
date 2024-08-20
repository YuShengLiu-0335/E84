
#include "M0518.h"
#include "def_const.h"
#include "def_exram.h"
#include "def_flag.h"

unsigned char read_timeout;

void Delay_10us(uint16_t cnt)
{
	unsigned char i;
	while(cnt--)
	{
		i = 29;
		while(i--) ;
	}
}

void Delay_16us(uint16_t cnt)
{
	unsigned char i;
	while(cnt--)
	{
		i = 47;
		while(i--) ;
	}
}

unsigned char Tms3705aReadAsync(void)
{
	unsigned char i;
	unsigned char read_byte;
	read_byte = 0;	
	read_timeout = 0;
	while((SCIO_GET == 0) && (read_timeout < 200))
	{
		__NOP();
		__NOP();
		read_timeout++;
	}
	if(read_timeout == 200)
	{
		return 0;
	}
	
	Delay_16us(2);
	for(i = 0 ; i < 8 ; i++)
	{
		Delay_16us(4);
		read_byte = read_byte >> 1;
		if(SCIO_GET)
			read_byte = read_byte | 0x80;
	}
	Delay_16us(6);
	Delay_10us(25);
	return read_byte;	
}

void write_page_addr(unsigned char page_addr)
{
	unsigned char i;
	
	page_addr = page_addr << 2;
	for(i = 0 ; i < 8 ; i++)
	{
		if(page_addr & 0x01)
		{
			TXCT_HI;
			Delay_10us(100);
			TXCT_LO;
			Delay_10us(100);
		}
		else
		{
			TXCT_HI;
			Delay_10us(50);
			TXCT_LO;
			Delay_10us(150);
		}
		page_addr = page_addr >> 1;
	}
}

unsigned int crc16_check(unsigned char *ptr)
{
	unsigned int crc_value;
	unsigned char i,j;
	
	crc_value = 0;
	for(i = 0 ; i < 8 ; i ++)
	{
		crc_value = crc_value ^ (*ptr);
		for(j = 0 ; j < 8 ; j++)
		{
			if((crc_value & 0x01)== 0x01)
				crc_value = (crc_value >> 1) ^ 0x8408;
			else
				crc_value = (crc_value >> 1);
		}
		ptr++;
	}
	return crc_value;	
}

unsigned char read_page(unsigned char page_addr)
{
	unsigned char i,diag_byte,start_byte;
	unsigned char rebuf[10];
	unsigned int crc_val;
	
	TXCT_HI;
	Delay_10us(205);
	TXCT_LO;
	Delay_10us(212);
	diag_byte = Tms3705aReadAsync();
	if(diag_byte == 0)
		return 1;
	Delay_10us(5000);
	write_page_addr(page_addr);
	TXCT_HI;
	Delay_10us(300);
	start_byte = Tms3705aReadAsync();
	if(start_byte == 0)
		return 1;
	for(i = 0 ; i < 10 ; i++)
		rebuf[i] = ~Tms3705aReadAsync();
		
	crc_val = crc16_check(&rebuf[0]);
	if(((crc_val & 0x00ff) == rebuf[8]) && ((crc_val >> 8) == rebuf[9]))
	{
		for(i = 0 ; i < 8 ; i++)
			HR.Tag_value[7 - i + 8*(page_addr - 1)] = rebuf[i];
		return 0;
	}
	else
		return 1;		
}

unsigned char Rfid_read_sub(void)
{	
	HR.Tag_status = 2;

	if(read_page(1))
		return 1;
	Delay_10us(1000);
	if(read_page(2))
		return 1;

	if(read_timeout == 200)
		return 1;
	
	HR.Tag_status = 1;
	return 0;
}

