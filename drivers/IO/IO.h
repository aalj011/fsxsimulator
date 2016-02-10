/*
 * IO.h
 *
 * Created: 4/01/2016 2:08:39 p.m.
 *  Author: Ali
 */ 


#ifndef IO_H_
#define IO_H_

typedef enum
{
	PORT_B,
	PORT_C,
	PORT_D,
	PORT_E,
	PORT_F
}ports;

uint8_t IO_read(ports port, uint8_t pinnumber);
void IO_write(ports port, uint8_t pinnumber, uint8_t state);
void IO_flash(ports port, uint8_t pinnumber);

#endif /* IO_H_ */