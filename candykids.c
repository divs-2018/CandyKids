#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include "bbuff.h"
#include "stats.h"

_Bool stop_thread = false;
typedef struct { 
    int factory_number; 
    double creation_ts_ms; 
} candy_t;

double current_time_in_ms(void); 
void* candyFactory(void* param);
void* consumerKid(void* param);

int main(int argc, char* argv[])
{
    //Variables
    int i = 0;
    int factories = 0;
    int kids = 0;
    int seconds = 0;
    //1. Extract Arguments
    if(argc != 4){
        printf("ERROR: Number of arguments is invalid.\n");
        exit(-1);
    }
    if(argv[1] <= 0 ||argv[2] <= 0  || argv[3] <= 0 ){
        printf("Argument is not valid");
        exit(-1);
    }
    factories = atoi(argv[1]);
    kids = atoi(argv[2]);
    seconds = atoi(argv[3]);

    // 2. Initialize modules 
    bbuff_init();
    stats_init(factories);

    // 3. Launch candy-factory threads
    pthread_t factory_threads[factories];
    int factory_id[factories];
    for(i=0; i<factories; i++){
        factory_id[i] = i;
        pthread_create(&factory_threads[i], NULL , candyFactory ,&factory_id[i]);

    }

    // 4. Launch kid threads 
    pthread_t kid_threads[kids];
    for(i=0; i<kids; i++){
        pthread_create(&kid_threads[i], NULL, consumerKid ,NULL);
    }

    // 5. Wait for requested time 
    for(i=0;i<seconds;i++){
	    sleep(1);
        printf("Time    %ds\n",i+1);
    }

    // 6. Stop candy-factory threads 
    printf("Stopping candy factory threads...\n");
    stop_thread = true;
    for(i=0;i<factories;i++){
	    pthread_join(factory_threads[i], NULL);
    }
    
    // 7. Wait until no more candy
    while(!bbuff_is_empty()){
        sleep(1);
    }

    // 8. Stop kid threads 
    printf("Stopping consumer kid threads...\n");
    for(i=0;i<kids;i++){
        pthread_cancel(kid_threads[i]);
	    pthread_join(kid_threads[i], NULL);
    }


    // 9. Print statistics 
    stats_display();

    // 10. Cleanup any allocated memory
    stats_cleanup();
    //free();
    //free();

    return 0;
}


double current_time_in_ms(void){ 
    struct timespec now; 
    clock_gettime(CLOCK_REALTIME, &now); 
    return now.tv_sec * 1000.0 + now.tv_nsec/1000000.0; 
}

void* candyFactory(void* param){
    int factory_num = *((int *)param);
    while(!stop_thread){
        //Pick a number of seconds which it will (later) wait. The number is randomly selected between 0 and 3 (inclusive).
        int wait = (rand()%4);
        //Print a message such as: "Factory 0 ships candy & waits 2s" 
        printf("Factory %d ships candy & waits %ds\n", factory_num , wait);
        //Dynamically allocate a new candy item and populate its fields. 
        candy_t* candy = malloc(sizeof(candy_t));
        candy->creation_ts_ms = current_time_in_ms();
        candy->factory_number = factory_num;
        //Add the candy item to the bounded buffer. 
        bbuff_blocking_insert(candy);
        stats_record_produced(factory_num);
        //Sleep for number of seconds identified in the first step.
        free(candy);
        sleep(wait);
    }
    printf("Candy factory %d done\n", factory_num);

    pthread_exit(NULL);
}

void* consumerKid(void* param){
    while(1){
        //Extract a candy item from the bounded buffer; this will block until there is a candy item to extract. Process the item. 
        candy_t* candy = malloc(sizeof(candy_t));
        candy = bbuff_blocking_extract();
        //Initially you may just want to printf() it to the screen; in the next section, you must add a statistics module that will track what candies have been eaten.
	printf("Factory # in consumer kid:%d\n", candy->factory_number);
        stats_record_consumed(candy->factory_number, current_time_in_ms() - candy->creation_ts_ms);
        //Sleep for either 0 or 1 seconds (randomly selected).
        sleep(rand()%2);
    }
    pthread_exit(NULL);
}

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include "bbuff.h"
#include "stats.h"

