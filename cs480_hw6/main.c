/***************
 HW6 Race Conditions: The purpose of this program is to demonstrate race conditions with a common resources, in this case the text file db.txt. The program creates 10 threads which adds and removes a record 10 time to the db.txt file. This is program using the pthread_mutex as a locking strategy.
 
 @Authors: Clarissa Calderon and Chris Whitney
 ****************/

/***************
 Preprocessor directives
 ****************/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <pthread.h>

#define DATABASE_FILE "db.txt"

/***************
 Type definition
 ****************/
typedef struct {
    char name[80];
    char number[10];
} PhoneBookRecord;

typedef struct{
    pthread_mutex_t* mutex;
    PhoneBookRecord* record;
} MutexRecord;

/***************
 Prototype declarations
 ****************/
void printDB();
void set_to_record (int fd,int record_number,pthread_mutex_t* mutex);
void add_record(PhoneBookRecord* a_record,pthread_mutex_t* mutex);
int remove_record(char* number,pthread_mutex_t* mutex);
void* add_remove_tentimes(void* data);


int main(int argn,char* argv[]) {
    //declares an array of the 10 pthreads
    pthread_t pthreads[10];
    
    //creates a record struct
    PhoneBookRecord record;
    //places number and name into record
    strncpy(record.number, "5206783055",10);
    strncpy(record.name, "Suh Dude",80);
    
    MutexRecord data;
    //inits the mutex lock
    pthread_mutex_t* mutex;
    pthread_mutex_init(mutex,NULL);
    *data.mutex = *mutex;
    *data.record = record;
    
    //loops 10 times and creates all 10 threads
    for(int i=0; i<10; i++){
        if(pthread_create(&pthreads[i],NULL,add_remove_tentimes,(void *) &data)){
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }
    }
    
    //loops through each thread and waits to terminate
    for(int i=0; i<10; i++){
        if(pthread_join(pthreads[i], NULL)) {
            fprintf(stderr, "Error joining thread\n");
            return 2;
        }
    }
    printDB();
    return 0;
    
}
void* add_remove_tentimes(void* data){
    //takes in the void pointer and assigns it to a record
    MutexRecord* thread_data  = data;
    PhoneBookRecord* record = thread_data->record;
    pthread_mutex_t* mutex = thread_data->mutex;
    
    //add and remove 1
    add_record(record,mutex);
    remove_record(record->number,mutex);
    //add and remove 2
    add_record(record,mutex);
    remove_record(record->number,mutex);
    //add and remove 3
    add_record(record,mutex);
    remove_record(record->number,mutex);
    //add and remove 4
    add_record(record,mutex);
    remove_record(record->number,mutex);
    //add and remove 5
    add_record(record,mutex);
    remove_record(record->number,mutex);
    //add and remove 6
    add_record(record,mutex);
    remove_record(record->number,mutex);
    //add and remove 7
    add_record(record,mutex);
    remove_record(record->number,mutex);
    //add and remove 8
    add_record(record,mutex);
    remove_record(record->number,mutex);
    //add and remove 9
    add_record(record,mutex);
    remove_record(record->number,mutex);
    //add and remove 10
    add_record(record,mutex);
    remove_record(record->number,mutex);
    //add final
    add_record(record,mutex);
    return NULL;
}
/****************
 printDB: This function simply prints out the content of the database to the standard output. This accomplished by looping record at a time through the file until the number of read reacord is equal to the number of record that exist.
 ****************/
void printDB() {
    
    int total;
    int record_count;
    int read_records = 0;
    PhoneBookRecord temp_record;
    
    int fd = open(DATABASE_FILE,O_RDWR,S_IREAD);
    if (fd<0) {
        printf("File not found \n");
    }
    //reads in record_count from the front of the db
    lseek(fd, 0, SEEK_CUR);
    read(fd, &record_count, sizeof(int));
    
    //calculates the total size of all the records
    total = record_count * sizeof(PhoneBookRecord);
    
    printf("======DATABASE======\n");
    //lops through each record and prints them out
    while (read_records<record_count) {
        read(fd, &temp_record, sizeof(temp_record));
        printf("Name: %s Number: %s \n",temp_record.name,temp_record.number);
        read_records++;
    }
}
/***************
 add_record: This function takes in a pointer to a PhoneBookRecord struct and adds it to the front of the database. It accomplishs this by first checking if the database is empty if so it just writes the record, if not it takes the first record writes it to the end and writes the given struct to the database.
 ****************/
