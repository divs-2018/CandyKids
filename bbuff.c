#include "bbuff.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <stdbool.h>

/*
Declare variables
*/
void* buffer[BUFFER_SIZE];
sem_t empty; //counting semaphore: number of empty slots
sem_t full; //counting semaphore: number of filled slots
sem_t mutex; //binary semaphore
int in, out;

/*
Initialize variables
*/
void bbuff_init(void){
    in = 0;
    out = 0;
    sem_init(&empty,SHARED_THREADS,BUFFER_SIZE);
    sem_init(&mutex,SHARED_THREADS,1);
    sem_init(&full,SHARED_THREADS,0); 
};

/*
Function to insert items from the buffer 
*/
void bbuff_blocking_insert(void *item){
    sem_wait(&empty); //wait until there is at least one empty slot
    sem_wait(&mutex); //acquire lock

    //Insert item in buffer when not full and increment counter
    buffer[in] = item;
    in = (in+1)%BUFFER_SIZE;
    
    sem_post(&mutex); //release lock
    sem_post(&full); //increment full
    
}

/*
Function to extract items from the buffer 
*/
void* bbuff_blocking_extract(void){
    sem_wait(&full); //wait until there is one full slot in the buffer
    sem_wait(&mutex); //acquire lock

    //Remove item when buffer not empty, remove item and decrement counter

    void* item = buffer[out];
    buffer[out] = NULL;
    out = (out+1)%BUFFER_SIZE;
    
    sem_post(&mutex); //release lock
    sem_post(&empty); //increment 'empty' becuase consumer has acquired removed the item

    return item; 
}

_Bool bbuff_is_empty(void){
    return in == out; //Check if the buffer is empty
};
