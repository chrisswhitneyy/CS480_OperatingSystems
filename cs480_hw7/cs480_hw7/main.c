/*
 CS480 HW7: Thread Synchronization
 May 3, 2016
 
 Purpose: The purpose of this program is to solve the coke machince 
 problem with threads and conditional variables. This is accomplished 
 with the pthread libarby, mutex locks, and conditional signaling. If 
 the machince is empty then the consumer falls asleep and waits to be 
 woken up by the refiller. If the machince is full then the refiller 
 falls asleep and waits to be signaled by the consumer. This signal is
 done through the conditional variables not_full and not_empty. 
 
 Authors: Clarissa Calderon and Christopher Whitney
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>

/* preprocessor directives*/
#define INITIAL_NUMBER_COKES 5
#define MAX_NUMBER_COKES 10

#define MAX_RUNS 10

#define NUMBER_PRODUCERS 2
#define NUMBER_CONSUMERS 20


/*for convenience, global variables - to be read by threads*/
pthread_mutex_t lock;
pthread_cond_t not_empty, not_full;
int cokes = INITIAL_NUMBER_COKES;


/* function prototype declarations*/
void* thread_function(void* function);
void refill_coke(void);
void consume_coke(void);


int main(int argc, const char * argv[]) {
    
    //init i and the array to store the pthreads
    int i;
    pthread_t producers[NUMBER_PRODUCERS];
    pthread_t consumers[NUMBER_CONSUMERS];
    
    // init mutex
    pthread_mutex_init(&lock,NULL);
    
    //init condition variables
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);
    
    //declare and assign a function ptr
    //used as an arg for the thread_function
    void (*function_ptr)(void);
    function_ptr= *consume_coke;
    
    // create consumer threads
    for (i=0; i<NUMBER_CONSUMERS; i++) {
        if (pthread_create(&consumers[i],NULL,thread_function,function_ptr)) {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
    }
    
    // create producer threads
    function_ptr= *refill_coke;
    for (i=0; i<NUMBER_PRODUCERS; i++) {
        if (pthread_create(&producers[i],NULL,thread_function,function_ptr)) {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
    }
    
    // just sleep and then bail out
    sleep(10);
    
    return EXIT_SUCCESS;
}


/*  generic thread function: Takes in a pointer to a function and
    executes that function the max number of run times.*/
void* thread_function(void* function) {
    int i;
    
    for (i=0; i<MAX_RUNS; i++) {
        // call function, either of refill_coke() or consume_coke()
        ((void (*)(void))function)();
    }
    
    pthread_exit(NULL);
}


/* refilling and consuming coke functions*/

/*  Refill_coke: Takes in no values (void) and simply fills the global cokes to
    max. If the machine is full (cokes == MAX_COKES) then the function goes to 
    sleep and waits to be woken by the consumer. This function is then
    sandwitched been mutex locks */
void refill_coke(void) {
    //lock the mutex
    pthread_mutex_lock(&lock);
    
    //if full fall asleep and wait for signal from consumer
    while(cokes==MAX_NUMBER_COKES){
        //print out a sleeping message before waiting
        //otherwise print statement will execute after woken
        printf("Machine full refiller sleeps...\n");
        //waits to be signaled by the consumer
        pthread_cond_wait(&not_full, &lock);
    }
    //refills the cokes to max
    int num_to_full = MAX_NUMBER_COKES - cokes;
    cokes = cokes+num_to_full;
    
    printf("->> Refilled\n");
    
    //signal consumer that machince not empty
    pthread_cond_signal(&not_empty);
    
    //unlock mutex
    pthread_mutex_unlock(&lock);
    
}

/*  Consume_coke: Takes in no values (void) and simply subtracts one from
    the global cokes. If the machine is empty (cokes == 0) then the function
    goes to sleep and waits to be woken by the refiller. This function is then
    sandwitched been mutex locks */
void consume_coke(void) {

    //lock mutex
    pthread_mutex_lock(&lock);

    //if full fall asleep and wait for signal from refiller
    while(cokes==0){
        //print out a sleeping message before waiting
        //otherwise print statement will execute after woken
        printf("Machine empty consumer sleeps...\n");
        //waits to be signaled by the refiller
        pthread_cond_wait(&not_empty, &lock);
    }
    
    //consume cokes
    cokes --;
    printf("<<- Take one from %i\n",cokes);
    
    //signal producer that machince is not full
    pthread_cond_signal(&not_full);
    
    //unlock mutex
    pthread_mutex_unlock(&lock);
}










