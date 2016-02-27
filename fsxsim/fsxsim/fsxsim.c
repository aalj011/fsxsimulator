
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

ISR(TWI_vect)
{
	cli();
	tw_status = (TWSR & MASK);
	
	switch(tw_status)
	{
		//Successfully transmitted start condition
		case MASTER_START_TX:
		{
			twi_master_sla_send_address(sMessagePacket.address, sMessagePacket.control);					// broadcast slave address
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
			//Start bit 0xFF
			twi_master_send_data(START_BIT);
			break;
		}
		case MASTER_SLA_W_NACK_RX:
		{
			error_handler(SET);											// error occurred
			break;
		}
		case MASTER_DATA_TX_ACK_RX:
		{
			//Send the message
			if(msg_count < MSG_BUFFER_SIZE)
			{
				twi_master_send_data(target_message[msg_count]);
				msg_count++;
			}
			else
			{
					msg_count =0;
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


int main(void)
{
	sei();
	error_init(ERROR_PORT, ERROR_LED_GREEN_PIN, ERROR_LED_RED_PIN);
	twi_master_init();
	twi_master_start_condition();
	
	//Send message to Autopilot
	sMessagePacket.address = AUTOPILOT_ADDRESS;
	sMessagePacket.control = TWI_WRITE; 
	sMessagePacket.start_bit = START_BIT;
	sMessagePacket.message.command = SET_HEADING;
	sMessagePacket.message.data[0] = 0x01;
	sMessagePacket.message.data[1] = 0x01;
	
	while (1) 
	{
		IO_flash(ERROR_PORT,ERROR_LED_GREEN_PIN);
	}
}


			/*
			if(msg_count == 0)
			{
				twi_master_send_data(data[0]);
				msg_count++;
			}
			else
			{
				twi_master_stop_condition();
			}
			
			*/
			//twi_clear_twint();