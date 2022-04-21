#include <avr/io.h>
#include "sensor.h"
#include "debug.h" //Include for debugging

int init_Indicators(void);

int main(){  

    uint16_t  Range2 = 0, Range1 = 0;
    Ultrasonic S1 = {'D','C',5,2};//Define Left Sensor  
    Ultrasonic S2 = {'C','C',0,1};//Define Right Sensor 
    DDRB |= ( 1 << PORTB0); //Set alarm pin output
    //init_UART(); //Init Uart - Debugging purpose only
    init_Ultrasonic_Sensor(S1);//Init Left Sensor
    init_Ultrasonic_Sensor(S2);//Init Right Sensor
    init_Indicators();
    while (1)
    {
        Range1 = get_Range(S1); //get the obstacle distance of Right Sensor
        Range2 = get_Range(S2); //get the obstacle distance of Left Sensor
        print_range_in_uart(Range1); //print the range in uart - For debugging purpose only
        print_range_in_uart(Range2); 
        if ( Range2 > 20 ){
            PORTB |= ( 1 << PORTB0 );
        }
        else { 
            PORTB &=~ ( 1 << PORTB0 );
        }     
    }
    
    return 0;   
}
int init_Indicators(void){
    
    return 0;
}