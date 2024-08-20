
#define GPIO_PIN_DATA(port, pin)    (*((volatile uint32_t *)((GPIO_PIN_DATA_BASE+(0x40*(port))) + ((pin)<<2))))
#define PA0             GPIO_PIN_DATA(0, 0 ) /*!< Specify PA.0 Pin Data Input/Output */
#define PA1             GPIO_PIN_DATA(0, 1 ) /*!< Specify PA.1 Pin Data Input/Output */
#define PA2             GPIO_PIN_DATA(0, 2 ) /*!< Specify PA.2 Pin Data Input/Output */
#define PA3             GPIO_PIN_DATA(0, 3 ) /*!< Specify PA.3 Pin Data Input/Output */
#define PA4             GPIO_PIN_DATA(0, 4 ) /*!< Specify PA.4 Pin Data Input/Output */
#define PA5             GPIO_PIN_DATA(0, 5 ) /*!< Specify PA.5 Pin Data Input/Output */
#define PA6             GPIO_PIN_DATA(0, 6 ) /*!< Specify PA.6 Pin Data Input/Output */
#define PA7             GPIO_PIN_DATA(0, 7 ) /*!< Specify PA.7 Pin Data Input/Output */
#define PA8             GPIO_PIN_DATA(0, 8 ) /*!< Specify PA.8 Pin Data Input/Output */
#define PA9             GPIO_PIN_DATA(0, 9 ) /*!< Specify PA.9 Pin Data Input/Output */
#define PA10            GPIO_PIN_DATA(0, 10) /*!< Specify PA.10 Pin Data Input/Output */
#define PA11            GPIO_PIN_DATA(0, 11) /*!< Specify PA.11 Pin Data Input/Output */
#define PA12            GPIO_PIN_DATA(0, 12) /*!< Specify PA.12 Pin Data Input/Output */
#define PA13            GPIO_PIN_DATA(0, 13) /*!< Specify PA.13 Pin Data Input/Output */
#define PA14            GPIO_PIN_DATA(0, 14) /*!< Specify PA.14 Pin Data Input/Output */
#define PA15            GPIO_PIN_DATA(0, 15) /*!< Specify PA.15 Pin Data Input/Output */
#define PB0             GPIO_PIN_DATA(1, 0 ) /*!< Specify PB.0 Pin Data Input/Output */
#define PB1             GPIO_PIN_DATA(1, 1 ) /*!< Specify PB.1 Pin Data Input/Output */
#define PB2             GPIO_PIN_DATA(1, 2 ) /*!< Specify PB.2 Pin Data Input/Output */
#define PB3             GPIO_PIN_DATA(1, 3 ) /*!< Specify PB.3 Pin Data Input/Output */
#define PB4             GPIO_PIN_DATA(1, 4 ) /*!< Specify PB.4 Pin Data Input/Output */
#define PB5             GPIO_PIN_DATA(1, 5 ) /*!< Specify PB.5 Pin Data Input/Output */
#define PB6             GPIO_PIN_DATA(1, 6 ) /*!< Specify PB.6 Pin Data Input/Output */
#define PB7             GPIO_PIN_DATA(1, 7 ) /*!< Specify PB.7 Pin Data Input/Output */
#define PB8             GPIO_PIN_DATA(1, 8 ) /*!< Specify PB.8 Pin Data Input/Output */
#define PB9             GPIO_PIN_DATA(1, 9 ) /*!< Specify PB.9 Pin Data Input/Output */
#define PB10            GPIO_PIN_DATA(1, 10) /*!< Specify PB.10 Pin Data Input/Output */
#define PB11            GPIO_PIN_DATA(1, 11) /*!< Specify PB.11 Pin Data Input/Output */
#define PB12            GPIO_PIN_DATA(1, 12) /*!< Specify PB.12 Pin Data Input/Output */
#define PB13            GPIO_PIN_DATA(1, 13) /*!< Specify PB.13 Pin Data Input/Output */
#define PB14            GPIO_PIN_DATA(1, 14) /*!< Specify PB.14 Pin Data Input/Output */
#define PB15            GPIO_PIN_DATA(1, 15) /*!< Specify PB.15 Pin Data Input/Output */
#define PC0             GPIO_PIN_DATA(2, 0 ) /*!< Specify PC.0 Pin Data Input/Output */
#define PC1             GPIO_PIN_DATA(2, 1 ) /*!< Specify PC.1 Pin Data Input/Output */
#define PC2             GPIO_PIN_DATA(2, 2 ) /*!< Specify PC.2 Pin Data Input/Output */
#define PC3             GPIO_PIN_DATA(2, 3 ) /*!< Specify PC.3 Pin Data Input/Output */
#define PC6             GPIO_PIN_DATA(2, 6 ) /*!< Specify PC.6 Pin Data Input/Output */
#define PC7             GPIO_PIN_DATA(2, 7 ) /*!< Specify PC.7 Pin Data Input/Output */
#define PC8             GPIO_PIN_DATA(2, 8 ) /*!< Specify PC.8 Pin Data Input/Output */
#define PC9             GPIO_PIN_DATA(2, 9 ) /*!< Specify PC.9 Pin Data Input/Output */
#define PC10            GPIO_PIN_DATA(2, 10) /*!< Specify PC.10 Pin Data Input/Output */
#define PC11            GPIO_PIN_DATA(2, 11) /*!< Specify PC.11 Pin Data Input/Output */
#define PC14            GPIO_PIN_DATA(2, 14) /*!< Specify PC.14 Pin Data Input/Output */
#define PC15            GPIO_PIN_DATA(2, 15) /*!< Specify PC.15 Pin Data Input/Output */
#define PD6             GPIO_PIN_DATA(3, 6 ) /*!< Specify PD.6 Pin Data Input/Output */
#define PD7             GPIO_PIN_DATA(3, 7 ) /*!< Specify PD.7 Pin Data Input/Output */
#define PD14            GPIO_PIN_DATA(3, 14) /*!< Specify PD.14 Pin Data Input/Output */
#define PD15            GPIO_PIN_DATA(3, 15) /*!< Specify PD.15 Pin Data Input/Output */
#define PE5             GPIO_PIN_DATA(4, 5 ) /*!< Specify PE.5 Pin Data Input/Output */
#define PF0             GPIO_PIN_DATA(5, 0 ) /*!< Specify PF.0 Pin Data Input/Output */
#define PF1             GPIO_PIN_DATA(5, 1 ) /*!< Specify PF.1 Pin Data Input/Output */
#define PF4             GPIO_PIN_DATA(5, 4 ) /*!< Specify PF.4 Pin Data Input/Output */
#define PF5             GPIO_PIN_DATA(5, 5 ) /*!< Specify PF.5 Pin Data Input/Output */
#define PF6             GPIO_PIN_DATA(5, 6 ) /*!< Specify PF.6 Pin Data Input/Output */
#define PF7             GPIO_PIN_DATA(5, 7 ) /*!< Specify PF.7 Pin Data Input/Output */
#define PF8             GPIO_PIN_DATA(5, 8 ) /*!< Specify PF.8 Pin Data Input/Output */

