#include "stm32f10x.h"
#include "modbus.h"
#include "GPIO.h"
#include "pwm.h"






unsigned short	array_mb[100]			= {};				// ������ ��� �������� ��������/���������� ����




int main(void)
{


	init_GPIO();
    init_modbus();
    init_pwm();




	for(;;)
	{


/*
		array_mb[8]++;

		Modbus_slave();
*/
	} // ������ ������������ �����
} // ������ �����
