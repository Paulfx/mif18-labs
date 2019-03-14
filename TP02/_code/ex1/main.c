// MIF18 Lab, 2018-19, Laure Gonnord

#include <avr/io.h>			// for the input/output register
#include <avr/interrupt.h> 	
#include <util/delay.h>  	// for the _delay_ms

#define PRESCALER
#define TIME_SLOT


#define NB_TICK 4242  // change here

void init_led_red(void)
{
  // TODO : init, red led is on analog 0
}

void init_led_yellow(void)
{
  // TODO : init, yellow led on analog 1
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
}


ISR(TIMER1_COMPA_vect)
{
  // TODO : blink the yellow led
}



int main(void)
{
  // TODO : init yellow led + timer.
  sei(); // this is mandatory
  
  while(1)
    {
      // TODO : which task(s) ?
    }
  
  return 0;
}
