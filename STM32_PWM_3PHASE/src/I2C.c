/*
 * I2C.c
 *
 *  Created on: 5 мая 2019 г.
 *      Author: Макеев
 */
#include "stm32f10x.h"
#include "I2C.h"


unsigned char* 	status;

// не глобальные !!!
typedef struct {
unsigned char 	I2C_TX_BUF[200];
unsigned short 	I2C_count_TX;
unsigned short 	I2C_ind_TX;
unsigned char* 	I2C_RX_BUF;
		 short 	I2C_count_RX;
		 short 	I2C_ind_RX;
unsigned char 	I2C_addr;
unsigned char 	status;
I2C_TypeDef * 	i2c;
}I2C_str;

I2C_str I2C_data = {.i2c = I2C1,.status = 0};


void Init_I2C1 (void){
//unsigned int in, countPuls;
//	return;
//	RCC->APB2ENR|=RCC_APB2ENR_AFIOEN;//тактирование альтернативных функций
	RCC->APB2ENR|=RCC_APB2ENR_IOPBEN;//тактирование порта В
	RCC->APB1ENR|=RCC_APB1ENR_I2C1EN;// тактирование I2C1

	GPIOB->CRL|=GPIO_CRL_MODE6|GPIO_CRL_MODE7;//управляющие сигналы - выхода 50 МГц
	GPIOB->CRL|=GPIO_CRL_CNF6|GPIO_CRL_CNF7; //альтернативная функция открытый сток
/*
	if((I2C2->SR2&I2C_SR2_BUSY) == I2C_SR2_BUSY)
	{
		GPIOB->CRH |= GPIO_CRH_MODE10_0;
		GPIOB->CRH&= ~GPIO_CRH_CNF10;
			GPIOB->BSRR = GPIO_BSRR_BS10;
			for(in=0; in< 720; in++){}
		for (countPuls = 0;countPuls<10;countPuls++ )
		{
			GPIOB->BSRR = GPIO_BSRR_BR10;
			for(in=0; in< 360; in++){}
			GPIOB->BSRR = GPIO_BSRR_BS10;
			for(in=0; in< 360; in++){}
		}
	GPIOB->CRH|=GPIO_CRH_MODE10|GPIO_CRH_MODE11;//управляющие сигналы - выхода 50 МГц
	GPIOB->CRH|=GPIO_CRH_CNF10|GPIO_CRH_CNF11; //альтернативная функция открытый сток

}
		*/
	/*Сброс в начальное состояние I2C2*/
	RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST;
//	I2C1->CR1 &= ~I2C_CR1_PE;
//	I2C1->CR1 |= I2C_CR1_SWRST;

//	for (int i = 0; i<100; i++){}

	RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;
//	I2C2->CR1 &= ~I2C_CR1_SWRST;
	/*параметры I2C2*/

	I2C1->CR1=0x0;// выключить модуль
	I2C1->CR2=0x24;//36 MHz (частота шины APB1)
	I2C1->CCR = 0x801E;//-400kHz; 0x803C;//-200kHz  0x8078;//-100kHz;
	// частота I2C = 1/[(CCR[11:0] / CR2)* 3 ]  MHz  3 - если FastMode и Duty = 0 (CCR = 0x8xxx)
	I2C1->TRISE=0x04;
	I2C1->CR1=0x401;// ack , en I2c
	I2C1->CR2 |= I2C_CR2_ITEVTEN; //прерывания
	I2C1->CR2 |= I2C_CR2_ITERREN; //прерывания

	I2C1->SR1 = 0;// обнулим что-нибудь...???
	// генерируем СТОП
	//		I2C2->CR1 |= I2C_CR1_STOP;
//	return;
	NVIC_EnableIRQ(I2C1_EV_IRQn);
	NVIC_EnableIRQ(I2C1_ER_IRQn);
	status = &I2C_data.status;
}
//

/**************************************************************************************************/
void Init_I2C21 (void){
//unsigned int in, countPuls;
//	return;
	RCC->APB2ENR|=RCC_APB2ENR_AFIOEN;//тактирование альтернативных функций
	RCC->APB2ENR|=RCC_APB2ENR_IOPBEN;//тактирование порта В
	RCC->APB1ENR|=RCC_APB1ENR_I2C2EN;// тактирование I2C2

	GPIOB->CRH|=GPIO_CRH_MODE10|GPIO_CRH_MODE11;//управляющие сигналы - выхода 50 МГц
	GPIOB->CRH|=GPIO_CRH_CNF10|GPIO_CRH_CNF11; //альтернативная функция открытый сток
/*
	if((I2C2->SR2&I2C_SR2_BUSY) == I2C_SR2_BUSY)
	{
		GPIOB->CRH |= GPIO_CRH_MODE10_0;
		GPIOB->CRH&= ~GPIO_CRH_CNF10;
			GPIOB->BSRR = GPIO_BSRR_BS10;
			for(in=0; in< 720; in++){}
		for (countPuls = 0;countPuls<10;countPuls++ )
		{
			GPIOB->BSRR = GPIO_BSRR_BR10;
			for(in=0; in< 360; in++){}
			GPIOB->BSRR = GPIO_BSRR_BS10;
			for(in=0; in< 360; in++){}
		}
	GPIOB->CRH|=GPIO_CRH_MODE10|GPIO_CRH_MODE11;//управляющие сигналы - выхода 50 МГц
	GPIOB->CRH|=GPIO_CRH_CNF10|GPIO_CRH_CNF11; //альтернативная функция открытый сток

}
		*/
	/*Сброс в начальное состояние I2C2*/
	RCC->APB1RSTR |= RCC_APB1RSTR_I2C2RST;
//	I2C2->CR1 &= ~I2C_CR1_PE;
//	I2C2->CR1 |= I2C_CR1_SWRST;

//	for (int i = 0; i<100; i++){}

	RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C2RST;
//	I2C2->CR1 &= ~I2C_CR1_SWRST;
	/*параметры I2C2*/

	I2C2->CR1=0x0;// выключить модуль
	I2C2->CR2=0x24;//36 MHz (частота шины APB1)
	I2C2->CCR = 0x801E;//-400kHz; 0x803C;//-200kHz  0x8078;//-100kHz;
	// частота I2C = 1/[(CCR[11:0] / CR2)* 3 ]  MHz  3 - если FastMode и Duty = 0 (CCR = 0x8xxx)
	I2C2->TRISE=0x04;
	I2C2->CR1=0x401;// ack , en I2c
//	I2C2->CR1 |= I2C_CR2_ITEVTEN; //прерывания

	// генерируем СТОП
	//		I2C2->CR1 |= I2C_CR1_STOP;
//	return;
}
//

/**************************************************************************************************/
/*запись команды*/
void cmd_WRITE1(char EE_adr, unsigned char data[2])//return ERROR
{
				//включить ACK
			I2C2->CR1 |= I2C_CR1_ACK;

	//ждать свободной шины
		while((I2C2->SR2 & I2C_SR2_BUSY)!=0)
		{/*обработка time out*/
//timeOut++;
//if(timeOut>5000){return 0;}
}
	//генерируем СТАРТ
		I2C2->CR1 |= I2C_CR1_START;
		//ждать СТАРТ
		while((I2C2->SR1 & I2C_SR1_SB) == 0)
			{/*обработка time out*/}
	/*отправить адрес (write)*/
		I2C2->DR = EE_adr & 0xFE;
			/*Ждать ACK
			(очистка флага SB)
			проверка ADDR - устанавливается после прохождения АСК*/
			while((I2C2->SR1 & I2C_SR1_ADDR) == 0)
			{/*обработка time out*/}

	//Ждать установки режима передачи

			while((I2C2->SR2 & I2C_SR2_TRA) == 0)
			{/*обработка time out*/}

			//Отправка старшего байта данных
	I2C2->DR = data[0];

			//Ждать опустошения буфера передачи данных
				while((I2C2->SR1 & I2C_SR1_TXE) == 0)
			{/*обработка time out*/}

		//Отправка младшего байта данных
	I2C2->DR = data[1];
			//Ждать опустошения буфера передачи данных и конца передачи
				while((I2C2->SR1 & (I2C_SR1_TXE | I2C_SR1_BTF)) == 0)
			{/*обработка time out*/}


// генерируем СТОП
			I2C2->CR1 |= I2C_CR1_STOP;
}
//


/**************************************************************************************************/
/*запись данных*/
void data_WRITE1(char EE_adr, unsigned char* data, unsigned int count)//return ERROR
{
unsigned int	index;
count--;

	//включить ACK
			I2C2->CR1 |= I2C_CR1_ACK;

	//ждать свободной шины
		while((I2C2->SR2 & I2C_SR2_BUSY)!=0)
		{/*обработка time out*/
//timeOut++;
//if(timeOut>5000){return 0;}
}
	//генерируем СТАРТ
		I2C2->CR1 |= I2C_CR1_START;
		//ждать СТАРТ
		while((I2C2->SR1 & I2C_SR1_SB) == 0)
			{/*обработка time out*/}
	/*отправить адрес (write)*/
		I2C2->DR = EE_adr & 0xFE;
			/*Ждать ACK
			(очистка флага SB)
			проверка ADDR - устанавливается после прохождения АСК*/
			while((I2C2->SR1 & I2C_SR1_ADDR) == 0)
			{/*обработка time out*/}

	//Ждать установки режима передачи

			while((I2C2->SR2 & I2C_SR2_TRA) == 0)
			{/*обработка time out*/}
/*****/

			//Отправка старшего байта данных


			for(index = 0; index < (count);index++){
				I2C2->DR = data[index];

			//Ждать опустошения буфера передачи данных
				while((I2C2->SR1 & I2C_SR1_TXE) == 0)
			{/*обработка time out*/}

			}

		//Отправка последнего байта данных
	I2C2->DR = data[count];
			//Ждать опустошения буфера передачи данных и конца передачи
				while((I2C2->SR1 & (I2C_SR1_TXE | I2C_SR1_BTF)) == 0)
			{/*обработка time out*/}


// генерируем СТОП
			I2C2->CR1 |= I2C_CR1_STOP;
}
//



/*
 * EVx= Event (with interrupt if ITEVFEN = 1)
 * генерируем старт
 * приходит событие (EV5) SB=1, очищается чтением SR1, и следом записью адреса слэйва в DR (+ ACK)
 * полсе записи приходит событие (EV6) ADDR=1, очищаем чтением SR1, следом SR2
 * 			следом событие (EV8_1) TxE = 1, сдвиговый регистр пуст, регистр данных пуст,
 *
 * 	ЦИКЛ-
 * 	пишем первый байт данных в  DR
 * 	приходит событие (EV8) TxE = 1, сдвиговый регистр НЕ пуст, регистр данных пуст, очищается записью в DR
 * 	ЦИКЛ-
 * когда данные кончились , ждем (EV8_2) TxE = 1, BTF = 1 и генерируем СТОП (чистятся TxE, BTF стопом)
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */


void I2C1_start_TX( unsigned char addr, unsigned char* reg, unsigned short reg_count, unsigned char* data, unsigned short cnt, short stupid){
int ind;

if(I2C_data.status)return;

if((I2C_data.i2c->SR2 & I2C_SR2_BUSY)!=0) return;//ждать свободной шины

	I2C_data.status = 1;
	I2C_data.I2C_addr = addr & 0xFE; //
	I2C_data.I2C_count_TX = cnt + reg_count;
	I2C_data.I2C_ind_TX = 0;

	for(ind = 0 ; ind < reg_count; ind ++){
		I2C_data.I2C_TX_BUF[ind] = reg[ind];
	}
	for(ind = reg_count ; ind < I2C_data.I2C_count_TX; ind ++){
		I2C_data.I2C_TX_BUF[ind] = data[(ind - reg_count)];
	}



	I2C_data.i2c->SR1 = 0;// обнулим что-нибудь...???
	//включить ACK
	I2C_data.i2c->CR1 |= I2C_CR1_ACK;
	//генерируем СТАРТ
	I2C_data.i2c->CR1 |= I2C_CR1_START;
//дальше магия.....
		while ((I2C_data.status && stupid )!= 0){
			//тупняк....
			if(I2C_data.status > 20) break;
		}
}

void I2C1_start_RX( unsigned char addr, unsigned char* dataTX, unsigned char* dataRX, unsigned short cnt_TX, unsigned short cnt_RX, short stupid){
int ind;

if(I2C_data.status)return;

if((I2C_data.i2c->SR2 & I2C_SR2_BUSY)!=0) return;//ждать свободной шины

if(cnt_RX==0)return;// ато повиснем

	I2C_data.status = 2;
	I2C_data.I2C_addr = addr & 0xFE; //
	I2C_data.I2C_count_TX = cnt_TX;
	I2C_data.I2C_count_RX = cnt_RX;
	I2C_data.I2C_ind_TX = 0;
	I2C_data.I2C_ind_RX = 0;

	I2C_data.I2C_RX_BUF = dataRX;

	for(ind = 0 ; ind < cnt_TX; ind ++){
		I2C_data.I2C_TX_BUF[ind] = dataTX[ind];
	}
	(void)I2C1->SR1;//clear ???
	(void)I2C1->SR2;//clear
	I2C_data.i2c->SR1 = 0;// обнулим что-нибудь...???
	//включить ACK
	I2C_data.i2c->CR1 |= I2C_CR1_ACK;
	//генерируем СТАРТ
	I2C_data.i2c->CR1 |= I2C_CR1_START;
//дальше магия.....
		while ((I2C_data.status && stupid )!= 0){
			//тупняк....
			if(I2C_data.status > 20) break;
		}
}












void I2C1_EV_IRQHandler(void){
unsigned short temp_SR1 = I2C1->SR1;


//ITEVFEN
//прошел старт
	if(temp_SR1 & I2C_SR1_SB){
			/*отправить адрес (write или read)*/
		if(I2C_data.status == 1 || I2C_data.status == 2){//передача
			I2C1->DR = I2C_data.I2C_addr;// r/w ставим при старте
		}
		if(I2C_data.status == 3){//прием
			I2C1->DR = I2C_data.I2C_addr | 1;// r/w ставим при старте

		}
	//		(void)I2C2->SR1;//clear ???
	//	return;
	}
//-------------------------------------------------------------
//		проверка ADDR - устанавливается после прохождения АСК*/
	if(temp_SR1 & I2C_SR1_ADDR){
		(void)I2C1->SR1;//clear ???
		(void)I2C1->SR2;//clear

			if(I2C_data.status == 1 || I2C_data.status == 2){//write

				if(I2C_data.I2C_ind_TX < I2C_data.I2C_count_TX){//write


					I2C1->DR = I2C_data.I2C_TX_BUF[I2C_data.I2C_ind_TX];
					I2C_data.I2C_ind_TX++;
				}
			}
/*		*/		if((I2C_data.status == 3) &&  (I2C_data.I2C_count_RX == 1) ){
				I2C1->CR1 &= ~I2C_CR1_ACK;			// NACK
				I2C1->CR1 |= I2C_CR1_STOP;			// генерируем СТОП
			}

			I2C1->CR2 |= I2C_CR2_ITBUFEN;
	}




	//-------------------------------------------------------------
    /*!< Byte Transfer Finished */
	if(temp_SR1 & I2C_SR1_BTF){

		if(I2C_data.status == 1){//write

			if(I2C_data.I2C_ind_TX < I2C_data.I2C_count_TX){//write -
				I2C1->DR = I2C_data.I2C_TX_BUF[I2C_data.I2C_ind_TX];
				I2C_data.I2C_ind_TX++;
			}else{
				// генерируем СТОП
				I2C1->CR1 |= I2C_CR1_STOP;
				I2C_data.status = 0;
			}
		}



		if(I2C_data.status == 2){//write

			if(I2C_data.I2C_ind_TX < I2C_data.I2C_count_TX){//write -
				I2C1->DR = I2C_data.I2C_TX_BUF[I2C_data.I2C_ind_TX];
				I2C_data.I2C_ind_TX++;
			}else{
				I2C_data.status = 3;
				// генерируем RESTART
				I2C1->CR1 |= I2C_CR1_START;

			}
		}

	}//if(temp_SR1 & I2C_SR1_BTF)
//-------------------------------------------------------------
//ITEVFEN and ITBUFEN
	if(temp_SR1 & I2C_SR1_RXNE){

		I2C_data.I2C_RX_BUF[I2C_data.I2C_ind_RX] = I2C1->DR;
		I2C_data.I2C_ind_RX ++;

		if(I2C_data.I2C_ind_RX == (I2C_data.I2C_count_RX)){
			I2C1->CR2 &= ~I2C_CR2_ITBUFEN;			//data end
			I2C_data.status = 0;
		}

		if(I2C_data.I2C_ind_RX == (I2C_data.I2C_count_RX-1)){
			I2C1->CR1 &= ~I2C_CR1_ACK;			// NACK
			I2C1->CR1 |= I2C_CR1_STOP;			// генерируем СТОП
		}





		/* работает!
		if(I2C_data.I2C_ind_RX == (I2C_data.I2C_count_RX-1)){
			I2C_data.I2C_RX_BUF[I2C_data.I2C_ind_RX] = I2C1->DR;
			I2C_data.I2C_ind_RX ++;

			I2C_data.status = 0;
			//data end
			I2C1->CR2 &= ~I2C_CR2_ITBUFEN;

		}

		if(I2C_data.I2C_ind_RX == (I2C_data.I2C_count_RX-2)){
			I2C_data.I2C_RX_BUF[I2C_data.I2C_ind_RX] = I2C1->DR;
			I2C_data.I2C_ind_RX ++;

			I2C1->CR1 &= ~I2C_CR1_ACK;			// NACK
			I2C1->CR1 |= I2C_CR1_STOP;			// генерируем СТОП
		}

		if(I2C_data.I2C_ind_RX < (I2C_data.I2C_count_RX-2)){
			I2C_data.I2C_RX_BUF[I2C_data.I2C_ind_RX] = I2C1->DR;
			I2C_data.I2C_ind_RX ++;

		}

	*/
	}//if(temp_SR1 & I2C_SR1_RXNE)
//-------------------------------------------------------------
	if(temp_SR1 & I2C_SR1_TXE){

		if(I2C_data.I2C_ind_TX < I2C_data.I2C_count_TX){//write
			I2C1->DR = I2C_data.I2C_TX_BUF[I2C_data.I2C_ind_TX];
			I2C_data.I2C_ind_TX++;
		}else{
			//data end
			I2C1->CR2 &= ~I2C_CR2_ITBUFEN;
		}
	}

}// end



void I2C1_ER_IRQHandler(void){
unsigned short temp_SR1 = I2C1->SR1;

I2C1->SR1 = 0;
// генерируем СТОП
I2C1->CR1 |= I2C_CR1_STOP;

//Bus error
	if(temp_SR1 & I2C_SR1_BERR){
		I2C_data.status = 21;
	}
//	Arbitration loss (Master)
	if(temp_SR1 & I2C_SR1_ARLO){
		I2C_data.status = 22;
	}

	//Acknowledge failure - NACK
	if(temp_SR1 & I2C_SR1_AF){


		I2C_data.status = 23;
	}

	//	Overrun/Underrun
		if(temp_SR1 & I2C_SR1_OVR){
			I2C_data.status = 24;
		}

		//	Overrun/Underrun
			if(temp_SR1 & I2C_SR1_OVR){
				I2C_data.status = 25;
			}
	//PEC error
			if(temp_SR1 & I2C_SR1_PECERR){
				I2C_data.status = 26;
			}
	//Timeout/Tlow error
			if(temp_SR1 & I2C_SR1_TIMEOUT){
				I2C_data.status = 27;
			}
	//SMBus Alert
			if(temp_SR1 & I2C_SR1_SMBALERT){
				I2C_data.status = 28;
			}


}// end




//-------------------------------------------------------------------
//LASER










