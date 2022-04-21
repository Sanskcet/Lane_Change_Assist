/**
 * @file sensor.h
 * @author Sanjeeve R (18euee122@skcet.ac.in)
 * @brief This header file contains the struct definition used for easy access of the Ultrasonic sensor. Also it contains the functions required to use the sensor at ease.
 * @version 0.1
 * @date 2022-04-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef SENSOR
#define SENSOR

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>

//User defined data type for Ultrasonic sensor
typedef struct{
    char Trig_Port;
    char Echo_Port;
    uint8_t Trig_pin:4; //since atmega328 has maximum 7 pins on each port, 7 can be represented as 0111 in binary and hence 3 bits are sufficient. 
    uint8_t Echo_pin:4;
}Ultrasonic;


int init_Ultrasonic_Sensor( Ultrasonic );
uint16_t get_Range( Ultrasonic );
int create_delay_for_Ultrasonic_Sensor ( void );
int Trigger ( Ultrasonic );
uint16_t Echo_Time( void );
int set_Interrupts( Ultrasonic );

#endif