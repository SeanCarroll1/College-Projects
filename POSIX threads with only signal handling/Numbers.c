#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

//Struct for numbers and file
typedef struct
{
    int num0, num1;
    FILE *textfile;
    pthread_t tid;
    
} data_t;

//method connectiong to cleanup to let you know when the threads exit
static void ReaderExit()
{    
    printf("Goodbye from Reader Thread\n");
}

static void CalculatorExit()
{
    printf("Goodbye from Calculator Thread\n");
}

//Reader thread
static void *reader(void *numbers_0)
{
    //Use a Cleanup handler
    pthread_cleanup_push(ReaderExit, NULL);

	int sig;
	sigset_t set;
	sigaddset(&set, SIGUSR1);
	sigprocmask(SIG_BLOCK, &set, NULL);
	

    data_t * numbers;
    numbers = (data_t *)numbers_0;
    unsigned int Random;
	
	

     //Reading loop
    while(1)
    {
	//wait for signal
        sigwait(&set, &sig);

 	usleep(rand_r(&Random) % 10000); 
            printf("Thread 1 submitting : %d %d\n", numbers->num0, numbers->num1);
	           

	pthread_kill(numbers->tid, SIGUSR1);
        
    }
	//clean up thread calls method
    pthread_cleanup_pop(1);
}


//Calculator thread
static void *calculator(void *numbers_0)
{
    // cleanup handler
    pthread_cleanup_push(CalculatorExit, NULL);
    unsigned int Random;
  
    data_t * numbers;
    numbers = (data_t *)numbers_0;

        int sig;
	sigset_t set;
	sigaddset(&set, SIGUSR2);
	sigprocmask(SIG_BLOCK, &set, NULL);


    //loop for adding the numbers
    while(1)
    {
              
       
       usleep(rand_r(&Random) % 100000);
       
       printf("Thread 2 calculated : %d\n", numbers->num0 + numbers->num1);
       sigwait(&set, &sig);
       pthread_kill(numbers->tid, SIGUSR2);
	
    }
	//clean up thread calls method
    pthread_cleanup_pop(1);
}


//Main
int main (int argc, char *argv[])
{
    //Declare threads, file pointer
    pthread_t readerThread,calcthread;
    FILE *textfile;
    data_t data;

    data_t * numbers;
    numbers = (data_t *)&data;

	//Exit if no file is given
    if(argc < 2)
    {
        printf("Please enter a text file as an argument\n");
        return 1;
    }

    //Open file
    textfile = fopen(argv[1], "r");

    //Assign values
    data.tid = pthread_self();
    data.textfile = textfile;


    //Create threads 
    pthread_create(&readerThread, NULL, reader, (void *)&data);
    pthread_create(&calcthread, NULL, calculator, (void *)&data);

 

	int sig;
	sigset_t set;
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);
	sigprocmask(SIG_BLOCK, &set, NULL);
  
//loops until the text file is finished, while loop signals the threads
  while(  fscanf(numbers->textfile, "%d %d", &numbers->num0, &numbers->num1) != EOF)
    {
    	
        pthread_kill(readerThread, SIGUSR1);
       
	sigwait(&set, &sig);

        pthread_kill(calcthread, SIGUSR2);
      
	sigwait(&set, &sig);
      
    }
//cancel threads
    pthread_cancel(readerThread);
    pthread_cancel(calcthread);
    pthread_join(readerThread, NULL);
    pthread_join(calcthread, NULL);

    fclose(textfile);
    return 0;
}
