#include "gpio.h"
volatile unsigned *gpio_fsel0 = (volatile unsigned *)(GPIO_BASE);           // GPIO Function Select 0
volatile unsigned *gpio_fsel1 = (volatile unsigned *)(GPIO_BASE + 0x04);    // GPIO Function Select 1
volatile unsigned *gpio_fsel2 = (volatile unsigned *)(GPIO_BASE + 0x08);    // GPIO Function Select 2
volatile unsigned *gpio_fsel3 = (volatile unsigned *)(GPIO_BASE + 0x0C);    // GPIO Function Select 3
volatile unsigned *gpio_fsel4 = (volatile unsigned *)(GPIO_BASE + 0x10);    // GPIO Function Select 4
volatile unsigned *gpio_set0  = (volatile unsigned *)(GPIO_BASE + 0x1C);    // GPIO Pin Output Set 0 
volatile unsigned *gpio_clr0  = (volatile unsigned *)(GPIO_BASE + 0x28);    // GPIO Pin Output Clear 0 
volatile unsigned *gpio_lev0  = (volatile unsigned *)(GPIO_BASE + 0x34);    // GPIO Pin Level 0
volatile unsigned *gpio_eds0  = (volatile unsigned *)(GPIO_BASE + 0x40);    // GPIO Pin Event Detect Status 0 
volatile unsigned *gpio_ren0  = (volatile unsigned *)(GPIO_BASE + 0x4C);    // GPIO Pin Rising Edge Detect Enable 0
volatile unsigned *gpio_fen0  = (volatile unsigned *)(GPIO_BASE + 0x58);    // GPIO Pin Falling Edge Detect Enable 0 
volatile unsigned *gpio_hen0  = (volatile unsigned *)(GPIO_BASE + 0x64);    // GPIO Pin High Detect Enable 0 
volatile unsigned *gpio_len0  = (volatile unsigned *)(GPIO_BASE + 0x70);    // GPIO Pin Low Detect Enable 0 
volatile unsigned *gpio_aren0 = (volatile unsigned *)(GPIO_BASE + 0x7C);    // GPIO Pin Async. Rising Edge Detect 0 
volatile unsigned *gpio_afen0 = (volatile unsigned *)(GPIO_BASE + 0x88);    // GPIO Pin Async. Falling Edge Detect 0
volatile unsigned *gpio_pud   = (volatile unsigned *)(GPIO_BASE + 0x94);    // GPIO Pin Pull-up/down Enable 
volatile unsigned *gpio_pudclk0 = (volatile unsigned *)(GPIO_BASE + 0x98);  // GPIO Pin Pull-up/down Enable Clock 0  

void delay(unsigned ticks) { 
    while (ticks-- > 0)
        asm("nop");
}

void gpio_set_output(unsigned pin) {
    // use gpio_fsel0 
    int FSEL_offset = pin / 10;
    volatile unsigned *gpio_fsel;
    switch (FSEL_offset) {
        case 0:
            gpio_fsel = gpio_fsel0;
            break;
        case 1:
            gpio_fsel = gpio_fsel1;
            break;
        case 2:
            gpio_fsel = gpio_fsel2;
            break;
        default:
            gpio_fsel = gpio_fsel0;
    }
    *gpio_fsel &= ~(0b111 << ((pin % 10) * 3));
    *gpio_fsel |= 0b000 << ((pin % 10) * 3); 
}

void gpio_set_on (unsigned pin) { *gpio_set0 = 1 << pin; }

void gpio_set_off(unsigned pin) { *gpio_clr0 = 1 << pin; }


