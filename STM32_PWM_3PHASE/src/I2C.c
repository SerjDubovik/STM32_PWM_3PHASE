/*
 * I2C.c
 *
 *  Created on: 5 ��� 2019 �.
 *      Author: ������
 */
#include "stm32f10x.h"
#include "I2C.h"


unsigned char* 	status;

// �� ���������� !!!
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
//	RCC->APB2ENR|=RCC_APB2ENR_AFIOEN;//������������ �������������� �������
	RCC->APB2ENR|=RCC_APB2ENR_IOPBEN;//������������ ����� �
	RCC->APB1ENR|=RCC_APB1ENR_I2C1EN;// ������������ I2C1

	GPIOB->CRL|=GPIO_CRL_MODE6|GPIO_CRL_MODE7;//����������� ������� - ������ 50 ���
	GPIOB->CRL|=GPIO_CRL_CNF6|GPIO_CRL_CNF7; //�������������� ������� �������� ����
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
	GPIOB->CRH|=GPIO_CRH_MODE10|GPIO_CRH_MODE11;//����������� ������� - ������ 50 ���
	GPIOB->CRH|=GPIO_CRH_CNF10|GPIO_CRH_CNF11; //�������������� ������� �������� ����

}
		*/
	/*����� � ��������� ��������� I2C2*/
	RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST;
//	I2C1->CR1 &= ~I2C_CR1_PE;
//	I2C1->CR1 |= I2C_CR1_SWRST;

//	for (int i = 0; i<100; i++){}

	RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;
//	I2C2->CR1 &= ~I2C_CR1_SWRST;
	/*��������� I2C2*/

	I2C1->CR1=0x0;// ��������� ������
	I2C1->CR2=0x24;//36 MHz (������� ���� APB1)
	I2C1->CCR = 0x801E;//-400kHz; 0x803C;//-200kHz  0x8078;//-100kHz;
	// ������� I2C = 1/[(CCR[11:0] / CR2)* 3 ]  MHz  3 - ���� FastMode � Duty = 0 (CCR = 0x8xxx)
	I2C1->TRISE=0x04;
	I2C1->CR1=0x401;// ack , en I2c
	I2C1->CR2 |= I2C_CR2_ITEVTEN; //����������
	I2C1->CR2 |= I2C_CR2_ITERREN; //����������

	I2C1->SR1 = 0;// ������� ���-������...???
	// ���������� ����
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
	RCC->APB2ENR|=RCC_APB2ENR_AFIOEN;//������������ �������������� �������
	RCC->APB2ENR|=RCC_APB2ENR_IOPBEN;//������������ ����� �
	RCC->APB1ENR|=RCC_APB1ENR_I2C2EN;// ������������ I2C2

	GPIOB->CRH|=GPIO_CRH_MODE10|GPIO_CRH_MODE11;//����������� ������� - ������ 50 ���
	GPIOB->CRH|=GPIO_CRH_CNF10|GPIO_CRH_CNF11; //�������������� ������� �������� ����
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
	GPIOB->CRH|=GPIO_CRH_MODE10|GPIO_CRH_MODE11;//����������� ������� - ������ 50 ���
	GPIOB->CRH|=GPIO_CRH_CNF10|GPIO_CRH_CNF11; //�������������� ������� �������� ����

}
		*/
	/*����� � ��������� ��������� I2C2*/
	RCC->APB1RSTR |= RCC_APB1RSTR_I2C2RST;
//	I2C2->CR1 &= ~I2C_CR1_PE;
//	I2C2->CR1 |= I2C_CR1_SWRST;

//	for (int i = 0; i<100; i++){}

	RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C2RST;
//	I2C2->CR1 &= ~I2C_CR1_SWRST;
	/*��������� I2C2*/

	I2C2->CR1=0x0;// ��������� ������
	I2C2->CR2=0x24;//36 MHz (������� ���� APB1)
	I2C2->CCR = 0x801E;//-400kHz; 0x803C;//-200kHz  0x8078;//-100kHz;
	// ������� I2C = 1/[(CCR[11:0] / CR2)* 3 ]  MHz  3 - ���� FastMode � Duty = 0 (CCR = 0x8xxx)
	I2C2->TRISE=0x04;
	I2C2->CR1=0x401;// ack , en I2c
//	I2C2->CR1 |= I2C_CR2_ITEVTEN; //����������

	// ���������� ����
	//		I2C2->CR1 |= I2C_CR1_STOP;
//	return;
}
//

/**************************************************************************************************/
/*������ �������*/
void cmd_WRITE1(char EE_adr, unsigned char data[2])//return ERROR
{
				//�������� ACK
			I2C2->CR1 |= I2C_CR1_ACK;

	//����� ��������� ����
		while((I2C2->SR2 & I2C_SR2_BUSY)!=0)
		{/*��������� time out*/
//timeOut++;
//if(timeOut>5000){return 0;}
}
	//���������� �����
		I2C2->CR1 |= I2C_CR1_START;
		//����� �����
		while((I2C2->SR1 & I2C_SR1_SB) == 0)
			{/*��������� time out*/}
	/*��������� ����� (write)*/
		I2C2->DR = EE_adr & 0xFE;
			/*����� ACK
			(������� ����� SB)
			�������� ADDR - ��������������� ����� ����������� ���*/
			while((I2C2->SR1 & I2C_SR1_ADDR) == 0)
			{/*��������� time out*/}

	//����� ��������� ������ ��������

			while((I2C2->SR2 & I2C_SR2_TRA) == 0)
			{/*��������� time out*/}

			//�������� �������� ����� ������
	I2C2->DR = data[0];

			//����� ����������� ������ �������� ������
				while((I2C2->SR1 & I2C_SR1_TXE) == 0)
			{/*��������� time out*/}

		//�������� �������� ����� ������
	I2C2->DR = data[1];
			//����� ����������� ������ �������� ������ � ����� ��������
				while((I2C2->SR1 & (I2C_SR1_TXE | I2C_SR1_BTF)) == 0)
			{/*��������� time out*/}


// ���������� ����
			I2C2->CR1 |= I2C_CR1_STOP;
}
//


/**************************************************************************************************/
/*������ ������*/
void data_WRITE1(char EE_adr, unsigned char* data, unsigned int count)//return ERROR
{
unsigned int	index;
count--;

	//�������� ACK
			I2C2->CR1 |= I2C_CR1_ACK;

	//����� ��������� ����
		while((I2C2->SR2 & I2C_SR2_BUSY)!=0)
		{/*��������� time out*/
//timeOut++;
//if(timeOut>5000){return 0;}
}
	//���������� �����
		I2C2->CR1 |= I2C_CR1_START;
		//����� �����
		while((I2C2->SR1 & I2C_SR1_SB) == 0)
			{/*��������� time out*/}
	/*��������� ����� (write)*/
		I2C2->DR = EE_adr & 0xFE;
			/*����� ACK
			(������� ����� SB)
			�������� ADDR - ��������������� ����� ����������� ���*/
			while((I2C2->SR1 & I2C_SR1_ADDR) == 0)
			{/*��������� time out*/}

	//����� ��������� ������ ��������

			while((I2C2->SR2 & I2C_SR2_TRA) == 0)
			{/*��������� time out*/}
/*****/

			//�������� �������� ����� ������


			for(index = 0; index < (count);index++){
				I2C2->DR = data[index];

			//����� ����������� ������ �������� ������
				while((I2C2->SR1 & I2C_SR1_TXE) == 0)
			{/*��������� time out*/}

			}

		//�������� ���������� ����� ������
	I2C2->DR = data[count];
			//����� ����������� ������ �������� ������ � ����� ��������
				while((I2C2->SR1 & (I2C_SR1_TXE | I2C_SR1_BTF)) == 0)
			{/*��������� time out*/}


// ���������� ����
			I2C2->CR1 |= I2C_CR1_STOP;
}
//



/*
 * EVx= Event (with interrupt if ITEVFEN = 1)
 * ���������� �����
 * �������� ������� (EV5) SB=1, ��������� ������� SR1, � ������ ������� ������ ������ � DR (+ ACK)
 * ����� ������ �������� ������� (EV6) ADDR=1, ������� ������� SR1, ������ SR2
 * 			������ ������� (EV8_1) TxE = 1, ��������� ������� ����, ������� ������ ����,
 *
 * 	����-
 * 	����� ������ ���� ������ �  DR
 * 	�������� ������� (EV8) TxE = 1, ��������� ������� �� ����, ������� ������ ����, ��������� ������� � DR
 * 	����-
 * ����� ������ ��������� , ���� (EV8_2) TxE = 1, BTF = 1 � ���������� ���� (�������� TxE, BTF ������)
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

if((I2C_data.i2c->SR2 & I2C_SR2_BUSY)!=0) return;//����� ��������� ����

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



	I2C_data.i2c->SR1 = 0;// ������� ���-������...???
	//�������� ACK
	I2C_data.i2c->CR1 |= I2C_CR1_ACK;
	//���������� �����
	I2C_data.i2c->CR1 |= I2C_CR1_START;
//������ �����.....
		while ((I2C_data.status && stupid )!= 0){
			//������....
			if(I2C_data.status > 20) break;
		}
}

void I2C1_start_RX( unsigned char addr, unsigned char* dataTX, unsigned char* dataRX, unsigned short cnt_TX, unsigned short cnt_RX, short stupid){
int ind;

if(I2C_data.status)return;

if((I2C_data.i2c->SR2 & I2C_SR2_BUSY)!=0) return;//����� ��������� ����

if(cnt_RX==0)return;// ��� ��������

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
	I2C_data.i2c->SR1 = 0;// ������� ���-������...???
	//�������� ACK
	I2C_data.i2c->CR1 |= I2C_CR1_ACK;
	//���������� �����
	I2C_data.i2c->CR1 |= I2C_CR1_START;
//������ �����.....
		while ((I2C_data.status && stupid )!= 0){
			//������....
			if(I2C_data.status > 20) break;
		}
}












void I2C1_EV_IRQHandler(void){
unsigned short temp_SR1 = I2C1->SR1;


//ITEVFEN
//������ �����
	if(temp_SR1 & I2C_SR1_SB){
			/*��������� ����� (write ��� read)*/
		if(I2C_data.status == 1 || I2C_data.status == 2){//��������
			I2C1->DR = I2C_data.I2C_addr;// r/w ������ ��� ������
		}
		if(I2C_data.status == 3){//�����
			I2C1->DR = I2C_data.I2C_addr | 1;// r/w ������ ��� ������

		}
	//		(void)I2C2->SR1;//clear ???
	//	return;
	}
//-------------------------------------------------------------
//		�������� ADDR - ��������������� ����� ����������� ���*/
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
				I2C1->CR1 |= I2C_CR1_STOP;			// ���������� ����
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
				// ���������� ����
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
				// ���������� RESTART
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
			I2C1->CR1 |= I2C_CR1_STOP;			// ���������� ����
		}





		/* ��������!
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
			I2C1->CR1 |= I2C_CR1_STOP;			// ���������� ����
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
// ���������� ����
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










