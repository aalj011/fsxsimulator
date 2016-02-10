/*
 * autopilot_main.c
 *
 * Created: 25/01/2016 9:49:39 p.m.
 *  Author: Ali
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "drivers/plateform.h"
#include "/Projects/Projects/fsxsimulator/fsxsimulator/drivers/IO/IO.h"
#include "/Projects/Projects/fsxsimulator/fsxsimulator/drivers/twi/twi.h"
#include "/Projects/Projects/fsxsimulator/fsxsimulator/drivers/twi/twi_defs.h"
#include "/Projects/Projects/fsxsimulator/fsxsimulator/drivers/error/error.h"

static uint8_t databuffer[DATABUFFER] = {0};
static uint8_t tw_status = 0;
static uint8_t i =0;	


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
			error_handler(1);
			break;
		}	
		
		default:
		{
			break;
		}
	}
}	

int main(void)
{
	sei();									// enable interrupt
	error_init(ERROR_PORT);
	twi_slave_init(AUTOPILOT_ADDRESS);
	
    while(1)
    {
		if(databuffer[0])
		{
			IO_write(PORT_D, 5, 1);
		}
		else
		{
			IO_write(PORT_D, 5, 0);	
		}
	}
}			
	

