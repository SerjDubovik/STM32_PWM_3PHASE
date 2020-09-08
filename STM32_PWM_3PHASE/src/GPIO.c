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
	/*
	// USART1
	RCC->APB2ENR |=   RCC_APB2ENR_USART1EN;              // ������������ USART1

	//���������������� PORTA.9 ��� TX
	GPIOA->CRH   &= ~(GPIO_CRH_MODE9 | GPIO_CRH_CNF9);   // ����������� MODE � CNF
	GPIOA->CRH   |=   GPIO_CRH_MODE9 | GPIO_CRH_CNF9_1;  // ����������� ����� � ���������� �-��, 50MHz

	//���������������� PORTA.10 ��� RX
	GPIOA->CRH   &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10);   // ����������� MODE � CNF
	GPIOA->CRH   |=   GPIO_CRH_CNF10_0;                   // ����, ������ ���������
	*/


	//USART2
	RCC->APB2ENR |=   RCC_APB2ENR_AFIOEN;				 // ������������ �������������� ������� GPIO
	RCC->APB1ENR |=   RCC_APB1ENR_USART2EN;              // ������������ USART2


	//���������������� PORTA.2 ��� TX
	GPIOA->CRL   &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2);   //����������� MODE � CNF
	GPIOA->CRL   |=   GPIO_CRL_MODE2 | GPIO_CRL_CNF2_1;  //����������� ����� � �������������� �-��, 50MHz

	//���������������� PORTA.3 ��� RX
	GPIOA->CRL   &= ~(GPIO_CRL_MODE3 | GPIO_CRL_CNF3);   //����������� MODE � CNF
	GPIOA->CRL   |=   GPIO_CRL_CNF3_0;                   //����, ������ ���������


	// ���������������� GPIOA.1								//���������� �����-������������
	GPIOA->CRL &= ~GPIO_CRL_MODE1;   						//�������� ������� MODE
	GPIOA->CRL &= ~GPIO_CRL_CNF1;    						//�������� ������� CNF
	GPIOA->CRL |=  GPIO_CRL_MODE1;   						//�����, 50MHz
	GPIOA->CRL &= ~GPIO_CRL_CNF1;    						//������ ����������, �����������

	// �������� 0 � GPIOA.1									// �������
	GPIOA->BSRR =  GPIO_BSRR_BR1;







} // �������� init_GPIO