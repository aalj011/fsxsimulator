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
#define SYNCBIT 0xB6
#define MSG_SIZE 3

typedef enum
{
	init,
	startup,
	running,
}estates;

typedef enum
{
	NONE,
	SET_HEADING,
	GET_HEADING,
}eCommands;

typedef struct  
{
	uint8_t address;
	uint8_t control;
	uint8_t syncbit;
	uint8_t data[MSG_SIZE];	
	uint8_t dataLen;
}sMessagePacket;


#endif /* COMMON_DEFS_H_ */