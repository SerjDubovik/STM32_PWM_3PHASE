#include "stm32f10x.h"
#include "modbus.h"
#include "GPIO.h"
#include "pwm.h"
#include "adc.h"



unsigned short	array_mb[100]			= {};						// ������ ��� �������� ��������/���������� ����

unsigned short	state_machine			= 0;						// ���������� ��������� �������. ����� ��������� ������� ���������
																	// 0 - ������� ����� ������ �� ������������. �������� ���������� �� �������.
																	// 1 - ������� � ���������� ���������� ������ ������.


int main(void)
{


	init_GPIO();
    init_modbus();
    init_pwm();
    init_adc();

    relay_1(off);
    relay_2(off);



    array_mb[0] = 0x8000;											// 15 ��� - 1. ����� ��������� ������� ������������

	for(;;)
	{


		switch (state_machine)
		{

			case 0:
			{
				if(array_mb[2] == 1)
				{
					array_mb[2] = 0;
				}

				if(array_mb[2] == 2)
				{
					array_mb[2] = 0;
				}

				if(array_mb[2] == 3)
				{
					array_mb[2] = 0;
				}
			}
			break;

			default:
			break;
		} // ������ �����



		array_mb[8]++;

		Modbus_slave();

	} // ������ ������������ �����
} // ������ �����
