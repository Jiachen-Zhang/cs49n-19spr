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
    // 
	return;
}

// countdown 'ticks' cycles; the asm probably isn't necessary.
void delay(unsigned ticks) {
    while(ticks-- > 0)
        asm("add r1, r1, #0");
}

/*
void reboot(void) {
    const int PM_RSTC = 0x2010001c;
    const int PM_WDOG = 0x20100024;
    const int PM_PASSWORD = 0x5a000000;
    const int PM_RSTC_WRCFG_FULL_RESET = 0x00000020;
    int i;
    for(i = 0; i < 100000; i++)
         dummy(i);
    PUT32(PM_WDOG, PM_PASSWORD | 1);
    PUT32(PM_RSTC, PM_PASSWORD | PM_RSTC_WRCFG_FULL_RESET);
    while(1);
}
*/

// when you run should blink 10 times. will have to restart the pi by pulling the usb connection out.
void notmain(void) {
    int led = 16;
    int led2 = 20;
    gpio_set_output(led);
    gpio_set_output(led2);
    for(int i = 0; i < 10; i++) {
        gpio_set_on(led);
        gpio_set_off(led2);
        delay(1000000);
        gpio_set_on(led2);
        gpio_set_off(led);
        delay(1000000);
    }
//	reboot();
}

