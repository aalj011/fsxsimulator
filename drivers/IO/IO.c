/*
 * IO.c
 *
 * Created: 4/01/2016 2:08:28 p.m.
 *  Author: Ali
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include "IO.h"
#include "common.h"

/*****************************************************************************
Name:			IO_write
Purpose:		sets the pin on a port to an output with the required state
Parameters:		port name, char which specifies with port name to address must be
				in capital letters. pinnumber specifies which pin to address
				state specifies whether we want to set the pin high or low.
Note:			None
*****************************************************************************/
void IO_write(ports port, uint8_t pinnumber,uint8_t state)
{
	switch (port)
	{
		case PORT_B:
		{
			#ifdef PORTB
			//set the pinnumber as output
			DDRB |= (1 << pinnumber);
			if(state)
			{
				PORTB |= (1 << pinnumber);
			}
			else
			{
				PORTB &= ~(1<< pinnumber);
			}
			#endif			
			break;
		}
		case PORT_C:
		{
			#ifdef PORTC
			//set the pinnumber as output
			DDRC |= (1 << pinnumber);
			if(state)
			{
				PORTC |= (1 << pinnumber);
			}
			else
			{
				PORTC &= ~(1<< pinnumber);
			}
			#endif				
			break;
		}
		case PORT_D:
		{
			#ifdef PORTD
			//set the pinnumber as output
			DDRD |= (1 << pinnumber);
			if(state)
			{
				PORTD |= (1 << pinnumber);
			}
			else
			{
				PORTD &= ~(1<< pinnumber);
			}
			#endif			
			break;
		}
		case PORT_E:
		{
			#ifdef PORTE
			//set the pinnumber as output
			DDRE |= (1 << pinnumber);
			if(state)
			{
				PORTE |= (1 << pinnumber);
			}
			else
			{
				PORTE &= ~(1<< pinnumber);
			}
			#endif			
			break;
		}
		case PORT_F:
		{
			#ifdef PORTF
			//set the pinnumber as output
			DDRF |= (1 << pinnumber);
			if(state)
			{
				PORTF |= (1 << pinnumber);
			}
			else
			{
				PORTF &= ~(1<< pinnumber);
			}
			#endif		
			break;
		}				
		default:
		{
			break;
		}
	}
}

/*****************************************************************************
Name:			IO_read
Purpose:		Read the required pin on the micro at a specific port
Parameters:		port this specifies the port required
				pinnumber specifies which pin we want to address (to read)
Returns:		returns a 1 or 0 depending on the state of the pin read
*****************************************************************************/
uint8_t IO_read(ports port, uint8_t pinnumber)
{
	uint8_t pinvalue = 0;
	
	switch (port)
	{
		case PORT_B:
		{
			#ifdef PORTB	
			//set pin as input
			DDRB &= ~(1 << pinnumber);
			//enable pull up resistor
			PORTB |= (1 << pinnumber);
			//read the value required
			pinvalue = (1 << pinnumber)&(PINB);	
			#endif				
			break;
		}
		case PORT_C:
		{
			#ifdef PORTC
			//set pin as input
			DDRC &= ~(1 << pinnumber);
			//enable pull up resistor
			PORTC |= (1 << pinnumber);
			//read the value required
			pinvalue = (1 << pinnumber)&(PINC);	
			#endif						
			break;
		}
		case PORT_D:
		{
			#ifdef PORTD
			//set pin as input
			DDRD &= ~(1 << pinnumber);
			//enable pull up resistor
			PORTD |= (1 << pinnumber);
			//read the value required
			pinvalue = (1 << pinnumber)&(PIND);
			#endif					
			break;
		}
		case PORT_E:
		{
			#ifdef PORTE
			//set pin as input
			DDRE &= ~(1 << pinnumber);
			//enable pull up resistor
			PORTE |= (1 << pinnumber);
			//read the value required
			pinvalue = (1 << pinnumber)&(PINE);	
			#endif		
			break;
		}
		case PORT_F:
		{
			#ifdef PORTF
			//set pin as input
			DDRF &= ~(1 << pinnumber);
			//enable pull up resistor
			PORTF |= (1 << pinnumber);
			//read the value required
			pinvalue = (1 << pinnumber)&(PINF);	
			#endif			
			break;
		}				
		default:
		{
			break;
		}
	}
	
	return pinvalue;
}
/*****************************************************************************
Name:			IO_flash
Purpose:		Flashes the IO of choosing (only once)
Parameters:		port this specifies the port required to flash and Pin number
Returns:		None
*****************************************************************************/
void IO_flash(ports port, uint8_t pinnumber)
{
	IO_write(port, pinnumber, CLEAR);
	_delay_ms(2000);
	IO_write(port, pinnumber, SET);
	_delay_ms(2000);
}