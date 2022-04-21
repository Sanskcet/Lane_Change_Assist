#include "debug.h"

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
int print_range_in_uart(uint16_t range){
    char Range_String[5];
    putStringtoUART(" Range: ");
    itoa(range,Range_String,10);
    putStringtoUART(Range_String);     
    putStringtoUART("\n");
    return 0;
}