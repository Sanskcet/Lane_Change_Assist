#include "uart.h"

/**
 * @brief This function initializes the UART protocol
 * 
 */
void init_UART()
{
	UBRR0H = (uint8_t)(BAUDRATE_PRESCALLER>>8); 
	UBRR0L = (uint8_t)(BAUDRATE_PRESCALLER);	
	UCSR0B = (1<<TXEN0); //enables Transmit data
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01); //UART 8bit mode
}

/**
 * @brief This function sends the string to the UART Receiver
 * 
 * @param stringPtr 
 */
void putStringtoUART(char* stringPtr)	
{
	while (*stringPtr != 0) {	//check wheather the end of character is reached
		while (!(UCSR0A & (1<<UDRE0)));	//wait for the current data transmission to complete
		UDR0 = *stringPtr;	//send dereferenced pointer of stringPtr to the transmission Register
		stringPtr++;	//increment the stringPtr
	}
}