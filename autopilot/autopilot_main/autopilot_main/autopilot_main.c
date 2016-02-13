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
#include "common.h"

static uint8_t databuffer[DATABUFFER] = {0};
static uint8_t data_rx_count =0;	
static bool recieved_data(uint8_t data);


ISR(TWI_vect)
{
	uint8_t tw_status = TWSR & MASK;
	switch(tw_status)
	{	
		case SLAVE_SLA_W_ACK_TX:
		{
			TWCR |= (1<<TWINT);
			break;
		}	
		case SLAVE_DATA_RX:
		{
			if(i< DATABUFFER)
			{
				databuffer[i] = TWDR;
				i++;
				twi_slave_ACK_tx();
			}
			else
			{
				twi_slave_NACK_tx();
				i =0;
			}
			break;
		}
		
		case SLAVE_DATA_RX_NACK:
		{
			error_handler(SET);
			break;
		}	
		
		default:
		{
			break;
		}
	}
}

bool recieved_data(uint8_t data)
{
	int i = 0;
	if(i< DATABUFFER)
	{
		databuffer[i] = TWDR;
		i++;
		twi_slave_ACK_tx();
	}
}	

int main(void)
{
	sei();									// enable interrupt
	error_init(ERROR_PORT, ERROR_LED_GREEN_PIN, ERROR_LED_RED_PIN);
	twi_slave_init(AUTOPILOT_ADDRESS);
	
    while(1)
    {
		if(databuffer[0])
		{
			IO_write(PORT_D, 5, SET);
		}
		else
		{
			IO_write(PORT_D, 5, CLEAR);	
		}
	}
}			
	

