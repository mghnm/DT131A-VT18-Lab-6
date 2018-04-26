/*
 * Lab6DT131A.c
 *
 * Created: 4/26/2018 5:34:10 PM
 * Author : Mohammed Stationary
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#define F_CPU 16000000

volatile uint8_t resultDigital;

void initializeADC();
uint8_t convertToDigital();

ISR(ADC_vect){
	resultDigital = ADCH;
}

int main(void)
{
    /* Replace with your application code */
	//Set port D as output for LED
	DDRB = 0xFF;
	initializeADC();
    while (1) 
    {
		convertToDigital();
		
		/*
		* We know that the value returned from the ADC is an  8 bit unsigned between 0 and 255 divided according to a 5 volt reference voltage.
		* That means our resolution is 5/256 approx 0.01953125 volts per unit.
		* 0 -> 1 Volts : 0 - 52			PortB = 0b00000001
		* 1 -> 2 volts : 52 - 103		PortB = 0b00000100
		* 2 -> 3 volts : 104 - 154		PortB = 0b00010000
		* 3 -> 4 volts : 155 - 205		PortB = 0b00100000
		* 4 -> 5 volts : 206 - 255		PortB = 0b10000000
		*/
		
		if(resultDigital < 53){
			PORTB = 0X01;
			break;
		} else if((resultDigital > 52) && (resultDigital < 104)){
			PORTB = 0X04;
			break;
		} else if((resultDigital > 103) && (resultDigital < 155)){
			PORTB = 0X10;
			break;
		} else if((resultDigital > 154) && (resultDigital < 206)){
			PORTB = 0X20;
			break;
		} else if((resultDigital > 205) && (resultDigital < 256)){
			PORTB = 0X80;
			break;
		} else {
			//Light up all led for debugging purposes
			PORTB = 0xFF;
		}
		
		//Delay for 5 second interval execution
		_delay_ms(5000);
		
		
    }
}


void initializeADC(){
	//Configure the ADMUX for AVCC as input (internal 5v reference) / left adjusted results (8-bit resolution) / ADC0 as input
	ADMUX = 0x60;
	//Configure ADCSRA for enable ADC / don't start conversion / prescaler 2 / disable auto-trigger / enable ADC interrupt 0b10001001
	ADCSRA = 0x89;
}

uint8_t convertToDigital(){
	//Set the Start bit to 1 in ADCSRA
	ADCSRA |= (1 << ADSC);
	
	return resultDigital;
	
}