
typedef struct
{
	unsigned char	Version[2];
	//CTR
	unsigned char	CTR_act;
	unsigned char	VALVE_ctr;
	unsigned char	LED_ctr;
	unsigned char	E84_ctr;
	unsigned char	MFC_val_ctr[2];
	//E84
	unsigned char	DI_status;
	unsigned char	DO_status;
	unsigned char	E84_step;
	unsigned char	ERR_code;
	unsigned char	FOUP_flag;
	//5ADIO
	unsigned char	EMO_MFC_sts;
	unsigned char	FOUP_status;
	unsigned char	VALVE_sts;
	unsigned char	LED_sts;
	unsigned char	E84_sts;
	unsigned char	Humidity[2];
	unsigned char	Temperature[2];
	unsigned char	InletPressure[2];
	unsigned char	OutletFlow[2];
	unsigned char	MFC_value[2];
	unsigned char	O2Meter[2];
	//Reader
	unsigned char	Tag_status;
	unsigned char	Tag_value[16];
	//Other
	unsigned char	RFID_retry_count;
	unsigned int	humi_val_i;
	unsigned int	temp_val_i;
	unsigned int	ADC_sum[3];
	//E84 timeout
	unsigned char	E84_TP1_time;
	unsigned char	E84_TP2_time;
	unsigned char	E84_TP3_time;
	unsigned char	E84_TP4_time;
	unsigned char	E84_TP5_time;
	//link timeout
	unsigned char	Link_time;
}HR_struct;

typedef struct
{
	unsigned char	RxBuf[259];    // Received data for SCI-A
	unsigned int	TxLength;
	unsigned int	TxCount;
	unsigned char	RTS_OnTmr;
	unsigned char	RTS_OffTmr;
	unsigned char	Rx_Dly_Time;
	unsigned char	Rx_Dly_Ram;
	unsigned int	Rx_Map_Byte;
	unsigned int	Rx_count;	
}SCI_struct;

