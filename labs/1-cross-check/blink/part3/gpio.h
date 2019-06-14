#ifndef _GPIO_H_
#define _GPIO_H_

#define GPIO_BASE 0x20200000
extern volatile unsigned *gpio_fsel0;   // GPIO Function Select 0
extern volatile unsigned *gpio_fsel1;   // GPIO Function Select 1
extern volatile unsigned *gpio_fsel2;   // GPIO Function Select 2
extern volatile unsigned *gpio_fsel3;   // GPIO Function Select 3
extern volatile unsigned *gpio_fsel4;   // GPIO Function Select 4
extern volatile unsigned *gpio_set0;    // GPIO Pin Output Set 0 
extern volatile unsigned *gpio_clr0;    // GPIO Pin Output Clear 0 
extern volatile unsigned *gpio_lev0;    // GPIO Pin Level 0
extern volatile unsigned *gpio_eds0;    // GPIO Pin Event Detect Status 0 
extern volatile unsigned *gpio_ren0;    // GPIO Pin Rising Edge Detect Enable 0
extern volatile unsigned *gpio_fen0;    // GPIO Pin Falling Edge Detect Enable 0 
extern volatile unsigned *gpio_hen0;    // GPIO Pin High Detect Enable 0 
extern volatile unsigned *gpio_len0;    // GPIO Pin Low Detect Enable 0 
extern volatile unsigned *gpio_aren0;   // GPIO Pin Async. Rising Edge Detect 0 
extern volatile unsigned *gpio_afen0;   // GPIO Pin Async. Falling Edge Detect 0
extern volatile unsigned *gpio_pud;     // GPIO Pin Pull-up/down Enable 
extern volatile unsigned *gpio_pudclk0; // GPIO Pin Pull-up/down Enable Clock 0 
/*
 * set the <pin> as an output
 */
void gpio_set_output(unsigned pin);

void gpio_set_on(unsigned pin);
void gpio_set_off(unsigned pin);
/* * set the <pin> as an input */void gpio_set_input(unsigned pin);

/*
 * change the state of the <pin> of detecting event,
 * 0: disable 1: enable
 */
void gpio_event_detect(unsigned pin, unsigned state);

/*
 * get the value of event detected on pin<pin>
 */
unsigned gpio_read_event(unsigned pin);


/*
 * delay 
 */
void delay(unsigned ticks);

#endif
