/*
 * common.h
 *
 * Created: 4/01/2016 2:08:39 p.m.
 *  Author: Ali
 */ 

#ifndef COMMON_DEFS_H_
#define COMMON_DEFS_H_

#define SET 1
#define CLEAR 0
#define AUTOPILOT_ADDRESS 0x01
#define START_BIT 0xFF
#define MSG_SIZE 2
#define COMMAND_DATA_SIZE 2

typedef enum
{
	init,
	startup,
	running,
}estates;

typedef enum
{
	SET_HEADING,
	GET_HEADING,
}eCommands;

typedef struct  
{
	uint8_t address;
	uint8_t control;
	uint8_t start_bit;
	eCommands command;
	uint8_t data[COMMAND_DATA_SIZE];	
}sMessagePacket;


#endif /* COMMON_DEFS_H_ */