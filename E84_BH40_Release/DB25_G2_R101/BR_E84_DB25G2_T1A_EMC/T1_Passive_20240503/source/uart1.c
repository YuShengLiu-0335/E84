
#include "M0518.h"
#include "def_const.h"
#include "def_exram.h"
#include "def_flag.h"
#include "Motor.h"
#include "UserFMC.h"
#include <stdio.h>
#include "FMC.h"
#include "sys.h"
extern unsigned char hex2ascH (unsigned char hexData);
extern unsigned char hex2ascL (unsigned char hexData);
extern unsigned char check_sum(unsigned char chk_num);

extern void VALVE_control(void);
extern void LED_control(void);
extern void DO_Init(void);

void UART1_Init(void)
{
    CLK->APBCLK |= CLK_APBCLK_UART1_EN_Msk;

    CLK->CLKSEL1 |= CLK_CLKSEL1_UART_S_Msk;
	CLK->CLKDIV &= ~CLK_CLKDIV_UART_N_Msk;
	/* Set GPB multi-function pins for UART1 RXD(PB.4) and TXD(PB.5) */
    SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB4_Msk | SYS_GPB_MFP_PB5_Msk);
    SYS->GPB_MFP |= SYS_GPB_MFP_PB4_UART1_RXD | SYS_GPB_MFP_PB5_UART1_TXD;
	
	NVIC_EnableIRQ(UART1_IRQn);
	
	UART1->FCR |= UART_FCR_TFR_Msk | UART_FCR_RFR_Msk;
	UART1->FCR &= ~(UART_FCR_RFITL_Msk | UART_FCR_RTS_TRI_LEV_Msk);
    
	UART1->BAUD |= UART_BAUD_DIV_X_EN_Msk;
	UART1->BAUD |= UART_BAUD_DIV_X_ONE_Msk;
	UART1->IER &= ~UART_IER_THRE_IEN_Msk;
	UART1->IER |= UART_IER_RDA_IEN_Msk;
	
	UART1->LCR &= ~UART_LCR_PBE_Msk;
	UART1->LCR &= ~UART_LCR_NSB_Msk;
	UART1->LCR |= UART_LCR_WLS_Msk;
	
	UART1->BAUD |= 22118400 / UART1_Baudrate - 2 ;
	SCI1.Rx_Dly_Ram = 3;
}
/*---------------------------------------------------------------------------------------------------------*/
/* Interrupt Handler                                                                                 	   */
/*---------------------------------------------------------------------------------------------------------*/
void UART1_IRQHandler(void)
{
	unsigned  char   vcr_temp;

	if(UART1->ISR & UART_ISR_RDA_INT_Msk)
	{
      vcr_temp = UART1->DATA;
      if(Stop_Rx1_Fg == 0)
      {
        SCI1.Rx_count++;
        SCI1.Rx_Dly_Time = SCI1.Rx_Dly_Ram;
        SCI1.RxBuf[SCI1.Rx_Map_Byte] = vcr_temp;

        SCI1.Rx_Map_Byte++;

        if(SCI1.Rx_Map_Byte > 259) SCI1.Rx_Map_Byte = 0;
      }
	}
	else if(UART1->ISR & UART_ISR_THRE_INT_Msk)
	{
		if(SCI1.TxLength > 0)
      	{
        	UART1->DATA = SCI1.RxBuf[SCI1.TxCount];
        	SCI1.TxCount++;
        	SCI1.TxLength--;
      	}
      	else
      	{	
  		//Disable interrupt
		UART1->IER &= ~UART_IER_THRE_IEN_Msk;
        	SCI1.RTS_OffTmr = 2;
        	SCI1.Rx_count = 0;
      	}
	}
}

void cmd_Ver_sub(void)
{
	unsigned char chk_sum = 0;
	
	SCI1.RxBuf[0] = 0x03;
	SCI1.RxBuf[1] = 0x35;
	SCI1.RxBuf[5] = HR.Version[0];
	SCI1.RxBuf[6] = HR.Version[1];
	
	chk_sum = check_sum(7);
	SCI1.RxBuf[7] = hex2ascH(chk_sum);
	SCI1.RxBuf[8] = hex2ascL(chk_sum);
	
	SCI1.TxLength = 9;
	SCI1.RTS_OnTmr = 2;
}

