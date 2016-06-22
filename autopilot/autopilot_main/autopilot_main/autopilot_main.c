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
#include <util/twi.h>

#include "IO.h"
#include "twi_ext.h"
#include "twi_defs.h"
#include "error.h"
#include "common_defs.h"

uint8_t tw_status;
uint8_t msg_count;
uint8_t syncByteFound = 0;
uint8_t bytebuffer[100] = {0};
static sMessagePacket messagePacketHeader = {0};

static void init_Variables(void);

ISR(TWI_vect)
{
	cli();
	switch(TW_STATUS)
	{
		case TW_SR_SLA_ACK:
		{
			init_Variables();
			TWI_SendACK();
			break;
		}	
		case TW_SR_DATA_ACK:
		{
			bytebuffer[msg_count++] = TWDR;
			TWI_SendACK();
			break;
		}
		case TW_SR_DATA_NACK:
		{
			error_handler(SET);
			break;
		}
		case TW_ST_SLA_ACK:
		{
			error_handler(SET);
			break;
		}
		case TW_ST_DATA_ACK:
		{
			error_handler(SET);
			break;
		}
		case TW_ST_DATA_NACK:
		{
			error_handler(SET);
			break;
		}
		case TW_SR_STOP:
		{
			TWI_SendTransmit();
		}
		default:
		{
			break;
		}
	}
	sei();
}

static void init_Variables(void)
{
	memset(messagePacketHeader.data, 0, sizeof(messagePacketHeader.data));
	msg_count = 0;
	syncByteFound = 0;

}

int main(void)
{	
	sei();
	error_init(ERROR_PORT, ERROR_LED_GREEN_PIN, ERROR_LED_RED_PIN);

	TWI_EXT_SlaveLoadAddress(AUTOPILOT_ADDRESS);
	TWI_EXT_SlaveInit();
	
	while (1)
	{
		if((bytebuffer[0] == SYNCBIT) && (bytebuffer[1] = SET_HEADING) && (bytebuffer[2] == 0x01) && (bytebuffer[3]= 0x01))
		{
			IO_flash(ERROR_PORT,ERROR_LED_GREEN_PIN);
		}
		else
		{
			IO_flash_slow(ERROR_PORT,ERROR_LED_GREEN_PIN);
		}
		
	}
}
	
	

	
		
		