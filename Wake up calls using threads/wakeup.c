//Justinas Ulevicius, 12367951
//Sean Carroll, 12517683
//Artur Vorobjov, 12407548

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#define ETIMEDOUT 110

//WakeUpCall object 
typedef struct {
	time_t time;
	int roomNumber;
} WakeUpCall;

//heap 
typedef struct {
	int size;
	WakeUpCall *alarm;

} AlarmHeap;

pthread_mutex_t lock;
pthread_cond_t cond;

AlarmHeap* heap=NULL;

static void AlarmGeneratorExit()
{    	
	 printf("\ncleaning memory up");
	//free memory up
  	 free(heap->alarm);
  
  	 printf("\nGoodbye from AlarmGenerator Thread\n");
	
}

static void GuestNotifyExit()
{
    printf("Goodbye from GuestNotify Thread\n");
}

//sort the heap after inserting new wake up call at the back of the heap
void SortInsert(int index) {
	int parentIndex;
	WakeUpCall tmp;
	//while not at the top of the heap 
	while(index>0)
		{
			parentIndex = (index - 1) / 2;
			//check if parent node is larger then child if so swap
			if(heap->alarm[parentIndex].time > heap->alarm[index].time) {
			tmp = heap->alarm[parentIndex];
			heap->alarm[parentIndex] = heap->alarm[index];
			heap->alarm[index] = tmp;
			
		}	
		//increment through the heap
		index=parentIndex;
	}
}

//sorting the heap after wake up call removal
void SortRemove(int index) {
	int lChild, rChild, minIndex;
	WakeUpCall tmp;
	while(1)
	{
		
		lChild=index*2;
		rChild=index*2+1;
		//check if at the end of the heap
		if(lChild>heap->size)
		{
			break;
		}	
		minIndex=lChild;
		//check if right child is within the heap and time is nearer then the left child 
		if(rChild<=heap->size && heap->alarm[rChild].time < heap->alarm[lChild].time)
		{
		
			minIndex = rChild;
		
		}
		//check if parent(index) is farther away then the nearest child (time wise)
		if(heap->alarm[index].time > heap->alarm[minIndex].time)
		 {
			tmp = heap->alarm[minIndex];
			heap->alarm[minIndex] = heap->alarm[index];
			heap->alarm[index] = tmp;
			index=minIndex;
		}
		//if not heap is sorted and break loop
		else
		{
			break;
		}
		
	
	}
	

}


//thread that checks when time has expired
static void * GuestNotify(void *heaper){
	
	pthread_cleanup_push(GuestNotifyExit, NULL);
	AlarmHeap *tempHeap = (AlarmHeap *)heaper;
	int expired,error = 0;
	struct timespec check;
	
	time_t currentTime;
	
	while(1){
		
		//check if heap has at least one value in it
		if(tempHeap->size>0)
		{		
		pthread_mutex_lock(&lock);
				//get time at front of the heap
				WakeUpCall test=tempHeap->alarm[0];
			
			 	check.tv_sec = test.time;
        			check.tv_nsec = 0;
			

				error = pthread_cond_timedwait(&cond, &lock, &check);

			//checking if time has expired
			if(error == ETIMEDOUT)
			{	
					//remove wake up call from heap
					tempHeap->alarm[0] = tempHeap->alarm[tempHeap->size-1];
					tempHeap->size--;
					tempHeap->alarm= realloc(tempHeap->alarm, (tempHeap->size*sizeof(WakeUpCall)));
					//set global heap to new heap after removal
					heap=tempHeap;
					//call method to sort heap after removal
					SortRemove(0);
					//print number of alarms that have expired
					expired += 1;
					//print wake up alarm
					printf("\nWake up! %i %s \n", test.roomNumber, ctime(&test.time));
					
					printf("Expired Alarms:    %i\n", expired);
					printf("Pending Alarms:    %i\n", tempHeap->size);
				
			}
			
			
			pthread_mutex_unlock(&lock);
		}
		
	}
	pthread_cleanup_pop(1);
}

//thread that generates wake up calls
static void * AlarmGenerator(void *heaper){

	pthread_cleanup_push(AlarmGeneratorExit, NULL);
	srand(time(NULL));
	AlarmHeap *tempHeap = (AlarmHeap *)heaper;
	
	while(1){
				
		//sleep for a random period of 1 to 5 secs
		sleep(rand()%(5-1)+1);
		
		time_t waketime;
		WakeUpCall call;

		//generate random room num, wake up calls up to a 100 seconds in the future
		call.roomNumber = rand()%(1-10000)+1;
		waketime = time(NULL) + rand()%(1-100)+1;
		call.time = waketime;
	
		//lock mutex
		pthread_mutex_lock(&lock);
	
		//add wake up call to the end of the heap	
		tempHeap->size++;
		tempHeap->alarm = realloc(tempHeap->alarm, (tempHeap->size*sizeof(WakeUpCall)));
		tempHeap->alarm[tempHeap->size-1] = call;
		heap=tempHeap;
		//call method to sort heap after inserting
		SortInsert(heap->size-1);
		
		printf("\nRegistering: %i %s \n", call.roomNumber, ctime(&waketime));
		pthread_cond_signal(&cond);	
		
			
		pthread_mutex_unlock(&lock);
	
	}
	pthread_cleanup_pop(1);
}  

int main(int argc, char *argv[]){
    
	//initialising variables
	pthread_t create,check;
	AlarmHeap heap;
	heap.alarm = NULL;
	heap.size = 0;
	heap.alarm = malloc( heap.size*sizeof(heap.alarm) );
	
	pthread_create(&create, NULL, AlarmGenerator, &heap);
	pthread_create(&check, NULL, GuestNotify, &heap);
	
   
	//wait for CTRL-C
	sigset_t listen_sigs;
	sigemptyset(&listen_sigs);
	sigaddset(&listen_sigs, SIGINT);
	int listen_sig_r;
	
	pthread_sigmask(SIG_BLOCK, &listen_sigs, NULL);	
	sigwait(&listen_sigs, &listen_sig_r);

	//cancel the threads
	pthread_cancel(create);
	pthread_cancel(check);

	pthread_join(create,NULL);
	pthread_join(check,NULL);

	pthread_mutex_destroy(&lock);
	pthread_cond_destroy (&cond);


	return 0;

}

