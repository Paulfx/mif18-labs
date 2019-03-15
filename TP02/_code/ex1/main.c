// MIF18 Lab, 2018-19, Laure Gonnord

#include <avr/io.h>			// for the input/output register
#include <avr/interrupt.h> 	
#include <util/delay.h>  	// for the _delay_ms

#include <stdio.h>

#define PRESCALER
#define TIME_SLOT


#define NB_TICK 6250 // 400ms

void init_led_red(void)
{
  // TODO : init, red led is on analog 0
	
	DDRC |= 0b000001;
	
}

void init_led_yellow(void)
{
  DDRC |= 0b000010;
}


void init_timer(void)
{
  // cf doc
  TCCR1B |= _BV(WGM12); // CTC mode with value in OCR1A 
  TCCR1B |= _BV(CS12);  // CS12 = 1; CS11 = 0; CS10 =1 => CLK/1024 prescaler
  TCCR1B |= _BV(CS10);
  OCR1A   = NB_TICK;
  TIMSK1 |= _BV(OCIE1A);
}


void task_led_red(void)
{
  // TODO : call to init for the red led
  // then blink, then wait (delay)
	
	PORTC ^= 0b000001;//blink
	_delay_ms(200);//delay
	
}


ISR(TIMER1_COMPA_vect)
{
  PORTC ^= 0b000010;
	
}



int main(void)
{
	init_timer();
	init_led_red();
	init_led_yellow();
	
  sei(); // this is mandatory
  
  while(1)
    {
      
		task_led_red();
		
    }
  
  return 0;
}
