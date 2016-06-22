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
		
#define TWI_SendStart()			(TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE))
#define TWI_SendStop()			(TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN)|(1<<TWIE))
#define TWI_SendTransmit()		(TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE)) 
#define TWI_SendACK()			(TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE)|(1<<TWEA))
#define TWI_SendNACK()			(TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE))


#endif /* TWI_DEFS_H_ */