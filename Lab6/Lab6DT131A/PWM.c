/*
 * PWM.c
 *
 * Created: 2018-04-28 17:04:04
 *  Author: mghan
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t resultDigital;
void initializePWM();
void initializeADC();
void convertToDigital();


ISR(ADC_vect){
	resultDigital = ADCH;
	
	
	/*
	*  1 unit has a resolution of 0.01953125 volts
	* To convert from resultDigital to voltage do value * 0.01953125
	* PWM to duty cycle is d = (5 - v) * 20
	* To go from duty cycle percentage to OCR0A value we do OCR0A = 255 * (d / 100)
	*/
	
	double voltage = resultDigital * 0.01953125;
	int d = (5 - voltage) * 20;
	OCR0A = 255 * (d / 100);
	
	
}

int main(void){
	//Set the PWM pin OC0A as output it will be hooked up to an LED.
	DDRD |= (1 << PIND6);
	initializePWM();
	initializeADC();
	sei();
	while(1){
		convertToDigital();
	}
	return 0;
}

void initializePWM(){
	/*
	* For PWM there are two control registers (TCCR0A / TCCROB) Modes of operation are determined by the two WGM bits in 
	* register A and a third WGM bit in register B. For fast pulse width modulation WGM00 and WGM 01 need to be set to 1 and WGM02 remains at 0.
	* if WGM02 is set to high then the counter will clear once it hits the compare value specified at OCR0A. The registers also determine pre-scalers
	* and other stuff. 
	* TCCR0A = 0b11000011
	* TCCR0B = 0x00
	*/
	
	// Enable inverted mode (High if greater LOW if less than).
	TCCR0A |= (1 << COM0A1);
	TCCR0A |= (1 << COM0A0);
	
	//Enable Fast PWM mode
	TCCR0A |= (1 << WGM00);
	TCCR0A |= (1 << WGM01);
	
	//Prescaler 256
	TCCR0B |= 0x04;
	
	//TODO: Set the compare value for the PWM using the OC0A pin.
	
}

void initializeADC(){
	//Configure the ADMUX for AVCC as input (internal 5v reference) / left adjusted results (8-bit resolution) / ADC0 as input 0b01100000
	ADMUX = 0x60;
	//Configure ADCSRA for enable ADC / don't start conversion / prescaler 128 / disable auto-trigger / enable ADC interrupt 0b10001001
	ADCSRA = 0x8F;
}

void convertToDigital(){
	//Set the Start bit to 1 in ADCSRA
	ADCSRA |= (1 << ADSC);
}

