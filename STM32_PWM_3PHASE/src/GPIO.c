#include "stm32f10x.h"
#include "GPIO.h"


void init_GPIO(void)
{

	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   // Разрешить тактирование GPIOA
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;   // Разрешить тактирование GPIOB
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;   // Разрешить тактирование GPIOC
	RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;   // Разрешить тактирование GPIOD
	RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;	  // Разрешить тактирование GPIOE


	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);


	/////////////////////////
	// Переферия
	/*
	// USART1
	RCC->APB2ENR |=   RCC_APB2ENR_USART1EN;              // тактирование USART1

	//конфигурирование PORTA.9 для TX
	GPIOA->CRH   &= ~(GPIO_CRH_MODE9 | GPIO_CRH_CNF9);   // предочистка MODE и CNF
	GPIOA->CRH   |=   GPIO_CRH_MODE9 | GPIO_CRH_CNF9_1;  // двухтактный выход с альтернати ф-ей, 50MHz

	//конфигурирование PORTA.10 для RX
	GPIOA->CRH   &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10);   // предочистка MODE и CNF
	GPIOA->CRH   |=   GPIO_CRH_CNF10_0;                   // вход, третье состояние
	*/


	//USART2
	RCC->APB2ENR |=   RCC_APB2ENR_AFIOEN;				// тактирование альтернативных функций GPIO
	RCC->APB1ENR |=   RCC_APB1ENR_USART2EN;             // тактирование USART2


	AFIO->MAPR|=AFIO_MAPR_SWJ_CFG_JTAGDISABLE | AFIO_MAPR_TIM1_REMAP_PARTIALREMAP;			// отключаем JTAG. Освобождаем пины для светодиодов.
																							// ремапим пины для шим на TIM1


	//Конфигурирование GPIOA.0							// Isens - вход с токового датчика
	GPIOA->CRL &= ~GPIO_CRL_MODE0;   					// очистить разряды MODE
	GPIOA->CRL &= ~GPIO_CRL_CNF0;    					// очистить разряды CNF


	// конфигурирование GPIOA.1							// управление приёмо-передатчиком
	GPIOA->CRL &= ~GPIO_CRL_MODE1;   					// очистить разряды MODE
	GPIOA->CRL &= ~GPIO_CRL_CNF1;    					// очистить разряды CNF
	GPIOA->CRL |=  GPIO_CRL_MODE1;   					// выход, 50MHz
	GPIOA->CRL &= ~GPIO_CRL_CNF1;    					// общего назначения, симетричный

	// записать 0 в GPIOA.1								// слушаем
	GPIOA->BSRR =  GPIO_BSRR_BR1;


	//конфигурирование PORTA.2 для TX
	GPIOA->CRL   &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2);  // Предочистка MODE и CNF
	GPIOA->CRL   |=   GPIO_CRL_MODE2 | GPIO_CRL_CNF2_1; // Двухтактный выход с альтернативной ф-ей, 50MHz

	//конфигурирование PORTA.3 для RX
	GPIOA->CRL   &= ~(GPIO_CRL_MODE3 | GPIO_CRL_CNF3);  // Предочистка MODE и CNF
	GPIOA->CRL   |=   GPIO_CRL_CNF3_0;                  // Вход, третье состояние


	//Конфигурирование GPIOA.4							// Uzpt - вход, напряжение звена постоянного тока
	GPIOA->CRL &= ~GPIO_CRL_MODE4;   					// очистить разряды MODE
	GPIOA->CRL &= ~GPIO_CRL_CNF4;    					// очистить разряды CNF

	//Конфигурирование GPIOA.5							// Un - выходное напряжение. обратная связь
	GPIOA->CRL &= ~GPIO_CRL_MODE5;   					// очистить разряды MODE
	GPIOA->CRL &= ~GPIO_CRL_CNF5;    					// очистить разряды CNF

	//Конфигурирование GPIOA.6							// Ibreak - ток отсечки. защитная функция.
	GPIOA->CRL &= ~GPIO_CRL_MODE6;   					// очистить разряды MODE
	GPIOA->CRL &= ~GPIO_CRL_CNF6;    					// очистить разряды CNF


	//Конфигурирование GPIOA.7							// ch1n - выход ШИМ
	GPIOA->CRL &= ~GPIO_CRL_MODE7;   					// очистить разряды MODE
	GPIOA->CRL &= ~GPIO_CRL_CNF7;    					// очистить разряды CNF
	GPIOA->CRL |=  GPIO_CRL_MODE7;   					// выход, 50MHz
	GPIOA->CRL |=  GPIO_CRL_CNF7_1;  					// альтернативная функция, симетричный


	//Конфигурирование GPIOA.8							// ch1 - выход ШИМ
	GPIOA->CRH &= ~GPIO_CRH_MODE8;   					// очистить разряды MODE
	GPIOA->CRH &= ~GPIO_CRH_CNF8;    					// очистить разряды CNF
	GPIOA->CRH |=  GPIO_CRH_MODE8;   					// выход, 50MHz
	GPIOA->CRH |=  GPIO_CRH_CNF8_1;  					// альтернативная функция, симетричный


