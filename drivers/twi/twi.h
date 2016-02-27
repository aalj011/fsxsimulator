/*
 * twi.h
 *
 * Created: 8/01/2016 7:13:35 p.m.
 *  Author: Ali
 */ 


#ifndef TWI_H_
#define TWI_H_

void twi_master_init(void);
void twi_master_start_condition(void);
void twi_master_sla_send_address(uint8_t address, uint8_t read);
void twi_master_stop_condition(void);
void twi_master_repeat_condition(void);
void twi_master_tx_nack(void);
void twi_master_tx_ack(void);


void twi_slave_init(uint8_t slaveaddress);
void twi_slave_tx_ack(void);
void twi_slave_tx_nack(void);

void twi_clear_twint(void);
uint8_t twi_read_data(void);
void twi_wait(void);
void twi_send_data(uint8_t data);


#endif /* TWI_H_ */