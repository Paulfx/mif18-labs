// MIF12 LAB 1

#include <avr/io.h>
#include <util/delay.h>

//Red Led on Digital 13 , Button on digital 10
//7 seg on digital 1 to 7 (a-> digital 1, ...)


void init(void)
{
  // DDRB is the configuration register for digital 7 to 18
  DDRB |= 0b00100000;// Digital 13 "outputmode"
  // TODO : enable write for digital 1 to 7 (7seg)
  // TODO : initialize global state values if required
  PORTB ^= 0b00100000;
}


void change_led_state(){
  PORTB ^= 0b00100000; // Change digital 13 on->off->on (xor is life!)
}


int get_reset_value(){ // on digital 10 ie bit 2
 return  PINB & 4; 
}


void display_7seg(int value){
  switch (value) {
  case 0: //a,b,c,d,e,f should be OFF (common anode)
    PORTD = 0b10000000;
    break; //TODO : complete for other values.
  default:
    PORTD = 0b11111111;
  }
}


void change_7seg_state (){
return;}



int main(void)
{
  init();
  while(1) //infinite loop
    {
      display_7seg(0); // change to test the 7-seg when DDRD is configured
      change_led_state();

      _delay_ms(1000);     // 1Hz period
    }
  
  return 0;
}
