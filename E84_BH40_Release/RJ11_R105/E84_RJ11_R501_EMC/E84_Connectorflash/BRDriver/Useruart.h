/******************************************************************************************************
 * @file     uart_handle.h
 * @version  V1.00
 * $Revision: 0.1
 * $Date: 2021/06/21 
 * @brief    M480 UART handle header file.
 *
 * @note
 *  Brillian Confidential 
 *  (C) Copyright 2021, Brillian Network & Automation Integrated System Co.,Ltd.  All rights reserved. 
******************************************************************************************************/

#ifndef __UART_HANDLE_H__
#define __UART_HANDLE_H__

#define RXBUFSIZE   256
#define TAGBUFSIZE  136
#define MFCBUFSIZE  64
#define UARTTIMEOUT 0x80000000


#define RS485TX1_ENABLE  PA8    //For RS485_1 using


#define RXBUFSIZE   256
#define TAGBUFSIZE  136
#define MFCBUFSIZE  64
#define UARTTIMEOUT 0x80000000


/*---------------------------------------------------------------------------------------------------------*/
/* Define functions prototype                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
extern char GetChar(void);
void UART_Init(void);
uint8_t GetDataByteFromBuffer(uint8_t bufferIndex);
void Uart_Send(UART_T* uart, uint8_t uartdata);
void RS485_SwitchRx(UART_T* uart);
uint8_t GetCommandFromRS485(uint8_t upTime);
uint8_t CommandDecode(void);
void ExecuteCommand(void);
uint8_t HexToAsciiH(uint8_t hexData);
uint8_t HexToAsciiL(uint8_t hexData);
void SendIOEcho(void);
void SendACK(void);
void SendInPut(void);

//========Define ASCII=========================
#define A               								0x41
#define B               								0x42
#define C               								0x43
#define D               								0x44
#define E               								0x45
#define F               								0x46
#define G               								0x47
#define H               								0x48
#define I               								0x49
#define J               								0x4A
#define K               								0x4B
#define L               								0x4C
#define M               								0x4D
#define N               								0x4E
#define O               								0x4F
#define P               								0x50
#define Q               								0x51
#define R               								0x52
#define S               								0x53
#define T               								0x54
#define U               								0x55
#define V               								0x56
#define W               								0x57
#define X               								0x58
#define Y               								0x59
#define Z               								0x5A
//========Define ASCII=========================


#endif /* __UART_HANDLE_H__ */

