#include "stm32f10x.h"
#include "pwm.h"

// ���������� ��� ���������� ������

const float sin_mass[180] = {

		0.0, 0.0175, 0.0349, 0.0523, 0.0698, 0.0872, 0.1045, 0.1219, 0.1392, 0.1564,
		0.1736, 0.1908, 0.2079, 0.2250, 0.2419, 0.2588, 0.2756, 0.2924, 0.3090, 0.3256,
		0.3420, 0.3584, 0.3746, 0.3907, 0.4067, 0.4226, 0.4384, 0.4540, 0.4695, 0.4848,
		0.5000, 0.5150, 0.5299, 0.5446, 0.5592, 0.5736, 0.5878, 0.6018, 0.6157, 0.6293,
		0.6428, 0.6561, 0.6691, 0.6820, 0.6947, 0.7071, 0.7193, 0.7314, 0.7431, 0.7547,
		0.7660, 0.7771, 0.7880, 0.7986, 0.8090, 0.8192, 0.8290, 0.8387, 0.8480, 0.8572,
		0.8660, 0.8746, 0.8829, 0.8910, 0.8988, 0.9063, 0.9135, 0.9205, 0.9272, 0.9336,
		0.9397, 0.9455, 0.9511, 0.9563, 0.9613, 0.9659, 0.9703, 0.9744, 0.9781, 0.9816,
		0.9848, 0.9877, 0.9903, 0.9925, 0.9945, 0.9962, 0.9976, 0.9986, 0.9994, 0.9998,
		1.0000, 0.9998, 0.9994, 0.9986, 0.9976, 0.9962, 0.9945, 0.9925, 0.9903, 0.9877,
		0.9848, 0.9816, 0.9781, 0.9744, 0.9703, 0.9659, 0.9613, 0.9563, 0.9511, 0.9455,
		0.9397, 0.9336, 0.9272, 0.9205, 0.9135, 0.9063, 0.8988, 0.8910, 0.8829, 0.8746,
		0.8660, 0.8572, 0.8480, 0.8387, 0.8290, 0.8192, 0.8090, 0.7986, 0.7880, 0.7771,
		0.7660, 0.7547, 0.7431, 0.7314, 0.7193, 0.7071, 0.6947, 0.6820, 0.6691, 0.6561,
		0.6428, 0.6293, 0.6157, 0.6018, 0.5878, 0.5736, 0.5592, 0.5446, 0.5299, 0.5150,
		0.5000, 0.4848, 0.4695, 0.4540, 0.4384, 0.4226, 0.4067, 0.3907, 0.3746, 0.3584,
		0.3420, 0.3256, 0.3090, 0.2924, 0.2756, 0.2588, 0.2419, 0.2250, 0.2079, 0.1908,
		0.1736, 0.1564,	0.1392, 0.1219, 0.1045, 0.0872, 0.0698, 0.0523, 0.0349, 0.0175
};

unsigned short	PWM_1 = period / 2;							// �������� ��� ������� 1. ��� ������. ����� ������� �����
unsigned short	PWM_2 = period / 2;
unsigned short	PWM_3 = period / 2;

unsigned short	angle = 0;									// ������� ���� ��� ���������� ������.

unsigned short step_PWM_1 = 0;								// ������ ��������� �������. ��� ������ ��� �� 120 ��������
unsigned short step_PWM_2 = 120;
unsigned short step_PWM_3 = 240;

float factor_voltage = 1.0f;								// ����������� ��� ������� ��������� ������




// ���������� ��� PP ������ ���������� (�����������/���������� ���������������)

unsigned short PWM_PP	 		= 0;						// ������� �������� ��� ��� ���������� �������
unsigned short TMR_T 			= period;					//	4500 - 16kHz COMPL_MODE		// 360 - 100 kHz PP_MODE
															// ��� PP_MODE:  TMR_T = (72 000 / ����������) / 2
															// 				 �������(���) = 72 000/(TMR_T * 2)
unsigned short deadtime_PP 		= DEADTIME;



// ������ ����������
unsigned long count_test = 0;
int triger = 0;





unsigned short get_pwm_value()
{
	return(PWM_PP);
}

void set_pwm_value(unsigned short pwm_fb)
{
	// ��� �� ������
    TIM1->CCR1=TMR_T - pwm_fb;
    TIM1->CCR2=pwm_fb;


	// ��� ���������������� ������
	//TIM1->CCR1=pwm_fb;
}

void set_pwm_value_buck(unsigned short pwm_zpt)
{
	// ��� ���������������� ������
	TIM1->CCR2=pwm_zpt;
}

unsigned short get_deadtime_value()
{
	return(deadtime_PP);
}

void pwm_PP_stop(void)
{
	PWM_PP = 0;
}

unsigned short get_frequency_pwm()
{
	unsigned short frequency;
	frequency = 72000/(TMR_T * 2);	// ������ �������
	return(frequency);
}


void init_pwm_3phase(void)
{
	init_timer3();

    RCC->APB2ENR|= RCC_APB2ENR_TIM1EN;

    TIM1->CCMR1=TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE;

    TIM1->CCER=TIM_CCER_CC1E | TIM_CCER_CC1NE;				// ���������� ���������������� ������
    TIM1->CCER=TIM_CCER_CC2E | TIM_CCER_CC2NE;
    TIM1->CCER=TIM_CCER_CC3E | TIM_CCER_CC3NE;

    TIM1->BDTR=TIM_BDTR_MOE | DEADTIME;
    TIM1->ARR=period;										// ������
    TIM1->CR1=TIM_CR1_ARPE;
    TIM1->CR1|=TIM_CR1_CEN;
    TIM1->EGR=TIM_EGR_UG;

    TIM1->CCR1=PWM_1;										// ���������� �������� ����
    TIM1->CCR2=PWM_2;
    TIM1->CCR3=PWM_3;

}

