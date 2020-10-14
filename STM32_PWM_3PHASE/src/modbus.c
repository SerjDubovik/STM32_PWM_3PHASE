#include "stm32f10x.h"
#include "modbus.h"


unsigned int delay_packet = 0;								// задержка между пакетами. будет равна 7, что значит 1,75 мс
char flag_UART_in = 0;										// флаг начала приёма посылки по UART
char swith_modbas = 0;										// переменная переключатель для разбора посылки

int 			array_modbas_in[255];						// буфер для приёма из уарта
int 			count_modbas_in 		= 0;				// счётчик кол-ва принятых байт
int 			array_modbas_out[255];						// буфер для передачи из уарта
int 			count_modbas_out 		= 0;				// счётчик кол-ва переданных байт
int 			buffer_index			= 0;				// счётчик для отправки


extern unsigned short	array_mb[leth_array];				// массив для хранения принятых/переданных слов

unsigned long 	crc_calc 				= 0;				// расчётная контрольная сумма
unsigned long 	adrr_var 				= 0;				// собраный адрес из массива принятого по сети. для сравнения.
unsigned long 	quantity_byte 			= 0;				// кол-во байт которое необходимо записать или считать по сети из памяти

unsigned int 	crc_read_low 			= 0;				// буферные переменные для сравнение контрольной суммы
unsigned int 	crc_read_high 			= 0;				//
unsigned int 	crc_calc_low 			= 0;				//
unsigned int 	crc_calc_high 			= 0;				//




// Обработчики прерываний

void TIM2_IRQHandler (void)						// обработчик таймера для всяких задержек
{
	unsigned short status;
	status = TIM2->SR;
	TIM2->SR = ~status;

	if(status & TIM_SR_UIF)
	{

		if(delay_packet != 0)					// отсчёт времени для слейва
		{
			delay_packet--;
		}

	}


}






void USART2_IRQHandler(void)
{
	unsigned short temp =  USART2->SR;
	USART2->SR = 0;

	 if (temp & USART_SR_RXNE)			// RXNE - что-то пришло, можно забирать. сбрасывается при чтении DR
		{

			array_modbas_in[(count_modbas_in & 0x0ff)] = USART2->DR;			// записываем байт в буферный массив для приёма.
			count_modbas_in++;										// увеличиваем счётчик байт в буфере


			delay_packet = 7;					// запускаем отсчёт 1,75 мс, каждый новый байт будет поддёргивать эту переменную. закончились байты - посылка принята.
			flag_UART_in = 1;					// флаг старта приёма посылки

			return;

		}

	 if(temp & USART_SR_TC)
	 {
		 USART2->CR1 	&= 	~(USART_CR1_TCIE); 		// выключаем прерывание по завершении передачи. Зачем? ... (злесь нужено включать приёмник)

		 GPIOA->BSRR =  GPIO_BSRR_BR1;					// записать 0 в GPIOA.1. слушаем

		return;
	 }

	 if (temp & USART_SR_TXE)
	 {
		 buffer_index++;								// Увеличиваем индекс


		 USART2->DR = array_modbas_out[buffer_index];   // Берем данные из буффера.

		if(buffer_index == (count_modbas_out-1))  		// Вывели весь буффер?
			{
				USART2->CR1 	&= 	~(USART_CR1_TXEIE); 	// Запрещаем прерывание по опустошению - передача закончена
				USART2->CR1 	|= 	USART_CR1_TCIE; 		// включаем прерывание по завершении передачи
			}
	 }
}



void init_timer2(void)						// Настройка таймера 2. Общий для отсчётов задержек
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	//TIM2->CR1 |= TIM_CR1_CKD_1;
	TIM2->CR1 |= TIM_CR1_ARPE; 				//shadow
	//TIM2->CR1 |= TIM_CR1_URS;
	TIM2->CR1 |= TIM_CR1_DIR;

	TIM2->PSC = 71;							// 72MHz/(PSC+1)	один тик  - 1 мкс
	TIM2->ARR = 249;

	//TIM2->DIER |= TIM_DIER_TIE;
	TIM2->DIER |= TIM_DIER_UIE;

	TIM2->CR1 |= TIM_CR1_CEN;

	NVIC_EnableIRQ (TIM2_IRQn);
}



void init_modbus(void)
{
	init_timer2();


	// Вычислим и настроим бодрейт. (72 000 000/19200)/16 = 234.375 Получаем старшую часть 0хЕА, а младшую 0х6. Итого BRR = 0xEA6.
	// (72 000 000 / 115 200) / 16 = 39,0625
	// 39 = 27h
	// 0.0625 * 16 = 1 (нужно округлять до целого в большую. не в этом случае канеш)
	// 27h и 1h = 271h

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

	// Задание режима работы
	USART2->BRR   =   0x0139;                           // 115200
	USART2->CR1  &=  ~USART_CR1_M;                      // 8 бит данных
	USART2->CR2  &=  ~USART_CR2_STOP;                   // кол-во стоп-бит: 1

	// Управление работой
	USART2->CR1  |=   USART_CR1_TE;                     // включение передатчика
	USART2->CR1  |=   USART_CR1_RE;                     // включение приёмника

	USART2->CR1  |=   USART_CR1_UE;                     // включение модуля USART2
	// Разрешить прерывания
	NVIC_EnableIRQ (USART2_IRQn);
	USART2->CR1  |=	USART_CR1_RXNEIE;				    // прерывание по завершению приёма
	//USART2->CR1  |= 	USART_CR1_TCIE;                 // прерывание по завершению передачи

	__enable_irq ();


}


