#include "stm32f10x.h"
#include "system.h"




unsigned char InitClk(void)
{
	  unsigned long int TimeOut = 10000;

	  //��������� HSE
	  RCC->CR   |=  RCC_CR_HSEON;            //�������� ��������� HSE
	  while((RCC->CR & RCC_CR_HSERDY)==0)    //�������� ���������� HSE
	  if(TimeOut) TimeOut--;
	  if(TimeOut==0) return 1;               //������!!! ��������� HSE �� ����������
	  RCC->CR   |=  RCC_CR_CSSON;            //��������� ������ ������� ������ ���� HSE

	  RCC->CFGR &= ~RCC_CFGR_PLLXTPRE;       //�� ������������ �������� HSE

	  //�������  SystemCoreClock ���� 24 MHz - ��������� ����� ��������������� ������� FLASH
	  FLASH->ACR|=  FLASH_ACR_PRFTBE;        //�������� ����� ��������������� �������
	  FLASH->ACR&= ~FLASH_ACR_LATENCY;       //�������� FLASH_ACR_LATENCY
	  FLASH->ACR |= FLASH_ACR_LATENCY_2;     //���������� 2 �����

	  //��������� PLL
	  RCC->CFGR  |= RCC_CFGR_PLLSRC;         //���������� ������� ��� PLL ������ HSE
	  RCC->CR   &= ~RCC_CR_PLLON;            //��������� ��������� PLL
	  RCC->CFGR &= ~RCC_CFGR_PLLMULL;        //�������� PLLMULL
	  RCC->CFGR |=  RCC_CFGR_PLLMULL9;       //���������� ��������� = 9
	  RCC->CR   |=  RCC_CR_PLLON;            //�������� ��������� PLL
	  while((RCC->CR & RCC_CR_PLLRDY)==0) {} //�������� ���������� PLL

	  //������������� �� ������������ �� PLL
	  RCC->CFGR &= ~RCC_CFGR_SW;             //������� ����� ������ ��������� ��������� �������
	  RCC->CFGR |=  RCC_CFGR_SW_PLL;         //������� ���������� ��������� ������� PLL
	  while((RCC->CFGR&RCC_CFGR_SWS)!=0x08){}//�������� ������������ �� PLL

	  //��������� �������� ��� ���� APB1
	  RCC->CFGR &= ~RCC_CFGR_PPRE1;          //������� ����� ������������ "APB1 Prescaler"
	  RCC->CFGR |=  RCC_CFGR_PPRE1_DIV2;     //���������� "APB1 Prescaler" ������ 2
	/*
	  //��������� �������� ��� ADC
	  RCC->CFGR &= ~RCC_CFGR_ADCPRE;             //������� ����� ������������ "ADC Prescaler"
	  RCC->CFGR |=  RCC_CFGR_ADCPRE_DIV6;        //���������� "ADC Prescaler" ������ 6
	*/
	  return 0;                              //��� ok, �������� �� HSE
}


void NMI_Handler(void) 						//Function: ���������� ���������� ��� ���� ���������� HSE
{
  //�������� ���� ������� �������� ���� HSE
  if (RCC->CIR & RCC_CIR_CSSF) RCC->CIR |= RCC_CIR_CSSC;  

  //���� ���������� �����, ������ HSE �� ��������
  //���-�� ����� �����������: ������������� ���������, ���� ������ ����� � �.�.
}


