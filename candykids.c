#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    //Variables
    int i = 0;

    //1. Extract Arguments
    for (i=1; i< argc; i++) {
    	if(argv[i] < 0){
		printf("Argument is not valid");
		exit(-1);
	}
    }    
    return 0;
}
