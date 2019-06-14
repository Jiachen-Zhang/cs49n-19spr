// simple test for your put/get implementation.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../blink/gpio.h"
#include "rpi.h"

int main(void) {
    srandom(49);

    // make sure that everyone has the same random.
    assert(89789692 == random());

#   define N 128

    // should the same as in test.out
    volatile void *addrs[N];
    unsigned vals[N];
    for(int i = 0; i < N; i++) {
        addrs[i] = (volatile void*)(random());// %256
        if(random()%2 == 0)
            vals[i] = get32(addrs[i]);
        else
            put32(addrs[i], vals[i] = random());
    }
//    printf("\n\n\n >>>>>>>>>>>>>>>>>>test<<<<<<<<<<<<<<<<<\n\n\n");
    for(int i = 0; i < N; i++) {
  //      printf("get32(addrs) = %ld, vals[i] = %ld, ", (size_t)get32(addrs[i]), (size_t)vals[i]);
       assert(get32(addrs[i]) == vals[i]);
    //   printf("pass i = %d\n", i);
   }

    return 0;
}
