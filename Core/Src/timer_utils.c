#include "main.h"

//REFERENCE:
//rm0383-stm32f411xce-advanced-armbased-32bit-mcus-stmicroelectronics.pdf


//Set up Timer 1, the Advanced Control Timer, in order to use it with the ultra sonic Sensor
void setup_Timer1(){
	RCC->AHB1ENR	|=	RCC_AHB1ENR_GPIOAEN; 		//enable GPIOA Clock, page 117/844 reference
	GPIOA->MODER	|=	((0x1UL) << (0U));   		//set PA0 to general purpose output, page 157/844

	//configure Timer1 to generate microseconds delay
	//page 287/844 reference
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // Enable TIM1 clock, page 121/844 reference
	TIM1->PSC = 16 - 1; // 16 000 000 / 16 = 1000 000, page 306/844 reference
	TIM1->ARR = 0x0001; // 1000 000 / 1 = 1000000, page 306/844 reference
	TIM1->CNT = 0; // page 306/844 reference
	TIM1->CR1 = 1; //page 287/844 reference
}


void delaymS(uint32_t ms) //delay for certain amount in milliseconds
	{
	SysTick->LOAD=16000-1;
	SysTick->VAL=0;
	SysTick->CTRL=0x5;

	for (int i=0;i<ms;i++)
	{
		while(!(SysTick->CTRL &0x10000)){}
	}

	SysTick->CTRL=0;

}


//Delay for certain amount in microseconds
void delayuS(uint32_t us){
	for(int i =0; i<us; i++){
		while(!(TIM1->SR & 1)){

		}   /*wait for UIF set*/
		/*Page 295/844 of the reference*/

		TIM1->SR &= ~1;
	}
}

uint32_t read_echo(uint32_t timeout){
	volatile uint32_t duration = 0;

	//Wait for pulse to arrive
	while(!((GPIOA->IDR)&GPIO_IDR_ID1)){
		duration++;
		delayuS(1);
		if(duration>timeout){
			return 0;
		}
	}

	duration=0;

	//Measure pulse duration
	while((GPIOA->IDR&GPIO_IDR_ID1)){
		duration++;
		delayuS(1);
		if(duration>timeout){
			return 0;
		}
	}

	return duration;
}