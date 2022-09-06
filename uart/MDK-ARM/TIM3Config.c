#include "TIM3Config.h"

void TIM3_Config(void){
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	
	TIM3->PSC = 839;
	TIM3->ARR = 1000; // APB1 timers fclk = 84MHz ARR = 1000 PSC = 840 --> TIM freq = 100hz 
	

	TIM3->CR1 |= TIM_CR1_URS;
	TIM3->DIER |= TIM_DIER_UIE;
	
	
	TIM3->CR1 |= TIM_CR1_CEN;
	
	NVIC_EnableIRQ(TIM3_IRQn);
	
	
	
}

/*void TIM3_IRQHandler (void){
	
	printf("%lu",millis);
	
	
	TIM3->SR &= ~(TIM_SR_UIF);
	
}*/
	