
#include "M0518.h"
#include "def_exram.h"
#include "def_const.h"
#include "def_flag.h"

void Init_data(void)
{	
	unsigned char i;
	
	SCI.TxLength = 0;
	SCI.TxCount = 0;
	SCI.RTS_OnTmr = 0;
	SCI.RTS_OffTmr = 2;
	SCI.Rx_Dly_Time = 0;
	SCI.Rx_Dly_Ram = 0;
	SCI.Rx_Map_Byte = 0;
	SCI.Rx_count = 0;	

	SCI1.TxLength = 0;
	SCI1.TxCount = 0;
	SCI1.RTS_OnTmr = 0;
	SCI1.RTS_OffTmr = 2;
	SCI1.Rx_Dly_Time = 0;
	SCI1.Rx_Dly_Ram = 0;
	SCI1.Rx_Map_Byte = 0;
	SCI1.Rx_count = 0;
	
	Flag00 = 0;
	Flag01 = 0;
	
	HR.Version[0] = 1;
	HR.Version[1] = 3;
	
	HR.CTR_act = 0;
	HR.VALVE_ctr = 0;
	HR.LED_ctr = 0;
	HR.E84_ctr = 0;
	HR.MFC_val_ctr[0] = 0;
	HR.MFC_val_ctr[1] = 0;
	
	DO_04 = 1;
	DO_05 = 1;
	E84_run_fg = 1;
	//E84
	HR.DI_status = 0;
	HR.DO_status = 0x08;
	HR.E84_step = 0;
	HR.ERR_code = 0;
	HR.FOUP_flag = 0;
	//5ADIO
	HR.EMO_MFC_sts = 0;
	HR.FOUP_status = 0;
	HR.VALVE_sts = 0;
	HR.LED_sts = 0;
	HR.E84_sts = 'O';
	HR.Humidity[0] = 0;
	HR.Humidity[1] = 0;	
	HR.Temperature[0] = 0;
	HR.Temperature[1] = 0;
	HR.InletPressure[0] = 0;
	HR.InletPressure[1] = 0;
	HR.OutletFlow[0] = 0;
	HR.OutletFlow[1] = 0;
	HR.MFC_value[0] = 0;
	HR.MFC_value[1] = 0;
	HR.O2Meter[0] = 0;
	HR.O2Meter[1] = 0;
	//Reader
	HR.Tag_status = 0;
	for(i = 0 ; i < 16 ; i++)
		HR.Tag_value[i] = 0;
	
	HR.RFID_retry_count = 0;
	HR.humi_val_i = 0;
	HR.temp_val_i = 0;
	HR.ADC_sum[0] = 0;
	HR.ADC_sum[1] = 0;
	HR.ADC_sum[2] = 0;
	//E84 timeout
	HR.E84_TP1_time = 0;
	HR.E84_TP2_time = 0;
	HR.E84_TP3_time = 0;
	HR.E84_TP4_time = 0;
	HR.E84_TP5_time = 0;
	//link timeout
	HR.Link_time = 0;
}


