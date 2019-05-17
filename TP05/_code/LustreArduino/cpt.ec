node cpt
  (reset: bool)
returns
  (led_on: bool;
  button_state: bool);

let
  led_on = (false -> (not (pre led_on)));
  button_state = (if reset then true else false);
tel