void cmd_IR_sub(void)
{
	unsigned char i,chk_sum = 0;
	
	HR.CTR_act = SCI1.RxBuf[5];
	HR.VALVE_ctr = SCI1.RxBuf[6];
	HR.LED_ctr = SCI1.RxBuf[7];
	HR.E84_ctr = SCI1.RxBuf[8];
	HR.MFC_val_ctr[0] = SCI1.RxBuf[9];
	HR.MFC_val_ctr[1] = SCI1.RxBuf[10];
	
	//VALVE
	if((HR.CTR_act & 0x01) == 0x01)
		VALVE_control();
	//LED
	if((HR.CTR_act & 0x02) == 0x02)
		LED_control();
	//MFC
	if((HR.CTR_act & 0x04) == 0x04)
		MFC_ctr_fg = 1;
	//RFID
	if((HR.CTR_act & 0x08) == 0x08)
	{
		if((HR.FOUP_flag == 1) && (HR.Tag_status == 3) && (RFID_retry_fg == 0))
			Read_tag_fg = 1;
	}
	//E84
	if(HR.E84_ctr == 'O')
	{
		if(HR.E84_sts != 'O')
		{
			DO_04 = 1;
			E84_run_fg = 1;
			HR.E84_sts = 'O';
			HR.DO_status |= 0x08;
		}
	}
	else if(HR.E84_ctr == 'C')
	{
		if(HR.E84_sts != 'C')
		{
			DO_04 = 0;
			E84_run_fg = 0;
			HR.E84_sts = 'C';
			DO_Init();
			HR.DO_status = 0x00;
			HR.ERR_code = 0;
		}
	}
	
	SCI1.RxBuf[0] = 0x03;	
	SCI1.RxBuf[1] = 0x5A;
	//E84
	SCI1.RxBuf[5] = HR.DI_status;
	SCI1.RxBuf[6] = HR.DO_status;
	SCI1.RxBuf[7] = HR.E84_step;
	SCI1.RxBuf[8] = HR.ERR_code;
	SCI1.RxBuf[9] = HR.FOUP_flag;
	//5ADIO
	SCI1.RxBuf[10] = HR.EMO_MFC_sts;
	SCI1.RxBuf[11] = HR.FOUP_status;
	SCI1.RxBuf[12] = HR.VALVE_sts;
	SCI1.RxBuf[13] = HR.LED_sts;
	SCI1.RxBuf[14] = HR.E84_sts;
	SCI1.RxBuf[15] = HR.Humidity[0];
	SCI1.RxBuf[16] = HR.Humidity[1];	
	SCI1.RxBuf[17] = HR.Temperature[0];
	SCI1.RxBuf[18] = HR.Temperature[1];
	SCI1.RxBuf[19] = HR.InletPressure[0];
	SCI1.RxBuf[20] = HR.InletPressure[1];
	SCI1.RxBuf[21] = HR.OutletFlow[0];
	SCI1.RxBuf[22] = HR.OutletFlow[1];
	SCI1.RxBuf[23] = HR.MFC_value[0];
	SCI1.RxBuf[24] = HR.MFC_value[1];
	SCI1.RxBuf[25] = HR.O2Meter[0];
	SCI1.RxBuf[26] = HR.O2Meter[1];
	//Reader
	SCI1.RxBuf[27] = HR.Tag_status;
	for(i = 0 ; i < 16 ; i++)
	{
		if(HR.Tag_status == 1)
			SCI1.RxBuf[28 + i] = HR.Tag_value[i];
		else
			SCI1.RxBuf[28 + i] = 0;
	}
	chk_sum = check_sum(44);
	SCI1.RxBuf[44] = hex2ascH(chk_sum);
	SCI1.RxBuf[45] = hex2ascL(chk_sum);
	
	SCI1.TxLength = 46;
	SCI1.RTS_OnTmr = 2;
}

