#include "stats.h"
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>

/**
 - Track # values for each candy factory
 create a struct with all required fields and then build array of such structs
 one elem for each candy-factory
 
 stats-init() fn can init data storage and get it ready to process produced and consumed events (via respective fns)
 stats_cleanup fn  used to free any dyn alloc memory
 called before main terminates
 */

/**
 - # candy each factory creates
 from candy-factory thread
 - # candy consumed from each factory
 - for each factory
 min, max, avg delays for how long it took from moment candy was produced(dyn alloc) until consumed
 by factory thread calling stats code when candy is created and kid thread calling stats code when item is consumed
 **/

typedef struct  {
    int factory_number;
    int candiesMade;
    int candiesEaten;
    float minDelay;
    float maxDelay;
    float avgDelay;
    
} candyFactory;


/*typedef struct {
 int numFactories;
 candyFactory *factory;
 } statsData;*/

candyFactory *factoryArray;
int numFactories;

//statsData stat1;
sem_t mutex;



//init data storage and get ready to process produced/consumed threads
void stats_init(int num_producers) {
    numFactories = num_producers;
    factoryArray = malloc(numFactories*sizeof(candyFactory));
    sem_init(&mutex, SHARED_THREADS, 1);
    int i = 0;
    for (i=0; i<numFactories; i++) {
        factoryArray[i].factory_number=i;
        factoryArray[i].candiesMade = 0;
        factoryArray[i].candiesEaten = 0;
        factoryArray[i].minDelay = 0;
        factoryArray[i].maxDelay = 0;
        factoryArray[i].avgDelay = 0;
    }
    
}

//frees dyn alloc mem
void stats_cleanup(void) {
    free(factoryArray);
}

void stats_record_produced(int factory_number) {
    //Each time this function is called, candy has been created
    sem_wait(&mutex);
    factoryArray[factory_number].candiesMade += 1;
    sem_post(&mutex);
}
void stats_record_consumed(int factory_number, double delay_in_ms) {
    //Each time this function is called, candy has been consumed
    sem_wait(&mutex);

    factoryArray[factory_number].candiesEaten += 1;
    
    if (delay_in_ms < factoryArray[factory_number].minDelay) {
        factoryArray[factory_number].minDelay = delay_in_ms;
    }
    if (delay_in_ms > factoryArray[factory_number].maxDelay) {
        factoryArray[factory_number].maxDelay = delay_in_ms;
    }
    
    // float pastDelay = factoryArray[factory_number].avgDelay;
    
    // //Calculating AVG delay
    // /*factoryArray[factory_number].avgDelay = ((pastDelay)*(factoryArray[factory_number].candiesEaten -1) + delay_in_ms)/factoryArray[factory_number].candiesEaten;*/
    // //or
    // //This one apparently has better precision
    // factoryArray[factory_number].avgDelay = pastDelay + (delay_in_ms - pastDelay)/factoryArray[factory_number].candiesEaten;
    
    sem_post(&mutex);
    
}
void stats_display(void) {
    int i=0;
    printf("\nStatistics: \n");
    printf("Factory#   #Made   #Eaten  Min Delay[ms]   Avg Delay[ms]   Max Delay[ms]\n");
    for (i=0; i<numFactories; i++) {
        printf("%8d%8d%8d%10.5f%10.5f%10.5f\n",
		       i, factoryArray[i].candiesMade, factoryArray[i].candiesEaten,
		       factoryArray[i].minDelay, factoryArray[i].avgDelay, factoryArray[i].maxDelay);
	    if(factoryArray[i].candiesMade != factoryArray[i].candiesEaten) {
	        printf("ERROR: Mismatch between number made and eaten.");
	    }
        
    }
}



//using counting semaphore because multiple instances of candyFactory
