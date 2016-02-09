/*
 * autopilot_main.c
 *
 * Created: 25/01/2016 9:49:39 p.m.
 *  Author: Ali
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "/Projects/Projects/fsxsimulator/fsxsimulator/drivers/IO/IO.h"
#include "/Projects/Projects/fsxsimulator/fsxsimulator/drivers/twi/twi.h"
#include "/Projects/Projects/fsxsimulator/fsxsimulator/drivers/twi/twi_defs.h"
#include "/Projects/Projects/fsxsimulator/fsxsimulator/drivers/error/error.h"

#define AUTOPILOT_ADDRESS 0x01
#define DATABUFFER	10

static uint8_t databuffer[DATABUFFER] = {0};
static uint8_t tw_status = 0;
static uint8_t i =0;	
	
void i2c_handler(void)
{
	tw_status = TWSR & MASK;
	switch(tw_status)
	{
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
	}	
}


int main(void)
{
	error_init(PORT_D);
	error_handler(twi_slave_init(0x01));
	twi_slave_ACK_tx();

    while(1)
    {
		i2c_handler();
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
	

