// Round Robin Scheduler, V1, with interruptions.
// Julien Forget, Thomas Vantroys, Laure Gonnord, Thierry Excoffier

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "lib_lcd.h"

// Reminder : to write on the serial port:
// screen /dev/ttyACM0 9600 in a new terminal
// screen -a k to quit.

#define PRESCALER	1024
#define TIME_SLOT	40	// time slot for round robin in millisecond
#define NB_TICK		(((F_CPU/PRESCALER)*TIME_SLOT)/1000)

#define BAUDRATE	103 // UBRR value for 9600

#define NOT_STARTED	0
#define RUNNING		1


// Only for scheduler V2
#define SAVE_CONTEXT()						\
  asm volatile (						\
		"push	r0				\n\t"	\
		"in		r0, __SREG__	\n\t"		\
		"cli					\n\t"	\
		"push	r0				\n\t"	\
		"push	r1				\n\t"	\
		"push	r2				\n\t"	\
		"push	r3				\n\t"	\
		"push	r4				\n\t"	\
		"push	r5				\n\t"	\
		"push	r6				\n\t"	\
		"push	r7				\n\t"	\
		"push	r8				\n\t"	\
		"push	r9				\n\t"	\
		"push	r10				\n\t"	\
		"push	r11				\n\t"	\
		"push	r12				\n\t"	\
		"push	r13				\n\t"	\
		"push	r14				\n\t"	\
		"push	r15				\n\t"	\
		"push	r16				\n\t"	\
		"push	r17				\n\t"	\
		"push	r18				\n\t"	\
		"push	r19				\n\t"	\
		"push	r20				\n\t"	\
		"push	r21				\n\t"	\
		"push	r22				\n\t"	\
		"push	r23				\n\t"	\
		"push	r24				\n\t"	\
		"push	r25				\n\t"	\
		"push	r26				\n\t"	\
		"push	r27				\n\t"	\
		"push	r28				\n\t"	\
		"push	r29				\n\t"	\
		"push	r30				\n\t"	\
		"push	r31				\n\t"	\
		 );


#define RESTORE_CONTEXT()					\
  asm volatile (						\
		"pop	r31				\n\t"	\
		"pop	r30				\n\t"	\
		"pop	r29				\n\t"	\
		"pop	r28				\n\t"	\
		"pop	r27				\n\t"	\
		"pop	r26				\n\t"	\
		"pop	r25				\n\t"	\
		"pop	r24				\n\t"	\
		"pop	r23				\n\t"	\
		"pop	r22				\n\t"	\
		"pop	r21				\n\t"	\
		"pop	r20				\n\t"	\
		"pop	r19				\n\t"	\
		"pop	r18				\n\t"	\
		"pop	r17				\n\t"	\
		"pop	r16				\n\t"	\
		"pop	r15				\n\t"	\
		"pop	r14				\n\t"	\
		"pop	r13				\n\t"	\
		"pop	r12				\n\t"	\
		"pop	r11				\n\t"	\
		"pop	r10				\n\t"	\
		"pop	r9				\n\t"	\
		"pop	r8				\n\t"	\
		"pop	r7				\n\t"	\
		"pop	r6				\n\t"	\
		"pop	r5				\n\t"	\
		"pop	r4				\n\t"	\
		"pop	r3				\n\t"	\
		"pop	r2				\n\t"	\
		"pop	r1				\n\t"	\
		"pop	r0				\n\t"	\
		"out 	__SREG__, r0	\n\t"			\
		"pop	r0				\n\t"	\
		 );


// TASKS

//http://www.avrbeginners.net/architecture/timers/timers.html
void init_timer()
{
  TCCR1B |= _BV(WGM12); // CTC mode with value in OCR1A 
  TCCR1B |= _BV(CS12);  // CS12 = 1; CS11 = 0; CS10 =1 => CLK/1024 prescaler
  TCCR1B |= _BV(CS10);
  OCR1A   = NB_TICK;
  TIMSK1 |= _BV(OCIE1A);
}

void init_led_red(void)
{
  DDRC |= 0b000001; //Analog 0
}

void init_led_yellow(void)
{
 DDRC |= 0b000010; //Analog 1
}

void init_serial(void)
{
  // THIS IS GIVEN
  /* ACHTUNG : we suppose UBRR value < 0xff */
  /* Not true in all case */
  uint8_t baudrate = BAUDRATE;
  /* Set baud rate */
  UBRR0H = 0;
  UBRR0L = baudrate;

  /* Enable transmitter */
  UCSR0B = (1<<TXEN0);

  /* Set frame format */
  UCSR0C = 0x06;
}


void send_serial(unsigned char c)
{
  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0 = c;
}


void init_task_lcd(){
  lcd_init_4d();
}


// NOW TASKS : infinite loops
void task_serial(void)
{
  //write a message on the serial port, redo, ...
	
	char* msg = "Bonjour";
	int l = 7;
	int i = 0;
	while(1) {
		i = 0;
		for (; i < l; i++) {
			send_serial(msg[i]);
			_delay_ms(100);
		}
		
	}
	
}


void task_led(void)
{
  //init, then blink red led (infinite loop)
	
	init_led_red();
	
	while(1) {
		PORTC ^= 0b000001;//blink
		_delay_ms(300);//delay
	}
}


void task_lcd(void) 
{
  //init, and send a message (infinite loop)
	
	init_task_lcd();
	//lcd_write_instruction_4d(lcd_Clear);
	
	 // display the first line of information */
	while(1) {
		
		 //lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo+ 5);
		
		
		//lcd_write_instruction_4d(lcd_Clear); //Clear
		lcd_write_string_4d("Bonjour"); //Take 80us = negligeable
		_delay_ms(400);
		lcd_write_instruction_4d(lcd_Clear);
		_delay_ms(100);
		
		
		
	}
}


/* SCHEDULER */
typedef struct task_s {
  volatile uint8_t state;
  void (*start)(void);//code for the task
  volatile uint16_t stack_pointer;//to store SP
} task_t;

// The third element is not useful for V1
static task_t tasks[] = {
  {NOT_STARTED, task_led, 0x500},
  {NOT_STARTED, task_serial, 0x700},
  {NOT_STARTED, task_lcd, 0x600}
};

#define NB_TASK (sizeof(tasks)/sizeof(tasks[0]))

// The isr interruption implements the scheduling activity
ISR(TIMER1_COMPA_vect)
{
	static int current_task = 0 ;
	PORTC ^= 2 ; // Yellow led blinks to show its activity
	task_t task = tasks[current_task];

	SAVE_CONTEXT()
	//Store SP in current task
	task.stack_pointer = SP;
	sei();

	//Next task
	current_task = (current_task + 1) % NB_TASK;
	task = tasks[current_task];
	//Get SP
	SP = task.stack_pointer;
	if (task.state == RUNNING)  {
			RESTORE_CONTEXT()
			sei();
	}
	else {
		//First launch
		task.state = RUNNING;
		sei();
		task.start();
	}
}

int main(void)
{
	init_led_yellow();// the yellow led blinks to show the sheduler activity.
	init_timer();
	init_serial();
	
	sei() ;
	while(1) // waits the first task, and then not useful any more.
	{
	}

  	return 0;
}


