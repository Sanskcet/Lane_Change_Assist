#include <avr/io.h>
#include "sensor.h"
#include "debug.h" //Include for debugging

Ultrasonic Left_Sensor = {'D','C',5,2};//Define Left Sensor  
Ultrasonic Right_Sensor = {'C','C',0,1};//Define Right Sensor 

int init_Indicators(void);
int check_Obstacle_Distance(uint16_t);
int main(){  

    DDRB |= ( 1 << PORTB0); //Set alarm pin output
    init_UART(); //Init Uart - Debugging purpose only
    init_Ultrasonic_Sensor(Left_Sensor);//Init Left Sensor
    init_Ultrasonic_Sensor(Right_Sensor);//Init Right Sensor
    init_Indicators();//Initialize interrupts on indicator pins
    while (1)
    {
        sei(); //Enable global interrupts
    }
    
    return 0;   
}

int init_Indicators(void){
    DDRD &=~ (( 1 << PD2 ) | (1 << PD3)); //Set both PD2 and PD3 as inputs
    PORTD |= (( 1 << PD2 ) | (1 << PD3)); //Setup Pullups on PD2 and PD3
    EIMSK |= ( 1 << INT1) | ( 1 << INT0 );//Setup External Interrupts on INT0 and INT;
    return 0;
}

int check_Obstacle_Distance(uint16_t range){
    if( range < 300 ){ //Check if the other vechicle is too near viz less than 300 cm
        PORTB |= ( 1 << PB0 ); //Enable alarm
    }
    else{
        PORTB &= ~( 1 << PB0 ); //disable alarm
    }
    return 0;
}

ISR(INT0_vect){
    cli();
    uint16_t  RangeLeft = 0;
    while( (PIND &  ( 1 << PIND2)) == 0 ){
        RangeLeft = get_Range(Left_Sensor); //get the obstacle distance of Right Sensor
        print_range_in_uart(RangeLeft); //print the range in uart - For debugging purpose only
        check_Obstacle_Distance(RangeLeft);//Check wheather another vechicle is nearing or not on Left Side
    }
}

ISR(INT1_vect){
    cli();
    uint16_t RangeRight = 0;
    while( (PIND &  ( 1 << PIND3)) == 0 ){
    RangeRight = get_Range(Right_Sensor); //get the obstacle distance of Left Sensor
    print_range_in_uart(RangeRight);   //print the range in uart - For debugging purpose only
    check_Obstacle_Distance(RangeRight); //Check wheather another vechicle is nearing or not on Right side
    } 
}