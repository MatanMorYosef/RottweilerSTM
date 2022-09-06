#include "TIM4Config.h"

void TIM4_Config(void){
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	
	TIM4->PSC = 2;
	TIM4->ARR = 42000; // APB1 timers fclk = 84MHz ARR = 84K --> TIM freq = 1Khz 
	

	TIM4->CR1 |= TIM_CR1_URS;
	TIM4->DIER |= TIM_DIER_UIE;
	
	
	TIM4->CR1 |= TIM_CR1_CEN;
	
	NVIC_EnableIRQ(TIM4_IRQn);
	
	
	
}

void TIM4_IRQHandler (void){
	
	millis++;
	TIM4->SR &= ~(TIM_SR_UIF);
	
	//GPIOB->ODR |= GPIO_ODR_OD0;
}
	