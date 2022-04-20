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

typedef struct{
    char Trig_Port;
    char Echo_Port;
    uint8_t Trig_pin;
    uint8_t Echo_pin;
}Ultrasonic;

int init_Ultrasonic_Sensor( Ultrasonic );
double get_Range( Ultrasonic );
int create_delay_for_Ultrasonic_Sensor ( void );
int Trigger ( Ultrasonic );
uint16_t Echo_Time( void );
int set_Interrupts( Ultrasonic );
#endif