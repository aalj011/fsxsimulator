/*
 * common.h
 *
 * Created: 4/01/2016 2:08:39 p.m.
 *  Author: Ali
 */ 

#ifndef COMMON_DEFS_H_
#define COMMON_DEFS_H_

typedef enum
{
	init,
	startup,
	running,
}states;
#define SET 1
#define CLEAR 0
#define AUTOPILOT_ADDRESS 0x01
#define START_BIT 0xFF

#endif /* COMMON_DEFS_H_ */