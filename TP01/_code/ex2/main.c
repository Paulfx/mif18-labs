// LAB 1 - Main File for students, to be completed

#include <avr/io.h>
#include <util/delay.h>

//Red Led on Digital 13 
//Other Led on digital 12

void init(void)
{
  // DDRB is the configuration register for digital 7 to 18
  // TODO : Set Digital 12 and 13 to "outputmode"
}



int main(void)
{
  init();
  while(1) //infinite loop
    {
      // TODO
      _delay_ms(1000);     // 1Hz period
    }
  
  return 0;
}