void cmd_AC_sub(void)
{
	unsigned char chk_sum = 0;
	
	Motor_Flash.T.min1 = 0x00;
	Motor_Flash.T.sec1= 0x00;
	Motor_Flash.T.min = SCI1.RxBuf[5];
	Motor_Flash.T.sec = SCI1.RxBuf[6];

	FMC_W(Motor_Flash.time);
	
	Motor_Flash.time = FMC_R();	
	
	SCI1.RxBuf[0] = 0x03;	
	SCI1.RxBuf[1] = 0x5A;
	SCI1.RxBuf[5] = Motor_Flash.T.min;
	SCI1.RxBuf[6] = Motor_Flash.T.sec;
	chk_sum = check_sum(7);
	SCI1.RxBuf[7] = hex2ascH(chk_sum);
	SCI1.RxBuf[8] = hex2ascL(chk_sum);
	SCI1.TxLength = 9;
	SCI1.RTS_OnTmr = 2;
}
void cmd_er_sub(void)
{	
	unsigned char chk_sum = 0;
	
	SCI1.RxBuf[0] = 0x03;

	SCI1.RxBuf[1] = 0x35;
	SCI1.RxBuf[5] = 'E';
	SCI1.RxBuf[6] = 'R';
	
	chk_sum = check_sum(7);
	SCI1.RxBuf[7] = hex2ascH(chk_sum);
	SCI1.RxBuf[8] = hex2ascL(chk_sum);
	
	SCI1.TxLength = 9;
	SCI1.RTS_OnTmr = 2;
}
void cmd_test_sub(void)
{
	unsigned char chk_sum = 0;
	SCI1.RxBuf[0] = 0x03;	
	SCI1.RxBuf[1] = 0x5A;
	SCI1.RxBuf[5] = Motor_Flash.T.min;
	SCI1.RxBuf[6] = Motor_Flash.T.sec;	
	chk_sum = check_sum(7);
	SCI1.RxBuf[7] = hex2ascH(chk_sum);
	SCI1.RxBuf[8] = hex2ascL(chk_sum);
	SCI1.TxLength = 9;
	SCI1.RTS_OnTmr = 2;
	HR.Link_time = 0;
}
/*----------------------------------------------------------------------------*/
//UART1
void UART1_SUB(void)
{
	unsigned char s_ret;
	//start error
	if(SCI1.RxBuf[0] != 0x02)
	{
		SCI1.Rx_count = 0;
		Stop_Rx1_Fg = 0;
		return;
	}

//cmd_lenth error	
	if(SCI1.RxBuf[4] == 0x31)
	{
		if((SCI1.Rx_count != 7) || (SCI1.RxBuf[1] != 0x33))
		{
			SCI1.Rx_count = 0;
			Stop_Rx1_Fg = 0;
			return;
		}
	}
	if(SCI1.RxBuf[4] == 0x36)
	{
		if((SCI1.Rx_count != 13) || (SCI1.RxBuf[1] != 0x39))
		{
			SCI1.Rx_count = 0;
			Stop_Rx1_Fg = 0;
			return;
		}
	}

	if(SCI1.RxBuf[4] == 0x38)
	{
		if((SCI1.Rx_count != 9) || (SCI1.RxBuf[1] != 0x35))
		{
			SCI1.Rx_count = 0;
			Stop_Rx1_Fg = 0;
			return;
		}
	}
	
//check_sum
	s_ret = check_sum(SCI1.Rx_count - 2);

	if((SCI1.RxBuf[SCI1.Rx_count - 1] == hex2ascL(s_ret)) && (SCI1.RxBuf[SCI1.Rx_count - 2] == hex2ascH(s_ret)))
	{
		switch(SCI1.RxBuf[4])
		{
			case 0x31:
				cmd_Ver_sub();
				break;
			case 0x36:
				cmd_IR_sub();
				break;
			case 0x38:
				cmd_AC_sub();
				break;	
			default:
				cmd_er_sub();
				break;
		}
		LED_LINK = 0;
		HR.Link_time = 0;
	}
	else
	{
		SCI1.Rx_count = 0;
		Stop_Rx1_Fg = 0;
	}

}


