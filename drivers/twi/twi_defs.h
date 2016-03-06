/*
 * twi_defs.h
 *
 * Created: 8/01/2016 7:14:28 p.m.
 *  Author: Ali
 */ 


#ifndef TWI_DEFS_H_
#define TWI_DEFS_H_

#define TWBR_BIT_RATE			0x20

#define SLA_ADD_ONE_WRITE		0x40
#define SLA_ADD_TWO_WRITE		0x42

#define SLA_ADD_ONE_READ		0x41
#define SLA_ADD_TWO_READ		0x43

#define TWI_WRITE				0x00
#define TWI_READ				0x01

#define MASK					0xF8
			


#define TWI_SendStart()			(TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE))
#define TWI_SendStop()			(TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN)|(1<<TWIE))
#define TWI_SendTransmit()		(TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE)) 
#define TWI_SendACK()			(TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE)|(1<<TWEA))
#define TWI_SendNACK()			(TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE))


//Master defines 
//**************************************************
#define MASTER_SLA_W_ACK_RX						0x18
#define MASTER_SLA_W_NACK_RX					0x20

#define MASTER_SLA_R_ACK_RX						0x40
#define MASTER_SLA_R_NACK_RX					0x48

#define MASTER_START_TX							0x08
#define MASTER_REPEAT_TX						0x10

#define MASTER_DATA_TX_ACK_RX					0x28
#define MASTER_DATA_TX_NACK_RX					0x30

#define MASTER_DATA_RX_ACK_TX					0x50
#define MASTER_DATA_RX_NACK_TX					0x58
//**************************************************

//Slave defines 
//**************************************************
#define SLAVE_SLA_W_ACK_TX						0x60
#define SLAVE_SLA_W_NACK_TX						0x20

#define SLAVE_DATA_RX_ACK_TX					0x80
#define SLAVE_DATA_RX_NACK_TX					0x88

#define SLAVE_SLA_R_ACK							0x40
#define SLAVE_SLA_R_NACK						0x48

#define SLAVE_COMS_LOST							0x68
#define SLAVE_BUS_ERROR							0x00
//**************************************************

#endif /* TWI_DEFS_H_ */