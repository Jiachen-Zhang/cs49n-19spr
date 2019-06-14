/*
 * Simple tracing memory implementation.  Used for cross-checking.
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "rpi.h"

/****************************************************************************
 * DO NOT CHANGE THIS CODE
 * DO NOT CHANGE THIS CODE
 * DO NOT CHANGE THIS CODE
 * DO NOT CHANGE THIS CODE
 * DO NOT CHANGE THIS CODE
 * DO NOT CHANGE THIS CODE
 */
#define panic(msg...) do {                      \
        fprintf(stderr, "%s:%s:%d:PANIC:", __FILE__, __FUNCTION__, __LINE__); \
        fprintf(stderr, ##msg);                     \
        exit(1);                                                        \
} while(0)
#define unimplemented() panic("implement this function!\n")


/* 
 * you will implement a simple fake memory that maps addresses to values.  e.g., 
 * an array, linked list, hash table of (<addr>, <val>) tuples such that after
 * you insert some (<addr>, <val>), you can lookup <val> using <addr> as a key.
 *
 * simplistic assumptions:
 *  - all loads/stores are 32 bits.
 *  - read returns value of last write, or random() if none.
 *  - load and store prints the addr,val
 * 
 * HINT: I woul suggest a fixed size array, that you do a simple linear scan to insert,
 * and lookup.
 *
 *
 * mildly formal-ish rules for fake memory:
 *  1. no duplicate entries.  
 *       if: 
 *           (<addr>, <val0>) \in memory
 *           (<addr>, <val1>) \in memory
 *      then <val0> == <val1>
 *  2. addresses are persistent: if you ever write <addr>,<val> to memory, then 
 *     <addr> stays there forever.
 *  3. values are persistent until the next write: if you ever write <addr>,<val> 
 *     to memory, then until the next write, if (<addr>, <v>) \in memory, then 
 *     <v> == <val>.
 */
typedef struct {
    volatile void *addr;
    unsigned val;
} mem_t;


// don't change print_write/print_read so we can compare to everyone.
static void print_write(mem_t *m) {
    printf("\tWRITE:addr=%p, val=%u\n", m->addr, m->val);
}
static void print_read(mem_t *m) {
    printf("\tREAD:addr=%p, val=%u\n", m->addr, m->val);
}

/****************************************************************************
 * Put your changes here.
 * Put your changes here.
 * Put your changes here.
 * Put your changes here.
 * Put your changes here.
 * Put your changes here.
 *
 * Implement these four functions:
 *  - lookup
 *  - insert
 *  - get32
 *  - put32
 */
#define SIZE  128   // same as size in test_put_get.c
// define an array of struct to implemente the fake memory
static mem_t Memory[SIZE];
static int cnt = 0; // sum of memory used
static int i = 0;
// lookup <addr> in your memory implementation.  Returns the associated <mem_t>
// or 0 if there is none (null).
static mem_t* lookup(volatile void *addr) {
    for (i = 0; i < cnt; i++) {
        if ((unsigned int*)Memory[i].addr == (unsigned int*)addr) {
            return &Memory[i];
        }
    }
    return (mem_t*)0;
}

// insert (<addr>, <val>) into your fake memory.  
// do an assertion check that:
//   - before: <addr> is not in fake memory.
//   - after: <addr> is in fake memory and lookup returns the corect <val>
static mem_t *insert(volatile void *addr, unsigned val) {
//    assert( lookup(addr) == 0);
    mem_t tmp = {addr, val};
    Memory[cnt++] = tmp;
 //   printf("cnt = %d\n", cnt);
  //  assert( lookup(addr)->addr == addr);
    return lookup(addr);
}

// return the value associated with <addr>.  if this <addr> has not been written
// to, insert a random() value and return that.
//
// before you return, call print_read on the mem_t associated with <addr>.
unsigned get32(volatile void *addr) {
    printf("get32(), addr = %p\n", addr);
    mem_t *find = lookup(addr);
    if ((size_t)find == 0) {
      //  printf("Not find, will insert an random number into it\n");
        find =  insert(addr, random());
    } else {
        //printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Have find\n");
    }
    print_read(find);
    return find -> val;
}

// write (<addr>, <val>) into your fake memory.  if the <addr> is already in fake
// memory, overwrite it.
// 
// before you return, call print_write on the mem_t associated with <addr>.
void put32(volatile void *addr, unsigned val) {
    //printf("put32(), addr = %p, val = %d\n", addr, val);
    
    mem_t tmp = {addr, val};
//    printf("*tmp = %p, tmp.addr = %p, tmp.val = %d\n ", &tmp, tmp.addr, tmp.val);
    
    mem_t *find = lookup(addr);
//    printf("find res = %ld\n", (size_t)find);
    if ( (size_t)find == 0 ) {
  //      printf("Not find, will insert into memory\n");
        insert(addr, val);
    } else {
//        printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Have found !!! The value will be modified\n");
        find->val = val;
    }
    print_write(&tmp);
    return;
}
