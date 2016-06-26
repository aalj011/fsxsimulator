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
#include "twi_ext.h"
#include "twi_defs.h"

//uint8_t bytebuffer[100] = {0};

/*
ISR(TWI_vect)
{
	cli();
	switch(TW_STATUS)
	{
		case TW_START:
		{
			msg_count = 0;
			TWI_EXT_MasterSlaveLoadAddress(messagePacketHeader.address, messagePacketHeader.control);	// broadcast slave address
			TWI_SendTransmit();

			break;
		}
		case TW_MT_SLA_ACK:
		{
			TWI_EXT_LoadData(messagePacketHeader.syncbit);
			TWI_SendTransmit();

			break;
		}
		case TW_MT_SLA_NACK:
		{
			error_handler(SET);
			break;
		}
		case TW_MT_DATA_ACK:
		{
			if(msg_count < messagePacketHeader.dataLen)
			{
				TWI_EXT_LoadData(messagePacketHeader.data[msg_count++]);
				TWI_SendTransmit();
			}
			else
			{
				TWI_SendStop();
			}
			break;
		}
		case TW_MT_DATA_NACK:
		{
			error_handler(SET);
			break;
		}
		///////////////////////
		case TW_MR_SLA_NACK:
		{
			error_handler(SET);
			break;
		}
		case TW_MR_SLA_ACK:
		{
			msg_count = 0;
			TWI_SendACK();
			break;
		}
		case TW_MR_DATA_ACK:
		{
			if(msg_count < messagePacketHeader.dataLen)
			{
				bytebuffer[msg_count++] = TWDR;
				TWI_SendACK();
			}
			else
			{
				TWI_SendNACK();
			}
			
			break;
		}
		case TW_MR_DATA_NACK:
		{
			TWI_SendStop();
			break;
		}
		case TW_SR_SLA_ACK:
		{
			init_Variables();
			TWI_SendACK();
			break;
		}
		case TW_SR_DATA_ACK:
		{
			bytebuffer[msg_count++] = TWDR;
			TWI_SendACK();
			break;
		}
		case TW_SR_DATA_NACK:
		{
			error_handler(SET);
			break;
		}
		////////////////////
		case TW_ST_SLA_ACK:
		{
			msg_count = 0;
			TWDR = 0x01;
			TWI_SendACK();
			break;
		}
		case TW_ST_DATA_ACK:
		{
			if(msg_count < messagePacketHeader.dataLen)
			{
				TWDR = (messagePacketHeader.data[msg_count++]);
				TWI_SendACK();
			}
			else
			{
				TWI_SendNACK();
			}
			break;
		}
		case TW_ST_DATA_NACK:
		{
			TWI_SendTransmit();
			break;
		}
		case TW_ST_LAST_DATA:
		{
			TWI_SendTransmit();
			break;
		}
		case TW_SR_STOP:
		{
			TWI_SendTransmit();
		}
		default:
		{
			break;
		}
	}
	sei();
}

*/

/*****************************************************************************
Name:			TWI_MasterInit
Purpose:		Initializes the twi communication for the master
Parameters:		None
Note:			None
****************************************************************************/
void TWI_EXT_MasterInit(void)
{
	//PRR0 = (0<<PRTWI);													//Turn off Power reduction on TWI 							
	TWSR = (0<<TWPS0)|(0<<TWPS1);
	TWBR = TWBR_BIT_RATE;													//Bit rate at 8mhz 100khz 0x20	
	TWDR = 0xFF;															//Dummy data
}

/*****************************************************************************
Name:			TWI_MasterSlaveLoadAddress
Purpose:		This function loads the Slave address and the read/write bit 
Parameters:		address, the address of the device slave required, a list of
				devices is found in the twi_defs.h.
				uint8_t "read" if set to 1 it will send the read address, if it is zero,
				it will send the write address.
Returns:		None
Note:			None
****************************************************************************/
void TWI_EXT_MasterSlaveLoadAddress(uint8_t address, uint8_t read)
{
	uint8_t slave_address = 0;
	slave_address = (address << 1)| (read);
	
	TWDR = slave_address;													//Set the data register with the slave address and the read/write bit
}

/*****************************************************************************
Name:			TWI_LoadData
Purpose:		This function write the data to be sent to the slave address.
Parameters:		uint8_t data to be written.
Returns:		None
Note:			None
****************************************************************************/
void TWI_EXT_LoadData(uint8_t data)
{	
	TWDR = data;	
}
/*****************************************************************************
Name:			TWI_SlaveLoadAddress
Purpose:		This function loads the slave address to TWAR
Parameters:		none
Returns:		None
Note:			None
****************************************************************************/
void TWI_EXT_SlaveLoadAddress(uint8_t slaveaddress)
{	
	TWAR = (slaveaddress<<1);																				//Load the slave addres
}
/*****************************************************************************
Name:			TWI_SlaveInit
Purpose:		Initiates the slave and initiates an ACK when addressed
Parameters:		none
Returns:		None
Note:			None
****************************************************************************/
void TWI_EXT_SlaveInit(void)
{
	TWCR = (1<<TWEA)|(1<<TWEN)|(1<<TWIE);
}
/*****************************************************************************
Name:			TWI_ReadData
Purpose:		This function reads the data from TWDR.
Parameters:		None
Returns:		None
Note:			None
****************************************************************************/
uint8_t TWI_EXT_ReadData(void)
{
	uint8_t rxdata = 0;
	rxdata = TWDR;
		
	return rxdata;
}