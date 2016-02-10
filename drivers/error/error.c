
/*
 * error.c
 *
 * Created: 25/01/2016 9:49:39 p.m.
 *  Author: Ali
 */ 

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdio.h>
#include "/Projects/Projects/fsxsimulator/fsxsimulator/drivers/IO/IO.h"

static ports error_port = PORT_C;

void error_init(ports port)
{
	error_port = port;
	
	// clear the io required
	IO_write(error_port, 6,1); // green lgiht
	IO_write(error_port,7,0);  // red light
}


void error_handler(uint8_t status)
{
	// clear the io required
	IO_write(error_port, 6,0);
	IO_write(error_port,7,0);

	if(status)
	{
		// error has been generated stay in this while loop
		while(1)
		{
			IO_flash(error_port,7);
		}
	}
	else
	{
		// no error generated
		IO_write(error_port,6,1);
	}
}