void init_pwm_PP_mode(void)
{
	init_timer3();

    RCC->APB2ENR|= RCC_APB2ENR_TIM1EN;

    TIM1->CCMR1=TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1PE | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE;
    TIM1->CCER=TIM_CCER_CC1E | TIM_CCER_CC2E;
    TIM1->BDTR=TIM_BDTR_MOE;

    TIM1->CCR1=TMR_T - PWM_PP;
    TIM1->CCR2=PWM_PP;

    TIM1->ARR=TMR_T;

    TIM1->CR1=TIM_CR1_ARPE | TIM_CR1_CMS_1 | TIM_CR1_CMS_0;
    TIM1->CR1|=TIM_CR1_CEN;
    TIM1->EGR=TIM_EGR_UG;

}


void init_pwm_Compl(void)
{

	init_timer3();


	RCC->APB2ENR|= RCC_APB2ENR_TIM1EN;




    TIM1->CCMR1 =  TIM_CCMR1_OC1M_2 |TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE | TIM_CCMR1_OC2M_2 |TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE;
    TIM1->CCMR2 =  TIM_CCMR2_OC3M_2 |TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE;

    TIM1->CR2 = TIM_CR2_OIS1 | TIM_CR2_OIS1N;


    TIM1->CCER =TIM_CCER_CC1E | TIM_CCER_CC1NE | TIM_CCER_CC2E | TIM_CCER_CC2NE| TIM_CCER_CC3E | TIM_CCER_CC3NE;	// ���������� ���������������� ������.
   // TIM1->BDTR =TIM_BDTR_MOE | TIM_BDTR_OSSI | TIM_BDTR_OSSR | DEADTIME;
    TIM1->BDTR =TIM_BDTR_MOE |  DEADTIME;


    TIM1->ARR =TMR_T;							// ������� �������������� ������������
    TIM1->EGR =TIM_EGR_UG;						// ������� ��������� �������. ��������� ������������
    TIM1->CR1 =TIM_CR1_ARPE | TIM_CR1_CEN;		// ��������� ���������������� ����� ������ � ������� ������������, ��������� ������ ��������


}

/*
void TIM1_CC_IRQHandler (void)
{

}
*/


void init_timer3(void)						// ��������� ������� 3. ����� ��� �������� ��������
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->CR1 |= TIM_CR1_ARPE; 				//shadow
	TIM3->CR1 |= TIM_CR1_DIR;

	TIM3->PSC = 71;							// 72MHz/(PSC+1)	���� ���  - 1 ���
	TIM3->ARR = 1000;						// 1000 - 1 ��
											// 50 - 55,55 ��
											// 25 - 111,111 ��

	TIM3->DIER |= TIM_DIER_UIE;
	TIM3->CR1 |= TIM_CR1_CEN;

	NVIC_EnableIRQ (TIM3_IRQn);
}


void TIM3_IRQHandler (void)									// ���������� ������� ��� ������� ������� ������� ������
{
	unsigned short status;
	status = TIM3->SR;
	TIM3->SR = ~status;

	if(status & TIM_SR_UIF)
	{


/*
		if(count_test < 100)
		{

			if(count_test == *P_time_pulse)
			{
				TIM1->CCR1 = 0;
			}

			count_test++;
		}
		if(count_test == 100)
		{
			count_test = 0;
			TIM1->CCR1 = *P_pwm;
		}

*/

	//	test_pwm_step();									// ������ ������ ��������� �������


/*

		if (count_test < 5000)								// �������� ������ ��� ������� �� �����
		{
			count_test ++;
		}
		else
		{
			count_test = 0;

			if(triger == 0)
			{
				triger = 1;
				GPIOA->BSRR =  GPIO_BSRR_BR15;  //GPIOA.15=0
				GPIOB->BSRR =  GPIO_BSRR_BS3;  //GPIOB.3=1

			}
			else
			{
				triger = 0;
				GPIOA->BSRR =  GPIO_BSRR_BS15;  //GPIOA.15=1
				GPIOB->BSRR =  GPIO_BSRR_BR3;  //GPIOB.3=0

			}
		}

*/

	}
} // ������ TIM2_IRQHandler




void test_pwm_step(void)
{

	if(step_PWM_1 <= 180)
	{
		PWM_1 = (factor_voltage * (sin_mass[step_PWM_1] * half_period)) + half_period;
	}

	if(step_PWM_1 > 180)
	{
		PWM_1 = half_period - (factor_voltage * (sin_mass[step_PWM_1 - 180] * half_period));
	}

	if(step_PWM_1 >= 360)
	{
		step_PWM_1 = 0;
	}
	else
	{
		step_PWM_1++;
	}
/////////

	if(step_PWM_2 <= 180)
	{
		PWM_2 = (factor_voltage * (sin_mass[step_PWM_2] * half_period)) + half_period;
	}

	if(step_PWM_2 > 180)
	{
		PWM_2 = half_period - (factor_voltage * (sin_mass[step_PWM_2 - 180] * half_period));
	}

	if(step_PWM_2 >= 360)
	{
		step_PWM_2 = 0;
	}
	else
	{
		step_PWM_2++;
	}
/////////
	if(step_PWM_3 <= 180)
	{
		PWM_3 = (factor_voltage * (sin_mass[step_PWM_3] * half_period)) + half_period;
	}

	if(step_PWM_3 > 180)
	{
		PWM_3 = half_period - (factor_voltage * (sin_mass[step_PWM_3 - 180] * half_period));
	}

	if(step_PWM_3 >= 360)
	{
		step_PWM_3 = 0;
	}
	else
	{
		step_PWM_3++;
	}
/////////

}





