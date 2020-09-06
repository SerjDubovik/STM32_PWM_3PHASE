#include "stm32f10x.h"
#include "GPIO.h"


void init_GPIO(void)
{

	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   // ��������� ������������ GPIOA
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;   // ��������� ������������ GPIOB
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;   // ��������� ������������ GPIOC
	RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;   // ��������� ������������ GPIOD
	RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;	  // ��������� ������������ GPIOE


	//���������������� GPIOC.13	������� ������ �� �����
	GPIOC->CRH &= ~GPIO_CRH_MODE13;   			// �������� ������� MODE
	GPIOC->CRH &= ~GPIO_CRH_CNF13;    			// �������� ������� CNF
	GPIOC->CRH |=  GPIO_CRH_MODE13;   			// �����, 50MHz
	GPIOC->CRH &= ~GPIO_CRH_CNF13;    			// ������ ����������, ������������


	/////////////////////////
	// ���������

	  //�������� �����������
	  RCC->APB2ENR |=   RCC_APB2ENR_AFIOEN;                // ������������ �������������� ������� GPIO
	  RCC->APB2ENR |=   RCC_APB2ENR_USART1EN;              // ������������ USART1

	  //���������������� PORTA.9 ��� TX
	  GPIOA->CRH   &= ~(GPIO_CRH_MODE9 | GPIO_CRH_CNF9);   // ����������� MODE � CNF
	  GPIOA->CRH   |=   GPIO_CRH_MODE9 | GPIO_CRH_CNF9_1;  // ����������� ����� � ���������� �-��, 50MHz

	  //���������������� PORTA.10 ��� RX
	  GPIOA->CRH   &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10);   // ����������� MODE � CNF
	  GPIOA->CRH   |=   GPIO_CRH_CNF10_0;                   // ����, ������ ���������



} // �������� init_GPIO
