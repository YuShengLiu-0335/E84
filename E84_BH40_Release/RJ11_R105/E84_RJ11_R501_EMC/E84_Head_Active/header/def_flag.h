
extern unsigned int	Flag00,Flag01;

typedef struct
{     	 
    unsigned int   b0:1; 
    unsigned int   b1:1;
    unsigned int   b2:1;
    unsigned int   b3:1;
    unsigned int   b4:1;
    unsigned int   b5:1;
    unsigned int   b6:1;
    unsigned int   b7:1;
    unsigned int   b8:1;
    unsigned int   b9:1;
    unsigned int   b10:1;
    unsigned int   b11:1;
    unsigned int   b12:1;
    unsigned int   b13:1;
    unsigned int   b14:1;
    unsigned int   b15:1;
    unsigned int   b16:1; 
    unsigned int   b17:1;
    unsigned int   b18:1;
    unsigned int   b19:1;
    unsigned int   b20:1;
    unsigned int   b21:1;
    unsigned int   b22:1;
    unsigned int   b23:1;
    unsigned int   b24:1;
    unsigned int   b25:1;
    unsigned int   b26:1;
    unsigned int   b27:1;
    unsigned int   b28:1;
    unsigned int   b29:1;
    unsigned int   b30:1;
    unsigned int   b31:1;
}FLAG_BITS;

#define     fg00_BIT	(*(FLAG_BITS *)&Flag00)

#define		ones_fg				fg00_BIT.b0
#define		t500ms_fg			fg00_BIT.b1

#define		MFC_ctr_fg			fg00_BIT.b2
#define		Read_tag_fg			fg00_BIT.b3
#define		Led_run_fg			fg00_BIT.b4
#define		sht_mode_fg			fg00_BIT.b5

#define		Stop_Rx_Fg			fg00_BIT.b6
#define		Rx_Last_Fg			fg00_BIT.b7
#define		Stop_Rx1_Fg			fg00_BIT.b8
#define		Rx1_Last_Fg			fg00_BIT.b9

#define		RFID_retry_fg		fg00_BIT.b10
#define		Read_adc_fg			fg00_BIT.b11
#define		MFC_check_fg		fg00_BIT.b12
#define		E84_run_fg			fg00_BIT.b13

#define		fg_motor_on			fg00_BIT.b14
#define		fg_motor_break		fg00_BIT.b15
#define		fg_motor_30s_fg		fg00_BIT.b16
#define		key_dn				fg00_BIT.b17
#define		key_up				fg00_BIT.b18


#define     	fg01_BIT	(*(FLAG_BITS *)&Flag01)

#define		E84_TP1_fg			fg01_BIT.b0
#define		E84_TP2_fg			fg01_BIT.b1
#define		E84_TP3_fg			fg01_BIT.b2
#define		E84_TP4_fg			fg01_BIT.b3
#define		E84_TP5_fg			fg01_BIT.b4



