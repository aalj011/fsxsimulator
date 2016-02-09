
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdio.h>
#include "/Projects/Projects/fsxsimulator/fsxsimulator/drivers/IO/IO.h"
#include "/Projects/Projects/fsxsimulator/fsxsimulator/drivers/usb_serial/usb_serial.h"
#include "/Projects/Projects/fsxsimulator/fsxsimulator/drivers/twi/twi.h"
#include "/Projects/Projects/fsxsimulator/fsxsimulator/drivers/twi/twi_defs.h"
#include "/Projects/Projects/fsxsimulator/fsxsimulator/drivers/error/error.h"

//Issues the read is not working
//the while loop for read halts and doesnt go outside of it
//need to look at how to do a read

int main(void)
{
	uint8_t slaveaddy = (0x01);
	uint8_t data = 0x01;
	
	//CPU_PRESCALE(0);
	error_init(PORT_C);	
	twi_master_init();
	
	error_handler(twi_master_start_condition());
	error_handler(twi_master_sla_sendAddress(slaveaddy, 0));
	error_handler(twi_master_sendData(data));
	error_handler(twi_master_stop_condition());
	
	
	while (1) {
	}
	
}