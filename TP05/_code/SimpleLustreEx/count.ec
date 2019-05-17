node count
  (r: bool;
  Start: int;
  Incr: int)
returns
  (c: int);

var
  V6_count: int;
  V34_b: bool;
  V35_y: int;

let
  c = (current (((V35_y + 5)) when V34_b));
  V6_count = (Start -> (if r then Start else (Incr + (pre c))));
  V34_b = (if (V35_y < 0) then true else false);
  V35_y = (V6_count -> ((pre V35_y) - 5));
tel

