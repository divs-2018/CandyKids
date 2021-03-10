#include "bbuff.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>


void* buffer[BUFFER_SIZE];
int in, out;
sem_t empty; //counting semaphore: number of empty slots
sem_t full; //counting semaphore: number of filled slots
sem_t mutex; //binary semaphore
void* itemConsumed = malloc (sizeof(void*));

void bbuff_init(void){
    in = 0;
    out = 0;
    sem_init(&empty,0,1);
    sem_init(&mutex,0,1);
    sem_init(&full,0,1); 
};

void bbuff_blocking_insert(void *item){
    do{
         
        sem_wait(&empty); //wait until there is at least one empty slot
        sem_wait(&mutex); //acquire lock

        //Insert item in buffer when not full and increment counter
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;

        sem_post(&mutex); //release lock
        sem_post(&full); //increment full
      
    }while(true);
}


void* bbuff_blocking_extract(void){
    do{
         
        sem_wait(&full); //wait until there is one full slot in the buffer
        sem_wait(&mutex); //acquire lock

        //Remove item when buffer not empty, remove item and decrement counter
        itemConsumed = buffer[out];
        out = (out + 1) % BUFFER_SIZE;

        sem_post(&mutex); //release lock
        sem_post(^empty); //increment 'empty' becuase consumer has acquired removed the item
        
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
