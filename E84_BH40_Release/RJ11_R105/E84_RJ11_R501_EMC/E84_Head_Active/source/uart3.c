
#include "M0518.h"
#include "def_const.h"
#include "def_exram.h"
#include "def_flag.h"

void UART3_Init(void)
{
    CLK->APBCLK1 |= CLK_APBCLK1_UART3_EN_Msk;

    CLK->CLKSEL1 |= CLK_CLKSEL1_UART_S_Msk;
	CLK->CLKDIV &= ~CLK_CLKDIV_UART_N_Msk;
	/* Set GPA multi-function pins for UART3 RXD(PA.3) and TXD(PA.2) */
	SYS->ALT_MFP3 &= ~(SYS_ALT_MFP3_PA2_Msk | SYS_ALT_MFP3_PA3_Msk);
	SYS->ALT_MFP4 |= SYS_ALT_MFP4_PA2_Msk | SYS_ALT_MFP4_PA3_Msk;
    SYS->GPA_MFP &= ~(SYS_GPA_MFP_PA2_Msk | SYS_GPA_MFP_PA3_Msk);
    SYS->GPA_MFP |= SYS_GPA_MFP_PA3_UART3_RXD | SYS_GPA_MFP_PA2_UART3_TXD;
	
	NVIC_EnableIRQ(UART3_IRQn);
	
	UART3->FCR |= UART_FCR_TFR_Msk | UART_FCR_RFR_Msk;
	UART3->FCR &= ~(UART_FCR_RFITL_Msk | UART_FCR_RTS_TRI_LEV_Msk);
		 
	UART3->BAUD |= UART_BAUD_DIV_X_EN_Msk;
	UART3->BAUD |= UART_BAUD_DIV_X_ONE_Msk;
	UART3->IER &= ~UART_IER_THRE_IEN_Msk;
	UART3->IER |= UART_IER_RDA_IEN_Msk; 
	
		
	UART3->LCR &= ~UART_LCR_PBE_Msk;//none
	//UART3->LCR &= ~UART_LCR_SPE_Msk;//Odd
	//UART3->LCR &= ~UART_LCR_EPE_Msk;
	//UART3->LCR |= UART_LCR_PBE_Msk;
	
	
	UART3->LCR &= ~UART_LCR_NSB_Msk;//1 stop
	UART3->LCR |= UART_LCR_WLS_Msk;//8bit
	//UART3->LCR &= ~UART_LCR_WLS_Msk;//7bit
	//UART3->LCR |= 2;
	UART3->BAUD |= 22118400 / UART3_Baudrate - 2 ;
	SCI.Rx_Dly_Ram = 3;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Interrupt Handler                                                                                 	   */
/*---------------------------------------------------------------------------------------------------------*/
void UART3_IRQHandler(void)
{
	unsigned char vcr_temp;
	unsigned char i;

	if(UART3->ISR & UART_ISR_RDA_INT_Msk)
	{
      vcr_temp = UART3->DATA;
      if(Stop_Rx_Fg == 0)
      {
        SCI.Rx_count++;
        SCI.Rx_Dly_Time = SCI.Rx_Dly_Ram;
        SCI.RxBuf[SCI.Rx_Map_Byte] = vcr_temp;
        SCI.Rx_Map_Byte++;

        if(SCI.Rx_Map_Byte > 259) SCI.Rx_Map_Byte = 0;
      }
	}
	else if(UART3->ISR & UART_ISR_THRE_INT_Msk)
	{
		if(SCI.TxLength > 0)
      	{
					
        	UART3->DATA = SCI.RxBuf[SCI.TxCount];
        	SCI.TxCount++;
        	SCI.TxLength--;
      	}
      	else
      	{	
  			//Disable interrupt
			UART3->IER &= ~UART_IER_THRE_IEN_Msk;
        	SCI.RTS_OffTmr = 2;
        	SCI.Rx_count = 0;
					
      	}
	}
}

unsigned char MFC_Checksum(unsigned char chk_num)
{
	unsigned char i;
	unsigned char chk_byte = 0;
	for(i = 1 ; i < chk_num ; i++)
		chk_byte += SCI.RxBuf[i];
	//chk_byte = chk_byte % 100;
	return chk_byte;
}

unsigned char hex_asc(unsigned char hexData)
{
	char c;
	c = hexData + 0x30;
	if(c > 0x39) 
		c = c + 0x7;
	return(c);
}

void MFC_read(void)
{
	
	
	
	SCI.RxBuf[0] = 0x21;
	SCI.RxBuf[1] = 0x02;
	SCI.RxBuf[2] = 0x80;
	SCI.RxBuf[3] = 0x03;
	SCI.RxBuf[4] = 0x6A;
	SCI.RxBuf[5] = 0x01;
	SCI.RxBuf[6] = 0xA9;
	SCI.RxBuf[7] = 0x00;
	//SCI.RxBuf[8] = 0x99;
	SCI.RxBuf[8] = MFC_Checksum(8);
	
	SCI.TxLength = 9;
	RTS_HI;
	SCI.RTS_OnTmr = 2;
	
	
	
		
}

void MFC_write(void)
{
	
	SCI.RxBuf[0] = 0x21;
	SCI.RxBuf[1] = 0x02;
	SCI.RxBuf[2] = 0x81;
	SCI.RxBuf[3] = 0x05;
	SCI.RxBuf[4] = 0x69;
	SCI.RxBuf[5] = 0x01;
	SCI.RxBuf[6] = 0xA4;
	SCI.RxBuf[7] = HR.MFC_val_ctr[0];
	SCI.RxBuf[8] = HR.MFC_val_ctr[1];
	SCI.RxBuf[9] = 0x00;
	SCI.RxBuf[10] = MFC_Checksum(10);
	
	
	
	SCI.TxLength = 11;
	RTS_HI;
	SCI.RTS_OnTmr = 2;
}

void MFC_control(void)
{
	if(MFC_check_fg)
		HR.EMO_MFC_sts  |= 0x02;
	else
		HR.EMO_MFC_sts  &= 0xFD;
	
	if(MFC_ctr_fg)
		MFC_write();
	else
		MFC_read();
	
	MFC_check_fg = 1;
}

/*----------------------------------------------------------------------------*/
//uart sub
void UART3_SUB(void)
{
	unsigned char i,j,k;
	unsigned int mfc_val;
	unsigned char chk_byte = 0;
	
	MFC_check_fg = 0;
	
	//HR.MFC_value[0] = 0x00;
	//HR.MFC_value[1] = 0x40;
	
	if(MFC_ctr_fg)
		MFC_ctr_fg = 0;
	else
	{
		
		//HR.MFC_value[0] = 0x00;
		//HR.MFC_value[1] = 0x40;
		
		//06 00 02 80 05 6A 01 A9 ED 3F 00 C7
		
		if(SCI.Rx_count - 11 > 0)
		{
			i = SCI.Rx_count - 9;
			for(;i<SCI.Rx_count;i++)
			{
				chk_byte += SCI.RxBuf[i];
			}
			
			if(chk_byte == SCI.RxBuf[SCI.Rx_count])
			{
				HR.MFC_value[0] = SCI.RxBuf[SCI.Rx_count-3];
				HR.MFC_value[1] = SCI.RxBuf[SCI.Rx_count-2];
			}
			
			
			
		}
		
		
		
//		//RMS
//		//0,0.000617463,0
//		i = SCI.Rx_count - 2;
//		i--;
//		for(;i>0;i--)
//		{
//			if(SCI.RxBuf[i] == ',') break;
//		}
//		j = i;
//		i--;
//		for(;i>0;i--)
//		{
//			if(SCI.RxBuf[i] == ',') break;
//		}

//		mfc_val = 0;
//		for(k = i+1 ; k <j ; k++)
//		{
//			if(SCI.RxBuf[k] == '.') break;
//			if(SCI.RxBuf[k] >= 0x30)
//				mfc_val = mfc_val*10 + SCI.RxBuf[k] - 0x30;
//		}
//		mfc_val = mfc_val*10 + SCI.RxBuf[k+1] - 0x30;
//		
//		if(mfc_val < 2)
//			mfc_val = 0;
//		HR.MFC_value[0] = mfc_val / 256;
//		HR.MFC_value[1] = mfc_val % 256;
	}
}


