//Interfacing with arduino led + 7segment

#include "glue_arduino.h"


int a = 1;
int b = 2;
int c = 3;
int d = 4;
int e = 5;
int f = 6;
int g = 7;

int led = 13; // Synchronized with the shield led
int led_on = 1; // led state
int button = 10; //button
int button_state= 0;

// TODO : add some more global variables
int i = 0;

void setup() {               
  //Setup for LED on pin 
  pinMode(led, OUTPUT);  
  
  led_on=1;//true

  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);

  return;
}

void turnOff() //TODO (copy paste from one preceding step !)
{
	int i;
  for (i = a; i <= g; ++i)
  {
    digitalWrite(i, LOW);
  }
}


void displayDigit(int digit)
{

  turnOff();


  //Conditions for displaying segment a
	if(digit!=1 && digit != 4)
	digitalWrite(a,HIGH);

	//Conditions for displaying segment b
	if(digit != 5 && digit != 6)
	digitalWrite(b,HIGH);

	//Conditions for displaying segment c
	if(digit !=2)
	digitalWrite(c,HIGH);

	//Conditions for displaying segment d
	if(digit != 1 && digit !=4 && digit !=7)
	digitalWrite(d,HIGH);

	//Conditions for displaying segment e 
	if(digit == 2 || digit ==6 || digit == 8 || digit==0)
	digitalWrite(e,HIGH);

	//Conditions for displaying segment f
	if(digit != 1 && digit !=2 && digit!=3 && digit !=7)
	digitalWrite(f,HIGH);
	if (digit!=0 && digit!=1 && digit !=7)
	digitalWrite(g,HIGH);

  

  return; // TODO!
}

void reset() {
	i = 0;
}

void incrAndShow() {
	i = (i + 1) % 9;
	displayDigit(i);
}
