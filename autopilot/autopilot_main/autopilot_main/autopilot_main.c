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


ISR(TWI_vect)
{
	cli();
	tw_status = (TWSR & MASK);
	
	switch(tw_status)
	{	
		case SLAVE_SLA_W_ACK_TX:
		{
			//Initilise the message receive packet
			msg_count = 0;
			syncByteFound = 0;
			messagePacketHeader.data[0] = 0x00;
			messagePacketHeader.data[1] = 0x00;
			messagePacketHeader.data[2] = 0x00;
			twi_clear_twint();
			break;
		}	
		case SLAVE_DATA_RX_ACK_TX:
		{
			if(recieve_message())
			{
				twi_slave_tx_ack();
			}
			else
			{
				twi_slave_tx_nack();
			}
			break;
		}
		case SLAVE_BUS_ERROR:
		{
			twi_slave_init(AUTOPILOT_ADDRESS);	
			break;
		}
		default:
		{
			twi_clear_twint();
			break;
		}
	}
	sei();
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

int main(void)
{	
	sei();
	error_init(ERROR_PORT, ERROR_LED_GREEN_PIN, ERROR_LED_RED_PIN);
	twi_slave_init(AUTOPILOT_ADDRESS);
	
	while (1)
	{
		if((messagePacketHeader.data[0] == SET_HEADING) && (messagePacketHeader.data[1] == 0x01) && (messagePacketHeader.data[2] == 0x01))
		{
			IO_write(ERROR_PORT,ERROR_LED_GREEN_PIN,SET);
		}
		else
		{
			IO_flash(ERROR_PORT,ERROR_LED_GREEN_PIN);
		}
		
	}
}
	
	

	
		
		