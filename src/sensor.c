/**
 * @file sensor.c
 * @author Sanjeeve R (18euee122@skcet.ac.in)
 * @brief This file contains the  function definitions for the declared functions in Sensor.h
 * @version 0.1
 * @date 2022-04-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "sensor.h" 
#include  "debug.h"

volatile short flag = 1;
volatile uint8_t ticks = 0;
volatile uint8_t Timer_Overflow;

/**
 * @brief This Function initiazes the Ultra sonic sensor on the given pins. The trigger pin is set output and the echo pin is set input.
 * 
 * @param S 
 * @return int 
 */
int init_Ultrasonic_Sensor( Ultrasonic S){
    //Test Conditions for incorrect Pins of Atmega328
    if ( S.Echo_Port < 'B' || S.Echo_Port > 'D' || S.Trig_Port < 'B' || S.Trig_Port > 'D' ){ 
        return 1; 
    }
    else{
        if ( S.Echo_Port == 'C' || S.Trig_Port == 'C' ){
            if(!( S.Trig_pin >=0 && S.Trig_pin <= 6 ))
                return 2;
        }
        else { 
            if(!( S.Trig_pin >=0 && S.Trig_pin <= 7 ))
                return 2;
        }
    }
    //Initialize the pins of the Sensor
    switch ( S.Trig_Port ){
        case 'B': DDRB |= ( 1 << S.Trig_pin ); break; 
        case 'C': DDRC |= ( 1 << S.Trig_pin ); break; 
        case 'D': DDRD |= ( 1 << S.Trig_pin ); break; 
    }
    switch ( S.Echo_Port ){
        case 'B': DDRB &=~ ( 1 << S.Echo_pin ); break; 
        case 'C': DDRC &=~ ( 1 << S.Echo_pin ); break; 
        case 'D': DDRD &=~ ( 1 << S.Echo_pin ); break; 
    }

    return 0;
}



int set_Interrupts ( Ultrasonic S ){

    if ( S.Echo_Port == 'B' ){ //If port B
        PCICR |= ( 1 << PCIE0 );
        PCMSK1 |= ( 1 << S.Echo_pin );    
    }
    else if ( S.Echo_Port == 'C' ){ //If port C
        PCICR |= ( 1 << PCIE1 );
        PCMSK1 |= ( 1 << S.Echo_pin );          
    }
    else if ( S.Echo_Port == 'D' ){ //If port C
        PCICR |= ( 1 << PCIE2 );
        PCMSK1 |= ( 1 << S.Echo_pin );        
    }
    //Timer Interrupt Configuration
    TCNT2 = 0;
    TIMSK2 = 0X00;
    TIMSK2 = ( 1 << TOIE2 );
    return 0;
}

uint16_t Echo_Time( void ){ //Return the Pulse width of the Echo in terms of timer count and overflow
    return ticks + ( 255 * Timer_Overflow );
}

int Trigger ( Ultrasonic S){

    switch ( S.Trig_Port ){ //Check the Trigger Port
        case 'B':  // if its port B
            PORTB |= ( 1 << S.Trig_pin);// High the Trig pin
            create_delay_for_Ultrasonic_Sensor(); // Create required delay => 10us
            while ( ( TCCR0B & ( 1 << CS00 ) ) != 0 ); //wait for the reset of timer
            PORTB &=~ ( 1 << S.Trig_pin); //Low the trig pin
            break; 
        case 'C': //if its port C
            PORTC |= ( 1 << S.Trig_pin);
            create_delay_for_Ultrasonic_Sensor();
            while ( ( TCCR0B & ( 1 << CS00 ) ) != 0 );
            PORTC &=~ ( 1 << S.Trig_pin);
            break;  
        case 'D': //if its port D
            PORTD |= ( 1 << S.Trig_pin);
            create_delay_for_Ultrasonic_Sensor();
            while ( ( TCCR0B & ( 1 << CS00 ) ) != 0 );
            PORTD &=~ ( 1 << S.Trig_pin);
            break; 
    }

    return 0;
}
int create_delay_for_Ultrasonic_Sensor ( void ){
    TCNT0 = 0; //Initialize the Timer zero counter to zero
    OCR0A = 159; //Initialize the Compare register to 159 => 10us delay using no prescalar
    TIMSK0 = 0X00; //clear the Timer 0 interrupt mask 
    TIMSK0 |= ( 1 << OCIE0A ); //Set On compare equal interrupt
    TCCR0B |= ( 1 << CS00 ); // Turn on the timer with no prescalar
    return 0;
}

uint16_t get_Range( Ultrasonic S ){
    uint16_t range = 0;
    Timer_Overflow = 0;
    flag = 1;
    set_Interrupts(S); //set the required interrupts
    sei(); //Enable global Interrupts
    Trigger(S); //Start Trigger
    while( flag != 3 ); //wait for the end of the pulse
    range = (uint16_t)((Echo_Time() / 0.0156) / 58.0);//Calculate the Range
    //debug_using_uart(range,Timer_Overflow,ticks); //clear the comment to enable debugging using uart. 
    return range;
}

ISR ( PCINT1_vect ){ //ISR for pin level change interrupt on port C
    cli(); //clear global Interrupts
    if ( flag ){ //if flag == 1 => high level signal is seen on the pin
        TCCR2B |=   ( 1 << CS20) | ( 1 << CS21 ) | ( 1 << CS22 ); // Turn on the timer 2 with 1024 prescalar value
        flag = 0; //set flag to 0
    }
    else { 
        ticks = TCNT2; // when a low level signal is detected on the pin
        TCCR2B &=~  (( 1 << CS20) | ( 1 << CS21 ) | ( 1 << CS22 ));  //Turn off the timer
        flag = 3;      //set flag to 3
    }
    sei(); //Enable global interrupts
}

ISR ( TIMER0_COMPA_vect ){ // ISR for Timer 0 compare A register. This ISR Turns off the timer 1 and resets all of its registers
    TCCR0B &=~ ( 1 << CS00 ); 
    TCNT0 = 0;
    OCR0A = 0;
    TIMSK0 = 0X00;
    TIMSK0 &=~ ( 1 << OCIE0A );
}

ISR ( TIMER2_OVF_vect ){ //Overflow ISR for Timer2
    Timer_Overflow = Timer_Overflow + 1; //Increament the Overflow variable
}