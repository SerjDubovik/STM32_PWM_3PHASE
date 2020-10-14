#include "stm32f10x.h"
#include "modbus.h"


unsigned int delay_packet = 0;								// �������� ����� ��������. ����� ����� 7, ��� ������ 1,75 ��
char flag_UART_in = 0;										// ���� ������ ����� ������� �� UART
char swith_modbas = 0;										// ���������� ������������� ��� ������� �������

int 			array_modbas_in[255];						// ����� ��� ����� �� �����
int 			count_modbas_in 		= 0;				// ������� ���-�� �������� ����
int 			array_modbas_out[255];						// ����� ��� �������� �� �����
int 			count_modbas_out 		= 0;				// ������� ���-�� ���������� ����
int 			buffer_index			= 0;				// ������� ��� ��������


extern unsigned short	array_mb[leth_array];				// ������ ��� �������� ��������/���������� ����

unsigned long 	crc_calc 				= 0;				// ��������� ����������� �����
unsigned long 	adrr_var 				= 0;				// �������� ����� �� ������� ��������� �� ����. ��� ���������.
unsigned long 	quantity_byte 			= 0;				// ���-�� ���� ������� ���������� �������� ��� ������� �� ���� �� ������

unsigned int 	crc_read_low 			= 0;				// �������� ���������� ��� ��������� ����������� �����
unsigned int 	crc_read_high 			= 0;				//
unsigned int 	crc_calc_low 			= 0;				//
unsigned int 	crc_calc_high 			= 0;				//




// ����������� ����������

void TIM2_IRQHandler (void)						// ���������� ������� ��� ������ ��������
{
	unsigned short status;
	status = TIM2->SR;
	TIM2->SR = ~status;

	if(status & TIM_SR_UIF)
	{

		if(delay_packet != 0)					// ������ ������� ��� ������
		{
			delay_packet--;
		}

	}


}






void USART2_IRQHandler(void)
{
	unsigned short temp =  USART2->SR;
	USART2->SR = 0;

	 if (temp & USART_SR_RXNE)			// RXNE - ���-�� ������, ����� ��������. ������������ ��� ������ DR
		{

			array_modbas_in[(count_modbas_in & 0x0ff)] = USART2->DR;			// ���������� ���� � �������� ������ ��� �����.
			count_modbas_in++;										// ����������� ������� ���� � ������


			delay_packet = 7;					// ��������� ������ 1,75 ��, ������ ����� ���� ����� ����������� ��� ����������. ����������� ����� - ������� �������.
			flag_UART_in = 1;					// ���� ������ ����� �������

			return;

		}

	 if(temp & USART_SR_TC)
	 {
		 USART2->CR1 	&= 	~(USART_CR1_TCIE); 		// ��������� ���������� �� ���������� ��������. �����? ... (����� ������ �������� �������)

		 GPIOA->BSRR =  GPIO_BSRR_BR1;					// �������� 0 � GPIOA.1. �������

		return;
	 }

	 if (temp & USART_SR_TXE)
	 {
		 buffer_index++;								// ����������� ������


		 USART2->DR = array_modbas_out[buffer_index];   // ����� ������ �� �������.

		if(buffer_index == (count_modbas_out-1))  		// ������ ���� ������?
			{
				USART2->CR1 	&= 	~(USART_CR1_TXEIE); 	// ��������� ���������� �� ����������� - �������� ���������
				USART2->CR1 	|= 	USART_CR1_TCIE; 		// �������� ���������� �� ���������� ��������
			}
	 }
}



void init_timer2(void)						// ��������� ������� 2. ����� ��� �������� ��������
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	//TIM2->CR1 |= TIM_CR1_CKD_1;
	TIM2->CR1 |= TIM_CR1_ARPE; 				//shadow
	//TIM2->CR1 |= TIM_CR1_URS;
	TIM2->CR1 |= TIM_CR1_DIR;

	TIM2->PSC = 71;							// 72MHz/(PSC+1)	���� ���  - 1 ���
	TIM2->ARR = 249;

	//TIM2->DIER |= TIM_DIER_TIE;
	TIM2->DIER |= TIM_DIER_UIE;

	TIM2->CR1 |= TIM_CR1_CEN;

	NVIC_EnableIRQ (TIM2_IRQn);
}



