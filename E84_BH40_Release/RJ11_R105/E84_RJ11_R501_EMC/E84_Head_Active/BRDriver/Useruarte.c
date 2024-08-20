/******************************************************************************************************
 * @file     Useruart.c
 * @version  V1.00
 * $Revision: 0.1
 * $Date: 2022/04/14 
 * @brief    M0518 UART handle source file.
 *
 * @note
 *  Brillian Confidential 
 *  (C) Copyright 2021, Brillian Network & Automation Integrated System Co.,Ltd.  All rights reserved. 
******************************************************************************************************/
#include <stdio.h>
#include "M0518.h"
#include "uart.h"
#include "Useruart.h"


/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
volatile uint8_t gSendData[12] = {0};
volatile uint8_t gReceiveData0[RXBUFSIZE]  = {0};
volatile uint8_t gReceiveData1[RXBUFSIZE]  = {0};
volatile uint8_t gReceiveData2[RXBUFSIZE/2]  = {0};
volatile uint8_t gReceiveData3[RXBUFSIZE/2]  = {0};
volatile uint8_t gReceiveData4[RXBUFSIZE/2]  = {0};
volatile uint8_t gReceiveData5[RXBUFSIZE/4]  = {0};

volatile uint8_t *pInlast0, *pInlast1, *pInlast2, *pInlast3, *pInlast4, *pInlast5;
volatile uint8_t *pGetlast0, *pGetlast1, *pGetlast2, *pGetlast3, *pGetlast4, *pGetlast5;

//********************************************************************************
//*                                                                              * 
//*Function Name: UART_Init                                                      *
//*                                                                              *
//*Object: The UART initialization                                               *
//*                                                                              *
//*                                                                              *
//*Parameter: None.                                                              *
//*                                                                              * 
//*                                                                              *
//*Return: None.                                                                 *
//********************************************************************************
void UART_Init(void)
{
    

    /* Set GPB multi-function pins for UART0 RXD(PB.0) and TXD(PB.1) */
    /* Set GPB multi-function pins for UART1 RXD(PB.4), TXD(PB.5), nRTS(PB.6) and nCTS(PB.7) */

    SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB4_Msk | SYS_GPB_MFP_PB5_Msk);

    SYS->GPB_MFP |= (SYS_GPB_MFP_PB4_UART1_RXD | SYS_GPB_MFP_PB5_UART1_TXD);
	
	
	
	
	  // Reset UART module 
    
    SYS_ResetModule(UART1_RST);
    
    
    //Set UART baudrate
    
	  UART_Open(UART1, 115200);
	  
	 
	   		
}


//********************************************************************************
//*                                                                              * 
//*Function Name: UART1_IRQHandler                                               *
//*                                                                              *
//*Object: ISR to handle UART Channel 1 interrupt event                          *
//*                                                                              *
//*                                                                              *
//*Parameter: None.                                                              *
//*                                                                              * 
//*                                                                              *
//*Return: None.                                                                 *
//********************************************************************************
void UART1_IRQHandler(void)
{
    RS485_HANDLE();
}

/*---------------------------------------------------------------------------------------------------------*/
/* RS485 Callback function                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void RS485_HANDLE()
{
    volatile uint32_t addr = 0;
    volatile uint32_t regRX = 0xFF;
    volatile uint32_t u32IntSts = UART1->ISR;;

    if(UART_GET_INT_FLAG(UART1, UART_ISR_RLS_INT_Msk) && UART_GET_INT_FLAG(UART1, UART_ISR_RDA_INT_Msk))      /* RLS INT & RDA INT */
    {
        if(UART_RS485_GET_ADDR_FLAG(UART1))             /* ADD_IF, RS485 mode */
        {
            addr = UART_READ(UART1);
            UART_RS485_CLEAR_ADDR_FLAG(UART1);         /* clear ADD_IF flag */
            printf("\nAddr=0x%x,Get:", addr);

#if (IS_USE_RS485NMM ==1) //RS485_NMM
            /* if address match, enable RX to receive data, otherwise to disable RX. */
            /* In NMM mode,user can decide multi-address filter. In AAD mode,only one address can set */
            if((addr == MATCH_ADDRSS1) || (addr == MATCH_ADDRSS2))
            {
                UART1->FCR &= ~ UART_FCR_RX_DIS_Msk;  /* Enable RS485 RX */
            }
            else
            {
                printf("\n");
                UART1->FCR |= UART_FCR_RX_DIS_Msk;    /* Disable RS485 RX */
                UART1->FCR |= UART_FCR_RFR_Msk;       /* Clear data from RX FIFO */
            }
#endif
        }
    }
    else if(UART_GET_INT_FLAG(UART1, UART_ISR_RDA_INT_Msk) || UART_GET_INT_FLAG(UART1, UART_ISR_TOUT_INT_Msk))       /* Rx Ready or Time-out INT*/
    {
        /* Handle received data */
        printf("%d,", UART1->RBR);
    }

    else if(UART_GET_INT_FLAG(UART1, UART_ISR_BUF_ERR_INT_Msk))       /* Buffer Error INT */
    {
        printf("\nBuffer Error...\n");
        UART_ClearIntFlag(UART1, UART_ISR_BUF_ERR_INT_Msk);
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/*  RS485 Receive Test  (IS_USE_RS485NMM: 0:AAD  1:NMM)                                                    */
/*---------------------------------------------------------------------------------------------------------*/
void RS485_9bitModeSlave()
{
    /* Set Data Format, only need parity enable whatever parity ODD/EVEN */
    UART_SetLine_Config(UART1, 0, UART_WORD_LEN_8, UART_PARITY_EVEN, UART_STOP_BIT_1);

    /* Set RTS pin active level as high level active */
    UART1->MCR &= ~UART_MCR_LEV_RTS_Msk;
    UART1->MCR |= UART_RTS_IS_HIGH_LEV_ACTIVE;

#if(IS_USE_RS485NMM == 1)
    printf("+-----------------------------------------------------------+\n");
    printf("|    Normal Multidrop Operation Mode                        |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| The function is used to test 9-bit slave mode.            |\n");
    printf("| Only Address %x and %x,data can receive                   |\n", MATCH_ADDRSS1, MATCH_ADDRSS2);
    printf("+-----------------------------------------------------------+\n");

    /* Set RX_DIS enable before set RS485-NMM mode */
    UART1->FCR |= UART_FCR_RX_DIS_Msk;

    /* Set RS485-NMM Mode */
    UART_SelectRS485Mode(UART1, UART_ALT_CSR_RS485_NMM_Msk | UART_ALT_CSR_RS485_AUD_Msk, 0);

    /* Set RS485 address detection enable */
    UART1->ALT_CSR |= UART_ALT_CSR_RS485_ADD_EN_Msk;

#else
    printf("Auto Address Match Operation Mode\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| The function is used to test 9-bit slave mode.            |\n");
    printf("|    Auto Address Match Operation Mode                      |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("|Only Address %x,data can receive                           |\n", MATCH_ADDRSS1);
    printf("+-----------------------------------------------------------+\n");

    /* Set RS485-AAD Mode and address match is 0xC0 */
    UART_SelectRS485Mode(UART1, UART_ALT_CSR_RS485_AAD_Msk | UART_ALT_CSR_RS485_AUD_Msk, MATCH_ADDRSS1);

    /* Set RS485 address detection enable */
    UART1->ALT_CSR |= UART_ALT_CSR_RS485_ADD_EN_Msk;

#endif

    /* Enable RDA\RLS\Time-out Interrupt */
    UART_ENABLE_INT(UART1, (UART_IER_RDA_IEN_Msk | UART_IER_RLS_IEN_Msk | UART_IER_TOUT_IEN_Msk));

    /* Enable UART1 interrupt */
    NVIC_EnableIRQ(UART1_IRQn);

    printf("Ready to receive data...(Press any key to stop test)\n");
    GetChar();

    /* Flush FIFO */
    while(UART_GET_RX_EMPTY(UART1) == 0)
    {
        UART_READ(UART1);
    }

    /* Disable RDA/RLS/RTO interrupt */
    UART_DISABLE_INT(UART1, (UART_IER_RDA_IEN_Msk | UART_IER_RLS_IEN_Msk | UART_IER_TOUT_IEN_Msk));

    /* Set UART Function */
    UART_Open(UART1, 115200);

    printf("\n\nEnd test\n");
}

