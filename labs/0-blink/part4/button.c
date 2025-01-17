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

// these are in start.s
void put32(volatile void *addr, unsigned v);
//void PUT32(int addr, unsigned v);
unsigned get32(const volatile void *addr);
//unsigned GET32(int addr);
void dumpy(int addr);

// see broadcomm documents for magic addresses.
#define GPIO_BASE 0x20200000
volatile unsigned *gpio_fsel0 = (volatile unsigned *)(GPIO_BASE);
volatile unsigned *gpio_fsel1 = (volatile unsigned *)(GPIO_BASE + 0x04);
volatile unsigned *gpio_fsel2 = (volatile unsigned *)(GPIO_BASE + 0x08);
volatile unsigned *gpio_set0  = (volatile unsigned *)(GPIO_BASE + 0x1C);
volatile unsigned *gpio_clr0  = (volatile unsigned *)(GPIO_BASE + 0x28); 
volatile unsigned *gpio_lev0 = (volatile unsigned *)(GPIO_BASE + 0x34);
// set <pin> to output.  note: fsel0, fsel1, fsel2 are contiguous in memory,
// so you can use array calculations!
void gpio_set_output(unsigned pin) {
    // use gpio_fsel0
    int FSEL_offset = pin / 10;

    //*gpio_fsel1 = (0b001 << 18);
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
    *gpio_fsel |= 0b001 << ((pin % 10) * 3);  // set the pin outset <pin> on.
    
}

void gpio_set_on(unsigned pin) {
    // use gpio_set0
	*gpio_set0 = 1 << pin;
}

// set <pin> off
void gpio_set_off(unsigned pin) {
    // use gpio_clr0
	*gpio_clr0 = 1 << pin;
}

// For later labs, write these routines as well.

// set <pin> to input.
void gpio_set_input(unsigned pin) {
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
    *gpio_fsel |= 0b000 << ((pin % 10) * 3);  // set the pin outt <pin> to <v> (v \in {0,1})
}

void gpio_write(unsigned pin, unsigned v) {
    *gpio_lev0 |= (v << pin); 
	return;
}

// countdown 'ticks' cycles; the asm probably isn't necessary.
void delay(unsigned ticks) {
    while(ticks-- > 0)
        asm("add r1, r1, #0");
}

// when you run should blink 10 times. will have to restart the pi by pulling the usb connection out.
void notmain(void) {
    int led_in = 20;
    int led_out = 21;
    gpio_set_output(led_out);
    gpio_set_input(led_in);
    volatile unsigned tmp = 1 << led_in;

    // event detected
    volatile unsigned *gpio_eds0 = (volatile unsigned *)(GPIO_BASE + 0x40); // gpio event detect status register0
    *gpio_eds0 |= (1 << led_in);
    // high detect enable 
    volatile unsigned *gpio_hen0 = (volatile unsigned *)(GPIO_BASE + 0x64);
    *gpio_hen0 = 1 << led_in;
    // low detect enable 
    volatile unsigned *gpio_len0 = (volatile unsigned *)(GPIO_BASE + 0x70);
    //gio_set_output(12);
    while(1) {
        *gpio_eds0 = tmp;
        // high level detected
        if (*gpio_eds0 == tmp && *gpio_hen0 == tmp) {
            *gpio_hen0 = 0 << led_in;   // disable high detected
            *gpio_len0 = 1 << led_in;   // enable low detected
            gpio_set_on(led_out);       // turn on led
        } else
        // low level detected
        if (*gpio_eds0 == tmp && *gpio_len0 == tmp) {
            *gpio_len0 = 0 << led_in;   // disable low detected
            *gpio_hen0 = 1 << led_in;   // enable hign detected
            gpio_set_off(led_out);
        }
        *gpio_eds0 = 0;
        delay(1000000);
    }
}

