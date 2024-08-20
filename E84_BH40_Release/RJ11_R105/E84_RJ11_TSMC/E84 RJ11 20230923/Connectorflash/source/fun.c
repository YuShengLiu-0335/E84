
#include "M0518.h"
#include "def_const.h"
#include "def_exram.h"
#include "def_flag.h"


unsigned char check_sum(unsigned char chk_num)
{
	unsigned char i;
	unsigned char chk_byte = 0;
	for(i = 2 ; i < chk_num ; i++)
		chk_byte += SCI1.RxBuf[i];
	return chk_byte;
}

unsigned char hex2ascH (unsigned char hexData)
{
	char c;
	c = (hexData >> 4) + 0x30;
	if(c > 0x39) 
		c = c + 0x7;
	return(c);
}

unsigned char hex2ascL (unsigned char hexData)
{
	char c;
	c = (hexData & 0x0f) + 0x30;
	if(c > 0x39)
		c = c + 0x7;
	return(c);
}
//no need and save for AClean
void FOUP_status(void)
{
#if 0	//alantest
	unsigned char i;
	HR.FOUP_status = 0x00;

	if(IR_01 == 1)
		HR.FOUP_status  |= 0x01;
	if(IR_02 == 1)
		HR.FOUP_status  |= 0x02;
	if(IR_03 == 1)
		HR.FOUP_status  |= 0x04;
	if(IR_04 == 1)
		HR.FOUP_status  |= 0x08;
	if(IR_05 == 0)
		HR.FOUP_status  |= 0x10;
	
	if(((HR.FOUP_status & 0x07) == 0x07) && (HR.FOUP_flag == 0))
	{
		HR.FOUP_flag = 1;
		if(HR.Tag_status == 0)
			Read_tag_fg = 1;
	}
	if(((HR.FOUP_status | 0xF8) == 0xF8) && (HR.FOUP_flag == 1))
	{
		HR.FOUP_flag = 0;
		HR.RFID_retry_count = 0;
		RFID_retry_fg = 0;
		Read_tag_fg = 0;
		HR.Tag_status = 0;
		
		for(i = 0 ; i < 16 ; i++)
			HR.Tag_value[i] = 0;
	}
	//EMO
	if(EMO == 0)
		HR.EMO_MFC_sts  |= 0x01;
	else
		HR.EMO_MFC_sts  &= 0xFE;
#endif		
}

//-------------------------------------------------------------
//no need and save for AClean
void E84_DI_status(void)
{
	HR.DI_status = 0;

	if(DI_01 == 0)
		HR.DI_status  |= 0x01;
	if(DI_02 == 0)
		HR.DI_status  |= 0x02;
	if(DI_03 == 0)
		HR.DI_status  |= 0x04;
	if(DI_04 == 0)
		HR.DI_status  |= 0x08;
	if(DI_05 == 0)
		HR.DI_status  |= 0x10;
}

//actived by uart1 
void DO_Init(void)
{
	DO_01 = 0;
	DO_02 = 0;
	DO_03 = 0;
	HR.DO_status = 0x08;
	HR.E84_step = 0;
}

