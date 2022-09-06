#include "EXTI.h"

void EXTI_Config(void){
	
	/*
			define pin PD0 as input pull-up and pin PD1 as input pin
	*/
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	
	GPIOD->PUPDR &= ~GPIO_PUPDR_PUPD0 & ~GPIO_PUPDR_PUPD1;
	GPIOD->PUPDR |= GPIO_PUPDR_PUPD0_0 | GPIO_PUPDR_PUPD1_0;
	
	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PD; // select interrupt source for EXTI line 0 to PD
	
	EXTI->IMR |= EXTI_IMR_IM0;
	
	EXTI->FTSR |= EXTI_FTSR_TR0;
	
	NVIC_SetPriority(EXTI0_IRQn,1);
	
	NVIC_EnableIRQ(EXTI0_IRQn);
	
}

int checkDirection(void){
	if(GPIOD->IDR & GPIO_IDR_ID1){
		return 1;
	} else {
		return -1;
	}
}

void EXTI0_IRQHandler(void){
	//printMsg("enterd EXTI0\r\n");
	dir = checkDirection();
	
	angle += dir * dAngle;
	if(EXTI->PR & EXTI_PR_PR0){
		EXTI->PR |= EXTI_PR_PR0;
	}
	
	//printMsg("finished EXTI0\r\n");
	
}
