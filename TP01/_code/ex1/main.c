// LAB 1 - Main File for students, to be completed

#include <avr/io.h>
#include <util/delay.h>

//Red Led on Digital 13 - 1Hz


void init(void)
{
  // DDRB is the configuration register for digital 7 to 18
  // TODO : Set Digital 13 to "outputmode"
}


void change_led_state(){
  // TODO 
  // Change digital 13 on->off->on (xor is life!)
}



int main(void)
{
  init();
  while(1) //infinite loop
    {
      change_led_state();
      _delay_ms(1000);     // 1Hz period
    }
  
  return 0;
}
