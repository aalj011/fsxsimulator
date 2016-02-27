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
sMessagePacket messagePacketHeader;

static uint8_t recieve_message(void);

ISR(TWI_vect)
{
	cli();
	tw_status = (TWSR & MASK);
	
	switch(tw_status)
	{	
		case SLAVE_SLA_W_ACK_TX:
		{
			msg_count = 0;
			twi_slave_tx_ack();
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
	if(msg_count < MSG_SIZE)
	{
		if(msg_count == 0)
		{
			messagePacketHeader.command = (eCommands)TWDR;
		}
		else
		{
			messagePacketHeader.data[msg_count] = (uint8_t)TWDR;
		}
		msg_count++;
		status = 1;
	}
	else
	{
		msg_count = 0;
		status = 0;
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
		if((messagePacketHeader.data[0] == 0x01) && (messagePacketHeader.data[1] == 0x01))
		{
			IO_write(ERROR_PORT,ERROR_LED_GREEN_PIN,SET);
		}
		else
		{
			IO_flash(ERROR_PORT,ERROR_LED_GREEN_PIN);
		}
		
	}
}
	
	
	
	
	
/*
if(data_rx_count <= DATA_BUFFER_SIZE)
{
	databuffer[data_rx_count] = TWDR;
	data_rx_count++;
				
	twi_slave_tx_ack();
}
else
{
	error_handler(SET);	
	twi_slave_tx_nack();
}
*/	
			
	
		
		