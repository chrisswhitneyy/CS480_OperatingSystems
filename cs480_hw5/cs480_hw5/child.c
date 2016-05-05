/***************
 DATABASE MANAGEMENT: This program creates and manages a simple database system using a text file as the method of storage. There are three functions which add a record to the database,remove a record, and print out all the content of the database.
 
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

#define DATABASE_FILE "db.txt"
#define LOCK_FILE "lock.txt"
#define USE_LOCK_FILE

/***************
 Type definition
 ****************/
typedef struct {
    char name[80];
    char number[10];
} PhoneBookRecord;

/***************
 Prototype declarations
 ****************/
void add_record(PhoneBookRecord* a_record);
int remove_record(char* number);
void set_to_record (int fd,int record_number);
void lock();
void unlock();

int main(int argn,char* argv[]) {
    
    if (argn==1) {
        printf("ERROR: Not enough arguments \n");
        return -1;
    }
    
    long lpid = (long) getpid();
    char* pid = malloc(sizeof(pid_t));
    sprintf(pid, "%lu",lpid);
    char* name = argv[1];
    
    //adds and removes the record 10 times
    PhoneBookRecord record;
    strncpy(record.number,pid, 10);
    strncpy(record.name, name, 80);
    
    //add and remove 1
    add_record(&record);
    remove_record(pid);
    //add and remove 2
    add_record(&record);
    remove_record(pid);
    //add and remove 3
    add_record(&record);
    remove_record(pid);
    //add and remove 4
    add_record(&record);
    remove_record(pid);
    //add and remove 5
    add_record(&record);
    remove_record(pid);
    //add and remove 6
    add_record(&record);
    remove_record(pid);
    //add and remove 7
    add_record(&record);
    remove_record(pid);
    //add and remove 8
    add_record(&record);
    remove_record(pid);
    //add and remove 9
    add_record(&record);
    remove_record(pid);
    //add and remove 10
    add_record(&record);
    remove_record(pid);
    //add final
    add_record(&record);
    
    return 0;
}

/***************
 add_record: This function takes in a pointer to a PhoneBookRecord struct and adds it to the front of the database. It accomplishs this by first checking if the database is empty if so it just writes the record, if not it takes the first record writes it to the end and writes the given struct to the database.
 ****************/
void add_record(PhoneBookRecord* a_record) {
    int record_count;
    PhoneBookRecord temp_record;
    char number[11];
    
    lock();
    int fd = open(DATABASE_FILE,O_CREAT|O_RDWR,S_IREAD|S_IWRITE);
    if (fd<0) {
        printf("File not found \n");
    }
    //moves file pointer to front
    lseek(fd, 0, SEEK_SET);
    //reads the number of records in the db
    read(fd, &record_count, sizeof(int));
    
    if(record_count !=0){
        //if there is more than zero entries
        //reads the first record and writes it to the end
        read(fd, &temp_record, sizeof(PhoneBookRecord));
        set_to_record(fd, record_count +1);
        write(fd, &temp_record, sizeof(PhoneBookRecord));
    }
    //writes a_record to the front of the db
    set_to_record(fd, 1);
    write(fd, a_record, sizeof(PhoneBookRecord));
    
    //updates record_count
    record_count++;
    lseek(fd, 0, SEEK_SET);
    write(fd,&record_count, sizeof(int));
    
    //loops through number and null terminates it
    for(int i=0; i<10; i++) number[i] = a_record->number[i];
    number[10] = '\0';
    unlock();
    //print out success message
    printf("Added %s, phone number: %s, number of entires: %i \n", a_record -> name, number, record_count);
}

/***************
 remove_record: This functions takes in a string parameter of a number and removes that record from the database. If it is successful it returns 0 otherwise it returns -1. It accomplishs this by looping through the file until it find a record that matchs the parameter, and writes over that record with the last record in the database.
 ****************/
int remove_record(char* number) {
    lock();
    int fd = open(DATABASE_FILE,O_CREAT|O_RDWR,S_IREAD|S_IWRITE);
    if (fd<0) {
        printf("File not found \n");
    }
    int record_count = 1;
    int total = 0;
    PhoneBookRecord phone_record, last_record;
    lseek(fd, 0, SEEK_SET);
    read(fd, &total, sizeof(int));
    
    if(!total){
        printf("[remove] - record with number %s is not in the database\n", number);
        unlock();
        return -1;
    }
    set_to_record(fd, total);
    read(fd, &last_record, sizeof(PhoneBookRecord));
    
    set_to_record(fd, 1);
    while(record_count <= total){
        read(fd, &phone_record, sizeof(PhoneBookRecord));
        if(!strncmp(phone_record.number, number,10)){
            set_to_record (fd, record_count);
            write(fd, &last_record, sizeof(PhoneBookRecord));
            
            lseek(fd, 0, SEEK_SET);
            total--;
            write(fd, &total, sizeof(int));
            
            truncate(DATABASE_FILE, (sizeof(int)+(total*sizeof(PhoneBookRecord))));
            printf("[remove] - person %s removed, %d record(s)remaining\n",phone_record.name,total);
            unlock();
            return 0;
        }
        record_count++;
    }
    unlock();
    printf("[remove] - record with number %s is not in the database \n", number);
    return -1;
}

/****************
 set_to_record: This function has two paramters a file handle and a record number, it then sets the give file handles file pointer the the index of the record number. This is accomplished using some math and the lseek system call to move the pointer the correct record.
 ****************/
void set_to_record (int fd,int record_number){
    lseek(fd,(record_number-1)*sizeof(PhoneBookRecord)+sizeof(int), SEEK_SET);
}
/****************
 lock: This function checks to see if the LOCK_FILE defined above it accessible, if so then it defines USE_LOCK_FILE which means that the resource is locked, else it trys again.
****************/
void lock(){
    #ifdef USE_LOCK_FILE
    while (open(LOCK_FILE,O_CREAT | O_EXCL) == -1) { }
    #endif
}
/****************
 unlock: This function checks to see if the USE_LOCK_FILE is defined if so then it unlicked (deletes) the lock file.
 ****************/
void unlock() {
#ifdef USE_LOCK_FILE
    unlink(LOCK_FILE);
#endif
    
}








