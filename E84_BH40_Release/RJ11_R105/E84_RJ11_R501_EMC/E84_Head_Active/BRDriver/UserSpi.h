/******************************************************************************************************
 * @file     spi_handle.h
 * @version  V1.00
 * $Revision: 0.1
 * $Date: 2021/07/28 
 * @brief    M480 SPI handle header file.
 *
 * @note
 *  Brillian Confidential 
 *  (C) Copyright 2021, Brillian Network & Automation Integrated System Co.,Ltd.  All rights reserved. 
******************************************************************************************************/
#include <stdio.h>
#include "M0518.h"

#ifndef __SPI_HANDLE_H__
#define __SPI_HANDLE_H__

#define SPIBUFSIZE 64
#define SPIRXMAXCOUNT 11

#define SPI_FREQUENCY 5000000   //5 MHz

void SPI_Init(void);
void SendSpi(void);
uint8_t EEPROM_Read(uint16_t ROM_Address);
void EEPROM_ReadStatus(void );
void EEPROM_Write(uint16_t ROM_Address,uint8_t ROM_value );

#endif /* __SPI_HANDLE_H__ */

