
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
uint8_t tw_status;
static uint8_t send_message(uint8_t bytes[]);

sMessagePacket messagePacketHeader;

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
		case MASTER_REPEAT_TX:
		{
			error_handler(SET);
			break;
		}
		case MASTER_SLA_R_ACK_RX:
		{
			error_handler(SET);
			break;
		}
		case MASTER_SLA_R_NACK_RX:
		{
			error_handler(SET);											// error occurred
			break;
		}
		case MASTER_SLA_W_ACK_RX:
		{
			msg_count = 0;
			//command type
			twi_send_data(messagePacketHeader.command);
			break;
		}
		case MASTER_SLA_W_NACK_RX:
		{
			error_handler(SET);											// error occurred
			break;
		}
		case MASTER_DATA_TX_ACK_RX:
		{
			
			//Send the message data
			if(!send_message(messagePacketHeader.data))
			{
				twi_master_stop_condition();
			}			
			break;
		}
		case MASTER_DATA_TX_NACK_RX:
		{
			twi_master_stop_condition();
			error_handler(SET);	
			break;
		}
		case MASTER_DATA_RX_NACK_TX:
		{
			error_handler(SET);													// error occurred	
			break;
		}
		default:
		{
			break;
		}
	}
	sei();
}

static uint8_t send_message(uint8_t bytes[])
{
	uint8_t status = 0;
	
	//Send the message data only
	if(msg_count < MSG_SIZE)
	{
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
	twi_master_init();
	twi_master_start_condition();

	
	//Send message to Autopilot
	messagePacketHeader.address = AUTOPILOT_ADDRESS;
	messagePacketHeader.control = TWI_WRITE; 
	messagePacketHeader.start_bit = START_BIT;
	messagePacketHeader.command = SET_HEADING;
	messagePacketHeader.data[0] = 0x01;
	messagePacketHeader.data[1] = 0x01;
	
	while (1) 
	{
		IO_flash(ERROR_PORT,ERROR_LED_GREEN_PIN);
	}
}