#include "stats.h"
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>

typedef struct  {
    int factory_number;
    int candiesMade;
    int candiesEaten;
    double minDelay;
    double maxDelay;
    double avgDelay;
    double totDelay; //Used to find avgDelay later on
} candyFactory;

candyFactory *factoryArray;
int numFactories;

sem_t mutex;

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

void stats_cleanup(void) {
    free(factoryArray);
}

void stats_record_produced(int factory_number) {
    sem_wait(&mutex);
    factoryArray[factory_number].candiesMade++;
    sem_post(&mutex);
}
void stats_record_consumed(int factory_number, double delay_in_ms) {
    sem_wait(&mutex);
	
    factoryArray[factory_number].candiesEaten += 1;

    //First candy: Set all delays to current delay, as there is no other value to compare with
    if (factoryArray[factory_number].candiesEaten == 1) {
	factoryArray[factory_number].minDelay = delay_in_ms;
	factoryArray[factory_number].maxDelay = delay_in_ms;
	factoryArray[factory_number].avgDelay = delay_in_ms;
	factoryArray[factory_number].totDelay = delay_in_ms;
    } else {
	if (delay_in_ms < factoryArray[factory_number].minDelay) {
            factoryArray[factory_number].minDelay = delay_in_ms;
    	}
	if (delay_in_ms > factoryArray[factory_number].maxDelay) {
	    factoryArray[factory_number].maxDelay = delay_in_ms;
	}
        //Use total delay to calculate average delay.
	factoryArray[factory_number].totDelay += delay_in_ms;
        factoryArray[factory_number].avgDelay = (factoryArray[factory_number].totDelay)/(double)factoryArray[factory_number].candiesEaten;
    }
    
    sem_post(&mutex);
    
}
void stats_display(void) {
    int i=0;
	
    printf("\nStatistics: \n");
    printf("%8s%10s%10s%15s%15s%15s\n","Factory#","#Made","#Eaten","Min Delay[ms]","Avg Delay[ms]","Max Delay[ms]");

    for (i=0; i<numFactories; i++) {
	printf("%8d%10d%10d%15.5f%15.5f%15.5f\n",
	       i, factoryArray[i].candiesMade, factoryArray[i].candiesEaten,
	       factoryArray[i].minDelay, factoryArray[i].avgDelay, factoryArray[i].maxDelay);
	if (factoryArray[i].candiesMade != factoryArray[i].candiesEaten) {
	    printf("ERROR: Mismatch between number made and eaten.\n");
	}
    }
}
