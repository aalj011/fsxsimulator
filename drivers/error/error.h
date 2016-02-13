/*
 * error.h
 *
 * Created: 4/01/2016 2:08:39 p.m.
 *  Author: Ali
 */ 


#ifndef ERROR_H_
#define ERROR_H_
void error_handler(uint8_t status);
void error_init(ports port, uint8_t green_led_pin, uint8_t red_led_pin);
#endif /* ERROR_H_ */