//DI:0 0 0 COMPT BUSY TR-REQ CS VALID
//DO:0 0 0 ES HO_AVBL READY U_REQ L_REQ
//no need and safe for AClean
void E84_Load_Unload(void)
{
	E84_DI_status();
	
	if(HR.DI_status == 0)
	{
		Flag01 = 0;
		DO_Init();					//step0
	}
	//CS 0N
	if(HR.DI_status == 0x02)
	{
		if(HR.E84_step == 0)	
			HR.E84_step = 1;		//step1
		if(HR.E84_step == 12)
			HR.E84_step = 13;		//step13
	}
	//CS VALID ON
	if(HR.DI_status == 0x03)
	{
		if((HR.DO_status & 0x04) == 0)
		{
			if(HR.E84_step < 2)
			{
				HR.E84_step = 2;		//step2
				if(HR.FOUP_flag == 0)	//Load
				{
					DO_01 = 1;
					HR.DO_status |= 0x01;
				}
				else					//Unload
				{
					DO_02 = 1;
					HR.DO_status |= 0x02;
				}
			}
			if(HR.E84_step == 2)			
			{
				HR.E84_step = 3;		//step3
				//TP1 Start
				if(E84_TP1_fg == 0)
				{
					HR.E84_TP1_time = 5;	//(5-1)*0.5s
					E84_TP1_fg = 1;
				}
			}
		}
		else
		{
			if(HR.E84_step == 8)
				HR.E84_step = 9;	//step9
			if(HR.E84_step == 11)
			{
				HR.E84_step = 12;	//step12
				//TP5 Stop
				HR.E84_TP5_time = 0;
				E84_TP5_fg = 0;
			}
		}
	}
	//CS VALID TR-REQ ON
	if(HR.DI_status == 0x07)
	{
		if(HR.E84_step == 3)
		{
			HR.E84_step = 4;			//step4
			//TP1 Stop
			HR.E84_TP1_time = 0;
			E84_TP1_fg = 0;
		}
		if((HR.DO_status & 0x04) == 0)
		{
			if(HR.E84_step == 4)
			{
				DO_03 = 1;
				HR.DO_status |= 0x04;
				HR.E84_step = 5;		//step5
				//TP2 Start
				if(E84_TP2_fg == 0)
				{
					HR.E84_TP2_time = 5;	//(5-1)*0.5s
					E84_TP2_fg = 1;
				}
			}
		}
		else
		{
			if(HR.E84_step == 7)
			{
				HR.E84_step = 8;		//step8
				//TP4 Stop
				HR.E84_TP4_time = 0;
				E84_TP4_fg = 0;
			}
		}
	}
	//CS VALID TR-REQ BUSY ON
	if(HR.DI_status == 0x0F)
	{
		if(HR.E84_step == 5)
		{
			HR.E84_step = 6;			//step6
			//TP2 Stop
			HR.E84_TP2_time = 0;
			E84_TP2_fg = 0;
			//TP3 Start
			if(E84_TP3_fg == 0)
			{
				HR.E84_TP3_time = 121;		//(121-1)*0.5s
				E84_TP3_fg = 1;
			}
		}
		if(HR.E84_step == 6)
		{
			if((HR.FOUP_flag == 1) && ((HR.DO_status & 0x01) == 0x01))	//Load finish
			{
				HR.E84_step = 7;		//step7
				//TP3 Stop
				HR.E84_TP3_time = 0;
				E84_TP3_fg = 0;
				//TP4 Start
				if(E84_TP4_fg == 0)
				{
					HR.E84_TP4_time = 121;	//(121-1)*0.5s
					E84_TP4_fg = 1;
				}			
				DO_01 = 0;
				HR.DO_status &= 0xFE;
			}
			if((HR.FOUP_flag == 0) && ((HR.DO_status & 0x02) == 0x02))	//Unload finish
			{
				HR.E84_step = 7;		//step7
				//TP3 Stop
				HR.E84_TP3_time = 0;
				E84_TP3_fg = 0;
				//TP4 Start
				if(E84_TP4_fg == 0)
				{
					HR.E84_TP4_time = 121;	//(121-1)*0.5s
					E84_TP4_fg = 1;
				}			
				DO_02 = 0;
				HR.DO_status &= 0xFD;
			}
		}
	}
	//CS VALID COMP ON
	if(HR.DI_status == 0x13)
	{
		if(HR.E84_step < 10)
		{
			HR.E84_step = 10;			//step10
			DO_03 = 0;
			HR.DO_status &= 0xFB;
		}
		if(HR.E84_step == 10)
		{
			HR.E84_step = 11;			//step11
			//TP5 Start
			if(E84_TP5_fg == 0)
			{
				HR.E84_TP5_time = 5;		//(5-1)*0.5s
				E84_TP5_fg = 1;
			}
		}
	}	
}
//no need and safe for AClean
void E84_timeout_check(void)
{
	if((E84_TP1_fg == 1) && (HR.E84_TP1_time > 0))
	{
		HR.E84_TP1_time --;
		if(HR.E84_TP1_time == 0)
		{
			E84_TP1_fg = 0;
			HR.ERR_code = 0x21;
			DO_Init();
		}
	}
	if((E84_TP2_fg == 1) && (HR.E84_TP2_time > 0))
	{
		HR.E84_TP2_time --;
		if(HR.E84_TP2_time == 0)
		{
			E84_TP2_fg = 0;
			HR.ERR_code = 0x22;
			DO_Init();
		}
	}
	if((E84_TP3_fg == 1) && (HR.E84_TP3_time > 0))
	{
		HR.E84_TP3_time --;
		if(HR.E84_TP3_time == 0)
		{
			E84_TP3_fg = 0;
			HR.ERR_code = 0x23;
			DO_Init();
		}
	}
	if((E84_TP4_fg == 1) && (HR.E84_TP4_time > 0))
	{
		HR.E84_TP4_time --;
		if(HR.E84_TP4_time == 0)
		{
			E84_TP4_fg = 0;
			HR.ERR_code = 0x24;
			DO_Init();
		}
	}
	if((E84_TP5_fg == 1) && (HR.E84_TP5_time > 0))
	{
		HR.E84_TP5_time --;
		if(HR.E84_TP5_time == 0)
		{
			E84_TP5_fg = 0;
			HR.ERR_code = 0x25;
			DO_Init();
		}
	}
}