#define RTS_LO		PA4 = 0
#define RTS_HI		PA4 = 1

//#define IR_01			PD14
//#define IR_02			PD7
//#define IR_03			PD6
#define IR_04			PB3
#define IR_05			PB2
#define EMO			PC7

#define DI_01		PA8
#define DI_02		PA9
#define DI_03		PA10
#define DI_04		PA11
#define DI_05		PF4

#define DO_01		PF5
#define DO_02		PB12
#define DO_03		PB8
#define DO_04		PF8
#define DO_05		PB15

#define LED_IDLE		PA14
#define LED_PURGE	PA13
#define LED_ALARM	PA12
#define LED_LINK		PA15

//#define VAL_IN		PC2
//#define VAL_VAC		PC1
//#define VAL_OUT		PC0

//#define VAL_4		PC3
//#define VAL_5		PD15

#define TXCT_HI 		PB0 = 1
#define TXCT_LO 		PB0 = 0
#define SCIO_GET 	PB1

#define SCK 			PA0	//SHT75 pin1
#define SDA1 			PA1	//SHT75 pin4
#define SDA2 			PA1	//SHT75 pin4

#define LED_RUN		PC6

#define UART3_Baudrate	57600
#define UART1_Baudrate	9600


#define IR1 			PD14  //Placement1
#define IR2			PD7   //Placement2  
#define KEY 			PD6   //Button
//#define Motor1 		PC2
//#define Motor2		PC1
//#define G_LED		PC3
#define Y_LED		PD15

#define ON_1			1 			
#define OFF_0		0	
#define ON_0			0  // use for val			
#define OFF_1		1  // use for val	

#define M_ON 		ON_0   
#define M_OFF           OFF_1
#define L_ON			ON_0
#define L_OFF		OFF_1