void init_modbus(void)
{
	init_timer2();


	// �������� � �������� �������. (72 000 000/19200)/16 = 234.375 �������� ������� ����� 0���, � ������� 0�6. ����� BRR = 0xEA6.
	// (72 000 000 / 115 200) / 16 = 39,0625
	// 39 = 27h
	// 0.0625 * 16 = 1 (����� ��������� �� ������ � �������. �� � ���� ������ �����)
	// 27h � 1h = 271h

	/*
	 * 115200 - 0x0271
	 * 76800 - 0x03AA
	 * 57600 - 0x04E2
	 * 38400 - 0x0753
	 * 28800 - 0x09C4
	 * 19200 - 0x0EA6
	 * 14400 - 0x1388
	 * 9600 - 0x1D4C
	 * 4800 - 0x3A98
	 * 2400 - 0x7530
	 */

	// ������� ������ ������
	USART2->BRR   =   0x0139;                           // 115200
	USART2->CR1  &=  ~USART_CR1_M;                      // 8 ��� ������
	USART2->CR2  &=  ~USART_CR2_STOP;                   // ���-�� ����-���: 1

	// ���������� �������
	USART2->CR1  |=   USART_CR1_TE;                     // ��������� �����������
	USART2->CR1  |=   USART_CR1_RE;                     // ��������� ��������

	USART2->CR1  |=   USART_CR1_UE;                     // ��������� ������ USART2
	// ��������� ����������
	NVIC_EnableIRQ (USART2_IRQn);
	USART2->CR1  |=	USART_CR1_RXNEIE;				    // ���������� �� ���������� �����
	//USART2->CR1  |= 	USART_CR1_TCIE;                 // ���������� �� ���������� ��������

	__enable_irq ();


}


