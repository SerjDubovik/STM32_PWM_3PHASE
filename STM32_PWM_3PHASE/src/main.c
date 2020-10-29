#include "stm32f10x.h"
#include "modbus.h"
#include "GPIO.h"
#include "pwm.h"
#include "adc.h"


#define threshold_U_zpt					200							// ����� ������������ ���������� ���� ���


unsigned short	array_mb[100]			= {};						// ������ ��� �������� ��������/���������� ����

unsigned short	state_machine			= 0;						// ���������� ��������� �������. ����� ��������� ������� ���������
																	// 0 - ������� ����� ������ �� ������������. �������� ���������� �� �������.
																	// 1 - ������� � ���������� ���������� ������ ������.

unsigned short v_threshold_U_zpt;									// ����������, ��� ������ � ������� ���������� ���
unsigned short clamp_U_zpt 				= 0;						// �������, ����� ��� ���������� ���������� ���� ��� ��� �������� ����������

unsigned short Isens;												// ���������� ����������� �����������. ��������� ����� �� ���������� �� �������� �������
unsigned short U_zpt;
unsigned short Un;
unsigned short Ibreak;



int main(void)
{


	init_GPIO();
    init_modbus();
    //init_pwm_3phase();
    init_pwm_PP_mode();
    init_adc();

    relay_1(off);
    relay_2(off);

    blue_led(off);
    red_led(off);



    array_mb[0] = 0x8000;											// 15 ��� - 1. ����� ��������� ������� ������������

    array_mb[21] 		= 	threshold_U_zpt;
    v_threshold_U_zpt 	= 	threshold_U_zpt;


	for(;;)
	{


		if((U_zpt < v_threshold_U_zpt) && (clamp_U_zpt == 0))
		{
			array_mb[0] &= ~(0x1000);								// 12 ��� - 0. ��� �������� �� ��������
			array_mb[0] &= ~(0x4000);								// 14 ��� - 0. ���� ��� ����������.
		    relay_1(off);											//
		}
		else
		{
			array_mb[0] |= 0x1000;									// 12 ��� - 1. ��� �������� ��������
			array_mb[0] |= 0x4000;									// 14 ��� - 1. ���� ��� ��������.
			relay_1(on);											// �������� ���� ���. ������� ������� ��������
			clamp_U_zpt = 1;										// ����������� �������. ������ ���� ��� �� ���������� ��� �������� �������. ������ �����
		}


		if(array_mb[1] |= 0x4000)									// �������� 14 ���. ���� 1 - �� ������������ ����� �������� ��� ��� ����������� �� ��������
		{

		}




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
