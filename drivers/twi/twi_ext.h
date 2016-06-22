/*
 * twi.h
 *
 * Created: 8/01/2016 7:13:35 p.m.
 *  Author: Ali
 */ 


#ifndef TWI_EXT_H_
#define TWI_EXT_H_

void TWI_EXT_MasterInit(void);
void TWI_EXT_MasterSlaveLoadAddress(uint8_t address, uint8_t read);

void TWI_EXT_SlaveLoadAddress(uint8_t slaveaddress);
void TWI_EXT_SlaveInit(void);

uint8_t TWI_EXT_ReadData(void);
void TWI_EXT_LoadData(uint8_t data);


#endif /* TWI_EXT_H_ */