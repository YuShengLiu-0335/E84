#include "UserE84.h"
void UserPWM_Init(void);
void PWM_Waveform(uint8_t dat, uint8_t mark);
void E84_IR_PWM_Trans(volatile union E84_Serial * E84);

//PWM pin
#define IR_OUT PB2

//waveform
#define HEADER 1
#define END 2
#define DAT 3
#define PWM_DELAY400US 4

#define FREQ_MARK_0 13900
#define FREQ_MARK_1 9600
#define DUTY_MARK_0 10
#define DUTY_MARK_1 10
//delay
#define T1 45
#define T2 13
#define T3 95

#define E84_EVEN_ODD ( E84.u16bit.sdat_0+ E84.u16bit.sdat_1+E84.u16bit.sdat_2+E84.u16bit.sdat_3+E84.u16bit.sdat_4+ E84.u16bit.sdat_5+E84.u16bit.sdat_6+E84.u16bit.sdat_7)%2

extern volatile union E84_Serial E84;
extern volatile uint8_t fgE84_IR_PWM_Trans;
