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
			

//Master defines 
//***********************************
#define MASTER_SLA_W_ACK_RECIEVED				0x18
#define MASTER_SLA_W_NACK_RECIEVED				0x20

#define MASTER_SLA_R_ACK_RECIEVED				0x40
#define MASTER_SLA_R_NACK_RECIEVED				0x48

#define MASTER_START_TRANSMITTED				0x08
#define MASTER_REPEAT_TRANSMITTED				0x10

#define MASTER_DATA_TX_ACK_RECIEVED				0x28
#define MASTER_DATA_TX_NACK_RECIEVED			0x30

#define MASTER_DATA_RX_ACK_TRANSMITTED			0x50
#define MASTER_DATA_RX_NACK_TRANSMITTED			0x58
//***********************************

//Slave defines 
//***********************************
#define SLAVE_SLA_W_ACK_TRANSMITTED				0x60
#define SLAVE_SLA_W_NACK_TRANSMITTED			0x20

#define SLAVE_DATA_RX							0x80
#define SLAVE_DATA_TX_NACK			0x30

#define SLAVE_SLA_R_ACK				0x40
#define SLAVE_SLA_R_NACK				0x48

#define SLAVE_DATA_RX_ACK				0x50
#define SLAVE_DATA_RX_NACK			0x58
//***********************************



#endif /* TWI_DEFS_H_ */