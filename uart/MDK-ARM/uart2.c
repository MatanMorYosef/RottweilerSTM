#include "stm32f4xx.h"                  // Device header
#include "uart.h"

#define AF7_USART2											7
#define APB1_CLK												42000000
#define UART_BAUDRATE										115200

void uart2_init(void){
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	GPIOA->MODER &= ~GPIO_MODER_MODE2 & ~GPIO_MODER_MODE3;
	GPIOA->MODER |= GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1;
	
	GPIOA->AFR[0] |= (AF7_USART2 << GPIO_AFRL_AFSEL2_Pos) | (AF7_USART2 << GPIO_AFRL_AFSEL3_Pos);
	
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);
	
	USART2->CR1 = USART_CR1_TE | USART_CR1_RE;
	
	USART2->CR1 |= USART_CR1_UE;
	
	
	
}

void uart_set_baudrate(USART_TypeDef* USARTx, uint32_t PeriphClk, uint32_t Baudrate){
	
	USARTx->BRR = compute_uart_baudrate(PeriphClk,Baudrate);
	
}


uint16_t compute_uart_baudrate(uint32_t PeriphClk, uint32_t Baudrate){
	
	return ( (PeriphClk + (Baudrate/2U))/Baudrate );
}


void uart2_write(int ch){
	while(!(USART2->SR & USART_SR_TXE));
	USART2->DR = (ch & 0xFF);
}


/*void uart2_read(void){
	int len = 0;
	if(USART2->SR & USART_SR_RXNE){
		
		char uartBuffer[buffSize];
		memset(uartBuffer,'\0',buffSize);
	  static int len = 0;
		for(len=0; len < buffSize-1; len ++){
		 
			while(!(USART2->SR & USART_SR_RXNE));
		  
			uartBuffer[len] = USART2->DR;
		  
			if(uartBuffer[len]==10 || uartBuffer[len]==13) break;
			USART2->SR &= ~USART_SR_RXNE;
	  }
		memcpy(buff,uartBuffer,sizeof(uartBuffer));
		
  }
	//printf("%s\n\r",buff);
}
*/