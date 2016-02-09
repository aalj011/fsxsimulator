/*
 * twi.h
 *
 * Created: 8/01/2016 7:13:35 p.m.
 *  Author: Ali
 */ 


#ifndef TWI_H_
#define TWI_H_

void twi_master_init(void);
uint8_t twi_master_start_condition(void);

uint8_t twi_master_sla_sendAddress(uint8_t address, uint8_t read);
uint8_t twi_master_sendData(uint8_t data);
uint8_t twi_master_stop_condition(void);
uint8_t twi_master_readData(uint8_t *dataptr);
uint8_t twi_master_readFinished(void);
uint8_t twi_master_repeat_condition(void);

uint8_t twi_slave_init(uint8_t slaveaddress);
void twi_slave_ACK_tx(void);
void twi_slave_NACK_tx(void);

#endif /* TWI_H_ */