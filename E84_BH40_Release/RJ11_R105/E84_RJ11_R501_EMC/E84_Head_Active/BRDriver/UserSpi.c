/******************************************************************************************************
 * @file     SPI_handle.c
 * @version  V1.00
 * $Revision: 0.1
 * $Date: 2021/07/28 
 * @brief    M480 SPI handle source file.
 *
 * @note
 *  Brillian Confidential 
 *  (C) Copyright 2021, Brillian Network & Automation Integrated System Co.,Ltd.  All rights reserved. 
******************************************************************************************************/
#include <stdio.h>
#include "M0518.h"
#include "spi.h"
#include "UserSpi.h"
#include "UserTimer1.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
volatile uint8_t gSPISendData[SPIBUFSIZE] = {0};
volatile uint8_t gSPIReceiveData[SPIBUFSIZE] = {0};
volatile uint8_t gSPITxTotalCount = 0, gSPITxCount = 0, gSPIRxCount = 0;
volatile uint8_t gSPITxStartFlag, gSPIRxDataOK;

#define TEST_COUNT 16

uint8_t g_au8SourceData[TEST_COUNT];
uint8_t g_au8DestinationData[TEST_COUNT];
volatile uint8_t g_u8TxDataCount;
volatile uint8_t g_u8RxDataCount;


void SPI_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init SPI                                                                                                */
    /*---------------------------------------------------------------------------------------------------------*/
	
		//CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HXT, CLK_CLKDIV_HCLK(1));
		/* Select HCLK as the clock source of SPI0 */
    //CLK_SetModuleClock(SPI0_MODULE, CLK_CLKSEL1_SPI0_S_PLL, MODULE_NoMsk);
	  CLK_SetModuleClock(SPI0_MODULE, CLK_CLKSEL1_SPI0_S_HCLK, MODULE_NoMsk);
	
		/* Enable SPI0 peripheral clock */
    CLK_EnableModuleClock(SPI0_MODULE);
	
		/* Setup SPI0 multi-function pins */
    SYS->GPC_MFP &= ~(SYS_GPC_MFP_PC0_Msk | SYS_GPC_MFP_PC1_Msk | SYS_GPC_MFP_PC2_Msk | SYS_GPC_MFP_PC3_Msk);
		SYS->GPC_MFP |= (SYS_GPC_MFP_PC0_SPI0_SS0 | SYS_GPC_MFP_PC1_SPI0_CLK | SYS_GPC_MFP_PC2_SPI0_MISO0 | SYS_GPC_MFP_PC3_SPI0_MOSI0);
    SYS->ALT_MFP &= ~(SYS_ALT_MFP_PC0_Msk | SYS_ALT_MFP_PC1_Msk | SYS_ALT_MFP_PC2_Msk | SYS_ALT_MFP_PC3_Msk);
		SYS->ALT_MFP |= (SYS_ALT_MFP_PC0_SPI0_SS0 | SYS_ALT_MFP_PC1_SPI0_CLK | SYS_ALT_MFP_PC2_SPI0_MISO0 | SYS_ALT_MFP_PC3_SPI0_MOSI0);
	
    /* Configure SPI0 as a master, clock idle low, 8-bit transaction, drive output on falling clock edge and latch input on rising edge. */
    /* Set IP clock divider. SPI clock rate = 2MHz */
    SPI_Open(SPI0, SPI_MASTER, SPI_MODE_0, 8, 2000000);
		SPI_EnableFIFO(SPI0, 5, 1);
		SPI_EnableInt(SPI0, SPI_FIFO_RX_INT_MASK | SPI_FIFO_TIMEOUT_INT_MASK);
    NVIC_EnableIRQ(SPI0_IRQn);
		//SPI_ENABLE_DUAL_OUTPUT_MODE(SPI0);
		//SPI_SET_SS0_LOW(SPI0);
		//SPI_SET_SS0_HIGH(SPI0);
		//SPI0->SSR=1;
    /* Enable the automatic hardware slave select function. Select the SPI0_SS pin and configure as low-active. */
    //SPI_EnableAutoSS(SPI0, SPI_SS0, SPI_SS_ACTIVE_LOW);
		//SPI_EnableAutoSS(SPI0, SPI_SS0, SPI_SS_ACTIVE_HIGH);
}

