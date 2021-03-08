#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "bbuff.h"
#include "stats.h"

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
    //pthread_t factory_threads[factories];
    //int factory_id[factories];
    for(i=0; i<factories; i++){
        //factory_id[i] = i;
        //pthread_create(&factory_threads[i], NULL,  ,&factory_id[i]);

    }

    // 4. Launch kid threads 
    //pthread_t kid_threads[kids];
    for(i=0; i<kids; i++){
        //pthread_create(&kid_threads[i], NULL,  ,NULL);
    }

    // 5. Wait for requested time 
    for(i=0;i<seconds;i++){
	    sleep(1);
    }
    // 6. Stop candy-factory threads 
    // 7. Wait until no more candy 
    // 8. Stop kid threads 
    // 9. Print statistics 
    // 10. Cleanup any allocated memory
    return 0;
}
