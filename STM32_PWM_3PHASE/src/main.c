#include "stm32f10x.h"
#include "modbus.h"



unsigned short	array_mb[100]			= {};				// ������ ��� �������� ��������/���������� ����

int main(void)
{


    init_modbus();

	for(;;)
	{




		array_mb[8]++;

		Modbus_slave();

	} // ������ ������������ �����
} // ������ �����