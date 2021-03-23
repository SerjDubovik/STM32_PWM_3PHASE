/*
 * I2C.h
 *
 *  Created on: 5 мая 2019 г.
 *      Author: Макеев
 */

#ifndef I2C_H_
#define I2C_H_

//void cmd_WRITE(char EE_adr, unsigned char data[2]);
//void data_WRITE(char EE_adr, unsigned char* data, unsigned int count);
//void Init_I2C2 (void);


void Init_I2C1 (void);
void I2C1_EV_IRQHandler(void);
void I2C1_ER_IRQHandler(void);
//void I2C1_start_TX( unsigned char addr, unsigned char* data, unsigned short cnt, short stupid);
void I2C1_start_TX( unsigned char addr, unsigned char* reg, unsigned short reg_count, unsigned char* data, unsigned short cnt, short stupid);
void I2C1_start_RX( unsigned char addr, unsigned char* dataTX, unsigned char* dataRX, unsigned short cnt_TX, unsigned short cnt_RX, short stupid);
extern unsigned char* 	status;





#endif /* I2C_H_ */
