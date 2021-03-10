//Crashes at wait and signal in csil (will debug later unless someone knows why)

#include "bbuff.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


void* buffer[BUFFER_SIZE];
int in, out;
int empty; //counting semaphore: number of empty slots
int full; //counting semaphore: number of filled slots
int mutex; //binary semaphore
void* itemConsumed = malloc (sizeof(void*));

void bbuff_init(void){
    in = 0;
    out = 0;
    empty = BUFFER_SIZE;
    mutex = 1;
    full = 0; 
};

void bbuff_blocking_insert(void *item){
    do{
         
        down(empty); //wait until there is at least one empty slot
        down(mutex); //acquire lock

        //Insert item in buffer when not full and increment counter
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;

        up(mutex); //release lock
        up(full); //increment full
      
    }while(true);
}


void* bbuff_blocking_extract(void){
    do{
         
        down(full); //wait until there is one full slot in the buffer
        down(mutex); //acquire lock

        //Remove item when buffer not empty, remove item and decrement counter
        itemConsumed = buffer[out];
        out = (out + 1) % BUFFER_SIZE;

        up(mutex); //release lock
        up(empty); //increment 'empty' becuase consumer has acquired removed the item
        
        //counsume the candy

    }while(true);

    return itemConsumed; 

}

_Bool bbuff_is_empty(void){
    return in == out;
};

_Bool bbuff_is_full(void){
    return ((in - BUFFER_SIZE) == out);
};
