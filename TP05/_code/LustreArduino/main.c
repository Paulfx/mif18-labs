/*Source code from L. Gonnord & L.Morel */

#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>

#include "cpt.h"           // Generated by Lustre from .lus
#include "glue_arduino.h"


/* Standard Input/Ouput procedures **************/
_boolean _get_reset_value(){
  button_state=digitalRead(button);
  return (button_state == HIGH);
}

void cpt_O_led_on(void* cdata, _boolean _V) {
  if (_V == 1) 
    digitalWrite(led, HIGH);
  else
    digitalWrite(led, LOW);
}


int main(){
  
  /* Lustre Context (state data structure) allocation for node "cpt"*/
  struct cpt_ctx* ctx = cpt_new_ctx(NULL);
  setup(); 
  
  
  /* Main loop */
  while(1){

    /* get button value */
    cpt_I_reset(ctx, _get_reset_value());
    cpt_step(ctx);

    /* Wait ! */
    _delay_ms(1000);
  }
   return 1;
   
}