void SPI0_IRQHandler(void)
{
    /* Check RX EMPTY flag */
    while(SPI_GET_RX_FIFO_EMPTY_FLAG(SPI0) == 0)
    {
        /* Read RX FIFO */
        gSPIReceiveData[g_u8RxDataCount++] = SPI_READ_RX(SPI0);
				//SPI_DisableInt(SPI0, SPI_FIFO_TX_INT_MASK); /* Disable TX FIFO threshold interrupt */
				//SPI_DisableInt(SPI0, SPI_FIFO_TX_INT_MASK);
    }
		
		
		
		
    /* Check the RX FIFO time-out interrupt flag */
    if(SPI_GetIntFlag(SPI0, SPI_FIFO_TIMEOUT_INT_MASK))
    {
        /* If RX FIFO is not empty, read RX FIFO. */
        while(SPI_GET_RX_FIFO_EMPTY_FLAG(SPI0) == 0)
            gSPIReceiveData[g_u8RxDataCount++] = SPI_READ_RX(SPI0);
    }
		//*********************************************************************************************
}

void SendSpi(void)
{
	
	uint8_t  index, datalength;
  uint16_t checksum, checksumtarget;
  uint16_t receivecount;
   
  g_u8TxDataCount = 0; 
  g_u8RxDataCount = 0;
  gSPISendData[0] = 0x05;   //SPI packet start
	gSPISendData[1] = 0x02;
	gSPISendData[2] = 0x1F;
  gSPISendData[3] = 0xAA;
	gSPISendData[4] = 0xBB;
	
	
	gSPITxTotalCount = 1;
	
	
	gSPITxStartFlag = 1;
	
  g_u8TxDataCount = 0;
  g_u8RxDataCount = 0;
	//SPI_EnableInt(SPI0, SPI_FIFO_TX_INT_MASK | SPI_FIFO_TIMEOUT_INT_MASK);
  //NVIC_EnableIRQ(SPI0_IRQn);
	//t1ms_3 =5;
	
	//while(t1ms_3 && gSPITxStartFlag);   //To wait Transmit data finish
	
	
	
	
	//SPI_DisableInt(SPI0, SPI_FIFO_TX_INT_MASK | SPI_FIFO_TIMEOUT_INT_MASK);
  //NVIC_DisableIRQ(SPI0_IRQn);
	//SPI0->CNTRL
		
		

}


uint8_t EEPROM_Read(uint16_t ROM_Address)
{
		uint8_t  i, datalength;
		uint8_t High_bit,Low_bit;
		uint16_t checksum, checksumtarget;
		uint16_t receivecount;
		 
		High_bit=ROM_Address>>8;
		Low_bit =ROM_Address;
		
		g_u8RxDataCount=0;
		
		for(i=0;i<=8; i++)
		{
			gSPIReceiveData[i]=0;
		}	
	
		
	
    SPI0->SSR=1;
		
    SPI0->TX =0x03;
	  SPI0->TX =High_bit;
		SPI0->TX =Low_bit;
		SPI0->TX =0x00;
		SPI_TRIGGER(SPI0);
				
		t1ms_3 =5;
		while(t1ms_3 );
		
		
    //PC0=1;
		SPI0->SSR=0;
		//NVIC_DisableIRQ(SPI0_IRQn);
    return gSPIReceiveData[3];
	
	



}



void EEPROM_ReadStatus(void )
{
	uint32_t Local_8=0;
	uint8_t i =0;
	g_u8RxDataCount=0;
	
	for(i=0;i<=8; i++)
	{
		gSPIReceiveData[i]=0;
	}	
	
	
	
    SPI0->SSR=1;
		
    SPI0->TX =0x05;
	  
		SPI0->TX =0x00;
		SPI_TRIGGER(SPI0);
				
		t1ms_3 =5;
		while(t1ms_3 );
		
		
    //PC0=1;
		SPI0->SSR=0;
    
}

void EEPROM_Write(uint16_t ROM_Address,uint8_t ROM_value )
{
	uint8_t High_bit,Low_bit;
	uint8_t i =0;
	
	High_bit=ROM_Address>>8;
	Low_bit =ROM_Address;
	
	if(High_bit==0 && Low_bit==0)
	{
		ROM_Address=ROM_Address;
	}
	
	//set EPPROM write enable
	SPI0->SSR=1;
  SPI0->TX =0x06;
	SPI_TRIGGER(SPI0);
	t1ms_3 =1;
	while(t1ms_3 );
	SPI0->SSR=0;
	//***********************************
	
	
    SPI0->SSR=1;
		
    SPI0->TX =0x02;
	  
		SPI0->TX =High_bit;
		SPI0->TX =Low_bit;
		SPI0->TX = ROM_value;
		SPI_TRIGGER(SPI0);
				
		t1ms_3 =5;
		while(t1ms_3 );
		
		
    //PC0=1;
		SPI0->SSR=0;
		t1ms_3 =5;
		while(t1ms_3 );
    
}


