_Bool stop_thread = false;
typedef struct { 
    int factory_number; 
    double creation_ts_ms; 
} candy_t;

double current_time_in_ms(void); 
void* candyFactory(void* param);
void* consumerKid(void* param);

int main(int argc, char* argv[])
{
    //Variables
    int i = 0;
    int factories = 0;
    int kids = 0;
    int seconds = 0;
    //1. Extract Arguments
    if(argc != 4){
        printf("ERROR: Number of arguments is invalid.\n");
        exit(-1);
    }
    if(argv[1] <= 0 ||argv[2] <= 0  || argv[3] <= 0 ){
        printf("Argument is not valid");
        exit(-1);
    }
    factories = atoi(argv[1]);
    kids = atoi(argv[2]);
    seconds = atoi(argv[3]);

    // 2. Initialize modules 
    bbuff_init();
    stats_init(factories);

    // 3. Launch candy-factory threads
    pthread_t factory_threads[factories];
    int factory_id[factories];
    for(i=0; i<factories; i++){
        factory_id[i] = i;
        pthread_create(&factory_threads[i], NULL , candyFactory ,&factory_id[i]);

    }

    // 4. Launch kid threads 
    pthread_t kid_threads[kids];
    for(i=0; i<kids; i++){
        pthread_create(&kid_threads[i], NULL, consumerKid ,NULL);
    }

    // 5. Wait for requested time 
    for(i=0;i<seconds;i++){
	    sleep(1);
        printf("Time    %ds\n",i+1);
    }

    // 6. Stop candy-factory threads 
    printf("Stopping candy factory threads...\n");
    stop_thread = true;
    for(i=0;i<factories;i++){
	    pthread_join(factory_threads[i], NULL);
    }
    
    // 7. Wait until no more candy
    while(!bbuff_is_empty()){
        sleep(1);
    }

    // 8. Stop kid threads 
    printf("Stopping consumer kid threads...\n");
    for(i=0;i<kids;i++){
        pthread_cancel(kid_threads[i]);
	    pthread_join(kid_threads[i], NULL);
    }


    // 9. Print statistics 
    stats_display();

    // 10. Cleanup any allocated memory
    stats_cleanup();
    //free();
    //free();

    return 0;
}


double current_time_in_ms(void){ 
    struct timespec now; 
    clock_gettime(CLOCK_REALTIME, &now); 
    return now.tv_sec * 1000.0 + now.tv_nsec/1000000.0; 
}

void* candyFactory(void* param){
    int factory_num = *((int *)param);
    while(!stop_thread){
        //Pick a number of seconds which it will (later) wait. The number is randomly selected between 0 and 3 (inclusive).
        int wait = (rand()%4);
        //Print a message such as: "Factory 0 ships candy & waits 2s" 
        printf("Factory %d ships candy & waits %ds\n", factory_num , wait);
        //Dynamically allocate a new candy item and populate its fields. 
        candy_t* candy = malloc(sizeof(candy_t));
        candy->creation_ts_ms = current_time_in_ms();
        candy->factory_number = factory_num;
        //Add the candy item to the bounded buffer. 
        bbuff_blocking_insert(candy);
        stats_record_produced(factory_num);
        //Sleep for number of seconds identified in the first step.
        free(candy);
        sleep(wait);
    }
    printf("Candy factory %d done\n", factory_num);

    pthread_exit(NULL);
}

void* consumerKid(void* param){
    while(1){
        //Extract a candy item from the bounded buffer; this will block until there is a candy item to extract. Process the item. 
        candy_t* candy = malloc(sizeof(candy_t));
        candy = bbuff_blocking_extract();
        //Initially you may just want to printf() it to the screen; in the next section, you must add a statistics module that will track what candies have been eaten.
	printf("Factory # in consumer kid:%d\n", candy->factory_number);
        stats_record_consumed(candy->factory_number, current_time_in_ms() - candy->creation_ts_ms);
        //Sleep for either 0 or 1 seconds (randomly selected).
        sleep(rand()%2);
    }
    pthread_exit(NULL);
}

