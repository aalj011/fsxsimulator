
#include "plateform.h"
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include "common_defs.h"
#include "IO.h"
#include "usb_serial.h"
#include "twi_ext.h"
#include "twi_defs.h"
#include "error.h"


//modes
typedef enum
{
	eInit,
	eLoadData,
	eStartTXData,
	eIdle,
}eStates;

//variables
static uint8_t msg_count;
static sMessagePacket messagePacketHeader;
static uint8_t syncByteFound;
uint8_t sendbyte;
eStates mode; 

uint8_t bytebuffer[100] = {0};

//functions
static void cmd_SetHeading(void);
static void init_Variables(void);

uint8_t data = 0;

ISR(TWI_vect)
{	
	cli();
	switch(TW_STATUS)
	{
		case TW_START:
		{
			msg_count = 0;
			TWI_EXT_MasterSlaveLoadAddress(messagePacketHeader.address, messagePacketHeader.control);	// broadcast slave address
			TWI_SendTransmit();

			break;
		}
		case TW_MT_SLA_ACK:
		{
			TWI_EXT_LoadData(messagePacketHeader.syncbit);
			TWI_SendTransmit();

			break;
		}
		case TW_MT_SLA_NACK:
		{
			error_handler(SET);
			break;
		}
		case TW_MT_DATA_ACK:
		{
			if(msg_count < messagePacketHeader.dataLen)
			{
				TWI_EXT_LoadData(messagePacketHeader.data[msg_count++]);
				TWI_SendTransmit();
			}
			else
			{
				TWI_SendStop();
			}
			break;
		}
		case TW_MT_DATA_NACK:
		{
			error_handler(SET);
			break;
		}
		///////////////////////
		case TW_MR_SLA_NACK:
		{
			error_handler(SET);
			break;
		}
		case TW_MR_SLA_ACK:
		{
			msg_count = 0;
			TWI_SendACK();
			break;
		}
		case TW_MR_DATA_ACK:
		{	
			if(msg_count < messagePacketHeader.dataLen)
			{
				bytebuffer[msg_count++] = TWDR;
				TWI_SendACK();
			}
			else
			{
				TWI_SendNACK();
			}
			
			break;
		}
		case TW_MR_DATA_NACK:
		{
			TWI_SendStop();
			break;
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

static void cmd_SetHeading(void)
{
	//Clear the message packet
	memset(messagePacketHeader.data,0,sizeof(messagePacketHeader.data));
	
	//load information on slave
	messagePacketHeader.address = AUTOPILOT_ADDRESS;
	messagePacketHeader.control = TW_READ;
	messagePacketHeader.syncbit = SYNCBIT;
	messagePacketHeader.data[0] = SET_HEADING;
	messagePacketHeader.data[1] = 0x01;
	messagePacketHeader.data[2] = 0x01;
	messagePacketHeader.dataLen = 3;
}

int main(void)
{
	sei();
	
	//init
	mode = eInit;

	while (1)
	{
		switch(mode)
		{
			case eInit:
			{
				init_Variables();																
				error_init(ERROR_PORT, ERROR_LED_GREEN_PIN, ERROR_LED_RED_PIN);								//ERROR init
				TWI_EXT_MasterInit();																			//I2C init

				mode = eLoadData;																		//next state	
				break;
			}
			case eLoadData:
			{
				cmd_SetHeading();
				mode = eStartTXData;
				break;
			}
			case eStartTXData:
			{
				TWI_SendStart();

				mode = eIdle;
				break;
			}
			case eIdle:
			{
				break;
			}
			default:
			{
				break;
			}
		}

		if((bytebuffer[0] == SET_HEADING) && (bytebuffer[1] == 0x01) && (bytebuffer[2] == 0x01))
		{
			IO_flash(ERROR_PORT,ERROR_LED_GREEN_PIN);
		}
		else
		{

			//IO_flash(ERROR_PORT,ERROR_LED_GREEN_PIN);
			IO_flash_slow(ERROR_PORT,ERROR_LED_GREEN_PIN);
		}
	}
}


/*
			if(msg_count < messagePacketHeader.dataLen)
			{
				bytebuffer[msg_count++] = TWDR;
				TWI_SendACK();
			}
			else
			{
				//TWI_SendNACK
			}
			break;


						if(msg_count < messagePacketHeader.dataLen)
						{
							TWI_EXT_LoadData(messagePacketHeader.data[msg_count++]);
							TWI_SendTransmit();
						}
						else
						{
							TWI_SendStop();
						}
						*/