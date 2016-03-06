
#include "plateform.h"
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "common_defs.h"
#include "IO.h"
#include "usb_serial.h"
#include "twi.h"
#include "twi_defs.h"
#include "error.h"

static uint8_t msg_count = 0;
static uint8_t tw_status;
static sMessagePacket messagePacketHeader = {0};
static uint8_t syncByteFound = 0;

static uint8_t send_message(uint8_t bytes[]);
static uint8_t recieve_message(void);
static void cmd_setHeading(void);
static void init_variables(void);

ISR(TWI_vect)
{
	cli();
	tw_status = (TWSR & MASK);
	
	switch(tw_status)
	{
		//Successfully transmitted start condition
		case MASTER_START_TX:
		{
			twi_master_sla_send_address(messagePacketHeader.address, messagePacketHeader.control);					// broadcast slave address
			break;
		}
		/*******************************WRITE MODE**************************************/
		case MASTER_SLA_W_ACK_RX:
		{
			//init variables for sending data
			msg_count = 0;
			//send the sync bit
			twi_send_data(messagePacketHeader.syncbit);
			TWI_SendTransmit();
			break;
		}
		case MASTER_SLA_W_NACK_RX:
		{
			error_handler(SET);											// error occurred
			break;
		}
		case MASTER_DATA_TX_ACK_RX:
		{
			if(!send_message(messagePacketHeader.data))
			{
				TWI_SendStop();
			}
			else
			{
				TWI_SendTransmit();
			}
			break;
		}
		case MASTER_DATA_TX_NACK_RX:
		{
			error_handler(SET);
			break;
		}
		/*******************************************************************************/
		/*******************************READ MODE***************************************/		
		case MASTER_REPEAT_TX:
		{
			TWI_SendTransmit();
			break;
		}
		/*******************************************************************************/	
		default:
		{
			break;
		}
	}
	sei();
}

static void init_variables(void)
{
	memset(messagePacketHeader.data, 0, sizeof(messagePacketHeader.data));
	msg_count = 0;
	syncByteFound = 0;
}

static uint8_t send_message(uint8_t bytes[])
{
	uint8_t status = 0;
	
	//Send the message data only
	if(msg_count < MSG_SIZE)
	{
		//Load TWDR buffer
		twi_send_data(bytes[msg_count]);
		msg_count++;
		status = 1;	
	}
	else
	{
		msg_count =0;
	}
	
	return 	status;
}

static uint8_t recieve_message(void)
{
	uint8_t status = 0;
	
	//Check to see if the first rx data is the sync bit
	if((twi_read_data() == SYNCBIT) && (msg_count == 0) && (!syncByteFound))
	{
		syncByteFound = 1;
		status = 1;
	}
	//sync bit found ready to rx data
	else if(syncByteFound)
	{
		if(msg_count < MSG_SIZE)
		{
			messagePacketHeader.data[msg_count] = twi_read_data();
			msg_count++;
			status = 1;
		}
	}
	else
	{
		//do nothing
	}
	
	return status;
}

static void cmd_setHeading(void)
{
	//Clear the message packet
	memset(messagePacketHeader.data,0,sizeof(messagePacketHeader.data));
	
	messagePacketHeader.address = AUTOPILOT_ADDRESS;
	messagePacketHeader.control = TWI_WRITE;
	messagePacketHeader.syncbit = SYNCBIT;
	messagePacketHeader.data[0] = SET_HEADING;
	messagePacketHeader.data[1] = 0x01;
	messagePacketHeader.data[2] = 0x01;
}

int main(void)
{

	cmd_setHeading();
	sei();
	error_init(ERROR_PORT, ERROR_LED_GREEN_PIN, ERROR_LED_RED_PIN);
	twi_master_init();
	TWI_SendStart();
	
	while (1)
	{
		if((messagePacketHeader.data[0] == SET_HEADING) && (messagePacketHeader.data[1] == 0x01) && (messagePacketHeader.data[2] == 0x01))
		{
			IO_flash_slow(ERROR_PORT,ERROR_LED_GREEN_PIN);
		}
		else
		{
			IO_flash(ERROR_PORT,ERROR_LED_GREEN_PIN);
		}
		
	}
}