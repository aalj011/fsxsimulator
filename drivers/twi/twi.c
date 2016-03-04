/*
 * twi.c
 *
 * Created: 8/01/2016 7:13:26 p.m.
 *  Author: Ali
 */ 
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>
#include <stdio.h>
#include "twi.h"
#include "twi_defs.h"

static void flagwait(void);

/*****************************************************************************
Name:			twi_master_init
Purpose:		Initializes the twi communication for the master
Parameters:		None
Note:			None
****************************************************************************/
void twi_master_init(void)
{
	//PRR0 = (0<<PRTWI);													//Turn off Power reduction on TWI 							
	TWSR = (0<<TWPS0)|(0<<TWPS1);
	TWBR = TWBR_BIT_RATE;													//Bit rate at 8mhz 100khz 0x20	
	TWDR = 0xFF;															//Dummy data
	TWCR =	(1<<TWEN)|														//Enables TWI interface
			(1<<TWIE)|(0<<TWINT)|											//Disable interrupt
			(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|								//Setting ACK gen,start condition and stop condition to zero init conditions 
			(0<<TWWC);														//Write collision flag to zero	
}

/*****************************************************************************
Name:			twi_master_start_condition
Purpose:		This function invokes the start condition for Master twi communication
Parameters:		none
Returns:		None
Note:			None
****************************************************************************/
void twi_master_start_condition(void)
{	
	TWCR =	(1<<TWEN)|														//Enables TWI interface
			(1<<TWIE)|(1<<TWINT)|											//Enable interrupt
			(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|								//Enabling the start condition bit 
			(0<<TWWC);
	//flagwait();														//write collision flag to zero	
}


/*****************************************************************************
Name:			twi_master_stop_Condition
Purpose:		This function invokes the stop condition for twi communication
Parameters:		none
Returns:		None
Note:			None
****************************************************************************/
void twi_master_stop_condition(void)
{
	TWCR =	(1<<TWEN)|														//Enables TWI interface
			(1<<TWIE)|(1<<TWINT)|											//Enable interrupt
			(0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|								//Enabling the stop condition bit 
			(0<<TWWC);														//write collision flag to zero	
}

/*****************************************************************************
Name:			twi_master_repeatCondition
Purpose:		This function invokes the repeat condition for twi communication
Parameters:		none
Returns:		None
Note:			None
****************************************************************************/
void twi_master_repeat_condition(void)
{
	TWCR =	(1<<TWEN)|														//Enables TWI interface
			(1<<TWIE)|(1<<TWINT)|											//Enable interrupt
			(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|								//Enabling the start condition bit 
			(0<<TWWC);														//write collision flag to zero	
}

/*****************************************************************************
Name:			twi_master_sla_sendAddress
Purpose:		This function sends the Slave address and the read/write bit 
Parameters:		address, the address of the device slave required, a list of
				devices is found in the twi_defs.h.
				uint8_t "read" if set to 1 it will send the read address, if it is zero,
				it will send the write address.
Returns:		Returns a 1 for NOT successful execution and 0 for successful
Note:			None
****************************************************************************/
void twi_master_sla_send_address(uint8_t address, uint8_t read)
{
	uint8_t slave_address = 0;
	slave_address = (address << 1)| (read);
	
	TWDR = slave_address;													//Set the data register with the slave address and the read/write bit
	TWCR =	(1<<TWEN)|														//Enables TWI interface
			(1<<TWIE)|(1<<TWINT)|											//Enable interrupt
			(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|								//Enabling the start condition bit 
			(0<<TWWC);		
}

/*****************************************************************************
Name:			twi_master_sendData
Purpose:		This function write the data to be sent to the slave address.
Parameters:		uint8_t data to be written.
Returns:		None
Note:			None
****************************************************************************/
void twi_send_data(uint8_t data)
{	
	TWDR = data;
	
	TWCR =	(1<<TWEN)|															//Enables TWI interface
			(1<<TWIE)|(1<<TWINT)|												//Enable interrupt
			(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|									//Enabling the start condition bit 
			(0<<TWWC);															//Data to transmit
}
/*****************************************************************************
Name:			twi_master_tx_nack
Purpose:		This transmits a NACK to the slave telling the slave we have
Parameters:		None.
Returns:		None
Note:			None
****************************************************************************/
void twi_master_tx_nack(void)
{
	TWCR =	(1<<TWEN)|														//Enables TWI interface
			(1<<TWIE)|(1<<TWINT)|											//Enable interrupt
			(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|								//Enabling the start condition bit 
			(0<<TWWC);	
}
/*****************************************************************************
Name:			twi_master_tx_ack
Purpose:		This transmits a ACK to the slave telling the slave we have
Parameters:		None.
Returns:		None
Note:			None
****************************************************************************/
void twi_master_tx_ack(void)
{
	TWCR =	(1<<TWEN)|														//Enables TWI interface
			(1<<TWIE)|(1<<TWINT)|											//Enable interrupt
			(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|								//Enabling the start condition bit
			(0<<TWWC);
}
/*****************************************************************************
Name:			twi_slave_init
Purpose:		This function invokes the start condition for twi communication
Parameters:		none
Returns:		None
Note:			None
****************************************************************************/
void twi_slave_init(uint8_t slaveaddress)
{	
	TWAR = (slaveaddress<<1);
																						//Load the slave address
	TWCR = (1<<TWEN)|														//Enables TWI interface
			(1<<TWIE)|(1<<TWINT)|											//Enable interrupt
			(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|								//Enabling the start condition bit 
			(0<<TWWC);														//Enable twi
																			//Wait till flag is cleared	
	flagwait();
}
/*****************************************************************************
Name:			twi_slave_tx_ack
Purpose:		transmitters ACK to master
Parameters:		none
Returns:		None
Note:			None
****************************************************************************/
void twi_slave_tx_ack(void)
{	
	TWCR =	(1<<TWEN)|														//Enables TWI interface
			(1<<TWIE)|(1<<TWINT)|											//Enable interrupt
			(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|								//Enabling the start condition bit 
			(0<<TWWC);										
}
/*****************************************************************************
Name:			twi_slave_NACK_tx
Purpose:		transmitters NACK to master
Parameters:		none
Returns:		None
Note:			None
****************************************************************************/
void twi_slave_tx_nack(void)
{		
	TWCR = (1<<TWINT)|(1<<TWEN)|(0<<TWEA)|(1<<TWIE);											
}

/*****************************************************************************
Name:			twi_readData
Purpose:		This function reads the data from TWDR.
Parameters:		None
Returns:		None
Note:			None
****************************************************************************/
uint8_t twi_read_data(void)
{
	uint8_t rxdata = 0;
	rxdata = TWDR;
		
	return rxdata;
}
/*****************************************************************************
Name:			twi_clear_twint
Purpose:		Clears TWINT flag
Parameters:		none
Returns:		None
Note:			None
****************************************************************************/
void twi_clear_twint(void)
{
	TWCR |=	(1<<TWINT);
}
/*****************************************************************************
Name:			flagwait
Purpose:		Waits for TWINT to be cleared
Parameters:		none
Returns:		None
Note:			None
****************************************************************************/
static void flagwait(void)
{
	while(!(TWCR &(1<<TWINT)));
}

void twi_wait(void)
{
	flagwait();
}