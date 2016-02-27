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

static uint8_t msg_rx[DATA_BUFFER_SIZE] = {0};
static uint8_t datarx_count =0;	
uint8_t tw_status;


ISR(TWI_vect)
{
	cli();
	tw_status = (TWSR & MASK);
	
	switch(tw_status)
	{	
		case SLAVE_SLA_W_ACK_TX:
		{
			datarx_count = 0;
			twi_clear_twint();
			break;
		}	
		case SLAVE_DATA_RX_ACK_TX:
		{
			if(datarx_count < DATA_BUFFER_SIZE)
			{
				msg_rx[datarx_count] = TWDR;
				datarx_count++;
				twi_slave_tx_ack();
			}
			else
			{
				datarx_count = 0;
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

int main(void)
{	
	sei();
	error_init(ERROR_PORT, ERROR_LED_GREEN_PIN, ERROR_LED_RED_PIN);
	twi_slave_init(AUTOPILOT_ADDRESS);

	while (1)
	{
		if((msg_rx[0] == 0xFF) && (msg_rx[1] == 0x01))
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
			
	
		
		