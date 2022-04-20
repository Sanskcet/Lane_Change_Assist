
#include "sensor.h"
#define F_CPU 16000000UL
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

void initUSART()
{
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8); //this sets the BAUD rate, must shift UBRROH 8 bits to the right
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);	//sets lower 8 bits of UBRR0
	UCSR0B = (1<<TXEN0); //enables send of data
	UCSR0C = (3<<UCSZ00); //it is set to 8 bit
}

int main(){  
    float Range1 = 0 , Range2 = 0;
    Ultrasonic S1 = {'D','C',6,0};
    Ultrasonic S2 = {'C','C',0,1};
    DDRB |= ( 1 << PORTB0);
    initUSART();
    init_Ultrasonic_Sensor(S1);
    init_Ultrasonic_Sensor(S2);


    while (1)
    {
        //Range1 = get_Range(S1);
        Range2 = get_Range(S2);
    /*    if ( Range2 > 20 ){
            PORTB |= ( 1 << PORTB0 );
        }
        else { 
            PORTB &=~ ( 1 << PORTB0 );
        }     
        
*/
    _delay_ms(100);
    }
    
    return 0;   
}
