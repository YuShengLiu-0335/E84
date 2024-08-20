
//void Motor(unsigned char active);
//void Motor_LED(unsigned char y_led,unsigned char g_led);
//void Motor_Delay(unsigned int time);
void Motor_Active(unsigned int time);
void Motor_Standby(unsigned int time);
void Motor_Idle(unsigned int time);
void Motor_Off(void);
void Motor_Init(void);
void Motor_LED(unsigned char y_led,unsigned char g_led);
union uint_data {
	uint32_t time;
	struct { //big-endian 
		unsigned char sec;
		unsigned char min;
		unsigned char sec1;
		unsigned char min1;
	}T;
};
extern union uint_data MotorD, Motor_Flash;
