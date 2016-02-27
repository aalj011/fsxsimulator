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
}estates;

typedef struct  
{
	uint8_t address,
	uint8_t control,
	uint8_t start_bit,
	struct sMessage message,
}sMessagePacket;

typedef struct  
{
	eCommands command,
	uint8_t data[MESSAGE_DATA_SIZE] 
}sMessage;

typedef enum
{
	SET_HEADING,
	GET_HEADING,
}eCommands;

#define SET 1
#define CLEAR 0
#define AUTOPILOT_ADDRESS 0x01
#define START_BIT 0xFF
#define MSG_SIZE 
#define COMMAND_DATA_SIZE 2

#endif /* COMMON_DEFS_H_ */