/*
	//Êîíôèãóðèðîâàíèå GPIOA.7
	GPIOA->CRL &= ~GPIO_CRL_MODE7;   //î÷èñòèòü ðàçðÿäû MODE
	GPIOA->CRL &= ~GPIO_CRL_CNF7;    //î÷èñòèòü ðàçðÿäû CNF
	GPIOA->CRL |=  GPIO_CRL_MODE7;   //âûõîä, 50MHz
	GPIOA->CRL &= ~GPIO_CRL_CNF7;    //îáùåãî íàçíà÷åíèÿ, ñèìåòðè÷íûé


	//Êîíôèãóðèðîâàíèå GPIOA.8
	GPIOA->CRH &= ~GPIO_CRH_MODE8;   //î÷èñòèòü ðàçðÿäû MODE
	GPIOA->CRH &= ~GPIO_CRH_CNF8;    //î÷èñòèòü ðàçðÿäû CNF
	GPIOA->CRH |=  GPIO_CRH_MODE8;   //âûõîä, 50MHz
	GPIOA->CRH &= ~GPIO_CRH_CNF8;    //îáùåãî íàçíà÷åíèÿ, ñèìåòðè÷íûé
*/

	//Конфигурирование GPIOA.9							// ch2 - выход ШИМ
	GPIOA->CRH &= ~GPIO_CRH_MODE9;   					// очистить разряды MODE
	GPIOA->CRH &= ~GPIO_CRH_CNF9;    					// очистить разряды CNF
	GPIOA->CRH |=  GPIO_CRH_MODE9;   					// выход, 50MHz
	GPIOA->CRH |=  GPIO_CRH_CNF9_1;  					// альтернативная функция, симетричный


	//Конфигурирование GPIOA.10							// ch3 - выход ШИМ
	GPIOA->CRH &= ~GPIO_CRH_MODE10;  					// очистить разряды MODE
	GPIOA->CRH &= ~GPIO_CRH_CNF10;   					// очистить разряды CNF
	GPIOA->CRH |=  GPIO_CRH_MODE10;  					// выход, 50MHz
	GPIOA->CRH |=  GPIO_CRH_CNF10_1; 					// альтернативная функция, симетричный

	//Конфигурирование GPIOA.11							// relay1 - выход управления реле 1
	GPIOA->CRH &= ~GPIO_CRH_MODE11;  					// очистить разряды MODE
	GPIOA->CRH &= ~GPIO_CRH_CNF11;   					// очистить разряды CNF
	GPIOA->CRH |=  GPIO_CRH_MODE11;  					// выход, 50MHz
	GPIOA->CRH &= ~GPIO_CRH_CNF11;   					// общего назначения, симетричный

	//Конфигурирование GPIOA.12							// relay2 - выход управления реле 2
	GPIOA->CRH &= ~GPIO_CRH_MODE12;  					// очистить разряды MODE
	GPIOA->CRH &= ~GPIO_CRH_CNF12;   					// очистить разряды CNF
	GPIOA->CRH |=  GPIO_CRH_MODE12;  					// выход, 50MHz
	GPIOA->CRH &= ~GPIO_CRH_CNF12;   					// общего назначения, симетричный

	// GPIOA.13											// swd отладка
	// GPIOA.14											// swd отладка

	//Конфигурирование GPIOA.15							// led1 - светодиод на плате.
	GPIOA->CRH &= ~GPIO_CRH_MODE15;  					// очистить разряды MODE
	GPIOA->CRH &= ~GPIO_CRH_CNF15;   					// очистить разряды CNF
	GPIOA->CRH |=  GPIO_CRH_MODE15;  					// выход, 50MHz
	GPIOA->CRH &= ~GPIO_CRH_CNF15;   					// общего назначения, симетричный

	//Конфигурирование GPIOB.0							// ch2n - выход ШИМ
	GPIOB->CRL &= ~GPIO_CRL_MODE0;   					// очистить разряды MODE
	GPIOB->CRL &= ~GPIO_CRL_CNF0;    					// очистить разряды CNF
	GPIOB->CRL |=  GPIO_CRL_MODE0;   					// выход, 50MHz
	GPIOB->CRL |=  GPIO_CRL_CNF0_1;  					// альтернативная функция, симетричный

	//Конфигурирование GPIOB.1							// ch3n - выход ШИМ
	GPIOB->CRL &= ~GPIO_CRL_MODE1;   					// очистить разряды MODE
	GPIOB->CRL &= ~GPIO_CRL_CNF1;    					// очистить разряды CNF
	GPIOB->CRL |=  GPIO_CRL_MODE1;   					// выход, 50MHz
	GPIOB->CRL |=  GPIO_CRL_CNF1_1;  					// альтернативная функция, симетричный

	//Конфигурирование GPIOB.3							// led2 - светодиод на плате.
	GPIOB->CRL &= ~GPIO_CRL_MODE3;   					// очистить разряды MODE
	GPIOB->CRL &= ~GPIO_CRL_CNF3;    					// очистить разряды CNF
	GPIOB->CRL |=  GPIO_CRL_MODE3;   					// выход, 50MHz
	GPIOB->CRL &= ~GPIO_CRL_CNF3;    					// общего назначения, симетричный

	// PB4 - Pin4
	// PB5 - Pin45

	//Конфигурирование GPIOB.6							// I2C1_SCL
	GPIOB->CRL &= ~GPIO_CRL_MODE6;   					// очистить разряды MODE
	GPIOB->CRL &= ~GPIO_CRL_CNF6;    					// очистить разряды CNF
	GPIOB->CRL |=  GPIO_CRL_MODE6;   					// выход, 50MHz
	GPIOB->CRL |=  GPIO_CRL_CNF6_1;  					// альтернативная функция, симетричный

	//Конфигурирование GPIOB.7							// I2C1_SDA
	GPIOB->CRL &= ~GPIO_CRL_MODE7;   					// очистить разряды MODE
	GPIOB->CRL &= ~GPIO_CRL_CNF7;    					// очистить разряды CNF
	GPIOB->CRL |=  GPIO_CRL_MODE7;   					// выход, 50MHz
	GPIOB->CRL |=  GPIO_CRL_CNF7_1;  					// альтернативная функция, симетричный

	// PB8 - Pin6
	// PB9 - Pin7
	// PB10 - Pin8
	// PB11 - Pin9

	//Конфигурирование GPIOB.12							// NSS
	GPIOB->CRH &= ~GPIO_CRH_MODE12;  					// очистить разряды MODE
	GPIOB->CRH &= ~GPIO_CRH_CNF12;   					// очистить разряды CNF
	GPIOB->CRH |=  GPIO_CRH_MODE12;  					// выход, 50MHz
	GPIOB->CRH |=  GPIO_CRH_CNF12_1; 					// альтернативная функция, симетричный

	//Конфигурирование GPIOB.13							// SCK
	GPIOB->CRH &= ~GPIO_CRH_MODE13;  					// очистить разряды MODE
	GPIOB->CRH &= ~GPIO_CRH_CNF13;   					// очистить разряды CNF
	GPIOB->CRH |=  GPIO_CRH_MODE13;  					// выход, 50MHz
	GPIOB->CRH |=  GPIO_CRH_CNF13_1; 					// альтернативная функция, симетричный

	//Конфигурирование GPIOB.14							// MISO
	GPIOB->CRH &= ~GPIO_CRH_MODE14;  					// очистить разряды MODE
	GPIOB->CRH &= ~GPIO_CRH_CNF14;   					// очистить разряды CNF
	GPIOB->CRH |=  GPIO_CRH_MODE14;  					// выход, 50MHz
	GPIOB->CRH |=  GPIO_CRH_CNF14_1; 					// альтернативная функция, симетричный

	//Конфигурирование GPIOB.15							// MOSI
	GPIOB->CRH &= ~GPIO_CRH_MODE15;  					// очистить разряды MODE
	GPIOB->CRH &= ~GPIO_CRH_CNF15;   					// очистить разряды CNF
	GPIOB->CRH |=  GPIO_CRH_MODE15;  					// выход, 50MHz
	GPIOB->CRH |=  GPIO_CRH_CNF15_1; 					// альтернативная функция, симетричный

	// PC13 - Pin1
	// PC14 - Pin2
	// PC15 - Pin3

} // скобочка init_GPIO






void relay_1(unsigned int set)
{
	if(set == on)
	{
		GPIOA->BSRR =  GPIO_BSRR_BS11;  				// GPIOA.11=1
	}
	if(set == off)
	{
		GPIOA->BSRR =  GPIO_BSRR_BR11;  				// GPIOA.11=0
	}
}



void relay_2(unsigned int set)
{
	if(set == on)
	{
		GPIOA->BSRR =  GPIO_BSRR_BS12;  				// GPIOA.12=1
	}
	if(set == off)
	{
		GPIOA->BSRR =  GPIO_BSRR_BR12;  				// GPIOA.12=0
	}
}


void blue_led(unsigned int set)
{
	if(set == on)
	{
		GPIOA->BSRR =  GPIO_BSRR_BS15;  //GPIOA.15=1
	}
	if(set == off)
	{
		GPIOA->BSRR =  GPIO_BSRR_BR15;  //GPIOA.15=0
	}
}


void red_led(unsigned int set)
{
	if(set == on)
	{
		GPIOB->BSRR =  GPIO_BSRR_BS3;  //GPIOB.3=1
	}
	if(set == off)
	{
		GPIOB->BSRR =  GPIO_BSRR_BR3;  //GPIOB.3=0
	}
}


