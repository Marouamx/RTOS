/** 
**  Marouamx
**  04/05/2020 01:56
**  avr_ATmega328 mcu
**  blinking leds with different rates
**/
    

#include<avr/io.h>
#include<avr/interrupt.h>

#define F_CPU 16000000UL

#define N_TASKS (4)

#define STATE_READY    (0x00)
#define STATE_RUNNING  (0x01)
#define STATE_WAITING  (0x02)
#define STATE_INACTIVE (0x03)  //waiting for event


typedef struct{

    uint8_t id,state;
    unsigned long delay, period;
    void (*fptr) (); // funtion to exec during task running

}TCB;

TCB task[N_TASKS];

void scheduler_init () {

    int i = 0 ;
    // task[0] -> ADC0
    task[i].id = i;
    task[i].state = STATE_READY;
    task[i].period = 50;
    task[i].delay = task[i].period;
    task[i].fptr = led;

    i++;
    // task[1] -> ADC1
    task[i].id = i;
    task[i].state = STATE_READY;
    task[i].period = 100;
    task[i].delay = task[i].period;
    task[i].fptr = led;


    i++;
    // task[2] -> NN _output // compute output of neural network
    task[i].id = i;
    task[i].state = STATE_READY;
    task[i].period = 200;
    task[i].delay = task[i].period;
    task[i].fptr = led;

    i++;
    // task[3] -> LED
    task[i].id = i;
    task[i].state = STATE_READY;
    task[i].period = 500;
    task[i].delay = task[i].period;
    task[i].fptr = led;

}

ISR(TIMER0_COMPA_vect) {

    for (uint8_t j = 0; j < N_TASKS ; j++ ) {


        if (task[j].state == STATE_READY && task[j].delay == 0) {

            task[j].state = STATE_RUNNING;
            PORTC ^= (1<<j); // toggle pin
            task[j].delay = task[j].period;
            task[j].state = STATE_READY ;

        }
        else {
            task[j].delay--;
        }
    }
}
    void hardware_init() {

        DDRC |= (1<<0) |(1<<1) | (1<<2) | (1<<3);

        TCCR0A = (1<<WGM01) ; //CTC mode

        OCR0A = 124; // generate 1kHz interruption -- 16000000/2/64/1k-1

        TIMSK0 = (1<<OCIE0A); // enabling interruptin of OCE0A

        TCCR0B = (3<<CS00); // start counting every 64 MCs

    }

void led() {};
int main () {

    hardware_init();
    scheduler_init();
    sei();

    while (1) {};
}
