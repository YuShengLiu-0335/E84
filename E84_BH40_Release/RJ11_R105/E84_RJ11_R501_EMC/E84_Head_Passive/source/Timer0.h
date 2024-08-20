void Timer0_Init(void);
void Timer1_Init(void);
void Timer2_Init(void);

void Delay_10us(uint16_t t);
void Delay_1ms(uint16_t t);
void Timer0_Capture_Init(void); 

extern volatile uint16_t t1us;
extern volatile uint16_t t1ms;
//extern volatile uint16_t Delay1Ms; 

