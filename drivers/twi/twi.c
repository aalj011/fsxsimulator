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
			(0<<TWIE)|(0<<TWINT)|											//Disable interrupt
			(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|								//Setting ACK gen,start condition and stop condition to zero init conditions 
			(0<<TWWC);														//Write collision flag to zero	
}

/*****************************************************************************
Name:			twi_master_start_condition
Purpose:		This function invokes the start condition for Master twi communication
Parameters:		none
Returns:		Returns a 1 for NOT successful execution and 0 for successful
Note:			None
****************************************************************************/
uint8_t twi_master_start_condition(void)
{
	uint8_t status = 0;	
	TWCR =	(1<<TWEN)|														//Enables TWI interface
			(0<<TWIE)|(1<<TWINT)|											//Enable interrupt
			(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|								//Enabling the start condition bit 
			(0<<TWWC);														//write collision flag to zero	
	
	while(!(TWCR &(1<<TWINT)));												//Wait till flag is cleared	
	
	if((TWSR & MASK) != MASTER_START_TRANSMITTED)							//Check the status reg if successful							
	{
		status = 1;															//Error has occurred
	}		
	return status;
}


/*****************************************************************************
Name:			twi_master_stop_Condition
Purpose:		This function invokes the stop condition for twi communication
Parameters:		none
Returns:		Returns a 1 for NOT successful execution and 0 for successful
Note:			None
****************************************************************************/
uint8_t twi_master_stop_condition(void)
{
	uint8_t status = 0;	
	TWCR =	(1<<TWEN)|														//Enables TWI interface
			(0<<TWIE)|(1<<TWINT)|											//Enable interrupt
			(0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|								//Enabling the stop condition bit 
			(0<<TWWC);														//write collision flag to zero	
	return status;
}

/*****************************************************************************
Name:			twi_master_repeatCondition
Purpose:		This function invokes the repeat condition for twi communication
Parameters:		none
Returns:		Returns a 1 for NOT successful execution and 0 for successful
Note:			None
****************************************************************************/
uint8_t twi_master_repeatCondition(void)
{
	uint8_t status = 0;	
	TWCR =	(1<<TWEN)|														//Enables TWI interface
			(0<<TWIE)|(1<<TWINT)|											//Enable interrupt
			(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|								//Enabling the start condition bit 
			(0<<TWWC);														//write collision flag to zero	
	
	while(!(TWCR &(1<<TWINT)));												//Wait till flag is cleared	
	
	if((TWSR & MASK) != MASTER_REPEAT_TRANSMITTED)											//Check the status reg if successful							
	{
		status = 1;															//Error has occurred
	}
			
	return status;
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
uint8_t twi_master_sla_sendAddress(uint8_t address, uint8_t read)
{
	uint8_t status = 0;
	uint8_t slave_address = 0;
	uint8_t ACK = 0;
		
	if(read == 1)
	{
		slave_address = (address << 1)| (TWI_READ);
		ACK = MASTER_SLA_R_ACK_RECIEVED;	
	}
	else
	{
		slave_address = (address << 1)| (TWI_WRITE);
		ACK = MASTER_SLA_W_ACK_RECIEVED;
	}	
		
	TWDR = slave_address;													//Set the data register with the slave address and the read/write bit
	TWCR = (1<<TWINT)|(1<<TWEN);
	
	while(!(TWCR &(1<<TWINT)));												// wait for flag to be cleared
	
	if((TWSR & MASK) != ACK)
	{
		status = 1;															//Error occured
	}					
	
	return status;
}

/*****************************************************************************
Name:			twi_master_sendData
Purpose:		This function write the data to be sent to the slave address.
Parameters:		uint8_t data to be written.
Returns:		Returns a 1 for NOT successful execution and 0 for successful
Note:			None
****************************************************************************/
uint8_t twi_master_sendData(uint8_t data)
{
	uint8_t status = 0;
	
	TWDR = data;															//Data to transmit
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	
	if((TWSR & MASK) != MASTER_DATA_TX_ACK_RECIEVED)
	{
		status = 1;															//Error occurred
	}	
	
	return status;
}

/*****************************************************************************
Name:			twi_master_readData
Purpose:		This function reads the data to be from the slave slave address.
Parameters:		uint8_t dataptr pointer which points to the passed variable
				to hold the recieved data.
Returns:		Returns a 1 for NOT successful execution and 0 for successful
Note:			None
****************************************************************************/
uint8_t twi_master_readData(uint8_t *dataptr)
{
	uint8_t status =0;
					
	TWCR =	(1<<TWEN)|														//Enables TWI interface
			(0<<TWIE)|(1<<TWINT)|											//Enable interrupt
			(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|								//Enable ACK bit
			(0<<TWWC);	
	
	while(!(TWCR & (1<<TWINT)));
	
	if((TWSR & MASK) != MASTER_DATA_RX_ACK_TRANSMITTED)
	{
		status = 1;															//Error occurred
	}

	*dataptr = TWDR;														//Received data
	
	return status;
}
/*****************************************************************************
Name:			twi_master_readFinished
Purpose:		This transmits a NACK to the slave telling the slave we have
				finished reading data.
Parameters:		None.
Returns:		Returns a 1 for NOT successful execution and 0 for successful
Note:			None
****************************************************************************/
uint8_t twi_master_readFinished(void)
{
	uint8_t status = 0;
	
	TWCR =	(1<<TWEN)|														//Enables TWI interface
			(0<<TWIE)|(1<<TWINT)|											//Enable interrupt
			(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|								//Enabling the start condition bit 
			(0<<TWWC);	
			
	while(!(TWCR & (1<<TWINT)));
	
	if((TWSR & MASK) != MASTER_DATA_RX_NACK_TRANSMITTED)
	{
		status = 1;															//Error occurred
	}	
	return status;
}

/*****************************************************************************
Name:			twi_slave_init
Purpose:		This function invokes the start condition for twi communication
Parameters:		none
Returns:		Returns a 1 for NOT successful execution and 0 for successful
Note:			None
****************************************************************************/
uint8_t twi_slave_init(uint8_t slaveaddress)
{	
	uint8_t status = 0;	
	TWAR = (slaveaddress<<1);
													//Load the slave address
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);												//Enable twi
	
	while(!(TWCR &(1<<TWINT)));												//Wait till flag is cleared	
	
	if((TWSR & MASK) != SLAVE_SLA_W_ACK_TRANSMITTED)											//Check the status reg if successful							
	{
		status = 1;															//Error has occurred
	}	
		
	return status;
}
/*****************************************************************************
Name:			twi_slave_ACK_tx
Purpose:		transmitters ACK to master
Parameters:		none
Returns:		Returns a 1 for NOT successful execution and 0 for successful
Note:			None
****************************************************************************/
void twi_slave_ACK_tx(void)
{	
	uint8_t status = 0;	
	
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);											
	
	while(!(TWCR &(1<<TWINT)));												//Wait till flag is cleared	
	
	return status;
}
/*****************************************************************************
Name:			twi_slave_NACK_tx
Purpose:		transmitters NACK to master
Parameters:		none
Returns:		Returns a 1 for NOT successful execution and 0 for successful
Note:			None
****************************************************************************/
void twi_slave_NACK_tx(void)
{		
	TWCR = (1<<TWINT)|(1<<TWEN)|(0<<TWEA);											
	while(!(TWCR &(1<<TWINT)));												//Wait till flag is cleared	
}
