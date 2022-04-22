#include "debug.h"

/**
 * @brief This function is used to debug the program using uart by printing the distance, Timer overflow and ticks taken by the timer with respect to the pulse width
 * 
 * @param range 
 * @param Timer_Overflow 
 * @param ticks 
 * @return int 
 */
int debug_using_uart(uint16_t range, uint8_t Timer_Overflow, uint8_t ticks){ 
    char TimerOV_String[5];
    char Ticks_String[5];
    char Range_String[5];
    putStringtoUART(" OV: "); 
    itoa(Timer_Overflow,TimerOV_String,10); //convert integer to string 
    putStringtoUART(TimerOV_String); 
    putStringtoUART(" Ticks: ");
    itoa(ticks,Ticks_String,10);
    putStringtoUART(Ticks_String); 
    putStringtoUART(" Range: ");
    itoa(range,Range_String,10);
    putStringtoUART(Range_String);     
    putStringtoUART("\n");
    return 0;
}

/**
 * @brief This function prints the range as measured by the ultrasonic sensor
 * 
 * @param range 
 * @return int 
 */
int print_range_in_uart(uint16_t range){
    char Range_String[5];
    putStringtoUART(" Range: ");
    itoa(range,Range_String,10);
    putStringtoUART(Range_String);     
    putStringtoUART("\n");
    return 0;
}