void add_record(PhoneBookRecord* a_record,pthread_mutex_t* mutex) {
    
    int record_count;
    PhoneBookRecord temp_record;
    char number[11];
    //locks the file
    pthread_mutex_lock(mutex);
    //opens db file
    int fd = open(DATABASE_FILE,O_CREAT|O_RDWR,S_IREAD|S_IWRITE);
    if (fd<0) {
        printf("File not found \n");
        pthread_mutex_unlock(mutex);
    }
    //moves file pointer to front
    lseek(fd, 0, SEEK_SET);
    //reads the number of records in the db
    read(fd, &record_count, sizeof(int));
    
    if(record_count !=0){
        //if there is more than zero entries
        //reads the first record and writes it to the end
        read(fd, &temp_record, sizeof(PhoneBookRecord));
        //unlocks for the set functions
        pthread_mutex_unlock(mutex);
        set_to_record(fd, record_count +1,mutex);
        //locks file
        pthread_mutex_lock(mutex);
        write(fd, &temp_record, sizeof(PhoneBookRecord));
    }

    //unlock file for set
    pthread_mutex_unlock(mutex);
    set_to_record(fd, 1,mutex);
    //locks after set function
    pthread_mutex_lock(mutex);
    //writes a_record to the front of the db
    write(fd, a_record, sizeof(PhoneBookRecord));
    
    //updates record_count
    record_count++;
    //writes record count to the begining of file
    lseek(fd, 0, SEEK_SET);
    write(fd,&record_count, sizeof(int));
    
    //loops through number and null terminates it
    for(int i=0; i<10; i++) number[i] = a_record->number[i];
    number[10] = '\0';
    //mutex unlock
    pthread_mutex_unlock(mutex);
    //print out success message
    printf("Added %s, phone number: %s, number of entires: %i \n", a_record -> name, number, record_count);
    
}

/***************
 remove_record: This functions takes in a string parameter of a number and removes that record from the database. If it is successful it returns 0 otherwise it returns -1. It accomplishs this by looping through the file until it find a record that matchs the parameter, and writes over that record with the last record in the database.
 ****************/
int remove_record(char* number,pthread_mutex_t* mutex) {
    //locks file
    pthread_mutex_lock(mutex);
    //open db file
    int fd = open(DATABASE_FILE,O_CREAT|O_RDWR,S_IREAD|S_IWRITE);
    if (fd<0) {
        printf("File not found \n");
        //unlocks file
        pthread_mutex_unlock(mutex);
        return -1;
    }
    
    int record_count = 1;
    int total = 0;
    PhoneBookRecord phone_record, last_record;
    
    lseek(fd, 0, SEEK_SET);
    read(fd, &total, sizeof(int));
    
    if(!total){
        printf("[remove] - record with number %s is not in the database\n", number);
        pthread_mutex_unlock(mutex);
        return -1;
    }
    //unlcoks for set function
    pthread_mutex_unlock(mutex);
    set_to_record(fd, total,mutex);
    //relocks file
    pthread_mutex_lock(mutex);
    
    //reads in last record
    read(fd, &last_record, sizeof(PhoneBookRecord));
    
    //unlcoks for set function
    pthread_mutex_unlock(mutex);
    set_to_record(fd, 1,mutex);
    //relocks file
    pthread_mutex_lock(mutex);
    
    while(record_count <= total){
        read(fd, &phone_record, sizeof(PhoneBookRecord));
        
        if(!strncmp(phone_record.number, number,10)){
            
            pthread_mutex_unlock(mutex);
            set_to_record (fd, record_count,mutex);
            pthread_mutex_lock(mutex);
            
            write(fd, &last_record, sizeof(PhoneBookRecord));
            lseek(fd, 0, SEEK_SET);
            total--;
            write(fd, &total, sizeof(int));
            truncate(DATABASE_FILE, (sizeof(int)+(total*sizeof(PhoneBookRecord))));
            printf("[remove] - person %s removed, %d record(s)remaining\n",phone_record.name,total);
            pthread_mutex_unlock(mutex);
            return 0;
        }
        record_count++;
    }
    printf("[remove] - record with number %s is not in the database \n", number);
    pthread_mutex_unlock(mutex);
    return -1;
}

/****************
 set_to_record: This function has two paramters a file handle and a record number, it then sets the give file handles file pointer the the index of the record number. This is accomplished using some math and the lseek system call to move the pointer the correct record.
 ****************/
void set_to_record (int fd,int record_number,pthread_mutex_t* mutex){
    pthread_mutex_lock(mutex);
    lseek(fd,(record_number-1)*sizeof(PhoneBookRecord)+sizeof(int), SEEK_SET);
    pthread_mutex_unlock(mutex);
}







