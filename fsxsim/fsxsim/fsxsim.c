
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "drivers/plateform.h"
#include "/Projects/Projects/fsxsimulator/fsxsimulator/drivers/IO/IO.h"
#include "/Projects/Projects/fsxsimulator/fsxsimulator/drivers/usb_serial/usb_serial.h"
#include "/Projects/Projects/fsxsimulator/fsxsimulator/drivers/twi/twi.h"
#include "/Projects/Projects/fsxsimulator/fsxsimulator/drivers/twi/twi_defs.h"
#include "/Projects/Projects/fsxsimulator/fsxsimulator/drivers/error/error.h"


static uint8_t data = 0x01;
static uint8_t slaveaddy = 0x01;

ISR(TWI_vect)
{
	uint8_t tw_status = TWSR & MASK;
	switch(tw_status)
	{
		//Successfully transmitted start condition
		case MASTER_START_TRANSMITTED:
		{
			twi_master_sla_sendAddress(slaveaddy, 0);					// broadcast slave address
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
			error_handler(1);											// error occurred
			break;
		}
		case MASTER_SLA_W_ACK_RECIEVED:
		{
			TWCR |= (1<<TWINT);
			break;
		}
		case MASTER_SLA_W_NACK_RECIEVED:
		{
			error_handler(1);											// error occurred
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
			error_handler(1);
			break;
		}
		case MASTER_DATA_RX_NACK_TRANSMITTED:
		{
			error_handler(1);													// error occurred	
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
	//CPU_PRESCALE(0);
	sei();
	error_init(ERROR_PORT);	
	twi_master_init();
	twi_master_start_condition();
	
	while (1) {
	}
	
}