/*
 * write code to allow blinking using arbitrary pins.    
 * Implement:
 *	- gpio_set_output(pin) --- set GPIO <pin> as an output (vs input) pin.
 *	- gpio_set_on(pin) --- set the GPIO <pin> on.
 * 	- gpio_set_off(pin) --- set the GPIO <pin> off.
 * Use the minimal number of loads and stores to GPIO memory.  
 *
 * start.s defines a of helper functions (feel free to look at the assembly!  it's
 *  not tricky):
 *      uint32_t get32(volatile uint32_t *addr) 
 *              --- return the 32-bit value held at <addr>.
 *
 *      void put32(volatile uint32_t *addr, uint32_t v) 
 *              -- write the 32-bit quantity <v> to <addr>
 * 
 * Check-off:
 *  1. get a single LED to blink.
 *  2. attach an LED to pin 19 and another to pin 20 and blink in opposite order (i.e.,
 *     one should be on, while the other is off).   Note, if they behave weirdly, look
 *     carefully at the wording for GPIO set.
 */
#include "gpio.c"

// these are in start.s
void put32(volatile void *addr, unsigned v);
unsigned get32(const volatile void *addr);
void dumpy(int addr);

// when you run should blink 10 times. will have to restart the pi by pulling the usb connection out.
void notmain(void) {
    int led = 20;
    gpio_set_output(led);
    volatile unsigned *gpio_fsel2 = (volatile unsigned *)(GPIO_BASE + 0x08);
    *gpio_fsel2 &= ~(0b111);
    *gpio_fsel2 |= 0b001;
    volatile unsigned *gpio_set0  = (volatile unsigned *)(GPIO_BASE + 0x1C);
    volatile unsigned *gpio_clr0  = (volatile unsigned *)(GPIO_BASE + 0x28);
    for(int i = 0; i < 10; i++) {
        gpio_set_on(led);
        *gpio_set0 = 1 << led;
        delay(1000000);
        gpio_set_off(led);
        *gpio_clr0 = 1 << led;
        delay(1000000);
    }
}

