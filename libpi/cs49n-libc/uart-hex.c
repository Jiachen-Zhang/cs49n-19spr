#include "rpi.h"

// trivial function needed by some drivers.
int uart_hex(unsigned h) {
    printk("%x");
    return 0;
}