void Modbus_slave(void)
{
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	// обработчик модбаса

	if (flag_UART_in == 1)					// проверка флага начала приёма. истина когда пришёл первый байт.
		{
			if (delay_packet == 0)			// проверка на таймаут приёма. пока время не вышло ждём байты
			{
				flag_UART_in = 0;			// сбрасываем флаг начала приема, переходим к анализу принятого
				swith_modbas = 1;			// свич разбора полётов. что пришло, правильно и как жить дальше.
				buffer_index = 0;

			} // скобка проверки таймаута


		} // скобка проверки флага начала приёма

		switch (swith_modbas)
		{
			case 1:	//
			{

				if (array_modbas_in[0] == adrr_devise) 							// проверка адреса устройства
				{																// если адрес совпал, то смотрим что в пакете дальше
					crc_calc = CRC16(array_modbas_in, (count_modbas_in-2));		// считаем CRC принятого пакета

					crc_read_high = array_modbas_in[count_modbas_in-2]; 		// старший
					crc_read_low = array_modbas_in[count_modbas_in-1];			// младший

					crc_calc_low = ((crc_calc >> 8) & 0x00FF);					// младший (count_modbas_in+2)
					crc_calc_high = (crc_calc & 0x00FF);						// старший (count_modbas_in+1)


					if((crc_read_low == crc_calc_low)&&(crc_read_high == crc_calc_high))// проверка соответствия контрольной суммы.
					{


						switch (array_modbas_in[1]) 	// свич определяет какая команда кроется в пакете.
							{

								case 0x06:				// запись значения в один регистр хранения (Preset Single Register).
								{
									adrr_var = array_modbas_in[2];								// собераем адрес из массива в одну переменную
									adrr_var = ((adrr_var << 8) | array_modbas_in[3]);

									if (adrr_var <= leth_array)									// проверка возможности записи по указанному адресу
									{

										array_mb[adrr_var] = array_modbas_in[4];								// !!!!! тестовый код !!!!!
										array_mb[adrr_var] = ((array_mb[adrr_var] << 8) | array_modbas_in[5]);



										for(int i = 0; i<= count_modbas_in; i++)				// скопируем масивы. так как ответ должен быть таким же как приёмный пакет.
										{
											array_modbas_out[i] = array_modbas_in[i];
										}

										count_modbas_out = count_modbas_in;
										count_modbas_in = 0;
										swith_modbas = 2; 										// переключаем на выполнение второго кейса


									} // скобка проверки адреса переменной
									else 														// если адрес выходит за диапазон доступних, нужно записать код ошибки
									{
										count_modbas_in = 0;
										swith_modbas = 0;
									} // else скобка

								} // скобка команды 0х06
								break;



								case 0x10:				// запись значений в несколько регистров хранения (Preset Multiple Registers)
								{
									adrr_var = array_modbas_in[2];								// собераем адрес из массива в одну переменную
									adrr_var = ((adrr_var << 8) | array_modbas_in[3]);


										if (adrr_var + (array_modbas_in[6]/2) <= leth_array)	// проверка возможности записи по указанному адресу
										{

											int adrr_var_buf = adrr_var;

											for (int i = 0; array_modbas_in[6] > i ; i++)
											{
												array_mb[adrr_var_buf] = array_modbas_in[(7+i)];	//
												adrr_var_buf++;
											} // скобка цикла


											array_modbas_out[0] = array_modbas_in[0];
											array_modbas_out[1] = array_modbas_in[1];
											array_modbas_out[2] = array_modbas_in[2];
											array_modbas_out[3] = array_modbas_in[3];
											array_modbas_out[4] = array_modbas_in[4];
											array_modbas_out[5] = array_modbas_in[5];


											crc_calc = CRC16(array_modbas_out, 6);				// считаем CRC отправляемого пакета, 3 точно известных байта и байты считанные , зависит от кол-ва в переменной

											crc_calc_low = ((crc_calc >> 8) & 0x00FF);			// младший (count_modbas_in+2)
											crc_calc_high = (crc_calc & 0x00FF);				// старший (count_modbas_in+1)

											array_modbas_out[6] = crc_calc_high;				// посчитаная контролька, пишем в массив для отправки
											array_modbas_out[7] = crc_calc_low;


											count_modbas_out = 8;								// ответ всегда равен 8 байт
											count_modbas_in = 0;
											swith_modbas = 2;
										} // скобка проверки адреса переменных
										else
										{
											count_modbas_in = 0;
											swith_modbas = 0;
										} // скобка проверки адреса переменных

								} // скобка команды 0х10
								break;



								case 0x03:				// чтение значений из нескольких регистров хранения (Read Holding Registers).
								{

									adrr_var = array_modbas_in[2];										// собераем адрес из массива в одну переменную
									adrr_var = ((adrr_var << 8) | array_modbas_in[3]);

									quantity_byte = array_modbas_in[4];
									quantity_byte = ((quantity_byte << 8) | array_modbas_in[5]);		// соберём кол-во байт, которое нужно прочитать


									if ((adrr_var <= leth_array) || ((adrr_var + quantity_byte) <= leth_array))	// проверка возможности чтения данных по указанному адресу.
									{
										array_modbas_out[0] = array_modbas_in[0];						// адрес слейва, который отвечает на запрос
										array_modbas_out[1] = array_modbas_in[1];						// код команды, на которую отвечает слейв (0х3 конкретно в этом случае)
										array_modbas_out[2] = quantity_byte * 2;						// колличество байт, которые передаём. переменная 2 байта, верх срежется, поэтому не можем разом в один пакет записать более 255-ти байт



										for (int i = 0; i <= quantity_byte; i++, adrr_var++)
										{
											array_modbas_out[i*2+3] = ((array_mb[adrr_var] >> 8) & 0x00FF);			// читаем из массива хранения в сеть по указанному адресу
											array_modbas_out[i*2+4] = (array_mb[adrr_var] & 0x00FF);
										}

										quantity_byte *= 2;
																										// в слове по два байта
										crc_calc = CRC16(array_modbas_out, (quantity_byte + 3));		// считаем CRC отправляемого пакета, 3 точно известных байта и байты считанные , зависит от колва в переменной

										crc_calc_low = ((crc_calc >> 8) & 0x00FF);						// младший (count_modbas_in+2)
										crc_calc_high = (crc_calc & 0x00FF);							// старший (count_modbas_in+1)

										array_modbas_out[(quantity_byte + 3)] = crc_calc_high;			// посчитаная контролька, пишем в массив для отправки
										array_modbas_out[(quantity_byte + 3)+1] = crc_calc_low;			//

										count_modbas_out = (quantity_byte + 3)+2;
										count_modbas_in = 0;
										swith_modbas = 2; 												// переключаем на выполнение второго кейса

									} // скобка проверки адреса переменной
									else // если адрес выходит за диапазон доступних, нужно записать код ошибки
									{
										count_modbas_in = 0;
										swith_modbas = 0;
									} // else скобка

								} // скобка команды 0х03
								break;



								case 0x04:				// чтение значений из нескольких регистров ввода (Read Input Registers).
								{

								} // скобка команды 0х04
								break;

							} // скобка свича разбора пакета на команды.




					} // скобка проверки совпадения принятого crc

					else
					{
						swith_modbas = 0; 										// улетаем в кейс обработки ошибочных ситуаций.
						count_modbas_in = 0;

					} // else скобка проверки совпадения принятого crc




				} // скобка проверки адреса


				else
				{
					swith_modbas = 0; 		// если адрес не совпадает, то дальше даже не проверяем пакет, выходим из кейса
					count_modbas_in = 0;
				} // else скобка проверки адреса
			}	// скобочка первого кейса
			break;

			case 2:
			{

				GPIOA->BSRR =  GPIO_BSRR_BS1;					// Ставим еденицу, переводим линию в передачу.

				buffer_index=0;									// Сбрасываем индекс

				USART2->DR = array_modbas_out[0];				// Отправляем первый байт из массива для отправки
				USART2->CR1 	|= 	USART_CR1_TXEIE;			// включаем прерывание по опустошению души


				swith_modbas = 0; 								// сбрасываем свич в дефолт и не заходим сюда
			}  // скобочка второго кейса
			break;

			case 3: // кейс ошибок, сюда попадаем при обнаружении ошибок. доработать!!!!
			{
				//swith_modbas = 0;
			}  // скобочка третего кейса
			break;

			default:
				break;
		} // скобочка свича


}






// функция для расчёта контрольной суммы
unsigned short CRC16(int *puchMsg,  /* Сообщение       */
                            unsigned short usDataLen /* Длина сообщения */)
{
    unsigned short crc = 0xFFFF;
    unsigned short uIndex;
    int i;
    for (uIndex = 0; uIndex < usDataLen; uIndex += 1) {
        crc ^= (unsigned short)*(puchMsg + uIndex);
        for (i = 8; i != 0; i -= 1) {
          if ((crc & 0x0001) == 0) { crc >>= 1; }
          else { crc >>= 1; crc ^= 0xA001; }			// полином задаём здесь
        }
    }
    // Изменим порядок следования байт
 //   crc = ((crc >> 8) & 0x00FF) | ((crc << 8) & 0xFF00);
    return crc;

}







