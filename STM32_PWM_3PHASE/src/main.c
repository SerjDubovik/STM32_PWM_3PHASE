#include "stm32f10x.h"
#include "modbus.h"
#include "GPIO.h"



#define period               4500	//	4500 - 16kHz COMPL_MODE		// 360 - 100 kHz PP_MODE
#define half_period			period / 2
#define DEADTIME            	20




unsigned short	array_mb[100]			= {};				// ������ ��� �������� ��������/���������� ����


const float sin_mass[360] = {

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
		0.1736, 0.1564,	0.1392, 0.1219, 0.1045, 0.0872, 0.0698, 0.0523, 0.0349, 0.0175,
};

unsigned short	PWM_1 = period / 2;							// �������� ��� ������� 1. ��� ������. ����� ������� �����
unsigned short	PWM_2 = period / 2;
unsigned short	PWM_3 = period / 2;

unsigned short	step_PWM = 0;								// ������� ��� ��� ���������� ������.

float factor_voltage = 1.0f;										// ����������� ��� ������� ��������� ������


int main(void)
{


	init_GPIO();
    init_modbus();


    RCC->APB2ENR|= RCC_APB2ENR_TIM1EN;

    TIM1->CCMR1=TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE;

    TIM1->CCER=TIM_CCER_CC1E | TIM_CCER_CC1NE;
    TIM1->CCER=TIM_CCER_CC2E | TIM_CCER_CC2NE;
    TIM1->CCER=TIM_CCER_CC3E | TIM_CCER_CC3NE;

    TIM1->BDTR=TIM_BDTR_MOE | DEADTIME;
    TIM1->ARR=period;										// ������
    TIM1->CR1=TIM_CR1_ARPE;
    TIM1->CR1|=TIM_CR1_CEN;
    TIM1->EGR=TIM_EGR_UG;

    TIM1->CCR1=PWM_1;
    TIM1->CCR2=PWM_2;
    TIM1->CCR3=PWM_3;





	for(;;)
	{

		if(step_PWM <= 180)
		{
			PWM_1 = (factor_voltage * (sin_mass[step_PWM] * half_period)) + half_period;
			PWM_2 = (factor_voltage * (sin_mass[step_PWM + 120] * half_period)) + half_period;
			PWM_3 = (factor_voltage * (sin_mass[step_PWM + 240] * half_period)) + half_period;

			step_PWM++;
		}
		if(step_PWM > 180)
		{
			PWM_1 = (factor_voltage * (sin_mass[step_PWM] * half_period)) - half_period;
			PWM_2 = (factor_voltage * (sin_mass[(step_PWM) + 120] * half_period)) - half_period;
			PWM_3 = (factor_voltage * (sin_mass[(step_PWM) + 240] * half_period)) - half_period;

			step_PWM++;
		}
		if(step_PWM == 360)
		{
			step_PWM = 0;
		}







		array_mb[8]++;

		Modbus_slave();

	} // ������ ������������ �����
} // ������ �����
