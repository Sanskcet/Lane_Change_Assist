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
#include <string.h>
#include <stdlib.h>

volatile short flag = 1;
volatile uint8_t ticks = 0;
volatile Timer_Overflow = 0;
/**
 * @brief This Function initiazes the Ultra sonic sensor on the given pins. The trigger pin is set output and the echo pin is set input.
 * 
 * @param S 
 * @return int 
 */

int init_Ultrasonic_Sensor( Ultrasonic S){

    if ( S.Echo_Port < 'B' || S.Echo_Port > 'D' || S.Trig_Port < 'B' || S.Trig_Port > 'D' ){ //Test Conditions for incorrect Pins of Atmega328
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

void putStringUSART(char* stringPtr)	//take pointer to char as parameter
{
	while (*stringPtr != 0) {	//while the data the pointer points to is not null
		while (!(UCSR0A & (1<<UDRE0)));	//wait to receive data
		UDR0 = *stringPtr;	//UDR0 = dereferenced stringPtr characters
		stringPtr++;	//advance stringPtr by 1
	}
}

int set_Interrupts ( Ultrasonic S ){
    if ( S.Echo_Port == 'B' ){
        PCICR |= ( 1 << PCIE0 );
        PCMSK1 |= ( 1 << S.Echo_pin );    
    }
    else if ( S.Echo_Port == 'C' ){
        PCICR |= ( 1 << PCIE1 );
        PCMSK1 |= ( 1 << S.Echo_pin );          
    }
    else if ( S.Echo_Port == 'D' ){
        PCICR |= ( 1 << PCIE2 );
        PCMSK1 |= ( 1 << S.Echo_pin );        
    }

    TCNT2 = 0;
    TIMSK2 = 0X00;
    TIMSK2 = ( 1 << TOIE2 );
    return 0;
}

uint16_t Echo_Time( void ){ 
    return ticks + ( 255 * Timer_Overflow );
}

int Trigger ( Ultrasonic S){
    PORTC |= ( 1 << S.Trig_pin);
    create_delay_for_Ultrasonic_Sensor();
    while ( ( TCCR0B & ( 1 << CS00 ) ) != 0 );
    PORTC &=~ ( 1 << S.Trig_pin);
    return 0;
}
int create_delay_for_Ultrasonic_Sensor ( void ){
    TCNT0 = 0;
    OCR0A = 159;
    TIMSK0 = 0X00;
    TIMSK0 |= ( 1 << OCIE0A );
    TCCR0B |= ( 1 << CS00 );
    return 0;
}

double get_Range( Ultrasonic S ){
    char range_String[5];
    double range = 0;
    Timer_Overflow = 0;
    set_Interrupts(S);
    sei(); //Enable global Interrupts
    Trigger(S); //Start Trigger
    range =  (Echo_Time() / 0.0156) / 58.0 ;//Calculate the Range
    putStringUSART(" Range: ");
    itoa((int)range,range_String,10);
    putStringUSART(range_String);    
    putStringUSART("\n");
    return range;
}

ISR ( PCINT1_vect ){
    if ( flag ){
        PORTB |= ( 1 << PORTB0 );
        TCCR2B |= ( 1 << CS21) | ( 1 << CS22 ) | ( 1 << CS20);
        flag = 0;
    }
    else { 
        ticks = TCNT2;
        TCCR2B &=~ ( ( 1 << CS21) | ( 1 << CS22 ) | ( 1 << CS20) );  
        flag = 1;      
       // PORTB &=~ ( 1 << PORTB0 );
    }
}

ISR ( TIMER0_COMPA_vect ){
    TCCR0B &=~ ( 1 << CS00 );
}

ISR ( TIMER2_OVF_vect ){
    cli();
    Timer_Overflow++;
    sei();
}