#include "stm32f10x.h"
#include "modbus.h"
#include "GPIO.h"
#include "pwm.h"
#include "adc.h"



unsigned short	array_mb[100]						= {};			// ������ ��� �������� ��������/���������� ����

unsigned short	state_machine						= 0;			// ���������� ��������� �������. ����� ��������� ������� ���������
																	// 0 - ������� ����� ������ �� ������������. �������� ���������� �� �������.
																	// 1 - ������� � ���������� ���������� ������ ������.

unsigned short threshold_U_zpt						= 200;			// ����������, ��� ������ � ������� ���������� ���
unsigned short clamp_U_zpt 							= 0;			// �������, ����� ��� ���������� ���������� ���� ��� ��� �������� ����������

unsigned short Isens;												// ���������� ����������� �����������. ��������� ����� �� ���������� �� �������� �������
unsigned short U_zpt;
unsigned short Un;
unsigned short Ibreak;


unsigned short Isens_threshold						= 33;
signed short temperature_plate						= 23;
signed short temperature_plate_threshold			= 60;
signed short temperature_heater						= 30;
signed short temperature_heater_threshold			= 100;

unsigned short	buf_mbus1							= 0;





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



    array_mb[0] = 0x8000;								// 15 ��� - 1. ����� ��������� ������� ������������


    array_mb[21] = threshold_U_zpt;						// ������ ��������� �������� ���������� ��� (�).
    array_mb[25] = get_deadtime_value();				// ������� ������� (��).
    array_mb[23] = 72000/(period * 2);					// ������� ������� (���).	�������(���) = 72 000/(TMR_T * 2)
    array_mb[27] = get_pwm_value();						// ������� ���.
    array_mb[30] = Isens_threshold;						// ������� ��������������� (��).
    array_mb[32] = temperature_plate;					// ����������� ����� ���������� (�).
    array_mb[33] = temperature_plate_threshold;			// ����� ����� ���������� (�).
    array_mb[35] = temperature_heater;					// ����������� ��������� (�).
    array_mb[36] = temperature_heater_threshold;		// ����� ��������� (�).







	for(;;)
	{


// **** ������ ��� *****

		buf_mbus1  = array_mb[1];

		if(buf_mbus1 &= 0x1000)											// �������� 12 ���. ���� 1 - �� ������������ ������ ���������� ������.
		{
			if((U_zpt < threshold_U_zpt) && (clamp_U_zpt == 0))
			{
				array_mb[0] &= ~(0x1000);								// 12 ��� - 0. ��� �������� �� ��������
				array_mb[0] &= ~(0x4000);								// 14 ��� - 0. ���� ��� ����������.

				relay_1(off);											//
				red_led(off);
			}
			else
			{
				array_mb[0] |= 0x1000;									// 12 ��� - 1. ��� �������� ��������
				array_mb[0] |= 0x4000;									// 14 ��� - 1. ���� ��� ��������.
				clamp_U_zpt = 1;										// ����������� �������. ������ ���� ��� �� ���������� ��� �������� �������. ������ �����

				relay_1(on);											// �������� ���� ���. ������� ������� ��������
				red_led(on);
			}
		}





		buf_mbus1  = array_mb[1];

		if((buf_mbus1 &= 0x1000) == 0)
		{


			buf_mbus1  = array_mb[1];

			if(buf_mbus1 &= 0x4000)									// �������� 14 ���. ���� 1 - �� ������������ ����� �������� ��� ��� ����������� �� ��������
			{
				array_mb[0] |= 0x1000;									// 12 ��� - 1. ��� �������� ��������
				array_mb[0] |= 0x4000;									// 14 ��� - 1. ���� ��� ��������.

				relay_1(on);
				red_led(on);
			}

			buf_mbus1  = array_mb[1];

			if(buf_mbus1 &= 0x2000)									// �������� 13 ���. ���� 1 - �� ������������ ����� ���������� ��� �������� �� �� ���)
			{
				array_mb[0] &= ~(0x1000);								// 12 ��� - 0. ��� �������� �� ��������
				array_mb[0] &= ~(0x4000);								// 14 ��� - 0. ���� ��� ����������.

				relay_1(off);
				red_led(off);
			}

		}

// *****************



	    if(array_mb[2] == 10)										// ����� ���
	    {
	    	array_mb[21] = array_mb[22];
	    	threshold_U_zpt = array_mb[21];
	    	array_mb[3] = 1;
	    }


	    if(array_mb[2] == 0)										// ������������� ����� �������. ���������� ��������� ���������
	    {
	    	array_mb[3] = 0;
	    }


		array_mb[8]++;

		Modbus_slave();

	} // ������ ������������ �����
} // ������ �����
