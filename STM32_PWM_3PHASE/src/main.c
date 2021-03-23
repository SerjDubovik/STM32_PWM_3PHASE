#include "stm32f10x.h"
#include "modbus.h"
#include "GPIO.h"
#include "pwm.h"
#include "adc.h"
#include "I2C.h"



/* LM75 defines */
	#define  LM75_ADDR1  0x90
	#define  LM75_ADDR2  0x92
	#define  LM75_ADDR3  0x94

/* LM75 registers */
#define LM75_REG_TEMP                 0x00 // Temperature
#define LM75_REG_CONF                 0x01 // Configuration
#define LM75_REG_THYS                 0x02 // Hysteresis
#define LM75_REG_TOS                  0x03 // Overtemperature shutdown


unsigned short reg_i2c		= LM75_REG_TEMP;
unsigned short addr_i2c		= LM75_ADDR1;

// i2c interrapt
char data_write[20];
char data_read[20]={0,};




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

unsigned short frequency_pwm						= 0;
unsigned short deadtime_pwm							= 0;
unsigned short set_out_voltage						= 0;

unsigned short	buf_mbus1							= 0;






int main(void)
{

//	int state = 1;										// ���������� � ����� ������ ��� ����������� �������������� �������



	init_GPIO();
    init_modbus();
    //init_pwm_3phase();
    init_pwm_PP_mode();
    //init_pwm_Compl();
    init_adc();
//    Init_I2C1();

    relay_1(off);
    relay_2(off);

    blue_led(off);
    red_led(off);

	//GPIOA->BSRR =  GPIO_BSRR_BS7;
	//GPIOA->BSRR =  GPIO_BSRR_BS8;  //GPIOA.8=1

    array_mb[0] = 0x8000;								// 15 ��� - 1. ����� ��������� ������� ������������


    array_mb[21] = threshold_U_zpt;						// ������ ��������� �������� ���������� ��� (�).
    deadtime_pwm = get_deadtime_value();				// ������� ������� (��).
    array_mb[25] = deadtime_pwm;
    frequency_pwm = get_frequency_pwm();				// �������(���) = 72 000/(TMR_T * 2)
    array_mb[23] = frequency_pwm;						// ������� ������� (���).
    //array_mb[27] = get_pwm_value();						// ������� ���.
    array_mb[30] = Isens_threshold;						// ������� ��������������� (��).
    //array_mb[32] = temperature_plate;					// ����������� ����� ���������� (�).
    array_mb[33] = temperature_plate_threshold;			// ����� ����� ���������� (�).
    //array_mb[35] = temperature_heater;					// ����������� ��������� (�).
    array_mb[36] = temperature_heater_threshold;		// ����� ��������� (�).




    data_write[0] = LM75_REG_TEMP;


	for(;;)
	{

/*
// **** ���������� ������������� �������� *****


		switch(state){
				case 1 :
							I2C1_start_RX( LM75_ADDR1, (unsigned char*) data_write, (unsigned char*) data_read, 1, 2, 0);	// ���������� ���� ������ ���
							state = 2;
							break;
				case 2:

							if(*status >20)
							{
								*status = 0;
							}

							if(*status!=0)
							{
								break;
							}
							else
							{
								state = 3;
							}


							break;
				case 3 :
							I2C1_start_RX( LM75_ADDR2, (unsigned char*) data_write, (unsigned char*) &data_read[2], 1, 2, 0);	// ���������� ���� ������ ���
							state = 4;
							break;
				case 4:
							if(*status >20)
							{
							*status = 0;
							}


							if(*status!=0)
							{
								break;
							}
							else
							{
							state = 1;
							}

							break;

				default:
							state = 1;
							break;

		};
*/

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


		buf_mbus1  = array_mb[1];									// �������� 15 ���. ���� 1 - ����� �������� ���������

		if(buf_mbus1 &= 0x8000)
		{
			relay_2(on);
		}
		else
		{
			relay_2(off);
		}


// *****************



	    if(array_mb[2] == 10)										// ����� ���
	    {
	    	array_mb[21] = array_mb[22];
	    	threshold_U_zpt = array_mb[21];
	    	array_mb[3] = 1;
	    }


	    if(array_mb[2] == 11)										// ������ ������� ���������� ���������
	    {
	    	array_mb[23] = array_mb[24];
	    	frequency_pwm = array_mb[23];
	    	array_mb[3] = 1;
	    }


	    if(array_mb[2] == 12)										// ����� �������
	    {
	    	array_mb[25] = array_mb[26];
	    	deadtime_pwm = array_mb[25];
	    	array_mb[3] = 1;
	    }


	    if(array_mb[2] == 13)										// ����� �������� ����������
	    {
	    	set_out_voltage = array_mb[29];
	    	array_mb[3] = 1;
	    }


	    if(array_mb[2] == 14)										// ����� ��������������� ������
	    {
	    	array_mb[30] = array_mb[31];
	    	Isens_threshold = array_mb[30];
	    	array_mb[3] = 1;
	    }


	    if(array_mb[2] == 15)										// ����� ����� ����������� �����
	    {
	    	array_mb[33] = array_mb[34];
	    	temperature_plate_threshold = array_mb[33];
	    	array_mb[3] = 1;
	    }


	    if(array_mb[2] == 16)										// ����� ����� ����������� ���������
	    {
	    	array_mb[36] = array_mb[37];
	    	temperature_heater_threshold = array_mb[36];
	    	array_mb[3] = 1;
	    }


	    if(array_mb[2] == 17)										// ����� ������� ��� � ������ ������
	    {
	    	array_mb[27] = array_mb[28];
	    	set_pwm_value(array_mb[27]);
	    	//pwm = array_mb[27];
	    	array_mb[3] = 1;
	    }


	    if(array_mb[2] == 18)										// ����� ������� ���������� � ������ ������
	    {
	    	array_mb[50] = array_mb[51];

	    	//time_pulse = array_mb[50];
	    	array_mb[3] = 1;
	    }


	    if(array_mb[2] == 19)										// ����� ������� ���������� Buck-���������� � ������ ������
	    {
	    	array_mb[52] = array_mb[53];
	    	set_pwm_value_buck(array_mb[52]);
	    	array_mb[3] = 1;
	    }


	    if(array_mb[2] == 0)										// ������������� ����� �������. ���������� ��������� ���������
	    {
	    	array_mb[3] = 0;
	    }



		array_mb[32]	= data_read[0];
		array_mb[35]	= data_read[2];


		array_mb[8]++;

		Modbus_slave();

	} // ������ ������������ �����
} // ������ �����
