/*
 * My_UART.h
 *
 *  Created on: Jul 13, 2014
 *      Author: The Riddler
 */

#ifndef MY_UART_H_
#define MY_UART_H_

#include "My_Init.h"




extern void Init_UART(void);
extern void UART0IntHandler(void);
extern void UART1IntHandler(void);
extern void Display_Uart(void);
extern void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount);
extern void dec2char(int data_dec, char k);

#endif /* MY_UART_H_ */
