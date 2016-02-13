
#include "plateform.h"
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "IO.h"
#include "usb_serial.h"
#include "twi.h"
#include "twi_defs.h"
#include "error.h"
#include "common.h"

static uint8_t data = 0x01;

ISR(TWI_vect)
{
	uint8_t tw_status = TWSR & MASK;
	switch(tw_status)
	{
		//Successfully transmitted start condition
		case MASTER_START_TRANSMITTED:
		{
			twi_master_sla_sendAddress(AUTOPILOT_ADDRESS, TWI_READ);					// broadcast slave address
			break;
		}
		case MASTER_REPEAT_TRANSMITTED:
		{
			break;
		}
		case MASTER_SLA_R_ACK_RECIEVED:
		{
			break;
		}
		case MASTER_SLA_R_NACK_RECIEVED:
		{
			error_handler(SET);											// error occurred
			break;
		}
		case MASTER_SLA_W_ACK_RECIEVED:
		{
			TWCR |= (1<<TWINT);
			break;
		}
		case MASTER_SLA_W_NACK_RECIEVED:
		{
			error_handler(SET);											// error occurred
			break;
		}
		case MASTER_DATA_TX_ACK_RECIEVED:
		{
			twi_master_sendData(data);
			twi_master_stop_condition();
			break;
		}
		case MASTER_DATA_TX_NACK_RECIEVED:
		{
			error_handler(SET);
			break;
		}
		case MASTER_DATA_RX_NACK_TRANSMITTED:
		{
			error_handler(SET);													// error occurred	
			//*dataptr = TWDR;
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
	sei();
	error_init(ERROR_PORT, ERROR_LED_GREEN_PIN, ERROR_LED_RED_PIN);	
	twi_master_init();
	twi_master_start_condition();
	
	while (1) {
	}
	
}