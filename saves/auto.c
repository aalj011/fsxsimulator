/*
 * autopilot_main.c
 *
 * Created: 25/01/2016 9:49:39 p.m.
 *  Author: Ali
 */ 

#include "plateform.h"
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "IO.h"
#include "twi.h"
#include "twi_defs.h"
#include "error.h"
#include "common_defs.h"

uint8_t tw_status;
uint8_t msg_count;
uint8_t syncByteFound = 0;
static sMessagePacket messagePacketHeader = {0};

static uint8_t recieve_message(void);
static void init_variables(void);
static uint8_t send_message(uint8_t bytes[]);

ISR(TWI_vect)
{
	cli();
	tw_status = (TWSR & MASK);
	
	switch(tw_status)
	{
		/*******************************WRITE MODE**************************************/	
		case SLAVE_SLA_W_ACK_TX:
		{
			init_variables();
			TWI_SendACK();
			break;
		}	
		case SLAVE_DATA_RX_ACK_TX:
		{
			if(recieve_message())
			{
				TWI_SendACK();
			}
			else
			{
				TWI_SendNACK();
			}
			break;
		}
		case SLAVE_DATA_RX_NACK_TX:
		{
			init_variables();
			error_handler(SET);
		}
		/*******************************************************************************/
		/*******************************READ MODE***************************************/
		case SLAVE_SLA_R_ACK_TX:
		{
			send_message(SYNCBIT);
			TWI_SendNACK();
			break;
		}
		case SLAVE_DATA_TX_ACK_RX:
		{
			TWI_SendTransmit();
			break;
		}
		case SLAVE_DATA_TX_NACK_RX:
		{
			TWI_SendTransmit();
			//error_handler(SET);
			break;
		}
		/*******************************************************************************/
		default:
		{
			twi_slave_init(AUTOPILOT_ADDRESS);
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
		if(msg_count <= MSG_SIZE)
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

int main(void)
{	
	sei();
	error_init(ERROR_PORT, ERROR_LED_GREEN_PIN, ERROR_LED_RED_PIN);
	twi_slave_init(AUTOPILOT_ADDRESS);
	
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
	
	

	
		
		