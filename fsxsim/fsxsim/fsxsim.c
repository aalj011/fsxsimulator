
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

static uint8_t data[MSG_BUFFER_SIZE] = {0x00,0x00};
static uint8_t msg_count = 0;
uint8_t tw_status;
uint8_t data_temp = 0x00;

uint8_t target_address = 0;
uint8_t target_control = 0;
uint8_t target_message[MSG_BUFFER_SIZE] = {0};

ISR(TWI_vect)
{
	cli();
	tw_status = (TWSR & MASK);
	
	switch(tw_status)
	{
		//Successfully transmitted start condition
		case MASTER_START_TRANSMITTED:
		{
			twi_master_sla_send_address(target_address, target_control);					// broadcast slave address
			break;
		}
		case MASTER_REPEAT_TRANSMITTED:
		{
			error_handler(SET);
			break;
		}
		case MASTER_SLA_R_ACK_RECIEVED:
		{
			error_handler(SET);
			break;
		}
		case MASTER_SLA_R_NACK_RECIEVED:
		{
			error_handler(SET);											// error occurred
			break;
		}
		case MASTER_SLA_W_ACK_RECIEVED:
		{
			msg_count = 0;
			//Start bit 0xFF
			twi_master_send_data(START_BIT);
			break;
		}
		case MASTER_SLA_W_NACK_RECIEVED:
		{
			error_handler(SET);											// error occurred
			break;
		}
		case MASTER_DATA_TX_ACK_RECIEVED:
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
		case MASTER_DATA_TX_NACK_RECIEVED:
		{
			twi_master_stop_condition();
			error_handler(SET);	
			break;
		}
		case MASTER_DATA_RX_NACK_TRANSMITTED:
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
	target_address = AUTOPILOT_ADDRESS;
	target_control = TWI_WRITE;
	target_message[0] = 0x01;
	target_message[1] = 0x01;
	
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