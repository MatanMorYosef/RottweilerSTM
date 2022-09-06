#ifndef __UART2_H_
#define __UART2_H_
#include "stm32f4xx.h"                  // Device header

#define buffSize			255

//char buff[buffSize];


void uart2_init(void);
void uart_set_baudrate(USART_TypeDef* USARTx, uint32_t PeriphClk, uint32_t Baudrate);
uint16_t compute_uart_baudrate(uint32_t PeriphClk, uint32_t Baudrate);
void uart2_write(int ch);
void uart2_read(void);

#endif