//-------------------------------------------------------------
//actived by uart1 
void VALVE_control(void)
{
	#if 0 //alantest
	//VAL1
	if((HR.VALVE_ctr & 0x01) == 0x01)
	{
		VAL_IN = 0;
		HR.VALVE_sts |= 0x01;
	}
	else
	{
		VAL_IN = 1;
		HR.VALVE_sts &= 0xFE;
	}
	//VAL2
	if((HR.VALVE_ctr & 0x02) == 0x02)
	{
		VAL_OUT = 0;
		HR.VALVE_sts |= 0x02;
	}
	else
	{
		VAL_OUT = 1;
		HR.VALVE_sts &= 0xFD;
	}
	//VAL3
	if((HR.VALVE_ctr & 0x04) == 0x04)
	{
		VAL_VAC = 0;
		HR.VALVE_sts |= 0x04;
	}
	else
	{
		VAL_VAC = 1;
		HR.VALVE_sts &= 0xFB;
	}
	//VAL_4
	if((HR.VALVE_ctr & 0x08) == 0x08)
	{
		VAL_4 = 0;
		HR.VALVE_sts |= 0x08;
	}
	else
	{
		VAL_4 = 1;
		HR.VALVE_sts &= 0xF7;
	}
	//VAL_5
	if((HR.VALVE_ctr & 0x10) == 0x10)
	{
		VAL_5 = 0;
		HR.VALVE_sts |= 0x10;
	}
	else
	{
		VAL_5 = 1;
		HR.VALVE_sts &= 0xEF;
	}
	#endif
}

//-------------------------------------------------------------
//actived by uart1 
void LED_control(void)
{
	//LED1
	if((HR.LED_ctr & 0x01) == 0x01)
	{
		LED_IDLE = 0;
		HR.LED_sts |= 0x01;
	}
	else
	{
		LED_IDLE = 1;
		HR.LED_sts &= 0xFE;
	}
	//LED2
	if((HR.LED_ctr & 0x02) == 0x02)
	{
		LED_PURGE = 0;
		HR.LED_sts |= 0x02;
	}
	else
	{
		LED_PURGE = 1;
		HR.LED_sts &= 0xFD;
	}
	//LED3
	if((HR.LED_ctr & 0x04) == 0x04)
	{
		LED_ALARM = 0;
		HR.LED_sts |= 0x04;
	}
	else
	{
		LED_ALARM = 1;
		HR.LED_sts &= 0xFB;
	}		
}