void Modbus_slave(void)
{
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	// ���������� �������

	if (flag_UART_in == 1)					// �������� ����� ������ �����. ������ ����� ������ ������ ����.
		{
			if (delay_packet == 0)			// �������� �� ������� �����. ���� ����� �� ����� ��� �����
			{
				flag_UART_in = 0;			// ���������� ���� ������ ������, ��������� � ������� ���������
				swith_modbas = 1;			// ���� ������� ������. ��� ������, ��������� � ��� ���� ������.
				buffer_index = 0;

			} // ������ �������� ��������


		} // ������ �������� ����� ������ �����

		switch (swith_modbas)
		{
			case 1:	//
			{

				if (array_modbas_in[0] == adrr_devise) 							// �������� ������ ����������
				{																// ���� ����� ������, �� ������� ��� � ������ ������
					crc_calc = CRC16(array_modbas_in, (count_modbas_in-2));		// ������� CRC ��������� ������

					crc_read_high = array_modbas_in[count_modbas_in-2]; 		// �������
					crc_read_low = array_modbas_in[count_modbas_in-1];			// �������

					crc_calc_low = ((crc_calc >> 8) & 0x00FF);					// ������� (count_modbas_in+2)
					crc_calc_high = (crc_calc & 0x00FF);						// ������� (count_modbas_in+1)


					if((crc_read_low == crc_calc_low)&&(crc_read_high == crc_calc_high))// �������� ������������ ����������� �����.
					{


						switch (array_modbas_in[1]) 	// ���� ���������� ����� ������� ������� � ������.
							{

								case 0x06:				// ������ �������� � ���� ������� �������� (Preset Single Register).
								{
									adrr_var = array_modbas_in[2];								// �������� ����� �� ������� � ���� ����������
									adrr_var = ((adrr_var << 8) | array_modbas_in[3]);

									if (adrr_var <= leth_array)									// �������� ����������� ������ �� ���������� ������
									{

										array_mb[adrr_var] = array_modbas_in[4];								// !!!!! �������� ��� !!!!!
										array_mb[adrr_var] = ((array_mb[adrr_var] << 8) | array_modbas_in[5]);



										for(int i = 0; i<= count_modbas_in; i++)				// ��������� ������. ��� ��� ����� ������ ���� ����� �� ��� ������� �����.
										{
											array_modbas_out[i] = array_modbas_in[i];
										}

										count_modbas_out = count_modbas_in;
										count_modbas_in = 0;
										swith_modbas = 2; 										// ����������� �� ���������� ������� �����


									} // ������ �������� ������ ����������
									else 														// ���� ����� ������� �� �������� ���������, ����� �������� ��� ������
									{
										count_modbas_in = 0;
										swith_modbas = 0;
									} // else ������

								} // ������ ������� 0�06
								break;



								case 0x10:				// ������ �������� � ��������� ��������� �������� (Preset Multiple Registers)
								{
									adrr_var = array_modbas_in[2];								// �������� ����� �� ������� � ���� ����������
									adrr_var = ((adrr_var << 8) | array_modbas_in[3]);


										if (adrr_var + (array_modbas_in[6]/2) <= leth_array)	// �������� ����������� ������ �� ���������� ������
										{

											int adrr_var_buf = adrr_var;

											for (int i = 0; array_modbas_in[6] > i ; i++)
											{
												array_mb[adrr_var_buf] = array_modbas_in[(7+i)];	//
												adrr_var_buf++;
											} // ������ �����


											array_modbas_out[0] = array_modbas_in[0];
											array_modbas_out[1] = array_modbas_in[1];
											array_modbas_out[2] = array_modbas_in[2];
											array_modbas_out[3] = array_modbas_in[3];
											array_modbas_out[4] = array_modbas_in[4];
											array_modbas_out[5] = array_modbas_in[5];


											crc_calc = CRC16(array_modbas_out, 6);				// ������� CRC ������������� ������, 3 ����� ��������� ����� � ����� ��������� , ������� �� ���-�� � ����������

											crc_calc_low = ((crc_calc >> 8) & 0x00FF);			// ������� (count_modbas_in+2)
											crc_calc_high = (crc_calc & 0x00FF);				// ������� (count_modbas_in+1)

											array_modbas_out[6] = crc_calc_high;				// ���������� ����������, ����� � ������ ��� ��������
											array_modbas_out[7] = crc_calc_low;


											count_modbas_out = 8;								// ����� ������ ����� 8 ����
											count_modbas_in = 0;
											swith_modbas = 2;
										} // ������ �������� ������ ����������
										else
										{
											count_modbas_in = 0;
											swith_modbas = 0;
										} // ������ �������� ������ ����������

								} // ������ ������� 0�10
								break;



								case 0x03:				// ������ �������� �� ���������� ��������� �������� (Read Holding Registers).
								{

									adrr_var = array_modbas_in[2];										// �������� ����� �� ������� � ���� ����������
									adrr_var = ((adrr_var << 8) | array_modbas_in[3]);

									quantity_byte = array_modbas_in[4];
									quantity_byte = ((quantity_byte << 8) | array_modbas_in[5]);		// ������ ���-�� ����, ������� ����� ���������


									if ((adrr_var <= leth_array) || ((adrr_var + quantity_byte) <= leth_array))	// �������� ����������� ������ ������ �� ���������� ������.
									{
										array_modbas_out[0] = array_modbas_in[0];						// ����� ������, ������� �������� �� ������
										array_modbas_out[1] = array_modbas_in[1];						// ��� �������, �� ������� �������� ����� (0�3 ��������� � ���� ������)
										array_modbas_out[2] = quantity_byte * 2;						// ����������� ����, ������� �������. ���������� 2 �����, ���� ��������, ������� �� ����� ����� � ���� ����� �������� ����� 255-�� ����



										for (int i = 0; i <= quantity_byte; i++, adrr_var++)
										{
											array_modbas_out[i*2+3] = ((array_mb[adrr_var] >> 8) & 0x00FF);			// ������ �� ������� �������� � ���� �� ���������� ������
											array_modbas_out[i*2+4] = (array_mb[adrr_var] & 0x00FF);
										}

										quantity_byte *= 2;
																										// � ����� �� ��� �����
										crc_calc = CRC16(array_modbas_out, (quantity_byte + 3));		// ������� CRC ������������� ������, 3 ����� ��������� ����� � ����� ��������� , ������� �� ����� � ����������

										crc_calc_low = ((crc_calc >> 8) & 0x00FF);						// ������� (count_modbas_in+2)
										crc_calc_high = (crc_calc & 0x00FF);							// ������� (count_modbas_in+1)

										array_modbas_out[(quantity_byte + 3)] = crc_calc_high;			// ���������� ����������, ����� � ������ ��� ��������
										array_modbas_out[(quantity_byte + 3)+1] = crc_calc_low;			//

										count_modbas_out = (quantity_byte + 3)+2;
										count_modbas_in = 0;
										swith_modbas = 2; 												// ����������� �� ���������� ������� �����

									} // ������ �������� ������ ����������
									else // ���� ����� ������� �� �������� ���������, ����� �������� ��� ������
									{
										count_modbas_in = 0;
										swith_modbas = 0;
									} // else ������

								} // ������ ������� 0�03
								break;



								case 0x04:				// ������ �������� �� ���������� ��������� ����� (Read Input Registers).
								{

								} // ������ ������� 0�04
								break;

							} // ������ ����� ������� ������ �� �������.




					} // ������ �������� ���������� ��������� crc

					else
					{
						swith_modbas = 0; 										// ������� � ���� ��������� ��������� ��������.
						count_modbas_in = 0;

					} // else ������ �������� ���������� ��������� crc




				} // ������ �������� ������


				else
				{
					swith_modbas = 0; 		// ���� ����� �� ���������, �� ������ ���� �� ��������� �����, ������� �� �����
					count_modbas_in = 0;
				} // else ������ �������� ������
			}	// �������� ������� �����
			break;

			case 2:
			{

				GPIOA->BSRR =  GPIO_BSRR_BS1;					// ������ �������, ��������� ����� � ��������.

				buffer_index=0;									// ���������� ������

				USART2->DR = array_modbas_out[0];				// ���������� ������ ���� �� ������� ��� ��������
				USART2->CR1 	|= 	USART_CR1_TXEIE;			// �������� ���������� �� ����������� ����


				swith_modbas = 0; 								// ���������� ���� � ������ � �� ������� ����
			}  // �������� ������� �����
			break;

			case 3: // ���� ������, ���� �������� ��� ����������� ������. ����������!!!!
			{
				//swith_modbas = 0;
			}  // �������� ������� �����
			break;

			default:
				break;
		} // �������� �����


}






// ������� ��� ������� ����������� �����
unsigned short CRC16(int *puchMsg,  /* ���������       */
                            unsigned short usDataLen /* ����� ��������� */)
{
    unsigned short crc = 0xFFFF;
    unsigned short uIndex;
    int i;
    for (uIndex = 0; uIndex < usDataLen; uIndex += 1) {
        crc ^= (unsigned short)*(puchMsg + uIndex);
        for (i = 8; i != 0; i -= 1) {
          if ((crc & 0x0001) == 0) { crc >>= 1; }
          else { crc >>= 1; crc ^= 0xA001; }			// ������� ����� �����
        }
    }
    // ������� ������� ���������� ����
 //   crc = ((crc >> 8) & 0x00FF) | ((crc << 8) & 0xFF00);
    return crc;

}







