#ifndef UART
#define UART

#include <avr/io.h>
#include <stdlib.h>
//Definition Macros for setting Baud Rate
#define FREQ 16000000UL
#define BAUDRATE 9600
#define BAUDRATE_PRESCALLER (((FREQ / (BAUDRATE * 16UL))) - 1)

void init_UART();
void putStringtoUART(char*);


#endif