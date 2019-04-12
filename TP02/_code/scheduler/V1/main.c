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

#define PRESCALER   1024
#define TIME_SLOT   40  // time slot for round robin in millisecond
#define NB_TICK     (((F_CPU/PRESCALER)*TIME_SLOT)/1000)

#define BAUDRATE    103 // UBRR value for 9600

#define NOT_STARTED 0
#define RUNNING     1

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
    //Write a message on the serial port, redo, ...
    
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
    //Init, then blink red led (infinite loop)
    
    init_led_red();
    
    while(1) {
        PORTC ^= 0b000001;//blink
        _delay_ms(300);//delay
    }
}


void task_lcd(void) 
{
    //Init, and send a message (infinite loop)
    init_task_lcd();
     // display the first line of information */
    uint8_t msg[] = "Bonjour";
    while(1) {
        lcd_write_string_4d(msg); //Take 80us = negligeable
        _delay_ms(400);
        lcd_write_instruction_4d(lcd_Clear);
        _delay_ms(100);
    }
}


/* SCHEDULER */
typedef struct task_s {
    volatile uint8_t state;
    void (*start)(void);//code for the task
} task_t;


static task_t tasks[] = {
    {NOT_STARTED, task_led},
    {NOT_STARTED, task_serial},
    {NOT_STARTED, task_lcd}
};

#define NB_TASK (sizeof(tasks)/sizeof(tasks[0]))

// The isr interruption implements the scheduling activity
ISR(TIMER1_COMPA_vect)
{
    static int current_task = 0 ;
    current_task = (current_task + 1) % NB_TASK;
    //PORTC ^= 2 ; // Yellow led blinks to show its activity

    //The scheduler.
    task_t task = tasks[current_task];
    if (task.state == RUNNING)  {
        sei();
        task.start();
    }
    else {
        task.state = RUNNING;
        sei();
        task.start();
    }
}

int main(void)
{
    // Nothing to do.
    init_led_yellow();// the yellow led blinks to show the sheduler activity.
    init_timer() ;
    init_serial();
    

    sei() ;
    //task_led();
    task_lcd();
    //task_serial();
    while(1) // waits the first task, and then not useful any more.
    {
    }

    return 0;
}


