#include <avr/io.h>
#include "sensor.h" //For Ultrasonic Sensor
#include "debug.h" //Include for debugging else comment it

Ultrasonic Left_Sensor = {'D','C',5,2};//Define Left Sensor  
Ultrasonic Right_Sensor = {'C','C',0,1};//Define Right Sensor 
unsigned short int_flag = 0;

int init_Indicators(void);
int check_Obstacle_Distance(uint16_t);
int init_Sleep_Mode();

ISR(INT0_vect){
    cli();
    int_flag = 1;
}

ISR(INT1_vect){
    cli();
    int_flag = 2;
}


int main(){  

    uint16_t  RangeLeft = 0;
    uint16_t RangeRight = 0;

    DDRB |= ( 1 << PORTB0); //Set alarm pin output
    init_Ultrasonic_Sensor(Left_Sensor);//Init Left Sensor
    init_Ultrasonic_Sensor(Right_Sensor);//Init Right Sensor
    init_Indicators();//Initialize interrupts on indicator pins
    init_Sleep_Mode();//Initializes sleep mode
    while (1)
    {
        sei(); //Enable global interrupts
        SMCR |= ( 1 << SE );  //Enable Sleep  
        PRR |= 0b11101111; //Enable all power reduction techniques    
        init_UART(); //Init Uart - Debugging purpose only
        if( int_flag == 1){
            while( (PIND &  ( 1 << PIND2)) == 0 ){
                RangeLeft = get_Range(Left_Sensor); //get the obstacle distance of Right Sensor
                print_range_in_uart(RangeLeft); //print the range in uart - For debugging purpose only
                check_Obstacle_Distance(RangeLeft);//Check wheather another vechicle is nearing or not on Left Side
            }
            int_flag = 0;
        }
        else if ( int_flag == 2){
            while( (PIND &  ( 1 << PIND3)) == 0 ){
                RangeRight = get_Range(Right_Sensor); //get the obstacle distance of Left Sensor
                print_range_in_uart(RangeRight);   //print the range in uart - For debugging purpose only
                check_Obstacle_Distance(RangeRight); //Check wheather another vechicle is nearing or not on Right side
            }
            int_flag = 0;
        }
        PORTB &= ~( 1 << PB0 ); //disable alarm

    }
    
    return 0;   
}


int init_Indicators(void){
    DDRD &=~ (( 1 << PD2 ) | (1 << PD3)); //Set both PD2 and PD3 as inputs
    PORTD |= (( 1 << PD2 ) | (1 << PD3)); //Setup Pullups on PD2 and PD3
    EICRA |= ( 1 << ISC11) | ( 1 << ISC01); //Set interrupt on falling edge of INT0 and INT 1
    EIMSK |= ( 1 << INT1) | ( 1 << INT0 );//Setup External Interrupts on INT0 and INT1;
    return 0;
}

int check_Obstacle_Distance(uint16_t range){
    if( range > 300 ){ //Check if the other vechicle is too near viz less than 300 cm
        PORTB |= ( 1 << PB0 ); //Enable alarm
    }
    else{
        PORTB &= ~( 1 << PB0 ); //disable alarm
    }
    return 0;
}

int init_Sleep_Mode(){
    MCUCR |= ( 1 << BODS ); //Disable Brown out detection during sleep
    SMCR |= ( 1 << SM1 ); //Put the MC into Power Down mode
    return 0;
}