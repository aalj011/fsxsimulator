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
#include "IO.h"
#include "common.h"

//Local variables
static ports error_port;
static uint8_t error_green_led_pin;
static uint8_t error_red_led_pin;

void error_init(ports port, uint8_t green_led_pin, uint8_t red_led_pin)
{
	error_port = port;
	error_red_led_pin = red_led_pin;
	error_green_led_pin = green_led_pin;
	
	// clear the io required
	IO_write(error_port,error_green_led_pin,SET);		// green light
	IO_write(error_port,error_red_led_pin,CLEAR);		// red light
}


void error_handler(uint8_t status)
{
	// clear the io required
	IO_write(error_port,error_green_led_pin,CLEAR);
	IO_write(error_port,error_red_led_pin,CLEAR);

	if(status)
	{
		// error has been generated stay in this while loop
		while(1)
		{
			IO_flash(error_port,error_red_led_pin);
		}
	}
	else
	{
		// no error generated
		IO_write(error_port,error_green_led_pin,SET);
	}
}