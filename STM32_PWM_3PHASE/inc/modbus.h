#ifndef SRC_MODBUS
#define SRC_MODBUS

#define adrr_devise 					81 // 2				// ����� ����� ���������� � ���� ������
#define leth_array						100					// ������ ������� ��� ��������/����������� ������. ������ ��� �������� ����������� ������ �� ������






	unsigned short CRC16(int *puchMsg,unsigned short usDataLen );
	extern void USART2_IRQHandler(void);
	void Modbus_slave(void);
	void init_modbus(void);
	void init_timer2(void);
	void TIM2_IRQHandler (void);


	/*
	 *  �������� ������������ � ��� �� ��� ��������:
	 *
	 *  ����� 			��������
	 *
	 *  0				������� ����� ��������� ������. ������ ������, �� �� ����� ����.
	 *  1				������� ����� ��� ���������� �������. ������ ����� ����, ����� �� ������ ��� ����������.
	 *  2				�����-������� ��� ���������� �������. ������ �����, ����� ������ � ���������� � ����.
	 *  3				�����-������� ��� �������� ��������� �������. ������ ����� ��������� ������������ ���������� � ����.
	 *
	 *  8				�������, ��������� �������������, �������� �� ���� �������������. ������ ��� �������, ���������� ��� �� ��� ��������.
	 *
	 *	9               Isens - ���� � �������� ������� GPIOA.0. ����������, ������ ������.
	 *	10              Uzpt - ����, ���������� ����� ����������� ���� GPIOA.4. ����������, ������ ������.
	 *	11              Un 	- �������� ����������. �������� ����� GPIOA.5. ����������, ������ ������.
	 *	12              Ibreak - ��� �������. �������� �������. GPIOA.6. ����������, ������ ������.
	 *
	 *	13				Isens	�������� ��� ��� ���������
	 *	14				Uzpt	�������� ��� ��� ���������
	 *	15				Un		�������� ��� ��� ���������
	 *	16				Ibreak	�������� ��� ��� ���������
	 *
	 *	17				����������� ��� ��������� �� ��� � ������ ���� ������
	 *	18				����������� ��� ��������� �� ��� � ����� ���
	 *	19				����������� ��� ��������� �� ��� � ����� ������
	 *	20				����������� ��� ��������� �� ��� � ������ �� �����
	 *
	 *	21				������ ��������� �������� ���������� ���. ����� ������������ ���� ���. ����������, ������ �� ������ ��� ��������
	 *	22				������ ��������� �������� ���������� ���. ����� ������������ ���� ���. ������� ��������, ����� �� ����� ��� ��������
	 *
	 */


	/*
	 *  ������� ����� ����� ��������� ������. ����� 0
	 *
	 *  ���				��������
	 *
	 *  0
	 *  1
	 *  2
	 *  3
	 *
	 *  4
	 *  5
	 *  6
	 *  7
	 *
	 *  8
	 *  9
	 *  10
	 *  11
	 *
	 *  12				1 - ����� ��� �������� ��������, 0 - ����� ��������/������� ����� ����������.
	 *  13				1 - ����� ������������ ���������� ������.
	 *  14				1 - ����� ���� ��� ��������.
	 *  15				1 - ����� ��������� ������� ������������. �������� ��� ������ ����������� ����� ���� �������������.
	 *
	 */

	/*
	 *  ������� ����� ����� ���������� ������. ����� 1
	 *
	 *  ���				��������
	 *
	 *  0
	 *  1
	 *  2
	 *  3
	 *
	 *  4
	 *  5
	 *  6
	 *  7
	 *
	 *  8
	 *  9
	 *  10
	 *  11
	 *
	 *  12				1 - ������������ ���������� ������ ���. 0 - ������� �� ������ ���������� ���.
	 *  13				1 - ���������� ���� ��� �������������
	 *  14				1 - �������� ���� ��� �������������
	 *  15
	 */



#endif // SRC_MODBUS
